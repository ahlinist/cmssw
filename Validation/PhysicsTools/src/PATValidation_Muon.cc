// -*- C++ -*-
//
// Package:    PATValidation
// Class:      PATValidation
// 
/**\class PATValidation PATValidation_Muon.cc PATValidation/PATValidation/src/PATValidation_Muon.cc

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

#include "DataFormats/PatCandidates/interface/Muon.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"   

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TH1.h"



//using namespace edm;
using namespace reco;
using namespace std; 


//
// class decleration
//

class PATValidation_Muon : public edm::EDAnalyzer {
   public:
      explicit PATValidation_Muon(const edm::ParameterSet&);
      ~PATValidation_Muon();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
       DQMStore* dbe;
      std::map<std::string, MonitorElement*> me;

      // ----------member data ---------------------------

edm::InputTag recoMuon_;
edm::InputTag patMuon_;


std::string outputFile_;

//TH1D *histo_Tagged_NumJets;
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
PATValidation_Muon::PATValidation_Muon(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

recoMuon_    = iConfig.getParameter<edm::InputTag>("recoMuon");
patMuon_     = iConfig.getParameter<edm::InputTag>("patMuon");


outputFile_ = iConfig.getUntrackedParameter<std::string>("OutputFile");


 if (outputFile_.size() > 0)
    edm::LogInfo("OutputInfo") << " PAT/RECO histograms will be saved to '" << outputFile_.c_str()<< "'";
    else edm::LogInfo("OutputInfo") << " ParticleFlow Task histograms will NOT be saved";
     
}

PATValidation_Muon::~PATValidation_Muon()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ------------
void
PATValidation_Muon::beginJob(const edm::EventSetup&)
{
                   
dbe = edm::Service<DQMStore>().operator->();
if(dbe){

      string path = "Benchmarks/";

      dbe->setCurrentFolder(path.c_str());

      dbe->setCurrentFolder("Benchmarks/muons/PAT");
      me["PatMuon_pt"]  = dbe->book1D("pt", "PatMuon", 50, 0, 200);
      me["PatMuon_eta"]  = dbe->book1D("eta", "PatMuon", 50, -6, 6);
      me["PatMuon_phi"]  = dbe->book1D("phi", "PatMuon", 50, -4, 4);
  
       
      dbe->setCurrentFolder("Benchmarks/muons/RECO");
      me["RecoMuon_pt"] = dbe->book1D("pt", "RecoMuon", 50, 0, 200);
      me["RecoMuon_eta"]  = dbe->book1D("eta", "RecoMuon", 50, -6, 6);
      me["RecoMuon_phi"]  = dbe->book1D("phi", "RecoMuon", 50, -4, 4);



  }
  return;
    
  
}


// ------------ method called to for each event  ------------
void
PATValidation_Muon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 //   using namespace edm;
   
   // Typedefs to use views

//====================================RECO MUON=====================================
    typedef edm::View<reco::Candidate> candidateCollection ;


     //get reco muon collection
     edm::Handle<candidateCollection> recomuon_hnd;
     iEvent.getByLabel(recoMuon_, recomuon_hnd);
     const candidateCollection & RECOMUON =*recomuon_hnd;
    // Loop over Reco Muons
     for (int i = 0; i != RECOMUON.size(); ++i) {
      me["RecoMuon_pt"]->Fill(RECOMUON[i].pt());
      me["RecoMuon_eta"]->Fill(RECOMUON[i].eta());
      me["RecoMuon_phi"]->Fill(RECOMUON[i].phi()); 
      cout << "reco pt is = " << RECOMUON[i].pt() << endl; 
       }


//=============================PAT MUON====================================


   //get pat muon collection
     edm::Handle <std::vector<pat::Muon> >  patmuon_hnd;
     iEvent.getByLabel(patMuon_, patmuon_hnd);
     std::vector<pat::Muon> const & PATMUON = *patmuon_hnd;
    // Loop over Pat Muons
     for (int i = 0; i != PATMUON.size(); ++i) {
     me["PatMuon_pt"]->Fill(PATMUON[i].pt());
     me["PatMuon_eta"]->Fill(PATMUON[i].eta());
     me["PatMuon_phi"]->Fill(PATMUON[i].phi());   
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

PATValidation_Muon::endJob() {

// Store the DAQ Histograms
  if (outputFile_.size() != 0)
    dbe->save(outputFile_);


}

//define this as a plug-in
DEFINE_FWK_MODULE(PATValidation_Muon);
