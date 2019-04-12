#include <pthread.h>


typedef struct _rwlock_t {
    pthread_mutex_t baseLock = PTHREAD_COND_INITIALIZER;
    pthread_cond_t writeLock = PTHREAD_MUTEX_INITIALIZER;
    int readers;
} rwlock_t;

void rwlock_acquire_readlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->baseLock);
    rw->readers++;
    if(rw->readers == 1)
       pthread_cond_wait(&rw->writeLock, &rw->baseLock);
    pthread_mutex_unlock(&rw->baseLock);
}

void rwlock_release_readlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->baseLock);
    rw->readers--;
    if (rw->readers == 0)
        pthread_cond_signal(&rw->writeLock, &rw->baseLock);
    pthread_mutex_unlock(&rw->baseLock);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
    pthread_cond_wait(&rw->writeLock, &rw->baseLock);
}

void rwlock_release_writelock(rwlock_t *rw) {
    pthread_cond_signal(&rw->writeLock, &rw->baseLock);
}