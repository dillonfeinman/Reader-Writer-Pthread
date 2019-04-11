#include "LinkedList.h"
#include <iostream>
#include <cstdlib>
#include <pthread.h>

pthread_mutex_t wlock;

LinkedList::LinkedList(){
	this->current = this->head = NULL;
}
LinkedList::~LinkedList(){
	current = head;
	while(current != NULL){
		delete current;
		current = current->next;
  }
}
void LinkedList::reset(){
	this->current = this->head;
}

void LinkedList::add(int x){
	this->reset();
	pthread_mutex_lock(&wlock);
	if(head == NULL){
		Node * n = new Node(x);
		head = current = n;
		pthread_mutex_unlock(&wlock);
		return;
	}
	else {
		Node * n = new Node(x);
		while(current->next != NULL){
			current = current->next;
		}
		current->setNext(n);
		current = current->next;
		current->setNext(NULL);
	}
	pthread_mutex_unlock(&wlock);
	return;
}
bool LinkedList::next(){
	if(current->next != NULL){
		this->current = this->current->next;
		return true;
	} else {
		current = head;
		return false;
	}
}

Node::Node(int x){
	this->data = x;
	this->next = NULL;
}
Node::Node(int x, Node * n){
	this->data = x;
	this->next = n;
}
void Node::setNext(Node * n){
	this->next = n;
}
