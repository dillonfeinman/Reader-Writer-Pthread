#ifndef _RWLOCK_H
#define _RWLOCK_H

#include <pthread.h>
#include <stdlib.h>

class rwlock{
private:
    pthread_mutex_t baseLock;
    pthread_cond_t writeLock;
    int readers = 0;
    
public:
    rwlock(){
	pthread_mutex_init(&baseLock, NULL);
	pthread_cond_init(&writeLock, NULL);	
    };
    void rwlock_acquire_readlock();
    void rwlock_release_readlock();
    void rwlock_acquire_writelock();
    void rwlock_release_writelock();
};

#endif
