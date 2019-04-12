#include "rwLock.h"
#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

pthread_mutex_t wl;
pthread_mutex_t readCount;
pthread_cond_t noRead;
bool writer = false;

int reads = 0;

void buffer::insert(int thrId, int num){
    //cout << "thread: " << thrId << "insert start" << endl;
    for(int i = 0; i < num; i++){
	if(reads==0){
		pthread_mutex_lock(&wl);
		writer = false;
		this->curr = this->head;
	        srand(time(0));
		for(;;){
			int val = rand() % 1000 + 1;
			if((val%10)==thrId){
				//cout << "need to insert: " << val << endl;
				if(this->curr==NULL){
					cout <<1;
					node *n = (node *)malloc(sizeof(node));
					cout << 2;
					node tmp = node(val);
					cout << 3;
					*n = tmp;
					cout << n->val << endl;
					//*n = tmp;
					//this->curr = n;
					//this->head = this->curr;
					//cout << "val inserted at head" << endl;
				}
				else{
					//cout << "non-empty list" << endl;
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
					//cout << "val inserted at tail" << endl;
				}
				break;
			}
		}
		pthread_mutex_unlock(&wl);
		nanosleep(0, NULL);
	}else {
		writer = true;
		pthread_cond_wait(&noRead, &readCount);
	}
    }
   // cout << "thread: " << thrId << "insert done" << endl;
}

int* buffer::read(int thrId, int num){
	cout << "read start" << endl;
    while(writer){ //Waiting writers get priority;
	nanosleep(0, NULL);
    }
	    
    int *count = (int *)malloc(num*sizeof(int));
    int i = 0;
    while(i < num){
        count[i] = 0;
	i++;
    }
    i = 0;
    while(i < num){
	pthread_mutex_lock(&readCount);
	reads++;
	cout << reads << endl;
	pthread_mutex_unlock(&readCount);

        node *tmp = this->head;
        while(tmp != NULL){
            if((*tmp->val % 10) == thrId){count[i]++;}
            tmp = tmp->next;
        }
	
	pthread_mutex_lock(&readCount);
	reads--;
	cout << reads << endl;
	if(reads==0){pthread_cond_signal(&noRead);}
	pthread_mutex_unlock(&readCount);
	nanosleep(0, NULL);
	i++;
    }
    cout << "read done" << endl;
    return count;
}
