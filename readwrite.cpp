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
  list<int> * ll;
};

void * write(void * in){
  args_struct * args = (args_struct *) in;
  srand(time(0));
  int i = (int)args->i;
  int n = (int)args->n;
  for(int j = 0; j < n; j++){
    string ret;
    int randNum = (rand() % 101);
    ret = ret + to_string(randNum) + to_string(i);
    int rn = stoi(ret);
    args->ll->push_back(rn);
  }
  return NULL;
}

void * read(void * in){
  
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
      list<int> * linkedList;
      for(int i = 0; i < r || i < w; i++){
        if(i < r){
          args_struct args;
          args.i = i;
          args.n = n;
          args.ll = linkedList;
          pthread_create(&numRead[i], NULL, read, (void *)&args);

        }
        if(i < w){
          pthread_create(&numWrite[i], NULL, write, (void *)i);
        }
      }
    }
  }
}
