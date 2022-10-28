#include <pmm.h>
#include <linked_list.h>

typedef struct node
{
    void *data;
    struct node *next;
} node;

typedef struct list
{
    struct node * start;
    struct node * end;
    struct node * current;
    //function that will compare node->data with argument "data" for deletion
    int (*comp_funct)(void*, void*);
} list;

static node *delete_helper(list *list, node *node, void *data)
{
    if (node == NULL)
    {
        return NULL;
    }
    if (list->comp_funct(node->data,data))
    {
        struct node * ret_node = node->next;
        kfree(node);
        return ret_node;
    }
    node->next = delete_helper(list, node->next, data);
    if (node->next == NULL)
    {
        list->end = node;
    }
    return node;
}

static node *create_node(void *data)
{
    node *new_node = (node *)kmalloc(sizeof(node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

list *new_linked_list(int (*comp_funct)(void *, void *))
{
    list *new_list = kmalloc(sizeof(list));
    new_list->start = NULL;
    new_list->end = NULL;
    new_list->comp_funct=comp_funct;
    return new_list;
}

void to_begin(list_t l){
    l->current = l->start;
}

int hasNext(list_t l){
    return l->current != NULL;
}

void * next(list_t l){
    void * element = l->current->data;
    l->current = l->current->next;
    return element;
}

void add(list *list, void *data)
{
    node *new_node = create_node(data);

    if (list->start == NULL)
    {
        list->start = new_node;
        list->end = new_node;
    }
    else
    {
        list->end->next = new_node;
        list->end = list->end->next;
    }
}

void remove(list *list, void *data)
{
    list->start = delete_helper(list, list->start, data);
}

void * find(list * list, void * data){
    node * node= list->start;
    while (node != NULL){
        if (list->comp_funct(node->data, data))
            return node->data;
        node=node->next;
    }
    return NULL;
}
