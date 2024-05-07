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


queue *q;
pthread_mutex_t mutex;
pthread_mutex_t producers_mutex;
pthread_mutex_t consumers_mutex; 
pthread_cond_t not_full; 
pthread_cond_t not_empty; 
int number_of_operations;

char **op_list

void producer();
void consumer();

int main (int argc, const char * argv[])
{
  if (argc!=4){
    perror("Not enough arguments");
    return -1;
  }
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&producers_mutex, NULL);
  pthread_mutex_init(&consumers_mutex, NULL);
  pthread_cond_init(&not_full, NULL);
  pthread_cond_init(&not_empty,NULL);

  int profits = 0;
  int product_stock [5] = {0};
  int n_producers = atoi(argv[2]);
  int n_consumers = atoi(argv[2]);
  int data_file;
  if (n_producers<0||n_consumers<0){
    perror("Wrong arguments");
    return -1;
  }
  int ths[n_producers+ n_consumers];

  if ((data_file = open(argv[1],O_RDONLY,0644))<0){
    perror("Error opening the file");
    return -1;
  }

  if (fscanf(data_file, "%d\n", &number_of_operations) < 0) {
      perror("Error when extracting data from the file");
      return -1;
  }
  char str[100];
  while (fgets(op_list[i], 100, data_file)) {

  }

  
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
