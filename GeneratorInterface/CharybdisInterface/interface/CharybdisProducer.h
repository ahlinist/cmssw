#ifndef Charybdis_Source_h
#define Charybdis_Source_h

/** \class CharybdisProducer
 *
 * Generates Pythia HepMC events
 *
 * Hector Naves                                  
 *   for the Generator Interface. 26/10/05 
 * Patrick Janot
 *   read all possible cards for Pythia Setup. 26/02/06
 *   ( port from FAMOS )
 ***************************************/

// #define PYGIVE pygive_
// extern "C" {
//   void PYGIVE(const char*,int length);
// }
// 
// #define PY1ENT py1ent_
// extern "C" {
//   void PY1ENT(int& ip, int& kf, double& pe, double& the, double& phi);
// }
// 
// #define PYMASS pymass_
// extern "C" {
//   double PYMASS(int& kf);
// }
// 
// #define PYEXEC pyexec_
// extern "C" {
//   void PYEXEC();
// }



// dummy hwaend 
#define hwaend hwaend_
extern "C" {
void hwaend(){/*dummy*/}
} 

#define PYCOMP pycomp_
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <map>
#include <string>
//#include "HepMC/GenEvent.h"
//#include "HepMC/SimpleVector.h"

#define CHGIVE chgive_
extern "C" {
  void CHGIVE(const int&, const int&, const double&);
}

namespace HepMC {
  class GenEvent;
  class GenParticle;
  class GenVertex;
}


namespace CLHEP
{
  class HepRandomEngine;
}

namespace edm
{
   class CharybdisProducer : public EDProducer {
   public:
      /// Constructor
      CharybdisProducer(const ParameterSet &);
      /// Destructor
      virtual ~CharybdisProducer();

   private:

      /// Interface to the PYGIVE/TXGIVE pythia routine, with add'l protections
      //bool 		call_pygive(const std::string& iParm );
      bool 		chb_pythia_init(const ParameterSet &);
      virtual void 	produce(Event & e, const EventSetup & es);
      void 		chb_charybdis_init();
      void 		clear();
      
      HepMC::GenEvent  *evt;
      unsigned int      eventNumber_;

      // Generic Pythia parameters
      unsigned int 	pythiaPylistVerbosity_; // Pythia PYLIST Verbosity flag
      unsigned int 	maxEventsToPrint_;      // Events to print if verbosity
      bool 		pythiaHepMCVerbosity_;          // HepMC verbosity flag
      double 		comEnergy_;

      //Charybdis parameters
      double 	MPLNCK_;
      double 	MINMSS_;
      double 	MAXMASS_;
      double 	THWMAX_;
      double 	RMMINM_;
      int 	Nsearch_;
      int 	MSSDEF_;
      int 	TOTDIM_;
      int 	NBODY_;
      int 	MSSDEC_;
      bool 	GTSCA_;
      bool 	TIMVAR_;
      bool 	GRYBDY_;
      bool 	KINCUT_;
      bool 	YRCSEC_;
      bool 	RMBOIL_;

      // for single particle generation in pythia
      int    	particleID;
      bool   	doubleParticle;
      double 	ptmin, ptmax;
      double 	etamin, etamax;
      double 	phimin, phimax;
      
      CLHEP::HepRandomEngine* fRandomEngine;
      std::string myName;

   };
} 

#endif
