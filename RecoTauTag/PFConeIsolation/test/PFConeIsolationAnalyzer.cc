/* class PFConeIsolationAnalyzer
 *  EDAnalyzer of the tagged TauJet with the PFConeIsolationAlgorithm, 
 *  created: Apr 23 2007,
 *  revised: Jun 16 2007,
 *  author: Ludovic Houchu.
 */
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h" 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h" 

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/PFIsolatedTauTagInfo.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/GeometryVector/interface/GlobalTag.h"
#include "DataFormats/GeometryVector/interface/Vector3DBase.h"
#include "DataFormats/GeometryVector/interface/Point3DBase.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticle.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementTrack.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Vector/LorentzVector.h"

#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"

#include <memory>
#include <string>
#include <iostream>
#include <limits>

#include <TROOT.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>

using namespace std; 
using namespace edm;
using namespace reco;
using namespace math;

class PFConeIsolationAnalyzer : public EDAnalyzer {
public:
  explicit PFConeIsolationAnalyzer(const ParameterSet&);
  ~PFConeIsolationAnalyzer() {;}
  virtual void analyze(const Event& iEvent, const EventSetup& iSetup);
  virtual void beginJob();
  virtual void endJob();
private:
  string PFSimParticleProd_;
  string PFRecTrackProd_;
  string PFBlockElementTrackProd_;
  string PFConeIsolationProd_;
  string PVProd_;
  bool test_useOnlyChargedHadrCand_;
  double test_Cand_minpt_;
  bool test_useOnlyChargedHadrforleadPFCand_;
  double test_trackercone_size_;
  double test_trackersignalcone_size_;
  double test_trackerisolcone_size_;
  double test_matchingcone_size_;
  double test_leadCand_minpt_;
  string output_filename_;
  TFile* thefile;
  TTree* theEventTree; 
  TTree* thePFIsolatedTauTagInfoTree; 
  int gets_recPV;
  float event_scale;
  static const int MaxGenTausnumber=100;
  int GenTausnumber;
  int GenTau_pid[MaxGenTausnumber];
  float GenTau_e[MaxGenTausnumber];
  float GenTau_theta[MaxGenTausnumber];
  float GenTau_phi[MaxGenTausnumber];
  float GenTau_visproducts_e[MaxGenTausnumber];
  float GenTau_visproducts_theta[MaxGenTausnumber];
  float GenTau_visproducts_phi[MaxGenTausnumber];
  int GenTau_decaytype[MaxGenTausnumber];
  static const int MaxSimTausnumber=100;
  int SimTausnumber;
  int SimTau_pid[MaxSimTausnumber];
  float SimTau_visproducts_e[MaxSimTausnumber];
  float SimTau_visproducts_p[MaxSimTausnumber];
  float SimTau_visproducts_theta[MaxSimTausnumber];
  float SimTau_visproducts_phi[MaxSimTausnumber];
  int SimTau_decaytype[MaxSimTausnumber];
  static const int MaxGenJet05snumber=200;
  int GenJet05snumber;
  float GenJet05_e[MaxGenJet05snumber];
  float GenJet05_et[MaxGenJet05snumber];
  float GenJet05_eta[MaxGenJet05snumber];
  float GenJet05_phi[MaxGenJet05snumber];
  int PFIsolatedTauTagInfo_eventgets_recPV;
  float PFIsolatedTauTagInfo_event_scale;
  float PFIsolatedTauTagInfo_e;
  float PFIsolatedTauTagInfo_et;
  float PFIsolatedTauTagInfo_eta;
  float PFIsolatedTauTagInfo_phi;
  float PFIsolatedTauTagInfo_invmass;
  float PFIsolatedTauTagInfo_mode;
  float PFIsolatedTauTagInfo_modet;
  float PFIsolatedTauTagInfo_modeta;
  float PFIsolatedTauTagInfo_modphi;
  float PFIsolatedTauTagInfo_modinvmass;
  int PFIsolatedTauTagInfo_passed_tracksel;
  int PFIsolatedTauTagInfo_passed_trackisolsel;
  int PFIsolatedTauTagInfo_passed_ECALisolsel;
  float PFIsolatedTauTagInfo_leadrectk_pt;
  int PFIsolatedTauTagInfo_rectks_number;
  int PFIsolatedTauTagInfo_signalrectks_number;
  int PFIsolatedTauTagInfo_isolrectks_number;
  float PFIsolatedTauTagInfo_discriminator;
  static const int ChargedHadrCands_nmax=100;
  int ChargedHadrCands_n;
  float PFIsolatedTauTagInfo_CHCandDR[ChargedHadrCands_nmax];
  static const int NeutrHadrCands_nmax=100;
  int NeutrHadrCands_n;
  float PFIsolatedTauTagInfo_NHCandDR[NeutrHadrCands_nmax];
  float PFIsolatedTauTagInfo_NHCandEt[NeutrHadrCands_nmax];
  static const int GammaCands_nmax=100;
  int GammaCands_n;
  float PFIsolatedTauTagInfo_GCandDR[GammaCands_nmax];
  float PFIsolatedTauTagInfo_GCandEt[GammaCands_nmax];
  float PFIsolatedTauTagInfo_GenTau_visproducts_e;
  float PFIsolatedTauTagInfo_GenTau_visproducts_et;
  float PFIsolatedTauTagInfo_GenTau_visproducts_eta;
  float PFIsolatedTauTagInfo_GenTau_visproducts_phi;
  int PFIsolatedTauTagInfo_GenTau_visproducts_type;
  float PFIsolatedTauTagInfo_SimTau_visproducts_e;
  float PFIsolatedTauTagInfo_SimTau_visproducts_et;
  float PFIsolatedTauTagInfo_SimTau_visproducts_eta;
  float PFIsolatedTauTagInfo_SimTau_visproducts_phi;
  int PFIsolatedTauTagInfo_SimTau_visproducts_type;
  float PFIsolatedTauTagInfo_GenJet05_e;
  float PFIsolatedTauTagInfo_GenJet05_et;
  float PFIsolatedTauTagInfo_GenJet05_eta;
  float PFIsolatedTauTagInfo_GenJet05_phi;
  class BydecreasingEt {
  public:
    bool operator()(HepLorentzVector a,HepLorentzVector b) {
      return (double)a.et()>(double)b.et();      
    }
  };
};

PFConeIsolationAnalyzer::PFConeIsolationAnalyzer(const edm::ParameterSet& iConfig){  
  PFSimParticleProd_                    = iConfig.getParameter<string>("PFSimParticleProd");
  PFRecTrackProd_                       = iConfig.getParameter<string>("PFRecTrackProd");
  PFBlockElementTrackProd_              = iConfig.getParameter<string>("PFBlockElementTrackProd");
  PFConeIsolationProd_                  = iConfig.getParameter<string>("PFConeIsolationProd");
  PVProd_                               = iConfig.getParameter<string>("PVProd");
  test_useOnlyChargedHadrCand_          = iConfig.getParameter<bool>("test_useOnlyChargedHadrCand");
  test_Cand_minpt_                      = iConfig.getParameter<double>("test_Cand_minpt");
  test_useOnlyChargedHadrforleadPFCand_ = iConfig.getParameter<bool>("test_useOnlyChargedHadrforleadPFCand");
  test_trackercone_size_                = iConfig.getParameter<double>("test_trackercone_size");
  test_trackersignalcone_size_          = iConfig.getParameter<double>("test_trackersignalcone_size");
  test_trackerisolcone_size_            = iConfig.getParameter<double>("test_trackerisolcone_size");
  test_matchingcone_size_               = iConfig.getParameter<double>("test_matchingcone_size");
  test_leadCand_minpt_                  = iConfig.getParameter<double>("test_leadCand_minpt");
  output_filename_             = iConfig.getParameter<string>("output_filename");
  thefile=TFile::Open(output_filename_.c_str(),"recreate");  
  thefile->cd();
  theEventTree = new TTree("theEventTree", "theEventTree");
  theEventTree->Branch("gets_recPV",&gets_recPV,"gets_recPV/I");
  theEventTree->Branch("event_scale",&event_scale,"event_scale/F");
  theEventTree->Branch("SimTausnumber",&SimTausnumber,"SimTausnumber/I");
  theEventTree->Branch("SimTau_pid",SimTau_pid,"SimTau_pid[SimTausnumber]/I");
  theEventTree->Branch("SimTau_visproducts_e",SimTau_visproducts_e,"SimTau_visproducts_e[SimTausnumber]/F");
  theEventTree->Branch("SimTau_visproducts_p",SimTau_visproducts_p,"SimTau_visproducts_p[SimTausnumber]/F");
  theEventTree->Branch("SimTau_visproducts_theta",SimTau_visproducts_theta,"SimTau_visproducts_theta[SimTausnumber]/F");
  theEventTree->Branch("SimTau_visproducts_phi",SimTau_visproducts_phi,"SimTau_visproducts_phi[SimTausnumber]/F");
  theEventTree->Branch("SimTau_decaytype",SimTau_decaytype,"SimTau_decaytype[SimTausnumber]/I");
  theEventTree->Branch("GenTausnumber",&GenTausnumber,"GenTausnumber/I");
  theEventTree->Branch("GenTau_pid",GenTau_pid,"GenTau_pid[GenTausnumber]/I");
  theEventTree->Branch("GenTau_e",GenTau_e,"GenTau_e[GenTausnumber]/F");
  theEventTree->Branch("GenTau_theta",GenTau_theta,"GenTau_theta[GenTausnumber]/F");
  theEventTree->Branch("GenTau_phi",GenTau_phi,"GenTau_phi[GenTausnumber]/F");
  theEventTree->Branch("GenTau_visproducts_e",GenTau_visproducts_e,"GenTau_visproducts_e[GenTausnumber]/F");
  theEventTree->Branch("GenTau_visproducts_theta",GenTau_visproducts_theta,"GenTau_visproducts_theta[GenTausnumber]/F");
  theEventTree->Branch("GenTau_visproducts_phi",GenTau_visproducts_phi,"GenTau_visproducts_phi[GenTausnumber]/F");
  theEventTree->Branch("GenTau_decaytype",GenTau_decaytype,"GenTau_decaytype[GenTausnumber]/I");
  theEventTree->Branch("GenJet05snumber",&GenJet05snumber,"GenJet05snumber/I");
  theEventTree->Branch("GenJet05_e",GenJet05_e,"GenJet05_e[GenJet05snumber]/F");
  theEventTree->Branch("GenJet05_et",GenJet05_et,"GenJet05_et[GenJet05snumber]/F");
  theEventTree->Branch("GenJet05_eta",GenJet05_eta,"GenJet05_eta[GenJet05snumber]/F");
  theEventTree->Branch("GenJet05_phi",GenJet05_phi,"GenJet05_phi[GenJet05snumber]/F");
  thePFIsolatedTauTagInfoTree = new TTree("thePFIsolatedTauTagInfoTree", "thePFIsolatedTauTagInfoTree"); 
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_eventgets_recPV",&PFIsolatedTauTagInfo_eventgets_recPV,"PFIsolatedTauTagInfo_eventgets_recPV/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_event_scale",&PFIsolatedTauTagInfo_event_scale,"PFIsolatedTauTagInfo_event_scale/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_e",&PFIsolatedTauTagInfo_e,"PFIsolatedTauTagInfo_e/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_et",&PFIsolatedTauTagInfo_et,"PFIsolatedTauTagInfo_et/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_eta",&PFIsolatedTauTagInfo_eta,"PFIsolatedTauTagInfo_eta/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_phi",&PFIsolatedTauTagInfo_phi,"PFIsolatedTauTagInfo_phi/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_invmass",&PFIsolatedTauTagInfo_invmass,"PFIsolatedTauTagInfo_invmass/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_mode",&PFIsolatedTauTagInfo_mode,"PFIsolatedTauTagInfo_mode/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_modet",&PFIsolatedTauTagInfo_modet,"PFIsolatedTauTagInfo_modet/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_modeta",&PFIsolatedTauTagInfo_modeta,"PFIsolatedTauTagInfo_modeta/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_modphi",&PFIsolatedTauTagInfo_modphi,"PFIsolatedTauTagInfo_modphi/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_modinvmass",&PFIsolatedTauTagInfo_modinvmass,"PFIsolatedTauTagInfo_modinvmass/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_passed_tracksel",&PFIsolatedTauTagInfo_passed_tracksel,"PFIsolatedTauTagInfo_passed_tracksel/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_passed_trackisolsel",&PFIsolatedTauTagInfo_passed_trackisolsel,"PFIsolatedTauTagInfo_passed_trackisolsel/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_passed_ECALisolsel",&PFIsolatedTauTagInfo_passed_ECALisolsel,"PFIsolatedTauTagInfo_passed_ECALisolsel/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_leadrectk_pt",&PFIsolatedTauTagInfo_leadrectk_pt,"PFIsolatedTauTagInfo_leadrectk_pt/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_rectks_number",&PFIsolatedTauTagInfo_rectks_number,"PFIsolatedTauTagInfo_rectks_number/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_signalrectks_number",&PFIsolatedTauTagInfo_signalrectks_number,"PFIsolatedTauTagInfo_signalrectks_number/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_isolrectks_number",&PFIsolatedTauTagInfo_isolrectks_number,"PFIsolatedTauTagInfo_isolrectks_number/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_discriminator",&PFIsolatedTauTagInfo_discriminator,"PFIsolatedTauTagInfo_discriminator/F");
  thePFIsolatedTauTagInfoTree->Branch("ChargedHadrCands_n",&ChargedHadrCands_n,"ChargedHadrCands_n/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_CHCandDR",PFIsolatedTauTagInfo_CHCandDR,"PFIsolatedTauTagInfo_CHCandDR[ChargedHadrCands_n]/F");
  thePFIsolatedTauTagInfoTree->Branch("NeutrHadrCands_n",&NeutrHadrCands_n,"NeutrHadrCands_n/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_NHCandDR",PFIsolatedTauTagInfo_NHCandDR,"PFIsolatedTauTagInfo_NHCandDR[NeutrHadrCands_n]/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_NHCandEt",PFIsolatedTauTagInfo_NHCandEt,"PFIsolatedTauTagInfo_NHCandEt[NeutrHadrCands_n]/F");
  thePFIsolatedTauTagInfoTree->Branch("GammaCands_n",&GammaCands_n,"GammaCands_n/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_GCandDR",PFIsolatedTauTagInfo_GCandDR,"PFIsolatedTauTagInfo_GCandDR[GammaCands_n]/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_GCandEt",PFIsolatedTauTagInfo_GCandEt,"PFIsolatedTauTagInfo_GCandEt[GammaCands_n]/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_GenTau_visproducts_e",&PFIsolatedTauTagInfo_GenTau_visproducts_e,"PFIsolatedTauTagInfo_GenTau_visproducts_e/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_GenTau_visproducts_et",&PFIsolatedTauTagInfo_GenTau_visproducts_et,"PFIsolatedTauTagInfo_GenTau_visproducts_et/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_GenTau_visproducts_eta",&PFIsolatedTauTagInfo_GenTau_visproducts_eta,"PFIsolatedTauTagInfo_GenTau_visproducts_eta/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_GenTau_visproducts_phi",&PFIsolatedTauTagInfo_GenTau_visproducts_phi,"PFIsolatedTauTagInfo_GenTau_visproducts_phi/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_GenTau_visproducts_type",&PFIsolatedTauTagInfo_GenTau_visproducts_type,"PFIsolatedTauTagInfo_GenTau_visproducts_type/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_SimTau_visproducts_e",&PFIsolatedTauTagInfo_SimTau_visproducts_e,"PFIsolatedTauTagInfo_SimTau_visproducts_e/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_SimTau_visproducts_et",&PFIsolatedTauTagInfo_SimTau_visproducts_et,"PFIsolatedTauTagInfo_SimTau_visproducts_et/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_SimTau_visproducts_eta",&PFIsolatedTauTagInfo_SimTau_visproducts_eta,"PFIsolatedTauTagInfo_SimTau_visproducts_eta/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_SimTau_visproducts_phi",&PFIsolatedTauTagInfo_SimTau_visproducts_phi,"PFIsolatedTauTagInfo_SimTau_visproducts_phi/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_SimTau_visproducts_type",&PFIsolatedTauTagInfo_SimTau_visproducts_type,"PFIsolatedTauTagInfo_SimTau_visproducts_type/I");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_GenJet05_e",&PFIsolatedTauTagInfo_GenJet05_e,"PFIsolatedTauTagInfo_GenJet05_e/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_GenJet05_et",&PFIsolatedTauTagInfo_GenJet05_et,"PFIsolatedTauTagInfo_GenJet05_et/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_GenJet05_eta",&PFIsolatedTauTagInfo_GenJet05_eta,"PFIsolatedTauTagInfo_GenJet05_eta/F");
  thePFIsolatedTauTagInfoTree->Branch("PFIsolatedTauTagInfo_GenJet05_phi",&PFIsolatedTauTagInfo_GenJet05_phi,"PFIsolatedTauTagInfo_GenJet05_phi/F");
}
void PFConeIsolationAnalyzer::beginJob(){}
void PFConeIsolationAnalyzer::endJob(){
  thefile->cd();
  theEventTree->Write();
  thePFIsolatedTauTagInfoTree->Write();
  thefile->Write();
  thefile->Close();
}
void PFConeIsolationAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  vector<pair<HepLorentzVector,int> > GenTau_pair;
  GenTau_pair.clear();
  GenTausnumber=0;
  vector<pair<HepLorentzVector,int> > SimTau_pair;
  SimTau_pair.clear();
  SimTausnumber=0;
  vector<HepLorentzVector> GenJet05_HepLV;
  GenJet05_HepLV.clear();
  GenJet05snumber=0;
  /* *******************************************************************
     generation step
     ******************************************************************* */
  Handle<HepMCProduct> evt;
  iEvent.getByLabel("VtxSmeared",evt);
  HepMC::GenEvent* myGenEvent = new  HepMC::GenEvent(*(evt->GetEvent()));
  event_scale=myGenEvent->event_scale();
  
  // select susy processes
  /*
  if (myGenEvent->signal_process_id()<200 ||  myGenEvent->signal_process_id()>300){
    delete myGenEvent;
    return;
  }
  */
   
  // select gamma*/Z0 processes
  /*
  if (myGenEvent->signal_process_id()!=1){
    delete myGenEvent;
    return;
  }
  */
  
  // select QCD-dijet processes
  /*   
  if (myGenEvent->signal_process_id()!=11 
      && myGenEvent->signal_process_id()!=12 
      && myGenEvent->signal_process_id()!=13 
      && myGenEvent->signal_process_id()!=68 
      && myGenEvent->signal_process_id()!=28 
      && myGenEvent->signal_process_id()!=53){
    delete myGenEvent;
    return;
  }
  */
  int iGenTau = 0;
  for (HepMC::GenEvent::particle_iterator iter=myGenEvent->particles_begin();iter!=myGenEvent->particles_end();iter++) {
    if ((**iter).status()==2 && (abs((**iter).pdg_id())==15)){
      HepMC::GenParticle* TheParticle=(*iter);
      Hep3Vector TheParticle_Hep3V(TheParticle->momentum().px(),TheParticle->momentum().py(),TheParticle->momentum().pz());
      HepLorentzVector TheParticle_HepLV(TheParticle_Hep3V,TheParticle->momentum().e());
      HepLorentzVector TheTauJet_HepLV=TheParticle_HepLV;
      HepMC::GenParticle* TheTauneutrino=0;
      HepMC::GenParticle* TheTauneutrinobar=0;
      int tau_children_n=TheParticle->end_vertex()->particles_out_size();
      for (HepMC::GenVertex::particles_out_const_iterator i_taudaughter=TheParticle->end_vertex()->particles_out_const_begin();i_taudaughter!=TheParticle->end_vertex()->particles_out_const_end();i_taudaughter++){
	if ((**i_taudaughter).status()==1 && abs((int)(**i_taudaughter).pdg_id())==16){
	  TheTauneutrino=*i_taudaughter;
	  Hep3Vector TheTauneutrino_Hep3V(TheTauneutrino->momentum().px(),TheTauneutrino->momentum().py(),TheTauneutrino->momentum().pz());
	  HepLorentzVector TheTauneutrino_HepLV(TheTauneutrino_Hep3V,TheTauneutrino->momentum().e());
	  TheTauJet_HepLV-=TheTauneutrino_HepLV;
	}
      }
      bool tau_issuinge = false;
      bool tau_issuingmu = false;
      bool tau_issuing1prongnu = false;
      bool tau_issuingpi0chargedpinu = false;
      bool tau_issuing3prongsnu = false;
      if (tau_children_n==2) {
	HepMC::GenVertex::particles_out_const_iterator i_1sttaudaughter=TheParticle->end_vertex()->particles_out_const_begin();
	HepMC::GenVertex::particles_out_const_iterator i_2ndtaudaughter=++TheParticle->end_vertex()->particles_out_const_begin();
	if (abs((**i_1sttaudaughter).pdg_id())==211 || abs((**i_2ndtaudaughter).pdg_id())==211) tau_issuing1prongnu = true;
      }
      if (tau_children_n==2) {
	HepMC::GenVertex::particles_out_const_iterator i_1sttaudaughter=TheParticle->end_vertex()->particles_out_const_begin();
	HepMC::GenVertex::particles_out_const_iterator i_2ndtaudaughter=++TheParticle->end_vertex()->particles_out_const_begin();
	if (abs((**i_1sttaudaughter).pdg_id())==213 || abs((**i_2ndtaudaughter).pdg_id())==213) tau_issuingpi0chargedpinu=true;
      }
      if (tau_children_n==3) {
	HepMC::GenVertex::particles_out_const_iterator i_1sttaudaughter=TheParticle->end_vertex()->particles_out_const_begin();
	HepMC::GenVertex::particles_out_const_iterator i_2ndtaudaughter=++TheParticle->end_vertex()->particles_out_const_begin();
	HepMC::GenVertex::particles_out_const_iterator i_3rdtaudaughter=++i_2ndtaudaughter;
	if ((abs((**i_1sttaudaughter).pdg_id())==16 && abs((**i_2ndtaudaughter).pdg_id())==211 && (**i_3rdtaudaughter).pdg_id()==111)
	    || (abs((**i_1sttaudaughter).pdg_id())==16 && abs((**i_3rdtaudaughter).pdg_id())==211 && (**i_2ndtaudaughter).pdg_id()==111)
	    || (abs((**i_2ndtaudaughter).pdg_id())==16 && abs((**i_1sttaudaughter).pdg_id())==211 && (**i_3rdtaudaughter).pdg_id()==111)
	    || (abs((**i_2ndtaudaughter).pdg_id())==16 && abs((**i_3rdtaudaughter).pdg_id())==211 && (**i_1sttaudaughter).pdg_id()==111)
	    || (abs((**i_3rdtaudaughter).pdg_id())==16 && abs((**i_1sttaudaughter).pdg_id())==211 && (**i_2ndtaudaughter).pdg_id()==111)
	    || (abs((**i_3rdtaudaughter).pdg_id())==16 && abs((**i_2ndtaudaughter).pdg_id())==211 && (**i_1sttaudaughter).pdg_id()==111)
	    ) tau_issuingpi0chargedpinu = true;
	if ((abs((**i_1sttaudaughter).pdg_id())==16 && abs((**i_2ndtaudaughter).pdg_id())==211 && (**i_3rdtaudaughter).pdg_id()==113)
	    || (abs((**i_1sttaudaughter).pdg_id())==16 && abs((**i_3rdtaudaughter).pdg_id())==211 && (**i_2ndtaudaughter).pdg_id()==113)
	    || (abs((**i_2ndtaudaughter).pdg_id())==16 && abs((**i_1sttaudaughter).pdg_id())==211 && (**i_3rdtaudaughter).pdg_id()==113)
	    || (abs((**i_2ndtaudaughter).pdg_id())==16 && abs((**i_3rdtaudaughter).pdg_id())==211 && (**i_1sttaudaughter).pdg_id()==113)
	    || (abs((**i_3rdtaudaughter).pdg_id())==16 && abs((**i_1sttaudaughter).pdg_id())==211 && (**i_2ndtaudaughter).pdg_id()==113)
	    || (abs((**i_3rdtaudaughter).pdg_id())==16 && abs((**i_2ndtaudaughter).pdg_id())==211 && (**i_1sttaudaughter).pdg_id()==113)
	    ) tau_issuing3prongsnu = true;
      }
      if (tau_children_n==4) {
	HepMC::GenVertex::particles_out_const_iterator i_1sttaudaughter=TheParticle->end_vertex()->particles_out_const_begin();
	HepMC::GenVertex::particles_out_const_iterator i_2ndtaudaughter=++TheParticle->end_vertex()->particles_out_const_begin();
	HepMC::GenVertex::particles_out_const_iterator i_3rdtaudaughter=++i_2ndtaudaughter;
	HepMC::GenVertex::particles_out_const_iterator i_4thtaudaughter=++i_3rdtaudaughter;
	if ((abs((**i_1sttaudaughter).pdg_id())==16 && abs((**i_2ndtaudaughter).pdg_id())==211 && abs((**i_3rdtaudaughter).pdg_id())==211 && abs((**i_4thtaudaughter).pdg_id())==211)
	    || (abs((**i_2ndtaudaughter).pdg_id())==16 && abs((**i_1sttaudaughter).pdg_id())==211 && abs((**i_3rdtaudaughter).pdg_id())==211 && abs((**i_4thtaudaughter).pdg_id())==211)
	    || (abs((**i_3rdtaudaughter).pdg_id())==16 && abs((**i_1sttaudaughter).pdg_id())==211 && abs((**i_2ndtaudaughter).pdg_id())==211 && abs((**i_4thtaudaughter).pdg_id())==211)
	    || (abs((**i_4thtaudaughter).pdg_id())==16 && abs((**i_1sttaudaughter).pdg_id())==211 && abs((**i_2ndtaudaughter).pdg_id())==211 && abs((**i_3rdtaudaughter).pdg_id())==211)
	    )  tau_issuing3prongsnu = true;
      }
      if (tau_children_n==3) {
	HepMC::GenVertex::particles_out_const_iterator i_1sttaudaughter=TheParticle->end_vertex()->particles_out_const_begin();
	HepMC::GenVertex::particles_out_const_iterator i_2ndtaudaughter=++TheParticle->end_vertex()->particles_out_const_begin();	
	HepMC::GenVertex::particles_out_const_iterator i_3rdtaudaughter=++i_2ndtaudaughter;	
	if (abs((**i_1sttaudaughter).pdg_id())==12 || abs((**i_2ndtaudaughter).pdg_id())==12 || abs((**i_3rdtaudaughter).pdg_id())==12) {
	  if (abs((**i_1sttaudaughter).pdg_id())==12) TheTauneutrinobar=(*i_1sttaudaughter);
	  if (abs((**i_2ndtaudaughter).pdg_id())==12) TheTauneutrinobar=(*i_2ndtaudaughter);
	  if (abs((**i_3rdtaudaughter).pdg_id())==12) TheTauneutrinobar=(*i_3rdtaudaughter);
	  Hep3Vector TheTauneutrinobar_Hep3V(TheTauneutrinobar->momentum().px(),TheTauneutrinobar->momentum().py(),TheTauneutrinobar->momentum().pz());
	  HepLorentzVector TheTauneutrinobar_HepLV(TheTauneutrinobar_Hep3V,TheTauneutrinobar->momentum().e());
	  TheTauJet_HepLV = TheTauJet_HepLV-TheTauneutrinobar_HepLV;
	  tau_issuinge = true;
	}
	if (abs((**i_1sttaudaughter).pdg_id())==14 || abs((**i_2ndtaudaughter).pdg_id())==14 || abs((**i_3rdtaudaughter).pdg_id())==14) {
	  if (abs((**i_1sttaudaughter).pdg_id())==14) TheTauneutrinobar=(*i_1sttaudaughter);
	  if (abs((**i_2ndtaudaughter).pdg_id())==14) TheTauneutrinobar=(*i_2ndtaudaughter);
	  if (abs((**i_3rdtaudaughter).pdg_id())==14) TheTauneutrinobar=(*i_3rdtaudaughter);
	  Hep3Vector TheTauneutrinobar_Hep3V(TheTauneutrinobar->momentum().px(),TheTauneutrinobar->momentum().py(),TheTauneutrinobar->momentum().pz());
	  HepLorentzVector TheTauneutrinobar_HepLV(TheTauneutrinobar_Hep3V,TheTauneutrinobar->momentum().e());
	  TheTauJet_HepLV = TheTauJet_HepLV-TheTauneutrinobar_HepLV;
	  tau_issuingmu = true;
	}	
      }
      GenTau_pid[iGenTau] = (int)(**iter).pdg_id();
      GenTau_e[iGenTau] = (float)(**iter).momentum().e();
      GenTau_theta[iGenTau] = (float)(**iter).momentum().theta();
      GenTau_phi[iGenTau] = (float)(**iter).momentum().phi();
      GenTau_visproducts_e[iGenTau] = (float)TheTauJet_HepLV.e();
      GenTau_visproducts_theta[iGenTau] = (float)TheTauJet_HepLV.theta();
      GenTau_visproducts_phi[iGenTau] = (float)TheTauJet_HepLV.phi();
      if (tau_issuinge) {
	GenTau_decaytype[iGenTau] = 1;
      }
      if (tau_issuingmu) {
	GenTau_decaytype[iGenTau] = 2;
      }
      if (tau_issuing1prongnu) {
	GenTau_decaytype[iGenTau] = 3;
      }
      if (tau_issuingpi0chargedpinu) {
	GenTau_decaytype[iGenTau] = 4;
      }
      if (tau_issuing3prongsnu) {
	GenTau_decaytype[iGenTau] = 5;
      }
      if (!tau_issuinge && !tau_issuingmu && !tau_issuing1prongnu && !tau_issuingpi0chargedpinu && !tau_issuing3prongsnu) {
	GenTau_decaytype[iGenTau] = 6;
      }
      pair<HepLorentzVector,int> TheTauJet_pair(TheTauJet_HepLV,GenTau_decaytype[iGenTau]);
      GenTau_pair.push_back(TheTauJet_pair);
      ++iGenTau;
    }		 
  }
  GenTausnumber=iGenTau;
  delete myGenEvent;
  
  int iSimTau = 0;
  Handle<PFSimParticleCollection> thePFSimParticleCollectionHandle;
  iEvent.getByLabel(PFSimParticleProd_,thePFSimParticleCollectionHandle);
  const PFSimParticleCollection& thePFSimParticleCollection=*(thePFSimParticleCollectionHandle.product());
  for (PFSimParticleCollection::const_iterator iPFSimParticle=thePFSimParticleCollection.begin();iPFSimParticle!=thePFSimParticleCollection.end();++iPFSimParticle) {
    const PFSimParticle& thePFSimParticle=(*iPFSimParticle);
    if (abs(thePFSimParticle.pdgCode())==15) {
      const vector<int>& thePFSimParticledaughters = thePFSimParticle.daughterIds();
      HepLorentzVector TheTauJet_HepLV(0.,0.,0.,0.);
      bool tau_issuinge=false;
      bool tau_issuingmu=false;
      bool tau_issuing1prongnu = false;
      bool tau_issuingpi0chargedpinu = false;
      bool tau_issuing3prongsnu = false;
      if ((int)thePFSimParticledaughters.size()==1){
	const PFSimParticle& theTauDaughter=thePFSimParticleCollection[thePFSimParticledaughters[0]];
	const PFTrajectoryPoint& theTauDaughter_1stTP=theTauDaughter.trajectoryPoint(0);
	Hep3Vector theTauDaughter_Hep3V(theTauDaughter_1stTP.momentum().Px(),theTauDaughter_1stTP.momentum().Py(),theTauDaughter_1stTP.momentum().Pz());
	HepLorentzVector theTauDaughter_HepLV(theTauDaughter_Hep3V,theTauDaughter_1stTP.momentum().E());
	TheTauJet_HepLV=theTauDaughter_HepLV;
	unsigned pdgdaugter=theTauDaughter.pdgCode();
	if ((int)pdgdaugter==11) tau_issuinge=true;
	if ((int)pdgdaugter==13) tau_issuingmu=true;
	if (abs((int)pdgdaugter)==211) tau_issuing1prongnu=true;
      }
      if ((int)thePFSimParticledaughters.size()==2){
	const PFSimParticle& the1stTauDaughter=thePFSimParticleCollection[thePFSimParticledaughters[0]];
	const PFTrajectoryPoint& the1stTauDaughter_1stTP=the1stTauDaughter.trajectoryPoint(0);
	Hep3Vector the1stTauDaughter_Hep3V(the1stTauDaughter_1stTP.momentum().Px(),the1stTauDaughter_1stTP.momentum().Py(),the1stTauDaughter_1stTP.momentum().Pz());
	HepLorentzVector the1stTauDaughter_HepLV(the1stTauDaughter_Hep3V,the1stTauDaughter_1stTP.momentum().E());
	TheTauJet_HepLV=the1stTauDaughter_HepLV;
	unsigned pdg1stdaugter=the1stTauDaughter.pdgCode();
	const PFSimParticle& the2ndTauDaughter=thePFSimParticleCollection[thePFSimParticledaughters[1]];
	const PFTrajectoryPoint& the2ndTauDaughter_1stTP=the2ndTauDaughter.trajectoryPoint(0);
	Hep3Vector the2ndTauDaughter_Hep3V(the2ndTauDaughter_1stTP.momentum().Px(),the2ndTauDaughter_1stTP.momentum().Py(),the2ndTauDaughter_1stTP.momentum().Pz());
	HepLorentzVector the2ndTauDaughter_HepLV(the2ndTauDaughter_Hep3V,the2ndTauDaughter_1stTP.momentum().E());
	TheTauJet_HepLV+=the2ndTauDaughter_HepLV;
	unsigned pdg2nddaugter=the2ndTauDaughter.pdgCode();
	if ((abs((int)pdg1stdaugter)==211 && (int)pdg2nddaugter==111) || (abs((int)pdg2nddaugter)==211 && (int)pdg1stdaugter==111)) tau_issuingpi0chargedpinu=true;
      }
      if ((int)thePFSimParticledaughters.size()==3){
	const PFSimParticle& the1stTauDaughter=thePFSimParticleCollection[thePFSimParticledaughters[0]];
	const PFTrajectoryPoint& the1stTauDaughter_1stTP=the1stTauDaughter.trajectoryPoint(0);
	Hep3Vector the1stTauDaughter_Hep3V(the1stTauDaughter_1stTP.momentum().Px(),the1stTauDaughter_1stTP.momentum().Py(),the1stTauDaughter_1stTP.momentum().Pz());
	HepLorentzVector the1stTauDaughter_HepLV(the1stTauDaughter_Hep3V,the1stTauDaughter_1stTP.momentum().E());
	TheTauJet_HepLV=the1stTauDaughter_HepLV;
	unsigned pdg1stdaugter=the1stTauDaughter.pdgCode();
	const PFSimParticle& the2ndTauDaughter=thePFSimParticleCollection[thePFSimParticledaughters[1]];
	const PFTrajectoryPoint& the2ndTauDaughter_1stTP=the2ndTauDaughter.trajectoryPoint(0);
	Hep3Vector the2ndTauDaughter_Hep3V(the2ndTauDaughter_1stTP.momentum().Px(),the2ndTauDaughter_1stTP.momentum().Py(),the2ndTauDaughter_1stTP.momentum().Pz());
	HepLorentzVector the2ndTauDaughter_HepLV(the2ndTauDaughter_Hep3V,the2ndTauDaughter_1stTP.momentum().E());
	TheTauJet_HepLV+=the2ndTauDaughter_HepLV;
	unsigned pdg2nddaugter=the2ndTauDaughter.pdgCode();
	const PFSimParticle& the3rdTauDaughter=thePFSimParticleCollection[thePFSimParticledaughters[2]];
	const PFTrajectoryPoint& the3rdTauDaughter_1stTP=the3rdTauDaughter.trajectoryPoint(0);
	Hep3Vector the3rdTauDaughter_Hep3V(the3rdTauDaughter_1stTP.momentum().Px(),the3rdTauDaughter_1stTP.momentum().Py(),the3rdTauDaughter_1stTP.momentum().Pz());
	HepLorentzVector the3rdTauDaughter_HepLV(the3rdTauDaughter_Hep3V,the3rdTauDaughter_1stTP.momentum().E());
	TheTauJet_HepLV+=the3rdTauDaughter_HepLV;
	unsigned pdg3rddaugter=the3rdTauDaughter.pdgCode();
	if (abs((int)pdg1stdaugter)==211 && abs((int)pdg2nddaugter)==211 && abs((int)pdg3rddaugter)==211) tau_issuing3prongsnu=true;
      }
      if ((int)thePFSimParticledaughters.size()>3){
	for (unsigned int idaughter=0;idaughter<thePFSimParticledaughters.size();++idaughter){
	  const PFSimParticle& theTauDaughter=thePFSimParticleCollection[thePFSimParticledaughters[idaughter]];
	  const PFTrajectoryPoint& theTauDaughter_1stTP=theTauDaughter.trajectoryPoint(0);
	  Hep3Vector theTauDaughter_Hep3V(theTauDaughter_1stTP.momentum().Px(),theTauDaughter_1stTP.momentum().Py(),theTauDaughter_1stTP.momentum().Pz());
	  HepLorentzVector theTauDaughter_HepLV(theTauDaughter_Hep3V,theTauDaughter_1stTP.momentum().E());
	  TheTauJet_HepLV+=theTauDaughter_HepLV;
	}
      }
      if (tau_issuinge) SimTau_decaytype[iSimTau] = 1;
      if (tau_issuingmu) SimTau_decaytype[iSimTau] = 2;
      if (tau_issuing1prongnu) SimTau_decaytype[iSimTau] = 3;
      if (tau_issuingpi0chargedpinu) SimTau_decaytype[iSimTau] = 4;
      if (tau_issuing3prongsnu) SimTau_decaytype[iSimTau] = 5;
      if (!tau_issuinge && !tau_issuingmu && !tau_issuing1prongnu && !tau_issuingpi0chargedpinu && !tau_issuing3prongsnu) SimTau_decaytype[iSimTau] = 6;
      SimTau_pid[iSimTau] = thePFSimParticle.pdgCode();
      SimTau_visproducts_e[iSimTau] = (float)TheTauJet_HepLV.e();
      SimTau_visproducts_p[iSimTau] = (float)TheTauJet_HepLV.rho();
      SimTau_visproducts_theta[iSimTau] = (float)TheTauJet_HepLV.theta();
      SimTau_visproducts_phi[iSimTau] = (float)TheTauJet_HepLV.phi();      
      pair<HepLorentzVector,int> TheTauJet_pair(TheTauJet_HepLV,SimTau_decaytype[iSimTau]);
      SimTau_pair.push_back(TheTauJet_pair);
      ++iSimTau;
    }
  }
  SimTausnumber=iSimTau;
  
  int iGenJet05 = 0; 
  Handle<GenJetCollection> genIter05Jets;
  iEvent.getByLabel("iterativeCone5GenJets",genIter05Jets);   
  for(GenJetCollection::const_iterator i_genIter05Jet=genIter05Jets->begin();i_genIter05Jet!=genIter05Jets->end();++i_genIter05Jet) {
    HepLorentzVector myGenJet05_HepLV(i_genIter05Jet->px(),i_genIter05Jet->py(),i_genIter05Jet->pz(),i_genIter05Jet->energy());
    GenJet05_e[iGenJet05] = myGenJet05_HepLV.e();
    GenJet05_et[iGenJet05] = myGenJet05_HepLV.et();
    GenJet05_eta[iGenJet05] = myGenJet05_HepLV.eta();
    GenJet05_phi[iGenJet05] = myGenJet05_HepLV.phi();
    GenJet05_HepLV.push_back(myGenJet05_HepLV);
    ++iGenJet05;
  }  
  GenJet05snumber=iGenJet05;
  stable_sort(GenJet05_HepLV.begin(),GenJet05_HepLV.end(),BydecreasingEt());
  
  Handle<VertexCollection> vertices;
  iEvent.getByLabel(PVProd_,vertices);
  const VertexCollection vertCollection = *(vertices.product());
  if(!vertCollection.size()) gets_recPV=0;
  else gets_recPV=1;
  
  /*
  Handle<PFRecTrackCollection> thePFRecTrackCollectionHandle;
  iEvent.getByLabel(PFRecTrackProd_,"",thePFRecTrackCollectionHandle);  
  const PFRecTrackCollection& thePFRecTrackCollection=*(thePFRecTrackCollectionHandle.product());
  for (PFRecTrackCollection::const_iterator iPFRecTrack=thePFRecTrackCollection.begin();iPFRecTrack!=thePFRecTrackCollection.end();++iPFRecTrack) {
    cout<<"(*iPFRecTrack).trackRef()->pt() "<<(*iPFRecTrack).trackRef()->pt()<<endl;
  }
  */
  Handle<JetTagCollection> jetTagHandle;
  iEvent.getByLabel(PFConeIsolationProd_,jetTagHandle);
  const JetTagCollection& myJetTagCollection=*(jetTagHandle.product()); 
  for (JetTagCollection::const_iterator iJetTag=myJetTagCollection.begin();iJetTag!=myJetTagCollection.end();++iJetTag) {
    PFIsolatedTauTagInfoRef thePFIsolatedTauTagInfo=(*iJetTag).tagInfoRef().castTo<PFIsolatedTauTagInfoRef>();
    HepLorentzVector ThePFIsolatedTauTagJet_HepLV((*thePFIsolatedTauTagInfo).pfjetRef()->px(),(*thePFIsolatedTauTagInfo).pfjetRef()->py(),(*thePFIsolatedTauTagInfo).pfjetRef()->pz(),(*thePFIsolatedTauTagInfo).pfjetRef()->energy());
    PFIsolatedTauTagInfo_eventgets_recPV=gets_recPV;
    PFIsolatedTauTagInfo_event_scale=event_scale;
    PFIsolatedTauTagInfo_e=ThePFIsolatedTauTagJet_HepLV.e();
    PFIsolatedTauTagInfo_et=ThePFIsolatedTauTagJet_HepLV.et();
    PFIsolatedTauTagInfo_eta=ThePFIsolatedTauTagJet_HepLV.eta();
    PFIsolatedTauTagInfo_phi=ThePFIsolatedTauTagJet_HepLV.phi();
    PFIsolatedTauTagInfo_invmass=ThePFIsolatedTauTagJet_HepLV.m(); 
    PFIsolatedTauTagInfo_mode=(*thePFIsolatedTauTagInfo).alternatLorentzVect().E();
    PFIsolatedTauTagInfo_modet=(*thePFIsolatedTauTagInfo).alternatLorentzVect().Et();
    PFIsolatedTauTagInfo_modeta=(*thePFIsolatedTauTagInfo).alternatLorentzVect().Eta();
    PFIsolatedTauTagInfo_modphi=(*thePFIsolatedTauTagInfo).alternatLorentzVect().Phi();
    PFIsolatedTauTagInfo_modinvmass=(*thePFIsolatedTauTagInfo).alternatLorentzVect().M(); 
    PFIsolatedTauTagInfo_passed_trackisolsel=(*thePFIsolatedTauTagInfo).passedtrackerisolation();
    PFIsolatedTauTagInfo_passed_ECALisolsel=(*thePFIsolatedTauTagInfo).passedECALisolation();
    PFIsolatedTauTagInfo_discriminator=(*iJetTag).discriminator();
    PFIsolatedTauTagInfo_rectks_number=0;   
    ChargedHadrCands_n=0;
    NeutrHadrCands_n=0;
    GammaCands_n=0;
    PFCandidateRef theleadPFCand;
    if (test_useOnlyChargedHadrforleadPFCand_) theleadPFCand=(*thePFIsolatedTauTagInfo).leadPFChargedHadrCand(test_matchingcone_size_,test_leadCand_minpt_);
    else theleadPFCand=(*thePFIsolatedTauTagInfo).leadPFCand(test_matchingcone_size_,test_leadCand_minpt_);
    if(!theleadPFCand.isNull()){
      PFIsolatedTauTagInfo_rectks_number=(int)(*thePFIsolatedTauTagInfo).PFChargedHadrCandsInCone((*theleadPFCand).momentum(),test_trackercone_size_,test_Cand_minpt_).size();
      for (PFCandidateRefVector::const_iterator iChargedHadrCand=(*thePFIsolatedTauTagInfo).PFChargedHadrCands().begin();iChargedHadrCand!=(*thePFIsolatedTauTagInfo).PFChargedHadrCands().end();++iChargedHadrCand){
	PFIsolatedTauTagInfo_CHCandDR[ChargedHadrCands_n]=-100.;
	if ((*iChargedHadrCand)!=theleadPFCand) PFIsolatedTauTagInfo_CHCandDR[ChargedHadrCands_n]=ROOT::Math::VectorUtil::DeltaR((**iChargedHadrCand).p4(),(*theleadPFCand).p4());
	++ChargedHadrCands_n;
      }
      for (PFCandidateRefVector::const_iterator iNeutrHadrCand=(*thePFIsolatedTauTagInfo).PFNeutrHadrCands().begin();iNeutrHadrCand!=(*thePFIsolatedTauTagInfo).PFNeutrHadrCands().end();++iNeutrHadrCand){
	PFIsolatedTauTagInfo_NHCandDR[NeutrHadrCands_n]=ROOT::Math::VectorUtil::DeltaR((**iNeutrHadrCand).p4(),(*theleadPFCand).p4());
	PFIsolatedTauTagInfo_NHCandEt[NeutrHadrCands_n]=(**iNeutrHadrCand).et();
	++NeutrHadrCands_n;
      }
      for (PFCandidateRefVector::const_iterator iGammaCand=(*thePFIsolatedTauTagInfo).PFGammaCands().begin();iGammaCand!=(*thePFIsolatedTauTagInfo).PFGammaCands().end();++iGammaCand){
	PFIsolatedTauTagInfo_GCandDR[GammaCands_n]=ROOT::Math::VectorUtil::DeltaR((**iGammaCand).p4(),(*theleadPFCand).p4());
	PFIsolatedTauTagInfo_GCandEt[GammaCands_n]=(**iGammaCand).et();
	++GammaCands_n;
      }
      PFIsolatedTauTagInfo_signalrectks_number=(int)(*thePFIsolatedTauTagInfo).PFChargedHadrCandsInCone(theleadPFCand->momentum(),test_trackersignalcone_size_,test_Cand_minpt_).size();
      PFIsolatedTauTagInfo_isolrectks_number=(int)(*thePFIsolatedTauTagInfo).PFChargedHadrCandsInBand(theleadPFCand->momentum(),test_trackersignalcone_size_,test_trackerisolcone_size_,test_Cand_minpt_).size();
      PFIsolatedTauTagInfo_leadrectk_pt=theleadPFCand->momentum().Rho();
      
      if (PFIsolatedTauTagInfo_isolrectks_number==0 && (PFIsolatedTauTagInfo_signalrectks_number==1 || PFIsolatedTauTagInfo_signalrectks_number==3)) PFIsolatedTauTagInfo_passed_tracksel=1;
      else PFIsolatedTauTagInfo_passed_tracksel=0;
    }else{
      PFIsolatedTauTagInfo_signalrectks_number=0;
      PFIsolatedTauTagInfo_isolrectks_number=-100;
      PFIsolatedTauTagInfo_leadrectk_pt=-100.;
      PFIsolatedTauTagInfo_passed_tracksel=0;
    }
    PFIsolatedTauTagInfo_GenTau_visproducts_e=-100.;
    PFIsolatedTauTagInfo_GenTau_visproducts_et=-100.;
    PFIsolatedTauTagInfo_GenTau_visproducts_eta=-100.;
    PFIsolatedTauTagInfo_GenTau_visproducts_phi=-100.;
    PFIsolatedTauTagInfo_GenTau_visproducts_type=-100;
    PFIsolatedTauTagInfo_GenJet05_e=-100.;
    PFIsolatedTauTagInfo_GenJet05_et=-100.;
    PFIsolatedTauTagInfo_GenJet05_eta=-100.;    
    PFIsolatedTauTagInfo_GenJet05_phi=-100.;    
    double min1stdeltaR_GenTau_deltaR=100.;
    pair<HepLorentzVector,int> min1stdeltaR_GenTau_pair;
    double min1stdeltaR_SimTau_deltaR=100.;
    pair<HepLorentzVector,int> min1stdeltaR_SimTau_pair;
    double min1stdeltaR_GenJet05_deltaR=100.;
    HepLorentzVector min1stdeltaR_GenJet05_HepLV(0.,0.,0.,0.);
    if (GenTausnumber>0){
      for (vector<pair<HepLorentzVector,int> >::iterator iGenTau_pair=GenTau_pair.begin();iGenTau_pair!=GenTau_pair.end();iGenTau_pair++) {
	if ((*iGenTau_pair).first.deltaR(ThePFIsolatedTauTagJet_HepLV)<min1stdeltaR_GenTau_deltaR) {
	  min1stdeltaR_GenTau_pair=(*iGenTau_pair);
	  min1stdeltaR_GenTau_deltaR=(*iGenTau_pair).first.deltaR(ThePFIsolatedTauTagJet_HepLV);
	}      
      }
    }
    if (SimTausnumber>0){
      for (vector<pair<HepLorentzVector,int> >::iterator iSimTau_pair=SimTau_pair.begin();iSimTau_pair!=SimTau_pair.end();iSimTau_pair++) {
	if ((*iSimTau_pair).first.deltaR(ThePFIsolatedTauTagJet_HepLV)<min1stdeltaR_SimTau_deltaR) {
	  min1stdeltaR_SimTau_pair=(*iSimTau_pair);
	  min1stdeltaR_SimTau_deltaR=(*iSimTau_pair).first.deltaR(ThePFIsolatedTauTagJet_HepLV);
	}      
      }
    }
    if (GenJet05snumber>0){
      iGenJet05 = 0;
      for (vector<HepLorentzVector>::iterator iGenJet05_HepLV=GenJet05_HepLV.begin();iGenJet05_HepLV!=GenJet05_HepLV.end();iGenJet05_HepLV++) {
	if (iGenJet05>1)break;
	if ((*iGenJet05_HepLV).deltaR(ThePFIsolatedTauTagJet_HepLV)<min1stdeltaR_GenJet05_deltaR) {
	  min1stdeltaR_GenJet05_HepLV=(*iGenJet05_HepLV);
	  min1stdeltaR_GenJet05_deltaR=(*iGenJet05_HepLV).deltaR(ThePFIsolatedTauTagJet_HepLV);
	} 
	++iGenJet05;     
      }
    }
    if (min1stdeltaR_GenTau_deltaR<0.15){
      PFIsolatedTauTagInfo_GenTau_visproducts_e=(min1stdeltaR_GenTau_pair).first.e();
      PFIsolatedTauTagInfo_GenTau_visproducts_et=(min1stdeltaR_GenTau_pair).first.et();
      PFIsolatedTauTagInfo_GenTau_visproducts_eta=(min1stdeltaR_GenTau_pair).first.eta();
      PFIsolatedTauTagInfo_GenTau_visproducts_phi=(min1stdeltaR_GenTau_pair).first.phi();
      PFIsolatedTauTagInfo_GenTau_visproducts_type=min1stdeltaR_GenTau_pair.second;          
    }
    if (min1stdeltaR_SimTau_deltaR<0.15){
      PFIsolatedTauTagInfo_SimTau_visproducts_e=(min1stdeltaR_SimTau_pair).first.e();
      PFIsolatedTauTagInfo_SimTau_visproducts_et=(min1stdeltaR_SimTau_pair).first.et();
      PFIsolatedTauTagInfo_SimTau_visproducts_eta=(min1stdeltaR_SimTau_pair).first.eta();
      PFIsolatedTauTagInfo_SimTau_visproducts_phi=(min1stdeltaR_SimTau_pair).first.phi();
      PFIsolatedTauTagInfo_SimTau_visproducts_type=min1stdeltaR_SimTau_pair.second;          
    }
    if (min1stdeltaR_GenJet05_deltaR<.25){
      PFIsolatedTauTagInfo_GenJet05_e=min1stdeltaR_GenJet05_HepLV.e();
      PFIsolatedTauTagInfo_GenJet05_et=min1stdeltaR_GenJet05_HepLV.et();
      PFIsolatedTauTagInfo_GenJet05_eta=min1stdeltaR_GenJet05_HepLV.eta(); 
      PFIsolatedTauTagInfo_GenJet05_phi=min1stdeltaR_GenJet05_HepLV.phi(); 
    }    
    thePFIsolatedTauTagInfoTree->Fill();        
  }
  theEventTree->Fill(); 
}
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(PFConeIsolationAnalyzer);
