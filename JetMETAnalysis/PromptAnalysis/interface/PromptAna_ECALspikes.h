#ifndef PromptAna_EcalSpikes
#define PromptAna_EcalSpikes

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"
#include "Geometry/CaloTopology/interface/EcalPreshowerTopology.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoCaloTools/Navigation/interface/CaloNavigator.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"

class PromptAna_ECALspikes : public edm::EDProducer{
 public: 
  explicit PromptAna_ECALspikes(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  float recHitEnergyECAL(DetId id, const EcalRecHitCollection *recHits);
  float recHitEnergyHCAL(DetId id, const HBHERecHitCollection *recHits);
  
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;  
  
  const EBRecHitCollection* allEBRecHits;
  const EBUncalibratedRecHitCollection* allEBUnCalRecHits;
  const HBHERecHitCollection* allHcalRecHits;
};

#endif
