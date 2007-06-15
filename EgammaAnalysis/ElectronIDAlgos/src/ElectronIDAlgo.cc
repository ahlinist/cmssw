#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

void ElectronIDAlgo::baseSetup(const edm::ParameterSet& conf) {
  useEoverPIn_ = conf.getParameter<bool>("useEoverPIn");
  useDeltaEtaIn_ = conf.getParameter<bool>("useDeltaEtaIn");
  useDeltaPhiIn_ = conf.getParameter<bool>("useDeltaPhiIn");
  useHoverE_ = conf.getParameter<bool>("useHoverE");
  useE9overE25_ = conf.getParameter<bool>("useE9overE25");
  useEoverPOut_ = conf.getParameter<bool>("useEoverPOut");
  useDeltaPhiOut_ = conf.getParameter<bool>("useDeltaPhiOut");
  useInvEMinusInvP_ = conf.getParameter<bool>("useInvEMinusInvP");
  useBremFraction_ = conf.getParameter<bool>("useBremFraction");
  useSigmaEtaEta_ = conf.getParameter<bool>("useSigmaEtaEta");
  useSigmaPhiPhi_ = conf.getParameter<bool>("useSigmaPhiPhi");

  barrelClusterShapeAssocProducer_ = conf.getParameter<edm::InputTag>("barrelClusterShapeAssociation");
  endcapClusterShapeAssocProducer_ = conf.getParameter<edm::InputTag>("endcapClusterShapeAssociation");
}

const reco::ClusterShapeRef& ElectronIDAlgo::getClusterShape(const reco::PixelMatchGsfElectron* electron, const edm::Event& e) {

  // Get association maps linking BasicClusters to ClusterShape.
  edm::Handle<reco::BasicClusterShapeAssociationCollection> clusterShapeHandle;
  if (electron->classification()<100) {
    e.getByLabel(barrelClusterShapeAssocProducer_, clusterShapeHandle);
  } else {
    e.getByLabel(endcapClusterShapeAssocProducer_, clusterShapeHandle);
  }

  // Find entry in map corresponding to seed BasicCluster of SuperCluster
  reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr;
  seedShpItr = clusterShapeHandle->find(electron->superCluster()->seed());
  return seedShpItr->val;
}
