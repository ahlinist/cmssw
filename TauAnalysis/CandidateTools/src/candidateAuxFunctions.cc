#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TMath.h"

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

bool isNeutrino(const reco::GenParticle* daughter)
{
  return ( TMath::Abs(daughter->pdgId()) == 12 || TMath::Abs(daughter->pdgId()) == 14 || TMath::Abs(daughter->pdgId()) == 16 );
}

reco::Candidate::LorentzVector getVisMomentum(const std::vector<const reco::GenParticle*>& daughters, int status)
{
  reco::Candidate::LorentzVector p4Vis(0,0,0,0);

  for ( std::vector<const reco::GenParticle*>::const_iterator daughter = daughters.begin();
	daughter != daughters.end(); ++daughter ) {
    if ( (status == -1 || (*daughter)->status() == status) && !isNeutrino(*daughter) ) p4Vis += (*daughter)->p4();
  }

  return p4Vis;
}

reco::Candidate::LorentzVector getVisMomentum(const reco::GenParticle* genLeg, const reco::GenParticleCollection* genParticles)
{
  std::vector<const reco::GenParticle*> stableDaughters;
  findDaughters(genLeg, stableDaughters, 1);

  reco::Candidate::LorentzVector p4Vis = getVisMomentum(stableDaughters);
  //std::cout << "vis. Momentum: E = " << p4Vis.energy() << ", eta = " << p4Vis.eta() << ", phi = " << p4Vis.phi() << std::endl;

  return p4Vis;
}

reco::Candidate::LorentzVector getInvisMomentum(const std::vector<const reco::GenParticle*>& daughters, int status)
{
  reco::Candidate::LorentzVector p4Invis(0,0,0,0);

  for ( std::vector<const reco::GenParticle*>::const_iterator daughter = daughters.begin();
	daughter != daughters.end(); ++daughter ) {
    if ( (status == -1 || (*daughter)->status() == status) && isNeutrino(*daughter) ) p4Invis += (*daughter)->p4();
  }

  return p4Invis;
}

reco::Candidate::LorentzVector getInvisMomentum(const reco::GenParticle* genLeg, const reco::GenParticleCollection* genParticles)
{
  std::vector<const reco::GenParticle*> stableDaughters;
  findDaughters(genLeg, stableDaughters, 1);
  
  reco::Candidate::LorentzVector p4Invis = getInvisMomentum(stableDaughters);
  //std::cout << "invis. Momentum: E = " << p4Invis.energy() << ", phi = " << p4Invis.phi() << std::endl;

  return p4Invis;
}

double getPhysX(double x, bool& isWithinPhysRange)
{
  double physX = x;

  isWithinPhysRange = true;

  if ( x < 0. ) {
    physX = 0.;
    isWithinPhysRange = false;
  }

  if ( x > 1. ) {
    physX = 1.;
    isWithinPhysRange = false;
  }

  return physX;
}
