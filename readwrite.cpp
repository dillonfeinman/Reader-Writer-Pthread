#include <iostream>
#include <string>
#include <cstdlib>
#include <pthread.h>
#include <list>
#include "LinkedList.cpp"

using namespace std;

struct args_struct{
  int i;
  int n;
  LinkedList * ll;
};

void * write(void * in){
  args_struct * args = (args_struct *) in;
  srand(time(0));
  cout << "where" << endl;
  int i = (int)args->i;
  int n = (int)args->n;
  cout << "am" << endl;
  for(int j = 0; j < n; j++){
    string ret;
    int randNum = (rand() % 101);
    ret = ret + to_string(randNum) + to_string(i);
    int rn = stoi(ret);
    cout << "i" << endl;
    args->ll->add(rn);
    cout << "breaking" << endl;

  }
  return NULL;
}

void * read(void * in){
  args_struct * args = (args_struct *) in;
  int i = (int)args->i;
  int n = (int)args->n;
  int correct = 0;
  LinkedList * list = (LinkedList *)args->ll;
  int count = 1;
  while(args->ll->getCurrent()->getNext() != NULL){
    if(list->getNodeData() % 10 == i){
      correct++;
    }
  }
  cout << "Reader i: Read: "<< count << ": " << correct << " values ending in " << i << "." << endl;
  count++;
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
      pthread_t numRead[r];
      pthread_t numWrite[w];
      LinkedList * linkedList = new LinkedList();
      args_struct wargs;
      args_struct rargs;
      wargs.ll = linkedList;
      wargs.n = n;
      rargs.ll = linkedList;
      rargs.n = n;
      cout << "get here" << endl;
      pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
      for(int i = 0; i < r; i++){
        pthread_mutex_lock(&lock);
        if(i < w){
          wargs.i = i;
          cout << i << ": get here" << endl;
          pthread_create(&numWrite[i], NULL, write, (void *)&wargs);
          cout << "also here" << endl;
        }
        if(i < r){
          rargs.i = i;
          cout << i << ": get here read" << endl;
          pthread_create(&numRead[i], NULL, read, (void *)&rargs);
          cout << "also here read" << endl;
        }
        pthread_mutex_unlock(&lock);
      }
      LinkedList * newlinkedList = new LinkedList();
      linkedList = newlinkedList;
    }
  }
}
