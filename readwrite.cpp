#include <iostream>
#include <string>
#include <cstdlib>
#include <pthread.h>
#include <list>
#include "LinkedList.cpp"

using namespace std;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct args_struct{
  int i;
  int n;
  LinkedList * ll;
};

void * write(void * in){
  args_struct * args = (args_struct *) in;
  pthread_mutex_lock(&m);
  srand(time(0));
  int i = (int)args->i;
  int n = (int)args->n;
  int j = 0;
  while(j != n){
    string ret;
    int randNum = (rand() % 101);
    ret = ret + to_string(randNum) + to_string(i);
    if(ret != ""){
      int rn = stoi(ret);
      args->ll->add(rn);
    }
    j++;
    pthread_cond_wait(&cond, &m);
  }
  pthread_mutex_unlock(&m);
  return NULL;
}

void * read(void * in){
  args_struct * args = (args_struct *) in;
  pthread_mutex_lock(&m);
  int i = (int)args->i;
  int n = (int)args->n;
  int correct = 0;
  LinkedList * list = (LinkedList *)args->ll;
  int count = 1;
  args->ll->reset();
  do{
    cout << list->current->data << endl;
    if(list->current->data % 10 == i){
      correct++;
    }
    pthread_cond_signal(&cond);
  } while(args->ll->next() == true);
  cout << "Reader i: Read: "<< count << ": " << correct << " values ending in " << i << "." << endl;
  count++;
  pthread_mutex_unlock(&m);
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
      for(int i = 0; i < r || i < w; i++){
        pthread_t numRead;
        pthread_t numWrite;
        LinkedList * linkedList = new LinkedList();
        rargs.ll = linkedList;
        wargs.ll = linkedList;
        if(i < w){
          wargs.i = i;
          pthread_create(&numWrite, NULL, write, (void *)&wargs);
        }
        if(i < r){
          rargs.i = i;
          pthread_create(&numRead, NULL, read, (void *)&rargs);
        }
        pthread_mutex_unlock(&m);
      }
    }
  }
}
