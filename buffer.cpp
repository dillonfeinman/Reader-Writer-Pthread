#include "rwLock.h"
#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

rwlock l = rwlock();

void buffer::insert(int thrId, int num){
    l.rwlock_acquire_writelock();
    this->curr = this->head;
    for(int i = 0; i < num; i++){
        srand(time(0));
        while(this->curr->next != NULL){
            this->curr = this->curr->next;
        }
        int val = 0;
        do
        {
            val = rand() % 1000 + 1;
        } while ((val % 10) != thrId);
        node *n = (node *)malloc(sizeof(node));
        *n = node(val);
        this->curr->next = n;
    }
    l.rwlock_release_writelock();
    sleep(1);
}

int* buffer::read(int thrId, int num){
    l.rwlock_acquire_readlock();
    int *count = (int *)malloc(num*sizeof(int));
    for(int i = 0; i < num; i++){
        count[i] = 0;
    }
    for(int i = 0; i < num; i++){
        node *tmp = this->head;
        while(tmp->next != NULL){
            if((*tmp->val % 10) == thrId){count[i]++;}
            tmp = tmp->next;
        }
        l.rwlock_release_readlock();
        sleep(1);
    }
    return count;
}