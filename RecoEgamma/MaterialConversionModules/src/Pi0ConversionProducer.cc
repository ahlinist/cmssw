

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
#include <TMath.h>

using namespace reco;
Pi0ConversionProducer::Pi0ConversionProducer(const edm::ParameterSet& ps)
{
  
  // The debug level
  std::string debugString = ps.getParameter<std::string>("debugLevel");
  if      (debugString == "DEBUG")   debugL = Pi0ConversionAlgo::pDEBUG;
  else if (debugString == "INFO")    debugL = Pi0ConversionAlgo::pINFO;
  else                               debugL = Pi0ConversionAlgo::pERROR;
  
  pi0conv_p = new Pi0ConversionAlgo(debugL);
  
  conversioncollection_ = ps.getParameter<std::string>("conversionCollection");
  basicclusterCollection_ = ps.getParameter<std::string>("basicclusterCollection");
  basicclusterProducer_ = ps.getParameter<std::string>("basicclusterProducer");
  //Note to me, steal these and hit accessor from SiStripElectron
  clusterdEtaMax_ = ps.getParameter<double>("clusterMaxEtaSep");
  clusterdRMax_ = ps.getParameter<double>("clusterdRMax");
  cluster_pt_thresh_ = ps.getParameter<double>("cluster_pt_thresh");
  clusterMinPT_ = ps.getParameter<double>("clusterMinPT");
  clusterLabel_ = ps.getParameter<std::string>("ConversionLabel");
  stubminimumhits_ = ps.getParameter<int>("stubminimumhits");
  //builderName_ =  = ps.getParameter<std::string>("TTRHBuilder");
  matchedStripRecHitsInputTag_ = ps.getParameter<edm::InputTag>("matchedStripRecHits");
  rphiStripRecHitsInputTag_    = ps.getParameter<edm::InputTag>("rphiStripRecHits");
  stereoStripRecHitsInputTag_  = ps.getParameter<edm::InputTag>("stereoStripRecHits");
  pixelRecHitsInputTag_  = ps.getParameter<edm::InputTag>("pixelRecHits");  
  TTRHbuilderName_ = ps.getParameter<std::string>("TTRHBuilder"); 
  produces< Pi0MaterialConversionCollection>(conversioncollection_);
  nEvt_ = 0;
}


Pi0ConversionProducer::~Pi0ConversionProducer()
{
  delete pi0conv_p;
}

void Pi0ConversionProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{

  // Get island super clusters
  edm::Handle<reco::BasicClusterCollection> pBarrelBasicClusters;
  try {
    evt.getByLabel(basicclusterProducer_, basicclusterCollection_, pBarrelBasicClusters);
  } catch ( cms::Exception& ex ) {
    std::cout << "Error! can't get collection with label " << basicclusterCollection_.c_str()<<std::endl; ;
  }
  // const reco::BasicClusterCollection* BarrelBasicClusters = pBarrelBasicClusters.product();  
  reco::BasicClusterCollection BarrelBasicClusters = *pBarrelBasicClusters;

  //okay, looking for three clusters in barrel ECAL, close together in dR.
  Pi0MaterialConversionCollection converts;
  if (BarrelBasicClusters.size()>2){
    for (int loop1=0;loop1<int(BarrelBasicClusters.size())-2;loop1++){
      BasicCluster clus1 = BarrelBasicClusters[loop1];
      float eta1 = clus1.eta();
      float energy1 = clus1.energy();
      float theta1 = 2*atan(exp(-1.*eta1));
      float cluspt1 = energy1 * sin(theta1);
      if (cluspt1 < cluster_pt_thresh_) continue;
      
      for (int loop2=loop1+1;loop2<int(BarrelBasicClusters.size())-1;loop2++){
	BasicCluster clus2 = BarrelBasicClusters[loop2];
	float eta2 = clus2.eta();
	float deta12 = fabs(eta1-eta2);
	if (deta12 > .2) continue;
	
	float energy2 = clus2.energy();
	float theta2 = 2*atan(exp(-1.*eta2));
	float cluspt2 = energy2 * sin(theta2);
	if (cluspt2 < cluster_pt_thresh_) continue;	
	
	for (int loop3=loop1+2;loop3<int(BarrelBasicClusters.size());loop3++){
	  BasicCluster clus3 = BarrelBasicClusters[loop3];
	  float eta3 = clus3.eta();
	  float deta13 = fabs( eta1-eta3);
	  if (deta13 > .2) continue;
	  
	  float energy3 = clus3.energy();
	  float theta3 = 2*atan(exp(-1.*eta3));
	  float cluspt3 = energy3 * sin(theta3);
	  if (cluspt3 < cluster_pt_thresh_) continue;
	  float deta23 = fabs(eta2-eta3);
	  //okay, three basic clusters, all over threshold, all close in eta.
	  
	  //Make RSep cut, require clusters vaguely close together( simple)	  
	  float phi1 = clus1.phi();
	  float phi2 = clus2.phi();
	  float phi3 = clus3.phi();
	  
	  float dR12 = GetSep(eta1, phi1, eta2, phi2);
	  float dR23 = GetSep(eta2, phi2, eta3, phi3);
	  float dR13 = GetSep(eta1, phi1, eta3, phi3);
	  if (dR12 > clusterdRMax_
	      || dR23 > clusterdRMax_
	      || dR13 > clusterdRMax_) continue;
	  
	  //Order by eta the clusters, assign identities (algo object internally calculates Mass)
	  //two closest objects in eta are assigned to be the electrons.
	  BasicCluster ele1;
	  BasicCluster ele2;
	  BasicCluster pho;
	  if (deta13 < deta12
	      && deta13 < deta23){
	    ele1 = BarrelBasicClusters[loop1];
	    ele2 = BarrelBasicClusters[loop3];
	    pho  = BarrelBasicClusters[loop2];
	  } 
	  if (deta12 < deta13
	      && deta12 < deta23){
	    ele1 = BarrelBasicClusters[loop1];
	    ele2 = BarrelBasicClusters[loop2];
	    pho  = BarrelBasicClusters[loop3];
	  } 
	  if (deta23 < deta12
	      && deta23 < deta13){
	    ele1 = BarrelBasicClusters[loop2];
	    ele2 = BarrelBasicClusters[loop3];
	    pho  = BarrelBasicClusters[loop1];
	  } 
	  
	  //Make call on algorithm object to ensure that conversion R, Phi exist. 
	  float ele1E = ele1.energy();
	  float eleT1 = 2*atan(exp(-1.*ele1.eta()));
	  float elept1 = ele1E * sin(eleT1);
	  
	  float ele2E = ele2.energy();
	  float eleT2 = 2*atan(exp(-1.*ele2.eta()));
	  float elept2 = ele2E * sin(eleT2);
	  
	  float RConv = pi0conv_p->GetConversionR(ele1.phi(), elept1, ele2.phi(), elept2);
	  //require that this point be within the feasible reconstruction area.
	  if (RConv > 0 && RConv < 80){
	    float PhiConv = pi0conv_p->GetConversionPhi(ele1.phi(), elept1, ele2.phi(), elept2);
	    //fill in all necessary information for this conversion candidate
	    float m = pimass(pho.eta(), pho.phi(), pho.energy(),
			     ele1.eta(), ele1.phi(), ele1.energy(),
			     ele2.eta(), ele2.phi(), ele2.energy());
	    
	    Pi0MaterialConversion conner(pho.energy(),pho.eta(), pho.phi(),
						 ele1.energy(), ele1.eta(), ele1.phi(),
						 ele2.energy(), ele2.eta(), ele2.phi(),
						 RConv, PhiConv, 0, m);


	    
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

	    //Select hit collections
	    DetHitAccess recHitVectorClass;
	    const std::vector<DetId> availableIDs = rphiRecHits->ids();
	    recHitVectorClass.setCollections(rphiRecHits,stereoRecHits,matchedRecHits,pixelRecHitCollection);
	    recHitVectorClass.setMode(DetHitAccess::standard);	    
	    std::vector <TrackingRecHit*> FullTracker = recHitVectorClass.getHitVector(&availableIDs[0]);
	    std::vector <TrackingRecHit*> Stub1;
	    std::vector <TrackingRecHit*> Stub2; 
	    edm::ESHandle<TrackerGeometry> tracker;
	    es.get<TrackerDigiGeometryRecord>().get(tracker);
	    const TrackerGeometry& geometry = *tracker;
	    pi0conv_p->GetStubHits(ele2, ele2, RConv, PhiConv, &FullTracker, &geometry, &Stub1, &Stub2);
	    
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

	    theMeasurementTracker.update(evt);

	    //Fit Tracks, found similar code to what was used before in RoadSearchTrackCandidateMakerAlgo
	    reco::TrackCollection trkCan1 = pi0conv_p->FitTrack(Stub1, &theMeasurementTracker, &ttrhBuilder, &magField, &geometry,es);
	    reco::TrackCollection trkCan2 = pi0conv_p->FitTrack(Stub2, &theMeasurementTracker, &ttrhBuilder, &magField, &geometry,es);

	    if (trkCan1.size()==1){
	      conner.setTrk1PTIS(sqrt( pow(trkCan1[0].innerMomentum().X(),2) +pow(trkCan1[0].innerMomentum().Y(),2)));
	      conner.setTrk1PhiIS(trkCan1[0].innerMomentum().Phi());
	      conner.setTrk1EtaIS(trkCan1[0].innerMomentum().Eta());
	      conner.setTrk1PTFS(sqrt( pow(trkCan1[0].outerMomentum().X(),2) +pow(trkCan1[0].outerMomentum().Y(),2)));
	      conner.setTrk1PhiFS(trkCan1[0].outerMomentum().Phi());
	      conner.setTrk1EtaFS(trkCan1[0].outerMomentum().Eta());
	      conner.setTrk1NHIT(trkCan1[0].found());
	      conner.setTrk1Chi2(trkCan1[0].chi2());
	      conner.setTrk1SmallRHit(trkCan1[0].innerPosition().R());
	    }
	    if (trkCan2.size()==1){
	      //track2 quantities
	      conner.setTrk2PTIS(sqrt( pow(trkCan2[0].innerMomentum().X(),2) +pow(trkCan2[0].innerMomentum().Y(),2)));
	      conner.setTrk2PhiIS(trkCan2[0].innerMomentum().Phi());
	      conner.setTrk2EtaIS(trkCan2[0].innerMomentum().Eta());
	      conner.setTrk2PTFS(sqrt( pow(trkCan2[0].outerMomentum().X(),2) +pow(trkCan2[0].outerMomentum().Y(),2)));
	      conner.setTrk2PhiFS(trkCan2[0].outerMomentum().Phi());
	      conner.setTrk2EtaFS(trkCan2[0].outerMomentum().Eta());
	      conner.setTrk2NHIT(trkCan2[0].found());
	      conner.setTrk2Chi2(trkCan2[0].chi2());
	      conner.setTrk2SmallRHit(trkCan2[0].innerPosition().R());
	    }

	    //if applicable, fit vertex
	    if (trkCan1.size()==1 && trkCan2.size()==1){
	      std::vector<reco::TransientTrack> trks;
	      for (int i=0;i<int(trkCan1.size());++i)
		trks.push_back(reco::TransientTrack(trkCan1[i],&magField));
	      for (int i=0;i<int(trkCan2.size());++i)
		trks.push_back(reco::TransientTrack(trkCan2[i],&magField));
	      CachingVertex vert = pi0conv_p->FitVertex(trks);
	      conner.setVtxXPos(vert.position().x());
	      conner.setVtxYpos(vert.position().y());
	      conner.setVtxZpos(vert.position().z());
	      conner.setVtxRpos(sqrt(pow(vert.position().x(),2) + pow(vert.position().y(),2)));
	      conner.setVtxPhipos(vert.position().phi());
	      conner.setVtxChi2(vert.totalChiSquared()/vert.degreesOfFreedom());
	    }

	    converts.push_back(conner);
	  }//valid conversion radius
	}//loop3
      }//Loop2
    }//Loop1
  }//BasicClusters > 2?
  
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
