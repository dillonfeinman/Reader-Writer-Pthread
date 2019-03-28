#include <iostream>
#include <string>
#include <cstdlib>
#include <pthread.h>

using namespace std;

void * write(void *){
  int randNum = (rand() % 1000);
  cout << randNum << endl;
}

void * read(void *){

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
      for(int i = 0; i < r || i < w; i++){
        if(i < r){
          pthread_create(&numRead[i], NULL, read, NULL);
        }
        if(i < w){
          pthread_create(&numWrite[i], NULL, write, NULL);
        }
      }
    }
  }
}
