/** MadGraphProducer
 *
 * A modified version of the PythiaProducer by 
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
 * Dorian Kcira : added reading of <slha> xml tags in the header (05/02/2008)
 * Dorian Kcira : added flag to allow reading of generic LH files without MadGraph specifics (06/02/2008)
 * Christophe Saout : read LHE data from EDM products and make MadGraphProducer an EDFilter
 *                    the EDM products can be filled from a LHE file via a dedicated LHESource
 ***************************************/
#include "GeneratorInterface/MadGraphInterface/interface/MadGraphProducer.h"
#include "GeneratorInterface/MadGraphInterface/interface/PYR.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHECommonBlocks.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream> 
#include <cstring>
#include <cstdio>
#include <string>

#include "time.h"

// Generator modifications
#include "HepMC/PythiaWrapper6_2.h"
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

#define TXGIVE txgive_
extern "C" {
  void TXGIVE(const char*,int length);
}

#define TXGIVE_INIT txgive_init_
extern "C" {
  void TXGIVE_INIT();
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

/*
      LOGICAL minimalLH,externalLH,validLH
      common /SOURCEPRS/minimalLH,externalLH,validLH
*/
extern "C"{
  extern struct SOURCEPRS {
    int minimalLH;
    int externalLH;
    int validLH;
  } sourceprs_;
}

// init LHNIN Fortran unit with file "fileName" to read.
extern "C" {
  void mgopen_(const char *fname, int len);
  void mgclos_();
}

// turn ME2pythia.f aborts into an exception, shutting down EDM cleanly
extern "C"{
  void pystop_(int *mcod)
  {
    throw cms::Exception("Generator|MadGraphProducer")
          << "PYSTOP called with code: " << *mcod << std::endl;
  }
}

//used for defaults - change these to defines? TODO
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;

using namespace edm;
using namespace std;

MadGraphProducer::MadGraphProducer( const ParameterSet & pset) :
 EDFilter(), evt(0),
 pythiaPylistVerbosity_ (pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0)),
 pythiaHepMCVerbosity_ (pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false)),
 maxEventsToPrint_ (pset.getUntrackedParameter<int>("maxEventsToPrint",0)),
 firstEvent_(pset.getUntrackedParameter<unsigned int>("firstEvent", 0)),
 MEMAIN_etaclmax(pset.getUntrackedParameter<double>("MEMAIN_etaclmax",0.)),
 MEMAIN_qcut(pset.getUntrackedParameter<double>("MEMAIN_qcut",0.)),
 MEMAIN_iexcfile(pset.getUntrackedParameter<unsigned int>("MEMAIN_iexcfile",0)),
 produceEventTreeFile_ (pset.getUntrackedParameter<bool>("produceEventTreeFile",false)),
 minimalLH_(pset.getUntrackedParameter<bool>("minimalLH",false)),
 initialized_(false),
 eventNumber_(firstEvent_),
 pset_(pset),
 useExternalGenerators_(false),
 useTauola_(false),
 useTauolaPolarization_(false)
{
  edm::LogInfo("Generator|MadGraph")<<" initializing MadGraphProducer";
  pdf_info = new HepMC::PdfInfo();

  // check whether using minimal LH
  sourceprs_.minimalLH = minimalLH_; // pass to ME2pythia through common block
  sourceprs_.externalLH = true;
  if(minimalLH_) edm::LogInfo("Generator|MadGraph")<<" ----- Using minimal Les Houches Accord functionality - ignoring MadGraph specifics.";
  
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

  edm::LogInfo("Generator|MadGraph")<<"MadGraphProducer: initializing Pythia.";
  // PYLIST Verbosity Level
  // Valid PYLIST arguments are: 1, 2, 3, 5, 7, 11, 12, 13
  edm::LogInfo("Generator|MadGraph")<<"Pythia PYLIST verbosity level = " << pythiaPylistVerbosity_;
  // HepMC event verbosity Level
  edm::LogInfo("Generator|MadGraph")<<"Pythia HepMC verbosity = " << pythiaHepMCVerbosity_;
  //Max number of events printed on verbosity level 
  edm::LogInfo("Generator|MadGraph")<<"Number of events to be printed = " << maxEventsToPrint_;
  // max nr of events and first event
//edm::LogInfo("Generator|MadGraph")<<"firstEvent / maxEvents = "<<firstEvent_<<" / "<< maxEvents();
  edm::LogInfo("Generator|MadGraph")<<"firstEvent / maxEvents = "<<firstEvent_<<" / 999999999";

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

  // TAUOLA, etc.
  //
  useExternalGenerators_ = pset.getUntrackedParameter<bool>("UseExternalGenerators",false);
  
  //Setting random numer seed
  edm::LogInfo("Generator|MadGraph")<<"----------------------------------------------";
  edm::LogInfo("Generator|MadGraph")<<"Setting Pythia random number seed";
  edm::LogInfo("Generator|MadGraph")<<"----------------------------------------------";
  edm::Service<RandomNumberGenerator> rng;
  randomEngine = fRandomEngine = &(rng->getEngine());
  uint32_t seed = rng->mySeed();
  std::ostringstream sRandomSet;
  sRandomSet <<"MRPY(1)="<<seed;
  call_pygive(sRandomSet.str());
  produces<HepMCProduct>();
  edm::LogInfo("Generator|MadGraph")<<"starting event generation ...";
}


MadGraphProducer::~MadGraphProducer(){
  edm::LogInfo("Generator|MadGraph")<<"event generation done.";
  delete pdf_info;
  clear();
  //  rm -f MGinput.dat;
}

void MadGraphProducer::clear() {
}

void MadGraphProducer::init() {
  initialized_ = true;

  // Call pythia initialisation with user defined upinit subroutine
  call_pyinit( "USER", "", "", 0.);

  if ( useExternalGenerators_ ) {
    // read External Generator parameters
    ParameterSet ext_gen_params =
       pset_.getParameter<ParameterSet>("ExternalGenerators") ;
    vector<string> extGenNames =
       ext_gen_params.getParameter< vector<string> >("parameterSets");
    for (unsigned int ip=0; ip<extGenNames.size(); ++ip )
    {
      string curSet = extGenNames[ip];
      ParameterSet gen_par_set =
         ext_gen_params.getUntrackedParameter< ParameterSet >(curSet);
/*
     cout << "----------------------------------------------" << endl;
     cout << "Read External Generator parameter set "  << endl;
     cout << "----------------------------------------------" << endl;
*/
     if ( curSet == "Tauola" )
     {
        useTauola_ = true;
        if ( useTauola_ ) {
           cout << "--> use TAUOLA" << endl;
        } 
	useTauolaPolarization_ = gen_par_set.getParameter<bool>("UseTauolaPolarization");
        if ( useTauolaPolarization_ ) 
	{
           cout << "(Polarization effects enabled)" << endl;
           tauola_.enablePolarizationEffects();
        } 
	else 
	{
           cout << "(Polarization effects disabled)" << endl;
           tauola_.disablePolarizationEffects();
        }
	vector<string> cards = gen_par_set.getParameter< vector<string> >("InputCards");
	cout << "----------------------------------------------" << endl;
        cout << "Initializing Tauola" << endl;
        for( vector<string>::const_iterator
                itPar = cards.begin(); itPar != cards.end(); ++itPar )
        {
           // call_txgive(*itPar);
	   TXGIVE( (*itPar).c_str(), (*itPar).length() );
	   cout << "     " <<  (*itPar).c_str() << endl;
        }
        tauola_.initialize();
        //call_pretauola(-1); // initialize TAUOLA package for tau decays
     }
    }
    // cout << "----------------------------------------------" << endl;
  }


  edm::LogInfo("Generator|MadGraph")<<"MEMAIN after ME2pythia initialization - etcjet ="<<memain_.etcjet<<" rclmax ="<<memain_.rclmax<<" etaclmax ="<<memain_.etaclmax<<" qcut ="<<memain_.qcut<<" clfact ="<<memain_.clfact<<" maxjets ="<<memain_.maxjets<<" minjets ="<<memain_.minjets<<" iexcfile ="<<memain_.iexcfile<<" ktsche ="<<memain_.ktsche;
}

bool MadGraphProducer::beginRun(Run& run, const EventSetup& es)
{
  edm::Handle<LHERunInfoProduct> product;
  run.getByLabel("source", product);
  lhef::CommonBlocks::fillHEPRUP(&product->heprup());

  if (!minimalLH_) {
    const char *fname = std::tmpnam(NULL);
    std::ofstream file(fname, std::fstream::out | std::fstream::trunc);
    std::copy(product->begin(), product->init(),
              std::ostream_iterator<std::string>(file));
    file.close();
    mgopen_(fname, std::strlen(fname));
    std::remove(fname);
  }

  return true;
}

bool MadGraphProducer::endRun(Run& run, const EventSetup& es)
{
  call_pystat(1);
  if ( useTauola_ ) {
    tauola_.print();
    //call_pretauola(1); // print TAUOLA decay statistics output
  }

  mgclos_();
  initialized_ = false;
  return true;
}

bool MadGraphProducer::filter(Event & e, const EventSetup& es) 
{
  edm::Handle<LHEEventProduct> product;
  e.getByLabel("source", product);
  lhef::CommonBlocks::fillHEPEUP(&product->hepeup());
  sourceprs_.validLH = true;
  if (!initialized_)
    init();

  std::auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  

  call_pyevnt();      // generate one event with Pythia

  if (hepeup_.nup == 0 || pypars.msti[0] == 1) {
    // the event got rejected by the matching and we have to filter
    edm::LogInfo("Generator|MadGraph") << "Event was skipped.";
    e.put(bare_product); // put an empty product, CMSSW filter will take care
    return false;
  }

  if ( useTauola_ ) {
    tauola_.processEvent();
    //call_pretauola(0); // generate tau decays with TAUOLA
  }

  call_pyhepc( 1 );

  if(produceEventTreeFile_) eventtree_(); // write out an event tree file

  HepMC::IO_HEPEVT conv;
  HepMC::GenEvent* evt = conv.read_next_event();
  evt->set_signal_process_id(pypars.msti[0]);
  evt->set_event_number(eventNumber_);
  ++eventNumber_;

  // store PDF information
  int id_1 = pypars.msti[14];
  int id_2 = pypars.msti[15];
  if (id_1 == 21) id_1 = 0;
  if (id_2 == 21) id_2 = 0;
  pdf_info->set_id1(id_1);
  pdf_info->set_id2(id_2);
  pdf_info->set_x1(pypars.pari[32]);
  pdf_info->set_x2(pypars.pari[33]);
  pdf_info->set_scalePDF(pypars.pari[20]);
  pdf_info->set_pdf1(pypars.pari[28]);
  pdf_info->set_pdf2(pypars.pari[29]);
  evt->set_pdf_info( *pdf_info);

  if(e.id().event() <= maxEventsToPrint_ && (pythiaPylistVerbosity_ || pythiaHepMCVerbosity_)) {
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

  bare_product->addHepMCData(evt );
  e.put(bare_product);

  return true;
}

bool MadGraphProducer::call_pygive(const std::string& iParm ) {
  int numWarn = pydat1.mstu[26]; //# warnings
  int numErr = pydat1.mstu[22];// # errors
  //call the fortran routine pygive with a fortran string
  PYGIVE( iParm.c_str(), iParm.length() );
  //  PYGIVE( iParm );
  //if an error or warning happens it is problem
  return pydat1.mstu[26] == numWarn && pydat1.mstu[22] == numErr;
}
