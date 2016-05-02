#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "sicengine.c"

ADDRESS a;

// Symbol Table
typedef struct {
        char name[6];
        int address;
} Symtab;


// Opcode Table
typedef struct {
        char name[6];
        int code;
} Optab;
  
  Optab opcode [31] =
{ {"ADD\0", 24}, {"AND\0", 88},{"BYTE\0", 252}, {"COMP\0", 40}, {"DIV\0", 36}, {"END\0", 251}, {"J\0", 60},
  {"JEQ\0", 48}, {"JGT\0", 52}, {"JLT\0", 56}, {"JSUB\0", 72}, {"LDA\0", 00},
  {"LDCH\0", 80}, {"LDL\0", 8}, {"LDX\0", 4}, {"MUL\0", 32}, {"OR\0", 68},{"RD\0", 216}, {"RESB\0", 254},{"RESW\0", 255},
  {"RSUB\0", 76}, {"STA\0", 12},{"START\0", 250}, {"STCH\0", 84}, {"STL\0", 20},
  {"STX\0", 16}, {"SUB\0", 28}, {"TD\0", 224}, {"TIX\0", 44}, {"WD\0", 220},{"WORD\0", 253} };


/* Prototypes for the functions */
char * s_gets(char * st, int n);
void clear(char[], int n);
void authenticator(char [], char [], char []);
void displayCommnads();
void welcomeMessage();

/* Directive functions */
void assemble(char []);
ADDRESS load(char []);

/* Directive dependencies */
void parameterFix(char [], char [], char []);
_Bool symtabSearch(char [], Symtab [], int, int, int*);
_Bool optabSearch(char [], int *, int );
int errorChecker(char [], char [], char [], int);
_Bool byteOperandCheck(char [], int *);
_Bool hexCheck( char [] );


/********************************************************
*               authenticator()                         *
*                                                       *
*  This fucntion takes three char arrays as paramters.  *
*  Using embedded if/else statements it determines      *
*  which commands require paramaters as well as which   *
*  don't.                                               *
********************************************************/
void authenticator(char comm[], char p1[], char p2[])
{
        /* If no paramaters needed only parameter 1 is checked.
           If only one parameter is needed both parameters are checked.
           If two parameters are needed both are checked.
        */
        
        if(!strcmp(comm,"load"))
                 {
                        if(p1[0] == '\0')
                                printf("Error: Parameter needed.\n");
                        else if(p2[0] != '\0')
                                printf("Error: Second parameter not required.\n");
                        else {
                                a = load(p1);
							}
                }
        else if(!strcmp(comm,"execute"))
                {
                        if(p1[0] != '\0')
                                printf("Error: No parameters needed.\n");
                        else {
							
								printf("addr: %d\n", a);
                                SICRun(&a,0);
						}
                }
        else if(!strcmp(comm,"debug"))
                {
                        if(p1[0] != '\0')
                                printf("Error: No parameters needed.\n");
                        else
                                printf("Command is %s.\n", comm);
                }
        else if(!strcmp(comm,"dump"))
                {
                        if(p1[0] == '\0')
                                printf("Error: First parameter incorrect.\n");
                        else if(p2[0] == '\0')
                                printf("Error: Second parameter incorrect.\n");
                        else
                                printf("Command is %s.\n", comm);
                }
        else if(!strcmp(comm,"help"))
                {
                        if(p1[0] != '\0')
                                printf("Error: No parameters needed.\n");
                        else
                                displayCommnads();
                }
        else if(!strcmp(comm,"assemble"))
                {
                        if(p1[0] == '\0')
                                printf("Error: Parameter needed.\n");
                        else if(p2[0] != '\0')
                                printf("Error: Second parameter not required.\n");
                        else{
                               assemble(p1);
                        }
                }
        else if(!strcmp(comm,"directory"))
                system("ls");
        else if(!strcmp(comm,"exit"))
                 {
                        if(p1[0] != '\0')
                                printf("Error: No  needed.\n");
                }
        else
        {
               printf("Error: %s is not a valid command.\n", comm); // ¯\_(ツ)_/¯
               displayCommnads();
        }
}

_Bool byteOperandCheck(char operand[], int *locctr)
{
	int len;
	
	len = strlen(operand) - 1;
	
	int strOperand = 2;
	int lstOperand = len;
	
	if (operand[0] != 'C' && operand[0] != 'X' && operand[1] != '\'' &&  operand[len] != '\'')
	{
		return 1;
	}
	else if ( operand[0] == 'C' )
	{
		if ( (lstOperand - strOperand) > 30)
			return 1;
	
		//fprintf(out, "%x\n", *locctr);
			
		*locctr += (lstOperand - strOperand);	
		
		return 0;
	}
	else 
	{
		if ( (lstOperand - strOperand)%2 != 0 )
			return 1;
			
		*locctr += (lstOperand - strOperand) / 2;
		return 0;
	}
	
}

/********************************************************
*                       clear()                         *
*                                                       *
*  This fucntion takes any given char array and fills   * 
*  it with null characters to erase previous inputs.    *
********************************************************/
void clear( char string[], int n)
{
        int i;
        for (i = 0; i < n; i++)
                string[i] = '\0';

}

/********************************************************
*                  displayCommnads()                    *
*                                                       *
*  This fucntion displays all available commands and    *
*  parameters.                                          *
********************************************************/
void displayCommnads()
{

    printf("\nload filename: Loads the specified file.\n\n");
	printf(	"execute: Executes the program that was previously loaded in memory.\n\n");
	printf(	"debug: Executes in debug mode.\n\n");
	printf(	"dump start end: Calls the dump function, passing the values of start and end.\n\n");
	printf(	"help: Prints out a list of available commands.\n\n");
	printf(	"assemble filename: Assembles an SIC assembly language program into a load module and store it in a file.\n\n");
	printf(	"directory: Lists the files stored in the current directory.\n\n");
	printf(	"exit: Exit from the simulator.\n\n");

}

/********************************************************
*                     s_gets()                          *
*                                                       *
*  This fucntion takes in a char array and array length *
*  to get rid of any '\n'.								*
********************************************************/
char * s_gets(char * st, int n)
{
	char * ret_val;
	char * find;

	ret_val = fgets(st, n, stdin);
	if (ret_val)
	{
		find = strchr(st, '\n');		// Look for newline
		if(find)						// if the address is not NULL,
			*find = '\0';				// place a null character there
		else
			while (getchar() != '\n')
				continue;				//dispose of rest of line
	}
	return ret_val;
}

/********************************************************
*                       split()                         *
*                                                       *
*  This fucntion takes in the users input and divides   *
*  into the cammand and two parameters. Any more        *
*  parameters than two are placed in extra char array.  *
********************************************************/
void split( char line[], char comm[], char p1[], char p2[], char extra[])
{

	int i, j;

	i = 0;
	j = 0;

	// Traverses any spaces to the first character
	while ( line[i] == ' ' || line[i] == '\t' )
		i++;

	// Once a character is found copy into comm
	// until another space or null character.
	while( line[i] != '\0')
	{

		comm[j] = line[i];
                
		++j;
		++i;
 
		if (line[i] == ' ' || line[i] == '\0' || line[i] == '\t')
		{
			comm[j] = '\0';
			break;
		}
    
	}

	// Find the next string to be copied into first paramenter.
	while ( line[i] == ' ' || line[i] == '\t')
		i++;

	if ( line[i] != '\0' )
	{
		j = 0;
		while( line[i] != '\0')
		{

			p1[j] = line[i];

			++j;
			++i;

			if (line[i] == ' ' || line[i] == '\0' || line[i] == '\t')
			{
				p1[j] = '\0';
				break;
			}
		}
	}


	// Find the next string to be copied into second paramenter.
	while ( line[i] == ' ' || line[i] == '\t')
		i++;

	if ( line[i] != '\0' )
	{
		j = 0;
		while( line[i] != '\0')
		{

			p2[j] = line[i];

			++j;
			++i;

			if (line[i] == ' ' || line[i] == '\0' || line[i] == '\t')
			{
				p2[j] = '\0';
				break;
			}
		}
	}

	// Find the next string
	while ( line[i] == ' ' || line[i] == '\t')
		i++;

	if ( line[i] != '\0' )
	{
		j = 0;
		while( line[i] != '\0')
		{

			extra[j] = line[i];

			++j;
			++i;

			if ( line[i] == '\0')
			{
				extra[j] = '\0';
				break;
			}
		}

	}

}


/********************************************************
*              welcomeMessage()                         *
*                                                       *
*  This fucntion displays a welcome message to the user *
*  as well as which command to start with.		*
********************************************************/
void welcomeMessage()
{
        static unsigned char asciipic_txt[] = {                                                
        0x20, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x20, 0x5f, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5f, 0x5f, 0x5f,
        0x5f, 0x5f, 0x20, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x20, 0x0a, 0x2f, 0x20,
        0x20, 0x5f, 0x5f, 0x5f, 0x28, 0x5f, 0x29, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7c, 0x20, 0x20, 0x5f, 0x20, 0x20,
        0x2f, 0x20, 0x20, 0x5f, 0x5f, 0x5f, 0x7c, 0x0a, 0x5c, 0x20, 0x60, 0x2d,
        0x2d, 0x2e, 0x20, 0x5f, 0x20, 0x5f, 0x20, 0x5f, 0x5f, 0x20, 0x5f, 0x5f,
        0x5f, 0x20, 0x20, 0x20, 0x7c, 0x20, 0x7c, 0x20, 0x7c, 0x20, 0x5c, 0x20,
        0x60, 0x2d, 0x2d, 0x2e, 0x20, 0x0a, 0x20, 0x60, 0x2d, 0x2d, 0x2e, 0x20,
        0x7c, 0x20, 0x7c, 0x20, 0x27, 0x5f, 0x20, 0x60, 0x20, 0x5f, 0x20, 0x5c,
        0x20, 0x20, 0x7c, 0x20, 0x7c, 0x20, 0x7c, 0x20, 0x7c, 0x60, 0x2d, 0x2d,
        0x2e, 0x20, 0x5c, 0x0a, 0x2f, 0x5c, 0x5f, 0x5f, 0x2f, 0x20, 0x7c, 0x20,
        0x7c, 0x20, 0x7c, 0x20, 0x7c, 0x20, 0x7c, 0x20, 0x7c, 0x20, 0x7c, 0x20,
        0x5c, 0x20, 0x5c, 0x5f, 0x2f, 0x20, 0x2f, 0x5c, 0x5f, 0x5f, 0x2f, 0x20,
        0x2f, 0x0a, 0x5c, 0x5f, 0x5f, 0x5f, 0x5f, 0x2f, 0x7c, 0x5f, 0x7c, 0x5f,
        0x7c, 0x20, 0x7c, 0x5f, 0x7c, 0x20, 0x7c, 0x5f, 0x7c, 0x20, 0x20, 0x5c,
        0x5f, 0x5f, 0x5f, 0x2f, 0x5c, 0x5f, 0x5f, 0x5f, 0x5f, 0x2f, 0x0a

        };

        system("clear");
        printf("%s\n", asciipic_txt);
        printf("Welcome to Sim OS 3.0\n");
        printf("For supported commands type: help\n\n");

} 

// This is the fucntion for the assmble directive

void assemble(char fileName [])
{

	char outName[64];
	char line[256];
	FILE *in, *out;
    int locctr;
    int error = 0;
    int index, opIndex, sybIndex;
	int intOperand = 0;	// Used when the operand is a number
	int programLength = 0;
	
	Symtab symbolTable[500] = {0};
	Symtab specialTable[1] = {0};
	
	// Variables for split
    char label[50], instruction[50], operand[50];
    char * pch;

    // initialize
    label[0], instruction[0], operand[0] = '\0';

	// get file names from user
	strcpy(outName, "intermediate");

	// open input and output files
    out = fopen (outName, "w");

	if ( (in = fopen(fileName, "r")) == NULL  )
	{
		printf("Can't open %s for reading.\n", fileName);
		return;
	}
    else {
                /************************************************
                *                   PASS 1                      *
                ************************************************/
				
                // Traverses any comments
                while (fgets(line, sizeof(line), in) && line[0] == '.' )
					continue;

                // The first line is a special case
                sscanf(line,"%s %s %s %*s", label, instruction, operand);
                
                if ( strcmp(instruction, "START") == 0)
                {
                        // Saving program name and starting address
                        strcpy(specialTable[0].name, label);
                        sscanf(operand, "%X", &specialTable[0].address);

                        // Initialize LOCCTR to starting address
                        sscanf(operand, "%x", &locctr);

                        // Write line to intermediate file
                        fprintf(out, "%s", line);
                        
                        // Error checking
                        error = errorChecker(label, instruction, operand, error);

						fprintf(out, "%d\n", error);
                }
                else
                {
                        locctr = 0;
                }
                
                // Read next line
                fgets(line, sizeof(line), in);
                
                // Split read line
                sscanf(line,"%s %s %s %*s", label, instruction, operand);
                parameterFix(label, instruction, operand);

                index = 0;
             	while ( strcmp(instruction, "END") != 0 )
	        {
						// Get string length to check blank lines
						int lineLength = strlen(line) - 1;

                        // If line is not a comment
                        if(line[0] != '.' && lineLength > 0)
                        {
							// Reinitialize error
							error = 0;
								
                                // Write line to intermediate file
								fprintf(out, "%s", line);

                                // If there is a label in instructions
                                if ( label[0] != '\0')
                                {
                                        // Search SYMTAB for lable
                                        if(symtabSearch(label, symbolTable, 0, index, &sybIndex))
                                                // set error flag for duplicate label
                                                error = error | 1;
                                        else
                                        {
                                                // Inserting label and locctr into SYMTAB
                                                strcpy(symbolTable[index].name, label);
                                                symbolTable[index].address = locctr;
                                        }
                                        
                                        // Update index for SYMTAB
										index ++;
                                
                                } // end if symbol

                                // Search for OPCODE in OPTAB
                                if(optabSearch(instruction, &opIndex, 1))
                                {
									//printf("Within the if: %s %X\n", opcode[opIndex].name,opcode[opIndex].code);
									
									if ( strcmp(instruction, "WORD") == 0){
										
										// Error checking blank operand
										if ( operand[0] == '\0' || !hexCheck(operand))
											error = error | 8;
									
										fprintf(out, "%x\n", locctr);
										locctr += 3;
									}
									else if ( strcmp(instruction, "RESW") == 0) {
										
										// Error checking blank operand
										if ( operand[0] == '\0'|| !hexCheck(operand))
											error = error | 8;
										
										fprintf(out, "%x\n", locctr);
										sscanf(operand, "%d", &intOperand);
										locctr += 3*intOperand;
										
									}
									
									else if ( strcmp(instruction, "RESB") == 0) {
										
										// Error checking blank operand
										if ( operand[0] == '\0'|| !hexCheck(operand))
											error = error | 8;
										
										fprintf(out, "%x\n", locctr);
										sscanf(operand, "%d", &intOperand);
										locctr += intOperand;
										
									}
									
									else if ( strcmp(instruction, "BYTE") == 0) {
										
										fprintf(out, "%x\n", locctr);
										
										// Check operand for BYTE directive
										if ( byteOperandCheck(operand, &locctr))
										{
												error = error | 8;
										}
											
									}
									else {
											// Error checking blank operand (4)
											if ( strcmp(instruction, "RSUB") == 0 )
											{	
												
												if ( operand[0] != '\0')
													error = error | 4;
											}
											else if (operand[0] == '\0')
												error = error | 4;

											fprintf(out, "%X\n", locctr);
											locctr += 3;
																	
									}
									
									
								}
								/*else if( strcmp(instruction, "WORD") == 0)
								{
									// Error checking blank operand
									if ( operand[0] == '\0' || !hexCheck(operand))
										error = error | 8;
									
									fprintf(out, "%x\n", locctr);
									locctr += 3;
								}
								else if ( strcmp(instruction, "RESW") == 0)
								{
									// Error checking blank operand
									if ( operand[0] == '\0'|| !hexCheck(operand))
										error = error | 8;
										
									fprintf(out, "%x\n", locctr);
									sscanf(operand, "%d", &intOperand);
									locctr += 3*intOperand;
								}
								else if ( strcmp(instruction, "RESB") == 0)
								{
									// Error checking blank operand
									if ( operand[0] == '\0'|| !hexCheck(operand))
										error = error | 8;
										
									fprintf(out, "%x\n", locctr);
									sscanf(operand, "%d", &intOperand);
									locctr += intOperand;
								}
								else if ( strcmp(instruction, "BYTE") == 0)
									{
										// Check operand for BYTE directive
										if ( byteOperandCheck(operand, &locctr))
										{
												error = error | 8;
										}
											
										fprintf(out, "%x\n", locctr);
											
									}*/
								else
								{
									// Set error flag for missing instruction (256)
									error = error | 256;
								}
								             
								// Check too many symbols
								if ( index > 499 )
								           error = error | 64;
								       
								// Check for remaining errors
								error = errorChecker(label, instruction, operand, error);  
									
								
								//printf("%s	%X\n", opcode[opIndex].name, opcode[opIndex].code);
									
								// Write to file
								fprintf(out, "%X\n", opcode[opIndex].code);
								fprintf(out, "%s\n", operand);
								fprintf(out, "%d\n", error);
				
								// Clear for next line to be read
								clear(label, 50);
								clear(instruction, 50);
								clear(operand, 50);
									
                        } // end if not a comment
                
                // Read next line and check for end of file
                if (fgets(line, sizeof(line), in) == NULL)
					break;
                
                // Split read line
                sscanf(line,"%s %s %s %*s", label, instruction, operand);
                parameterFix(label, instruction, operand);         
                
	        } // end while not END
	        
	        if ( strcmp(instruction, "END") == 0)
            {
				programLength = locctr - specialTable[0].address;
	        
				error = 0;
	        
				// Write line to intermediate file
				fprintf(out, "%s %s %s\n", label, instruction, operand);
			
				// Retrieve the opcode
				optabSearch(instruction, &opIndex, 3);
			
				if(!symtabSearch(operand, symbolTable, 0, index, &sybIndex))
					error = error | 32;
				
				if ( programLength > 32000)
					error = error | 128;
					
				error = errorChecker(label, instruction, operand, error);
			
				// Write lines to intermediate file
				fprintf(out, "%X\n", locctr);
				fprintf(out, "%X\n", opcode[opIndex].code);
				fprintf(out, "%s\n", operand);
				fprintf(out, "%d\n", error);	
			}
	        
        }// end Pass 1
	
	
	/*
	int k; 
	for ( k = 0; k < index; k++)
		printf("%s		%X\n", symbolTable[k].name, symbolTable[k].address);
	*/
	
	fclose(in);
	fclose(out);
	
	
	//--------------------------------------------------------------------------------
	
	FILE *output;
	char outFile[64];
	
	void readIntermediateLine(Symtab [], char [], char [], char [], char [], int *, int , FILE *);
	_Bool errorInterpreter( int, char [] );
	
	_Bool errorFlag = 0;
	
	// String varibles for input from intermediate file
	char statement[256], strAddress[7], strError[16], strOpcode[4];
	char errorMessage[500];
	
	char temp[10];
	
	//
	char textRecord1[50];
	char textRecord2[256];
	char objCodeLength[5];
	
	// Initialize strings
	temp[0] = '\0';
	textRecord1[0] = '\0';
	textRecord2[0] = '\0';
	objCodeLength[0] = '\0';
	errorMessage[0] = '\0';
	statement[0] = '\0';
	
	// Integer varibles for conversion from string
	int intAddress, intError, intOpcode, symAddress;
	
	int symIndex;
	int objectCodeLength = 0;
	
	clear(label, 50);
    clear(instruction, 50);
	clear(operand, 50);
	clear(fileName, 64);
	clear(outName, 64);
	strcpy(fileName, "intermediate");
	strcpy(outName, "objectfile");
	strcpy(outFile, "listing");
	
	// Open output file
	out = fopen (outName, "w");
	output = fopen(outFile, "w");
	
	if( (in = fopen(fileName, "r")) == NULL){
		
		printf("Can't open %s for reading.\n", fileName);
		return;
	}
	else
	{
		/*
		 *			pass2 
		 */
		
		// Read first line from intermediate file
		fgets(line, sizeof(line), in);
		
		// Print souce line to listing file
		fprintf(output, "%s", line );
		
		// Split read line
        sscanf(line,"%s %s %s %*s", label, instruction, operand);
        parameterFix(label, instruction, operand);
        
        // Search for opcode
        optabSearch(instruction, &opIndex, 2);
        
        // If opcode is start
        if (opcode[opIndex].code == 250)
		{
			// Check for error
			fgets(line, sizeof(line), in);
			sscanf(line, "%d", &error);
			errorFlag = errorInterpreter(error, errorMessage);
			
			if( errorFlag){
				fprintf(output, "%s\n", errorMessage );
			}
						
			
			// Read next line
			readIntermediateLine( symbolTable, statement, strAddress, strOpcode, operand, &error, index, in );
			
			// Check for error
			errorFlag = errorInterpreter(error, errorMessage);	
			
		} // end if opcode start
		
		// Write header record to oject program
		fprintf(out, "H%s%06X%06X\n", specialTable[0].name,specialTable[0].address, programLength );
		
		//printf("name: %s\n", specialTable[0].name);
		
		// Initialize first part of Text record string
		textRecord1[0] = 'T';
		strcat(textRecord1, strAddress);
		
		// Initialize second part of Text record string
		strcpy(textRecord2, strOpcode);
		strcat(textRecord2, operand);
		
		while ( strcmp( strOpcode, "FB") != 0 )
		{
			objectCodeLength = strlen(textRecord2);
			
			readIntermediateLine( symbolTable, statement, strAddress, strOpcode, operand, &error, index, in );
			
			if ( strlen(statement) == 0)
				break;
			
			if ( strcmp(strOpcode, "FB") == 0 )
				fprintf(output, "%-75s\n", statement );
			else
			fprintf(output, "%-75s%s\t%s%s\n", statement, strAddress, strOpcode, operand );
			
			errorFlag = errorInterpreter(error, errorMessage);
			
			if( errorFlag){
				fprintf(output, "%s\n", errorMessage );
			}
			
			// Starts a new Text Record for RESW or RESB
			if ( strcmp(strOpcode, "FF") == 0 || strcmp(strOpcode, "FE") == 0 || (objectCodeLength + 6) > 60 ){
				
				// Get the length in hex
				sprintf(temp, "%02X", objectCodeLength/2);  // convert from int to string
				
				// Attatch length to textrecord1
				strcat(textRecord1, temp);
				
				// Attatch textrecord2 to textrecord1
				strcat(textRecord1, textRecord2);
				
				// Write header record to oject program
				fprintf(out, "%s\n", textRecord1 );
				//printf( "%s\n", textRecord1 );
				
				clear(temp, 10);
				clear(textRecord1, 50);
				clear(textRecord2, 256);
				objectCodeLength = 0;
				
				
				// Start new textrecord
				while ( strcmp(strOpcode, "FF") == 0 || strcmp(strOpcode, "FE") == 0 )
				{
					readIntermediateLine( symbolTable, statement, strAddress, strOpcode, operand, &error, index, in );
					fprintf(output, "%-75s%s\t%s%s\n", statement, strAddress, strOpcode, operand );
					errorFlag = errorInterpreter(error, errorMessage);
					if( errorFlag){
						fprintf(output, "%s\n", errorMessage );
					}
				}
				
				strcpy(textRecord1, "T");
				strcat(textRecord1, strAddress);
				strcpy(textRecord2, strOpcode);
				strcat(textRecord2, operand);     
			
				
			}
			else if( strcmp(strOpcode, "FC") == 0 ) {
				strcat(textRecord2, operand);
			}
			else if ( strcmp(strOpcode, "FB") == 0 ) {
				
				// Get the length in hex
				sprintf(temp, "%02X", objectCodeLength/2);  // convert from int to string
				
				// Attatch length to textrecord1
				strcat(textRecord1, temp);
				
				// Attatch textrecord2 to textrecord1
				strcat(textRecord1, textRecord2);
				
				// Write header record to oject program
				fprintf(out, "%s\n", textRecord1 );
				//printf( "%s\n", textRecord1 );
				
				clear(temp, 10);
				clear(textRecord1, 50);
				clear(textRecord2, 256);
				objectCodeLength = 0;
				
				strcpy(textRecord1, "E");
				strcat(textRecord1, "00");
				strcat(textRecord1, operand);
				fprintf(out, "%s\n", textRecord1 ); 
				//printf( "%s\n", textRecord1 );
				
			}
			else {
				
			
				strcat(textRecord2, strOpcode);
				strcat(textRecord2, operand);
				
			}
		
			
		} // end while
		
		
	}// end Pass 2
	
	
	int k; 
	for ( k = 0; k < index; k++)
		fprintf(output,"%s		%X\n", symbolTable[k].name, symbolTable[k].address);
		
	int r;
	
	if ( errorFlag)	{
		
		r = remove("objectfile");
		
	}
	
	fclose(in);
	fclose(out);
	
	fclose(output);
	
	
	puts("File assembled.");

}

void readIntermediateLine(Symtab symbol[], char statement[], char strAddress[], char strOpcode[], char strOperand[], int *intError, int index, FILE *in ){
	
	char line[256];
	char strTemp[50];
	
	line[0] = '\0';
	strTemp[0] = '\0';
	
	int symAddress, symIndex = 0;
	int temp;
	int errorTemp = 0;
	
	fgets(line, sizeof(line), in);		// Get source statement
	strcpy(statement, line);
	
	fgets(line, sizeof(line), in);		// Get address location
	sscanf(line, "%06X", &temp);
	sprintf(strAddress, "%06X", temp);

	
	fgets(line, sizeof(line), in);		// Get instruction opcode
	sscanf(line, "%02X", &temp);
	sprintf(strOpcode, "%02X", temp);
	
	fgets(line, sizeof(line), in);		// Get operand
	int len = strlen(line);	
	
	line[len - 1] = '\0';	
	
	// instruction is WORD
	if ( strcmp( strOpcode, "FD") == 0 ){
		
		strcpy(strOpcode, "00");
		
		sscanf(line, "%04d", &temp);
		
		sprintf(strOperand, "%04X", temp);
		
	}
	// instruction is BYTE
	else if ( strcmp( strOpcode, "FC") == 0 ){
		
		clear(strOpcode, 50);
		
		strcpy(strOperand, line);
		
		if ( strOperand[0] == 'C' ){
			
			len = strlen(strOperand);
			
			int i;
			int j = 0;
			
			for ( i = 0; i < len; i++){
				
				if ( strOperand[i] != 'C' && strOperand[i] != '\''){
					//printf("str: %c\n", strOperand[i]);
					strTemp[j] = strOperand[i];
					j++;	
				}
			}	
				
			strTemp[j] = '\0';
			
			len = strlen(strTemp);
			
			/*printf("strTemp[0]: %02X\n", strTemp[0]);
			printf("strTemp[1]: %02X\n", strTemp[1]);
			printf("strTemp[2]: %02X\n", strTemp[2]);
			*/
			for (i = 0; i < len; i++){
				
				if ( i == 0 ){
					temp = (int)strTemp[i];
					sprintf(strOperand, "%02X", temp);
				}
				else {
					temp = (int)strTemp[i];
					//printf("temp: %X\n", temp);
					sprintf(line, "%02X", temp);
					strcat(strOperand, line);
				}
			}
			
			
			//printf("%s\n", strOperand);		
			
			//puts("Press enter");
			//int c = getchar();
			
		}
		else {
			
			len = strlen(strOperand);
			
			int i;
			int j = 0;
			
			for ( i = 0; i < len; i++){
				
				if ( strOperand[i] != 'X' && strOperand[i] != '\''){
					//printf("str: %c\n", strOperand[i]);
					strTemp[j] = strOperand[i];
					j++;	
				}
			}	
				
			strTemp[j] = '\0';
			strcpy(strOperand, strTemp);
		}
				
	}
	else if ( line[len - 2] == 'X' && line[len - 3] == ','){
		
		line[len-2] = line[len -3] = '\0';
		
		symtabSearch(line, symbol, 0, index, &symIndex);
		
		temp = symbol[symIndex].address;
		temp += 32768;

		sprintf(strOperand, "%04X", temp);
		
	}
	else if ( line[0] == '\0' ){
		strcpy(strOperand, "0000");
	}
	else if (strcmp(strOpcode, "FF") == 0 || strcmp(strOpcode, "FE") == 0 ){
		
		strcpy(strOperand, "0000");
		clear(strOperand, 50);	
	}
	else {
		
		// Find address for symbol
		if (symtabSearch(line, symbol, 0, index, &symIndex) ){
			symAddress = symbol[symIndex].address;
			sprintf(strOperand, "%X", symAddress);		
		}	
		else {
			
			strcpy(strOperand, "0000");
			errorTemp = 4;
		}
			
	}
	
	
	fgets(line, sizeof(line), in);		// Get error
	sscanf(line, "%d", intError);
	
	*intError = *intError | errorTemp;
	
	len = strlen(statement);
	if (statement[len -1] == '\n')
		statement[len -1] = '\0';
			
	len = strlen(strOpcode);
	if (strOpcode[len -1] == '\n')
		strOpcode[len -1] ='\0';
	
}

_Bool errorInterpreter ( int numberToConvert, char errorMessage[])
{
	if (numberToConvert == 0)
		return 0;
	
	const char baseDigits[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	
	char tempMessage[500];
	tempMessage[0] = '\0';	
	
	char errorBitMask[16];
	errorBitMask[0] = '\0';

	int	convertedNumber[64];
	int	nextDigit, base, index = 0;
	
	base = 2;

	// convert to the indicated base

	do{
		convertedNumber[index] = numberToConvert % base;
		++index;
		numberToConvert = numberToConvert / base;
	} 
	while(numberToConvert != 0);

	// Arrange the bits from LSB ---> MSB
	int i;
	for ( i = 0; i < index; i++){
		nextDigit = convertedNumber[i];
		
		errorBitMask[i] = baseDigits[nextDigit];
	
	}
	
	errorBitMask[i] = '\0';
	
	int len = strlen(errorBitMask);
	
	// Check for errors and assign message
	for (i = 0; i < len; i++)
	{

		if( i == 0 && errorBitMask[0] == '1' ){

			strcat(tempMessage, "Duplicate labels. ");
		}
			
		if( i == 1 && errorBitMask[1] == '1' ){

			strcat(tempMessage, "Illegal label. ");
		}
			
		if( i == 2 && errorBitMask[2] == '1' ){

			strcat(tempMessage, "Illegal operation. ");
		}
			
		if( i == 3 && errorBitMask[3] == '1' ){

			strcat(tempMessage, "Missing or illegal operand on data storage directive. ");
		}
			
		if( i == 4 && errorBitMask[4] == '1' ){

			strcat(tempMessage, "Missing or illegal operand on START directive. ");
		}
			
		if( i == 5 && errorBitMask[5] == '1' ){

			strcat(tempMessage, "Missing or illegal operand on END directive. ");
		}
			
		if( i == 6 && errorBitMask[6] == '1' ){

			strcat(tempMessage, "Too many symbols. ");
		}
			
		if( i == 7 && errorBitMask[7] == '1' ){

			strcat(tempMessage, "Program is too long. ");
		}
			
		if( i == 8 && errorBitMask[8] == '1' ){

			strcat(tempMessage, "Missing instruction. ");
		}
	}
		
		strcat(errorMessage, "Error: ");
		strcat(errorMessage, tempMessage);
		
		return 1;
	
}

void parameterFix(char p1[], char p2[], char p3[])
{
        _Bool found = 0;
        int i;

        // Check to see if label is a mnemonic
        for( i = 0; i < 30; i++)
                if(strcmp(p1, opcode[i].name) == 0)
                {       
                        found = 1;
                        break;
                }

        // If label is a mnemonic place in correct parameter
        if (found == 1)
        {
                clear(p3, 50);
                strcpy(p3, p2);
                clear(p2, 50);
                strcpy(p2, p1);
                clear(p1, 50);

                // Special case for RSUB
                if(strcmp(p2, "RSUB") == 0)
                        clear(p3, 50);
        }

}

_Bool symtabSearch(char key[], Symtab symtab[], int min, int max, int * index )
{
	int len = strlen(key);
	
	if ( key[len -1 ] == '\n')
		key[len - 1] = '\0';
	
	void sort(Symtab [], int, int);
	
	sort(symtab, min, max);
	
	int i;

        while ( min <= max)
        {
                
                int mid = (min + max) /2;
                //printf("$%s$	$%s$ \n", symtab[mid].name, key);
                if ( strcmp(symtab[mid].name, key) == 0){
					//printf("mid: %d\n", mid);
					*index = mid; 
					return 1;}
                else if (strcmp(symtab[mid].name, key) < 0) min = mid + 1;
                else max = mid -1;
                
        }

       return 0;      
}

void sort(Symtab table[], int min, int max){
	Symtab temp;
	int i, j;
	
	for (i = 1; i < max; i++)
      for (j = min; j < max - i; j++) {
         if (strcmp(table[j].name, table[j + 1].name) > 0) {
            temp = table[j];
            table[j] = table[j + 1];
            table[j + 1] = temp;
         }
      }
}


_Bool optabSearch(char key[], int  *n, int code)
{
		if (key[0] == '\0')
			return 0;
	
		int min = 0;
		int max = 30;
        
        while ( min <= max)
        {

                int mid = (min + max) /2;                
                if ( strcmp(opcode[mid].name, key) == 0){
					 *n = mid;
					 return 1;
				 }
                else if (strcmp(opcode[mid].name, key) < 0) min = mid + 1;
                else max = mid -1;
                
        }

       return 0;
}

int errorChecker(char label[], char instruction[], char operand[], int error)
{
	int index = 0;
	int intLable;
	int operandLength = strlen(operand);
	
	// Check for all/any errors with label
	int strLength = strlen(label);
	if ( strLength > 6 )	// Label too long (2)
		error = error | 2;
		 
	// Check label for leading digits
	if ( isdigit(label[0]) )
		error = error | 2;
		
	// Check label and operands for reserved words(4)
	if( optabSearch(label, &index, 3) || optabSearch(operand, &index, 3) )
		error = error | 4;
	
	if ( strcmp( instruction, "START") == 0 )
	{
		// Check for all/any errors with label
		int strLength = strlen(label);
		if ( strLength > 6 )	// Label too long (2)
			error = error | 2;	
			
		// Operand for START should be hex
		// if hex should start with '0'
		// if begin 'A' - 'Z'
		if ( !hexCheck(operand))
			error = error | 16;
			
		//printf("%d\n", error);
		return error;	
	}
	else if ( strcmp ( instruction, "END") == 0 )
	{
		
		if ( isdigit(operand[0]) || operand[0] == '\0')
			error = error | 32;

		return error;
	}
	else
	{
		// Check hex are 'A' - 'Z'	
		if (!isalpha(operand[0]) && !hexCheck(operand) && operand[0] != '\0')
			error = error | 4;
		
		return error;
	}

}

_Bool hexCheck( char num[])
{
		int numLength = strlen(num);
		if ( isdigit(num[0]) && numLength > 0)
		{
			int i;
			for ( i = 1; i < numLength; i++)
			{
				if ( isalpha(num[i]) && ( num[i] < 'A' || num[i] > 'F'))
					return 0;
			}
		}
		else
			return 0;
			
		return 1;
}

ADDRESS load(char fileName []){
	
	FILE *in;
	char line[256];
	char temp[50];
	
	temp[0] = '\0';
	
	// Assign file name
	//strcpy(fileName, "objectfile");
	
	// Open thefile for reading and check to availability
	if ( (in = fopen(fileName, "r")) == NULL  )
	{
		printf("Can't open %s for reading.\n", fileName);
		return;
	}
    else{
		
		int num;
		int i;
		int j;
		ADDRESS addr;
		BYTE data;
		char textAddr[6];
		
		// Start reading the object file
		// Get Header record
		fgets(line, sizeof(line), in);
		
		// main loop for reading object file rows
		while ( 1 ){
			
			// Get next line
			fgets(line, sizeof(line), in);
			if (line[0] == 'E')
				break;
			
			// Now that line has been retrieved
			// Getting address from line
			for ( i = 1; i < 7; i++){
				textAddr[i-1] = line[i];
			}
			sscanf(textAddr, "%06X", &num);			// convert from line to int
			addr = (ADDRESS) num;
			
			// get the start of text record 2 after the length
			for ( i = 9; i < strlen(line)-1; i+=2){
				
				j = 0;
				temp[j] = line[i];
				temp[j+1] = line[i+1];
				
				// Temp now has two char from line
				sscanf(temp, "%X", &num);
				
				data = (BYTE) num;
				
				PutMem ( addr, &data, 0);
				
				addr++;
				
			} // end of loading data
			
		} // end while
		
	} // end else
	
	int len = strlen(line);
	
	clear(temp,50);
	int i, num;
	
	for ( i = 1; i < len; i++){
		temp[i-1] = line[i];
	}
	
	sscanf(temp, "%06X", &num);
	
	ADDRESS addr = (ADDRESS) num;
	
	printf("ADDR: %X\n", addr);
	
	return addr;
	
} //end load


#endif
