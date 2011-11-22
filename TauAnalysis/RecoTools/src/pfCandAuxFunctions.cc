#include "TauAnalysis/RecoTools/interface/pfCandAuxFunctions.h"

#include "DataFormats/Math/interface/deltaR.h"

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
			   const reco::PFCandidateCollection& pfNoPileUpCandidates_input, double dRmatch,
			   std::vector<const reco::PFCandidate*>& pfNoPileUpCandidates_output, 
			   std::vector<const reco::PFCandidate*>& pfPileUpCandidates_output)
{
  for ( std::vector<const reco::PFCandidate*>::const_iterator pfCandidate = pfCandidates.begin();
	pfCandidate != pfCandidates.end(); ++pfCandidate ) {

    bool isNoPileUp_matched = false;
    for ( reco::PFCandidateCollection::const_iterator pfNoPileUpCandidate = pfNoPileUpCandidates_input.begin();
	  pfNoPileUpCandidate != pfNoPileUpCandidates_input.end(); ++pfNoPileUpCandidate ) {
      if ( (*pfCandidate)->particleId() == pfNoPileUpCandidate->particleId() &&
           deltaR((*pfCandidate)->p4(), pfNoPileUpCandidate->p4()) < dRmatch ) isNoPileUp_matched = true;
    }
    
    if ( isNoPileUp_matched ) pfNoPileUpCandidates_output.push_back(*pfCandidate);
    else pfPileUpCandidates_output.push_back(*pfCandidate);
  }
}
