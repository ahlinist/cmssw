#ifndef EgammaEfficiencyAlgos_CtfWithMaterialTrackEmObjectAlgo_h
#define EgammaEfficiencyAlgos_CtfWithMaterialTrackEmObjectAlgo_h

#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/EmObjectAlgo.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include <string>

class CtfWithMaterialTrackEmObjectAlgo : public EmObjectAlgo
{
   public:
      virtual ~CtfWithMaterialTrackEmObjectAlgo(){};
      explicit CtfWithMaterialTrackEmObjectAlgo(){};

      virtual void initialise(const edm::ParameterSet&);
      virtual void run(edm::Event&, EgEff::EmObjectCollection&);

   protected:

      double computeTrackIsolation(const reco::Track&, const reco::TrackCollection&);

      edm::InputTag trackProducer_;

      bool requireFiducial_;
      bool requireIsolation_;
      double ptCut_;
      int validHits_;
      double normalizedChi2_;

};

#endif

