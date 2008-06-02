// system include files
#include <memory>
#include <iostream>
#include <string>

// user include files
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"


#include "TH1.h"
#include "TFile.h"
#include "TTree.h"

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
      edm::InputTag Photonsrc_; 
      edm::InputTag Jetsrcite_; 
      edm::InputTag Jetsrckt_; 
      edm::InputTag Jetsrcsis_; 
      edm::InputTag JetGensrcite_; 
      edm::InputTag JetGensrckt_; 
      edm::InputTag JetGensrcsis_; 
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
   
      Int_t nPhot;
      Float_t pxPhot[40];
      Float_t pyPhot[40];
      Float_t pzPhot[40];
      Float_t ePhot[40];
      Float_t etaPhot[40];
      Float_t phiPhot[40];
      Float_t ptiso02Phot[40];
      Int_t ntrkiso02Phot[40];
      Float_t ptiso025Phot[40];
      Int_t ntrkiso025Phot[40];
      Float_t ptiso03Phot[40];
      Int_t ntrkiso03Phot[40];
      Float_t ptiso035Phot[40];
      Int_t ntrkiso035Phot[40];
      Float_t ptiso04Phot[40];
      Int_t ntrkiso04Phot[40];
      Float_t ptisoatecal02Phot[40];
      Int_t ntrkisoatecal02Phot[40];
      Float_t ptisoatecal025Phot[40];
      Int_t ntrkisoatecal025Phot[40];
      Float_t ptisoatecal03Phot[40];
      Int_t ntrkisoatecal03Phot[40];
      Float_t ptisoatecal035Phot[40];
      Int_t ntrkisoatecal035Phot[40];
      Float_t ptisoatecal04Phot[40];
      Int_t ntrkisoatecal04Phot[40];
      Float_t hcalovecal01Phot[40];
      Float_t hcalovecal02Phot[40];
      Float_t hcalovecal025Phot[40];
      Float_t hcalovecal03Phot[40]; 
      Float_t hcalovecal04Phot[40]; 
      Float_t hcalovecal05Phot[40]; 
      Float_t ecaliso02Phot[40]; 
      Float_t ecaliso03Phot[40];  
      Float_t ecaliso035Phot[40];  
      Float_t ecaliso04Phot[40];  
      Float_t ecaliso05Phot[40];  
      Float_t LATPhot[40];
      Float_t sMinMinPhot[40];
      Float_t sMajMajPhot[40];
      Float_t FisherPhot[40];

      Int_t nJet_ite;
      Float_t pxJet_ite[100];
      Float_t pyJet_ite[100];
      Float_t pzJet_ite[100];
      Float_t eJet_ite[100];
      Float_t etaJet_ite[100];
      Float_t phiJet_ite[100];

      Int_t nJet_kt;
      Float_t pxJet_kt[100];
      Float_t pyJet_kt[100];
      Float_t pzJet_kt[100];
      Float_t eJet_kt[100];
      Float_t etaJet_kt[100];
      Float_t phiJet_kt[100];

      Int_t nJet_sis;
      Float_t pxJet_sis[100];
      Float_t pyJet_sis[100];
      Float_t pzJet_sis[100];
      Float_t eJet_sis[100];
      Float_t etaJet_sis[100];
      Float_t phiJet_sis[100];

      Int_t nJetGen_ite;
      Float_t pxJetGen_ite[100];
      Float_t pyJetGen_ite[100];
      Float_t pzJetGen_ite[100];
      Float_t eJetGen_ite[100];
      Float_t etaJetGen_ite[100];
      Float_t phiJetGen_ite[100];

      Int_t nJetGen_kt;
      Float_t pxJetGen_kt[100];
      Float_t pyJetGen_kt[100];
      Float_t pzJetGen_kt[100];
      Float_t eJetGen_kt[100];
      Float_t etaJetGen_kt[100];
      Float_t phiJetGen_kt[100];

      Int_t nJetGen_sis;
      Float_t pxJetGen_sis[100];
      Float_t pyJetGen_sis[100];
      Float_t pzJetGen_sis[100];
      Float_t eJetGen_sis[100];
      Float_t etaJetGen_sis[100];
      Float_t phiJetGen_sis[100];

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

   
};

