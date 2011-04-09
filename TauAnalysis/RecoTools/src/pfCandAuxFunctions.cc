#include "TauAnalysis/RecoTools/interface/pfCandAuxFunctions.h"

#include <TMath.h>

const double epsilon = 0.01;

std::vector<const reco::PFCandidate*> getPFCandidatesOfType(const reco::PFCandidateCollection& pfCandidates, 
							    reco::PFCandidate::ParticleType pfParticleType)
{
  std::vector<const reco::PFCandidate*> retVal;

  for ( reco::PFCandidateCollection::const_iterator pfCandidate = pfCandidates.begin();
	pfCandidate != pfCandidates.end(); ++pfCandidate ) {
    if ( pfCandidate->particleId() == pfParticleType ) retVal.push_back(&(*pfCandidate));
  }

  return retVal;
}

void getPileUpPFCandidates(const std::vector<const reco::PFCandidate*>& pfCandidates, 
			   const std::vector<reco::TrackBaseRef>& signalTracks, 
			   const reco::VertexCollection& vertices, double deltaZ, const reco::BeamSpot& bs,
			   std::vector<const reco::PFCandidate*>& pfNoPileUpCandidates, 
			   std::vector<const reco::PFCandidate*>& pfPileUpCandidates)
{
  std::vector<const reco::Vertex*> signalVertices;
  for ( std::vector<reco::TrackBaseRef>::const_iterator signalTrack = signalTracks.begin();
	signalTrack != signalTracks.end(); ++signalTrack ) {
    if ( signalTrack->isNonnull() && signalTrack->isAvailable() ) {
      const reco::Vertex* signalVertex = findVertex(*signalTrack, vertices, deltaZ, bs);
      if ( signalVertex != 0 ) signalVertices.push_back(signalVertex);
    }
  }

  for ( std::vector<const reco::PFCandidate*>::const_iterator pfCandidate = pfCandidates.begin();
	pfCandidate != pfCandidates.end(); ++pfCandidate ) {
    if ( (*pfCandidate)->trackRef().isNonnull() && (*pfCandidate)->trackRef().isAvailable() ) {
      reco::TrackBaseRef pfCandidateTrack((*pfCandidate)->trackRef());

      const reco::Vertex* pfCandidateVertex = findVertex(pfCandidateTrack, vertices, deltaZ, bs);

      bool isSignalVtx_associated = false;
      for ( std::vector<const reco::Vertex*>::const_iterator signalVertex = signalVertices.begin();
	    signalVertex != signalVertices.end(); ++signalVertex ) {
	if ( pfCandidateVertex == (*signalVertex) ) {
	  isSignalVtx_associated = true;
	  break;
	}
      }

      if ( isSignalVtx_associated ) {
	pfNoPileUpCandidates.push_back(*pfCandidate);
      } else { 
	pfPileUpCandidates.push_back(*pfCandidate);
      }
    }
  }
}

const reco::Vertex* findVertex(const reco::TrackBaseRef& signalTrack, const reco::VertexCollection& vertices, 
			       double deltaZ, const reco::BeamSpot& bs)
{
// CV: this code has been taken from PhysicsTools/PFCandProducer/src/PFPileUp.cc (04/05/2011)

  const reco::Vertex* retVal = 0;

  if ( !(signalTrack.isNonnull() && signalTrack.isAvailable()) ) return retVal;

//--- find vertex associated to track
  for ( reco::VertexCollection::const_iterator vertex = vertices.begin();
	vertex != vertices.end(); ++vertex ) {
    for ( reco::Vertex::trackRef_iterator vtxAssocTrack = vertex->tracks_begin();
	  vtxAssocTrack != vertex->tracks_end(); ++vtxAssocTrack ) {      
      if ( vtxAssocTrack->isNonnull() && vtxAssocTrack->isAvailable()                             && 
	   TMath::Abs((*vtxAssocTrack)->eta() - signalTrack->eta()) < epsilon                     &&
	   TMath::Abs((*vtxAssocTrack)->phi() - signalTrack->phi()) < epsilon                     &&
	   TMath::Abs((*vtxAssocTrack)->pt()  - signalTrack->pt())  < (epsilon*signalTrack->pt()) ) {
	retVal = &(*vertex);
	break;
      }
    }
  }

  if ( retVal ) return retVal;

//--- no vertex associated to track found,
//    find vertex best matching signalTrack by deltaZ
  double minDeltaZ = 1.e+3;
  double refZ = signalTrack->dz(bs.position());
  for ( reco::VertexCollection::const_iterator vertex = vertices.begin();
	vertex != vertices.end(); ++vertex ) {
    double deltaZ = TMath::Abs(vertex->z() - refZ);
    if ( retVal == 0 || deltaZ < minDeltaZ ) {
      retVal = &(*vertex);
      minDeltaZ = deltaZ;
    }
  }

  return retVal;
}
