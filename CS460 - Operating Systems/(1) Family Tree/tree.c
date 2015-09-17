#include <errno.h>
// #include <setjmp.h> Included below
// #include <stdio.h> Included below
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/* Copyright (C) 2009-2015 Francesco Nidito 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. 
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 */

#ifndef _TRY_THROW_CATCH_H_
#define _TRY_THROW_CATCH_H_

#include <stdio.h>
#include <setjmp.h>

/* For the full documentation and explanation of the code below, please refer to
 * /~nids/docs/longjump_try_trow_catch.html
 */

#define TRY do { jmp_buf ex_buf__; switch( setjmp(ex_buf__) ) { case 0: while(1) {
#define CATCH(x) break; case x:
// Not needed in this program: #define FINALLY break; } default: {
#define ETRY break; } } }while(0)
#define THROW(x) longjmp(ex_buf__, x)

#endif /*!_TRY_THROW_CATCH_H_*/

void storeNames(const char* fileName, char* familyArray[][16], int* numRows, int* numColPerRow);
void outputData(char* familyArray[][16], int numRows, int numColsPerRow[]);
bool checkIfAllAreNames(char* familySubArray[16], int numCols);
bool insert(char* inputString, char *listOfNames[32], int numberOfNames);
bool alreadyInList(char* inputString, char* listOfNames[32], int numberOfNames);
int getIndexOfChild(char* childName, char* familyArray[][16], int numRows);

int main(int argc, char* argv[]) 
{
	int numRows = 0;
	int numNames = 0;
	int *pNumRows = &numRows;
	int numColsPerRow[16];
	int *pNumNames = &numNames;
	char* familyArray[16][16];
	int curRow, curCol;
	for(curRow = 0; curRow < 16; curRow++)
	{
		for(curCol = 0; curCol < 16; curCol++)
		{
			familyArray[curRow][curCol] = malloc(sizeof(char[10])); // None of the names we are testing against have more than 10 characters in them
		}
	}
	
	
	storeNames(argv[1], familyArray, pNumRows, numColsPerRow);
	outputData(familyArray, numRows, numColsPerRow);
	return 0;
}

void storeNames(const char *filename, char* familyArray[16][16], int* numRows, int* numColPerRow)
{
	bool stringsLeftToRead;
	char *listOfNames[32];
	char currentLine[256];
	FILE *fp;
	int curRow = 0;
	int numNames = 0;
	TRY
	{
		if((fp = fopen(filename, "r")) == NULL)
		{
			THROW(1);
		}
		while(fgets(currentLine, 500, fp) != NULL && strcmp(currentLine, "\n") != 0)
		{
			numColPerRow[curRow] = sscanf(currentLine, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", familyArray[curRow][0],familyArray[curRow][1],familyArray[curRow][2],familyArray[curRow][3],familyArray[curRow][4],familyArray[curRow][5],familyArray[curRow][6],familyArray[curRow][7],familyArray[curRow][8],familyArray[curRow][9],familyArray[curRow][10],familyArray[curRow][11],familyArray[curRow][12],familyArray[curRow][13],familyArray[curRow][14],familyArray[curRow][15]);
			if(numColPerRow[curRow] == -1)
			{
				THROW(1); // Error reading the strings
			}
			if(checkIfAllAreNames(familyArray[curRow], numColPerRow[curRow]) == false)
			{
				THROW(2); // One or more of the strings were not names (IE, only alphabet characters)
			}
			numNames+= numColPerRow[curRow]; 
			curRow++;
		}
		*numRows = curRow+1;
		fclose(fp);
	}
	CATCH(1)
	{
		printf("System Error Message: %s\nFailed to open input file. Program terminating.\n", strerror(errno));
		exit(0);
	}
	CATCH(2)
	{
		printf("There was an error reading the input file. Program terminating.\n");
		exit(0);
	}
	CATCH(3)
	{
		printf("One or more of the names were made up of non-alphabet characters(A-Z,a-z). Program terminating.\n");
		 exit(0);
	}
	ETRY;
}

void outputData(char* familyArray[][16], int numRows, int numColsPerRow[])
{
	int curGen = 0;
	int curCol = 0;
	int curRow = 0;
	int genIndex = 0;
	int indexOfChild = 0;
	pid_t pid = 0;

	while(curCol < numColsPerRow[curRow])
	{
		if(curCol == 0)
		{
			for(genIndex = 0; genIndex < curGen; genIndex++)
			{
				printf("\t");
			}
			printf("%s(%d)-%s\n", familyArray[curRow][curCol], getppid(), familyArray[curRow][curCol+1]);
			curCol = 2;
		}
		else
		{
			pid = fork();
			if(pid > 0)
			{
				waitpid(pid, 0, NULL);
				curCol++;
			}
			else if(pid == 0)
			{
				curGen++;
				indexOfChild = getIndexOfChild(familyArray[curRow][curCol], familyArray, numRows);
				if(indexOfChild == -1)
				{
					for(genIndex = 0; genIndex < curGen; genIndex++)
					{
						printf("\t");
					}
					printf("%s(%d)\n", familyArray[curRow][curCol], getppid());
					return;
				}
				else
				{
					curRow = indexOfChild;
					curCol = 0;
				}
			}
		}
	}
}

bool checkIfAllAreNames(char* familySubArray[16], int numCols)
{
	int curCol, curChar, curLength;
	for(curCol = 0; curCol < numCols; curCol++)
	{
		curLength = strlen(familySubArray[curCol]);
		for(curChar = 0; curChar < curLength; curChar++)
		{
			if(isalpha(familySubArray[curCol][curChar]) == 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool insert(char* inputString, char *listOfNames[32], int numberOfNames)
{
	if(numberOfNames == 0)
	{
		listOfNames[numberOfNames] = malloc(strlen(inputString)+1);
		strcpy(listOfNames[numberOfNames], inputString);
		return true;
	}
	if(alreadyInList(inputString, listOfNames, numberOfNames))
	{
		return false;
	}
	listOfNames[numberOfNames] = malloc(strlen(inputString)+1);
	strcpy(listOfNames[numberOfNames], inputString);
	return true;
	
}

bool alreadyInList(char* inputString, char* listOfNames[32], int numberOfNames)
{
	int nameIndex;
	for(nameIndex = 0; nameIndex < numberOfNames; nameIndex++)
	{
		if(strcmp(listOfNames[nameIndex], inputString) == 0)
		{
			return true;
		}
	}
	return false;
}

int getIndexOfChild(char* childName, char* familyArray[][16], int numRows)
{
	int curRow;
	for(curRow = 0; curRow < numRows; curRow++)
	{
		if(strcmp(familyArray[curRow][0], childName) == 0)
		{
			return curRow;
		}
	}

	return -1;
}
