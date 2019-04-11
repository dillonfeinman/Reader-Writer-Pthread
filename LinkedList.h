#ifndef LINKEDLIST_H
#define LINKEDLIST_H

class Node{
public:
	int data;
	Node * next;

	Node(int);
	Node(int, Node *);
	void setNext(Node *);
};

class LinkedList{
public:
	Node * head;
	Node * current;

	LinkedList();
	~LinkedList();
	void print();
	bool next();
	void reset();
	bool remove(int);
	void add(int);
};





#endif
