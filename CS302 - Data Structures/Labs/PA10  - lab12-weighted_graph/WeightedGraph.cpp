/** @file WeightedGraph.cpp
@author Tim Kwist
@version 1.0
@brief This program will implement a Weighted Graph
@details The specifications of this program are defined by C++ Data Structures: A Laboratory Course (3rd edition) by Brandle, JGeisler, Roberge, Whittington, lab 12.
@date Wednesday, November 21, 2014
*/


#include <stdexcept>
#include <iostream>
#include <climits>
#include <string>
#include "WeightedGraph.h"
using namespace std;
/**
 * Constructor for Weighted Graph. Allocates enough memory for a graph containing maxNumber vertices
 *
 * @param maxNumber : Number of vertices allowed in this graph
 * @return none
 * @pre none
 * @post Graph will have memory allocated to it and maxSize will be set
*/
WeightedGraph::WeightedGraph (int maxNumber)
{
	size = 0;
	maxSize = maxNumber;
	vertexList = new Vertex[maxSize];
	adjMatrix = new int[maxSize*maxSize];
	for(int i = 0; i < maxSize; i++)
	{
		for(int j = 0; j < maxSize; j++)
		{
			adjMatrix[i*maxSize + j] = infiniteEdgeWt;
		}
	}
	pathMatrix = new int[maxSize*maxSize];
}

/**
 * Copy Constructor for Weighted Graph.
 *
 * @param other : Weighted which will be copied into this graph
 * @return None
 * @pre None
 * @post This graph will be equivalent to other
*/
WeightedGraph::WeightedGraph ( const WeightedGraph& other )
{
	maxSize = other.maxSize;
	size = other.size;
	vertexList = new Vertex[maxSize];
	adjMatrix = new int[maxSize*maxSize];
	for(int i = 0; i < maxSize; i++)
	{
		vertexList[i] = other.vertexList[i];
		for(int j = 0; j < maxSize; j++)
		{
			adjMatrix[i*maxSize + j] = other.adjMatrix[i*maxSize + j];
			pathMatrix[i*maxSize + j] = other.pathMatrix[i*maxSize + j];
		}
	}
}

/**
 * Overloaded assignment operator for Weighted Graph
 *
 * @param other : Weighted Graph which will be copied into this graph
 * @return WeightedGraph& : A copy of this graph
 * @pre None
 * @post This graph will be equivalent to other
*/
WeightedGraph& WeightedGraph::operator= ( const WeightedGraph& other )
{
	if(this != &other)
	{
		clear();
		maxSize = other.maxSize;
		size = other.size;
		vertexList = new Vertex[maxSize];
		adjMatrix = new int[maxSize*maxSize];
		for(int i = 0; i < maxSize; i++)
		{
			vertexList[i] = other.vertexList[i];
			for(int j = 0; j < maxSize; j++)
			{
				adjMatrix[i*maxSize+j] = other.adjMatrix[i*maxSize+j];
				pathMatrix[i*maxSize + j] = other.pathMatrix[i*maxSize + j];
			}
	
		}
	}
	
	return *this;
}

/**
 * Destructor for Weighted Graph
 *
 * @param None
 * @return None
 * @pre None
 * @post Memory of the graph will be deallocated
*/
WeightedGraph::~WeightedGraph ()
{
	clear();
}

/**
 * Insert newVertex into a graph. If it already exists in the graph, then updates it.
 *
 * @param newVertex : the new vertex to be inserted into the graph
 * @return None
 * @pre Graph is not full
 * @post newVertex will be inserted or will be updated (if it already exists in the graph)
 * @throws logic_error : Throws "The Graph is full" is the graph is full
*/
void WeightedGraph::insertVertex ( const Vertex& newVertex ) throw ( logic_error )
{		
	for(int i = 0; i < size; i++)
	{
		if(vertexList[i].getLabel() == newVertex.getLabel())
		{
			vertexList[i] = newVertex;
			for(int j = 0; j < size; j++)
			{
				setEdge(i, j, infiniteEdgeWt);
			}
			return;
		}
	}
	
	if(isFull())
	{
		throw logic_error("The Graph is full");
	}
	vertexList[size++] = newVertex;
	for(int i =0; i < size; i++)
	{
		setEdge(size, i, infiniteEdgeWt);
	}
}

/**
 * Inserts an undirected edge connecting vertices v1 and v2 into the graph. The weight of the edge is wt. If there is already an edge connecting these vertices, then updates the weight of the edge.
 *
 * @param v1 : vertice 1
 * @param v2 : vertice 2
 * @param wt : weight of the edge
 * @return none
 * @pre Graph includes vertices v1 and v2.
 * @post Inserts an undirected edge connecting vertices v1 and v2 into the graph. The weight of the edge is wt. If there is already an edge connecting these vertices, then updates the weight of the edge.
 * @throws logic_error : If graph does not included vertices v1 and v2, through "Vertex not found" error.
*/
void WeightedGraph::insertEdge ( const string& v1, const string& v2, int wt ) throw ( logic_error )
{
	int v1Index = -1;
	int v2Index = -1;
	for(int i = 0; i < size; i++)
	{
		if(vertexList[i].getLabel() == v1)
		{
			v1Index = i;
		}
		else if(vertexList[i].getLabel() == v2)
		{
			v2Index = i;
		}
	}
	if((v1Index == -1) || (v2Index == -1)) // if either of them are still false, then we know that vertex is not in the graph so throw error
	{
		throw logic_error("Vertex not found");
	}
	setEdge(v1Index, v2Index, wt);
	cout << "Size = " << size << ", idx_v1 = " << v1Index << ", idx_v2 = " << v2Index << endl;
}

/**
 * Searches a graph for vertex v. If this vertex is found, then places the value of the vertex's data in vData and returns true. Otherwise, returns false with vData undefined.
 *
 * @param v : vertex string label to look for
 * @param vData : if the given vertex is found, it will be copied into this reference variable
 * @return bool : true if the given vertex is in the graph, false otherwise
 * @pre None
 * @post If this vertex is found, then places the value of the vertex's data in vData and returns true. Otherwise, returns false with vData undefined.
*/
bool WeightedGraph::retrieveVertex ( const string& v, Vertex& vData ) const
{
	for(int i = 0; i < size; i++)
	{
		if(vertexList[i].getLabel() == v)
		{
			vData = vertexList[i];
			return true;
		}
	}
	
	return false;
}

/**
 * Searches the graph for the edge connecting vertices v1  and v2. If this edge exists, then places the weight of the edge in wt and returns true. Otherwise, returns false with wt undefined.
 *
 * @param v1 : vertice 1 of edge to search for
 * @param v2 : vertice 2 of the edge to search for
 * @param wt : If the given edge is found, its weight will be copied into this reference variable
 * @return  bool : If the edge exists, true; else false
 * @pre Graph includes vertices v1 and v2
 * @post If this edge exists, then places the weight of the edge in wt and returns true. Otherwise, returns false with wt undefined.
*/
bool WeightedGraph::getEdgeWeight ( const string& v1, const string& v2, int& wt ) const throw ( logic_error )
{
	int v1Index = -1;
	int v2Index = -1;
	for(int i = 0; i < size; i++)
	{
		if(vertexList[i].getLabel() == v1)
		{
			v1Index = i;
		}
		else if(vertexList[i].getLabel() == v2)
		{
			v2Index = i;
		}
	}
	if((v1Index == -1) || (v2Index == -1)) // if either of them are still false, then we know that vertex is not in the graph so throw error
	{
		throw logic_error("Vertex not found");
	}
	
	if(adjMatrix[v1Index*maxSize + v2Index] != infiniteEdgeWt)
	{
		wt = adjMatrix[v1Index*maxSize + v2Index];
		return true;
	}
		
	return false;
}

/**
 * Removes vertex v from the graph and any edges connected to v.
 *
 * @param v : vertex to be removed
 * @return none
 * @pre Graph includes vertex v
 * @post Removes vertex v from the graph and any edges connected to v.
 * @throws logic_error : If the given vertex does not exist in the graph, throw a logic_error
*/
void WeightedGraph::removeVertex ( const string& v ) throw ( logic_error )
{
	for(int i = 0; i < size; i++ )
	{
		if(vertexList[i].getLabel() == v)
		{
			// Shift adjacency matrix one to the left
			for(int j = i; j < size - 1; j++)
			{
				for(int k = 0; k < size; k++)
				{
					adjMatrix[k*maxSize + j] = adjMatrix[k*maxSize + (j + 1)];
				}
			}
			// Shift adjacency matrix one to the south
			for(int j = 0; j < size - 1; j++)
			{
				for(int k = i; k < size; k++)
				{
					adjMatrix[k*maxSize + j] = adjMatrix[(k + 1)*maxSize + j];
				}
			}
			// Move all the vertex's in the vertexList one to the left
			for(int j = i; j < size; j++)
			{
				vertexList[j] = vertexList[j+1];
			}
			size--;
			return;
		}
	}
	
	throw logic_error("Vertex not found");
}

/**
 * Removes the edge connecting vertices v1 and v2 from the graph.
 *
 * @param v1 : vertice one of edge to remove
 * @param v2 : vertice two of edge to remove
 * @return None
 * @pre Graph includes vertices v1 and v2
 * @post Removes the edge connecting vertices v1 and v2 from the graph.
*/
void WeightedGraph::removeEdge ( const string& v1, const string& v2 ) throw ( logic_error )
{
	int v1Index = -1;
	int v2Index = -1;
	for(int i = 0; i < size; i++)
	{
		if(vertexList[i].getLabel() == v1)
		{
			v1Index = i;
		}
		if(vertexList[i].getLabel() == v2)
		{
			v2Index = i;
		}
	}
	
	if((v1Index == -1) || (v2Index == -1))
	{
		throw logic_error("Not Found");
	}
	
	adjMatrix[v1Index*maxSize + v2Index] = infiniteEdgeWt;
	adjMatrix[v2Index*maxSize + v1Index] = infiniteEdgeWt;
}

/**
 * Removes all the vertices and edges in the graph.
 *
 * @param None
 * @return None
 * @pre None
 * @post Removes all the vertices and edges in the graph.
*/
void WeightedGraph::clear ()
{
	size = 0;
	delete [] vertexList;
	delete [] adjMatrix;
	delete [] pathMatrix;
}

/**
 * Returns true if the graph is empty (no vertices). Otherwise, returns false.
 *
 * @param None
 * @return bool : Returns true if the graph is empty (no vertices). Otherwise, returns false.
 * @pre None
 * @post Contents of graph will be unaffected
*/
bool WeightedGraph::isEmpty () const
{
	return size == 0;
}

/**
 * Returns true if the graph is full (cannot add any more vertices). Otherwise, returns false.
 *
 * @param None
 * @return bool : Returns true if the graph is full (cannot add any more vertices). Otherwise, returns false.
 * @pre None
 * @post Contents of graph will be unaffected
*/
bool WeightedGraph::isFull () const
{
	return size == maxSize;
}

/**
 * Outputs a graph's vertex list and adjacency matrix. This operation is intended for testing/debugging purposes only.
 *
 * @param None
 * @return None
 * @pre None
 * @post Contents of graph will be unaffected
*/
void WeightedGraph::showStructure () const
{
	if (size == 0) 
	{
		cout << "Empty graph" << endl;
	} else 
	{
		cout << endl << "Vertex list : " << endl;
		for (int row = 0; row < size; row++)
		{
			cout << row << '\t' << vertexList[row].getLabel();
			#if! LAB12_TEST2
			cout << '\t' << vertexList[row].getColor();
			#endif
			cout << endl;
		}
		cout << endl << "Edge matrix : " << endl << '\t';
		for (int col = 0; col < size; col++)
		{
			cout << col << '\t';
		}
		cout << endl;
		for (int row = 0; row < size; row++)
		{
			cout << row << '\t';
			for (int col = 0; col < size; col++)
			{
				int wt = getEdge(row,col);
				if (wt == infiniteEdgeWt)
				{
					cout << "- \t";
				}
				else
				{
					cout << wt << '\t';
				}
			}
			cout << endl;
		}
	}
}

/**
 * Computes and displays the graph's path matrix
 * If there exists a path (a to c) in which there are subsequent paths (a to b) and (b to c), in which ((a to b) + (b to c)) < (a to c), replace a to c with ((a to b) + (b + c)).
 *
 * @param None
 * @return None
 * @pre None
 * @post Contents of the graph will be unaffected
*/
void WeightedGraph::showShortestPaths () const
{
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			setPath(i, j, getEdge(i, j));
		}
		setPath(i, i, 0);
	}
	for(int b = 0; b < size; b++)
	{
		for(int a = 0; a < size; a++)
		{
			for(int c = 0; c < size; c++)
			{
				if((getPath(a, b) != infiniteEdgeWt) && (getPath(b, c) != infiniteEdgeWt))
				{
					if((getPath(a, b) + getPath(b, c) < getPath(a, c)) || (getPath(a, c) == infiniteEdgeWt))
					{
						setPath(a, c, (getPath(a, b) + getPath(b, c)));
					}
				}
			}
		}
	}
	
    cout << endl << "Path matrix : " << endl << '\t';
    for (int col = 0; col < size; col++)
	{
        cout << col << '\t';
	}
    cout << endl;
    for (int row = 0; row < size; row++)
    {
        cout << row << '\t';
        for (int col = 0 ; col < size ; col++)
        {
            int wt = getPath(row, col);
            if (wt == infiniteEdgeWt)
			{
				cout << "- \t";
			}
            else
			{
				cout << wt << '\t';
			}
        }
        cout << endl;
    }
}

/**
 * Returns true if no vertex in the graph has the same color as an adjacent vertex. Otherwise, returns false.
 *
 * @param None
 * @return bool : Returns true if no vertex in the graph has the same color as an adjacent vertex. Otherwise, returns false.
 * @pre All vertices have been assigned a color
 * @post The contents of the graph will be unaffected
*/
bool WeightedGraph::hasProperColoring() const
{
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			if((getEdge(i, j) != infiniteEdgeWt) && (vertexList[i].getColor() == vertexList[j].getColor()) && (i != j))
			{
				return false;	
			}
		}
	}
	return true;
}

/**
 * Returns true if every vertex in a graph is of even degree. Otherwise, return false.
 *
 * @param None
 * @return Returns true if every vertex in a graph is of even degree. Otherwise, return false.
 * @pre None
 * @post The contents of this graph will be unaffected
*/
bool WeightedGraph::areAllEven () const
{
	int numEdges = 0;
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			if(adjMatrix[i*maxSize + j] != infiniteEdgeWt)
			{
				numEdges++;
			}
		}
		if(numEdges % 2 != 0)
		{
			return false;
		}
		numEdges = 0;
	}
	return true;
}

/**
 * Return the index of the given string label, if it exists in the graph.
 *
 * @param v : vertex label to search for
 * @return int : index of string label if found; -1 otherwise
 * @pre None
 * @post Contents of this graph will not be affected
*/
int WeightedGraph::getIndex ( const string& v ) const
{
	for(int i = 0; i < size; i++ )
	{
		if(vertexList[i].getLabel() == v )
		{
			return i;
		}
	}
	return -1;
}

/**
 * Return the edge associated with the given row and column
 *
 * @param row : row of edge being searched for 
 * @param col : col of edge being searched for
 * @return int : weight of the edge being searched for
 * @pre row and col must be less than maxSize (ie, within the boundaries)
 * @post The contents of this graph will be unaffected
*/
int WeightedGraph::getEdge ( int row, int col ) const
{
	if((row < maxSize) && (col < maxSize))
	{
		return adjMatrix[row*maxSize + col];
	}
	return -1;
}

/**
 * Sets the weight of the given edge
 *
 * @param row : row of the edge whose weight is being set
 * @param col : column of the edge whose weight is being set
 * @param wt : weight of the edge at (row, col) in the adjacency matrix
 * @return None
 * @pre Row and col must be less than maxSize (ie, within the boundaries)
 * @post The edge at the given row, col wil be set with the given weight
*/
void WeightedGraph::setEdge ( int row, int col, int wt)
{
	if((row < maxSize) && (col < maxSize))
	{
		adjMatrix[row*maxSize + col] = wt;
		adjMatrix[col*maxSize + row] = wt;
	}
}

/**
 * Get the path from the path matrix at row, col
 *
 * @param row : row of the path being retrieved
 * @param col : column of the path being retried
 * @return int : path at given row, col
 * @pre Row and column are less than maxSize (ie , within boundaries)
 * @post Contents of this graph will be unaffected
*/
int WeightedGraph::getPath(int row, int col) const
{
	if((row < maxSize) && (col < maxSize))
	{
		return pathMatrix[row*maxSize + col];
	}
	return -1;
}

/**
 * Sets the path with the given weight at the given row, col
 *
 * @param row : row of the path being set 
 * @param col : column of the path being set
 * @param wt : weight of the path being set
 * @return none
 * @pre row and col are less than maxSize (ie, within boundaries)
 * @post The contents of this graph will be unaffected
*/
void WeightedGraph::setPath(int row, int col, int wt) const
{
	if((row < maxSize) && (col < maxSize))
	{
		pathMatrix[row*maxSize + col] = wt;
		pathMatrix[col*maxSize + row] = wt;
	}
}