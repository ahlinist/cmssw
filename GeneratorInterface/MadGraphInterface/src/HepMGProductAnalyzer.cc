#include "GeneratorInterface/MadGraphInterface/interface/HepMGProductAnalyzer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>

HepMGProductAnalyzer::HepMGProductAnalyzer(const edm::ParameterSet& iConfig) :
label_(iConfig.getUntrackedParameter("moduleLabel",std::string("source")))
{

}


HepMGProductAnalyzer::~HepMGProductAnalyzer()
{
}

void HepMGProductAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<HepMCProduct> evt;
   iEvent.getByType(evt);

   HepMC::GenEvent * myGenEvent = new  HepMC::GenEvent(*(evt->GetEvent()));

   //Print out the HepMCProduct
   //evt->GetEvent()->print();
  
   //To print out status code, pdg, mass, etc table uncomment below
      this->printTable(myGenEvent);

}

void HepMGProductAnalyzer::printTable(HepMC::GenEvent * event)
{
  HepMC::GenEvent * myGenEvent = event;
  std::cout<<std::endl;
  std::cout<<"Barcode:  Pgd:     Status:  Mother:"<<std::endl;   
  for ( HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin();
	p != myGenEvent->particles_end(); ++p) { 
    std::cout<<" "<<(*p)->barcode()<<"        "<<(*p)->pdg_id()<<"       "<<(*p)->status()<<"        "<<(*p)->Mother()<<std::endl;
  }
}
