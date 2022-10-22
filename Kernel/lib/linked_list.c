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
} list;

static node *delete_helper(list *list, node *node, void *data)
{
    if (node == NULL)
    {
        return NULL;
    }
    if (node->data == data)
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

list *new_linked_list()
{
    list *new_list = kmalloc(sizeof(list));
    new_list->start = NULL;
    new_list->end = NULL;
    return new_list;
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