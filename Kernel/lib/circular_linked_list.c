#include <circular_linked_list.h>
#include <stdlib.h>

#define ERROR   -1
#define SUCCESS 0

static int visited = 0;

typedef struct node_list_t *node_list_ptr;
typedef struct node_list_t {
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

list_t *new_circular_linked_list(int (*comp_funct)(void *, void *)) {
    list_t *new_list = kmalloc(sizeof(list_t));
    new_list->start = NULL;
    new_list->end = NULL;
    new_list->comp_funct = comp_funct;
    new_list->size = 0;
    return new_list;
}

static node_list_t *create_node(void *data) {
    node_list_t *new_node = (node_list_t *)kmalloc(sizeof(node_list_t));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void cl_to_begin(list_t *l) {
    l->current = l->start;
}

int cl_has_next(list_t *l) {
    if (l->current == l->start && visited) {
        return 0;
    }
    visited = 1;
    return 1;
}

int cl_size(list_t *l) {
    return l->size;
}

void *cl_next(list_t *l) {
    void *element = l->current->data;
    l->current = l->current->next;
    return element;
}

void cl_add(list_t *list, void *data) {
    node_list_t *new_node = create_node(data);

    if (list->start == NULL) {
        list->start = new_node;
        list->current = list->start;
    } else
        list->end->next = new_node;

    list->end = new_node;
    list->end->next = list->start;

    list->size++;
}

static node_list_t *find_node_before(list_t *list, void *data) {
    node_list_t *aux_node = list->start;

    do {
        if (list->comp_funct(aux_node->next->data, data))
            return aux_node;

        aux_node = aux_node->next;
    } while (aux_node != list->start);

    return NULL;
}

void *cl_remove(list_t *list, void *data) {
    node_list_t *node_before_target = find_node_before(list, data);

    if (node_before_target == NULL)
        return NULL;

    node_list_t *target_node = node_before_target->next;

    if (list->current == target_node)
        list->current = list->current->next;

    node_before_target->next = target_node->next;

    if (list->end == target_node)
        list->end = node_before_target;

    if (list->start == target_node)
        list->start = target_node->next;

    kfree(target_node);
    list->size--;
    return target_node->data;
}

void *cl_find(list_t *list, void *data, int (*comp_funct)(void *, void *)) {
    int (*funct)(void *, void *);
    if (comp_funct == NULL) {
        funct = list->comp_funct;
    } else {
        funct = comp_funct;
    }
    node_list_t *node = list->start;
    do {
        if (funct(node->data, data))
            return node->data;

        node = node->next;
    } while (node != list->start);
    return NULL;
}

void cl_free_list(circular_list_t list) {
    node_list_ptr node = list->start;
    node_list_ptr tmp;
    do {
        tmp = node;
        node = node->next;
        kfree(tmp);
    } while (node != list->start);
    kfree(list);
}
