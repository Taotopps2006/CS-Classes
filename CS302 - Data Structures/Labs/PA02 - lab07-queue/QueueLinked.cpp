/** @file QueueLinked.cpp
@author Tim Kwist
@version 1.0
@brief This program will implement a Linked Queue
@details The specifications of this program are defined by C++ Data Structures: A Laboratory Course (3rd edition) by Brandle, JGeisler, Roberge, Whittington, exercise 7. This implementation only implements the Linked List version of the Queue.
@date Wednesday, September 10, 2014
*/

/*
Author self-assessment statement:
This program is complete and working according to specifications, according to the best of my knowledge.
*/

/********************************************//**
 *  Header Files
 ***********************************************/
#include "QueueLinked.h"
using namespace std;


/********************************************//**
 *  Method implementations
 ***********************************************/
 /**
 * This constructor creates an empty queue.
 *
 * @param int Max Number of data items this queue can hold
 * @return This constructor does not return anything
 * @pre None
 * @post Creates an empty queue. Allocates enough memory for the queue containing maxNumber data items if necessary)
 */
template <class DataType>
QueueLinked<DataType>::QueueLinked(int maxNumber)
{
	front = NULL;
	back = NULL;
}

 /**
 * This copy constructor will make a deep copy of another queue. Initializes front and back to NULL, then uses the enqueue method to add items from the other queue.
 *
 * @param QueueLinked& Linked Queue to copy
 * @return This copy constructor does not return anything
 * @pre None
 * @post Initializes the queue to be equivalent to the other Queue object paramter
 */
template <class DataType>
QueueLinked<DataType>::QueueLinked(const QueueLinked& other)
{
	front = back = NULL;
	QueueNode* temp = other->front;
	while(temp->next != NULL)
	{
		temp = temp->next;
		enqueue(temp->dataItem);
	}
}

 /**
 * Overloaded assignment operator. Creates a deep copy of another linked queue by clearing the current list and then using the enqueue method to insert new items into the list.
 *
 * @param QueueLinked& Other linked queue that this queue will be a copy of
 * @return A reference to the modified queue
 * @pre None
 * @post Sets the queue to be equivalent to the other Queue object parameters and returns a reference to the modified queue
 */
template <class DataType>
QueueLinked<DataType>& QueueLinked<DataType>::operator=(const QueueLinked& other)
{
	clear();
	QueueNode* temp = other->front;
	while(temp->next != NULL)
	{
		temp = temp->next;
		enqueue(temp->dataItem);
	}
	
	return this;
}

 /**
 * Destructor. Deallocates the memory used to store the queue. Calls the clear method.
 *
 * @return This destructor does not return anything
 * @pre None
 * @post Deallocates the memory used to store the queue.
 */
template <class DataType>
QueueLinked<DataType>::~QueueLinked()
{
	clear();
}

 /**
 * Inserts a new item to the back of the queue.
 *
 * @param DataType& Data to be inserted into the queue
 * @return This function is void 
 * @pre Queue is not full
 * @post Inserts newDataItem at the rear of the queue
 */
template <class DataType>
void QueueLinked<DataType>::enqueue(const DataType& newDataItem) throw (logic_error)
{
	/*
	* Possible cases:
	* 0: Full list, throw error
	* 1: Empty List (front == back == NULL), create new front; set back to front
	* 2: 1 Element List (front == back != NULL), create a next for back, set back to its next, point front to back
	* 3: 2+ elements (front != back != NULL), create a new back, assign back to its next
	*/
	
	if(isFull())
	{
		throw logic_error("enqueue() while queue full");
	}
	
	if(isEmpty()) // Case 1
	{
		front = new QueueNode(newDataItem, NULL);
		back = front;
	}
	else if(front == back) // Case 2
	{
		back->next = new QueueNode(newDataItem, NULL);
		back = back->next;
		front->next = back;
	}
	else // Case 3
	{
		back->next = new QueueNode(newDataItem, NULL);
		back = back->next;
	}
}

/**
 * If the queue is not empty, remove the first item in the queue (from the front) and return it. Otherwise, an error is thrown.
 *
 * @return DataType The first item in the queue
 * @pre Queue is not empty
 * @post Removes the least recently added data item from the queue and returns it.
 */
template <class DataType>
DataType QueueLinked<DataType>::dequeue() throw (logic_error)
{
	/*
	* Possible Dequeue cases:
	* 0: Empty list (front == back == NULL), throw error
	* 1: 1 Element List (front == back != NULL), clear list and return the only item
	* 2: 2+ Element List (front != back != NULL), delete the front node and return data
	*/
	
	if(isEmpty()) // Case 0
	{
		throw logic_error("dequeue() while queue empty");
	}
	
	DataType returnData = front->dataItem;
	if(front == back) // Case 1
	{
		QueueNode* temp = front;
		back = front = NULL;
		delete temp;
	}
	else // Case 2
	{
		QueueNode* temp = front;
		front = front->next;
		delete temp;
	}
	return returnData;
}

/**
 * Deletes all items in the queue, freeing memory and making the queue empty.
 *
 * @return This method returns void.
 * @pre None.
 * @post This queue will be empty
 */
template <class DataType>
void QueueLinked<DataType>::clear()
{
	/*
	* Possible clear cases
	* 1: Empty List (front == back == NULL)
	* 2: 1 Element List (front == back != NULL)
	* 3: 2+ Element List (front != back != NULL)
	*/
	
	if(isEmpty()) // Case 1
	{
		// Do nothing, job's done
	}
	else if(front == back) // Case 2
	{
		delete front;
	}
	else // Case 3
	{
		QueueNode* temp = front;
		while(front != NULL)
		{
			front = front->next;
			delete temp;
			temp = front;
		}
	}
	front = back = NULL;
}

/**
 * Returns true if the queue is empty. Otherwise, returns false.
 *
 * @return bool Returns true if the queue has no items in it.
 * @pre None
 * @post The queue will be unchanged
 */
template <class DataType>
bool QueueLinked<DataType>::isEmpty() const
{
	return (front == NULL && back == NULL);
}

/**
 * Returns true if the queue is full. Otherwise, returns false.
 *
 * @param
 * @return bool Always returns false.
 * @pre None.
 * @post The queue will be unchanged
 */
template <class DataType>
bool QueueLinked<DataType>::isFull() const
{
	return false;
}

// Programming Exercise 2
/**
 * Inserts a new data item to the front of the queue without changing the order of the original queue.
 *
 * @param DataType& A new item to add to the queue
 * @return This function returns void.
 * @pre Queue is not full
 * @post newDataItem will be inserted at the front of the queue
 */
template <class DataType>
void QueueLinked<DataType>::putFront(const DataType& newDataItem) throw (logic_error)
{
	/*
	* Possible putFront cases:
	* 0: Queue is full, throw error
	* 1: List is empty (front == back == NULL), insert new item
	* 2: 1 Element List (front == back != NULL), set front to new item and point it to back
	* 3: 2+ Element List (front != back != NULL), create a new front, point it to old front, set front to new front
	*/
	if(isFull()) // Case 0
	{
		throw logic_error("putFront() while queue full");
	}
	
	if(isEmpty()) // Case 1
	{
		enqueue(newDataItem);
	}
	else if(front == back) // Case 2
	{
		front->dataItem = newDataItem;
		front->next = back;
	}
	else // Case 3
	{
		QueueNode* newFront = new QueueNode(newDataItem, front);
		front = newFront;
	}
}

/**
 * Removes the last item from the queue and returns it.
 *
 * @return DataType Returns the last item in the queue
 * @pre Queue is not empty.
 * @post Removes the last item from the queue and returns it.
 */
template <class DataType>
DataType QueueLinked<DataType>::getRear() throw (logic_error)
{
	/*
	* Possible cases for getRear:
	* 0: Queue is empty (front == back == NULL), throw error
	* 1: 1 Element List (front == back != NULL), clear list and return its only element
	* 2: 2+ Element List (front != back != NULL), delete back, reassign back to the new back, and return the original back's value
	*/
	
	if(isEmpty()) // Case 0
	{
		throw logic_error("getRear() while queue is empty");
	}
	
	DataType returnData = back->dataItem;
	if(front == back) // Case 1
	{
		clear();
	}
	else // Case 2
	{
		QueueNode* newBack = front;
		while(newBack->next != back)
		{
			newBack = newBack->next;
		}
		delete back;
		back = newBack;
	}
	return returnData;
}

// Programming Exercise 3
/**
 * Counts and returns the number of items in the queue.
 *
 * @return int Returns the number of items in the queue
 * @pre None
 * @post The order and items in the queue will be unchanged.
 */
template <class DataType>
int QueueLinked<DataType>::getLength() const
{
	/*
	* Possible cases for getLength:
	* 1: Queue is empty (front == back == NULL), return 0
	* 2: 1 Element Queue (front == back != NULL), return 1
	* 3: 2+ Element Queue (front != back != NULL), traverse through list and return number of items in queue
	*/
	
	if(isEmpty()) // Case 1
	{
		return 0;
	}
	else if(front == back) // Case 2
	{
		return 1;
	}
	else // Case 3
	{
		QueueNode* temp = front;
		int count = 0;
		while(temp != NULL)
		{
			temp = temp->next;
			count++;
		}
		
		return count;
	}
}

/**
 * Outputs the data items in the queue. If the queue is empty, outputs "Empty Queue". This operation is intended only for testing/debugging purposes, and only supports queue data items that are one of C++'s predefined data types (int, char, etc) or other data structures with an overriden ostream operator<<.
 *
 * @return This function returns void
 * @pre None
 * @post The order and items in the queue will be unchanged.
 */
template <typename DataType>
void QueueLinked<DataType>::showStructure () const

// Linked list implementation. Outputs the elements in a queue. If
// the queue is empty, outputs "Empty queue". This operation is
// intended for testing and debugging purposes only.

{
    // Iterates through the queue
	QueueNode* p;

    if ( isEmpty() )
	cout << "Empty queue" << endl;
    else
    {
	cout << "Front\t";
	for ( p = front ; p != NULL ; p = p->next )
	{
	    if( p == front )
	    {
		cout << '[' << p->dataItem << "] ";
	    }
	    else
	    {
		cout << p->dataItem << " ";
	    }
	}
	cout << "\trear" << endl;
    }
}

/**
 * Sets the data and next pointer for a new queue node 
 *
 * @param DataType& Value of data for this node 
 * @param QueueNode* Value of pointer for this node
 * @return This constructor does not return anything 
 * @pre None
 * @post Creates a new node with the nodeData and nextPtr provided
 */
template <typename DataType>
QueueLinked<DataType>::QueueNode::QueueNode(const DataType& nodeData, QueueNode* nextPtr)
{
	dataItem = nodeData;
	next = nextPtr;
}