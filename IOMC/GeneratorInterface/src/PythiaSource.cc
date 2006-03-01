/*
 *  $Date: 2006/02/28 20:08:25 $
 *  $Revision: 1.12 $
 *  
 *  Filip Moorgat & Hector Naves 
 *  26/10/05
 * 
 * Patrick Janot : Add the PYTHIA card reading
 */


#include "IOMC/GeneratorInterface/interface/PythiaSource.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include <iostream>
#include "time.h"

using namespace edm;
using namespace std;

// Generator modifications
// ***********************
#include "CLHEP/HepMC/include/PythiaWrapper6_2.h"
#include "CLHEP/HepMC/ConvertHEPEVT.h"
#include "CLHEP/HepMC/CBhepevt.h"

#define PYGIVE pygive_
extern "C" {
  void PYGIVE(const char*,int length);
}

HepMC::ConvertHEPEVT conv;
// ***********************


//used for defaults
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;

PythiaSource::PythiaSource( const ParameterSet & pset, 
			    InputSourceDescription const& desc ) :
  GeneratedInputSource(pset, desc), evt(0), 
  pythiaVerbosity_ (pset.getUntrackedParameter<bool>("pythiaVerbosity",false))
{
  
  cout << "PythiaSource: initializing Pythia. " << endl;

  // Verbosity
  pythiaVerbosity_ = pset.getUntrackedParameter<bool>("pythiaVerbosity",false);
  cout << "Pythia verbosity = " << pythiaVerbosity_ << endl;

  // Set PYTHIA parameters in a single ParameterSet
  ParameterSet pythia_params = 
    pset.getParameter<ParameterSet>("PythiaParameters") ;

  // The parameter sets to be read (default, min bias, user ...)
  vector<string> setNames = 
    pythia_params.getParameter<vector<string> >("parameterSets");
  
  // Loop over the sets
  for ( unsigned i=0; i<setNames.size(); ++i ) {
    
    string mySet = setNames[i];

    // Read the PYTHIA parameters for each set of parameters
    vector<string> pars = 
      pythia_params.getParameter<vector<string> >(mySet);

    cout << "----------------------------------------------" << endl;
    cout << "Read PYTHIA parameter set " << mySet << endl;
    cout << "----------------------------------------------" << endl;

    // Loop over all parameters and stop in case of mistake
    for( vector<string>::const_iterator  
	   itPar = pars.begin(); itPar != pars.end(); ++itPar ) {
      if( ! call_pygive(*itPar) ) {
	throw edm::Exception(edm::errors::Configuration,"PythiaError") 
	  <<" pythia did not accept the following \""<<*itPar<<"\"";
      }
    }
  }


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

    auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  
    //cout << "PythiaSource: Generating event ...  " << endl;

    //********                                         
    //
    call_pyevnt();      // generate one event with Pythia
    call_pyhepc( 1 );
    
    HepMC::GenEvent* evt = conv.getGenEventfromHEPEVT();
    evt->set_signal_process_id(pypars.msti[0]);
    evt->set_event_number(numberEventsInRun() - remainingEvents() - 1);
    
    if (pythiaVerbosity_) {
     cout << "Event process = " << pypars.msti[0] << endl 
	 << "----------------------" << endl;
     evt->print();
    }
    //evt = reader_->fillCurrentEventData(); 
    //********                                      

    if(evt)  bare_product->addHepMCData(evt );

    e.put(bare_product);

    return true;
}

bool 
PythiaSource::call_pygive(const std::string& iParm ) {

  int numWarn = pydat1.mstu[26]; //# warnings
  int numErr = pydat1.mstu[22];// # errors
  
//call the fortran routine pygive with a fortran string
  PYGIVE( iParm.c_str(), iParm.length() );  
  //  PYGIVE( iParm );  
//if an error or warning happens it is problem
  return pydat1.mstu[26] == numWarn && pydat1.mstu[22] == numErr;   

}
