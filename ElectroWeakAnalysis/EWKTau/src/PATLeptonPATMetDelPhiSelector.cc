// -*- C++ -*-
//
// Package:    PATLeptonPATMetDelPhiSelector
// Class:      PATLeptonPATMetDelPhiSelector
// 
/**\class PATLeptonPATMetDelPhiSelector PATLeptonPATMetDelPhiSelector.cc MyCode/PATLeptonPATMetDelPhiSelector/src/PATLeptonPATMetDelPhiSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Alfredo Gurrola
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id: PATLeptonPATMetDelPhiSelector.cc,v 1.1 2008/10/16 10:11:54 gurrola Exp $
//
//

#include "ElectroWeakAnalysis/EWKTau/interface/PATLeptonPATMetDelPhiSelector.h"

PATLeptonPATMetDelPhiSelector::PATLeptonPATMetDelPhiSelector(const edm::ParameterSet& iConfig)
{
  src_ = iConfig.getParameter<edm::InputTag>("src");
  LeptonType_ = iConfig.getParameter<std::string>("LeptonType");
  MetSource_ = iConfig.getParameter<edm::InputTag>("MetSource");
  DelPhiMaxCut_ = iConfig.getParameter<double>("DelPhiMaxCut");
  DelPhiMinCut_ = iConfig.getParameter<double>("DelPhiMinCut");
  if(LeptonType_ == "Muon") {
    produces<PATMuonCollection>();
  } else {
    produces<PATElectronCollection>();
  }
}


PATLeptonPATMetDelPhiSelector::~PATLeptonPATMetDelPhiSelector() {}

void PATLeptonPATMetDelPhiSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;

  if(LeptonType_ == "Muon") {
    Handle<View<pat::Muon> > leptonHandle;
    iEvent.getByLabel(src_,leptonHandle);
    View<pat::Muon> leptons = *leptonHandle;

    Handle<View<pat::MET> > metHandle;
    iEvent.getByLabel(MetSource_,metHandle);
    View<pat::MET> mets = *metHandle;

    std::auto_ptr<PATMuonCollection> pOut(new PATMuonCollection);

    for(View<pat::Muon>::const_iterator trk = leptons.begin();trk != leptons.end(); ++trk) {
      bool goodLepton = true;
      for(View<pat::MET>::const_iterator metit = mets.begin();metit != mets.end(); ++metit) {
        double dphi = fabs(trk->phi() - metit->phi());
        if(dphi > acos(-1.)) {dphi = 2. * acos(-1.) - dphi;}
        if( (dphi<=DelPhiMinCut_) || (dphi>=DelPhiMaxCut_) ) {goodLepton = false;}
      }
      if (goodLepton) {pOut->push_back( * trk );}
      std::cout << "Good Lepton ? :  " << goodLepton << std::endl;
    }

    iEvent.put(pOut);

  } else if(LeptonType_ == "Electron") {
    Handle<View<pat::Electron> > leptonHandle;
    iEvent.getByLabel(src_,leptonHandle);
    View<pat::Electron> leptons = *leptonHandle;

    Handle<View<pat::MET> > metHandle;
    iEvent.getByLabel(MetSource_,metHandle);
    View<pat::MET> mets = *metHandle;

    std::auto_ptr<PATElectronCollection> pOut(new PATElectronCollection);

    for(View<pat::Electron>::const_iterator trk = leptons.begin();trk != leptons.end(); ++trk) {
      bool goodLepton = true;
      for(View<pat::MET>::const_iterator metit = mets.begin();metit != mets.end(); ++metit) {
        double dphi = fabs(trk->phi() - metit->phi());
        if(dphi > acos(-1.)) {dphi = 2. * acos(-1.) - dphi;}
        if( (dphi<=DelPhiMinCut_) || (dphi>=DelPhiMaxCut_) ) {goodLepton = false;}
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
void PATLeptonPATMetDelPhiSelector::beginJob(const edm::EventSetup&) {}

// ------------ method called once each job just after ending the event loop  ------------
void PATLeptonPATMetDelPhiSelector::endJob() {}
