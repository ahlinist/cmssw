#ifndef PromptAna_PMTNOISE
#define PromptAna_PMTNOISE

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "RecoCaloTools/Navigation/interface/CaloNavigator.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "Geometry/HcalTowerAlgo/src/HcalHardcodeGeometryData.h"

#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"

#include "FWCore/Framework/interface/TriggerNames.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

#include "TString.h"
#include <fstream>
#include <iostream>

class PromptAna_PMTNoise : public edm::EDProducer {
 public:
  explicit PromptAna_PMTNoise(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );

  
  bool isPrimaryVertex(edm::Event& iEvent);
  bool isPhysDeclared(edm::Event& iEvent,
		      const edm::EventSetup& iSetup);

  void  BSCTriggers(edm::Event& iEvent,
		    L1GlobalTriggerReadoutRecord gtRecord,
		    auto_ptr<bool>& isbscminbias,
		    auto_ptr<bool>& isbschalo  
		    );
  void BPTXTriggers(edm::Event& iEvent,
		    L1GlobalTriggerReadoutRecord gtRecord,
		    const L1GtTriggerMenu menu,
		    auto_ptr<bool>& isbptx0,  
		    auto_ptr<bool>& isbptxplus,     
		    auto_ptr<bool>& isbptxminus);

  
  const edm::InputTag   HFRecHitTag, METTag, TrackTag;
  const std::string     prefix,suffix;
  bool isMC;
  const edm::InputTag hlTriggerResults_;
  const edm::InputTag L1Triggers_;

  edm::TriggerNames triggerNames_;

  std::vector<std::string>  hlNames_;

  int debug_;
};

#endif
