#include "rwLock.h"
#include "buffer.h"
#include <stdlib.h>
#include <pthread.h>

rwlock l = rwlock();

void buffer::insert(int thrId, int num){
    l.rwlock_acquire_writelock();
    this->curr = this->head;
    for(int i = 0; i < num; i++){
        while(this->curr->next != nullptr){
            this->curr = this->curr->next;
        }
        int val;
        do
        {
            val = rand() % 1000 + 1;
        } while ((val % 10) != thrId);
        this->curr->next = node(val);
    }
    l.rwlock_release_writelock();
    pthread_yield();
}

int buffer::read(int thrId){
    l.rwlock_acquire_readlock();
    int count = 0;
    node *tmp = this->head;
    while(tmp->next != nullptr){
        if((tmp->val % 10) == thrId){count++;}
        tmp = tmp->next;
    }
    l.rwlock_release_readlock();
    pthread_yield();
    return count;
}