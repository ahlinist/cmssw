#ifndef Pythia_Source_h
#define Pythia_Source_h

/** \class PythiaSource
 *
 * Generates Pythia HepMC events
 *
 * Hector Naves                                  
 *   for the Generator Interface. 26/10/05             
 ***************************************/

#include "FWCore/Framework/interface/GeneratedInputSource.h"
#include <map>
#include <string>
#include "CLHEP/HepMC/GenEvent.h"

class HepMCFileReader;

namespace edm
{
  class PythiaSource : public GeneratedInputSource {
  public:
    PythiaSource(const ParameterSet &, const InputSourceDescription &);
   virtual ~PythiaSource();


  private:
   
   virtual bool produce(Event & e);
    void clear();
    
    // **************
    // Pythia modificators
    int pysubs_msel_;
    int pysubs_msub_;
    int pydatr_mrpy_;
    int pypars_mstp_;
    int pydat2_pmas_;
    // **************

    HepMC::GenEvent  *evt;
    
    	
  };
} 

#endif
