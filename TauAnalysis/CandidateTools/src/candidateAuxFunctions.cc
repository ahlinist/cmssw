#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TMath.h"

const reco::GenParticle* findGenParticle(const reco::Candidate::LorentzVector& direction, 
					 const reco::GenParticleCollection& genParticles, double dRmax, int status,
					 std::vector<int>* pdgIds)
{
  const reco::GenParticle* bestMatch = 0;

  for ( reco::GenParticleCollection::const_iterator genParticle = genParticles.begin();
	genParticle != genParticles.end(); ++genParticle ) {
    bool matchesPdgId = false;
    if ( pdgIds ) {
      for ( std::vector<int>::const_iterator pdgId = pdgIds->begin();
	    pdgId != pdgIds->end(); ++pdgId ) {
	if ( genParticle->pdgId() == (*pdgId) ) matchesPdgId = true;
      }
    } else {
      matchesPdgId = true;
    }

    if ( matchesPdgId && (status == -1 || genParticle->status() == status) && 
	 (bestMatch == 0 || genParticle->energy() > bestMatch->energy()) ) {
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
    if ( (status == -1 || (*daughter)->status() == status) && !isNeutrino(*daughter) ) {
      //std::cout << "adding daughter: pdgId = " << (*daughter)->pdgId() << ", Pt = " << (*daughter)->pt() << ","
      //	  << " eta = " << (*daughter)->eta() << ", phi = " << (*daughter)->phi()*180./TMath::Pi() << std::endl;
      p4Vis += (*daughter)->p4();
    }
  }

  //std::cout << "--> vis. Momentum: Pt = " << p4Vis.pt() << ", eta = " << p4Vis.eta() << ", phi = " << p4Vis.phi() << std::endl;

  return p4Vis;
}

reco::Candidate::LorentzVector getVisMomentum(const reco::GenParticle* genLeg, const reco::GenParticleCollection* genParticles)
{
  std::vector<const reco::GenParticle*> stableDaughters;
  findDaughters(genLeg, stableDaughters, 1);

  reco::Candidate::LorentzVector p4Vis = getVisMomentum(stableDaughters);

  return p4Vis;
}

reco::Candidate::LorentzVector getInvisMomentum(const std::vector<const reco::GenParticle*>& daughters, int status)
{
  reco::Candidate::LorentzVector p4Invis(0,0,0,0);

  for ( std::vector<const reco::GenParticle*>::const_iterator daughter = daughters.begin();
	daughter != daughters.end(); ++daughter ) {
    if ( (status == -1 || (*daughter)->status() == status) && isNeutrino(*daughter) ) {
      //std::cout << "adding daughter: pdgId = " << (*daughter)->pdgId() << ", Pt = " << (*daughter)->pt() << ","
      //	  << " eta = " << (*daughter)->eta() << ", phi = " << (*daughter)->phi()*180./TMath::Pi() << std::endl;
      p4Invis += (*daughter)->p4();
    }
  }

  //std::cout << "--> invis. Momentum: Pt = " << p4Invis.pt() << ", phi = " << p4Invis.phi() << std::endl;

  return p4Invis;
}

reco::Candidate::LorentzVector getInvisMomentum(const reco::GenParticle* genLeg, const reco::GenParticleCollection* genParticles)
{
  std::vector<const reco::GenParticle*> stableDaughters;
  findDaughters(genLeg, stableDaughters, 1);
  
  reco::Candidate::LorentzVector p4Invis = getInvisMomentum(stableDaughters);

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

