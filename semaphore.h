#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <pthread.h>


#define HANDLE_ERROR(msg) \
do{			  \
  perror(msg); 		  \
  exit(EXIT_FAILURE);     \
} while(0)


// the semaphore structure
typedef struct sem{
  pthread_mutex_t mutex; // In this design every semaphore is associated with a mutex.
  unsigned int init_value;
  int count; // if less than 1 it indicates the nbr of waiting threads.
  int pending_posts; 
} sem_t;

int sem_init(sem_t*, int, unsigned int);
int sem_post(sem_t*);
int sem_wait(sem_t*);

#endif 
