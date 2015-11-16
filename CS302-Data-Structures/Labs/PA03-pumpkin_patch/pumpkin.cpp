/** @file pumpkin.cpp
* @mainpage The Great Pumpkin Patch Problem
* @author Tim Kwist
* @version 1.00
* @brief Input
The input to this program will be a number of different gardens. The first line of the input for
each garden will be the dimensions of the garden, r, the number of rows in the garden, and c, the
number of columns, where 0 ≤ r ≤ 40 and 0 ≤ c ≤ 40. Following the dimensions will be r lines
with c characters on each line. Each of these characters will be a lower case letter representing
the type of gourd grown in the square. A lower case 'p' will represent pumpkins.
A garden with 0 for the number of rows and/or columns indicates the end of input and should not
be processed.
Output
For each garden, output the number of the garden (with the first input set being garden 1), the
number of pumpkin patches in the garden, and the size of the pumpkin patches in order from
smallest to largest. If there is more than one patch of a given size, print the size as many times as
it occurs. Use the following format:Garden # 1: 4 patches, sizes: 1 4 8 10
Have a blank line between each line of output.
@date Wednesday, September 17, 2014
*/

/********************************************//**
 *  Header Files
 ***********************************************/
 #include <algorithm>
 #include <iostream>
#include <fstream> 
 using namespace std;
 /********************************************//**
 *  Function headers
 ***********************************************/
 int findTotalPumpkins(char **pPatch);
 bool checkForPatches(char **pPatch, int &row, int &col);
 int findPatchSize(char **pPatch, int row, int col, int from);
 /********************************************//**
 *  Global Variables
 ***********************************************/
int patchRows = 0;
int patchCols = 0;
 /********************************************//**
 *  Main method implementation
 ***********************************************/
int main()
{
int count = 1; // Current "Garden" the program is on
while(true) // Program will break when it comes across a row and/or column of 0
{
	/*******************************//**
	*  Get input from the keyboard and create pumpkin patch
	***********************************/
	// Ask user for input
	cin >> patchRows;
	cin >> patchCols;
	/*	Check for valid input
	*	If row or col is 0, end of input - terminate program
	*/
	if(patchRows == 0 || patchCols == 0)
		return 0; 
	char** pumpkinPatch = new char*[patchRows];
	for(int i = 0; i < patchRows; i++)
	{
		pumpkinPatch[i] = new char[patchCols];
	}
	for(int i = 0; i < patchRows; i++)
	{
		for(int j = 0; j < patchCols; j++)
		{
			cin >> pumpkinPatch[i][j];
		}
	}
	// Calculate number of patches and size of patches
	int pRow = -1;
	int pCol = -1;
	int* listOfSizes = new int[(findTotalPumpkins(pumpkinPatch) / 2) + 1]; // There can't possibly be more than ((total # of pumpkins / 2) + 1), by the nature of patches
	int index = 0; // Index of sizes currently on
	while(checkForPatches(pumpkinPatch, pRow, pCol))
	{
		 listOfSizes[index] = findPatchSize(pumpkinPatch, pRow, pCol, 0);
		 index++;
	}
	sort(listOfSizes, listOfSizes + index);
	// Output info about pumpkins
	cout << "Garden # " << count << ": " << index << " patches, sizes:";
	for(int i = 0; i < index; i++)
	{
		cout << " " << listOfSizes[i];
	}
	cout << endl;
	// Clean up
	for(int i = 0; i < patchRows; i++) {
		delete [] pumpkinPatch[i];
	}
	delete [] pumpkinPatch;
	delete [] listOfSizes;
	count++;
}
}

 /********************************************//**
 *  Method implementation
 ***********************************************/
 
/**
 * Sequentially count the number of pumpkins in the given array
 * 
 * @param pumpkinPatch 2D array of characters that represents the 'pumpkin patch' to be searched though
 * @return Number of 'p' s in the array
 * @pre None
 * @post The array will be unchanged.
 */
int findTotalPumpkins(char **pPatch)
{
	int count = 0;
	for(int i = 0; i < patchRows; i++)
	{
		for(int j = 0; j < patchCols; j++)
		{
			if(pPatch[i][j] == 'p')
			{
				count++;
			}
		}
	}
	
	return count;
}  
  
/**
 * Sequentially search through the 2D array of characters to see if there are any pumpkins (represented by 'p' s). If there are, it will return true and the ints passed by reference will contain the coordinates of the pumpkin.
 * 
 * @param pumpkinPatch 2D array of characters that represents the 'pumpkin patch' to be searched though
 * @param row Blank int passed by reference. If a pumpkin is found, this variable will contain the x coordinate (or row) the pumpkin is in
 * @param col Blank int passed by reference. If a pumpkin is found, this variable will contain the y coordinate (or col) the pumpkin is in
 * @return True if a 'p' is found in the array. Otherwise, false.
 * @pre None
 * @post The array will be unchanged. If there is a patch found, the value of row and col will be changed to the coordinates of that pumpkin
 */
bool checkForPatches(char **pPatch, int &row, int &col)
{
	for(int i = 0; i < patchRows; i++)
	{
		for(int j = 0; j < patchCols; j++)
		{
			if(pPatch[i][j] == 'p')
			{
				row = i;
				col = j;
				return true;
			}
		}
	}
	
	return false;
}

/**
 * Recursively check the size of a pumpkin patch. The original coordinate must contain a pumpkin ('p'). Once a pumpkin is found, the character will be changed to a 0 to avoid passing over the same pumpkin. If valid, check for pumpkins in each 4 main directions (north, south, east, west) for more pumpkins. If more are found, call this method with that pumpkin's location.
 *
 * @param pumpkinPatch A 2D array of chars that represents the pumpkin patch
 * @param row The row at which a pumpkin exists
 * @param col The column at which a pumpkin exists
 * @param from The direction from which this method was called. 0: none, 1: South, 2: West, 3: North, 4: East
 * @return The size(int) of 'p' s in a patch.
 * @pre A pumpkin('p') exists at pumpkinPatch[row][col]
 * @post All pumpkin's within the patch of the original pumpkin will be changed to 0s in the pumpkinPatch array
 */
int findPatchSize(char **pPatch, int row, int col, int from)
{
	int count = 1;
	// Set current pumpkin to 0
	pPatch[row][col] = 0;	
	// Search north - check that row-1 is greater than or equal to 0 (ie, still within bounds of array) and check that we were not just called from the cell to the north (from = 1 = south)
	if((from != 1) && (row - 1 >= 0))
	{
		if(pPatch[row-1][col] == 'p')
		{
			count += findPatchSize(pPatch, row-1, col, 3);
		}
	}
	// Search east - check that col+1 is less than patchCols (ie, still within bounds of array) and check that we were not just called from the cell to the east (from = 2 = west)
	if((from != 2) && (col + 1 < patchCols))
	{
		if(pPatch[row][col+1] == 'p')
		{
			count += findPatchSize(pPatch, row, col+1, 4);
		}
	}
	// Search south - check that row+1 is less than patchRows (ie, still within the bounds of array) and check that we were not just called from the cell to the south (from = 3 = north)
	if((from != 3) && (row + 1 < patchRows))
	{
		if(pPatch[row+1][col] == 'p')
		{
			count += findPatchSize(pPatch, row+1, col, 1);
		}
	}
	// Search west - check that col-1 is greater than or equal to 0 (ie, still within bounds of array) and check that we were not just called from the cell to the west (from = 4 = east)
	if((from != 4) && (col - 1 >= 0))
	{
		if(pPatch[row][col-1] == 'p')
		{
			count += findPatchSize(pPatch, row, col-1, 2);
		}
	}
	
	return count;
}