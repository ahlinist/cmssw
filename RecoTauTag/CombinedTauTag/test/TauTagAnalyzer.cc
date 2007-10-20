/* class TauTagAnalyzer
 *  EDAnalyzer of the tagged TauJet with the CombinedTauTagAlgorithm, 
 *  created: Dec 18 2006,
 *  revised: May 10 2007,
 *  author: Ludovic Houchu.
 */
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h" 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h" 

#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/CombinedTauTagInfo.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/GeometryVector/interface/GlobalTag.h"
#include "DataFormats/GeometryVector/interface/Vector3DBase.h"
#include "DataFormats/GeometryVector/interface/Point3DBase.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "RecoTauTag/CombinedTauTag/interface/ECALBounds.h"

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

class TauTagAnalyzer : public EDAnalyzer {
public:
  explicit TauTagAnalyzer(const ParameterSet&);
  ~TauTagAnalyzer() {;}
  virtual void analyze(const Event& iEvent, const EventSetup& iSetup);
  virtual void beginJob();
  virtual void endJob();
private:
  string CombinedTauTagProd_;
  string PVProd_;
  string output_filename_;
  TFile* thefile;
  TTree* theEventTree; 
  TTree* theCombinedTauTagInfoTree; 
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
  static const int MaxGenJet05snumber=200;
  int GenJet05snumber;
  float GenJet05_e[MaxGenJet05snumber];
  float GenJet05_et[MaxGenJet05snumber];
  float GenJet05_eta[MaxGenJet05snumber];
  float GenJet05_phi[MaxGenJet05snumber];
  int CombinedTauTagInfo_eventgets_recPV;
  float CombinedTauTagInfo_event_scale;
  float CombinedTauTagInfo_e;
  float CombinedTauTagInfo_et;
  float CombinedTauTagInfo_eta;
  float CombinedTauTagInfo_phi;
  float CombinedTauTagInfo_mode;
  float CombinedTauTagInfo_modet;
  float CombinedTauTagInfo_modeta;
  float CombinedTauTagInfo_modphi;
  float CombinedTauTagInfo_invmass;
  int CombinedTauTagInfo_passed_tracksel;
  int CombinedTauTagInfo_is_GoodTauCandidate;
  int CombinedTauTagInfo_infact_GoodElectronCandidate;
  int CombinedTauTagInfo_infact_GoodMuonCandidate;
  float CombinedTauTagInfo_ECALEt_o_leadtkPt;
  float CombinedTauTagInfo_HCALEt_o_leadtkPt;
  int CombinedTauTagInfo_needs_LikelihoodRatio_discrimination;
  int CombinedTauTagInfo_signalrectks_number;
  int CombinedTauTagInfo_neutralECALclus_number;
  float CombinedTauTagInfo_neutralECALclus_radius;
  float CombinedTauTagInfo_neutrE_ratio;
  float CombinedTauTagInfo_isolneutrE_o_tksEneutrE_ratio;
  float CombinedTauTagInfo_tksEtJetEt_ratio;
  float CombinedTauTagInfo_neutrEtksE_ratio;
  float CombinedTauTagInfo_leadtk_signedipt_significance;
  float CombinedTauTagInfo_leadtk_signedip3D_significance;
  float CombinedTauTagInfo_signedflightpath_significance;
  float CombinedTauTagInfo_discriminator;
  float CombinedTauTagInfo_GenTau_visproducts_e;
  float CombinedTauTagInfo_GenTau_visproducts_et;
  float CombinedTauTagInfo_GenTau_visproducts_eta;
  float CombinedTauTagInfo_GenTau_visproducts_phi;
  int CombinedTauTagInfo_GenTau_visproducts_type;
  float CombinedTauTagInfo_GenJet05_e;
  float CombinedTauTagInfo_GenJet05_et;
  float CombinedTauTagInfo_GenJet05_eta;
  float CombinedTauTagInfo_GenJet05_phi;
  class BydecreasingEt {
  public:
    bool operator()(HepLorentzVector a,HepLorentzVector b) {
      return (double)a.et()>(double)b.et();      
    }
  };
};

TauTagAnalyzer::TauTagAnalyzer(const edm::ParameterSet& iConfig){  
  CombinedTauTagProd_ = iConfig.getParameter<string>("CombinedTauTagProd");
  PVProd_ = iConfig.getParameter<string>("PVProd");
  output_filename_=iConfig.getParameter<string>("output_filename");
  thefile=TFile::Open(output_filename_.c_str(),"recreate");  
  thefile->cd();
  theEventTree = new TTree("theEventTree", "theEventTree");
  theEventTree->Branch("gets_recPV",&gets_recPV,"gets_recPV/I");
  theEventTree->Branch("event_scale",&event_scale,"event_scale/F");
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
  theCombinedTauTagInfoTree = new TTree("theCombinedTauTagInfoTree", "theCombinedTauTagInfoTree"); 
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_eventgets_recPV",&CombinedTauTagInfo_eventgets_recPV,"CombinedTauTagInfo_eventgets_recPV/I");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_event_scale",&CombinedTauTagInfo_event_scale,"CombinedTauTagInfo_event_scale/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_e",&CombinedTauTagInfo_e,"CombinedTauTagInfo_e/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_et",&CombinedTauTagInfo_et,"CombinedTauTagInfo_et/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_eta",&CombinedTauTagInfo_eta,"CombinedTauTagInfo_eta/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_phi",&CombinedTauTagInfo_phi,"CombinedTauTagInfo_phi/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_mode",&CombinedTauTagInfo_mode,"CombinedTauTagInfo_mode/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_modet",&CombinedTauTagInfo_modet,"CombinedTauTagInfo_modet/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_modeta",&CombinedTauTagInfo_modeta,"CombinedTauTagInfo_modeta/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_modphi",&CombinedTauTagInfo_modphi,"CombinedTauTagInfo_modphi/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_invmass",&CombinedTauTagInfo_invmass,"CombinedTauTagInfo_invmass/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_passed_tracksel",&CombinedTauTagInfo_passed_tracksel,"CombinedTauTagInfo_passed_tracksel/I");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_is_GoodTauCandidate",&CombinedTauTagInfo_is_GoodTauCandidate,"CombinedTauTagInfo_is_GoodTauCandidate/I");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_needs_LikelihoodRatio_discrimination",&CombinedTauTagInfo_needs_LikelihoodRatio_discrimination,"CombinedTauTagInfo_needs_LikelihoodRatio_discrimination/I");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_signalrectks_number",&CombinedTauTagInfo_signalrectks_number,"CombinedTauTagInfo_signalrectks_number/I");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_neutralECALclus_number",&CombinedTauTagInfo_neutralECALclus_number,"CombinedTauTagInfo_neutralECALclus_number/I");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_neutralECALclus_radius",&CombinedTauTagInfo_neutralECALclus_radius,"CombinedTauTagInfo_neutralECALclus_radius/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_neutrE_ratio",&CombinedTauTagInfo_neutrE_ratio,"CombinedTauTagInfo_neutrE_ratio/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_tksEtJetEt_ratio",&CombinedTauTagInfo_tksEtJetEt_ratio,"CombinedTauTagInfo_tksEtJetEt_ratio/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_neutrEtksE_ratio",&CombinedTauTagInfo_neutrEtksE_ratio,"CombinedTauTagInfo_neutrEtksE_ratio/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_isolneutrE_o_tksEneutrE_ratio",&CombinedTauTagInfo_isolneutrE_o_tksEneutrE_ratio,"CombinedTauTagInfo_isolneutrE_o_tksEneutrE_ratio/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_leadtk_signedipt_significance",&CombinedTauTagInfo_leadtk_signedipt_significance,"CombinedTauTagInfo_leadtk_signedipt_significance/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_leadtk_signedip3D_significance",&CombinedTauTagInfo_leadtk_signedip3D_significance,"CombinedTauTagInfo_leadtk_signedip3D_significance/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_signedflightpath_significance",&CombinedTauTagInfo_signedflightpath_significance,"CombinedTauTagInfo_signedflightpath_significance/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_infact_GoodElectronCandidate",&CombinedTauTagInfo_infact_GoodElectronCandidate,"CombinedTauTagInfo_infact_GoodElectronCandidate/I");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_infact_GoodMuonCandidate",&CombinedTauTagInfo_infact_GoodMuonCandidate,"CombinedTauTagInfo_infact_GoodMuonCandidate/I");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_ECALEt_o_leadtkPt",&CombinedTauTagInfo_ECALEt_o_leadtkPt,"CombinedTauTagInfo_ECALEt_o_leadtkPt/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_HCALEt_o_leadtkPt",&CombinedTauTagInfo_HCALEt_o_leadtkPt,"CombinedTauTagInfo_HCALEt_o_leadtkPt/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_discriminator",&CombinedTauTagInfo_discriminator,"CombinedTauTagInfo_discriminator/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_GenTau_visproducts_e",&CombinedTauTagInfo_GenTau_visproducts_e,"CombinedTauTagInfo_GenTau_visproducts_e/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_GenTau_visproducts_et",&CombinedTauTagInfo_GenTau_visproducts_et,"CombinedTauTagInfo_GenTau_visproducts_et/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_GenTau_visproducts_eta",&CombinedTauTagInfo_GenTau_visproducts_eta,"CombinedTauTagInfo_GenTau_visproducts_eta/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_GenTau_visproducts_phi",&CombinedTauTagInfo_GenTau_visproducts_phi,"CombinedTauTagInfo_GenTau_visproducts_phi/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_GenTau_visproducts_type",&CombinedTauTagInfo_GenTau_visproducts_type,"CombinedTauTagInfo_GenTau_visproducts_type/I");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_GenJet05_e",&CombinedTauTagInfo_GenJet05_e,"CombinedTauTagInfo_GenJet05_e/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_GenJet05_et",&CombinedTauTagInfo_GenJet05_et,"CombinedTauTagInfo_GenJet05_et/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_GenJet05_eta",&CombinedTauTagInfo_GenJet05_eta,"CombinedTauTagInfo_GenJet05_eta/F");
  theCombinedTauTagInfoTree->Branch("CombinedTauTagInfo_GenJet05_phi",&CombinedTauTagInfo_GenJet05_phi,"CombinedTauTagInfo_GenJet05_phi/F");
}
void TauTagAnalyzer::beginJob(){}
void TauTagAnalyzer::endJob(){
  thefile->cd();
  theEventTree->Write();
  theCombinedTauTagInfoTree->Write();
  thefile->Write();
  thefile->Close();
}
void TauTagAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  vector<pair<HepLorentzVector,int> > GenTau_pair;
  GenTau_pair.clear();
  GenTausnumber=0;
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
    return;
  }
  */
   
  // select gamma*/Z0 processes
  /*
  if ((*(evt->GetEvent())).signal_process_id()!=1){
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
	HepMC::GenVertex::particles_out_const_iterator i_4thtaudaughter=TheParticle->end_vertex()->particles_out_const_begin();
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

  Handle<JetTagCollection> jetTagHandle;
  iEvent.getByLabel(CombinedTauTagProd_,jetTagHandle);
  const JetTagCollection& myJetTagCollection=*(jetTagHandle.product()); 
  for (JetTagCollection::const_iterator iJetTag=myJetTagCollection.begin();iJetTag!=myJetTagCollection.end();++iJetTag) {
    CombinedTauTagInfoRef theCombinedTauTagInfo=(*iJetTag).tagInfoRef().castTo<CombinedTauTagInfoRef>();
    HepLorentzVector TheCombinedTauTagJet_HepLV((*theCombinedTauTagInfo).jet()->px(),(*theCombinedTauTagInfo).jet()->py(),(*theCombinedTauTagInfo).jet()->pz(),(*theCombinedTauTagInfo).jet()->energy());
    CombinedTauTagInfo_eventgets_recPV=gets_recPV;
    CombinedTauTagInfo_event_scale=event_scale;
    CombinedTauTagInfo_e=TheCombinedTauTagJet_HepLV.e();
    CombinedTauTagInfo_et=TheCombinedTauTagJet_HepLV.et();
    CombinedTauTagInfo_eta=TheCombinedTauTagJet_HepLV.eta();
    CombinedTauTagInfo_phi=TheCombinedTauTagJet_HepLV.phi();
    CombinedTauTagInfo_mode=(*theCombinedTauTagInfo).alternatrecJet_HepLV().e();
    CombinedTauTagInfo_modet=(*theCombinedTauTagInfo).alternatrecJet_HepLV().et();
    CombinedTauTagInfo_modeta=(*theCombinedTauTagInfo).alternatrecJet_HepLV().eta();
    CombinedTauTagInfo_modphi=(*theCombinedTauTagInfo).alternatrecJet_HepLV().phi();
    CombinedTauTagInfo_invmass=(*theCombinedTauTagInfo).alternatrecJet_HepLV().m();
    CombinedTauTagInfo_passed_tracksel=(*theCombinedTauTagInfo).passed_trackerselection();
    CombinedTauTagInfo_is_GoodTauCandidate=(*theCombinedTauTagInfo).is_GoodTauCandidate();
    CombinedTauTagInfo_infact_GoodElectronCandidate=(*theCombinedTauTagInfo).infact_GoodElectronCandidate();
    CombinedTauTagInfo_infact_GoodMuonCandidate=(*theCombinedTauTagInfo).infact_GoodMuonCandidate();
    if (!isnan((*theCombinedTauTagInfo).ECALEt_o_leadTkPt())) CombinedTauTagInfo_ECALEt_o_leadtkPt=(*theCombinedTauTagInfo).ECALEt_o_leadTkPt();
    else CombinedTauTagInfo_ECALEt_o_leadtkPt=-100.;
    if (!isnan((*theCombinedTauTagInfo).HCALEt_o_leadTkPt())) CombinedTauTagInfo_HCALEt_o_leadtkPt=(*theCombinedTauTagInfo).HCALEt_o_leadTkPt();
    else CombinedTauTagInfo_HCALEt_o_leadtkPt=-100.;
    CombinedTauTagInfo_needs_LikelihoodRatio_discrimination=(*theCombinedTauTagInfo).needs_LikelihoodRatio_discrimination();
    CombinedTauTagInfo_signalrectks_number=(int)(*theCombinedTauTagInfo).signalTks().size();
    CombinedTauTagInfo_neutralECALclus_number=(*theCombinedTauTagInfo).neutralECALClus_number();
    if (!isnan((*theCombinedTauTagInfo).neutralECALClus_radius())) CombinedTauTagInfo_neutralECALclus_radius=(*theCombinedTauTagInfo).neutralECALClus_radius();
    else CombinedTauTagInfo_neutralECALclus_radius=-100.;
    if (!isnan((*theCombinedTauTagInfo).neutralE_ratio())) CombinedTauTagInfo_neutrE_ratio=(*theCombinedTauTagInfo).neutralE_ratio();
    else CombinedTauTagInfo_neutrE_ratio=-100.;
    CombinedTauTagInfo_isolneutrE_o_tksEneutrE_ratio=(*theCombinedTauTagInfo).isolneutralE_o_TksEneutralE();
    CombinedTauTagInfo_tksEtJetEt_ratio=(*theCombinedTauTagInfo).TksEt_o_JetEt();
    CombinedTauTagInfo_neutrEtksE_ratio=(*theCombinedTauTagInfo).neutralE_o_TksEneutralE();   
    if (!isnan((*theCombinedTauTagInfo).leadTk_signedipt_significance())) CombinedTauTagInfo_leadtk_signedipt_significance=(*theCombinedTauTagInfo).leadTk_signedipt_significance();
    else CombinedTauTagInfo_leadtk_signedipt_significance=-100.;
    if (!isnan((*theCombinedTauTagInfo).leadTk_signedip3D_significance())) CombinedTauTagInfo_leadtk_signedip3D_significance=(*theCombinedTauTagInfo).leadTk_signedip3D_significance();
    else CombinedTauTagInfo_leadtk_signedip3D_significance=-100.;
    if (!isnan((*theCombinedTauTagInfo).signedflightpath_significance())) CombinedTauTagInfo_signedflightpath_significance=(*theCombinedTauTagInfo).signedflightpath_significance();
    else CombinedTauTagInfo_signedflightpath_significance=-100.;
    if (!isnan((*iJetTag).discriminator())) CombinedTauTagInfo_discriminator=(*iJetTag).discriminator();
    else CombinedTauTagInfo_discriminator=-100.;
    CombinedTauTagInfo_GenTau_visproducts_e=-100.;
    CombinedTauTagInfo_GenTau_visproducts_et=-100.;
    CombinedTauTagInfo_GenTau_visproducts_eta=-100.;
    CombinedTauTagInfo_GenTau_visproducts_phi=-100.;
    CombinedTauTagInfo_GenTau_visproducts_type=-100;
    CombinedTauTagInfo_GenJet05_e=-100.;
    CombinedTauTagInfo_GenJet05_et=-100.;
    CombinedTauTagInfo_GenJet05_eta=-100.;    
    CombinedTauTagInfo_GenJet05_phi=-100.;    
    double min1stdeltaR_GenTau_deltaR=100.;
    pair<HepLorentzVector,int> min1stdeltaR_GenTau_pair;
    double min1stdeltaR_GenJet05_deltaR=100.;
    HepLorentzVector min1stdeltaR_GenJet05_HepLV(0.,0.,0.,0.);
    if (GenTausnumber>0){
      for (vector<pair<HepLorentzVector,int> >::iterator iGenTau_pair=GenTau_pair.begin();iGenTau_pair!=GenTau_pair.end();iGenTau_pair++) {
	if ((*iGenTau_pair).first.deltaR(TheCombinedTauTagJet_HepLV)<min1stdeltaR_GenTau_deltaR) {
	  min1stdeltaR_GenTau_pair=(*iGenTau_pair);
	  min1stdeltaR_GenTau_deltaR=(*iGenTau_pair).first.deltaR(TheCombinedTauTagJet_HepLV);
	}      
      }
    }
    if (GenJet05snumber>0){
      iGenJet05 = 0;
      for (vector<HepLorentzVector>::iterator iGenJet05_HepLV=GenJet05_HepLV.begin();iGenJet05_HepLV!=GenJet05_HepLV.end();iGenJet05_HepLV++) {
	if (iGenJet05>1)break;
	if ((*iGenJet05_HepLV).deltaR(TheCombinedTauTagJet_HepLV)<min1stdeltaR_GenJet05_deltaR) {
	  min1stdeltaR_GenJet05_HepLV=(*iGenJet05_HepLV);
	  min1stdeltaR_GenJet05_deltaR=(*iGenJet05_HepLV).deltaR(TheCombinedTauTagJet_HepLV);
	} 
	++iGenJet05;     
      }
    }
    if (min1stdeltaR_GenTau_deltaR<0.15){
      CombinedTauTagInfo_GenTau_visproducts_e=(min1stdeltaR_GenTau_pair).first.e();
      CombinedTauTagInfo_GenTau_visproducts_et=(min1stdeltaR_GenTau_pair).first.et();
      CombinedTauTagInfo_GenTau_visproducts_eta=(min1stdeltaR_GenTau_pair).first.eta();
      CombinedTauTagInfo_GenTau_visproducts_phi=(min1stdeltaR_GenTau_pair).first.phi();
      CombinedTauTagInfo_GenTau_visproducts_type=min1stdeltaR_GenTau_pair.second;          
    }
    if (min1stdeltaR_GenJet05_deltaR<.25){
      CombinedTauTagInfo_GenJet05_e=min1stdeltaR_GenJet05_HepLV.e();
      CombinedTauTagInfo_GenJet05_et=min1stdeltaR_GenJet05_HepLV.et();
      CombinedTauTagInfo_GenJet05_eta=min1stdeltaR_GenJet05_HepLV.eta(); 
      CombinedTauTagInfo_GenJet05_phi=min1stdeltaR_GenJet05_HepLV.phi(); 
    }    
    theCombinedTauTagInfoTree->Fill();        
  }
  theEventTree->Fill(); 
}
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(TauTagAnalyzer);
