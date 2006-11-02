
#include "GeneratorInterface/MuEnrichInterface/interface/PythiaWeightFilter.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


using namespace edm;
using namespace std;


PythiaWeightFilter::PythiaWeightFilter(const edm::ParameterSet& iConfig) :
label_(iConfig.getUntrackedParameter("moduleLabel",std::string("source"))),
minweight(iConfig.getUntrackedParameter<double>("MinWeight", 0.1))
{
  edm::LogInfo ("PythiaWeightFilter") << "Minimum accepted weight"<<minweight<<endl;   //now do what ever initialization is needed

}


PythiaWeightFilter::~PythiaWeightFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
bool PythiaWeightFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   bool accepted = false;
   Handle<HepMCProduct> evt;
   iEvent.getByLabel(label_, evt);
   HepMC::GenEvent * myGenEvent = new  HepMC::GenEvent(*(evt->GetEvent()));
   if (myGenEvent->weights()[0]>minweight)accepted=true;
   edm::LogInfo ("PythiaWeightFilter") << "Weight="<<myGenEvent->weights()[0]<<" accepted="<<accepted<<endl;
   delete myGenEvent;
   return accepted;

}

// ------------ method called once each job just before starting event loop  ------------
void 
PythiaWeightFilter::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PythiaWeightFilter::endJob() {
}
