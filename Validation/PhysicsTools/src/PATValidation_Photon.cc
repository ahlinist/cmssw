// -*- C++ -*-
//
// Package:    PATValidation
// Class:      PATValidation
// 
/**\class PATValidation PATValidation_Photon.cc PATValidation/PATValidation/src/PATValidation_Photon.cc

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

#include "DataFormats/PatCandidates/interface/Photon.h"


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

class PATValidation_Photon : public edm::EDAnalyzer {
   public:
      explicit PATValidation_Photon(const edm::ParameterSet&);
      ~PATValidation_Photon();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
       DQMStore* dbe;
      std::map<std::string, MonitorElement*> me;

      // ----------member data ---------------------------

edm::InputTag recoPhoton_;
edm::InputTag patPhoton_;



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
PATValidation_Photon::PATValidation_Photon(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

recoPhoton_  = iConfig.getParameter<edm::InputTag>("recoPhoton");
patPhoton_   = iConfig.getParameter<edm::InputTag>("patPhoton");



outputFile_ = iConfig.getUntrackedParameter<std::string>("OutputFile");


 if (outputFile_.size() > 0)
    edm::LogInfo("OutputInfo") << " PAAT/RECO histograms will be saved to '" << outputFile_.c_str()<< "'";
    else edm::LogInfo("OutputInfo") << " ParticleFlow Task histograms will NOT be saved";
     
}

PATValidation_Photon::~PATValidation_Photon()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ------------
void
PATValidation_Photon::beginJob(const edm::EventSetup&)
{
                   
dbe = edm::Service<DQMStore>().operator->();
if(dbe){

      string path = "Benchmarks/";

      dbe->setCurrentFolder(path.c_str());

      dbe->setCurrentFolder("Benchmarks/photons/PAT");
	 me["PatPhoton_pt"]  = dbe->book1D("pt", "PatPhoton", 50, 0, 200);
         me["PatPhoton_eta"]  = dbe->book1D("eta", "PatPhoton", 50, -6, 6);
         me["PatPhoton_phi"]  = dbe->book1D("phi", "PatPhoton", 50, -4, 4);	

      dbe->setCurrentFolder("Benchmarks/photons/RECO");
         me["RecoPhoton_pt"] = dbe->book1D("pt", "RecoPhoton", 50, 0, 200);
         me["RecoPhoton_eta"]  = dbe->book1D("eta", "RecoPhoton", 50, -6, 6);
         me["RecoPhoton_phi"]  = dbe->book1D("phi", "RecoPhoton", 50, -4, 4);

  

  }
  return;
    
  
}


// ------------ method called to for each event  ------------
void
PATValidation_Photon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 //   using namespace edm;
   
   // Typedefs to use views

//====================================RECO PHOTON=====================================
    typedef edm::View<reco::Candidate> candidateCollection ;

   //get reco photon collection
     edm::Handle<candidateCollection> recopho_hnd;
     iEvent.getByLabel(recoPhoton_, recopho_hnd); 
     const candidateCollection & RECOPHO =*recopho_hnd; 
    // Loop over Reco Taus 
     for (int i = 0; i != RECOPHO.size(); ++i) { 
      me["RecoPhoton_pt"]->Fill(RECOPHO[i].pt());
	me["RecoPhoton_eta"]->Fill(RECOPHO[i].eta());
	me["RecoPhoton_phi"]->Fill(RECOPHO[i].phi());  
      cout << "reco pt is = " << RECOPHO[i].pt() << endl; 

     }

//=============================PAT PHOTON====================================

    //get pat photons collection
     edm::Handle <std::vector<pat::Photon> >  patpho_hnd;
     iEvent.getByLabel(patPhoton_, patpho_hnd);
     std::vector<pat::Photon> const & PATPHO = *patpho_hnd;
   // Loop over Pat Photons
     for (int i = 0; i != PATPHO.size(); ++i) {
     me["PatPhoton_pt"]->Fill(PATPHO[i].pt());
	me["PatPhoton_eta"]->Fill(PATPHO[i].eta());
	me["PatPhoton_phi"]->Fill(PATPHO[i].phi());
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
PATValidation_Photon::endJob() {

// Store the DAQ Histograms
  if (outputFile_.size() != 0)
    dbe->save(outputFile_);


}

//define this as a plug-in
DEFINE_FWK_MODULE(PATValidation_Photon);
