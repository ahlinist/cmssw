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

#include "SUSYBSMAnalysis/Zprime2eeAnalysis/interface/GsfEleCreationEfficiency.h"

//
// class decleration
//

class GsfEleCreationAnalysis : public edm::EDAnalyzer {
   public:
      explicit GsfEleCreationAnalysis(const edm::ParameterSet&);
      ~GsfEleCreationAnalysis();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      GsfEleCreationEfficiency gsfEleCreationEff_;

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
GsfEleCreationAnalysis::GsfEleCreationAnalysis(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   //std::cout << "initialising" << std::endl;
   gsfEleCreationEff_.Initialise(iConfig);
   //std::cout << "done initialising" << std::endl;
}


GsfEleCreationAnalysis::~GsfEleCreationAnalysis()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
GsfEleCreationAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   //std::cout << "adding an event" << std::endl;
   gsfEleCreationEff_.Add(iEvent, iSetup);
}


// ------------ method called once each job just before starting event loop  ------------
void 
GsfEleCreationAnalysis::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GsfEleCreationAnalysis::endJob() {
   gsfEleCreationEff_.FinishHistograms();
}

//define this as a plug-in
DEFINE_FWK_MODULE(GsfEleCreationAnalysis);
