// -*- C++ -*-
//
// Package:    EgammaAnalysis/Examples
// Class:      ElectronIDFakeRateAnalyzer
// 
/**\class ElectronIDFakeRateAnalyzer ElectronIDFakeRateAnalyzer.cc EgammaAnalysis/Examples/src/ElectronIDFakeRateAnalyzer.cc

 Description: module to analyze electron fake rate 

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pedro Ribeiro
//         Created:  Thu Jul 26 12:07:34 WEST 2007
// $Id$
//
//


// system include files
#include <memory>
#include <algorithm>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDFwd.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
//#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"


#include "EgammaAnalysis/Examples/interface/ElectronIDFakeRateAnalyzer.h"

ElectronIDFakeRateAnalyzer::ElectronIDFakeRateAnalyzer(const edm::ParameterSet& iConfig) :
  tRecoElectrons(0),tSelElectrons(0),tRecoJets(0),tGenJets(0),tGenPartons1(0),tGenPartons2(0),tEvents(0)
{
  
  std::cout<<"Getting Parameters"<<std::endl;

  PtCut = iConfig.getParameter<double>("PtCut");
  EtaCut = iConfig.getParameter<double>("EtaCut");
  JetPtCut = iConfig.getParameter<double>("JetPtCut");
  JetEtaCut = iConfig.getParameter<double>("JetEtaCut");
  JetEtCut = iConfig.getParameter<double>("JetEtCut");
  dRElecJetCut = iConfig.getParameter<double>("dRElecJetCut");

  datasetName = (TString)iConfig.getParameter<std::string>("DatasetName");

  loadCMSSWObjects(iConfig);
  loadHistoParameters(iConfig);

  rootFile_ = TFile::Open(iConfig.getParameter<std::string>("outputFile").c_str(),"RECREATE");
  rootFile_->cd();

  hRecoElecPT = new TH1D("HRecoElecPT","# Reco electrons as function of p_{T}",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_);
  hSelElecPT = new TH1D("HSelElecPT","# Good electrons as function of p_{T}",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_);
  hRecoJetsPT = new TH1D("HRecoJetsPT","# Reco Jets ",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_);
  hGenJetsPT = new TH1D("HGenJetsPT","# Gen Jets ",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_);
  hGenPartons1PT = new TH1D("HGenPartons1PT","# hard partons ",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_);
  hGenPartons2PT = new TH1D("HGenPartons2PT","# partons ",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_);
  hEventsPT = new TH1D("HEventsPT","# events*2",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_);

  hRecoElecEta = new TH1D("HRecoElecEta","# Reco electrons as function of #eta",histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hSelElecEta = new TH1D("HSelElecEta","# Good electrons as function of #eta",histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hRecoJetsEta = new TH1D("HRecoJetsEta","# Reco Jets ",histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hGenJetsEta = new TH1D("HGenJetsEta","# Gen Jets ",histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hGenPartons1Eta = new TH1D("HGenPartons1Eta","# hard partons ",histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hGenPartons2Eta = new TH1D("HGenPartons2Eta","# partons ",histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hEventsEta = new TH1D("HEventsEta","# events*2",histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);

  hRecoElec = new TH2D("HRecoElec","# Reco electrons as function of p_{T} and #eta",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_,
		       histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hSelElec = new TH2D("HSelElec","# Good electrons as function of p_{T} and #eta",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_,
		      histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hRecoJets = new TH2D("HRecoJets","# Reco Jets ",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_,
		       histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hGenJets = new TH2D("HGenJets","# Gen Jets ",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_,
		      histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hGenPartons1 = new TH2D("HGenPartons1","# hard partons ",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_,
			  histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hGenPartons2 = new TH2D("HGenPartons2","# partons ",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_,
			  histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);
  hEvents = new TH2D("HEvents","# events*2",histPT_bins_Pt_,histPT_min_Pt_,histPT_max_Pt_,
			    histEta_bins_Eta_,histEta_min_Eta_,histEta_max_Eta_);

  hStatistics= new TH1D ("hStatistics","Event Statistics",8,1,9);

  hFakeNumEt= new TH1D("HFakeNumEt"," et of jets matched to electrons  ",14,10,80);
  hFakeNumSelEt= new TH1D("HFakeNumSelEt"," et of jets matched to good electrons  ",14,10,80);
  hFakeDenEt= new TH1D("HFakeDenEt"," et of all jets",14,10,80);
  hFakeNumEta= new TH1D("HFakeNumEta","#eta of jets matched to electrons  ",50,-2.5,2.5);
  hFakeNumSelEta= new TH1D("HFakeNumSelEta","#eta of jets matched to good electrons  ",50,-2.5,2.5);
  hFakeDenEta= new TH1D("HFakeDenEta","#eta of all jets",50,-2.5,2.5);
  hdRminElecJet= new TH1D("HdRminElecjet"," dR min between electron and jets",50,0,2.0);
  hEtJetvsEtElectron = new TH2D ("HEtJetvsEtElectron"," Et of electron vs Et of Jet ",70,10,80,75,5,80);  

}

ElectronIDFakeRateAnalyzer::~ElectronIDFakeRateAnalyzer()
{
 

  delete rootFile_;


}

// ------------ method called to for each event  ------------
void
ElectronIDFakeRateAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;

  double MinElectronPT = PtCut;
  double MaxElectronEta = EtaCut;
  
  int nRecoElectrons = 0;
  int nSelElectrons = 0;
  int nRecoJets = 0;
  int nGenJets = 0;
  int nGenPartons1 = 0;
  int nGenPartons2 = 0;


  Handle<reco::CandidateCollection> genParticlesHandle;
  iEvent.getByLabel(MCTruthCollection_, genParticlesHandle);
  const reco::CandidateCollection genParticles = *(genParticlesHandle.product());

  
  Handle<reco::PixelMatchGsfElectronCollection> gsfElectronsHandle;
  iEvent.getByLabel(RecoCollection_,gsfElectronsHandle);
  const reco::PixelMatchGsfElectronCollection gsfElectrons = *(gsfElectronsHandle.product());

  //get Association Map between electrons and electron ID information from event
  
  Handle<reco::ElectronIDAssociationCollection> eleIDassociationHandle;
  iEvent.getByLabel(ElectronIDAssocMap_,eleIDassociationHandle);
  const reco::ElectronIDAssociationCollection eleIDassociation = *(eleIDassociationHandle.product());

  // get Gen Jets
  Handle<reco::GenJetCollection> genJetsH;
  iEvent.getByLabel(GenJetCollection_,genJetsH);
  const reco::GenJetCollection genJets = *(genJetsH.product());

  //get Reco Jets
  Handle<reco::CaloJetCollection> jetsH;
  iEvent.getByLabel(RecoJetCollection_,jetsH);
  const reco::CaloJetCollection recoJets = (*jetsH.product());

 
  for (reco::GenJetCollection::const_iterator jet = genJets.begin();jet!=genJets.end();jet++){
    if (jet->pt()>=JetPtCut && fabs(jet->eta())<JetEtaCut) {
      nGenJets+=1;
    }
  }

  for (reco::CaloJetCollection::const_iterator jet = recoJets.begin();jet!=recoJets.end();jet++){
    if (jet->pt()>=JetPtCut && fabs(jet->eta())<JetEtaCut) {
      nRecoJets+=1;
    }
  }
 
  for (reco::CandidateCollection::const_iterator parton = genParticles.begin();parton!=genParticles.end();parton++){
    if ( parton->status()==3){
      if (parton->pdgId()==21 || 
	  abs(parton->pdgId())==1 || abs(parton->pdgId())==2 || abs(parton->pdgId())==3 ||
	  abs(parton->pdgId())==4 || abs(parton->pdgId())==5 ) {
	if (parton->pt()>=JetPtCut && fabs(parton->eta())<JetEtaCut){
	  if (parton-genParticles.begin()==6 || parton-genParticles.begin()==7) nGenPartons1+=1;
	  if (parton-genParticles.begin()>=6) nGenPartons2+=1;

	}
      }
    }
  }
 
      
  reco::PixelMatchGsfElectronCollection::const_iterator iElectron ;
  for (iElectron = gsfElectrons.begin();iElectron!= gsfElectrons.end();iElectron++) {
    if (iElectron->pt()>=MinElectronPT && fabs(iElectron->eta()) < MaxElectronEta) {
      nRecoElectrons+=1;	   
      hRecoElecPT->Fill(iElectron->pt());
      hRecoElecEta->Fill(iElectron->eta());
      hRecoElec->Fill(iElectron->pt(),iElectron->eta());
    }
  }
  
  reco::ElectronIDAssociationCollection::const_iterator iAssoc;
  for (iAssoc = eleIDassociation.begin();iAssoc != eleIDassociation.end(); iAssoc++){
    if (iAssoc->val->cutBasedDecision() == true && iAssoc->key->pt()>=MinElectronPT && fabs(iAssoc->key->eta()) < MaxElectronEta ) {
      nSelElectrons +=1;
      hSelElecPT->Fill(iAssoc->key->pt());
      hSelElecEta->Fill(iAssoc->key->eta());
      hSelElec->Fill(iAssoc->key->pt(),iAssoc->key->eta());
    }
  }


  //differential distributions
  
  for (int binx = 1; binx <= hRecoJetsPT->GetNbinsX(); binx++){    
    hRecoJetsPT->AddBinContent(binx,nRecoJets);
    hGenJetsPT->AddBinContent(binx,nGenJets);
    hGenPartons1PT->AddBinContent(binx,nGenPartons1);
    hGenPartons2PT->AddBinContent(binx,nGenPartons2);
    hEventsPT->AddBinContent(binx,2);

  }

  for (int binx = 1; binx <= hRecoJetsEta->GetNbinsX(); binx++){    
    hRecoJetsEta->AddBinContent(binx,nRecoJets);
    hGenJetsEta->AddBinContent(binx,nGenJets);
    hGenPartons1Eta->AddBinContent(binx,nGenPartons1);
    hGenPartons2Eta->AddBinContent(binx,nGenPartons2);
    hEventsEta->AddBinContent(binx,2);
  }

  for (int binx = 1; binx <= hRecoJets->GetNbinsX(); binx++){    
    for (int biny = 1;  biny<=hRecoJets->GetNbinsY(); biny++){
      int gbin = hRecoJets->GetBin(binx,biny);
      hRecoJets->AddBinContent(gbin,nRecoJets);
      hGenJets->AddBinContent(gbin,nGenJets);
      hGenPartons1->AddBinContent(gbin,nGenPartons1);
      hGenPartons2->AddBinContent(gbin,nGenPartons2);
      hEvents->AddBinContent(gbin,2);
    }
  }

  tRecoElectrons+=nRecoElectrons;
  tSelElectrons+=nSelElectrons;
  tRecoJets+=nRecoJets;
  tGenJets+=nGenJets;
  tGenPartons1+=nGenPartons1;
  tGenPartons2+=nGenPartons2;

  nEvents+=1;
  tEvents+=1;


  //other definition of fake rate
  
  for (iElectron = gsfElectrons.begin();iElectron!= gsfElectrons.end();iElectron++) {
    if (iElectron->pt()>=MinElectronPT && fabs(iElectron->eta()) < MaxElectronEta) {

      double dRmin = 999.0; 
      const reco::CaloJet * theJet  = &(recoJets[0]);  

      for (reco::CaloJetCollection::const_iterator jet = recoJets.begin();jet!=recoJets.end();jet++){
	if (jet->et()>=JetEtCut && fabs(jet->eta())<JetEtaCut) {
	  double thedR = getDeltaR(&(*jet),&(*iElectron));  
	  if (thedR < dRmin)  {
	    dRmin = thedR;
	    theJet=&(*jet);
	  }
	} 
      }

      hdRminElecJet->Fill(dRmin);

      if (dRmin<=dRElecJetCut)   {
	hFakeNumEt->Fill(theJet->et());
	hFakeNumEta->Fill(theJet->eta());
	hEtJetvsEtElectron->Fill(theJet->et(),iElectron->et());
	
	int index = int(&(*iElectron)-&(gsfElectrons.front()));
	reco::PixelMatchGsfElectronRef electronRef (gsfElectronsHandle,index);		        
	reco::ElectronIDAssociationCollection::const_iterator iAssoc1_ = eleIDassociation.find(electronRef);
	const reco::ElectronIDRef electronID = iAssoc1_->val;
	if (electronID->cutBasedDecision() == true) {
	  hFakeNumSelEt->Fill(theJet->et());
	  hFakeNumSelEta->Fill(theJet->eta());
	}
      }
    }
  }

  for (reco::CaloJetCollection::const_iterator jet = recoJets.begin();jet!=recoJets.end();jet++){
    if (jet->et()>=JetEtCut && fabs(jet->eta())<JetEtaCut) {
      hFakeDenEt->Fill(jet->et());
      hFakeDenEta->Fill(jet->eta());
    }   
  }
}

// ------------ method called once each job just before starting event loop  ------------
void 
ElectronIDFakeRateAnalyzer::beginJob(const edm::EventSetup&)
{
  nEvents = 0; 

  for (int binx = 1; binx <= hRecoJetsPT->GetNbinsX(); binx++){    
    hRecoJetsPT->SetBinContent(binx,0);
    hGenJetsPT->SetBinContent(binx,0);
    hGenPartons1PT->SetBinContent(binx,0);
    hGenPartons2PT->SetBinContent(binx,0);
    hEventsPT->SetBinContent(binx,0);
    
  }

  for (int binx = 1; binx <= hRecoJetsEta->GetNbinsX(); binx++){    
    hRecoJetsEta->SetBinContent(binx,0);
    hGenJetsEta->SetBinContent(binx,0);
    hGenPartons1Eta->SetBinContent(binx,0);
    hGenPartons2Eta->SetBinContent(binx,0);
    hEventsEta->SetBinContent(binx,0);
  }

  for (int binx = 1; binx <= hRecoJets->GetNbinsX(); binx++){    
    for (int biny = 1;  biny<=hRecoJets->GetNbinsY(); biny++){
      int gbin = hRecoJets->GetBin(binx,biny);
      hRecoJets->SetBinContent(gbin,0);
      hGenJets->SetBinContent(gbin,0);
      hGenPartons1->SetBinContent(gbin,0);
      hGenPartons2->SetBinContent(gbin,0);
      hEvents->SetBinContent(gbin,0);
    }
  }


}

// ------------ method called once each job just after ending the event loop  ------------
void 
ElectronIDFakeRateAnalyzer::endJob() {

  hStatistics->SetBinContent(1,tRecoElectrons);
  hStatistics->SetBinContent(2,tSelElectrons);
  hStatistics->SetBinContent(3,tRecoJets);
  hStatistics->SetBinContent(4,tGenJets);
  hStatistics->SetBinContent(5,tGenPartons1);
  hStatistics->SetBinContent(6,tGenPartons2);
  hStatistics->SetBinContent(7,tEvents*2);
  hStatistics->SetBinContent(8,tEvents);

  hRecoJets->SetEntries(tEvents);
  hGenJets->SetEntries(tEvents);
  hGenPartons1->SetEntries(tEvents);
  hGenPartons2->SetEntries(tEvents);
  hEvents->SetEntries(tEvents);

  hRecoJetsPT->SetEntries(tEvents);
  hGenJetsPT->SetEntries(tEvents);
  hGenPartons1PT->SetEntries(tEvents);
  hGenPartons2PT->SetEntries(tEvents);
  hEventsPT->SetEntries(tEvents);

  hRecoJetsEta->SetEntries(tEvents);
  hGenJetsEta->SetEntries(tEvents);
  hGenPartons1Eta->SetEntries(tEvents);
  hGenPartons2Eta->SetEntries(tEvents);
  hEventsEta->SetEntries(tEvents);

  rootFile_->cd();
  rootFile_->Write();
  rootFile_->Close();
 
}

void ElectronIDFakeRateAnalyzer::loadCMSSWObjects(const edm::ParameterSet& iConfig)
{
  MCTruthCollection_ = iConfig.getParameter<edm::InputTag>("MCTruthCollection");
  ElectronIDAssocMap_ = iConfig.getParameter<edm::InputTag>("ElectronIDAssociationMap");
  RecoCollection_=iConfig.getParameter<edm::InputTag>("ElectronRecoCollection");
  RecoJetCollection_=iConfig.getParameter<edm::InputTag>("RecoJetCollection");
  GenJetCollection_=iConfig.getParameter<edm::InputTag>("GenJetCollection");  

}

void ElectronIDFakeRateAnalyzer::loadHistoParameters(const edm::ParameterSet& iConfig)
{

  histPT_min_Pt_ = iConfig.getParameter<double>("hist_min_PT");
  histPT_max_Pt_ = iConfig.getParameter<double>("hist_max_PT");
  histPT_bins_Pt_ = iConfig.getParameter<int>("hist_bins_PT");

  histEta_min_Eta_ = iConfig.getParameter<double>("hist_min_Eta");
  histEta_max_Eta_ = iConfig.getParameter<double>("hist_max_Eta");
  histEta_bins_Eta_ = iConfig.getParameter<int>("hist_bins_Eta");

}

double ElectronIDFakeRateAnalyzer::getDeltaR (const reco::Candidate* candidate1, const reco::Candidate* candidate2) {
  return sqrt (pow(candidate1->eta()-candidate2->eta(),2)+pow(getDeltaPhi(candidate1,candidate2),2));
}

double ElectronIDFakeRateAnalyzer::getDeltaPhi (const reco::Candidate* candidate1,const reco::Candidate* candidate2) {
  double diff = fabs ( candidate2->phi()-candidate1->phi()); 
  double corr = 2*acos(-1.)-diff;
  if (diff < acos(-1.)) {return diff;} else {return corr;}
}

//define this as a plug-in
DEFINE_FWK_MODULE(ElectronIDFakeRateAnalyzer);
