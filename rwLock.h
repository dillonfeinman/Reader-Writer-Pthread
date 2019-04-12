#ifndef _RWLOCK_H
#define _RWLOCK_H

#include <pthread.h>

class rwlock{
private:
    pthread_mutex_t baseLock = PTHREAD_COND_INITIALIZER;
    pthread_cond_t writeLock = PTHREAD_MUTEX_INITIALIZER;
    int readers = 0;
    
public:
    rwlock();
    void rwlock_acquire_readlock();
    void rwlock_release_readlock();
    void rwlock_acquire_writelock();
    void rwlock_release_writelock();
};

#endif