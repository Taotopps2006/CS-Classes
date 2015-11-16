/** @file Heap.cpp
@author Tim Kwist
@version 1.0
@brief This program will implement a Heap
@details The specifications of this program are defined by C++ Data Structures: A Laboratory Course (3rd edition) by Brandle, JGeisler, Roberge, Whittington, lab 11.
@date Wednesday, November 5, 2014
*/

#include "Heap.h"

using namespace std;

/**
 * Constructor for Heap. Creates an empty heap. Allocates enough memory for a heap containing maxNumber data items.
 * Pseudocode: set maxSize equal to maxNumber, allocate memory for dataItems based on maxSize, and set size to 0
 *
 * @param maxNumber : The maximum number of elements that can be in this heap
 * @return None
 * @pre None
 * @post This heap will have a maxSize and size defined, and our heap will be initialized
*/
template <typename DataType, typename KeyType, typename Comparator>
Heap<DataType,KeyType,Comparator>::Heap(int maxNumber)
{
	maxSize = maxNumber;
	dataItems = new DataType[maxSize];
	size = 0;
}

/**
 * Copy constructor for Heap. Initializes the object to be an equivalent copy of other.
 * Pseudocode:
 * 1. Set maxSize equal to other's max size, size to other's max size, allocate memory to dataItems based on maxSize, and use a loop to traverse through our elements to set them equal to other's elements.
 *
 * @param 
 * @return 
 * @pre None
 * @post This heap will be equal to other
*/
template <typename DataType, typename KeyType, typename Comparator>
Heap<DataType,KeyType,Comparator>::Heap(const Heap<DataType,KeyType,Comparator>& other)
{
	maxSize = other.maxSize;
	dataItems = new DataType[maxSize];
	size = other.size;
	for(int i = 0; i < maxSize; i++)
	{
		dataItems[i] = other.dataItems[i];
	}
}

/**
 * Overloaded assignment operator for Heap. Sets the heap to be equivalent to the other Heap and returns a reference to this object.
 * Pseudocode:
 * 1. Check if this already equals the other. If it does, stop right there and just return
 * 2. Otherwise, clear the heap.
 * 3. Set maxSize equal to other's max size, size to other's max size, allocate memory to dataItems based on maxSize, and use a loop to traverse through our elements to set them equal to other's elements.
 * 4. Returns this.
 *
 * @param other : Another heap of which this heap will become a copy of
 * @return Heap : This
 * @pre None
 * @post This heap will be equal to other
*/
template <typename DataType, typename KeyType, typename Comparator>
Heap<DataType,KeyType,Comparator>& Heap<DataType,KeyType,Comparator>::operator=(const Heap& other)
{
	if(this != other)
	{
		clear();
		maxSize = other.maxSize;
		dataItems = new DataType[maxSize];
		size = other.size;
		for(int i = 0; i < maxSize; i++)
		{
			dataItems[i] = other.dataItems[i];
		}
	}
	
	return *this;
}

/**
 * Destructor for Heap. Deallocates (frees) the memory used to store the heap.
 * Pseudocode: Call the clear method
 *
 * @param None
 * @return None
 * @pre None
 * @post All memory being used by this heap will be deallocated
*/
template <typename DataType, typename KeyType, typename Comparator>
Heap<DataType,KeyType,Comparator>::~Heap()
{
	clear();
}

/**
 * Inserts newDataItem into the heap. Inserts this data item as the bottom rightmost data item in the heap and moves it upward until the properties that define a heap are restored.
 * Pseudocode:
 * 1. Check if full. If so, throw logic error. Otherwise, go to 2.
 * 2. Set dataItems[size] to parameter passed in
 * 3. Increment size (so that the index we just set is now valid within the heap)
 * 4. Call the reprioritize method
 *
 * @param newDataItem : new item to be inserted into the list
 * @return None
 * @pre Heap is not full
 * @post The newDataItem will be inserted, and the rest of the heap will be sorted based on this new insertion
 * @throws logic_error : Throws a logic error if the heap is full
*/
template <typename DataType, typename KeyType, typename Comparator>
void Heap<DataType,KeyType,Comparator>::insert(const DataType &newDataItem) throw (logic_error)
{
	if(isFull())
	{
		throw logic_error("Heap is full");
	}
	dataItems[size] = newDataItem;
	size++;
	reprioritize();
	
}

/**
 * Removes the data item with the highest priority (the root) from the heap and returns it. Replaces the root data item with the bottom rightmost data item and moves this data item downward until the properties that define a heap are restored.
 * Pseudocode: 
 * 1. If empty, throw logic error. Else go to 2.
 * 2. Swap root with last elements
 * 3. Decrement size
 * 4. Call reprioritize method
 * 5. Return item at size (that item is technically out of bounds of our heap, but it still exists in memory so utilize this)
 *
 * @param None
 * @return DataType Highest priority item / root
 * @pre Heap is not empty
 * @post Root will have been removed, and replaced, and everything will be sorted
 * @throws logic_error : Throws an error if the heap is empty
*/
template <typename DataType, typename KeyType, typename Comparator>
DataType Heap<DataType,KeyType,Comparator>::remove() throw (logic_error)
{
	if(isEmpty())
	{
		throw logic_error("Heap is empty");
	}
	swap(0, size-1);
	size--;
	reprioritize();
	return dataItems[size];
}

/**
 * This method will do a general reprioritization of the heap. That is to say, no matter whether an item was just inserted or just removed, this method will sort every part of the heap.
 * Pseudocode: while we aren't sorted, follow the below steps:
 * 1. Check whether our left is within the size bound. If it is, go to step 2. Otherwise, we know that we have reached the end of the list(whether that be a leaf node or the actual last node) and we can now break out of the loop.
 * 2. Check whether our right is within the size bound.
 * 3. If our right is within bound, compare the root to both the left and the right with comparator. If either of them returns false, then check the comparator of left versus right and switch the result of that with the root
 * 4. If our right isn't within bound, only compare the root to the left. If returns false, switch the left and root.
 * 5. If we have made any changes, reset our root to 0 - ie, go back to the beginning because a change made here may affect the structure farther up the heap.
 * 6. Otherwise, increment the root, meaning that we are checking the next set of nodes.
 * 7. Adjust the left and right accordingly (left = 2*root + 1, right = left + 1 OR right = 2*root + 2).
 *
 * @param None
 * @return None
 * @pre None
 * @post All elements of the array (within size) will be sorted correctly
*/
template <typename DataType, typename KeyType, typename Comparator>
void Heap<DataType, KeyType, Comparator>::reprioritize()
{
	int root = 0;
	int left = 1;
	int right = 2;
	bool sorted = false;
	int rootPriority;
	int leftPriority;
	int rightPriority;
	
	while(!sorted)
	{
		if(left < size) // If our current left node is within the size, we have not yet reached the point at which our root is a leaf
		{
			rootPriority = dataItems[root].getPriority();
			leftPriority = dataItems[left].getPriority();
			if(right < size) // Both our nodes are within our bounds, and therefore we can compare root, left, and right
			{
				rightPriority = dataItems[right].getPriority();
				if((comparator(leftPriority, rootPriority)) || (comparator(rightPriority, rootPriority))) // If either our left or right are out of order
				{
					if(comparator(rightPriority, leftPriority)) // Find out which is out of order
					{
						swap(root, right);
					}
					else
					{
						swap(root, left);
					}
					root = 0; // If we have found an error, send us back to node one to check everything all over again
				}
				else
				{
					root++; // If no error is found, increment root, indicating that we are ready to check the next set of nodes
				}
				
			}
			else // Otherwise, we only our left is within the bounds, then only compare the left and root
			{
				// Compare only left and root
				if(comparator(leftPriority, rootPriority)) // If our comparator condition returns false (ie, the tree is out of balance)
				{
					swap(root, left);
					root = 0; // If we have found an error, send us back to node one to check everything all over again
				}
				else
				{
					root++; // If no error is found, increment root, indicating that we are ready to check the next set of nodes
				}
			}
		}
		else // Otherwise, our root is a leaf, in which case we can deduce that the list is sorted (since the balanced nature of a heap requires that our loop would  check all nodes above the leaf before the root was a leaf)
		{
			sorted = true;
		}
		
		// If we haven't broken out of the loop, set our left and right for the next iteration
		left = 2*root + 1;
		right = left + 1;
	}
}

/**
 * Swaps two elements of the heap
 * Pseudocode: Put the data at element a in a temporary variable. Set a equal to b. Set b equal to the temporary variable.
 *
 * @param a : index of first item to swap
 * @param b : index of second item to swap
 * @return None
 * @pre None
 * @post The items at index a and b will be swapped.
*/
template <typename DataType, typename KeyType, typename Comparator>
void Heap<DataType,KeyType,Comparator>::swap(int a, int b)
{
	DataType tempA = dataItems[a];
	dataItems[a] = dataItems[b];
	dataItems[b] = tempA;
}

/**
 * Removes all the data items in the heap.
 * Pseudocode: delete the array. Set size to 0.
 *
 * @param None
 * @return None
 * @pre None
 * @post Size will be set to 0 and all memory deallocated from the heap
*/
template <typename DataType, typename KeyType, typename Comparator>
void Heap<DataType,KeyType,Comparator>::clear()
{
	delete[] dataItems;
	size = 0;
}

/**
 * Returns true if the heap is empty. Otherwise, returns false.
 * Pseudocode: return size == 0
 *
 * @param None
 * @return bool : true if heap is empty, false otherwise
 * @pre None
 * @post The elements of the heap will not be changed.
*/
template <typename DataType, typename KeyType, typename Comparator>
bool Heap<DataType,KeyType,Comparator>::isEmpty() const
{
	return size == 0;
}

/**
 * Returns true if the heap is full. Otherwise, returns false.
 * Pseudocode: return size == maxSize
 *
 * @param None
 * @return bool : True if heap is full, false otherwise
 * @pre None
 * @post The elements of the heap will not be changed.
*/
template <typename DataType, typename KeyType, typename Comparator>
bool Heap<DataType,KeyType,Comparator>::isFull() const
{
	return size == maxSize;
}

/**
 * Outputs the priorities of the data items in the heap in both array and tree form. The tree is output with its branches oriented from left (root) to right (leaves) - that is, the tree is output rotated counterclockwise ninety degrees from its conventional orientation. If the heap is empty, outputs "Empty heap". Note that this operation is intended for testing/debugging purposes only.
 *
 * @param None
 * @return None
 * @pre None
 * @post The contents of this heap will not be changed.
*/
template <typename DataType, typename KeyType, typename Comparator>
void Heap<DataType,KeyType,Comparator>::showStructure() const
{
    int j;   // Loop counter

    cout << endl;
    if ( size == 0 )
       cout << "Empty heap" << endl;
    else
    {
       cout << "size = " << size << endl;       // Output array form
       for ( j = 0 ; j < maxSize ; j++ )
           cout << j << "\t";
       cout << endl;
       for ( j = 0 ; j < size ; j++ )
           cout << dataItems[j].getPriority() << "\t";
       cout << endl << endl;
       showSubtree(0,0);                        // Output tree form
    }
}

/**
 * Helper function for the showStructure() function. Outputs the subtree (subheap) whose root is stored in dataItems[index]. Argument level is the level of this dataItems within the tree.
 *
 * @param None
 * @return None
 * @pre None
 * @post The contents of this heap will not be changed.
*/
template <typename DataType, typename KeyType, typename Comparator>
void Heap<DataType,KeyType,Comparator>::showSubtree(int index, int level) const
{
     int j;   // Loop counter

     if ( index < size )
     {
        showSubtree(2*index+2,level+1);        // Output right subtree
        for ( j = 0 ; j < level ; j++ )        // Tab over to level
            cout << "\t";
        cout << " " << dataItems[index].getPriority();   // Output dataItems's priority
        if ( 2*index+2 < size )                // Output "connector"
           cout << "<";
        else if ( 2*index+1 < size )
           cout << "\\";
        cout << endl;
        showSubtree(2*index+1,level+1);        // Output left subtree
    }
}

/**
 * Writes each level of the tree.
 * Pseudocode: 
 * 1. Use loop to go through entire tree (condition being that the "currentNode" is less than size).
 * 2. Check if the currentNode index is less than the index of the next level.
 * 3. If it is, output the currentNode and increment currentNode.
 * 4. Otherwise, output an endl and increase the nextLevel to the appropriate length (2*currentNode + 1)
 *
 * @param None
 * @return None
 * @pre None
 * @post None
*/
template <typename DataType, typename KeyType, typename Comparator>
void Heap<DataType,KeyType,Comparator>::writeLevels() const
{
	int currentNode = 0;
	int nextLevel = 1;
	while(currentNode < size)
	{
		if(currentNode < nextLevel)
		{
			cout << dataItems[currentNode].getPriority() << " ";
			currentNode++;
		}
		else
		{
			cout << endl;
			nextLevel = 2*currentNode + 1;
		}
		
	}
	cout << endl;
}