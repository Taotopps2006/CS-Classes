/** @file ossim.cpp
@author Tim Kwist
@version 1.0
@brief This program will implement an operating system task scheduling system
@details The specifications of this program are defined by C++ Data Structures: A Laboratory Course (3rd edition) by Brandle, JGeisler, Roberge, Whittington, lab 11.
@date Wednesday, November 5, 2014
*/

//--------------------------------------------------------------------
//
//  Laboratory 11, Programming Exercise 1                     ossim.cs
//
//  (Shell) Operating system task scheduling simulation
//
//--------------------------------------------------------------------

// Simulates an operating system's use of a priority queue to regulate
// access to a system resource (printer, disk, etc.).

#include <iostream>
#include <cstdlib>
#include "PriorityQueue.cpp"

using namespace std;

//--------------------------------------------------------------------
//
// Declaration for the task data struct
//

struct TaskData
{
    int getPriority () const
        { return priority; }     // Returns the priority. Needed by the heap.

    int priority,                // Task's priority
        arrived;                 // Time when task was enqueued

};


//--------------------------------------------------------------------

/**
 * Programming Exercise #1 for Lab 11. (Shell) Operating system task scheduling simulation
 * Pseudocode:
 * 1. Get number of priority levels
 * 2. Get length of time to run the simulator
 * 3. Run loop for length of time to run the simulator.
 * 3a. Dequeue item in PriorityQueue if it exists. Output the item dequeue'd.
 * 3b. Determine number of items to add to the list based on a random number (50% no items, 25% 1 item, 25% 2 items)
 * 3c. Insert previously determined number of items into PriorityQueue.
 *
 * @param None
 * @return int : default return for main function
 * @pre None
 * @post None
 */
int main ()
{
    PriorityQueue<TaskData, int, Less<int> > taskPQ;   // Priority queue of tasks
    TaskData task;               // Task
    int simLength,               // Length of simulation (minutes)
        minute,                  // Current minute
        numPtyLevels,            // Number of priority levels
        numArrivals,             // Number of new tasks arriving
        j;                       // Loop counter

    // Seed the random number generator
    srand(7);

    cout << endl << "Enter the number of priority levels : ";
    cin >> numPtyLevels;

    cout << "Enter the length of time to run the simulator : ";
    cin >> simLength;

    for ( minute = 0 ; minute < simLength ; minute++ )
    {
        // Dequeue the first task in the queue (if any).
		if(!taskPQ.isEmpty())
		{
			task = taskPQ.dequeue();
			cout << "At " << minute << " dequeued : " << task.priority << " " << task.arrived << " " << (minute-task.arrived) << endl;
		}
        // Determine the number of new tasks and add them to
        // the queue.
		switch(rand() % 4)
		{
			case 2: { numArrivals = 2; break;}
			case 1: { numArrivals = 1; break;}
			default: { numArrivals = 0;}
		}
		
		for(j = 1; j <= numArrivals; j++)
		{
			task.arrived = minute;
			task.priority = rand() % numPtyLevels;
			taskPQ.enqueue(task);
		}
    }

    return 0;
}

