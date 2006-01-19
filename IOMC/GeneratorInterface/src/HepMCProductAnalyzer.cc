
#include "IOMC/GeneratorInterface/interface/HepMCProductAnalyzer.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>

using namespace edm;
using namespace std;

HepMCProductAnalyzer::HepMCProductAnalyzer(const edm::ParameterSet& iConfig) :
label_(iConfig.getUntrackedParameter("moduleLabel",std::string("PythiaSource")))
{
   //now do what ever initialization is needed

}


HepMCProductAnalyzer::~HepMCProductAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
HepMCProductAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<HepMCProduct> evt;
//   iEvent.getByLabel("PythiaSource",evt);
//   iEvent.getByLabel("MCFileSource",evt);
   iEvent.getByLabel(label_, evt);
   evt->GetEvent()->print();


}

