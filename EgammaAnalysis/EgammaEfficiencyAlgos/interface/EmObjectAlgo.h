#ifndef EgammaEfficiencyAlgos_EmObjectAlgo_h
#define EgammaEfficiencyAlgos_EmObjectAlgo_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObjectFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

class EmObjectAlgo
{
   public:
      virtual ~EmObjectAlgo(){};
      virtual void initialise(const edm::ParameterSet&) = 0;
      virtual void run(edm::Event&, EgEff::EmObjectCollection&) = 0;

   protected:

      double ecalEta(double &, double &, double &);
      double ecalPhi(double &, double &, double &, int , double &);


};

#endif

