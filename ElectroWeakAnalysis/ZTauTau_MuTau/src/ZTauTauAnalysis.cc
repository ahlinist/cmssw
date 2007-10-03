// -*- C++ -*-
//
// Package:    ZTauTauAnalysis
// Class:      ZTauTauAnalysis
// 
/**\class ZTauTauAnalysis ZTauTauAnalysis.cc UserCode/ZTauTauAnalysis/src/ZTauTauAnalysis.cc
   
Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
// Original Author:  pts/13
//         Created:  Mon Apr  2 18:52:55 CEST 2007
// $Id$
//
//
////////

// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TLorentzVector.h"

#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "Math/GenVector/BitReproducible.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "MagneticField/Engine/interface/MagneticField.h" 
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 
#include "DataFormats/Common/interface/RefVector.h"
#include "TrackingTools/GeomPropagators/interface/HelixExtrapolatorToLine2Order.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "DataFormats/TauReco/interface/Tau.h"
#include "ElectroWeakAnalysis/ZTauTau_MuTau/interface/ZTauTauAnalysis.h"

#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"

#include <TROOT.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <cmath>
#include "TStyle.h"

#include "HepMC/GenEvent.h"
#include "Math/GenVector/VectorUtil.h"
#include "TLorentzVector.h"
#include "DataFormats/Common/interface/Handle.h"
#include <string>
#include <vector>
#include <set>



using namespace edm;
using namespace std;
using namespace reco;
using namespace HepMC; 

// //
// // class declaration
// //

// class ZTauTauAnalysis : public edm::EDAnalyzer {
// public:
//   explicit ZTauTauAnalysis(const edm::ParameterSet&);
//   ~ZTauTauAnalysis();
  
  
// private:
//   virtual void beginJob(const edm::EventSetup&) ;
//   virtual void analyze(const edm::Event&, const edm::EventSetup&);
//   virtual void endJob() ;
  
  
//   // ----------member data ---------------------------
//   int irun, ievt;
  
  
//   edm::InputTag trackTags_; 
  
//   string rootfile_;
  
//   //  string CaloTaus_;
//   string RecoMuons_;
//   string PFTaus_;
//   string GenMet_;
//   string CaloMet_;
  
//   bool skip;
//   int eventCount_;
//   TTree* tree_;
//   TFile* file_;  
  
//   TLorentzVector tauneut, tauh_ap, taul_ap , mu, neutau, neumu, jetMom;
  
//   //    int nEvent;
//   //    vector<float> nEventsUsed;
//   //    vector<float> nEventsRiso;
//   //    int nEventTaggedJets;
  
//   float neumuener, neumuenertr, neutauener, neutauenertr, neutaupx;
//   float neutaupy, neutaupz, neumupx, neumupy, neumupz, neutaupt, neumupt;
//   float jetMomener, jetMomenertr,jetMompt, jetMometa, jetMommass, jetMommasstr, jetMompx, jetMompy, jetMompz, jetMomtheta;
//   float muener, muenertr,mupt, mueta, mumass, mumasstr, mupx, mupy, mupz, mutheta;
//   //float Zpt, 
// float Z_eta;
//   float Z_rap, Z_rapidity;
  
//   //float Zener,
//   //float Zenertr; 
//   //Zpz, Zpx, Zpy, 
//   float tauhener_ap, tauhenertr_ap, tauhpt_ap, tauheta_ap, tauhmass_ap, tauhmasstr_ap;
//   float taulener_ap, taulenertr_ap, taulpt_ap, tauleta_ap, taulmass_ap, taulmasstr_ap;
//   float tauneutener, tauneutenertr, tauneutpx, tauneutpy, tauneutpz, tauneutpt;
  
//   float pxtaul_bp, pytaul_bp,pztaul_bp, pxtauh_bp, pytauh_bp, pztauh_bp, etaul_bp, etauh_bp; 
//   //Zmass_bp, 
//   //float Zmass_ap, 
//   float pxtaul_ap, pytaul_ap,pztaul_ap, pxtauh_ap, pytauh_ap, pztauh_ap, etaul_ap, etauh_ap;
//   //float Zmass_vis, 
//   //float Zmasstr_vis,Zmasslongit_vis,  
//   //float Zmasstr_all;  
//   //Zmass_all, 
  
//   //float ZmassTLV, 
//   //float ZmasstrTLV;
//   float Zeta_fin_px, Zeta_fin_py, Zeta_fin_pz, Zeta_fin_pt;
//   float Zeta_fin_ener, Zeta_fin_enertr, Zeta_fin_eta, Zeta_fin_rapidity;
//   // Zeta_fin_masstrTLV;
//     //Zeta_fin_massTLV, 
//   float ptleadingtrack, ptparticle;
//   int barcodemulept;
//   int barcodemujet;
//   int countchpartjet;
//   float charged;
  
//   float energiatrasvZ;
  
//   //float Zeta_fin_masstr_relat; 
//     //Zeta_masstr_relat, Zmasstr_all_relat;
//   //float Zvisibletransversemass_relat1, 
// float Zvisibletransversemass_relat2  ;
//   //float trmass1, trmass2, trmass3, trmass4, trmass5, 
//   //float trmass6;
//   float trvismass1; 
//     //trvismass4, trvismass5, trvismass6; 
//   //trvismass2, trvismass3, 
//   float tauh_aptheta, taul_aptheta;
//   //float leptpx, leptpy, leptpz, leptener, leptenertr;
//   //float energiaZ, energiatrZ, pxZ, pyZ, pzZ, ptZ;
  
//   //float adrpx, adrpy, adrpz, adrener, adrenertr;
  
//   float CaloRecoP_TTau, CaloRecoP_TLeadTrk, CaloRecoInvMassTau, CaloRecoNumTrks, CaloRecoEmEnergyFraction, CaloRecoMaxHadronEnergy, CaloRecoNumSignalTrks,CaloRecoNumIsolationTrks, CaloRecoSumpTIsolationTracks, CaloRecoSumE_TIsolationGammaCandidates, CalosignalgammacandidatesSize;
  
//   float CaloRecoEnergyTau, CaloRecoEtaTau, CaloRecoTrEnergyTau, CaloRecoTrMassTau;
  
//   float RecoP_TMuon, RecoP_TMuonFrompxpy, RecoEnergyMuon, RecoTrEnergyMuon, RecoInvMassMuon, RecoEtaMuon, RecoTrMassMuon, RecoP_XMuon, RecoP_YMuon, RecoP_ZMuon;
  
//   float CaloRecoVisMassZ, CaloRecoInvMassZ, CaloRecoP_TZ;
//   float CaloRecoP_XTau, CaloRecoP_YTau, CaloRecoP_ZTau;
  
//   float PFRecoP_TTau, PFRecoInvMassTau, PFRecoEmEnergyFraction, PFRecoMaxHadronEnergy,PFRecoNumChargHadrCand, PFRecoNumNeutHadrCand, PFRecoNumGammaCand, PFRecoSignChargHadr, PFRecoNumIsolChargHadr, PFRecoIsolGammaCand, PFRecoSumpTIsolationTracks, PFRecoSumE_TIsolationGammaCandidates, PFsignalgammacandidatesSize,PFRecoP_TLeadTrk,PFRecoSignalGammaCandSize;
  
//   float PFRecoEnergyTau, PFRecoEtaTau, PFRecoTrEnergyTau, PFRecoTrMassTau;
  
//   float PFRecoVisMassZ, PFRecoInvMassZ_A, PFRecoInvMassZ_B,  PFRecoInvMassZ_C, PFRecoP_TZ, PFRecoEnergyZ, PFRecoTrEnergyZ, PFRecoTrVisEnergyZ,PFRecoVisEnergyZ, PFRecoP_XZ, PFRecoP_YZ, PFRecoP_ZZ, PFRecoP_Z, PFRecoTrMassZ_A, PFRecoTrMassZ_B,PFRecoTrMassZ_C, PFRecoTrVisMassZ,PFRecoTrVisMassZ_rel ;
 
//   float  PFRecoVisP_XZ,PFRecoVisP_YZ, PFRecoVisP_ZZ, PFRecoVisP_Z;
//   float PFRecoP_XTau, PFRecoP_YTau, PFRecoP_ZTau;
  
//   float GenMETMissTrEner, GenMETSumMissTrEner, GenMETSignificance, GenMET_E_long;
//   float CaloMETMissTrEner, CaloMETSumMissTrEner, CaloMETSignificance, CaloMET_E_long, CaloMET_x, CaloMET_y, CaloMET_time;
  
//   //float ResPFRecoVisMassZ,ResPFRecoEnergyZ; 
//   //float ResPFRecoP_TZ,  ResPFRecoP_XZ, ResPFRecoP_YZ, ResPFRecoP_ZZ;
  
//   float Zmasstr_all_A; 
//     //Zmasstr_all_B;
//   // Zeta_fin_masstr_relat_A;
//     //Zeta_fin_masstr_relat_B;
//   float GenEnergyZ, GenTrEnergyZ,GenVisTrEnergyZ,GenP_XZ, GenP_YZ, GenP_ZZ, GenP_TZ, GenInvMassZ, GenTrMassZ_rel, GenTrMassZ_A, GenVisMassZ;
//     //GenVisTrMassZ_B, 
// float GenVisTrMassZ_A,GenVisTrMassZ_rel,energiatrasvvisZ, GenVisEnergyZ;   
//   //GenTrMassZ_B, 

 

// edm::ESHandle<MagneticField> theMF;
//   GlobalVector magField; 
  
// };

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

ZTauTauAnalysis::ZTauTauAnalysis(const edm::ParameterSet& iConfig){
  
  //now do what ever initialization is needed
  
  trackTags_=iConfig.getUntrackedParameter<edm::InputTag>("tracks");
  //CaloTaus_= iConfig.getParameter<string>("Taus");
  PFTaus_        = iConfig.getParameter<string>("PFTaus");
  GenMet_=iConfig.getParameter<string>("GenMet");
  CaloMet_=iConfig.getParameter<string>("CaloMet"); 
  RecoMuons_=iConfig.getParameter<string>("Muons");
  
  eventCount_=0;  
 
  
  //  nEvent=0;
  //   nEventTaggedJets=0;
  
  rootfile_   = iConfig.getUntrackedParameter<string>("rootfile","myroot.root");
  file_ = new TFile(rootfile_.c_str(),"RECREATE");
  
  // initialize the tree
  
  tree_ = new TTree("ZTauTauAnalysis","ZTauTauAnalysis Tree");
  
  tree_->Branch("Run",&irun,"irun/i");
  tree_->Branch("Event",&ievt,"ievt/i");
  
  tree_->Branch("neumuener",&neumuener,"neumuener/F");
  tree_->Branch("neutauener",&neutauener,"neutauener/F");
  
  tree_->Branch("neumuenertr",&neumuenertr,"neumuenertr/F");
  tree_->Branch("neutauenertr",&neutauenertr,"neutauenertr/F");
  
  tree_->Branch("jetMomener",&jetMomener,"jetMomener/F");
  tree_->Branch("jetMomenertr",&jetMomenertr,"jetMomenertr/F");
  tree_->Branch("jetMompt",&jetMompt,"jetMompt/F");
  tree_->Branch("jetMometa",&jetMometa,"jetMometa/F");
  tree_->Branch("jetMommass",&jetMommass,"jetMommass/F");
  tree_->Branch("jetMommasstr",&jetMommasstr,"jetMommasstr/F");
  
  tree_->Branch("muener",&muener,"muener/F");
  tree_->Branch("muenertr",&muenertr,"muenertr/F");
  tree_->Branch("mupt",&mupt,"mupt/F");
  tree_->Branch("mueta",&mueta,"mueta/F");
  tree_->Branch("mumass",&mumass,"mumass/F");
  tree_->Branch("mumasstr",&mumasstr,"mumasstr/F");
  
  //tree_->Branch("Zener",&Zener,"Zener/F");
  //tree_->Branch("Zenertr",&Zenertr,"Zenertr/F");

  //tree_->Branch("Zeta_fin_enertr",&Zeta_fin_enertr,"Zeta_fin_enertr/F");
   //tree_->Branch("Zpz",&Zpz,"Zpz/F");
   //tree_->Branch("Zpy",&Zpy,"Zpy/F");
   //tree_->Branch("Zpx",&Zpx,"Zpx/F");
   //tree_->Branch("Zeta_fin_pz",&Zeta_fin_pz,"Zeta_fin_pz/F");  
  //tree_->Branch("Zpt",&Zpt,"Zpt/F");
  tree_->Branch("Z_eta",&Z_eta,"Z_eta/F");
  tree_->Branch("Z_rap",&Z_rap,"Z_rap/F");
  tree_->Branch("Z_rapidity",&Z_rapidity,"Z_rapidity/F"); 
  tree_->Branch("Zeta_fin_rapidity",&Zeta_fin_rapidity,"Zeta_fin_rapidity/F");
  //tree_->Branch("Zeta_fin_massTLV",&Zeta_fin_massTLV,"Zeta_fin_massTLV/F");
  //tree_->Branch("Zeta_fin_masstrTLV",&Zeta_fin_masstrTLV,"Zeta_fin_masstrTLV/F"); 
  //tree_->Branch("Zmass_bp",&Zmass_bp,"Zmass_bp/F");
  //tree_->Branch("Zmass_ap",&Zmass_ap,"Zmass_ap/F");
  //tree_->Branch("Zmass_vis",&Zmass_vis,"Zmass_vis/F");
  //tree_->Branch("Zmasstr_vis",&Zmasstr_vis,"Zmasstr_vis/F");
  //tree_->Branch("Zmass_all",&Zmass_all,"Zmass_all/F");
  //tree_->Branch("Zmasstr_all_A",&Zmasstr_all_A,"Zmasstr_all_A/F");
  //tree_->Branch("Zmasstr_all_B",&Zmasstr_all_B,"Zmasstr_all_B/F");
  //tree_->Branch("ZmasstrTLV",&ZmasstrTLV,"ZmasstrTLV/F");
  //tree_->Branch("ZmassTLV",&ZmassTLV,"ZmassTLV/F");
  //tree_->Branch("Zmasslongit_all",&Zmasslongit_all,"Zmasslongit_all/F");
  //tree_->Branch("Zmasslongit_vis",&Zmasslongit_vis,"Zmasslongit_vis/F");
  //tree_->Branch("energiatrasvZ",&energiatrasvZ,"energiatrasvZ/F");
  //tree_->Branch("Zeta_fin_masstr_relat_A",&Zeta_fin_masstr_relat_A,"Zeta_fin_masstr_relat_A/F");
  //tree_->Branch("Zeta_fin_masstr_relat_B",&Zeta_fin_masstr_relat_B,"Zeta_fin_masstr_relat_B/F");
  //tree_->Branch("Zeta_masstr_relat",&Zeta_masstr_relat,"Zeta_masstr_relat/F");
  //tree_->Branch("Zmasstr_all_relat",&Zmasstr_all_relat,"Zmasstr_all-relat/F"); 
  //tree_->Branch("trmass1",&trmass1,"trmass1/F");
  //tree_->Branch("trmass2",&trmass2,"trmass2/F");
  //tree_->Branch("trmass3",&trmass3,"trmass3/F");
  //tree_->Branch("trmass4",&trmass4,"trmass4/F");
  //tree_->Branch("trmass5",&trmass5,"trmass5/F");
  //tree_->Branch("trmass6",&trmass6,"trmass6/F");
  tree_->Branch("trvismass1",&trvismass1,"trvismass1/F");
  //tree_->Branch("trvismass2",&trvismass2,"trvismass2/F");
  //tree_->Branch("trvismass3",&trvismass3,"trvismass3/F");
  //tree_->Branch("trvismass4",&trvismass4,"trvismass4/F");
  //tree_->Branch("Zvisibletransversemass_relat1",&Zvisibletransversemass_relat1,"Zvisibletransversemass_relat1/F");
  tree_->Branch("Zvisibletransversemass_relat2",&Zvisibletransversemass_relat2,"Zvisibletransversemass_relat2/F");
  //tree_->Branch("energiatrasvZ",&energiatrasvZ,"energiatrasvZ/F");
  //tree_->Branch("Zeta_fin_masstr_relat_A",&Zeta_fin_masstr_relat_A,"Zeta_fin_masstr_relat_A/F");
  //tree_->Branch("Zeta_fin_masstr_relat_B",&Zeta_fin_masstr_relat_B,"Zeta_fin_masstr_relat_B/F");
  //tree_->Branch("Zeta_masstr_relat",&Zeta_masstr_relat,"Zeta_masstr_relat/F");

  tree_->Branch("tauhener_ap",&tauhener_ap,"tauhener_ap/F");
  tree_->Branch("tauhenertr_ap",&tauhenertr_ap,"tauhenertr_ap/F");
  tree_->Branch("tauhpt_ap",&tauhpt_ap,"tauhpt_ap/F");
  tree_->Branch("tauheta_ap",&tauheta_ap,"tauheta_ap/F");
  tree_->Branch("tauhmass_ap",&tauhmass_ap,"tauhmass_ap/F");
  tree_->Branch("tauhmasstr_ap",&tauhmasstr_ap,"tauhmasstr_ap/F");
  
  tree_->Branch("taulener_ap",&taulener_ap,"taulener_ap/F");
  tree_->Branch("taulenertr_ap",&taulenertr_ap,"taulenertr_ap/F");
  tree_->Branch("taulpt_ap",&taulpt_ap,"taulpt_ap/F");
  tree_->Branch("tauleta_ap",&tauleta_ap,"tauleta_ap/F");
  tree_->Branch("taulmass_ap",&taulmass_ap,"taulmass_ap/F");
  tree_->Branch("taulmasstr_ap",&taulmasstr_ap,"taulmasstr_ap/F");
  
  tree_->Branch("tauneutener",&tauneutener,"tauneutener/F");
  tree_->Branch("tauneutenertr",&tauneutenertr,"tauneutenertr/F");
  
  tree_->Branch("ptleadingtrack",&ptleadingtrack,"ptleadingtrack/F");
  tree_->Branch("charged",&charged,"charged/F");
  
  tree_->Branch("GenEnergyZ",&GenEnergyZ,"GenEnergyZ/F");
  tree_->Branch("GenTrEnergyZ",&GenTrEnergyZ,"GenTrEnergyZ/F");
  tree_->Branch("GenP_XZ",&GenP_XZ,"GenP_XZ/F");
  tree_->Branch("GenP_YZ",&GenP_YZ,"GenP_YZ/F");
  tree_->Branch("GenP_ZZ",&GenP_ZZ,"GenP_ZZ/F");
  tree_->Branch("GenP_TZ",&GenP_TZ,"GenP_TZ/F");
  tree_->Branch("GenInvMassZ",&GenInvMassZ,"GenInvMassZ/F");
  tree_->Branch("GenTrMassZ_rel",&GenTrMassZ_rel,"GenTrMassZ_rel/F");
  tree_->Branch("GenTrMassZ_A",&GenTrMassZ_A,"GenTrMassZ_A/F");
  //tree_->Branch("GenTrMassZ_B",&GenTrMassZ_B,"GenTrMassZ_B/F");
  tree_->Branch("GenVisMassZ",&GenVisMassZ,"GenVisMassZ/F");
  tree_->Branch("GenVisTrMassZ_A",&GenVisTrMassZ_A,"GenVisTrMassZ_A/F");
  //tree_->Branch("GenVisTrMassZ_B",&GenVisTrMassZ_B,"GenVisTrMassZ_B/F");
  tree_->Branch("GenVisTrMassZ_rel",&GenVisTrMassZ_rel,"GenVisTrMassZ_rel/F");
  //tree_->Branch("energiatrasvZ",&energiatrasvZ,"energiatrasvZ/F");
  //tree_->Branch("energiatrasvvisZ",&energiatrasvvisZ,"energiatrasvvisZ/F");

  //  tree_->Branch("CaloRecoP_TTau",&CaloRecoP_TTau,"CaloRecoP_TTau/F");
  //   tree_->Branch("CaloRecoP_TLeadTrk",&CaloRecoP_TLeadTrk,"CaloRecoP_TLeadTrk/F");
  //   tree_->Branch("CaloRecoInvMassTau",&CaloRecoInvMassTau,"CaloRecoInvMassTau/F");
  //   tree_->Branch("CaloRecoNumTrks",&CaloRecoNumTrks,"CaloRecoNumTrks/F");
  //   tree_->Branch("CaloRecoEmEnergyFraction",&CaloRecoEmEnergyFraction,"CaloRecoEmEnergyFraction/F");
  //   tree_->Branch("CaloRecoMaxHadronEnergy",&CaloRecoMaxHadronEnergy,"CaloRecoMaxHadronEnergy/F");
  //   tree_->Branch("CaloRecoNumSignalTrks",&CaloRecoNumSignalTrks,"CaloRecoNumSignalTrks/F");
  //   tree_->Branch("CaloRecoNumIsolationTrks",&CaloRecoNumIsolationTrks,"CaloRecoNumIsolationTrks/F");
  //   tree_->Branch("CaloRecoSumpTIsolationTracks",&CaloRecoSumpTIsolationTracks,"CaloRecoSumpTIsolationTracks/F");
  //   tree_->Branch("CaloRecoSumE_TIsolationGammaCandidates",&CaloRecoSumE_TIsolationGammaCandidates,"CaloRecoSumE_TIsolationGammaCandidates/F");
  //   tree_->Branch("CalosignalgammacandidatesSize",&CalosignalgammacandidatesSize,"CalosignalgammacandidatesSize/F");
  
  //   tree_->Branch("CaloRecoEnergyTau",&CaloRecoEnergyTau,"CaloRecoEnergyTau/F");
  //   tree_->Branch("CaloRecoEtaTau",&CaloRecoEtaTau,"CaloRecoEtaTau/F");
  //   tree_->Branch("CaloRecoTrEnergyTau",&CaloRecoTrEnergyTau,"CaloRecoTrEnergyTau/F");
  //   tree_->Branch("CaloRecoTrMassTau",&CaloRecoTrMassTau,"CaloRecoTrMassTau/F");
  
  
  tree_->Branch("RecoP_TMuon",&RecoP_TMuon,"RecoP_TMuon/F");
  //tree_->Branch("RecoP_TMuonFrompxpy",&RecoP_TMuonFrompxpy,"RecoP_TMuonFrompxpy/F");
  // tree_->Branch("",&,"/F");
  
  tree_->Branch("RecoEnergyMuon",&RecoEnergyMuon,"RecoEnergyMuon/F");
  tree_->Branch("RecoTrEnergyMuon",&RecoTrEnergyMuon,"RecoTrEnergyMuon/F");
  tree_->Branch("RecoInvMassMuon",&RecoInvMassMuon,"RecoInvMassMuon/F");
  tree_->Branch("RecoEtaMuon",&RecoEtaMuon,"RecoEtaMuon/F");
  tree_->Branch("RecoTrMassMuon",&RecoTrMassMuon,"RecoTrMassMuon/F");
  
  tree_->Branch("CaloRecoVisMassZ",&CaloRecoVisMassZ,"CaloRecoVisMassZ/F");
  tree_->Branch("CaloRecoInvMassZ",&CaloRecoInvMassZ,"CaloRecoInvMassZ/F");
  tree_->Branch("CaloRecoP_TZ",&CaloRecoP_TZ,"CaloRecoP_TZ/F");
  
  tree_->Branch("PFRecoEnergyZ",&PFRecoEnergyZ,"PFRecoEnergyZ/F");
  tree_->Branch("PFRecoTrEnergyZ",&PFRecoTrEnergyZ,"PFRecoTrEnergyZ/F");
  tree_->Branch("PFRecoTrVisEnergyZ",&PFRecoTrVisEnergyZ,"PFRecoTrVisEnergyZ/F");
  tree_->Branch("PFRecoP_XZ",&PFRecoP_XZ,"PFRecoP_XZ/F");
  tree_->Branch("PFRecoP_YZ",&PFRecoP_YZ,"PFRecoP_YZ/F");
  tree_->Branch("PFRecoP_ZZ",&PFRecoP_ZZ,"PFRecoP_ZZ/F");
  tree_->Branch("PFRecoP_TZ",&PFRecoP_TZ,"PFRecoP_TZ/F");
//    tree_->Branch("ResPFRecoVisMassZ",&ResPFRecoVisMassZ,"ResPFRecoVisMassZ/F");
//     tree_->Branch("ResPFRecoP_TZ",&ResPFRecoP_TZ,"ResPFRecoP_TZ/F");
//      tree_->Branch("ResPFRecoEnergyZ",&ResPFRecoEnergyZ,"ResPFRecoEnergyZ/F");
//      tree_->Branch("ResPFRecoP_XZ",&ResPFRecoP_XZ,"ResPFRecoP_XZ/F");
//      tree_->Branch("ResPFRecoP_YZ",&ResPFRecoP_YZ,"ResPFRecoP_YZ/F");
//      tree_->Branch("ResPFRecoP_ZZ",&ResPFRecoP_ZZ,"ResPFRecoP_ZZ/F");
  
  tree_->Branch("PFRecoP_TTau",&PFRecoP_TTau,"PFRecoP_TTau/F");
  tree_->Branch("PFRecoInvMassTau",&PFRecoInvMassTau,"PFRecoInvMassTau/F");
  tree_->Branch("PFRecoEmEnergyFraction",&PFRecoEmEnergyFraction,"PFRecoEmEnergyFraction/F");
  tree_->Branch("PFRecoMaxHadronEnergy",&PFRecoMaxHadronEnergy,"PFRecoMaxHadronEnergy/F");
  tree_->Branch("PFRecoNumChargHadrCand",&PFRecoNumChargHadrCand,"PFRecoNumChargHadrCand/F");
  tree_->Branch("PFRecoNumNeutHadrCand",&PFRecoNumNeutHadrCand,"PFRecoNumNeutHadrCand/F");
  tree_->Branch("PFRecoNumGammaCand",&PFRecoNumGammaCand,"PFRecoNumGammaCand/F");
  tree_->Branch("PFRecoSignChargHadr",&PFRecoSignChargHadr,"PFRecoSignChargHadr/F");
  tree_->Branch("PFRecoNumIsolChargHadr",&PFRecoNumIsolChargHadr,"PFRecoNumIsolChargHadr/F");
  tree_->Branch("PFRecoIsolGammaCand",&PFRecoIsolGammaCand,"PFRecoIsolGammaCand/F");
  tree_->Branch("PFRecoSumpTIsolationTracks",&PFRecoSumpTIsolationTracks,"PFRecoSumpTIsolationTracks/F");
  tree_->Branch("PFRecoSumE_TIsolationGammaCandidates",&PFRecoSumE_TIsolationGammaCandidates,"PFRecoSumE_TIsolationGammaCandidates/F");
  
  tree_->Branch("PFRecoP_TLeadTrk",&PFRecoP_TLeadTrk,"PFRecoP_TLeadTrk/F"); 
  tree_->Branch("PFRecoSignalGammaCandSize",&PFRecoSignalGammaCandSize,"PFRecoSignalGammaCandSize/F");
  
  tree_->Branch("PFRecoEnergyTau",&PFRecoEnergyTau,"PFRecoEnergyTau/F");
  tree_->Branch("PFRecoEtaTau",&PFRecoEtaTau,"PFRecoEtaTau/F");
  tree_->Branch("PFRecoTrEnergyTau",&PFRecoTrEnergyTau,"PFRecoTrEnergyTau/F");
  tree_->Branch("PFRecoTrMassTau",&PFRecoTrMassTau,"PFRecoTrMassTau/F");
  
  tree_->Branch("PFRecoVisMassZ",&PFRecoVisMassZ,"PFRecoVisMassZ/F");
  tree_->Branch("PFRecoInvMassZ_A",&PFRecoInvMassZ_A,"PFRecoInvMassZ_A/F");
  tree_->Branch("PFRecoInvMassZ_B",&PFRecoInvMassZ_B,"PFRecoInvMassZ_B/F");
  tree_->Branch("PFRecoInvMassZ_C",&PFRecoInvMassZ_C,"PFRecoInvMassZ_C/F");
  tree_->Branch("PFRecoP_TZ",&PFRecoP_TZ,"PFRecoP_TZ/F");
  tree_->Branch("PFRecoTrMassZ_A",&PFRecoTrMassZ_A,"PFRecoTrMassZ_A/F");
  tree_->Branch("PFRecoTrMassZ_B",&PFRecoTrMassZ_B,"PFRecoTrMassZ_B/F");
  tree_->Branch("PFRecoTrMassZ_C",&PFRecoTrMassZ_C,"PFRecoTrMassZ_C/F");
  tree_->Branch("PFRecoTrVisMassZ",&PFRecoTrVisMassZ,"PFRecoTrVisMassZ/F");
  tree_->Branch("PFRecoTrVisMassZ_rel",&PFRecoTrVisMassZ_rel,"PFRecoTrVisMassZ_rel/F");

  tree_->Branch("GenMETMissTrEner",&GenMETMissTrEner,"GenMETMissTrEner/F");
  tree_->Branch("GenMETSumMissTrEner",&GenMETSumMissTrEner,"GenMETSumMissTrEner/F");
  tree_->Branch("GenMETSignificance",&GenMETSignificance,"GenMETSignificance/F");
  tree_->Branch("GenMET_E_long",&GenMET_E_long,"GenMET_E_long/F");
  
  tree_->Branch("CaloMETMissTrEner",&CaloMETMissTrEner,"CaloMETMissTrEner/F");
  tree_->Branch("CaloMETSumMissTrEner",&CaloMETSumMissTrEner,"CaloMETSumMissTrEner/F");
  tree_->Branch("CaloMETSignificance",&CaloMETSignificance,"CaloMETSignificance/F");
  tree_->Branch("CaloMET_E_long",&CaloMET_E_long,"CaloMET_E_long/F"); 
  tree_->Branch("CaloMET_time",&CaloMET_time,"CaloMET_time/F");
tree_->Branch("CaloMET_x",&CaloMET_x,"CaloMET_x/F");
tree_->Branch("CaloMET_y",&CaloMET_y,"CaloMET_y/F");
}


ZTauTauAnalysis::~ZTauTauAnalysis()
{
  // std::cout<<"ZTauTauAnalysis:: endJob Processed  "<< eventCount_<<" events"<<std::endl;
  
  //store the tree in the output file
  file_->Write();
  
  //closing the file deletes the tree
  file_->Close();
  tree_=0;  
}

//
// member functions
//

// ------------ method called to for each event  ------------
void
ZTauTauAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
  
{  
  eventCount_++;
  
  skip=false; 
  
  bool foundmufromtau=false;
  bool foundneumufromtau=false;
  bool foundneutaufromtau=false;
  
  bool foundZ = false;
  
  bool foundjetfromtaup=false;
  
  bool foundlepttaup=false;
  
  bool foundtaup=false;
  bool foundtaun=false;
  
  bool foundjetfromtaun=false;
  
  bool foundlepttaun=false;
  
  bool foundmufromtaup=false;
  bool foundneumufromtaup=false;
  bool foundneutaufromtaup=false;
  bool  notfoundelectronp=true;
  
  bool foundmufromtaun=false;
  bool foundneumufromtaun=false;
  bool foundneutaufromtaun=false;
  bool notfoundelectronn=true;
  
bool FlagRecoTau=false;
  bool FlagRecoMuon=false;
  bool FlagRecoMET=false;
  bool FlagRecoZ_A=false;
 bool FlagRecoZ_B=false;
 
  edm::Handle<HepMCProduct> evt;
  //iEvent.getByLabel("genParticles", evt);
  //iEvent.getByLabel("VtxSmeared", evt);
  iEvent.getByLabel("source", evt);
  
  HepMC::GenEvent *generated_event = new HepMC::GenEvent(*(evt->GetEvent()));
  HepMC::GenEvent::particle_iterator p;
  for (p = generated_event->particles_begin(); p!=generated_event->particles_end(); p++)
    {
      
      if ( ((*p) ->pdg_id()== 23) && ((*p)->status() == 3) )
  	{
  	  foundZ = true;
	  
  	  if ((*p)->end_vertex()->particles_out_size()>2) 
	    
	    {
	      
	      muener=-100.,muenertr=-100.,mupt=-100.,mueta=-100.,mumass=-100.,mumasstr=-100.,mupx=-100.,mupy=-100.,mupz=-100., mutheta=-100.;
	      
	      neumuener=-100.,neumuenertr=-100.,neumupx=-100.,neumupy=-100.,neumupz=-100.,neumupt=-100.,neutaupt=-100.;
	      neutauener=-100.,neutauenertr=-100.,neutaupx=-100.,neutaupy=-100.,neutaupz=-100.;
	      
	      taulener_ap=-100.,taulenertr_ap=-100.,taulpt_ap=-100.,tauleta_ap=-100.,taulmass_ap=-100.,taulmasstr_ap=-100.;
	      
	      tauhener_ap=-100.,tauhenertr_ap=-100.,tauhpt_ap=-100.,tauheta_ap=-100.,tauhmass_ap=-100.,tauhmasstr_ap=-100.;
	      
	      jetMomener=-100.,jetMomenertr=-100.,jetMompt=-100.,jetMometa=-100.,jetMommass=-100.,jetMommasstr=-100., jetMomtheta=-100.;
	      jetMompx=-100.,jetMompy=-100.,jetMompz=-100.;
	      
	      //Zpt=-100.,
	      Z_eta=-100.,Z_rap=-100.,Z_rapidity=-100.;
	      //Zener=-100.,
	      //Zenertr=-100.;
	      //Zpx=-100.,Zpy=-100.,Zpz=-5000.;
	      
	      tauneutener=-100.,tauneutenertr=-100.,tauneutpx=-100.,tauneutpy=-100.,tauneutpz=-100.,tauneutpt=-100.;
	      
	      //Zmass_bp=-100.,Zmass_ap=-100.,
	      //Zmass_vis=-100.,
	      //Zmasstr_vis=-100.;
	      //Zmass_all=-100.,
	      //Zmasstr_all=-100.,
		//ZmassTLV=-100.,
		//ZmasstrTLV=-100;
		//Zmasslongit_vis=-100.,Zmasslongit_all=-100.;
	      
	      etaul_bp=-100.,pxtaul_bp=-100.,pytaul_bp=-100.,pztaul_bp=-100.;
	      etauh_bp=-100.,pxtauh_bp=-100.,pytauh_bp=-100.,pztauh_bp=-100.;
	      etaul_ap=-100.,pxtaul_ap=-100.,pytaul_ap=-100.,pztaul_ap=-100.;
	      etauh_ap=-100.,pxtauh_ap=-100.,pytauh_ap=-100.,pztauh_ap=-100.;
	      
	      //Zeta_fin_px=-100.,Zeta_fin_py=-100.,Zeta_fin_pz=-5000.,Zeta_fin_pt=-100.;
	      //Zeta_fin_ener=-100.,Zeta_fin_enertr=-100.,
	      //Zeta_fin_massTLV=-100.,
              //Zeta_fin_masstrTLV=-100.,
		Zeta_fin_eta=-100.,  Zeta_fin_rapidity=-100.;
	      //energiatrasvZ=-100.;      
	      //Zeta_fin_masstr_relat=-100.;
	      //Zeta_fin_masstr_relat_A=-100.;
	      // Zeta_masstr_relat=-100., Zmasstr_all_relat=-100.,
	      //Zmasstr_all_A=-100.;
	      //Zmasstr_all_B=-100.;
	      //Zvisibletransversemass_relat1=-100., 
	      Zvisibletransversemass_relat2=-100.;
	      //trmass1=-100., trmass2=-100., trmass3=-100., trmass4=-100., trmass5=-100., 
	      //trmass6=-100.  ;
	      trvismass1=-100.;
	      //trvismass2=-100.,trvismass3=-100.,trvismass4=-100. ;
	      tauh_aptheta=-100., taul_aptheta=-100;
	      //leptpx=-100.,leptpy=-100., leptpz=-100., leptener=-100., leptenertr=-100.;
	      //adrpx=-100., adrpy=-100., adrpz=-100.,adrener=-100., adrenertr=-100.;
	      //energiaZ=-100., energiatrZ=-100.,pxZ=-100., pyZ=-100., pzZ=-100., ptZ=-100.;
	      GenEnergyZ=-100., GenTrEnergyZ=-100., GenVisTrEnergyZ=-100.,GenP_XZ=-100., GenP_YZ=-100., GenP_ZZ=-500., GenP_TZ=-100., GenInvMassZ=-100., GenTrMassZ_rel=-100., GenTrMassZ_A=-100.; 
		//GenTrMassZ_B=-100., 
		GenVisMassZ=-100., GenVisTrMassZ_A=-100.;
	      //GenVisTrMassZ_B=-100., 
	      GenVisTrMassZ_rel=-100., GenVisEnergyZ=-100.;
	      // energiatrasvvisZ=-100.;
	      
for( HepMC::GenVertex::particle_iterator aDaughter=(*p)->end_vertex()->particles_begin(HepMC::descendants); aDaughter !=(*p)->end_vertex()->particles_end(HepMC::descendants);aDaughter++)
		
		{ 
		  
		  ///////////////////////////////////////////////////////////////////
		  
		  if( (*aDaughter)->pdg_id()==15 ) 
		    {     
		      
		      //vector<HepMC::GenParticle*> bychildren = (*aDaughter)->listChildren();
		      // vector<HepMC::GenParticle*>::const_iterator byaDaughter;  
		      //for (byaDaughter = bychildren.begin();byaDaughter != bychildren.end();byaDaughter++)
		      
		      for( HepMC::GenVertex::particle_iterator byaDaughter=(*aDaughter)->end_vertex()->particles_begin(HepMC::descendants); byaDaughter !=(*aDaughter)->end_vertex()->particles_end(HepMC::descendants);byaDaughter++)		  
			{
			  
			  if ((*byaDaughter)->status()==2 && (*byaDaughter)->pdg_id()==15) 
			    {  
			      foundtaup=true;
			      
			      //  vector<HepMC::GenParticle*> tauchildren = (*byaDaughter)->listChildren();
			      // vector<HepMC::GenParticle*>::const_iterator tauDaughter;			    
			      // for (tauDaughter = tauchildren.begin();tauDaughter != tauchildren.end();tauDaughter++)
			      
			      for( HepMC::GenVertex::particle_iterator tauDaughter=(*byaDaughter)->end_vertex()->particles_begin(HepMC::descendants); tauDaughter !=(*byaDaughter)->end_vertex()->particles_end(HepMC::descendants);tauDaughter++)
				
				{
				  if ( (*tauDaughter)->pdg_id()==13) 
				    {     
				      foundmufromtaup=true;
				      
				      mu.SetPxPyPzE((*tauDaughter)->momentum().px(),(*tauDaughter)->momentum().py(),
						    (*tauDaughter)->momentum().pz(),(*tauDaughter)->momentum().e());
				      
				      muener=mu.E();
				      mupx=mu.Px();
				      mupy=mu.Py();
				      mupz=mu.Pz();
				      muenertr=mu.Et();
				      mupt=mu.Pt();
				      mueta=mu.Eta();
				      mutheta=mu.Theta();
				      mumass=mu.M();
				      mumasstr=sqrt((muenertr)*(muenertr)-(mupt)*(mupt));
				      barcodemulept=abs((*tauDaughter)->barcode());
				    }
				  else if ( (*tauDaughter)->pdg_id()==-14) 
				    {
				      
				      neumu.SetPxPyPzE((*tauDaughter)->momentum().px(),
						       (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
						       (*tauDaughter)->momentum().e());
				      
				      neumuener=neumu.E();
				      neumuenertr=neumu.Et();
				      neumupx=neumu.Px();
				      neumupy=neumu.Py();
				      neumupz=neumu.Pz();
				      neumupt=neumu.Pt();
				      
				      foundneumufromtaup=true;
				    }
				  
				  else if ( (*tauDaughter)->pdg_id()==16)
				    { 
				      foundneutaufromtaup=true;
				    }      
				  else if((*tauDaughter)->pdg_id()==11)
				    { 
				      notfoundelectronp=false;
				    } 
				  
				  if (foundneutaufromtaup && foundneumufromtaup && foundmufromtaup)
				    {  
				      foundlepttaup=true;
	     
				      if ( (*tauDaughter)->pdg_id()==16){ 
					
					neutau.SetPxPyPzE((*tauDaughter)->momentum().px(),
							  (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
							  (*tauDaughter)->momentum().e());
					
					neutauener=neutau.E();
					neutauenertr=neutau.Et();
					neutaupx=neutau.Px();
					neutaupy=neutau.Py();
					neutaupz=neutau.Pz();
					neutaupt=neutau.Pt();
				      }
				      
				      taul_ap.SetPxPyPzE((*byaDaughter)->momentum().px(),
							 (*byaDaughter)->momentum().py(),(*byaDaughter)->momentum().pz(),
							 (*byaDaughter)->momentum().e());
				      
				      taulener_ap=taul_ap.E();
				      taulenertr_ap=taul_ap.Et();
				      taulpt_ap=taul_ap.Pt();
				      tauleta_ap=taul_ap.Eta();
				      taulmass_ap=taul_ap.M();
				      taulmasstr_ap=sqrt((taulenertr_ap)*(taulenertr_ap)-(taulpt_ap)*(taulpt_ap));
				      
				      pxtaul_bp=(*aDaughter)->momentum().px();
				      pytaul_bp=(*aDaughter)->momentum().py();
				      pztaul_bp=(*aDaughter)->momentum().pz();
				      etaul_bp=(*aDaughter)->momentum().e();
				      
				      pxtaul_ap=(*byaDaughter)->momentum().px();
				      pytaul_ap=(*byaDaughter)->momentum().py();
				      pztaul_ap=(*byaDaughter)->momentum().pz();
				      etaul_ap=(*byaDaughter)->momentum().e();  		      
				      
				    }		    
				  
				  else if ((foundneutaufromtaup==1)&&(foundmufromtaup==0)&&(notfoundelectronp==1)) 
				    {
				      foundjetfromtaup=true;
				      
				      if ( (*tauDaughter)->pdg_id()==16)
					{  
					  tauneut.SetPxPyPzE((*tauDaughter)->momentum().px(),
							     (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
							     (*tauDaughter)->momentum().e());
					  
					  tauneutener=tauneut.E();
					  tauneutenertr=tauneut.Et();
					  tauneutpx=tauneut.Px();
					  tauneutpy=tauneut.Py();
					  tauneutpz=tauneut.Pz();
					  tauneutpt=tauneut.Pt();
					}
				      
				      pxtauh_bp=(*aDaughter)->momentum().px();
				      pytauh_bp=(*aDaughter)->momentum().py();
				      pztauh_bp=(*aDaughter)->momentum().pz();
				      etauh_bp=(*aDaughter)->momentum().e();
				      
				      pxtauh_ap=(*byaDaughter)->momentum().px();
				      pytauh_ap=(*byaDaughter)->momentum().py();
				      pztauh_ap=(*byaDaughter)->momentum().pz();
				      etauh_ap=(*byaDaughter)->momentum().e();
				      
				      tauh_ap.SetPxPyPzE((*byaDaughter)->momentum().px(),
							 (*byaDaughter)->momentum().py(),(*byaDaughter)->momentum().pz(),
							 (*byaDaughter)->momentum().e());				      			 
				      tauhener_ap=tauh_ap.E();
				      tauhenertr_ap=tauh_ap.Et();
				      tauhpt_ap=tauh_ap.Pt();
				      tauheta_ap=tauh_ap.Eta();
				      tauhmass_ap=tauh_ap.M();
				      tauhmasstr_ap=sqrt((tauhenertr_ap)*(tauhenertr_ap)-(tauhpt_ap)*(tauhpt_ap));
				      
				      jetMom=tauh_ap-tauneut; 
				      jetMomener=jetMom.E();
				      jetMompx=jetMom.Px();
				      jetMompy=jetMom.Py();
				      jetMompz=jetMom.Pz();
				      jetMomenertr=jetMom.Et();
				      jetMompt=jetMom.Pt();
				      jetMometa=jetMom.Eta();
				      jetMomtheta=jetMom.Theta();
				      jetMommass=sqrt((jetMomener)*(jetMomener)-(jetMompx)*(jetMompx)-(jetMompy)*(jetMompy)-(jetMompz)*(jetMompz));
				      jetMommasstr=sqrt((jetMomenertr)*(jetMomenertr)-(jetMompt)*(jetMompt));
				      
				    }  
				}
			    }//if byaD
			}//for byaD
		    }//if aD
		  
		  //////////////////////////////////////////////////////////////////////////////////////////////////////
		  
		  if( (*aDaughter)->pdg_id()==-15 ) 
		    {     
		      
		      //vector<HepMC::GenParticle*> bychildren = (*aDaughter)->listChildren();
		      //vector<HepMC::GenParticle*>::const_iterator byaDaughter;
		      //for (byaDaughter = bychildren.begin();byaDaughter != bychildren.end();byaDaughter++)
		      
		      for( HepMC::GenVertex::particle_iterator byaDaughter=(*aDaughter)->end_vertex()->particles_begin(HepMC::descendants); byaDaughter !=(*aDaughter)->end_vertex()->particles_end(HepMC::descendants);byaDaughter++)
			{
	  
			  if ((*byaDaughter)->status()==2 && (*byaDaughter)->pdg_id()==-15) 
			    {  
			      foundtaun=true;
			      
			      //vector<HepMC::GenParticle*> tauchildren = (*byaDaughter)->listChildren();
			      //vector<HepMC::GenParticle*>::const_iterator tauDaughter;    
			      //for (tauDaughter = tauchildren.begin();tauDaughter != tauchildren.end();tauDaughter++)
			      
			      for( HepMC::GenVertex::particle_iterator tauDaughter=(*byaDaughter)->end_vertex()->particles_begin(HepMC::descendants); tauDaughter !=(*byaDaughter)->end_vertex()->particles_end(HepMC::descendants);tauDaughter++)		
				{
				  if ( (*tauDaughter)->pdg_id()==-13) 
				    {     
				      foundmufromtaun=true;
				      
				      mu.SetPxPyPzE((*tauDaughter)->momentum().px(),(*tauDaughter)->momentum().py(),
						    (*tauDaughter)->momentum().pz(),(*tauDaughter)->momentum().e());
				      
				      muener=mu.E();
				      mupx=mu.Px();
				      mupy=mu.Py();
				      mupz=mu.Pz();
				      muenertr=mu.Et();
				      mupt=mu.Pt();
				      mueta=mu.Eta();
				      mumass=mu.M();
				      mutheta=mu.Theta();
				      mumasstr=sqrt((muenertr)*(muenertr)-(mupt)*(mupt));
				      barcodemulept=abs((*tauDaughter)->barcode());
				    }
				  else if ( (*tauDaughter)->pdg_id()==14) 
				    {
				      
				      neumu.SetPxPyPzE((*tauDaughter)->momentum().px(),
						       (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
						       (*tauDaughter)->momentum().e());
				      
				      neumuener=neumu.E();
				      neumuenertr=neumu.Et();
				      neumupx=neumu.Px();
				      neumupy=neumu.Py();
				      neumupz=neumu.Pz();
				      neumupt=neumu.Pt();
				     
				      foundneumufromtaun=true;
				    }
				  
				  else if ( (*tauDaughter)->pdg_id()==-16)
				    { 
				      foundneutaufromtaun=true;
				    }      
				  else if((*tauDaughter)->pdg_id()==-11)
				    { 
				      notfoundelectronn=false;
				    } 
				  
				  if (foundneutaufromtau && foundneumufromtau && foundmufromtau)
				    {
     
				      foundlepttaun=true;
	 
				      if ( (*tauDaughter)->pdg_id()==-16){ 
					
					neutau.SetPxPyPzE((*tauDaughter)->momentum().px(),
							  (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
							  (*tauDaughter)->momentum().e());
					
					neutauener=neutau.E();
					neutauenertr=neutau.Et();
					neutaupx=neutau.Px();
					neutaupy=neutau.Py();
					neutaupz=neutau.Pz();
					neutaupt=neutau.Pt();
				      }
				      
				      taul_ap.SetPxPyPzE((*byaDaughter)->momentum().px(),
							 (*byaDaughter)->momentum().py(),(*byaDaughter)->momentum().pz(),
							 (*byaDaughter)->momentum().e());
				      
				      taulener_ap=taul_ap.E();
				      taulenertr_ap=taul_ap.Et();
				      taulpt_ap=taul_ap.Pt();
				      tauleta_ap=taul_ap.Eta();
				      taulmass_ap=taul_ap.M();
				      taulmasstr_ap=sqrt((taulenertr_ap)*(taulenertr_ap)-(taulpt_ap)*(taulpt_ap));
				      
				      pxtaul_bp=(*aDaughter)->momentum().px();
				      pytaul_bp=(*aDaughter)->momentum().py();
				      pztaul_bp=(*aDaughter)->momentum().pz();
				      etaul_bp=(*aDaughter)->momentum().e();
				      
				      pxtaul_ap=(*byaDaughter)->momentum().px();
				      pytaul_ap=(*byaDaughter)->momentum().py();
				      pztaul_ap=(*byaDaughter)->momentum().pz();
				      etaul_ap=(*byaDaughter)->momentum().e();  
				    }		    
				  
				  else if ((foundneutaufromtaun==1)&&(foundmufromtaun==0)&&(notfoundelectronn==1)) 
				    {
				   
				      foundjetfromtaun=true;
				      
				      if ( (*tauDaughter)->pdg_id()==-16)
					{  
					  tauneut.SetPxPyPzE((*tauDaughter)->momentum().px(),
							     (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
							     (*tauDaughter)->momentum().e());
					  
					  tauneutener=tauneut.E();
					  tauneutenertr=tauneut.Et();
					  tauneutpx=tauneut.Px();
					  tauneutpy=tauneut.Py();
					  tauneutpz=tauneut.Pz();
					  tauneutpt=tauneut.Pt();
					}
				      
				      pxtauh_bp=(*aDaughter)->momentum().px();
				      pytauh_bp=(*aDaughter)->momentum().py();
				      pztauh_bp=(*aDaughter)->momentum().pz();
				      etauh_bp=(*aDaughter)->momentum().e();
				      
				      pxtauh_ap=(*byaDaughter)->momentum().px();
				      pytauh_ap=(*byaDaughter)->momentum().py();
				      pztauh_ap=(*byaDaughter)->momentum().pz();
				      etauh_ap=(*byaDaughter)->momentum().e();
				      
				      tauh_ap.SetPxPyPzE((*byaDaughter)->momentum().px(),
							 (*byaDaughter)->momentum().py(),(*byaDaughter)->momentum().pz(),
							 (*byaDaughter)->momentum().e());				      			 
				      tauhener_ap=tauh_ap.E();
				      tauhenertr_ap=tauh_ap.Et();
				      tauhpt_ap=tauh_ap.Pt();
				      tauheta_ap=tauh_ap.Eta();
				      tauhmass_ap=tauh_ap.M();
				      tauhmasstr_ap=sqrt((tauhenertr_ap)*(tauhenertr_ap)-(tauhpt_ap)*(tauhpt_ap));
				      
				      jetMom=tauh_ap-tauneut; 
				      jetMomener=jetMom.E();
				      jetMompx=jetMom.Px();
				      jetMompy=jetMom.Py();
				      jetMompz=jetMom.Pz();
				      jetMomenertr=jetMom.Et();
				      jetMompt=jetMom.Pt();
				      jetMometa=jetMom.Eta();
				      jetMomtheta=jetMom.Theta();
				      jetMommass=sqrt((jetMomener)*(jetMomener)-(jetMompx)*(jetMompx)-(jetMompy)*(jetMompy)-(jetMompz)*(jetMompz));
				      jetMommasstr=sqrt((jetMomenertr)*(jetMomenertr)-(jetMompt)*(jetMompt));
				    }  
				}
			    }//if byaD
			  
			}//for byaD
		    }//if aD
		  ///////////////////////////////////////////////////////////////////////////////////
 
		} //for aD
	      
	      if (foundZ==1&&foundtaup==1&&foundtaun==1)
		{
		  
		  if ((foundlepttaup&&foundjetfromtaun)||(foundlepttaun&&foundjetfromtaup)) 
		    {
		      std::cout<<"°°°°°°°°°°°°°°°°°°°°°°°°°°GOOD EVENT°°°°°°°°°°°°°°°°°°°°°"<<endl;
		      
		      GenEnergyZ=muener+neumuener+ neutauener+jetMomener+tauneutener;
		      //GenTrEnergyZ=muenertr+neumuenertr+neutauenertr+jetMomenertr+tauneutenertr;
		      //energiatrasvZ=GenEnergyZ*sin(Zeta_fin.Theta()); 
		      
		      GenP_ZZ=mupz+neumupz+neutaupz+jetMompz+tauneutpz;
		      GenP_XZ=mupx+neumupx+neutaupx+jetMompx+tauneutpx;
		      GenP_YZ=mupy+neumupy+neutaupy+jetMompy+tauneutpy;
		      
		      GenTrEnergyZ=sqrt((GenEnergyZ*GenEnergyZ)-(GenP_ZZ*GenP_ZZ));
		      GenVisEnergyZ=muener+jetMomener;
		      GenVisTrEnergyZ=sqrt((GenVisEnergyZ*GenVisEnergyZ)-((mupz+jetMompz)*(mupz+jetMompz)));		      
		     
		      GenP_TZ=sqrt((GenP_XZ*GenP_XZ)+(GenP_YZ*GenP_YZ));
		      
		      GenInvMassZ=sqrt((GenEnergyZ*GenEnergyZ)-(GenP_XZ*GenP_XZ)-(GenP_YZ*GenP_YZ)-(GenP_ZZ*GenP_ZZ));
		     
		      GenTrMassZ_rel=sqrt((GenEnergyZ*GenEnergyZ)-(GenP_ZZ*GenP_ZZ));
		      
		      GenTrMassZ_A=sqrt((GenTrEnergyZ*GenTrEnergyZ)-(GenP_XZ*GenP_XZ)-(GenP_YZ*GenP_YZ));
		      //GenTrMassZ_B=sqrt((energiatrasvZ*energiatrasvZ)-(GenP_XZ*GenP_XZ)-(GenP_YZ*GenP_YZ));
		     
		      GenVisMassZ=sqrt(((muener+jetMomener)*(muener+jetMomener))-((mupx+jetMompx)*(mupx+jetMompx))-((mupy+jetMompy)*(mupy+jetMompy))-((mupz+jetMompz)*(mupz+jetMompz)));

		      GenVisTrMassZ_A=sqrt(((GenVisTrEnergyZ)*(GenVisTrEnergyZ))-((mupx+jetMompx)*(mupx+jetMompx))-((mupy+jetMompy)*(mupy+jetMompy)));
		      //GenVisTrMassZ_B=sqrt((energiatrasvvisZ*energiatrasvvisZ)-(mupx+jetMompx)*(mupx+jetMompx)-(mupy+jetMompy)*(mupy+jetMompy));
		      GenVisTrMassZ_rel=sqrt((muener+jetMomener)*(muener+jetMomener)-(mupz+jetMompz)*(mupz+jetMompz));

		      TLorentzVector Zeta_fin=tauneut+mu+jetMom+neutau+neumu;
		      Zeta_fin_eta=Zeta_fin.Eta();
		      Zeta_fin_rapidity=Zeta_fin.Rapidity();
		     
		      TLorentzVector Zeta=tauh_ap+taul_ap;
		      Z_eta=Zeta.Eta();
		      Z_rapidity=Zeta.Rapidity();
		      Z_rap=0.5*log(((GenEnergyZ+GenP_ZZ)/(GenEnergyZ-GenP_ZZ)));
		      
		      //Zpt=Zeta.Pt();
		      //Zenertr=Zeta.Et();
		      //Zpx=Zeta.Px();
		      //Zpy=Zeta.Py();
		      //Zpz=Zeta.Pz();
		     
		      //Zeta_fin_massTLV=sqrt((Zeta_fin_ener*Zeta_fin_ener)-(Zeta_fin_px*Zeta_fin_px)-(Zeta_fin_py*Zeta_fin_py)-(Zeta_fin_pz*Zeta_fin_pz));
		      //Zeta_fin_masstrTLV=sqrt((Zeta_fin_enertr*Zeta_fin_enertr)-(Zeta_fin_px*Zeta_fin_px)-(Zeta_fin_py*Zeta_fin_py));
		      // Zeta_fin_masstrTLV=sqrt((ener*energiatrasvZ)-(Zeta_fin_px*Zeta_fin_px)-(Zeta_fin_py*Zeta_fin_py));
 
		     		     
		      ///////// masse Z
		      //Zmass_vis=sqrt((jetMomener+muener)*(jetMomener+muener)-(jetMompx+mupx)*(jetMompx+mupx)-(jetMompy+mupy)*(jetMompy+mupy)-(jetMompz+mupz)*(jetMompz+mupz));
		      
		      //Zmasstr_vis=sqrt((jetMomenertr+muenertr)*(jetMomenertr+muenertr)-(jetMompt+mupt)*(jetMompt+mupt));
		      
		      // Zmasstr_vis=sqrt((

		      //Zmass_all=sqrt((jetMomener+muener+tauneutener+neutauener+neumuener)*(jetMomener+muener+tauneutener+neutauener+neumuener)-(jetMompx+mupx+neumupx+neutaupx+tauneutpx)*(jetMompx+mupx+neumupx+neutaupx+tauneutpx)-(jetMompy+mupy+neumupy+neutaupy+tauneutpy)*(jetMompy+mupy+neumupy+neutaupy+tauneutpy)-(jetMompz+mupz+neumupz+neutaupz+tauneutpz)*(jetMompz+mupz+neumupz+neutaupz+tauneutpz));
		      
		      //Zmasstr_all_A=sqrt((GenTrEnergyZ)*(GenTrEnergyZ)-(jetMompx+mupx+neumupx+neutaupx+tauneutpx)*(jetMompx+mupx+neumupx+neutaupx+tauneutpx)-(jetMompy+mupy+neumupy+neutaupy+tauneutpy)*(jetMompy+mupy+neumupy+neutaupy+tauneutpy));	   
		     
		      //Zmasstr_all_B=sqrt((GenEnergyZ*GenEnergyZ)-(GenP_ZZ*GenP_ZZ));


		      ////////////////togli SONO CALCOLATE DAL TLV DEI TAU INVECE CHE DAI PRODOTTI FINALI
		      //ZmassTLV=sqrt((Zener*Zener)-(GenP_XZ*GenP_XZ)-(GenP_YZ*GenP_YZ)-(GenP_ZZ*GenP_ZZ));
		      //ZmasstrTLV=sqrt((GenTrEnergyZ*GenTrEnergyZ)-(GenP_XZ*GenP_XZ)-(GenP_YZ*GenP_YZ));
		     
		      //Zmass_bp=sqrt((etaul_bp+etauh_bp)*(etaul_bp+etauh_bp)-(pxtaul_bp+pxtauh_bp)*(pxtaul_bp+pxtauh_bp)-(pytaul_bp+pytauh_bp)*(pytaul_bp+pytauh_bp)-(pztaul_bp+pztauh_bp)*(pztaul_bp+pztauh_bp));
		      
		      //Zmass_ap=sqrt((etaul_ap+etauh_ap)*(etaul_ap+etauh_ap)-(pxtaul_ap+pxtauh_ap)*(pxtaul_ap+pxtauh_ap)-(pytaul_ap+pytauh_ap)*(pytaul_ap+pytauh_ap)-(pztaul_ap+pztauh_ap)*(pztaul_ap+pztauh_ap));
		      
		      //Zeta_masstr_relat=sqrt((GenEnergyZ*GenEnergyZ)-(GenP_ZZ*GenP_ZZ));
		     
 
		      //////masse trasverse relativistiche
		      
		      // Zmasstr_all_relat=sqrt((jetMomener+muener+tauneutener+neutauener+neumuener)*(jetMomener+muener+tauneutener+neutauener+neumuener)-(jetMompz+mupz+neumupz+neutaupz+tauneutpz)*(jetMompz+mupz+neumupz+neutaupz+tauneutpz));
		      
		      //Zvisibletransversemass_relat1=sqrt((jetMomener+muener)*(jetMomener+muener)-(jetMompz+mupz)*(jetMompz+mupz));
		      // Zeta_fin_masstr_relat_A=sqrt((Zeta_fin_ener*Zeta_fin_ener)-(Zeta_fin_pz*Zeta_fin_pz));
		      //Zeta_fin_masstr_relat_B=sqrt((energiaZ*energiaZ)-(pzZ*pzZ));
		      
		      /////////////SBAGLIATA		      
		      Zvisibletransversemass_relat2=sqrt((jetMomener*muener)-(jetMompz*mupz));
		      		      		      
		     //  ////////////////quadrivettori di tutte le particelle finali
// 		      leptpx=mupx+neutaupx+neumupx;
// 		      leptpy=mupy+neutaupy+neumupy;
// 		      leptpz=mupz+neutaupz+neumupz;
// 		      leptener=muener+neutauener+neumuener;
// 		      leptenertr=muenertr+neumuenertr+neutauenertr;
		      
// 		      adrpx=jetMompx+tauneutpx;
// 		      adrpy=jetMompy+tauneutpy;
// 		      adrpz=jetMompz+tauneutpz;
// 		      adrener=jetMomener+tauneutener;
// 		      adrenertr=jetMomenertr+tauneutenertr;
		      
		      
		      //////////////masse trasverse
		      //trmass1=sqrt(sqrt((pxtaul_ap*pxtaul_ap)+(pytaul_ap*pytaul_ap))*sqrt((pxtauh_ap*pxtauh_ap)+(pytauh_ap*pytauh_ap))-(pxtaul_ap*pxtauh_ap)-(pytaul_ap*pytauh_ap));
		      
		      //trmass2=sqrt(leptenertr*adrenertr-(leptpx*adrpx)-(leptpy*adrpy));
		      
		      //trmass3=sqrt((etaul_ap*sin(taul_aptheta)*etauh_ap*sin(tauh_aptheta))-(pxtaul_ap*pxtauh_ap)-(pytaul_ap*pytauh_ap));		    
		      
		      //trmass4=sqrt(sqrt(((mupx+neutaupx+neumupx)*(mupx+neutaupx+neumupx))+(mupy+neutaupy+neumupy)*(mupy+neutaupy+neumupy))*sqrt((jetMompx+tauneutpx)*(jetMompx+tauneutpx)+(jetMompy+tauneutpy)*(jetMompy+tauneutpy))-((mupx+neutaupx+neumupx)*(jetMompx+tauneutpx))-((mupy+neutaupy+neumupy)*(jetMompy+tauneutpy)));  
		      
		      //trmass5=sqrt((muenertr+neumuenertr+neutauenertr+jetMomenertr+tauneutenertr)*(muenertr+neumuenertr+neutauenertr+jetMomenertr+tauneutenertr)-(mupx+neumupx+neutaupx+jetMompx+tauneutpx)*(mupx+neumupx+neutaupx+jetMompx+tauneutpx)-(mupy+neumupy+neutaupy+jetMompy+tauneutpy)*(mupy+neumupy+neutaupy+jetMompy+tauneutpy));
		      
		      //trmass6=sqrt((GenTrEnergyZ*GenTrEnergyZ)-(GenP_XZ*GenP_XZ)-(GenP_YZ*GenP_YZ));
		      
		      
		      ///////masse trasverse visibili
		      
		      trvismass1=sqrt(sqrt(mupx*mupx+mupy*mupy)*sqrt(jetMompx*jetMompx+jetMompy*jetMompy)-((mupx*mupx)+(jetMompx*jetMompx))-((mupy*mupy)+(jetMompy*jetMompy)));
		      //trvismass2=sqrt(muenertr*jetMomenertr-(mupx*jetMompx)-(mupy*jetMompy));
		      //trvismass3=sqrt((muener*sin(mutheta))*(jetMomener*sin(jetMomtheta))-(mupx*jetMompx)-(mupy*jetMompy));
		      //trvismass4=sqrt((jetMomenertr+muenertr)*(jetMomenertr+muenertr)-(jetMompt+mupt)*(jetMompt+mupt));
		      
		    } 
		}  //primo if
	      //std::cout << "found2SimTaus: " << found2taus << std::endl;
	    } //if condizione su 2 figli      
	  //}    
	} //if riconoscimento Z     	
    }  //for (p = generated_event->particles_begin(); p!=generated_event->particles_end(); p++)
  
  
  //       // //   //  /// Charged particles
  
  
  countchpartjet=0;
  
  ptleadingtrack=-100.;
  ptparticle=-100.;    
  charged=0;

  for (HepMC::GenEvent::particle_iterator q = generated_event->particles_begin(); q!= generated_event->particles_end();++q) 
    {
      
      if ( (*q)->status() == 1 && 
	   (*q)->pdg_id() != 22 && 
	   abs((*q)->pdg_id()) != 16 && 
	   abs((*q)->pdg_id()) != 14 && 
	   abs((*q)->pdg_id()) != 12 &&
	   abs((*q)->pdg_id()) != 3122 &&
	   abs((*q)->pdg_id()) != 3322 && 
	   abs((*q)->pdg_id()) != 2112 &&
	   abs((*q)->pdg_id()) != 4112 &&
	   abs((*q)->pdg_id()) != 3212 &&
	   abs((*q)->pdg_id()) != 4132 &&
	   abs((*q)->pdg_id()) != 4312 &&
	   abs((*q)->pdg_id()) != 4332 &&
	   abs((*q)->pdg_id()) != 5122 &&
	   abs((*q)->pdg_id()) != 5212 &&
	   abs((*q)->pdg_id()) != 2114 &&
	   abs((*q)->pdg_id()) != 3214 &&
	   abs((*q)->pdg_id()) != 3324 &&
	   abs((*q)->pdg_id()) != 4114 &&
	   abs((*q)->pdg_id()) != 4314 &&
	   abs((*q)->pdg_id()) != 4334 &&
	   abs((*q)->pdg_id()) != 5214 &&
	   abs((*q)->pdg_id()) != 130 &&
	   abs((*q)->pdg_id()) != 311 &&
	   abs((*q)->pdg_id()) != 421 &&
	   abs((*q)->pdg_id()) != 511 &&
	   abs((*q)->pdg_id()) != 531 &&
	   abs((*q)->pdg_id()) != 111 &&
	   abs((*q)->pdg_id()) != 313 &&
	   abs((*q)->pdg_id()) != 423 &&
	   abs((*q)->pdg_id()) != 513 &&
	   abs((*q)->pdg_id()) != 533 &&
	   abs((*q)->pdg_id()) != 113 &&
	   abs((*q)->pdg_id()) != 310 &&
	   abs((*q)->pdg_id()) != 10313 &&
	   abs((*q)->pdg_id()) != 10423 &&
	   abs((*q)->pdg_id()) != 10113 &&
	   abs((*q)->pdg_id()) != 10223 &&
	   abs((*q)->pdg_id()) != 10333 &&
	   abs((*q)->pdg_id()) != 10443 &&
	   abs((*q)->pdg_id()) != 20313 &&
	   abs((*q)->pdg_id()) != 20423 &&
	   abs((*q)->pdg_id()) != 20113 &&
	   abs((*q)->pdg_id()) != 20223 &&
	   abs((*q)->pdg_id()) != 20333 &&
	   abs((*q)->pdg_id()) != 20443 &&
	   abs((*q)->pdg_id()) != 10311 &&
	   abs((*q)->pdg_id()) != 10421 &&
	   abs((*q)->pdg_id()) != 10111 &&
	   abs((*q)->pdg_id()) != 10221 &&
	   abs((*q)->pdg_id()) != 10331 &&
	   abs((*q)->pdg_id()) != 10441 &&
	   abs((*q)->pdg_id()) != 315 &&
	   abs((*q)->pdg_id()) != 425 &&
	   abs((*q)->pdg_id()) != 115 &&
	   abs((*q)->pdg_id()) != 335 &&
	   abs((*q)->pdg_id()) != 445 )
	{ 	
	  
	  // 	//vector<HepMC::GenParticle*> ancestor = (*p)->listAncestors();
	  // 	//vector<HepMC::GenParticle*>::const_iterator ancestors_iter;
	  
	 bool foundtauancestor=false;
 	  bool noleptmuons=true;
	  
	  
// 	  // 	//for (ancestors_iter = ancestor.begin(); ancestors_iter != ancestor.end(); ancestors_iter++) 

//for( HepMC::GenVertex::particle_iterator aDaughter=(*p)->end_vertex()->particles_begin(HepMC::descendants); aDaughter !=(*p)->end_vertex()->particles_end(HepMC::descendants);aDaughter++)

	  
	  for( HepMC::GenVertex::particle_iterator ancestors_iter=(*q)->production_vertex()->particles_begin(HepMC::parents); ancestors_iter!=(*q)->production_vertex()->particles_end(HepMC::parents); ancestors_iter++)
	  	{
		  if ( abs((*ancestors_iter)->pdg_id()) == 15) 
		    {
		      if (abs((*q)->pdg_id())==13)
			{
			  barcodemujet=abs((*q)->barcode());
			  //cout<<"BARCODE mu JET= "<<barcodemulept<<endl;
			  if (barcodemulept==barcodemujet)
			    {
			      noleptmuons=false;
			    }
			}	
		      
		      foundtauancestor=true;
		    }
		  
		  }
	      
	  
	  if ((foundtauancestor == true)&&(noleptmuons==true))
	    
	    {
	      countchpartjet++; 
	      
	      ptparticle=(*q)->momentum().perp();
	      if(ptparticle>ptleadingtrack)
		{
		  ptleadingtrack=ptparticle;
		}
	    }
	  charged=countchpartjet;
	}
 
    }     //for calcolo particelle cariche
 
 
  
  //   //   ///////////////////////////////////////
  //   //   ////////INIZIA RICOSTRUZIONE
  //   //   /////////////////////////////7
  


////////---------------------CALO 
  
//   Handle<TauCollection> tauHandle;
//   iEvent.getByLabel(CaloTaus_,tauHandle);
//   const TauCollection& myTauCollection=*(tauHandle.product()); 
  
  CaloRecoP_TTau=-100.;
  CaloRecoP_XTau=-100.;
  CaloRecoP_YTau=-100.;
  CaloRecoP_ZTau=-100.;
  CaloRecoP_TLeadTrk=-100.;
  CaloRecoInvMassTau=-100.;
  CaloRecoNumTrks=-100.;
  CaloRecoEmEnergyFraction=-100.;
  CaloRecoMaxHadronEnergy=-100.;
  CaloRecoNumSignalTrks=-100.;
  CaloRecoNumIsolationTrks=-100.;
  CaloRecoSumpTIsolationTracks=-100.;
  CaloRecoSumE_TIsolationGammaCandidates=-100.;
  CalosignalgammacandidatesSize=-100.;
  
  CaloRecoEnergyTau=-100.;
  CaloRecoEtaTau=-100.;
  CaloRecoTrEnergyTau=-100;
  CaloRecoTrMassTau=-100.;
  
    
//   for(TauCollection::const_iterator iT =myTauCollection.begin();iT !=myTauCollection.end();iT++){  
//     //Prints out some quantities
//     // cout<<"Jet Number "<<it<<endl;
//     //it++;
     
//      cout<<"Pt of the Tau "<<iT->pt()<<endl;
//      // RecoP_TTau =iT->pt();
     
//      TrackRef theLeadTk = iT->getLeadingTrack();
//      if(!theLeadTk) 
//        {
// 	 cout<<"No Lead Tk "<<endl;
//        }
//      else
//        {
// 	 cout<<"Lead Tk pt "<<(*theLeadTk).pt()<<endl;
// 	 CaloRecoP_TLeadTrk=(*theLeadTk).pt();
	 
// 	 CaloRecoP_TTau =iT->pt();
// 	 CaloRecoP_XTau =iT->px();
// 	 CaloRecoP_YTau =iT->py();
// 	 CaloRecoP_ZTau =iT->pz();
// 	 CaloRecoEnergyTau=iT->energy();
// 	 CaloRecoEtaTau=iT->eta();
// 	 CaloRecoTrEnergyTau=iT->et();
// 	 CaloRecoTrMassTau=iT->mt();
	 
// 	 CaloRecoInvMassTau=iT->getInvariantMass();
	 
// 	 // 	 if (iT->getSignalGammaCandidates().size()>0)
// 	 //  	   {
// 	 //  	     cout<<"InvariantMass of the Tau "<<iT->getInvariantMass()<<endl;
	 
// 	 //  	     RecoInvMassTau=iT->getInvariantMass();
	     
// 	 //  	   }
// 	 cout<<"Vertex of the Tau "<<iT->vz()<<endl;
// 	 cout<<"Charge of the Tau "<<iT->charge()<<endl;
	 
// 	 cout<<"Em energy fraction "<<iT->getEmEnergyFraction()<<endl;
// 	 CaloRecoEmEnergyFraction=iT->getEmEnergyFraction();
	 
// 	 cout<<"Max Hadron energy "<<iT->getMaximumHcalEnergy()<<endl;
// 	 CaloRecoMaxHadronEnergy=iT->getMaximumHcalEnergy();
	 
// 	 cout<<"# Tracks "<<iT->getSelectedTracks().size()<<endl;
// 	 CaloRecoNumTrks=iT->getSelectedTracks().size();
	 
// 	 cout<<"Number of Signal Tracks = "<<iT->getSignalTracks().size()<<endl;
// 	 CaloRecoNumSignalTrks=iT->getSignalTracks().size();
	 
// 	 cout<<"Number of Isolation Tracks = "<<iT->getIsolationTracks().size()<<endl;
// 	 CaloRecoNumIsolationTrks=iT->getIsolationTracks().size();
	 
// 	 cout<<"Sum pT of Isolation Tracks = "<<iT->getSumPtIsolation()<<endl;
// 	 CaloRecoSumpTIsolationTracks=iT->getSumPtIsolation();
	 
// 	 cout<<"Sum E_T of Isolation Gamma Candidates = "<<iT->getEMIsolation()<<endl;
// 	  CaloRecoSumE_TIsolationGammaCandidates=iT->getEMIsolation();
	 
// 	  CalosignalgammacandidatesSize=iT->getSignalGammaCandidates().size();
	 
// 	 // 	 if ((signalgammacandidatesSize>0)&&(iT->getLeadingTrack().isNonnull())){
// 	 // 	   cout<<"InvariantMass of the Tau "<<iT->getInvariantMass()<<endl;
// 	 }
//   }
//   //}

////////---------------------PF
  
  Handle<TauCollection> tauHandlePF;
  iEvent.getByLabel(PFTaus_,tauHandlePF);
  const TauCollection& myTauCollectionPF=*(tauHandlePF.product()); 
  
   cout<<"Found "<<myTauCollectionPF.size()<<" had.tau-jet candidates"<<endl;
  

   PFRecoP_TTau=-100.;
   PFRecoP_XTau=-100.;
   PFRecoP_YTau=-100.;
   PFRecoP_ZTau=-100.;
   PFRecoNumChargHadrCand=-100.;
   PFRecoNumNeutHadrCand=-100.;
   PFRecoNumGammaCand=-100.;
   PFRecoSignChargHadr=-100.;
   PFRecoNumIsolChargHadr=-100.;
   PFRecoIsolGammaCand=-100.;
   PFRecoInvMassTau=-10.;
   PFRecoEmEnergyFraction=-100.;
   PFRecoMaxHadronEnergy=-100.;
   PFRecoSumpTIsolationTracks=-100.;
   PFRecoSumE_TIsolationGammaCandidates=-100.;
   
   PFRecoP_TLeadTrk=-100.;
   PFRecoEnergyTau=-100.;
   PFRecoEtaTau=-100.;
   PFRecoTrEnergyTau=-100;
   PFRecoTrMassTau=-100.;
   PFRecoSignalGammaCandSize=-100.;
   for(TauCollection::const_iterator iT =myTauCollectionPF.begin();iT !=myTauCollectionPF.end();iT++){  
 
     
     //cout<<"Pt of the Tau "<<iT->pt()<<endl;
     
	
     //FlagRecoTau=true;	


    PFCandidateRef theLeadPFCand = iT->getLeadingChargedHadron();
      // PFRecoInvMassTau=iT->getInvariantMass();
        
// 	 PFRecoP_TTau =iT->pt();
// 	 PFRecoP_XTau =iT->px();
// 	 PFRecoP_YTau =iT->py();
// 	 PFRecoP_ZTau =iT->pz();
// 	 PFRecoEnergyTau=iT->energy();
// 	 PFRecoEtaTau=iT->eta();
// 	 PFRecoTrEnergyTau=iT->et();
// 	 PFRecoTrMassTau=iT->mt();
	     
      if(!theLeadPFCand) 
       {
	 cout<<"No Lead PFCand "<<endl;
       }
     else
       {
	 FlagRecoTau=true;	 
cout<<" Lead PFCand pt"<<(*theLeadPFCand).pt()<<endl;
	 PFRecoP_TLeadTrk=(*theLeadPFCand).pt();
       
	 PFRecoP_TTau =iT->pt();
	 PFRecoP_XTau =iT->px();
	 PFRecoP_YTau =iT->py();
	 PFRecoP_ZTau =iT->pz();
	 PFRecoEnergyTau=iT->energy();
	 PFRecoEtaTau=iT->eta();
	 PFRecoTrEnergyTau=iT->et();

	 PFRecoTrMassTau=sqrt((PFRecoEnergyTau*PFRecoEnergyTau)-(PFRecoP_ZTau*PFRecoP_ZTau)); 
 PFRecoInvMassTau=sqrt((PFRecoEnergyTau*PFRecoEnergyTau)-(PFRecoP_XTau*PFRecoP_XTau)-(PFRecoP_YTau*PFRecoP_YTau)-(PFRecoP_ZTau*PFRecoP_ZTau)); 	 
	 //PFRecoInvMassTau=iT->getInvariantMass();
	 
 	
	  cout<<"Vertex of the Tau "<<iT->vz()<<endl;
	  cout<<"Charge of the Tau "<<iT->charge()<<endl;
	  
	 cout<<"Em energy fraction "<<iT->getEmEnergyFraction()<<endl;
	 PFRecoEmEnergyFraction=iT->getEmEnergyFraction();
	 
	 cout<<"Max Hadron energy "<<iT->getMaximumHcalEnergy()<<endl;
	 PFRecoMaxHadronEnergy=iT->getMaximumHcalEnergy();
	 
	 cout<<"# PF charged hadr. cand's "<<iT->getSelectedChargedHadrons().size()<<endl;
	 PFRecoNumChargHadrCand=iT->getSelectedChargedHadrons().size();
	 
	 cout<<"# PF neutral hadr. cand's "<<iT->getSelectedNeutralHadrons().size()<<endl;
	 PFRecoNumNeutHadrCand=iT->getSelectedNeutralHadrons().size();
	 
	 cout<<"# PF gamma cand's "<<iT->getSelectedGammaCandidates().size()<<endl;	   
	 PFRecoNumGammaCand=iT->getSelectedGammaCandidates().size();

	cout<<"Number of SignalPFChargedHadrons = "<<iT->getSignalChargedHadrons().size()<<endl;
	PFRecoSignChargHadr=iT->getSignalChargedHadrons().size();
	 
	cout<<"Number of IsolationPFChargedHadrons = "<<iT->getIsolationChargedHadrons().size()<<endl;
	PFRecoNumIsolChargHadr=iT->getIsolationChargedHadrons().size();

	cout<<"Number of IsolationPFGammaCandidate = "<<iT->getIsolationGammaCandidates().size()<<endl;
	PFRecoIsolGammaCand=iT->getIsolationGammaCandidates().size();
	 
	 cout<<"Sum pT of Isolation Charged Hadrons= "<<iT->getSumPtIsolation()<<endl;
	 PFRecoSumpTIsolationTracks=iT->getSumPtIsolation();
	 
	 cout<<"Sum E_T of Isolation Gamma Candidates = "<<iT->getEMIsolation()<<endl;
	 PFRecoSumE_TIsolationGammaCandidates=iT->getEMIsolation();
	 
	 PFRecoSignalGammaCandSize=iT->getSignalGammaCandidates().size();
	 
	//  if ((PFRecoSignalGammaCandSize!=0)) 
//   	   { 
//   	     PFRecoInvMassTau=iT->getInvariantMass(); 
// 	   }
	 
	 
       }
   }
   
   
   //////////////////////////////INIZIA RECO MUON/////////////////////
   
   
   Handle<MuonCollection> muonHandle;
   iEvent.getByLabel(RecoMuons_,muonHandle);
   const MuonCollection& myMuonCollection=*(muonHandle.product()); 
   
   
   RecoP_TMuon=-100.;
   RecoP_TMuonFrompxpy=-100.;
   
   RecoP_XMuon=-100.;
   RecoP_YMuon=-100.;
   RecoP_ZMuon=-100.;
   RecoEnergyMuon=-100.;
   RecoTrEnergyMuon=-100.;
   RecoInvMassMuon=-100.;
   RecoEtaMuon=-100.;
   RecoTrMassMuon=-100.;
   
   
   
   for(MuonCollection::const_iterator iT =myMuonCollection.begin();iT !=myMuonCollection.end();iT++){  
     if((iT)->energy()>((iT-1)->energy())){
       FlagRecoMuon=true;       
RecoEnergyMuon=iT->energy();
      
RecoP_TMuon=iT->pt();
       // RecoP_ZMuon=sqrt(RecoEnergyMuon*RecoEnergyMuon-RecoInvMassMuon*RecoInvMassMuon-RecoP_TMuon*RecoP_TMuon);
      
       RecoP_XMuon=iT->px();
       RecoP_YMuon=iT->py();
       RecoP_ZMuon=iT->pz();
       //RecoP_TMuonFrompxpy=sqrt(RecoP_XMuon*RecoP_XMuon+RecoP_YMuon*RecoP_YMuon);

       RecoInvMassMuon=sqrt((RecoEnergyMuon*RecoEnergyMuon)-(RecoP_XMuon*RecoP_XMuon)-(RecoP_YMuon*RecoP_YMuon)-(RecoP_ZMuon*RecoP_ZMuon)); 
       RecoEtaMuon=iT->eta();
       RecoTrMassMuon=iT->mt();
       RecoTrEnergyMuon=iT->et();
     }
   }
   
   //////////////////////////////////////////INIZIA RECO GENMET
   
   Handle<GenMETCollection> genmetHandle;
   iEvent.getByLabel(GenMet_,genmetHandle);
   const GenMETCollection& myGenMetCollection=*(genmetHandle.product()); 
   
   GenMETMissTrEner=-100.;
   GenMETSumMissTrEner=-100.;
   GenMETSignificance=-100.;
   GenMET_E_long=-100.;
   
   for(GenMETCollection::const_iterator iT =myGenMetCollection.begin();iT !=myGenMetCollection.end();iT++){ 
     
     //Missing Transverse energy
     GenMETMissTrEner=(iT->mEtSig())*sqrt(iT->sumEt());
     
     //scalar sum of transverse energy over all objects   
     GenMETSumMissTrEner=iT->sumEt();
     
     //MET significance =MET/ srrt(SumET);
     GenMETSignificance=iT->mEtSig();
     
     //longitudinal component of the vector sum of energy over all objects
     GenMET_E_long=iT->e_longitudinal();
   }
CaloMETMissTrEner=-100.;
   CaloMETSumMissTrEner=-100.;
   CaloMETSignificance=-100.;
   CaloMET_E_long=-100.;
   CaloMET_x=-100.;
   CaloMET_y=-100.;
   //   CaloMET_z=-100.;
   CaloMET_time=-100.;
 
// RECO CALO MET

   Handle<CaloMETCollection> calometHandle;
   iEvent.getByLabel(CaloMet_,calometHandle);
   const CaloMETCollection& myCaloMetCollection=*(calometHandle.product());
   // const CaloMET calomet= myCaloMetCollection.front();

   

   for(CaloMETCollection::const_iterator iT =myCaloMetCollection.begin();iT !=myCaloMetCollection.end();iT++){ 
     FlagRecoMET=true;  

     //for(CaloMET::const_iterator iT =calomet.begin();iT !=calomet.end();iT++){    
     //Missing Transverse energy
     CaloMETMissTrEner=(iT->mEtSig())*sqrt(iT->sumEt());

     //scalar sum of transverse energy over all objects   
      CaloMETSumMissTrEner=iT->sumEt();
     
     CaloMET_x=iT->px();
     CaloMET_y=iT->py();
     //CaloMET_z=iT->pz();
     //CaloMET_time=iT->pt();
     CaloMET_time=sqrt((CaloMET_x*CaloMET_x)+(CaloMET_y*CaloMET_y));

     //MET significance =MET/ sqrt(SumET);
     CaloMETSignificance=iT->mEtSig();
     
     //longitudinal component of the vector sum of energy over all objects
     CaloMET_E_long=iT->e_longitudinal();
   }

 
  // PFRecoP_Z=-100.;
   //PFRecoVisMassZ=-100.;
  
  //  PFRecoInvMassZ_A=-10.;
//    PFRecoInvMassZ_B=-10.;
//    PFRecoInvMassZ_C=-10.;
  
 PFRecoP_TZ=-500.;
  PFRecoP_XZ=-500.;
  PFRecoP_YZ=-500.;
  PFRecoP_ZZ=-500.;

 
  PFRecoVisP_XZ=-500.;
  PFRecoVisP_YZ=-500.;
  PFRecoVisP_ZZ=-500.;
 
 PFRecoEnergyZ=-500.;
  PFRecoTrEnergyZ=-500.;
  PFRecoTrVisEnergyZ=-500.;
   PFRecoVisEnergyZ=-500.;
  
   //PFRecoTrMassZ_A=-100.;
   //PFRecoTrMassZ_B=-100.;
   //PFRecoTrMassZ_C=-100.;
  
    //PFRecoTrVisMassZ=-100.;
   //PFRecoTrVisMassZ_rel=-100.;

  if((FlagRecoTau==true)&&(FlagRecoMuon==true)&&(FlagRecoMET==true)){

    PFRecoEnergyZ=PFRecoEnergyTau+RecoEnergyMuon+CaloMET_time;     
    PFRecoP_XZ=PFRecoP_XTau+RecoP_XMuon+CaloMET_x;
    PFRecoP_YZ=PFRecoP_YTau+RecoP_YMuon+CaloMET_y;  
    PFRecoP_ZZ=PFRecoP_ZTau+RecoP_ZMuon;

    PFRecoVisP_XZ=PFRecoP_XTau+RecoP_XMuon;
    PFRecoVisP_YZ=PFRecoP_YTau+RecoP_YMuon;
    PFRecoVisP_ZZ=PFRecoP_ZTau+RecoP_ZMuon;
    

   
    
    PFRecoP_Z=sqrt((PFRecoP_XZ*PFRecoP_XZ)+(PFRecoP_YZ*PFRecoP_YZ)+(PFRecoP_ZZ*PFRecoP_ZZ));
    PFRecoVisP_Z=sqrt((PFRecoVisP_XZ*PFRecoVisP_XZ)+(PFRecoVisP_YZ*PFRecoVisP_YZ)+(PFRecoVisP_ZZ*PFRecoVisP_ZZ));
    PFRecoP_TZ=sqrt((PFRecoP_XZ*PFRecoP_XZ)+(PFRecoP_YZ*PFRecoP_YZ));
    
    PFRecoTrEnergyZ=sqrt((PFRecoEnergyZ*PFRecoEnergyZ)-(PFRecoP_ZZ*PFRecoP_ZZ));

    PFRecoVisEnergyZ=PFRecoEnergyTau+RecoEnergyMuon;
    
    PFRecoTrVisEnergyZ=sqrt((PFRecoVisEnergyZ*PFRecoVisEnergyZ)-(PFRecoP_ZZ*PFRecoP_ZZ));
     
    
    ///////////MASSE    
    PFRecoTrMassZ_B=sqrt((PFRecoEnergyZ*PFRecoEnergyZ)-(PFRecoP_ZZ*PFRecoP_ZZ));   
    
    PFRecoTrVisMassZ_rel=sqrt((PFRecoVisEnergyZ*PFRecoVisEnergyZ)-(PFRecoVisP_ZZ*PFRecoVisP_ZZ));    
    

 PFRecoInvMassZ_A=sqrt((PFRecoEnergyZ*PFRecoEnergyZ)-(PFRecoP_XZ*PFRecoP_XZ)-(PFRecoP_YZ*PFRecoP_YZ)-(PFRecoP_ZZ*PFRecoP_ZZ));

 PFRecoInvMassZ_C=sqrt((PFRecoTrEnergyZ*PFRecoTrEnergyZ)-(PFRecoP_TZ*PFRecoP_TZ));

 PFRecoInvMassZ_B=sqrt(((PFRecoEnergyTau+RecoEnergyMuon+CaloMET_time)*(PFRecoEnergyTau+RecoEnergyMuon+CaloMET_time))-((PFRecoP_XTau+RecoP_XMuon+CaloMET_x)*(PFRecoP_XTau+RecoP_XMuon+CaloMET_x))-((PFRecoP_YTau+RecoP_YMuon+CaloMET_y)*(PFRecoP_YTau+RecoP_YMuon+CaloMET_y))-((PFRecoP_ZTau+RecoP_ZMuon)*(PFRecoP_ZTau+RecoP_ZMuon)));

  
 PFRecoTrMassZ_A=sqrt((PFRecoTrEnergyZ*PFRecoTrEnergyZ)-(PFRecoP_XZ*PFRecoP_XZ)-(PFRecoP_YZ*PFRecoP_YZ));

 PFRecoTrMassZ_C=sqrt((PFRecoTrEnergyZ*PFRecoTrEnergyZ)-(PFRecoP_TZ*PFRecoP_TZ));
 
 PFRecoTrVisMassZ=sqrt((PFRecoTrVisEnergyZ*PFRecoTrVisEnergyZ)-(PFRecoVisP_XZ*PFRecoVisP_XZ)-(PFRecoVisP_YZ*PFRecoVisP_YZ));


PFRecoVisMassZ=sqrt((PFRecoEnergyTau+RecoEnergyMuon)*(PFRecoEnergyTau+RecoEnergyMuon)-(PFRecoP_XTau+RecoP_XMuon)*(PFRecoP_XTau+RecoP_XMuon)-(PFRecoP_YTau+RecoP_YMuon)*(PFRecoP_YTau+RecoP_YMuon)-(PFRecoP_ZTau+RecoP_ZMuon)*(PFRecoP_ZTau+RecoP_ZMuon));
 
//PFRecoVisMassZ=sqrt((PFRecoVisEnergyZ*PFRecoVisEnergyZ)-(PFRecoVisP_Z*PFRecoVisP_Z));
 
 }
 
  //Dump Run and Event
  irun=iEvent.id().run();
  ievt=iEvent.id().event();
  
  
  std::cout << "\n now filling the tree" <<std::endl;      
  tree_->Fill();
  ///////
  /////////////  /////////
}


// ------------ method called once each job just before starting event loop  ------------
void 
ZTauTauAnalysis::beginJob(const edm::EventSetup&)
{
  std::cout << "ZTauTauAnalysis Starting! \n "; 
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ZTauTauAnalysis::endJob() {
  cout<<"Analysis completed"<<endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(ZTauTauAnalysis);





