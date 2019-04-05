#ifndef LINKEDLIST_H
#define LINKEDLIST_H

class Node{
private:
	int data;
	Node * next;
public:
	Node(int);
	Node(int, Node *);
	Node * getNext();
	int getData();
	void setNext(Node *);
};

class LinkedList{
private:
	Node * head;
	Node * current;
public:
	LinkedList();
	~LinkedList();
	void print();
	void reset();
	bool next();
	bool remove(int);
	void add(int);
};





#endif