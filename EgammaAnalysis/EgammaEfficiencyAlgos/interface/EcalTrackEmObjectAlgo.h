#ifndef EgammaEfficiencyAlgos_EcalTrackEmObjectAlgo_h
#define EgammaEfficiencyAlgos_EcalTrackEmObjectAlgo_h

#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/EmObjectAlgo.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <string>

class EcalTrackEmObjectAlgo : public EmObjectAlgo
{
   public:
      virtual ~EcalTrackEmObjectAlgo(){};
      explicit EcalTrackEmObjectAlgo(){};

      virtual void initialise(const edm::ParameterSet&);
      virtual void run(const edm::Event&, EgEff::EmObjectCollection&);

   protected:
      std::string trackProducer_;
      std::string vertexProducer_;
      std::string eeSuperClusterProducer_;
      std::string ebSuperClusterProducer_;
      double minSuperClusterEnergy_;
      double minTrackPt_;
      bool trackAssociation_;

      void processSCCollection(const math::XYZPoint&, edm::Handle<reco::SuperClusterCollection>&, edm::Handle<reco::TrackCollection>&, EgEff::EmObjectCollection&);
      math::XYZTLorentzVector initP4(const math::XYZPoint&, const reco::SuperClusterRef&);
      int findTrack(const reco::SuperClusterRef&, const reco::TrackCollection*);
      double dR(double, double, double, double);
      double dPhi(double, double);
};

#endif

