/** MadGraphSource
 *
 * A modified version of the PythiaSource by 
 * Filip Moorgat & Hector Naves, that generates 
 * events by reading in a file produced with MadGraph/MadEvent
 * and shower them with Pythia.
 * 
 * July 2006
 * Maria Hansen, University of Bristol
 * Hector Naves : added the MCDB Interface (25/10/06)
 * Dorian Kcira : add automatic end of event processing when hitting
 *                the end of the lhe file (29/01/07)
 ***************************************/
#include "GeneratorInterface/MadGraphInterface/interface/MadGraphSource.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
#include <fstream> 
#include "time.h"

using namespace edm;
using namespace std;


// Generator modifications
#include "CLHEP/HepMC/include/PythiaWrapper6_2.h"
#include "CLHEP/HepMC/ConvertHEPEVT.h"
#include "CLHEP/HepMC/CBhepevt.h"

// MCDB Interface 
#include "GeneratorInterface/MadGraphInterface/interface/MCDBInterface.h"


#define PYGIVE pygive_
extern "C" {
  void PYGIVE(const char*,int length);
}

/*
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP

      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,IDUP(MAXNUP),
     &   ISTUP(MAXNUP),MOTHUP(2,MAXNUP),ICOLUP(2,MAXNUP),PUP(5,MAXNUP),
     &   VTIMUP(MAXNUP),SPINUP(MAXNUP)
*/
extern "C" {
 extern struct HEPEUP{
 int nup;
 int idprup;
 double xwgtup;
 double scalup;
 double aqedup;
 double aqcdup;
 int idup[500];
 int istup[500];
 int mothup[500][2];
 int icolup[500][2];
 double pup[500][5];
 double vtimup[500];
 double spinup[500];
 }hepeup_;
}


HepMC::ConvertHEPEVT conv;

//used for defaults
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;

MadGraphSource::MadGraphSource( const ParameterSet & pset, 
			    InputSourceDescription const& desc) :
  GeneratedInputSource(pset, desc), evt(0),
  //  mcdbArticleID_ (pset.addParameter<int>("mcdbArticleID",0)),
pythiaPylistVerbosity_ (pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0)),pythiaHepMCVerbosity_ (pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false)),maxEventsToPrint_ (pset.getUntrackedParameter<int>("maxEventsToPrint",0)),MGfile_ (pset.getParameter<string>("MadGraphInputFile")),getInputFromMCDB_ (pset.getUntrackedParameter<bool>("getInputFromMCDB",false)),MCDBArticleID_ (pset.getParameter<int>("MCDBArticleID")){

  ifstream file;
  ofstream ofile;

  //******
  // Interface with the LCG MCDB
  //
  if (getInputFromMCDB_)  mcdbGetInputFile(MGfile_, MCDBArticleID_);  
  //  
  //******


  file.open(MGfile_.c_str(),std::ios::in);  
  if(!file){
    cout << "Error: Cannot open MadGraph input file" << endl;
    throw edm::Exception(edm::errors::Configuration,"OpenMadGraphFileError")
      <<" Cannot open MadGraph input file, check file name and path.";
  }else{
    cout << "Opened MadGraph file successfully!" << endl;
    file.close();
  }
  
  cout << "MadGraph input file is " << MGfile_ <<endl;
  
  //Write to file name and first event to be read in to a file which the MadGraph subroutine will read in
  ofile.open("MGinput.dat",ios::out);
  ofile<<MGfile_;
  ofile<<"\n";
  ofile.close();
  
  cout << "MadGraphSource: initializing Pythia. " << endl;  
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

  // Set PYTHIA parameters in a single ParameterSet
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
  }
  
  //Setting random numer seed
    cout << "----------------------------------------------" << endl;
    cout << "Setting Pythia random number seed" << endl;
    cout << "----------------------------------------------" << endl;
  edm::Service<RandomNumberGenerator> rng;
  uint32_t seed = rng->mySeed();
  ostringstream sRandomSet;
  sRandomSet <<"MRPY(1)="<<seed;
  call_pygive(sRandomSet.str());
  // Call pythia initialisation with user defined upinit subroutine
  call_pyinit( "USER", "", "", 0.);
  cout << endl; 
  
  produces<HepMCProduct>();
  cout << "MadGraphSource: starting event generation ... " << endl;
}


MadGraphSource::~MadGraphSource(){
  cout << "MadGraphSource: event generation done. " << endl;
  clear();
  //  rm -f MGinput.dat;
}

void MadGraphSource::clear() {
 
}


bool MadGraphSource::produce(Event & e) {

    auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  

    call_pyevnt();      // generate one event with Pythia
    call_pyhepc( 1 );
    

    HepMC::GenEvent* evt = conv.getGenEventfromHEPEVT();
    evt->set_signal_process_id(pypars.msti[0]);
    evt->set_event_number(numberEventsInRun() - remainingEvents() - 1);

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

   if(hepeup_.nup==0){
      cout << "MadGraphSource::produce - the interface signalled end of Les Houches file. Finishing event processing here."<<endl;
      return false; // finish event processing if variable nup from common block HEPEUP set to 0 in ME2Pythia.f
   }

    if(evt)  bare_product->addHepMCData(evt );

    e.put(bare_product);

    return true;
}

bool MadGraphSource::call_pygive(const std::string& iParm ) {

  int numWarn = pydat1.mstu[26]; //# warnings
  int numErr = pydat1.mstu[22];// # errors
  
//call the fortran routine pygive with a fortran string
  PYGIVE( iParm.c_str(), iParm.length() );  
  //  PYGIVE( iParm );  
//if an error or warning happens it is problem
  return pydat1.mstu[26] == numWarn && pydat1.mstu[22] == numErr;   

}
