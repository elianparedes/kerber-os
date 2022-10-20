#ifndef _FIFO_QUEUE_H_
#define _FIFO_QUEUE_H_

typedef struct fifo_queue * fifo_queue_ptr;
typedef struct node  * node_ptr;

fifo_queue_ptr new_fifo_queue();
node_ptr create_node();
void enqueue(fifo_queue_ptr queue, void *data);
void *dequeue(fifo_queue_ptr queue);

#endif