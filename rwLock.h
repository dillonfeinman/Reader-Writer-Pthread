#ifndef _RWLOCK_H
#define _RWLOCK_H

#include <pthread.h>

class rwlock_t{
private:
    pthread_mutex_t baseLock = PTHREAD_COND_INITIALIZER;
    pthread_cond_t writeLock = PTHREAD_MUTEX_INITIALIZER;
    int readers = 0;
public:
    rwlock_t();
    void rwlock_acquire_readlock(rwlock_t *rw);
    void rwlock_release_readlock(rwlock_t *rw);
    void rwlock_acquire_writelock(rwlock_t *rw);
    void rwlock_release_writelock(rwlock_t *rw);
};

#endif