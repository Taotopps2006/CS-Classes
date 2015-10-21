/** @file rushHour.cpp
* @mainpage Rush Hour Using STL
* @author Tim Kwist
* @version 1.00
* @date December 5, 2014
*/

/************************************************
*  Header Files
***********************************************/
#include <iostream>
#include <stdio.h>
#include <set>
#include <queue>
#include "Board.h"
 
 using namespace std;
/************************************************
*  Function headers
***********************************************/
int solveBoard();

/************************************************
*  Global Variables
***********************************************/

/**
 * Main method: continue through multiple scenarios of rush hour until calling the solveBoard method returns -1.
 * 
 * @param None
 * @return int : mandatory for main method
 * @pre None
 * @post None
 */
int main()
{
	int currentScen = 1;
	int moves = 0;
	while(true)
	{
		moves = solveBoard();
		if(moves == -1)
			return 0;
		printf ("Scenario %i requires %i moves\n", currentScen, moves);
		currentScen++;
	}
}

/**
 * Solve the rush hour puzzle given to us using a breadth-first search.
 * Pseudo code:
 * 1. Set up a queue that will hold each possible move and a set that will hold a "snap shot" of each board to check whether we've encountered it before or not.
 * 2. Call the board's addCars method to get all the input of the cars and set them up on the board. If the number of cars on the board is 0, exit out of the method. Otherwise, push this board onto the queue.
 * 3. While loop
 * 3a. Set the current board to the board from the front of the queue, and pop the queue.
 * 3b. Check if the board is solved. If it is, return the number of moves stored on the board.
 * 3c. Otherwise, create a snapshot of this board in the form of converting the board to a string.
 * 3d. If the snapshot is found in the set (dejaVu), don't go any farther in the loop, go back to the beginning.
 * 3e. Otherwise, insert the snapshot into dejaVu and go into our for loop
 * 3f. For each car on the board:
 * 3fa. Try to move the car forward; if we can, increase the number of moves, push it onto the queue, then decrease the number of moves and move it back to where it was.
 * 3fb. Do the same with backwards; check if we can do it, and if we can then increase the number of moves, push it onto the queue, then decrease the number and move it back to where it was.
 * 
 * @param None
 * @return int : Smallest number of moves required to solve this board; if number of cars on the board is 0, return -1
 * @pre None
 * @post None
 */
int solveBoard()
{
	Board board;
	queue<Board> boardQueue;
	set<string> dejaVu;
	string temp = "";
	board.addCars();
	if(board.numberOfCars == 0)
		return -1;
	boardQueue.push(board);
	while(!boardQueue.empty())
	{
		board = boardQueue.front();
		boardQueue.pop();
		if(board.isSolved())
		{
			return board.numberOfMoves;
		}
		temp = board.boardToString();
		if(dejaVu.find(temp) == dejaVu.end()) // If this particular board hasn't already been tested
		{
			dejaVu.insert(temp);
			for(int i = 0; i < board.numberOfCars; i++)
			{
				if(board.moveForward(i))
				{
					board.numberOfMoves++;
					boardQueue.push(board);
					board.moveBackward(i);
					board.numberOfMoves--;
				}
				
				if(board.moveBackward(i))
				{
					board.numberOfMoves++;
					boardQueue.push(board);
					board.moveForward(i);
					board.numberOfMoves--;
				}
			}
		}
	}
	return -2;
}