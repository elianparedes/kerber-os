#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

typedef struct node * node_t;
typedef struct list * list_t;

list_t new_linked_list(int (*comp_funct)(void *, void *));
void add(list_t list, void * data);
void remove(list_t list, void * data);
void * find(list_t list, void * data);

/**
 * @brief   Moves the iterator to the first node of the list
 * @param  l: The list
 * @retval None
 */
void to_begin(list_t l);

/**
 * @brief  Checks whether the iterator has a next node or not
 * @param  l: The list
 * @retval 0 if there is not other node. != 0 if there is other node
 */
int hasNext(list_t l);

/**
 * @brief   Gets the current node element and moves to the next node
 * @param  l: The list
 * @note    Use hasNext function before using next. If current node is NULL the behaviur is undefined
 * @retval The element of the currrent node
 */
void * next(list_t l);

#endif