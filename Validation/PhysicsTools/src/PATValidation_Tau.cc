// -*- C++ -*-
//
// Package:    PATValidation
// Class:      PATValidation
// 
/**\class PATValidation PATValidation_Tau.cc PATValidation/PATValidation/src/PATValidation_Tau.cc

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

#include "DataFormats/PatCandidates/interface/Tau.h"


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"   

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TH1.h"



using namespace reco;
using namespace std; 


    int nRecoTaus=0;
    int nPatTaus=0;


//
// class decleration
//

class PATValidation_Tau : public edm::EDAnalyzer {
   public:
      explicit PATValidation_Tau(const edm::ParameterSet&);
      ~PATValidation_Tau();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
       DQMStore* dbe;
      std::map<std::string, MonitorElement*> me;

      // ----------member data ---------------------------

edm::InputTag recoTau_;
edm::InputTag patTau_;


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
PATValidation_Tau::PATValidation_Tau(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

recoTau_     = iConfig.getParameter<edm::InputTag>("recoTau");
patTau_      = iConfig.getParameter<edm::InputTag>("patTau");


outputFile_ = iConfig.getUntrackedParameter<std::string>("OutputFile");


 if (outputFile_.size() > 0)
    edm::LogInfo("OutputInfo") << " ParticleFLow Task histograms will be saved to '" << outputFile_.c_str()<< "'";
    else edm::LogInfo("OutputInfo") << " ParticleFlow Task histograms will NOT be saved";
     
}

PATValidation_Tau::~PATValidation_Tau()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ------------
void
PATValidation_Tau::beginJob(const edm::EventSetup&)
{
                   
dbe = edm::Service<DQMStore>().operator->();
if(dbe){

      string path = "Benchmarks/";

      dbe->setCurrentFolder(path.c_str());

      dbe->setCurrentFolder("Benchmarks/taus/PAT");
      me["PatTau_pt"]  = dbe->book1D("pt", "PatTau", 50, 0, 200);
      me["PatTau_eta"]  = dbe->book1D("eta", "PatTau", 50, -6, 6);
      me["PatTau_phi"]  = dbe->book1D("phi", "PatTau", 50, -4, 4);
      me["PatTau_energy"]=dbe->book1D("energy","PatEnergy",50, 0.,450.); 

      dbe->setCurrentFolder("Benchmarks/taus/RECO");
      me["RecoTau_pt"] = dbe->book1D("pt", "RecoTau", 50, 0, 200);
      me["RecoTau_eta"]  = dbe->book1D("eta", "RecoTau", 50, -6, 6);
      me["RecoTau_phi"]  = dbe->book1D("phi", "RecoTau", 50, -4, 4);
      me["RecoTau_energy"]=dbe->book1D("energy","RecoEnergy",50, 0.,450.);
    

  }
  return;
    
  
}


// ------------ method called to for each event  ------------
void
PATValidation_Tau::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 //   using namespace edm;
   
   // Typedefs to use views

//====================================RECO TAU=====================================
    typedef edm::View<reco::Candidate> candidateCollection ;

   //get reco tau collection
     edm::Handle<candidateCollection> recotau_hnd;
     iEvent.getByLabel(recoTau_, recotau_hnd);
     const candidateCollection & RECOTAU =*recotau_hnd;
    // Loop over Reco Taus
     for (int i = 0; i != RECOTAU.size(); ++i) {
        me["RecoTau_pt"]->Fill(RECOTAU[i].pt());
        nRecoTaus++;
	me["RecoTau_eta"]->Fill(RECOTAU[i].eta());
	me["RecoTau_phi"]->Fill(RECOTAU[i].phi());
        me["RecoTau_energy"]->Fill(RECOTAU[i].energy());
       cout << "RECO tau pt is = " << RECOTAU[i].pt() << endl; 
       }


//=============================PAT TAU====================================


    //get pat tau collection
     edm::Handle <std::vector<pat::Tau> >  pattau_hnd;
     iEvent.getByLabel(patTau_, pattau_hnd);
     std::vector<pat::Tau> const & PATTAU = *pattau_hnd;
   // Loop over Pat Taus
     for (int i = 0; i != PATTAU.size(); ++i) {
        me["PatTau_pt"]->Fill(PATTAU[i].pt());
       nPatTaus++;
	me["PatTau_eta"]->Fill(PATTAU[i].eta());
	me["PatTau_phi"]->Fill(PATTAU[i].phi());
        me["PatTau_energy"]->Fill(PATTAU[i].energy());
    cout << "PAT tau pt is = " << PATTAU[i].pt() << endl;
    }
    
cout << "NUM OF PATTAUS = " << nPatTaus << endl;
cout << "NUM OF RECOTAUS = " << nRecoTaus << endl;

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
PATValidation_Tau::endJob() {

// Store the DAQ Histograms
  if (outputFile_.size() != 0)
    dbe->save(outputFile_);


}

//define this as a plug-in
DEFINE_FWK_MODULE(PATValidation_Tau);
