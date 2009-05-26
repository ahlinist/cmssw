#ifndef GsfElectronAlgo_H
#define GsfElectronAlgo_H

/** \class GsfElectronAlgo

  Top algorithm producing GsfElectron objects from supercluster driven Gsf tracking

  \author U.Berthon, C.Charlot, LLR Palaiseau

  \version   2nd Version Oct 10, 2006

 ************************************************************/

//class ElectronHcalHelper ;

#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaTowerIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaRecHitIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/ElectronTkIsolation.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCoreFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "TrackingTools/MaterialEffects/interface/PropagatorWithMaterial.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"

#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include <list>

class MultiTrajectoryStateTransform;
class MultiTrajectoryStateMode;
//class GsfConstraintAtVertex;
#include "TrackingTools/GsfTracking/interface/GsfConstraintAtVertex.h"

class GsfElectronAlgo {

  public:

    GsfElectronAlgo(
      const edm::ParameterSet & conf,
      double minSCEtBarrel, double minSCEtEndcaps,
      double maxEOverPBarrel, double maxEOverPEndcaps,
      double minEOverPBarrel, double minEOverPEndcaps,
      double maxDeltaEtaBarrel, double maxDeltaEtaEndcaps,
      double maxDeltaPhiBarrel,double maxDeltaPhiEndcaps,
      double maxSigmaIetaIetaBarrel, double maxSigmaIetaIetaEndcaps,
      double maxFbremBarrel, double maxFbremEndcaps,
      bool isBarrel, bool isEndcaps, bool isFiducial,
      bool seedFromTEC,
      double minMVA, double maxTIP,
      double minSCEtBarrelPflow, double minSCEtEndcapsPflow,
      double maxEOverPBarrelPflow, double maxEOverPEndcapsPflow,
      double minEOverPBarrelPflow, double minEOverPEndcapsPflow,
      double maxDeltaEtaBarrelPflow, double maxDeltaEtaEndcapsPflow,
      double maxDeltaPhiBarrelPflow,double maxDeltaPhiEndcapsPflow,
      double hOverEConeSizePflow, double hOverEPtMinPflow,
      double maxHOverEDepth1BarrelPflow, double maxHOverEDepth1EndcapsPflow,
      double maxHOverEDepth2Pflow,
      double maxSigmaIetaIetaBarrelPflow, double maxSigmaIetaIetaEndcapsPflow,
      double maxFbremBarrelPflow, double maxFbremEndcapsPflow,
      bool isBarrelPflow, bool isEndcapsPflow, bool isFiducialPflow,
      double minMVAPflow, double maxTIPPflow,
      bool applyPreselection, bool applyEtaCorrection, bool applyAmbResolution,
      bool addPflowElectrons,
      double intRadiusTk, double ptMinTk, double maxVtxDistTk, double maxDrbTk,
      double intRadiusHcal, double etMinHcal,
      double intRadiusEcalBarrel, double intRadiusEcalEndcaps, double jurassicWidth,
      double etMinBarrel, double eMinBarrel, double etMinEndcaps, double eMinEndcaps,
      bool vetoClustered, bool useNumCrystals) ;

    ~GsfElectronAlgo() ;

    void setupES( const edm::EventSetup & setup ) ;
    void run( edm::Event &, reco::GsfElectronCollection & ) ;

  private :

    // temporary collection of electrons
    typedef std::list<reco::GsfElectron *> GsfElectronPtrCollection ;

    // create electrons from superclusters, tracks and Hcal rechits
    void process
     ( //edm::Handle<reco::GsfTrackCollection> gsfTracksH,
       edm::Handle<reco::GsfElectronCoreCollection> coresH,
       edm::Handle<reco::TrackCollection> ctfTracksH,
       edm::Handle<edm::ValueMap<float> > pfMVAH,
       edm::Handle<CaloTowerCollection> towersH,
       edm::Handle<EcalRecHitCollection> reducedEBRecHits,
       edm::Handle<EcalRecHitCollection> reducedEERecHits,
       const reco::BeamSpot &bs,
       GsfElectronPtrCollection & outEle);

    // interface to be improved...
    void createElectron
     ( const reco::GsfElectronCoreRef & coreRef,
       const reco::CaloClusterPtr & elbcRef,
       const reco::TrackRef & ctfTrackRef, const float shFracInnerHits,
       double HoE1, double HoE2,
       ElectronTkIsolation & tkIso03, ElectronTkIsolation & tkIso04,
       EgammaTowerIsolation & had1Iso03, EgammaTowerIsolation & had2Iso03,
       EgammaTowerIsolation & had1Iso04, EgammaTowerIsolation & had2Iso04,
       EgammaRecHitIsolation & ecalBarrelIso03,EgammaRecHitIsolation & ecalEndcapsIso03,
       EgammaRecHitIsolation & ecalBarrelIso04,EgammaRecHitIsolation & ecalEndcapsIso04,
       edm::Handle<EcalRecHitCollection> reducedEBRecHits,edm::Handle<EcalRecHitCollection> reducedEERecHits,
       float mva, GsfElectronPtrCollection & outEle ) ;

    void preselectElectrons( GsfElectronPtrCollection &, GsfElectronPtrCollection & outEle, const reco::BeamSpot& ) ;

    //void resolveElectrons( GsfElectronPtrCollection &, reco::GsfElectronCollection & outEle ) ;
    void resolveElectrons( GsfElectronPtrCollection &, reco::GsfElectronCollection & outEle,
       edm::Handle<EcalRecHitCollection> & reducedEBRecHits,
       edm::Handle<EcalRecHitCollection> & reducedEERecHits );

    //Gsf mode calculations
    GlobalVector computeMode(const TrajectoryStateOnSurface &tsos);

    // associations
    const reco::SuperClusterRef getTrSuperCluster(const reco::GsfTrackRef & trackRef);

    const reco::CaloClusterPtr getEleBasicCluster(const reco::GsfTrackRef &
     trackRef, const reco::SuperCluster *scRef);

    // From Puneeth Kalavase : returns the CTF track that has the highest fraction
    // of shared hits in Pixels and the inner strip tracker with the electron Track
    std::pair<reco::TrackRef,float> getCtfTrackRef
     ( const reco::GsfTrackRef &, edm::Handle<reco::TrackCollection> ctfTracksH ) ;
    int sharedHits ( const reco::GsfTrackRef &, const reco::GsfTrackRef & ) ;
    int sharedDets ( const reco::GsfTrackRef &, const reco::GsfTrackRef & ) ;
    float sharedEnergy ( const reco::CaloCluster *, const reco::CaloCluster*, 
       edm::Handle<EcalRecHitCollection> & reducedEBRecHits,
       edm::Handle<EcalRecHitCollection> & reducedEERecHits );
    float sharedEnergy ( const reco::SuperClusterRef &, const reco::SuperClusterRef &, 
       edm::Handle<EcalRecHitCollection> & reducedEBRecHits,
       edm::Handle<EcalRecHitCollection> & reducedEERecHits );

    // intermediate calculations
    bool calculateTSOS(const reco::GsfTrack &t,const reco::SuperCluster & theClus, const
     reco::BeamSpot& bs);

    // preselection parameters (ecal driven electrons)
    // minimum SC Et
    double minSCEtBarrel_;
    double minSCEtEndcaps_;
    // maximum E/p where E is the supercluster corrected energy and p the track momentum at innermost state
    double maxEOverPBarrel_;
    double maxEOverPEndcaps_;
    // minimum E/p where E is the supercluster corrected energy and p the track momentum at innermost state
    double minEOverPBarrel_;
    double minEOverPEndcaps_;
    // maximum eta difference between the supercluster position and the track position at the closest impact to the supercluster
    double maxDeltaEtaBarrel_;
    double maxDeltaEtaEndcaps_;
    // maximum phi difference between the supercluster position and the track position at the closest impact to the supercluster
    // position to the supercluster
    double maxDeltaPhiBarrel_;
    double maxDeltaPhiEndcaps_;

    // H/E evaluation
    //bool useHcalRecHits_ ;
    //ElectronHcalHelper * hcalHelper_ ; // if use rechits
    //bool useHcalTowers_ ;
    edm::InputTag hcalTowers_;      // parameter if use towers
    double hOverEConeSize_;         // parameter if use towers
    double hOverEPtMin_;            // parameter if use towers : min tower Et for H/E evaluation
    double maxHOverEDepth1Barrel_;  // parameter if use towers : maximum H/E for depth1
    double maxHOverEDepth1Endcaps_; // parameter if use towers : maximum H/E for depth1
    double maxHOverEDepth2_;        // parameter if use towers : maximum H/E for depth2

    // maximum sigma ieta ieta
    double maxSigmaIetaIetaBarrel_;
    double maxSigmaIetaIetaEndcaps_;
    // maximum fbrem
    double maxFbremBarrel_;
    double maxFbremEndcaps_;
    // fiducial regions
    bool isBarrel_;
    bool isEndcaps_;
    bool isFiducial_;
    // select electrons with seed second hit in TEC layers
    bool seedFromTEC_;
    // BDT output (if available)
    double minMVA_;
    // transverse impact parameter wrt beam spot
    double maxTIP_;

    // preselection parameters (tracker driven only electrons)
    // minimum SC Et
    double minSCEtBarrelPflow_;
    double minSCEtEndcapsPflow_;
    // maximum E/p where E is the pflow supercluster energy and p the track momentum at innermost state
    double maxEOverPBarrelPflow_;
    double maxEOverPEndcapsPflow_;
    // minimum E/p where E is the pflow supercluster energy and p the track momentum at innermost state
    double minEOverPBarrelPflow_;
    double minEOverPEndcapsPflow_;
    // maximum eta difference between the pflow supercluster position and the track position at the closest impact to the supercluster
    double maxDeltaEtaBarrelPflow_;
    double maxDeltaEtaEndcapsPflow_;
    // maximum phi difference between the pflow supercluster position and the track position at the closest impact to the supercluster
    // position to the supercluster
    double maxDeltaPhiBarrelPflow_;
    double maxDeltaPhiEndcapsPflow_;
    // cone size for H/E evaluation
    double hOverEConeSizePflow_;
    // min tower Et for H/E evaluation
    double hOverEPtMinPflow_;
    // maximum H/E for depth1
    double maxHOverEDepth1BarrelPflow_;
    double maxHOverEDepth1EndcapsPflow_;
    // maximum H/E for depth2
    double maxHOverEDepth2Pflow_;
    // maximum sigma ieta ieta
    double maxSigmaIetaIetaBarrelPflow_;
    double maxSigmaIetaIetaEndcapsPflow_;
    // maximum fbrem
    double maxFbremBarrelPflow_;
    double maxFbremEndcapsPflow_;
    // fiducial regions
    bool isBarrelPflow_;
    bool isEndcapsPflow_;
    bool isFiducialPflow_;
    // BDT output
    double minMVAPflow_;
    // transverse impact parameter wrt beam spot
    double maxTIPPflow_;


    // if this parameter is true, electron preselection is applied
    bool applyPreselection_;

    // if this parameter is true, electron level escale corrections are used on top
    // of the cluster level corrections
    bool applyEtaCorrection_;

    // if this parameter is true, "double" electrons are resolved
    bool applyAmbResolution_;

    // if this parameter is true, trackerDriven electrons are added
    bool addPflowElectrons_;

    // isolation variables parameters
    double intRadiusTk_;
    double ptMinTk_;
    double maxVtxDistTk_;
    double maxDrbTk_;
    double intRadiusHcal_;
    double etMinHcal_;
    double intRadiusEcalBarrel_;
    double intRadiusEcalEndcaps_;
    double jurassicWidth_;
    double etMinBarrel_;
    double eMinBarrel_;
    double etMinEndcaps_;
    double eMinEndcaps_;
    bool vetoClustered_;
    bool useNumCrystals_;

    // input configuration
    edm::InputTag barrelSuperClusters_;
    edm::InputTag endcapSuperClusters_;
    //edm::InputTag tracks_;
    edm::InputTag gsfElectronCores_ ;
    edm::InputTag ctfTracks_;
    edm::InputTag reducedBarrelRecHitCollection_ ;
    edm::InputTag reducedEndcapRecHitCollection_ ;
    edm::InputTag pfMVA_;


    edm::ESHandle<MagneticField>                theMagField;
    edm::ESHandle<CaloGeometry>                 theCaloGeom;
    edm::ESHandle<CaloTopology>                 theCaloTopo;
    edm::ESHandle<TrackerGeometry>              trackerHandle_;

    const MultiTrajectoryStateTransform *mtsTransform_;
    const MultiTrajectoryStateMode *mtsMode_;
    GsfConstraintAtVertex *constraintAtVtx_;

    // internal variables
    int subdet_; //subdetector for this cluster
    GlobalPoint sclPos_;
    GlobalVector vtxMom_;
    TrajectoryStateOnSurface innTSOS_;
    TrajectoryStateOnSurface outTSOS_;
    TrajectoryStateOnSurface vtxTSOS_;
    TrajectoryStateOnSurface sclTSOS_;
    TrajectoryStateOnSurface seedTSOS_;
    TrajectoryStateOnSurface eleTSOS_;
    TrajectoryStateOnSurface constrainedVtxTSOS_;

    unsigned long long cacheIDGeom_;
    unsigned long long cacheIDTopo_;
    unsigned long long cacheIDTDGeom_;
    unsigned long long cacheIDMagField_;

 } ;

struct innermost_electron {

  innermost_electron(edm::ESHandle<TrackerGeometry> &geom): trackerHandle_(geom) {}

  bool operator()( const reco::GsfElectron * e1, const reco::GsfElectron * e2 )
   { 
    reco::HitPattern gsfHitPattern1 = e1->gsfTrack()->hitPattern();
    reco::HitPattern gsfHitPattern2 = e2->gsfTrack()->hitPattern();
    // retreive first valid hit
    int gsfHitCounter1 = 1;
    for(trackingRecHit_iterator elHitsIt1 = e1->gsfTrack()->recHitsBegin();
          elHitsIt1 != e1->gsfTrack()->recHitsEnd(); elHitsIt1++, gsfHitCounter1++) {
	if(((**elHitsIt1).isValid())) break;
    }
    int gsfHitCounter2 = 1;
    for(trackingRecHit_iterator elHitsIt2 = e2->gsfTrack()->recHitsBegin();
          elHitsIt2 != e2->gsfTrack()->recHitsEnd(); elHitsIt2++, gsfHitCounter2++) {
	if(((**elHitsIt2).isValid())) break;
    } 
    uint32_t gsfHit1 = gsfHitPattern1.getHitPattern(gsfHitCounter1);
    uint32_t gsfHit2 = gsfHitPattern2.getHitPattern(gsfHitCounter2);
    if (gsfHitPattern1.getSubStructure(gsfHit1)!=gsfHitPattern2.getSubStructure(gsfHit2)) {
     return (gsfHitPattern1.getSubStructure(gsfHit1)<gsfHitPattern2.getSubStructure(gsfHit2)); 
    } else if (gsfHitPattern1.getLayer(gsfHit1)!=gsfHitPattern2.getLayer(gsfHit2)) {
     return gsfHitPattern1.getLayer(gsfHit1)<gsfHitPattern2.getLayer(gsfHit2);
    } else {
    return (fabs(e1->eSuperClusterOverP()-1)<fabs(e2->eSuperClusterOverP()-1));
   }
 }

 edm::ESHandle<TrackerGeometry> trackerHandle_;
 
};

#endif // GsfElectronAlgo_H


