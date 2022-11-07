// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <linked_list.h>
#include <pmm.h>

#define ERROR -1
#define SUCCESS 0

typedef struct node_list_t
{
    void *data;
    struct node_list_t *next;
} node_list_t;

typedef struct list_t {
    struct node_list_t *start;
    struct node_list_t *end;
    struct node_list_t *current;
    // function that will compare node->data with argument "data" for deletion
    int (*comp_funct)(void *, void *);
    int size;
} list_t;

list_t *new_linked_list(int (*comp_funct)(void *, void *)) {
    list_t *new_list = kmalloc(sizeof(list_t));
    new_list->start = NULL;
    new_list->end = NULL;
    new_list->comp_funct = comp_funct;
    new_list->size = 0;
    return new_list;
}

static node_list_t *delete_helper(list_t *list, node_list_t *node, void *data) {
    if (node == NULL) {
        return NULL;
    }
    if (list->comp_funct(node->data, data)) {
        struct node_list_t *ret_node = node->next;
        kfree(node);
        return ret_node;
    }
    node->next = delete_helper(list, node->next, data);
    if (node->next == NULL) {
        list->end = node;
    }
    return node;
}

static node_list_t *create_node(void *data) {
    node_list_t *new_node = (node_list_t *)kmalloc(sizeof(node_list_t));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void to_begin(list_t *l) {
    l->current = l->start;
}

int hasNext(list_t * l){
    return l->current != NULL;
}

int size(list_t *l) {
    return l->size;
}

void *next(list_t *l) {
    void *element = l->current->data;
    l->current = l->current->next;
    return element;
}

void add(list_t *list, void *data) {
    node_list_t *new_node = create_node(data);

    if (list->start == NULL) {
        list->start = new_node;
        list->end = new_node;
    } else {
        list->end->next = new_node;
        list->end = list->end->next;
    }

    list->size++;
}

int remove(list_t *list, void *data)
{
    list->start = delete_helper(list, list->start, data);
    list->size--;
    return SUCCESS;
}

void *find(list_t *list, void *data, int (*comp_funct)(void *, void *)) {
    int (*funct)(void *, void *);
    if (comp_funct == NULL) {
        funct = list->comp_funct;
    } else {
        funct = comp_funct;
    }
    node_list_t *node = list->start;
    while (node != NULL) {
        if (funct(node->data, data))
            return node->data;
        node = node->next;
    }
    return NULL;
}

static void free_list_helper(node_list_ptr node){
    if (node == NULL){
        return;
    }
    free_list_helper(node->next);
    kfree(node);
}

void free_list(list_ptr list){
    free_list_helper(list->start);
    kfree(list);
}
