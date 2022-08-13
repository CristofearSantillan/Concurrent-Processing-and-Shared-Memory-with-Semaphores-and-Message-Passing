
#include "global_memory.h"

pid_t fish; // process id of fish
pid_t pellet; // process if of pellet

int processes_count = 2; // to make sure there is no more than twenty processes in the system; by default the swim_mill and fish is running

void createGrid();
void terminate(int);
void decrementCounter();
void endGame(int);

int main(int argc, char* argv[]){

    file = fopen("output.txt", "a");

    alarm(30); // the 30 second timer

    shared_memory(); // initiate the shared memory

    createGrid(); // initialize the 2D array to default values

    signal(SIGINT, terminate); // this handles the ^C from the user

    signal(SIGCHLD, decrementCounter); // handles the decrementation of the processes counter when a child dies

    signal(SIGALRM, terminate); // this handles the 30 seconds timer to kill everything and itself

    signal(SIGTERM, endGame); // this handles the termination of the main program

    fish = fork(); // creates a child

    if(fish < 0){ // if fork output is negative then the creation failed
        fprintf(stderr, "Fish creation failed!\n");
        exit(0);
    } else if(fish == 0){ // if the fork output is zero then the child was created successful and the current process is the child
        execv("fish", NULL); // runs the fish.c file
        fprintf(stderr, "Fish execution failed!\n");
        exit(0);
    }

    srand(time(NULL));

    while(1){

        if(processes_count > 20){ // checks for more than twenty processes
            wait(NULL); // waits for at least one of the child to die
        }

        usleep((rand() % (4000001)) + 800000);

        pellet = fork(); // creates a child

        processes_count++;

        //semop(sem_id, &p, 1);

        if(pellet < 0){
            fprintf(stderr, "Pellet creation failed!\n");
            exit(0);
        }else if(pellet == 0){
            execv("pellet", NULL); // runs the pellet.c file
            fprintf(stderr, "Pellet execution failed!\n");
            exit(0);
        }

        //semop(sem_id, &v, 1);
        
        printf("\n");
        //Grid(); // prints the 2d array each time both fish and pellets move

    }

    terminate(-1);

    return 0;

}

/* Initializes the 2D array to default value of '-' */
void createGrid(){

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            matrix[i*columns + j] = '-';
        }
    }

}

/* First terminates all running process and itself at then end. In between, the shared memory is detached from memory and destroyed */
void terminate(int sn){

    if(sn != SIGALRM){
        fprintf(file, "\nInterrupt is received!\n");
        fprintf(stderr, "\nInterrupt is received!\n");
    }else{
        fprintf(file, "\nGame is Over!\n");
        fprintf(stderr, "\nGame is Over!\n");
    }
    killpg(getpid(), SIGTERM); // this kills the group of processes and sends a signal to all of them

}

/* Decrements the process counter and prints the ID of a pellet after dying */
void decrementCounter(){

    processes_count--;

}

/* Waits for the rest of the processes to die and main program terminates after deallocating and destroying the shared memory */
void endGame(int sn){

    while(wait(NULL) > 0); // waits for all the children to die
    shmdt(matrix); // detachtes the shared memory segment at a specific location
    shmctl(matrix_id, IPC_RMID, NULL); // destroys the shared memory but you must detach first
    semctl(sem_id, 0, IPC_RMID);
    fclose(file);
    exit(0);

}
