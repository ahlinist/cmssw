#ifndef PromptAna_BeamHaloClass
#define PromptAna_BeamHaloClass

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "DataFormats/L1CSCTrackFinder/interface/L1CSCTrackCollection.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1CSCStatusDigiCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CSCHaloData.h"
#include "DataFormats/METReco/interface/EcalHaloData.h"
#include "DataFormats/METReco/interface/HcalHaloData.h"
#include "DataFormats/METReco/interface/GlobalHaloData.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"

class PromptAna_BeamHalo : public edm::EDProducer{
 public: 
  explicit PromptAna_BeamHalo(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  
  edm::InputTag IT_L1MuGMTReadout;

  //RecHit Level
  edm::InputTag IT_CSCRecHit;
  edm::InputTag IT_EBRecHit;
  edm::InputTag IT_EERecHit;
  edm::InputTag IT_ESRecHit;
  edm::InputTag IT_HBHERecHit;
  edm::InputTag IT_HORecHit;
  edm::InputTag IT_HFRecHit;

  //Higher Level Reco
  edm::InputTag IT_CosmicMuon;
  edm::InputTag IT_CSCSegment;
  edm::InputTag IT_CollisionMuon;
  edm::InputTag IT_CollisionStandAloneMuon;
  edm::InputTag IT_BeamHaloMuon;
  edm::InputTag IT_CosmicStandAloneMuon;
  edm::InputTag IT_met;
  edm::InputTag IT_CaloTower;
  edm::InputTag IT_SuperCluster;
  edm::InputTag IT_Photon;

  // Halo Data
  edm::InputTag IT_CSCHaloData;
  edm::InputTag IT_EcalHaloData;
  edm::InputTag IT_HcalHaloData;
  edm::InputTag IT_GlobalHaloData;
  edm::InputTag IT_BeamHaloSummary;

  std::string     prefix,suffix;
};

#endif
