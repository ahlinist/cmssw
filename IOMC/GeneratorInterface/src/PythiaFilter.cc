
#include "IOMC/GeneratorInterface/interface/PythiaFilter.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>

using namespace edm;
using namespace std;


PythiaFilter::PythiaFilter(const edm::ParameterSet& iConfig) :
label_(iConfig.getUntrackedParameter("moduleLabel",std::string("PythiaSource"))),
minptcut(iConfig.getUntrackedParameter("MinMuonPt", 20.))
{
   //now do what ever initialization is needed

}


PythiaFilter::~PythiaFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
bool PythiaFilter::filter(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   bool accepted = false;
   Handle<HepMCProduct> evt;
   iEvent.getByLabel(label_, evt);


    HepMC::GenEvent * myGenEvent = new  HepMC::GenEvent(*(evt->GetEvent()));
    
    for ( HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin();
	  p != myGenEvent->particles_end(); ++p ) {
	  
	
	if ( abs((*p)->pdg_id()) == 13 
	     && (*p)->momentum().perp() > minptcut ) {accepted = true;} 
	  
    }




    delete myGenEvent; 


   if (accepted){
   return true; } else {return false;}

}

