/*
 *  $Date: 2006/02/03 10:34:09 $
 *  $Revision: 1.7 $
 *  
 *  Filip Moorgat & Hector Naves 
 *  26/10/05
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



PythiaSource::PythiaSource( const ParameterSet & pset, InputSourceDescription const& desc ) :
  GeneratedInputSource(pset, desc), evt(0), pythiaVerbosity_(pset.getUntrackedParameter<bool>("pythiaVerbosity", 
  false)) {


  ParameterSet pythia_params = pset.getParameter<ParameterSet>("PythiaParameters") ;

  // prototype with only a few parameters

  pysubs_msel_ = pythia_params.getParameter<int>("pysubsMsel");
  pysubs_msub_ = pythia_params.getParameter<int>("pysubsMsub");
  pydatr_mrpy_ = pythia_params.getParameter<int>("pydatrMrpy");
  pypars_mstp_ = pythia_params.getParameter<int>("pyparsMstp");
  pydat2_pmas_ = pythia_params.getParameter<int>("pydat2Pmas");



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

  cout << "PythiaSource: initializing Pythia. " << endl;
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


  produces<HepMCProduct>();
  cout << "PythiaSource: starting event generation ... " << endl;
 }


PythiaSource::~PythiaSource(){
  cout << "PythiaSource: event generation done. " << endl;
  clear();
}

void PythiaSource::clear() {
 
}


bool PythiaSource::produce(Event & e) {
    
    // clean up GenEvent memory 
    if (evt != 0) {
	  delete evt;
	  evt = 0;
    }
		
    auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  
    if (pythiaVerbosity_) {
      cout << "PythiaSource: Generating event number " 
       <<  numberEventsInRun() - remainingEvents()    <<". " << endl;
    }
    //********                                         
    //
    call_pyevnt();      // generate one event with Pythia
    call_pyhepc( 1 );
    
    HepMC::GenEvent* evt = conv.getGenEventfromHEPEVT();
    evt->set_signal_process_id(pysubs_msub_);
    evt->set_event_number(numberEventsInRun() - remainingEvents());
    
    if (pythiaVerbosity_) evt->print();
    
    if (evt == 0) return false;
 
    if (evt)  bare_product->addHepMCData(evt );

    e.put(bare_product);

    return true;
}

