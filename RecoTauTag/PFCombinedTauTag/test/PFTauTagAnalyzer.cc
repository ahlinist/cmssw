/* class PFTauTagAnalyzer
 *  EDAnalyzer of the tagged TauJet with the PFCombinedTauTagAlgorithm, 
 *  created: Apr 23 2007,
 *  
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
#include "DataFormats/BTauReco/interface/PFCombinedTauTagInfo.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticle.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementTrack.h"

#include "DataFormats/GeometryVector/interface/GlobalTag.h"
#include "DataFormats/GeometryVector/interface/Vector3DBase.h"
#include "DataFormats/GeometryVector/interface/Point3DBase.h"

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

class PFTauTagAnalyzer : public EDAnalyzer {
public:
  explicit PFTauTagAnalyzer(const ParameterSet&);
  ~PFTauTagAnalyzer() {;}
  virtual void analyze(const Event& iEvent, const EventSetup& iSetup);
  virtual void beginJob();
  virtual void endJob();
private:
  string PFSimParticleProd_;
  string PFRecTrackProd_;
  string PFBlockElementTrackProd_;
  string PFCombinedTauTagProd_;
  string PVProd_;
  bool test_useOnlyChargedHadrCand_;
  double test_Cand_minpt_;
  bool test_useOnlyChargedHadrforleadPFCand_;
  double test_trackercone_size_;
  double test_matchingcone_size_;
  double test_leadCand_minpt_;
  string output_filename_;
  TFile* thefile;
  TTree* theEventTree; 
  TTree* thePFCombinedTauTagInfoTree; 
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
  int PFCombinedTauTagInfo_eventgets_recPV;
  float PFCombinedTauTagInfo_event_scale;
  float PFCombinedTauTagInfo_e;
  float PFCombinedTauTagInfo_et;
  float PFCombinedTauTagInfo_eta;
  float PFCombinedTauTagInfo_phi;
  float PFCombinedTauTagInfo_invmass;
  float PFCombinedTauTagInfo_mode;
  float PFCombinedTauTagInfo_modet;
  float PFCombinedTauTagInfo_modeta;
  float PFCombinedTauTagInfo_modphi;
  float PFCombinedTauTagInfo_modinvmass;
  float PFCombinedTauTagInfo_mode_bis;
  float PFCombinedTauTagInfo_modet_bis;
  float PFCombinedTauTagInfo_modeta_bis;
  float PFCombinedTauTagInfo_modphi_bis;
  int PFCombinedTauTagInfo_passed_tracksel;
  int PFCombinedTauTagInfo_infact_GoodElectronCandidate;
  int PFCombinedTauTagInfo_infact_GoodMuonCandidate;
  float PFCombinedTauTagInfo_ECALEt_o_leadtkPt;
  float PFCombinedTauTagInfo_HCALEt_o_leadtkPt;
  int PFCombinedTauTagInfo_rectks_number;
  int PFCombinedTauTagInfo_signalrectks_number;
  int PFCombinedTauTagInfo_isolrectks_number;
  int PFCombinedTauTagInfo_neutralECALclus_number;
  float PFCombinedTauTagInfo_neutralECALclus_radius;
  int PFCombinedTauTagInfo_neutralHCALclus_number;
  float PFCombinedTauTagInfo_neutralHCALclus_radius;
  float PFCombinedTauTagInfo_neutrE_ratio;
  float PFCombinedTauTagInfo_isolneutrE_o_tksEneutrE_ratio;
  float PFCombinedTauTagInfo_tksEtJetEt_ratio;
  float PFCombinedTauTagInfo_neutrEtksE_ratio;
  float PFCombinedTauTagInfo_leadtk_signedipt_significance;
  float PFCombinedTauTagInfo_leadtk_signedip3D_significance;
  float PFCombinedTauTagInfo_signedflightpath_significance;
  float PFCombinedTauTagInfo_discriminator;
  static const int ChargedHadrCands_nmax=100;
  int ChargedHadrCands_n;
  float PFCombinedTauTagInfo_CHCandDR[ChargedHadrCands_nmax];
  static const int NeutrHadrCands_nmax=100;
  int NeutrHadrCands_n;
  float PFCombinedTauTagInfo_NHCandDR[NeutrHadrCands_nmax];
  float PFCombinedTauTagInfo_NHCandEt[NeutrHadrCands_nmax];
  static const int GammaCands_nmax=100;
  int GammaCands_n;
  float PFCombinedTauTagInfo_GCandDR[GammaCands_nmax];
  float PFCombinedTauTagInfo_GCandEt[GammaCands_nmax];
  float PFCombinedTauTagInfo_GenTau_visproducts_e;
  float PFCombinedTauTagInfo_GenTau_visproducts_et;
  float PFCombinedTauTagInfo_GenTau_visproducts_eta;
  float PFCombinedTauTagInfo_GenTau_visproducts_phi;
  int PFCombinedTauTagInfo_GenTau_visproducts_type;
  float PFCombinedTauTagInfo_SimTau_visproducts_e;
  float PFCombinedTauTagInfo_SimTau_visproducts_et;
  float PFCombinedTauTagInfo_SimTau_visproducts_eta;
  float PFCombinedTauTagInfo_SimTau_visproducts_phi;
  int PFCombinedTauTagInfo_SimTau_visproducts_type;
  float PFCombinedTauTagInfo_GenJet05_e;
  float PFCombinedTauTagInfo_GenJet05_et;
  float PFCombinedTauTagInfo_GenJet05_eta;
  float PFCombinedTauTagInfo_GenJet05_phi;
  class BydecreasingEt {
  public:
    bool operator()(HepLorentzVector a,HepLorentzVector b) {
      return (double)a.et()>(double)b.et();      
    }
  };
};

PFTauTagAnalyzer::PFTauTagAnalyzer(const edm::ParameterSet& iConfig){  
  PFSimParticleProd_                    = iConfig.getParameter<string>("PFSimParticleProd");
  PFRecTrackProd_                       = iConfig.getParameter<string>("PFRecTrackProd");
  PFBlockElementTrackProd_              = iConfig.getParameter<string>("PFBlockElementTrackProd");
  PFCombinedTauTagProd_                 = iConfig.getParameter<string>("PFCombinedTauTagProd");
  PVProd_                               = iConfig.getParameter<string>("PVProd");
  test_useOnlyChargedHadrCand_          = iConfig.getParameter<bool>("test_useOnlyChargedHadrCand");
  test_Cand_minpt_                      = iConfig.getParameter<double>("test_Cand_minpt");
  test_useOnlyChargedHadrforleadPFCand_ = iConfig.getParameter<bool>("test_useOnlyChargedHadrforleadPFCand");
  test_trackercone_size_                = iConfig.getParameter<double>("test_trackercone_size");
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
  thePFCombinedTauTagInfoTree = new TTree("thePFCombinedTauTagInfoTree", "thePFCombinedTauTagInfoTree"); 
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_eventgets_recPV",&PFCombinedTauTagInfo_eventgets_recPV,"PFCombinedTauTagInfo_eventgets_recPV/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_event_scale",&PFCombinedTauTagInfo_event_scale,"PFCombinedTauTagInfo_event_scale/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_e",&PFCombinedTauTagInfo_e,"PFCombinedTauTagInfo_e/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_et",&PFCombinedTauTagInfo_et,"PFCombinedTauTagInfo_et/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_eta",&PFCombinedTauTagInfo_eta,"PFCombinedTauTagInfo_eta/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_phi",&PFCombinedTauTagInfo_phi,"PFCombinedTauTagInfo_phi/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_invmass",&PFCombinedTauTagInfo_invmass,"PFCombinedTauTagInfo_invmass/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_mode",&PFCombinedTauTagInfo_mode,"PFCombinedTauTagInfo_mode/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_modet",&PFCombinedTauTagInfo_modet,"PFCombinedTauTagInfo_modet/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_modeta",&PFCombinedTauTagInfo_modeta,"PFCombinedTauTagInfo_modeta/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_modphi",&PFCombinedTauTagInfo_modphi,"PFCombinedTauTagInfo_modphi/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_modinvmass",&PFCombinedTauTagInfo_modinvmass,"PFCombinedTauTagInfo_modinvmass/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_mode_bis",&PFCombinedTauTagInfo_mode_bis,"PFCombinedTauTagInfo_mode_bis/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_modet_bis",&PFCombinedTauTagInfo_modet_bis,"PFCombinedTauTagInfo_modet_bis/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_modeta_bis",&PFCombinedTauTagInfo_modeta_bis,"PFCombinedTauTagInfo_modeta_bis/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_modphi_bis",&PFCombinedTauTagInfo_modphi_bis,"PFCombinedTauTagInfo_modphi_bis/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_passed_tracksel",&PFCombinedTauTagInfo_passed_tracksel,"PFCombinedTauTagInfo_passed_tracksel/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_rectks_number",&PFCombinedTauTagInfo_rectks_number,"PFCombinedTauTagInfo_rectks_number/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_signalrectks_number",&PFCombinedTauTagInfo_signalrectks_number,"PFCombinedTauTagInfo_signalrectks_number/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_isolrectks_number",&PFCombinedTauTagInfo_isolrectks_number,"PFCombinedTauTagInfo_isolrectks_number/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_neutralECALclus_number",&PFCombinedTauTagInfo_neutralECALclus_number,"PFCombinedTauTagInfo_neutralECALclus_number/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_neutralECALclus_radius",&PFCombinedTauTagInfo_neutralECALclus_radius,"PFCombinedTauTagInfo_neutralECALclus_radius/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_neutralHCALclus_number",&PFCombinedTauTagInfo_neutralHCALclus_number,"PFCombinedTauTagInfo_neutralHCALclus_number/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_neutralHCALclus_radius",&PFCombinedTauTagInfo_neutralHCALclus_radius,"PFCombinedTauTagInfo_neutralHCALclus_radius/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_neutrE_ratio",&PFCombinedTauTagInfo_neutrE_ratio,"PFCombinedTauTagInfo_neutrE_ratio/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_tksEtJetEt_ratio",&PFCombinedTauTagInfo_tksEtJetEt_ratio,"PFCombinedTauTagInfo_tksEtJetEt_ratio/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_neutrEtksE_ratio",&PFCombinedTauTagInfo_neutrEtksE_ratio,"PFCombinedTauTagInfo_neutrEtksE_ratio/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_isolneutrE_o_tksEneutrE_ratio",&PFCombinedTauTagInfo_isolneutrE_o_tksEneutrE_ratio,"PFCombinedTauTagInfo_isolneutrE_o_tksEneutrE_ratio/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_leadtk_signedipt_significance",&PFCombinedTauTagInfo_leadtk_signedipt_significance,"PFCombinedTauTagInfo_leadtk_signedipt_significance/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_leadtk_signedip3D_significance",&PFCombinedTauTagInfo_leadtk_signedip3D_significance,"PFCombinedTauTagInfo_leadtk_signedip3D_significance/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_signedflightpath_significance",&PFCombinedTauTagInfo_signedflightpath_significance,"PFCombinedTauTagInfo_signedflightpath_significance/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_infact_GoodElectronCandidate",&PFCombinedTauTagInfo_infact_GoodElectronCandidate,"PFCombinedTauTagInfo_infact_GoodElectronCandidate/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_infact_GoodMuonCandidate",&PFCombinedTauTagInfo_infact_GoodMuonCandidate,"PFCombinedTauTagInfo_infact_GoodMuonCandidate/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_ECALEt_o_leadtkPt",&PFCombinedTauTagInfo_ECALEt_o_leadtkPt,"PFCombinedTauTagInfo_ECALEt_o_leadtkPt/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_HCALEt_o_leadtkPt",&PFCombinedTauTagInfo_HCALEt_o_leadtkPt,"PFCombinedTauTagInfo_HCALEt_o_leadtkPt/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_discriminator",&PFCombinedTauTagInfo_discriminator,"PFCombinedTauTagInfo_discriminator/F");
  thePFCombinedTauTagInfoTree->Branch("ChargedHadrCands_n",&ChargedHadrCands_n,"ChargedHadrCands_n/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_CHCandDR",PFCombinedTauTagInfo_CHCandDR,"PFCombinedTauTagInfo_CHCandDR[ChargedHadrCands_n]/F");
  thePFCombinedTauTagInfoTree->Branch("NeutrHadrCands_n",&NeutrHadrCands_n,"NeutrHadrCands_n/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_NHCandDR",PFCombinedTauTagInfo_NHCandDR,"PFCombinedTauTagInfo_NHCandDR[NeutrHadrCands_n]/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_NHCandEt",PFCombinedTauTagInfo_NHCandEt,"PFCombinedTauTagInfo_NHCandEt[NeutrHadrCands_n]/F");
  thePFCombinedTauTagInfoTree->Branch("GammaCands_n",&GammaCands_n,"GammaCands_n/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_GCandDR",PFCombinedTauTagInfo_GCandDR,"PFCombinedTauTagInfo_GCandDR[GammaCands_n]/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_GCandEt",PFCombinedTauTagInfo_GCandEt,"PFCombinedTauTagInfo_GCandEt[GammaCands_n]/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_GenTau_visproducts_e",&PFCombinedTauTagInfo_GenTau_visproducts_e,"PFCombinedTauTagInfo_GenTau_visproducts_e/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_GenTau_visproducts_et",&PFCombinedTauTagInfo_GenTau_visproducts_et,"PFCombinedTauTagInfo_GenTau_visproducts_et/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_GenTau_visproducts_eta",&PFCombinedTauTagInfo_GenTau_visproducts_eta,"PFCombinedTauTagInfo_GenTau_visproducts_eta/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_GenTau_visproducts_phi",&PFCombinedTauTagInfo_GenTau_visproducts_phi,"PFCombinedTauTagInfo_GenTau_visproducts_phi/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_GenTau_visproducts_type",&PFCombinedTauTagInfo_GenTau_visproducts_type,"PFCombinedTauTagInfo_GenTau_visproducts_type/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_SimTau_visproducts_e",&PFCombinedTauTagInfo_SimTau_visproducts_e,"PFCombinedTauTagInfo_SimTau_visproducts_e/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_SimTau_visproducts_et",&PFCombinedTauTagInfo_SimTau_visproducts_et,"PFCombinedTauTagInfo_SimTau_visproducts_et/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_SimTau_visproducts_eta",&PFCombinedTauTagInfo_SimTau_visproducts_eta,"PFCombinedTauTagInfo_SimTau_visproducts_eta/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_SimTau_visproducts_phi",&PFCombinedTauTagInfo_SimTau_visproducts_phi,"PFCombinedTauTagInfo_SimTau_visproducts_phi/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_SimTau_visproducts_type",&PFCombinedTauTagInfo_SimTau_visproducts_type,"PFCombinedTauTagInfo_SimTau_visproducts_type/I");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_GenJet05_e",&PFCombinedTauTagInfo_GenJet05_e,"PFCombinedTauTagInfo_GenJet05_e/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_GenJet05_et",&PFCombinedTauTagInfo_GenJet05_et,"PFCombinedTauTagInfo_GenJet05_et/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_GenJet05_eta",&PFCombinedTauTagInfo_GenJet05_eta,"PFCombinedTauTagInfo_GenJet05_eta/F");
  thePFCombinedTauTagInfoTree->Branch("PFCombinedTauTagInfo_GenJet05_phi",&PFCombinedTauTagInfo_GenJet05_phi,"PFCombinedTauTagInfo_GenJet05_phi/F");
}
void PFTauTagAnalyzer::beginJob(){}
void PFTauTagAnalyzer::endJob(){
  thefile->cd();
  theEventTree->Write();
  thePFCombinedTauTagInfoTree->Write();
  thefile->Write();
  thefile->Close();
}
void PFTauTagAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
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
  event_scale=(*(evt->GetEvent())).event_scale();
  
  // select susy processes
  /*
  if ((*(evt->GetEvent())).signal_process_id()<200 ||  (*(evt->GetEvent())).signal_process_id()>300){
    delete myGenEvent;
    return;
  }
  */
   
  // select gamma*/Z0 processes
  /*
  if ((*(evt->GetEvent())).signal_process_id()!=1){
    delete myGenEvent;
    return;
  }
  */
  
  // select QCD-dijet processes
  /*   
  if ((*(evt->GetEvent())).signal_process_id()!=11 
      && (*(evt->GetEvent())).signal_process_id()!=12 
      && (*(evt->GetEvent())).signal_process_id()!=13 
      && (*(evt->GetEvent())).signal_process_id()!=68 
      && (*(evt->GetEvent())).signal_process_id()!=28 
      && (*(evt->GetEvent())).signal_process_id()!=53){
    delete myGenEvent;
    return;
  }
  */
  int iGenTau = 0;
  for (HepMC::GenEvent::particle_const_iterator iter=(*(evt->GetEvent())).particles_begin();iter!=(*(evt->GetEvent())).particles_end();iter++) {
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
  iEvent.getByLabel(PFCombinedTauTagProd_,jetTagHandle);
  const JetTagCollection& myJetTagCollection=*(jetTagHandle.product()); 
  for (JetTagCollection::const_iterator iJetTag=myJetTagCollection.begin();iJetTag!=myJetTagCollection.end();++iJetTag) {
    PFCombinedTauTagInfoRef thePFCombinedTauTagInfo=(*iJetTag).tagInfoRef().castTo<PFCombinedTauTagInfoRef>();
    HepLorentzVector ThePFCombinedTauTagJet_HepLV((*thePFCombinedTauTagInfo).pfjetRef()->px(),(*thePFCombinedTauTagInfo).pfjetRef()->py(),(*thePFCombinedTauTagInfo).pfjetRef()->pz(),(*thePFCombinedTauTagInfo).pfjetRef()->energy());
    PFCombinedTauTagInfo_eventgets_recPV=gets_recPV;
    PFCombinedTauTagInfo_event_scale=event_scale;
    PFCombinedTauTagInfo_e=ThePFCombinedTauTagJet_HepLV.e();
    PFCombinedTauTagInfo_et=ThePFCombinedTauTagJet_HepLV.et();
    PFCombinedTauTagInfo_eta=ThePFCombinedTauTagJet_HepLV.eta();
    PFCombinedTauTagInfo_phi=ThePFCombinedTauTagJet_HepLV.phi();
    PFCombinedTauTagInfo_invmass=ThePFCombinedTauTagJet_HepLV.m();
    PFCombinedTauTagInfo_mode=(*thePFCombinedTauTagInfo).alternatLorentzVect().E();
    PFCombinedTauTagInfo_modet=(*thePFCombinedTauTagInfo).alternatLorentzVect().Et();
    PFCombinedTauTagInfo_modeta=(*thePFCombinedTauTagInfo).alternatLorentzVect().Eta();
    PFCombinedTauTagInfo_modphi=(*thePFCombinedTauTagInfo).alternatLorentzVect().Phi();
    PFCombinedTauTagInfo_modinvmass=(*thePFCombinedTauTagInfo).alternatLorentzVect().M();
    PFCombinedTauTagInfo_passed_tracksel=(*thePFCombinedTauTagInfo).selectedByPFChargedHadrCands();
    PFCombinedTauTagInfo_infact_GoodElectronCandidate=(*thePFCombinedTauTagInfo).electronTagged();
    PFCombinedTauTagInfo_infact_GoodMuonCandidate=(*thePFCombinedTauTagInfo).muonTagged();
    if (!isnan((*thePFCombinedTauTagInfo).ECALEtleadPFChargedHadrCandPtRatio())) PFCombinedTauTagInfo_ECALEt_o_leadtkPt=(*thePFCombinedTauTagInfo).ECALEtleadPFChargedHadrCandPtRatio();
    else PFCombinedTauTagInfo_ECALEt_o_leadtkPt=-100.;
    if (!isnan((*thePFCombinedTauTagInfo).HCALEtleadPFChargedHadrCandPtRatio())) PFCombinedTauTagInfo_HCALEt_o_leadtkPt=(*thePFCombinedTauTagInfo).HCALEtleadPFChargedHadrCandPtRatio();
    else PFCombinedTauTagInfo_HCALEt_o_leadtkPt=-100.;
    PFCombinedTauTagInfo_signalrectks_number=(int)(*thePFCombinedTauTagInfo).signalPFChargedHadrCands().size();
    PFCombinedTauTagInfo_isolrectks_number=(int)(*thePFCombinedTauTagInfo).isolPFChargedHadrCands().size();
    PFCombinedTauTagInfo_neutralECALclus_number=(*thePFCombinedTauTagInfo).PFGammaCandsN();
    if (!isnan((*thePFCombinedTauTagInfo).PFGammaCandsRadius())) PFCombinedTauTagInfo_neutralECALclus_radius=(*thePFCombinedTauTagInfo).PFGammaCandsRadius();
    else PFCombinedTauTagInfo_neutralECALclus_radius=-100.;
    PFCombinedTauTagInfo_neutralHCALclus_number=(*thePFCombinedTauTagInfo).PFNeutrHadrCandsN();
    if (!isnan((*thePFCombinedTauTagInfo).PFNeutrHadrCandsRadius())) PFCombinedTauTagInfo_neutralHCALclus_radius=(*thePFCombinedTauTagInfo).PFNeutrHadrCandsRadius();
    else PFCombinedTauTagInfo_neutralHCALclus_radius=-100.;
    if (!isnan((*thePFCombinedTauTagInfo).PFGammaCandsERatio())) PFCombinedTauTagInfo_neutrE_ratio=(*thePFCombinedTauTagInfo).PFGammaCandsERatio();
    else PFCombinedTauTagInfo_neutrE_ratio=-100.;
    PFCombinedTauTagInfo_isolneutrE_o_tksEneutrE_ratio=(*thePFCombinedTauTagInfo).isolPFGammaCandsEJetalternatERatio();
    PFCombinedTauTagInfo_tksEtJetEt_ratio=(*thePFCombinedTauTagInfo).PFChargedHadrCandsEtJetEtRatio();
    PFCombinedTauTagInfo_neutrEtksE_ratio=(*thePFCombinedTauTagInfo).PFGammaCandsEJetalternatERatio();   
    if (!isnan((*thePFCombinedTauTagInfo).leadPFChargedHadrCandsignedSipt())) PFCombinedTauTagInfo_leadtk_signedipt_significance=(*thePFCombinedTauTagInfo).leadPFChargedHadrCandsignedSipt();
    else PFCombinedTauTagInfo_leadtk_signedipt_significance=-100.;
    if (!isnan((*thePFCombinedTauTagInfo).leadPFChargedHadrCandsignedSip3D())) PFCombinedTauTagInfo_leadtk_signedip3D_significance=(*thePFCombinedTauTagInfo).leadPFChargedHadrCandsignedSip3D();
    else PFCombinedTauTagInfo_leadtk_signedip3D_significance=-100.;
    if (!isnan((*thePFCombinedTauTagInfo).signedSflightpath())) PFCombinedTauTagInfo_signedflightpath_significance=(*thePFCombinedTauTagInfo).signedSflightpath();
    else PFCombinedTauTagInfo_signedflightpath_significance=-100.;
    if (!isnan((*iJetTag).discriminator())) PFCombinedTauTagInfo_discriminator=(*iJetTag).discriminator();
    else PFCombinedTauTagInfo_discriminator=-100.;
    math::XYZTLorentzVector recjet_alternatXYZTLorentzVector;
    recjet_alternatXYZTLorentzVector.SetPx(0.);
    recjet_alternatXYZTLorentzVector.SetPy(0.);
    recjet_alternatXYZTLorentzVector.SetPz(0.);
    recjet_alternatXYZTLorentzVector.SetE(0.);
    ChargedHadrCands_n=0;
    NeutrHadrCands_n=0;
    GammaCands_n=0;
    PFCandidateRef theleadPFCand;
    if (test_useOnlyChargedHadrforleadPFCand_) theleadPFCand=(*thePFCombinedTauTagInfo).isolatedtautaginfoRef()->leadPFChargedHadrCand(test_matchingcone_size_,test_leadCand_minpt_);
    else theleadPFCand=(*thePFCombinedTauTagInfo).isolatedtautaginfoRef()->leadPFCand(test_matchingcone_size_,test_leadCand_minpt_);
    if(!theleadPFCand.isNull()){
      PFCombinedTauTagInfo_rectks_number=(int)(*thePFCombinedTauTagInfo).isolatedtautaginfoRef()->PFChargedHadrCandsInCone((*theleadPFCand).momentum(),test_trackercone_size_,test_Cand_minpt_).size();
      for (PFCandidateRefVector::const_iterator iChargedHadrCand=(*thePFCombinedTauTagInfo).isolatedtautaginfoRef()->PFChargedHadrCands().begin();iChargedHadrCand!=(*thePFCombinedTauTagInfo).isolatedtautaginfoRef()->PFChargedHadrCands().end();++iChargedHadrCand){
	recjet_alternatXYZTLorentzVector+=(**iChargedHadrCand).p4();
	PFCombinedTauTagInfo_CHCandDR[ChargedHadrCands_n]=-100.;
	if ((*iChargedHadrCand)!=theleadPFCand) PFCombinedTauTagInfo_CHCandDR[ChargedHadrCands_n]=ROOT::Math::VectorUtil::DeltaR((**iChargedHadrCand).p4(),(*theleadPFCand).p4());
	++ChargedHadrCands_n;
      }
      for (PFCandidateRefVector::const_iterator iNeutrHadrCand=(*thePFCombinedTauTagInfo).isolatedtautaginfoRef()->PFNeutrHadrCands().begin();iNeutrHadrCand!=(*thePFCombinedTauTagInfo).isolatedtautaginfoRef()->PFNeutrHadrCands().end();++iNeutrHadrCand){
	PFCombinedTauTagInfo_NHCandDR[NeutrHadrCands_n]=ROOT::Math::VectorUtil::DeltaR((**iNeutrHadrCand).p4(),(*theleadPFCand).p4());
	PFCombinedTauTagInfo_NHCandEt[NeutrHadrCands_n]=(**iNeutrHadrCand).et();
	++NeutrHadrCands_n;
      }
      for (PFCandidateRefVector::const_iterator iGammaCand=(*thePFCombinedTauTagInfo).isolatedtautaginfoRef()->PFGammaCands().begin();iGammaCand!=(*thePFCombinedTauTagInfo).isolatedtautaginfoRef()->PFGammaCands().end();++iGammaCand){
	recjet_alternatXYZTLorentzVector+=(**iGammaCand).p4();
	PFCombinedTauTagInfo_GCandDR[GammaCands_n]=ROOT::Math::VectorUtil::DeltaR((**iGammaCand).p4(),(*theleadPFCand).p4());
	PFCombinedTauTagInfo_GCandEt[GammaCands_n]=(**iGammaCand).et();
	++GammaCands_n;
      }      
    }else PFCombinedTauTagInfo_rectks_number=0;
    PFCombinedTauTagInfo_mode_bis=recjet_alternatXYZTLorentzVector.E();
    PFCombinedTauTagInfo_modet_bis=recjet_alternatXYZTLorentzVector.Et();
    PFCombinedTauTagInfo_modeta_bis=recjet_alternatXYZTLorentzVector.Eta();
    PFCombinedTauTagInfo_modphi_bis=recjet_alternatXYZTLorentzVector.Phi();   
    PFCombinedTauTagInfo_GenTau_visproducts_e=-100.;
    PFCombinedTauTagInfo_GenTau_visproducts_et=-100.;
    PFCombinedTauTagInfo_GenTau_visproducts_eta=-100.;
    PFCombinedTauTagInfo_GenTau_visproducts_phi=-100.;
    PFCombinedTauTagInfo_GenTau_visproducts_type=-100;
    PFCombinedTauTagInfo_GenJet05_e=-100.;
    PFCombinedTauTagInfo_GenJet05_et=-100.;
    PFCombinedTauTagInfo_GenJet05_eta=-100.;    
    PFCombinedTauTagInfo_GenJet05_phi=-100.;    
    double min1stdeltaR_GenTau_deltaR=100.;
    pair<HepLorentzVector,int> min1stdeltaR_GenTau_pair;
    double min1stdeltaR_SimTau_deltaR=100.;
    pair<HepLorentzVector,int> min1stdeltaR_SimTau_pair;
    double min1stdeltaR_GenJet05_deltaR=100.;
    HepLorentzVector min1stdeltaR_GenJet05_HepLV(0.,0.,0.,0.);
    if (GenTausnumber>0){
      for (vector<pair<HepLorentzVector,int> >::iterator iGenTau_pair=GenTau_pair.begin();iGenTau_pair!=GenTau_pair.end();iGenTau_pair++) {
	if ((*iGenTau_pair).first.deltaR(ThePFCombinedTauTagJet_HepLV)<min1stdeltaR_GenTau_deltaR) {
	  min1stdeltaR_GenTau_pair=(*iGenTau_pair);
	  min1stdeltaR_GenTau_deltaR=(*iGenTau_pair).first.deltaR(ThePFCombinedTauTagJet_HepLV);
	}      
      }
    }
    if (SimTausnumber>0){
      for (vector<pair<HepLorentzVector,int> >::iterator iSimTau_pair=SimTau_pair.begin();iSimTau_pair!=SimTau_pair.end();iSimTau_pair++) {
	if ((*iSimTau_pair).first.deltaR(ThePFCombinedTauTagJet_HepLV)<min1stdeltaR_SimTau_deltaR) {
	  min1stdeltaR_SimTau_pair=(*iSimTau_pair);
	  min1stdeltaR_SimTau_deltaR=(*iSimTau_pair).first.deltaR(ThePFCombinedTauTagJet_HepLV);
	}      
      }
    }
    if (GenJet05snumber>0){
      iGenJet05 = 0;
      for (vector<HepLorentzVector>::iterator iGenJet05_HepLV=GenJet05_HepLV.begin();iGenJet05_HepLV!=GenJet05_HepLV.end();iGenJet05_HepLV++) {
	if (iGenJet05>1)break;
	if ((*iGenJet05_HepLV).deltaR(ThePFCombinedTauTagJet_HepLV)<min1stdeltaR_GenJet05_deltaR) {
	  min1stdeltaR_GenJet05_HepLV=(*iGenJet05_HepLV);
	  min1stdeltaR_GenJet05_deltaR=(*iGenJet05_HepLV).deltaR(ThePFCombinedTauTagJet_HepLV);
	} 
	++iGenJet05;     
      }
    }
    if (min1stdeltaR_GenTau_deltaR<0.15){
      PFCombinedTauTagInfo_GenTau_visproducts_e=(min1stdeltaR_GenTau_pair).first.e();
      PFCombinedTauTagInfo_GenTau_visproducts_et=(min1stdeltaR_GenTau_pair).first.et();
      PFCombinedTauTagInfo_GenTau_visproducts_eta=(min1stdeltaR_GenTau_pair).first.eta();
      PFCombinedTauTagInfo_GenTau_visproducts_phi=(min1stdeltaR_GenTau_pair).first.phi();
      PFCombinedTauTagInfo_GenTau_visproducts_type=min1stdeltaR_GenTau_pair.second;          
    }
    if (min1stdeltaR_SimTau_deltaR<0.15){
      PFCombinedTauTagInfo_SimTau_visproducts_e=(min1stdeltaR_SimTau_pair).first.e();
      PFCombinedTauTagInfo_SimTau_visproducts_et=(min1stdeltaR_SimTau_pair).first.et();
      PFCombinedTauTagInfo_SimTau_visproducts_eta=(min1stdeltaR_SimTau_pair).first.eta();
      PFCombinedTauTagInfo_SimTau_visproducts_phi=(min1stdeltaR_SimTau_pair).first.phi();
      PFCombinedTauTagInfo_SimTau_visproducts_type=min1stdeltaR_SimTau_pair.second;          
    }
    if (min1stdeltaR_GenJet05_deltaR<.25){
      PFCombinedTauTagInfo_GenJet05_e=min1stdeltaR_GenJet05_HepLV.e();
      PFCombinedTauTagInfo_GenJet05_et=min1stdeltaR_GenJet05_HepLV.et();
      PFCombinedTauTagInfo_GenJet05_eta=min1stdeltaR_GenJet05_HepLV.eta(); 
      PFCombinedTauTagInfo_GenJet05_phi=min1stdeltaR_GenJet05_HepLV.phi(); 
    }    
    thePFCombinedTauTagInfoTree->Fill();        
  }
  theEventTree->Fill(); 
}
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(PFTauTagAnalyzer);
