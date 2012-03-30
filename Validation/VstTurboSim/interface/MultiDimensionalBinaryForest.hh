/**
 * A MultiDimensionalBinaryForest would be best 
 * implemented as a map< T0, MultiDimensionalBinaryTree >.
 * Practically, it stores the search tree for each
 * type of particle.
 */

#ifndef __MultiDimensionalBinaryForest
#define __MultiDimensionalBinaryForest

#include <vector>
#include <string>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstTurboSim/interface/MultiDimensionalBinaryTree.hh"

// T0 is treeName ( tree identifier ) type
template<class T0, class T1, class T2>
class MultiDimensionalBinaryForest {

public:

   MultiDimensionalBinaryForest() {}

   ~MultiDimensionalBinaryForest() { 
     cout << "Forest Destructor . . . " << flush;
      clear();
     cout << "Forest Destructed." << endl; 
   }

   void Add( T0 treeName, std::vector<T1> key, T2 value );
   T2 findClosest( T0 treeName, 
		   std::vector<T1>& targetKey ) const;
   void clear();

   // debuging functions
   T2 findClosest( const T0 &treeName, 
		   std::vector<T1>& targetKey,
		   std::string &searchTrace ) const;
   double bushiness( const T0 &treeName ) const;

   
private:

   std::vector< MultiDimensionalBinaryTree< T1, T2 > > trees;
   std::vector< T0 > treeNames;

};

#include "Validation/VstTurboSim/interface/MultiDimensionalBinaryForest.ii"

#endif

