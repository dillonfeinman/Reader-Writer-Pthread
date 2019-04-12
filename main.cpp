#include "buffer.h"
#include "rwLock.h"
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;

pthread_mutex_t o;
buffer buff;
int num, readers;

void * write(void * in){
    int *input = (int *) in;
    int threadID = *input;
    buff.insert(threadID, num);
}

void * read(void * in){
    int *input = (int *) in;
    int threadID = *input;
    int *readCount = (int *)malloc(sizeof(int)*num);

    readCount = buff.read(threadID, num);

    //file output
    string outfile = "reader_" + to_string(threadID) + ".txt";

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
	num = n;
        int r = stoi(argv[2]);
        int w = stoi(argv[3]);
        if(n < 1 || n > 1000 || r < 1 || r > 9 || w < 1 || w > 9){
            cerr << "R and W must be between 1 and 9, inclusive, and N must be between 1 and 1000, inclusive." << endl;
            exit(2);
        } else {
            buff = buffer();
			pthread_t writer;
            for(int j = 0; j < w; j++){
                int *in = (int *)malloc(sizeof(int));
				*in  = j+1;
                pthread_create(&writer, NULL, write, (void *) in);
				
            }
			pthread_t reader;
            for(int k = 0; k < r; k++){
                int *in = (int *)malloc(sizeof(int));
				*in = k+1;
                pthread_create(&reader, NULL, read, (void *) in);
            }
			pthread_join(writer, NULL);
			pthread_join(reader, NULL);
        }
    }

    return 0;
}
