
/// A container of morphisms mapping a single Quaero particle to zero or more Quaero particles
/** 
 */

#ifndef __QuaeroParticleMorphismsSingle
#define __QuaeroParticleMorphismsSingle

#include <vector>
#include <string>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstTurboSim/interface/MultiDimensionalBinaryForest.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroParticle.hh"


// T1 - search key (usually searching for a particle with a particle)
// T2 - particle class (value that is returned)
// T1 == T2
template<class T1, class T2>
class QuaeroParticleMorphismsSingle
{

   struct Node {
   public:
      std::vector<T2> particles;
      std::string comment;
   };

public:

   QuaeroParticleMorphismsSingle();
   ~QuaeroParticleMorphismsSingle() {
     //cout << "MorphismsSingle Destructor Called. " << endl;
      clear();
     //cout << "MorphismsSingle Destructor Done. " << endl;
   }
   
   void Add( const T1 &key, 
	     const std::vector< T2 > &value, 
	     double zvtx, 
	     std::string comment = "" );
   
   /// Find the entry closest to key, and return the corresponding value
   /// corresponding value is a vector of particles ( type T2 )
   std::vector<T2> findClosest( const T1 &key, 
				double zvtx, 
				std::string &comment);
   std::vector<T2> findClosest( const T1 &key, 
				double zvtx, 
				std::string &comment,
				std::string &searchTrace );
   void clear();

   double bushiness( const T1 & );

private:
  
   std::vector<double> getKey( const T1& quaeroParticle, 
			       double zvtx );

   void shimmy( T2& quaeroParticle, 
		const std::vector<double>& targetKey, 
		const std::vector<double>& closestKey, 
		double zvtx );
   
   MultiDimensionalBinaryForest<std::string, double, Node > forest;

   enum { DEPENDENT, INDEPENDENT, PERIODIC };
   std::map< std::string, pair< int, double > > phiDependencies;

};

#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsSingle.ii"

#endif

