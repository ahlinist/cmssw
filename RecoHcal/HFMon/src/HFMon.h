// system include files
#include <memory>
#include <fstream>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

//geometry
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
//point
#include "DataFormats/GeometryVector/interface/GlobalPoint.h" 
//vector
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

//HBHERecHits
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

//trigger
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtPsbWord.h"


//@@@@@@@@@@@@@@@@@@@@ L1 Technical Trigger Bits
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//Ntuple creation
#include "TTree.h"

class TH1F;
class TH2F;
class TStyle;
class TTree;

#include "TH1I.h"
#include "TMath.h"
#include "TString.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TProfile.h"

//
// class decleration
//

class HFMon : public edm::EDAnalyzer {
   public:
      explicit HFMon(const edm::ParameterSet&);
      ~HFMon();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      //------- histogram bookers ----------
      void bookHistograms ();
      TH1F* book1DHistogram (TFileDirectory& fDir, const std::string& fName, const std::string& fTitle, 
			     int fNbins, double fXmin, double fXmax) const;
      TH2F* book2DHistogram (TFileDirectory& fDir, const std::string& fName, const std::string& fTitle, 
			     int fNbinsX, double fXmin, double fXmax,
			     int fNbinsY, double fYmin, double fYmax) const;
      TProfile* bookProfileHistogram (TFileDirectory & fDir, const std::string & fName, const std::string & fTitle, int fNbins, double fXmin, double fXmax, double fYmin, double fYmax) const;
      
      // ----------member data ---------------------------
      edm::Service<TFileService> fs;

      std::string baseHtmlDir_;
      long runBegin,lumibegin,lumiend,evtNo;
      std::string startTime;
      int run,event;

      TH1F* HFlumi_ETsum_perwedge;
      TH1F* HFlumi_Occupancy_above_thr_r1;
      TH1F* HFlumi_Occupancy_between_thrs_r1;
      TH1F* HFlumi_Occupancy_below_thr_r1;
      TH1F* HFlumi_Occupancy_above_thr_r2;
      TH1F* HFlumi_Occupancy_between_thrs_r2;
      TH1F* HFlumi_Occupancy_below_thr_r2;
      TH1F* CenterOfEnergyRadius;
      TH1F* HFCenterOfEnergyRadius;
      TH1F* Etsum_ratio_p;
      TH1F* Energy_Occ;
      TH1F* Etsum_ratio_m;
      TH2F* CenterOfEnergy;
      TH2F* HFCenterOfEnergy;
      TH2F* Etsum_map_L;
      TH2F* Etsum_map_S;
      TH2F* Etsum_rphi_S;
      TH2F* Etsum_rphi_L;
      TH2F* Etsum_ratio_map;
      TH2F* Occ_rphi_S;
      TH2F* Occ_rphi_L;
      TH2F* Occ_map_L;
      TH2F* Occ_map_S;
      TProfile *COEradiusVSeta;
      TProfile *Etsum_eta_L;
      TProfile *Etsum_eta_S;
      TProfile *Etsum_phi_L;
      TProfile *Etsum_phi_S;
      TProfile *Occ_eta_S;
      TProfile *Occ_eta_L;
      TProfile *Occ_phi_L;
      TProfile *Occ_phi_S;
};
