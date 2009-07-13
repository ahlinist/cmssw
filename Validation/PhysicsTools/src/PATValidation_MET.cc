// -*- C++ -*-
//
// Package:    PATValidation
// Class:      PATValidation
// 
/**\class PATValidation PATValidation_MET.cc PATValidation/PATValidation/src/PATValidation_MET.cc

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

#include "PATValidation_MET.h"


#include "TH1.h"



using namespace reco;
using namespace std; 

    int nRecoMET=0;   
    int nPatMET=0;



//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PATValidation_MET::PATValidation_MET(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

recoMET_     = iConfig.getParameter<edm::InputTag>("recoMET");
patMET_      = iConfig.getParameter<edm::InputTag>("patMET");
outputFile_ = iConfig.getUntrackedParameter<std::string>("OutputFile");
 _etThreshold = iConfig.getParameter<double>("etThreshold");


 if (outputFile_.size() > 0)
    edm::LogInfo("OutputInfo") << " ParticleFLow Task histograms will be saved to '" << outputFile_.c_str()<< "'";
    else edm::LogInfo("OutputInfo") << " ParticleFlow Task histograms will NOT be saved";
     
}

PATValidation_MET::~PATValidation_MET()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ------------
void
PATValidation_MET::beginJob(const edm::EventSetup&)
{
                   
dbe = edm::Service<DQMStore>().operator->();
if(dbe){

      string path = "Benchmarks/";

      dbe->setCurrentFolder(path.c_str());
 
      dbe->setCurrentFolder("Benchmarks/mets/MET/PAT");
      me["PatMet_pt"]  = dbe->book1D("pt", "PatMet", 50, 0, 200);
      me["PatMet_eta"]  = dbe->book1D("eta", "PatMet", 50, -6, 6);
      me["PatMet_phi"]  = dbe->book1D("phi", "PatMet", 50, -4, 4);
      patjetME = dbe->book1D("met", "metPAT", 4, 1, 5);
      patjetME->setBinLabel(2,"MET",1);
  
      pathNevents            = dbe->book1D("METTask_Nevents",   "METTask_Nevents"   ,1,0,1);
      pathMEx                = dbe->book1D("METTask_MEx",       "METTask_MEx"   ,500,-500,500);
      pathMEy                = dbe->book1D("METTask_MEy",       "METTask_MEy"   ,500,-500,500);
      pathEz                 = dbe->book1D("METTask_Ez",        "METTask_Ez"    ,500,-500,500);
      pathMETSig             = dbe->book1D("METTask_METSig",    "METTask_METSig",51,0,51);
      pathMET                = dbe->book1D("METTask_MET",       "METTask_MET"   ,500,0,1000);
      pathMETPhi             = dbe->book1D("METTask_METPhi",    "METTask_METPhi",80,-4,4);
      pathSumET              = dbe->book1D("METTask_SumET",     "METTask_SumET" ,1000,0,2000); 

      dbe->setCurrentFolder("Benchmarks/mets/MET/RECO");
      me["RecoMet_pt"] = dbe->book1D("pt", "RecoMet", 50, 0, 200);
      me["RecoMet_eta"]  = dbe->book1D("eta", "RecoMet", 50, -6, 6);
      me["RecoMet_phi"]  = dbe->book1D("phi", "RecoMet", 50, -4, 4);
      recojetME = dbe->book1D("met", "metReco", 4, 1, 5);
      recojetME->setBinLabel(2,"MET",1);

      recohNevents            = dbe->book1D("METTask_Nevents",   "METTask_Nevents"   ,1,0,1);
      recohMEx                = dbe->book1D("METTask_MEx",       "METTask_MEx"   ,500,-500,500);
      recohMEy                = dbe->book1D("METTask_MEy",       "METTask_MEy"   ,500,-500,500);
      recohEz                 = dbe->book1D("METTask_Ez",        "METTask_Ez"    ,500,-500,500);
      recohMETSig             = dbe->book1D("METTask_METSig",    "METTask_METSig",51,0,51);
      recohMET                = dbe->book1D("METTask_MET",       "METTask_MET"   ,500,0,1000);
      recohMETPhi             = dbe->book1D("METTask_METPhi",    "METTask_METPhi",80,-4,4);
      recohSumET              = dbe->book1D("METTask_SumET",     "METTask_SumET" ,1000,0,2000);




 

  }
  return;
    
  
}


// ------------ method called to for each event  ------------
void
PATValidation_MET::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 //   using namespace edm;
   
   // Typedefs to use views

//====================================RECO MET=====================================
    typedef edm::View<reco::MET> candidateCollection ;

    //get reco met collection
     edm::Handle<candidateCollection> recomet_hnd;
     iEvent.getByLabel(recoMET_, recomet_hnd);
     const candidateCollection & RECOMET =*recomet_hnd;
    // Loop over Reco Mets
     for (int i = 0; i != RECOMET.size(); ++i) {
      me["RecoMet_pt"]->Fill(RECOMET[i].pt());
	 nRecoMET++;
//     cout << "RecoMET pt is = " << RECOMET[i].pt() << endl;
	me["RecoMet_eta"]->Fill(RECOMET[i].eta());
	me["RecoMet_phi"]->Fill(RECOMET[i].phi());
//     cout << "RecoMET pt is = " << RECOMET[i].pt() << endl; 
       recojetME->Fill(2);
       //std::cout << "_etThreshold = " << _etThreshold << std::endl;
	
     if (RECOMET[i].pt() > _etThreshold){

  	  recohMEx->Fill(RECOMET[i].px());
	  recohMEy->Fill(RECOMET[i].py());
	  recohMET->Fill(RECOMET[i].pt());
	  recohMETPhi->Fill(RECOMET[i].phi());
	  recohSumET->Fill(RECOMET[i].sumEt());
	  recohMETSig->Fill(RECOMET[i].mEtSig());
	  recohEz->Fill(RECOMET[i].e_longitudinal());

  }


       }


//=============================PAT MET====================================

    //get pat met collection
     edm::Handle <std::vector<pat::MET> >  patmet_hnd;
     iEvent.getByLabel(patMET_, patmet_hnd);
     std::vector<pat::MET> const & PATMET = *patmet_hnd;
   // Loop over Pat Mets
     for (int i = 0; i != PATMET.size(); ++i) {
     me["PatMet_pt"]->Fill(PATMET[i].pt());
     nPatMET++;
  //      cout << "PatMET pt is " << PATMET[i].pt() << endl;
     me["PatMet_eta"]->Fill(PATMET[i].eta());
     me["PatMet_phi"]->Fill(PATMET[i].phi());

       patjetME->Fill(2);
       //std::cout << "_etThreshold = " << _etThreshold << std::endl;
          
 if (PATMET[i].pt() > _etThreshold){
  
          pathMEx->Fill(PATMET[i].px());
          pathMEy->Fill(PATMET[i].py());
          pathMET->Fill(PATMET[i].pt());
          pathMETPhi->Fill(PATMET[i].phi()); 
          pathSumET->Fill(PATMET[i].sumEt());
          pathMETSig->Fill(PATMET[i].mEtSig());
          pathEz->Fill(PATMET[i].e_longitudinal());
     
  }
     





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
PATValidation_MET::endJob() {

// Store the DAQ Histograms
  if (outputFile_.size() != 0)
    dbe->save(outputFile_);


}

//define this as a plug-in
DEFINE_FWK_MODULE(PATValidation_MET);
