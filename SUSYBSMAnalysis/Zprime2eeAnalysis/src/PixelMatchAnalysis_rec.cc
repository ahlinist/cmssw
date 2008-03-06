// -*- C++ -*-
// Barbara Clerbaux
// Package:    PixelMatchAnalysis
// Class:      PixelMatchAnalysis
// To look at the main code: go directly to: PixelMatchAnalysis::analyze
// main code to analyse data: look for a high energy e+e- pair
// look for 2 pixelmatch gsf electron with opposit charge
// ------------------------------------------------------


#include "SUSYBSMAnalysis/Zprime2eeAnalysis/interface/PixelMatchAnalysis.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include <vector>
 
#include "FWCore/MessageLogger/interface/MessageLogger.h"



#define PI 3.141592654
#define TWOPI 6.283185308

// #include "FWCore/Framework/interface/MakerMacros.h"

int debug=1;
float valeur=0.;

int event_tot=0;
int gsf_sizegreater1=0;
int gsf_goodcharge=0;
int gsf_ntkisolated = 0;
int gsf_tkisolated = 0;
int gsf_hcalisolated = 0;
int gsf_ecalisolated = 0;
int gsf_el_po_ptmin = 0;
int gsf_el_po_acc = 0;

using namespace std;


// ------------ constuctor   -------------------------------
PixelMatchAnalysis::PixelMatchAnalysis(const edm::ParameterSet& iConfig)
{

  //Define variables for branches
  gsf_Z_iso_m_var = 0.;
  gsf_Z_vtxcor_iso_m_var = 0.;
  gsf_Z_iso_e_var = 0.; 
  gsf_Z_iso_pt_var = 0.; 
  gsf_Z_iso_eta_var = 0.; 
  gsf_Z_iso_phi_var = 0.;

  gsf_el_iso_pt_var = 0.;
  gsf_po_iso_pt_var = 0.;
  gsf_el_iso_eta_var = 0.;
  gsf_po_iso_eta_var = 0.;

  gsf_el_vtxcor_iso_pt_var = 0.;
  gsf_po_vtxcor_iso_pt_var = 0.; 
  gsf_el_vtxcor_iso_eta_var = 0.;
  gsf_po_vtxcor_iso_eta_var = 0.;
  gsf_el_vtxcor_iso_e_var = 0.;
  gsf_po_vtxcor_iso_e_var = 0.; 
  gsf_el_vtxcor_iso_phi_var = 0.;
  gsf_po_vtxcor_iso_phi_var = 0.;

  gsf_weight = 0. ;


  drellyan_ = iConfig.getParameter<bool>("drellyan");
  ecalconesize_ = iConfig.getParameter<double>("ecalconesize");
  hcalconesizemin_ = iConfig.getParameter<double>("hcalconesizemin");
  hcalconesizemax_ = iConfig.getParameter<double>("hcalconesizemax");
  hcalptMin_ = iConfig.getParameter<double>("hcalptMin");
  trackptMin_ = iConfig.getParameter<double>("trackptMin");
  trackconesize_ = iConfig.getParameter<double>("trackconesize");
  rspan_ = iConfig.getParameter<double>("rspan");
  zspan_ = iConfig.getParameter<double>("zspan");
  vetoConesize_ = iConfig.getParameter<double>("vetoConesize");
  jet_ = iConfig.getParameter<bool>("jet");
  ptrecmin_ = iConfig.getParameter<double>("ptrecmin");
  etarecmin_ = iConfig.getParameter<double>("etarecmin");
  ntrackisol_max_ = iConfig.getParameter<double>("ntrackisol_max");
  trackisol_max_ = iConfig.getParameter<double>("trackisol_max");
  hcalisol_max_ = iConfig.getParameter<double>("hcalisol_max");
  ecalisol_max_ = iConfig.getParameter<double>("ecalisol_max");

  if (debug) {
  edm::LogVerbatim("Zprime2eeAna")<<"Parameters for Gsf/Gsf:";
  edm::LogVerbatim("Zprime2eeAna")<<"===============================================";
    edm::LogVerbatim("Zprime2eeAna")<<"drellyan_ = " << drellyan_ ;
    edm::LogVerbatim("Zprime2eeAna")<<"rootanalfile_ = " << rootanalfile_ ;
    edm::LogVerbatim("Zprime2eeAna")<<"ecalconesize_ = " << ecalconesize_ ;
    edm::LogVerbatim("Zprime2eeAna")<<"hcalconesizemin_ = " << hcalconesizemin_ ;
    edm::LogVerbatim("Zprime2eeAna")<<"hcalconesizemax_ = " << hcalconesizemax_ ;
    edm::LogVerbatim("Zprime2eeAna")<<"hcalptMin_ = " << hcalptMin_ ;
    edm::LogVerbatim("Zprime2eeAna")<<"trackptMin_ = " << trackptMin_ ;
    edm::LogVerbatim("Zprime2eeAna")<<"trackconesize_ = " <<trackconesize_;;
    edm::LogVerbatim("Zprime2eeAna")<<"rspan_ = "<<rspan_;;
    edm::LogVerbatim("Zprime2eeAna")<<"zspan_ = "<<zspan_;;
    edm::LogVerbatim("Zprime2eeAna")<<"vetoConesize_ = "<<vetoConesize_;;
    edm::LogVerbatim("Zprime2eeAna")<<"jet_ = "<<jet_;;
    edm::LogVerbatim("Zprime2eeAna")<<"ptrecmin_ = "<<ptrecmin_;;
    edm::LogVerbatim("Zprime2eeAna")<<"etarecmin_ = "<<etarecmin_;;
    edm::LogVerbatim("Zprime2eeAna")<<"trackisol_max_ = "<<trackisol_max_;;
    edm::LogVerbatim("Zprime2eeAna")<<"hcalisol_max_ = "<<hcalisol_max_;;
    edm::LogVerbatim("Zprime2eeAna")<<"ecalisol_max_ = "<<ecalisol_max_;;
 edm::LogVerbatim("Zprime2eeAna")<<"===============================================";
   }
}


// ------------ destructor  -------------------------------
PixelMatchAnalysis::~PixelMatchAnalysis()
{
}


// ------------ method called once each job just before starting event loop  ------------
void PixelMatchAnalysis::beginJob(const edm::EventSetup&)
{
  edm::Service<TFileService> fs;


  //mytree = new TTree("test","test sur donnees");
  //mybranch = mytree->Branch("essai",&valeur,"essai/F");

//  HISTO INITIALISATION :

// histos: on all events:
  histo_run = fs->make<TH1I>("histo_run","histo_run",2000,0.,2000.);
  histo_event = fs->make<TH1I>("histo_event","histo_event",100,0.,10000.);
  gsfcoll_size = fs->make<TH1I>("gsfcoll_size","gsfcoll_size",20,0.,20.);

// histos: before isolation cuts:
  gsf_Z_m= fs->make<TH1F>("gsf_Z_m","gsf_Z_m",600,0.,3000.);
  gsf_Z_vtxcor_m= fs->make<TH1F>("gsf_Z_vtxcor_m","gsf_Z_vtxcor_m",600,0.,3000.);
  gsf_el_pt= fs->make<TH1F>("gsf_el_pt","gsf_el_pt",400,0.,2000.);
  gsf_po_pt= fs->make<TH1F>("gsf_po_pt","gsf_po_pt",400,0.,2000.);
  gsf_el_vtxcor_pt= fs->make<TH1F>("gsf_el_vtxcor_pt","gsf_el_vtxcor_pt",400,0.,2000.);
  gsf_po_vtxcor_pt= fs->make<TH1F>("gsf_po_vtxcor_pt","gsf_po_vtxcor_pt",400,0.,2000.);

  //histos for tracker isolation 
  gsf_el_numbertrackisol = fs->make<TH1F>("gsf_el_numbertrackisol","gsf_el_numbertrackisol",200,0.,20.);
  gsf_el_track_isol_abs = fs->make<TH1F>("gsf_el_track_isol_abs","gsf_el_track_isol_abs",200,0.,200.);
  gsf_el_track_isol = fs->make<TH1F>("gsf_el_track_isol","gsf_el_track_isol",200,0.,2.);
  gsf_po_numbertrackisol = fs->make<TH1F>("gsf_po_numbertrackisol","gsf_po_numbertrackisol",200,0.,20.);
  gsf_po_track_isol_abs = fs->make<TH1F>("gsf_po_track_isol_abs","gsf_po_track_isol_abs",200,0.,200.);
  gsf_po_track_isol = fs->make<TH1F>("gsf_po_track_isol","gsf_po_track_isol",200,0.,2.);
  //histos for hcal isolation 
  gsf_el_hcal_isol = fs->make<TH1F>("gsf_el_hcal_isol","gsf_el_hcal_isol",200,0.,2.);
  gsf_el_hcal_isol_abs = fs->make<TH1F>("gsf_el_hcal_isol_abs","gsf_el_hcal_isol_abs",200,0.,200.);
  gsf_po_hcal_isol = fs->make<TH1F>("gsf_po_hcal_isol","gsf_po_hcal_isol",200,0.,2.);
  gsf_po_hcal_isol_abs = fs->make<TH1F>("gsf_po_hcal_isol_abs","gsf_po_hcal_isol_abs",200,0.,200.);
  //histos for ecal isolation 
  gsf_el_ecal_isol = fs->make<TH1F>("gsf_el_ecal_isol","gsf_el_ecal_isol",200,0.,2.);
  gsf_el_ecal_isol_abs = fs->make<TH1F>("gsf_el_ecal_isol_abs","gsf_el_ecal_isol_abs",200,0.,200.);
  gsf_po_ecal_isol = fs->make<TH1F>("gsf_po_ecal_isol","gsf_po_ecal_isol",200,0.,2.);
  gsf_po_ecal_isol_abs = fs->make<TH1F>("gsf_po_ecal_isol_abs","gsf_po_ecal_isol_abs",200,0.,200.);

// histos: after isolation cuts:
  gsf_Z_iso_m= fs->make<TH1F>("gsf_Z_iso_m","gsf_Z_iso_m",600,0.,3000.);
  gsf_Z_vtxcor_iso_m= fs->make<TH1F>("gsf_Z_vtxcor_iso_m","gsf_Z_vtxcor_iso_m",600,0.,3000.);
  gsf_Z_iso_e= fs->make<TH1F>("gsf_Z_iso_e","gsf_Z_iso_e",400,0.,4000.);
  gsf_Z_iso_pt= fs->make<TH1F>("gsf_Z_iso_pt","gsf_Z_iso_pt",400,0.,2000.);
  gsf_Z_iso_eta= fs->make<TH1F>("gsf_Z_iso_eta","gsf_Z_iso_eta",200,-10.,10.);
  gsf_Z_iso_phi= fs->make<TH1F>("gsf_Z_iso_phi","gsf_Z_iso_phi",160,-4.,4.);

  gsf_el_iso_pt= fs->make<TH1F>("gsf_el_iso_pt","gsf_el_iso_pt",400,0.,2000.);
  gsf_po_iso_pt= fs->make<TH1F>("gsf_po_iso_pt","gsf_po_iso_pt",400,0.,2000.);
  gsf_el_iso_eta = fs->make<TH1F>("gsf_el_iso_eta","gsf_el_iso_eta",120,-3.,3.);
  gsf_po_iso_eta = fs->make<TH1F>("gsf_po_iso_eta","gsf_po_iso_eta",120,-3.,3.);

  gsf_el_vtxcor_iso_pt= fs->make<TH1F>("gsf_el_vtxcor_iso_pt","gsf_el_vtxcor_iso_pt",400,0.,2000.);
  gsf_po_vtxcor_iso_pt= fs->make<TH1F>("gsf_po_vtxcor_iso_pt","gsf_po_vtxcor_iso_pt",400,0.,2000.);
  gsf_el_vtxcor_iso_eta = fs->make<TH1F>("gsf_el_vtxcor_iso_eta","gsf_el_vtxcor_iso_eta",120,-3.,3.);
  gsf_po_vtxcor_iso_eta = fs->make<TH1F>("gsf_po_vtxcor_iso_eta","gsf_po_vtxcor_iso_eta",120,-3.,3.);
  gsf_el_vtxcor_iso_e = fs->make<TH1F>("gsf_el_vtxcor_iso_e","gsf_el_vtxcor_iso_e",400,0.,4000.);
  gsf_po_vtxcor_iso_e = fs->make<TH1F>("gsf_po_vtxcor_iso_e","gsf_po_vtxcor_iso_e",400,0.,4000.);
  gsf_el_vtxcor_iso_phi = fs->make<TH1F>("gsf_el_vtxcor_iso_phi","gsf_el_vtxcor_iso_phi",160,-4.,4.);
  gsf_po_vtxcor_iso_phi = fs->make<TH1F>("gsf_po_vtxcor_iso_phi","gsf_po_vtxcor_iso_phi",160,-4.,4.);

  //Define tree
  mytree = fs->make<TTree>("tree","tr");

  //Define branches
  mytree->Branch("gsf_Z_iso_m_branch",&gsf_Z_iso_m_var,"gsf_Z_iso_m_branch/F");
  mytree->Branch("gsf_Z_vtxcor_iso_m_branch",&gsf_Z_vtxcor_iso_m_var,"gsf_Z_vtxcor_iso_m_branch/F");
  mytree->Branch("gsf_Z_iso_e_branch",&gsf_Z_iso_e_var ,"gsf_Z_iso_e_branch/F");
  mytree->Branch("gsf_Z_iso_pt_branch",&gsf_Z_iso_pt_var,"gsf_Z_iso_pt_branch/F");
  mytree->Branch("gsf_Z_iso_eta_branch",&gsf_Z_iso_eta_var,"gsf_Z_iso_eta_branch/F");
  mytree->Branch("gsf_Z_iso_phi_branch",&gsf_Z_iso_phi_var,"gsf_Z_iso_phi_branch/F");
  
  mytree->Branch("gsf_el_iso_pt_branch",&gsf_el_iso_pt_var,"gsf_el_iso_pt_branch/F");
  mytree->Branch("gsf_po_iso_pt_branch",&gsf_po_iso_pt_var,"gsf_po_iso_pt_branch/F");
  mytree->Branch("gsf_el_iso_eta_branch",&gsf_el_iso_eta_var,"gsf_el_iso_eta_branch/F");
  mytree->Branch("gsf_po_iso_eta_branch",&gsf_po_iso_eta_var,"gsf_po_iso_eta_branch/F");
  
  mytree->Branch("gsf_el_vtxcor_iso_pt_branch",&gsf_el_vtxcor_iso_pt_var,"gsf_el_vtxcor_iso_pt_branch/F");
  mytree->Branch("gsf_po_vtxcor_iso_pt_branch",&gsf_po_vtxcor_iso_pt_var,"gsf_po_vtxcor_iso_pt_branch/F");
  mytree->Branch("gsf_el_vtxcor_iso_eta_branch",&gsf_el_vtxcor_iso_eta_var,"gsf_el_vtxcor_iso_eta_branch/F");
  mytree->Branch("gsf_po_vtxcor_iso_eta_branch",&gsf_po_vtxcor_iso_eta_var,"gsf_po_vtxcor_iso_eta_branch/F");
  mytree->Branch("gsf_el_vtxcor_iso_e_branch",&gsf_el_vtxcor_iso_e_var,"gsf_el_vtxcor_iso_e_branch/F");
  mytree->Branch("gsf_po_vtxcor_iso_e_branch",&gsf_po_vtxcor_iso_e_var,"gsf_po_vtxcor_iso_e_branch/F");
  mytree->Branch("gsf_el_vtxcor_iso_phi_branch",&gsf_el_vtxcor_iso_phi_var,"gsf_el_vtxcor_iso_phi_branch/F");
  mytree->Branch("gsf_po_vtxcor_iso_phi_branch",&gsf_po_vtxcor_iso_phi_var,"gsf_po_vtxcor_iso_phi_branch/F");

  mytree->Branch("gsf_weight",&gsf_weight,"gsf_weight/D");
}


//-------------------------------------------------------------------------
double PixelMatchAnalysis::ecaletisol( const edm::Event& Evt, reco::SuperClusterRef maxsupercluster ){

  using namespace edm; // needed for all fwk related classes
  using namespace std;
  using namespace reco;

  Handle<reco::SuperClusterCollection> pHybridSuperClusters;
  Handle<reco::SuperClusterCollection> pIslandSuperClusters;
  Handle<reco::BasicClusterCollection> pHybridBasicClusters;
  Handle<reco::BasicClusterCollection> pIslandBasicClusters;

  Handle<double> weightHandle;
  Evt.getByLabel ("weight","weight", weightHandle);
  gsf_weight= * weightHandle;

  try {
    Evt.getByLabel("hybridSuperClusters","", pHybridSuperClusters);
    Evt.getByLabel("islandSuperClusters","islandEndcapSuperClusters", pIslandSuperClusters);
    Evt.getByLabel("hybridSuperClusters","", pHybridBasicClusters);
    Evt.getByLabel("islandBasicClusters","islandEndcapBasicClusters", pIslandBasicClusters);
  } catch ( cms::Exception& ex ) {
    LogDebug("Zprime2eeAna")<<"could not found the product with producer hybridSuperClusters or islandEndcapSuperClusters" << ex ;
  }

  const reco::BasicClusterCollection* hybridBasicClusters = pHybridBasicClusters.product();
  const reco::BasicClusterCollection* islandBasicClusters = pIslandBasicClusters.product();

  std::vector<const reco::BasicCluster*> bclusters;
  for (reco::BasicClusterCollection::const_iterator ibc = hybridBasicClusters->begin(); 
       ibc < hybridBasicClusters->end(); ibc++ ){bclusters.push_back(&(*ibc));}
  for (reco::BasicClusterCollection::const_iterator iec = islandBasicClusters->begin(); 
       iec < islandBasicClusters->end(); iec++ ){bclusters.push_back(&(*iec));}

  float ecalIsol=0.;
  
  float candSCphi = maxsupercluster->phi();
  float candSCeta = maxsupercluster->eta();

  LogDebug("Zprime2eeAna")<<"energie(raw and presh) eta and phi of max SC "
      <<maxsupercluster->rawEnergy()<<" "
      <<maxsupercluster->preshowerEnergy()<<" "
      <<maxsupercluster->eta()<<" "
      <<maxsupercluster->phi();

  bool MATCHEDSC = true;

  const reco::BasicCluster *cluster= 0;

  //loop over basic clusters
  for(std::vector<const reco::BasicCluster*>::const_iterator cItr = bclusters.begin(); cItr != bclusters.end(); ++cItr){
 
    cluster = *cItr;
    float ebc_bce    = cluster->energy();
    float ebc_bceta  = cluster->eta();
    float ebc_bcphi  = cluster->phi();
    float ebc_bcet   = ebc_bce*sin(2*atan(exp(ebc_bceta)));
    float newDelta;


    if(MATCHEDSC){
      bool inSuperCluster = false;

      reco::basicCluster_iterator theEclust = maxsupercluster->clustersBegin();
      // loop over the basic clusters of the matched supercluster
      for(;theEclust != maxsupercluster->clustersEnd();
	  theEclust++) {
	if (&(**theEclust) ==  cluster) inSuperCluster = true;
      }
      if (!inSuperCluster) {
	float deltaphi;
	if(ebc_bcphi<0) ebc_bcphi+=TWOPI;
	if(candSCphi<0) candSCphi+=TWOPI;
	deltaphi=fabs(ebc_bcphi-candSCphi);
	if(deltaphi>TWOPI) deltaphi-=TWOPI;
	if(deltaphi>PI) deltaphi=TWOPI-deltaphi;
	float deltaeta=fabs(ebc_bceta-candSCeta);
	newDelta= sqrt(deltaphi*deltaphi+ deltaeta*deltaeta);
	if(newDelta < ecalconesize_) {
	  LogDebug("Zprime2eeAna")<<"found a new one with energy, eta and phi "<<ebc_bce<<" "<<ebc_bceta<<" "<<ebc_bcphi;
	  ecalIsol+=ebc_bcet;
	}
      }
    }
  }
  return ecalIsol;
}

double PixelMatchAnalysis::hcaletisol(reco::SuperClusterRef maxsupercluster, const CaloTowerCollection* hbhe){

  using namespace edm; // needed for all fwk related classes
  using namespace std;
  using namespace reco;

  float hcalIsol=0.;

  float candSCphi = maxsupercluster->phi();
  float candSCeta = maxsupercluster->eta();

  for(CaloTowerCollection::const_iterator hbheItr = hbhe->begin(); hbheItr != hbhe->end(); ++hbheItr){
    double HcalHit_eta=hbheItr->eta();
    double HcalHit_phi=hbheItr->phi();
    float HcalHit_pth=hbheItr->hadEt();
    if(HcalHit_pth>hcalptMin_) {
      float deltaphi;
      if(HcalHit_phi<0) HcalHit_phi+=TWOPI;
      if(candSCphi<0) candSCphi+=TWOPI;
      deltaphi=fabs(HcalHit_phi-candSCphi);
      if(deltaphi>TWOPI) deltaphi-=TWOPI;
      if(deltaphi>PI) deltaphi=TWOPI-deltaphi;
      float deltaeta=fabs(HcalHit_eta-candSCeta);
      float newDelta= sqrt(deltaphi*deltaphi+ deltaeta*deltaeta);
      if(newDelta<hcalconesizemax_ && newDelta>hcalconesizemin_) hcalIsol+=HcalHit_pth;
    }      
  }
  return hcalIsol;
}





//-------------------------------------------------------------------------
double PixelMatchAnalysis::hcaletisol(reco::SuperClusterRef maxsupercluster, const HBHERecHitCollection* hbhe, const CaloGeometry* geometry){

  using namespace edm; // needed for all fwk related classes
  using namespace std;
  using namespace reco;

  float hcalIsol=0.;

  float candSCphi = maxsupercluster->phi();
  float candSCeta = maxsupercluster->eta();

  for(HBHERecHitCollection::const_iterator hbheItr = hbhe->begin(); hbheItr != hbhe->end(); ++hbheItr){
    double HcalHit_energy=hbheItr->energy();
    double HcalHit_eta=geometry->getPosition(hbheItr->id()).eta();
    double HcalHit_phi=geometry->getPosition(hbheItr->id()).phi();
    float HcalHit_pth=HcalHit_energy*sin(2*atan(exp(-HcalHit_eta)));
    if(HcalHit_pth>hcalptMin_) {
      float deltaphi;
      if(HcalHit_phi<0) HcalHit_phi+=TWOPI;
      if(candSCphi<0) candSCphi+=TWOPI;
      deltaphi=fabs(HcalHit_phi-candSCphi);
      if(deltaphi>TWOPI) deltaphi-=TWOPI;
      if(deltaphi>PI) deltaphi=TWOPI-deltaphi;
      float deltaeta=fabs(HcalHit_eta-candSCeta);
      float newDelta= sqrt(deltaphi*deltaphi+ deltaeta*deltaeta);
      if(newDelta<hcalconesizemax_ && newDelta>hcalconesizemin_) hcalIsol+=HcalHit_pth;
    }      
  }
  return hcalIsol;
}



//-------------------------------------------------------------------------
std::pair<int,float> PixelMatchAnalysis::findIsoTracks(GlobalVector mom, GlobalPoint vtx,  const reco::TrackCollection* isoTracks, bool isElectron, bool useVertex)
{

  // Check that reconstructed tracks fit within cone boundaries,
  // (Note: tracks will not always stay within boundaries)
  int ntrack = 0;
  float ptSum = 0.;

  for(reco::TrackCollection::const_iterator trItr = isoTracks->begin(); trItr != isoTracks->end(); ++trItr){

    GlobalPoint ivtx(trItr->vertex().x(),trItr->vertex().y(),trItr->vertex().z());
    reco::Track::Vector ip = trItr->momentum();
    GlobalVector imom ( ip.x(), ip.y(), ip.z());

    float pt = imom.perp();
    float dperp = 0.;
    float dz = 0.;
    float deta = 0.;
    float dphi = 0.;
    if (useVertex) {
      dperp = ivtx.perp()-vtx.perp();
      dz = ivtx.z()-vtx.z();
      deta = imom.eta()-mom.eta();
      dphi = imom.phi()-mom.phi();
    } else {
      //in case of unkown photon vertex, modify direction of photon to point from
      //current track vertex to sc instead of from (0.,0.,0.) to sc.  In this 
      //way, all tracks are considered based on direction alone.
      GlobalVector mom_temp = mom - GlobalVector(ivtx.x(),ivtx.y(),ivtx.z());
      deta = imom.eta()-mom_temp.eta();
      dphi = imom.phi()-mom_temp.phi();
    }
    // Correct dmom_phi's from [-2pi,2pi] to [-pi,pi]
    if (dphi>M_PI) dphi = dphi - 2*M_PI;
    else if (dphi<-M_PI) dphi = dphi + 2*M_PI;

    float R = sqrt( dphi*dphi + deta*deta );

    // Apply boundary cut
    bool selected=false;

    if (pt > trackptMin_ && R < trackconesize_ &&
	fabs(dperp) < rspan_ && fabs(dz) < zspan_) selected=true;
  
    if (selected) {
      ntrack++;
      if (!isElectron || R > vetoConesize_) ptSum+=pt; //to exclude electron track
    }

  }

  if (isElectron) ntrack-=1; //to exclude electron track

  return (std::pair<int,float>(ntrack,ptSum));

}



//-------------------------------------------------------------------------
std::pair<int,float> PixelMatchAnalysis::trackisol(const reco::GsfTrackRef tr, const reco::TrackCollection* isoTracks, double zvtx)
{ 
  // Just to insure consistency with no-vertex-code
  GlobalPoint vtx(0,0,zvtx);
  reco::Track::Vector p = tr->momentum();
  GlobalVector mom( p.x(), p.y(), p.z() );
  return findIsoTracks(mom,vtx,isoTracks,true,true);
}


//-------------------------------------------------------------------------
void PixelMatchAnalysis::searchmax(const reco::PixelMatchGsfElectronCollection* electrons, int &prime, int &second) 
{
  reco::SuperClusterRef aClus;
  int icluster=0;
  int icluster1=0;
  int icluster2=0;

  double Et;
  double eta,theta;
  double  maxenergy1,maxenergy2;
  Et=maxenergy1=maxenergy2=eta=theta=0.;

  // loop over the super clusters and do the comparisons of energies
  for(reco::PixelMatchGsfElectronCollection::const_iterator eleiterator = electrons->begin(); eleiterator != electrons->end(); eleiterator++)
    {
    aClus = eleiterator->superCluster();
    eta = aClus->position().Eta();
    theta = 2*atan( exp( -1. * eta ) );
    Et = aClus->energy() * sin(theta);
    
    if (debug) {
      LogDebug("Zprime2eeAna")<<"energie, eta, theta and phi and Et of the supercluster being processed in searchmax "<<icluster;
      LogDebug("Zprime2eeAna")<<aClus->energy()<<"  "<<aClus->position().Eta()<<"  "<<theta<<"  "<<aClus->position().Phi()<<"  "<<Et;
    }

    if( Et > maxenergy1 ) {
      maxenergy2=maxenergy1;icluster2=icluster1;
      maxenergy1=Et;icluster1=icluster;
    }
    if (  (Et > maxenergy2) && (Et < maxenergy1)  ) {
      maxenergy2=Et;icluster2=icluster;
    }
  
    icluster++;
  }//end of loop over gsf collection

  if (debug) { LogDebug("Zprime2eeAna")<<"found cluster number:  "<<icluster1<<" and  "<<icluster2<<" "; }

  prime=icluster1;
  second=icluster2;

}//end of searchmax method






//-------------------------------------------------------------------------
void  PixelMatchAnalysis::vertexcorr(HepLorentzVector elecvector, HepLorentzVector & elecvector_vtxcor, double recozvtx, double radius, double tradius) {

 
  if(recozvtx!=0.) {
    
    float etacor_el = asinh( sinh(elecvector.eta())-recozvtx/tradius );
    //      float ptcor_el=(elecvector.et()*cosh(elecvector.eta())/cosh(etacor_el));
    elecvector_vtxcor.setREtaPhi(elecvector.e(),etacor_el,elecvector.phi());
    elecvector_vtxcor.setE(elecvector.e());
    /*
      if(debug) {
      LogDebug("Zprime2eeAna")<<"in PixelMatchAnalysis::vertexcorr method ";
      LogDebug("Zprime2eeAna")<<" recovertex z  and tradius "<<recozvtx<<" "<<tradius;
      LogDebug("Zprime2eeAna") << "  elecvector.theta() = " <<  elecvector.theta() <<  endl;
      LogDebug("Zprime2eeAna") << "  elecvector.e()*sin(elecvector.theta() )  = " <<  elecvector.e()*sin(elecvector.theta() )  <<  endl;
      LogDebug("Zprime2eeAna") << "  elecvector.et()/elecvector.e()= " << elecvector.et()/elecvector.e() <<  endl;
      LogDebug("Zprime2eeAna") << "  tradius/radius = " << tradius/radius  <<  endl;
      LogDebug("Zprime2eeAna") << "  elecvector_vtxcor.et()/elecvector_vtxcor.e()= " << elecvector_vtxcor.et()/elecvector_vtxcor.e() <<  endl;
      LogDebug("Zprime2eeAna") << "  ptcor_el= " << ptcor_el <<  endl;
      LogDebug("Zprime2eeAna") << "  radius= " << radius <<  endl;
      LogDebug("Zprime2eeAna") << "  tradius= " << tradius <<  endl;
      }
    */
    
  }
}



// main code 
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void PixelMatchAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
{
  using namespace edm;
  using namespace std; 
  using namespace reco;

  if (debug)
   {
    LogDebug("Zprime2eeAna") << "=======================================================" ; 
    LogDebug("Zprime2eeAna") << "iEvent.id().run() = " << iEvent.id().run() << "  iEvent.id().event() = " << iEvent.id().event()  ; 
    LogDebug("Zprime2eeAna") << "=======================================================" ; 
   }
  histo_run->Fill(iEvent.id().run() ,gsf_weight);
  histo_event->Fill(iEvent.id().event() ,gsf_weight);

  // Get the barrel hcal hits
  //edm::Handle<HBHERecHitCollection> hhitBarrelHandle;
  //try {iEvent.getByLabel("hbhereco", hhitBarrelHandle);} catch (cms::Exception& ex) { }
  //const HBHERecHitCollection* hcalhitBarrelCollection = hhitBarrelHandle.product();
  // Hcal Rechits are unavailable in AOD => use CaloTowers instead
  edm::Handle<CaloTowerCollection> hhitBarrelHandle;
  //try {iEvent.getByLabel("caloTowers", hhitBarrelHandle);} catch (cms::Exception& ex) { }
  iEvent.getByLabel("towerMaker", hhitBarrelHandle);
  const CaloTowerCollection* towerCollection = hhitBarrelHandle.product();

  LogDebug("Zprime2eeAna") << "calotowers";
  //Get Calo Geometry
  edm::ESHandle<CaloGeometry> pG;
  iSetup.get<IdealGeometryRecord>().get(pG);
  const CaloGeometry* caloGeom = pG.product();

  LogDebug("Zprime2eeAna") << "calogeometry";

  //Reconstructed vertexes
  edm::Handle<reco::VertexCollection> vertices;
  try {iEvent.getByLabel("offlinePrimaryVerticesFromCTFTracks",vertices);} catch (cms::Exception& ex) { }
  const VertexCollection* VertexData = vertices.product();

  LogDebug("Zprime2eeAna") << "vertex";

  // Get the general tracks
  edm::Handle<reco::TrackCollection> trackHandle;
  try {iEvent.getByLabel("ctfWithMaterialTracks", trackHandle);} catch (cms::Exception& ex) { }
  const reco::TrackCollection* trackcoll = trackHandle.product();

  LogDebug("Zprime2eeAna") << "tracks";

  //totla number of events on which the code is runed:
  event_tot++;

  // test root tree:
  valeur=event_tot;
  //mybranch->Fill();

  LogDebug("Zprime2eeAna") << "tree";

  // HepLorentzVector and SuperClusterRef initialisation:
  HepLorentzVector superclus;
  HepLorentzVector sc_el;
  HepLorentzVector sc_po;
  HepLorentzVector sc_el_vtxcor;
  HepLorentzVector sc_po_vtxcor;

  HepLorentzVector sc_raw_el;
  HepLorentzVector sc_raw_po;
  HepLorentzVector sc_raw_presh_el;
  HepLorentzVector sc_raw_presh_po;

  HepLorentzVector track_el;
  HepLorentzVector track_po;
  HepLorentzVector hyb_el;
  HepLorentzVector hyb_po;

  HepLorentzVector superclusone;
  HepLorentzVector superclustwo;

  reco::SuperClusterRef sclus;
  reco::SuperClusterRef scluster;
  reco::SuperClusterRef sclusterone;
  reco::SuperClusterRef sclustertwo;
  reco::SuperClusterRef scluselec;
  reco::SuperClusterRef sclusposi;

  // private variable initialisation:
  int hottest=0;
  int sndhottest=0;
  double chargeone=0.;
  double chargetwo=0.;
  double rad_el = 0.;
  double radt_el = 0.;
  double rad_po = 0.;
  double radt_po = 0.;
  double sclusterenergy = 0.;
  double sclustertheta = 0.;


  // primary vertex reco:  -----------------------------
  double recozvtx = 0.;

  LogDebug("Zprime2eeAna") << "vtxloop";

  for(reco::VertexCollection::const_iterator avertex = VertexData->begin();avertex != VertexData->end();++avertex) {
    recozvtx = avertex->z();
  }
  LogDebug("Zprime2eeAna") << " vxt:reco:   recozvtx= " << recozvtx <<  endl;
  


  // run on PixelMatchGsfElectronCollection -----------------------------------------
  reco::PixelMatchGsfElectronCollection::const_iterator gsfhottest;
  reco::PixelMatchGsfElectronCollection::const_iterator gsfsndhottest;

  Handle<reco::PixelMatchGsfElectronCollection> pixelmatchelec;
  try{ iEvent.getByLabel("pixelMatchGsfElectrons","",pixelmatchelec); } catch(cms::Exception& ex) {
    LogDebug("Zprime2eeAna")<<"could not found the product with producer pixelMatchGsfElectrons" << ex ;
  }
  const reco::PixelMatchGsfElectronCollection* collpixelmatchelec = pixelmatchelec.product();

  LogDebug("Zprime2eeAna") << "collpixelmatchelec->size() = " <<  collpixelmatchelec->size() ;
  gsfcoll_size->Fill(collpixelmatchelec->size() ,gsf_weight);

  typedef reco::PixelMatchGsfElectron myElectron;
  std::vector<myElectron> gsf_el;
  std::vector<myElectron> gsf_po;
  std::vector<myElectron> isol_el;

  //Get the two highest pt gsf electrons (only if gsf collection size is > 1)
  if( collpixelmatchelec->size() > 1) {
    gsf_sizegreater1++;
    // call a routine to select the two highest pt gsf electrons
    searchmax(collpixelmatchelec, hottest, sndhottest);
    
    gsfhottest = collpixelmatchelec->begin() + hottest ;
    sclusterone = gsfhottest->superCluster();
    sclusterenergy = sclusterone->rawEnergy()+sclusterone->preshowerEnergy();
    sclustertheta = 2. * atan( exp( -1. * sclusterone->eta() ) );
    superclusone.setRThetaPhi(sclusterenergy, sclustertheta, sclusterone->phi() );
    superclusone.setE(sclusterenergy);
    chargeone = gsfhottest->gsfTrack()->charge();
  
    gsfsndhottest = collpixelmatchelec->begin() + sndhottest ;
    sclustertwo = gsfsndhottest->superCluster();
    sclusterenergy = sclustertwo->rawEnergy()+sclustertwo->preshowerEnergy();
    sclustertheta = 2. * atan( exp( -1. * sclustertwo->eta() ) );
    superclustwo.setRThetaPhi(sclusterenergy, sclustertheta, sclustertwo->phi() );
    superclustwo.setE(sclusterenergy);
    chargetwo = gsfsndhottest->gsfTrack()->charge();

     if(debug) {
      LogDebug("Zprime2eeAna")<<"gsfelec (hottest): raw e = "<<sclusterone->rawEnergy()<<" presh e = "<<sclusterone->preshowerEnergy() << " pt= " << superclusone.et() <<  " eta= "<<superclusone.eta()<<" phi= "<<superclusone.phi();
      LogDebug("Zprime2eeAna")<<"gsfelec (hottest): SC info:  e = "<<sclusterone->energy() <<  endl;
      LogDebug("Zprime2eeAna")<<"gsfelec (second hottest): raw e = "<<sclustertwo->rawEnergy()<<" presh e = "<<sclustertwo->preshowerEnergy() << " pt= " << superclustwo.et() << " eta= "<<superclustwo.eta()<<" phi= "<<superclustwo.phi();
      LogDebug("Zprime2eeAna")<<"gsfelec (second hottest): SC info:  e = "<<sclustertwo->energy() ;
      LogDebug("Zprime2eeAna") << " chargeone = " << chargeone <<  " chargetwo = " << chargetwo ;
    }
     
  }

  //check min pt and eta

  int gsf_acc=0;
  if(superclusone.et() > ptrecmin_ && superclustwo.et() > ptrecmin_ ) {
    gsf_el_po_ptmin++;
    if( fabs(superclusone.eta()) < etarecmin_ && fabs(superclustwo.eta()) < etarecmin_ ) {
      gsf_el_po_acc++;
      gsf_acc=1;
    }
  }

  //check charges
  if( gsf_acc==1 && (chargeone * chargetwo ) < 0 ) {
    gsf_goodcharge++;
    if(debug) { LogDebug("Zprime2eeAna") << " FOUND: 2 GSF electrons with opossit charge  "; }
  
  if(chargeone < 0.) {
    sc_el=superclusone;
    gsf_el.push_back(*gsfhottest);
    sc_po=superclustwo;
    gsf_po.push_back(*gsfsndhottest);
    scluselec=sclusterone;
    sclusposi=sclustertwo;
    rad_el = sqrt(  (scluselec->x()*scluselec->x()) + (scluselec->y()*scluselec->y()) + (scluselec->z()*scluselec->z())  );
    radt_el = sqrt(  (scluselec->x()*scluselec->x()) + (scluselec->y()*scluselec->y())  );
    rad_po = sqrt(  (sclusposi->x()*sclusposi->x()) + (sclusposi->y()*sclusposi->y()) + (sclusposi->z()*sclusposi->z())  );
    radt_po = sqrt(  (sclusposi->x()*sclusposi->x()) + (sclusposi->y()*sclusposi->y())   );
  }

  if(chargeone > 0.) {
    sc_po=superclusone;
    gsf_po.push_back(*gsfhottest);
    sc_el=superclustwo;
    gsf_el.push_back(*gsfsndhottest);
    scluselec=sclustertwo;
    sclusposi=sclusterone;
    rad_el = sqrt(  (scluselec->x()*scluselec->x()) + (scluselec->y()*scluselec->y()) + (scluselec->z()*scluselec->z())  );
    radt_el = sqrt(  (scluselec->x()*scluselec->x()) + (scluselec->y()*scluselec->y())   );
    rad_po = sqrt(  (sclusposi->x()*sclusposi->x()) + (sclusposi->y()*sclusposi->y()) + (sclusposi->z()*sclusposi->z())  );
    radt_po = sqrt(  (sclusposi->x()*sclusposi->x()) + (sclusposi->y()*sclusposi->y())  );
  }


  //Consider only events with  one gsf elec and one gsf posi
  if(  (gsf_el.size() > 0) && (gsf_po.size() > 0)  ) {
 
    //Compute the correct eta (from primary vertex position and not from (0,0,0))
    sc_el_vtxcor = sc_el;
    sc_po_vtxcor = sc_po;
    vertexcorr(sc_el, sc_el_vtxcor, recozvtx, rad_el, radt_el);
    vertexcorr(sc_po, sc_po_vtxcor, recozvtx, rad_po, radt_po);

    if (debug) {
      LogDebug("Zprime2eeAna") << "vertex correction for eta:" ;
      LogDebug("Zprime2eeAna") << "el : et,e,eta,phi before: " << sc_el.et()<<" " <<sc_el.e()<<" " <<sc_el.eta()<<" " <<sc_el.phi();
      LogDebug("Zprime2eeAna") << "el : et,e,eta,phi  after: " << sc_el_vtxcor.et()<<" " <<sc_el_vtxcor.e()<<" " <<sc_el_vtxcor.eta()<<" " <<sc_el_vtxcor.phi();
      LogDebug("Zprime2eeAna") << "po : et,e,eta,phi before: " << sc_po.et()<<" " <<sc_po.e()<<" " <<sc_po.eta()<<" " <<sc_po.phi();
      LogDebug("Zprime2eeAna") << "po : et,e,eta,phi  after: " << sc_po_vtxcor.et()<<" " <<sc_po_vtxcor.e()<<" " <<sc_po_vtxcor.eta()<<" " <<sc_po_vtxcor.phi();
    }

    //Compute the invariant mass and fill histos before isolation cuts
    float  sc_mass=(sc_po+sc_el).m();
    float  sc_mass_vtxcor=(sc_po_vtxcor+sc_el_vtxcor).m();
    gsf_Z_m->Fill(sc_mass ,gsf_weight);
    gsf_Z_vtxcor_m->Fill(sc_mass_vtxcor ,gsf_weight);
    gsf_el_pt->Fill(sc_el.et() ,gsf_weight);
    gsf_po_pt->Fill(sc_po.et() ,gsf_weight);
    gsf_el_vtxcor_pt->Fill(sc_el_vtxcor.et() ,gsf_weight);
    gsf_po_vtxcor_pt->Fill(sc_po_vtxcor.et() ,gsf_weight);
    
    // Compute the isolation variables and fill histos

    // 1. tracker isolation  
    const reco::GsfTrackRef trackelec = gsf_el[0].gsfTrack();
    const reco::GsfTrackRef trackposi = gsf_po[0].gsfTrack();
    std::pair<int,float> testelec = trackisol(trackelec, trackcoll, recozvtx);
    std::pair<int,float> testposi = trackisol(trackposi, trackcoll, recozvtx);
  
    gsf_el_track_isol_abs->Fill(testelec.second ,gsf_weight);
    gsf_el_track_isol->Fill(testelec.second/sc_el.et() ,gsf_weight);
    gsf_po_track_isol_abs->Fill(testposi.second ,gsf_weight);
    gsf_po_track_isol->Fill(testposi.second/sc_po.et() ,gsf_weight);
    gsf_el_numbertrackisol->Fill(testelec.first ,gsf_weight);
    gsf_po_numbertrackisol->Fill(testposi.first ,gsf_weight);

    if(debug) {
      LogDebug("Zprime2eeAna")<<" gsf_el_track_isol_abs = "<<testelec.second;
      LogDebug("Zprime2eeAna")<<" gsf_el_track_isol = "<<testelec.second/sc_el.et();
      LogDebug("Zprime2eeAna")<<" gsf_po_track_isol_abs = "<<testposi.second;
      LogDebug("Zprime2eeAna")<<" gsf_po_track_isol = "<<testposi.second/sc_po.et();
      LogDebug("Zprime2eeAna")<<" #tracks for e- "<<testelec.first<<" and pt sum e-"<<testelec.second;
      LogDebug("Zprime2eeAna")<<" #tracks for e+ "<<testposi.first<<" and pt sum e+"<<testposi.second;
    }


    // 2. HCAL isolation  
    double elec_hcalisol = hcaletisol(scluselec,towerCollection);
    double posi_hcalisol = hcaletisol(sclusposi,towerCollection);

    gsf_el_hcal_isol_abs->Fill(elec_hcalisol ,gsf_weight);
    gsf_el_hcal_isol->Fill(elec_hcalisol/sc_el.et() ,gsf_weight);
    gsf_po_hcal_isol_abs->Fill(posi_hcalisol ,gsf_weight);
    gsf_po_hcal_isol->Fill(posi_hcalisol/sc_po.et() ,gsf_weight);

    if(debug) {
      LogDebug("Zprime2eeAna")<<" gsf_el_hcal_isol_abs = "<<elec_hcalisol;
      LogDebug("Zprime2eeAna")<<" gsf_el_hcal_isol = "<<elec_hcalisol/sc_el.et();
      LogDebug("Zprime2eeAna")<<" gsf_po_hcal_isol_abs = "<<posi_hcalisol;
      LogDebug("Zprime2eeAna")<<" gsf_po_hcal_isol = "<<posi_hcalisol/sc_po.et();
    }

    // 3. ECAL isolation  
    double elec_ecalisol = ecaletisol(iEvent,scluselec);
    double posi_ecalisol = ecaletisol(iEvent,sclusposi);

    gsf_el_ecal_isol_abs->Fill(elec_ecalisol ,gsf_weight);
    gsf_el_ecal_isol->Fill(elec_ecalisol/sc_el.et() ,gsf_weight);
    gsf_po_ecal_isol_abs->Fill(posi_ecalisol ,gsf_weight);
    gsf_po_ecal_isol->Fill(posi_ecalisol/sc_po.et() ,gsf_weight);
 
    if(debug) {
      LogDebug("Zprime2eeAna")<<" gsf_el_ecal_isol_abs = "<< elec_ecalisol;
      LogDebug("Zprime2eeAna")<<" gsf_el_ecal_isol = "<<elec_ecalisol/sc_el.et();
      LogDebug("Zprime2eeAna")<<" gsf_po_ecal_isol_abs = "<<posi_ecalisol;
      LogDebug("Zprime2eeAna")<<" gsf_po_ecal_isol = "<<posi_ecalisol/sc_po.et();
    }


  // Cut on Isolation variables

  if(testelec.first < ntrackisol_max_ && testposi.first < ntrackisol_max_) {
    gsf_ntkisolated++;
  if(testelec.second/sc_el.et() < trackisol_max_ && testposi.second/sc_po.et() < trackisol_max_) {
    gsf_tkisolated++;
	
    if(elec_hcalisol/sc_el.et() < hcalisol_max_ && posi_hcalisol/sc_po.et() < hcalisol_max_) {
      gsf_hcalisolated++;

      if(elec_ecalisol/sc_el.et() < ecalisol_max_ && posi_ecalisol/sc_po.et() < ecalisol_max_) {
	gsf_ecalisolated++;




        //Compute the invariant mass and fill histos AFTER isolation cuts
        float  sc_mass=(sc_po+sc_el).m();
        float  sc_mass_vtxcor=(sc_po_vtxcor+sc_el_vtxcor).m();
        gsf_Z_iso_m->Fill(sc_mass ,gsf_weight);
        gsf_Z_vtxcor_iso_m->Fill(sc_mass_vtxcor ,gsf_weight);
        gsf_Z_iso_e->Fill((sc_po_vtxcor+sc_el_vtxcor).e() ,gsf_weight);
        gsf_Z_iso_pt->Fill((sc_po_vtxcor+sc_el_vtxcor).et() ,gsf_weight);
        gsf_Z_iso_eta->Fill((sc_po_vtxcor+sc_el_vtxcor).eta() ,gsf_weight);
        gsf_Z_iso_phi->Fill((sc_po_vtxcor+sc_el_vtxcor).phi() ,gsf_weight);

        gsf_el_iso_pt->Fill(sc_el.et() ,gsf_weight);
        gsf_po_iso_pt->Fill(sc_po.et() ,gsf_weight);
        gsf_el_iso_eta->Fill(sc_el.eta() ,gsf_weight);
        gsf_po_iso_eta->Fill(sc_po.eta() ,gsf_weight);

        gsf_el_vtxcor_iso_pt->Fill(sc_el_vtxcor.et() ,gsf_weight);
        gsf_po_vtxcor_iso_pt->Fill(sc_po_vtxcor.et() ,gsf_weight);
        gsf_el_vtxcor_iso_eta->Fill(sc_el_vtxcor.eta() ,gsf_weight);
        gsf_po_vtxcor_iso_eta->Fill(sc_po_vtxcor.eta() ,gsf_weight);
        gsf_el_vtxcor_iso_e->Fill(sc_el_vtxcor.e() ,gsf_weight);
        gsf_po_vtxcor_iso_e->Fill(sc_po_vtxcor.e() ,gsf_weight);
        gsf_el_vtxcor_iso_phi->Fill(sc_el_vtxcor.phi() ,gsf_weight);
        gsf_po_vtxcor_iso_phi->Fill(sc_po_vtxcor.phi() ,gsf_weight);
	
	//Fill variables for the tree
	gsf_Z_iso_m_var = sc_mass;
	gsf_Z_vtxcor_iso_m_var = sc_mass_vtxcor;
	gsf_Z_iso_e_var = (sc_po_vtxcor+sc_el_vtxcor).e(); 
	gsf_Z_iso_pt_var = (sc_po_vtxcor+sc_el_vtxcor).et(); 
	gsf_Z_iso_eta_var = (sc_po_vtxcor+sc_el_vtxcor).eta(); 
	gsf_Z_iso_phi_var = (sc_po_vtxcor+sc_el_vtxcor).phi();

	gsf_el_iso_pt_var = sc_el.et();
	gsf_po_iso_pt_var = sc_po.et();
	gsf_el_iso_eta_var = sc_el.eta();
	gsf_po_iso_eta_var = sc_po.eta();

	gsf_el_vtxcor_iso_pt_var = sc_el_vtxcor.et();
	gsf_po_vtxcor_iso_pt_var = sc_po_vtxcor.et(); 
	gsf_el_vtxcor_iso_eta_var = sc_el_vtxcor.eta();
	gsf_po_vtxcor_iso_eta_var = sc_po_vtxcor.eta();
	gsf_el_vtxcor_iso_e_var = sc_el_vtxcor.e();
	gsf_po_vtxcor_iso_e_var = sc_po_vtxcor.e(); 
	gsf_el_vtxcor_iso_phi_var = sc_el_vtxcor.phi();
	gsf_po_vtxcor_iso_phi_var = sc_po_vtxcor.phi();

	//Fill the tree
	mytree->Fill();

      }//end of ecal isolation cut
    }//end of hcal isolation cut
  }//end of tracker isolation cut 
  }

}
}
}

// ------------ method called once each job just after ending the event loop  ------------
void PixelMatchAnalysis::endJob() {

  double eff1=double(gsf_sizegreater1)/double(event_tot);
  double eff2=double(gsf_el_po_ptmin)/double(gsf_sizegreater1);
  double eff3=double(gsf_el_po_acc)/double(gsf_el_po_ptmin);
  double eff4=double(gsf_goodcharge)/double(gsf_el_po_acc);
  double eff5=double(gsf_ntkisolated)/double(gsf_goodcharge);
  double eff6=double(gsf_tkisolated)/double(gsf_ntkisolated);
  double eff7=double(gsf_hcalisolated)/double(gsf_tkisolated);
  double eff8=double(gsf_ecalisolated)/double(gsf_hcalisolated);
  double efffinal=double(gsf_ecalisolated)/double(event_tot);

  edm::LogVerbatim("Zprime2eeAna")<<"";
  edm::LogVerbatim("Zprime2eeAna")<<"";
  edm::LogVerbatim("Zprime2eeAna")<<"--------------------------------------------------------";
  edm::LogVerbatim("Zprime2eeAna")<<"SUMMARY of number of events Gsf/Gsf";
  edm::LogVerbatim("Zprime2eeAna")<<"N tot                       "<< event_tot <<",    eff=1.00";
  edm::LogVerbatim("Zprime2eeAna")<<"--------------------------------------------------------";
  edm::LogVerbatim("Zprime2eeAna")<<"N >= 2e GSF REC:              "<<gsf_sizegreater1<<",    eff="<<eff1 ; 
  edm::LogVerbatim("Zprime2eeAna")<<"     + pt>cut                 "<<gsf_el_po_ptmin <<",    eff="<<eff2 ; 
  edm::LogVerbatim("Zprime2eeAna")<<"     + eta<cut                "<<gsf_el_po_acc   <<",    eff="<<eff3 ;  
  edm::LogVerbatim("Zprime2eeAna")<<"2GSF + good charge:           "<<gsf_goodcharge  <<",    eff="<<eff4 ;  
  edm::LogVerbatim("Zprime2eeAna")<<" + gsf_ntkisolated            "<<gsf_ntkisolated <<",    eff="<<eff5 ;  
  edm::LogVerbatim("Zprime2eeAna")<<" + gsf_tkisolated             "<<gsf_tkisolated  <<",    eff="<<eff6 ;  
  edm::LogVerbatim("Zprime2eeAna")<<" + gsf_hcalisolated           "<<gsf_hcalisolated<<",    eff="<<eff7 ; 
  edm::LogVerbatim("Zprime2eeAna")<<" + gsf_ecalisolated           "<<gsf_ecalisolated<<",    eff="<<eff8 ; 
  edm::LogVerbatim("Zprime2eeAna")<<"--------------------------------------------------------";
  edm::LogVerbatim("Zprime2eeAna")<<"----------------- Totl eff              ="<<efffinal;
  edm::LogVerbatim("Zprime2eeAna") << "" ;
}
