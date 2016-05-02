/****************************************************************
*  Marco Lugo                                           		*
*  20291903                                             		*
*  3/30/2015                                            		*
*                                                       		*
*	This program is the second phase to write pass one of a		*
*   two-pass assembler for the SIC assembler. Pass one will		*
* 	read each line of the source file and this pass will		*
* 	create the symbol table. The intermediate file will be		*
* 	created for pass two.										*
****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

int main()
{
	
	SICInit();
	
     // char arrays for input, command, and parameters
	char line[80], comm[20], p1[10], p2[10], extra[20];

    // Display welcome message
    welcomeMessage();

	do{
		
        // These functions clear command and parameters
        clear(comm, 20);
        clear(p1, 10);
        clear(p2, 10);
		clear(extra,20);

		// Prompt user for command line
		printf("~command>: ");
		s_gets(line, 80);
		
		
		// Blank lines prompts for user input
		while ( line[0] == '\0' )
		{
			printf("~command>: ");
			s_gets(line, 80);
		}

		// Split up the line into the command and parameters
		split(line, comm, p1, p2, extra);

		// Check for extra parameters
		if (extra[0] != '\0')
			printf("Error: Too many parameters.\n");
		else
			// This function authenticates commands and parameters
            authenticator(comm, p1, p2);


	}while(!strcmp(comm,"exit") == 0 || p1[0] != '\0');
        printf("Thank you! Bye~\n");

	return 0;
} // end of main

