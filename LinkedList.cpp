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
void LinkedList::reset(){
	this->current = this->head;
}

void LinkedList::print(){
	this->reset();
	while(current->getNext() != NULL){
		std::cout << current->getData() << ", ";
		current = current->getNext();
	}
	if(current != NULL){
		std::cout << current->getData() << ".";
	}
	std::cout << std::endl;
}

void LinkedList::add(int x){
	this->reset();
	if(head == NULL){
		Node * n = new Node(x);
		head = current = n;
		return;
	}
	else {
		Node * n = new Node(x);
		while(current->getNext() != NULL){
			current = current->getNext();
		}
		current->setNext(n);
		current = current->getNext();
		current->setNext(NULL);
	}
	return;
}
bool LinkedList::next(){
	if(current->getNext() != NULL){
		this->current = this->current->getNext();
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
Node * Node::getNext(){
	if(this->next == NULL) return NULL;
	else{
		return this->next;
	}
}
Node * LinkedList::getCurrent(){
	return this->current;
}
int Node::getData(){
	return this->data;

}
int LinkedList::getNodeData(){
	return current->getData();
}
