
#include "global_memory.h"

void terminate(int);
int closestPellet();

int position;

int main(int argc, char* argv[]){

    file = fopen("output.txt", "a");

    shared_memory();

    signal(SIGTERM, terminate); // this handles the signal SIGTERM to terminate the fish process

    signal(SIGINT, terminate); // handles the ^C interrupt

    int mid = ((rows-1)*columns) + (int)(columns/2);

    position = mid;

    matrix[position] = 'F'; // the fish will always spawn in the middle of the stream

    int pelletPosition;
    int temp; // for reducing redundant operations

    while(1){

        usleep(1500000);

        semop(sem_id, &p, 1);

        pelletPosition = closestPellet() % columns;

        //semop(sem_id, &p, 1);

        if(pelletPosition != -1){
            temp = position%columns;
            if(pelletPosition != temp){ // checks if it DOES need to move left or right
                matrix[position] = '-';
                if( pelletPosition > temp){ // moves to the right depending on the position of the fish
                    matrix[(position++)+1] = 'F';
                    
                }else if (pelletPosition < temp){ // moves to the left depending on the position of the fish
                    matrix[(position--)-1] = 'F'; 
                }
            }
        }else {
            if(position != mid){ // makes the fish go back to the middle of the stream after consumption or miss
                matrix[position] = '-';
                position = mid;
                matrix[position] = 'F';
            }
        }

        //semop(sem_id, &v, 1);

        printf("\n");
        Grid();

        semop(sem_id, &v, 1);

    }

    return 0;

}

/* Find the closest pellet in the 2d array by starting at end of the array */
int closestPellet(){

    //semop(sem_id, &p, 1);
    int pelletPosition = -1;

    for(int i = rows-1; i >= 0; i--){
        for(int j = columns-1; j >= 0;j--){
            if( matrix[i*columns + j] == 'P'){pelletPosition = i*columns + j; break;}
        }
    }
    //semop(sem_id, &v, 1);

    return pelletPosition;

}

/* The fish first prints its identification information then deallocates the shared memory */
void terminate(int sn){
    
    usleep(500000);
    fprintf(file, "\nFish ID: %d \nFish Position: %d \n", getpid(), position);
    fprintf(stderr, "\nFish ID: %d \nFish Position: %d \n", getpid(), position);
    shmdt(matrix);
    fclose(file);
    exit(sn);

}