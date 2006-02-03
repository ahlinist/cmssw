/*
 *  $Date: 2006/01/16 15:41:39 $
 *  $Revision: 1.4 $
 *  \author N. Amapane - S. Argiro'
 *  Modified by Filip Moorgat & Hector Naves 
 *   for the Generator Interface. 26/10/05
 */


#include "IOMC/GeneratorInterface/interface/PythiaSource.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include <iostream>

#include "FWCore/ParameterSet/interface/ParameterSet.h"


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
  GeneratedInputSource(pset, desc),  
  //                              
  //********
  pysubs_msel_(pset.getUntrackedParameter<int>("pysubsMsel", 1)),
  pysubs_msub_(pset.getUntrackedParameter<int>("pysubsMsub", 1)),
  pydatr_mrpy_(pset.getUntrackedParameter<int>("pydatrMrpy", 1)),
  pypars_mstp_(pset.getUntrackedParameter<int>("pyparsMstp", 1)),
  pydat2_pmas_(pset.getUntrackedParameter<int>("pydat2Pmas", 1)),
  pythiaVerbosity_(pset.getUntrackedParameter<bool>("pythiaVerbosity", 
  false)) {


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
  produces<HepMCProduct>();
 }


PythiaSource::~PythiaSource(){
  clear();
}

void PythiaSource::clear() {
 
}


bool PythiaSource::produce(Event & e) {

    auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  
    cout << "PythiaSource: Generating event ...  " << endl;

    //********                                         
    //
    call_pyevnt();      // generate one event with Pythia
    call_pyhepc( 1 );
    
    HepMC::GenEvent* evt = conv.getGenEventfromHEPEVT();
    evt->set_signal_process_id(pysubs_msub_);
    evt->set_event_number(numberEventsInRun() - remainingEvents() - 1);
    
    if (pythiaVerbosity_) evt->print();

    //evt = reader_->fillCurrentEventData(); 
    //********                                      

    if(evt)  bare_product->addHepMCData(evt );

    e.put(bare_product);

    return true;
}

