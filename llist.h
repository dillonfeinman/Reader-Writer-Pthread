#ifndef _LLIST_H
#define _LLIST_H

#include "node.h"
#include "stdlib.h"

class llist{
	public:
		node *head;
		node *tail;

		llist(){
			head = NULL;
			tail = NULL;
		};
};

#endif
