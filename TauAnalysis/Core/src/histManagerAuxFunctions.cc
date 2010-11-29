#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>

void bookWeightHistograms(DQMStore& dqmStore, const char* name, const char* title, 
			  MonitorElement*& h_posLog, MonitorElement*& h_negLog, MonitorElement*& h_zero, MonitorElement*& h_linear)
{
  std::string hName_posLog = std::string(name).append("PosLog");
  std::string hTitle_posLog = std::string("log10(|").append(title).append("|), ").append(title).append(" #geq 0");
  h_posLog = dqmStore.book1D(hName_posLog.data(), hTitle_posLog.data(), 104, -6.2, 4.2);

  std::string hName_negLog = std::string(name).append("NegLog");
  std::string hTitle_negLog = std::string("log10(|").append(title).append("|), ").append(title).append(" #leq 0");
  h_negLog = dqmStore.book1D(hName_negLog.data(), hTitle_negLog.data(), 104, -6.2, 4.2);

  std::string hName_zero = std::string(name).append("Zero");
  std::string hTitle_zero = std::string(title).append(" == 0");
  h_zero = dqmStore.book1D(hName_zero.data(), hTitle_zero.data(), 2, -0.5, 1.5);

  std::string hName_linear = std::string(name).append("Linear");
  std::string hTitle_linear = std::string(title);
  h_linear = dqmStore.book1D(hName_linear.data(), hTitle_linear.data(), 403, -1.075, +1.075);
}

void fillWeightHistograms(MonitorElement* h_posLog, MonitorElement* h_negLog, MonitorElement* h_zero, 
			  MonitorElement* h_linear, double weight)
{
  if ( weight != 0. ) {
    double log10weight = TMath::Log10(TMath::Abs(weight));
    
    if ( log10weight >  4. ) log10weight =  4.15;
    if ( log10weight < -6. ) log10weight = -6.15;
    
    if ( weight > 0. ) {
      h_posLog->Fill(log10weight);
    } else if ( weight < 0. ) {
      h_negLog->Fill(log10weight);
    }
  } else {
    h_zero->Fill(1);
  }

  double linWeight = weight;
  if ( linWeight < -1. ) linWeight = -1.05;
  if ( linWeight > +1. ) linWeight = +1.05;
  h_linear->Fill(linWeight);
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

//
//-----------------------------------------------------------------------------------------------------------------------
//

double compDecayEigenTime(const reco::Candidate::Point& primaryVertexPos, const reco::Candidate::Point& decayVertexPos, 
			  double tauLeptonEnergy)
{
  double tauLeptonDecayDistance = TMath::Sqrt((decayVertexPos - primaryVertexPos).Mag2());
  return compDecayEigenTime(tauLeptonDecayDistance, tauLeptonEnergy);
}

double compDecayEigenTime(double tauLeptonDecayDistance,  double tauLeptonEnergy)
{
  const double speedOfLight = 3.e-5; // speed of light [cm/fs]
  double gamma = tauLeptonEnergy/SVfit_namespace::tauLeptonMass;
  return tauLeptonDecayDistance/(speedOfLight*gamma);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void fillHistogramGenMatch(MonitorElement* histo, double value, const reco::Candidate::LorentzVector& p4MatchDirection,
			   const reco::GenParticleCollection& genParticles, const std::vector<int>& pdgIds, 
			   double weight)
{
  if ( findGenParticle(p4MatchDirection, genParticles, 0.5, -1, &pdgIds) ) histo->Fill(value, weight);
}
