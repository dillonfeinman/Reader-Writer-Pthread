#include "rwLock.h"
#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

pthread_mutex_t wl;

void buffer::insert(int thrId, int num){
    for(int i = 0; i < num; i++){
	pthread_mutex_lock(&wl);
	this->curr = this->head;
        srand(time(0));
        while(this->curr->next != NULL){
            this->curr = this->curr->next;
        }
        int val = 0;
        do
        {
            val = rand() % 1000 + 1;
        } while ((val % 10) != thrId);
	cout << "insert: " << val << endl;;
        node *n = (node *)malloc(sizeof(node));
        *n = node(val);
        this->curr->next = n;
	pthread_mutex_unlock(&wl);
	nanosleep(0, NULL);
    }
}

int* buffer::read(int thrId, int num){
    int *count = (int *)malloc(num*sizeof(int));
    for(int i = 0; i < num; i++){
        count[i] = 0;
    }
    for(int i = 0; i < num; i++){
        node *tmp = this->head;
	//l.rwlock_acquire_readlock();
        while(tmp->next != NULL){
            if((*tmp->val % 10) == thrId){count[i]++;}
            tmp = tmp->next;
        }
        //l.rwlock_release_readlock();
	//sleep(1);
    }
    return count;
}
