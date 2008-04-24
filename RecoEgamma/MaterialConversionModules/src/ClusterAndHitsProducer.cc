#include <vector>
#include "RecoEgamma/MaterialConversionModules/interface/ClusterAndHitsProducer.h"
#include "AnalysisDataFormats/Egamma/interface/ClusterAndHits.h"
#include "AnalysisDataFormats/Egamma/interface/ClusterAndHitsFwd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h" 
#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h" 
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h" 
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/SiStripCluster/interface/SiStripClusterCollection.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "TrackingTools/RoadSearchHitAccess/interface/DetHitAccess.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"
#include <TMath.h>
#include <iostream>
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/Common/interface/DetSet.h"

using namespace reco;
ClusterAndHitsProducer::ClusterAndHitsProducer(const edm::ParameterSet& ps)
{
  
  clusterandhitsCollection_ = ps.getParameter<std::string>("clusterandhitsCollection");
  hybridsuperclusterCollection_ = ps.getParameter<std::string>("hybridsuperclusterCollection");
  hybridsuperclusterProducer_ = ps.getParameter<std::string>("hybridsuperclusterProducer");

  cluster_pt_thresh_ = ps.getParameter<double>("cluster_pt_thresh");
 
  matchedStripRecHitsInputTag_ = ps.getParameter<edm::InputTag>("matchedStripRecHits");
  rphiStripRecHitsInputTag_    = ps.getParameter<edm::InputTag>("rphiStripRecHits");
  stereoStripRecHitsInputTag_  = ps.getParameter<edm::InputTag>("stereoStripRecHits");
  pixelRecHitsInputTag_  = ps.getParameter<edm::InputTag>("pixelRecHits");  
  
  clusterMatchedRecHitsColl_ = ps.getParameter<std::string>("clusterMatchedRecHitsColl");
  clusterRPhiRecHitsColl_ = ps.getParameter<std::string>("clusterRPhiRecHitsColl");
  clusterStereoRecHitsColl_ = ps.getParameter<std::string>("clusterStereoRecHitsColl");
  siClusterColl_ = ps.getParameter<std::string>("siClusterColl");
  siPixClusterColl_ = ps.getParameter<std::string>("siPixClusterColl");

  produces<ClusterAndHitsCollection>(clusterandhitsCollection_);
  produces<SiStripMatchedRecHit2DCollection>(clusterMatchedRecHitsColl_);
  produces<SiStripRecHit2DCollection>(clusterRPhiRecHitsColl_);
  produces<SiStripRecHit2DCollection>(clusterStereoRecHitsColl_);
  produces<SiPixelRecHitCollection>(clusterPixelRecHitsColl_);
  produces<edm::DetSetVector<SiStripCluster> >(siClusterColl_);
  produces<edm::DetSetVector<SiPixelCluster> >(siPixClusterColl_);

  nEvt_ = 0;
}


ClusterAndHitsProducer::~ClusterAndHitsProducer()
{
}

void ClusterAndHitsProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  //This is where the products from our event will go.
  //clear the decks.
  clusterplushits_.clear();
  rphiMappa_.clear();
  rphidetIdSet_.clear();
  pixMappa_.clear();
  pixdetIdSet_.clear();
  pixMappaClus_.clear();
  sterMappa_.clear();
  sterdetIdSet_.clear();
  matchedMappa_.clear();
  matcheddetIdSet_.clear();
  
  //Get HybridSuperClusters to start.
  edm::Handle<reco::SuperClusterCollection> pBarrelHybridSuperClusters;
  
  evt.getByLabel(hybridsuperclusterProducer_, hybridsuperclusterCollection_, pBarrelHybridSuperClusters);
  if (!pBarrelHybridSuperClusters.isValid()){
    std::cout << "Error! can't get collection with label " << hybridsuperclusterCollection_.c_str()<<std::endl; ;
  }
  reco::SuperClusterCollection BarrelHybridSuperClusters = *pBarrelHybridSuperClusters;
  //Got the HybridSuperClusters

  //Loop over superclusters, and apply threshold
  for (int loop1=0;loop1<int(BarrelHybridSuperClusters.size());loop1++){
    SuperCluster clus1 = BarrelHybridSuperClusters[loop1];
    float eta1 = clus1.eta();
    float phi1 = clus1.phi();
    float energy1 = clus1.energy();
    float theta1 = 2*atan(exp(-1.*eta1));
    float cluspt1 = energy1 * sin(theta1);
    if (cluspt1 > cluster_pt_thresh_){
      
      // get Inputs
      edm::Handle<SiStripMatchedRecHit2DCollection> pmatchedRecHits;
      evt.getByLabel(matchedStripRecHitsInputTag_ ,pmatchedRecHits);
      edm::Handle<SiStripRecHit2DCollection> prphiRecHits;
      evt.getByLabel(rphiStripRecHitsInputTag_ ,prphiRecHits);
      edm::Handle<SiStripRecHit2DCollection> pstereoRecHits;
      evt.getByLabel(stereoStripRecHitsInputTag_ ,pstereoRecHits);
  
      
      const SiStripMatchedRecHit2DCollection* matchedRecHits = pmatchedRecHits.product();
      const SiStripRecHit2DCollection *rphiRecHits = prphiRecHits.product();
      const SiStripRecHit2DCollection *stereoRecHits = pstereoRecHits.product();
      
      const SiPixelRecHitCollection *pixelRecHitCollection = 0;
      edm::Handle<SiPixelRecHitCollection> pixelRecHits;
      evt.getByLabel(pixelRecHitsInputTag_, pixelRecHits);
      pixelRecHitCollection = pixelRecHits.product();

      //Need tracker geometry to tell me where stuff is.
      edm::ESHandle<TrackerGeometry> tracker;
      es.get<TrackerDigiGeometryRecord>().get(tracker);
      const TrackerGeometry& geometry = *tracker;
	    
      const std::vector<DetId> availableIDRPhi = rphiRecHits->ids();
      const std::vector<DetId> availableIDStereo = stereoRecHits->ids();
      const std::vector<DetId> availableIDMatched = matchedRecHits->ids();
      const std::vector<DetId> availableIDPixel = pixelRecHitCollection->ids();
  
      //Okay, what we'd actually like to do is sift through the different
      //DetId and select those which are geometrically close to the
      //passage of electron/photon.

      for (int kj=0;kj < int(availableIDRPhi.size());++kj){
	std::vector <SiStripRecHit2D> vecset;

	const GeomDetUnit *det = tracker->idToDetUnit(availableIDRPhi[kj].rawId());
	
	// calculate global position of center of detector
	GlobalPoint center = det->surface().toGlobal(LocalPoint(0,0,0)); 
	double phi = center.phi();
	double zed = center.z();
	double dphi = 0;
	double dphi1 = fabs(phi-phi1);
	if (dphi1 > TMath::Pi()) dphi = 2.*TMath::Pi() - dphi1;
	else dphi = dphi1;
	
	bool testDetZ = TestZPosition(clus1.x(), clus1.y(), clus1.z(),
				      0.,
				      center.x(), center.y(), zed,
				      0., 20.);

	if (dphi < 0.3 && testDetZ){
	  SiStripRecHit2DCollection::range rphiDetHits = rphiRecHits->get(availableIDRPhi[kj]);
	  for ( SiStripRecHit2DCollection::const_iterator rphiDetHit = rphiDetHits.first;
		rphiDetHit != rphiDetHits.second; 
		++rphiDetHit ) {
	    
	    
	    //Now only select likely hits. 
	    GlobalPoint position = geometry.idToDet( 
						    rphiDetHit->geographicalId()
						    )->surface().toGlobal(
									  rphiDetHit->localPosition());
	    float position_err_zz = 0;
	    if ( (unsigned int)availableIDRPhi[kj].subdetId() == StripSubdetector::TEC)
	      position_err_zz = 1.;
	    if ( (unsigned int)availableIDRPhi[kj].subdetId() == StripSubdetector::TOB){
	      TOBDetId tobid(availableIDRPhi[kj].rawId()); 
	      if ( !tobid.glued() ) {
		position_err_zz=12.;
	      }
	    }
	    if ( (unsigned int)availableIDRPhi[kj].subdetId() == StripSubdetector::TIB){
	      TIBDetId tobid(availableIDRPhi[kj].rawId()); 
	      if ( !tobid.glued() ) {
		position_err_zz=12.;
	      }
	    }
	    
	    bool TestZ = TestZPosition(clus1.x(), clus1.y(), clus1.z(),
				       0.,
				       position.x(), position.y(), position.z(),
				       0., position_err_zz);
	    double HitR = position.perp();
	    double PhiDiffMax = 0.01 + 0.0025 * HitR;
	    double HitPhi = position.phi();
	    double HitPhiDiff = fabs(phi1 - HitPhi);
	    if (HitPhiDiff > TMath::Pi()) HitPhiDiff = TMath::Pi()*2. - HitPhiDiff;
	    if (TestZ && HitPhiDiff < PhiDiffMax){
	      std::map<DetId, std::vector<SiStripRecHit2D> >::iterator it = rphiMappa_.find(availableIDRPhi[kj]);
	      std::map<DetId, std::vector<SiStripCluster> >::iterator sit = rphiMappaClus_.find(availableIDRPhi[kj]);
	      if (it == rphiMappa_.end()){
		std::vector <SiStripRecHit2D> vecca;
		std::vector <SiStripCluster> Cvecca;
		vecca.push_back(*rphiDetHit);
		SiStripRecHit2D::ClusterRef reffer = rphiDetHit->cluster();
		//typedef edm::Ref< edm::DetSetVector<SiStripCluster>,SiStripCluster, edm::refhelper::FindForDetSetVector<SiStripCluster>  > ClusterRef;
		//    const edm::DetSetVector<SiStripCluster>* rphiClusters = prphiClus.product();
		const SiStripCluster *clussy = reffer.get();

		Cvecca.push_back(*clussy);
		rphiMappa_.insert(make_pair(availableIDRPhi[kj], vecca));
		rphiMappaClus_.insert(make_pair(availableIDRPhi[kj], Cvecca));
		rphidetIdSet_.push_back(availableIDRPhi[kj]);
	      }
	      else{
		//This is where I would look to see if I already added the hit.

		it->second.push_back(*rphiDetHit);
		
		SiStripRecHit2D::ClusterRef reffer = rphiDetHit->cluster();
		const SiStripCluster *clussy = reffer.get();
		sit->second.push_back(*clussy);
		//		sit->second.push_back(rphiDetHit->cluster());
		
	      }
	    }	   
	  }
	}
      }   
      
      for (int kj=0;kj < int(availableIDPixel.size());++kj){
	
	const GeomDetUnit *det = tracker->idToDetUnit(availableIDPixel[kj].rawId());
	
	// calculate global position of center
	GlobalPoint center = det->surface().toGlobal(LocalPoint(0,0,0)); 
	double phi = center.phi();
	double zed = center.z();
	double dphi = 0;
	double dphi1 = fabs(phi-phi1);
	if (dphi1 > TMath::Pi()) dphi = 2.*TMath::Pi() - dphi1;
	else dphi = dphi1;
	
	bool testDetZ = TestZPosition(clus1.x(), clus1.y(), clus1.z(),
				      0.,
				      center.x(), center.y(), zed,
				      0., 20.);
	
	if (dphi < .3 && testDetZ){
	  SiPixelRecHitCollection::range pixDetHits = pixelRecHitCollection->get(availableIDPixel[kj]);
	  for ( SiPixelRecHitCollection::const_iterator pixDetHit = pixDetHits.first;
		pixDetHit != pixDetHits.second; 
		++pixDetHit ) {
	    
	    
	    //Now only select likely hits. 
	    GlobalPoint position = geometry.idToDet( 
						    pixDetHit->geographicalId()
						    )->surface().toGlobal(
									  pixDetHit->localPosition());

	    float position_err_zz = 0;
	    
	    
	    bool TestZ = TestZPosition(clus1.x(), clus1.y(), clus1.z(),
				       0.,
				       position.x(), position.y(), position.z(),
				       0., position_err_zz);
	    double HitR = position.perp();
	    double PhiDiffMax = 0.01 + 0.0025 * HitR;
	    double HitPhi = position.phi();
	    double HitPhiDiff = fabs(phi1 - HitPhi);
	    if (HitPhiDiff > TMath::Pi()) HitPhiDiff = TMath::Pi()*2. - HitPhiDiff;
	    if (TestZ && HitPhiDiff < PhiDiffMax){
	      std::map<DetId, std::vector<SiPixelRecHit> >::iterator it = pixMappa_.find(availableIDPixel[kj]);
	      std::map<DetId, std::vector<SiPixelCluster> >::iterator sit = pixMappaClus_.find(availableIDPixel[kj]);
	      if (it == pixMappa_.end()){
		std::vector <SiPixelRecHit> vecca;
		std::vector <SiPixelCluster> Cvecca;
		vecca.push_back(*pixDetHit);
		SiPixelRecHit::ClusterRef reffer = pixDetHit->cluster();
		const SiPixelCluster *clussy = reffer.get();

		Cvecca.push_back(*clussy);
		pixMappa_.insert(make_pair(availableIDPixel[kj], vecca));
		pixMappaClus_.insert(make_pair(availableIDPixel[kj], Cvecca));
		pixdetIdSet_.push_back(availableIDPixel[kj]);
	      }
	      else{
		//This is where I would look to see if I already added the hit.

		it->second.push_back(*pixDetHit);
		
		SiPixelRecHit::ClusterRef reffer = pixDetHit->cluster();
		const SiPixelCluster *clussy = reffer.get();
		sit->second.push_back(*clussy);
		
	      }
	    }
	  }
	}
      }  
      for (int kj=0;kj < int(availableIDStereo.size());++kj){
	
	const GeomDetUnit *det = tracker->idToDetUnit(availableIDStereo[kj].rawId());
	
	// calculate global position of center
	GlobalPoint center = det->surface().toGlobal(LocalPoint(0,0,0)); 
	double phi = center.phi();
	double zed = center.z();
	double dphi = 0;
	double dphi1 = fabs(phi-phi1);
	if (dphi1 > TMath::Pi()) dphi = 2.*TMath::Pi() - dphi1;
	else dphi = dphi1;
	
	bool testDetZ = TestZPosition(clus1.x(), clus1.y(), clus1.z(),
				      0.,
				      center.x(), center.y(), zed,
				      0., 20.);
	if (dphi < 0.3 && testDetZ){
	  SiStripRecHit2DCollection::range sterDetHits = stereoRecHits->get(availableIDStereo[kj]);
	  for ( SiStripRecHit2DCollection::const_iterator sterDetHit = sterDetHits.first;
		sterDetHit != sterDetHits.second; 
		++sterDetHit ) {
	    
	    
	    //Now only select likely hits. 
	    GlobalPoint position = geometry.idToDet( 
						    sterDetHit->geographicalId()
						    )->surface().toGlobal(
									  sterDetHit->localPosition());
	    float position_err_zz = 0;
	    
	    
	    bool TestZ = TestZPosition(clus1.x(), clus1.y(), clus1.z(),
				       0.,
				       position.x(), position.y(), position.z(),
				       0., position_err_zz);
	    double HitR = position.perp();
	    double PhiDiffMax = 0.01 + 0.0025 * HitR;
	    double HitPhi = position.phi();
	    double HitPhiDiff = fabs(phi1 - HitPhi);
	    if (HitPhiDiff > TMath::Pi()) HitPhiDiff = TMath::Pi()*2. - HitPhiDiff;
	    if (TestZ && HitPhiDiff < PhiDiffMax){
	      std::map<DetId, std::vector<SiStripRecHit2D> >::iterator it = rphiMappa_.find(availableIDStereo[kj]);
	      std::map<DetId, std::vector<SiStripCluster> >::iterator sit = rphiMappaClus_.find(availableIDStereo[kj]);
	      if (it == rphiMappa_.end()){
		std::vector <SiStripRecHit2D> vecca;
		std::vector <SiStripCluster> Cvecca;
		vecca.push_back(*sterDetHit);
		SiStripRecHit2D::ClusterRef reffer = sterDetHit->cluster();

		const SiStripCluster *clussy = reffer.get();

		Cvecca.push_back(*clussy);
		rphiMappa_.insert(make_pair(availableIDStereo[kj], vecca));
		rphiMappaClus_.insert(make_pair(availableIDStereo[kj], Cvecca));
		rphidetIdSet_.push_back(availableIDStereo[kj]);
	      }
	      else{
		//This is where I would look to see if I already added the hit.

		it->second.push_back(*sterDetHit);
		
		SiStripRecHit2D::ClusterRef reffer = sterDetHit->cluster();
		const SiStripCluster *clussy = reffer.get();
		sit->second.push_back(*clussy);
		//		sit->second.push_back(rphiDetHit->cluster());
		
	      }
	    }	   
	  }
	}
      }

      
      //This is total hackage
      edm::OwnVector <TrackingRecHit> v;
      for (int ik=0;ik<int(rphidetIdSet_.size());++ik){
	std::map<DetId, std::vector<SiStripRecHit2D> >::iterator mpa = rphiMappa_.find(rphidetIdSet_[ik]);
	if (mpa != rphiMappa_.end())
	  for (int ij=0;ij < int(mpa->second.size());++ij){
	    v.push_back(mpa->second[ij].clone());
	  }
      }

      ClusterAndHits *clus = new ClusterAndHits(clus1, v);
      clusterplushits_.push_back(*clus);
	
      
      
      
    }//PT threshold


  }//BasicClusters loop

  std::auto_ptr<SiStripRecHit2DCollection > rpCol(new SiStripRecHit2DCollection);
                                                      
  //Done with supercluster loop, go through maps and place into the RangeMap objects for storage.
  if (rphidetIdSet_.size() > 0){
    for (int ik=0;ik<int(rphidetIdSet_.size());++ik){
      std::map<DetId, std::vector<SiStripRecHit2D> >::iterator mpa = rphiMappa_.find(rphidetIdSet_[ik]);
      if (mpa != rphiMappa_.end())
	rpCol->put(mpa->first, mpa->second.begin(), mpa->second.end());
    }
  }
  std::auto_ptr<SiPixelRecHitCollection > pxCol(new SiPixelRecHitCollection);
                                                      
  if (pixdetIdSet_.size() > 0){
    for (int ik=0;ik<int(pixdetIdSet_.size());++ik){
      std::map<DetId, std::vector<SiPixelRecHit> >::iterator mpa = pixMappa_.find(pixdetIdSet_[ik]);
      if (mpa != pixMappa_.end())
	pxCol->put(mpa->first, mpa->second.begin(), mpa->second.end());
    }
  }
  
  std::auto_ptr<SiStripRecHit2DCollection > stCol(new SiStripRecHit2DCollection);
                                                      
  if (sterdetIdSet_.size() > 0){
    for (int ik=0;ik<int(sterdetIdSet_.size());++ik){
      std::map<DetId, std::vector<SiStripRecHit2D> >::iterator mpa = sterMappa_.find(sterdetIdSet_[ik]);
      if (mpa != sterMappa_.end())
	stCol->put(mpa->first, mpa->second.begin(), mpa->second.end());
    }
  }

  std::vector<edm::DetSet <SiStripCluster> > theDetSetVector;
  //Done with supercluster loop, go through maps and place into the RangeMap objects for storage.
  if (rphidetIdSet_.size() > 0){
    for (int ik=0;ik<int(rphidetIdSet_.size());++ik){
      std::map<DetId, std::vector<SiStripCluster> >::iterator mpa = rphiMappaClus_.find(rphidetIdSet_[ik]);
      if (mpa != rphiMappaClus_.end()){
	edm::DetSet<SiStripCluster> detta(mpa->first);
	for (int ij=0;ij<int(mpa->second.size());++ij){
	  detta.push_back(mpa->second[ij]);
	}
	theDetSetVector.push_back(detta);
      }
    }
  }

  std::vector<edm::DetSet <SiPixelCluster> > thePDetSetVector;
  if (pixdetIdSet_.size() > 0){
    for (int ik=0;ik<int(pixdetIdSet_.size());++ik){
      std::map<DetId, std::vector<SiPixelCluster> >::iterator mpa = pixMappaClus_.find(pixdetIdSet_[ik]);
      if (mpa != pixMappaClus_.end()){
	edm::DetSet<SiPixelCluster> detta(mpa->first);
	for (int ij=0;ij<int(mpa->second.size());++ij){
	  detta.push_back(mpa->second[ij]);
	}
	thePDetSetVector.push_back(detta);
      }
    }
  }

  std::auto_ptr<edm::DetSetVector<SiStripCluster> > arfiCol(new edm::DetSetVector<SiStripCluster>(theDetSetVector));
  evt.put(arfiCol, siClusterColl_);

  std::auto_ptr<edm::DetSetVector<SiPixelCluster> > parfiCol(new edm::DetSetVector<SiPixelCluster>(thePDetSetVector));
  evt.put(parfiCol, siPixClusterColl_);


  std::auto_ptr<ClusterAndHitsCollection > converts_p(new ClusterAndHitsCollection);
  converts_p->assign(clusterplushits_.begin(), clusterplushits_.end());
  evt.put(converts_p, clusterandhitsCollection_);
  //  rpCol->assign(clusterRPhiRecHits_.begin(), clusterRPhiRecHits_.end());

  evt.put(rpCol,clusterRPhiRecHitsColl_);
  evt.put(pxCol,clusterPixelRecHitsColl_);
  evt.put(stCol,clusterStereoRecHitsColl_);
  
}

bool ClusterAndHitsProducer::TestZPosition(double _SeedX, double _SeedY, double _SeedZ,
				   double _VtxZ,
				   double _TestX, double _TestY, double _TestZ,
				   double _TestXXerr, double _TestYYerr
				   )
{
  //Testing the Z coordinate position of hit.
  double _ZVTXCONSTERR = 10.;
  double _CALZERR=2.;
  //Define the vertex error on Z
  double ZVtxMin = _VtxZ - _ZVTXCONSTERR;
  double ZVtxMax = _VtxZ + _ZVTXCONSTERR;
  
  double SeedZMin = _SeedZ - _CALZERR;
  double SeedZMax = _SeedZ + _CALZERR;
  
  double RCAL = sqrt(_SeedX*_SeedX + _SeedY*_SeedY);
  double slopemin = (SeedZMin - ZVtxMin) / RCAL;
  double slopemax = (SeedZMax - ZVtxMax) / RCAL;
  
  double interceptmin = ZVtxMin;
  double interceptmax = ZVtxMax;
  
  double TESTR = sqrt(_TestX*_TestX + _TestY*_TestY);
  
  double REGIONZMax = slopemax * TESTR + interceptmax + 2.*_TestYYerr;
  double REGIONZMin = slopemin * TESTR + interceptmin - 2.*_TestYYerr;
  
  if (  REGIONZMax > _TestZ && REGIONZMin < _TestZ){
    return true; 
  }
  return false;
}

