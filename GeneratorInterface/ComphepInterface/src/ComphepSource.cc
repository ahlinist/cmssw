/*
 *  $Date: 2006/07/07 07:19:42 $
 *  $Revision: 1.20 $
 *  
 *  Filip Moorgat & Hector Naves 
 *  26/10/05
 * 
 *  Patrick Janot : added the PYTHIA card reading
 *
 *  Serge SLabospitsky : added Comphep reading tools 
 */


#include "GeneratorInterface/ComphepInterface/interface/ComphepSource.h"
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
#include "CLHEP/HepMC/include/PythiaWrapper6_2.h"
#include "GeneratorInterface/ComphepInterface/interface/ComphepWrapper.h"
//#include "GeneratorInterface/CommonInterface/interface/PretauolaWrapper.h"
#include "CLHEP/HepMC/ConvertHEPEVT.h"
#include "CLHEP/HepMC/CBhepevt.h"

#include "GeneratorInterface/CommonInterface/interface/PythiaCMS.h"
#include "GeneratorInterface/CommonInterface/interface/Txgive.h"

HepMC::ConvertHEPEVT conv;
// ***********************


//used for defaults
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;

ComphepSource::ComphepSource( const ParameterSet & pset, 
			    InputSourceDescription const& desc ) :
  GeneratedInputSource(pset, desc), evt(0), 
  pythiaPylistVerbosity_ (pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0)),
  pythiaHepMCVerbosity_ (pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false)),
  maxEventsToPrint_ (pset.getUntrackedParameter<int>("maxEventsToPrint",1))
  
{
  
  cout << "ComphepSource: initializing Pythia. " << endl;
  
 
  
  // PYLIST Verbosity Level
  // Valid PYLIST arguments are: 1, 2, 3, 5, 7, 11, 12, 13
  pythiaPylistVerbosity_ = pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0);
  cout << "Pythia PYLIST verbosity level = " << pythiaPylistVerbosity_ << endl;
  
  // HepMC event verbosity Level
  pythiaHepMCVerbosity_ = pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false);
  cout << "Pythia HepMC verbosity = " << pythiaHepMCVerbosity_ << endl; 

  //Max number of events printed on verbosity level 
  maxEventsToPrint_ = pset.getUntrackedParameter<int>("maxEventsToPrint",0);
  cout << "Number of events to be printed = " << maxEventsToPrint_ << endl;

  
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
    cout << "----------------------------------------------" << endl;
    cout << "Read PYTHIA parameter set " << mySet << endl;
    cout << "----------------------------------------------" << endl;
    
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

   cout << "----------------------------------------------" << endl; 
   cout << "Reading CSA parameter settings. " << endl;
   cout << "----------------------------------------------" << endl;                                                                           

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
    cout << "----------------------------------------------" << endl;
    cout << "Setting Pythia random number seed " << endl;
    cout << "----------------------------------------------" << endl;
  edm::Service<RandomNumberGenerator> rng;
  uint32_t seed = rng->mySeed();
  ostringstream sRandomSet;
  sRandomSet <<"MRPY(1)="<<seed;
  call_pygive(sRandomSet.str());

  call_pevmain(); 

  //  call_pretauola(-1);     // TAUOLA initialization

  cout << endl; // Stetically add for the output
  //********                                      
  
  produces<HepMCProduct>();
  cout << "ComphepSource: starting event generation ... " << endl;
}


ComphepSource::~ComphepSource(){
  cout << "ComphepSource: event generation done. " << endl;
  call_pystat(1);
  //  call_pretauola(1);  // output from TAUOLA 
  clear(); 
}

void ComphepSource::clear() {
 
}


bool ComphepSource::produce(Event & e) {

    auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  
    //cout << "ComphepSource: Generating event ...  " << endl;

    //********                                         
    //

    call_pyevnt();      // generate one event with Pythia
    //    call_pretauola(0);  // tau-lepton decays with TAUOLA 

    call_pyhepc( 1 );
    
    HepMC::GenEvent* evt = conv.getGenEventfromHEPEVT();
    evt->set_signal_process_id(pypars.msti[0]);
    evt->set_event_number(numberEventsInRun() - remainingEvents() - 1);
    

    //******** Verbosity ********
    
    if(event() <= maxEventsToPrint_ &&
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

    return true;
}

bool 
ComphepSource::call_pygive(const std::string& iParm ) 
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
ComphepSource::call_txgive(const std::string& iParm ) 
   {
    //call the fortran routine txgive with a fortran string
     TXGIVE( iParm.c_str(), iParm.length() );  
     return 1;  
   }

bool
ComphepSource::call_txgive_init() 
{
   TXGIVE_INIT();
   cout << "  Setting CSA defaults.   "   << endl;
   return 1;
}
