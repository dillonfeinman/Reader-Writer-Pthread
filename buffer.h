#ifndef _BUFFER_H
#define _BUFFER_H

class node{
public:
    int *val;
    node *next;

    node(int *v){
        val = v;
        next = nullptr;
    }
};

class buffer{
private:
    node *head;
    node *curr;

public:
    buffer(){
        head = nullptr;
        curr = nullptr;
    };
    void insert(int, int);
    int read(int);
    ~buffer();
};

#endif