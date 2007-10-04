#ifndef EgammaEfficiencyAlgos_GeorgiosTagProbeAlgo_h__
#define EgammaEfficiencyAlgos_GeorgiosTagProbeAlgo_h__

#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObject.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/TagProbeAssociation.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

class GeorgiosTagProbeAlgo
{
   private:
      edm::InputTag tagProducer_;
      edm::InputTag probeProducer_;

      double massCutMin_;
      double massCutMax_;
      double deltaPVCut_;
      bool requireOS_;

      bool requireTagTrigger_;
      edm::InputTag triggerProducer_;

      bool triggerPass(const reco::Candidate &trigCand,  EgEff::EmObjectRef tagCand);

   public:
      void initialise(const edm::ParameterSet&);
      void run(const edm::Event&, EgEff::TagProbeAssociationCollection&);
};

#endif

