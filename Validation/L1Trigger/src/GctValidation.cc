// -*- C++ -*-
//
// Package:    Validation/L1Trigger
// Class:      GctValidation
// 
/**\class GctValidation GctValidation.cc Validation/L1Trigger/src/GctValidation.cc

 Description: Make validation plots for Gct emulator

 Implementation:
     DQM module
*/
//
// Original Author:  Jim Brooke
//         Created:  Wed Aug 19 13:32:42 CEST 2009
// $Id$
// $Log$
//
//
//


// system include files
#include <memory>

// framework includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// DQM includes
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

// RCT/Gct includes
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"


//
// class decleration
//

class GctValidation : public edm::EDAnalyzer {
public:
  explicit GctValidation(const edm::ParameterSet&);
  ~GctValidation();
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  // DQM stuff
  DQMStore * dbe_;
  bool verbose_;
  std::string outputFile_;
  
  // Gct stuff
  edm::InputTag rctTag_;
  edm::InputTag gctTag_;
  
  MonitorElement* dummy_;

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
GctValidation::GctValidation(const edm::ParameterSet& iConfig) :
  verbose_(iConfig.getUntrackedParameter<bool>("verbose", false)),
  outputFile_(iConfig.getUntrackedParameter<std::string>("outputFile", "")),
  rctTag_(iConfig.getParameter<edm::InputTag>("rctTag")), 
  gctTag_(iConfig.getParameter<edm::InputTag>("gctTag"))
{

  if ( outputFile_.size() != 0 ) {
    edm::LogInfo("OutputInfo") << " GCT validation histograms will be saved to " << outputFile_.c_str();
  } else {
    edm::LogInfo("OutputInfo") << " GCT validation histograms will NOT be saved";
  }
  
  // get hold of back-end interface
  dbe_ = edm::Service<DQMStore>().operator->();           
  if ( dbe_ ) {
    if ( verbose_ ) { 
      dbe_->setVerbose(1); 
      dbe_->showDirStructure();
    } 
    else {
      dbe_->setVerbose(0); 
    }
  }

  dbe_->setCurrentFolder("L1Trigger/GCTVal");

}


GctValidation::~GctValidation()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
GctValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   dummy_->Fill(iEvent.run());

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// book histograms in beginJob
void GctValidation::beginJob() {

  dummy_ = dbe_->book1D("Dummy", "Test", 100, 0., 100.);

}

// 
void GctValidation::endJob() {

}

//define this as a plug-in
DEFINE_FWK_MODULE(GctValidation);
