// -*- C++ -*-
//
// Package:    TTEffAnalyzer
// Class:      TTEffAnalyzer
// 
/**\class TTEffAnalyzer TTEffAnalyzer.cc ElectroWeakAnalysis/TTEffAnalyzer/src/TTEffAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Chi Nhan Nguyen
//         Created:  Wed Oct  1 13:04:54 CEST 2008
// $Id$
//
//


#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/TTEffAnalyzer.h"

//
TTEffAnalyzer::TTEffAnalyzer(const edm::ParameterSet& iConfig)
{

  // File setup
  //TTEffFile = TFile::Open(rootFile_.c_str(), "RECREATE");
  _TTEffFile = TFile::Open("test.root", "RECREATE");
  // Tree setup
  _TTEffTree = new TTree("TTEffTree", "Tau Trigger Efficiency Tree");

  _L1analyzer.Setup(iConfig,_TTEffTree);

}

//
TTEffAnalyzer::~TTEffAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TTEffAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   _L1analyzer.analyze(iEvent,iSetup);
  
}

// ------------ method called once each job just before starting event loop  ------------
void 
TTEffAnalyzer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TTEffAnalyzer::endJob() {
  _TTEffFile->Write();
}

//define this as a plug-in
//DEFINE_FWK_MODULE(TTEffAnalyzer);
