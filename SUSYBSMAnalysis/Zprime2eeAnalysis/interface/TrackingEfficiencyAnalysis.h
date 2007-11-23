#ifndef RecoEgamma_PixelMatchAnalysis_TrackingEfficiencyAnalysis_h
#define RecoEgamma_PixelMatchAnalysis_TrackingEfficiencyAnalysis_h
// -*- C++ -*-
//
// Package:    PixelMatchAnalysisGsfEm
// Class:      PixelMatchAnalysisGsfEm
// 
/**\class PixelMatchAnalysisGsfEm PixelMatchAnalysisGsfEm.cc RecoEgamma/PixelMatchAnalysis/src/PixelMatchAnalysisGsfEm.cc
Description: <one line class summary>
Implementation:
<Notes on implementation>
*/

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include <CLHEP/Matrix/Vector.h>
#include <CLHEP/Vector/LorentzVector.h>
#include <vector>


#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1I.h"
#include "TTree.h"
#include <sstream>
#include <string>


class TFile;
class TH1F;
class TH1I;

//
// class decleration
//
class  TrackingEfficiencyAnalysis : public edm::EDAnalyzer {
 public:
  explicit  TrackingEfficiencyAnalysis(const edm::ParameterSet&);
  ~ TrackingEfficiencyAnalysis();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  double ecaletisol(const edm::Event& Evt, reco::SuperClusterRef maxsupercluster );
  double hcaletisol(reco::SuperClusterRef maxsupercluster, const HBHERecHitCollection* hbhe, const CaloGeometry* geometry);
  double hcaletisol(reco::SuperClusterRef maxsupercluster, const CaloTowerCollection* hbhe);
  std::pair<int,float> trackisol(reco::SuperClusterRef acluster, const reco::GsfTrackRef tr, const reco::TrackCollection* isoTracks, double zvtx, bool isgsf, bool usevertexcorr);
  std::pair<int,float> findIsoTracks(GlobalVector mom, GlobalPoint vtx,  const reco::TrackCollection* isoTracks, bool isElectron, bool useVertex, bool isgsf, bool usevertexcorr);
  void searchmax( const reco::PixelMatchGsfElectronCollection*, std::vector<reco::SuperClusterRef> superclusters, int &prime, int &second);
  void vertexcorr(HepLorentzVector scvector, HepLorentzVector & scvector_vtxcor, double recozvtx, double radius, double tradius);

  bool SCbelongsToAGsf(const reco::SuperCluster*  , const reco::PixelMatchGsfElectron* );

  double SCGsfInvMass(const reco::SuperCluster*  , const reco::PixelMatchGsfElectron* );

  std::pair<int,float> SCtrackIsolation(const reco::SuperCluster* , const reco::TrackCollection*, const reco::PixelMatchGsfElectron* );

  std::pair<int,float> scHasMatchedTrack(const reco::SuperCluster* , const reco::TrackCollection*, const reco::PixelMatchGsfElectron* );

  Hep3Vector getVertex(const reco::PixelMatchGsfElectron* ele );

  double getScCorrectedEta(const reco::SuperCluster* , Hep3Vector );

  double closestTrackToSc(const reco::SuperCluster*, const reco::TrackCollection*, const reco::PixelMatchGsfElectron* );

  // ----------member data ---------------------------

  std::string analprocess_ ;
  std::string electronproducer_;
  std::string electronlabel_;
  std::string hybridsuperclusterproducer_;
  std::string hybridsuperclusterlabel_;
  std::string endcapsuperclusterproducer_ ;
  std::string endcapsuperclusterlabel_ ;
  std::string rootanalfile_ ;
  
  double trackConeSize_;
  double vetoConeSize_;
  double minTagProbeInvMass_;
  double tagEtMin_;
  double probeEtMin_;
  double scWithMatchedTrack;
  double tagProbeEvents;
  double zVertexCut_;


  int debug;
  int event_tot;
  int gsf_sizegreater0;
  int sc_sizegreater1;
  int sc_ntkisolated;
  int sc_tkisolated;
  int sc_hcalisolated;
  int sc_ecalisolated;
  int sc_one_two_ptmin;
  int sc_one_two_acc;

  TFile *rootfile;
  TTree* mytree;

  //Define variables for branches
  float sc_Z_iso_m_var;
  float sc_Z_vtxcor_iso_m_var;
  float sc_Z_iso_e_var; 
  float sc_Z_iso_pt_var; 
  float sc_Z_iso_eta_var; 
  float sc_Z_iso_phi_var;

  float sc_one_iso_pt_var;
  float sc_two_iso_pt_var;
  float sc_one_iso_eta_var;
  float sc_two_iso_eta_var;

  float sc_one_vtxcor_iso_pt_var;
  float sc_two_vtxcor_iso_pt_var; 
  float sc_one_vtxcor_iso_eta_var;
  float sc_two_vtxcor_iso_eta_var;
  float sc_one_vtxcor_iso_e_var;
  float sc_two_vtxcor_iso_e_var; 
  float sc_one_vtxcor_iso_phi_var;
  float sc_two_vtxcor_iso_phi_var;

  double sc_weight;

  // HISTO initialisation

  TH1I* gsfcoll_size;
  TH1I* sccoll_size;

  TH1F* h_tagCollectionSize;
  TH1F* h_tagEnergy;
  TH1F* h_trackPtAroundSC;
  TH1F* h_trackPtAroundSCFrac;
  TH1F* h_TagProbeInvMass;
  TH1F* h_matchedTrackPtoverSCEt;
  TH1F* h_minDeltaRTrackSC;

};

#endif
