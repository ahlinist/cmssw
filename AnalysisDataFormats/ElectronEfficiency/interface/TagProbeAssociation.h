#ifndef ElectronEfficiency_TagProbeAssociation_h
#define ElectronEfficiency_TagProbeAssociation_h

#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObjectFwd.h"
#include "DataFormats/Common/interface/AssociationMap.h"

namespace EgEff
{
   typedef edm::AssociationMap<edm::OneToMany<EmObjectCollection, EmObjectCollection> > TagProbeAssociationCollection;
   typedef TagProbeAssociationCollection::value_type TagProbeAssociation;

   typedef edm::Ref<TagProbeAssociationCollection> TagProbeAssociationRef;
   typedef edm::RefProd<TagProbeAssociationCollection> TagProbeAssociationRefProd;
   typedef edm::RefVector<TagProbeAssociationCollection> TagProbeAssociationRefVector;
}

#endif

