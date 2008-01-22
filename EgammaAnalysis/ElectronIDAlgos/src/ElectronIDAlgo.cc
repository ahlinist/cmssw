#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

void ElectronIDAlgo::baseSetup(const edm::ParameterSet& conf) {
  barrelClusterShapeAssocProducer_ = conf.getParameter<edm::InputTag>("barrelClusterShapeAssociation");
  endcapClusterShapeAssocProducer_ = conf.getParameter<edm::InputTag>("endcapClusterShapeAssociation");
}

const reco::ClusterShapeRef& ElectronIDAlgo::getClusterShape(const reco::GsfElectron* electron, const edm::Event& e) {

  // Get association maps linking BasicClusters to ClusterShape.
  edm::Handle<reco::BasicClusterShapeAssociationCollection> clusterShapeHandleBarrel;
  edm::Handle<reco::BasicClusterShapeAssociationCollection> clusterShapeHandleEndcap;
  e.getByLabel(barrelClusterShapeAssocProducer_, clusterShapeHandleBarrel);
  e.getByLabel(endcapClusterShapeAssocProducer_, clusterShapeHandleEndcap);

  // Find entry in map corresponding to seed BasicCluster of SuperCluster
  reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr;

  if (electron->classification()<100) {
    seedShpItr=clusterShapeHandleBarrel->find(electron->superCluster()->seed());
    if (electron->classification()==40 && seedShpItr == clusterShapeHandleBarrel->end()) 
        seedShpItr=clusterShapeHandleEndcap->find(electron->superCluster()->seed());
  } else {
    seedShpItr=clusterShapeHandleEndcap->find(electron->superCluster()->seed());
  }

  return seedShpItr->val;
}
