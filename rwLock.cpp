#include <pthread.h>
#include "rwLock.h"

void rwlock::rwlock_acquire_readlock() {
    pthread_mutex_lock(&this->baseLock);
    this->readers++;
    if(this->readers == 1)
       pthread_cond_wait(&this->writeLock, &this->baseLock);
    pthread_mutex_unlock(&this->baseLock);
}

void rwlock::rwlock_release_readlock() {
    pthread_mutex_lock(&this->baseLock);
    this->readers--;
    if (this->readers == 0)
        pthread_cond_signal(&this->writeLock);
    pthread_mutex_unlock(&this->baseLock);
}

void rwlock::rwlock_acquire_writelock() {
    pthread_cond_wait(&this->writeLock, &this->baseLock);
}

void rwlock::rwlock_release_writelock() {
    pthread_cond_signal(&this->writeLock);
}