#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

void bookWeightHistograms(DQMStore& dqmStore, const char* name, const char* title, 
			  MonitorElement*& h_posUnweighted, MonitorElement*& h_posWeighted,
			  MonitorElement*& h_negUnweighted, MonitorElement*& h_negWeighted)
{
  std::string hName_posUnweighted = std::string(name).append("PosUnweighted");
  std::string hTitle_posUnweighted = std::string("log10(|").append(title).append("|), ").append(title).append("#geq 0 (unweighted)");
  h_posUnweighted = dqmStore.book1D(hName_posUnweighted.data(), hTitle_posUnweighted.data(), 74, -6.2, 1.2);

  std::string hName_posWeighted = std::string(name).append("PosUnweighted");
  std::string hTitle_posWeighted = std::string("log10(|").append(title).append("|), ").append(title).append("#geq 0 (weighted)");
  h_posWeighted = dqmStore.book1D(hName_posWeighted.data(), hTitle_posWeighted.data(), 74, -6.2, 1.2);

  std::string hName_negUnweighted = std::string(name).append("NegUnweighted");
  std::string hTitle_negUnweighted = std::string("log10(|").append(title).append("|), ").append(title).append("#leq 0 (unweighted)");
  h_negUnweighted = dqmStore.book1D(hName_negUnweighted.data(), hTitle_negUnweighted.data(), 74, -6.2, 1.2);

  std::string hName_negWeighted = std::string(name).append("NegWeighted");
  std::string hTitle_negWeighted = std::string("log10(|").append(title).append("|), ").append(title).append("#leq 0 (weighted)");
  h_negWeighted = dqmStore.book1D(hName_negWeighted.data(), hTitle_negWeighted.data(), 74, -6.2, 1.2);
}

void fillWeightHistograms(MonitorElement* h_posUnweighted, MonitorElement* h_posWeighted, 
			  MonitorElement* h_negUnweighted, MonitorElement* h_negWeighted, 
			  double weight)
{
  if ( weight == 0. ) return;

  double log10weight = TMath::Log10(TMath::Abs(weight));

  if ( log10weight >  1. ) log10weight =  1.15;
  if ( log10weight < -6. ) log10weight = -6.15;

  if ( weight > 0. ) {
    h_posUnweighted->Fill(log10weight);
    h_posWeighted->Fill(log10weight, weight);
  } else if ( weight < 0. ) {
    h_negUnweighted->Fill(log10weight);
    h_negWeighted->Fill(log10weight, weight);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void fillLeptonIsoDepositHistograms(const pat::IsoDeposit* isoDeposit, 
				    MonitorElement* isoDepositValProfile,
				    MonitorElement* isoDepositEtaDistProfile, 
				    MonitorElement* isoDepositPhiDistProfile, 
				    double evtWeight)
{
  if ( isoDeposit ) {
    for ( pat::IsoDeposit::const_iterator it = isoDeposit->begin();
	  it != isoDeposit->end(); ++it ) {
      isoDepositValProfile->Fill(it->value(), evtWeight);
      isoDepositEtaDistProfile->Fill(TMath::Abs(it->eta() - isoDeposit->eta()), evtWeight);
      isoDepositPhiDistProfile->Fill(TMath::Abs(it->phi() - isoDeposit->phi()), evtWeight);
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

//
//-----------------------------------------------------------------------------------------------------------------------
//

double getDeltaRnearestJet(const reco::Particle::LorentzVector& refMomentum, 
			   edm::Handle<pat::JetCollection>& patJetCollection, 
			   double deltaRmin)
{
//--- compute eta-phi distance to nearest jet
  double deltaRnearestJet = 1.e+3;
  for ( std::vector<pat::Jet>::const_iterator patJet = patJetCollection->begin(); 
	patJet != patJetCollection->end(); ++patJet ) {
    double deltaR = reco::deltaR(patJet->p4(), refMomentum);
    if ( deltaR > deltaRmin && deltaR < deltaRnearestJet ) deltaRnearestJet = deltaR;
  }
  
  return deltaRnearestJet;
}



