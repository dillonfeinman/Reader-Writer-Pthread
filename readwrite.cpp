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
  cout << "Hey" << endl;
  args_struct * args = (args_struct *) in;
  cout << "Hey2" << endl;
  int i = (int)args->i;
  int n = (int)args->n;
  cout << "Hey2" << endl;
  for(int j = 0; j < n; j++){
    int count = 0;
    cout << "Hey3" << endl;
    pthread_mutex_lock(&wmutex);
    cout << "wmutex acquired" << endl;
    writecount++;
    cout << "Hey5" << endl;
    if(writecount == 1){
      pthread_mutex_lock(&readTry);
      cout << "readTry acquired" << endl;
    }
    pthread_mutex_unlock(&wmutex);
    cout << "wmutex unlocked" << endl;
    cout << "lock res" << endl;
    pthread_mutex_lock(&resource);
    cout << "resource acquired" << endl;
    string ret;
    int randNum = (rand() % 1000) + 1;
    while(randNum % 10 != i){
      randNum = (rand() % 1000) + 1;
    }
    if(randNum % 10 == i){
	    count++;
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
    }
    linkedList->current = linkedList->head;
    while(linkedList->current->next != NULL){
      cout << linkedList->current-> data << ", " << endl;
      linkedList->current = linkedList->current->next;
    }
    cout << linkedList->current-> data << "." << endl;
    pthread_mutex_unlock(&resource);
    cout << "resource unlocked" << endl;
    pthread_mutex_lock(&wmutex);
    cout << "wmutex locked" << endl;
    writecount--;
    if(writecount == 0){
      pthread_mutex_unlock(&readTry);
      cout << "readtry unlocked" << endl;
    }
    pthread_mutex_unlock(&wmutex);
    cout << "wmutex unlocked" << endl;
  }
  usleep(10);
  return NULL;
}

void * read(void * in){
  cout << "Hi" << endl;
  args_struct * args = (args_struct *) in;
  int i = (int)args->i;
  int n = (int)args->n;
  int correct = 0;
  cout << "Hi2" << endl;
  for(int j = 0; j < n; j++){
    cout << "Hi3" << endl;
    cout << "Hi4" << endl;
    pthread_mutex_lock(&readTry);
    cout << "readTry locked" << endl;
    cout << "Hi5" << endl;
    pthread_mutex_lock(&rmutex);
    cout << "rmutex acquired" << endl;
    cout << "Hi6" << endl;
    readcount++;
    if(readcount == 1){
      cout << "lock res" << endl;
      pthread_mutex_lock(&resource);
      cout << "res acquired" << endl;
    }
    pthread_mutex_unlock(&rmutex);
    cout << "rmutex unloc" << endl;
    pthread_mutex_unlock(&readTry);
    cout << "readtry unlock" << endl;
    linkedList->current = linkedList->head;
    while(linkedList->current->next != NULL){
      cout << "am i here" << endl;
      if(linkedList->current->data % 10 == i){
        correct++;
      }
	    linkedList->current = linkedList->current->next;
    }
    cout << "Reader " << i << ": Read " << correct << endl;
    pthread_mutex_lock(&rmutex);
    readcount--;
    if(readcount == 0){
      pthread_mutex_unlock(&resource);
    }
    pthread_mutex_unlock(&rmutex);
  }
  usleep(10);
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
      linkedList->current = NULL;
      linkedList->head = NULL;
      for(int i = 0; i < r || i < w; i++){
        if(i < w){
          wargs.i = i;
          pthread_create(&numWrite[i], NULL, write, (void *)&wargs);
        }
        if(i < r){
          rargs.i = i;
          pthread_create(&numRead[i], NULL, read, (void *)&rargs);
        }
      }
      for(int i = 0; i < r || i < w; i++){
      	if(i < w){
      		pthread_join(numWrite[i], NULL);
      	}
      	if(i < r){
      		pthread_join(numRead[i], NULL);
      	}
      }
    }
  }
}
