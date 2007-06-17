#ifndef MadGraph_Source_h
#define MadGraph_Source_h
/** \class MadGraphSource
 *
 * A modified version of the PythiaSource that generates 
 * events by reading in a file produced with MadGraph/MadEvent
 * and shower them with Pythia. Includes Matrix Element - Parton Shower matching
 * 
 * July 2006
 * Maria Hansen, University of Bristol
 * Hector Naves : added the MCDB Interface (25/10/06)
 * Dorian Kcira : change to new Les Houches format.
 * Dorian Kcira : add automatic end of event processing when hitting
 *                the end of the lhe file (29/01/07)
 * Dorian Kcira : added ME-PS matching (22/05/2007)
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
    /// Get input file from LCG MCDB 
    bool getInputFromMCDB_;
    /// Sets the MCDB Article ID (0 if none)
    int MCDBArticleID_;
    unsigned int firstEvent_;
    unsigned int lhe_event_counter_;
    // Matching & MEMAIN
    double MEMAIN_etaclmax;
    double MEMAIN_qcut;
    unsigned int MEMAIN_iexcfile; // 1 = perform exclusive matching
    bool produceEventTreeFile_;
  };
} 
#endif
