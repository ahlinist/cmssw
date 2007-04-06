#ifndef PythiaFilterGammaJetWithIsoPi0_h
#define PythiaFilterGammaJetWithIsoPi0_h

/** \class PythiaFilterGammaJetWithIsoPi0
 *
 *  PythiaFilterGammaJetWithIsoPi0 filter implements generator-level preselections 
 *  for photon+jet like events to be used in pi0 rejection studies.
 *  Based on A. Ulyanov PythiaFilterGammaJetWithBg.h code 
 *
 * \author A.Kyriakis, NCSR "Demokritos"
 *
 ************************************************************/

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


class PythiaFilterGammaJetWithIsoPi0 : public edm::EDFilter {
   public:
      explicit PythiaFilterGammaJetWithIsoPi0(const edm::ParameterSet&);
      ~PythiaFilterGammaJetWithIsoPi0();

      virtual bool filter(edm::Event&, const edm::EventSetup&);

   private:
      
       std::string label_;
       double etaMin;
       double energyMin;
       double etaMax;
       double energyMax;        
       double isocone;
       double isodr;
       double isodeta;
       double ebEtaMax;
       double deltaEB;
       double deltaEE;

       int theNumberOfTestedEvt;
       int theNumberOfSelected;
       int maxnumberofeventsinrun;

};
#endif
