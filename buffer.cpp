#include "rwLock.h"
#include "buffer.h"
#include <stdlib.h>
#include <pthread.h>

rwlock l = rwlock();

void buffer::insert(int thrId, int num){
    l.rwlock_acquire_writelock();
    this->curr = this->head;
    for(int i = 0; i < num; i++){
        srand(time(0));
        while(this->curr->next != nullptr){
            this->curr = this->curr->next;
        }
        int val = 0;
        do
        {
            val = rand() % 1000 + 1;
        } while ((val % 10) != thrId);
        this->curr->next = &node(val);
    }
    l.rwlock_release_writelock();
    pthread_yield();
}

int* buffer::read(int thrId, int num){
    l.rwlock_acquire_readlock();
    int *count = (int *)malloc(num*sizeof(int));
    for(int i = 0; i < num; i++){
        count[i] = 0;
    }
    for(int i = 0; i < num; i++){
        node *tmp = this->head;
        while(tmp->next != nullptr){
            if((*tmp->val % 10) == thrId){count[i]++;}
            tmp = tmp->next;
        }
        l.rwlock_release_readlock();
        pthread_yield();
    }
    return count;
}