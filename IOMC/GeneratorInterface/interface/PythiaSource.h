#ifndef Pythia_Source_h
#define Pythia_Source_h

/** \class PythiaSource
 *
 * Generates Pythia HepMC events
 *
 * Hector Naves                                  
 *   for the Generator Interface. 26/10/05 
 * Patrick Janot
 *   read all possible cards for Pythia Setup. 26/02/06
 *   ( port from FAMOS )
 ***************************************/

#define PYCOMP pycomp_

#include "FWCore/Framework/interface/GeneratedInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <map>
#include <string>
#include "CLHEP/HepMC/GenEvent.h"

namespace edm
{
  class PythiaSource : public GeneratedInputSource {
  public:

    /// Constructor
    PythiaSource(const ParameterSet &, const InputSourceDescription &);
    /// Destructor
    virtual ~PythiaSource();


  private:

    /// Interface to the PYGIVE pythia routine, with add'l protections
    bool call_pygive(const std::string& iParm );

  private:
    
    virtual bool produce(Event & e);
    void clear();
    
    HepMC::GenEvent  *evt;
    
    /// Verbosity flag
    bool pythiaVerbosity_;    	
    
  };
} 

#endif
