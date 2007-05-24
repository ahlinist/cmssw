#ifndef EgammaEfficiencyAlgos_EmObjectAlgo_h
#define EgammaEfficiencyAlgos_EmObjectAlgo_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObjectFwd.h"

class EmObjectAlgo
{
   public:
      virtual ~EmObjectAlgo(){};
      virtual void initialise(const edm::ParameterSet&) = 0;
      virtual void run(const edm::Event&, EgEff::EmObjectCollection&) = 0;
};

#endif

