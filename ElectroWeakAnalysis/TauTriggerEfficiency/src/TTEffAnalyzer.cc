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
// $Id: TTEffAnalyzer.cc,v 1.19 2009/04/04 06:48:55 smaruyam Exp $
//
//

#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/TTEffAnalyzer.h"
#include "Math/GenVector/VectorUtil.h"

TTEffAnalyzer::TTEffAnalyzer(const edm::ParameterSet& iConfig):
  PFTaus_(iConfig.getParameter<edm::InputTag>("PFTauCollection")),
  PFTauIso_(iConfig.getParameter<edm::InputTag>("PFTauIsoCollection")),
  MCTaus_(iConfig.getParameter<edm::InputTag>("MCTauCollection")),
  rootFile_(iConfig.getParameter<std::string>("outputFileName"))
{
  // File setup
  _TTEffFile = TFile::Open(rootFile_.c_str(), "RECREATE");
  //_TTEffFile = TFile::Open("test.root", "RECREATE");
  // Tree setup
  _TTEffTree = new TTree("TTEffTree", "Tau Trigger Efficiency Tree");

  //reset vars
  PFPt = 0.;
  PFInvPt = 0.;
  PFEt = 0.;
  PFEta = 0.;
  PFPhi = 0.;
  PFProng = 0.;
  PFIso = 0;
  PFIsoSum = 0;
  PFEnergy = 0;

  _TTEffTree->Branch("PFTauPt", &PFPt, "PFTauPt/F");
  _TTEffTree->Branch("PFTauInvPt", &PFInvPt, "PFTauInvPt/F");
  _TTEffTree->Branch("PFTauEt",&PFEt,"PFTauEt/F");
  _TTEffTree->Branch("PFTauEta", &PFEta, "PFTauEta/F");
  _TTEffTree->Branch("PFTauPhi", &PFPhi, "PFTauPhi/F");
  _TTEffTree->Branch("PFTauProng", &PFProng, "PFTauProng/F");
  _TTEffTree->Branch("PFTauIso", &PFIso, "PFTauIso/F");
  _TTEffTree->Branch("PFTauIsoSum", &PFIsoSum, "PFTauIsoSum/F");
  _TTEffTree->Branch("PFTauEnergy", &PFEnergy, "PFTauEnergy/F");

  _L1analyzer.Setup(iConfig,_TTEffTree);
  _L2analyzer.Setup(iConfig,_TTEffTree);
  _L25and3analyzer.Setup(iConfig,_TTEffTree);
}

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

   edm::Handle<std::vector<LorentzVector> > mcTaus;
   edm::Handle<CaloTauCollection> caloTaus;
   edm::Handle<reco::GsfElectronCollection> electronTaus;

   if(iEvent.getByLabel(MCTaus_, mcTaus)){
    loop(iEvent, *mcTaus);
    }
   else if(iEvent.getByLabel(PFTaus_, PFTaus)) {
      try{
	iEvent.getByLabel(PFTauIso_,thePFTauDiscriminatorByIsolation);
      }catch(...){}
	loop(iEvent, *PFTaus);
   }  
   else if(iEvent.getByLabel(PFTaus_, caloTaus)) {
     loop(iEvent, *caloTaus);
   }
   else if(iEvent.getByLabel(PFTaus_, electronTaus)) {
     loop(iEvent, *electronTaus);
   }
   
   // For electron lorentzvectors, add similar clauses
}

void TTEffAnalyzer::fill(const LorentzVector& tau,unsigned int i) {
  PFPt = tau.Pt();
  PFEt = tau.Et();
  PFEta = tau.Eta();
  PFPhi = tau.Phi();
}

void TTEffAnalyzer::fill(const reco::GsfElectron& tau,unsigned int i) {
  fill(tau.p4());
}

void
TTEffAnalyzer::fill(const reco::PFTau& tau,unsigned int i) {
// Here is workaround, though not beautiful, to access PFIso info with PFRef
// While keeping the generic structure of loop method
  const PFTauRef thisTauRef(PFTaus,i);
  if(thePFTauDiscriminatorByIsolation.isValid()){
	const PFTauDiscriminator & ds = *(thePFTauDiscriminatorByIsolation.product());
	PFIso = ds[thisTauRef];// it should crash if CMSSW cannot find a match between TauRef and Iso collection
                            // Then check configuration files to make sure a correct pair is being fed into TTEFF
  }
   if(thisTauRef->leadPFChargedHadrCand().isNonnull()) PFInvPt = 1./thisTauRef->leadPFChargedHadrCand()->pt();
  // Fill common variables
   fill(PFTaus->at(i));

  // Fill #signal tracks, and PtSum in isolation annulus 
  PFProng  = PFTaus->at(i).signalPFChargedHadrCands().size(); // check config file
  PFIsoSum = PFTaus->at(i).isolationPFChargedHadrCandsPtSum();
  PFEnergy = PFTaus->at(i).energy();

  /*
  //get RMS Values of Candidates
  std::vector<double> rms = clusterSeparation(tau.isolationPFGammaCands(),tau.signalPFCands());
  PFEGEtaRMS = rms[0];
  PFEGPhiRMS = rms[1];
  PFEGDrRMS = rms[2];
  */
}

void TTEffAnalyzer::fill(const reco::CaloTau& tau,unsigned int i) {
  fill(tau.p4());
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


std::vector<double> 
TTEffAnalyzer::clusterSeparation(const reco::PFCandidateRefVector& isol_cands,const reco::PFCandidateRefVector& signal_cands)
{
  LV center(0.,0.,0.,0.);
  
  //find the weighted position
  if(isol_cands.size()>0)
  for(reco::PFCandidateRefVector::const_iterator i=isol_cands.begin();i!=isol_cands.end();++i)
    {
      center+=(*i)->p4();
    }

  if(signal_cands.size()>0)
  for(reco::PFCandidateRefVector::const_iterator i=signal_cands.begin();i!=signal_cands.end();++i)
    {
      center+=(*i)->p4();
    }

  //Now find the rms
  double sumet=0;
  double etarms=0;
  double phirms=0;
  double drrms=0;

  if(isol_cands.size()>0)
  for(reco::PFCandidateRefVector::const_iterator i=isol_cands.begin();i!=isol_cands.end();++i)
    {
      sumet+=(*i)->et();
      etarms+=(*i)->et()*pow((*i)->eta()-center.Eta(),2);
      phirms+=(*i)->et()*pow(ROOT::Math::VectorUtil::DeltaPhi(center,(*i)->p4()),2);
      drrms+=(*i)->et()*pow(ROOT::Math::VectorUtil::DeltaR(center,(*i)->p4()),2);

    }

  if(sumet<0.1)
    sumet=1;

  std::vector<double> out;
  out.push_back(etarms/sumet);
  out.push_back(phirms/sumet);
  out.push_back(drrms/sumet);

  return out;
}
