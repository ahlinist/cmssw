/*
 *  $Date: 2005/10/05 04:45:33 $
 *  $Revision: 1.1 $
 *  \author N. Amapane - S. Argiro'
 *  Modified by Filip Moorgat & Hector Naves 
 *   for the Generator Interface. 26/10/05
 */


#include "FWCore/Framework/src/TypeID.h" 
#include "IOMC/GeneratorInterface/interface/PythiaSource.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/EDProduct/interface/EDProduct.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include <FWCore/EDProduct/interface/Wrapper.h>
#include <iostream>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "FWCore/Framework/interface/InputSource.h"
#include "FWCore/EDProduct/interface/EventID.h"


using namespace edm;
using namespace std;

// Generator modifications
// ***********************
#include "CLHEP/HepMC/include/PythiaWrapper6_2.h"
#include "CLHEP/HepMC/ConvertHEPEVT.h"
#include "CLHEP/HepMC/CBhepevt.h"


HepMC::ConvertHEPEVT conv;
// ***********************


//used for defaults
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;

PythiaSource::PythiaSource( const ParameterSet & pset, InputSourceDescription const& desc ) :
  InputSource( desc ) ,  
  remainingEvents_(pset.getUntrackedParameter<int>("maxEvents", -1)), 
  numberEventsInRun_(pset.getUntrackedParameter<unsigned int>("numberEventsInRun", remainingEvents_+1)),
  presentRun_( pset.getUntrackedParameter<unsigned int>("firstRun",1)  ),
  nextTime_(pset.getUntrackedParameter<unsigned int>("firstTime",1)),  //time in ns
  timeBetweenEvents_(pset.getUntrackedParameter<unsigned int>("timeBetweenEvents",kNanoSecPerSec/kAveEventPerSec) ),
  numberEventsInThisRun_(0),
  //                              
  //********
  pysubs_msel_(pset.getUntrackedParameter<int>("pysubsMsel", 1)),
  pysubs_msub_(pset.getUntrackedParameter<int>("pysubsMsub", 1)),
  pydatr_mrpy_(pset.getUntrackedParameter<int>("pydatrMrpy", 1)),
  pypars_mstp_(pset.getUntrackedParameter<int>("pyparsMstp", 1)),
  pydat2_pmas_(pset.getUntrackedParameter<int>("pydat2Pmas", 1)),
  //********
  //
  nextID_(presentRun_, 1 ) {


  //********
  // For debugging (and info!) porpuses :P
  cout << endl;
  cout << "******* Pythia Event Selection *******" << endl;
  cout << "pysubsMsel : " << pysubs_msel_ << endl;
  cout << "pysubsMsub : " << pysubs_msub_ << endl;
  cout << "pydatrMrpy : " << pydatr_mrpy_ << endl;
  cout << "pyparsMstp : " << pypars_mstp_ << endl;
  cout << "pydat2Pmas : " << pydat2_pmas_ << endl;
  cout << "**************************************" << endl;
  cout << endl;
  //********


  // Pythia event selection
  //********
  pysubs.msel = pysubs_msel_;
  pysubs.msub[pysubs_msub_ -1] = 1;
  pydatr.mrpy[0] = pydatr_mrpy_;
  pypars.mstp[128-1] = pypars_mstp_;
  pydat2.pmas[25-1][0] = pydat2_pmas_;
  call_pyinit( "CMS", "p", "p", 14000. );
  cout << endl; // Stetically add for the output
  //********                                      


  cout << "PythiaSource: initializing Pythia. " << endl;
  ModuleDescription      modDesc_; 
  modDesc_.pid = PS_ID("PythiaSource");
  modDesc_.moduleName_ = "PythiaSource";
  modDesc_.moduleLabel_ = "PythiaInput";
  modDesc_.versionNumber_ = 1UL;
  modDesc_.processName_ = "HepMC";
  modDesc_.pass = 1UL;  
    
  branchDesc_.module = modDesc_;   
  branchDesc_.fullClassName_= "HepMCProduct";
  branchDesc_.friendlyClassName_ = "HepMCProduct";   
  preg_->addProduct(branchDesc_);
 }


PythiaSource::~PythiaSource(){
  clear();
}

void PythiaSource::clear() {
 
}


auto_ptr<EventPrincipal> PythiaSource::read() {

  auto_ptr<EventPrincipal> result(0);
 

  // event loop
  if (remainingEvents_-- != 0) {
  
    result = auto_ptr<EventPrincipal>(new EventPrincipal(nextID_, Timestamp(nextTime_), *preg_));
    HepMCProduct *bare_product = new HepMCProduct();  
    cout << "PythiaSource: Start generating ...  " << endl;


    //********                                         
    //
    call_pyevnt();      // generate one event with Pythia
    call_pyhepc( 1 );
    
    HepMC::GenEvent* evt = conv.getGenEventfromHEPEVT();
    evt->set_signal_process_id(pysubs_msub_);
    evt->set_event_number(numberEventsInRun_ - remainingEvents_ - 1);
    
    evt->print();

    //evt = reader_->fillCurrentEventData(); 
    //********                                      

    if(evt)  bare_product->addHepMCData(evt );
    edm::Wrapper<HepMCProduct> *wrapped_product = 
      new edm::Wrapper<HepMCProduct> (*bare_product); 
    auto_ptr<EDProduct>  prod(wrapped_product);
    auto_ptr<Provenance> prov(new Provenance(branchDesc_));

    result->put(prod, prov);
    cout << "PythiaSource: Generation done " << endl;
    if( ++numberEventsInThisRun_ < numberEventsInRun_ ) {
        nextID_ = nextID_.next();
      } else {
        nextID_ = nextID_.nextRunFirstEvent();
        numberEventsInThisRun_ = 0;
      }
    nextTime_ += timeBetweenEvents_;
  }
  return result;



}

