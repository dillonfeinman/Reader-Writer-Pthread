#include "buffer.h"
#include "rwLock.h"
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;

buffer buff;
int num, readers;

void * write(void * in){
    int *input = (int *) in;
    int threadID = *input;
    cout << "w1" << endl;
    buff.insert(threadID, num);
    cout << "w2" << endl;
}

void * read(void * in){
    int *input = (int *) in;
    int threadID = *input;
    int *readCount;

    cout << "r1" << endl;

    //readCount = buff.read(threadID, num);

    cout << "r2" << endl;

    //file output
    string outfile = "reader_" + to_string(threadID) + ".txt";
    
    cout << outfile << endl;

    ofstream out (outfile);
    for(int i = 0; i < num; i++){
        out << "Reader " << threadID << ": Read " << i+1 << ": " << readCount[i] << " values ending in " << threadID << endl;
    }
    out.close();
    cout << "r3" << endl;
}

int main(int argc, char * argv[]){
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
            buff = buffer();
            for(int i = 0; i < w; i++){
                int *in = (int *)malloc(sizeof(int));
		*in  = i+1;
                pthread_t writer;
                pthread_create(&writer, NULL, write, (void *) in);
            }
            for(int i = 0; i < r; i++){
                int *in = (int *)malloc(sizeof(int));
		*in = i+1;
                pthread_t reader;
                pthread_create(&reader, NULL, read, (void *) in);
            }
        }
    }
    return 0;
}
