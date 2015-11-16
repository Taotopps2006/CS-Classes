/** @file PriorityQueue.cpp
@author Tim Kwist
@version 1.0
@brief This program will implement a PriorityQueue
@details The specifications of this program are defined by C++ Data Structures: A Laboratory Course (3rd edition) by Brandle, JGeisler, Roberge, Whittington, lab 11.
@date Wednesday, November 5, 2014
*/

#include "PriorityQueue.h"

using namespace std;

/**
 * Constructor for PriorityQueue. Calls constructor for heap. Set maxNumber of items this PriorityHeap can hold.
 *
 * @param maxNumber : maximum number of items allowed in this heap.
 * @return None
 * @pre None
 * @post This PriorityHeap will be initialized.
*/
template <typename DataType, typename KeyType, typename Comparator>
PriorityQueue<DataType, KeyType, Comparator>::PriorityQueue(int maxNumber)
{
	//this->template Heap<DataType, KeyType, Comparator>(maxNumber);
	//Heap<DataType, KeyType, Comparator>::Heap(maxNumber);
	//Heap<DataType, KeyType, Comparator>::Heap<DataType, KeyType, Comparator>(maxNumber);
	// None of these worked.
}

/**
 * Destructor for PriorityHeap. Deallocates all memory in this heap. Calls Heap clear method.
 *
 * @param None
 * @return None
 * @pre None
 * @post All memory of this heap will be deallocated.
*/
template <typename DataType, typename KeyType, typename Comparator>
PriorityQueue<DataType, KeyType, Comparator>::~PriorityQueue()
{
	Heap<DataType, KeyType, Comparator>::clear();
}

/**
 * Enqueues an item into the PriorityQueue. Uses heap insert method.
 *
 * @param newDataItem : New item to insert into the PriorityHeap.
 * @return None
 * @pre None
 * @post Item will be inserted into the heap.
*/
template <typename DataType, typename KeyType, typename Comparator>
void PriorityQueue<DataType, KeyType, Comparator>::enqueue(const DataType &newDataItem)
{
	Heap<DataType,KeyType,Comparator>::insert(newDataItem);
}

/**
 * Removes highest priority item in the PriorityHeap. Uses base heap's remove method.
 *
 * @param None
 * @return DataType : highest priority item that was removed from the PriorityHeap
 * @pre None
 * @post HIghest priority item will be removed from the PriorityHeap.
*/
template <typename DataType, typename KeyType, typename Comparator>
DataType PriorityQueue<DataType, KeyType, Comparator>::dequeue()
{
	return Heap<DataType,KeyType,Comparator>::remove();
}