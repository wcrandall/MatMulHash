#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <omp.h> 

// function prototype for professors hash function 
unsigned long hash(char *str);

// function to find out what the encoded string passed in is 
char *crack( long encoded)
{
	int lettersInAbcs = 26; 
	const char* theAbcs = "abcdefghijklmnopqrstuvwxyz";
	char * answer = malloc(5);
	int found = 0;
	omp_set_num_threads(24);  
	#pragma omp parallel for collapse(4) schedule(dynamic) 
	for (int i = 0; i < lettersInAbcs; i++)
	{
		for(int j = 0; j < lettersInAbcs; j++)
		{
			for(int k = 0; k < lettersInAbcs; k++)
			{
				for(int l = 0; l < lettersInAbcs; l++)
				{
					if(found == 0) 
					{
						char *currentString = malloc(5); 
						currentString[0] = theAbcs[i];
						currentString[1] = theAbcs[j];
						currentString[2] = theAbcs[k]; 
						currentString[3] = theAbcs[l]; 
						long compareToEncoded = hash(currentString);
						if(compareToEncoded == encoded)
						{
							answer = currentString; 
							found = 1;
						}
					} 
				}
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
	char *theString = crack(value); 
	
	// printing out the found string 
	printf("the word is %s\n", theString); 


	return 0; 
}



