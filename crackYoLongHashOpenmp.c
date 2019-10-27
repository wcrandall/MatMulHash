#include <stdio.h> 
#include <unistd.h> 
#include <string.h>
#include <omp.h> 
#include <stdlib.h>
#include <math.h> 
#define SIZE 8



// function prototype for professors hash function 
unsigned long hash(char *str);
   
// function to find out what the encoded string passed in is 
char *crack( long encoded, int size)
{
    
    int lettersInAbcs = 26; 
    const char* theAbcs = "abcdefghijklmnopqrstuvwxyz";
    // creating value to store answer 
    char * answer = malloc(size+1);
    //once the value is found this will be set to one
    // then the program will skip over the code
    // that creates words, hashes, and compares the hashes
    int found = 0;

    // if openmp not included in compilation
    // run without omp_set_num_threads(24); 
#ifdef _OPENMP
    omp_set_num_threads(24);
#endif

    // a parallel for loop that cycles through every possible letter combo
    // for the given word size
#pragma omp parallel for
    for (long i = 0; i < (long)pow(lettersInAbcs,size); i++)
    {
	// if the word is founnd skip over creating a word, hashing it, and comparing
	// it to the given hash 
	if(found == 0) 
	{
	    // making variable to store guesses in 
	    char * guess = malloc(size +1);
	    // creating a word
	    for (int j = 0; j < size; j++)
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
	    if(compareToEncoded == encoded)
	    {
		answer = guess;
		found = 1; 
	    }
	    // free guess to avoid memory leaks if encoded doesn't equal guesses hash
	    else
	    {
		free(guess); 
	    }
	}
    }
    return answer;
    
}

int main(int argc, char *argv[])
{
    
   
    
    // checking to see if the right amount of command line arguments was given  
    if (argc != 2) 
    {
	// send a message to the user and exit if they were not
	printf("you must supply only two command line arguments\n"); 
	exit(0); 
    }


    
    // pointer to null for strtol function
    char *endPtr;
    
    // converting userinput to long 
    long value =strtol(argv[1],&endPtr,10);
    // finding the string with the crack function 
    char *theString = crack(value, SIZE); 
    
    // printing out the found string 
    printf("the word is %s\n", theString); 

    
    free(theString);
    
    return 0; 
}



