// system include files
#include <memory>
#include <iostream>
#include <string>
#include <vector>

// user include files
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
//#include <CommonTools/UtilAlgos/interface/TFileService.h>

#include "TH1.h"
#include "TFile.h"
#include "TTree.h"

#include <map>
#include <set>
class SimTrack;
class SimVertex;

#define MAXHLTBITS    200

using namespace edm;
using namespace std;

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

      edm::InputTag MCTruthCollection_; 
      edm::InputTag triggerTag_;
      edm::InputTag Vertexsrc_;
      edm::InputTag Photonsrc_; 
      edm::InputTag Jetsrcite_; 
      edm::InputTag Jetsrckt4_; 
      edm::InputTag Jetsrckt6_; 
      edm::InputTag Jetsrcakt5_; 
      //edm::InputTag Jetsrcakt7_; 
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
      string JetCorrector_pfakt5_; 
      string JetCorrector_pfakt7_; 
      double genjetptthr_;
      double calojetptthr_;
      double pfjetptthr_;
      double jptjetptthr_;
      int genjetnmin_;
      int pfjetnmin_;
      int jptjetnmin_;

//      edm::Service<TFileService> fs_;
      TFile* outfile;

      // Tree with multiple info
      TTree * m_tree ;

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
      Float_t vx;
      Float_t vy;
      Float_t vz;
      Float_t vntracks;
      Float_t vchi2;
      Float_t vndof;

 
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

      // SIM particles (those not already in MC particles list)
      // help to study in-flight decays of Kshort, Lambda etc.
      // These are also useful to study photon conversions and
      // performance of PFlow
      // NB: go back and mark decayed MC particles with status = -1 
      static const int nMaxSIM = 150;
      Int_t nSIM;
      Int_t pdgIdSIM[nMaxSIM];
      Int_t statusSIM[nMaxSIM];
      Float_t ptSIM[nMaxSIM];
      Float_t eSIM[nMaxSIM];
      Float_t etaSIM[nMaxSIM];
      Float_t phiSIM[nMaxSIM];
      Float_t rSIM[nMaxSIM];
      Float_t zSIM[nMaxSIM];

      // PFlow candindates from the two leading jets
      static const int nMaxPF = 150;
      Int_t nPF;
      Int_t pdgIdPF[nMaxPF];
      Float_t ptPF[nMaxPF];
      Float_t ePF[nMaxPF];
      Float_t etaPF[nMaxPF];
      Float_t phiPF[nMaxPF];

      Float_t genpt;

      Int_t nPhot;
      Float_t ptPhot[40];
      Float_t ePhot[40];
      Float_t eseedPhot[40];
      Float_t escPhot[40];
      Float_t etaPhot[40];
      Float_t phiPhot[40];

      // Reconstructed photon conversions
      Int_t nconvPhot;
      Float_t chi2convPhot[10];
      Float_t ndofconvPhot[10];
      Float_t rconvPhot[10];
      Float_t phiconvPhot[10];
      Float_t zconvPhot[10];
      Int_t ntrkconvPhot[10];
      Float_t eovpconvPhot[10];
      Float_t etaecalconvPhot[10];
      Float_t phiecalconvPhot[10];
      Float_t energyecalconvPhot[10];
      // Extra conversion ID - pairwise
      Int_t algoconvPhot[10];
      Float_t d0convPhot[10];
      Float_t detaecalconvPhot[10];
      Float_t dphiecalconvPhot[10];
      Float_t dphivtxconvPhot[10];
      Float_t pairsepconvPhot[10];
      Float_t pairmassconvPhot[10];
      // Extra conversion ID - trackwise
      Float_t trchi21convPhot[10];
      Float_t trndof1convPhot[10];
      Int_t trqual1convPhot[10];
      Float_t trpt1convPhot[10];
      Float_t trerr1convPhot[10];
      Float_t trchi22convPhot[10];
      Float_t trndof2convPhot[10];
      Int_t trqual2convPhot[10];
      Float_t trpt2convPhot[10];
      Float_t trerr2convPhot[10];

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
      Float_t ptiso05Phot[40];
      Int_t ntrkiso05Phot[40];
      Float_t ptiso07Phot[40];
      Int_t ntrkiso07Phot[40];
      Float_t ptiso1Phot[40];
      Int_t ntrkiso1Phot[40];
      Float_t hcalovecal01Phot[40];
      Float_t hcalovecal015Phot[40];
      Float_t hcalovecal04Phot[40]; 
      Float_t hcalovecal05Phot[40]; 
      Float_t hcalovecal07Phot[40];
      Float_t hcalovecal1Phot[40]; 
      Float_t ecaliso01Phot[40]; 
      Float_t ecaliso015Phot[40]; 
      Float_t ecaliso04Phot[40];  
      Float_t ecaliso05Phot[40];  
      Float_t ecaliso07Phot[40];  
      Float_t ecaliso1Phot[40];  
      Float_t LATPhot[40];
      Float_t sMinMinPhot[40];
      Float_t sMajMajPhot[40];
      Float_t FisherPhot[40];
      Float_t alphaPhot[40];
      Float_t sEtaEtaPhot[40];
      Float_t sPhiPhiPhot[40];
      Float_t E1Phot[40];
      Float_t E9Phot[40];
      Float_t E25Phot[40];


      Int_t nJet_ite;
      Float_t ptJet_ite[100];
      Float_t eJet_ite[100];
      Float_t etaJet_ite[100];
      Float_t phiJet_ite[100];
      Float_t emfJet_ite[100];

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

      Int_t nJet_akt5;
      Float_t ptJet_akt5[100];
      Float_t ptCorrJet_akt5[100];
      Float_t eJet_akt5[100];
      Float_t etaJet_akt5[100];
      Float_t phiJet_akt5[100];
      Float_t emfJet_akt5[100];

      //Int_t nJet_akt7;
      //Float_t ptJet_akt7[100];
      //Float_t eJet_akt7[100];
      //Float_t etaJet_akt7[100];
      //Float_t phiJet_akt7[100];
      //Float_t emfJet_akt7[100];

      Int_t nJet_sis5;
      Float_t ptJet_sis5[100];
      Float_t eJet_sis5[100];
      Float_t etaJet_sis5[100];
      Float_t phiJet_sis5[100];
      Float_t emfJet_sis5[100];

      Int_t nJet_sis7;
      Float_t ptJet_sis7[100];
      Float_t eJet_sis7[100];
      Float_t etaJet_sis7[100];
      Float_t phiJet_sis7[100];
      Float_t emfJet_sis7[100];

      Int_t nJet_jptak5;
      Float_t ptJet_jptak5[100];
      Float_t eJet_jptak5[100];
      Float_t etaJet_jptak5[100];
      Float_t phiJet_jptak5[100];
      Float_t emfJet_jptak5[100];

      Int_t nJet_pfite;
      Float_t ptJet_pfite[100];
      Float_t eJet_pfite[100];
      Float_t etaJet_pfite[100];
      Float_t phiJet_pfite[100];

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

      Float_t ptChargedHadrons_pfakt5[100];
      Float_t ptPhotons_pfakt5[100];
      Float_t ptElectrons_pfakt5[100];
      Float_t ptMuons_pfakt5[100];
      Float_t ptNeutralHadrons_pfakt5[100];
      Float_t ptHFHadrons_pfakt5[100];
      Float_t ptHFEM_pfakt5[100];

      Float_t phiChargedHadrons_pfakt5[100];
      Float_t phiPhotons_pfakt5[100];
      Float_t phiElectrons_pfakt5[100];
      Float_t phiMuons_pfakt5[100];
      Float_t phiNeutralHadrons_pfakt5[100];
      Float_t phiHFHadrons_pfakt5[100];
      Float_t phiHFEM_pfakt5[100];

      Float_t etaChargedHadrons_pfakt5[100];
      Float_t etaPhotons_pfakt5[100];
      Float_t etaElectrons_pfakt5[100];
      Float_t etaMuons_pfakt5[100];
      Float_t etaNeutralHadrons_pfakt5[100];
      Float_t etaHFHadrons_pfakt5[100];
      Float_t etaHFEM_pfakt5[100];

      Int_t nJet_pfakt7;
      Float_t ptJet_pfakt7[100];
      Float_t ptCorrJet_pfakt7[100];
      Float_t eJet_pfakt7[100];
      Float_t etaJet_pfakt7[100];
      Float_t phiJet_pfakt7[100];

      Int_t nJet_pfsis5;
      Float_t ptJet_pfsis5[100];
      Float_t eJet_pfsis5[100];
      Float_t etaJet_pfsis5[100];
      Float_t phiJet_pfsis5[100];

      Int_t nJet_pfkt6;
      Float_t ptJet_pfkt6[100];
      Float_t eJet_pfkt6[100];
      Float_t etaJet_pfkt6[100];
      Float_t phiJet_pfkt6[100];

      Int_t nJet_pfsis7;
      Float_t ptJet_pfsis7[100];
      Float_t eJet_pfsis7[100];
      Float_t etaJet_pfsis7[100];
      Float_t phiJet_pfsis7[100];


      Int_t nJetGen_ite;
      Float_t ptJetGen_ite[100];
      Float_t eJetGen_ite[100];
      Float_t etaJetGen_ite[100];
      Float_t phiJetGen_ite[100];

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

      Float_t ptTracksGen_akt5[100];
      Float_t ptPhotonsGen_akt5[100];
      Float_t ptElectronsGen_akt5[100];
      Float_t ptMuonsGen_akt5[100];
      Float_t ptNeutralHadronsGen_akt5[100];
      Float_t ptHFHadronsGen_akt5[100];
      Float_t ptHFEMGen_akt5[100];

      Float_t phiTracksGen_akt5[100];
      Float_t phiPhotonsGen_akt5[100];
      Float_t phiElectronsGen_akt5[100];
      Float_t phiMuonsGen_akt5[100];
      Float_t phiNeutralHadronsGen_akt5[100];
      Float_t phiHFHadronsGen_akt5[100];
      Float_t phiHFEMGen_akt5[100];

      Float_t etaTracksGen_akt5[100];
      Float_t etaPhotonsGen_akt5[100];
      Float_t etaElectronsGen_akt5[100];
      Float_t etaMuonsGen_akt5[100];
      Float_t etaNeutralHadronsGen_akt5[100];
      Float_t etaHFHadronsGen_akt5[100];
      Float_t etaHFEMGen_akt5[100];
      
      Int_t nJetGen_akt7;
      Float_t ptJetGen_akt7[100];
      Float_t eJetGen_akt7[100];
      Float_t etaJetGen_akt7[100];
      Float_t phiJetGen_akt7[100];
      
      // Extra variables for PFlow studies
      Int_t nChargedHadrons_pfakt7[100];
      Int_t nPhotons_pfakt7[100];
      Int_t nElectrons_pfakt7[100];
      Int_t nMuons_pfakt7[100];
      Int_t nNeutralHadrons_pfakt7[100];
      Int_t nHFHadrons_pfakt7[100];
      Int_t nHFEM_pfakt7[100];

      Float_t eChargedHadrons_pfakt7[100];
      Float_t ePhotons_pfakt7[100];
      Float_t eElectrons_pfakt7[100];
      Float_t eMuons_pfakt7[100];
      Float_t eNeutralHadrons_pfakt7[100];
      Float_t eHFHadrons_pfakt7[100];
      Float_t eHFEM_pfakt7[100];

      Float_t ptChargedHadrons_pfakt7[100];
      Float_t ptPhotons_pfakt7[100];
      Float_t ptElectrons_pfakt7[100];
      Float_t ptMuons_pfakt7[100];
      Float_t ptNeutralHadrons_pfakt7[100];
      Float_t ptHFHadrons_pfakt7[100];
      Float_t ptHFEM_pfakt7[100];

      Float_t phiChargedHadrons_pfakt7[100];
      Float_t phiPhotons_pfakt7[100];
      Float_t phiElectrons_pfakt7[100];
      Float_t phiMuons_pfakt7[100];
      Float_t phiNeutralHadrons_pfakt7[100];
      Float_t phiHFHadrons_pfakt7[100];
      Float_t phiHFEM_pfakt7[100];

      Float_t etaChargedHadrons_pfakt7[100];
      Float_t etaPhotons_pfakt7[100];
      Float_t etaElectrons_pfakt7[100];
      Float_t etaMuons_pfakt7[100];
      Float_t etaNeutralHadrons_pfakt7[100];
      Float_t etaHFHadrons_pfakt7[100];
      Float_t etaHFEM_pfakt7[100];

      Int_t nJetGen_sis5;
      Float_t ptJetGen_sis5[100];
      Float_t eJetGen_sis5[100];
      Float_t etaJetGen_sis5[100];
      Float_t phiJetGen_sis5[100];

      Int_t nJetGen_sis7;
      Float_t ptJetGen_sis7[100];
      Float_t eJetGen_sis7[100];
      Float_t etaJetGen_sis7[100];
      Float_t phiJetGen_sis7[100];

      Float_t sMet;
      Float_t eMet;
      Float_t phiMet;

      Float_t stcMet;
      Float_t etcMet;
      Float_t phitcMet;

      Float_t spfMet;
      Float_t epfMet;
      Float_t phipfMet;

      Float_t sMetGen;
      Float_t eMetGen;
      Float_t phiMetGen;

      Float_t sMetGen2;
      Float_t eMetGen2;
      Float_t phiMetGen2;

      Bool_t   hltPass;
      char     aHLTNames[6000];
      Int_t    hltNamesLen;
      Int_t    hltCount;
      bool     aHLTResults[MAXHLTBITS];

      int nHLT;
      std::map<std::string, int> hltTriggers;
};

