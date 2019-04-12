#include <pthread.h>
#include "rwLock.h"

void rwlock_t::rwlock_acquire_readlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->baseLock);
    rw->readers++;
    if(rw->readers == 1)
       pthread_cond_wait(&rw->writeLock, &rw->baseLock);
    pthread_mutex_unlock(&rw->baseLock);
}

void rwlock_t::rwlock_release_readlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->baseLock);
    rw->readers--;
    if (rw->readers == 0)
        pthread_cond_signal(&rw->writeLock);
    pthread_mutex_unlock(&rw->baseLock);
}

void rwlock_t::rwlock_acquire_writelock(rwlock_t *rw) {
    pthread_cond_wait(&rw->writeLock, &rw->baseLock);
}

void rwlock_t::rwlock_release_writelock(rwlock_t *rw) {
    pthread_cond_signal(&rw->writeLock);
}