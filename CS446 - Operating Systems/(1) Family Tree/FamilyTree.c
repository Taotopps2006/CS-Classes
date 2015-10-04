//////////////////////////////////////////////////////////////////////
/*
 FileName: FamilyTree.c

 Description: This program will read input from a file whose name
 must be passed in through a command line argument.
 A family tree will be stored in a 2D array of character arrays,
 then the family tree will be outputted to the standard output.
 Parents will create a new process for each of their children.
 For example: for the family of
 Adam BB Casey Donna X
 Donna Yong Mary Emily
 Mary Frank
 Casey Paul Karen
 A family tree of
 Adam(0)–BB
     Casey(839)-Paul
         Karen(857)
    Donna(839)-Yong
        Mary(906)-Frank
        Emily(906)
    X(839)
Where the number in parenthesis is the parent ID of that process
*/
// Version/Revision Information ///////////////////////////////////
/*
 1.00 ( 01/18/2015 ) - Tim Kwist
 Original Code
*/
 // Program Description/Support /////////////////////////////////////
/*
 This program relies on a file whose name must be passed in as
 a command line argument.
*/
 // Precompiler Directives //////////////////////////////////////////
//
 #ifndef FAMILY_TREE_C
 #define FAMILY_TREE_C
//
// Header Files ///////////////////////////////////////////////////
//
#include <errno.h>
// #include <setjmp.h> Included below
// #include <stdio.h> Included below
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
//
// 
/* Copyright ( C ) 2009-2015 Francesco Nidito 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files ( the "Software" ), to deal
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

#define TRY do { jmp_buf ex_buf__; switch( setjmp( ex_buf__ ) ) { case 0: while( 1 ) {
#define CATCH( x ) break; case x:
// Not needed in this program: #define FINALLY break; } default: {
#define ETRY break; } } } while( 0 )
#define THROW( x ) longjmp( ex_buf__, x )

#endif /*!_TRY_THROW_CATCH_H_*/

// Global Constant Definitions ////////////////////////////////////
//
///// NONE
//
// Class Definitions //////////////////////////////////////////////
//
///// NONE
//
// Free Function Prototypes ///////////////////////////////////////
//
extern int isalpha( int );
extern pid_t getppid( void );
extern pid_t waitpid( pid_t, int *, int );

void StoreNames( const char * fileName, char * familyArray[ ][ 16 ], int * numRows, int * numColPerRow );
void OutputData( char * familyArray[ ][ 16 ], int numRows, int numColsPerRow[ ] );
bool CheckIfAllAreNames( char * familySubArray[ 16 ], int numCols );
int GetIndexOfChild( char*  childName, char * familyArray[ ][ 16 ], int numRows );

// Main Function Implementation ///////////////////////////////////
//
int main( int argc, char * argv[ ] ) 
{
    // Variable Declarations /////////
    //
    int curRow, curCol; // The current row and column for iterating over familyArray
    const int MAX_ROWS_IN_ARRAY = 16;
    const int MAX_COLS_IN_ARRAY = 16;
    int numRows = 0; // Total number of rows that have names in our familyArray
    int * pNumRows = & numRows; // Pointer to the number of rows so that we can pass by reference
    int numColsPerRow[ MAX_COLS_IN_ARRAY ]; // Number of columns with names in them per row in our familyArray
    char* familyArray[ MAX_ROWS_IN_ARRAY ][ MAX_COLS_IN_ARRAY ]; // Our over family tree / family array.
                                   // Each row correlates to input file; each cell holds a name
    // Initialize familyArray
    for( curRow = 0; ( curRow < 16 ); curRow++ )
    {
        for( curCol = 0; ( curCol < 16 ); curCol++ )
        {
            // None of the names we are testing against have more than 10 characters in them
            familyArray[ curRow ][ curCol ] = malloc( sizeof( char[ 10 ] ) );
        }
    }
    //
    // Program execution /////////
    //
    StoreNames( argv[ 1 ], familyArray, pNumRows, numColsPerRow );
    OutputData( familyArray, numRows, numColsPerRow );
    return 0;
    //
}
//
// Free Function Implementation ///////////////////////////////////
//
/***********************************************************
Function Specification: StoreFamily
============================================================
Preconditions:
 - const char * fileName refers to name of file that holds
 a family tree with valid input
 - char * familyArray[ 16 ][ 16 ] has had memory allocated
 to each cell large enough to hold a 10 character name
 - int * numRows points to an int variable that will be
 used for notifying how many rows of data familyArray has
 int * numColPerRow is an array that will hold the
 number of columns that each row in familyArray has data
 for
Postconditions:
 - fileName will not be changed in the calling function
 - familyArray will be filled with all the names of
 the family in the input file
 - The value pointed to by numRows will be equal to
 the number of rows that have data in familyArray
 - Each index of numColPerRow will hold the number
 of columns that have data in each row that has data
 in familyArray
Algorithm:
 - Open the file
 - Use a while loop to iterate over each line in the file
 - Use fgets to get each line; if it returns null or gets
 a \n, end the loop
 - Use sscanf to get the number of columns(names) in each
 row and to store each name in an element of familyArray
 - After each sscanf, increment the number of rows
 Exceptional/Error Conditions:
 - If the file doesn't open correctly, output message
 and terminate program
 - If there is an error when sscanf'ing the input string,
 output a message and terminate.
 - If not all names in the input string are made up of
 only alphabetic characters, output a message and terminate.
============================================================
***********************************************************/
/* Parameters:
 fileName - holds the name of the variable to open
 familyArray - will hold all names of the family
 numRows - will hold the number of rows that
           are in familyArray
 numColPerRow - will hold the number of columns
                that holds data in each row that
                holds data in familyArray
*/
void StoreNames( const char * filename, char * familyArray[ 16 ][ 16 ], int * numRows, int * numColPerRow )
{
    // Variable Declarations /////////
    //
    char currentLine[ 256 ]; // Current line from the file
    FILE *fp; // Pointer to the file we'll open
    //
    // Function Execution /////////
    //
    TRY
    {
        if( ( fp = fopen( filename, "r" ) ) == NULL ) /* OPEN FILE */
        {
            THROW( 1 );
        }
        while( ( fgets( currentLine, 500, fp ) != NULL ) 
            && ( strcmp( currentLine, "\n" ) != 0 ) )
        {
            numColPerRow[ curRow ] = 
                sscanf( currentLine, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                 familyArray[ curRow ][ 0 ],familyArray[ curRow ][ 1 ],familyArray[ curRow ][ 2 ],
                 familyArray[ curRow ][ 3 ],familyArray[ curRow ][ 4 ],familyArray[ curRow ][ 5 ],
                 familyArray[ curRow ][ 6 ],familyArray[ curRow ][ 7 ],familyArray[ curRow ][ 8 ],
                 familyArray[ curRow ][ 9 ],familyArray[ curRow ][ 10 ],familyArray[ curRow ][ 11 ],
                 familyArray[ curRow ][ 12 ],familyArray[ curRow ][ 13 ],familyArray[ curRow ][ 14 ],
                 familyArray[ curRow ][ 15 ] );

            if( numColPerRow[ curRow ] == -1 )
            {
                THROW( 1 ); // Error reading the strings
            }
            if( CheckIfAllAreNames( familyArray[ curRow ], numColPerRow[ curRow ] ) == false )
            {
                THROW( 2 ); // One or more of the strings were not names ( IE, only alphabet characters )
            }
            (*numRows)++;
        }
        fclose( fp ); /* CLOSE FILE */
    }
    CATCH( 1 )
    {
        printf( "System Error Message: %s\nFailed to open input file. Program terminating.\n", strerror( errno ) );
        exit( 0 ); /* EXIT PROGRAM */
    }
    CATCH( 2 )
    {
        printf( "There was an error reading the input file. Program terminating.\n" );
        exit( 0 ); /* EXIT PROGRAM */
    }
    CATCH( 3 )
    {
        printf( "One or more of the names were made up of non-alphabet characters( A-Z,a-z ). Program terminating.\n" );
         exit( 0 ); /* EXIT PROGRAM */
    }
    ETRY;
    //
}

/***********************************************************
Function Specification: OutputFamily
============================================================
Preconditions:
 - char * familyArray[ ][ 16 ] has been initialized with
 all the names from the input file
 - int numRows holds the number of rows in familyArray that
 have data in them
 - int numColsPerRow[ ] holds the number of columns in each
 row of familyArray that have data in them
Postconditions:
 - familyArray, numRows, and numColsPerRow will not be changed
 in the calling function
 - The family tree will be printed to the standard output
 in the form of
 parent1(parentID) -parent2
    child1(parentID) -child1Parent
        child1Child(parentID)
    child2(parentID)
etc.
where parentID refers to each process' parents' ID
Algorithm:
 - Iterate over the elements in familyArray from
 col = o to col = numCols
 - Iterate over each column of the first row
 - Print the first two names, then move to the first
 child at index 2
 - For each child, fork
 - If parent, wait for child then increment column
 - If child, increment current generation and
 get the index of yourself on your own row
 - If you find it, change the current row to that row
 and column to 0
 - Otherwise, print out child name as is and exit
 - Each process will continue this loop until the column
 they are pointing at is out of bounds of their row's number
 of columns
 Exceptional/Error Conditions:
 - If there is a problem forking (pid < 0), the program will
 output, then terminate.
============================================================
***********************************************************/
/* Parameters:
 familyArray - holds all names of the family
 numRows - holds the number of rows that
           are in familyArray
 numColPerRow - holds the number of columns
                that holds data in each row that
                holds data in familyArray
*/
void OutputData( char* familyArray[ ][ 16 ], int numRows, int numColsPerRow[ ] )
{
    // Variable Declarations /////////
    int genIndex = 0; // Index of generation for iterating
    int curCol = 0; // Current column in familyArray
    int curRow = 0; // Current row in familyArray
    int curGen = 0; // Current generation of the family
    int indexOfChild = 0;
    pid_t pid = 0;
    //
    // Function Execution /////////
    //
    while( curCol < numColsPerRow[ curRow ] )
    {
        if( curCol == 0 )
        {
            for( genIndex = 0; ( genIndex < curGen ); genIndex++ )
            {
                printf( "\t" );
            }
            printf( "%s(%d)-%s\n", 
                familyArray[ curRow ][ curCol ], 
                getppid( ), 
                familyArray[ curRow ][ curCol+1 ] );
            curCol = 2;
        }
        else
        {
            pid = fork( );
            if( pid < 0 )
            {
                printf("Error forking, now terminating program");
                exit(0);
            }
            if( pid > 0 )
            {
                waitpid( pid, 0, 0 ); // Child off at college, wait for it to come back
                curCol++;
            }
            else if( pid == 0 )
            {
                curGen++;
                indexOfChild = GetIndexOfChild( familyArray[ curRow ][ curCol ], 
                                                familyArray, 
                                                numRows );
                if( indexOfChild == -1 )
                {
                    for( genIndex = 0; ( genIndex < curGen ); genIndex++ )
                    {
                        printf( "\t" );
                    }
                    printf( "%s( %d )\n", 
                        familyArray[ curRow ][ curCol ], 
                        getppid( ) );
                    exit( 0 ); // Oh god, Michael - why did you make me kill all these children?
                }
                else
                {
                    curRow = indexOfChild;
                    curCol = 0;
                }
            }
        }
    }
    //
}

/***********************************************************
Function Specification: CheckIfAllAreNames
============================================================
Preconditions:
 - char * familySubArray[ 16 ] has numCols names in it
 - int numCols holds the number of columns that have data
 in them in familySubArray
Postconditions:
 - familySubArray and numCols are not changed in the
 calling function
 - If a character is found that is not in the alphabet
 (A-Z, a-z), false is returned
Algorithm:
 - Iterate over the elements in familyArray from
 col = o to col = numCols
 - Iterate over each character of each name in familyArray from
 char = 0 to char = name length
 - Use isalpha to check if each character is alphabetic
 - If isalpha returns 0, this function returns false
 - If all characters are checked with isalpha and none return
 0, this function returns 0
 Exceptional/Error Conditions:
============================================================
***********************************************************/
/* Parameters:
 familySubArray - holds a subset of the names that will be
                  put into familyArray
 numCols - holds the number of columns in familySubArray
*/
bool CheckIfAllAreNames( char* familySubArray[ 16 ], int numCols )
{
    // Variable Declarations /////////
    //
    int curCol, curChar, curLength;
    //
    // Method Execution /////////
    //
    for( curCol = 0; curCol < numCols; curCol++ )
    {
        curLength = strlen( familySubArray[ curCol ] );
        for( curChar = 0; ( curChar < curLength ); curChar++ )
        {
            if( isalpha( familySubArray[ curCol ][ curChar ] ) == 0 )
            {
                return false;
            }
        }
    }
    return true;
    //
}

/***********************************************************
Function Specification: GetIndexOfChild
============================================================
Preconditions:
 - char * childName contains the name of a child in
 familyArray
 - char * familyArray[ ][ 16 ] has already gotten all of the
 names of the current family from the input file
 - int numRows holds the number of rows that have data in them
 in familyArray
Postconditions:
 - childName, familyArray, and numRows are not changed in the
 calling function
 - If the child is found, return their row index; the column index
 is implicitly 0 since we are only looking for children with
 families
Algorithm:
 - Iterate over the elements in familyArray from
 row = 0 to row = numRows
 col = 0 (always)
 - Use strcmp to compare strings
 - If an element contains the desired childName, return that
 index
  - If the child's name is not found, return -1 to signal that
 this child is not married
 Exceptional/Error Conditions:
============================================================
***********************************************************/
/* Parameters:
 childName - holds the name of the child that this method is
             looking for
 familyArray - holds the name of each person in the family
               tree
 numRows - holds the number of rows in familyArray that holds
           data
*/
int GetIndexOfChild( char* childName, char* familyArray[ ][ 16 ], int numRows )
{
    // Variable Declarations /////////
    int curRow; // Current row in familyArray
    //
    // Program Execution /////////
    //
    for( curRow = 0; ( curRow < numRows ); curRow++ )
    {
        if( strcmp( familyArray[ curRow ][ 0 ], childName ) == 0 )
        {
            return curRow;
        }
    }
    return - 1;
    //
}
//
// Class/Data Structure Member Implementation //////////////////////
//
//
// Terminating Precompiler Directives ///////////////////////////////
//
 #endif // FAMILY_TREE_C
//