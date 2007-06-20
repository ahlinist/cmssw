#ifndef EgammaEfficiencyAlgos_TagProbeAlgo_h__
#define EgammaEfficiencyAlgos_TagProbeAlgo_h__

#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObject.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/TagProbeAssociation.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

class TagProbeAlgo
{
   private:
      std::string tagProducer_;
      std::string probeProducer_;
      double massCutMin_;
      double massCutMax_;
      double invariantMass(const EgEff::EmObjectRef&, const EgEff::EmObjectRef&);

   public:
      void initialise(const edm::ParameterSet&);
      void run(const edm::Event&, EgEff::TagProbeAssociationCollection&);
};

#endif

