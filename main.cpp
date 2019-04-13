#include <pthread.h>
#include "node.h"
#include "llist.h"
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int num;

int reader = 0;
int writer = 0;
llist *list;

pthread_mutex_t rm = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t waitMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t wm = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rt = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t re = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void * wait(void * in){
  pthread_mutex_lock(&waitMutex);
  pthread_cond_wait(&c, &waitMutex);
  cout << "Almost Done!" << endl;
  pthread_mutex_unlock(&waitMutex);
}

void * read(void * in){
    int *input = (int *) in;
    int thr = *input;
    int *count = (int *)malloc(sizeof(int)*num);
    for(int i = 0; i < num; i++){
        count[i]=0;
    }
    for(int i = 0; i < num; i++){
        //Entry
        pthread_mutex_lock(&rt);
        pthread_mutex_lock(&rm);
        reader++;
        if(reader==1){
            pthread_mutex_lock(&re);
        }
        pthread_mutex_unlock(&rm);
        pthread_mutex_unlock(&rt);

        //Critical section

        //Read list
        node *tmp = list->head;
        int k = 0;
        while(tmp->next != NULL){
            if((tmp->val % 10) == thr){
                count[i]++;
            }
            tmp = tmp->next;
        }
        cout << "here" << endl;
        //////////
        cout << count[i] << endl;
        string out = "reader_" + to_string(thr) + ".txt";

    	ofstream o (out);
    	for(int i = 0; i < num; i++){
        o << "Reader " << thr << ": Read " << (i+1) << ": " << count[i] << " values ending in " << thr << endl;
    }
    	o.close();
        //Exit
        pthread_mutex_lock(&rm);
        reader--;
        if(reader==0){
             pthread_mutex_unlock(&re);
        }
        pthread_mutex_unlock(&rm);
        usleep(100000000);
    }
}

void * write(void * in){
    for(int e = 0; e < num; e++){
    	  pthread_mutex_lock(&wm);
    	  if(writer == 1){
          pthread_mutex_lock(&rt);
    	  }
  	  pthread_mutex_lock(&re);

	  //Critical section
  	  int *input = (int *) in;
  	  int thr = *input;

	  // cout << thr << endl;

  	  int val;
  	  for(;;){
  	      val = rand()%1000 + 1;
  	      if((val%10)==thr){
  	          break;
  	      }
  	  }

	  if(list->head == NULL){
		node *n = new node(val);
		list->head = n;
		list->tail = n;
	  } else {
  	  	node *n = new node(val);
  	 	list->tail->next = n;
		list->tail = n;
	  }
	// node *tmp = list->head;
 //    while(tmp->next != NULL){
	// 	cout << tmp->val << "->";
	// 	tmp = tmp->next;
 //    }
 //    cout << tmp->val << endl;
	  //Exit
  	  pthread_mutex_unlock(&re);
  	  pthread_mutex_unlock(&rt);
  	  pthread_mutex_unlock(&wm);
	  usleep(100000000);
    }
    cout << "done" << endl;
    return NULL;
}

int main(int argc, char * argv[]){
    list = new llist();
    if(argc != 4){
      cerr << "Needs 3 arguments: <N rand ints> <R readers> <W writers>." << endl;
      exit(1);
    } else {
        int n = stoi(argv[1]);
	    num = n;
        int r = stoi(argv[2]);
        int w = stoi(argv[3]);
        if(n < 1 || n > 1000 || r < 1 || r > 9 || w < 1 || w > 9){
            cerr << "R and W must be between 1 and 9, inclusive, and N must be between 1 and 1000, inclusive." << endl;
            exit(2);
        } else {
            pthread_t writers[w];
            pthread_t readers[r];
            pthread_t waiter;

            //Create threads
            pthread_create(&waiter, NULL, wait, NULL);
            for(int i = 0; i < w; i++){
                int *v = (int *)malloc(sizeof(int));
		*v = i+1;
                pthread_create(&writers[i], NULL, write, v);
            }
            for(int i = 0; i < r; i++){
                int *v = (int *)malloc(sizeof(int));
		*v = i+1;
                pthread_create(&readers[r], NULL, read, v);
            }

            //rejoin threads
            for(int i = 0; i < w; i++){
              if(w>r && i == w-2){
                pthread_join(writers[i], NULL);
                pthread_cond_signal(&c);
              } else if(r == w && i == w-2){
                pthread_cond_signal(&c);
                pthread_join(waiter, NULL);
                pthread_join(writers[i], NULL);
              }
              else
                pthread_join(writers[i], NULL);
            }
            for(int i = 0; i < r; i ++){
              if(r>w && i == r-2){
                pthread_cond_signal(&c);
                pthread_join(waiter, NULL);
                pthread_join(readers[i], NULL);
              }
                pthread_join(readers[i], NULL);
            }
        }
    }
    return 0;
}
