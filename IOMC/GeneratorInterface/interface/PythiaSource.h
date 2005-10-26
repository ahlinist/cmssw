#ifndef Pythia_Source_h
#define Pythia_Source_h

/** \class PythiaSource
 *
 * Generates Pythia HepMC events
 *
 * Hector Naves                                  
 *   for the Generator Interface. 26/10/05             
 ***************************************/

#include "FWCore/Framework/interface/InputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "FWCore/Framework/interface/ProductRegistry.h"
#include "FWCore/Framework/interface/BranchDescription.h"
#include "FWCore/EDProduct/interface/EventID.h"
#include <map>
#include <string>
#include "CLHEP/HepMC/GenEvent.h"

class HepMCFileReader;

namespace edm
{
  class PythiaSource : public InputSource {
  public:
    PythiaSource(const ParameterSet &, const InputSourceDescription &  );
   virtual ~PythiaSource();


  private:
   
   virtual std::auto_ptr<EventPrincipal> read();
    void clear();
    
    int remainingEvents_;
    unsigned long numberEventsInRun_;
    unsigned long presentRun_;
    unsigned long nextTime_;
    unsigned long timeBetweenEvents_;
    unsigned long numberEventsInThisRun_;

    // **************
    // Pythia modificators
    int pysubs_msel_;
    int pysubs_msub_;
    int pydatr_mrpy_;
    int pypars_mstp_;
    int pydat2_pmas_;
    // **************

    EventID nextID_;
    
    HepMC::GenEvent  *evt;
    
    std::string filename_;
    BranchDescription branchDesc_;
	
    
    	
  };
} 

#endif
