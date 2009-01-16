// -*- C++ -*-
//
// Package:    PATLeptonPATTauOSSSChargeSelector
// Class:      PATLeptonPATTauOSSSChargeSelector
// 
/**\class PATLeptonPATTauOSSSChargeSelector PATLeptonPATTauOSSSChargeSelector.cc MyCode/PATLeptonPATTauOSSSChargeSelector/src/PATLeptonPATTauOSSSChargeSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Konstantinos A. Petridis
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id: PATLeptonPATTauOSSSChargeSelector.cc,v 1.1 2008/10/16 17:54:42 gurrola Exp $
//
//

#include "ElectroWeakAnalysis/EWKTau/interface/PATLeptonPATTauOSSSChargeSelector.h"

PATLeptonPATTauOSSSChargeSelector::PATLeptonPATTauOSSSChargeSelector(const edm::ParameterSet& iConfig)
{
  src_ = iConfig.getParameter<edm::InputTag>("LeptonSource");
  LeptonType_ = iConfig.getParameter<std::string>("LeptonType");
  TauSource_ = iConfig.getParameter<edm::InputTag>("TauSource");
  doOSSS_ = iConfig.getParameter<bool>("DoOSSS");
  eventWeight_ = iConfig.getParameter<double>("EventWeight");
  produces<PATTauCollection>();

  edm::Service<TFileService> fs;
  hLeptonCharge=fs->make<TH1F>("LeptonCharge","LeptonCharge",10,-5,5);
  hTauJetCharge=fs->make<TH1F>("TauJetCharge","TauJetCharge",10,-5,5);
  hCombinedCharge=fs->make<TH1F>("CombinedCharge","CombinedCharge",10,-5,5);
}


PATLeptonPATTauOSSSChargeSelector::~PATLeptonPATTauOSSSChargeSelector() {}

void PATLeptonPATTauOSSSChargeSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;

  if(LeptonType_ == "Muon") {
    Handle<View<pat::Muon> > leptonHandle;
    iEvent.getByLabel(src_,leptonHandle);
    View<pat::Muon> leptons = *leptonHandle;

    Handle<View<pat::Tau> > tauHandle;
    iEvent.getByLabel(TauSource_,tauHandle);
    View<pat::Tau> taus = *tauHandle;

    std::auto_ptr<PATTauCollection> pOut(new PATTauCollection);
    int lchg=0;
    if(leptons.size()&&taus.size()){
      lchg=leptons.at(0).charge();  
      hLeptonCharge->Fill(lchg,eventWeight_);
      for(View<pat::Tau>::const_iterator tauit = taus.begin();tauit != taus.end(); ++tauit) {
	int tchg=tauit->charge();
	hTauJetCharge->Fill(tchg,eventWeight_);
	hCombinedCharge->Fill(tchg*lchg*1.,eventWeight_);
	if( (tchg*lchg<0&&doOSSS_) ||
	    (tchg*lchg>0&&!doOSSS_) ){
	  pOut->push_back( *tauit );
	}
      }
      iEvent.put(pOut);
    }
  } else if(LeptonType_ == "Electron") {
    Handle<View<pat::Electron> > leptonHandle;
    iEvent.getByLabel(src_,leptonHandle);
    View<pat::Electron> leptons = *leptonHandle;

    Handle<View<pat::Tau> > tauHandle;
    iEvent.getByLabel(TauSource_,tauHandle);
    View<pat::Tau> taus = *tauHandle;

    std::auto_ptr<PATTauCollection> pOut(new PATTauCollection);
    int lchg=0;
    if(leptons.size()&&taus.size()){
      lchg=leptons.at(0).charge();  
      hLeptonCharge->Fill(lchg,eventWeight_);
      for(View<pat::Tau>::const_iterator tauit = taus.begin();tauit != taus.end(); ++tauit) {
	int tchg=tauit->charge();
	hTauJetCharge->Fill(tchg,eventWeight_);
	hCombinedCharge->Fill(tchg*lchg*1.,eventWeight_);
	if( (tchg*lchg<0&&doOSSS_) ||
	    (tchg*lchg>0&&!doOSSS_) ){
	  pOut->push_back( *tauit );
	}
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
void PATLeptonPATTauOSSSChargeSelector::beginJob(const edm::EventSetup&) {}

// ------------ method called once each job just after ending the event loop  ------------
void PATLeptonPATTauOSSSChargeSelector::endJob() {}
