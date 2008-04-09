/*
 *  $Date: 2007/11/28 15:35:29 $
 *  $Revision: 1.4 $
 *  
 *  Filip Moorgat & Hector Naves 
 *  26/10/05
 * 
 *  Patrick Janot : added the PYTHIA card reading
 *
 *  Serge SLabospitsky : added Comphep reading tools 
 *
 *  Hector Naves : added MCDB Interface (25/10/06)
 */


#include "GeneratorInterface/ComphepInterface/interface/ComphepProducer.h"
#include "GeneratorInterface/ComphepInterface/interface/ComphepWrapper.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RandFlat.h"

#include <iostream>
#include "time.h"

using namespace edm; 
using namespace std;

// Generator modifications
// ***********************
#include "HepMC/PythiaWrapper6_2.h"
#include "HepMC/IO_HEPEVT.h"
#include "GeneratorInterface/ComphepInterface/interface/ComphepWrapper.h"
//#include "GeneratorInterface/CommonInterface/interface/PretauolaWrapper.h"

#include "GeneratorInterface/CommonInterface/interface/PythiaCMS.h"
#include "GeneratorInterface/CommonInterface/interface/Txgive.h"

HepMC::IO_HEPEVT conv2;
// ***********************

// MCDB Interface
#include "GeneratorInterface/ComphepInterface/interface/MCDBInterface.h"

//used for defaults
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;

// Added 12 March '08 by JMM as part of forcing Pythia to use
// the random number engines from the random service.  The
// remaining question is how to make sure this version of PYR
// is used instead of the one embedded in Pythia.

#define PYR pyr_
extern "C" {
  static HepRandomEngine* RandomEnginePointer;

  double PYR(int idummy)
  {
    return RandomEnginePointer->flat();
  }
}

ComphepProducer::ComphepProducer( const ParameterSet & pset) :
  EDProducer(), evt(0), 
  pythiaPylistVerbosity_ (pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0)),
  pythiaHepMCVerbosity_ (pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false)),
  maxEventsToPrint_ (pset.getUntrackedParameter<int>("maxEventsToPrint",1)),
  getInputFromMCDB_ (pset.getUntrackedParameter<bool>("getInputFromMCDB",false)),
  MCDBArticleID_ (pset.getParameter<int>("MCDBArticleID")),
  eventNumber_(0)
  {


  //******
  // Interface with the LCG MCDB
  //
  if (getInputFromMCDB_) {
    CHFile_ = pset.getUntrackedParameter<string>("ComphepInputFile");
    mcdbGetInputFile(CHFile_, MCDBArticleID_);
  }
  //  
  //******


  
  
  // PYLIST Verbosity Level
  // Valid PYLIST arguments are: 1, 2, 3, 5, 7, 11, 12, 13
  pythiaPylistVerbosity_ = pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0);
  
  // HepMC event verbosity Level
  pythiaHepMCVerbosity_ = pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false);

  //Max number of events printed on verbosity level 
  maxEventsToPrint_ = pset.getUntrackedParameter<int>("maxEventsToPrint",0);
  
  ////////////////////////
  // Set PYTHIA parameters in a single ParameterSet
  {
  ParameterSet pythia_params = 
    pset.getParameter<ParameterSet>("PythiaParameters") ;
  
  // The parameter sets to be read (default, min bias, user ...) in the
  // proper order.
  vector<string> setNames = 
    pythia_params.getParameter<vector<string> >("parameterSets");
  
  // Loop over the sets
  for ( unsigned i=0; i<setNames.size(); ++i ) {
    
    string mySet = setNames[i];
    
    // Read the PYTHIA parameters for each set of parameters
    vector<string> pars = 
      pythia_params.getParameter<vector<string> >(mySet);
    
    if (mySet != "CSAParameters"){
    
    // Loop over all parameters and stop in case of mistake
    for( vector<string>::const_iterator  
	   itPar = pars.begin(); itPar != pars.end(); ++itPar ) {
      static string sRandomValueSetting("MRPY(1)");
      if( 0 == itPar->compare(0,sRandomValueSetting.size(),sRandomValueSetting) ) {
	throw edm::Exception(edm::errors::Configuration,"PythiaError")
	  <<" attempted to set random number using pythia command 'MRPY(1)' this is not allowed.\n  Please use the RandomNumberGeneratorService to set the random number seed.";
      }
      if( ! call_pygive(*itPar) ) {
	throw edm::Exception(edm::errors::Configuration,"PythiaError") 
	  <<" pythia did not accept the following \""<<*itPar<<"\"";
      }
    }
    }else if(mySet == "CSAParameters"){   

   // Read CSA parameter
  
   pars = pythia_params.getParameter<vector<string> >("CSAParameters");


    call_txgive_init();
  
  
   // Loop over all parameters and stop in case of a mistake
    for (vector<string>::const_iterator 
            itPar = pars.begin(); itPar != pars.end(); ++itPar) {
      call_txgive(*itPar); 
     
         } 
  
  }
  }

}


 // Read the Comphep parameter
#include "GeneratorInterface/CommonInterface/interface/ExternalGenRead.inc"



  //In the future, we will get the random number seed on each event and tell 
  // pythia to use that new seed
  edm::Service<RandomNumberGenerator> rng;
  RandomEnginePointer = &(rng->getEngine());

  uint32_t seed = rng->mySeed();
  ostringstream sRandomSet;
  sRandomSet <<"MRPY(1)="<<seed;
  call_pygive(sRandomSet.str());

  call_pevmain(); 

  //  call_pretauola(-1);     // TAUOLA initialization

  cout << endl; // Statically add for the output
  //********                                      
  
  produces<HepMCProduct>();
}


ComphepProducer::~ComphepProducer(){
  call_pystat(1);
  //  call_pretauola(1);  // output from TAUOLA 
  clear(); 
}

void ComphepProducer::clear() {
 
}


void ComphepProducer::produce(Event & e, const EventSetup& es ) {

    auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  

    //********                                         
    //

    call_pyevnt();      // generate one event with Pythia
    //    call_pretauola(0);  // tau-lepton decays with TAUOLA 

    call_pyhepc( 1 );
    
    //    HepMC::GenEvent* evt = conv2.getGenEventfromHEPEVT();
    HepMC::GenEvent* evt = conv2.read_next_event();
    evt->set_signal_process_id(pypars.msti[0]);
    ++eventNumber_;
    evt->set_event_number(eventNumber_);
    

    //******** Verbosity ********
    
    if(e.id().event() <= maxEventsToPrint_ &&
       (pythiaPylistVerbosity_ || pythiaHepMCVerbosity_)) {

      // Prints PYLIST info
      if(pythiaPylistVerbosity_) {
	call_pylist(pythiaPylistVerbosity_);
      }
      
      // Prints HepMC event
      if(pythiaHepMCVerbosity_) {
	cout << "Event process = " << pypars.msti[0] << endl 
	<< "----------------------" << endl;
	evt->print();
      }
    }
    
    
    //evt = reader_->fillCurrentEventData(); 
    //********                                      

    if(evt)  bare_product->addHepMCData(evt );

    e.put(bare_product);
}

bool 
ComphepProducer::call_pygive(const std::string& iParm ) 
 {
  int numWarn = pydat1.mstu[26]; //# warnings
  int numErr = pydat1.mstu[22];// # errors
//call the fortran routine pygive with a fortran string
  PYGIVE( iParm.c_str(), iParm.length() );  
//if an error or warning happens it is problem
  return pydat1.mstu[26] == numWarn && pydat1.mstu[22] == numErr;   
}
//------------
bool 
ComphepProducer::call_txgive(const std::string& iParm ) 
   {
    //call the fortran routine txgive with a fortran string
     TXGIVE( iParm.c_str(), iParm.length() );  
     return 1;  
   }

bool
ComphepProducer::call_txgive_init() 
{
   TXGIVE_INIT();
   return 1;
}
