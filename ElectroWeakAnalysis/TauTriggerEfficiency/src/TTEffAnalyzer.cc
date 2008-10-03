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
// $Id: TTEffAnalyzer.cc,v 1.1 2008/10/01 13:09:31 chinhan Exp $
//
//


#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/TTEffAnalyzer.h"
#include "Math/GenVector/VectorUtil.h"

//
TTEffAnalyzer::TTEffAnalyzer(const edm::ParameterSet& iConfig):
  PFTaus_(iConfig.getParameter<edm::InputTag>("PFTauCollection")),
  rootFile_(iConfig.getParameter<std::string>("outputFileName"))
{

  // File setup
  _TTEffFile = TFile::Open(rootFile_.c_str(), "RECREATE");
  //_TTEffFile = TFile::Open("test.root", "RECREATE");
  // Tree setup
  _TTEffTree = new TTree("TTEffTree", "Tau Trigger Efficiency Tree");

  _TTEffTree->Branch("PFTauPt", &PFPt, "PFTauPt/F");
  _TTEffTree->Branch("PFTauEt",&PFEt,"PFTauEt/F");
  _TTEffTree->Branch("PFTauEta", &PFEta, "PFTauEta/F");
  _TTEffTree->Branch("PFTauPhi", &PFPhi, "PFTauPhi/F");
  _TTEffTree->Branch("PFEGIsolEt",&PFEGIsolEt,"PFEGIsolEt/F");
  _TTEffTree->Branch("PFNEGammaCandsAnnulus",&NEGCandsInAnnulus,"PFNEGammaCandsAnnulus/I");
  _TTEffTree->Branch("PFNHadCandsAnnulus",&NHadCandsInAnnulus,"PFNHadCandsAnnulus/I");
  _TTEffTree->Branch("PFHighestClusterEt",&PFHighestClusterEt,"PFHighestClusterEt/F");

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

   edm::Handle<PFTauCollection> PFTaus;
   if(iEvent.getByLabel(PFTaus_, PFTaus)) {
     _L1analyzer.beginEvent(iEvent, iSetup);
     
     //std::cout << "Collection size " << PFTaus->size() << std::endl; 
     for(PFTauCollection::const_iterator iTau = PFTaus->begin(); iTau != PFTaus->end(); ++iTau) {
       // Fill PF Tau variables
       fillPFTau(*iTau);

       // Call individual analyzers
       _L1analyzer.analyzeTau(*iTau,iSetup);

       // Finally, fill the entry to tree
       _TTEffTree->Fill();
     }
   }
}

void
TTEffAnalyzer::fillPFTau(const reco::PFTau& tau) {
  // Standsrd PF variables
  PFPt = tau.pt();
  PFEt = tau.et();
  PFEta = tau.eta();
  PFPhi = tau.phi();
  PFEGIsolEt = tau.isolationPFGammaCandsEtSum();
  PFHighestClusterEt = tau.maximumHCALPFClusterEt();
  NEGCandsInAnnulus = tau.isolationPFGammaCands().size();
  NHadCandsInAnnulus = tau.isolationPFChargedHadrCands().size() + tau.isolationPFNeutrHadrCands().size();

}

// ------------ method called once each job just before starting event loop  ------------
void 
TTEffAnalyzer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TTEffAnalyzer::endJob() {
  //std::cout <<  "In endJob" << std::endl;
  _TTEffFile->Write();
  //std::cout << "After write" << std::endl; 
  _TTEffFile->Close();
  //std::cout << "After close" << std::endl;
  //delete _TTEffTree;
  //std::cout << "After delete" << std::endl;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(TTEffAnalyzer);
