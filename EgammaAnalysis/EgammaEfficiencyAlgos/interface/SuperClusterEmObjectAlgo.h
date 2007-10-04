#ifndef EgammaEfficiencyAlgos_SuperClusterEmObjectAlgo_h
#define EgammaEfficiencyAlgos_SuperClusterEmObjectAlgo_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/EmObjectAlgo.h"
#include "DataFormats/Math/interface/LorentzVector.h"


class SuperClusterEmObjectAlgo : public EmObjectAlgo
{
   public:
      virtual ~SuperClusterEmObjectAlgo(){};
      explicit SuperClusterEmObjectAlgo(){};

      virtual void initialise(const edm::ParameterSet&);
      virtual void run(edm::Event&, EgEff::EmObjectCollection&);

   protected:
      edm::InputTag eeSuperClusterProducer_;
      edm::InputTag ebSuperClusterProducer_;
      edm::InputTag vertexProducer_;

      double scEtCut_;
      bool requireFiducial_;

      void processSCCollection(const math::XYZPoint &,
                               edm::Handle<reco::SuperClusterCollection> &,
                               EgEff::EmObjectCollection &);

      math::XYZTLorentzVector initP4(const math::XYZPoint &, 
                                     const reco::SuperClusterRef &);

};

#endif

