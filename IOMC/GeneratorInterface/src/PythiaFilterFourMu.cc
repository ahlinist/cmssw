#include "IOMC/GeneratorInterface/interface/PythiaFilterFourMu.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <cmath>

PythiaFilterFourMu::PythiaFilterFourMu(const edm::ParameterSet& iConfig) :
  label_(iConfig.getUntrackedParameter("moduleLabel",std::string("source"))),
  etaMuMax(iConfig.getUntrackedParameter<double>("MaxEta", 2.5)),
  etaMuMin(iConfig.getUntrackedParameter<double>("MinEta", -2.5)),
  ptMuMin(iConfig.getUntrackedParameter<double>("MinMuonPt", 3.)),
  maxnumberofeventsinrun(iConfig.getUntrackedParameter<int>("MaxEvents",50000)) { 
  theNumberOfSelected = 0;
  NumberOfAllEvents = 0;
 }


PythiaFilterFourMu::~PythiaFilterFourMu(){
  std::cout<<"                    ***************   PythiaFilterFourMu says:  *************** "<<std::endl;
  std::cout<<"  The MC-Generator generated "<<NumberOfAllEvents<<" events"<<std::endl;
  std::cout<<"  I accepted                 "<<theNumberOfSelected<< " events" <<std::endl;
  std::cout<<"  Cut efficiency E= "<< std::setprecision(5)<<static_cast<double>(theNumberOfSelected)/NumberOfAllEvents<<std::endl;
  std::cout<<"                      ************************************************"<<std::endl; 
}


// ------------ method called to produce the data  ------------
bool PythiaFilterFourMu::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  if(theNumberOfSelected>=maxnumberofeventsinrun)   {
    throw cms::Exception("endJob") << "We have reached the maximum number of events "<<maxnumberofeventsinrun<<std::endl;
  }

  NumberOfAllEvents++;
  bool accepted = false;
  edm::Handle<edm::HepMCProduct> evt;
  iEvent.getByLabel(label_, evt);
  
  const HepMC::GenEvent * myGenEvent = evt->GetEvent();

  int muPlusCount =0;
  int muMinusCount =0;
  
     

  // cycle on particles in the event
  for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin();   p != myGenEvent->particles_end(); p++ ) {
      
    if ( std::abs((*p)->pdg_id()) == 13               &&
	 (*p)->status() == 1                          &&
	 (*p)->momentum().eta() < etaMuMax            &&
	 (*p)->momentum().eta() > etaMuMin            &&
	 (*p)->momentum().perp() > ptMuMin              ){
      if (((*p)->pdg_id())<0) muPlusCount++;
      else muMinusCount++; 
      if(muPlusCount>=2 && muMinusCount>=2) {
	accepted = true;
	break;
      }
    }

  } // end for
   
  if (accepted) {
    theNumberOfSelected++;
    return true; 
  }

  else return false;
  
}


