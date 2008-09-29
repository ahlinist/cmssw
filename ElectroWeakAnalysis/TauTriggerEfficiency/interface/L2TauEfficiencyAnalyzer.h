// Original Author:  Michail Bachtis
//         Created:  Sun Jan 20 20:10:02 CST 2008
// University of Wisconsin-Madison


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/TauReco/interface/L2TauInfoAssociation.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <string>
#include <TTree.h>
#include <TFile.h>

typedef math::XYZTLorentzVectorD   LV;
typedef std::vector<LV>            LVColl;

//Matching struct




class L2TauEfficiencyAnalyzer : public edm::EDAnalyzer {
   public:
      explicit L2TauEfficiencyAnalyzer(const edm::ParameterSet&);
      ~L2TauEfficiencyAnalyzer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      std::vector<double> clusterSeparation(const reco::PFCandidateRefVector& ,const reco::PFCandidateRefVector& );
      void matchAndFillL2(const reco::PFTau& ,const L2TauInfoAssociation&);//See if this Jet Is Matched


      //Parameters to read
      edm::InputTag  PFTaus_; //Path to analyze
      edm::InputTag  l2TauInfoAssoc_; //Path to analyze
      std::string rootFile_;          //Output File Name


      double matchDR_;

      //L2 Variables
      int cl_Nclusters,NTowers60,NTowers90;
      float  ecalIsol_Et,towerIsol_Et,cl_etaRMS,cl_phiRMS,cl_drRMS,seedTowerEt,JetEt,JetEta,JetPhi,hadFraction;
 
      //PF Variables
      int NEGCandsInAnnulus,NHadCandsInAnnulus;
      float PFEt,PFEta,PFPhi,PFEGIsolEt,PFHighestClusterEt,PFEGEtaRMS,PFEGPhiRMS,PFEGDrRMS;
      int hasL2Jet;	

      TFile *l2file;//File to store the histos...
      TTree *l2tree;


};


