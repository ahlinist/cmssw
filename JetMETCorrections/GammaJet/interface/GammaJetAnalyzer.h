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


#include "TH1.h"
#include "TFile.h"
#include "TTree.h"

#define MAXHLTBITS    200

using namespace edm;
using namespace std;

//
// class decleration
//

class GammaJetAnalyzer : public edm::EDAnalyzer {
   public:
      explicit GammaJetAnalyzer(const edm::ParameterSet&);
      ~GammaJetAnalyzer();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      edm::InputTag MCTruthCollection_; 
      edm::InputTag triggerTag_;
      edm::InputTag Photonsrc_; 
      edm::InputTag Jetsrcite_; 
      edm::InputTag Jetsrckt4_; 
      edm::InputTag Jetsrckt6_; 
      edm::InputTag Jetsrcsis5_; 
      edm::InputTag Jetsrcsis7_; 
      edm::InputTag Pfjetsrc_; 
      edm::InputTag JetGensrcite_; 
      edm::InputTag JetGensrckt4_; 
      edm::InputTag JetGensrckt6_; 
      edm::InputTag JetGensrcsis5_; 
      edm::InputTag JetGensrcsis7_; 
      edm::InputTag METsrc_; 
      edm::InputTag METGensrc_; 
      edm::InputTag trackTags_; 
      edm::InputTag HBhitsrc_; 
      string recoCollection_; 
      string recoProducer_; 
      string filename_; 
      TFile*      hOutputFile ;
      TH1D*       PtPhoton1st;   
      TH1D*       PtPhoton2st;   
      TH1D*       PtPhoton3st;   
      TH1D*       PtPhotonMC1st;   
      TH1D*       PtPhotonMC2st;   
      TH1D*       PtPhotonMC3st;   

      // Tree with multiple info
      TTree * m_tree ;

      Int_t event;

      Int_t nMC;
      Int_t pdgIdMC[100];
      Int_t statusMC[100];
      Float_t massMC[100];
      Int_t motherIDMC[100];
      Float_t pxMC[100];
      Float_t pyMC[100];
      Float_t pzMC[100];
      Float_t eMC[100];
      Float_t etaMC[100];
      Float_t phiMC[100];
   
      Float_t genpt;

      Int_t nPhot;
      Float_t pxPhot[40];
      Float_t pyPhot[40];
      Float_t pzPhot[40];
      Float_t ePhot[40];
      Float_t escPhot[40];
      Float_t etaPhot[40];
      Float_t phiPhot[40];

      Int_t nconvPhot;
      Float_t chi2convPhot[10];
      Float_t ndofconvPhot[10];
      Float_t xconvPhot[10];
      Float_t yconvPhot[10];
      Float_t zconvPhot[10];
      Int_t ntrkconvPhot[10];
      Float_t eovpconvPhot[10];
      Float_t etaecalconvPhot[10];
      Float_t phiecalconvPhot[10];
      Float_t energyecalconvPhot[10];
     
      Float_t ptiso0015Phot[40];
      Int_t ntrkiso0015Phot[40];
      Float_t ptiso035Phot[40];
      Int_t ntrkiso035Phot[40];
      Float_t ptiso05Phot[40];
      Int_t ntrkiso05Phot[40];
      Float_t ptiso07Phot[40];
      Int_t ntrkiso07Phot[40];
      Float_t ptiso1Phot[40];
      Int_t ntrkiso1Phot[40];
/*       Float_t ptisoatecal02Phot[40]; */
/*       Int_t ntrkisoatecal02Phot[40]; */
/*       Float_t ptisoatecal025Phot[40]; */
/*       Int_t ntrkisoatecal025Phot[40]; */
/*       Float_t ptisoatecal03Phot[40]; */
/*       Int_t ntrkisoatecal03Phot[40]; */
/*       Float_t ptisoatecal035Phot[40]; */
/*       Int_t ntrkisoatecal035Phot[40]; */
/*       Float_t ptisoatecal04Phot[40]; */
/*       Int_t ntrkisoatecal04Phot[40]; */
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
      Float_t pxJet_ite[100];
      Float_t pyJet_ite[100];
      Float_t pzJet_ite[100];
      Float_t eJet_ite[100];
      Float_t etaJet_ite[100];
      Float_t phiJet_ite[100];

      Int_t nJet_kt4;
      Float_t pxJet_kt4[100];
      Float_t pyJet_kt4[100];
      Float_t pzJet_kt4[100];
      Float_t eJet_kt4[100];
      Float_t etaJet_kt4[100];
      Float_t phiJet_kt4[100];

      Int_t nJet_kt6;
      Float_t pxJet_kt6[100];
      Float_t pyJet_kt6[100];
      Float_t pzJet_kt6[100];
      Float_t eJet_kt6[100];
      Float_t etaJet_kt6[100];
      Float_t phiJet_kt6[100];

      Int_t nJet_sis5;
      Float_t pxJet_sis5[100];
      Float_t pyJet_sis5[100];
      Float_t pzJet_sis5[100];
      Float_t eJet_sis5[100];
      Float_t etaJet_sis5[100];
      Float_t phiJet_sis5[100];

      Int_t nJet_sis7;
      Float_t pxJet_sis7[100];
      Float_t pyJet_sis7[100];
      Float_t pzJet_sis7[100];
      Float_t eJet_sis7[100];
      Float_t etaJet_sis7[100];
      Float_t phiJet_sis7[100];

      Int_t nJet_pfite;
      Float_t pxJet_pfite[100];
      Float_t pyJet_pfite[100];
      Float_t pzJet_pfite[100];
      Float_t eJet_pfite[100];
      Float_t etaJet_pfite[100];
      Float_t phiJet_pfite[100];

      Int_t nJetGen_ite;
      Float_t pxJetGen_ite[100];
      Float_t pyJetGen_ite[100];
      Float_t pzJetGen_ite[100];
      Float_t eJetGen_ite[100];
      Float_t etaJetGen_ite[100];
      Float_t phiJetGen_ite[100];

      Int_t nJetGen_kt4;
      Float_t pxJetGen_kt4[100];
      Float_t pyJetGen_kt4[100];
      Float_t pzJetGen_kt4[100];
      Float_t eJetGen_kt4[100];
      Float_t etaJetGen_kt4[100];
      Float_t phiJetGen_kt4[100];

      Int_t nJetGen_kt6;
      Float_t pxJetGen_kt6[100];
      Float_t pyJetGen_kt6[100];
      Float_t pzJetGen_kt6[100];
      Float_t eJetGen_kt6[100];
      Float_t etaJetGen_kt6[100];
      Float_t phiJetGen_kt6[100];

      Int_t nJetGen_sis5;
      Float_t pxJetGen_sis5[100];
      Float_t pyJetGen_sis5[100];
      Float_t pzJetGen_sis5[100];
      Float_t eJetGen_sis5[100];
      Float_t etaJetGen_sis5[100];
      Float_t phiJetGen_sis5[100];

      Int_t nJetGen_sis7;
      Float_t pxJetGen_sis7[100];
      Float_t pyJetGen_sis7[100];
      Float_t pzJetGen_sis7[100];
      Float_t eJetGen_sis7[100];
      Float_t etaJetGen_sis7[100];
      Float_t phiJetGen_sis7[100];

      Float_t pxMet;
      Float_t pyMet;
      Float_t pzMet;
      Float_t eMet;
      Float_t phiMet;
      Float_t etaMet;

      Float_t pxMetGen;
      Float_t pyMetGen;
      Float_t pzMetGen;
      Float_t eMetGen;
      Float_t phiMetGen;
      Float_t etaMetGen;

      char     aHLTNames[6000];
      Int_t    hltNamesLen;
      Int_t    hltCount;
      bool     aHLTResults[MAXHLTBITS];
};

