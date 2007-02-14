#ifndef EgammaReco_MaterialConversionModules_Pi0MaterialConversionFwd_h
#define EgammaReco_MaterialConversionModules_Pi0MaterialConversionFwd_h
#include <vector>
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Common/interface/RefProd.h"

class Pi0MaterialConversion;
// collection of conversion objects.  This name is WAY too long.
typedef std::vector<Pi0MaterialConversion> Pi0MaterialConversionCollection;
 
// persistent reference to conversion objects
typedef edm::Ref<Pi0MaterialConversionCollection> Pi0MaterialConversionRef;
 
// reference to conversion collection
typedef edm::RefProd<Pi0MaterialConversionCollection> Pi0MaterialConversionRefProd;
 
// vector of references to conversion objects all in the same collection
typedef edm::RefVector<Pi0MaterialConversionCollection> Pi0MaterialConversionRefVector;
 
// iterator over a vector of references to BasicCluster objects
typedef Pi0MaterialConversionRefVector::iterator Pi0MaterialConversion_iterator;
#endif
