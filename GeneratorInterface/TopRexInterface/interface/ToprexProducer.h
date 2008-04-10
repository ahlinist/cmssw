#ifndef Toprex_Producer_h
#define Toperx_Producer_h

/** \class ToprexProducer
 *
 * Generates TopReX + Pythia ==> HepMC events
 *
 * Hector Naves                                  
 *   for the Generator Interface. 26/10/05 
 * Patrick Janot
 *   read all possible cards for Pythia Setup. 26/02/06
 *   ( port from FAMOS )
 * Serge Slabospitsky 11/09/06
 *   read all possible cards for TopReX Setup
 ***************************************/

#define PYCOMP pycomp_

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <map>
#include <string>
#include "HepMC/GenEvent.h"


namespace CLHEP
{
  class RandFlat ;
  class HepRandomEngine;
}

namespace edm
{
  class ToprexProducer : public EDProducer {
  public:

    /// Constructor
    ToprexProducer(const ParameterSet &);
    /// Destructor
    virtual ~ToprexProducer();


  private:

    /// Interface to the PYGIVE/TXGIVE pythia routine, with add'l protections
    bool call_pygive(const std::string& iParm );
    bool call_txgive(const std::string& iParm );
    bool call_txgive_init();
                                                                                

  private:
    
    virtual void produce(Event & e, const EventSetup& es);
    void clear();
    
    HepMC::GenEvent  *evt;
    
    //  void Nest( HepMC::GenEvent & evt ) ;      //HepMC::GenEvent&) ;

    /// Pythia PYLIST Verbosity flag
    unsigned int pythiaPylistVerbosity_;
    /// HepMC verbosity flag
    bool pythiaHepMCVerbosity_;
    /// Events to print if verbosity
    unsigned int maxEventsToPrint_;    
    
 
    //    // for single particle generation in pythia
    // int    particleID;
    // bool   doubleParticle;
    // double ptmin, ptmax;
    // double etamin, etamax;
    // double phimin, phimax;
    
     CLHEP::HepRandomEngine* fRandomEngine;
     CLHEP::RandFlat*        fRandomGenerator; 

     int eventNumber_;
  };
} 

#endif
