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

#include "TH1.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"

#include <map>
#include <set>
class SimTrack;
class SimVertex;

#define MAXHLTBITS    5000

using namespace edm;
using namespace std;
using namespace reco;

//
// class declaration
//

class GammaJetAnalyzer : public edm::EDAnalyzer {
   public:
      explicit GammaJetAnalyzer(const edm::ParameterSet&);
      ~GammaJetAnalyzer();


   private:
      virtual void beginJob();
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      //  calculate phi1-phi2 keeping value between 0 and pi
      inline float delta_phi(float phi1, float phi2);
      // calculate eta1-eta2 keeping eta2 positive
      inline float delta_eta(float eta1, float eta2);
      // calculate sum in quadrature
      inline double oplus(double a, double b);
      // fix EMF in HF
      inline double fixEMF(double emf, double eta);

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

TH1F* h1_hbherh_detid;
TH1F* h1_etaPhot;
TH2D* h2_n_vs_eta;
      edm::InputTag MCTruthCollection_; 
      edm::InputTag triggerTag_;
      edm::InputTag Vertexsrc_;
      edm::InputTag Photonsrc_; 
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

      Int_t nPhot;
      Float_t ptPhot[40];
      Float_t ePhot[40];
      Float_t eseedPhot[40];
      Float_t escPhot[40];
      Float_t etaPhot[40];
      Float_t phiPhot[40];
      Float_t timePhot[40];
      Float_t e4SwissCrossPhot[40];
      Int_t hasPixelSeedPhot[40];


      // Default PAT photon ID variables
      Bool_t pid_isEM[40];
      Bool_t pid_isLoose[40];
      Bool_t pid_isTight[40];
      Float_t pid_jurECAL[40]; // jurassic ECAL isolation
      Float_t pid_twrHCAL[40]; // Tower-based HCAL isolation
      Float_t pid_HoverE[40]; // Hadronic / EM
      Float_t pid_hlwTrack[40]; // Hollow cone track isolation
      Float_t pid_etawid[40]; // eta width
     
      Float_t ptiso0015Phot[40];
      Int_t ntrkiso0015Phot[40];
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
      Float_t E9Phot[40];
      Float_t E25Phot[40];

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

      Float_t smuCorrMet;
      Float_t emuCorrMet;
      Float_t phimuCorrMet;
      Float_t signifmuCorrMet;

      Float_t sNoHFMet;
      Float_t eNoHFMet;
      Float_t phiNoHFMet;
      Float_t signifNoHFMet;

      Float_t stcMet;
      Float_t etcMet;
      Float_t phitcMet;
      Float_t signiftcMet;

      Float_t spfMet;
      Float_t epfMet;
      Float_t phipfMet;
      Float_t signifpfMet;

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

      std::vector<std::string>*  aHLTNames;
      //bool     aHLTResults[MAXHLTBITS];
      std::vector<bool>*  aHLTResults;
      
      int nHLT;
      std::map<std::string, int> hltTriggers;
};

