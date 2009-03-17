/*
 *  $Date: 2008/11/20 13:58:59 $
 *  $Revision: 1.7 $
 *  
 *  Filip Moorgat & Hector Naves 
 *  26/10/05
 * 
 *  Patrick Janot : added the PYTHIA card reading
 *
 *  Sergei SLabospitsky : added Comphep reading tools 
 *
 *  Hector Naves : added MCDB Interface (25/10/06)
 * 
 *  Sandro Fonseca: Fix bug in HLT using ExternalInputSource(14/11/08) 
 *
 *  Sandro Fonseca de Souza: created old parameters in new format (19/11/08)
 * 
 *  Sandro Fonseca de Souza: testing MCDB (20/11/08)
 
 *  Sandro Fonseca de Souza: Implementation of TAUOLA (03/16/09)

 */

#include "FWCore/Sources/interface/ExternalInputSource.h"
#include "GeneratorInterface/ComphepInterface/interface/ComphepSource.h"
#include "GeneratorInterface/ComphepInterface/interface/PYR.h"
//new
//#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RandFlat.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"



#include <sstream>
#include <string>
#include <iostream>
#include "time.h"

// Generator modifications
// ***********************
#include "HepMC/PythiaWrapper6_2.h"
#include "HepMC/IO_HEPEVT.h"
#include "GeneratorInterface/ComphepInterface/interface/ComphepWrapper.h"
#include "GeneratorInterface/CommonInterface/interface/PythiaCMS.h"
#include "GeneratorInterface/CommonInterface/interface/Txgive.h"
// MCDB Interface
#include "GeneratorInterface/ComphepInterface/interface/MCDBInterface.h"


using namespace edm; 
using namespace std;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
HepMC::IO_HEPEVT conv;

//Extern function in comphep.f
#define MGOPEN mgopen_
extern "C" {
    void mgopen_(const char *fname, int len);
 }

#define EXTPROCESS extprocess_
extern "C" {
    void extprocess_(const char *process, int lenprocess);
 }

#define CHFIRSTEVT chfirstevt_
extern "C" {
    void chfirstevt_(int&);
 }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//used for defaults
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;
  std::ifstream file;
  std::ofstream ofile;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ComphepSource::ComphepSource( const ParameterSet & pset,InputSourceDescription const& desc ) 
               :ExternalInputSource (pset, desc,false), evt(0),
            
                pythiaPylistVerbosity_ (pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0)),
                pythiaHepMCVerbosity_ (pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false)),
	        maxEventsToPrint_ (pset.getUntrackedParameter<int>("maxEventsToPrint",1)),
	        getInputFromMCDB_ (pset.getUntrackedParameter<bool>("getInputFromMCDB",false)),
		MCDBArticleID_ (pset.getParameter<int>("MCDBArticleID")),              		
                CHfile_ (fileNames()[0]),
                process_(pset.getUntrackedParameter<string>("process","")),
                CompHEPFirstEvent_ (pset.getParameter<int>("CompHEPFirstEvent")),
                useExternalGenerators_(false),
                useTauola_(false),
                useTauolaPolarization_(false)



 {
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  


//Introduction
 cout <<"############################################################################################################################################"<< endl
 << "####################### Using the COMPHEPInterface in CMSSW ################################################################################"<< endl
      <<"############################################################################################################################################"<< endl
      <<"## CompHEP: a package for evaluation of Feynman diagrams, integration over multi-particle phase space and event generation                ##"<< endl
      <<"## (supported in part by RFBR grants).                                                                                                    ##"<< endl
      <<"##                           CompHEP Collaboration (2008):                                                                                ##"<< endl
      <<"## E.Boos, V.Bunichev, M.Dubinin, L.Dudko, V.Edneral, V.Ilyin, A.Kryukov, V.Savrin-SINP MSU, Moscow, Russia.A.Semenov-JINR, Dubna, Russia.##"<< endl
      <<"## A.Sherstnev - SINP MSU, Moscow, Russia and University of Cambridge, UK.                                                                ##"<< endl
      <<"##                 More information in :http://comphep.sinp.msu.ru/                                                                       ##"<< endl
      <<"############################################################################################################################################"<< endl
      <<"############################################################################################################################################"<< endl;

  
 
  cout << "----------------------------------------------" << endl;
  cout << "Initialization ComphepSource....." << endl;
  cout << "----------------------------------------------" << endl;
  
 
  //******
  // Interface with the LCG MCDB
  //
    if (getInputFromMCDB_) {
    cout << "Using MCDB in ComphepSource....." << endl;

     CHFile_ = pset.getUntrackedParameter<string>("ComphepInputFile"),
      mcdbGetInputFile(CHFile_, MCDBArticleID_);
   }
 

  // strip the input file name
  if ( CHfile_.find("file:") || CHfile_.find("rfio:")){
    CHfile_.erase(0,5);
  }   
 //
  file.open(CHfile_.c_str(),std::ios::in);  
  if(!file){
    edm::LogError("GeneratorError|OpenCompHEPError")<< "Error: Cannot open CompHEP input file";
    throw edm::Exception(edm::errors::Configuration,"Open CompHEP FileError")
      <<" Cannot open CompHEP input file, check file name and path.";
  }else{
    edm::LogInfo("Generator|CompHEP")<<"Opened CompHEP file successfully!";
    file.close();
  }
  
  edm::LogInfo("Generator|CompHEP")<< "CompHEP input file is " << CHfile_;
  
  //cout << "----------------------------------------------" << endl;
  cout << "Setting some parameters of the CompHEP File in CompHEPInterface....." << endl;
  cout << "----------------------------------------------" << endl;
  

  //Move the name file from comphep.F 
     const char* myfilename = CHfile_.c_str();
     MGOPEN(myfilename,strlen(myfilename));
     //cout << "----------------------------------------------" << endl;
     cout << "CompHep input file is: " << CHfile_.c_str() << endl;
     //cout << "----------------------------------------------" << endl;
     //external process
     const char* myprocess =  process_.c_str();
     EXTPROCESS(myprocess,strlen(myprocess));
     //cout << "----------------------------------------------" << endl;
     cout << "PROCESS in CompHEPInterface: " << process_.c_str() << endl;
     //cout << "----------------------------------------------" << endl;
    //setting the number of first event
    
     int myfirst =  CompHEPFirstEvent_;
     CHFIRSTEVT(myfirst);
     //cout << "----------------------------------------------" << endl;    
     cout << "Number of first event in CompHEP data file: " << myfirst << endl;
     cout << "----------------------------------------------" << endl;

  
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

 //In the future, we will get the random number seed on each event and tell 
  // pythia to use that new seed 
 //Setting random numer seed
  cout << "----------------------------------------------" << endl;
  cout << "ComphepSource.....Setting Pythia random number seed" << endl;
  cout << "----------------------------------------------" << endl;


 
  edm::Service<RandomNumberGenerator> rng;
  fRandomEngine = &(rng->getEngine());
  randomEngine = fRandomEngine;
  uint32_t seed = rng->mySeed();
  ostringstream sRandomSet;
  sRandomSet <<"MRPY(1)="<<seed;
  call_pygive(sRandomSet.str());


  cout << "----------------------------------------------" << endl;
  cout << "ComphepSource.....Starting Pythia....." << endl;
  cout << "----------------------------------------------" << endl;

  call_pevmain(); 


//##################################################################
// TAUOLA, etc.
//####################################################################
  useExternalGenerators_ = pset.getUntrackedParameter<bool>("UseExternalGenerators",false);
//  useTauola_ = pset.getUntrackedParameter<bool>("UseTauola", false);
//  useTauolaPolarization_ = pset.getUntrackedParameter<bool>("UseTauolaPolarization", false);

  if ( useExternalGenerators_ ) {
 // read External Generator parameters
    ParameterSet ext_gen_params =
       pset.getParameter<ParameterSet>("ExternalGenerators") ;
    vector<string> extGenNames =
       ext_gen_params.getParameter< vector<string> >("parameterSets");
    for (unsigned int ip=0; ip<extGenNames.size(); ++ip )
    {
      string curSet = extGenNames[ip];
      ParameterSet gen_par_set =
         ext_gen_params.getUntrackedParameter< ParameterSet >(curSet);

     cout << "----------------------------------------------" << endl;
     cout << "Read External Generator parameter set "  << endl;
     cout << "----------------------------------------------" << endl;
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

 
  produces<HepMCProduct>();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ComphepSource::~ComphepSource(){
  call_pystat(1);
  //TAUOLA
  if ( useTauola_ ) {
    tauola_.print();
    //call_pretauola(1); // print TAUOLA decay statistics output
  }

  clear(); 


}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ComphepSource::clear() {
 
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool ComphepSource::produce(Event & e) {

    auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  

    //********                                         
    //

    call_pyevnt();      // generate one event with Pythia
    //Using TAUOLA
 if ( useTauola_ ) {
      tauola_.processEvent();
      //call_pretauola(0); // generate tau decays with TAUOLA
    }


    call_pyhepc( 1 );
    



    //    HepMC::GenEvent* evt = conv.getGenEventfromHEPEVT();
    HepMC::GenEvent* evt = conv.read_next_event();
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
    
    
    if(evt)  bare_product->addHepMCData(evt );

    e.put(bare_product);

    return true;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool 
ComphepSource::call_txgive(const std::string& iParm ) 
   {
    //call the fortran routine txgive with a fortran string
     TXGIVE( iParm.c_str(), iParm.length() );  
     return 1;  
   }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool
ComphepSource::call_txgive_init() 
{
   TXGIVE_INIT();
   return 1;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
