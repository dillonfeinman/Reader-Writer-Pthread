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
    for(int i = 0; i < num; i++){
	if(reads==0){
		pthread_mutex_lock(&wl);
		writer = false;
		this->curr = this->head;
		for(;;){
			int val = rand() % 1000 + 1;
			if((val%10)==thrId){
				if(this->curr==NULL){
					cout << "hey" << endl;
					node *n = (node *)malloc(sizeof(node));
					node tmp = node(val);
					cout << "hey2" << endl;
					n = &tmp;
					this->curr = n;
					this->curr->next = NULL;
					this->head = this->curr;
					
				}
				else{
					for(;;){
						if(this->curr->next==NULL){
							node *n = (node *)malloc(sizeof(node));
							node tmp = node(val);
							n = &tmp;
							this->curr->next = n;
							this->curr = this->curr->next;
							this->curr->next = NULL;
						       	break;	
						} else {
							this->curr = this->curr->next;
						}
						
					}
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
	cout << "here" << endl;
    for(int j = 0; j < num; j++){
	pthread_mutex_lock(&readCount);
	cout << "here2" << endl;
	reads++;
	pthread_mutex_unlock(&readCount);
	cout << "here3" << endl;
        node *tmp = this->head;
	cout << "here4" << endl;
        while(tmp != NULL){
            if((tmp->val % 10) == thrId){count[j]++;}
            tmp = tmp->next;
        }
	cout << "here5" << endl;
	
	pthread_mutex_lock(&readCount);
	reads--;
	if(reads==0){pthread_cond_signal(&noRead);}
	pthread_mutex_unlock(&readCount);
	nanosleep(0, NULL);
    }
    cout << "read done" << endl;
    return count;
}
