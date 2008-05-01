/****************************************
A MultiDimensionalBinaryTree is just like the usual binary search tree, 
but generalized to d dimensions.  

The first level is sorted on the first of these variables, the second
level on the second, . . . , the d^th level on the d^th variable,
the (d+1)^th level on the 1st variable, and so forth.

Here is a picture for d=3: 

1            ^
2        /       \
3      /   \   /   \
1     / \ / \ / \ / \
2    /\  /\ /\  /   /\

Each node has both a key (which determines the node's placement within the tree)
and a value (which can be anything).

A MultiDimensionalBinaryTree 

Bruce Knuteson 2003
******************************************/


#ifndef __MultiDimensionalBinaryTree
#define __MultiDimensionalBinaryTree


#include <vector>
#include <sstream>
#include <string>
#include "Validation/VstMathUtils/interface/Math.hh"
//#include "Validation/VstTurboSim/interface/xtoa.hh"

template<class T1, class T2>
class MultiDimensionalBinaryTree
{
public:
   
   /// Default constructor
   MultiDimensionalBinaryTree();
   
   /** 
    * Constructor
    * Inputs:
    *   _dimensionality:  dimensionality of the keys in the tree
    *            _level:  the level of the tree at this node
    *              _key:  the key at this node
    *            _value:  the value at this node
    */
   MultiDimensionalBinaryTree( int _dimensionality, 
			       int _level, 
			       std::vector<T1> _key, 
			       T2 _value );

   /// Deconstructor
   ~MultiDimensionalBinaryTree();

   // Usage
   void Add( std::vector<T1> key, 
	     T2 value );
   T2 findClosest( std::vector<T1>& targetKey ) const;
   void clear();
   
   // Debug Functions
   void bushiness( int &, int & ) const;
   T2 findClosest( std::vector<T1>& targetKey,
		   std::string &treeTrace ) const; 
   
   
private:

   int d, level;
   MultiDimensionalBinaryTree<T1,T2> *left;
   MultiDimensionalBinaryTree<T1,T2> *right;
   std::vector<T1> key;
   T2 value;

};

#include "Validation/VstTurboSim/interface/MultiDimensionalBinaryTree.ii"

#endif

