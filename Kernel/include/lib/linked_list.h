#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

typedef struct node * node_t;
typedef struct list * list_t;

list_t new_linked_list();
void add(list_t list, void * data);
void remove(list_t list, void * data);

#endif