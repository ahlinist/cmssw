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
 * Dorian Kcira : added ME-PS matching (22/05/2007)
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

// Generator modifications
#include "CLHEP/HepMC/include/PythiaWrapper6_2.h"
//#include "CLHEP/HepMC/ConvertHEPEVT.h"
#include "HepMC/IO_HEPEVT.h"
//#include "CLHEP/HepMC/CBhepevt.h"

// MCDB Interface 
#include "GeneratorInterface/MadGraphInterface/interface/MCDBInterface.h"

#define PYGIVE pygive_
extern "C" {
  void PYGIVE(const char*,int length);
}

extern "C"{
 void eventtree_();
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

/*
ME2Pythia.f
      double precision etcjet,rclmax,etaclmax,qcut,clfact
      integer maxjets,minjets,iexcfile,ktsche
      common/MEMAIN/etcjet,rclmax,etaclmax,qcut,clfact,
     $   maxjets,minjets,iexcfile,ktsche
      DATA ktsche/0/
      DATA qcut,clfact/0d0,0d0/
*/
extern "C" {
 extern struct MEMAIN{
 double etcjet;
 double rclmax;
 double etaclmax;
 double qcut;
 double clfact;
 int maxjets;
 int minjets;
 int iexcfile;
 int ktsche;
 }memain_;
}

//HepMC::ConvertHEPEVT conv;
 HepMC::IO_HEPEVT conv;

//used for defaults - change these to defines? TODO
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;

using namespace edm;

MadGraphSource::MadGraphSource( const ParameterSet & pset, InputSourceDescription const& desc) : GeneratedInputSource(pset, desc), evt(0),
pythiaPylistVerbosity_ (pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0)),pythiaHepMCVerbosity_ (pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false)),maxEventsToPrint_ (pset.getUntrackedParameter<int>("maxEventsToPrint",0)),MGfile_ (pset.getParameter<std::string>("MadGraphInputFile")),getInputFromMCDB_ (pset.getUntrackedParameter<bool>("getInputFromMCDB",false)),MCDBArticleID_ (pset.getParameter<int>("MCDBArticleID")),firstEvent_(pset.getUntrackedParameter<unsigned int>("firstEvent", 0)),lhe_event_counter_(0),MEMAIN_etaclmax(pset.getUntrackedParameter<double>("MEMAIN_etaclmax",0.)),MEMAIN_qcut(pset.getUntrackedParameter<double>("MEMAIN_qcut",0.)),MEMAIN_iexcfile(pset.getUntrackedParameter<unsigned int>("MEMAIN_iexcfile",0)), produceEventTreeFile_ (pset.getUntrackedParameter<bool>("produceEventTreeFile",false)) {

  std::ifstream file;
  std::ofstream ofile;

  // Interface with the LCG MCDB
  if (getInputFromMCDB_)  mcdbGetInputFile(MGfile_, MCDBArticleID_);

  file.open(MGfile_.c_str(),std::ios::in);  
  if(!file){
    edm::LogError("GeneratorError|OpenMadGraphFileError")<< "Error: Cannot open MadGraph input file";
    throw edm::Exception(edm::errors::Configuration,"OpenMadGraphFileError")
      <<" Cannot open MadGraph input file, check file name and path.";
  }else{
    edm::LogInfo("Generator|MadGraph")<<"Opened MadGraph file successfully!";
    file.close();
  }
  
  edm::LogInfo("Generator|MadGraph")<< "MadGraph input file is " << MGfile_;
  
  //Write to file name and first event to be read in to a file which the MadGraph subroutine will read in
  ofile.open("MGinput.dat",std::ios::out);
  ofile<<MGfile_;
  ofile<<"\n";
  ofile.close();
  
  // first set to default values, mostly zeros
  memain_.etcjet=0.;
  memain_.rclmax=0.;
  memain_.etaclmax=0.;
  memain_.qcut=0.;
  memain_.clfact=0.;
  memain_.maxjets=0;
  memain_.minjets=0;
  memain_.iexcfile=0;
  memain_.ktsche=0;
  // then set (some) values from cards
  memain_.iexcfile=MEMAIN_iexcfile;
  memain_.etaclmax=MEMAIN_etaclmax;
  memain_.qcut=MEMAIN_qcut;
  // print out
  edm::LogInfo("Generator|MadGraph")<<"MEMAIN before ME2pythia initialization - etcjet ="<<memain_.etcjet<<" rclmax ="<<memain_.rclmax<<" etaclmax ="<<memain_.etaclmax<<" qcut ="<<memain_.qcut<<" clfact ="<<memain_.clfact<<" maxjets ="<<memain_.maxjets<<" minjets ="<<memain_.minjets<<" iexcfile ="<<memain_.iexcfile<<" ktsche ="<<memain_.ktsche;

  edm::LogInfo("Generator|MadGraph")<<"MadGraphSource: initializing Pythia.";
  // PYLIST Verbosity Level
  // Valid PYLIST arguments are: 1, 2, 3, 5, 7, 11, 12, 13
  edm::LogInfo("Generator|MadGraph")<<"Pythia PYLIST verbosity level = " << pythiaPylistVerbosity_;
  // HepMC event verbosity Level
  edm::LogInfo("Generator|MadGraph")<<"Pythia HepMC verbosity = " << pythiaHepMCVerbosity_;
  //Max number of events printed on verbosity level 
  edm::LogInfo("Generator|MadGraph")<<"Number of events to be printed = " << maxEventsToPrint_;
  // max nr of events and first event
  edm::LogInfo("Generator|MadGraph")<<"firstEvent / maxEvents = "<<firstEvent_<<" / "<< maxEvents();

  // Set PYTHIA parameters in a single ParameterSet
  ParameterSet pythia_params = pset.getParameter<ParameterSet>("PythiaParameters") ;
  
  // The parameter sets to be read (default, min bias, user ...) in the proper order.
  std::vector<std::string> setNames = pythia_params.getParameter<std::vector<std::string> >("parameterSets");
  
  // Loop over the sets
  for ( unsigned i=0; i<setNames.size(); ++i ) {
    std::string mySet = setNames[i];
    
    // Read the PYTHIA parameters for each set of parameters
    std::vector<std::string> pars = 
      pythia_params.getParameter<std::vector<std::string> >(mySet);
    
    edm::LogInfo("Generator|MadGraph")<<"----------------------------------------------";
    edm::LogInfo("Generator|MadGraph")<<"Read PYTHIA parameter set " << mySet;
    edm::LogInfo("Generator|MadGraph")<<"----------------------------------------------";
    
    // Loop over all parameters and stop in case of mistake
    for( std::vector<std::string>::const_iterator  
	   itPar = pars.begin(); itPar != pars.end(); ++itPar ) {
      static std::string sRandomValueSetting("MRPY(1)");
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
  edm::LogInfo("Generator|MadGraph")<<"----------------------------------------------";
  edm::LogInfo("Generator|MadGraph")<<"Setting Pythia random number seed";
  edm::LogInfo("Generator|MadGraph")<<"----------------------------------------------";
  edm::Service<RandomNumberGenerator> rng;
  uint32_t seed = rng->mySeed();
  std::ostringstream sRandomSet;
  sRandomSet <<"MRPY(1)="<<seed;
  call_pygive(sRandomSet.str());
  // Call pythia initialisation with user defined upinit subroutine
  call_pyinit( "USER", "", "", 0.);

  edm::LogInfo("Generator|MadGraph")<<"MEMAIN after ME2pythia initialization - etcjet ="<<memain_.etcjet<<" rclmax ="<<memain_.rclmax<<" etaclmax ="<<memain_.etaclmax<<" qcut ="<<memain_.qcut<<" clfact ="<<memain_.clfact<<" maxjets ="<<memain_.maxjets<<" minjets ="<<memain_.minjets<<" iexcfile ="<<memain_.iexcfile<<" ktsche ="<<memain_.ktsche;

  produces<HepMCProduct>();
  edm::LogInfo("Generator|MadGraph")<<"starting event generation ...";
}


MadGraphSource::~MadGraphSource(){
  edm::LogInfo("Generator|MadGraph")<<"event generation done.";
  clear();
  //  rm -f MGinput.dat;
}

void MadGraphSource::clear() {
}


bool MadGraphSource::produce(Event & e) {
  std::auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  

  // skip LHE events - read firstEvent_ times <event>...</event> from the LHE file without returning from produce()
  for(;lhe_event_counter_<firstEvent_; ++lhe_event_counter_){
    call_pyevnt();      // generate one event with Pythia
    call_pyhepc( 1 );
    edm::LogWarning("Generator|MadGraph")<<"skipping LHE event "<<lhe_event_counter_;
  }
    
  call_pyevnt();      // generate one event with Pythia
  call_pyhepc( 1 );

  if(produceEventTreeFile_) eventtree_(); // write out an event tree file

//  HepMC::GenEvent* evt = conv.getGenEventfromHEPEVT();
  HepMC::GenEvent* evt = conv.read_next_event();
  evt->set_signal_process_id(pypars.msti[0]);
  evt->set_event_number(numberEventsInRun() - remainingEvents() - 1);

  if(event() <= maxEventsToPrint_ && (pythiaPylistVerbosity_ || pythiaHepMCVerbosity_)) {
    // Prints PYLIST info
    if(pythiaPylistVerbosity_) {
      call_pylist(pythiaPylistVerbosity_);
    }
    // Prints HepMC event
    if(pythiaHepMCVerbosity_) {
      edm::LogInfo("Generator|MadGraph")<<"Event process = " << pypars.msti[0];
      evt->print();
    }
  }

  if(hepeup_.nup==0){
    edm::LogInfo("Generator|MadGraph")<<"The interface signalled end of Les Houches file. Finishing event processing here.";
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

