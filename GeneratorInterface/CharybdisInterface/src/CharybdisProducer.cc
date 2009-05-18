/*
 *  $Date: 2007/10/31 12:43:59 $
 *  $Revision: 1.3 $
 *  
 *  Filip Moorgat & Hector Naves 
 *  26/10/05
 * 
 *  Patrick Janot : added the PYTHIA card reading
 *
 *  Serge SLabospitsky : added Charybdis reading tools 
 *
 *  Halil Gamsizkan : Modifications for CMSSW_3_1_X migration
 *
 */

#include "GeneratorInterface/CharybdisInterface/interface/CharybdisProducer.h"
#include "GeneratorInterface/Pythia6Interface/interface/Pythia6Declarations.h"
#include "GeneratorInterface/CharybdisInterface/interface/PYR.h"
//#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
//#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
//#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
//#include "CLHEP/Random/JamesRandom.h"
//#include "CLHEP/Random/RandFlat.h"
//#include "CLHEP/Random/RandomEngine.h"


#include <iostream>
#include "time.h"

using namespace edm; 
using namespace std;

// Generator modifications
// ***********************
#include "HepMC/PythiaWrapper6_2.h"
#include "HepMC/IO_HEPEVT.h"

HepMC::IO_HEPEVT conv;
// ***********************

//used for defaults
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;


CharybdisProducer::CharybdisProducer( const ParameterSet & pset ):
   EDProducer(), evt(0), eventNumber_(0),
   pythiaPylistVerbosity_ (pset.getUntrackedParameter<unsigned int>("pythiaPylistVerbosity",0)),
   maxEventsToPrint_ (pset.getUntrackedParameter<unsigned int>("maxEventsToPrint",0)),
   pythiaHepMCVerbosity_ (pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false))
{
  
   myName = "[CharybdisProducer]";
   // PYLIST Verbosity Level
   // Valid PYLIST arguments are: 1, 2, 3, 5, 7, 11, 12, 13
   cout << myName << " Pythia PYLIST verbosity level = " << pythiaPylistVerbosity_ << endl;
   
   // HepMC event verbosity Level
   cout << myName << " Pythia HepMC verbosity = " << pythiaHepMCVerbosity_ << endl; 

   //Max number of events printed on verbosity level 
   cout << myName << " Number of events to be printed = " << maxEventsToPrint_ << endl;

   // Set the Charybdis physics parameter defaults
   comEnergy_ = 10000.0;
   MPLNCK_ = 1000.0;
   MINMSS_ = 5000.0;
   MAXMASS_ = -1;
   THWMAX_ = 1000.0;
   RMMINM_ = 1000.0;
   Nsearch_ = 10000;
   MSSDEF_ = 2;
   TOTDIM_ = 7;
   NBODY_ = 2;
   MSSDEC_ = 3;
   GTSCA_ = false; 
   TIMVAR_ = false;
   GRYBDY_ = false;
   KINCUT_ = false;
   YRCSEC_ = false;
   RMBOIL_ = false;
   
   // Read the Charybdis physics parameter defaults from configuration
   if (pset.exists("comEnergy")) comEnergy_ = pset.getParameter<double>("comEnergy");
   if (pset.exists("MPLNCK")) 	MPLNCK_ = pset.getParameter<double>("MPLNCK");
   if (pset.exists("MINMSS")) 	MINMSS_ = pset.getParameter<double>("MINMSS");
   if (pset.exists("MAXMASS")) 	MAXMASS_ = pset.getParameter<double>("MAXMASS");
   if (pset.exists("THWMAX")) 	THWMAX_ = pset.getParameter<double>("THWMAX");
   if (pset.exists("RMMINM")) 	RMMINM_ = pset.getParameter<double>("RMMINM");
   if (pset.exists("Nsearch")) 	Nsearch_ = pset.getParameter<int>("Nsearch");
   if (pset.exists("MSSDEF")) 	MSSDEF_ = pset.getParameter<int>("MSSDEF");
   if (pset.exists("TOTDIM")) 	TOTDIM_ = pset.getParameter<int>("TOTDIM");
   if (pset.exists("NBODY")) 	NBODY_ = pset.getParameter<int>("NBODY");
   if (pset.exists("MSSDEC")) 	MSSDEC_ = pset.getParameter<int>("MSSDEC");
   if (pset.exists("GTSCA")) 	GTSCA_ = pset.getParameter<bool>("GTSCA");
   if (pset.exists("TIMVAR")) 	TIMVAR_ = pset.getParameter<bool>("TIMVAR");
   if (pset.exists("GRYBDY")) 	GRYBDY_ = pset.getParameter<bool>("GRYBDY");
   if (pset.exists("KINCUT")) 	KINCUT_ = pset.getParameter<bool>("KINCUT");
   if (pset.exists("YRCSEC")) 	YRCSEC_ = pset.getParameter<bool>("YRCSEC");
   if (pset.exists("RMBOIL")) 	RMBOIL_ = pset.getParameter<bool>("RMBOIL");
  
   bool result;
   chb_charybdis_init();
   result=chb_pythia_init(pset);  

   produces<HepMCProduct>();
   cout << myName << " Rolling .." << endl;
}

CharybdisProducer::~CharybdisProducer(){
  cout << myName << " Event generation done. " << endl;
  call_pystat(1);
  clear(); 
}

void 
CharybdisProducer::clear() {
 
}


void 
CharybdisProducer::produce(Event & e, const EventSetup& es) {

   auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  
   call_pyevnt();      	// generate one event with Pythia
   call_pyhepc( 1 );
   ++eventNumber_; 	// Event numbers are 1 based 
   HepMC::GenEvent* evt = conv.read_next_event();
   evt->set_event_number(eventNumber_);
   evt->set_signal_process_id(pypars.msti[0]);      // type of the process
   evt->set_event_scale(pypars.pari[16]);           // Q^2

   //******** Verbosity ********
    
   if(eventNumber_ <= maxEventsToPrint_ &&
      (pythiaPylistVerbosity_ || pythiaHepMCVerbosity_)) {

      // Prints PYLIST info
      if(pythiaPylistVerbosity_) {
	 call_pylist(pythiaPylistVerbosity_);
      }
	 
      // Prints HepMC event
      if(pythiaHepMCVerbosity_) {
	 cout << myName << " Event process = " << pypars.msti[0] << endl 
	 << "----------------------" << endl;
	 evt->print();
      }
   }
    
   if(evt)  bare_product->addHepMCData(evt );
   e.put(bare_product);
}

bool 
CharybdisProducer::chb_pythia_init(const ParameterSet & pset){

   cout << "[CharybdisProducer] Initializing Pythia. " << endl;

   ////////////////////////
   // Set PYTHIA parameters in a single ParameterSet
   ParameterSet pythia_params = pset.getParameter<ParameterSet>("PythiaParameters") ;
  
   // The parameter sets to be read (default, min bias, user ...) in the
   // proper order.
   vector<string> setNames = pythia_params.getParameter<vector<string> >("parameterSets");
  
   // Loop over the sets
   for ( unsigned i=0; i<setNames.size(); ++i ) {
      string mySet = setNames[i];
    
      // Read the PYTHIA parameters for each set of parameters
      vector<string> pars = pythia_params.getParameter<vector<string> >(mySet);
      if (mySet != "CSAParameters"){
	 cout << "----------------------------------------------" << endl;
	 cout << myName << "Read PYTHIA parameter set " << mySet << endl;
	 cout << "----------------------------------------------" << endl;
      
	 // Loop over all parameters and stop in case of mistake
	 for( vector<string>::const_iterator itPar = pars.begin(); itPar != pars.end(); ++itPar ) {
	    static string sRandomValueSetting("MRPY(1)");
	    if( 0 == itPar->compare(0,sRandomValueSetting.size(),sRandomValueSetting) ) {
	       throw edm::Exception(edm::errors::Configuration,"PythiaError")
		  <<" attempted to set random number using pythia command 'MRPY(1)' this is not allowed.\n  Please use the RandomNumberGeneratorService to set the random number seed.";
	    }
	    if( ! gen::call_pygive(*itPar) ) {
	       throw edm::Exception(edm::errors::Configuration,"PythiaError") 
		  <<" pythia did not accept the following \""<<*itPar<<"\"";
	    }
	 }
      }
   }


   cout << "----------------------------------------------" << endl;
   cout << myName << " Setting Pythia random number seed " << endl;
   cout << "----------------------------------------------" << endl;
   edm::Service<RandomNumberGenerator> rng;
   randomEngine = fRandomEngine = &(rng->getEngine());
   uint32_t seed = rng->mySeed();
   ostringstream sRandomSet;
   sRandomSet <<"MRPY(1)="<<seed;
   gen::call_pygive(sRandomSet.str());
   call_pyinit( "USER", "p", "p", comEnergy_ );
   return true; 
}

void 
CharybdisProducer::chb_charybdis_init(){
   // Send the physics parameters to Charybdis
   cout << myName << " Sending physics parameters to Charybdis" << endl;

   //ints
   CHGIVE(140, Nsearch_,0.);
   CHGIVE(141, MSSDEF_, 0.);
   CHGIVE(142, TOTDIM_, 0.);
   CHGIVE(143, NBODY_,  0.);
   CHGIVE(144, MSSDEC_, 0.);

   // bools
   CHGIVE(145, int(GTSCA_), 0.);
   CHGIVE(146, int(TIMVAR_),0.);
   CHGIVE(147, int(GRYBDY_),0.);
   CHGIVE(148, int(KINCUT_),0.);
   CHGIVE(149, int(YRCSEC_),0.);
   CHGIVE(150, int(RMBOIL_),0.);

   // doubles
   CHGIVE(-140, 0, comEnergy_/2.);
   CHGIVE(-141, 0, MPLNCK_);
   CHGIVE(-142, 0, MINMSS_);
   CHGIVE(-143, 0, MAXMASS_);
   CHGIVE(-144, 0, THWMAX_);
   CHGIVE(-145, 0, RMMINM_);
}

// bool 
// CharybdisProducer::call_pygive(const std::string& iParm ) {
// 
//   int numWarn = pydat1.mstu[26]; //# warnings
//   int numErr = pydat1.mstu[22];// # errors
//   
//    //call the fortran routine pygive with a fortran string
//   gen::pygive_( iParm.c_str(), iParm.length() );  
// 
//    //if an error or warning happens it is problem
//   return pydat1.mstu[26] == numWarn && pydat1.mstu[22] == numErr;   
// }
