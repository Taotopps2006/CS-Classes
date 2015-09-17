#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void storeNames(const char* fileName, char* familyArray[][16], int* numRows, int* numColPerRow);
void outputData(char* familyArray[][16], int numRows, int numColsPerRow[]);
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
	int rowIndex, colIndex;
	for(rowIndex = 0; rowIndex < 16; rowIndex++)
	{
		for(colIndex = 0; colIndex < 16; colIndex++)
		{
			familyArray[rowIndex][colIndex] = "";
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
	char currentName[32];
	FILE *fp;
	int curRow = 0;
	int curCol = 0;
	int numNames = 0;
	fp = fopen(filename, "r");
	stringsLeftToRead = fscanf(fp, "%s", currentName);
	do
	{
		printf("New name = %s \n", currentName);
		if(!insert(currentName, listOfNames, numNames))
		{
			numColPerRow[curRow] = curCol;
			curRow++;
			curCol = 0;
		}
		else
		{
			numNames++;
		}
		familyArray[curRow][curCol] = malloc(strlen(currentName)+1);
		strcpy(familyArray[curRow][curCol], currentName);
		printf("At row = %d , col = %d , currentName = %s \n", curRow, curCol, familyArray[curRow][curCol]);
		curCol++;
	} while(fscanf(fp, "%s", currentName) != EOF);
	numColPerRow[curRow] = curCol;
	*numRows = curRow+1;
	fclose(fp);
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
