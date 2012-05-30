// system include files
#include <memory>
#include <iostream>
#include <string>
#include <vector>

// user include files
//#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
//#include <CommonTools/UtilAlgos/interface/TFileService.h>

#include "RecoJets/JetProducers/interface/JetIDHelper.h"

//For HggVertexAnalysis
#include "Analysis/VertexAnalysis/interface/HggVertexAnalyzer.h"
#include "Analysis/VertexAnalysis/interface/HggVertexFromConversions.h"
#include "Analysis/VertexAnalysis/interface/VertexAlgoParameters.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

// for jetID
#include "CMGTools/External/interface/PileupJetIdAlgo.h"
#include "CMGTools/External/interface/PileupJetIdentifier.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"

#include "TH1.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"

#include "TMVA/Reader.h"

#include <map>
#include <set>
class SimTrack;
class SimVertex;

#define MAXHLTBITS    5000

using namespace edm;
using namespace std;
using namespace reco;

#include "HiggsAnalysis/HiggsTo2photons/interface/CiCPhotonID.h"

//
// class declaration
//

class EGEnergyCorrector;

class GammaJetAnalyzer : public edm::EDAnalyzer {
   public:
      explicit GammaJetAnalyzer(const edm::ParameterSet&);
      ~GammaJetAnalyzer();


   private:
      virtual void beginJob();
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      std::vector<float> getESHits(double X, double Y, double Z, std::map<DetId, EcalRecHit> rechits_map, const CaloGeometry& geometry, CaloSubdetectorTopology *topology_p, int row=0);
      std::vector<float> getESShape(std::vector<float> ESHits0);
      
      //  calculate phi1-phi2 keeping value between 0 and pi
      inline float delta_phi(float phi1, float phi2);
      // calculate eta1-eta2 keeping eta2 positive
      inline float delta_eta(float eta1, float eta2);
      // calculate sum in quadrature
      inline double oplus(double a, double b);
      // fix EMF in HF
      inline double fixEMF(double emf, double eta);

      inline float recHitE( const  DetId id,  const EcalRecHitCollection &recHits );

      inline float recHitE( const DetId id, const EcalRecHitCollection & recHits,int di, int dj );

      inline float recHitApproxEt(  const DetId id,  const EcalRecHitCollection &recHits );


      float GetE2OverE9( const DetId id, const EcalRecHitCollection & recHits);

      // Method for iterative printing of decay chains
      bool printChildren(const SimTrack* p, 
	 std::map<const SimTrack*, std::set<const SimTrack*> > const& ptokids,
	 std::map<const SimTrack*, const SimVertex*> const& ptovtx,
	 int level, bool save);
      // Remove unneeded SimTracks from tables
      bool pruneKids(const SimTrack* p,
         std::map<const SimTrack*, std::set<const SimTrack*> > & decays,
	 std::map<const SimTrack*, const SimTrack*> & parent,
	 std::map<const SimTrack*, const SimVertex*> & vertex,
	 int level);
      // Constants
      static const int kParton = 3;
      static const int kPhoton = 22;
      static const int kElectron = 11;


      // ----------member data ---------------------------
      bool _debug;
      
      //To compute additional pid variables
      CiCPhotonID* cicPhotonId;

TH1F* h1_hbherh_detid;
TH1F* h1_etaPhot;
TH2D* h2_n_vs_eta;
      edm::InputTag puSummaryInfo_;
      edm::InputTag MCTruthCollection_; 
      edm::InputTag triggerTag_;
      edm::InputTag Vertexsrc_;
      edm::InputTag Photonsrc_; 
      edm::InputTag Conversionsrc_; 
      edm::InputTag Electronsrc_; 
      edm::InputTag Jetsrcite_; 
      edm::InputTag Jetsrckt4_; 
      edm::InputTag Jetsrckt6_; 
      edm::InputTag Jetsrcakt5_; 
      edm::InputTag Jetsrcakt7_; 
      edm::InputTag Jetsrcsis5_; 
      edm::InputTag Jetsrcsis7_; 
      edm::InputTag JetJPTsrcak5_;
      edm::InputTag JetPFsrcite_;
      edm::InputTag JetPFsrckt4_;
      edm::InputTag JetPFsrcakt5_;
      edm::InputTag JetPFsrcakt7_;
      edm::InputTag JetPFsrcsis5_;
      edm::InputTag JetPFsrckt6_;
      edm::InputTag JetPFsrcsis7_;
      edm::InputTag JetGensrcite_; 
      edm::InputTag JetGensrckt4_; 
      edm::InputTag JetGensrckt6_; 
      edm::InputTag JetGensrcakt5_; 
      edm::InputTag JetGensrcakt7_; 
      edm::InputTag JetGensrcsis5_; 
      edm::InputTag JetGensrcsis7_; 
      edm::InputTag METsrc_; 
      edm::InputTag METGensrc_; 
      edm::InputTag trackTags_; 
      edm::InputTag HBhitsrc_; 
      string recoCollection_; 
      string recoProducer_; 
      string JetCorrector_akt5_; 
      string JetCorrector_akt7_; 
      string JetCorrector_jptak5_; 
      string JetCorrector_pfakt5_; 
      string JetCorrector_pfakt7_; 
      double genjetptthr_;
      double calojetptthr_;
      double pfjetptthr_;
      double jptjetptthr_;
      int genjetnmin_;
      int pfjetnmin_;
      int jptjetnmin_;
      double Xsec_;

//      edm::Service<TFileService> fs_;
      TFile* outfile;

      // Tree with multiple info
      TTree * m_tree ;

      reco::helper::JetIDHelper *jetID_;

      // Auxiliary event info will help to study correction stability
      // for different stores, as a function of instantaneous lumi (pile-up),
      // bunch-crossing (out-of-time pile-up), orbit number (beam heating) etc.
      Bool_t isMC;
      Int_t store;
      Int_t lbn;
      Int_t bx;
      Int_t orbit;
      Int_t run;
      Int_t event;

      // Vertex distribution
      Int_t nvertex;
      Float_t vx[100];
      Float_t vy[100];
      Float_t vz[100];
      Float_t vntracks[100];
      Float_t vchi2[100];
      Float_t vndof[100];
      Float_t vlogsumpt2[100];

      //Best vertex for each gammagamma Hypothesis
      Int_t nPreselPhotonPairs;
      Int_t indexPreselPhot1[20];
      Int_t indexPreselPhot2[20];
      Int_t vrankPhotonPairs[20];
      Float_t vevtMvaPhotonPairs[20];
      Float_t vevtProbPhotonPairs[20];
      Float_t vptbalPhotonPairs[20];
      Float_t vptasymPhotonPairs[20];

      
      // Vertex distribution at MC truth level
      Float_t vxMC;
      Float_t vyMC;
      Float_t vzMC;

      // MC particles help to reconstruct original jet parton,
      // particle jet and jet properties (fake photon from pi0, rho0?)
      static const int nMaxMC = 150;//100;
      Int_t nMC;
      Int_t pdgIdMC[nMaxMC];
      Int_t statusMC[nMaxMC];
      //Float_t massMC[nMaxMC];
      Int_t motherIDMC[nMaxMC];
      Float_t ptMC[nMaxMC];
      Float_t eMC[nMaxMC];
      Float_t etaMC[nMaxMC];
      Float_t phiMC[nMaxMC];

      Float_t genpt;
      Int_t genProcessId;
      Float_t genQScale;

      Int_t nPhot;
      Float_t ptPhot[40];
      Float_t ePhot[40];
      Float_t eseedPhot[40];
      Float_t escPhot[40];
      Float_t escRegrPhot[40];
      Float_t escRegrPhotError[40];
      Float_t escPhFixPhot[40];
      Float_t escPhFixPhotError[40];
      Float_t escRawPhot[40];
      Float_t etaPhot[40];
      Float_t phiPhot[40];
      Float_t etascPhot[40];
      Float_t phiscPhot[40];
      Float_t xscPhot[40];
      Float_t yscPhot[40];
      Float_t zscPhot[40];
      Float_t xcaloPhot[40];
      Float_t ycaloPhot[40];
      Float_t zcaloPhot[40];
      Float_t timePhot[40];
      Float_t e4SwissCrossPhot[40];
      Int_t hasPixelSeedPhot[40];
      Int_t hasMatchedConvPhot[40];
      Int_t hasMatchedPromptElePhot[40];
      //For ConvertedPhotons
      Bool_t isValidVtxConvPhot[40];
      Int_t nTracksConvPhot[40];
      Float_t pairInvariantMassConvPhot[40];
      Float_t pairCotThetaSeparationConvPhot[40];
      Float_t pairMomentum_xConvPhot[40];
      Float_t pairMomentum_yConvPhot[40];
      Float_t pairMomentum_zConvPhot[40];
      Float_t chi2ConvPhot[40];
      Float_t nDofConvPhot[40];
      Float_t eOverPConvPhot[40];
      Float_t conv_vxConvPhot[40];
      Float_t conv_vyConvPhot[40];
      Float_t conv_vzConvPhot[40];
      Float_t distOfMinimumApproachConvPhot[40];
      Float_t dPhiTracksAtVtxConvPhot[40]; 
/*       Float_t dPhiTracksAtEcalConvPhot[40]; */
/*       Float_t dEtaTracksAtEcalConvPhot[40]; */
      
      bool isEBPhot[40];
      bool isEEPhot[40];
      bool isEBEEGapPhot[40];

      // Default PAT photon ID variables
      Bool_t pid_isEM[40];
      Bool_t pid_isLoose[40];
      Bool_t pid_isTight[40];
      Float_t pid_jurECAL[40]; // jurassic ECAL isolation
      Float_t pid_twrHCAL[40]; // Tower-based HCAL isolation
      Float_t pid_HoverE[40]; // Hadronic / EM
      Float_t pid_hlwTrack[40]; // Hollow cone track isolation
      Float_t pid_etawid[40]; // eta width
      Float_t pid_jurECAL03[40]; // jurassic ECAL isolation
      Float_t pid_twrHCAL03[40]; // Tower-based HCAL isolation
      Float_t pid_hlwTrack03[40]; // Hollow cone track isolation
      Float_t pid_hlwTrack03NoDz[40]; // Hollow cone track isolation
      Float_t pid_hlwTrackNoDz[40]; // Hollow cone track isolation
      Float_t pid_hlwTrackForCiC[40][100]; // Hollow cone track isolation
      Float_t pid_hlwTrack03ForCiC[40][100]; // Hollow cone track isolation

      //PFIsolations

      Float_t pid_pfIsoCharged01ForCiC[40][100]; // Hollow cone track isolation
      Float_t pid_pfIsoCharged02ForCiC[40][100]; // Hollow cone track isolation
      Float_t pid_pfIsoCharged03ForCiC[40][100]; // Hollow cone track isolation
      Float_t pid_pfIsoCharged04ForCiC[40][100]; // Hollow cone track isolation
      Float_t pid_pfIsoCharged05ForCiC[40][100]; // Hollow cone track isolation
      Float_t pid_pfIsoCharged06ForCiC[40][100]; // Hollow cone track isolation

      Float_t pid_pfIsoPhotons01ForCiC[40]; 
      Float_t pid_pfIsoPhotons02ForCiC[40]; 
      Float_t pid_pfIsoPhotons03ForCiC[40]; 
      Float_t pid_pfIsoPhotons04ForCiC[40]; 
      Float_t pid_pfIsoPhotons05ForCiC[40]; 
      Float_t pid_pfIsoPhotons06ForCiC[40]; 

      Float_t pid_pfIsoNeutrals01ForCiC[40]; 
      Float_t pid_pfIsoNeutrals02ForCiC[40]; 
      Float_t pid_pfIsoNeutrals03ForCiC[40]; 
      Float_t pid_pfIsoNeutrals04ForCiC[40]; 
      Float_t pid_pfIsoNeutrals05ForCiC[40]; 
      Float_t pid_pfIsoNeutrals06ForCiC[40]; 

      //Other variables for MVA id
      Float_t pid_scetawid[40]; // eta width
      Float_t pid_scphiwid[40]; // eta width
      Float_t pid_lambdaRatio[40]; // eta width
      Float_t pid_esXwidth[40]; // eta width
      Float_t pid_esYwidth[40]; // eta width

      
      Float_t ptiso004Phot[40];
      Int_t ntrkiso004Phot[40];
      Float_t ptiso035Phot[40];
      Int_t ntrkiso035Phot[40];
      Float_t ptiso04Phot[40];
      Int_t ntrkiso04Phot[40];
      Float_t hcalovecal04Phot[40]; 
      Float_t ecaliso04Phot[40];  
      Float_t sMinMinPhot[40];
      Float_t sMajMajPhot[40];
      Float_t alphaPhot[40];
      Float_t sEtaEtaPhot[40];
      Float_t sEtaPhiPhot[40];
      Float_t sPhiPhiPhot[40];
      Float_t E1Phot[40];
      Float_t E2OverE9Phot[40];
      Float_t E4Phot[40];
      Float_t E9Phot[40];
      Float_t E25Phot[40];
      Int_t ieleassocPhot[40];
      Float_t pid_deltaRToTrackPhot[40];
      
      Int_t nElePhot;
      Float_t pid_jurECALElePhot[40]; 
      Float_t pid_twrHCALElePhot[40]; 
      Float_t pid_HoverEElePhot[40]; 
      Float_t pid_hlwTrackElePhot[40]; 
      Float_t pid_etawidElePhot[40]; 
      Float_t pid_dphivtxElePhot[40]; 
      Float_t pid_detavtxElePhot[40]; 
      Float_t pid_distElePhot[40]; 
      Float_t pid_dcotElePhot[40]; 
      Int_t pid_mishitsElePhot[40]; 
      Float_t pid_ptElePhot[40]; 


      
      Int_t nEle;
      float electron_pt[200];
      float electron_px[200];
      float electron_py[200];
      float electron_pz[200];
      float electron_vx[200];
      float electron_vy[200];
      float electron_vz[200];
      float electron_energy[200];
      float electron_charge[200];
      float electron_eta[200];
      float electron_phi[200];
      Int_t electron_misHits[200]; 
      Int_t electron_seedType[200]; 
      float electron_trkIso[200];
      float electron_ecalIso[200];
      float electron_hcalIso[200];
      float electron_trkIso03[200];
      float electron_ecalIso03[200];
      float electron_hcalIso03[200];
      float electron_HoE[200];
      float electron_pFlowMVA[200];
      float electron_fBrem[200];
      float electron_dist[200];
      float electron_dcot[200];
      float electron_EoP[200];
      float electron_r9[200];
      Int_t electron_nSubClusters[200];
      float electron_OneOverEMinusOneOverP[200];
      float electron_SigmaIetaIeta[200];
      float electron_SigmaIphiIphi[200];
      float electron_dEtaIn[200];
      float electron_dPhiIn[200];
      float electron_sc_energy[200];
      float electron_sc_eta[200];
      float electron_sc_phi[200];
      

      Int_t nJet_akt5;
      Float_t ptJet_akt5[100];
      Float_t ptCorrJet_akt5[100];
      Float_t eJet_akt5[100];
      Float_t etaJet_akt5[100];
      Float_t phiJet_akt5[100];
      Float_t emfJet_akt5[100];
      Float_t n90Jet_akt5[100];
      Float_t n90HitsJet_akt5[100];
      Float_t fHPDJet_akt5[100];
      Float_t fRBXJet_akt5[100];

      Int_t nJet_akt7;
      Float_t ptJet_akt7[100];
      Float_t ptCorrJet_akt7[100];
      Float_t eJet_akt7[100];
      Float_t etaJet_akt7[100];
      Float_t phiJet_akt7[100];
      Float_t emfJet_akt7[100];
      Float_t n90Jet_akt7[100];
      Float_t n90HitsJet_akt7[100];
      Float_t fHPDJet_akt7[100];
      Float_t fRBXJet_akt7[100];

      Int_t nJet_jptak5;
      Float_t ptJet_jptak5[100];
      Float_t ptCorrJet_jptak5[100];
      Float_t eJet_jptak5[100];
      Float_t etaJet_jptak5[100];
      Float_t phiJet_jptak5[100];
      Float_t emfJet_jptak5[100];

      Int_t nJet_kt4;
      Float_t ptJet_kt4[100];
      Float_t eJet_kt4[100];
      Float_t etaJet_kt4[100];
      Float_t phiJet_kt4[100];
      Float_t emfJet_kt4[100];

      Int_t nJet_kt6;
      Float_t ptJet_kt6[100];
      Float_t eJet_kt6[100];
      Float_t etaJet_kt6[100];
      Float_t phiJet_kt6[100];
      Float_t emfJet_kt6[100];

      Float_t   rho;
      Float_t   rhoCalo;
      Float_t   rhoAllJets;

      Int_t nJet_pfkt4;
      Float_t ptJet_pfkt4[100];
      Float_t eJet_pfkt4[100];
      Float_t etaJet_pfkt4[100];
      Float_t phiJet_pfkt4[100];

      Int_t nJet_pfakt5;
      Float_t ptJet_pfakt5[100];
      Float_t ptCorrJet_pfakt5[100];
      Float_t eJet_pfakt5[100];
      Float_t etaJet_pfakt5[100];
      Float_t phiJet_pfakt5[100];
      Float_t ptDJet_pfakt5[100];
      Float_t rmsCandJet_pfakt5[100];

      Float_t jetId_dRMean_pfakt5[100];
      Float_t jetId_frac01_pfakt5[100];
      Float_t jetId_frac02_pfakt5[100];
      Float_t jetId_frac03_pfakt5[100];
      Float_t jetId_frac04_pfakt5[100];
      Float_t jetId_frac05_pfakt5[100];
      Float_t jetId_nNeutrals_pfakt5[100];
      Float_t jetId_beta_pfakt5[100];
      Float_t jetId_betaStar_pfakt5[100];
      Float_t jetId_dZ_pfakt5[100];
      Float_t jetId_nCharged_pfakt5[100];
      Float_t jetId_dR2Mean_pfakt5[100];
      Float_t jetId_betaStarClassic_pfakt5[100];
      Float_t jetIdSimple_mva_pfakt5[100];
      Float_t jetIdFull_mva_pfakt5[100];
      Float_t jetIdCutBased_mva_pfakt5[100];
      Int_t jetIdSimple_wp_pfakt5[100];
      Int_t jetIdFull_wp_pfakt5[100];
      Int_t jetIdCutBased_wp_pfakt5[100];

      std::vector<PileupJetIdAlgo* > jetId_algos;
      std::vector<edm::ParameterSet > jetMVAAlgos;

      Float_t beta_pfakt5[100][100];
      Float_t betaStar_pfakt5[100][100];
      Float_t combinedSecondaryVertexBJetTags_pfakt5[100], 
              combinedSecondaryVertexMVABJetTags_pfakt5[100],
              jetBProbabilityBJetTags_pfakt5[100],
              jetProbabilityBJetTags_pfakt5[100],
              simpleSecondaryVertexHighEffBJetTags_pfakt5[100],
              simpleSecondaryVertexHighPurBJetTags_pfakt5[100],
              softMuonBJetTags_pfakt5[100],
              softMuonByIP3dBJetTags_pfakt5[100],
              softMuonByPtBJetTags_pfakt5[100],
              softElectronBJetTags_pfakt5[100],
              softElectronByIP3dBJetTags_pfakt5[100],
              softElectronByPtBJetTags_pfakt5[100],
              trackCountingHighPurBJetTags_pfakt5[100],
              trackCountingHighEffBJetTags_pfakt5[100];




      // Extra variables for PFlow studies
      Int_t nChargedHadrons_pfakt5[100];
      Int_t nPhotons_pfakt5[100];
      Int_t nElectrons_pfakt5[100];
      Int_t nMuons_pfakt5[100];
      Int_t nNeutralHadrons_pfakt5[100];
      Int_t nHFHadrons_pfakt5[100];
      Int_t nHFEM_pfakt5[100];

      Float_t eChargedHadrons_pfakt5[100];
      Float_t ePhotons_pfakt5[100];
      Float_t eElectrons_pfakt5[100];
      Float_t eMuons_pfakt5[100];
      Float_t eNeutralHadrons_pfakt5[100];
      Float_t eHFHadrons_pfakt5[100];
      Float_t eHFEM_pfakt5[100];

      Int_t nJet_pfakt7;
      Float_t ptJet_pfakt7[100];
      Float_t ptCorrJet_pfakt7[100];
      Float_t eJet_pfakt7[100];
      Float_t etaJet_pfakt7[100];
      Float_t phiJet_pfakt7[100];

      Int_t nJet_pfkt6;
      Float_t ptJet_pfkt6[100];
      Float_t eJet_pfkt6[100];
      Float_t etaJet_pfkt6[100];
      Float_t phiJet_pfkt6[100];

      Int_t nJetGen_kt4;
      Float_t ptJetGen_kt4[100];
      Float_t eJetGen_kt4[100];
      Float_t etaJetGen_kt4[100];
      Float_t phiJetGen_kt4[100];

      Int_t nJetGen_kt6;
      Float_t ptJetGen_kt6[100];
      Float_t eJetGen_kt6[100];
      Float_t etaJetGen_kt6[100];
      Float_t phiJetGen_kt6[100];

      Int_t nJetGen_akt5;
      Float_t ptJetGen_akt5[100];
      Float_t eJetGen_akt5[100];
      Float_t etaJetGen_akt5[100];
      Float_t phiJetGen_akt5[100];

      // Extra variables for PFlow studies
      Int_t nTracksGen_akt5[100];
      Int_t nPhotonsGen_akt5[100];
      Int_t nElectronsGen_akt5[100];
      Int_t nMuonsGen_akt5[100];
      Int_t nNeutralHadronsGen_akt5[100];
      Int_t nHFHadronsGen_akt5[100];
      Int_t nHFEMGen_akt5[100];

      Int_t nNeutronsGen_akt5[100];
      Int_t nK0LGen_akt5[100];
      Int_t nK0SGen_akt5[100];
      Int_t nLambdasGen_akt5[100];
      Int_t nCsiGen_akt5[100];
      Int_t nOtherNeutralHadronsGen_akt5[100];

      Float_t eTracksGen_akt5[100];
      Float_t ePhotonsGen_akt5[100];
      Float_t eElectronsGen_akt5[100];
      Float_t eMuonsGen_akt5[100];
      Float_t eNeutralHadronsGen_akt5[100];
      Float_t eHFHadronsGen_akt5[100];
      Float_t eHFEMGen_akt5[100];

      Float_t eNeutronsGen_akt5[100];
      Float_t eK0LGen_akt5[100];
      Float_t eK0SGen_akt5[100];
      Float_t eLambdasGen_akt5[100];
      Float_t eCsiGen_akt5[100];
      Float_t eOtherNeutralHadronsGen_akt5[100];

      Int_t nJetGen_akt7;
      Float_t ptJetGen_akt7[100];
      Float_t eJetGen_akt7[100];
      Float_t etaJetGen_akt7[100];
      Float_t phiJetGen_akt7[100];
      
      Float_t sMet;
      Float_t eMet;
      Float_t phiMet;
      Float_t signifMet;

      Float_t sCorrMet;
      Float_t eCorrMet;
      Float_t phiCorrMet;
      Float_t signifCorrMet;

      // new for 52X
      // Float_t smuCorrMet;
      // Float_t emuCorrMet;
      // Float_t phimuCorrMet;
      // Float_t signifmuCorrMet;

      Float_t sNoHFMet;
      Float_t eNoHFMet;
      Float_t phiNoHFMet;
      Float_t signifNoHFMet;

      Float_t stcMet;
      Float_t etcMet;
      Float_t phitcMet;
      Float_t signiftcMet;

      Float_t sglobalPfMet;
      Float_t eglobalPfMet;
      Float_t phiglobalPfMet;
      Float_t signifglobalPfMet;

      Float_t scentralPfMet;
      Float_t ecentralPfMet;
      Float_t phicentralPfMet;
      Float_t signifcentralPfMet;

      Float_t eassocPfMet[100];
      Float_t phiassocPfMet[100];
      Float_t signifassocPfMet[100];

      Float_t eassocOtherVtxPfMet[100];
      Float_t phiassocOtherVtxPfMet[100];
      Float_t signifassocOtherVtxPfMet[100];

      Float_t etrkPfMet[100];
      Float_t phitrkPfMet[100];
      Float_t signiftrkPfMet[100];

      Float_t ecleanPfMet[100];
      Float_t phicleanPfMet[100];
      Float_t signifcleanPfMet[100];

      // new for 52X
      // Float_t ecleanedSaclayPfMet[100];
      // Float_t phicleanedSaclayPfMet[100];
      // Float_t signifcleanedSaclayPfMet[100];
      // Float_t eminTypeICleanSaclayPfMet[100];
      // Float_t phiminTypeICleanSaclayPfMet[100];
      // Float_t signifminTypeICleanSaclayPfMet[100];

      Float_t globalPfSums[12];

      Float_t spfMet;
      Float_t epfMet;
      Float_t phipfMet;
      Float_t signifpfMet;

      Float_t spfMetType1;
      Float_t epfMetType1;
      Float_t phipfMetType1;
      Float_t signifpfMetType1;

      Float_t sMetGen;
      Float_t eMetGen;
      Float_t phiMetGen;
      Float_t signifMetGen;

      Float_t sMetGen2;
      Float_t eMetGen2;
      Float_t phiMetGen2;

      Bool_t   hltPass;
      Int_t    hltNamesLen;
      Int_t    hltCount;

      bool dumpAKT5Jets_;
      bool dumpAKT7Jets_;

      bool dumpJPTAKT5Jets_;
      bool dumpJPTAKT7Jets_;

      bool dumpPFAKT5Jets_;
      bool dumpPFAKT7Jets_;

      bool dumpKT4Jets_;
      bool dumpKT6Jets_;

      //************** FOR BEAM HALO STUDIES ****************//

      bool dumpBeamHaloInformations_;
      //BeamHaloSummary
      bool isBeamHaloIDTightPass;
      bool isBeamHaloIDLoosePass;

      bool isBeamHaloEcalLoosePass;
      bool isBeamHaloHcalLoosePass;
      bool isBeamHaloCSCLoosePass;
      bool isBeamHaloGlobalLoosePass;

      bool isBeamHaloEcalTightPass;
      bool isBeamHaloHcalTightPass;
      bool isBeamHaloCSCTightPass;
      bool isBeamHaloGlobalTightPass;

      bool isSmellsLikeHalo_Tag;
      bool isLooseHalo_Tag;
      bool isTightHalo_Tag;
      bool isExtremeTightHalo_Tag;

      //muon variables

      Int_t nMuons;
      Float_t muon_pt[200];
      Float_t muon_px[200];
      Float_t muon_py[200];
      Float_t muon_pz[200];
      Float_t muon_vx[200];
      Float_t muon_vy[200];
      Float_t muon_vz[200];
      Float_t muon_energy[200];
      Float_t muon_charge[200];
      Float_t muon_eta[200];
      Float_t muon_phi[200];
      Bool_t  muon_isGlobalMuon[200];
      Bool_t  muon_isTrackerMuon[200];
      Bool_t  muon_isStandAloneMuon[200];
      bool  muon_InnerTrack_isNonnull[200];
      bool  muon_OuterTrack_isNonnull[200];
      float muon_OuterPoint_x[200];
      float muon_OuterPoint_y[200];
      float muon_OuterPoint_z[200];
      float muon_InnerPoint_x[200];
      float muon_InnerPoint_y[200];
      float muon_InnerPoint_z[200];
      float muon_trackIso[200];
      float muon_ecalIso[200];
      float muon_hcalIso[200];
      float muon_relIso[200];
      int   muon_normChi2[200];
      int   muon_validHits[200];
      int   muon_tkHits[200];
      int   muon_pixHits[200];
      int  muon_numberOfMatches[200];
      
      //cosmicmuon variables
      Int_t nCosmicMuons;
      Float_t cosmicmuon_pt[200];
      Float_t cosmicmuon_px[200];
      Float_t cosmicmuon_py[200];
      Float_t cosmicmuon_pz[200];
      Float_t cosmicmuon_energy[200];
      Float_t cosmicmuon_charge[200];
      Float_t cosmicmuon_eta[200];
      Float_t cosmicmuon_phi[200];
      Bool_t  cosmicmuon_isGlobalMuon[200];
      Bool_t  cosmicmuon_isTrackerMuon[200];
      Bool_t  cosmicmuon_isStandAloneMuon[200];
      bool  cosmicmuon_InnerTrack_isNonnull[200];
      bool  cosmicmuon_OuterTrack_isNonnull[200];
      Float_t cosmicmuon_OuterPoint_x[200];
      Float_t cosmicmuon_OuterPoint_y[200];
      Float_t cosmicmuon_OuterPoint_z[200];

      std::vector<std::string>*  aHLTNames;
      //Bool_t     aHLTResults[MAXHLTBITS];
      std::vector<bool>*  aHLTResults;
      
      int nHLT;
      std::map<std::string, int> hltTriggers;

      int pu_n;
      int pu_true_n;
      int pu_bunchcrossing;
      float pu_zpos[50];
      float pu_sumpt_lowpt[50];
      float pu_sumpt_highpt[50];
      float pu_ntrks_lowpt[50];
      float pu_ntrks_highpt[50];

      HggVertexAnalyzer* vtxAna;
      HggVertexFromConversions* vtxAnaFromConv;
      VertexAlgoParameters vtxPar;


      std::string outFileName;
      std::string regressionWeights;
      EGEnergyCorrector* ecorr_;

      std::vector<std::string> rankVariables;
      std::vector<std::string> tmvaPerVtxVariables_;
      TMVA::Reader *tmvaPerVtxReader_;
      TMVA::Reader *tmvaPerEvtReader_;
      std::string tmvaPerVtxMethod;
      std::string tmvaPerEvtMethod;
};

