#include <fifo_queue.h>
#include <pmm.h>

typedef struct node
{
    void *data;
    struct node *next;
} node_t;

typedef struct fifo_queue
{
    node_t *front;
    node_t *rear;
} fifo_queue;

fifo_queue *new_fifo_queue()
{
    fifo_queue *new_fifo_queue = kmalloc(sizeof(fifo_queue));
    new_fifo_queue->front = NULL;
    new_fifo_queue->rear = NULL;
    return new_fifo_queue;
}

node_t *create_node(void *data)
{
    node_t *new_node = (node_t *)kmalloc(sizeof(node_t));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void enqueue(fifo_queue *queue, void *data)
{
    node_t *new_node = create_node(data);

    if (queue->rear == NULL)
    {
        queue->front = queue->rear = new_node;
        return;
    }

    queue->rear->next = new_node;
    queue->rear = new_node;
}

void *dequeue(fifo_queue *queue)
{

    if (queue->front == NULL)
        return NULL;

    node_t *ret_node = queue->front;
    void *data = ret_node->data;

    queue->front = queue->front->next;

    if (queue->front == NULL)
        queue->rear = NULL;

    kfree(ret_node);
    return data;
}