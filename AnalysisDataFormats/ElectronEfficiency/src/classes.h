#include "DataFormats/Common/interface/Wrapper.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObject.h"
#include "DataFormats/Common/interface/RefToBase.h"

namespace
{
   namespace
   {
      EgEff::EmObjectCollection c1;
      edm::Wrapper<EgEff::EmObjectCollection> w1;
      edm::Ref<EgEff::EmObjectCollection> r1;
      edm::RefProd<EgEff::EmObjectCollection> rp1;
      edm::RefVector<EgEff::EmObjectCollection> rv1;
   }
}
