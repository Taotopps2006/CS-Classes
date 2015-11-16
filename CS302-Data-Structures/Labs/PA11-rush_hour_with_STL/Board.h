/** @file Board.cpp
* @mainpage Rush Hour Using STL
* @author Tim Kwist
* @version 1.00
* @date December 5, 2014
*/

#include <iostream>
#include <stdio.h>

using namespace std;

class Board
{
public:
	// Methods
	Board(int r = 6, int c = 6);
	Board& operator=(const Board& other);
	void addCars();
	void swapCells(int, int, int, int);
	bool moveForward(int);
	bool moveBackward(int);
	bool canMoveHere(int row, int col) const;
	bool isSolved() const;
	string boardToString() const;
	inline void printEverything() const // Used only for testing purposes - prints out all the members of the board to see if they are being changed as expected.
	{
		cout << "Rows: " << rows << endl;
		cout << "Cols: " << cols << endl;
		cout << "Number of cars: " << numberOfCars << endl;
		cout << "Number of moves: " << numberOfMoves << endl;
		cout << "List of Cars: " << endl;
		for(int i = 0; i < numberOfCars; i++)
		{
			cout << i << ": x = " << listOfCars[i].x << " y = " << listOfCars[i].y << " length: " << listOfCars[i].length << " orientation: " << listOfCars[i].orientation << endl;
		}
		
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < cols; j++)
			{
				cout << board[i][j];
			}
			cout << endl;
		}
	}
	// Variables
	struct Car
	{
		int x; // Row
		int y; // Col
		int length;
		char orientation;
		inline Car& operator=(const Car& other)
		{
			x = other.x;
			y = other.y;
			length = other.length;
			orientation = other.orientation;
			return *this;
		}
	};
	int rows;
	int cols;
	int numberOfCars;
	int numberOfMoves;
	vector<Car> listOfCars;
	vector<vector<char> > board;
};

/**
 * Board constructor.
 * Sets rows and columns to parameters or default of 6. Sets number of cars and number of moves to 0
 * Initializes 2D vector of all periods
 * 
 * @param r : rows
 * @param c : cols
 * @return None
 * @pre None
 * @post None
 */
Board::Board(int r, int c)
{
	rows = r;
	cols = c;
	numberOfCars = 0;
	numberOfMoves = 0;
	board.resize(rows);
	for(int i = 0; i < rows; i++)
	{
		board[i].resize(cols);
		for(int j = 0; j < cols; j++)
		{
			board[i][j] = '.';
		}
	}
	
}

/**
 * Equal operator overload for board
 * Creates deep copy of other board
 * @param other : other board that this one is being set to
 * @return Board& : returns this board after it has been deep copied
 * @pre None
 * @post None
 */
Board& Board::operator=(const Board& other)
{
	if(this != &other)
	{
		rows = other.rows;
		cols = other.cols;
		numberOfCars = other.numberOfCars;
		numberOfMoves = other.numberOfMoves;
		listOfCars.resize(numberOfCars);
		for(int i = 0; i < numberOfCars; i++)
		{
			listOfCars[i] = other.listOfCars[i];
		}
		board.resize(rows);
		for(int i = 0; i < rows; i++)
		{
			board[i].resize(cols);
			for(int j = 0; j < cols; j++)
			{
				board[i][j] = other.board[i][j];
			}
		}
	}
	
	return *this;
}

 /**
 * Get input from the user to build the board
 * Input should come in the following order:
 * - [number of cars]
 * - [length of car] 
 * - [car orientation] 
 * - [row of car] 
 * - [col of car]
 * if number of cars is 0, skip getting any more input and assume end of file
 * This method will also update the list of cars
 * @param None
 * @return None
 * @pre None
 * @post The board will be generated based on given input, and list of cars will also be updated
 */
void Board::addCars()
{	
	cin >> numberOfCars;
	listOfCars.resize(numberOfCars);
	Car temp;
	for(int i = 0; i < numberOfCars; i++)
	{
		char buffer[1];
		sprintf(buffer, "%d", i);
		cin >> temp.length;
		cin >> temp.orientation;
		cin >> temp.x;
		cin >> temp.y;
		listOfCars[i] = temp;
		board[temp.x][temp.y] = buffer[0];
		if(temp.orientation == 'H')
		{
			board[temp.x][temp.y+1] = buffer[0];
			if(temp.length == 3)
			{
				board[temp.x][temp.y+2] = buffer[0];
			}
		}
		else if(temp.orientation == 'V')
		{
			board[temp.x+1][temp.y] = buffer[0];
			if(temp.length == 3)
			{
				board[temp.x+2][temp.y] = buffer[0];
			}
		}
	}
}

/**
 * Swap cells on the 2D vector
 * 
 * @param r1 : row of the first item to be swapped
 * @param c1 : column of the first item to be swapped
 * @param r2 : row of the second item to be swapped
 * @param c2 : column of the second item to be swapped
 * @return None
 * @pre None
 * @post None
 */
void Board::swapCells(int r1, int c1, int r2, int c2)
{
	char temp = board[r1][c1];
	board[r1][c1] = board[r2][c2];
	board[r2][c2] = temp;
}

/**
 *  Move the car forward if possible. For vertical cars, this is up. For horizontal cars, this is right.
 * Pseudo-code: Checks to see if the desired location to move the car is A. valid and B. empty
 * If it is, swap cells and change row/col to update position after move.
 * 
 * @param i : Index of Car being moved forward
 * @return True if car can be moved forward, false otherwise
 * @pre There are cars on the board to be moved
 * @post The car specified will be moved forward if possible
 */
bool Board::moveForward(int i)
{
	Car *car = &(listOfCars[i]);
	int sRow = car->x; // Starting row
	int sCol = car->y; // Staring column
	int dRow; // Desired row to move to
	int dCol; // Desired col to move to
	if(car->orientation == 'H')
	{
		dRow = sRow;
		dCol = (sCol + car->length);
		if(canMoveHere(dRow, dCol))
		{
			swapCells(car->x, car->y, dRow, dCol);
			(car->y)++;
			return true;
		}
	}
	else if(car->orientation == 'V')
	{
		dRow = (sRow - 1);
		dCol = sCol;
		if(canMoveHere(dRow, dCol))
		{
			swapCells(car->x + (car->length-1), car->y, dRow, dCol);
			(car->x)--;
			return true;
		}
	}
	
	return false;
}

/**
 *  Move the car backwards if possible. For vertical cars, this is down. For horizontal cars, this is left
 * Pseudo-code: Checks to see if the desired location to move the car is A. valid and B. empty
 * If it is, swap cells and change row/col to update position after move.
 * 
 * @param i : Index of Car being moved backwards
 * @return True if car can be moved backwards, false otherwise
 * @pre There are cars on the board to be moved
 * @post The car specified will be moved backwards if possible
 */
bool Board::moveBackward(int i)
{
	Car *car = &(listOfCars[i]);
	int sRow = car->x; // Starting row
	int sCol = car->y; // Staring column
	int dRow; // Desired row to move to
	int dCol; // Desired col to move to
	if(car->orientation == 'H')
	{
		dRow = sRow;
		dCol = (sCol - 1);
		if(canMoveHere(dRow, dCol))
		{
			swapCells(car->x, car->y + (car->length - 1), dRow, dCol);
			(car->y)--;
			return true;
		}
	}
	else if(car->orientation == 'V')
	{
		dRow = (sRow + car->length);
		dCol = sCol;
		if(canMoveHere(dRow, dCol))
		{
			swapCells(car->x, car->y, dRow, dCol);
			(car->x)++;
			return true;
		}
	}
	
	return false;
}

/**
 * Determine whether the given position in the 2D vector can be moved to.
 * Pseudo-code: Check if the row and col given are within the bounds of the array ( >= 0, < #rows/cols)
 * Check if spot on board is a period. If it is, true; else, false.
 * 
 * @param row : row of the spot being checked if valid move
 * @param col : column of the spot being checked if valid move
 * @return bool : true if the spot is valid to move to, false if not
 * @pre None
 * @post No variables will be changed
 */
bool Board::canMoveHere(int row, int col) const
{
	if((row >= 0) && (col >= 0) && (row < rows) && (col < cols))
	{
		if(board[row][col] == '.')
		{
			return true;
		}
	}
	return false;
}

/**
 * Determine whether the rush hour puzzle is solved by checking if the car is on the last column
 * 
 * @param None
 * @return True if car at index 0 is on 5th column, false otherwise
 * @pre None
 * @post No variables will be changed
 */
bool Board::isSolved() const
{
	return ((listOfCars[0].y + 1) == 5);
}

/**
 * Convert the board to a string and return it.
 * Pseudo-code: create a temp string, then concat every character on the board to the string and return the string.
 * 
 * @param None
 * @return string : string containing all the characters in the board.
 * @pre None
 * @post No variables will be changed
 */
string Board::boardToString() const
{
	string temp = "";
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			temp += board[i][j];
		}
	}
	return temp;
}