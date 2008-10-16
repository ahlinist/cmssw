// -*- C++ -*-
//
// Package:    PATLeptonPATTauSeperationSelector
// Class:      PATLeptonPATTauSeperationSelector
// 
/**\class PATLeptonPATTauSeperationSelector PATLeptonPATTauSeperationSelector.cc MyCode/PATLeptonPATTauSeperationSelector/src/PATLeptonPATTauSeperationSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Alfredo Gurrola
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id: PATLeptonPATTauSeperationSelector.cc,v 1.2 2008/10/16 17:22:41 gurrola Exp $
//
//

#include "ElectroWeakAnalysis/EWKTau/interface/PATLeptonPATTauSeperationSelector.h"

PATLeptonPATTauSeperationSelector::PATLeptonPATTauSeperationSelector(const edm::ParameterSet& iConfig)
{
  src_ = iConfig.getParameter<edm::InputTag>("src");
  LeptonType_ = iConfig.getParameter<std::string>("LeptonType");
  TauSource_ = iConfig.getParameter<edm::InputTag>("TauSource");
  DeltaRMaxCut_ = iConfig.getParameter<double>("DeltaRMaxCut");
  DeltaRMinCut_ = iConfig.getParameter<double>("DeltaRMinCut");
  if(LeptonType_ == "Muon") {
    produces<PATMuonCollection>();
  } else {
    produces<PATElectronCollection>();
  }
}


PATLeptonPATTauSeperationSelector::~PATLeptonPATTauSeperationSelector() {}

void PATLeptonPATTauSeperationSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;

  if(LeptonType_ == "Muon") {
    Handle<View<pat::Muon> > leptonHandle;
    iEvent.getByLabel(src_,leptonHandle);
    View<pat::Muon> leptons = *leptonHandle;

    Handle<View<pat::Tau> > tauHandle;
    iEvent.getByLabel(TauSource_,tauHandle);
    View<pat::Tau> taus = *tauHandle;

    std::auto_ptr<PATMuonCollection> pOut(new PATMuonCollection);

    for(View<pat::Muon>::const_iterator trk = leptons.begin();trk != leptons.end(); ++trk) {
      bool goodLepton = true;
      for(View<pat::Tau>::const_iterator tauit = taus.begin();tauit != taus.end(); ++tauit) {
        if( (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),tauit->momentum())<=DeltaRMinCut_) ||
            (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),tauit->momentum())>=DeltaRMaxCut_) ) {goodLepton = false;}
      }
      if (goodLepton) {pOut->push_back( * trk );}
      std::cout << "Good Lepton ? :  " << goodLepton << std::endl;
    }

    iEvent.put(pOut);

  } else if(LeptonType_ == "Electron") {
    Handle<View<pat::Electron> > leptonHandle;
    iEvent.getByLabel(src_,leptonHandle);
    View<pat::Electron> leptons = *leptonHandle;

    Handle<View<pat::Tau> > tauHandle;
    iEvent.getByLabel(TauSource_,tauHandle);
    View<pat::Tau> taus = *tauHandle;

    std::auto_ptr<PATElectronCollection> pOut(new PATElectronCollection);

    for(View<pat::Electron>::const_iterator trk = leptons.begin();trk != leptons.end(); ++trk) {
      bool goodLepton = true;
      for(View<pat::Tau>::const_iterator tauit = taus.begin();tauit != taus.end(); ++tauit) {
        if( (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),tauit->momentum())<=DeltaRMinCut_) ||
            (ROOT::Math::VectorUtil::DeltaR(trk->momentum(),tauit->momentum())>=DeltaRMaxCut_) ) {goodLepton = false;}
      }
      if (goodLepton) {pOut->push_back( * trk );}
      std::cout << "Good Lepton ? :  " << goodLepton << std::endl;
    }

    iEvent.put(pOut);

  } else {
    std::cerr << "Incorrect Lepton Type!!! " << std::endl;
    std::cerr << "Please re-run and set the LeptonType to 'Muon' or 'Electron' " << std::endl;
    return;
  }
 
}

// ------------ method called once each job just before starting event loop  ------------
void PATLeptonPATTauSeperationSelector::beginJob(const edm::EventSetup&) {}

// ------------ method called once each job just after ending the event loop  ------------
void PATLeptonPATTauSeperationSelector::endJob() {}
