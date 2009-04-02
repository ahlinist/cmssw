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

#include "TH1.h"



using namespace reco;
using namespace std; 


//
// class decleration
//

class PATValidation_MET : public edm::EDAnalyzer {
   public:
      explicit PATValidation_MET(const edm::ParameterSet&);
      ~PATValidation_MET();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
       DQMStore* dbe;
      std::map<std::string, MonitorElement*> me;

      // ----------member data ---------------------------

edm::InputTag recoMET_;
edm::InputTag patMET_;

std::string outputFile_;

};

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
 
      dbe->setCurrentFolder("Benchmarks/mets/PAT");
      me["PatMet_pt"]  = dbe->book1D("pt", "PatMet", 50, 0, 200);
      me["PatMet_eta"]  = dbe->book1D("eta", "PatMet", 50, -6, 6);
      me["PatMet_phi"]  = dbe->book1D("phi", "PatMet", 50, -4, 4);

      dbe->setCurrentFolder("Benchmarks/mets/RECO");
      me["RecoMet_pt"] = dbe->book1D("pt", "RecoMet", 50, 0, 200);
      me["RecoMet_eta"]  = dbe->book1D("eta", "RecoMet", 50, -6, 6);
      me["RecoMet_phi"]  = dbe->book1D("phi", "RecoMet", 50, -4, 4);

 

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
    typedef edm::View<reco::Candidate> candidateCollection ;

    //get reco met collection
     edm::Handle<candidateCollection> recomet_hnd;
     iEvent.getByLabel(recoMET_, recomet_hnd);
     const candidateCollection & RECOMET =*recomet_hnd;
    // Loop over Reco Mets
     for (int i = 0; i != RECOMET.size(); ++i) {
      me["RecoMet_pt"]->Fill(RECOMET[i].pt());
	me["RecoMet_eta"]->Fill(RECOMET[i].eta());
	me["RecoMet_phi"]->Fill(RECOMET[i].phi());
      cout << "reco pt is = " << RECOMET[i].pt() << endl; 
       }


//=============================PAT MET====================================

    //get pat met collection
     edm::Handle <std::vector<pat::MET> >  patmet_hnd;
     iEvent.getByLabel(patMET_, patmet_hnd);
     std::vector<pat::MET> const & PATMET = *patmet_hnd;
   // Loop over Pat Mets
     for (int i = 0; i != PATMET.size(); ++i) {
     me["PatMet_pt"]->Fill(PATMET[i].pt());
     me["PatMet_eta"]->Fill(PATMET[i].eta());
     me["PatMet_phi"]->Fill(PATMET[i].phi());
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
PATValidation_MET::endJob() {

// Store the DAQ Histograms
  if (outputFile_.size() != 0)
    dbe->save(outputFile_);


}

//define this as a plug-in
DEFINE_FWK_MODULE(PATValidation_MET);
