#include "rwLock.h"
#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

pthread_mutex_t wl, test;

void buffer::insert(int thrId, int num){
    for(int i = 0; i < num; i++){
	pthread_mutex_lock(&wl);
	this->curr = this->head;
        srand(time(0));
	cout << i+1 << ": thread: " << thrId << " enter gen loop" << endl;
	for(;;){
		int val = rand() % 1000 + 1;
		if((val%10)==thrId){
			cout << "need to insert: " << val << endl;
			if(this->curr==NULL){
				//node *n = (node *)malloc(sizeof(node));
				//node tmp = node(val);
				//*n = tmp;
				//this->curr = n;
				//this->head = this->curr;
				cout << "val inserted at head" << endl;
			}
			else{
				cout << "non-empty list" << endl;
				for(;;){
					if(this->curr->next==NULL){
						//node *n = (node *)malloc(sizeof(node));
						//node tmp = node(val);
						//*n = tmp;
						//this->curr->next = n;
					       	break;	
					} else {
						this->curr = this->curr->next;
					}
					
				}
				cout << "val inserted at tail" << endl;
			}
			break;
		}
	}
	pthread_mutex_unlock(&wl);
	nanosleep(0, NULL);
    }
    cout << "job done" << endl;
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
