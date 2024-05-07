//SSOO-P3 23/24

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>
#include <pthread.h>
#include "queue.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// Global variables
int number_of_operations;
queue *buffer;
int op_number;
int profits = 0;
int product_stock [5] = {0};
int operation_count;
char **op_list;
struct element

pthread_mutex_t mutex;
pthread_cond_t not_full; 
pthread_cond_t not_empty; 

// Producer function
void *producer(void *param){
  while(operation_count<number_of_operations) {
    pthread_mutex_lock(&mutex);
    while(queue_full(buffer)){
      pthread_cond_wait(&not_full, &mutex);
    }
    struct element current_operation;
    sscanf(op_list[operation_count], "%d %s %d", current_operation.product_id, current_operation.op, current_operation.units);
    
    queue_put(buffer, current_operation);
    operation_count++;
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(0);
}


void *consumer(void *param){
  
}



int main (int argc, const char * argv[])
{
  if (argc!=4){
    perror("Not enough arguments");
    return -1;
  }
  int buffsize = atoi(argv[4]);
  queue *buffer = queue_init(buffsize);

  int n_producers = atoi(argv[2]);
  int n_consumers = atoi(argv[2]);
  int data_file;
  if (n_producers<0||n_consumers<0){
    perror("Wrong arguments");
    return -1;
  }
  pthread_t ths[n_producers+ n_consumers];

  if ((data_file = open(argv[1],O_RDONLY,0644))<0){
    perror("Error opening the file");
    return -1;
  }

  if (fscanf(data_file, "%d\n", &number_of_operations) < 0) {
      perror("Error when extracting data from the file");
      return -1;
  }

  if(n_producers>number_of_operations||n_consumers>number_of_operations){
    perror("Too much threads");
    return -1;
  }

  op_list= (char**)malloc(number_of_operations * sizeof(char *));
  for (i=0; i<number_of_operations; i++){
    op_list[i]=(char*)malloc(64 * sizeof(char));
  }

  for(int i = 0; i<number_of_operations, i++) {
    fgets(op_list[i], 100, fd);
  }


  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&not_full, NULL);
  pthread_cond_init(&not_empty,NULL);

  for (int i = 0; i<n_producers;i++){
    pthread_create(&ths[i], NULL, producer, );
  }

  for (int i = 0; i<n_consumers, i++){
    pthread_create(&ths[i+n_producers], NULL, consumer, );
  }

  for (int i =0; i<(n_producers+ n_consumers)){
    pthread_join(ths[i],NULL);
  }
  
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&not_empty);
  pthread_cond_destroy(&not_full);

  free(op_list);
  fclose(data_file);
  // Output
  printf("Total: %d euros\n", profits);
  printf("Stock:\n");
  printf("  Product 1: %d\n", product_stock[0]);
  printf("  Product 2: %d\n", product_stock[1]);
  printf("  Product 3: %d\n", product_stock[2]);
  printf("  Product 4: %d\n", product_stock[3]);
  printf("  Product 5: %d\n", product_stock[4]);

  return 0;
}
