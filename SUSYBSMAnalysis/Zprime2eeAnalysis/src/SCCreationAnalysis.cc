// -*- C++ -*-
//
// Package:    Analysis
// Class:      Analysis
// 
/**\class Analysis Analysis.cc TagProbeAnalysis/Analysis/src/Analysis.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  James Jackson (2004)
//         Created:  Mon Jun 25 12:08:54 BST 2007
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

#include "SUSYBSMAnalysis/Zprime2eeAnalysis/interface/SCCreationEfficiency.h"

//
// class decleration
//

class SCCreationAnalysis : public edm::EDAnalyzer {
   public:
      explicit SCCreationAnalysis(const edm::ParameterSet&);
      ~SCCreationAnalysis();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      SCCreationEfficiency scCreationEff_;

      // ----------member data ---------------------------
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
SCCreationAnalysis::SCCreationAnalysis(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   scCreationEff_.Initialise(iConfig);
}


SCCreationAnalysis::~SCCreationAnalysis()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
SCCreationAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   scCreationEff_.Add(iEvent, iSetup);
}


// ------------ method called once each job just before starting event loop  ------------
void 
SCCreationAnalysis::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SCCreationAnalysis::endJob() {
   scCreationEff_.FinishHistograms();
}

//define this as a plug-in
DEFINE_FWK_MODULE(SCCreationAnalysis);
