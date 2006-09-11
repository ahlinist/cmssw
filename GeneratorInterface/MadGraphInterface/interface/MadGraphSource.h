#ifndef MadGraph_Source_h
#define MadGraph_Source_h

/** \class MadGraphSource
 *
 * A modified version of the PythiaSource that generates 
 * events by reading in a file produced with MadGraph/MadEvent
 * and shower them with Pythia.
 * 
 * 
 * July 2006
 * Maria Hansen, University of Bristol
 ***************************************/

#define PYCOMP pycomp_

#include "FWCore/Framework/interface/GeneratedInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <map>
#include <string>
#include "CLHEP/HepMC/GenEvent.h"

namespace edm
{
  class MadGraphSource : public GeneratedInputSource {
  public:

    /// Constructor
    MadGraphSource(const ParameterSet &, const InputSourceDescription &);
    /// Destructor
    virtual ~MadGraphSource();
 
  private:
    /// Interface to the PYGIVE pythia routine, with add'l protections
    bool call_pygive(const std::string& iParm );
    virtual bool produce(Event & e);
    void clear();
    
  private:

    HepMC::GenEvent  *evt;
    /// Pythia PYLIST Verbosity flag
    unsigned int pythiaPylistVerbosity_;
    /// HepMC verbosity flag
    bool pythiaHepMCVerbosity_;
    /// Events to print if verbosity
    unsigned int maxEventsToPrint_;    
    /// Name of file which contains the unweighted MadGraph events
    std::string MGfile_;
  };
} 

#endif
