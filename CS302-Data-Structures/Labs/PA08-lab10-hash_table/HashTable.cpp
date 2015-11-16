/** @file HashTable.cpp
@author Tim Kwist
@version 1.0
@brief This program will implement a Hash Table
@details The specifications of this program are defined by C++ Data Structures: A Laboratory Course (3rd edition) by Brandle, JGeisler, Roberge, Whittington, lab 10.
@date Wednesday, October 29, 2014
*/
#include "HashTable.h"

using namespace std;

/**
 * HashTable constructor.
 * Sets table size, and initializes dataTable as an array of size tableSize of BSTrees
 *
 * @param initTableSize : Table size
 * @return none
 * @pre none
 * @post tableSize will be set, and dataTable will be initialized
*/
template <typename DataType, typename KeyType>
HashTable<DataType, KeyType>::HashTable(int initTableSize)
{
	tableSize = initTableSize;
	dataTable = new BSTree<DataType, KeyType>[tableSize];
}

/**
 * HashTable copy constructor
 * Utilizes copyTable method to do its dirty work.
 *
 * @param other : HashTable of which we are creating a copy of
 * @return None
 * @pre None
 * @post This HashTable will have the same contents as our other parameter
*/
template <typename DataType, typename KeyType>
HashTable<DataType, KeyType>::HashTable(const HashTable<DataType, KeyType>& other)
{
	copyTable(other);
}

/**
 * HashTable Operator = Overload
 * If the HashTables are already equal, don't do anything.
 * Otherwise, clear out this HashTable and use copyTable to do our dirty work.
 *
 * @param other : HashTable of which we are settings ourselves equal to
 * @return HashTable& : This HashTable
 * @pre None
 * @post This HashTable will be equal to our other parameter
*/
template <typename DataType, typename KeyType>
HashTable<DataType, KeyType>& HashTable<DataType, KeyType>::operator=(const HashTable<DataType, KeyType>& other)
{
	if(this != &other)
	{
		clear();
		copyTable(other);
	}
	return *this;
}

/**
 * Deallocate all the memory in this HashTable.
 * Use the clear method to do our dirty work.
 *
 * @param None
 * @return None
 * @pre None
 * @post This HashTable will be free of memory
*/
template <typename DataType, typename KeyType>
HashTable<DataType, KeyType>::~HashTable()
{
	clear();
}

/**
 * Insert a new data item into the HashTable.
 * Determine the index of this item based on its hash modulus'd by table size.
 * Then use BSTree's insert method on the index selected
 *
 * @param newDataItem : New data item to be inserted into the HashTable
 * @return None
 * @pre None
 * @post The newDataItem will be a part of this HashTable
*/
template <typename DataType, typename KeyType>
void HashTable<DataType, KeyType>::insert(const DataType& newDataItem)
{
	int index = newDataItem.hash(newDataItem.getKey()) % tableSize;
	dataTable[index].insert(newDataItem);
}

/**
 * Remove a delete key from the HashTable if it exists within the table.
 * Use BSTree's remove method on each element of our table.
 * If the BSTree remove method returns true (meaning it deleted a key), return true.
 * Otherwise, if we get through all of the elements in the table, return false.
 *
 * @param deleteKey : key that we are looking to delete from this table
 * @return bool : true if we successfully deleted the key, false if the key does not exist in this HashTable
 * @pre None
 * @post If the deleteKey passed in exists in the list, it will be removed from the list. Otherwise, everything in the list will not be modified.
*/
template <typename DataType, typename KeyType>
bool HashTable<DataType, KeyType>::remove(const KeyType& deleteKey)
{
	for(int i = 0; i < tableSize; i++)
	{
		if(dataTable[i].remove(deleteKey))
		{
			return true;
		}
	}
	return false;
}

/**
 * Retrieve a data item from this HashTable based on a search key.
 * For each element in the table, use BSTree's retrieve method. If this ever returns true, this method will return true.
 * Otherwise, if the loop goes through all elements in the table, return false - indicating that the searchKey does not exist in this HashTable.
 *
 * @param searchKey : Key being searched through our HashTable for
 * @param returnItem : Data Item that will either be unmodified if the searchKey cannot be found, or will be changed to the dataItem that corresponds to the searchKey if it is found.
 * @return bool : True if the searchKey exists in this HashTable, false if the searchKey does not exist in this HashTable
 * @pre None
 * @post The contents of this HashTable will not be modified.
*/
template <typename DataType, typename KeyType>
bool HashTable<DataType, KeyType>::retrieve(const KeyType& searchKey, DataType& returnItem) const
{
	for(int i = 0; i < tableSize; i++)
	{
		if(dataTable[i].retrieve(searchKey, returnItem))
		{
			return true;
		}
	}
	return false;
}

/**
 * Clears all the data/memory of this HashTable / deallocates all the memory of this HashTable.
 * Goes through each element in the array and uses BSTree's clear method on each element.
 *
 * @param None
 * @return None
 * @pre None
 * @post The memory of this HashTable will be deallocated.
*/
template <typename DataType, typename KeyType>
void HashTable<DataType, KeyType>::clear()
{
	for(int i = 0; i < tableSize; i++)
	{
		dataTable[i].clear();
	}
}

/**
 * Returns whether or not this Hashtable is empty by checking whether the dataTable is NULL or not.
 *
 * @param None
 * @return bool : True if dataTable is NULL, false if dataTable is not NULL
 * @pre None
 * @post The contents of this HashTable will not be changed.
*/
template <typename DataType, typename KeyType>
bool HashTable<DataType, KeyType>::isEmpty() const
{
	return dataTable == NULL;
}

/**
 * Shows the structure of this HashTable.
 * Goes through each element of the HashTable and uses BSTree's writeKeys method.
 *
 * @param None
 * @return None
 * @pre None
 * @post The contents of this HashTable will be unchanged.
*/
template <typename DataType, typename KeyType>
void HashTable<DataType, KeyType>::showStructure() const
{
    for (int i = 0; i < tableSize; ++i)
	{
		cout << i << ": ";
		dataTable[i].writeKeys();
    }
}

/**
 * Copies the contents of a HashTable into this HashTable.
 * Set this tableSize to other tableSize.
 * Allocate a new dataTable of size tableSize.
 * Go through each element in each HashTable and copy contents.
 *
 * @param source : other HashTable to copy contents of
 * @return None
 * @pre None
 * @post This HashTable will have the same contents as the source HashTable.
*/
template <typename DataType, typename KeyType>
void HashTable<DataType, KeyType>::copyTable(const HashTable<DataType, KeyType>& source)
{
	tableSize = source.tableSize;
	dataTable = new BSTree<DataType, KeyType>[tableSize];
	for(int i = 0; i < tableSize; i++)
	{
		dataTable[i] = source.dataTable[i];
	}
}