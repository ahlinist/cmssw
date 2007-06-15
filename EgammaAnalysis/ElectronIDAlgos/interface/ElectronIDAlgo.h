#ifndef ElectronIDAlgo_H
#define ElectronIDAlgo_H

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"

class ElectronIDAlgo {

public:

  ElectronIDAlgo(){};

  virtual ~ElectronIDAlgo(){};

  void baseSetup(const edm::ParameterSet& conf);

 protected:

  const reco::ClusterShapeRef& getClusterShape(const reco::PixelMatchGsfElectron*, const edm::Event&);

  bool useEoverPIn_;
  bool useDeltaEtaIn_;
  bool useDeltaPhiIn_;
  bool useHoverE_;
  bool useE9overE25_;
  bool useEoverPOut_;
  bool useDeltaPhiOut_;
  bool useInvEMinusInvP_;
  bool useBremFraction_;
  bool useSigmaEtaEta_;
  bool useSigmaPhiPhi_;

  edm::InputTag barrelClusterShapeAssocProducer_;
  edm::InputTag endcapClusterShapeAssocProducer_;
};

#endif // ElectronIDAlgo_H
