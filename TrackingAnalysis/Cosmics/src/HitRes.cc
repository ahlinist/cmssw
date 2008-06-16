// -*- C++ -*-
//
// Package:    TrackingAnalysis/Cosmics
// Class:      HitRes
// 
/**\class TrackingAnalysis/Cosmics HitRes.cc TrackingAnalysis/Cosmics/src/HitRes.cc

 Description: <one line class summary>
Use overlaps in TIF cosmics data to evaluate hit resolution

 Implementation:
See sample cfg files in TrackingAnalysis/Cosmics/test/hitRes*cfg
*/
//
// Original Authors:  Wolfgang Adam, Keith Ulmer
//         Created:  Thu Oct 11 14:53:32 CEST 2007
// $Id: HitRes.cc,v 1.4 2008/06/12 09:11:47 adamwo Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include  "DataFormats/TrackReco/interface/TrackFwd.h"
#include  "DataFormats/TrackReco/interface/Track.h"
#include  "DataFormats/TrackReco/interface/TrackExtra.h"
#include  "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/DetId/interface/DetId.h" 
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/GluedGeomDet.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "RecoTracker/TransientTrackingRecHit/interface/TSiStripMatchedRecHit.h"
#include "RecoTracker/TransientTrackingRecHit/interface/ProjectedRecHit2D.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"

#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "TrackingTools/AnalyticalJacobians/interface/JacobianLocalToCurvilinear.h"
#include "TrackingTools/AnalyticalJacobians/interface/JacobianCurvilinearToLocal.h"
#include "TrackingTools/AnalyticalJacobians/interface/AnalyticalCurvilinearJacobian.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "TFile.h"
#include "TTree.h"

#include <vector>
#include <utility>

//
// class decleration
//


class HitRes : public edm::EDAnalyzer {
public:
  explicit HitRes(const edm::ParameterSet&);
  ~HitRes();

private:
  typedef TransientTrackingRecHit::ConstRecHitPointer ConstRecHitPointer;
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  virtual void analyze(const Trajectory&, const Propagator&, TrackerHitAssociator&);
  int layerFromId (const DetId&) const;

      // ----------member data ---------------------------
  edm::ParameterSet config_;
  edm::InputTag trajectoryTag_;
  bool doSimHit_;
  const TrackerGeometry* trackerGeometry_;
  const MagneticField* magField_;

  TrajectoryStateCombiner combiner_;
  int overlapCounts_[3];

  TFile* rootFile_;
  TTree* rootTree_;
  float overlapPath_;
  unsigned short hitCounts_[3];
  float chi2_;
  unsigned int overlapIds_[2];
  float predictedPositions_[3][2];
  float predictedLocalParameters_[5][2];
  float predictedLocalErrors_[5][2];
  float predictedDeltaXError_;
  char relativeXSign_;
  float hitPositions_[2];
  float hitErrors_[2];
  float simHitPositions_[2];
  
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

using std::vector;
using std::cout;
using std::endl;
//
// constructors and destructor
//
HitRes::HitRes(const edm::ParameterSet& iConfig) :
  config_(iConfig), rootFile_(0), rootTree_(0) {
    //now do what ever initialization is needed
    trajectoryTag_ = iConfig.getParameter<edm::InputTag>("trajectories");
    doSimHit_ = iConfig.getParameter<bool>("associateStrip");

    overlapCounts_[0] = 0;  // #trajectories
    overlapCounts_[1] = 0;  // #hits
    overlapCounts_[2] = 0;  // #overlap hits
  }


HitRes::~HitRes()
{
  
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
  cout << "Counters =" ;
  for ( int i=0; i<3; ++i )  cout << " " << overlapCounts_[i];
  cout << endl;
}


//
// member functions
//


// ------------ method called to for each event  ------------
void
HitRes::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  //
  // mag field & search tracker
  //
  edm::ESHandle<MagneticField> magFieldHandle;
  iSetup.get<IdealMagneticFieldRecord>().get(magFieldHandle);
  magField_ = magFieldHandle.product();
  //
  // propagator
  //
  AnalyticalPropagator propagator(magField_,anyDirection);
  //
  // geometry
  //
  edm::ESHandle<TrackerGeometry> geometryHandle;
  iSetup.get<TrackerDigiGeometryRecord>().get(geometryHandle);
  trackerGeometry_ = geometryHandle.product();
  //
  // make associator for SimHits
  //
  //TrackerHitAssociator
  TrackerHitAssociator* associator;
  if(doSimHit_) associator = new TrackerHitAssociator(iEvent, config_); else associator = 0; 

  //
  // trajectories (from refit)
  //
  typedef vector<Trajectory> TrajectoryCollection;
  edm::Handle<TrajectoryCollection> trajectoryCollectionHandle;
  iEvent.getByLabel(trajectoryTag_,trajectoryCollectionHandle);
  const TrajectoryCollection* trajectoryCollection = trajectoryCollectionHandle.product();
//   edm::LogInfo("HitRes") 
//     << " Found " << trajectoryCollection->size() << " trajectories";

  //
  // loop over trajectories from refit
  //
  if ( trajectoryCollection->size()!=1 )  return;
  for ( TrajectoryCollection::const_iterator it=trajectoryCollection->begin();
	it!=trajectoryCollection->end(); ++it )  analyze(*it,propagator,*associator);

}


void
HitRes::analyze (const Trajectory& trajectory,
		     const Propagator& propagator,
		     TrackerHitAssociator& associator)
{
  typedef std::pair<const TrajectoryMeasurement*, const TrajectoryMeasurement*> Overlap;
  typedef vector<Overlap> OverlapContainer;
  ++overlapCounts_[0];


  const TrajectoryMeasurement* previousTM(0);
  DetId previousId(0);
  int previousLayer(-1);
  OverlapContainer overlapHits;
  //
  // quality cuts on trajectory
  // min. # hits / matched hits, barrel only
  //
  unsigned int nhMatched(0);
  vector<TrajectoryMeasurement> measurements(trajectory.measurements());
  for ( vector<TrajectoryMeasurement>::const_iterator itm=measurements.begin();
	itm!=measurements.end(); ++itm ) {
    if ( !itm->recHit()->isValid() ) continue;
    if ( dynamic_cast<const TSiStripMatchedRecHit*>(&(*itm->recHit())) )  ++nhMatched;
    if ( itm->recHit()->geographicalId().subdetId()!=StripSubdetector::TIB &&
	 itm->recHit()->geographicalId().subdetId()!=StripSubdetector::TOB )  return;
  }
  if ( trajectory.foundHits()<6 || nhMatched<0 )  return;
  
  //
  // loop over measurements in the trajectory and calculate residuals
  //
  for ( vector<TrajectoryMeasurement>::const_iterator itm=measurements.begin();
	itm!=measurements.end(); ++itm ) {
    //
    // skip "invalid" (i.e. missing) hits
    //
    ConstRecHitPointer hit = itm->recHit();
    if ( !hit->isValid() ) {
      previousTM = 0;
      continue;
    }
    //
    // check for overlap: same subdet-id && layer number for
    // two consecutive hits
    //
    ++overlapCounts_[1];
    DetId id = hit->geographicalId();
    int layer(-1);
    if ( previousTM==0 ) {
      layer = layerFromId(id);
    }
    else {
      if ( id.det()==previousId.det() &&
	   id.subdetId()==previousId.subdetId() ) {
	layer = layerFromId(id);
	if ( layer!=-1 && layer==previousLayer ) {
	  overlapHits.push_back(std::make_pair(previousTM,&(*itm)));
	  edm::LogVerbatim("HitRes") << "adding overlap pair from layer = " << layer;
	}
      }
    }
    previousTM = &(*itm);
    previousId = id;
    previousLayer = layer;
  }
  //
  // Loop over all overlap pairs. 
  //
  hitCounts_[0] = trajectory.foundHits();
  hitCounts_[1] = trajectory.lostHits();
  hitCounts_[2] = nhMatched;
  chi2_ = trajectory.chiSquared();
  for ( OverlapContainer::const_iterator iol=overlapHits.begin();
 	iol!=overlapHits.end(); ++iol ) {
    // 
    // verify minimum number of matched hits (excluding the overlap) REMOVED for now.
    //
    int nhm(nhMatched);
    if ( dynamic_cast<const TSiStripMatchedRecHit*>(&(*(*iol).first->recHit())) )  --nhm;
    if ( dynamic_cast<const TSiStripMatchedRecHit*>(&(*(*iol).second->recHit())) )  --nhm;
    if ( nhm<0 )  continue;
    //
    // Check for non-overlaping rphi and stereo due to position in corner of detector, and
    // remove such hits for stereo detectors

    if ( dynamic_cast<const ProjectedRecHit2D*>(&(*(*iol).first->recHit())) ){
      //cout << "found glued layer hit" << endl;
      const ProjectedRecHit2D* proj1 = dynamic_cast<const ProjectedRecHit2D*>(&(*(*iol).first->recHit()));
      const GeomDet* det1 = proj1->originalDet();
      const DetId id1 = det1->geographicalId();
      //if((id1.rawId() & 0x3)==0) cout << "hit is matched" << endl;
      if((id1.rawId() & 0x3)==1) continue; //hit is stereo
      //if((id1.rawId() & 0x3)==2) cout << "hit is rphi" << endl;
    }
    if ( dynamic_cast<const ProjectedRecHit2D*>(&(*(*iol).second->recHit())) ) {
      const ProjectedRecHit2D* proj2 = dynamic_cast<const ProjectedRecHit2D*>(&(*(*iol).second->recHit()));
      const GeomDet* det2 = proj2->originalDet();
      const DetId id2 = det2->geographicalId();
      if((id2.rawId() & 0x3)==1) continue; //hit is stereo
    }

    
    //              
    // create reference state @ module 1 (no info from overlap hits)
    //
    ++overlapCounts_[2];
    // forward and backward predicted states at module 1
//     TrajectoryStateOnSurface fwdPred1 = (*iol).first->forwardPredictedState();
//     TrajectoryStateOnSurface bwdPred1 = (*iol).first->backwardPredictedState();
    TrajectoryStateOnSurface fwdPred1 = (*iol).first->backwardPredictedState();
    TrajectoryStateOnSurface bwdPred1 = (*iol).first->forwardPredictedState();
    if ( !fwdPred1.isValid() || !bwdPred1.isValid() )  continue;
    // backward predicted state at module 2
//     TrajectoryStateOnSurface bwdPred2 = (*iol).second->backwardPredictedState();
    TrajectoryStateOnSurface bwdPred2 = (*iol).second->forwardPredictedState();
    if ( !bwdPred2.isValid() )  continue;
    // extrapolation bwdPred2 to module 1
    TrajectoryStateOnSurface bwdPred2At1 = propagator.propagate(bwdPred2,fwdPred1.surface());
    if ( !bwdPred2At1.isValid() )  continue;
    // combination with fwdPred1 (ref. state, best estimate without hits 1 and 2)
    TrajectoryStateOnSurface comb1 = combiner_.combine(fwdPred1,bwdPred2At1);
    if ( !comb1.isValid() )  continue;
    //
    // propagation of reference parameters to module 2
    //
    std::pair<TrajectoryStateOnSurface,double> tsosWithS = 
      propagator.propagateWithPath(comb1,bwdPred2.surface());
//     cout << "Path = " << tsosWithS.second << endl;
    TrajectoryStateOnSurface comb1At2 = tsosWithS.first;
    if ( !comb1At2.isValid() )  continue;
    overlapPath_ = tsosWithS.second;
    // global position on module 1
    GlobalPoint position = comb1.globalPosition();
    predictedPositions_[0][0] = position.x();
    predictedPositions_[1][0] = position.y();
    predictedPositions_[2][0] = position.z();
    // local parameters and errors on module 1
    AlgebraicVector5 pars = comb1.localParameters().vector();
    AlgebraicSymMatrix55 errs = comb1.localError().matrix();
    for ( int i=0; i<5; ++i ) {
      predictedLocalParameters_[i][0] = pars[i];
      predictedLocalErrors_[i][0] = sqrt(errs(i,i));
    }
    // global position on module 2
    position = comb1At2.globalPosition();
    predictedPositions_[0][1] = position.x();
    predictedPositions_[1][1] = position.y();
    predictedPositions_[2][1] = position.z();
    // local parameters and errors on module 2
    pars = comb1At2.localParameters().vector();
    errs = comb1At2.localError().matrix();
    for ( int i=0; i<5; ++i ) {
      predictedLocalParameters_[i][1] = pars[i];
      predictedLocalErrors_[i][1] = sqrt(errs(i,i));
    }
    //
    // jacobians (local-to-global@1,global 1-2,global-to-local@2)
    //
    JacobianLocalToCurvilinear jacLocToCurv(comb1.surface(),
					    comb1.localParameters(),
					    *magField_);
    AnalyticalCurvilinearJacobian jacCurvToCurv(comb1.globalParameters(),
						comb1At2.globalPosition(),
						comb1At2.globalMomentum(),
						tsosWithS.second);
    JacobianCurvilinearToLocal jacCurvToLoc(comb1At2.surface(),
					    comb1At2.localParameters(),
					    *magField_);
    // combined jacobian local-1-to-local-2
    AlgebraicMatrix55 jacobian = 
      jacLocToCurv.jacobian()*jacCurvToCurv.jacobian()*jacCurvToLoc.jacobian();
    // covariance on module 1
    AlgebraicSymMatrix55 covComb1 = comb1.localError().matrix();
    // variance and correlations for predicted local_x on modules 1 and 2
    double c00 = covComb1(3,3);
    double c10(0.);
    double c11(0.);
    for ( int i=1; i<5; ++i ) {
      c10 += jacobian(3,i)*covComb1(i,3);
      for ( int j=1; j<5; ++j )  c11 += jacobian(3,i)*covComb1(i,j)*jacobian(3,j);
    }
    // choose relative sign in order to minimize error on difference
    //    double relSign = c10>0 ? -1 : 1;
    double diff = c00 - 2*fabs(c10) + c11;
    diff = diff>0 ? sqrt(diff) : -sqrt(-diff);
    predictedDeltaXError_ = diff;
    relativeXSign_ = c10>0 ? -1 : 1;
    // information on modules and hits
    overlapIds_[0] = (*iol).first->recHit()->geographicalId().rawId();
    overlapIds_[1] = (*iol).second->recHit()->geographicalId().rawId();
    //if its a matched hit fill hitPosition with the rphi x position, otherwise the
    //recHit position is fine (glued layer, stereo only hits are already gone)
    const TransientTrackingRecHit::ConstRecHitPointer firstRecHit = &(*(*iol).first->recHit());
    const SiStripMatchedRecHit2D* firstMatchedhit=dynamic_cast<const SiStripMatchedRecHit2D*>((*firstRecHit).hit());
    const TransientTrackingRecHit::ConstRecHitPointer secondRecHit = &(*(*iol).second->recHit());
    const SiStripMatchedRecHit2D* secondMatchedhit=dynamic_cast<const SiStripMatchedRecHit2D*>((*secondRecHit).hit());
    hitPositions_[0] = (*iol).first->recHit()->localPosition().x();
    hitErrors_[0] = sqrt((*iol).first->recHit()->localPositionError().xx());
    hitPositions_[1] = (*iol).second->recHit()->localPosition().x();
    hitErrors_[1] = sqrt((*iol).second->recHit()->localPositionError().xx());
    
    //try writing out the SimHit info (for MC only)
    if(doSimHit_){
      std::vector<PSimHit> psimHits1;
      std::vector<PSimHit> psimHits2;
      //calculate layer
      DetId id = (*iol).first->recHit()->geographicalId();
      int layer(-1);
      layer = layerFromId(id);
      edm::LogVerbatim("HitRes") << "layer = " << layer;
      
      if ( firstMatchedhit ) {
	edm::LogVerbatim("HitRes") << "rechit2D";//if matchedHit take the rphi hit only
	psimHits1 = associator.associateHit( *firstMatchedhit->monoHit() );
      } else {
	psimHits1 = associator.associateHit( *(*iol).first->recHit()->hit() );
	edm::LogVerbatim("HitRes") << "single hit ";
      }
      edm::LogVerbatim("HitRes") << "length of psimHits1: " << psimHits1.size();
      if ( !psimHits1.empty() ) {
	float closest_dist = 99999.9;
	std::vector<PSimHit>::const_iterator closest_simhit = psimHits1.begin();
	for (std::vector<PSimHit>::const_iterator m = psimHits1.begin(); m < psimHits1.end(); m++) {
	  //find closest simHit to the recHit
	  float simX = (*m).localPosition().x();
	  float dist = fabs( simX - ((*iol).first->recHit()->localPosition().x()) );
	  if (firstMatchedhit) dist = fabs( simX - firstMatchedhit->monoHit()->localPosition().x());
	  edm::LogVerbatim("HitRes") << "simHit1 simX = " << simX << "   hitX = " << (*iol).first->recHit()->localPosition().x() << "   distX = " << dist << "   layer = " << layer;
	  if (dist<closest_dist) {
	    //cout << "found newest closest dist for simhit1" << endl;
	    closest_dist = dist;
	    closest_simhit = m;
	  }
	}
	//if glued layer, convert sim hit position to matchedhit surface
	//layer index from 1-4 for TIB, 1-6 for TOB
	if ( layer==1||layer==2 ) {
	  const GluedGeomDet* gluedDet = (const GluedGeomDet*)(*trackerGeometry_).idToDet((*firstRecHit).hit()->geographicalId());
	  const StripGeomDetUnit* stripDet =(StripGeomDetUnit*) gluedDet->monoDet();
	  GlobalPoint gp = stripDet->surface().toGlobal( (*closest_simhit).localPosition() );
	  LocalPoint lp = gluedDet->surface().toLocal( gp );
	  LocalVector localdirection = (*closest_simhit).localDirection();
	  GlobalVector globaldirection = stripDet->surface().toGlobal(localdirection);
	  LocalVector direction = gluedDet->surface().toLocal(globaldirection);
	  float scale = -lp.z() / direction.z();
	  LocalPoint projectedPos = lp + scale*direction;
          simHitPositions_[0] =	projectedPos.x();
	  edm::LogVerbatim("HitRes") << "simhit position from matched layer = " << simHitPositions_[0];
	} else {
	  simHitPositions_[0] = (*closest_simhit).localPosition().x();
	  edm::LogVerbatim("HitRes") << "simhit position from non-matched layer = " << simHitPositions_[0];
	}
	edm::LogVerbatim("HitRes") << "hit position = " << hitPositions_[0];
      } else {
	simHitPositions_[0] = -99.;
	//cout << " filling simHitX: " << -99 << endl;
      }
      
      if (secondMatchedhit) {
	psimHits2 = associator.associateHit( *secondMatchedhit->monoHit() );
      } else {
	psimHits2 = associator.associateHit( *(*iol).second->recHit()->hit() );
      }
      if ( !psimHits2.empty() ) {
	float closest_dist = 99999.9;
	std::vector<PSimHit>::const_iterator closest_simhit = psimHits2.begin();
	for (std::vector<PSimHit>::const_iterator m = psimHits2.begin(); m < psimHits2.end(); m++) {
	  float simX = (*m).localPosition().x();
	  float dist = fabs( simX - ((*iol).second->recHit()->localPosition().x()) );
	  if (secondMatchedhit) dist = fabs( simX - secondMatchedhit->monoHit()->localPosition().x());
	  if (dist<closest_dist) {
	    closest_dist = dist;
	    closest_simhit = m;
	  }
	}
	//if glued layer, convert sim hit position to matchedhit surface
	if ( layer==1||layer==2 ) {
	  const GluedGeomDet* gluedDet = (const GluedGeomDet*)(*trackerGeometry_).idToDet((*secondRecHit).hit()->geographicalId());
	  const StripGeomDetUnit* stripDet =(StripGeomDetUnit*) gluedDet->monoDet();
	  GlobalPoint gp = stripDet->surface().toGlobal( (*closest_simhit).localPosition() );
	  LocalPoint lp = gluedDet->surface().toLocal( gp );
	  LocalVector localdirection = (*closest_simhit).localDirection();
	  GlobalVector globaldirection = stripDet->surface().toGlobal(localdirection);
	  LocalVector direction = gluedDet->surface().toLocal(globaldirection);
	  float scale = -lp.z() / direction.z();
	  LocalPoint projectedPos = lp + scale*direction;
          simHitPositions_[1] =	projectedPos.x();
	} else {
	  simHitPositions_[1] = (*closest_simhit).localPosition().x();
	}
      } else {
	simHitPositions_[1] = -99.;
      }
    }
    
    rootTree_->Fill();
//     cout << "DiffErr " 
// 	 << c00 << " " << c10 << " " << c11 << " " << diff << std::endl;
//     cout << "Differences = " 
// 	 << comb1.localPosition().x()+relSign*comb1At2.localPosition().x() << " "
// 	 << (*iol).first->recHit()->localPosition().x()+
//       relSign*(*iol).second->recHit()->localPosition().x() << endl;
  }
}

int
HitRes::layerFromId (const DetId& id) const
{
  if ( id.subdetId()==StripSubdetector::TIB ) {
    TIBDetId tibId(id);
    return tibId.layer();
  }
  else if ( id.subdetId()==StripSubdetector::TOB ) {
    TOBDetId tobId(id);
    return tobId.layer();
  }
  return -1;
}

// ------------ method called once each job just before starting event loop  ------------
void 
HitRes::beginJob(const edm::EventSetup&)
{
  //
  // root output
  //
  rootFile_ = new TFile("HitRes.root","recreate");
  rootTree_ = new TTree("Overlaps","Overlaps");
  rootTree_->Branch("hitCounts",hitCounts_,"found/s:lost/s:matched/s");
  rootTree_->Branch("chi2",&chi2_,"chi2/F");
  rootTree_->Branch("path",&overlapPath_,"path/F");
  rootTree_->Branch("detids",overlapIds_,"id[2]/i");
  rootTree_->Branch("predPos",predictedPositions_,"gX[2]/F:gY[2]/F:gZ[2]/F");
  rootTree_->Branch("predPar",predictedLocalParameters_,
		    "predQP[2]/F:predDX[2]/F:predDY[2]/F:predX[2]/F:predY[2]/F");
  rootTree_->Branch("predErr",predictedLocalErrors_,
		    "predEQP[2]/F:predEDX[2]/F:predEDY[2]/F:predEX[2]/F:predEY[2]/F");
  rootTree_->Branch("predEDeltaX",&predictedDeltaXError_,"sigDeltaX/F");
  rootTree_->Branch("relSignX",&relativeXSign_,"relSignX/B");
  rootTree_->Branch("hitX",hitPositions_,"hitX[2]/F");
  rootTree_->Branch("hitEX",hitErrors_,"hitEX[2]/F");
  rootTree_->Branch("simX",simHitPositions_,"simX[2]/F");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HitRes::endJob() {
  if ( rootTree_ ) {
    rootTree_->GetDirectory()->cd();
    rootTree_->Write();
    delete rootTree_;
    delete rootFile_;
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(HitRes);
