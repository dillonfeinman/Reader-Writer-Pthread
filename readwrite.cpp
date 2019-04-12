#include <iostream>
#include <string>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

using namespace std;

struct Node{
  int data;
  Node * next;
};

struct LinkedList{
  Node * head;
  Node * current;
};

struct args_struct{
  int i;
  int n;
};

int writecount = 0;
int readcount = 0;

pthread_mutex_t rmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t wmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t readTry = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource = PTHREAD_MUTEX_INITIALIZER;

LinkedList * linkedList = (LinkedList *)malloc(sizeof(LinkedList));

void * write(void * in){
  args_struct * args = (args_struct *) in;
  int i = (int)args->i;
  int n = (int)args->n;
  for(int i = 0; i < n; i++){
    pthread_mutex_lock(&wmutex);
    writecount++;
    if(writecount == 1){
      pthread_mutex_lock(&readTry);
    }
    pthread_mutex_unlock(&wmutex);
    pthread_mutex_lock(&resource);
    string ret;
    int randNum = (rand() % 1000) + 1;
    if(randNum % 10 == i){
	if(linkedList->head == NULL){
      Node * n = (Node *)malloc(sizeof(Node));
      n->data = randNum;
      linkedList->head = linkedList->current = n;
      linkedList->current->next = NULL;
    }
    else {
      Node * n = (Node *)malloc(sizeof(Node));
      n->data = randNum;
      while(linkedList->current->next != NULL){
        linkedList->current = linkedList->current->next;
      }
      linkedList->current->next = n;
      linkedList->current = linkedList->current->next;
      linkedList->current->next = NULL;
    }
    pthread_mutex_unlock(&resource);
    pthread_mutex_lock(&wmutex);
    writecount--;
    if(writecount == 0){
      pthread_mutex_unlock(&readTry);
    }
    pthread_mutex_unlock(&wmutex);
    }
  }
  usleep(100000);
  return NULL;
}

void * read(void * in){
  args_struct * args = (args_struct *) in;
  int i = (int)args->i;
  int n = (int)args->n;
  int correct = 0;
  for(int j = 0; j < n; j++){
    pthread_mutex_lock(&readTry);
    pthread_mutex_lock(&rmutex);
    readcount++;
    if(readcount == 1){
      pthread_mutex_lock(&resource);
    }
    pthread_mutex_unlock(&rmutex);
    pthread_mutex_unlock(&readTry);
    linkedList->current = linkedList->head;
    while(linkedList->current->next != NULL){
      if(linkedList->current->data % 10 == i){
        correct++;
      }
	linkedList->current = linkedList->current->next;
    }
    cout << correct << endl;
    pthread_mutex_lock(&rmutex);
    readcount--;
    if(readcount == 0){
      pthread_mutex_unlock(&resource);
    }
    pthread_mutex_unlock(&rmutex);
  }
  usleep(1000);
  return NULL;
}

int main(int argc, char * argv[]){

  if(argc != 4){
    cerr << "Needs 3 arguments: <N rand ints> <R readers> <W writers>." << endl;
    exit(1);
  } else {
    int n = stoi(argv[1]);
    int r = stoi(argv[2]);
    int w = stoi(argv[3]);
    if(n < 1 || n > 1000 || r < 1 || r > 9 || w < 1 || w > 9){
      cerr << "R and W must be between 1 and 9, inclusive, and N must be between 1 and 1000, inclusive." << endl;
      exit(2);
      //start code
    } else {
      args_struct wargs;
      args_struct rargs;
      wargs.n = n;
      rargs.n = n;
      pthread_t * numRead = new pthread_t[r];
      pthread_t * numWrite = new pthread_t[w];
      for(int i = 0; i < r || i < w; i++){
        linkedList->current = NULL;
        linkedList->head = NULL;
        if(i < w){
          wargs.i = i;
          pthread_create(&numWrite[i], NULL, write, (void *)&wargs);
        }
        if(i < r){
          rargs.i = i;
          pthread_create(&numRead[i], NULL, read, (void *)&rargs);
        }
      }
    }
  }
}
