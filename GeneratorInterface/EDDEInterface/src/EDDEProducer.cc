#include "GeneratorInterface/EDDEInterface/interface/EDDEWrapper.h"
#include "GeneratorInterface/EDDEInterface/interface/EDDEProducer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "HepMC/IO_HEPEVT.h"
#include "HepMC/PythiaWrapper6_2.h"
#include "HepMC/HEPEVT_Wrapper.h" //=
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Event.h"

// SimDataFormats headers
//#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"


#include <memory>

#define PYGIVE pygive_
extern "C" {
  void PYGIVE(const char*,int length);
}

#define TXGIVE txgive_
extern "C" {
  void TXGIVE(const char*,int length);
}

#define TXGIVE_INIT txgive_init_
extern "C" {
  void TXGIVE_INIT();
}

//using namespace edm;
//using namespace std;

edm::EDDEProducer::EDDEProducer( const ParameterSet & pset ) :
   EDProducer(), evt(0), 
  pythiaPylistVerbosity_ (pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0)),
  pythiaHepMCVerbosity_ (pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false)),
  maxEventsToPrint_ (pset.getUntrackedParameter<int>("maxEventsToPrint",1)),
  EventNum(0)
{

  ////////////////////////
  // Set PYTHIA parameters in a single ParameterSet
  {
    ParameterSet pythia_params = 
      pset.getParameter<ParameterSet>("PythiaParameters") ;
  
    // The parameter sets to be read (default, min bias, user ...) in the
    // proper order.
    std::vector<std::string> setNames = 
      pythia_params.getParameter<std::vector<std::string> >("parameterSets");
  
    // Loop over the sets
    for ( unsigned i=0; i<setNames.size(); ++i ) {
    
      std::string mySet = setNames[i];
    
      // Read the PYTHIA parameters for each set of parameters
      std::vector<std::string> pars = 
	pythia_params.getParameter<std::vector<std::string> >(mySet);
    
      if (mySet != "CSAParameters"){
	LogInfo ("EDDE") << "----------------------------------------------\n"
			 << "Read PYTHIA parameter set \n"
			 << "----------------------------------------------\n";
    
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
      }else if(mySet == "CSAParameters"){   

   // Read CSA parameter
	/*	  
	pars = pythia_params.getParameter<std::vector<std::string> >("CSAParameters");

	LogInfo ("EDDE") << "----------------------------------------------\n" 
			 << "Reading CSA parameter settings. \n"
			 << "----------------------------------------------\n";

	call_txgive_init();
  
  
	// Loop over all parameters and stop in case of a mistake
	for (std::vector<std::string>::const_iterator 
	       itPar = pars.begin(); itPar != pars.end(); ++itPar) {
	  call_txgive(*itPar); 
     
	} 
	*/
      }
    }// for ( unsigned i=0; i<setNames.size(); ++i ) 

  }
  //Finish reading Pythia parameters

  //In the future, we will get the random number seed on each event and tell 
  // pythia to use that new seed
    LogInfo ( "EDDE" ) << "----------------------------------------------\n"
		       << "Setting Pythia random number seed \n"
		       << "----------------------------------------------\n";
  edm::Service<RandomNumberGenerator> rng;
  uint32_t seed = rng->mySeed();
  std::ostringstream sRandomSet;
  sRandomSet <<"MRPY(1)="<<seed;
  call_pygive(sRandomSet.str());

  // EDDE parameters

  edm::ParameterSet edde_par = pset.getParameter<edm::ParameterSet>("EDDE");

  KI_TXPAR.ipars[1 - 1]  = edde_par.getParameter<int>("ProcessNum");
  KI_TXPAR.ipars[4 - 1]  = edde_par.getParameter<int>("CodPart400");
  KI_TXPAR.ipars[5 - 1]  = edde_par.getParameter<int>("JPCofSystem");
  KI_TXPAR.ipars[6 - 1]  = edde_par.getParameter<int>("NRSNum");

  KI_TXPAR.rpars[1 - 1]  = (float)edde_par.getParameter<double>("CMEnergy");
  KI_TXPAR.rpars[2 - 1]  = (float)edde_par.getParameter<double>("LowerEt");
  KI_TXPAR.rpars[3 - 1]  = (float)edde_par.getParameter<double>("MassX");
  KI_TXPAR.rpars[5 - 1]  = (float)edde_par.getParameter<double>("XIMixing");
  KI_TXPAR.rpars[6 - 1]  = (float)edde_par.getParameter<double>("GAM");
  KI_TXPAR.rpars[7 - 1]  = (float)edde_par.getParameter<double>("MH");
  KI_TXPAR.rpars[8 - 1]  = (float)edde_par.getParameter<double>("MR");
  KI_TXPAR.rpars[9 - 1]  = (float)edde_par.getParameter<double>("PseudoRapRange");
  KI_TXPAR.rpars[10 - 1] = (float)edde_par.getParameter<double>("MQinQQ");


  call_eddeini();

  produces<HepMCProduct>();
}

edm::EDDEProducer::~EDDEProducer() {
  call_pystat(1);
  //  clear();
}

void edm::EDDEProducer::produce( Event & e, const EventSetup& es) {

  if ( EventNum < maxEventsToPrint_ ) call_pylist(pythiaPylistVerbosity_);
  EventNum++;

  std::auto_ptr<HepMCProduct> edde_product(new HepMCProduct());

  // generates one new event

  call_eddeeve();

  // gets event
  HepMC::IO_HEPEVT conv;
  //  HepMC::GenEvent* evt = conv.read_next_event();
  evt = conv.read_next_event();

  // writes event in to edm::Event
  if ( evt )  edde_product->addHepMCData( evt );

  e.put( edde_product );

  return; //=
}

bool edm::EDDEProducer::call_pygive(const std::string& iParm )
{
  int numWarn = pydat1.mstu[26]; //# warnings
  int numErr = pydat1.mstu[22];// # errors
  //call the fortran routine pygive with a fortran string
  PYGIVE( iParm.c_str(), iParm.length() );
  //if an error or warning happens it is problem
  return pydat1.mstu[26] == numWarn && pydat1.mstu[22] == numErr;
}
//------------
bool edm::EDDEProducer::call_txgive(const std::string& iParm )
{
  //  TXGIVE( iParm.c_str(), iParm.length() );
  return 1;
}

bool edm::EDDEProducer::call_txgive_init()
{
  //  TXGIVE_INIT();
  LogInfo ("EDDE") << "  Setting CSA defaults.   \n";
  return 1;
}
