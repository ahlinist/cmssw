#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

void fillLeptonIsoDepositHistograms(const pat::IsoDeposit* isoDeposit, 
				    MonitorElement* isoDepositValProfile,
				    MonitorElement* isoDepositEtaDistProfile, 
				    MonitorElement* isoDepositPhiDistProfile)
{
  if ( isoDeposit ) {
    for ( pat::IsoDeposit::const_iterator it = isoDeposit->begin();
	  it != isoDeposit->end(); ++it ) {
      isoDepositValProfile->Fill(it->value());
      isoDepositEtaDistProfile->Fill(TMath::Abs(it->eta() - isoDeposit->eta()));
      isoDepositEtaDistProfile->Fill(TMath::Abs(it->phi() - isoDeposit->phi()));
    }
  }
}

void clearIsoParam(reco::isodeposit::AbsVetos& isoParam)
{
  for ( reco::isodeposit::AbsVetos::const_iterator it = isoParam.begin();
	it != isoParam.end(); ++it ) {
    delete (*it);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

int getMatchingGenParticlePdgId(const reco::Particle::LorentzVector& recoMomentum,
				edm::Handle<reco::GenParticleCollection>& genParticleCollection)
{
//--- select genParticles matching direction of reconstructed particle
//    within cone of size dR = 0.5;
//    require generated transverse momentum to be at least half of reconstructed transverse momentum
  reco::GenParticleCollection matchingGenParticles;
  for ( reco::GenParticleCollection::const_iterator genParticle = genParticleCollection->begin(); 
	genParticle != genParticleCollection->end(); ++genParticle ) {

//--- skip "documentation line" entries
//    (copied over to reco::GenParticle from HepMC product)
    if ( genParticle->status() == 3 ) continue;

    if ( genParticle->pt() > 0.50*recoMomentum.pt() &&
	 reco::deltaR(genParticle->p4(), recoMomentum) < 0.5 ) {
      matchingGenParticles.push_back(*genParticle);
    }
  }

//--- find highest Pt matching genParticle 
  double ptMax = -1.;
  int pdgId = -1;
  for ( reco::GenParticleCollection::const_iterator matchingGenParticle = matchingGenParticles.begin(); 
	matchingGenParticle != matchingGenParticles.end(); ++matchingGenParticle ) {
    
    if ( matchingGenParticle->pt() > ptMax ) {
      pdgId = matchingGenParticle->pdgId();
      ptMax = matchingGenParticle->pt();
    }
  }

  return pdgId;
}
