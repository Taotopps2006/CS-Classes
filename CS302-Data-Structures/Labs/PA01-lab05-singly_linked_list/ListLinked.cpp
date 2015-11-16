#include "ListLinked.h"

using namespace std;

template <typename DataType>
List<DataType>::List(int ignored)
{
	cursor = NULL;
	head = NULL;
}

template <typename DataType>
List<DataType>::List(const List& other)
{
	head = cursor = NULL;
	List* temp = other;
	temp->gotoBeginning();
	while (temp->cursor->next != NULL)
	{
		insert(temp->cursor->dataItem);
		temp->gotoNext();
	}
	// gotoBeginning();
}

template <typename DataType>
List<DataType>& List<DataType>::operator=(const List& other)
{
	
	if (!isEmpty())
	{
		clear();
	}

	List<DataType> temp = other;
	temp.gotoPrior();
	ListNode previous = NULL;
	previous.dataItem = temp.getCursor();
	temp.gotoBeginning();
	while (temp.cursor.next != NULL)
	{
		insert(temp.cursor.dataItem);
		temp.gotoNext();
	}
	gotoBeginning();
	while (getCursor() != previous.dataItem)
	{
		gotoNext();
	}

	return this;
	
	/*
	clear();
	ListNode* temp = other.head;
	while (temp != NULL)
	{
		insert(temp->dataItem);
		temp = temp->next;
	}
	cursor = temp;
	gotoBeginning();
	head = cursor;
	return this;
	*/
}

template <typename DataType>
List<DataType>::~List()
{
	clear();
}

template <typename DataType>
bool List<DataType>::insert(const DataType& newDataItem)
{
	if (isFull())
	{
		return false;
	}

	if (isEmpty()) // If at the beginning of the list, set the first node
	{
		head = new ListNode(newDataItem, NULL);
		cursor = head;
	}
	else // Otherwise, place new node after current node and set cursor to the new node
	{
		if (cursor->next != NULL) // If cursor already has a next, make sure we give this item a way to access it
		{
			cursor->next = new ListNode(newDataItem, cursor->next->next);
		}
		else // Otherwise we are at the end and the new item can have a null next
		{
			cursor->next = new ListNode(newDataItem, NULL);
		}

		cursor = cursor->next;
	}
	
	return true;
}

template <typename DataType>
bool List<DataType>::remove()
{
	if (isEmpty())
	{
		return false;
	}
	/* Three possible cases: 
	   #1: Cursor is at beginning of list, and the head will need to be reassigned
	   #2: Cursor is in the middle of the list and needs to be set to the next item after removal
	   #3: Cursor is at the end of the list and needs to be set to the head of the list after removal
	*/

	// Check for case #1
	if (head == cursor)
	{
		if(cursor->next != NULL)
		{
			head = cursor->next;
		}
		else
		{
			delete head;
			head = NULL;
		}
		delete cursor;
		cursor = head;
	}
	else if (cursor->next != NULL) // Check for case #2
	{
		ListNode* temp = cursor->next;
		gotoPrior();
		delete cursor->next;
		cursor->next = temp;
		gotoNext();
	}
	else // By default case #3
	{
		gotoPrior();
		delete cursor->next;
		cursor->next = NULL;
		gotoBeginning();
	}

	return true;
}

template <typename DataType>
bool List<DataType>::replace(const DataType& newDataItem)
{
	if (isEmpty())
	{
		return false;
	}

	cursor->dataItem = newDataItem;

	return true;
}

template <typename DataType>
void List<DataType>::clear()
{
	while (remove())
	{

	}
}

template <typename DataType>
bool List<DataType>::isEmpty() const
{
	if (head == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <typename DataType>
bool List<DataType>::isFull() const
{
	return false;
}

template <typename DataType>
bool List<DataType>::gotoBeginning()
{
	if (isEmpty())
	{
		return false;
	}

	cursor = head;
	return true;
}

template <typename DataType>
bool List<DataType>::gotoEnd()
{
	if (isEmpty())
	{
		return false;
	}

	while (gotoNext())
	{
	}

	return true;
}

template <typename DataType>
bool List<DataType>::gotoNext()
{
	if (cursor->next == NULL)
	{
		return false;
	}
	
	cursor = cursor->next;
	return true;
}

template <typename DataType>
bool List<DataType>::gotoPrior()
{
	if (head == cursor)
	{
		return false;
	}

	ListNode* temp = cursor;
	cursor = head;
	while (cursor->next != temp)
	{
		gotoNext();
	}

	return true;
}

template <typename DataType>
DataType List<DataType>::getCursor() const
{
	if (isEmpty())
	{
		terminate();
	}

	return cursor->dataItem;
}


// Programming exercise 2
template <typename DataType>
bool List<DataType>::moveToBeginning()
{
	if (isEmpty())
	{
		return false;
	}

	if (head == cursor) // If the cursor is currently at the beginning, removing it and putting it back at the beginning is a waste of operation
	{
		return true;
	}

	gotoPrior();
	ListNode* temp = cursor;
	gotoNext();
	temp->next = cursor->next;
	temp = cursor;
	temp->next = head;
	cursor = head = temp;
}


// Programming exercise 3
template <typename DataType>
bool List<DataType>::insertBefore(const DataType& newDataItem)
{
	if (isFull())
	{
		return false;
	}

	if (cursor != head)
	{
		gotoPrior();
		insert(newDataItem);
	}
	else
	{
		head = cursor = new ListNode(newDataItem, head);
	}

	return true;
}

template <typename DataType>
void List<DataType>::showStructure() const

// Outputs the items in a list. If the list is empty, outputs
// "Empty list". This operation is intended for testing and
// debugging purposes only.

{
    if ( isEmpty() )
    {
       cout << "Empty list" << endl;
    } 
    else
    {
	for (ListNode* temp = head; temp != NULL; temp = temp->next) {
	    if (temp == cursor) {
		cout << "[";
	    }

	    // Assumes that dataItem can be printed via << because
	    // is is either primitive or operator<< is overloaded.
	    cout << temp->dataItem;	

	    if (temp == cursor) {
		cout << "]";
	    }
	    cout << " ";
	}
	cout << endl;
    }
}

template<typename DataType>
List<DataType>::ListNode::ListNode(const DataType& nodeData, ListNode* nextPtr)
{
	dataItem = nodeData;
	next = nextPtr;
}
