// -*- C++ -*-
//
// Package:    PATValidation
// Class:      PATValidation
// 
/**\class PATValidation PATValidation_CaloMET.cc PATValidation/PATValidation/src/PATValidation_CaloMET.cc

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

#include "DataFormats/PatCandidates/interface/MET.h"


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"   

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
//includes for Reco
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "PATValidation_CaloMET.h"


#include "TH1.h"



using namespace reco;
using namespace std; 

    int nRecoCaloMET=0;   
    int nPatCaloMET=0;



//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PATValidation_CaloMET::PATValidation_CaloMET(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

recoCaloMET_     = iConfig.getParameter<edm::InputTag>("recoCaloMET");
patCaloMET_      = iConfig.getParameter<edm::InputTag>("patCaloMET");
_etThreshold     = iConfig.getParameter<double>("etThreshold");
_allhist         = iConfig.getParameter<bool>("allHist");


outputFile_ = iConfig.getUntrackedParameter<std::string>("OutputFile");

     
}

PATValidation_CaloMET::~PATValidation_CaloMET()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ------------
void
PATValidation_CaloMET::beginJob(const edm::EventSetup&)
{
                   
dbe = edm::Service<DQMStore>().operator->();
if(dbe){

      string path = "Benchmarks/";

      dbe->setCurrentFolder(path.c_str());
 
      dbe->setCurrentFolder("Benchmarks/mets/CaloMET/PAT/");
      me["PatMet_pt"]  = dbe->book1D("pt", "PatMet", 50, 0, 200);
      me["PatMet_eta"]  = dbe->book1D("eta", "PatMet", 50, -6, 6);
      me["PatMet_phi"]  = dbe->book1D("phi", "PatMet", 50, -4, 4);

      patjetME                   = dbe->book1D("met", "metPAT", 4, 1, 5);
      pathNevents                = dbe->book1D("METTask_Nevents",   "METTask_Nevents"   ,1,0,1);
      pathCaloMEx                = dbe->book1D("METTask_CaloMEx",   "METTask_CaloMEx"   ,500,-500,500);
      pathCaloMEy                = dbe->book1D("METTask_CaloMEy",   "METTask_CaloMEy"   ,500,-500,500);
      pathCaloEz                 = dbe->book1D("METTask_CaloEz",    "METTask_CaloEz"    ,500,-500,500);
      pathCaloMETSig             = dbe->book1D("METTask_CaloMETSig","METTask_CaloMETSig",51,0,51);
      pathCaloMET                = dbe->book1D("METTask_CaloMET",   "METTask_CaloMET"   ,500,0,1000);
      pathCaloMETPhi             = dbe->book1D("METTask_CaloMETPhi","METTask_CaloMETPhi",80,-4,4);
      pathCaloSumET              = dbe->book1D("METTask_CaloSumET", "METTask_CaloSumET" ,1000,0,2000);
	
      if (_allhist){
	  pathCaloMExLS              = dbe->book2D("METTask_CaloMEx_LS","METTask_CaloMEx_LS",200,-200,200,500,0.,500.);
	  pathCaloMEyLS              = dbe->book2D("METTask_CaloMEy_LS","METTask_CaloMEy_LS",200,-200,200,500,0.,500.);

	  patCaloMaxEtInEmTowers    = dbe->book1D("METTask_CaloMaxEtInEmTowers",   "METTask_CaloMaxEtInEmTowers"   ,1000,0,2000);
	  pathCaloMaxEtInHadTowers   = dbe->book1D("METTask_CaloMaxEtInHadTowers",  "METTask_CaloMaxEtInHadTowers"  ,1000,0,2000);
	  pathCaloEtFractionHadronic = dbe->book1D("METTask_CaloEtFractionHadronic","METTask_CaloEtFractionHadronic",100,0,1);
	  pathCaloEmEtFraction       = dbe->book1D("METTask_CaloEmEtFraction",      "METTask_CaloEmEtFraction"      ,100,0,1);

	  pathCaloHadEtInHB          = dbe->book1D("METTask_CaloHadEtInHB","METTask_CaloHadEtInHB",1000,0,2000);
	  pathCaloHadEtInHO          = dbe->book1D("METTask_CaloHadEtInHO","METTask_CaloHadEtInHO",1000,0,2000);
	  pathCaloHadEtInHE          = dbe->book1D("METTask_CaloHadEtInHE","METTask_CaloHadEtInHE",1000,0,2000);
	  pathCaloHadEtInHF          = dbe->book1D("METTask_CaloHadEtInHF","METTask_CaloHadEtInHF",1000,0,2000);
	  pathCaloEmEtInHF           = dbe->book1D("METTask_CaloEmEtInHF" ,"METTask_CaloEmEtInHF" ,1000,0,2000);
	  pathCaloEmEtInEE           = dbe->book1D("METTask_CaloEmEtInEE" ,"METTask_CaloEmEtInEE" ,1000,0,2000);
	  pathCaloEmEtInEB           = dbe->book1D("METTask_CaloEmEtInEB" ,"METTask_CaloEmEtInEB" ,1000,0,2000);
	  }




      dbe->setCurrentFolder("Benchmarks/mets/CaloMET/RECO");
      me["RecoMet_pt"] = dbe->book1D("pt", "RecoMet", 50, 0, 200);
      me["RecoMet_eta"]  = dbe->book1D("eta", "RecoMet", 50, -6, 6);
      me["RecoMet_phi"]  = dbe->book1D("phi", "RecoMet", 50, -4, 4);

      recojetME                   = dbe->book1D("met", "metReco", 4, 1, 5);
      recohNevents                = dbe->book1D("METTask_Nevents",   "METTask_Nevents"   ,1,0,1);
      recohCaloMEx                = dbe->book1D("METTask_CaloMEx",   "METTask_CaloMEx"   ,500,-500,500);
      recohCaloMEy                = dbe->book1D("METTask_CaloMEy",   "METTask_CaloMEy"   ,500,-500,500);
      recohCaloEz                 = dbe->book1D("METTask_CaloEz",    "METTask_CaloEz"    ,500,-500,500);
      recohCaloMETSig             = dbe->book1D("METTask_CaloMETSig","METTask_CaloMETSig",51,0,51);
      recohCaloMET                = dbe->book1D("METTask_CaloMET",   "METTask_CaloMET"   ,500,0,1000);
      recohCaloMETPhi             = dbe->book1D("METTask_CaloMETPhi","METTask_CaloMETPhi",80,-4,4);
      recohCaloSumET              = dbe->book1D("METTask_CaloSumET", "METTask_CaloSumET" ,1000,0,2000);
 
       if (_allhist){
          recohCaloMExLS              = dbe->book2D("METTask_CaloMEx_LS","METTask_CaloMEx_LS",200,-200,200,500,0.,500.);
          recohCaloMEyLS              = dbe->book2D("METTask_CaloMEy_LS","METTask_CaloMEy_LS",200,-200,200,500,0.,500.);
      
          recoCaloMaxEtInEmTowers    = dbe->book1D("METTask_CaloMaxEtInEmTowers",   "METTask_CaloMaxEtInEmTowers"   ,1000,0,2000);
          recohCaloMaxEtInHadTowers   = dbe->book1D("METTask_CaloMaxEtInHadTowers",  "METTask_CaloMaxEtInHadTowers"  ,1000,0,2000);
          recohCaloEtFractionHadronic = dbe->book1D("METTask_CaloEtFractionHadronic","METTask_CaloEtFractionHadronic",100,0,1);
          recohCaloEmEtFraction       = dbe->book1D("METTask_CaloEmEtFraction",      "METTask_CaloEmEtFraction"      ,100,0,1);
      
          recohCaloHadEtInHB          = dbe->book1D("METTask_CaloHadEtInHB","METTask_CaloHadEtInHB",1000,0,2000);
          recohCaloHadEtInHO          = dbe->book1D("METTask_CaloHadEtInHO","METTask_CaloHadEtInHO",1000,0,2000);
          recohCaloHadEtInHE          = dbe->book1D("METTask_CaloHadEtInHE","METTask_CaloHadEtInHE",1000,0,2000);
          recohCaloHadEtInHF          = dbe->book1D("METTask_CaloHadEtInHF","METTask_CaloHadEtInHF",1000,0,2000);
          recohCaloEmEtInHF           = dbe->book1D("METTask_CaloEmEtInHF" ,"METTask_CaloEmEtInHF" ,1000,0,2000);
          recohCaloEmEtInEE           = dbe->book1D("METTask_CaloEmEtInEE" ,"METTask_CaloEmEtInEE" ,1000,0,2000);
          recohCaloEmEtInEB           = dbe->book1D("METTask_CaloEmEtInEB" ,"METTask_CaloEmEtInEB" ,1000,0,2000);
          }



  }
  return;
    
  
}


// ------------ method called to for each event  ------------
void
PATValidation_CaloMET::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 //   using namespace edm;
   
   // Typedefs to use views

//====================================RECO MET=====================================

//    typedef edm::View<reco::Candidate> candidateCollection ;
      typedef edm::View<reco::CaloMET> candidateCollection ;

    //get reco met collection
     edm::Handle<candidateCollection> recomet_hnd;
     iEvent.getByLabel(recoCaloMET_, recomet_hnd);
     const candidateCollection & RECOMET =*recomet_hnd;
    // Loop over Reco Mets
     for (int i = 0; i != RECOMET.size(); ++i) {
      me["RecoMet_pt"]->Fill(RECOMET[i].pt());
	 nRecoCaloMET++;
//     cout << "RecoMET pt is = " << RECOMET[i].pt() << endl;
	me["RecoMet_eta"]->Fill(RECOMET[i].eta());
	me["RecoMet_phi"]->Fill(RECOMET[i].phi());
//     cout << "RecoMET pt is = " << RECOMET[i].pt() << endl; 

       recojetME->Fill(1);
       //std::cout << "_etThreshold = " << _etThreshold << std::endl;
       if (RECOMET[i].pt() > _etThreshold){
       recohCaloMEx->Fill(RECOMET[i].px());
       recohCaloMEy->Fill(RECOMET[i].py());
       recohCaloMET->Fill(RECOMET[i].pt());
       recohCaloMETPhi->Fill(RECOMET[i].phi());
       recohCaloSumET->Fill(RECOMET[i].sumEt());
       recohCaloMETSig->Fill(RECOMET[i].mEtSig());
       recohCaloEz->Fill(RECOMET[i].e_longitudinal());
       } //end of _etthreshold
 
 if (_allhist){
     recohCaloMExLS->Fill(RECOMET[i].px(),iEvent.luminosityBlock());
     recohCaloMEyLS->Fill(RECOMET[i].py(),iEvent.luminosityBlock());
     
     recohCaloEtFractionHadronic->Fill(RECOMET[i].etFractionHadronic());  
     recohCaloEmEtFraction->Fill(RECOMET[i].emEtFraction());  
     
     recoCaloMaxEtInEmTowers->Fill(RECOMET[i].maxEtInEmTowers());  
     recohCaloMaxEtInHadTowers->Fill(RECOMET[i].maxEtInHadTowers());  
     
     recohCaloHadEtInHB->Fill(RECOMET[i].hadEtInHB());  
     recohCaloHadEtInHO->Fill(RECOMET[i].hadEtInHO());  
     recohCaloHadEtInHE->Fill(RECOMET[i].hadEtInHE());  
     recohCaloHadEtInHF->Fill(RECOMET[i].hadEtInHF());  
     recohCaloEmEtInEB->Fill(RECOMET[i].emEtInEB());  
     recohCaloEmEtInEE->Fill(RECOMET[i].emEtInEE());  
     recohCaloEmEtInHF->Fill(RECOMET[i].emEtInHF());  
  } // _allhist
 

       }


//=============================PAT MET====================================

    //get pat met collection
     edm::Handle <std::vector<pat::MET> >  patmet_hnd;
     iEvent.getByLabel(patCaloMET_, patmet_hnd);
     std::vector<pat::MET> const & PATMET = *patmet_hnd;
   // Loop over Pat Mets
     for (int i = 0; i != PATMET.size(); ++i) {
     me["PatMet_pt"]->Fill(PATMET[i].pt());
     nPatCaloMET++;
  //      cout << "PatMET pt is " << PATMET[i].pt() << endl;
     me["PatMet_eta"]->Fill(PATMET[i].eta());
     me["PatMet_phi"]->Fill(PATMET[i].phi());


       patjetME->Fill(1);
       if (PATMET[i].pt() > _etThreshold){
       pathCaloMEx->Fill(PATMET[i].px());
       pathCaloMEy->Fill(PATMET[i].py());
       pathCaloMET->Fill(PATMET[i].pt());
       pathCaloMETPhi->Fill(PATMET[i].phi());
       pathCaloSumET->Fill(PATMET[i].sumEt());
       pathCaloMETSig->Fill(PATMET[i].mEtSig());
       pathCaloEz->Fill(PATMET[i].e_longitudinal());
      }//end of _etthreshold

     if (_allhist){
     pathCaloMExLS->Fill(PATMET[i].px(),iEvent.luminosityBlock());
     pathCaloMEyLS->Fill(PATMET[i].py(),iEvent.luminosityBlock());

     pathCaloEtFractionHadronic->Fill(PATMET[i].etFractionHadronic());
     pathCaloEmEtFraction->Fill(PATMET[i].emEtFraction());

     patCaloMaxEtInEmTowers->Fill(PATMET[i].maxEtInEmTowers());
     pathCaloMaxEtInHadTowers->Fill(PATMET[i].maxEtInHadTowers());

     pathCaloHadEtInHB->Fill(PATMET[i].hadEtInHB());
     pathCaloHadEtInHO->Fill(PATMET[i].hadEtInHO());
     pathCaloHadEtInHE->Fill(PATMET[i].hadEtInHE());
     pathCaloHadEtInHF->Fill(PATMET[i].hadEtInHF());
     pathCaloEmEtInEB->Fill(PATMET[i].emEtInEB());
     pathCaloEmEtInEE->Fill(PATMET[i].emEtInEE());
     pathCaloEmEtInHF->Fill(PATMET[i].emEtInHF());
  } // _allhist


    }

//cout << "NUM OF PATMET = " << nPatMET << endl;
//cout << "NUM OF RECOMET = " << nRecoMET << endl;
	
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
PATValidation_CaloMET::endJob() {

// Store the DAQ Histograms
  if (outputFile_.size() != 0)
    dbe->save(outputFile_);


}

//define this as a plug-in
DEFINE_FWK_MODULE(PATValidation_CaloMET);
