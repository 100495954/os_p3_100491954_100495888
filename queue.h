//SSOO-P3 23/24

#ifndef HEADER_FILE
#define HEADER_FILE
#include <pthread.h>

#define MAX_QUEUE_SIZE 50 //Based on the info of file.txt


struct element {
  int product_id; //Product identifier
  int op;         //Operation
  int units;      //Product units
};

typedef struct queue {
  struct element *buffer[MAX_QUEUE_SIZE];
  int head, tail;
  int curr_size; //It is the current number of elements in the queue
  int max_size; //It is the maximum number of elements in the queue
  
}queue;

queue* queue_init (int size);
int queue_destroy (queue *q);
int queue_put (queue *q, struct element* elem);
struct element * queue_get(queue *q);
int queue_empty (queue *q);
int queue_full(queue *q);

#endif
