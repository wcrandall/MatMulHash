
#include <stdio.h>

#include <unistd.h>

#include <string.h>

#include <mpi.h>

#include <stdlib.h>

#include <math.h>



// function prototype for professors hash function

unsigned long hash(char *str);



// function to find out what the endcoded string passed in it

void crack( long encoded, int size, int lettersInAbcs, char* theAbcs, int my_rank, int world_size)

{
    int found_rank = 0; 

    // creating value to store answer

    char *answer = malloc(size+1);

    // once the value is found this will be set to one

    // then the program will skip over the code

    // that create words, hashes, and compares the hashes

    int found = 0;



    for (long i = my_rank; i < (long)pow(lettersInAbcs,size); i+=world_size)

    {

        // if the word is found skip over creating a word, hashing it and

        // comparing it to the given hash

        if (found == 0)

        {

            // making the variable to store guesses in

            char * guess = malloc(size+1);

            // creating a word

            for ( int j = 0; j < size; j++)

            {

                // finding letter index

                long letterIndex = i / (unsigned long)pow(lettersInAbcs, j);

                letterIndex = letterIndex % lettersInAbcs;

                // putting letter into guess

                guess[j] = theAbcs[letterIndex];



            }

            // finding the hash of current guess

            unsigned long compareToEncoded = hash(guess);



            // if the current guess has a hash equal to the

            // passed in encoded value set answer to guess and found to 1

            if (compareToEncoded == encoded)

            {

                answer = guess;

                found = 1;

		found_rank = my_rank; 

		
                if ( my_rank != 0)

                    MPI_Send(answer, strlen(answer)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

            }

	   
		
	    
            free(guess);
	    // MPI_Barrier(MPI_COMM_WORLD); 
	    MPI_Bcast(&found, 1, MPI_INT,found_rank, MPI_COMM_WORLD);
	    
	    
	    
	   
            if(found == 1)
	    {
		printf("%d this\n", found);
		break; 
	    }

            // free guess to avoid memory leaks if encoded doesn't equal

            // guesses hash

//          else

//          {


//          }

        }

    }

}



int main(int argc, char *argv[])

{





    // checking to see if the right amount of cmd line args are given

    if ( argc != 3)

    {

        // send a message to the user and exit

        printf("you must supply only two command line arguments\n");

        exit(0);

    }

    MPI_Init(NULL, NULL);

    int my_rank;

    int world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);


    char* theString;



    int lettersInAbcs = 26;

    double t1, t2;

    char* theAbcs = "abcdefghijklmnopqrstuvwzyz";



    // pointer to null for strtol function

    char *endPtr;



    // finding the string with the crack function

    long value = strtol(argv[1],&endPtr,10);



    long size = strtol(argv[2],&endPtr,10);










    crack(value, size, lettersInAbcs, theAbcs, my_rank, world_size);



    // printing out the found string

    if( my_rank == 0)

    {
	theString = malloc(size+1); 
	 
        MPI_Recv(theString, size+1, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("the word is %s\n", theString);

        t2 = MPI_Wtime();

        printf("Time: %1.8lf\n", t2-t1);



    }

    free(theString);

    MPI_Finalize(); 

    return 0;



}


