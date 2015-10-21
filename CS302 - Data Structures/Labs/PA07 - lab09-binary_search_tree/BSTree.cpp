/** @file BSTree.cpp
@author Tim Kwist
@version 1.0
@brief This program will implement a Binary Search Tree
@details The specifications of this program are defined by C++ Data Structures: A Laboratory Course (3rd edition) by Brandle, JGeisler, Roberge, Whittington, lab 9.
@date Wednesday, October 8, 2014
*/

#include "BSTree.h"

using namespace std;

/**
 * Default constructor for BSTree
 *
 * @param None
 * @return None
 * @pre None
 * @post Creates an empty binary search tree
*/
template <typename DataType, typename KeyType> 
BSTree<DataType, KeyType>::BSTree()
{
	root = NULL;
}

/**
 * Copy constructor for BSTree
 *
 * @param None
 * @return None
 * @pre None
 * @post Initializes the binary search tree to be equivalent to the other BSTree object parameter
*/
template <typename DataType, typename KeyType> 
BSTree<DataType, KeyType>::BSTree(const BSTree<DataType, KeyType>& other)
{
	root = NULL;
	copy(root, other.root);
}

/**
 * Overloaded assignment operator for BSTree
 *
 * @param other BSTree object to be set equal to
 * @return BSTree A reference to this BSTree object
 * @pre None
 * @post Sets the binary search tree to be equivalent to the other BSTree object parameter and returns a reference to this object
*/
template <typename DataType, typename KeyType> 
BSTree<DataType, KeyType>& BSTree<DataType, KeyType>::operator=(const BSTree<DataType, KeyType>& other)
{
	if(this != &other)
	{
		clear();
		copy(root, other.root);
	}
	return *this;
}

/**
 * Helper function for copy constructor and assignment operator. Copies the contents, recursively, from one BSTree to this one
 *
 * @param current Current node of this BSTree
 * @param other Current node of other BSTree
 * @return None
 * @pre None
 * @post Copies other BSTree to current BSTree
*/
template <typename DataType, typename KeyType> 
void BSTree<DataType, KeyType>::copyHelper(BSTreeNode*& current, BSTreeNode* other)
{
	
	if(other != NULL)
	{
		current = new BSTreeNode(other->dataItem, NULL, NULL);
		copy(current->left, other->left);
		copy(current->right, other->right);
	}
	else
	{
		current = NULL;
	}
}

/**
 * Destructor for BSTree
 *
 * @param None
 * @return None
 * @pre None
 * @post Deallocates (frees) the memory used to store the binary search tree.
*/
template <typename DataType, typename KeyType> 
BSTree<DataType, KeyType>::~BSTree()
{
	clear();
}

/**
 * Inserts newDataItem into the binary search tree. If a data item with the same key as newDataItem already exists in the tree, then updates that data item with newDataItem.
 *
 * @param newDataItem item to be inserted into binary search tree
 * @return None
 * @pre None
 * @post Another item will be added to this binary search tree if the passed in data is new.
*/
template <typename DataType, typename KeyType> 
void BSTree<DataType, KeyType>::insert(const DataType& newDataItem)
{
	insertHelper(root, newDataItem);
}

/**
 * Recursive helper for insert method.
 * If null, create new node
 * Else continue down tree until we find a null node
 * If data greater than current val, go down right
 * If data less than current val, go down left
 * Inserts newDataItem into the binary search tree. If a data item with the same key as newDataItem already exists in the tree, then updates that data item with newDataItem.
 *
 * @param p current node being evaluated for whether or not to insert into
 * @param newDataItem item to be inserted into binary search tree
 * @return None
 * @pre None
 * @post Another item will be added to this binary search tree if the passed in data is new.
*/
template <typename DataType, typename KeyType> 
void BSTree<DataType, KeyType>::insertHelper(BSTreeNode*& p, const DataType& newDataItem)
{
	// If ptr is NULL, create a new node and break
	if(p == NULL)
	{
		p = new BSTreeNode(newDataItem, NULL, NULL);
		return;
	}
	// If ptr data is greater than value, go down left tree
	if(p->dataItem.getKey() > newDataItem.getKey())
	{
		insertHelper(p->left, newDataItem);
	}
	else if(p->dataItem.getKey() < newDataItem.getKey()) // If ptr data is less than value, go down right tree
	{
		insertHelper(p->right, newDataItem);
	}
	else	// If ptr data is equal to value, update data item with new DataItem; ie, don't do anything
	{
		p->dataItem = newDataItem;
	}
}

/**
 * Searches the binary search tree for the data item with key searchKey. If this data item is found, then copies the data item to searchDataItem and return true. Otherwise returns false with searchDataItem
 *
 * @param searchKey key to be searched for
 * @param searchDataItem data to be updated if key found
 * @return True if data item found; false otherwise
 * @pre None
 * @post The contents of this tree will not be changed
*/
template <typename DataType, typename KeyType> 
bool BSTree<DataType, KeyType>::retrieve(const KeyType& searchKey, DataType& searchDataItem) const
{
	retrieveHelper(root, searchKey, searchDataItem);
}

/**
 * Recursive helper function for retrieve.
 * If current value greater than search key, go down left
 * If current value less than search key, go down right
 * If null, return false
 * Searches the binary search tree for the data item with key searchKey. If this data item is found, then copies the data item to searchDataItem and return true. Otherwise returns false with searchDataItem
 *
 * @param p current node being checked for if it is the searchKey
 * @param searchKey key to be searched for
 * @param searchDataItem data to be updated if key found
 * @return True if data item found; false otherwise
 * @pre None
 * @post The contents of this tree will not be changed
*/
template <typename DataType, typename KeyType> 
bool BSTree<DataType, KeyType>::retrieveHelper(BSTreeNode* p, const KeyType& searchKey, DataType& searchDataItem) const
{
	// If we came to a NULL node, the value isn't in the tree
	if(p == NULL)
	{
		return false;
	}
	
	// If the current node's data is greater than the search key, go down the left node
	if(p->dataItem.getKey() > searchKey)
	{
		retrieveHelper(p->left, searchKey, searchDataItem);
	}
	else if(p->dataItem.getKey() < searchKey) // Otherwise, if the current node's data is less than the search key and we go down the right node
	{
		retrieveHelper(p->right, searchKey, searchDataItem);
	}
	else // we found the item - ie, the current node holds the value - set searchDataItem and return true
	{
		searchDataItem = p->dataItem;
		return true;
	}
}

/**
 * Deletes the data item with the key deleteKey from the binary search tree. If this data item is found, then deletes it from the tree and returns true. Otherwise return false.
 *
 * @param deleteKey key to be deleted
 * @return True if item is found and deleted; false otherwise.
 * @pre None
 * @post The deleteKey will be deleted from the tree if it exists in the tree. Otherwise the tree will be unchanged.
*/
template <typename DataType, typename KeyType> 
bool BSTree<DataType, KeyType>::remove(const KeyType& deleteKey)
{
	removeHelper(root, deleteKey);
	
}

/**
 * Recursive helper function for remove
 * If no children, delete the node
 * If one child, set child to current position then delete node
 * If two children, find the predecessor to replace node, then delete node
 * Deletes the data item with the key deleteKey from the binary search tree. If this data item is found, then deletes it from the tree and returns true. Otherwise return false.
 *
 * @param p current node being looked for deletion
 * @param deleteKey key to be deleted
 * @return True if item is found and deleted; false otherwise.
 * @pre None
 * @post The deleteKey will be deleted from the tree if it exists in the tree. Otherwise the tree will be unchanged.
*/
template <typename DataType, typename KeyType> 
bool BSTree<DataType, KeyType>::removeHelper(BSTreeNode*& p, const KeyType& deleteKey)
{
	// If we have come to a NULL node, then the item is not in the tree and we are to return false
	if(p == NULL)
	{
		return false;
	}
	// First find the node
	if(p->dataItem.getKey() == deleteKey)
	{
		// If we find it, we have three possible cases
		// Case 1: 0 children; just delete the node
		if((p->left == NULL) && (p->right == NULL))
		{
			delete p;
			p = NULL;
			return true;
		}
		// Case 2: 1 child
		// 2a: Left is child
		if((p->left != NULL) && (p->right == NULL))
		{
			BSTreeNode* tmp = p;
			p = p->left;
			delete tmp;
			return true;
		}
	
		// 2a: Right is child
		if((p->left == NULL) && (p->right != NULL))
		{
			BSTreeNode* tmp = p;
			p = p->left;
			delete tmp;
			return true;
		}
		
		// Case 3: 2 children
		// If the code has gotten this far, we know this is the only possible choice so no need for if statement
		// Get predecessor to replace current
		BSTreeNode* tmp = p->left;
		while(tmp->right != NULL)
		{
			tmp = tmp->right;
		}
		p->dataItem = tmp->dataItem;
		return removeHelper(p->left, tmp->dataItem.getKey());
	}
	else // If we aren't currently at the item, go down the tree
	{
		// If the current node's data is greater than the delete key, go down the left node
		if(p->dataItem.getKey() > deleteKey)
		{
			removeHelper(p->left, deleteKey);
		}
		else // Otherwise, the current node's data is less than the delete key and we go down the right node
		{
			removeHelper(p->right, deleteKey);
		}
	}
	
}

/**
 * Outputs the keys of the data items in the binary search tree. The keys are output in ascending order on one line, separated by spaces.
 *
 * @param None
 * @return None
 * @pre None
 * @post The contents of this BSTree will be unchanged.
*/
template <typename DataType, typename KeyType> 
void BSTree<DataType, KeyType>::writeKeys() const
{
	if(!isEmpty())
	{
		writeKeysHelper(root);
		cout << endl;
	}
}

/**
* Recursive helper function for writeKeys
 * Outputs the keys of the data items in the binary search tree. The keys are output in ascending order on one line, separated by spaces.
 *
 * @param None
 * @return None
 * @pre None
 * @post The contents of this BSTree will be unchanged.
*/
template <typename DataType, typename KeyType> 
void BSTree<DataType, KeyType>::writeKeysHelper(const BSTreeNode* p) const
{
	if(p->left != NULL)
	{
		writeKeysHelper(p->left);
	}
	
	cout << p->dataItem.getKey() << " ";
	
	if(p->right != NULL)
	{
		writeKeysHelper(p->right);
	}
}

/**
 * Removes all the data items in the binary search tree.
 *
 * @param None
 * @return None
 * @pre None
 * @post This binary search tree will have nothing in it and all memory will be deallocated.
*/
template <typename DataType, typename KeyType> 
void BSTree<DataType, KeyType>::clear()
{
	if(!isEmpty()) // If root is NULL, no need doing anything else
	{
		clearHelper(root);
	}
}

/**
 * Recursively assist the clear method by deleting nodes one by one. Go as far as left as possible, go as far right as possible, delete the child nodes, then go back to the previous stack call and delete the child nodes which now have no other children
 *
 * @param None
 * @return None
 * @pre None
 * @post This binary search tree will have nothing in it and all memory will be deallocated.
*/
template <typename DataType, typename KeyType> 
void BSTree<DataType, KeyType>::clearHelper(BSTreeNode*& p)
{
	if(p->left != NULL) // Go as far left as possible
	{
		clearHelper(p->left);
	}
	if(p->right != NULL) // Go as far right as possible
	{
		clearHelper(p->right);
	}
	// We have established by this point in the code that both the left an right are NULL, so there are no children; kill the node
	delete p;
	p = NULL;
}

/**
 * Return if tree is empty or not.
 *
 * @param None
 * @return True if the binary search tree is empty. Otherwise, returns false.
 * @pre None 
 * @post The contents of this binary search tree will not be changed.
*/
template <typename DataType, typename KeyType> 
bool BSTree<DataType, KeyType>::isEmpty() const
{
	return root == NULL;
}

/**
 * Outputs the keys in a binary search tree. The tree is output rotated counterclockwie 90 degrees from its conventional orientation using a "reverse" inorder traversal. This operation is intended for testing and debugging purposes only.
 *
 * @param None
 * @return None
 * @pre None
 * @post The contents of this BSTree will be unchanged.
*/
template <typename DataType, typename KeyType> 
void BSTree<DataType, KeyType>::showStructure() const
{
	if(root == 0)
       cout << "Empty tree" << endl;
    else
    {
       cout << endl;
       showHelper(root,1);
       cout << endl;
    }
}


/**
 * Recursive helper for showStructure. Outputs the subtree whose root node is pointed to by p.
 *
 * @param p BSTreeNode currently being outputted
 * @param level the level of this node within the tree
 * @return None
 * @pre None
 * @post The contents of thi BSTree will be unchanged.
*/
template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>:: showHelper(BSTreeNode *p, int level) const
{
     int j;   // Loop counter

     if(p != 0)
     {
        showHelper(p->right,level+1);         // Output right subtree
        for(j = 0 ; j < level ; j++)    // Tab over to level
            cout << "\t";
        cout << " " << p->dataItem.getKey();   // Output key
        if((p->left != 0) &&           // Output "connector"
            (p->right != 0))
           cout << "<";
        else if(p->right != 0)
           cout << "/";
        else if(p->left != 0)
           cout << "\\";
        cout << endl;
        showHelper(p->left,level+1);          // Output left subtree
    }
}

/**
 * Calculate and return the height of the BSTree
 *
 * @param None
 * @return The height of the binary search tree
 * @pre None
 * @post The contents of this BSTree will be unchanged.
*/
template <typename DataType, typename KeyType> 
int BSTree<DataType, KeyType>::getHeight() const
{
	if(isEmpty())
	{
		return 0; // Empty means nothing in the tree, which means 0 items.
	}
	
	return getHeightHelper(root);
}

/**
 * Recursive helper function for getHeight
 * If left not NULL, go down left side; if right not NULL, go down right side. Once at the bottom, return 1, then as we go back up compare whether the left or right side has a higher value to determine what to return
 * Calculate and return the height of the BSTree
 *
 * @param None
 * @return The height of the binary search tree
 * @pre None
 * @post The contents of this BSTree will be unchanged.
*/
template <typename DataType, typename KeyType> 
int BSTree<DataType, KeyType>::getHeightHelper(const BSTreeNode* p) const
{
	int leftHeight = 0, rightHeight = 0;
	if(p->left != NULL)
	{
		leftHeight += getHeightHelper(p->left);
	}
	if(p->right != NULL)
	{
		rightHeight += getHeightHelper(p->right);
	}
	
	if(leftHeight > 0 || rightHeight > 0)
	{
		if(leftHeight > rightHeight)
		{
			return leftHeight + 1; // Left tree, plus one for current
		}
		else
		{
			return rightHeight + 1; // Right tree, plus one for current
		}
	}
	else
	{
		return 1; // Left and right are both null, only return 1 for the current node
	}
}

/**
 * Return the count of the number of data items in the binary search tree.
 *
 * @param None
 * @return Number of data items in the binary search tree
 * @pre None
 * @post The contents of this BSTree will be unchanged.
*/
template <typename DataType, typename KeyType> 
int BSTree<DataType, KeyType>::getCount() const
{
	if(isEmpty())
	{
		return 0; // Empty means nothing in the tree, which means 0 items.
	}
	
	return getCountHelper(root);
}

/**
 * Recursive helper function for getCount
 * Check every node on the left side, check every node on the right side. Add one for each that isn't null, return total.
 * Return the count of the number of data items in the binary search tree.
 *
 * @param None
 * @return Number of data items in the binary search tree
 * @pre None
 * @post The contents of this BSTree will be unchanged.
*/
template <typename DataType, typename KeyType> 
int BSTree<DataType, KeyType>::getCountHelper(const BSTreeNode* p) const
{
	int count = 1; // Start at 1 for this current node
	
	if(p->left != NULL)
	{
		count += getCountHelper(p->left);
	}
	if(p->right != NULL)
	{
		count += getCountHelper(p->right);
	}
	
	return count;
}

/**
 * Default constructor for BSTreeNode
 *
 * @param None
 * @return None
 * @pre None
 * @post Creates an empty binary search tree
*/
template <typename DataType, typename KeyType> 
BSTree<DataType, KeyType>::BSTreeNode::BSTreeNode(const DataType &nodeDataItem, BSTreeNode *leftPtr, BSTreeNode *rightPtr)
{
	dataItem = nodeDataItem;
	left = leftPtr;
	right = rightPtr;
}