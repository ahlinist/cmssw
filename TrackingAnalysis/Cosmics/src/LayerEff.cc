// -*- C++ -*-
//
// Package:    LayerEff
// Class:      LayerEff
// 
/**\class LayerEff LayerEff.cc TrackingAnalysis/Cosmics/src/LayerEff.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  "Keith Ulmer"
//         Created:  Fri Mar 20 03:16:50 CDT 2009
// $Id: LayerEff.cc,v 1.1 2009/03/22 22:01:59 kaulmer Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CalibTracker/Records/interface/SiStripQualityRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripQuality.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h" 
#include "TrackingAnalysis/Cosmics/interface/TrajectoryAtValidHit.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"

#include "TFile.h"
#include "TTree.h"
//
// class decleration
//

class LayerEff : public edm::EDAnalyzer {
   public:
      explicit LayerEff(const edm::ParameterSet&);
      ~LayerEff();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

  // ----------member data ---------------------------
  edm::ParameterSet config_;
  edm::InputTag trackTag_;
  TTree* rootTree_;
  uint layer_;
  uint valid_;
  uint module_;
  uint qualBad_;
  float TrajGlbX, TrajGlbY, TrajGlbZ;
  float TrajLocX, TrajLocY, TrajLocErrX, TrajLocErrY, TrajLocAngleX, TrajLocAngleY;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
LayerEff::LayerEff(const edm::ParameterSet& iConfig) :
  config_(iConfig), rootTree_(0) 
{
   //now do what ever initialization is needed
    trackTag_ = iConfig.getParameter<edm::InputTag>("tracks");
}


LayerEff::~LayerEff()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
LayerEff::analyze(const edm::Event& e, const edm::EventSetup& es)
{
  using namespace edm;
  using namespace std;

  //  edm::ParameterSet config_;
  
  //Combinatorial Tracks
  Handle<reco::TrackCollection> trackCollectionCKF;
  edm::InputTag TkTagCKF = config_.getParameter<edm::InputTag>("tracks");
  e.getByLabel(TkTagCKF,trackCollectionCKF);

  //Trajectories
  edm::Handle<std::vector<Trajectory> > TrajectoryCollectionCKF;
  edm::InputTag TkTrajCKF = config_.getParameter<edm::InputTag>("trajectories");
  e.getByLabel(TkTrajCKF,TrajectoryCollectionCKF);

  // get the SiStripQuality records
  edm::ESHandle<SiStripQuality> SiStripQuality_;
  es.get<SiStripQualityRcd>().get(SiStripQuality_);

  //get tracker geometry
  edm::ESHandle<TrackerGeometry> tracker;
  es.get<TrackerDigiGeometryRecord>().get(tracker);
  const TrackerGeometry * tkgeom=&(* tracker);

  edm::ESHandle<MagneticField> magFieldHandle;
  es.get<IdealMagneticFieldRecord>().get(magFieldHandle);
  const MagneticField* magField_ = magFieldHandle.product();


  const   reco::TrackCollection *tracksCKF=trackCollectionCKF.product();
  if (tracksCKF->size() == 1 ){
    reco::TrackCollection::const_iterator iCKF=trackCollectionCKF.product()->begin();
    uint previousLayer = 0;

    const Trajectory traject = *(TrajectoryCollectionCKF.product()->begin());
    std::vector<TrajectoryMeasurement> TMeas=traject.measurements();
    vector<TrajectoryMeasurement>::iterator itm;
    TrajGlbX = 0.0; TrajGlbY = 0.0; TrajGlbZ = 0.0;
    TrajLocX = 0.0; TrajLocY = 0.0; TrajLocErrX = 0.0; TrajLocErrY = 0.0; 
    TrajLocAngleX = -999.0; TrajLocAngleY = -999.0;
    
    for (itm=TMeas.begin();itm!=TMeas.end();itm++){
      if(! itm->updatedState().isValid()) continue;

      ConstReferenceCountingPointer<TransientTrackingRecHit> theHit;
      theHit = (*itm).recHit();
      
      uint iidd = theHit->geographicalId().rawId();
      
      StripSubdetector strip=StripSubdetector(iidd);
      unsigned int subid=strip.subdetId();
      uint TKlayers = 0;
      if (subid ==  StripSubdetector::TIB) { 
	TIBDetId tibid(iidd);
	TKlayers = tibid.layer();
      }
      if (subid ==  StripSubdetector::TOB) { 
	TOBDetId tobid(iidd);
	TKlayers = tobid.layer() + 4 ; 
      }
      if (subid ==  StripSubdetector::TID) { 
	TIDDetId tidid(iidd);
	TKlayers = tidid.wheel() + 10;
      }
      if (subid ==  StripSubdetector::TEC) { 
	TECDetId tecid(iidd);
	TKlayers = tecid.wheel() + 13 ; 
      }

      if ( SiStripQuality_->getBadApvs(iidd)!=0 ) {
	qualBad_ = 1; 
      } else {
	qualBad_ = 0; 
      }
      
      cout << "rechit in layer = " << TKlayers << " with valid status = " << theHit->isValid() << " and previous layer = " << previousLayer << endl;


      // Make AnalyticalPropagator to use in TAVH constructor
      AnalyticalPropagator propagator(magField_,anyDirection); 
      TrajectoryAtValidHit TM(*itm,tkgeom, propagator, 1);

      layer_ = TKlayers;
      valid_ = theHit->isValid();
      module_ = iidd;

      TrajGlbX = TM.globalX();
      TrajGlbY = TM.globalY();
      TrajGlbZ = TM.globalZ();
      TrajLocX = TM.localX();
      TrajLocY = TM.localY();
      TrajLocErrX = TM.localErrorX();
      TrajLocErrY = TM.localErrorY();
      TrajLocAngleX = atan( TM.localDxDz() );
      TrajLocAngleY = atan( TM.localDyDz() );
      
      if (TKlayers!=previousLayer)     rootTree_->Fill();
      
      previousLayer = TKlayers;
    }
  }
}


// ------------ method called once each job just before starting event loop  ------------
void 
LayerEff::beginJob(const edm::EventSetup&)
{
  edm::Service<TFileService> fs;
  //
  // root output
  //
  rootTree_ = fs->make<TTree>("LayerEff","LayerEff");
  rootTree_->Branch("layer",&layer_,"layer/i");
  rootTree_->Branch("valid",&valid_,"valid/i");
  rootTree_->Branch("modules",&module_,"module/i");
  rootTree_->Branch("qualBad",&qualBad_,"qualBad/i");
  rootTree_->Branch("TrajGlbX",&TrajGlbX,"TrajGlbX/F");
  rootTree_->Branch("TrajGlbY",&TrajGlbY,"TrajGlbY/F");
  rootTree_->Branch("TrajGlbZ",&TrajGlbZ,"TrajGlbZ/F");
  rootTree_->Branch("TrajLocX",&TrajLocX,"TrajLocX/F");
  rootTree_->Branch("TrajLocY",&TrajLocY,"TrajLocY/F");
  rootTree_->Branch("TrajLocErrX",&TrajLocErrX,"TrajLocErrX/F");
  rootTree_->Branch("TrajLocErrY",&TrajLocErrY,"TrajLocErrY/F");
  rootTree_->Branch("TrajLocAngleX",&TrajLocAngleX,"TrajLocAngleX/F");
  rootTree_->Branch("TrajLocAngleY",&TrajLocAngleY,"TrajLocAngleY/F");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
LayerEff::endJob() {
  if ( rootTree_ ) {
    rootTree_->GetDirectory()->cd();
    rootTree_->Write();
    delete rootTree_;
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(LayerEff);
