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


pthread_mutex_t mutex;
pthread_cond_t not_full;
pthread_cond_t not_empty;

// Global variables
int purchase_cost[5] = {2, 5, 15, 25, 100};
int sell_price[5] = {3, 10, 20, 40, 125};

int number_of_operations;
queue *buffer;
struct element* operator;
int op_number;
int profits = 0;
int product_stock[5] = {0};
int insert_count; // Counter for tracking the number of operations inserted into the buffer
int pop_count;    // Counter for tracking the number of operations consumed from the buffer   // Array to store operation strings read from file



// Structure to represent an operation
struct element;

// Producer function
void *producer(void *param) {
    while (insert_count < number_of_operations) {
        pthread_mutex_lock(&mutex);
        struct element current_operation;
        while (queue_full(buffer)) {
            pthread_cond_wait(&not_full, &mutex); // Wait while buffer is full
        }
        
        // Extract operation details from the operation string
        current_operation = operator[insert_count];
        // Put the operation into the buffer
        queue_put(buffer, &current_operation);
        insert_count++;
        pthread_cond_signal(&not_empty); // Signal that buffer is not empty
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

// Consumer function
void *consumer(void *param) {
    while (pop_count < number_of_operations) {
        pthread_mutex_lock(&mutex);
        struct element current_operation;
        while (queue_empty(buffer)) {
            pthread_cond_wait(&not_empty, &mutex); // Wait while buffer is empty
        }
        
        current_operation = *queue_get(buffer); // Get an operation from the buffer
        if (current_operation.op == 0) {        // Purchase operation
            profits -= purchase_cost[current_operation.product_id - 1] * current_operation.units;
            product_stock[current_operation.product_id - 1] += current_operation.units;
        } else if (current_operation.op == 1) { // Sale operation
            profits += sell_price[current_operation.product_id - 1] * current_operation.units;
            product_stock[current_operation.product_id - 1] -= current_operation.units;
        }
        pop_count++;
        pthread_cond_signal(&not_full); // Signal that buffer is not full
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

int main(int argc, const char *argv[]) {
    if (argc < 4) {
        perror("Not enough arguments");
        return -1;
    }
    int buffsize = atoi(argv[4]);
    buffer = queue_init(buffsize);
    

    int n_producers = atoi(argv[2]);
    int n_consumers = atoi(argv[2]);
    FILE *data_file = fopen(argv[1], "r");
    if (n_producers < 0 || n_consumers < 0) {
        perror("Wrong arguments");
        return -1;
    }
    pthread_t ths[n_producers + n_consumers];

    if (fscanf(data_file, "%d\n", &number_of_operations) < 0) {
        perror("Error when extracting data from the file");
        return -1;
    }

    if (n_producers > number_of_operations || n_consumers > number_of_operations) {
        perror("Too many threads");
        return -1;
    }

    operator = (struct element*)malloc(sizeof(struct element)*number_of_operations);
    char aux[20];
    for (int i = 0; i < number_of_operations; i++) {
        fscanf(data_file, "%d %s %d", &operator[i].product_id, aux, &operator[i].units); // Read operation strings from file
        if (strcmp(aux, "PURCHASE")==0){
            operator[i].op = 0;
        }else if (strcmp(aux, "SALE")==0){
            operator[i].op = 1;
        }
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    for (int i = 0; i < n_producers; i++) {
        pthread_create(&ths[i], NULL, producer, NULL); // Create producer threads
    }

    for (int i = 0; i < n_consumers; i++) {
        pthread_create(&ths[i + n_producers], NULL, consumer, NULL); // Create consumer threads
    }

    for (int i = 0; i < (n_producers + n_consumers); i++) {
        pthread_join(ths[i], NULL); // Wait for all threads to finish
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    free(operator);
    fclose(data_file);

    // Output
    printf("Total: %d euros\n", profits);
    printf("Stock:\n");
    printf("  Product 1: %d\n", product_stock[0]);
    printf("  Product 2: %d\n", product_stock[1]);
    printf("  Product 3: %d\n", product_stock[2]);
    printf("  Product 4: %d\n", product_stock[3]);
    printf("  Product 5: %d\n", product_stock[4]);

    queue_destroy(buffer);
    return 0;
} 

