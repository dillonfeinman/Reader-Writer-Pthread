#include "buffer.h"
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;

buffer buff;

void * write(void * in){
    int *input = (int *) in;
    int threadID = input[0];
    int num = input[1];
    buff.insert(threadID, num);
}

void * read(void * in){
    int *input = (int *) in;
    int threadID = input[0];
    int num = input[1];
    int *readCount;
    readCount = buff.read(threadID, num);
    
    //file output
    string outfile = "reader_";
    outfile += "";
    outfile += ".txt";

    ofstream out (outfile);
    for(int i = 0; i < num; i++){
        out << "Reader " << threadID << ": Read " << i+1 << ": " << readCount[i] << " values ending in " << threadID << endl;
    }
    out.close();
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
        } else {
            buff = buffer();
            for(int i = 0; i < w; i++){
                int in[2];
                in[0] = i+1;
                in[1] = n;
                pthread_t writer;
                pthread_create(&writer, NULL, write, (void *) in);
            }
            for(int i = 0; i < w; i++){
                int in[2];
                in[0] = i+1;
                in[1] = n;
                pthread_t reader;
                pthread_create(&reader, NULL, read, (void *) in);
            }
        }
    }
    return 0;
}