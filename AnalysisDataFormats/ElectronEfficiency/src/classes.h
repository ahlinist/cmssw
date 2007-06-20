#include "DataFormats/Common/interface/Wrapper.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObject.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/TagProbeAssociation.h"
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

      EgEff::TagProbeAssociationCollection c2;
      edm::Wrapper<EgEff::TagProbeAssociationCollection> w2;
      EgEff::TagProbeAssociation val;
      EgEff::TagProbeAssociationRef vr2;
      EgEff::TagProbeAssociationRefProd vrp1;
      EgEff::TagProbeAssociationRefVector vrv1;
   }
}
