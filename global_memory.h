#ifndef GLOBAL_MEMORY_H_
#define GLOBAL_MEMORY_H_

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

int rows = 8;
int columns = 8;
int matrix_id; // return share memory id from shmget()
int *matrix; // points to shared memory segment by shmat() function

int sem_id; 
struct sembuf p = {0, -1, 0};
struct sembuf v = {0, +1, 0};

FILE *file; // file pointer to print out the events to a .txt

/* Creating the shared memory for the processes */
void shared_memory(){

    key_t k = ftok("swim_mill.c", 'a'); // creates a key from an existing file and id
    key_t s = ftok("swim_mill.c", 'z');
    if(k == (key_t) -1 || s == (key_t) -1){
        fprintf(stderr, "Initializing Key Error!\n");
        exit(0);
    }
    matrix_id = shmget(k, sizeof(int) * rows * columns, 0666|IPC_CREAT); // create a new shared memory segment or referes to an existing shared memory
    sem_id = semget(s, 1, 0666|IPC_CREAT);
    if(matrix_id == -1 || sem_id == -1){
        fprintf(stderr, "Initializing ID Error!\n");
        exit(0);
    }
    matrix = shmat(matrix_id, NULL, 0); // attach the shared memory to the memory space of the current process
    if(matrix == (void *) -1){
        fprintf(stderr, "Obtaining Shared Memory Error!\n");
        exit(0);
    }
    if(semctl(sem_id, 0, SETVAL, 1) == -1){
        fprintf(stderr, "Initializing Semaphore Error!\n");
        exit(0);
    }

}

/*  Displays the 2D array to the console output */
void Grid(){

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            printf("%c ", matrix[i*columns + j]);
        }
        printf("\n");
    }

}

#endif