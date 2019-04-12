#include "rwLock.h"
#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

pthread_mutex_t wl, readCount;
pthread_cond_t noRead;

int reads = 0;

void buffer::insert(int thrId, int num){
    for(int i = 0; i < num; i++){
	if(reads==0){
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
	}else {
		pthread_cond_wait(&noRead, &readCount);
	}
    }
    cout << "job done" << endl;
}

int* buffer::read(int thrId, int num){
    if(reads==0){
	pthread_mutex_lock(&wl);
    }
    pthread_mutex_lock(&readCount);
    reads++;
    pthread_mutex_lock(&readCount);
	    
    int *count = (int *)malloc(num*sizeof(int));
    for(int i = 0; i < num; i++){
        count[i] = 0;
    }
    for(int i = 0; i < num; i++){

        //node *tmp = this->head;
        //while(tmp != NULL){
           // if((*tmp->val % 10) == thrId){count[i]++;}
            //tmp = tmp->next;
        //}
        pthread_mutex_unlock(&wl);
	pthread_mutex_lock(&readCount);
	reads--;
	if(reads==0){pthread_cond_signal(&noRead);}
	pthread_mutex_unlock(&readCount);
	nanosleep(0, NULL);
    }
    return count;
}
