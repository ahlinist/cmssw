// -*- C++ -*-
//
// Package:    EgammaAnalysis/Examples
// Class:      ElectronIDValAnalyzer
// 
/**\class ElectronIDValAnalyzer ElectronIDValAnalyzer.cc EgammaAnalysis/Examples/src/ElectronIDValAnalyzer.cc

 Description: Tool to perform validation of electron identification

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pedro Ribeiro
//         Created:  Thu Jul 26 12:07:34 WEST 2007
// $Id: ElectronIDValAnalyzer.cc,v 1.1 2007/11/06 11:43:16 ppereira Exp $
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

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDFwd.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/Particle.h"

#include "EgammaAnalysis/Examples/interface/ElectronIDValAnalyzer.h"
#include "EgammaAnalysis/Examples/interface/MCMatchingTools.h"

ElectronIDValAnalyzer::ElectronIDValAnalyzer(const edm::ParameterSet& iConfig) : mcMatchingTools(iConfig),
tmc(0),treco(0),tgood(0),tmatchedreco(0),tmatchedgood(0)
{

  std::cout<<"Getting Parameters"<<std::endl;

  PtCut = iConfig.getParameter<double>("PtCut");
  EtaCut = iConfig.getParameter<double>("EtaCut");
  maxdEoverECrack = iConfig.getParameter<double>("maxdEoverECrack");
  maxdEoverEBarrel = iConfig.getParameter<double>("maxdEoverEBarrel");
  maxdEoverEEndcap = iConfig.getParameter<double>("maxdEoverEEndcap");

  datasetName = (TString)iConfig.getParameter<std::string>("DatasetName");

  loadCMSSWObjects(iConfig);
  loadHistoParameters(iConfig);

  rootFile_ = TFile::Open(iConfig.getParameter<std::string>("outputFile").c_str(),"RECREATE");

  rootFile_->cd();
  rootFile_->mkdir("Reconstruction");
  rootFile_->mkdir("Identification");
  rootFile_->mkdir("Rec&Id");
  rootFile_->mkdir("RecPurity");
  rootFile_->mkdir("IdPurity");
  rootFile_->mkdir("Energy");
  rootFile_->mkdir("MisCharge");


  hMCElecPT = new TH1D("HMCElecPT","# MC electrons as function of p_{T}",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);
  hRECElecPT = new TH1D("HRECElecPT","# RC electrons that match MC as function of MC electron p_{T}",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);
  hRECElecPTEne = new TH1D("HRECElecPTEne","# RC electrons that match MC as function of MC electron p_{T} (energy requirement)",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);
  hSelElecPT =new TH1D("HSelElecPT","Electron Selection Efficiency vs electron MC p_{T}",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);
  hSelElecPTEne =new TH1D("HSelElecPTEne","Electron Selection Efficiency vs electron MC p_{T} (energy requirement)",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);
  hRECElecPTAllCharge = new TH1D("HRECElecPTAllCharge","# RC electrons that match MC as function of MC electron p_{T} (all charge)",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);
  hRECElecPTChargeMis = new TH1D("HRECElecPTChargeMis","# RC electrons that match MC as function of MC electron p_{T} (charge req)",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);       
  hSelElecPTAllCharge = new TH1D("HSelElecPTAllCharge","# Sel electrons that match MC as function of MC electron p_{T} (all charge)",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);
  hSelElecPTChargeMis = new TH1D("HSelElecPTChargeMis","# Sel electrons that match MC as function of MC electron p_{T} (charge req)",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);
        
  hRECElecPT1 = new TH1D("HRECElecPT1","# RC electrons that match MC as function of Reco electron p_{T}",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);
  hRECElecPT2 = new TH1D("HRECElecPT2","# Reco electrons as function of Reco electron p_{T}",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);
  hSelElecPT1 =new TH1D("HSelElecPT1","# Good electrons that match MC as function of Reco electron  p_{T}",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);
  hSelElecPT2 =new TH1D("HSelElecPT2","# Good electrons as function of  Reco electron p_{T}",histEffPT_bins_Pt_,histEffPT_min_Pt_,histEffPT_max_Pt_);

  hMCElecEta = new TH1D("HMCElecEta","# MC electrons as function of #eta",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);
  hRECElecEta = new TH1D("HRECElecEta","# RC electrons that match MC as function of MC electron #eta",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);
  hSelElecEta =new TH1D("HSelElecEta","Electron Selection Efficiency vs electron MC #eta",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);
  hRECElecEtaEne = new TH1D("HRECElecEtaEne","# RC electrons that match MC as function of MC electron #eta (energy requirement)",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);
  hSelElecEtaEne =new TH1D("HSelElecEtaEne","Electron Selection Efficiency vs electron MC #eta (energy requirement)",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);
  hRECElecEtaAllCharge = new TH1D("HRECElecEtaAllCharge","# RC electrons that match MC as function of MC electron p_{T} (all charge)",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);
  hRECElecEtaChargeMis = new TH1D("HRECElecEtaChargeMis","# RC electrons that match MC as function of MC electron p_{T} (charge req)",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);
  hSelElecEtaAllCharge = new TH1D("HSelElecEtaAllCharge","# Sel electrons that match MC as function of MC electron p_{T} (all charge)",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);
  hSelElecEtaChargeMis = new TH1D("HSelElecEtaChargeMis","# Sel electrons that match MC as function of MC electron p_{T} (charge req)",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);        

  hRECElecEta1 = new TH1D("HRECElecEta1","# RC electrons that match MC as function of Reco electron #eta",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);
  hRECElecEta2 = new TH1D("HRECElecEta2","# Reco electrons as function of Reco electron #eta",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);
  hSelElecEta1 =new TH1D("HSelElecEta1","# Good electrons that match MC as function of Reco electron #eta",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);
  hSelElecEta2 =new TH1D("HSelElecEta2","# Good electrons as function of  Reco electron #eta",histEffEta_bins_Eta_,histEffEta_min_Eta_,histEffEta_max_Eta_);

  hMCElecPhi = new TH1D("HMCElecPhi","# MC electrons as function of #phi",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);
  hRECElecPhi = new TH1D("HRECElecPhi","# RC electrons that match MC as function of MC electron #phi",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);
  hSelElecPhi =new TH1D("HSelElecPhi","Electron Selection Efficiency vs electron MC #phi",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);
  hRECElecPhiEne =new TH1D("HRECElecPhiEne","# RC electrons that match MC as function of MC electron #eta (energy requirement)",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);
  hSelElecPhiEne =new TH1D("HSelElecPhiEne","Electron Selection Efficiency vs electron MC #phi (energy requirement)",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);
  hRECElecPhiAllCharge = new TH1D("HRECElecPhiAllCharge","# RC electrons that match MC as function of MC electron p_{T} (all charge)",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);
  hRECElecPhiChargeMis = new TH1D("HRECElecPhiChargeMis","# RC electrons that match MC as function of MC electron p_{T} (charge req)",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);
  hSelElecPhiAllCharge = new TH1D("HSelElecPhiAllCharge","# Sel electrons that match MC as function of MC electron p_{T} (all charge)",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);
  hSelElecPhiChargeMis = new TH1D("HSelElecPhiChargeMis","# Sel electrons that match MC as function of MC electron p_{T} (charge req)",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);   

  hRECElecPhi1 = new TH1D("HRECElecPhi1","# RC electrons that match MC as function of Reco electron #phi",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);
  hRECElecPhi2 = new TH1D("HRECElecPhi2","# Reco electrons as function of Reco electron #phi",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);
  hSelElecPhi1 =new TH1D("HSelElecPhi1","# Good electrons that match MC as function of Reco electron #phi",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);
  hSelElecPhi2 =new TH1D("HSelElecPhi2","# Good electrons as function of  Reco electron #phi",histEffPhi_bins_Phi_,histEffPhi_min_Phi_,histEffPhi_max_Phi_);

  hDPByPMatch = new TH1D("HDPByPMatch","#Deltap_{T}/p_{T}",100,0,10);

  
  hMCNElec = new TH1D("HMCNElec","# MC electrons",histNElec_bins_,histNElec_min_,histNElec_max_);
  hRECNElec = new TH1D("HRECNElec","# Reco electrons",histNElec_bins_,histNElec_min_,histNElec_max_);
  hSelNElec =new TH1D("HSelNElec","# of good electrons",histNElec_bins_,histNElec_min_,histNElec_max_);

  rootFile_->cd("/Reconstruction");

  hRECEffInclusive= new TH1D("HRECEffInclusive","Inclusive distribution for N matched reco electrons",histEffInclusive_bins_,histEffInclusive_min_,histEffInclusive_max_);

  rootFile_->cd("/Identification");

  hSelEffInclusive= new TH1D("HSelEffInclusive","Inclusive distribution for N matched good electrons",histEffInclusive_bins_,histEffInclusive_min_,histEffInclusive_max_);

  rootFile_->cd();

  efficiencyInf = new TTree ("EfficiencyInf","Efficiency Information");

}

ElectronIDValAnalyzer::~ElectronIDValAnalyzer()
{

  delete rootFile_;


}


// ------------ method called to for each event  ------------
void
ElectronIDValAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;

  double MinElectronPT = PtCut;
  double MaxElectronEta = EtaCut;
  
  int nmc = 0;
  int nreco = 0;
  int ngood = 0;
  int nmatchedreco = 0;
  int nmatchedgood = 0;


  //get MC truth

  Handle<reco::CandidateCollection> genParticlesHandle;
  iEvent.getByLabel(MCTruthCollection_, genParticlesHandle);
  const reco::CandidateCollection genParticles = *(genParticlesHandle.product());

  //get Electron Collection from the event

  Handle<reco::PixelMatchGsfElectronCollection> gsfElectronsHandle;
  iEvent.getByLabel(RecoCollection_,gsfElectronsHandle);
  const reco::PixelMatchGsfElectronCollection gsfElectrons = *(gsfElectronsHandle.product());

  //get Association Map between electrons and electron ID information from event
  
  Handle<reco::ElectronIDAssociationCollection> eleIDassociationHandle;
  iEvent.getByLabel(ElectronIDAssocMap_,eleIDassociationHandle);
  const reco::ElectronIDAssociationCollection eleIDassociation = *(eleIDassociationHandle.product());


  //store pointers to Gen particles   

  std::vector<const reco::Candidate*> particleCollection; 
  reco::CandidateCollection::const_iterator iGenPart ;
  for (iGenPart = genParticles.begin();iGenPart!= genParticles.end();iGenPart++) {
    particleCollection.push_back(&(*iGenPart));
  }

  //store pointers to GsfElectrons 
    
  std::vector<const reco::Candidate*> electronCollection;
  reco::PixelMatchGsfElectronCollection::const_iterator iElectron ;

  for (iElectron = gsfElectrons.begin();iElectron!= gsfElectrons.end();iElectron++) {
    electronCollection.push_back(&(*iElectron));
    if (iElectron->pt()>=MinElectronPT && fabs(iElectron->eta()) < MaxElectronEta) {
      hRECElecPT2->Fill(iElectron->pt());
      hRECElecEta2->Fill(iElectron->eta());
      hRECElecPhi2->Fill(iElectron->phi());
      nreco+=1;
    }
  }
  
  reco::ElectronIDAssociationCollection::const_iterator iAssoc;
  for (iAssoc = eleIDassociation.begin();iAssoc != eleIDassociation.end(); iAssoc++){
    if (iAssoc->val->cutBasedDecision() == true && iAssoc->key->pt()>=MinElectronPT && fabs(iAssoc->key->eta()) < MaxElectronEta ) {
      ngood +=1;
      hSelElecPT2->Fill(iAssoc->key->pt());
      hSelElecEta2->Fill(iAssoc->key->eta());
      hSelElecPhi2->Fill(iAssoc->key->phi());
    }
  }

  MatchMap* matchMap= new MatchMap;
  MatchMap::const_iterator iMatch;

  std::vector<const reco::Candidate*>* leptons = new std::vector<const reco::Candidate*>;
  std::vector<const reco::Candidate*>::const_iterator iLepton;
  mcMatchingTools.getMCLeptons(particleCollection,*leptons);

  mcMatchingTools.getMatched(&particleCollection,&electronCollection,*matchMap);

  for (iLepton = leptons->begin();iLepton!= leptons->end();iLepton++) {
    if ( (*iLepton)->pt() < MinElectronPT ||
	 fabs((*iLepton)->eta()) > MaxElectronEta ) continue;
    
    nmc+=1;
    hMCElecPT->Fill((*iLepton)->pt());
    hMCElecEta->Fill((*iLepton)->eta());
    hMCElecPhi->Fill((*iLepton)->phi());

    if (matchMap->find(*iLepton) != matchMap->end()){
      const reco::PixelMatchGsfElectron* electron = dynamic_cast<const reco::PixelMatchGsfElectron*>(matchMap->find(*iLepton)->second); 
      
      double productCharge = mcMatchingTools.getProductCharge(**iLepton,*electron);
      double dPbyP = mcMatchingTools.getDPbyP(**iLepton,*electron);
      double dEoverE= mcMatchingTools.getDEoverE(**iLepton,*electron);
      bool passdEoverE = false;
	      
      hRECElecPTAllCharge->Fill((*iLepton)->pt());
      hRECElecEtaAllCharge->Fill((*iLepton)->eta());
      hRECElecPhiAllCharge->Fill((*iLepton)->phi());
      
      if (productCharge == -1 ){
	hRECElecPTChargeMis->Fill((*iLepton)->pt());
	hRECElecEtaChargeMis->Fill((*iLepton)->eta());
	hRECElecPhiChargeMis->Fill((*iLepton)->phi());
      }
            
      if (productCharge == 1 ) {
	nmatchedreco+=1;

	hDPByPMatch->Fill(dPbyP);
	int EleClassification = electron->classification();
	
	if (EleClassification == 40) {
	  if (dEoverE<= maxdEoverECrack) passdEoverE = true;
	}
	else {
	  if (EleClassification < 100) {
	    if (dEoverE<= maxdEoverEBarrel) passdEoverE = true;
	  }
	  else {
	    if (dEoverE<= maxdEoverEEndcap) passdEoverE = true;
	  }
	} 
	
	hRECElecPT->Fill((*iLepton)->pt());
	hRECElecEta->Fill((*iLepton)->eta());
	hRECElecPhi->Fill((*iLepton)->phi());
	
	if (passdEoverE ==true)  {
	  hRECElecPTEne->Fill((*iLepton)->pt()); 
	  hRECElecEtaEne->Fill((*iLepton)->eta()); 
	  hRECElecPhiEne->Fill((*iLepton)->phi()); 
	}
	
	if  (electron->pt() >=  MinElectronPT &&  
	     fabs(electron->eta()) <= MaxElectronEta ) {
	  hRECElecPT1->Fill(electron->pt());
	  hRECElecEta1->Fill(electron->eta());
	  hRECElecPhi1->Fill(electron->phi());
	}

      }

      int index = int(electron-&(gsfElectrons.front()));
      reco::PixelMatchGsfElectronRef electronRef (gsfElectronsHandle,index);		        
      reco::ElectronIDAssociationCollection::const_iterator iAssoc_ = eleIDassociation.find(electronRef);
      const reco::ElectronIDRef electronID = iAssoc_->val;

      if (electronID->cutBasedDecision() == true){

	hSelElecPTAllCharge->Fill((*iLepton)->pt());
	hSelElecEtaAllCharge->Fill((*iLepton)->eta());
	hSelElecPhiAllCharge->Fill((*iLepton)->phi());
	
	if (productCharge == -1 ){
	  hSelElecPTChargeMis->Fill((*iLepton)->pt());
	  hSelElecEtaChargeMis->Fill((*iLepton)->eta());
	  hSelElecPhiChargeMis->Fill((*iLepton)->phi());
	}

	if (productCharge == 1) {
	  nmatchedgood+=1;
	  hSelElecPT->Fill((*iLepton)->pt());
	  hSelElecEta->Fill((*iLepton)->eta());
	  hSelElecPhi->Fill((*iLepton)->phi());
	  if (passdEoverE ==true) {
	    hSelElecPTEne->Fill((*iLepton)->pt()); 
	    hSelElecEtaEne->Fill((*iLepton)->eta()); 
	    hSelElecPhiEne->Fill((*iLepton)->phi()); 
	  }
	  if  (electron->pt() >=  MinElectronPT &&  
	       fabs(electron->eta()) <= MaxElectronEta) {
	    hSelElecPT1->Fill(electron->pt());
	    hSelElecEta1->Fill(electron->eta());
	    hSelElecPhi1->Fill(electron->phi());
	  }
	}
      }
    }
  }
  
  hMCNElec->Fill(nmc);
  hRECNElec->Fill(nreco);
  hSelNElec->Fill(ngood);  
  
  tmc+=nmc;
  treco+=nreco;
  tgood+=ngood;
  tmatchedreco+=nmatchedreco;
  tmatchedgood+=nmatchedgood;


  for (int binx = 0; binx <= hRECEffInclusive->GetNbinsX() + 1; binx++){    
    if ( double(nmatchedreco) >= hRECEffInclusive->GetBinLowEdge(binx) )  hRECEffInclusive->AddBinContent(binx,1);
  }

  for (int binx = 0; binx <= hSelEffInclusive->GetNbinsX() + 1; binx++){    
    if ( double(nmatchedgood) >= hSelEffInclusive->GetBinLowEdge(binx) )  hSelEffInclusive->AddBinContent(binx,1);
  }

  delete leptons;
  delete matchMap;

  nEvents+=1;
	        
}

// ------------ method called once each job just before starting event loop  ------------
void 
ElectronIDValAnalyzer::beginJob(const edm::EventSetup&)
{
  nEvents = 0; 
  efficiencyInf->Branch("DatasetName",&datasetName,"DatasetName/C");
  efficiencyInf->Branch("EleRecEfficiency",&EleRecEfficiency,"EleRecEfficiency/F");
  efficiencyInf->Branch("EleIdEfficiency",&EleIdEfficiency,"EleIdEfficiency/F");
  efficiencyInf->Branch("EleEfficiency",&EleEfficiency,"EleEfficiency/F");
  efficiencyInf->Branch("EleRecPurity",&EleRecPurity,"EleRecPurity/F");
  efficiencyInf->Branch("EleIdPurity",&EleIdPurity,"EleIdPurity/F");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
ElectronIDValAnalyzer::endJob() {

  EleRecEfficiency=double(tmatchedreco)/double(tmc);
  EleIdEfficiency=double(tmatchedgood)/double(tmatchedreco);
  EleEfficiency=double(tmatchedgood)/double(tmc);
  EleRecPurity=double(tmatchedreco)/double(treco); 
  EleIdPurity=double(tmatchedgood)/double(tgood); 

  rootFile_->cd();
  efficiencyInf->Fill();

  rootFile_->cd("/Reconstruction");
  gEffPT= new TGraphAsymmErrors(hRECElecPT,hMCElecPT);
  gEffPT->Write("EfficiencyReconstructionPTGraph");
  gEffEta= new TGraphAsymmErrors(hRECElecEta,hMCElecEta);
  gEffEta->Write("EfficiencyReconstructionEtaGraph");  
  gEffPhi= new TGraphAsymmErrors(hRECElecPhi,hMCElecPhi);
  gEffPhi->Write("EfficiencyReconstructionPhiGraph");
  rootFile_->cd("/Identification");
  gEffIdPT= new TGraphAsymmErrors(hSelElecPT,hRECElecPT);
  gEffIdPT->Write("EfficiencyIdentificationPTGraph");
  gEffIdEta= new TGraphAsymmErrors(hSelElecEta,hRECElecEta);
  gEffIdEta->Write("EfficiencyIdentificationEtaGraph");
  gEffIdPhi= new TGraphAsymmErrors(hSelElecPhi,hRECElecPhi);
  gEffIdPhi->Write("EfficiencyIdentificationPhiGraph");
  rootFile_->cd("/Rec&Id");
  gEffRecIdPT= new TGraphAsymmErrors(hSelElecPT,hMCElecPT);
  gEffRecIdPT->Write("EfficiencyRecIdentificationPTGraph");
  gEffRecIdEta= new TGraphAsymmErrors(hSelElecEta,hMCElecEta);
  gEffRecIdEta->Write("EfficiencyRecIdentificationEtaGraph");
  gEffRecIdPhi= new TGraphAsymmErrors(hSelElecPhi,hMCElecPhi);
  gEffRecIdPhi->Write("EfficiencyRecIdentificationPhiGraph");
  rootFile_->cd("/RecPurity");
  gPurRecPT= new TGraphAsymmErrors(hRECElecPT1,hRECElecPT2);
  gPurRecPT->Write("PurityRecPTGraph");
  gPurRecEta= new TGraphAsymmErrors(hRECElecEta1,hRECElecEta2);
  gPurRecEta->Write("PurityRecEtaGraph");
  gPurRecPhi= new TGraphAsymmErrors(hRECElecPhi1,hRECElecPhi2);
  gPurRecPhi->Write("PurityRecPhiGraph");
  rootFile_->cd("/IdPurity");
  gPurIdPT= new TGraphAsymmErrors(hSelElecPT1,hSelElecPT2);
  gPurIdPT->Write("PurityIdPTGraph");
  gPurIdEta= new TGraphAsymmErrors(hSelElecEta1,hSelElecEta2);
  gPurIdEta->Write("PurityIdEtaGraph");
  gPurIdPhi= new TGraphAsymmErrors(hSelElecPhi1,hSelElecPhi2);
  gPurIdPhi->Write("PurityIdPhiGraph");

  rootFile_->cd("/Energy");
  gEffPTEne= new TGraphAsymmErrors(hRECElecPTEne,hMCElecPT);
  gEffPTEne->Write("EfficiencyReconstructionPTEneGraph");
  gEffEtaEne= new TGraphAsymmErrors(hRECElecEtaEne,hMCElecEta);
  gEffEtaEne->Write("EfficiencyReconstructionEtaEneGraph");  
  gEffPhiEne= new TGraphAsymmErrors(hRECElecPhiEne,hMCElecPhi);
  gEffPhiEne->Write("EfficiencyReconstructionPhiEneGraph");
  gEffIdPTEne= new TGraphAsymmErrors(hSelElecPTEne,hRECElecPTEne);
  gEffIdPTEne->Write("EfficiencyIdentificationPTEneGraph");
  gEffIdEtaEne= new TGraphAsymmErrors(hSelElecEtaEne,hRECElecEtaEne);
  gEffIdEtaEne->Write("EfficiencyIdentificationEtaEneGraph");
  gEffIdPhiEne= new TGraphAsymmErrors(hSelElecPhiEne,hRECElecPhiEne);
  gEffIdPhiEne->Write("EfficiencyIdentificationPhiEneGraph");
  gEffRecIdPTEne= new TGraphAsymmErrors(hSelElecPTEne,hMCElecPT);
  gEffRecIdPTEne->Write("EfficiencyRecIdentificationPTEneGraph");
  gEffRecIdEtaEne= new TGraphAsymmErrors(hSelElecEtaEne,hMCElecEta);
  gEffRecIdEtaEne->Write("EfficiencyRecIdentificationEtaEneGraph");
  gEffRecIdPhiEne= new TGraphAsymmErrors(hSelElecPhiEne,hMCElecPhi);
  gEffRecIdPhi->Write("EfficiencyRecIdentificationPhiEneGraph");

  rootFile_->cd("/MisCharge");

  gMisChargeRecPT= new TGraphAsymmErrors(hRECElecPTChargeMis,hRECElecPTAllCharge);
  gMisChargeRecPT->Write("MisChargeRecPTGraph");
  gMisChargeRecEta= new TGraphAsymmErrors(hRECElecEtaChargeMis,hRECElecEtaAllCharge);
  gMisChargeRecEta->Write("MisChargeRecEtaGraph");
  gMisChargeRecPhi= new TGraphAsymmErrors(hRECElecPhiChargeMis,hRECElecPhiAllCharge);
  gMisChargeRecPhi->Write("MisChargeRecPhiGraph");
  gMisChargeIdPT= new TGraphAsymmErrors(hSelElecPTChargeMis,hSelElecPTAllCharge);
  gMisChargeIdPT->Write("MisChargeIdPTGraph");
  gMisChargeIdEta= new TGraphAsymmErrors(hSelElecEtaChargeMis,hSelElecEtaAllCharge);
  gMisChargeIdEta->Write("MisChargeIdEtaGraph");
  gMisChargeIdPhi= new TGraphAsymmErrors(hSelElecPhiChargeMis,hSelElecPhiAllCharge);
  gMisChargeIdPhi->Write("MisChargeIdPhiGraph");

  rootFile_->cd();  
 
  hRECEffInclusive->Scale(1.0/double(nEvents));
  hSelEffInclusive->Scale(1.0/double(nEvents));
  hMCNElec->Scale(1.0/double(nEvents));
  hRECNElec->Scale(1.0/double(nEvents));
  hSelNElec->Scale(1.0/double(nEvents));

//   hMCNElec->Write();
//   hRECNElec->Write();
//   hSelNElec->Write();
//   hEffRECEvent->Write();
//   hPurRECEvent->Write();
//   hEffSelEvent->Write();
//   hPurSelEvent->Write();
//   hRECEffInclusive->Write();
//   hSelEffInclusive->Write();
//   efficiencyInf->Write();
  rootFile_->Write();
  rootFile_->Close();

  delete gEffPT;
  delete gEffIdPT;
  delete gEffPTEne;
  delete gEffIdPTEne;
  delete gEffEta;
  delete gEffIdEta;
  delete gEffEtaEne;
  delete gEffIdEtaEne; 
  delete gEffPhi;
  delete gEffIdPhi;
  delete gEffPhiEne;
  delete gEffIdPhiEne;
  delete gEffRecIdPT;
  delete gEffRecIdEta;
  delete gEffRecIdPhi;
  delete gEffRecIdPTEne;
  delete gEffRecIdEtaEne;
  delete gEffRecIdPhiEne;
  delete gPurRecPT;
  delete gPurRecEta;
  delete gPurRecPhi;
  delete gPurIdPT;
  delete gPurIdEta;
  delete gPurIdPhi;
  delete gMisChargeRecPT;
  delete gMisChargeRecEta;
  delete gMisChargeRecPhi;
  delete gMisChargeIdPT;
  delete gMisChargeIdEta;
  delete gMisChargeIdPhi;


  edm::LogInfo("ElectronID")<<"----------- Validation Information----------";
  edm::LogInfo("ElectronID")<<"Efficiency of : Electron Reconstruction"<<EleRecEfficiency<<"Electron Identification"<<EleIdEfficiency
		<<"Electron Rec. and Id."<<EleEfficiency;
  edm::LogInfo("ElectronID")<<"Purity of : Electron Reconstruction"<<EleRecPurity<<"Electron Identification"<<EleIdPurity;

}

void ElectronIDValAnalyzer::loadCMSSWObjects(const edm::ParameterSet& iConfig)
{
  MCTruthCollection_ = iConfig.getParameter<edm::InputTag>("MCTruthCollection");
  ElectronIDAssocMap_ = iConfig.getParameter<edm::InputTag>("ElectronIDAssociationMap");
  RecoCollection_=iConfig.getParameter<edm::InputTag>("ElectronRecoCollection");

}

void ElectronIDValAnalyzer::loadHistoParameters(const edm::ParameterSet& iConfig)
{

  histEffPT_min_Pt_ = iConfig.getParameter<double>("hist_Eff_min_PT");
  histEffPT_max_Pt_ = iConfig.getParameter<double>("hist_Eff_max_PT");
  histEffPT_bins_Pt_ = iConfig.getParameter<int>("hist_Eff_bins_PT");

  histEffEta_min_Eta_ = iConfig.getParameter<double>("hist_Eff_min_Eta");
  histEffEta_max_Eta_ = iConfig.getParameter<double>("hist_Eff_max_Eta");
  histEffEta_bins_Eta_ = iConfig.getParameter<int>("hist_Eff_bins_Eta");

  histEffPhi_min_Phi_ = iConfig.getParameter<double>("hist_Eff_min_Phi");
  histEffPhi_max_Phi_ = iConfig.getParameter<double>("hist_Eff_max_Phi");
  histEffPhi_bins_Phi_ = iConfig.getParameter<int>("hist_Eff_bins_Phi");  

  histNElec_min_ = iConfig.getParameter<double>("histNElec_min");
  histNElec_max_ = iConfig.getParameter<double>("histNElec_max");
  histNElec_bins_ = iConfig.getParameter<int>("histNElec_bins");
  
  histEffEvent_min_ = iConfig.getParameter<double>("histEffEvent_min");
  histEffEvent_max_ = iConfig.getParameter<double>("histEffEvent_max");
  histEffEvent_bins_ = iConfig.getParameter<int>("histEffEvent_bins");

  histPurEvent_min_ = iConfig.getParameter<double>("histPurEvent_min");
  histPurEvent_max_ = iConfig.getParameter<double>("histPurEvent_max");
  histPurEvent_bins_ = iConfig.getParameter<int>("histPurEvent_bins");

  histEffInclusive_min_ = iConfig.getParameter<double>("histEffInclusive_min");
  histEffInclusive_max_ = iConfig.getParameter<double>("histEffInclusive_max");
  histEffInclusive_bins_ = iConfig.getParameter<int>("histEffInclusive_bins");

}


 //define this as a plug-in
//DEFINE_FWK_MODULE(ElectronIDValAnalyzer);
