#ifndef ECALPi0OnlineCalibMonitor_ECALPi0OnlineCalibMonitor_H
#define ECALPi0OnlineCalibMonitor_ECALPi0OnlineCalibMonitor_H

// Framework files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/Vector/interface/GlobalPoint.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"
#include "Geometry/CaloTopology/interface/EcalPreshowerTopology.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"

#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"



// DQM files
#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Daemon/interface/MonitorDaemon.h"

class ECALPi0OnlineCalibMonitor : public edm::EDAnalyzer {

 public:

  explicit ECALPi0OnlineCalibMonitor( const edm::ParameterSet& );
  ~ECALPi0OnlineCalibMonitor();
   
  virtual void analyze( const edm::Event&, const edm::EventSetup& );
  virtual void beginJob(edm::EventSetup const&);
  virtual void endJob(void);

 private:

  ECALPi0OnlineCalibMonitor();

  MonitorElement* m_pi0calibEBRecHitsSize;


  static const unsigned int EBRECHITSSIZEBINS;    static const float EBRECHITSSIZEMIN;    static const float EBRECHITSSIZEMAX;

  int m_nevts;

  // back-end interface
  DaqMonitorBEInterface * m_dbe;

  // Enable the daemon
  bool m_enableMonitorDaemon;

  std::string m_EcalRecHitBarrelProducer;
  std::string m_EcalRecHitBarrelCollection;

  // Write events to an output file named in parameters
  bool m_writeOutputFile;
  std::string m_outputFileName;
  
};

#endif 
