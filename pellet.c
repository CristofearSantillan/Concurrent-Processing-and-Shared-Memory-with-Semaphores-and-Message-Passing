
#include "global_memory.h"

void terminate(int);

int position; // keeps track of the pellet position in the 2d array

int main(int argc, char* argv[]){

    file = fopen("output.txt", "a");

    shared_memory();

    signal(SIGTERM, terminate); // once a SIGTERM is received then terminate handles the responds

    signal(SIGINT, terminate); // handles the ^C interrupt

    srand(time(NULL));

    int row, column;

    semop(sem_id, &p, 1);
    
    do{
        row = rand() % (rows-3);
        column = rand() % (columns-1);
        position = (row * columns) + column;

    }while(matrix[position] == 'P');

    matrix[position] = 'P'; // Pellet spawns at a random location in the 2d array

    semop(sem_id, &v, 1);

    while(1){

        usleep(1500000);

        semop(sem_id, &p, 1);

        matrix[position] = '-';

        position += columns;

        if(matrix[position] == 'F'){ // checks if the next position is the fish
            terminate(-1);
        }else if(position >= (rows-1)*columns){ // checks if the pellet is at the last row of the 2d array
            terminate(-2);
        }

        matrix[position] = 'P';

        //semop(sem_id, &v, 1);

        printf("\n");
        Grid();

        semop(sem_id, &v, 1);

    }

    return 0;

}

/* The pellet deallocates the shared memory and prints out their identification information */
void terminate(int sn){

    usleep(500000);
    if(sn == -1){
        fprintf(file, "\nPellet Eaten by Fish");
        fprintf(stderr, "\nPellet Eaten by Fish");
    }else if(sn == -2){
        fprintf(file, "\nPellet Missed by Fish");
        fprintf(stderr, "\nPellet Missed by Fish");
    }
    fprintf(file, "\nPellet ID: %d \nPellet Position: %d \n", getpid(), position);
    fprintf(stderr, "\nPellet ID: %d \nPellet Position: %d \n", getpid(), position);
    shmdt(matrix);
    fclose(file);
    exit(sn);

}