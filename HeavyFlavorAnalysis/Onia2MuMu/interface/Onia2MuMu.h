#ifndef Onia2MuMu_h
#define Onia2MuMu_h

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
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
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/HepMCProduct/interface/GenInfoProduct.h"
#include "FWCore/Framework/interface/Run.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"

#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/Common/interface/TriggerResults.h"


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
      virtual void fillMuons(const edm::Event &iEvent);
      virtual void fillPrimaryVertex(const edm::Event &iEvent);
      virtual void fillOniaMuMuTracks(const edm::Event &iEvent, const edm::EventSetup&,string OniaMuonType);

      double PhiInRange(const double& phi) const;
      template <class T, class U> double deltaR(const T & t, const U & u) const;
      double calcDeltaR(double eta1, double phi1,double eta2, double phi2);
      double GetTheta( TLorentzVector & a,  TLorentzVector & b) const;
      void printTrack(const reco::Track& muon) const;
      TLorentzVector lorentzMomentum(const reco::Track& muon) const;
      float invMass(const reco::Track& lhs, const reco::Track& rhs) const;

      string theOutFileName;               // Filename
      int theOniaType;                     // 443 for Jpsi, 553 for Upsilon, etc
      int theDebugLevel;                   // 0 no prints, 1 some, 2 lots
      string theStandAloneMuonsLabel;      // Muon information from standalone muons
      string theTrackerMuonsLabel;         // Muon information from tracker muons // does not work
      string theGlobalMuonsLabel;          // Muon information from global muons
      string theMuonsLabel;                // From this one can get both global and track info
      string theTrackLabel;                // Track information
      string theOniaMuonsLabel;            // From this one we reconstruct dimuons
      string thePrimaryVertexLabel;        // Primary Vertex
      string theL1ParticleMap;             // L1 trigger bits
      string theL1GTReadoutRec;            // L1 trigger results
      string theHLTriggerResults;          // HLT trigger results
      bool theStoreGenFlag;                // Yes or No to store generator information
      bool theStoreHLTFlag;                // Yes or No to store HLT information
      bool theStoreL1Flag;                 // Yes or No to store L1 information
      bool theStoreTrkFlag;                // Yes or No to store track information (e.g. for hadr. act.)
      bool theStoreMuonFlag;               // Yes or No to store all muons
      bool theStorePriVtxFlag;             // Yes or No to store primary vertex
      bool theStoreOniaFlag;               // Yes or No to store Onium info 
      bool theStoreOniaRadiation;          // Yes or No to study Onium radiation
      TFile* outFile;
      TTree *fTree;
      double oniaMass;
      double branch_ratio;      
      
      
      int Max_track_size;
      int Max_QQ_size;
      int Max_mu_size;
      int Max_PriVtx_size;
      int Max_trig_size;

      int Mc_QQ_size;                     // Number of Onia in event (usually 1)
      TClonesArray* Mc_QQ_4mom;       // Array of 4-momentum of Onium
      TClonesArray* Mc_QQ_3vec;       // Array of 3-d creation vertex of Onium
      TClonesArray* Mc_QQmoth_4mom;   // Array of 4-momentum of Onium mother
      TClonesArray* Mc_QQmoth_3vec;   // Array of 3-d creation vertex of Onium mother
      int Mc_QQmupl_indx[10];         // Vector of index of mu+ from Onium in muon array
      int Mc_QQmumi_indx[10];         // Vector of index of mu- from Onium in muon array
      int Mc_mu_size;                     // Number of muons in event (usually 2 from 1 Onium)
      TClonesArray* Mc_mu_4mom;       // Array of 4-momentum of muons (all muons, could also be weak)
      TClonesArray* Mc_mu_3vec;       // Array of 3-d creation vertex of muons (all muons, could also be weak)
      int Mc_mu_id[10];               // ID (13 or -13) of muons (all muons, could also be weak)
      int Mc_mumoth_id[10];           // MotherID of muons (all muons, could also be weak)
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
      double Reco_track_d0err[3000];  // Vector of d0 of tracks
      double Reco_track_dz[3000];     // Vector of dz of tracks
      double Reco_track_dzerr[3000];  // Vector of dz of tracks
      double Reco_track_charge[3000]; // Vector of charge of tracks
      double Reco_track_chi2[3000];   // Vector of chi2 of tracks
      double Reco_track_ndof[3000];   // Vector of ndof of tracks
      double Reco_track_nhits[3000];  // Vector of nhits of tracks


      int Reco_track_muflag[3000];    // Vector denoting of  if this track couldhave been a muon (0 =no muon, 1 =muon)
      int Reco_mu_glb_size;           // Number of reconstructed global muons
      TClonesArray* Reco_mu_glb_4mom; // Array of 4-momentum of Reconstructed global muons
      TClonesArray* Reco_mu_glb_3vec; // Array of 3-d creation vertex of Reconstructed global muons
      TClonesArray* Reco_mu_glb_CovM;
      double Reco_mu_glb_ptErr[10];   // Vector of err on pt of global muons
      double Reco_mu_glb_phiErr[10];  // Vector of err on phi of global muons
      double Reco_mu_glb_etaErr[10];  // Vector of err on eta of global muons
      double Reco_mu_glb_d0[10];      // Vector of d0 of global muons
      double Reco_mu_glb_d0err[10];   // Vector of d0err of global muons
      double Reco_mu_glb_dz[10];      // Vector of dz of global muons
      double Reco_mu_glb_dzerr[10];   // Vector of dzerr of global muons
      double Reco_mu_glb_charge[10];  // Vector of charge of global muons
      double Reco_mu_glb_chi2[10];   // Vector of chi2 of global muons
      double Reco_mu_glb_ndof[10];   // Vector of ndof of global muons
      double Reco_mu_glb_nhits[10];  // Vector of number of valid hits of global muons
      
      int Reco_mu_sta_size;           // Number of reconstructed standalone muons
      TClonesArray* Reco_mu_sta_4mom; // Array of 4-momentum of Reconstructed standalone muons
      TClonesArray* Reco_mu_sta_3vec; // Array of 3-d creation vertex of Reconstructed standalone muons
      TClonesArray* Reco_mu_sta_CovM;
      double Reco_mu_sta_ptErr[10];   // Vector of err on pt of standalone muons
      double Reco_mu_sta_phiErr[10];  // Vector of err on phi of standalone muons
      double Reco_mu_sta_etaErr[10];  // Vector of err on eta of standalone muons
      double Reco_mu_sta_d0[10];      // Vector of d0 of standalone muons
      double Reco_mu_sta_d0err[10];   // Vector of d0err of standalone muons
      double Reco_mu_sta_dz[10];      // Vector of dz of standalone muons
      double Reco_mu_sta_dzerr[10];   // Vector of dzerr of standalone muons
      double Reco_mu_sta_charge[10];   // Vector of charge of standalone muons
      double Reco_mu_sta_chi2[10];   // Vector of chi2 of standalone muons
      double Reco_mu_sta_ndof[10];   // Vector of ndof of standalone muons
      double Reco_mu_sta_nhits[10];  // Vector of number of valid hits of standalone muons
      

      int Reco_mu_trk_size;           // Number of reconstructed tracker muons
      TClonesArray* Reco_mu_trk_4mom; // Array of 4-momentum of Reconstructed tracker muons
      TClonesArray* Reco_mu_trk_3vec; // Array of 3-d creation vertex of Reconstructed tracker muons
      TClonesArray* Reco_mu_trk_CovM;
      double Reco_mu_trk_ptErr[10];   // Vector of err on pt of tracker muons
      double Reco_mu_trk_phiErr[10];  // Vector of err on phi of tracker muons
      double Reco_mu_trk_etaErr[10];  // Vector of err on eta of tracker muons
      double Reco_mu_trk_d0[10];      // Vector of d0 of tracker muons
      double Reco_mu_trk_d0err[10];   // Vector of d0err of tracker muons
      double Reco_mu_trk_dz[10];      // Vector of dz of tracker muons
      double Reco_mu_trk_dzerr[10];   // Vector of dzerr of tracker muons
      double Reco_mu_trk_charge[10];   // Vector of charge of tracker muons
      double Reco_mu_trk_chi2[10];   // Vector of chi2 of tracker muons
      double Reco_mu_trk_ndof[10];   // Vector of ndof of tracker muons
      double Reco_mu_trk_nhits[10];  // Vector of number of valid hits of tracker muons
      


      int Reco_QQ_glb_size;           // Number of reconstructed Onia from global muons
      TClonesArray* Reco_QQ_glb_4mom; // Array of 4-momentum of Reconstructed onia
      int Reco_QQ_glb_mupl[10];       // Index of muon plus in onia 
      int Reco_QQ_glb_mumi[10];       // Index of muon minus in onia
      double Reco_QQ_glb_DeltaR[10];  // DeltaR of the two muons
      double Reco_QQ_glb_cosTheta[10];// Polarization angle 
      double Reco_QQ_glb_s[10];       // S : the sum of the to muons impact parameter significance 
      bool Reco_QQ_glb_VtxIsVal[10];// Vertex is valid or not  
      TClonesArray* Reco_QQ_glb_Vtx;  // 3-d Vertex 
      double Reco_QQ_glb_VtxE[10][3]; // errors of x, y, z
      double Reco_QQ_glb_lxy[10];     // Decay length
      double Reco_QQ_glb_lxyErr[10];  // Decay length errors
      double Reco_QQ_glb_normChi2[10];// Normalized chi2 of vertex fitting 
      double Reco_QQ_glb_cosAlpha[10];// Alpha: the angle of lxy and onia moemtum
      double Reco_QQ_glb_ctau[10];    // ctau: flying time

      int Reco_PriVtx_size;                // Number of reconstructed primary vertex
      TClonesArray* Reco_PriVtx_3vec;      // 3-d vector of primary vertex
      double Reco_PriVtx_xE[10];           // X error
      double Reco_PriVtx_yE[10];           // Y error
      double Reco_PriVtx_zE[10];           // Z error
      int Reco_PriVtx_trkSize[10];      // Number of tracks in this primary vertex
      double Reco_PriVtx_chi2[10];         // chi2 of primary vertex 
      double Reco_PriVtx_ndof[10];         // number of freedom degree 

      int L1TBits_size;               // Number of L1 trigger bits
      bool L1TBits_accept[200];       // L1 trigger bits
      bool L1TGlobal_Decision;        // L1 trigger global decision
 
      int HLTBits_size;               // Number of HLT trigger bits 
      bool HLTBits_wasrun[200];       // Each HLT bits was run or not
      bool HLTBits_accept[200];       // Each HLT bits fired or not
      bool HLTBits_error[200];        // Each HLT bits run successfully or not 
      bool HLTGlobal_wasrun;          // The HLT was run or not
      bool HLTGlobal_Decision;        // Global HLT decision
      bool HLTGlobal_error;           // HLT path error or not
 

      unsigned int  fNevt;            // event number
      
      int Mc_ProcessId;               // Process ID in PYTHIA (e.g. octet vs singlet)
      double Mc_EventScale;           // Pthat of process
      double Mc_EventWeight;          // Weight of event generated

      
     
      
};

#endif
