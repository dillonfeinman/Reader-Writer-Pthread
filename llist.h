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

		~llist(){
			while(head != tail){
				node *tmp = head;
				head = head->next;
				free(tmp);
			}
			free(head);
		}
};

#endif
