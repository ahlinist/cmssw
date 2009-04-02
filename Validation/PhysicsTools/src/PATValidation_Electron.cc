// -*- C++ -*-
//
// Package:    PATValidation
// Class:      PATValidation
// 
/**\class PATValidation PATValidation_Electron.cc PATValidation/PATValidation/src/PATValidation_Electron.cc

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

#include "DataFormats/PatCandidates/interface/Electron.h"

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

class PATValidation_Electron : public edm::EDAnalyzer {
   public:
      explicit PATValidation_Electron(const edm::ParameterSet&);
      ~PATValidation_Electron();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
       DQMStore* dbe;
      std::map<std::string, MonitorElement*> me;

      // ----------member data ---------------------------

edm::InputTag recoElectron_;
edm::InputTag patElectron_;


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
PATValidation_Electron::PATValidation_Electron(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

recoElectron_= iConfig.getParameter<edm::InputTag>("recoElectron");
patElectron_ = iConfig.getParameter<edm::InputTag>("patElectron");

outputFile_ = iConfig.getUntrackedParameter<std::string>("OutputFile");


 if (outputFile_.size() > 0)
    edm::LogInfo("OutputInfo") << " ParticleFLow Task histograms will be saved to '" << outputFile_.c_str()<< "'";
    else edm::LogInfo("OutputInfo") << " ParticleFlow Task histograms will NOT be saved";
     
}

PATValidation_Electron::~PATValidation_Electron()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ------------
void
PATValidation_Electron::beginJob(const edm::EventSetup&)
{
                   
dbe = edm::Service<DQMStore>().operator->();
if(dbe){

      string path = "Benchmarks/";

      dbe->setCurrentFolder(path.c_str());

      dbe->setCurrentFolder("Benchmarks/electrons/PAT");
 
      me["PatElectron_pt"]  = dbe->book1D("pt", "PatElectron", 50, 0, 200);
      me["PatElectron_eta"]  = dbe->book1D("eta", "PatElectron", 50, -6, 6);
      me["PatElectron_phi"]  = dbe->book1D("phi", "PatElectron", 50, -4, 4);
  
      dbe->setCurrentFolder("Benchmarks/electrons/RECO");

      me["RecoElectron_pt"] = dbe->book1D("pt", "RecoElectron", 50, 0, 200);
      me["RecoElectron_eta"]  = dbe->book1D("eta", "RecoElectron", 50, -6, 6);
      me["RecoElectron_phi"]  = dbe->book1D("phi", "RecoElectron", 50, -4, 4);

 

  }
  return;
    
  
}


// ------------ method called to for each event  ------------
void
PATValidation_Electron::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 //   using namespace edm;
   
   // Typedefs to use views

//====================================RECO ELECTRON=====================================
    typedef edm::View<reco::Candidate> candidateCollection ;

     //get reco electron collection
     edm::Handle<candidateCollection> recoele_hnd;
     iEvent.getByLabel(recoElectron_, recoele_hnd);
     const candidateCollection & RECOELE =*recoele_hnd;
    // Loop over Reco Electrons
     for (int i = 0; i != RECOELE.size(); ++i) {
      me["RecoElectron_pt"]->Fill(RECOELE[i].pt());
      me["RecoElectron_eta"]->Fill(RECOELE[i].eta());
      me["RecoElectron_phi"]->Fill(RECOELE[i].phi());
      cout << "reco pt is = " << RECOELE[i].pt() << endl; 
       }

//====================================RECO ELECTRON=====================================

    //get pat electron collection
     edm::Handle <std::vector<pat::Electron> >  patele_hnd;
     iEvent.getByLabel(patElectron_, patele_hnd);
     std::vector<pat::Electron> const & PATELE = *patele_hnd;
   // Loop over Pat Electrons
     for (int i = 0; i != PATELE.size(); ++i) {
	me["PatElectron_pt"]->Fill(PATELE[i].pt());
	me["PatElectron_eta"]->Fill(PATELE[i].eta());
	me["PatElectron_phi"]->Fill(PATELE[i].phi()); 
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
PATValidation_Electron::endJob() {

// Store the DAQ Histograms
  if (outputFile_.size() != 0)
    dbe->save(outputFile_);


}

//define this as a plug-in
DEFINE_FWK_MODULE(PATValidation_Electron);
