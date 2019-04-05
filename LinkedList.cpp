#include "LinkedList.h"
#include <iostream>
#include <cstdlib>


LinkedList::LinkedList(){
	this->current = this->head = NULL;
}
LinkedList::~LinkedList(){
	current = head;
	while(current != NULL){
		delete current;
		current = current->getNext();
  }
}

bool LinkedList::next(){
	if(current != NULL){
		current = current->getNext();
	}
	if(current == NULL){
		current = head;
		return false;
	} else {
		return true;
	}
}

void LinkedList::reset(){
	this->current = this->head;
}

void LinkedList::print(){
	this->reset();
	while(next()){
		std::cout << current->getData() << ", ";
	}
	std::cout << std::endl;
}

void LinkedList::add(int x){
	if(head == NULL){
		Node * n = new Node(x);
		head = current = n;
		return;
	}
	else if(current == NULL){
		current = head;
	}
	else {
		Node * n = new Node(x);
		while(current != NULL){
			if(current->getNext() == NULL){
				current->setNext(n);
			}
			current = current->getNext();
		}
		current = n;
		current->setNext(NULL);
	}
	return;
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
Node * Node::getNext(){
	if(this->next == NULL) return NULL;
	else{
		return this->next;
	}
}
int Node::getData(){
	return this->data;

}
