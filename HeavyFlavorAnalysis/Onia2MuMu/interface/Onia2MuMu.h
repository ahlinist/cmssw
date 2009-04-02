#ifndef Onia2MuMu_h
#define Onia2MuMu_h

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/CaloMuon.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// #include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/HepMCProduct/interface/GenInfoProduct.h"
#include "FWCore/Framework/interface/Run.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"

#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"


#include <memory>
#include <iostream>
#include <string>
#include <map>
#include <set>

#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TParameter.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TMatrixD.h>
#include <TClonesArray.h>

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Common/interface/View.h"

using namespace reco;
using namespace edm;
using namespace std;

class Onia2MuMu : public edm::EDAnalyzer {
 public:
      explicit Onia2MuMu(const edm::ParameterSet&);
      ~Onia2MuMu();
 private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      virtual void l1Report(const edm::Event &iEvent); 
      virtual void hltReport(const edm::Event &iEvent);
      virtual void fillGeneratorBlock(const edm::Event &iEvent);
      virtual void fillRecTracks(const edm::Event &iEvent);
      virtual void fillPhotons(const edm::Event &iEvent); 
      virtual void fillMuons(const edm::Event &iEvent);
      virtual void fillPATMuons(const edm::Event &iEvent);
      virtual void PAT_l1Report(const edm::Event &iEvent); 
      virtual void PAT_hltReport(const edm::Event &iEvent);
      virtual void fillBeamSpot(const edm::Event &iEvent);
      virtual void fillPrimaryVertex(const edm::Event &iEvent);
      virtual void findOniaCategories(const edm::Event &iEvent);
      virtual void fillOniaMuMuTracks(TrackRef muon1, int m1, TrackRef muon2, int m2, TVector3 vperp2, int oniacato);

      double PhiInRange(const double& phi) const;
      template <class T, class U> double deltaR(const T & t, const U & u) const;
      double calcDeltaR(double eta1, double phi1,double eta2, double phi2);
      double GetTheta( TLorentzVector & a,  TLorentzVector & b) const;
      void printTrack(const reco::Track& muon) const;
      TLorentzVector lorentzMomentum(const reco::Track& muon) const;
      TLorentzVector lorentzMomentum(const pat::Muon& muon) const;
      TLorentzVector lorentzMomentum(const reco::PFCandidate& pfcl) const;
      TLorentzVector lorentzMomentumPi(const reco::Track& tr) const;
      TLorentzVector lorentzMomentumKa(const reco::Track& tr) const;
      TLorentzVector lorentzTriObj(const trigger::TriggerObject& muon) const;
      double invMass(const reco::Track& lhs, const reco::Track& rhs) const;

      string theOutFileName;               // Filename
      int theOniaType;                     // 443 for Jpsi, 553 for Upsilon, etc
      int theDebugLevel;                   // 0 no prints, 1 some, 2 lots
      edm::InputTag thegenParticlesLabel;
      edm::InputTag theStandAloneMuonsLabel;      // Muon information from standalone muons
      edm::InputTag theGlobalMuonsLabel;          // Muon information from global muons
      edm::InputTag theMuonsLabel;                // From this one can get both global and track info
      edm::InputTag theCaloMuonsLabel;             
      edm::InputTag theTrackLabel;                // Track information
      edm::InputTag thePhotonLabel;               // Photon information
      edm::InputTag theBeamSpotLabel;            // Beam Spot
      edm::InputTag thePrimaryVertexLabel;        // Primary Vertex
      edm::InputTag thetriggerEventLabel;
      edm::InputTag theL1GTReadoutRec;            // L1 trigger results
      edm::InputTag theHLTriggerResults;          // HLT trigger results
      edm::InputTag theL1MuonLabel;
      edm::InputTag thePATMuonsLabel;      

      bool theStoreGenFlag;                // Yes or No to store generator information
      bool theStoreHLTFlag;                // Yes or No to store HLT information
      bool theStoreL1Flag;                 // Yes or No to store L1 information
      bool theStoreTrkFlag;                // Yes or No to store track information (e.g. for hadr. act.)
      bool theStorePhotFlag;                // Yes or No to store photon information (e.g. for Chi_c)

      bool theStoreSTAMuonFlag;
      bool theStoreGLBMuonFlag;
      bool theStoreAllMuonFlag;
      bool theStoreBeamSpotFlag; 
      bool theStorePriVtxFlag;             // Yes or No to store primary vertex
      bool theStoreOniaFlag;               // Yes or No to store Onium info
      bool theStoreChicFlag;               // Yes or No to store Onium + gamma combinations
      bool theStoreBpFlag;                 // Yes or No to store Onium + track combinations
      bool theStoreOniaRadiation;          // Yes or No to study Onium radiation
      bool theStoreWSOnia;                 // Yes or No to store wrong-sign mu-mu combinations
      bool theBeamSpotFlag;
      bool theminimumFlag;
      bool theAODFlag;
      bool theStorePATFlag;

      TFile* outFile;
      TTree *fTree;
      double oniaMass;
      double branch_ratio;      
      
      static const int NTRIGGERS = 8;
      int Max_track_size;
      int Max_QQ_size;
      int Max_mu_size;
      int Max_PriVtx_size;
      unsigned int Max_trig_size;

      int Mc_ProcessId;               // Process ID in PYTHIA (e.g. octet vs singlet)
      double Mc_EventScale;           // Pthat of process
      double Mc_EventWeight;          // Weight of event generated

      int Mc_QQ_size;                     // Number of Onia in event (usually 1)
      TClonesArray* Mc_QQ_4mom;       // Array of 4-momentum of Onium
      TClonesArray* Mc_QQ_3vec;       // Array of 3-d creation vertex of Onium
      TClonesArray* Mc_QQmoth_4mom;   // Array of 4-momentum of Onium mother
      TClonesArray* Mc_QQmoth_3vec;   // Array of 3-d creation vertex of Onium mother
      int Mc_QQmoth_id[20];           // particle id of Onium mother 
      int Mc_QQmupl_indx[20];         // Vector of index of mu+ from Onium in muon array
      int Mc_QQmumi_indx[20];         // Vector of index of mu- from Onium in muon array
      int Mc_mu_size;                     // Number of muons in event (usually 2 from 1 Onium)
      TClonesArray* Mc_mu_4mom;       // Array of 4-momentum of muons (all muons, could also be weak)
      TClonesArray* Mc_mu_3vec;       // Array of 3-d creation vertex of muons (all muons, could also be weak)
      int Mc_mu_id[20];               // ID (13 or -13) of muons (all muons, could also be weak)
      int Mc_mumoth_id[20];           // MotherID of muons (all muons, could also be weak)
      int Mc_chargedtrk_size;           // Amount of charged  particles in MC
      TClonesArray* Mc_chargedtrk_4mom; // Array of 4-vector of charged particles in MC
      int Mc_chargedtrk_charge[3000];   // Vector of charge of particles in MC
      
      int Reco_track_size;            // Number of reconstructed tracks
      TClonesArray* Reco_track_4mom;  // Array of 4-momentum of Reconstructed trackss
      TClonesArray* Reco_track_3vec;  // Array of 3-d creation vertex of Reconstructed trackss
      TClonesArray* Reco_track_CovM;  // Array of 5*5 covariance matrix
      double Reco_track_ptErr[3000];  // Vector of err on pt of global muons
      double Reco_track_phiErr[3000]; // Vector of err on phi of global muons
      double Reco_track_etaErr[3000]; // Vector of err on eta of global muons
      double Reco_track_d0[3000];     // Vector of d0 of tracks
      double Reco_track_d0err[3000];  // Vector of d0err of tracks
      double Reco_track_dz[3000];     // Vector of dz of tracks
      double Reco_track_dzerr[3000];  // Vector of dzerr of tracks
      int Reco_track_charge[3000]; // Vector of charge of tracks
      double Reco_track_chi2[3000];   // Vector of chi2 of tracks
      double Reco_track_ndof[3000];   // Vector of ndof of tracks
      int Reco_track_nhits[3000];  // Vector of nhits of tracks

      int Reco_gamma_size;            // Number of reconstructed gammas
      TClonesArray* Reco_gamma_4mom;  // Array of 4-momentum of Reconstructed gammas
      double Reco_gamma_phi[3000];  // phi 
      double Reco_gamma_eta[3000];  // eta

      int Reco_mu_size;
      int Reco_mu_Normsize;
      int Reco_mu_Calmsize;
      int Reco_mu_links_glb[200];
      int Reco_mu_links_sta[200];
      int Reco_mu_links_trk[200];
      bool Reco_mu_is_sta[200];
      bool Reco_mu_is_glb[200];
      bool Reco_mu_is_trk[200];
      bool Reco_mu_is_cal[200];
      double Reco_mu_caloComp[200];

      int Reco_mu_glb_size;           // Number of reconstructed global muons
      TClonesArray* Reco_mu_glb_4mom; // Array of 4-momentum of Reconstructed global muons
      TClonesArray* Reco_mu_glb_3vec; // Array of 3-d creation vertex of Reconstructed global muons
      TClonesArray* Reco_mu_glb_CovM;
      double Reco_mu_glb_ptErr[200];   // Vector of err on pt of global muons
      double Reco_mu_glb_phiErr[200];  // Vector of err on phi of global muons
      double Reco_mu_glb_etaErr[200];  // Vector of err on eta of global muons
      double Reco_mu_glb_d0[200];      // Vector of d0 of global muons
      double Reco_mu_glb_d0err[200];   // Vector of d0err of global muons
      double Reco_mu_glb_dz[200];      // Vector of dz of global muons
      double Reco_mu_glb_dzerr[200];   // Vector of dzerr of global muons
      int Reco_mu_glb_charge[200];  // Vector of charge of global muons
      double Reco_mu_glb_chi2[200];   // Vector of chi2 of global muons
      double Reco_mu_glb_ndof[200];   // Vector of ndof of global muons
      int Reco_mu_glb_nhits[200];  // Vector of number of valid hits of global muons


/////////////////// PAT muons
      
      int Pat_mu_glb_size;           // Number of reconstructed global muons
      TClonesArray* Pat_mu_glb_4mom; // Array of 4-momentum of Reconstructed global muons
      TClonesArray* Pat_mu_glb_3vec; // Array of 3-d creation vertex of Reconstructed global muons
      TClonesArray* Pat_mu_glb_CovM;
      double Pat_mu_glb_ptErr[200];   // Vector of err on pt of global muons
      double Pat_mu_glb_phiErr[200];  // Vector of err on phi of global muons
      double Pat_mu_glb_etaErr[200];  // Vector of err on eta of global muons
      double Pat_mu_glb_d0[200];      // Vector of d0 of global muons
      double Pat_mu_glb_d0err[200];   // Vector of d0err of global muons
      double Pat_mu_glb_dz[200];      // Vector of dz of global muons
      double Pat_mu_glb_dzerr[200];   // Vector of dzerr of global muons
      int Pat_mu_glb_charge[200];  // Vector of charge of global muons
      double Pat_mu_glb_chi2[200];   // Vector of chi2 of global muons
      double Pat_mu_glb_ndof[200];   // Vector of ndof of global muons
      int Pat_mu_glb_nhits[200];  // Vector of number of valid hits of global muons
      
      int Pat_mu_sta_size;           // Number of reconstructed stand alone muons
      TClonesArray* Pat_mu_sta_4mom; // Array of 4-momentum of Reconstructed stand alone muons
      TClonesArray* Pat_mu_sta_3vec; // Array of 3-d creation vertex of Reconstructed stand alone muons
      TClonesArray* Pat_mu_sta_CovM;
      double Pat_mu_sta_ptErr[200];   // Vector of err on pt of stand alone muons
      double Pat_mu_sta_phiErr[200];  // Vector of err on phi of stand alone muons
      double Pat_mu_sta_etaErr[200];  // Vector of err on eta of stand alone muons
      double Pat_mu_sta_d0[200];      // Vector of d0 of stand alone muons
      double Pat_mu_sta_d0err[200];   // Vector of d0err of stand alone muons
      double Pat_mu_sta_dz[200];      // Vector of dz of stand alone muons
      double Pat_mu_sta_dzerr[200];   // Vector of dzerr of stand alone muons
      int Pat_mu_sta_charge[200];  // Vector of charge of stand alone muons
      double Pat_mu_sta_chi2[200];   // Vector of chi2 of stand alone muons
      double Pat_mu_sta_ndof[200];   // Vector of ndof of stand alone muons
      int Pat_mu_sta_nhits[200];  // Vector of number of valid hits of stand alone muons
      
      int Pat_mu_trk_size;           // Number of reconstructed tracker muons
      TClonesArray* Pat_mu_trk_4mom; // Array of 4-momentum of Reconstructed tracker muons
      TClonesArray* Pat_mu_trk_3vec; // Array of 3-d creation vertex of Reconstructed tracker muons
      TClonesArray* Pat_mu_trk_CovM;
      double Pat_mu_trk_ptErr[200];   // Vector of err on pt of tracker muons
      double Pat_mu_trk_phiErr[200];  // Vector of err on phi of tracker muons
      double Pat_mu_trk_etaErr[200];  // Vector of err on eta of tracker muons
      double Pat_mu_trk_d0[200];      // Vector of d0 of tracker muons
      double Pat_mu_trk_d0err[200];   // Vector of d0err of tracker muons
      double Pat_mu_trk_dz[200];      // Vector of dz of tracker muons
      double Pat_mu_trk_dzerr[200];   // Vector of dzerr of tracker muons
      int Pat_mu_trk_charge[200];  // Vector of charge of tracker muons
      double Pat_mu_trk_chi2[200];   // Vector of chi2 of tracker muons
      double Pat_mu_trk_ndof[200];   // Vector of ndof of tracker muons
      int Pat_mu_trk_nhits[200];  // Vector of number of valid hits of tracker muons
      
      int Pat_mu_cal_size;           // Number of reconstructed calorimeter muons
      TClonesArray* Pat_mu_cal_4mom; // Array of 4-momentum of Reconstructed calorimeter muons
      TClonesArray* Pat_mu_cal_3vec; // Array of 3-d creation vertex of Reconstructed calorimeter muons
      TClonesArray* Pat_mu_cal_CovM;
      double Pat_mu_cal_ptErr[200];   // Vector of err on pt of calorimeter muons
      double Pat_mu_cal_phiErr[200];  // Vector of err on phi of calorimeter muons
      double Pat_mu_cal_etaErr[200];  // Vector of err on eta of calorimeter muons
      double Pat_mu_cal_d0[200];      // Vector of d0 of calorimeter muons
      double Pat_mu_cal_d0err[200];   // Vector of d0err of calorimeter muons
      double Pat_mu_cal_dz[200];      // Vector of dz of calorimeter muons
      double Pat_mu_cal_dzerr[200];   // Vector of dzerr of calorimeter muons
      int Pat_mu_cal_charge[200];  // Vector of charge of calorimeter muons
      double Pat_mu_cal_chi2[200];   // Vector of chi2 of calorimeter muons
      double Pat_mu_cal_ndof[200];   // Vector of ndof of calorimeter muons
      int Pat_mu_cal_nhits[200];  // Vector of number of valid hits of calorimeter muons             
                  
//////////////////////////////////////////      

      int Reco_mu_sta_size;           // Number of reconstructed standalone muons
      TClonesArray* Reco_mu_sta_4mom; // Array of 4-momentum of Reconstructed standalone muons
      TClonesArray* Reco_mu_sta_3vec; // Array of 3-d creation vertex of Reconstructed standalone muons
      TClonesArray* Reco_mu_sta_CovM;
      double Reco_mu_sta_ptErr[200];   // Vector of err on pt of standalone muons
      double Reco_mu_sta_phiErr[200];  // Vector of err on phi of standalone muons
      double Reco_mu_sta_etaErr[200];  // Vector of err on eta of standalone muons
      double Reco_mu_sta_d0[200];      // Vector of d0 of standalone muons
      double Reco_mu_sta_d0err[200];   // Vector of d0err of standalone muons
      double Reco_mu_sta_dz[200];      // Vector of dz of standalone muons
      double Reco_mu_sta_dzerr[200];   // Vector of dzerr of standalone muons
      int Reco_mu_sta_charge[200];   // Vector of charge of standalone muons
      double Reco_mu_sta_chi2[200];   // Vector of chi2 of standalone muons
      double Reco_mu_sta_ndof[200];   // Vector of ndof of standalone muons
      int Reco_mu_sta_nhits[200];  // Vector of number of valid hits of standalone muons

      int Reco_QQ_size;           // Number of reconstructed Onia 
      int Reco_QQ_type[3000];     // Onia category:
                                  // 0 = golden (2 global muons)
                                  // 1 = silver (1 global - 1 tracker muon)
                                  // 2 = bronze (2 tracker muons)
                                  // 3 = copper (1 global - 1 calo muon)
                                  // 4 = iron (1 tracker - 1 calo muon)
                                  // 5 = crap (2 calo muons)
      int Reco_QQ_sign[3000];     // Mu Mu combinations sign:
                                  // 0 = +/- (signal)
                                  // 1 = +/+
                                  // 2 = -/-
      TClonesArray* Reco_QQ_4mom; // Array of 4-momentum of Reconstructed onia
      int Reco_QQ_mupl[3000];       // Index of muon plus in onia 
      int Reco_QQ_mumi[3000];       // Index of muon minus in onia
      double Reco_QQ_DeltaR[3000];  // DeltaR of the two muons
      double Reco_QQ_cosTheta[3000];// Polarization angle 
      double Reco_QQ_s[3000];       // S : the sum of the to muons impact parameter significance 
      bool Reco_QQ_VtxIsVal[3000];  // Vertex is valid or not  
      TClonesArray* Reco_QQ_Vtx;  // 3-d Vertex 
      double Reco_QQ_VxE[3000];     // errors of x
      double Reco_QQ_VyE[3000];     // errors of y
      double Reco_QQ_VzE[3000];     // errors of z
      double Reco_QQ_lxy[3000];     // Decay length
      double Reco_QQ_lxyErr[3000];  // Decay length errors
      double Reco_QQ_normChi2[3000];// Normalized chi2 of vertex fitting 
      double Reco_QQ_cosAlpha[3000];// Alpha: the angle of lxy and onia moemtum
      double Reco_QQ_ctau[3000];    // ctau: flying time

      int Reco_Chic_size;           // Number of reconstructed chi_c
      TClonesArray* Reco_Chic_4mom; // Array of 4-momentum of Reconstructed chi_c
      int Reco_Chic_OniaDaug[3000];   // Index of onia in chi_c 
      int Reco_Chic_GammaDaug[3000];  // Index of gamma in chi_c
      double Reco_Chic_DeltaM[3000];  // M(onia-gamma) - M(onia)
      
      int Reco_Bp_size;           // Number of reconstructed Onia from global muons
      TClonesArray* Reco_Bp_4mom; // Array of 4-momentum of Reconstructed Bp
      int Reco_Bp_OniaDaug[3000];   // Index of onia in Bp 
      int Reco_Bp_KDaug[3000];      // Index of kaon in Bp
      bool Reco_Bp_VtxIsVal[3000];  // Vertex is valid or not  
      TClonesArray* Reco_Bp_Vtx;  // 3-d Vertex 
      double Reco_Bp_VxE[3000];     // errors of x
      double Reco_Bp_VyE[3000];     // errors of y
      double Reco_Bp_VzE[3000];     // errors of z
      double Reco_Bp_lxy[3000];     // Decay length
      double Reco_Bp_lxyErr[3000];  // Decay length errors
      double Reco_Bp_normChi2[3000];// Normalized chi2 of vertex fitting 
      double Reco_Bp_cosAlpha[3000];// Alpha: the angle of lxy and onia moemtum
      double Reco_Bp_ctau[3000];    // ctau: flying time

      double Reco_BeamSpot_x;
      double Reco_BeamSpot_y;
      double Reco_BeamSpot_z;
      double Reco_BeamSpot_xE;
      double Reco_BeamSpot_yE;
      double Reco_BeamSpot_zE;

      int Reco_PriVtx_size;                // Number of reconstructed primary vertex
      TClonesArray* Reco_PriVtx_3vec;      // 3-d vector of primary vertex
      double Reco_PriVtx_xE[100];           // X error
      double Reco_PriVtx_yE[100];           // Y error
      double Reco_PriVtx_zE[100];           // Z error
      int Reco_PriVtx_trkSize[100];      // Number of tracks in this primary vertex
      double Reco_PriVtx_chi2[100];         // chi2 of primary vertex 
      double Reco_PriVtx_ndof[100];         // number of freedom degree 

      int Reco_PriVtx_1st_trkSize;        // Number of tracks in the first primary vertex
      int Reco_PriVtx_1st_trkindex[3000]; //index for the tracks in the first primary vertex

      int L1TBits_size;               // Number of L1 trigger bits
      bool L1TBits_accept[3000];       // L1 trigger bits
      bool L1TGlobal_Decision;        // L1 trigger global decision
    
      int L1_mu_size;
      TClonesArray* L1_mu_4mom;
      int L1_mu_charge[200];

      int HLTBits_size;               // Number of HLT trigger bits 
      bool HLTBits_wasrun[1000];       // Each HLT bits was run or not
      bool HLTBits_accept[1000];       // Each HLT bits fired or not
      bool HLTBits_error[1000];        // Each HLT bits run successfully or not 
      bool HLTGlobal_wasrun;          // The HLT was run or not
      bool HLTGlobal_Decision;        // Global HLT decision
      bool HLTGlobal_error;           // HLT path error or not
  
      int HLT1Mu3_L2_size;
      TClonesArray* HLT1Mu3_L2_4mom;
      int HLT1Mu3_L2_id[20];
      int HLT1Mu3_L3_size;
      TClonesArray* HLT1Mu3_L3_4mom;
      int HLT1Mu3_L3_id[20];

      int HLT1Mu5_L2_size;
      TClonesArray* HLT1Mu5_L2_4mom;
      int HLT1Mu5_L2_id[20];
      int HLT1Mu5_L3_size;
      TClonesArray* HLT1Mu5_L3_4mom;
      int HLT1Mu5_L3_id[20];

      int HLT1Mu9_L2_size;
      TClonesArray* HLT1Mu9_L2_4mom;
      int HLT1Mu9_L2_id[20];
      int HLT1Mu9_L3_size;
      TClonesArray* HLT1Mu9_L3_4mom;
      int HLT1Mu9_L3_id[20];

      int HLT1Mu11_L2_size;
      TClonesArray* HLT1Mu11_L2_4mom;
      int HLT1Mu11_L2_id[20];
      int HLT1Mu11_L3_size;
      TClonesArray* HLT1Mu11_L3_4mom;
      int HLT1Mu11_L3_id[20];

      int HLTI2Mu3_L2_size;
      TClonesArray* HLTI2Mu3_L2_4mom;
      int HLTI2Mu3_L2_id[20];
      int HLTI2Mu3_L3_size;
      TClonesArray* HLTI2Mu3_L3_4mom;
      int HLTI2Mu3_L3_id[20];

      int HLTNoI2Mu3_L2_size;
      TClonesArray* HLTNoI2Mu3_L2_4mom;
      int HLTNoI2Mu3_L2_id[20];
      int HLTNoI2Mu3_L3_size;
      TClonesArray* HLTNoI2Mu3_L3_4mom;
      int HLTNoI2Mu3_L3_id[20];

      int HLTJpsi2Mu_L2_size;
      TClonesArray* HLTJpsi2Mu_L2_4mom;
      int HLTJpsi2Mu_L2_id[20];
      int HLTJpsi2Mu_L3_size;
      TClonesArray* HLTJpsi2Mu_L3_4mom;
      int HLTJpsi2Mu_L3_id[20];
      int HLTUpsilon2Mu_L2_size;
      TClonesArray* HLTUpsilon2Mu_L2_4mom;
      int HLTUpsilon2Mu_L2_id[20];
      int HLTUpsilon2Mu_L3_size;
      TClonesArray* HLTUpsilon2Mu_L3_4mom;
      int HLTUpsilon2Mu_L3_id[20];

      InputTag hltModules[2][NTRIGGERS]; // in order: L2, L3
      std::vector<std::string> hltPaths;
      int hltBits[NTRIGGERS];

      HLTConfigProvider hltConfig;
      edm::ESHandle<TransientTrackBuilder> theB;
      edm::Handle<reco::PFCandidateCollection> pfAll;
      reco::PFCandidateCollection pfClusters;
      edm::Handle<TrackCollection> allTracks;
 
      unsigned int fNevt;            // event number
      unsigned int maxCatToStoreChic;
      unsigned int maxCatToStoreBp;
};

#endif
