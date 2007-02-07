//
// test program demonstrating the uses of the macros.
// assumes you have a test.root file in ExRootAnalysis
//
// run by issuing a command
//    root.exe -q test.C
//

#include <vector>
#include <Pi0ConversionProducer.h>
#include <Pi0ConversionAlgo.h>

Pi0ConversionProducer::Pi0ConversionProducer(const edm::ParameterSet& ps)
{
  
  // The debug level
  std::string debugString = ps.getParameter<std::string>("debugLevel");
  if      (debugString == "DEBUG")   debugL = Pi0ConversionAlgo::pDEBUG;
  else if (debugString == "INFO")    debugL = Pi0ConversionAlgo::pINFO;
  else                               debugL = Pi0ConversionAlgo::pERROR;
  
  pi0conv_p = new Pi0ConversionAlgo(debugL);
  
  basicclusterCollection_ = ps.getParameter<std::string>("basicclusterCollection");
  basicclusterProducer_ = ps.getParameter<std::string>("basicclusterProducer");
  //Note to me, steal these and hit accessor from SiStripElectron
  clusterdEtaMax_ = ps.getParameter<double>("clusterMaxEtaSep");
  clusterdRMax_ = ps.getParameter<double>("clusterdRMax");
  clusterMinPT_ = ps.getParameter<double>("clusterMinPT");
  clusterLabel_ = ps.getParameter<std::string>("ConversionLabel");
  stubminimumhits_ = ps.getParameter<int>("stubminimumhits");
  
  matchedStripRecHitsInputTag_ = ps.getParameter<edm::InputTag>("matchedStripRecHits");
  rphiStripRecHitsInputTag_    = ps.getParameter<edm::InputTag>("rphiStripRecHits");
  stereoStripRecHitsInputTag_  = ps.getParameter<edm::InputTag>("stereoStripRecHits");
  pixelRecHitsInputTag  = ps.getParameter<edm::InputTag>("pixelRecHits");  
  std::string TTRHbuilderName_ = ps.getParameter<std::string>("TTRHBuilder"); 
  produces< reco::Pi0ConversionCollection>(conversioncollection_);
  nEvt_ = 0;
}


Pi0ConversionProducer::~Pi0ConversionProducer()
{
  delete pi0conv_p;
}

void Pi0ConversionProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{

  // Get island super clusters
  Handle<reco::BasicClusterCollection> pBarrelBasicClusters;
  try {
    evt.getByLabel(basicclusterProducer_, basicclusterCollection_, pBarrelBasicClusters);
  } catch ( cms::Exception& ex ) {
    edm::LogError("Pi0ConversionProducer") << "Error! can't get collection with label " << basicclusterCollection_.c_str() ;
  }
  const reco::BasicClusterCollection* BarrelBasicClusters = pBarrelBasicClusters.product();  

  //okay, looking for three clusters in barrel ECAL, close together in dR.
  
  if (BarrelBasicClusters->size()>2){
    for (int loop1=0;loop1<int(BarrelBasicClusters->size())-2;loop1++){
      float eta1 = BarrelBasicClusters[loop1].eta();
      float energy1 = BarrelBasicClusters[loop1].energy();
      float theta1 = 2*atan(exp(-1.*eta1));
      float cluspt1 = energy1 * sin(theta1);
      if (cluspt1 < cluster_pt_thresh_) continue;
      
      for (int loop2=loop1+1;loop2<int(BarrelBasicClusters->size())-1;loop2++){
	float eta2 = BarrelBasicClusters[loop2].eta();
	float deta12 = fabs(eta1-eta2);
	if (deta12 > .2) continue;
	
	float energy2 = BarrelBasicClusters[loop2].energy();
	float theta2 = 2*atan(exp(-1.*eta2));
	float cluspt2 = energy1 * sin(theta2);
	if (cluspt2 < cluster_pt_thresh_) continue;	
	
	for (int loop3=loop1+2;loop3<int(BarrelBasicClusters->size());loop3++){
	  float eta3 = BarrelBasicClusters[loop3].eta();
	  float deta13 = fabs( eta1-eta3);
	  if (deta13 > .2) continue;
	  
	  float energy3 = BarrelBasicClusters[loop3].energy();
	  float theta3 = 2*atan(exp(-1.*eta3));
	  float cluspt3 = energy1 * sin(theta3);
	  if (cluspt3 < cluster_pt_thresh_) continue;
	  float deta23 = fabs(eta2-eta3);
	  //okay, three basic clusters, all over threshold, all close in eta.
	  
	  //Make RSep cut, require clusters vaguely close together( simple)	  
	  float phi1 = BarrelBasicClusters[loop1].phi();
	  float phi2 = BarrelBasicClusters[loop2].phi();
	  float phi3 = BarrelBasicClusters[loop3].phi();
	  
	  float dR12 = GetSep(eta1, phi1, eta2, phi2);
	  float dR23 = GetSep(eta2, phi2, eta3, phi3);
	  float dR13 = GetSep(eta1, phi1, eta3, phi3);
	  if (dR12 > clusterdRMax_
	      || dR23 > clusterdRMax_
	      || dR13 > clusterdRMax_) continue;
	  
	  //Order by eta the clusters, assign identities (algo object internally calculates Mass)
	  //two closest objects in eta are assigned to be the electrons.
	  BasicCluster ele1();
	  BasicCluster ele2();
	  BasicCluster pho();
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
							
	    //Having that valid point, call stub creation on clusters (StubCandidate, ConvCand), collect hits
	    //we'll need hits now.  Wasn't worth it until now.

	    // get Inputs
	    edm::Handle<SiStripMatchedRecHit2DCollection> matchedRecHits;
	    evt.getByLabel(matchedStripRecHitsInputTag_ ,matchedRecHits);
	    edm::Handle<SiStripRecHit2DCollection> rphiRecHits;
	    evt.getByLabel(rphiStripRecHitsInputTag_ ,rphiRecHits);
	    edm::Handle<SiStripRecHit2DCollection> stereoRecHits;
	    evt.getByLabel(stereoStripRecHitsInputTag_ ,stereoRecHits);
	    // retrieve InputTag for pixel rechits

	    // special treatment for getting pixel collection
	    // if collection exists in file, use collection from file
	    // if collection does not exist in file, create empty collection
	    const SiPixelRecHitCollection *pixelRecHitCollection = 0;
	    try {
	      edm::Handle<SiPixelRecHitCollection> pixelRecHits;
	      evt.getByLabel(pixelRecHitsInputTag, pixelRecHits);
	      pixelRecHitCollection = pixelRecHits.product();
	    }
	    catch (edm::Exception const& x) {
	      if ( x.categoryCode() == edm::errors::ProductNotFound ) {
		if ( x.history().size() == 1 ) {
		  static const SiPixelRecHitCollection s_empty;
		  pixelRecHitCollection = &s_empty;
		  edm::LogWarning("RoadSearch") << "Collection SiPixelRecHitCollection with InputTag " << pixelRecHitsInputTag << " cannot be found, using empty collection of same type.";
		}
	      }
	    }

	    //Select hit collections
	    DetHitAccess recHitVectorClass;
	    recHitVectorClass.setCollections(rphiRecHits,stereoRecHits,matchedRecHits,pixRecHits);
	    recHitVectorClass.setMode(DetHitAccess::standard);	    
	    std::vector <TrackingRecHit*> FullTracker = recHitVectorClass.getHitVector();
	    std::vector <TrackingRecHit*> Stub1;
	    std::vector <TrackingRecHit*> Stub2; 
	    edm::ESHandle<TrackerGeometry> tracker;
	    es.get<TrackerDigiGeometryRecord>().get(tracker);
	    pi0conv_p->GetStubHits(ele2, ele2, RConv, PhiConv, &FullTracker, tracker, &Stub1, &Stub2);
	    
	    //I don't think I need this.  I might, if the actual track fitter/smoother wants it.
	    edm::ESHandle<MagneticField> magneticFieldHandle;
	    es.get<IdealMagneticFieldRecord>().get(magneticFieldHandle);
	    const MagneticField * magField = magneticFieldHandle.product();

	    edm::ESHandle<TransientTrackingRecHitBuilder> theBuilder;
	    es.get<TransientRecHitRecord>().get(TTRHbuilderName_,theBuilder);
	    ttrhBuilder = theBuilder.product();

	    edm::ESHandle<MeasurementTracker>    measurementTrackerHandle;
	    es.get<CkfComponentsRecord>().get(measurementTrackerHandle);
	    theMeasurementTracker = measurementTrackerHandle.product();

	    theMeasurementTracker->update(evt);

	    //Fit Tracks, found similar code to what was used before in RoadSearchTrackCandidateMakerAlgo
	    pi0conv_p->FitTrack(Stub1, tracker, theMeasurementTracker, ttrhBuilder, magField);
	    pi0conv_p->FitTrack(Stub2);
	    //if applicable, fit vertex
	    
	    
	    pi0conv_p->FitVertex(Track1, Track2);
	    //ALWAYS APPLICABLE, if kinematically possible, put DataFormat object in event with relevant info.
	    
	    



}


float Pi0ConversionProducer::pimass(float eta_g, float phi_g, float e_g, float eta_e, float phi_e, float e_e, float eta_p, float phi_p, float e_p)
{


  float dphi0 = phi_e - phi_p;
  if(dphi0 > 3.1416) dphi0 -= 6.2832;
  if(dphi0 <-3.1416) dphi0 += 6.2832;
  float ttd1 = phi_p - phi_g;
  float ttd2 = phi_e - phi_g;
  
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
