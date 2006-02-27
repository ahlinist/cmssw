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
    PythiaSource(const ParameterSet &, const InputSourceDescription &);
    virtual ~PythiaSource();
    
  private:
    
    virtual bool produce(Event & e);
    void clear();
    
    HepMC::GenEvent  *evt;
    
    bool pythiaVerbosity_;    	
    
    ///Method to initialise Pysubs: msel, mselpd, msub, kfin, ckin.
    void initializePysubs(const ParameterSet &);
    
    ///Method to initialise Pypars: mstp, parp, msti, pari
    void initializePypars(const ParameterSet& pset);
    
    ///Method to initialise Pydat1: mstu, paru, mstj, parj.
    void initializePydat1(const ParameterSet& pset);
    
    ///Method to initialise Pydat2: kchg, pmas, parf, vckm.
    void initializePydat2(const ParameterSet& pset);
    
    ///Method to initialise Pydat3: mdcy, mdme, brat, kfdp.
    void initializePydat3(const ParameterSet& pset);
    
    ///Method to initialise Pydatr: mrpy, rrpy
    void initializePydatr(const ParameterSet& pset);
    
    ///Method to initialise Pymssm: imss, rmss
    //  void initializePymssm(const ParameterSet& pset);
    
    ///Method to initialise Pyint2: kfpr
    void initializePyint2(const ParameterSet& pset);
    
    ///Template method for individual initialization
    void initCard(const ParameterSet&,double&,const std::string&,
		  int maxBound,int minBound=1,int compressed=0);
    
    void initCard(const ParameterSet&,int&,const std::string&,
		  int maxBound,int minBound=1,int compressed=0);

    /*    
    template <class T>
      void test(const ParameterSet&,T&,const std::string&);
    */

    template <class T>
      void genericInitialization(T&, 
				 const std::vector<T>&,
				 const std::string&,
				 int maxBound, 
				 int minBound=1, 
				 int compressed=0);
    
    
  };
} 

#endif
