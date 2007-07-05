#include "IOMC/GeneratorInterface/interface/BsandBoFilter.h"

using namespace edm;
using namespace std;
using namespace HepMC;



BsandBoFilter::BsandBoFilter(const edm::ParameterSet& iConfig)
{
  label_ = iConfig.getUntrackedParameter("moduleLabel",std::string("source"));

  noAccepted = 0;
}


BsandBoFilter::~BsandBoFilter()
{  
  std::cout << "Total number of accepted events = " << noAccepted << std::endl;
}



bool BsandBoFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel(label_, evt);
  
  HepMC::GenEvent * generated_event = new HepMC::GenEvent(*(evt->GetEvent()));

  bool event_passed = true;
							
  for(HepMC::GenEvent::particle_iterator particle_it = generated_event->particles_begin(); particle_it != generated_event->particles_end(); particle_it++){
  	if((*particle_it)->momentum().pz() > (*particle_it)->momentum().e()){
		cout << "pz larger then E!" << endl;
		event_passed = false;
	}
  }
  
  if (event_passed) noAccepted++;
  
  delete generated_event; 
  
  return event_passed;
}
