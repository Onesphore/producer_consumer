#include "semaphore.h"
#include "producer_consumer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 4
#define PRODUCER_NBR 3
#define CONSUMER_NBR 3

int buffer[BUFSIZE];
pthread_mutex_t buf_lock;
unsigned int put_index = 0;
unsigned int remove_index = 0;




// 2 semaphores are used.
/* this semaphore is used as a condition variable, to notify one 
   of the consumers that one of the producers has inserted an elt
   in the buffer */
sem_t producer_insert_cond; 
/* this semaphore is used as a condition variable, to notify one
   of the producers that there are spots available in the buffer.*/
sem_t consumer_remove_cond;


// the leading thread, "main"
int main(int argc, char *argv[]){
  // initially there is no elt in the buffer, hence this semaphore
  // is initialized to 0.
  if (sem_init(&producer_insert_cond, 0, 0) != 0){
    HANDLE_ERROR("sem_init()");
  }

  // initially all slots in the buffer are empty, hence
  // this semaphore is initialized to BUFSIZE.
  if (sem_init(&consumer_remove_cond, 0, BUFSIZE) != 0){
    HANDLE_ERROR("sem_init()");
  }
  
  if (pthread_mutex_init(&buf_lock, NULL) != 0){
    HANDLE_ERROR("pthread_mutex_init()");
  }

  int i;
  // producer threads.
  pthread_t producer_threads[PRODUCER_NBR];
  for (i=0; i< PRODUCER_NBR; ++i){
    if (pthread_create(&producer_threads[i], NULL,
                         producer_routine, NULL) != 0){
      HANDLE_ERROR("pthread_create()");
    }
  }
  // consumer threads.
  pthread_t consumer_threads[CONSUMER_NBR];
  for (i=0; i< CONSUMER_NBR; ++i){
    if (pthread_create(&consumer_threads[i], NULL,
                         consumer_routine, NULL) != 0){
      HANDLE_ERROR("pthread_create()");
    }
  }

  // wait for producer_threads
  for (i=0; i<PRODUCER_NBR; ++i){
    if (pthread_join(producer_threads[i], NULL) != 0){
      HANDLE_ERROR("pthread_join()");
    }
  }
  // wait for consumer_threads
  for (i=0; i<CONSUMER_NBR; ++i){
    if (pthread_join(consumer_threads[i], NULL) != 0){
      HANDLE_ERROR("pthread_join()");
    }
  }
 
  exit(EXIT_SUCCESS);
}

void *producer_routine(void *args){
  int i = 1;
  while(1){
    sleep(rand() % 5);
    // wait for the empty slot.
    sem_wait(&consumer_remove_cond);
    // wait for a lock to enter the critical region: buffer.
    pthread_mutex_lock(&buf_lock);
    put_elt(i);
    i++;
    // release the lock.
    pthread_mutex_unlock(&buf_lock);
    // signal one of the consumers: an elt was added to the buffer
    sem_post(&producer_insert_cond);
  }
  return NULL;
}

void *consumer_routine(void *agrs){
  while(1){
    sleep(rand() % 5);
    // wait for one of the producers to add an elt to the buffer.
    sem_wait(&producer_insert_cond);
    // wait for a lock to enter the critical region: buffer.
    pthread_mutex_lock(&buf_lock);
    remove_elt();
    // release the lock.
    pthread_mutex_unlock(&buf_lock);
    // signal of the producers: there is an(other) empty slot.
    sem_post(&consumer_remove_cond);
  }
  return NULL;
}


void
put_elt(int elt){
  buffer[put_index] = elt;
  put_index++;
  if (put_index >= BUFSIZE){
    put_index = 0;
  }
}

void
remove_elt(void){
  printf("%d\n", buffer[remove_index]);
  fflush(stdout);
  remove_index++;
  if (remove_index >= BUFSIZE){
    remove_index = 0;
  }
}

