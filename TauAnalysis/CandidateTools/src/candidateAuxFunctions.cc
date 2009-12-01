#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include "PhysicsTools/Utilities/interface/deltaR.h"

const reco::GenParticle* findGenParticle(const reco::Candidate::LorentzVector& direction, 
					 const reco::GenParticleCollection& genParticles, double dRmax, int status)
{
  const reco::GenParticle* bestMatch = 0;

  for ( reco::GenParticleCollection::const_iterator genParticle = genParticles.begin();
	genParticle != genParticles.end(); ++genParticle ) {
    if ( (status == -1 || genParticle->status() == status) && (bestMatch == 0 || genParticle->energy() > bestMatch->energy()) ) {
      double dR = reco::deltaR(direction, genParticle->p4());
      if ( dR < dRmax ) bestMatch = &(*genParticle);
    }
  }

  return bestMatch;
}

void findDaughters(const reco::GenParticle* mother, std::vector<const reco::GenParticle*>& daughters, int status)
{
  unsigned numDaughters = mother->numberOfDaughters();
  for ( unsigned iDaughter = 0; iDaughter < numDaughters; ++iDaughter ) {
    const reco::GenParticle* daughter = mother->daughterRef(iDaughter).get();
    
    if ( status == -1 || daughter->status() == status ) daughters.push_back(daughter);
    
    findDaughters(daughter, daughters, status);
  }
}

reco::Candidate::LorentzVector getVisMomentum(const std::vector<const reco::GenParticle*>& daughters, int status)
{
  reco::Candidate::LorentzVector p4Vis(0,0,0,0);

  for ( std::vector<const reco::GenParticle*>::const_iterator daughter = daughters.begin();
	daughter != daughters.end(); ++daughter ) {
    if ( status == -1 || (*daughter)->status() == status ) p4Vis += (*daughter)->p4();
  }

  return p4Vis;
}
