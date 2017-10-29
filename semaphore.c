#include "semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

int
sem_init(sem_t *sem, int pshared, unsigned int initial_val){
  if (pthread_mutex_init(&(sem->mutex), NULL) != 0){
    HANDLE_ERROR("pthread_mutex_init()");
  }
  sem->init_value = initial_val;
  sem->count = initial_val;
  sem->pending_posts = 0;
  
  return 0;
}


int
sem_wait(sem_t *sem){
  while(sem->count == 0);
  if (pthread_mutex_lock(&(sem->mutex)) != 0){
    HANDLE_ERROR("pthread_mutex_lock()");
  }
  sem->count--;
  if (pthread_mutex_unlock(&(sem->mutex)) != 0){
    HANDLE_ERROR("pthread_mutex_unlock()");
  } 

  


//
//  if (pthread_mutex_lock(&(sem->mutex)) != 0){
//    HANDLE_ERROR("pthread_mutex_lock()");
//  }
//  if (sem->count < sem->init_value){
//    while(sem->pending_posts <= sem->count); // block
//  }
//
//  int mycount = sem->count;
//  sem->count--;
//  if (sem->count < 0){
//    if (pthread_mutex_unlock(&(sem->mutex)) != 0){
//      HANDLE_ERROR("pthread_mutex_unlock()");
//    }
//    while (sem->count < mycount); // block
//    if (pthread_mutex_lock(&(sem->mutex)) != 0){
//      HANDLE_ERROR("pthread_mutex_lock()");
//    }
//  }
//  // if a thread made it here then it will post, hence increase the nbr of pending posts.
//  sem->pending_posts++;
//  if (pthread_mutex_unlock(&(sem->mutex)) != 0){
//    HANDLE_ERROR("pthread_mutex_unlock()");
//  }
  
  return 0;
}

int 
sem_post(sem_t *sem){
  if (pthread_mutex_lock(&(sem->mutex)) != 0){
    HANDLE_ERROR("pthread_mutex_lock()");
  }
  sem->count++;
  sem->pending_posts--;
  if (pthread_mutex_unlock(&(sem->mutex)) != 0){
    HANDLE_ERROR("pthread_mutex_lock()");
  }  
 
  return 0;
}
