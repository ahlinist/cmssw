#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

void ElectronIDAlgo::baseSetup(const edm::ParameterSet& conf) {
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
