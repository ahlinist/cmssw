#include <vector>
#include "RecoEgamma/MaterialConversionModules/interface/Pi0ConversionProducer.h"
#include "RecoEgamma/MaterialConversionTools/interface/Pi0ConversionAlgo.h"
#include "RecoEgamma/MaterialConversionModules/interface/Pi0MaterialConversion.h"
#include "RecoEgamma/MaterialConversionModules/interface/Pi0MaterialConversionFwd.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
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
#include "TrackingTools/RoadSearchHitAccess/interface/DetHitAccess.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"
#include "RecoVertex/VertexPrimitives/interface/CachingVertex.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include <TMath.h>
#include <iostream>

using namespace reco;
Pi0ConversionProducer::Pi0ConversionProducer(const edm::ParameterSet& ps)
{
  
  // The debug level
  std::string debugString = ps.getParameter<std::string>("debugLevel");
  if      (debugString == "DEBUG")   debugL = Pi0ConversionAlgo::pDEBUG;
  else if (debugString == "INFO")    debugL = Pi0ConversionAlgo::pINFO;
  else                               debugL = Pi0ConversionAlgo::pERROR;
 
  float stubmaxchi2 = ps.getParameter<double>("stubMaxChi2");
  pi0conv_p = new Pi0ConversionAlgo(debugL, stubmaxchi2);
  
  basicclusterCollection_ = ps.getParameter<std::string>("basicclusterCollection");
  basicclusterProducer_ = ps.getParameter<std::string>("basicclusterProducer");

  clusterdEtaMax_ = ps.getParameter<double>("clusterMaxEtaSep");
  clusterdRMax_ = ps.getParameter<double>("clusterdRMax");
  cluster_pt_thresh_ = ps.getParameter<double>("cluster_pt_thresh");
  clusterIso_ = ps.getParameter<double>("clusterIso");
  clusterLabel_ = ps.getParameter<std::string>("ConversionLabel");
  stubminimumhits_ = ps.getParameter<int>("stubminimumhits");
 
  matchedStripRecHitsInputTag_ = ps.getParameter<edm::InputTag>("matchedStripRecHits");
  rphiStripRecHitsInputTag_    = ps.getParameter<edm::InputTag>("rphiStripRecHits");
  stereoStripRecHitsInputTag_  = ps.getParameter<edm::InputTag>("stereoStripRecHits");
  pixelRecHitsInputTag_  = ps.getParameter<edm::InputTag>("pixelRecHits");  
  TTRHbuilderName_ = ps.getParameter<std::string>("TTRHBuilder"); 
  produces< Pi0MaterialConversionCollection>(conversioncollection_);
  nEvt_ = 0;
  hbheLabel_ = ps.getParameter<string>("hbheModule");
  hbheInstanceName_ = ps.getParameter<string>("hbheInstance");
  if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Finished constructor" << std::endl;
}


Pi0ConversionProducer::~Pi0ConversionProducer()
{
  delete pi0conv_p;
}

void Pi0ConversionProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  
  //Get IslandBasicClusters to start.
  edm::Handle<reco::BasicClusterCollection> pBarrelBasicClusters;
  try {
    evt.getByLabel(basicclusterProducer_, basicclusterCollection_, pBarrelBasicClusters);
  } catch ( cms::Exception& ex ) {
    std::cout << "Error! can't get collection with label " << basicclusterCollection_.c_str()<<std::endl; ;
  }

  reco::BasicClusterCollection BarrelBasicClusters = *pBarrelBasicClusters;
  if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Acquired clusters" << std::endl;
  //Got the IslandBasicClusters

  edm::Handle<reco::ClusterShapeCollection> pBarrelShapeClusters;
  try {
    evt.getByLabel("islandBasicClusters", "islandBarrelShape", pBarrelShapeClusters);
  } catch ( cms::Exception& ex ) {
    std::cout << "Error! can't get collection with label " << basicclusterCollection_.c_str()<<std::endl; ;
  }

  reco::ClusterShapeCollection BarrelClusterShapes = *pBarrelShapeClusters;
  if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Acquired clusters" << std::endl;

  edm::Handle<HBHERecHitCollection> hbhe;
  HBHERecHitMetaCollection *mhbhe=0;
  try{
    evt.getByLabel(hbheLabel_,hbheInstanceName_,hbhe);  
    mhbhe=  new HBHERecHitMetaCollection(*hbhe);
  }
  catch(cms::Exception& ex){
    std::cout << "Can't get HCAL recs!" << endl;
  }
  
  //okay, looking for three clusters in barrel ECAL, close together in dR.
  Pi0MaterialConversionCollection converts;
  //Don't even bother if there aren't three clusters.
  if (BarrelBasicClusters.size()>2){

    //A triple loop, so that I consider all combinations of three basic clusters in the event.
    for (int loop1=0;loop1<int(BarrelBasicClusters.size())-2;loop1++){
      BasicCluster clus1 = BarrelBasicClusters[loop1];
      float eta1 = clus1.eta();
      float energy1 = clus1.energy();
      float theta1 = 2*atan(exp(-1.*eta1));
      float cluspt1 = energy1 * sin(theta1);

      //As per usual, don't bother if not over threshold.
      if (cluspt1 < cluster_pt_thresh_) continue;

      //Loop 2 begins.
      for (int loop2=loop1+1;loop2<int(BarrelBasicClusters.size())-1;loop2++){
	BasicCluster clus2 = BarrelBasicClusters[loop2];

	float eta2 = clus2.eta();
	float deta12 = fabs(eta1-eta2);
	//Must have minimal separation with cluster 1 in eta.
	if (deta12 > .2) continue;
	float energy2 = clus2.energy();
	float theta2 = 2*atan(exp(-1.*eta2));
	float cluspt2 = energy2 * sin(theta2);
	//Abort if not over threshold.
	if (cluspt2 < cluster_pt_thresh_) continue;	
	
	//Begin Loop 3
	for (int loop3=loop2+1;loop3<int(BarrelBasicClusters.size());loop3++){
	  BasicCluster clus3 = BarrelBasicClusters[loop3];
	  float eta3 = clus3.eta();
	  float deta13 = fabs( eta1-eta3);
	  //Minimal 1-3 separation.
	  if (deta13 > .2) continue;
	  float energy3 = clus3.energy();
	  float theta3 = 2*atan(exp(-1.*eta3));
	  float cluspt3 = energy3 * sin(theta3);

	  //Check on threshold.
	  if (cluspt3 < cluster_pt_thresh_) continue;
	  if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Have three clusters identified" << std::endl;

	  //Check 2-3 separation.
	  float deta23 = fabs(eta2-eta3);

	  //okay, three basic clusters, all over threshold, all close in eta.
	  //Make RSep cut, require clusters vaguely close together( simple)	  
	  float phi1 = clus1.phi();
	  float phi2 = clus2.phi();
	  float phi3 = clus3.phi();
	  
	  if (phi1 < 0) phi1+=TMath::Pi()*2.;
	  if (phi2 < 0) phi2+=TMath::Pi()*2.;
	  if (phi3 < 0) phi3+=TMath::Pi()*2.;
		
	  float dR12 = GetSep(eta1, phi1, eta2, phi2);
	  float dR23 = GetSep(eta2, phi2, eta3, phi3);
	  float dR13 = GetSep(eta1, phi1, eta3, phi3);
	  if (dR12 > clusterdRMax_
	      || dR23 > clusterdRMax_
	      || dR13 > clusterdRMax_) continue;
	  if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Cluster separation good" << std::endl;

	  //Order by eta the clusters, assign identities (calculate mass later)
	  //two closest objects in eta are assigned to be the electrons.
	  BasicCluster ele1;
	  BasicCluster ele2;
	  BasicCluster pho;
	  int ele1Idx=-1;
	  int ele2Idx=-1;
	  int phoIdx=-1;
	  if (deta13 < deta12
	      && deta13 < deta23){
	    ele1 = BarrelBasicClusters[loop1];
	    ele2 = BarrelBasicClusters[loop3];
	    pho  = BarrelBasicClusters[loop2];
	    ele1Idx = loop1;
	    ele2Idx = loop3;
	    phoIdx = loop2;
	  } 
	  if (deta12 < deta13
	      && deta12 < deta23){
	    ele1 = BarrelBasicClusters[loop1];
	    ele2 = BarrelBasicClusters[loop2];
	    pho  = BarrelBasicClusters[loop3];
	    ele1Idx = loop1;
	    ele2Idx = loop2;
	    phoIdx = loop3;
	  } 
	  if (deta23 < deta12
	      && deta23 < deta13){
	    ele1 = BarrelBasicClusters[loop2];
	    ele2 = BarrelBasicClusters[loop3];
	    pho  = BarrelBasicClusters[loop1];
	    ele1Idx = loop2;
	    ele2Idx = loop3;
	    phoIdx = loop1;
	  } 
	  //Calculate isolation here:
	  float totE = ele1.energy() + ele2.energy() + pho.energy();
	  float xtot = (ele1.position().x()*ele1.energy() + ele2.position().x()*ele2.energy() + pho.position().x()*pho.energy())/totE;
	  float ytot = (ele1.position().y()*ele1.energy() + ele2.position().y()*ele2.energy() + pho.position().y()*pho.energy())/totE;
	  float ztot = (ele1.position().z()*ele1.energy() + ele2.position().z()*ele2.energy() + pho.position().z()*pho.energy())/totE;
	  float phi_tot = atan2(ytot,xtot);
	  float theta_tot = atan2(sqrt(xtot*xtot+ytot*ytot),ztot);
	  float eta_tot = -log(tan(0.5*theta_tot));
	  es.get<IdealGeometryRecord>().get(theCaloGeom);
	  float iso = emIso(BarrelBasicClusters, ele1, ele2, pho);

	  //HCAL energy behind triplet
	  CaloConeSelector sel(0.4, theCaloGeom.product(), DetId::Hcal);
	  GlobalPoint pclu(xtot,ytot,ztot);
	  double hcalEnergy = 0.;
	  std::auto_ptr<CaloRecHitMetaCollectionV> chosen=sel.select(pclu,*mhbhe);
	  for (CaloRecHitMetaCollectionV::const_iterator i=chosen->begin(); i!=chosen->end(); i++) {
	    const CaloCellGeometry *this_cell = (*theCaloGeom).getGeometry(i->detid());
	    GlobalPoint position = this_cell->getPosition();
	    hcalEnergy += i->energy();// * sin(position.theta());
	  }

	  if (debugL == Pi0ConversionAlgo::pDEBUG){
	    std::cout << "Calculated three cluster isolation from other BasicClusters." << std::endl;
	    std::cout << "Iso: " << iso << std::endl;
	    if (iso > clusterIso_) std::cout << "Failing this candidate!" << std::endl;
	    std::cout << "ele1Idx = " << ele1Idx << std::endl;
	    std::cout << "ele2Idx = " << ele2Idx << std::endl;
	    std::cout << "phoIdx = " << phoIdx << std::endl;
	  }
	  if (ele1Idx==-1 || ele2Idx==-1 || phoIdx==-1) continue;
	  if (iso > clusterIso_) continue;
	  //GetClusterShapes
	  ClusterShape ele1Shape = BarrelClusterShapes[ele1Idx];
	  ClusterShape ele2Shape = BarrelClusterShapes[ele2Idx];
	  ClusterShape phoShape = BarrelClusterShapes[phoIdx];

	  
	
	  //Make call on algorithm object to ensure that conversion R, Phi exist. 
	  float ele1E = ele1.energy();
	  float eleT1 = 2*atan(exp(-1.*ele1.eta()));
	  float elept1 = ele1E * sin(eleT1);
	  
	  float ele2E = ele2.energy();
	  float eleT2 = 2*atan(exp(-1.*ele2.eta()));
	  float elept2 = ele2E * sin(eleT2);
	  
	  float RConv = pi0conv_p->GetConversionR(ele1.phi(), elept1, ele2.phi(), elept2);
	  //require that this point be within the feasible reconstruction area.
	  if (RConv > 0 && RConv < 70){
	    
	    float PhiConv = pi0conv_p->GetConversionPhi(ele1.phi(), elept1, ele2.phi(), elept2);
	    if (debugL == Pi0ConversionAlgo::pDEBUG){
	      std::cout << "Identified possible conversion point at: " << std::endl;
	      std::cout << "Conversion R: " << RConv << std::endl;
	      std::cout << "Conversion phi: " << PhiConv << std::endl; 
	    }
	    
	    //fill in all necessary information for this conversion candidate
	    float m = pimass(pho.eta(), pho.phi(), pho.energy(),
			     ele1.eta(), ele1.phi(), ele1.energy(),
			     ele2.eta(), ele2.phi(), ele2.energy());
	    
	    if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Three body mass: " << m << std::endl;
	    
	    if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Isolation: " << iso << std::endl;
	    Pi0MaterialConversion conner(pho.energy(),pho.eta(), pho.phi(),
					 ele1.energy(), ele1.eta(), ele1.phi(),
					 ele2.energy(), ele2.eta(), ele2.phi(),
					 RConv, PhiConv, iso, m);
	    
	    conner.setHCALEnergy(hcalEnergy);
	    conner.setGamEmax(phoShape.eMax());
	    conner.setGamE2nd(phoShape.e2nd());
	    conner.setGame2x2(phoShape.e2x2());
	    conner.setGame3x3(phoShape.e3x3());
	    conner.setGametaeta(phoShape.covEtaEta());
	    conner.setGamphiphi(phoShape.covPhiPhi());
	    conner.setGametaphi(phoShape.covEtaPhi());
	    
	    conner.setE1Emax(ele1Shape.eMax());
	    conner.setE1E2nd(ele1Shape.e2nd());
	    conner.setE1e2x2(ele1Shape.e2x2());
	    conner.setE1e3x3(ele1Shape.e3x3());
	    conner.setE1etaeta(ele1Shape.covEtaEta());
	    conner.setE1phiphi(ele1Shape.covPhiPhi());
	    conner.setE1etaphi(ele1Shape.covEtaPhi());
	    
	    conner.setE2Emax(ele2Shape.eMax());
	    conner.setE2E2nd(ele2Shape.e2nd());
	    conner.setE2e2x2(ele2Shape.e2x2());
	    conner.setE2e3x3(ele2Shape.e3x3());
	    conner.setE2etaeta(ele2Shape.covEtaEta());
	    conner.setE2phiphi(ele2Shape.covPhiPhi());
	    conner.setE2etaphi(ele2Shape.covEtaPhi());
	    
	    if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Created new Pi0MaterialConversion object" << std::endl;
	    
	    
	    //Having that valid point, call stub creation on clusters (StubCandidate, ConvCand), collect hits
	    //we'll need hits now.  Wasn't worth it until now.
	    
	    // get Inputs
	    edm::Handle<SiStripMatchedRecHit2DCollection> pmatchedRecHits;
	    evt.getByLabel(matchedStripRecHitsInputTag_ ,pmatchedRecHits);
	    edm::Handle<SiStripRecHit2DCollection> prphiRecHits;
	    evt.getByLabel(rphiStripRecHitsInputTag_ ,prphiRecHits);
	    edm::Handle<SiStripRecHit2DCollection> pstereoRecHits;
	    evt.getByLabel(stereoStripRecHitsInputTag_ ,pstereoRecHits);
	    // retrieve InputTag for pixel rechits
	    
	    const SiStripMatchedRecHit2DCollection* matchedRecHits = pmatchedRecHits.product();
	    const SiStripRecHit2DCollection *rphiRecHits = prphiRecHits.product();
	    const SiStripRecHit2DCollection *stereoRecHits = pstereoRecHits.product();
	    
	    // special treatment for getting pixel collection
	    // if collection exists in file, use collection from file
	    // if collection does not exist in file, create empty collection
	    const SiPixelRecHitCollection *pixelRecHitCollection = 0;
	    try {
	      edm::Handle<SiPixelRecHitCollection> pixelRecHits;
	      evt.getByLabel(pixelRecHitsInputTag_, pixelRecHits);
	      pixelRecHitCollection = pixelRecHits.product();
	    }
	    catch (edm::Exception const& x) {
	      if ( x.categoryCode() == edm::errors::ProductNotFound ) {
		if ( x.history().size() == 1 ) {
		  static const SiPixelRecHitCollection s_empty;
		  pixelRecHitCollection = &s_empty;
		  
		}
	      }
	    }
	    if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Got tracker hit products" << std::endl;
	    
	    //Select hit collections
	    DetHitAccess recHitVectorClass;
	    const std::vector<DetId> availableIDs = rphiRecHits->ids();
	    const std::vector<DetId> availableID2 = stereoRecHits->ids();
	    const std::vector<DetId> availableID3 = matchedRecHits->ids();
	    const std::vector<DetId> availableID4 = pixelRecHitCollection->ids();
	    
	    recHitVectorClass.setCollections(rphiRecHits,stereoRecHits,matchedRecHits,pixelRecHitCollection);
	    recHitVectorClass.setMode(DetHitAccess::standard);
	    //	    recHitVectorClass.use_rphiRecHits(true);
	    //	    recHitVectorClass.use_stereoRecHits(true);
	    
	    //This is where I will put ALL the hits.
	    //	    std::cout <<"Creating tracker."<<std::endl;
	    std::vector <TrackingRecHit*> FullTracker;
	    for (int kj=0;kj < int(availableIDs.size());++kj){
	      std::vector <TrackingRecHit*> FullTracker1 = recHitVectorClass.getHitVector(&availableIDs[kj]);
	      for (int ji=0;ji<int(FullTracker1.size());ji++){
		FullTracker.push_back((FullTracker1)[ji]);
	      }
	    }
	    
	    for (int kj=0;kj < int(availableID2.size());++kj){
	      std::vector <TrackingRecHit*> FullTracker2 = recHitVectorClass.getHitVector(&availableID2[kj]);
	      for (int ji=0;ji<int(FullTracker2.size());ji++){
		FullTracker.push_back((FullTracker2)[ji]);
	      }
	    }
	    
	    for (int kj=0;kj < int(availableID3.size());++kj){
	      std::vector <TrackingRecHit*> FullTracker3 = recHitVectorClass.getHitVector(&availableID3[kj]);
	      for (int ji=0;ji<int(FullTracker3.size());ji++){
		FullTracker.push_back((FullTracker3)[ji]);
	      }
	    }
	    
	    for (int kj=0;kj < int(availableID4.size());++kj){
	      std::vector <TrackingRecHit*> FullTracker4 = recHitVectorClass.getHitVector(&availableID4[kj]);
	      for (int ji=0;ji<int(FullTracker4.size());ji++){
		FullTracker.push_back((FullTracker4)[ji]);
	      }
	    }
	    
	    //	    std::cout << "filled tracker" << std::endl;
	    if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Handing in:" << FullTracker.size() << std::endl;
	    //if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Handing RPHIin:" << FullTracker1.size() << std::endl;
	    //	    if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Handing Sterin:" << FullTracker2.size() << std::endl;
	    //	    if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Handing Matin:" << FullTracker3.size() << std::endl;
	    
	    
	    std::vector <TrackingRecHit*> Stub1;
	    std::vector <TrackingRecHit*> Stub2; 
	    edm::ESHandle<TrackerGeometry> tracker;
	    es.get<TrackerDigiGeometryRecord>().get(tracker);
	    const TrackerGeometry& geometry = *tracker;
	    int char1=0;
	    int char2=0;
	    pi0conv_p->GetStubHits(ele1, ele2, RConv, PhiConv, &FullTracker, &geometry, &Stub1, &Stub2, char1, char2);
	    if (debugL == Pi0ConversionAlgo::pDEBUG){
	      std::cout << "Got stubs from roads: " << std::endl;
	      std::cout << "Stub 1 has " << Stub1.size() << " hits." << std::endl;
	      std::cout << "Stub 2 has " << Stub2.size() << " hits." << std::endl;
	    }
	    conner.setStub1Charge(char1);
	    conner.setStub2Charge(char2);
	    conner.setNStub1Hits(Stub1.size());
	    conner.setNStub2Hits(Stub2.size());
	    //Take this point to store the global positions of the stub hits in the output object for
	    //debugging
	    std::vector<GlobalPoint> stb1;
	    std::vector<GlobalPoint> stb2;
	    for (int q=0;q<int(Stub1.size());++q){
	      TrackingRecHit *htr = (Stub1[q]);
	      GlobalPoint position = geometry.idToDet( 
						      htr->geographicalId()
						      )->surface().toGlobal(
									    htr->localPosition());
	      stb1.push_back(position);
	    }
	    conner.setStub1Hits(stb1);
	    
	    for (int q=0;q<int(Stub2.size());++q){
	      TrackingRecHit *htr = (Stub2[q]);
	      GlobalPoint position = geometry.idToDet( 
						      htr->geographicalId()
						      )->surface().toGlobal(
									    htr->localPosition());
	      stb2.push_back(position);
	    }
	    conner.setStub2Hits(stb2); 
	    
	    
	    if (int(Stub1.size()) > stubminimumhits_ || int(Stub2.size()) > stubminimumhits_){
	      //I don't think I need this.  I might, if the actual track fitter/smoother wants it.
	      edm::ESHandle<MagneticField> magneticFieldHandle;
	      es.get<IdealMagneticFieldRecord>().get(magneticFieldHandle);
	      const MagneticField& magField = *magneticFieldHandle;
	      
	      edm::ESHandle<TransientTrackingRecHitBuilder> theBuilder;
	      es.get<TransientRecHitRecord>().get(TTRHbuilderName_,theBuilder);
	      const TransientTrackingRecHitBuilder &ttrhBuilder = *theBuilder;
	      
	      edm::ESHandle<MeasurementTracker>    measurementTrackerHandle;
	      es.get<CkfComponentsRecord>().get(measurementTrackerHandle);
	      const MeasurementTracker &theMeasurementTracker = *measurementTrackerHandle;
	      
	      //	      theMeasurementTracker.update(evt);
	      if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Got remaining tracker setup items.  Ready to try fitting." << std::endl;
	      
	      //Fit Tracks, found similar code to what was used before in RoadSearchTrackCandidateMakerAlgo
	      reco::TrackCollection trkCan1;
	      reco::TrackCollection trkCan2;
	      reco::TrackExtraCollection trkColl1;
	      reco::TrackExtraCollection trkColl2;
	      trkCan1.clear();
	      trkCan2.clear();
	      edm::OwnVector <TrackingRecHit> usedhits1;
	      edm::OwnVector <TrackingRecHit> usedhits2;
	      
	      if (int(Stub1.size()) > stubminimumhits_)
		pi0conv_p->FitTrack(Stub1, &theMeasurementTracker, &ttrhBuilder, &magField, &geometry,es, trkCan1, trkColl1, usedhits1);
	      if (debugL==Pi0ConversionAlgo::pDEBUG) std::cout << "Returned " << trkCan1.size() << " tracks for stub1. "<<std::endl;
	      
	      if (int(Stub2.size()) > stubminimumhits_)
	        pi0conv_p->FitTrack(Stub2, &theMeasurementTracker, &ttrhBuilder, &magField, &geometry,es, trkCan2, trkColl2, usedhits2);
	      if (debugL==Pi0ConversionAlgo::pDEBUG) std::cout << "Returned " << trkCan2.size() << " tracks for stub2. "<<std::endl;
	      
	      if (trkCan1.size()>0){
		if (debugL==Pi0ConversionAlgo::pDEBUG) std::cout << "Trying to fill information for track1"<<std::endl;
		if (debugL==Pi0ConversionAlgo::pDEBUG) std::cout << "Track collection size: " << trkCan1.size() <<std::endl;
		reco::Track trk1 = trkCan1[0];
		reco::TrackExtra trkE1 = trkColl1[0];
		if (debugL==Pi0ConversionAlgo::pDEBUG) std::cout << "Got track 1"<<std::endl;
		conner.setTrk1PTIS(sqrt( pow(trkE1.innerMomentum().X(),2) +pow(trkE1.innerMomentum().Y(),2)));
		conner.setTrk1PhiIS(trkE1.innerMomentum().Phi());
		conner.setTrk1EtaIS(trkE1.innerMomentum().Eta());
		conner.setTrk1Charge(trk1.charge());
		if (debugL==Pi0ConversionAlgo::pDEBUG) std::cout << "Set inner momentum variables."<<std::endl;
		conner.setTrk1PTFS(sqrt( pow(trkE1.outerMomentum().X(),2) +pow(trkE1.outerMomentum().Y(),2)));
		conner.setTrk1PhiFS(trkE1.outerMomentum().Phi());
		conner.setTrk1EtaFS(trkE1.outerMomentum().Eta());
		if (debugL==Pi0ConversionAlgo::pDEBUG) std::cout << "Set outer momentum variables."<<std::endl;
		conner.setTrk1NHIT(usedhits1.size());
		if (debugL==Pi0ConversionAlgo::pDEBUG) std::cout << "Set chi2 hit."<<std::endl;
		conner.setTrk1Chi2(trk1.chi2());
		conner.setTrk1SmallRHit(trkE1.innerPosition().R());
		std::vector<GlobalPoint> ttb1;
		for (int q=0;q<int(usedhits1.size());++q){
		  TrackingRecHit *htr = (usedhits1[q].clone());
		  GlobalPoint position = geometry.idToDet( 
							  htr->geographicalId()
							  )->surface().toGlobal(
										htr->localPosition());
		  ttb1.push_back(position);
		}
		conner.setTrack1Hits(ttb1);
		if (debugL==Pi0ConversionAlgo::pDEBUG) std::cout << "Done Trying to fill information for track1"<<std::endl;
		
	      }
	      
	      if (trkCan2.size()>0){
		//track2 quantities
		if (debugL==Pi0ConversionAlgo::pDEBUG) std::cout << "Trying to fill information for track2"<<std::endl;
		reco::Track trk2 = trkCan2[0];
		reco::TrackExtra trk2E = trkColl2[0];
		conner.setTrk2PTIS(sqrt( pow(trk2E.innerMomentum().X(),2) +pow(trk2E.innerMomentum().Y(),2)));
		conner.setTrk2PhiIS(trk2E.innerMomentum().Phi());
		conner.setTrk2EtaIS(trk2E.innerMomentum().Eta());
		conner.setTrk2PTFS(sqrt( pow(trk2E.outerMomentum().X(),2) +pow(trk2E.outerMomentum().Y(),2)));
		conner.setTrk2PhiFS(trk2E.outerMomentum().Phi());
		conner.setTrk2EtaFS(trk2E.outerMomentum().Eta());
		conner.setTrk2NHIT(usedhits2.size());
		conner.setTrk2Chi2(trk2.chi2());
		conner.setTrk2Charge(trk2.charge());
		
		conner.setTrk2SmallRHit(trk2E.innerPosition().R());
		
		std::vector<GlobalPoint> ttb2;
		for (int q=0;q<int(usedhits2.size());++q){
		  TrackingRecHit *htr = (usedhits2[q].clone());
		  GlobalPoint position = geometry.idToDet( 
							  htr->geographicalId()
							  )->surface().toGlobal(
										htr->localPosition());
		  ttb2.push_back(position);
		}
		conner.setTrack2Hits(ttb2);
		if (debugL==Pi0ConversionAlgo::pDEBUG) std::cout << "Done Trying to fill information for track2"<<std::endl;
	      }
	      
	      //if applicable, fit vertex
	      if (trkCan1.size()==1 && trkCan2.size()==1){
		if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "Attempting vertex fit. " << std::endl;
		std::vector<reco::TransientTrack> trks;
		for (int i=0;i<int(trkCan1.size());++i)
		  trks.push_back(reco::TransientTrack(trkCan1[i],&magField));
		for (int i=0;i<int(trkCan2.size());++i)
		  trks.push_back(reco::TransientTrack(trkCan2[i],&magField));
		try {
		  CachingVertex vert = pi0conv_p->FitVertex(trks);
		  conner.setVtxXPos(vert.position().x());
		  conner.setVtxYpos(vert.position().y());
		  conner.setVtxZpos(vert.position().z());
		  conner.setVtxRpos(sqrt(pow(vert.position().x(),2) + pow(vert.position().y(),2)));
		  conner.setVtxPhipos(vert.position().phi());
		  conner.setVtxChi2(vert.totalChiSquared()/vert.degreesOfFreedom());
		  if (debugL == Pi0ConversionAlgo::pDEBUG) {
		    std::cout << "Vertex fit R: " << conner.getVtxRpos() << std::endl;
		  }
		}
		catch (std::exception& cs){
		  //		  if (debugL == Pi0ConversionAlgo::pDEBUG){
		  if (true){
		    std::cout << "VERTEX EXCEPTION. " << std::endl;
		    std::cout << "trk1 smallR: " << conner.getTrk1SmallRHit()<< std::endl;
		    std::cout << "trk2 smallR: " << conner.getTrk2SmallRHit()<< std::endl;
		    std::cout << "trk2 smallX: " << trkColl1[0].innerPosition().X() << std::endl;
		    std::cout << "trk2 smallX: " << trkColl2[0].innerPosition().X() << std::endl;
		    std::cout << "trk2 smallY: " << trkColl1[0].innerPosition().Y() << std::endl;
		    std::cout << "trk2 smallY: " << trkColl2[0].innerPosition().Y() << std::endl;
		    std::cout << "trk2 smallZ: " << trkColl1[0].innerPosition().Z() << std::endl;
		    std::cout << "trk2 smallZ: " << trkColl2[0].innerPosition().Z() << std::endl;
		    std::cout << "trk1 chi2: " <<conner.getTrk1Chi2() << std::endl;
		    std::cout << "trk2 chi2: " <<conner.getTrk2Chi2() << std::endl;
		    std::cout << "trk1 nhit: " << conner.getTrk1NHIT() << std::endl;
		    std::cout << "trk2 nhit: " << conner.getTrk2NHIT() << std::endl;
		    std::cout << "trk1 charge: " << conner.getTrk1Charge() <<std::endl;
		    std::cout << "trk2 charge: " << conner.getTrk2Charge() <<std::endl;
		    std::cout << "stub1 charge: " << conner.getStub1Char() << std::endl;
		    std::cout << "stub2 charge: " <<  conner.getStub2Char() << std::endl;
		    std::cout << "VERTEX EXCEPTION. " << std::endl;
		  }		    
		}
	      }
	    }
	    converts.push_back(conner);
	  }//valid conversion radius
	}//loop3
      }//Loop2
    }//Loop1
  }//BasicClusters > 2?
  if (debugL == Pi0ConversionAlgo::pDEBUG) std::cout << "placing: " << converts.size() << " into event." << std::endl;
  std::auto_ptr<Pi0MaterialConversionCollection > converts_p(new Pi0MaterialConversionCollection);
  converts_p->assign(converts.begin(), converts.end());
  evt.put(converts_p, conversioncollection_);
}


float Pi0ConversionProducer::pimass(float eta_g, float phi_g, float e_g, float eta_e, float phi_e, float e_e, float eta_p, float phi_p, float e_p)
{


  float dphi0 = phi_e - phi_p;
  if(dphi0 > 3.1416) dphi0 -= 6.2832;
  if(dphi0 <-3.1416) dphi0 += 6.2832;
  //  float ttd1 = phi_p - phi_g;
  //  float ttd2 = phi_e - phi_g;
  
  float theta_g = 2*atan(exp(-eta_g));
  float theta_e = 2*atan(exp(-eta_e));
  float theta_p = 2*atan(exp(-eta_p));
  float et_e = e_e*sin(theta_e);
  float et_p = e_p*sin(theta_p);
  
  float r1 = et_p/1.2;
  float r2 = et_e/1.2;
  float dphi1 = -dphi0 * r2 / (r1+r2);
  float phi_gam = phi_p - dphi1; 
  float eta_gam = 0.5*(eta_p + eta_e);
  float energy_gam = e_e + e_p;
  
  float d_1 = (phi_gam - phi_g)*sin(theta_g);
  if(d_1 > 3.1416) d_1 -= 6.2832;
  if(d_1 <-3.1416) d_1 += 6.2832;
  float d_2 = (eta_gam - eta_g)*sin(theta_g);
  
  float mass = energy_gam*e_g*(d_1*d_1+d_2*d_2);
  mass = mass > 0 ? sqrt(mass) : mass;
  
  return mass;
}

Float_t Pi0ConversionProducer::GetSep(float eta1, float phi1, float eta2, float phi2){

  Float_t RSEP = 99999;
  Float_t dphi1 = fabs(phi1 -phi2);
  Float_t Dphi;
  if (dphi1 > TMath::Pi()) Dphi = 2 * TMath::Pi() - dphi1;
  else Dphi = dphi1;
  Float_t Deta = fabs(eta1 - eta2);
  RSEP = sqrt(Dphi*Dphi + Deta*Deta);
  return RSEP;
}

float Pi0ConversionProducer::emIso(std::vector <reco::BasicCluster> clus, reco::BasicCluster sel1, reco::BasicCluster sel2, reco::BasicCluster sel3){


  //first determine the direction of the cluster
  float x0 = 0;
  float y0 = 0;
  float z0 = 0;
  float e0 = 0;
  e0 += sel1.energy()+sel2.energy()+sel3.energy();
  x0 += sel1.x()+sel2.x()+sel3.x();
  y0 += sel1.y()+sel2.y()+sel3.y();
  z0 += sel1.z()+sel2.z()+sel3.z();
  
  x0 = x0 / e0;
  y0 = y0 / e0;
  z0 = z0 / e0;
  
  float phi = atan2(y0,x0);
  float theta = atan2(sqrt(x0*x0+y0*y0),z0);
  float eta = -log(tan(0.5*theta));
  
  float eiso = 0;
  //now collect the clusters in the radius
  for(int icl=0; icl<int(clus.size()); icl++){
    bool skip = false;
    if (clus[icl]==sel1 || clus[icl]==sel2 || clus[icl]==sel3)
      skip=true;
    if(skip) continue;
    
    float dphi = clus[icl].phi() - phi;
    if(dphi > 3.1416) dphi -= 6.2832;
    if(dphi <-3.1416) dphi += 6.2832;
    float deta = clus[icl].eta() - eta;
    float dr = sqrt(deta*deta+dphi*dphi);
    if(dr < .4) eiso += clus[icl].energy();
  }
  
  return eiso;
    
}
