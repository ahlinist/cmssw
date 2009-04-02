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

#include "TH1.h"

using namespace reco;
using namespace std; 


//
// class decleration
//

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

edm::InputTag recoJet_;
edm::InputTag patJet_;

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
PATValidation_Jet::PATValidation_Jet(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

recoJet_     = iConfig.getParameter<edm::InputTag>("recoJet");
patJet_      = iConfig.getParameter<edm::InputTag>("patJet");

outputFile_ = iConfig.getUntrackedParameter<std::string>("OutputFile");


 if (outputFile_.size() > 0)
    edm::LogInfo("OutputInfo") << " PAT/RECOhistograms will be saved to '" << outputFile_.c_str()<< "'";
    else edm::LogInfo("OutputInfo") << " ParticleFlow Task histograms will NOT be saved";
     
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

      string path = "Benchmarks/";

      dbe->setCurrentFolder(path.c_str());

      dbe->setCurrentFolder("Benchmarks/jets/PAT");
      me["PatJet_pt"]  = dbe->book1D("pt", "PatJet", 50, 0, 200);  
      me["PatJet_eta"]  = dbe->book1D("eta", "PatJet", 50, -6, 6);
      me["PatJet_phi"]  = dbe->book1D("phi", "PatJet", 50, -4, 4);       

      dbe->setCurrentFolder("Benchmarks/jets/RECO");
      me["RecoJet_pt"] = dbe->book1D("pt", "RecoJet", 50, 0, 200);
      me["RecoJet_eta"]  = dbe->book1D("eta", "PatJet", 50, -6, 6);
      me["RecoJet_phi"]  = dbe->book1D("phi", "PatJet", 50, -4, 4);

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
    typedef edm::View<reco::Candidate> candidateCollection ;

   //get reco jet collection
     edm::Handle<candidateCollection> recojet_hnd;
     iEvent.getByLabel(recoJet_, recojet_hnd);
     const candidateCollection & RECOJET =*recojet_hnd;
    // Loop over Reco Jets
     for (int i = 0; i != RECOJET.size(); ++i) {
      me["RecoJet_pt"]->Fill(RECOJET[i].pt());
      me["RecoJet_eta"]->Fill(RECOJET[i].eta());
      me["RecoJet_phi"]->Fill(RECOJET[i].phi());
      cout << "reco pt is = " << RECOJET[i].pt() << endl;
       }
//=============================PAT JET====================================
     
    //get pat jet collection
     edm::Handle <std::vector<pat::Jet> >  patjet_hnd;
     iEvent.getByLabel(patJet_, patjet_hnd);
     std::vector<pat::Jet> const & PATJET = *patjet_hnd;
   // Loop over Pat Jets
     for (int i = 0; i != PATJET.size(); ++i) {
        me["PatJet_pt"]->Fill(PATJET[i].pt());
	me["PatJet_eta"]->Fill(PATJET[i].eta());
	me["PatJet_phi"]->Fill(PATJET[i].phi());

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
