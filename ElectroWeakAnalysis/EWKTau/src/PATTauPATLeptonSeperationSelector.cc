// -*- C++ -*-
//
// Package:    PATTauPATLeptonSeperationSelector
// Class:      PATTauPATLeptonSeperationSelector
// 
/**\class PATTauPATLeptonSeperationSelector PATTauPATLeptonSeperationSelector.cc MyCode/PATTauPATLeptonSeperationSelector/src/PATTauPATLeptonSeperationSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Alfredo Gurrola
// Modified by: Konstantinos A. Petridis
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id: PATTauPATLeptonSeperationSelector.cc,v 1.1 2008/10/16 17:54:42 gurrola Exp $
//
//

#include "ElectroWeakAnalysis/EWKTau/interface/PATTauPATLeptonSeperationSelector.h"

PATTauPATLeptonSeperationSelector::PATTauPATLeptonSeperationSelector(const edm::ParameterSet& iConfig)
{
  src_ = iConfig.getParameter<edm::InputTag>("LeptonSource");
  LeptonType_ = iConfig.getParameter<std::string>("LeptonType");
  TauSource_ = iConfig.getParameter<edm::InputTag>("TauSource");
  DeltaRMaxCut_ = iConfig.getParameter<double>("DeltaRMaxCut");
  DeltaRMinCut_ = iConfig.getParameter<double>("DeltaRMinCut");
  produces<PATTauCollection>();
}


PATTauPATLeptonSeperationSelector::~PATTauPATLeptonSeperationSelector() {}

void PATTauPATLeptonSeperationSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;
  std::auto_ptr<PATTauCollection> pOut(new PATTauCollection);
  Handle<View<pat::Tau> > tauHandle;
  iEvent.getByLabel(TauSource_,tauHandle);
  View<pat::Tau> taus = *tauHandle;
  
  if(LeptonType_ == "Muon") {
    Handle<View<pat::Muon> > leptonHandle;
    if(iEvent.getByLabel(src_,leptonHandle)){
      View<pat::Muon> leptons = *leptonHandle;
    
      for(View<pat::Tau>::const_iterator tauit = taus.begin();tauit != taus.end(); ++tauit){
	bool goodTau = true;
	for(View<pat::Muon>::const_iterator trk = leptons.begin();trk != leptons.end(); ++trk){
	  if( (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),tauit->momentum())<=DeltaRMinCut_) ||
	      (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),tauit->momentum())>=DeltaRMaxCut_) ) {
	    goodTau = false;
	  }
	}
	if (goodTau) {pOut->push_back( * tauit );}
	std::cout << "Good Tau ? :  " << goodTau << std::endl;
      }
      
      iEvent.put(pOut);
    }
    
  } else if(LeptonType_ == "Electron") {
    Handle<View<pat::Electron> > leptonHandle;
    if(iEvent.getByLabel(src_,leptonHandle)){
      View<pat::Electron> leptons = *leptonHandle;
      for(View<pat::Tau>::const_iterator tauit = taus.begin();tauit != taus.end(); ++tauit) {
	bool goodTau = true;
	for(View<pat::Electron>::const_iterator trk = leptons.begin();trk != leptons.end(); ++trk) {
	  if( (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),tauit->momentum())<=DeltaRMinCut_) ||
	      (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),tauit->momentum())>=DeltaRMaxCut_) ) {
	    goodTau = false;
	  }
	}
	if (goodTau) {pOut->push_back( * tauit );}
	std::cout << "Good Tau ? :  " << goodTau << std::endl;
      }
      iEvent.put(pOut);
    }
  } else {
    std::cerr << "Incorrect Lepton Type!!! " << std::endl;
    std::cerr << "Please re-run and set the LeptonType to 'Muon' or 'Electron' " << std::endl;
    return;
  }
 
}

// ------------ method called once each job just before starting event loop  ------------
void PATTauPATLeptonSeperationSelector::beginJob(const edm::EventSetup&) {}

// ------------ method called once each job just after ending the event loop  ------------
void PATTauPATLeptonSeperationSelector::endJob() {}
