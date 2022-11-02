#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

typedef struct node_list_t * node_list_ptr;
typedef struct list_t * list_ptr;

list_ptr new_linked_list(int (*comp_funct)(void *, void *));

void add(list_ptr list, void * data);

int remove(list_ptr list, void * data);

void * find(list_ptr list, void * data, int (*comp_funct)(void *, void *));

/**
 * @brief   Moves the iterator to the first node of the list
 * @param  l: The list
 * @retval None
 */
void to_begin(list_ptr l);

/**
 * @brief  Checks whether the iterator has a next node or not
 * @param  l: The list
 * @retval 0 if there is not other node. != 0 if there is other node
 */
int hasNext(list_ptr l);

/**
 * @brief   Gets the current node element and moves to the next node
 * @param  l: The list
 * @note    Use hasNext function before using next. If current node is NULL the behaviur is undefined
 * @retval The element of the currrent node
 */
void * next(list_ptr l);

void free_list(list_ptr list);

int size(list_ptr l);

#endif 