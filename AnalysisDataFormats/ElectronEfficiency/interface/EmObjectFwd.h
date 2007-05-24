#ifndef EgammaEfficiency_ElectronFwd_h
#define EgammaEfficiency_ElectronFwd_h

#include <vector>
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObject.h"

namespace EgEff
{
   class EmObject;
   typedef std::vector<EmObject> EmObjectCollection;
   typedef edm::Ref<EmObjectCollection> EmObjectRef;
   typedef edm::RefProd<EmObjectCollection> EmObjectRefProd;
   typedef edm::RefVector<EmObjectCollection> EmObjectRefVector;
   typedef EmObjectRefVector::iterator emobject_iterator;
}

#endif

