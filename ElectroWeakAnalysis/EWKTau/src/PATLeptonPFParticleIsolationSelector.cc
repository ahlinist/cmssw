// -*- C++ -*-
//
// Package:    PATLeptonPFParticleIsolationSelector
// Class:      PATLeptonPFParticleIsolationSelector
// 
/**\class PATLeptonPFParticleIsolationSelector PATLeptonPFParticleIsolationSelector.cc MyCode/PATLeptonPFParticleIsolationSelector/src/PATLeptonPFParticleIsolationSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Alfredo Gurrola
//         Created:  Wed Oct 16 21:33:34 CEST 2008
// $Id: PATLeptonPFParticleIsolationSelector.cc,v 1.1 2008/10/16 11:00:02 gurrola Exp $
//
//

#include "ElectroWeakAnalysis/EWKTau/interface/PATLeptonPFParticleIsolationSelector.h"

PATLeptonPFParticleIsolationSelector::PATLeptonPFParticleIsolationSelector(const edm::ParameterSet& iConfig)
{
  src_ = iConfig.getParameter<edm::InputTag>("src");
  LeptonType_ = iConfig.getParameter<std::string>("LeptonType");
  PFCandidateProd_ = iConfig.getParameter<std::string>("PFCandidateProd");
  PFParticleCode_ = iConfig.getParameter<double>("PFParticleCode");
  OuterIsolationConeSize_ = iConfig.getParameter<double>("OuterIsolationConeSize");
  InnerIsolationConeSize_ = iConfig.getParameter<double>("InnerIsolationConeSize");
  IsolationMinCut_ = iConfig.getParameter<double>("IsolationMinCut");
  IsolationMaxCut_ = iConfig.getParameter<double>("IsolationMaxCut");
  if(LeptonType_ == "Muon") {
    produces<PATMuonCollection>();
  } else {
    produces<PATElectronCollection>();
  }
}


PATLeptonPFParticleIsolationSelector::~PATLeptonPFParticleIsolationSelector() {}

void PATLeptonPFParticleIsolationSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;

  if(LeptonType_ == "Muon") {
    Handle<View<pat::Muon> > leptonHandle;
    iEvent.getByLabel(src_,leptonHandle);
    View<pat::Muon> leptons = *leptonHandle;

    // PFCandidates
    Handle<reco::PFCandidateCollection> pFCandidateHandle;
    iEvent.getByLabel(PFCandidateProd_, pFCandidateHandle);
    const reco::PFCandidateCollection& thePFCandidateCollection = *(pFCandidateHandle.product());

    std::auto_ptr<PATMuonCollection> pOut(new PATMuonCollection);

    for(View<pat::Muon>::const_iterator trk = leptons.begin();trk != leptons.end(); ++trk) {
      double sumPFPT = 0.0;
      // ** Calculate muon absolute PFParticle isolation variables
      for(reco::PFCandidateCollection::const_iterator iPFCand=thePFCandidateCollection.begin();iPFCand!=thePFCandidateCollection.end();iPFCand++) {
        const reco::PFCandidate *cand = &(*iPFCand);
        if( (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),cand->momentum())<=OuterIsolationConeSize_) &&
            (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),cand->momentum())>=InnerIsolationConeSize_) &&
            (cand->particleId() == PFParticleCode_) ) {sumPFPT = sumPFPT + (*cand).pt();}
      }
      if ( ( sumPFPT <= IsolationMaxCut_ ) && ( sumPFPT >= IsolationMinCut_ ) ) {pOut->push_back( * trk );}
      std::cout << "sum_PF_PT :  " << sumPFPT << std::endl;
    }

    iEvent.put(pOut);

  } else if(LeptonType_ == "Electron") {
    Handle<View<pat::Electron> > leptonHandle;
    iEvent.getByLabel(src_,leptonHandle);
    View<pat::Electron> leptons = *leptonHandle;

    // PFCandidates
    Handle<reco::PFCandidateCollection> pFCandidateHandle;
    iEvent.getByLabel(PFCandidateProd_, pFCandidateHandle);
    const reco::PFCandidateCollection& thePFCandidateCollection = *(pFCandidateHandle.product());

    std::auto_ptr<PATElectronCollection> pOut(new PATElectronCollection);

    for(View<pat::Electron>::const_iterator trk = leptons.begin();trk != leptons.end(); ++trk) {
      double sumPFPT = 0.0;
      // ** Calculate electron absolute PFParticle isolation variables
      for(reco::PFCandidateCollection::const_iterator iPFCand=thePFCandidateCollection.begin();iPFCand!=thePFCandidateCollection.end();iPFCand++) {
        const reco::PFCandidate *cand = &(*iPFCand);
        if( (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),cand->momentum())<=OuterIsolationConeSize_) &&
            (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),cand->momentum())>=InnerIsolationConeSize_) &&
            (cand->particleId() == PFParticleCode_) ) {sumPFPT = sumPFPT + (*cand).pt();}
      }
      if ( ( sumPFPT <= IsolationMaxCut_ ) && ( sumPFPT >= IsolationMinCut_ ) ) {pOut->push_back( * trk );}
      std::cout << "sum_PF_PT :  " << sumPFPT << std::endl;
    }

    iEvent.put(pOut);

  } else {
    std::cerr << "Incorrect Lepton Type!!! " << std::endl;
    std::cerr << "Please re-run and set the LeptonType to 'Muon' or 'Electron' " << std::endl;
    return;
  }
 
}

// ------------ method called once each job just before starting event loop  ------------
void PATLeptonPFParticleIsolationSelector::beginJob(const edm::EventSetup&) {}

// ------------ method called once each job just after ending the event loop  ------------
void PATLeptonPFParticleIsolationSelector::endJob() {}
