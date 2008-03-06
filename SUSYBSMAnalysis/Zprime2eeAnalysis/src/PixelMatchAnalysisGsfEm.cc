// -*- C++ -*-
// O.Charaf, B.Clerbaux, M.Mozer
// Package:    PixelMatchAnalysis
// Class:      PixelMatchAnalysis
// To look at the main code: go directly to: PixelMatchAnalysisGsfEm::analyze
// ------------------------------------------------------

#include "SUSYBSMAnalysis/Zprime2eeAnalysis/interface/PixelMatchAnalysisGsfEm.h"
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
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"


#include <vector>
 
#define PI 3.141592654
#define TWOPI 6.283185308

using namespace std;


// ------------ constuctor   -------------------------------
PixelMatchAnalysisGsfEm::PixelMatchAnalysisGsfEm(const edm::ParameterSet& iConfig)
{

  //mytree = new TTree("tree","tr");

  //Define variables for branches
  sc_Z_iso_m_var = 0.;
  sc_Z_vtxcor_iso_m_var = 0.;
  sc_Z_iso_e_var = 0.; 
  sc_Z_iso_pt_var = 0.; 
  sc_Z_iso_eta_var = 0.; 
  sc_Z_iso_phi_var = 0.;

  sc_one_iso_pt_var = 0.;
  sc_two_iso_pt_var = 0.;
  sc_one_iso_eta_var = 0.;
  sc_two_iso_eta_var = 0.;

  sc_one_vtxcor_iso_pt_var = 0.;
  sc_two_vtxcor_iso_pt_var = 0.; 
  sc_one_vtxcor_iso_eta_var = 0.;
  sc_two_vtxcor_iso_eta_var = 0.;
  sc_one_vtxcor_iso_e_var = 0.;
  sc_two_vtxcor_iso_e_var = 0.; 
  sc_one_vtxcor_iso_phi_var = 0.;
  sc_two_vtxcor_iso_phi_var = 0.;

  sc_weight = 0.;
 
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

  debug=1;
  event_tot=0;
  gsf_sizegreater0=0;
  sc_sizegreater1=0;
  sc_ntkisolated = 0;
  sc_tkisolated = 0;
  sc_hcalisolated = 0;
  sc_ecalisolated = 0;
  sc_one_two_ptmin = 0;
  sc_one_two_acc = 0;

//  if (debug) {
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"Parameters for Gsf/EM:";
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"===============================================";
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"drellyan_ = " << drellyan_ ;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"rootanalfile_ = " << rootanalfile_ ;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"ecalconesize_ = " << ecalconesize_ ;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"hcalconesizemin_ = " << hcalconesizemin_ ;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"hcalconesizemax_ = " << hcalconesizemax_ ;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"hcalptMin_ = " << hcalptMin_ ;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"trackptMin_ = " << trackptMin_ ;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"trackconesize_ = " <<trackconesize_;;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"rspan_ = "<<rspan_;;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"zspan_ = "<<zspan_;;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"vetoConesize_ = "<<vetoConesize_;;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"jet_ = "<<jet_;;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"ptrecmin_ = "<<ptrecmin_;;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"etarecmin_ = "<<etarecmin_;;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"ntrackisol_max_ = "<<ntrackisol_max_;;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"trackisol_max_ = "<<trackisol_max_;;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"hcalisol_max_ = "<<hcalisol_max_;;
    edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"ecalisol_max_ = "<<ecalisol_max_;;
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"===============================================";

//  }
}


// ------------ destructor  -------------------------------
PixelMatchAnalysisGsfEm::~PixelMatchAnalysisGsfEm()
{
}


// ------------ method called once each job just before starting event loop  ------------
void PixelMatchAnalysisGsfEm::beginJob(const edm::EventSetup&)
{
  edm::Service<TFileService> fs;


  //  HISTO INITIALISATION :
  
  // histos: on all events:
  histo_run = fs->make<TH1I>("histo_run","histo_run",2000,0.,2000.);
  histo_event = fs->make<TH1I>("histo_event","histo_event",100,0.,10000.);
  sccoll_size = fs->make<TH1I>("sccoll_size","sccoll_size",20,0.,20.);
  gsfcoll_size = fs->make<TH1I>("gsfcoll_size","gsfcoll_size",20,0.,20.);

  // histos: before isolation cuts:
  sc_Z_m= fs->make<TH1F>("sc_Z_m","sc_Z_m",600,0.,3000.);
  sc_Z_vtxcor_m= fs->make<TH1F>("sc_Z_vtxcor_m","sc_Z_vtxcor_m",600,0.,3000.);
  sc_one_pt= fs->make<TH1F>("sc_one_pt","sc_one_pt",400,0.,2000.);
  sc_two_pt= fs->make<TH1F>("sc_two_pt","sc_two_pt",400,0.,2000.);
  sc_one_vtxcor_pt= fs->make<TH1F>("sc_one_vtxcor_pt","sc_one_vtxcor_pt",400,0.,2000.);
  sc_two_vtxcor_pt= fs->make<TH1F>("sc_two_vtxcor_pt","sc_two_vtxcor_pt",400,0.,2000.);

  //histos for tracker isolation 
  sc_one_numbertrackisol = fs->make<TH1F>("sc_one_numbertrackisol","sc_one_numbertrackisol",200,0.,20.);
  sc_one_track_isol_abs = fs->make<TH1F>("sc_one_track_isol_abs","sc_one_track_isol_abs",200,0.,200.);
  sc_one_track_isol = fs->make<TH1F>("sc_one_track_isol","sc_one_track_isol",200,0.,2.);
  sc_two_numbertrackisol = fs->make<TH1F>("sc_two_numbertrackisol","sc_two_numbertrackisol",200,0.,20.);
  sc_two_track_isol_abs = fs->make<TH1F>("sc_two_track_isol_abs","sc_two_track_isol_abs",200,0.,200.);
  sc_two_track_isol = fs->make<TH1F>("sc_two_track_isol","sc_two_track_isol",200,0.,2.);
  //histos for hcal isolation 
  sc_one_hcal_isol = fs->make<TH1F>("sc_one_hcal_isol","sc_one_hcal_isol",200,0.,2.);
  sc_one_hcal_isol_abs = fs->make<TH1F>("sc_one_hcal_isol_abs","sc_one_hcal_isol_abs",200,0.,200.);
  sc_two_hcal_isol = fs->make<TH1F>("sc_two_hcal_isol","sc_two_hcal_isol",200,0.,2.);
  sc_two_hcal_isol_abs = fs->make<TH1F>("sc_two_hcal_isol_abs","sc_two_hcal_isol_abs",200,0.,200.);
  //histos for ecal isolation 
  sc_one_ecal_isol = fs->make<TH1F>("sc_one_ecal_isol","sc_one_ecal_isol",200,0.,2.);
  sc_one_ecal_isol_abs = fs->make<TH1F>("sc_one_ecal_isol_abs","sc_one_ecal_isol_abs",200,0.,200.);
  sc_two_ecal_isol = fs->make<TH1F>("sc_two_ecal_isol","sc_two_ecal_isol",200,0.,2.);
  sc_two_ecal_isol_abs = fs->make<TH1F>("sc_two_ecal_isol_abs","sc_two_ecal_isol_abs",200,0.,200.);

  // histos: after isolation cuts:
  sc_Z_iso_m= fs->make<TH1F>("sc_Z_iso_m","sc_Z_iso_m",600,0.,3000.);
  sc_Z_vtxcor_iso_m= fs->make<TH1F>("sc_Z_vtxcor_iso_m","sc_Z_vtxcor_iso_m",600,0.,3000.);
  sc_Z_iso_e= fs->make<TH1F>("sc_Z_iso_e","sc_Z_iso_e",400,0.,4000.);
  sc_Z_iso_pt= fs->make<TH1F>("sc_Z_iso_pt","sc_Z_iso_pt",400,0.,2000.);
  sc_Z_iso_eta= fs->make<TH1F>("sc_Z_iso_eta","sc_Z_iso_eta",200,-10.,10.);
  sc_Z_iso_phi= fs->make<TH1F>("sc_Z_iso_phi","sc_Z_iso_phi",160,-4.,4.);

  sc_one_iso_pt= fs->make<TH1F>("sc_one_iso_pt","sc_one_iso_pt",400,0.,2000.);
  sc_two_iso_pt= fs->make<TH1F>("sc_two_iso_pt","sc_two_iso_pt",400,0.,2000.);
  sc_one_iso_eta = fs->make<TH1F>("sc_one_iso_eta","sc_one_iso_eta",120,-3.,3.);
  sc_two_iso_eta = fs->make<TH1F>("sc_two_iso_eta","sc_two_iso_eta",120,-3.,3.);

  sc_one_vtxcor_iso_pt= fs->make<TH1F>("sc_one_vtxcor_iso_pt","sc_one_vtxcor_iso_pt",400,0.,2000.);
  sc_two_vtxcor_iso_pt= fs->make<TH1F>("sc_two_vtxcor_iso_pt","sc_two_vtxcor_iso_pt",400,0.,2000.);
  sc_one_vtxcor_iso_eta = fs->make<TH1F>("sc_one_vtxcor_iso_eta","sc_one_vtxcor_iso_eta",120,-3.,3.);
  sc_two_vtxcor_iso_eta = fs->make<TH1F>("sc_two_vtxcor_iso_eta","sc_two_vtxcor_iso_eta",120,-3.,3.);
  sc_one_vtxcor_iso_e = fs->make<TH1F>("sc_one_vtxcor_iso_e","sc_one_vtxcor_iso_e",400,0.,4000.);
  sc_two_vtxcor_iso_e = fs->make<TH1F>("sc_two_vtxcor_iso_e","sc_two_vtxcor_iso_e",400,0.,4000.);
  sc_one_vtxcor_iso_phi = fs->make<TH1F>("sc_one_vtxcor_iso_phi","sc_one_vtxcor_iso_phi",160,-4.,4.);
  sc_two_vtxcor_iso_phi = fs->make<TH1F>("sc_two_vtxcor_iso_phi","sc_two_vtxcor_iso_phi",160,-4.,4.);

  //Define tree
  mytree = fs->make<TTree>("tree","tr");
 
  //Define branches
  mytree->Branch("sc_Z_iso_m_branch",&sc_Z_iso_m_var,"sc_Z_iso_m_branch/F");
  mytree->Branch("sc_Z_vtxcor_iso_m_branch",&sc_Z_vtxcor_iso_m_var,"sc_Z_vtxcor_iso_m_branch/F");
  mytree->Branch("sc_Z_iso_e_branch",&sc_Z_iso_e_var ,"sc_Z_iso_e_branch/F");
  mytree->Branch("sc_Z_iso_pt_branch",&sc_Z_iso_pt_var,"sc_Z_iso_pt_branch/F");
  mytree->Branch("sc_Z_iso_eta_branch",&sc_Z_iso_eta_var,"sc_Z_iso_eta_branch/F");
  mytree->Branch("sc_Z_iso_phi_branch",&sc_Z_iso_phi_var,"sc_Z_iso_phi_branch/F");
  
  mytree->Branch("sc_one_iso_pt_branch",&sc_one_iso_pt_var,"sc_one_iso_pt_branch/F");
  mytree->Branch("sc_two_iso_pt_branch",&sc_two_iso_pt_var,"sc_two_iso_pt_branch/F");
  mytree->Branch("sc_one_iso_eta_branch",&sc_one_iso_eta_var,"sc_one_iso_eta_branch/F");
  mytree->Branch("sc_two_iso_eta_branch",&sc_two_iso_eta_var,"sc_two_iso_eta_branch/F");
  
  mytree->Branch("sc_one_vtxcor_iso_pt_branch",&sc_one_vtxcor_iso_pt_var,"sc_one_vtxcor_iso_pt_branch/F");
  mytree->Branch("sc_two_vtxcor_iso_pt_branch",&sc_two_vtxcor_iso_pt_var,"sc_two_vtxcor_iso_pt_branch/F");
  mytree->Branch("sc_one_vtxcor_iso_eta_branch",&sc_one_vtxcor_iso_eta_var,"sc_one_vtxcor_iso_eta_branch/F");
  mytree->Branch("sc_two_vtxcor_iso_eta_branch",&sc_two_vtxcor_iso_eta_var,"sc_two_vtxcor_iso_eta_branch/F");
  mytree->Branch("sc_one_vtxcor_iso_e_branch",&sc_one_vtxcor_iso_e_var,"sc_one_vtxcor_iso_e_branch/F");
  mytree->Branch("sc_two_vtxcor_iso_e_branch",&sc_two_vtxcor_iso_e_var,"sc_two_vtxcor_iso_e_branch/F");
  mytree->Branch("sc_one_vtxcor_iso_phi_branch",&sc_one_vtxcor_iso_phi_var,"sc_one_vtxcor_iso_phi_branch/F");
  mytree->Branch("sc_two_vtxcor_iso_phi_branch",&sc_two_vtxcor_iso_phi_var,"sc_two_vtxcor_iso_phi_branch/F");

  mytree->Branch("sc_weight",&sc_weight,"sc_weight/D");

}

// ---------------------------------------------------------------------------------------
double PixelMatchAnalysisGsfEm::ecaletisol( const edm::Event& Evt, reco::SuperClusterRef maxsupercluster ){

  using namespace edm; // needed for all fwk related classes
  using namespace std;
  using namespace reco;

  Handle<reco::SuperClusterCollection> pHybridSuperClusters;
  Handle<reco::SuperClusterCollection> pIslandSuperClusters;
  Handle<reco::BasicClusterCollection> pHybridBasicClusters;
  Handle<reco::BasicClusterCollection> pIslandBasicClusters;

 
  Handle<double> weightHandle;
  Evt.getByLabel ("weight","weight", weightHandle);
  sc_weight= * weightHandle;

  try {
    Evt.getByLabel("hybridSuperClusters","", pHybridSuperClusters);
    Evt.getByLabel("islandSuperClusters","islandEndcapSuperClusters", pIslandSuperClusters);
    Evt.getByLabel("hybridSuperClusters","", pHybridBasicClusters);
    Evt.getByLabel("islandBasicClusters","islandEndcapBasicClusters", pIslandBasicClusters);
  } catch ( cms::Exception& ex ) {
    LogDebug("Zprime2eeAnaGsfEm")<<"could not found the product with producer hybridSuperClusters or islandEndcapSuperClusters" << ex ;
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

  if(debug) {
      LogDebug("Zprime2eeAnaGsfEm")<<"energie(raw and presh) eta and phi of max SC "
      <<maxsupercluster->rawEnergy()<<" "
      <<maxsupercluster->preshowerEnergy()<<" "
      <<maxsupercluster->eta()<<" "
      <<maxsupercluster->phi();
  }

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

    //     if (ebc_bcet > etMin && ebc_bcalgo == 0) {
    //       if (ebc_bcchi2 < 30.) {
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
	  ecalIsol+=ebc_bcet;
	}
      }
    }
    //   } // matches ebc_bcchi2
    // } // matches ebc_bcet && ebc_bcalgo

  }
  return ecalIsol;
}

// ---------------------------------------------------------------------------------------
double PixelMatchAnalysisGsfEm::hcaletisol(reco::SuperClusterRef maxsupercluster, const CaloTowerCollection* hbhe){

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

// ---------------------------------------------------------------------------------------
double PixelMatchAnalysisGsfEm::hcaletisol(reco::SuperClusterRef maxsupercluster, const HBHERecHitCollection* hbhe, const CaloGeometry* geometry){

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

// ---------------------------------------------------------------------------------------
std::pair<int,float> PixelMatchAnalysisGsfEm::findIsoTracks(GlobalVector mom, GlobalPoint vtx,  const reco::TrackCollection* isoTracks, bool isElectron, bool useVertex, bool isgsf, bool usevertexcorr)
{
  if(debug){
    LogDebug("Zprime2eeAnaGsfEm") << " in PixelMatchAnalysisGsfEm::findIsoTracks " ;
    LogDebug("Zprime2eeAnaGsfEm")<<" vtx.z() "<<vtx.z();
    LogDebug("Zprime2eeAnaGsfEm")<<" isElectron "<<isElectron <<" useVertex "<<useVertex <<" isgsf "<<isgsf <<" usevertexcorr "<<usevertexcorr ;
  }

  // Check that reconstructed tracks fit within cone boundaries,
  // (Note: tracks will not always stay within boundaries)
  int ntrack = 0;
  float ptSum = 0.;
  float eta = 0.;
  float tradius = sqrt( mom.x()*mom.x() + mom.y()*mom.y() );

  eta = mom.eta();
  if(debug) LogDebug("Zprime2eeAnaGsfEm")<<" eta "<< eta ;
  if(usevertexcorr) eta = asinh(sinh(mom.eta())-vtx.z()/tradius );
  if(debug) LogDebug("Zprime2eeAnaGsfEm")<<" eta (cor) "<< eta ;

  for(reco::TrackCollection::const_iterator trItr = isoTracks->begin(); trItr != isoTracks->end(); ++trItr){

    GlobalPoint ivtx(trItr->vertex().x(),trItr->vertex().y(),trItr->vertex().z());
    reco::Track::Vector ip = trItr->momentum();
    GlobalVector imom ( ip.x(), ip.y(), ip.z());

    float pt = imom.perp();
    float dperp = 0.;
    float dz = 0.;
    float deta = 0.;
    float dphi = 0.;
    float R = 0.;

    if (useVertex) {
      dperp = ivtx.perp()-vtx.perp();
      dz = ivtx.z()-vtx.z();
      deta = imom.eta()-eta;
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

    R = sqrt( dphi*dphi + deta*deta );

    // Apply boundary cut
    bool selected=false;
    if (pt > trackptMin_ && R < trackconesize_ &&
	fabs(dperp) < rspan_ && fabs(dz) < zspan_) selected=true;
  

    if (selected) {
    if(debug) LogDebug("Zprime2eeAnaGsfEm")<<" track selected: R =   "<< R ;
	if (R > vetoConesize_) {     //to exclude electron track
        if(debug) LogDebug("Zprime2eeAnaGsfEm")<<" in vetoConesize_ " << "pt = " << pt ;
	  ptSum+=pt;
	  ntrack++;
	}
    }//end of if selected
  }//end of loop
    
  return (std::pair<int,float>(ntrack,ptSum));
}

//-------------------------------------------------------------------------
std::pair<int,float> PixelMatchAnalysisGsfEm::trackisol(reco::SuperClusterRef acluster, const reco::GsfTrackRef tr, const reco::TrackCollection* isoTracks, double zvtx, bool isgsf, bool usevertexcorr)
{ 
  // Just to insure consistency with no-vertex-code
  GlobalPoint vtx(0,0,zvtx);
  reco::Track::Vector p = tr->momentum();
  GlobalVector momtrack( p.x(), p.y(), p.z() );
  GlobalVector momcluster( acluster->x(), acluster->y(), acluster->z() );

  if(debug) LogDebug("Zprime2eeAnaGsfEm")<<" in PixelMatchAnalysisGsfEm::trackisol " ;
  if(debug) LogDebug("Zprime2eeAnaGsfEm")<<" momtrack "<<momtrack<<" momcluster "<<momcluster;

  if(isgsf) return findIsoTracks(momtrack,vtx,isoTracks,true,true,isgsf,!usevertexcorr);
  if(!isgsf) return findIsoTracks(momcluster,vtx,isoTracks,true,true,isgsf,usevertexcorr);
}

//-------------------------------------------------------------------------
void PixelMatchAnalysisGsfEm::searchmax( const reco::PixelMatchGsfElectronCollection* electrons, std::vector<reco::SuperClusterRef> superclusters, int &prime, int &second) 
{
  HepLorentzVector gsfhottest;
  HepLorentzVector schottest;
  reco::SuperClusterRef aClus;
  reco::SuperClusterRef aCluster;

  int icluster=0;
  int icluster1=0;
  int icluster2=0;
  double Et;
  double eta,theta,phi;
  double  maxenergy1,eta1,theta1,phi1;
  Et=maxenergy1=eta1=theta1=phi1=eta=theta=0.;

  if(debug) LogDebug("Zprime2eeAnaGsfEm")<<"size of vector in searchmax "<<electrons->size();

  // loop over the pixelmatchGsfElectrons and compare energies
  for(reco::PixelMatchGsfElectronCollection::const_iterator eleiterator = electrons->begin(); eleiterator != electrons->end(); eleiterator++)
  {
    aClus = eleiterator->superCluster();
    eta = aClus->position().Eta();
    theta = 2*atan( exp( -1. * eta ) );
    Et = aClus->energy() * sin(theta);
    
    if (debug) {
      LogDebug("Zprime2eeAnaGsfEm")<<"energie, eta, theta and phi and Et of the supercluster being processed in searchmax "<<icluster;
      LogDebug("Zprime2eeAnaGsfEm")<<aClus->energy()<<"  "<<aClus->position().Eta()<<"  "<<theta<<"  "<<aClus->position().Phi()<<"  "<<Et;
    }

    if( Et > maxenergy1 ) {
      maxenergy1=Et;icluster1=icluster;
      eta1=aClus->position().Eta();
      theta1=2*atan( exp( -1.*eta1 ) );
      phi1=aClus->position().Phi();
      gsfhottest.setRThetaPhi(aClus->energy(),theta1,phi1);
      gsfhottest.setE(aClus->energy());
    }

    icluster++;

  }//end of loop over pmgsf collection

  if (debug) {
    LogDebug("Zprime2eeAnaGsfEm")<<"number found "<<icluster1;
    LogDebug("Zprime2eeAnaGsfEm")<<" and e, et, eta and phi of hottest gsf "<<gsfhottest.e()<<" "
	<<gsfhottest.et()<<" "<<gsfhottest.eta()<<" "<<gsfhottest.phi()<<" ";
  }

  prime=icluster1;
  
   //Now we have the hottest gsf. Let's look for the hottest SC
  Et=maxenergy1=eta1=theta1=phi1=eta=theta=phi=0.;
  icluster = 0;

   // loop over the super clusters and compare energies
  for(std::vector<reco::SuperClusterRef>::const_iterator sciterator = superclusters.begin(); sciterator != superclusters.end(); sciterator++)
  {
     aCluster = *sciterator;
     eta = aCluster->eta();
     phi = aCluster->phi();

     theta = 2*atan( exp( -1. * eta ) );
     Et = aCluster->energy() * sin(theta);
     schottest.setRThetaPhi(aCluster->energy(),theta,phi);
     schottest.setE(aCluster->energy());
    
     if (debug) {
	LogDebug("Zprime2eeAnaGsfEm")<<"energie, eta, theta and phi and Et of the supercluster being processed in searchmax "<<icluster;
	LogDebug("Zprime2eeAnaGsfEm")<<aCluster->energy()<<"  "<<aCluster->position().Eta()<<"  "<<theta<<"  "<<aCluster->position().Phi()<<"  "<<Et;
	LogDebug("Zprime2eeAnaGsfEm")<<"maxenergy1 "<<maxenergy1<<" theta1 "<<theta1<<" phi1 "<<phi1;
	LogDebug("Zprime2eeAnaGsfEm")<<"schottest e eta phi et "<<schottest.e()<<" "<<schottest.eta()<<"  "<<schottest.phi()<<"  "<<schottest.et();
	LogDebug("Zprime2eeAnaGsfEm")<<"gsfhottest e eta phi et "<<gsfhottest.e()<<" "<<gsfhottest.eta()<<"  "<<gsfhottest.phi()<<"  "<<gsfhottest.et();
        LogDebug("Zprime2eeAnaGsfEm")<<" schottest.deltaR(gsfhottest) "<<schottest.deltaR(gsfhottest);
     }


     //be careful not to take the same SC of the gsf
     if(schottest.deltaR(gsfhottest) < 0.05) {
	icluster++;
        if(debug) LogDebug("Zprime2eeAnaGsfEm")<<"yes this is the same "<< " icluster = " << icluster ;
        continue;
     }

     if( Et > maxenergy1 ) {
	maxenergy1=Et;icluster2=icluster;
	eta1=aCluster->eta();
	theta1=2*atan( exp( -1.*eta1 ) );
	phi1=aCluster->phi();
	schottest.setRThetaPhi(aCluster->energy(),theta1,phi1);
	schottest.setE(aCluster->energy());
     }

     icluster++;
     if(debug) LogDebug("Zprime2eeAnaGsfEm")<<"icluster = "<<icluster;
  }

  if (debug) {
    LogDebug("Zprime2eeAnaGsfEm")<<"number found "<<icluster2;
    LogDebug("Zprime2eeAnaGsfEm")<<" and e, et, eta and phi of hottest sc "<<schottest.e()<<" "
        <<schottest.et()<<" "<<schottest.eta()<<" "<<schottest.phi()<<" ";
  }

  second=icluster2;
}//end of searchmax method


// ----------------------------------------------------------------------
void  PixelMatchAnalysisGsfEm::vertexcorr(HepLorentzVector scvector, HepLorentzVector & scvector_vtxcor, double recozvtx, double radius, double tradius)
{
  scvector_vtxcor=scvector;
  if(recozvtx!=0.) {
    float etacor_one = asinh(sinh(scvector.eta())-recozvtx/tradius );
    scvector_vtxcor.setREtaPhi(scvector.e(),etacor_one,scvector.phi());
    scvector_vtxcor.setE(scvector.e());
    }
}


// Main code to analyse data: look for a high energy e+e- pair
// asking at least one GSF electron + another SC
// ---------------------------------------------------------------------------------------
void PixelMatchAnalysisGsfEm::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std; 
  using namespace reco;

  if (debug)
    {
      LogDebug("Zprime2eeAnaGsfEm") << "=======================================================" ; 
      LogDebug("Zprime2eeAnaGsfEm") << "iEvent.id().run() = " << iEvent.id().run() << "  iEvent.id().event() = " << iEvent.id().event()  ; 
      LogDebug("Zprime2eeAnaGsfEm") << "=======================================================" ; 
    }

  histo_run->Fill(iEvent.id().run() ,sc_weight);
  histo_event->Fill(iEvent.id().event() ,sc_weight);

  //HepLorentzVector and SuperClusterRef initialisation:

  HepLorentzVector superclusone;
  HepLorentzVector superclustwo;
  HepLorentzVector superclusone_vtxcor;
  HepLorentzVector superclustwo_vtxcor;

  reco::SuperClusterRef hottestobj;
  reco::SuperClusterRef sndhottestobj;
  std::vector<reco::SuperClusterRef>::const_iterator  hottestsc;

  // private variable initialisation:
  int hottest=0;
  int sndhottest=0;
  double charge=0.;
  double rad_one = 0.;
  double radt_one = 0.;
  double rad_two = 0.;
  double radt_two = 0.;

  double sclusterenergyone = 0.;
  double sclusterthetaone = 0.;
  double sclusterphione = 0.;
  double sclusterenergytwo = 0.;
  double sclusterthetatwo = 0.;
  double sclusterphitwo = 0.;

  double gsfhottestenergy = 0.;
  double gsfhottesttheta = 0.;
  double gsfhottestet = 0.;
  double hottestscenergy = 0.;
  double hottestsctheta = 0.;
  double hottestscet = 0.;

  bool hottestIsGsf = false;

  //-------Retrieve different collections-------

  // Get the barrel hcal hits
  //edm::Handle<HBHERecHitCollection> hhitBarrelHandle;
  //try {iEvent.getByLabel("hbhereco", hhitBarrelHandle);} catch (cms::Exception& ex) { }
  //const HBHERecHitCollection* hcalhitBarrelCollection = hhitBarrelHandle.product();
  // Hcal Rechits are unavailable in AOD => use CaloTowers instead
  edm::Handle<CaloTowerCollection> hhitBarrelHandle;
  //try {iEvent.getByLabel("caloTowers", hhitBarrelHandle);} catch (cms::Exception& ex) { }
  iEvent.getByLabel("towerMaker", hhitBarrelHandle);
  const CaloTowerCollection* towerCollection = hhitBarrelHandle.product();

  //Get Calo Geometry
  edm::ESHandle<CaloGeometry> pG;
  iSetup.get<IdealGeometryRecord>().get(pG);
  const CaloGeometry* caloGeom = pG.product();

  //Reconstructed vertexes
  edm::Handle<reco::VertexCollection> vertices;
  try {iEvent.getByLabel("offlinePrimaryVerticesFromCTFTracks",vertices);} catch (cms::Exception& ex) { }
  const VertexCollection* VertexData = vertices.product();

  // Get the general tracks
  edm::Handle<reco::TrackCollection> trackHandle;
  try {iEvent.getByLabel("ctfWithMaterialTracks", trackHandle);} catch (cms::Exception& ex) { }
  const reco::TrackCollection* trackcoll = trackHandle.product();

  //total number of events on which the code is run:
  event_tot++;

  // Get PixelMatchGsfElectron Collection -----------------------------------------
  reco::PixelMatchGsfElectronCollection::const_iterator gsfhottest;

  Handle<reco::PixelMatchGsfElectronCollection> pixelmatchelec;
  try{ iEvent.getByLabel("pixelMatchGsfElectrons","",pixelmatchelec); } catch(cms::Exception& ex) {
    LogDebug("Zprime2eeAnaGsfEm")<<"could not found the product with producer pixelMatchGsfElectrons" << ex ;
  }
  const reco::PixelMatchGsfElectronCollection* collpixelmatchelec = pixelmatchelec.product();
  if(debug) LogDebug("Zprime2eeAnaGsfEm") << "collpixelmatchelec->size() = " <<  collpixelmatchelec->size() ;

  //Get the hybrid and endcap supercluster collection
  Handle<reco::SuperClusterCollection> pHybridSuperClusters;
  Handle<reco::SuperClusterCollection> pIslandSuperClusters;

  try {
    iEvent.getByLabel("hybridSuperClusters","", pHybridSuperClusters);
    iEvent.getByLabel("islandSuperClusters","islandEndcapSuperClusters", pIslandSuperClusters);
  } catch ( cms::Exception& ex ) {
    LogDebug("Zprime2eeAnaGsfEm")<<"could not found the product with producer hybridSuperClusters or islandEndcapSuperClusters" << ex ;
  }

  const reco::SuperClusterCollection* hybridSuperClusters = pHybridSuperClusters.product();
  const reco::SuperClusterCollection* islandSuperClusters = pIslandSuperClusters.product();

  //Merge these two collections into one (sclusters collection)
  std::vector<reco::SuperClusterRef> sclusters;
  for(unsigned int i = 0;i<hybridSuperClusters->size();i++) {
    reco::SuperClusterRef hscref = edm::Ref<reco::SuperClusterCollection>(pHybridSuperClusters,i);
    sclusters.push_back(hscref);
  }
  for(unsigned int i = 0;i<islandSuperClusters->size();i++) {
    reco::SuperClusterRef iscref = edm::Ref<reco::SuperClusterCollection>(pIslandSuperClusters,i);
    sclusters.push_back(iscref);
  }

  if(debug) {
    LogDebug("Zprime2eeAnaGsfEm") << "hybridSuperClusters->size() = " <<  hybridSuperClusters->size() ;
    LogDebug("Zprime2eeAnaGsfEm") << "islandSuperClusters->size() = " <<  islandSuperClusters->size() ;
    LogDebug("Zprime2eeAnaGsfEm") << "sclusters.size() = " <<  sclusters.size() ;
  }

  gsfcoll_size->Fill(collpixelmatchelec->size() ,sc_weight);
  sccoll_size->Fill(sclusters.size() ,sc_weight);

  //Get the highest pt gsf electron and the highest pt sc
  //only if gsf collection size is > 0 and if sc collection size is > 0
  if(  collpixelmatchelec->size() > 0)  gsf_sizegreater0++;
  if( (collpixelmatchelec->size() > 0) && (sclusters.size() > 1)  ) {
    sc_sizegreater1++;
    searchmax(collpixelmatchelec, sclusters, hottest, sndhottest);

    gsfhottest = collpixelmatchelec->begin() + hottest ;
    gsfhottestenergy = gsfhottest->superCluster()->rawEnergy()+gsfhottest->superCluster()->preshowerEnergy();
    gsfhottesttheta = 2. * atan( exp( -1. * gsfhottest->superCluster()->eta() ) );
    gsfhottestet = gsfhottestenergy * sin(gsfhottesttheta);

    hottestsc = sclusters.begin();
    hottestsc = sclusters.begin() + sndhottest ;
    hottestscenergy = (*hottestsc)->rawEnergy()+(*hottestsc)->preshowerEnergy();
    hottestsctheta = 2. * atan( exp( -1. * (*hottestsc)->eta() ) );
    hottestscet = hottestscenergy * sin(hottestsctheta);

    if(hottestscet > gsfhottestet) {
      hottestobj = *hottestsc;
      sndhottestobj = gsfhottest->superCluster();
      hottestIsGsf = false;
    }

    if(hottestscet < gsfhottestet) {
      hottestobj = gsfhottest->superCluster();
      sndhottestobj = *hottestsc;
      hottestIsGsf = true;
    }

    sclusterenergyone = hottestobj->rawEnergy() + hottestobj->preshowerEnergy();
    sclusterthetaone = 2. * atan( exp( -1. * hottestobj->eta() ) );
    sclusterphione = hottestobj->phi();
    sclusterenergytwo = sndhottestobj->rawEnergy() + sndhottestobj->preshowerEnergy();
    sclusterthetatwo = 2. * atan( exp( -1. * sndhottestobj->eta() ) );
    sclusterphitwo = sndhottestobj->phi();

    const reco::GsfTrackRef track = gsfhottest->gsfTrack();
    charge = gsfhottest->gsfTrack()->charge();
    
    superclusone.setRThetaPhi(sclusterenergyone, sclusterthetaone, sclusterphione);
    superclusone.setE(sclusterenergyone);

    superclustwo.setRThetaPhi(sclusterenergytwo, sclusterthetatwo, sclusterphitwo);
    superclustwo.setE(sclusterenergytwo);


    if(debug) {
      LogDebug("Zprime2eeAnaGsfEm") << " " ;
      LogDebug("Zprime2eeAnaGsfEm")<<" hottest: e et eta phi = " <<superclusone.e()<<" " <<superclusone.et()<<" "
	  <<superclusone.eta()<<" " <<superclusone.phi();
      LogDebug("Zprime2eeAnaGsfEm")<<" 2nd hottest: e et eta phi = " <<superclustwo.e()<<" " <<superclustwo.et()<<" "
	  <<superclustwo.eta()<<" " <<superclustwo.phi();
      if(hottestIsGsf) LogDebug("Zprime2eeAnaGsfEm")<<" hottest is a gsf with charge "<<charge;
      if(!hottestIsGsf) LogDebug("Zprime2eeAnaGsfEm")<<" hottest is a supercluster ";
    }
     
    //Now we have the heplorentz vectors (superclusone,superclustwo) of the hottest and 2nd hottest
    //and also their superclusters (hottestobj,sndhottestobj)
    //check min pt and eta

    int sc_acc=0;
    if(superclusone.et() > ptrecmin_ && superclustwo.et() > ptrecmin_ ) {
      sc_one_two_ptmin++;
      if( fabs(superclusone.eta()) < etarecmin_ && fabs(superclustwo.eta()) < etarecmin_ ) {
	sc_one_two_acc++;
	sc_acc=1;
      }
    }
    if(debug) LogDebug("Zprime2eeAnaGsfEm") << " sc_acc= " << sc_acc <<  endl;

    // primary vertex reco:
    double recozvtx = 0.;
    for(reco::VertexCollection::const_iterator avertex = VertexData->begin();avertex != VertexData->end();++avertex) {
      recozvtx = avertex->z();
    }
    if(debug) LogDebug("Zprime2eeAnaGsfEm") << " vxt:reco:   recozvtx= " << recozvtx <<  endl;
  
    if(sc_acc) {
    rad_one = sqrt(  (hottestobj->x()*hottestobj->x()) + (hottestobj->y()*hottestobj->y()) + (hottestobj->z()*hottestobj->z())  );
    radt_one = sqrt(  (hottestobj->x()*hottestobj->x()) + (hottestobj->y()*hottestobj->y())  );
    rad_two = sqrt(  (sndhottestobj->x()*sndhottestobj->x()) + (sndhottestobj->y()*sndhottestobj->y()) + (sndhottestobj->z()*sndhottestobj->z())  );
    radt_two = sqrt(  (sndhottestobj->x()*sndhottestobj->x()) + (sndhottestobj->y()*sndhottestobj->y())   );


    //Compute the correct eta (from primary vertex position and not from (0,0,0))
    superclusone_vtxcor = superclusone;
    superclustwo_vtxcor = superclustwo;
    vertexcorr(superclusone, superclusone_vtxcor, recozvtx, rad_one, radt_one);
    vertexcorr(superclustwo, superclustwo_vtxcor, recozvtx, rad_two, radt_two);

    if (debug) {
      LogDebug("Zprime2eeAnaGsfEm") << "vertex correction for eta:" ;
      LogDebug("Zprime2eeAnaGsfEm") << "superclusone : et,e,eta,phi before: " << superclusone.et()<<" " 
	   <<superclusone.e()<<" " <<superclusone.eta()<<" " <<superclusone.phi();
      LogDebug("Zprime2eeAnaGsfEm") << "superclusone : et,e,eta,phi  after: " << superclusone_vtxcor.et()<<" " 
	   <<superclusone_vtxcor.e()<<" " <<superclusone_vtxcor.eta()<<" " <<superclusone_vtxcor.phi();
      LogDebug("Zprime2eeAnaGsfEm") << "superclustwo : et,e,eta,phi before: " << superclustwo.et()<<" " 
	   <<superclustwo.e()<<" " <<superclustwo.eta()<<" " <<superclustwo.phi();
      LogDebug("Zprime2eeAnaGsfEm") << "superclustwo : et,e,eta,phi  after: " << superclustwo_vtxcor.et()<<" " 
	   <<superclustwo_vtxcor.e()<<" " <<superclustwo_vtxcor.eta()<<" " <<superclustwo_vtxcor.phi();
    }

    //Compute the invariant mass and fill histos before isolation cuts
    float  sc_mass=(superclustwo+superclusone).m();
    float  sc_mass_vtxcor=(superclustwo_vtxcor+superclusone_vtxcor).m();
    sc_Z_m->Fill(sc_mass ,sc_weight);
    sc_Z_vtxcor_m->Fill(sc_mass_vtxcor ,sc_weight);
    sc_one_pt->Fill(superclusone.et() ,sc_weight);
    sc_two_pt->Fill(superclustwo.et() ,sc_weight);
    sc_one_vtxcor_pt->Fill(superclusone_vtxcor.et() ,sc_weight);
    sc_two_vtxcor_pt->Fill(superclustwo_vtxcor.et() ,sc_weight);


    // Compute the isolation variables and fill histos

    // 1. tracker isolation  

    std::pair<int,float> sconetrackisol = trackisol(hottestobj,track,trackcoll,recozvtx,hottestIsGsf,true);
    std::pair<int,float> sctwotrackisol = trackisol(sndhottestobj,track,trackcoll,recozvtx,!hottestIsGsf,true);
  
    sc_one_track_isol_abs->Fill(sconetrackisol.second ,sc_weight);
    sc_one_track_isol->Fill(sconetrackisol.second/superclusone.et() ,sc_weight);
    sc_two_track_isol_abs->Fill(sctwotrackisol.second ,sc_weight);
    sc_two_track_isol->Fill(sctwotrackisol.second/superclustwo.et() ,sc_weight);
    sc_one_numbertrackisol->Fill(sconetrackisol.first ,sc_weight);
    sc_two_numbertrackisol->Fill(sctwotrackisol.first ,sc_weight);

    if(debug) {
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_one_track_isol_abs = "<<sconetrackisol.second;
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_one_track_isol = "<<sconetrackisol.second/superclusone.et();
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_two_track_isol_abs = "<<sctwotrackisol.second;
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_two_track_isol = "<<sctwotrackisol.second/superclustwo.et();
      LogDebug("Zprime2eeAnaGsfEm")<<" #tracks for e- "<<sconetrackisol.first<<" and pt sum e-"<<sconetrackisol.second;
      LogDebug("Zprime2eeAnaGsfEm")<<" #tracks for e+ "<<sctwotrackisol.first<<" and pt sum e+"<<sctwotrackisol.second;
    }


    // 2. HCAL isolation  
    double one_hcalisol = hcaletisol(hottestobj,towerCollection);
    double two_hcalisol = hcaletisol(sndhottestobj,towerCollection);

    sc_one_hcal_isol_abs->Fill(one_hcalisol ,sc_weight);
    sc_one_hcal_isol->Fill(one_hcalisol/superclusone.et() ,sc_weight);
    sc_two_hcal_isol_abs->Fill(two_hcalisol ,sc_weight);
    sc_two_hcal_isol->Fill(two_hcalisol/superclustwo.et() ,sc_weight);

    if(debug) {
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_one_hcal_isol_abs = "<<one_hcalisol;
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_one_hcal_isol = "<<one_hcalisol/superclusone.et();
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_two_hcal_isol_abs = "<<two_hcalisol;
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_two_hcal_isol = "<<two_hcalisol/superclustwo.et();
    }

    // 3. ECAL isolation  
    double one_ecalisol = ecaletisol(iEvent,hottestobj);
    double two_ecalisol = ecaletisol(iEvent,sndhottestobj);

    sc_one_ecal_isol_abs->Fill(one_ecalisol ,sc_weight);
    sc_one_ecal_isol->Fill(one_ecalisol/superclusone.et() ,sc_weight);
    sc_two_ecal_isol_abs->Fill(two_ecalisol ,sc_weight);
    sc_two_ecal_isol->Fill(two_ecalisol/superclustwo.et() ,sc_weight);
 
    if(debug) {
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_one_ecal_isol_abs = "<< one_ecalisol;
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_one_ecal_isol = "<<one_ecalisol/superclusone.et();
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_two_ecal_isol_abs = "<<two_ecalisol;
      LogDebug("Zprime2eeAnaGsfEm")<<" sc_two_ecal_isol = "<<two_ecalisol/superclustwo.et();
    }


    // Cut on Isolation variables

    if(sconetrackisol.first < ntrackisol_max_ && sctwotrackisol.first < ntrackisol_max_) {
      sc_ntkisolated++;

      if(  (sconetrackisol.second/superclusone.et() < trackisol_max_) && (sctwotrackisol.second/superclustwo.et() < trackisol_max_)  ) {
	sc_tkisolated++;

	if(one_hcalisol/superclusone.et() < hcalisol_max_ && two_hcalisol/superclustwo.et() < hcalisol_max_) {
	  sc_hcalisolated++;

	  if(one_ecalisol/superclusone.et() < ecalisol_max_ && two_ecalisol/superclustwo.et() < ecalisol_max_) {
	    sc_ecalisolated++;

	    //Compute the invariant mass and fill histos AFTER isolation cuts
	    float  sc_mass=(superclustwo+superclusone).m();
	    float  sc_mass_vtxcor=(superclustwo_vtxcor+superclusone_vtxcor).m();
	    sc_Z_iso_m->Fill(sc_mass ,sc_weight);
	    sc_Z_vtxcor_iso_m->Fill(sc_mass_vtxcor ,sc_weight);
	    sc_Z_iso_e->Fill((superclustwo_vtxcor+superclusone_vtxcor).e() ,sc_weight);
	    sc_Z_iso_pt->Fill((superclustwo_vtxcor+superclusone_vtxcor).et() ,sc_weight);
	    sc_Z_iso_eta->Fill((superclustwo_vtxcor+superclusone_vtxcor).eta() ,sc_weight);
	    sc_Z_iso_phi->Fill((superclustwo_vtxcor+superclusone_vtxcor).phi() ,sc_weight);

	    sc_one_iso_pt->Fill(superclusone.et() ,sc_weight);
	    sc_two_iso_pt->Fill(superclustwo.et() ,sc_weight);
	    sc_one_iso_eta->Fill(superclusone.eta() ,sc_weight);
	    sc_two_iso_eta->Fill(superclustwo.eta() ,sc_weight);

	    sc_one_vtxcor_iso_pt->Fill(superclusone_vtxcor.et() ,sc_weight);
	    sc_two_vtxcor_iso_pt->Fill(superclustwo_vtxcor.et() ,sc_weight);
	    sc_one_vtxcor_iso_eta->Fill(superclusone_vtxcor.eta() ,sc_weight);
	    sc_two_vtxcor_iso_eta->Fill(superclustwo_vtxcor.eta() ,sc_weight);
	    sc_one_vtxcor_iso_e->Fill(superclusone_vtxcor.e() ,sc_weight);
	    sc_two_vtxcor_iso_e->Fill(superclustwo_vtxcor.e() ,sc_weight);
	    sc_one_vtxcor_iso_phi->Fill(superclusone_vtxcor.phi() ,sc_weight);
	    sc_two_vtxcor_iso_phi->Fill(superclustwo_vtxcor.phi() ,sc_weight);
	
	
	    //Fill variables for the tree
	    sc_Z_iso_m_var = sc_mass;
	    sc_Z_vtxcor_iso_m_var = sc_mass_vtxcor;
	    sc_Z_iso_e_var = (superclustwo_vtxcor+superclusone_vtxcor).e(); 
	    sc_Z_iso_pt_var = (superclustwo_vtxcor+superclusone_vtxcor).et(); 
	    sc_Z_iso_eta_var = (superclustwo_vtxcor+superclusone_vtxcor).eta(); 
	    sc_Z_iso_phi_var = (superclustwo_vtxcor+superclusone_vtxcor).phi();

	    sc_one_iso_pt_var = superclusone.et();
	    sc_two_iso_pt_var = superclustwo.et();
	    sc_one_iso_eta_var = superclusone.eta();
	    sc_two_iso_eta_var = superclustwo.eta();

	    sc_one_vtxcor_iso_pt_var = superclusone_vtxcor.et();
	    sc_two_vtxcor_iso_pt_var = superclustwo_vtxcor.et(); 
	    sc_one_vtxcor_iso_eta_var = superclusone_vtxcor.eta();
	    sc_two_vtxcor_iso_eta_var = superclustwo_vtxcor.eta();
	    sc_one_vtxcor_iso_e_var = superclusone_vtxcor.e();
	    sc_two_vtxcor_iso_e_var = superclustwo_vtxcor.e(); 
	    sc_one_vtxcor_iso_phi_var = superclusone_vtxcor.phi();
	    sc_two_vtxcor_iso_phi_var = superclustwo_vtxcor.phi();

	    //Fill the tree
	    mytree->Fill();

	  }//end of ecal isolation cut
	}//end of hcal isolation cut
      }//end of tracker pt isolation cut 
    }//end of tracker #tracks isolation cut 

    }//end of cut on sc_acc
  }//end of if size of collection is > 1

}

// ------------ method called once each job just after ending the event loop  ------------
void 
PixelMatchAnalysisGsfEm::endJob() {

  double eff0=double(gsf_sizegreater0)/double(event_tot);
  double eff1=double(sc_sizegreater1)/double(gsf_sizegreater0);
  double eff2=double(sc_one_two_ptmin)/double(sc_sizegreater1);
  double eff3=double(sc_one_two_acc)/double(sc_one_two_ptmin);
  double eff4=double(sc_ntkisolated)/double(sc_one_two_acc);
  double eff5=double(sc_tkisolated)/double(sc_ntkisolated);
  double eff6=double(sc_hcalisolated)/double(sc_tkisolated);
  double eff7=double(sc_ecalisolated)/double(sc_hcalisolated);
  double efffinal=double(sc_ecalisolated)/double(event_tot);

  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"";
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"";
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"--------------------------------------------------------";
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"SUMMARY of number of events Gsf/EM";
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"N tot                         "<<event_tot<<",    eff= 1.00";
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"--------------------------------------------------------";
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"N > 0 GSF                     "<<gsf_sizegreater0<<",    eff="<<eff0;
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"N > 0 GSF + 1 add SC REC:     "<<sc_sizegreater1 <<",    eff="<<eff1;
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"     + pt>cut                 "<<sc_one_two_ptmin<<",    eff="<<eff2;
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"     + eta<cut                "<<sc_one_two_acc  <<",    eff="<<eff3;
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<" + sc_ntkisolated             "<<sc_ntkisolated  <<",    eff="<<eff4;
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<" + sc_tkisolated              "<<sc_tkisolated   <<",    eff="<<eff5;
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<" + sc_hcalisolated            "<<sc_hcalisolated <<",    eff="<<eff6;
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<" + sc_ecalisolated            "<<sc_ecalisolated <<",    eff="<<eff7;
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"--------------------------------------------------------";
  edm::LogVerbatim("Zprime2eeAnaGsfEm")<<"----------------- Totl eff              ="<<efffinal;
  edm::LogVerbatim("Zprime2eeAnaGsfEm") << "" ;

}
//define this as a plug-in
DEFINE_FWK_MODULE(PixelMatchAnalysisGsfEm);

