#ifndef MadGraph_Producer_h
#define MadGraph_Producer_h
/** \class MadGraphProducer
 *
 * A modified version of the PythiaProducer that generates 
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
 * Carsten Hof  : add saving of flavours and fractional momenta of ingoing partons in pdfinfo (06/02/2008)
 ***************************************/
#define PYCOMP pycomp_
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <map>
#include <string>
#include "HepMC/GenEvent.h"
#include "HepMC/PdfInfo.h"

namespace CLHEP {
class HepRandomEngine;
}

namespace edm
{
  class MadGraphProducer : public EDProducer {
  public:
    /// Constructor
    MadGraphProducer(const ParameterSet & );
    /// Destructor
    virtual ~MadGraphProducer();
  private:
    /// Interface to the PYGIVE pythia routine, with add'l protections
    bool call_pygive(const std::string& iParm );
    virtual void produce(Event & e, const EventSetup& es);
    void clear();
  private:
    HepMC::GenEvent  *evt;
    HepMC::PdfInfo* pdf_info;
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
    // Flag to allow reading a simple LHE file using the minimal Les Houches functionality (ignoring reading parameters from the header)
   bool minimalLH_;
   unsigned int eventNumber_;
   CLHEP::HepRandomEngine* fRandomEngine;
  };
} 
#endif
