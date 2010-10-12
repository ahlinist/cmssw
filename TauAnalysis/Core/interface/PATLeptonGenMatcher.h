#ifndef TauAnalysis_Core_PATLeptonGenMatcher_h
#define TauAnalysis_Core_PATLeptonGenMatcher_h 

/** \class PATLeptonGenMatcher
 *
 * Auxiliary class for matching to generator level leptons/
 * generated tau lepton hadronic decay modes
 * 
 * \authors Christian Veelken
 *
 * \version $Revision: 1.3 $
 *
 * $Id: PATLeptonGenMatcher.h,v 1.3 2010/02/20 20:58:54 wmtan Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"

#include <vector>
#include <string>

template <typename T>
class PATLeptonGenMatcher
{
 public:
  PATLeptonGenMatcher(const edm::ParameterSet&) {}
  
  bool operator()(const edm::Event&, const T&) const { 
    assert(0);   // CV: generic class template should **never** be called
    return true;
  }
};

template <>
class PATLeptonGenMatcher<pat::Tau>
{
 public:
  typedef std::vector<std::string> vstring;

  PATLeptonGenMatcher(const edm::ParameterSet& cfg)
    : genParticleSrc_(cfg.getParameter<edm::InputTag>("genParticleSource")),
      dRmatch_(cfg.getParameter<double>("dRmatch")),
      genTauJetSrc_(cfg.getParameter<edm::InputTag>("genTauJetSource")),
      tauDecayModes_(cfg.getParameter<vstring>("tauDecayModes"))
  {}

  bool operator()(const edm::Event& evt, const pat::Tau& tau) const
  {
    edm::Handle<reco::GenParticleCollection> genParticles;
    evt.getByLabel(genParticleSrc_, genParticles);

    edm::Handle<reco::GenJetCollection> genTauJets;
    evt.getByLabel(genTauJetSrc_, genTauJets);

    bool genTauLeptonMatch = false;
    for ( reco::GenParticleCollection::const_iterator genParticle = genParticles->begin();
	  genParticle != genParticles->end(); ++genParticle ) {
      if ( genParticle->status() == 3 ) continue;
      
      if ( TMath::Abs(genParticle->pdgId()) == 15 ) {
	double dR = deltaR(tau.p4(), genParticle->p4());
	if ( dR < dRmatch_ ) genTauLeptonMatch = true;
      }
    }

     bool genTauDecayModeMatch = false;
     if ( genTauLeptonMatch ) {
       for ( reco::GenJetCollection::const_iterator genTauJet = genTauJets->begin();
	     genTauJet != genTauJets->end(); ++genTauJet ) {
	 double dR = deltaR(tau.p4(), genTauJet->p4());
	 if ( dR < dRmatch_ ) {
	   std::string genTauDecayMode = JetMCTagUtils::genTauDecayMode(*genTauJet);
	   for ( vstring::const_iterator tauDecayMode = tauDecayModes_.begin();
		 tauDecayMode != tauDecayModes_.end(); ++tauDecayMode ) {
	     if ( genTauDecayMode == (*tauDecayMode) ) genTauDecayModeMatch = true;
	   }
	 }
       }
     }

     return genTauDecayModeMatch;
  }

 private:
  edm::InputTag genParticleSrc_;
  double dRmatch_;
  edm::InputTag genTauJetSrc_;
  vstring tauDecayModes_;
};

template <>
class PATLeptonGenMatcher<pat::Muon>
{
 public:
  PATLeptonGenMatcher(const edm::ParameterSet& cfg)
    : genParticleSrc_(cfg.getParameter<edm::InputTag>("genParticleSource")),
      dRmatch_(cfg.getParameter<double>("dRmatch"))
  {}

    bool operator()(const edm::Event& evt, const pat::Muon& muon) const
  {
    edm::Handle<reco::GenParticleCollection> genParticles;
    evt.getByLabel(genParticleSrc_, genParticles);

    bool genMuonMatch = false;
    for ( reco::GenParticleCollection::const_iterator genParticle = genParticles->begin();
	  genParticle != genParticles->end(); ++genParticle ) {
      if ( genParticle->status() != 1 ) continue;
      
      if ( TMath::Abs(genParticle->pdgId()) == 13 ) {
	double dR = deltaR(muon.p4(), genParticle->p4());
	if ( dR < dRmatch_ ) genMuonMatch = true;
      }
    }

    return genMuonMatch;
  }

 private:
  edm::InputTag genParticleSrc_;
  double dRmatch_;
};

template <>
class PATLeptonGenMatcher<pat::Electron>
{
 public:
  PATLeptonGenMatcher(const edm::ParameterSet& cfg)
    : genParticleSrc_(cfg.getParameter<edm::InputTag>("genParticleSource")),
      dRmatch_(cfg.getParameter<double>("dRmatch"))
  {}

    bool operator()(const edm::Event& evt, const pat::Electron& electron) const
  {
    edm::Handle<reco::GenParticleCollection> genParticles;
    evt.getByLabel(genParticleSrc_, genParticles);

    bool genElectronMatch = false;
    for ( reco::GenParticleCollection::const_iterator genParticle = genParticles->begin();
	  genParticle != genParticles->end(); ++genParticle ) {
      if ( genParticle->status() != 1 ) continue;
      
      if ( TMath::Abs(genParticle->pdgId()) == 11 ) {
	double dR = deltaR(electron.p4(), genParticle->p4());
	if ( dR < dRmatch_ ) genElectronMatch = true;
      }
    }

    return genElectronMatch;
  }

 private:
  edm::InputTag genParticleSrc_;
  double dRmatch_;
};

#endif
