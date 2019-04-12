#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdio.h>
#include <stdlib.h>

class node{
public:
    int *val;
    node *next;

    node(int v){
        *val = v;
        next = NULL;
    }
};

class buffer{
private:
    node *head;
    node *curr;

public:
    buffer(){
        head = NULL;
        curr = NULL;
    };
    void insert(int, int);
    int* read(int, int);
    ~buffer();
};

#endif