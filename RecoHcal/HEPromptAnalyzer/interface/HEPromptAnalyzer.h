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
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

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

//propagation
#include "FastSimulation/BaseParticlePropagator/interface/BaseParticlePropagator.h"
#include "FastSimulation/CalorimeterProperties/interface/Calorimeter.h"
#include "FastSimulation/CaloGeometryTools/interface/CaloGeometryHelper.h"

//HE Petr
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"

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

class HEPromptAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HEPromptAnalyzer(const edm::ParameterSet&);
      ~HEPromptAnalyzer();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      void EtaPhiDepth(float x, float y, float z, int &EtaTmp, int &PhiTmp, int &DepthTmp);

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

  long runBegin,lumibegin,lumiend,evtNo;
  std::string startTime;

  TH1F *he_cros_en_top[2];
  TH1F *he_cros_en_bot[2];
  TH1F *he_cros_en_nt_top[2];
  TH1F *he_cros_en_ang_top[2];
  TH1F *he_cros_en_nt_bot[2];
  TH1F *he_cros_en_ang_bot[2];
  TH1F *he_cor_coef;
  TH1F *he_event_lng_plus;
  TH1F *he_event_lng_minus;
  TH1F *he_event_trk_nmb;
  TH1F *he_rechit_plus;
  TH1F *he_rechit_minus;
  int flagEndCap;

   TrackDetectorAssociator trackAssociator_;
   edm::InputTag inputRecoTrackColl_;
   TrackAssociatorParameters parameters_;
};
