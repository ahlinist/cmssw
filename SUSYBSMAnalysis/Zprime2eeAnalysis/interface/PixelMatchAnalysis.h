#ifndef RecoEgamma_PixelMatchAnalysis_PixelMatchAnalysis_h
#define RecoEgamma_PixelMatchAnalysis_PixelMatchAnalysis_h
// -*- C++ -*-
//
// Package:    PixelMatchAnalysis
// Class:      PixelMatchAnalysis
// 
/**\class PixelMatchAnalysis PixelMatchAnalysis.cc RecoEgamma/PixelMatchAnalysis/src/PixelMatchAnalysis.cc

Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//


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
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"

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
// class declaration
//

class PixelMatchAnalysis : public edm::EDAnalyzer {
 public:
  explicit PixelMatchAnalysis(const edm::ParameterSet&);
  ~PixelMatchAnalysis();


 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  double ecaletisol(const edm::Event& Evt, reco::SuperClusterRef maxsupercluster );
  double hcaletisol(reco::SuperClusterRef maxsupercluster, const HBHERecHitCollection* hbhe, const CaloGeometry* geometry);

  double hcaletisol(reco::SuperClusterRef maxsupercluster, const CaloTowerCollection* hbhe);
  std::pair<int,float> trackisol(const reco::GsfTrackRef tr, const reco::TrackCollection* isoTracks, double zvtx);
  std::pair<int,float> findIsoTracks(GlobalVector mom, GlobalPoint vtx,  const reco::TrackCollection* isoTracks, bool isElectron, bool useVertex);
  void searchmax( const reco::PixelMatchGsfElectronCollection*, int &prime, int &second);
  void vertexcorr(HepLorentzVector elecvector, HepLorentzVector & elecvector_vtxcor, double recozvtx, double radius, double tradius);


  // ----------member data ---------------------------

  std::string analprocess_ ;

  std::string electronproducer_;
  std::string electronlabel_;

  std::string hybridsuperclusterproducer_;
  std::string hybridsuperclusterlabel_;

  std::string endcapsuperclusterproducer_ ;
  std::string endcapsuperclusterlabel_ ;

  std::string rootanalfile_ ;

  bool drellyan_;
  double ecalconesize_;
  double hcalconesizemin_;
  double hcalconesizemax_;
  double hcalptMin_;
  double trackptMin_;
  double trackconesize_;
  double rspan_;
  double zspan_;
  double vetoConesize_;
  double jet_;
  double ptrecmin_;
  double etarecmin_;
  double ntrackisol_max_;
  double trackisol_max_;
  double hcalisol_max_;
  double ecalisol_max_;

  TFile *rootfile;
  TTree* mytree;
  TBranch* mybranch;


// HISTO initialisation

  TH1I* histo_run;
  TH1I* histo_event;
  TH1I* gsfcoll_size;

// histos: before isolation cuts:
  TH1F* gsf_Z_m;
  TH1F* gsf_Z_vtxcor_m;
  TH1F* gsf_el_pt;
  TH1F* gsf_po_pt;
  TH1F* gsf_el_vtxcor_pt;
  TH1F* gsf_po_vtxcor_pt;

  //histos for tracker isolation
  TH1F* gsf_el_numbertrackisol;
  TH1F* gsf_el_track_isol_abs;
  TH1F* gsf_el_track_isol;
  TH1F* gsf_po_numbertrackisol;
  TH1F* gsf_po_track_isol_abs;
  TH1F* gsf_po_track_isol;
  //histos for hcal isolation
  TH1F* gsf_el_hcal_isol;
  TH1F* gsf_el_hcal_isol_abs;
  TH1F* gsf_po_hcal_isol;
  TH1F* gsf_po_hcal_isol_abs;
  //histos for ecal isolation
  TH1F* gsf_el_ecal_isol;
  TH1F* gsf_el_ecal_isol_abs;
  TH1F* gsf_po_ecal_isol;
  TH1F* gsf_po_ecal_isol_abs;

// histos: after isolation cuts:
  TH1F* gsf_Z_iso_m;
  TH1F* gsf_Z_vtxcor_iso_m;
  TH1F* gsf_Z_iso_e;
  TH1F* gsf_Z_iso_pt;
  TH1F* gsf_Z_iso_eta;
  TH1F* gsf_Z_iso_phi;

  TH1F* gsf_el_iso_pt;
  TH1F* gsf_po_iso_pt;
  TH1F* gsf_el_iso_eta;
  TH1F* gsf_po_iso_eta;

  TH1F* gsf_el_vtxcor_iso_pt;
  TH1F* gsf_po_vtxcor_iso_pt;
  TH1F* gsf_el_vtxcor_iso_eta;
  TH1F* gsf_po_vtxcor_iso_eta;
  TH1F* gsf_el_vtxcor_iso_e;
  TH1F* gsf_po_vtxcor_iso_e;
  TH1F* gsf_el_vtxcor_iso_phi;
  TH1F* gsf_po_vtxcor_iso_phi;


};

//define this as a plug-in
DEFINE_FWK_MODULE(PixelMatchAnalysis);

#endif
