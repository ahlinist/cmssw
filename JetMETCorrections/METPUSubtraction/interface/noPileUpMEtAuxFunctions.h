#ifndef JetMETCorrections_METPUSubtraction_noPileUpMEtAuxFunctions_h
#define JetMETCorrections_METPUSubtraction_noPileUpMEtAuxFunctions_h

#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/Common/interface/OneToManyWithQuality.h"
#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

typedef edm::AssociationMap<edm::OneToManyWithQuality<reco::VertexCollection, reco::PFCandidateCollection, float> > 
  PFCandidateToVertexAssociationMap;
typedef edm::AssociationMap<edm::OneToManyWithQuality<reco::PFCandidateCollection, reco::VertexCollection, float> >
  reversedPFCandidateToVertexAssociationMap;

int isVertexAssociated(const reco::PFCandidate&, const PFCandidateToVertexAssociationMap&, const reco::VertexCollection&, double);

reversedPFCandidateToVertexAssociationMap reversePFCandToVertexAssociation(const PFCandidateToVertexAssociationMap&);

int isVertexAssociated_fast(const reco::PFCandidateRef&, const reversedPFCandidateToVertexAssociationMap&, const reco::VertexCollection&, double, int&, int);

#endif
