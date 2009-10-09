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
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"

//Calorimeter!!
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h" 
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"


#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/LorentzVectorFwd.h"

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
      void fill(const edm::Event&,const edm::EventSetup& ,const reco::PFTau&);
      void fill(const edm::Event&,const edm::EventSetup& ,const reco::CaloTau&);
      void fill(const edm::Event&,const edm::EventSetup& ,const LV&);
      void fill(const edm::Event&,const edm::EventSetup& ,const reco::GsfElectron&);

   private:
      std::vector<double> clusterSeparation(const reco::PFCandidateRefVector& ,const reco::PFCandidateRefVector& );
      void matchAndFillL2(const LV& ,const L2TauInfoAssociation&);//See if this Jet Is Matched
      math::PtEtaPhiELorentzVectorCollection getECALHits(const LV&,const edm::Event& ,const edm::EventSetup&);
      math::PtEtaPhiELorentzVectorCollection getHCALTowers(const LV& jet,const edm::Event& iEvent,const edm::EventSetup& iSetup);
      math::PtEtaPhiELorentzVectorCollection getECALTowers(const LV& jet,const edm::Event& iEvent,const edm::EventSetup& iSetup);
      double isolatedEt(const LV&,const math::PtEtaPhiELorentzVectorCollection& ) const;

      //Parameters to read
      edm::InputTag  l2TauInfoAssoc_; //Path to analyze
      edm::InputTag  CaloTowers_;
      edm::InputTag  EBRecHits_;
      edm::InputTag  EERecHits_;
      double outerCone_; //Association Distance  for a tower/crystal
      double innerCone_; //Association Distance  for a tower/crystal

      //Thresholding
      double crystalThresholdE_;
      double crystalThresholdB_;

      std::string rootFile_;          //Output File Name

      double matchDR_;

      //L2 Variables
      int cl_Nclusters,NTowers60,NTowers90;
      float  ecalIsol_Et,towerIsol_Et,cl_etaRMS,cl_phiRMS,cl_drRMS,seedTowerEt,JetEt,JetEta,JetPhi,hadFraction;
 
      //PF Variables
      int NEGCandsInAnnulus,NHadCandsInAnnulus;
      float PFEGIsolEt,PFHighestClusterEt,PFEGEtaRMS,PFEGPhiRMS,PFEGDrRMS;

      float PFEcalIsol_Et;
      float PFHcalTowIsol_Et;
      float PFEcalTowIsol_Et;

      int hasL2Jet;	

};


