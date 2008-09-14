//#ifndef TrackingAnalysis_Cosmics_HitEff_h
//#define TrackingAnalysis_Cosmics_HitEff_h

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "TrackingTools/MaterialEffects/interface/PropagatorWithMaterial.h"
#include "TrackingTools/KalmanUpdators/interface/KFUpdator.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "TrackingTools/TrackFitters/interface/KFTrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/KFTrajectorySmoother.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "RecoTracker/SingleTrackPattern/interface/CosmicTrajectoryBuilder.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementError.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementVector.h"
#include "TrackingAnalysis/Cosmics/interface/TrackLocalAngle.h"

#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include <vector>
#include "TTree.h"
#include <iostream>
#include "stdlib.h"
#include "stdio.h"
#include "Riostream.h"
#include "TRandom2.h"


class HitEff : public edm::EDAnalyzer {
 public:  
  explicit HitEff(const edm::ParameterSet& conf);
  double checkConsistency(const SiStripRecHit2D* rechit, double xx, double xerr);

  virtual ~HitEff();

 private:
  virtual void beginJob(const edm::EventSetup& c);
  virtual void endJob(); 
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);

        // ----------member data ---------------------------

  edm::ParameterSet conf_;
  edm::ESHandle<SiStripDetCabling> SiStripDetCabling_;
  edm::ESHandle<SiStripQuality> SiStripQuality_;

  TTree* rechits;
  TTree* traj;
  int events,EventSeedCKF,EventTrackCKF,EventTriggCKF;

  TrackLocalAngle* theAngleFinder;
  uint layers;
  bool DEBUG;
  vector<uint> ActiveLay;
  std::vector<const SiStripRecHit2D*> vSiStripRecHit2D;
  TRandom2 RanGen2;

  // Tree declarations
  // All RecHit
  int RHNum;
  float	RHgpx[200];
  float	RHgpy[200];
  float	RHgpz[200];
  int RHMod[200];
  int RphiSte[200];

  // Track RecHit
  int TRHNum;
  float	TRHgpx[200];
  float	TRHgpy[200];
  float	TRHgpz[200];
  int TRHLay[200];
  uint TRHMod[200];

  // Modules
  float ModGlbX, ModGlbY, ModGlbZ, trackAngle;
  float ModLocX, ModLocY, ModLocErrX, ModLocErrY;
  float Moddiscr1, Moddiscr2, Modres, ResSig;
  uint ModIsBad; uint Id; uint SiStripQualBad; uint rod;
  uint run; uint event;

};


//#endif
