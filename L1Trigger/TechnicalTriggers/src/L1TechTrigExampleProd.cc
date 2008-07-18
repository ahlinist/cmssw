// -*- C++ -*-
//
// Package:    L1TechTrigExampleProd
// Class:      L1TechTrigExampleProd
// 
/**\class L1TechTrigExampleProd L1TechTrigExampleProd.cc L1Trigger/L1TechTrigExampleProd/src/L1TechTrigExampleProd.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Jim Brooke
//         Created:  Fri Jul 18 14:58:27 CEST 2008
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTrigger.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTriggerRecord.h"

//
// class decleration
//

class L1TechTrigExampleProd : public edm::EDProducer {
public:
  explicit L1TechTrigExampleProd(const edm::ParameterSet&);
  ~L1TechTrigExampleProd();
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  // ----------member data ---------------------------
  unsigned ttBit_;
  unsigned freq_;

  unsigned nEvt_;
  
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
L1TechTrigExampleProd::L1TechTrigExampleProd(const edm::ParameterSet& ps) :
  ttBits_(ps.getParameter< std::vector<unsigned> >("bitNumbers")),
  freqs_(ps.getParameter< std::vector<double> >("frequencies")),
{
  produces<L1GtTechnicalTriggerRecord>();
  
}


L1TechTrigExampleProd::~L1TechTrigExampleProd()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
L1TechTrigExampleProd::produce(edm::Event& e, const edm::EventSetup& es)
{
   using namespace edm;

   // create my technical triggers
   std::vector<L1GtTechnicalTrigger> ttVec;

   // set the trigger bits
   for (unsigned i=0; i!=ttBits.size(); ++i) {
     
     // decide whether this bit fired
     bool fire = false;
     if ((nEvt_ % freq_.at(i)) == 0) fire = true;

     // create trigger bit and store in vector
     // this example only creates triggers for BxInEvent=0
     ttVec.push_back(L1GtTechnicalTrigger(name, ttBits_.at(i), 0, fire);

   }

   // create output record
   std::auto_ptr<L1GtTechnicalTriggerRecord> output(new L1GtTechnicalTriggerRecord());
   output->setGtTechnicalTrigger(ttVec);

   iEvent.put(output);
}

// ------------ method called once each job just before starting event loop  ------------
void 
L1TechTrigExampleProd::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1TechTrigExampleProd::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TechTrigExampleProd);
