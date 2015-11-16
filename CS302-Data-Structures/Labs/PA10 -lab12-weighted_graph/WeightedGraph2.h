//--------------------------------------------------------------------
//
//  Laboratory 13, In-lab Exercise 1                      wtgraph2.h
//
//  Class declaration for the adjacency matrix implementation of
//  the Weighted Graph ADT -- including a path matrix
//
//--------------------------------------------------------------------

#include <stdexcept>
#include <iostream>
#include <climits>    // For INT_MAX
#include <string>     // Used for labels

using namespace std;

const int defMaxGraphSize = 10,      // Default number of vertices
          vertexLabelLength = 11,    // Length of a vertex label
          infiniteEdgeWt = INT_MAX;   // "Weight" of a missing edge

//--------------------------------------------------------------------

class Vertex
{
  public:

    char label [vertexLabelLength];   // Vertex label
};

//--------------------------------------------------------------------

class WeightedGraph
{
  public:

    // Constructor
    WeightedGraph ( int maxNumber = defMaxGraphSize );
    WeightedGraph ( const WeightedGraph& other );
    WeightedGraph& operator= ( const WeightedGraph& other );

    // Destructor
    ~WeightedGraph ();

    // Graph manipulation operations
    void insertVertex ( Vertex newVertex )            // Insert vertex
        throw ( logic_error );
    void insertEdge ( char *v1, char *v2, int wt )    // Insert edge
        throw ( logic_error );
    bool retrieveVertex ( char *v, Vertex &vData ) const;
                                                      // Get vertex
    int edgeWeight ( char *v1, char *v2, int &wt ) const
        throw ( logic_error );                        // Get edge wt.

    bool getEdgeWeight ( char *v1, char *v2, int &wt ) const
        throw ( logic_error );                        // Get edge wt.
    void removeVertex ( char *v )                     // Remove vertex
        throw ( logic_error );
    void removeEdge ( char *v1, char *v2 )            // Remove edge
        throw ( logic_error );
    void clear ();                                    // Clear graph

    void computePaths();   // Computes path matrix

    // Graph status operations
    bool isEmpty () const;                            // Graph is empty
    bool isFull () const;                             // Graph is full

    // Output the graph structure -- used in testing/debugging
    void showStructure () const;

  private:

    // Facilitator functions
    int getIndex ( char *v ) const;            // Converts vertex label to an
                                               //   adjacency matrix index
    int  getEdge ( int row, int col ) const;   // Get edge weight using
                                               //   adjacency matrix indices
    int  getPath ( int row, int col ) const;   // Get path weight using
                                               //   path matrix indices
    void setEdge ( int row, int col, int wt);  // Set edge weight using
                                               //   adjacency matrix indices
    void setPath ( int row, int col, int wt);  // Set path weight using
                                               //   path matrix indices

    // Data members
    int maxSize,          // Maximum number of vertices in the graph
        size;             // Actual number of vertices in the graph
    Vertex *vertexList;   // Vertex list
    int *adjMatrix;       // Adjacency matrix
    int *pathMatrix;      // Path matrix
};
