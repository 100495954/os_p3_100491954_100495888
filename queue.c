//SSOO-P3 23/24

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"

//To create a queue
queue* queue_init(int size)
{
  queue * q = (queue *)malloc(sizeof(queue));

  q->max_size = size;
  q->curr_size = 0;
  q->head = 0;
  q->tail = -1;

  pthread_mutex_init(&q->mutex, NULL);
  pthread_cond_init(&q->full, NULL);
  pthread_cond_init(&q->empty, NULL);

  return q;
}

// To Enqueue an element
int queue_put(queue *q, struct element *x)
{
  pthread_mutex_lock(&q->mutex);
  while (q->curr_size >= q->max_size){ // checking if the buffer is full
    pthread_cond_wait(&q->full, &q->mutex);
  }
  q->tail = (q->tail + 1) % q->max_size; // % is used to go to the beginning of the buffer once the tail reaches the last position of it
  q->buffer[q->tail] = x; 
  q->curr_size++;
  pthread_cond_signal(&q->empty);
  pthread_mutex_unlock(&q->mutex);

  
  return 0;
}

// To Dequeue an element.
struct element* queue_get(queue *q)
{
  struct element* element;
  pthread_mutex_lock(&q->mutex);
  while (q->curr_size <= 0) {
    pthread_cond_wait(&q->empty, &q->mutex);
  }
  element = q->buffer[q->head]; 
  q->head = (q->head+1) % q->max_size;
  q->curr_size--;
  pthread_cond_signal(&q->full);
  pthread_mutex_unlock(&q->mutex);
  return element;
}

//To check queue state
int queue_empty(queue *q)
{ 
  if (q->curr_size == 0){
    return 1;
  } 
  return 0;
}

int queue_full(queue *q)
{
  if (q->curr_size == q->max_size){
    return 1;
  } 
  return 0;
}

//To destroy the queue and free the resources
int queue_destroy(queue *q)
{
  pthread_mutex_destroy(&q->mutex);
  pthread_cond_destroy(&q->full);
  pthread_cond_destroy(&q->empty);
  free(q);

  return 0;
}
