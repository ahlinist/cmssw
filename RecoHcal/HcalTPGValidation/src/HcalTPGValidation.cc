// -*- C++ -*-
//
// Package:    HcalTPGValidation
// Class:      HcalTPGValidation
// 
/**\class HcalTPGValidation HcalTPGValidation.cc RecoHcal/HcalTPGValidation/src/HcalTPGValidation.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Efe Yazgan
//         Created:  Thu Jul 24 16:35:48 CEST 2008
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
//
// class decleration
//

class HcalTPGValidation : public edm::EDAnalyzer {
   public:
      explicit HcalTPGValidation(const edm::ParameterSet&);
      ~HcalTPGValidation();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

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
HcalTPGValidation::HcalTPGValidation(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


HcalTPGValidation::~HcalTPGValidation()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HcalTPGValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;



#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
HcalTPGValidation::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HcalTPGValidation::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HcalTPGValidation);
