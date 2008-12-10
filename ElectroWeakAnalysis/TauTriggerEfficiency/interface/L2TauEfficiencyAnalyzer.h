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
#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/CaloTauFwd.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include <string>
#include <TTree.h>
#include <TFile.h>

typedef math::XYZTLorentzVectorD   LV;
typedef std::vector<LV>            LVColl;

using namespace reco; // 10122008/S.Lehti, added this to have the code compiling with both CMSSW_2_1_X and CMSSW_2_2_X. In CMSSW_2_2_X L2TauInfoAssociation is moved inside namespace reco

class L2TauEfficiencyAnalyzer {
   public:
      L2TauEfficiencyAnalyzer();
      ~L2TauEfficiencyAnalyzer();

      void Setup(const edm::ParameterSet&,TTree*);
      void fill(const edm::Event&,const reco::PFTau&);
      void fill(const edm::Event&,const reco::CaloTau&);
      void fill(const edm::Event&,const LV&);

   private:
      std::vector<double> clusterSeparation(const reco::PFCandidateRefVector& ,const reco::PFCandidateRefVector& );
      void matchAndFillL2(const LV& ,const L2TauInfoAssociation&);//See if this Jet Is Matched

      //Parameters to read
      edm::InputTag  l2TauInfoAssoc_; //Path to analyze
      std::string rootFile_;          //Output File Name
      double matchDR_;

      //L2 Variables
      int cl_Nclusters,NTowers60,NTowers90;
      float  ecalIsol_Et,towerIsol_Et,cl_etaRMS,cl_phiRMS,cl_drRMS,seedTowerEt,JetEt,JetEta,JetPhi,hadFraction;
 
      //PF Variables
      int NEGCandsInAnnulus,NHadCandsInAnnulus;
      float PFEGIsolEt,PFHighestClusterEt,PFEGEtaRMS,PFEGPhiRMS,PFEGDrRMS;
      int hasL2Jet;	

};


