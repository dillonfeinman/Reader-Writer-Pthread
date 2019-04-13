#ifndef _NODE_H
#define _NODE_H

#include <stdlib.h>

class node{
	public:
		int val;
		node *next;

		node(int v){
			val = v;
			next = NULL;
		};

		~node(){
			free(next);
		}
};

#endif
