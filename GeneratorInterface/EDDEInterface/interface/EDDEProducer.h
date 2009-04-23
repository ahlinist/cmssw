#ifndef EDDEProducer_EDDEProducer_h
#define EDDEProducer_EDDEProducer_h

/** \class EDDESource
 *
 * Generates EDDE + Pythia ==> HepMC events
 *
 * Kirill Datsko
 *   for the Generator Interface. 04/07/07
 * Patrick Janot
 *   read all possible cards for Pythia Setup. 26/02/06
 *   ( port from FAMOS )
 * Roman Ryutin and Andrey Sobol 04/07/07
 *   read all possible cards for EDDE Setup
 ***************************************/
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/GeneratedInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include <map>
//#include <string>
#include "HepMC/GenEvent.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RandFlat.h"

//class Run;

namespace edm
{
  class EDDEProducer : public EDProducer{
  public:
    EDDEProducer(const ParameterSet &);
    virtual ~EDDEProducer();

    //    void endRun( Run& r);
  private:

    /// Interface to the PYGIVE/TXGIVE pythia routine, with add'l protections
    bool call_pygive(const std::string& iParm );
    bool call_txgive(const std::string& iParm );
    bool call_txgive_init();

  private:

    virtual void produce(Event & e, const EventSetup& es);
    //    void clear() {};
    
    HepMC::GenEvent  *evt;
    
    /// Pythia PYLIST Verbosity flag
    unsigned int pythiaPylistVerbosity_;
    /// HepMC verbosity flag
    bool pythiaHepMCVerbosity_;
    /// Events to print if verbosity
    unsigned int maxEventsToPrint_;    
    
    CLHEP::HepRandomEngine* fRandomEngine;
    CLHEP::RandFlat*        fRandomGenerator; 

    unsigned int EventNum;
   
  };
}

#endif
