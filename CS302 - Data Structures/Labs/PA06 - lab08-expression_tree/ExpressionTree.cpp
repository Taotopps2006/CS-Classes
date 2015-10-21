/** @file ExpressionTree.cpp
@author Tim Kwist
@version 1.0
@brief This program will implement an Expression Tree
@details The specifications of this program are defined by C++ Data Structures: A Laboratory Course (3rd edition) by Brandle, JGeisler, Roberge, Whittington, exercise 8.
@date Wednesday, October 8, 2014
*/

#include <ExpressionTree.h>
#include <stdlib.h> 

using namespace std;


/**
 * Constructor for Expression Tree
 *
 * @param None
 * @return None
 * @pre None
 * @post Creates an empty expression tree.
*/
template <typename DataType>
ExprTree<DataType>::ExprTree ()
{
	root = NULL;
}

/**
 * Copy constructor for Expression Tree
 *
 * @param source Expression tree that this expression tree will become a copy of
 * @return None
 * @pre None
 * @post Initializes the expression tree to be equivalent to the other ExprTree object parameter.
*/
template <typename DataType>
ExprTree<DataType>::ExprTree(const ExprTree& source)
{
	root = NULL;
	copy(root, source.root);
}

/**
 * Assignment operator overload for Expression Tree
 *
 * @param source Expression tree that this expression tree will become a copy of
 * @return ExprTree& Reference to this object
 * @pre None
 * @post Sets the expression tree to be equivalent to the other ExprTree object parameter. Will not change any part of source parameter
*/
template <typename DataType>
ExprTree<DataType>& ExprTree<DataType>::operator=(const ExprTree& source)
{
	if(this != &source)
	{
		clear();
		copy(root, source.root);
	}
	return *this;
}

/**
 * Helper function for copy constructor and assignment operator. Copies the contents, recursively, from one expression tree to this one
 *
 * @param None
 * @return None
 * @pre None
 * @post Creates an empty expression tree.
*/
template <typename DataType>
void ExprTree<DataType>::copy(ExprTreeNode*& current, ExprTreeNode* source)
{
	
	if(source != NULL)
	{
		current = new ExprTreeNode(source->dataItem, NULL, NULL);
		copy(current->left, source->left);
		copy(current->right, source->right);
	}
	else
	{
		current = NULL;
	}
}

/**
 * Deconstructor for Expression Tree
 *
 * @param None
 * @return None
 * @pre None
 * @post Deallocates the memory used to store this expression tree
*/
template <typename DataType>
ExprTree<DataType>::~ExprTree ()
{
	clear();
}

/**
 * Builds the Expression Tree from user input
 *
 * @param None
 * @return None
 * @pre None
 * @post Builds an expression tree according to keyboard input
*/
template <typename DataType>
void ExprTree<DataType>::build ()
{
	char currChar;
	cin >> currChar;
	root = new ExprTreeNode(currChar, NULL, NULL);
	if((currChar == '+') || (currChar == '-') || (currChar == '*') || (currChar == '/'))
		buildHelper(root);
}

/**
 * Recursive helper function for build. 
 *
 * @param node current node for which to put current input into
 * @return None
 * @pre None
 * @post The Expression Tree will be built from user input
*/
template <typename DataType>
void ExprTree<DataType>::buildHelper(ExprTreeNode*& node)
{
	char currChar;
	cin >> currChar;
	node->left = new ExprTreeNode(currChar, NULL, NULL);
	// If we have a second operator, we'll call the buildHelper to recursively build our tree
	// If we just have numbers or anything else, just set the right node and we're done
	if((currChar == '+') || (currChar == '-') || (currChar == '*') || (currChar == '/'))
	{
		buildHelper(node->left);
		
	}
	cin >> currChar;
	node->right = new ExprTreeNode(currChar, NULL, NULL);
	if((currChar == '+') || (currChar == '-') || (currChar == '*') || (currChar == '/')) // If the right char is an operator, call the buildHelper again
	{
		buildHelper(node->right);
	}
	
}

/**
 * Outputs the expression corresponding to the values in the tree in fully parenthesized infix form.
 *
 * @param None
 * @return None
 * @pre None
 * @post The contents of this expression tree will not be changed. An expression corresponding to the value of the tree in fully parenthesized infix form will be outputted.
*/
template <typename DataType>
void ExprTree<DataType>::expression () const
{
	if(root != NULL)
	{
		expressionHelper(root);
	}
}

/**
 * Recursive helper function for expression. Outputs data item if number; otherwise outputs parenthesis before recursively outputting the numbers of the tree
 *
 * @param p Expression Tree Node currently being looked at to be outputted
 * @return None
 * @pre None
 * @post The contents of the expression tree will not be changed
*/
template <typename DataType>
void ExprTree<DataType>::expressionHelper(const ExprTreeNode* p) const
{

	if(isdigit(p->dataItem))
	{
		cout << p->dataItem;
	}
	else
	{
		cout << "(";
		expressionHelper(p->left);
		cout << p->dataItem;
		expressionHelper(p->right);
		cout << ")";
	}
}

/**
 * Evaluate the expression contained within the expression tree.
 *
 * @param None
 * @return Datatype value of the corresponding arithmetic expression contained within this Expression Tree
 * @pre Expression tree is not empty.
 * @post The contents of this expression tree will not be changed. The value of the corresponding arithmetic expression will be returned.
 * @throws logic_error Throws error if tree is empty
*/
template <typename DataType>
DataType ExprTree<DataType>::evaluate() const throw (logic_error)
{
	if(root == NULL)
		throw logic_error("Tree is empty");
	return evalHelper(root);
}

/**
 * Helper function for evaluate. Recursively goes through the tree, choosing the appropriate mathematical operations based on found operands.
 *
 * @param p Current Expression Tree Node being looked at
 * @return DataType Value of the corresponding arithmetic expression contained within this Expression Tree
 * @pre None
 * @post The contents of this expression tree will not be changed
*/
template <typename DataType>
DataType ExprTree<DataType>::evalHelper(const ExprTreeNode* p) const
{

	if(isdigit(p->dataItem))
	{
		return atoi(&(p->dataItem));
	}
	
	// Choose appropriate arithmetic operation
	if(p->dataItem == '+')
	{
		return (evalHelper(p->left) + evalHelper(p->right));
	}
	
	if(p->dataItem == '-')
	{
		return (evalHelper(p->left) - evalHelper(p->right));
	}
	
	if(p->dataItem == '*')
	{
		return (evalHelper(p->left) * evalHelper(p->right));
	}
	
	if(p->dataItem == '/')
	{
		return (evalHelper(p->left) / evalHelper(p->right));
	}
	
	
}

/**
 * Deallocate memory assigned to this tree and remove its contents.
 *
 * @param None
 * @return None
 * @pre None
 * @post All the data items in this expression tree will be removed.
*/
template <typename DataType>
void ExprTree<DataType>::clear()
{
	if(root != NULL) // If root is NULL, no need doing anything else
	{
		clearHelper(root);
	}
}

/**
 * Recursively assist the clear method by deleting nodes one by one. Go as far as left as possible, go as far right as possible, delete the child nodes, then go back to the previous stack call and delete the child nodes which now have no other children
 *
 * @param None
 * @return None44
 * @pre None
 * @post All the data items in this expression tree will be removed.
*/
template <typename DataType>
void ExprTree<DataType>::clearHelper(ExprTreeNode*& p)
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
 * Apply the commutative property to the expression tree
 *
 * @param None
 * @return None
 * @pre None
 * @post The contents of the expression tree will be swapped around according to the commutative property
*/
template <typename DataType>
void ExprTree<DataType>::commute()
{
	commuteHelper(root);
}
/**
 * Recursive helper function for commute. 
 *
 * @param p Current Expression Tree Node being looked at to be swap the contents of
 * @return None
 * @pre None
 * @post The contents of the expression tree will be swapped around according to the commutative property
*/
template <typename DataType>
void ExprTree<DataType>::commuteHelper(ExprTreeNode*& p)
{
	if(p != NULL) // Base case: node is NULL
	{
		// Swap left and right, then use commute helper on left and right
		ExprTreeNode* tmp = p->left;
		p->left = p->right;
		p->right = tmp;
		commuteHelper(p->left);
		commuteHelper(p->right);
	}
}

/**
 * Checks whether a given expression tree is equivalent to the current expression tree. This means that the overall value of the tree is equivalent, and the children nodes are equal. The order of child nodes may be different if the overall expression is not changed. IE 1 + 3 is equivalent to 3 + 1, but 3 - 1 is not equivalent to 1 - 3.
 *
 * @param source Another expression tree to which this expression tree is being compared to
 * @return True if both trees are equivalent, otherwise false.
 * @pre None
 * @post The contents of both trees will not be changed.
*/
template <typename DataType>
bool ExprTree<DataType>::isEquivalent(const ExprTree& source) const
{
	if(root == NULL || source.root == NULL) // If either nodes are NULL, compare them and return the result. Cannot reach the data of either as there is no guarantee that either is not NULL and could result in segfault.
	{
		return root == source.root;
	}
	if(root->dataItem == source.root->dataItem) // If the root data item and source data item are equivalent, there is a chance the rest are equal; else, return false immediately.
	{
		return isEquivalentHelper(root, source.root);
	}
	return false;
}

/**
 * Recursively assist the isEquivalent method. 
 * 
 * @param p current node of this Expression Tree being compared to the other Expression Tree Node
 * @param other current node of the other Expression Tree being compared to this Expression Tree Node
 * @return True if both trees are equivalent, otherwise false
 * @pre None
 * @post The contents of both trees will not be changed.
*/
template <typename DataType>
bool ExprTree<DataType>::isEquivalentHelper(ExprTreeNode* p, ExprTreeNode* other) const
{
/*
 * Base cases:
 * 1. Current item is a number: return whether current items are equal
 * 2. Current item is an operator and the left side, right side, or both, is an operator: check whether left = other left and right = other right, or left = other right and right = other left then go another level deeper
 * 3. Current item is an operator, but both sides are numbers: if the operator is division or subtraction, left must equal other left and right must equal other right; otherwise, left can equal other left or other right if right equals other right or other left, respectively.
 */
	if(isdigit(p->dataItem))
	{
		return (p->dataItem == other->dataItem);
	}
	if(!(isdigit(p->left->dataItem)) || !(isdigit(p->right->dataItem)))
	{
		if((p->left->dataItem == other->left->dataItem) && (p->right->dataItem == other->right->dataItem))
		{
			return ((isEquivalentHelper(p->left, other->left)) && (isEquivalentHelper(p->right, other->right)));
		}
		else if((p->left->dataItem == other->right->dataItem) && (p->right->dataItem == other->left->dataItem))
		{
			return ((isEquivalentHelper(p->left, other->right)) && (isEquivalentHelper(p->right, other->left)));
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(p->dataItem == '-' || p->dataItem == '/')
		{
			return (((p->left->dataItem == other->left->dataItem) && (p->right->dataItem == other->right->dataItem))); // If division or subtraction, values must be in exact order
		}
		return (((p->left->dataItem == other->left->dataItem) && (p->right->dataItem == other->right->dataItem)) || ((p->left->dataItem == other->right->dataItem) && (p->right->dataItem == other->left->dataItem)));
	}
}

/**
 * Checks if the current Exprssion Tree is empty
 *
 * @param None
 * @return True if tree is empty, false otherwise
 * @pre None
 * @post The contents of this expression tree will not be changed
*/
template <typename DataType>
bool ExprTree<DataType>::isEmpty() const
{
	return root == NULL;
}

/**
 * Outputs an express tree with its branches oriented from left (root) to right (leaves) - that is, the tree output is rotated counterclockwise ninety degrees from its conventional orientation. If the tree is empty, outputs "Empty Tree". Note that this operation is intended for testing/debugging purposes only. It assumes that arithmetic expressions contain only single-digit, nonnegative integers and the arithmetic operators for addition, subtraction, mutiplication, and division.
 *
 * @param None
 * @return None
 * @pre None
 * @post The contents of this expression tree will not be modified. The expression tree will be outputted.
*/
template <typename DataType>
void ExprTree<DataType>:: showStructure () const

// Outputs an expression tree. The tree is output rotated counter-
// clockwise 90 degrees from its conventional orientation using a
// "reverse" inorder traversal. This operation is intended for testing
// and debugging purposes only.

{
    if ( isEmpty() )
       cout << "Empty tree" << endl;
    else
    {
       cout << endl;
       showHelper(root,1);
       cout << endl;
    }
}

/**
 * Outputs an express tree with its branches oriented from left (root) to right (leaves) - that is, the tree output is rotated counterclockwise ninety degrees from its conventional orientation. If the tree is empty, outputs "Empty Tree". Note that this operation is intended for testing/debugging purposes only. It assumes that arithmetic expressions contain only single-digit, nonnegative integers and the arithmetic operators for addition, subtraction, mutiplication, and division.
 *
 * @param None
 * @return None
 * @pre None
 * @post The contents of this expression tree will not be modified. The expression tree will be outputted.
*/
template <typename DataType>
void ExprTree<DataType>:: showHelper ( ExprTreeNode *p, int level ) const

// Recursive helper for the showStructure() function. Outputs the
// subtree whose root node is pointed to by p. Parameter level is the
// level of this node within the expression tree.

{
     int j;   // Loop counter

     if ( p != 0 )
     {
        showHelper(p->right,level+1);        // Output right subtree
        for ( j = 0 ; j < level ; j++ )   // Tab over to level
            cout << "\t";
        cout << " " << p->dataItem;        // Output dataItem
        if ( ( p->left != 0 ) &&          // Output "connector"
             ( p->right != 0 ) )
           cout << "<";
        else if ( p->right != 0 )
           cout << "/";
        else if ( p->left != 0 )
           cout << "\\";
        cout << endl;
        showHelper(p->left,level+1);         // Output left subtree
     }
}

/**
 * Outputs an express tree with its branches oriented from left (root) to right (leaves) - that is, the tree output is rotated counterclockwise ninety degrees from its conventional orientation. If the tree is empty, outputs "Empty Tree". Note that this operation is intended for testing/debugging purposes only. It assumes that arithmetic expressions contain only single-digit, nonnegative integers and the arithmetic operators for addition, subtraction, mutiplication, and division.
 *
 * @param None
 * @return None
 * @pre None
 * @post The contents of this expression tree will not be modified. The expression tree will be outputted.
*/
template <typename DataType>
ExprTree<DataType>::ExprTreeNode::ExprTreeNode ( char elem, ExprTreeNode *leftPtr, ExprTreeNode *rightPtr )
{
	dataItem = elem;
	left = leftPtr;
	right = rightPtr;
}