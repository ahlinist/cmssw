#ifndef Comphep_Source_h
#define Comphep_Source_h

/** \class ComphepSource
 *
 * Generates Pythia HepMC events
 *
 * Hector Naves                                  
 *   for the Generator Interface. 26/10/05 
 * Patrick Janot
 *   read all possible cards for Pythia Setup. 26/02/06
 *   ( port from FAMOS )
 * Serge Slabospitsky 11/09/06
 *   read all possible cards for CompHEP Setup

 * Sandro Fonseca de Souza :Implementation of TAUOLA (03/16/09)

 ***************************************/

#define PYCOMP pycomp_

#include "FWCore/Sources/interface/ExternalInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <map>
#include <string>
#include "HepMC/GenEvent.h"
#include "GeneratorInterface/CommonInterface/interface/TauolaInterface.h"

namespace CLHEP
{
  class RandFlat ;
  class HepRandomEngine;
}

namespace edm
{
  class ComphepSource : public ExternalInputSource { 
  public:

    /// Constructor
    ComphepSource(const ParameterSet &, const InputSourceDescription &);
    /// Destructor
    virtual ~ComphepSource();


  private:

    /// Interface to the PYGIVE/TXGIVE pythia routine, with add'l protections
    bool call_pygive(const std::string& iParm );
    bool call_txgive(const std::string& iParm );
    bool call_txgive_init();

  private:
    
    virtual bool produce(Event & e);
    void clear();
    
    HepMC::GenEvent  *evt;
    
    /// Pythia PYLIST Verbosity flag
    unsigned int pythiaPylistVerbosity_;
    /// HepMC verbosity flag
    bool pythiaHepMCVerbosity_;
    /// Events to print if verbosity
    unsigned int maxEventsToPrint_;    
    /// MCDB input file
    std::string CHFile_;
    /// Get input file from LCG MCDB
    bool getInputFromMCDB_;
    /// Sets the MCDB Article ID (0 if none)
    int MCDBArticleID_;

    ///  input file
      std::string CHfile_;
      std::string process_ ;
      int CompHEPFirstEvent_;

 // external generators (tauola,...)
    bool useExternalGenerators_ ;
    bool useTauola_ ;
    bool useTauolaPolarization_ ;
    TauolaInterface tauola_ ;
    
    // for single particle generation in pythia
    int    particleID;
    bool   doubleParticle;
    double ptmin, ptmax;
    double etamin, etamax;
    double phimin, phimax;
    
    CLHEP::HepRandomEngine* fRandomEngine;
    CLHEP::RandFlat*        fRandomGenerator; 

  };
} 

#endif
