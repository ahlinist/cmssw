// -*- C++ -*-
//
// Package:    PATValidation
// Class:      PATValidation
// 
/**\class PATValidation PATValidation_Jet.cc PATValidation/PATValidation/src/PATValidation_Jet.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Sudhir_Malik
//         Created:  Fri Mar 13 09:52:17 CDT 2009
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//my includes
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"   

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "PATValidation_Jet.h"
////////////
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

//I don't know which of these I actually need yet
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"

#include "RecoJets/JetAlgorithms/interface/JetMatchingTools.h"
#include <cmath>

//#include "TH1.h"

using namespace reco;
using namespace std; 


//
// class decleration
//
/*
class PATValidation_Jet : public edm::EDAnalyzer {
   public:
      explicit PATValidation_Jet(const edm::ParameterSet&);
      ~PATValidation_Jet();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
       DQMStore* dbe;
      std::map<std::string, MonitorElement*> me;

      // ----------member data ---------------------------
};
*/


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PATValidation_Jet::PATValidation_Jet(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

///recoJet_     = iConfig.getParameter<edm::InputTag>("recoJet");
///patJet_      = iConfig.getParameter<edm::InputTag>("patJet");

recoJet_     = iConfig.getParameter<edm::InputTag>("recoJet");
patJet_      = iConfig.getParameter<edm::InputTag>("patJet");

benchmarkLabel_ = iConfig.getParameter<std::string>("BenchmarkLabel"); 
//recoJetCorr_     =  iConfig.getParameter<bool> ("recoJetCorr");
//patJetCorr_      = iConfig.getParameter<edm::InputTag>("patJetCorr");
//recoJetCorrSource_  = iConfig.getParameter<edm::InputTag>("recoJetCorrSource");

outputFile_ = iConfig.getUntrackedParameter<std::string>("OutputFile");


 if (outputFile_.size() > 0)
    edm::LogInfo("OutputInfo") << " PAT/RECOhistograms will be saved to '" << outputFile_.c_str()<< "'";
    else edm::LogInfo("OutputInfo") << " PAT Validation histograms will NOT be saved";
     
}

PATValidation_Jet::~PATValidation_Jet()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ------------
void
PATValidation_Jet::beginJob(const edm::EventSetup&)
{
                   
dbe = edm::Service<DQMStore>().operator->();
if(dbe){

        string path = "Benchmarks/" + benchmarkLabel_ + "/";

/////////////////////Defining PAT histograms///////////////////////
    
   
  dbe->setCurrentFolder("Benchmarks/" + benchmarkLabel_ + "/"+ "RECO");

    rmE                = dbe->book1D("E", "E", 100, 0, 500);
    rmE_80             = dbe->book1D("E_80", "E_80", 100, 0, 4500);
    rmE_3000           = dbe->book1D("E_3000", "E_3000", 100, 0, 6000);
    rmP                = dbe->book1D("P", "P", 100, 0, 500);
    rmP_80             = dbe->book1D("P_80", "P_80", 100, 0, 4500);
    rmP_3000           = dbe->book1D("P_3000", "P_3000", 100, 0, 6000);
    rmPt               = dbe->book1D("Pt", "Pt", 100, 0, 50);
    rmPt_80            = dbe->book1D("Pt_80", "Pt_80", 100, 0, 140);
    rmPt_3000          = dbe->book1D("Pt_3000", "Pt_3000", 100, 0, 4000);
    rmMass             = dbe->book1D("Mass", "Mass", 100, 0, 25);
    rmMass_80          = dbe->book1D("Mass_80", "Mass_80", 100, 0, 120);
    rmMass_3000        = dbe->book1D("Mass_3000", "Mass_3000", 100, 0, 1500);
    rmConstituents     = dbe->book1D("Constituents", "# of Constituents", 100, 0, 100);
    rmConstituents_80  = dbe->book1D("Constituents_80", "# of Constituents_80", 40, 0, 40);
    //
    rmEta              = dbe->book1D("Eta", "Eta", 100, -5, 5);
    rmEtaFirst         = dbe->book1D("EtaFirst", "EtaFirst", 100, -5, 5);
    rmPhi              = dbe->book1D("Phi", "Phi", 70, -3.5, 3.5);
    rmPhiFirst         = dbe->book1D("PhiFirst", "PhiFirst", 70, -3.5, 3.5);
    rmEFirst           = dbe->book1D("EFirst", "EFirst", 100, 0, 1000);
    rmEFirst_80        = dbe->book1D("EFirst_80", "EFirst_80", 100, 0, 180);
    rmEFirst_3000      = dbe->book1D("EFirst_3000", "EFirst_3000", 100, 0, 4000);
    rmPtFirst          = dbe->book1D("PtFirst", "PtFirst", 100, 0, 500);
    //
    rmMjj              = dbe->book1D("Mjj", "Mjj", 100, 0, 2000);
    rmMjj_3000         = dbe->book1D("Mjj_3000", "Mjj_3000", 100, 0, 10000);
    rmDelEta           = dbe->book1D("DelEta", "DelEta", 100, -.5, .5); 
    rmDelPhi           = dbe->book1D("DelPhi", "DelPhi", 100, -.5, .5);
    rmDelPt            = dbe->book1D("DelPt", "DelPt", 100, -1, 1);

    rmMaxEInEmTowers   = dbe->book1D("MaxEInEmTowers", "MaxEInEmTowers", 100, 0, 100);
    rmMaxEInHadTowers  = dbe->book1D("MaxEInHadTowers", "MaxEInHadTowers", 100, 0, 100);
    rmHadEnergyInHO    = dbe->book1D("HadEnergyInHO", "HadEnergyInHO", 100, 0, 10);
    rmHadEnergyInHB    = dbe->book1D("HadEnergyInHB", "HadEnergyInHB", 100, 0, 50);
    rmHadEnergyInHF    = dbe->book1D("HadEnergyInHF", "HadEnergyInHF", 100, 0, 50);
    rmHadEnergyInHE    = dbe->book1D("HadEnergyInHE", "HadEnergyInHE", 100, 0, 100);
    
    rmHadEnergyInHO_80    = dbe->book1D("HadEnergyInHO_80", "HadEnergyInHO_80", 100, 0, 50);
    rmHadEnergyInHB_80    = dbe->book1D("HadEnergyInHB_80", "HadEnergyInHB_80", 100, 0, 200);
    rmHadEnergyInHE_80    = dbe->book1D("HadEnergyInHE_80", "HadEnergyInHE_80", 100, 0, 1000);
    rmHadEnergyInHO_3000  = dbe->book1D("HadEnergyInHO_3000", "HadEnergyInHO_3000", 100, 0, 500);
    rmHadEnergyInHB_3000  = dbe->book1D("HadEnergyInHB_3000", "HadEnergyInHB_3000", 100, 0, 3000);
    rmHadEnergyInHE_3000  = dbe->book1D("HadEnergyInHE_3000", "HadEnergyInHE_3000", 100, 0, 2000);
      
    rmEmEnergyInEB     = dbe->book1D("EmEnergyInEB", "EmEnergyInEB", 100, 0, 50);
    rmEmEnergyInEE     = dbe->book1D("EmEnergyInEE", "EmEnergyInEE", 100, 0, 50);
    rmEmEnergyInHF     = dbe->book1D("EmEnergyInHF", "EmEnergyInHF", 120, -20, 100);
    rmEmEnergyInEB_80  = dbe->book1D("EmEnergyInEB_80", "EmEnergyInEB_80", 100, 0, 200);
    rmEmEnergyInEE_80  = dbe->book1D("EmEnergyInEE_80", "EmEnergyInEE_80", 100, 0, 1000);
    rmEmEnergyInEB_3000= dbe->book1D("EmEnergyInEB_3000", "EmEnergyInEB_3000", 100, 0, 3000);
    rmEmEnergyInEE_3000= dbe->book1D("EmEnergyInEE_3000", "EmEnergyInEE_3000", 100, 0, 2000);
    rmEnergyFractionHadronic = dbe->book1D("EnergyFractionHadronic", "EnergyFractionHadronic", 120, -0.1, 1.1);
    rmEnergyFractionEm = dbe->book1D("EnergyFractionEm", "EnergyFractionEm", 120, -0.1, 1.1);
    rmHFTotal          = dbe->book1D("HFTotal", "HFTotal", 100, 0, 500);
    rmHFTotal_80       = dbe->book1D("HFTotal_80", "HFTotal_80", 100, 0, 3000);
    rmHFTotal_3000     = dbe->book1D("HFTotal_3000", "HFTotal_3000", 100, 0, 6000);
    rmHFLong           = dbe->book1D("HFLong", "HFLong", 100, 0, 500);  
    rmHFLong_80        = dbe->book1D("HFLong_80", "HFLong_80", 100, 0, 200);
    rmHFLong_3000      = dbe->book1D("HFLong_3000", "HFLong_3000", 100, 0, 1500);
    rmHFShort          = dbe->book1D("HFShort", "HFShort", 100, 0, 500);
    rmHFShort_80       = dbe->book1D("HFShort_80", "HFShort_80", 100, 0, 200);
    rmHFShort_3000     = dbe->book1D("HFShort_3000", "HFShort_3000", 100, 0, 1500);
    rmN90              = dbe->book1D("N90", "N90", 50, 0, 50);




/////////////////////Defining PAT histograms///////////////////////

   dbe->setCurrentFolder("Benchmarks/" + benchmarkLabel_ + "/"+ "PAT");

    mE                = dbe->book1D("E", "E", 100, 0, 500);
    mE_80             = dbe->book1D("E_80", "E_80", 100, 0, 4500);
    mE_3000           = dbe->book1D("E_3000", "E_3000", 100, 0, 6000);
    mP                = dbe->book1D("P", "P", 100, 0, 500);
    mP_80             = dbe->book1D("P_80", "P_80", 100, 0, 4500);
    mP_3000           = dbe->book1D("P_3000", "P_3000", 100, 0, 6000);
    mPt               = dbe->book1D("Pt", "Pt", 100, 0, 50);
    mPt_80            = dbe->book1D("Pt_80", "Pt_80", 100, 0, 140);
    mPt_3000          = dbe->book1D("Pt_3000", "Pt_3000", 100, 0, 4000);
    mMass             = dbe->book1D("Mass", "Mass", 100, 0, 25);
    mMass_80          = dbe->book1D("Mass_80", "Mass_80", 100, 0, 120);
    mMass_3000        = dbe->book1D("Mass_3000", "Mass_3000", 100, 0, 1500);
    mConstituents     = dbe->book1D("Constituents", "# of Constituents", 100, 0, 100);
    mConstituents_80  = dbe->book1D("Constituents_80", "# of Constituents_80", 40, 0, 40);
    //
    mEta	      = dbe->book1D("Eta", "Eta", 100, -5, 5);
    mEtaFirst         = dbe->book1D("EtaFirst", "EtaFirst", 100, -5, 5);
    mPhi              = dbe->book1D("Phi", "Phi", 70, -3.5, 3.5);
    mPhiFirst         = dbe->book1D("PhiFirst", "PhiFirst", 70, -3.5, 3.5);
    mEFirst           = dbe->book1D("EFirst", "EFirst", 100, 0, 1000);
    mEFirst_80        = dbe->book1D("EFirst_80", "EFirst_80", 100, 0, 180);
    mEFirst_3000      = dbe->book1D("EFirst_3000", "EFirst_3000", 100, 0, 4000);
    mPtFirst          = dbe->book1D("PtFirst", "PtFirst", 100, 0, 500);
    //
    mMjj              = dbe->book1D("Mjj", "Mjj", 100, 0, 2000);
    mMjj_3000         = dbe->book1D("Mjj_3000", "Mjj_3000", 100, 0, 10000);
    mDelEta           = dbe->book1D("DelEta", "DelEta", 100, -.5, .5);  
    mDelPhi           = dbe->book1D("DelPhi", "DelPhi", 100, -.5, .5);
    mDelPt            = dbe->book1D("DelPt", "DelPt", 100, -1, 1);
    //
    mMaxEInEmTowers   = dbe->book1D("MaxEInEmTowers", "MaxEInEmTowers", 100, 0, 100);
    mMaxEInHadTowers  = dbe->book1D("MaxEInHadTowers", "MaxEInHadTowers", 100, 0, 100);
    mHadEnergyInHO    = dbe->book1D("HadEnergyInHO", "HadEnergyInHO", 100, 0, 10);
    mHadEnergyInHB    = dbe->book1D("HadEnergyInHB", "HadEnergyInHB", 100, 0, 50);
    mHadEnergyInHF    = dbe->book1D("HadEnergyInHF", "HadEnergyInHF", 100, 0, 50);
    mHadEnergyInHE    = dbe->book1D("HadEnergyInHE", "HadEnergyInHE", 100, 0, 100);
    
    mHadEnergyInHO_80    = dbe->book1D("HadEnergyInHO_80", "HadEnergyInHO_80", 100, 0, 50);
    mHadEnergyInHB_80    = dbe->book1D("HadEnergyInHB_80", "HadEnergyInHB_80", 100, 0, 200);
    mHadEnergyInHE_80    = dbe->book1D("HadEnergyInHE_80", "HadEnergyInHE_80", 100, 0, 1000);
    mHadEnergyInHO_3000  = dbe->book1D("HadEnergyInHO_3000", "HadEnergyInHO_3000", 100, 0, 500);
    mHadEnergyInHB_3000  = dbe->book1D("HadEnergyInHB_3000", "HadEnergyInHB_3000", 100, 0, 3000);
    mHadEnergyInHE_3000  = dbe->book1D("HadEnergyInHE_3000", "HadEnergyInHE_3000", 100, 0, 2000);

    mEmEnergyInEB     = dbe->book1D("EmEnergyInEB", "EmEnergyInEB", 100, 0, 50);
    mEmEnergyInEE     = dbe->book1D("EmEnergyInEE", "EmEnergyInEE", 100, 0, 50);
    mEmEnergyInHF     = dbe->book1D("EmEnergyInHF", "EmEnergyInHF", 120, -20, 100);
    mEmEnergyInEB_80  = dbe->book1D("EmEnergyInEB_80", "EmEnergyInEB_80", 100, 0, 200);
    mEmEnergyInEE_80  = dbe->book1D("EmEnergyInEE_80", "EmEnergyInEE_80", 100, 0, 1000);
    mEmEnergyInEB_3000= dbe->book1D("EmEnergyInEB_3000", "EmEnergyInEB_3000", 100, 0, 3000);
    mEmEnergyInEE_3000= dbe->book1D("EmEnergyInEE_3000", "EmEnergyInEE_3000", 100, 0, 2000);
    mEnergyFractionHadronic = dbe->book1D("EnergyFractionHadronic", "EnergyFractionHadronic", 120, -0.1, 1.1);
    mEnergyFractionEm = dbe->book1D("EnergyFractionEm", "EnergyFractionEm", 120, -0.1, 1.1);
    mHFTotal          = dbe->book1D("HFTotal", "HFTotal", 100, 0, 500);
    mHFTotal_80       = dbe->book1D("HFTotal_80", "HFTotal_80", 100, 0, 3000);
    mHFTotal_3000     = dbe->book1D("HFTotal_3000", "HFTotal_3000", 100, 0, 6000);
    mHFLong           = dbe->book1D("HFLong", "HFLong", 100, 0, 500);
    mHFLong_80        = dbe->book1D("HFLong_80", "HFLong_80", 100, 0, 200);
    mHFLong_3000      = dbe->book1D("HFLong_3000", "HFLong_3000", 100, 0, 1500);
    mHFShort          = dbe->book1D("HFShort", "HFShort", 100, 0, 500);
    mHFShort_80       = dbe->book1D("HFShort_80", "HFShort_80", 100, 0, 200);
    mHFShort_3000     = dbe->book1D("HFShort_3000", "HFShort_3000", 100, 0, 1500);
    mN90              = dbe->book1D("N90", "N90", 50, 0, 50);





  }
  return;
    
  
}


// ------------ method called to for each event  ------------
void
PATValidation_Jet::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 //   using namespace edm;
   
   // Typedefs to use views
//====================================RECO JETS=====================================

//    typedef edm::View<reco::Candidate> candidateCollection ;
    

     //get reco jet collection
////     edm::Handle<candidateCollection> recojet_hnd;
//////     iEvent.getByLabel(recoJet_, recojet_hnd);
///     const candidateCollection & RECOJET =*recojet_hnd;
//      const candidateCollection & RECOJET =*recojet_hnd;

    math::XYZTLorentzVector rp4tmp[2];    
    typedef edm::View<reco::CaloJet> candidateCollection ;
    edm::Handle<candidateCollection> recojet_hnd;
    iEvent.getByLabel(recoJet_, recojet_hnd);

//////reverse pat reco///     edm::Handle<std::vector<pat::Jet> >  recojet_hnd;
//////reverse pat reco///     iEvent.getByLabel(recoJet_, recojet_hnd);
////reverse pat reco///     for (std::vector<pat::Jet>::const_iterator  recojet = recojet_hnd->begin(); recojet != recojet_hnd->end(); recojet++) {

     int rjetIndex = 0;
     int rnJet = 0;
     int rnJetF = 0;
     int rnJetC = 0;
 
    
      for (candidateCollection::const_iterator  rjet = recojet_hnd->begin(); rjet != recojet_hnd->end(); rjet++) {
//////  cout << "reco jet  is" << recoJet_ << "and reco Jet pt is = "<< rjet->pt() << endl;    
//     cout << "I AM HERE 1" << endl;  
     if (rmEta) rmEta->Fill(rjet->eta());   
//     cout << "I AM HERE 2" << endl;
    
    if (rjet->pt() > 10.) {
      if (fabs(rjet->eta()) > 1.3)
        rnJetF++;
      else
        rnJetC++;
    }
//    cout << "I AM HERE 3" << endl;
/*    if (rjet->pt() > 10.) {
      if (rmEtaFineBin) rmEtaFineBin->Fill (rjet->eta());   
      if (rmEtaFineBin1p) rmEtaFineBin1p->Fill(rjet->eta()); 
      if (rmEtaFineBin2p) rmEtaFineBin2p->Fill(rjet->eta());
      if (rmEtaFineBin3p) rmEtaFineBin3p->Fill(rjet->eta());
      if (rmEtaFineBin1m) rmEtaFineBin1m->Fill(rjet->eta());
      if (rmEtaFineBin2m) rmEtaFineBin2m->Fill(rjet->eta());
      if (rmEtaFineBin3m) rmEtaFineBin3m->Fill(rjet->eta());
      if (rmPhiFineBin) rmPhiFineBin->Fill (rjet->phi());
    }
*/
//    cout << "I AM HERE 4" << endl;
    if (rmPhi) rmPhi->Fill(rjet->phi());
    if (rmE) rmE->Fill (rjet->energy());
    if (rmE_80) rmE_80->Fill(rjet->energy());
    if (rmE_3000) rmE_3000->Fill(rjet->energy());
    if (rmP) rmP->Fill (rjet->p());
    if (rmP_80) rmP_80->Fill (rjet->p());
    if (rmP_3000) rmP_3000->Fill(rjet->p());
    if (rmPt) rmPt->Fill (rjet->pt());    
    if (rmPt_80) rmPt_80->Fill (rjet->pt());
    if (rmPt_3000) rmPt_3000->Fill(rjet->pt());
    if (rmMass) rmMass->Fill (rjet->mass());
    if (rmMass_80) rmMass_80->Fill(rjet->mass());
    if (rmMass_3000) rmMass_3000->Fill(rjet->mass());
    if (rmConstituents) rmConstituents->Fill(rjet->nConstituents());
    if (rmConstituents_80) rmConstituents_80->Fill(rjet->nConstituents());
    if (rjet == recojet_hnd->begin ()) { // first rjet
      if (rmEtaFirst) rmEtaFirst->Fill(rjet->eta());
      if (rmPhiFirst) rmPhiFirst->Fill(rjet->phi());
      if (rmEFirst) rmEFirst->Fill(rjet->energy());
      if (rmEFirst_80) rmEFirst_80->Fill(rjet->energy());   
      if (rmEFirst_3000) rmEFirst_3000->Fill(rjet->energy());
      if (rmPtFirst) rmPtFirst->Fill(rjet->pt());
    }
    if (rjetIndex == 0) {
      rnJet++;
      rp4tmp[0] = rjet->p4();
    }
    if (rjetIndex == 1) {
      rnJet++;
      rp4tmp[1] = rjet->p4();
  }
//    cout << "I AM HERE 5" << endl;

  if (rmMaxEInEmTowers) rmMaxEInEmTowers->Fill(rjet->maxEInEmTowers());
    if (rmMaxEInHadTowers) rmMaxEInHadTowers->Fill(rjet->maxEInHadTowers());
    if (rmHadEnergyInHO) rmHadEnergyInHO->Fill(rjet->hadEnergyInHO());
    if (rmHadEnergyInHO_80)   rmHadEnergyInHO_80->Fill(rjet->hadEnergyInHO());
    if (rmHadEnergyInHO_3000) rmHadEnergyInHO_3000->Fill(rjet->hadEnergyInHO());
    if (rmHadEnergyInHB) rmHadEnergyInHB->Fill(rjet->hadEnergyInHB());
    if (rmHadEnergyInHB_80)   rmHadEnergyInHB_80->Fill(rjet->hadEnergyInHB());
    if (rmHadEnergyInHB_3000) rmHadEnergyInHB_3000->Fill(rjet->hadEnergyInHB());
    if (rmHadEnergyInHF) rmHadEnergyInHF->Fill(rjet->hadEnergyInHF());
    if (rmHadEnergyInHE) rmHadEnergyInHE->Fill(rjet->hadEnergyInHE());
    if (rmHadEnergyInHE_80)   rmHadEnergyInHE_80->Fill(rjet->hadEnergyInHE());
    if (rmHadEnergyInHE_3000) rmHadEnergyInHE_3000->Fill(rjet->hadEnergyInHE());
    if (rmEmEnergyInEB) rmEmEnergyInEB->Fill(rjet->emEnergyInEB());
    if (rmEmEnergyInEB_80)   rmEmEnergyInEB_80->Fill(rjet->emEnergyInEB());
    if (rmEmEnergyInEB_3000) rmEmEnergyInEB_3000->Fill(rjet->emEnergyInEB());
    if (rmEmEnergyInEE) rmEmEnergyInEE->Fill (rjet->emEnergyInEE());
    if (rmEmEnergyInEE_80)   rmEmEnergyInEE_80->Fill(rjet->emEnergyInEE());
    if (rmEmEnergyInEE_3000) rmEmEnergyInEE_3000->Fill(rjet->emEnergyInEE());
    if (rmEmEnergyInHF) rmEmEnergyInHF->Fill(rjet->emEnergyInHF());
    if (rmEnergyFractionHadronic) rmEnergyFractionHadronic->Fill(rjet->energyFractionHadronic());
    if (rmEnergyFractionEm) rmEnergyFractionEm->Fill(rjet->emEnergyFraction());
      
    if (rmHFTotal)      rmHFTotal->Fill(rjet->hadEnergyInHF()+rjet->emEnergyInHF());
    if (rmHFTotal_80)   rmHFTotal_80->Fill(rjet->hadEnergyInHF()+rjet->emEnergyInHF());
    if (rmHFTotal_3000) rmHFTotal_3000->Fill(rjet->hadEnergyInHF()+rjet->emEnergyInHF());
    if (rmHFLong)       rmHFLong->Fill(rjet->hadEnergyInHF()*0.5+rjet->emEnergyInHF());
    if (rmHFLong_80)    rmHFLong_80->Fill(rjet->hadEnergyInHF()*0.5+rjet->emEnergyInHF());
    if (rmHFLong_3000)  rmHFLong_3000->Fill(rjet->hadEnergyInHF()*0.5+rjet->emEnergyInHF());
    if (rmHFShort)      rmHFShort->Fill(rjet->hadEnergyInHF()*0.5);
    if (rmHFShort_80)   rmHFShort_80->Fill(rjet->hadEnergyInHF()*0.5);   
    if (rmHFShort_3000) rmHFShort_3000->Fill(rjet->hadEnergyInHF()*0.5);
    
    if (rmN90) rmN90->Fill(rjet->n90());
 //   rmJetEnergyProfile->Fill(rjet->eta(), rjet->phi(), rjet->energy());
//    rmHadJetEnergyProfile->Fill(rjet->eta(), rjet->phi(), rjet->hadEnergyInHO()+rjet->hadEnergyInHB()+rjet->hadEnergyInHF()+rjet->hadEnergyInHE());
//    rmEMJetEnergyProfile->Fill(rjet->eta(), rjet->phi(), rjet->emEnergyInEB()+rjet->emEnergyInEE()+rjet->emEnergyInHF());
  }

//  if (rmNJetsEtaC) rmNJetsEtaC->Fill( nJetC );
//  if (rmNJetsEtaF) rmNJetsEtaF->Fill( nJetF );
    
  if (rnJet == 2) {
    if (rmMjj) rmMjj->Fill( (rp4tmp[0]+rp4tmp[1]).mass() );
    if (rmMjj_3000) rmMjj_3000->Fill( (rp4tmp[0]+rp4tmp[1]).mass() );
  }

     
  // Count Jets above Pt cut
  for (int ristep = 0; ristep < 100; ++ristep) {
    int     rnjet = 0;
    float rptStep = (ristep * (200./100.));
    
    
for (candidateCollection::const_iterator rcal = recojet_hnd->begin(); rcal != recojet_hnd->end(); ++ rcal ) {

      if ( rcal->pt() > rptStep ) rnjet++;
    }
//    rmNJets1->Fill( rptStep, rnjet );
  }
  
  for (int ristep = 0; ristep < 100; ++ristep) {
        int     rnjet = 0;
        float rptStep = (ristep * (4000./100.));

    
for (candidateCollection::const_iterator rcal = recojet_hnd->begin(); rcal != recojet_hnd->end(); ++ rcal ) {
    
                if ( rcal->pt() > rptStep ) rnjet++;
        }
//      rmNJets2->Fill( rptStep, rnjet );
  }
    
    


//=============================PAT JET====================================


  //***********************************
  //*** Get the Jet collection
  //***********************************


     math::XYZTLorentzVector p4tmp[2];
//
//     typedef edm::View<std::vector<pat::Jet> > patCollection ;
//      edm::Handle<patCollection>  patjet_hnd;
//     std::vector<pat::Jet> const & jets = *patjet_hnd;

	edm::Handle<std::vector<pat::Jet> >  patjet_hnd;
	iEvent.getByLabel(patJet_, patjet_hnd);

///////reverse pat reco///        typedef edm::View<reco::CaloJet> candidateCollection ;
///////reverse pat reco///        edm::Handle<candidateCollection> patjet_hnd;
///////reverse pat reco/// iEvent.getByLabel(patJet_, patjet_hnd);
////reverse pat reco///     for (candidateCollection::const_iterator  jet = patjet_hnd->begin(); jet != patjet_hnd->end(); jet++) {

     int jetIndex = 0;
     int nJet = 0;
     int nJetF = 0;
     int nJetC = 0;


 //    for (; jet != patjet_hnd->end(); jet++, jetIndex++) {
      for (std::vector<pat::Jet>::const_iterator  jet = patjet_hnd->begin(); jet != patjet_hnd->end(); jet++) {
/////     cout << "PAT jet  is" << patJet_ << "and PAT Jet pt is = "<< jet->pt() << endl;
//     cout << "I AM HERE 1" << endl;
     if (mEta) mEta->Fill(jet->eta());
//     cout << "I AM HERE 2" << endl;
    
    if (jet->pt() > 10.) {
      if (fabs(jet->eta()) > 1.3)
        nJetF++;
      else
        nJetC++;
    }
//    cout << "I AM HERE 3" << endl;
/*    if (jet->pt() > 10.) {
      if (mEtaFineBin) mEtaFineBin->Fill (jet->eta());
      if (mEtaFineBin1p) mEtaFineBin1p->Fill(jet->eta());
      if (mEtaFineBin2p) mEtaFineBin2p->Fill(jet->eta());
      if (mEtaFineBin3p) mEtaFineBin3p->Fill(jet->eta());
      if (mEtaFineBin1m) mEtaFineBin1m->Fill(jet->eta());
      if (mEtaFineBin2m) mEtaFineBin2m->Fill(jet->eta());
      if (mEtaFineBin3m) mEtaFineBin3m->Fill(jet->eta());
      if (mPhiFineBin) mPhiFineBin->Fill (jet->phi());
    }
*/
//    cout << "I AM HERE 4" << endl;
    if (mPhi) mPhi->Fill(jet->phi()); 
    if (mE) mE->Fill (jet->energy());
    if (mE_80) mE_80->Fill(jet->energy());
    if (mE_3000) mE_3000->Fill(jet->energy());
    if (mP) mP->Fill (jet->p());
    if (mP_80) mP_80->Fill (jet->p());
    if (mP_3000) mP_3000->Fill(jet->p());
    if (mPt) mPt->Fill (jet->pt());
    if (mPt_80) mPt_80->Fill (jet->pt());
    if (mPt_3000) mPt_3000->Fill(jet->pt());
    if (mMass) mMass->Fill (jet->mass());
    if (mMass_80) mMass_80->Fill(jet->mass());
    if (mMass_3000) mMass_3000->Fill(jet->mass());
    if (mConstituents) mConstituents->Fill(jet->nConstituents());
    if (mConstituents_80) mConstituents_80->Fill(jet->nConstituents());
    if (jet == patjet_hnd->begin ()) { // first jet
      if (mEtaFirst) mEtaFirst->Fill(jet->eta());
      if (mPhiFirst) mPhiFirst->Fill(jet->phi());
      if (mEFirst) mEFirst->Fill(jet->energy());
      if (mEFirst_80) mEFirst_80->Fill(jet->energy());
      if (mEFirst_3000) mEFirst_3000->Fill(jet->energy());
      if (mPtFirst) mPtFirst->Fill(jet->pt());
    }
    if (jetIndex == 0) {
      nJet++;
      p4tmp[0] = jet->p4();
    }
    if (jetIndex == 1) {
      nJet++;
      p4tmp[1] = jet->p4();
  }
//    cout << "I AM HERE 5" << endl;

    if (mMaxEInEmTowers) mMaxEInEmTowers->Fill(jet->maxEInEmTowers());
    if (mMaxEInHadTowers) mMaxEInHadTowers->Fill(jet->maxEInHadTowers());
    if (mHadEnergyInHO) mHadEnergyInHO->Fill(jet->hadEnergyInHO());
    if (mHadEnergyInHO_80)   mHadEnergyInHO_80->Fill(jet->hadEnergyInHO());
    if (mHadEnergyInHO_3000) mHadEnergyInHO_3000->Fill(jet->hadEnergyInHO());
    if (mHadEnergyInHB) mHadEnergyInHB->Fill(jet->hadEnergyInHB());
    if (mHadEnergyInHB_80)   mHadEnergyInHB_80->Fill(jet->hadEnergyInHB());
    if (mHadEnergyInHB_3000) mHadEnergyInHB_3000->Fill(jet->hadEnergyInHB());
    if (mHadEnergyInHF) mHadEnergyInHF->Fill(jet->hadEnergyInHF());
    if (mHadEnergyInHE) mHadEnergyInHE->Fill(jet->hadEnergyInHE());
    if (mHadEnergyInHE_80)   mHadEnergyInHE_80->Fill(jet->hadEnergyInHE());
    if (mHadEnergyInHE_3000) mHadEnergyInHE_3000->Fill(jet->hadEnergyInHE());
    if (mEmEnergyInEB) mEmEnergyInEB->Fill(jet->emEnergyInEB());
    if (mEmEnergyInEB_80)   mEmEnergyInEB_80->Fill(jet->emEnergyInEB());
    if (mEmEnergyInEB_3000) mEmEnergyInEB_3000->Fill(jet->emEnergyInEB());
    if (mEmEnergyInEE) mEmEnergyInEE->Fill (jet->emEnergyInEE());
    if (mEmEnergyInEE_80)   mEmEnergyInEE_80->Fill(jet->emEnergyInEE());
    if (mEmEnergyInEE_3000) mEmEnergyInEE_3000->Fill(jet->emEnergyInEE());
    if (mEmEnergyInHF) mEmEnergyInHF->Fill(jet->emEnergyInHF());
    if (mEnergyFractionHadronic) mEnergyFractionHadronic->Fill(jet->energyFractionHadronic());
    if (mEnergyFractionEm) mEnergyFractionEm->Fill(jet->emEnergyFraction());

    if (mHFTotal)      mHFTotal->Fill(jet->hadEnergyInHF()+jet->emEnergyInHF());
    if (mHFTotal_80)   mHFTotal_80->Fill(jet->hadEnergyInHF()+jet->emEnergyInHF());
    if (mHFTotal_3000) mHFTotal_3000->Fill(jet->hadEnergyInHF()+jet->emEnergyInHF());
    if (mHFLong)       mHFLong->Fill(jet->hadEnergyInHF()*0.5+jet->emEnergyInHF());
    if (mHFLong_80)    mHFLong_80->Fill(jet->hadEnergyInHF()*0.5+jet->emEnergyInHF());
    if (mHFLong_3000)  mHFLong_3000->Fill(jet->hadEnergyInHF()*0.5+jet->emEnergyInHF());
    if (mHFShort)      mHFShort->Fill(jet->hadEnergyInHF()*0.5);
    if (mHFShort_80)   mHFShort_80->Fill(jet->hadEnergyInHF()*0.5);
    if (mHFShort_3000) mHFShort_3000->Fill(jet->hadEnergyInHF()*0.5); 
    
    if (mN90) mN90->Fill(jet->n90());
 //   mJetEnergyProfile->Fill(jet->eta(), jet->phi(), jet->energy());
//    mHadJetEnergyProfile->Fill(jet->eta(), jet->phi(), jet->hadEnergyInHO()+jet->hadEnergyInHB()+jet->hadEnergyInHF()+jet->hadEnergyInHE());
//    mEMJetEnergyProfile->Fill(jet->eta(), jet->phi(), jet->emEnergyInEB()+jet->emEnergyInEE()+jet->emEnergyInHF());
  }
    
//  if (mNJetsEtaC) mNJetsEtaC->Fill( nJetC );
//  if (mNJetsEtaF) mNJetsEtaF->Fill( nJetF );
    
  if (nJet == 2) {
    if (mMjj) mMjj->Fill( (p4tmp[0]+p4tmp[1]).mass() );
    if (mMjj_3000) mMjj_3000->Fill( (p4tmp[0]+p4tmp[1]).mass() );
  }
    
  // Count Jets above Pt cut
  for (int istep = 0; istep < 100; ++istep) {
    int     njet = 0;
    float ptStep = (istep * (200./100.));
    
for ( std::vector<pat::Jet>::const_iterator cal = patjet_hnd->begin(); cal != patjet_hnd->end(); ++ cal ) {

///reverse pat reco////        for (candidateCollection::const_iterator cal = patjet_hnd->begin(); cal != patjet_hnd->end(); ++ cal ) {

      if ( cal->pt() > ptStep ) njet++;
    }
//    mNJets1->Fill( ptStep, njet );
  }
    
  for (int istep = 0; istep < 100; ++istep) {
    	int     njet = 0;
    	float ptStep = (istep * (4000./100.));
 for ( std::vector<pat::Jet>::const_iterator cal = patjet_hnd->begin(); cal != patjet_hnd->end(); ++ cal ) {

///reverse pat reco////    for (candidateCollection::const_iterator cal = patjet_hnd->begin(); cal != patjet_hnd->end(); ++ cal ) {
          
      		if ( cal->pt() > ptStep ) njet++;
    	}
//    	mNJets2->Fill( ptStep, njet );
  }
    


     

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just after ending the event loop  ------------
void 
PATValidation_Jet::endJob() {

// Store the DAQ Histograms
  if (outputFile_.size() != 0)
    dbe->save(outputFile_);


}

//define this as a plug-in
DEFINE_FWK_MODULE(PATValidation_Jet);
