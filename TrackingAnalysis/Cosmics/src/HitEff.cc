////////////////////////////////////////////////////////////////////////////////
// Package:          TrackingAnalysis/Cosmics
// Class:            HitEff
// Original Author:  Daniele Benedetti-INFN perugia
//
//
//*************** Important Notes ***********************************
//  The code works only for TIB-TOB modules
//  To run the code is necessary change the number of maxLostHit and maxConsecLostHit in GroupedCkfTrajectoryBuilderESProducerTIFTIBTOB.cff
//
//  replace GroupedCkfTrajectoryBuilderTIF.maxLostHit          = 6
//  replace GroupedCkfTrajectoryBuilderTIF.maxConsecLostHit    = 4
//  (or)
//  replace ckfBaseTrajectoryFilterP5.filterPset.maxLostHits = 6
//  replace ckfBaseTrajectoryFilterP5.filterPset.maxConsecLostHits = 4
//  
//  The code has been tested using a seeding from pairs and only in TIF data. 
//
//  For Layer 2,3,4 ---> Select Seed Pairs from TOB (Lay = 8,9,10)
//  For Layer 5,6,7 ---> Select Seed Pairs From TIB (Lay = 1,2,3) and TOB (Lay = 8,9,10)
//  For Layer 8,9   ---> Select Seed Pairs From TIB (Lay = 1,2,3)
//
//  I'm trying things out with triplet seeding. The standard triplet P5 seeding uses TOB-TEC,
// which only uses layers TOB2-6, so it is fine for TIB and TOB1. For the 5 outer layers, I
// use a second set of seeding based on the first 5 layers always including TIB1 because that
// is required in the hit eff code already.
//
//  Change accordly the file in RecoTracker/SpecialSeedGenerators/data/CombinatorialSeedGeneratorForCosmicsTIFTOB.cff
// (or)
//  Change accordly the file in RecoTracker/SpecialSeedGenerators/data/CombinatorialSeedGeneratorForCosmicsP5.cff
//
///////////////////////////////////////////////////////////////////////////////

// system include files
#include <memory>
#include <string>
#include <iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TrackingAnalysis/Cosmics/interface/HitEff.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h" 
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "TrackingAnalysis/Cosmics/interface/TrajectoryAtValidHit.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"

#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"
#include "CalibTracker/Records/interface/SiStripQualityRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripQuality.h"
#include "DataFormats/SiStripDetId/interface/SiStripSubStructure.h"
#include "Geometry/TrackerGeometryBuilder/interface/GluedGeomDet.h"

#include "TMath.h"
#include "TH1F.h"

//
// constructors and destructor
//

using namespace std;
HitEff::HitEff(const edm::ParameterSet& conf) : 
  conf_(conf)
{
  layers =conf_.getParameter<int>("Layer");
  DEBUG = conf_.getParameter<bool>("Debug");
}

// Virtual destructor needed.
HitEff::~HitEff() { }

void HitEff::beginJob(const edm::EventSetup& c){

  edm::Service<TFileService> fs;

  //Tree
  rechits = fs->make<TTree>("rechits","tree of recHit positions");
  rechits->Branch("RHNum",&RHNum,"RHNum/I");
  rechits->Branch("RHgpx",RHgpx,"RHgpx[RHNum]/F");
  rechits->Branch("RHgpy",RHgpy,"RHgpy[RHNum]/F");
  rechits->Branch("RHgpz",RHgpz,"RHgpz[RHNum]/F");
  rechits->Branch("RHMod",RHMod,"RHMod[RHNum]/I");
  rechits->Branch("RphiSte",RphiSte,"RphiSte[RHNum]/I");

  rechits->Branch("TRHNum",&TRHNum,"TRHNum/I");
  rechits->Branch("TRHgpx",TRHgpx,"TRHgpx[TRHNum]/F");
  rechits->Branch("TRHgpy",TRHgpy,"TRHgpy[TRHNum]/F");
  rechits->Branch("TRHgpz",TRHgpz,"TRHgpz[TRHNum]/F");
  rechits->Branch("TRHLay",TRHLay,"TRHLay[TRHNum]/I");
  rechits->Branch("TRHMod",TRHMod,"TRHMod[TRHNum]/I");

  traj = fs->make<TTree>("traj","tree of trajectory positions");
  traj->Branch("ModGlbX",&ModGlbX,"ModGlbX/F");
  traj->Branch("ModGlbY",&ModGlbY,"ModGlbY/F");
  traj->Branch("ModGlbZ",&ModGlbZ,"ModGlbZ/F");
  traj->Branch("trackAngle",&trackAngle,"trackAngle/F");
  traj->Branch("ModLocX",&ModLocX,"ModLocX/F");
  traj->Branch("ModLocY",&ModLocY,"ModLocY/F");
  traj->Branch("ModLocErrX",&ModLocErrX,"ModLocErrX/F");
  traj->Branch("ModLocErrY",&ModLocErrY,"ModLocErrY/F");
  traj->Branch("Moddiscr1",&Moddiscr1,"Moddiscr1/F");
  traj->Branch("Moddiscr2",&Moddiscr2,"Moddiscr2/F");
  traj->Branch("Modres",&Modres,"Modres/F");
  traj->Branch("ResSig",&ResSig,"ResSig/F");
  traj->Branch("ModIsBad",&ModIsBad,"ModIsBad/i");
  traj->Branch("SiStripQualBad",&SiStripQualBad,"SiStripQualBad/i");
  traj->Branch("Id",&Id,"Id/i");
  traj->Branch("run",&run,"run/i");
  traj->Branch("event",&event,"event/i");
  traj->Branch("layer",&layers,"layer/i");
  traj->Branch("rod",&rod,"rod/i");

  // take from eventSetup the SiStripDetCabling object
  c.get<SiStripDetCablingRcd>().get(SiStripDetCabling_);

  // get the SiStripQuality record
  c.get<SiStripQualityRcd>().get(SiStripQuality_);

  // get list of active detectors from SiStripDetCabling
  std::vector<uint32_t> activeDets;
  activeDets.clear(); // just in case
  SiStripDetCabling_->addActiveDetectorsRawIds(activeDets);

  // use SiStripSubStructure for selecting certain regions
  SiStripSubStructure substructure;
  std::vector<uint32_t> DetIds = activeDets;

  // Active modules
  for (std::vector<uint32_t>::const_iterator DetItr=activeDets.begin(); DetItr!=activeDets.end(); DetItr++){
    
    StripSubdetector iid=StripSubdetector(*DetItr);
    unsigned int subid=iid.subdetId();
    //cout << "subid = " << subid << "  StripSubdetector::TIB = " << StripSubdetector::TIB << "  StripSubdetector::TOB = " << StripSubdetector::TOB << endl;

    uint TkLayers = 1000;

    if(subid ==  StripSubdetector::TIB) { 
      TIBDetId tibid(*DetItr);
      uint laytib = 0;
      laytib = tibid.layer();
      TkLayers = laytib;
    }
    if(subid ==  StripSubdetector::TOB) { 
      TOBDetId tobid(*DetItr);
      uint laytob = 0;
      laytob =tobid.layer();
      TkLayers = laytob + 4;
    }
    //cout << "TkLayers = " << TkLayers << " and layers = " << layers << endl; 
    if (TkLayers  == layers ) {
      ActiveLay.push_back(*DetItr);
    }
  }
  
  events = 0;
  EventSeedCKF = 0;
  EventTrackCKF = 0;
  EventTriggCKF = 0;
  
}


void HitEff::analyze(const edm::Event& e, const edm::EventSetup& es){

  //  bool DEBUG = false;

  if (DEBUG)  cout << "beginning analyze from HitEff" << endl;

  using namespace edm;
  // Step A: Get Inputs 

  int run_nr = e.id().run();
  int ev_nr = e.id().event();

  //CombinatorialSeed
  edm::Handle<TrajectorySeedCollection> seedcollCKF;
  edm::InputTag seedTagCKF = conf_.getParameter<edm::InputTag>("combinatorialSeeds");
  e.getByLabel(seedTagCKF,seedcollCKF);
  
  //CombinatoriaTrack
  edm::Handle<reco::TrackCollection> trackCollectionCKF;
  edm::InputTag TkTagCKF = conf_.getParameter<edm::InputTag>("combinatorialTracks");
  e.getByLabel(TkTagCKF,trackCollectionCKF);
  
  edm::Handle<std::vector<Trajectory> > TrajectoryCollectionCKF;
  edm::InputTag TkTrajCKF = conf_.getParameter<edm::InputTag>("trajectories");
  e.getByLabel(TkTrajCKF,TrajectoryCollectionCKF);
  
  
  
  // RecHit
  //rphi
  edm::Handle<SiStripRecHit2DCollection> rphirecHits;
  edm::InputTag rphirecHitsTag = conf_.getParameter<edm::InputTag>("rphirecHits");
  e.getByLabel( rphirecHitsTag ,rphirecHits);

  //cout << "Number of rphi rechits = " << rphirecHits.product()->size() << endl;

  //stereo
  edm::Handle<SiStripRecHit2DCollection> stereorecHits;
  edm::InputTag stereorecHitsTag = conf_.getParameter<edm::InputTag>("stereorecHits");
  e.getByLabel( stereorecHitsTag ,stereorecHits);

  //cout << "Number of stereo rechits = " << stereorecHits.product()->size() << endl;
  
  edm::ESHandle<TrackerGeometry> tracker;
  es.get<TrackerDigiGeometryRecord>().get(tracker);
  const TrackerGeometry * tkgeom=&(* tracker);

  
  theAngleFinder = new TrackLocalAngle( tracker.product() );
  
  
  events++;
  
  
  for (int i=0;i<200;i++){
    RHgpx[i] = 0.;
    RHgpy[i] = 0.;
    RHgpz[i] = 0.;
    RHMod[i] = 0;
    RphiSte[i] = 0;
    TRHgpx[i] = 0.;
    TRHgpy[i] = 0.;
    TRHgpz[i] = 0.;
    TRHLay[i] = 0;
    TRHMod[i] = 0;
  }
  
  
  // ************ Stereo RecHit Collection
  int IsRphiSte = 2;
  int ContRH = 0;
  vSiStripRecHit2D.clear();
  SiStripRecHit2DCollection::const_iterator istripSt;
  SiStripRecHit2DCollection stripcollSt=*stereorecHits;
  
  
  if (stereorecHits.product()->size() > 0) {
    for(istripSt=stripcollSt.begin();istripSt!=stripcollSt.end();istripSt++) {
      const SiStripRecHit2D *hitSt = &*(istripSt);
      vSiStripRecHit2D.push_back(hitSt);

      IsRphiSte = 1;
      DetId idet=(*istripSt).geographicalId();
      uint idetMod = idet.rawId();
      unsigned int subid=idet.subdetId();
      GlobalPoint gpsRec = tracker->idToDet(idet)->surface().toGlobal(istripSt->localPosition());
      if    (subid==  StripSubdetector::TIB || subid==  StripSubdetector::TOB ) {
	if (ContRH < 200){
	  RHgpx[ContRH] = gpsRec.x();
	  RHgpy[ContRH] = gpsRec.y();
	  RHgpz[ContRH] = gpsRec.z();
	  RHMod[ContRH] = idetMod;
	  RphiSte[ContRH] = IsRphiSte;
	}
	ContRH++;
      }  
    }
  } 
  // ************ END Stereo RecHit Collection
  
  // ************ RPhi RecHit Collection
  SiStripRecHit2DCollection::const_iterator istrip;
  SiStripRecHit2DCollection stripcoll=*rphirecHits;
  if (rphirecHits.product()->size() > 0) {
    for(istrip=stripcoll.begin();istrip!=stripcoll.end();istrip++) {
      DetId idet=(*istrip).geographicalId();
      uint idetMod = idet.rawId();
      IsRphiSte = 0;
      
      uint toblayer=0;
      uint tiblayer=0;
      uint Totlayer=0;
      unsigned int subid=idet.subdetId();
      GlobalPoint gpsRec = tracker->idToDet(idet)->surface().toGlobal(istrip->localPosition());
      if    (subid==  StripSubdetector::TIB) {
	tiblayer= TIBDetId(idet).layer();
	Totlayer = tiblayer;
      }
      if    (subid==  StripSubdetector::TOB) {
	toblayer= TOBDetId(idet).layer();
	Totlayer = toblayer + 4;
      }

      if    (subid==  StripSubdetector::TIB || subid==  StripSubdetector::TOB ) {
	if (ContRH < 200){
	  RHgpx[ContRH] = gpsRec.x();
	  RHgpy[ContRH] = gpsRec.y();
	  RHgpz[ContRH] = gpsRec.z();
	  RHMod[ContRH] = idetMod;
	  RphiSte[ContRH] = IsRphiSte;
	}
	ContRH++;
      }
      const SiStripRecHit2D *hit = &*(istrip);
      
      //cout << "adding hit to vSiStripRecHit2D from layer = " << Totlayer << " with id = " << idetMod << endl;

      bool FakeArt = false;
      if (FakeArt == false) {
	vSiStripRecHit2D.push_back(hit);
      }
      else {
	// Simulation of Artificial RPhi Inefficiency in layers, modules
	if(Totlayer  == layers ) {
	  double prob;
	  prob = RanGen2.Rndm();
	  if (prob > 0.05) {
	    vSiStripRecHit2D.push_back(hit);
	  }
	}
	else {
	  vSiStripRecHit2D.push_back(hit);
	}
      }
    }
  } //end fill rphi vector

  //cout << "Number of hits in recHit2D vector = " << vSiStripRecHit2D.size() << endl;

  RHNum = ContRH;
  if (RHNum > 200 ) RHNum = 199;
  
  // Seed Trigg
  int SeedNumComb = 0;
  if (seedcollCKF.product()->size() > 0.) {
    SeedNumComb++;
  }
  if (SeedNumComb > 0) EventSeedCKF++;

  
  // Tracking 
  const   reco::TrackCollection *tracksCKF2=trackCollectionCKF.product();
  if (DEBUG)  cout << "number ckf tracks found = " << tracksCKF2->size() << endl;
  if (tracksCKF2->size() == 1 && SeedNumComb > -1 ){
  if (DEBUG)    cout << "starting checking good single track event" << endl;
    reco::TrackCollection::const_iterator iCKF=trackCollectionCKF.product()->begin();
    bool TIBTrigg = false;
    bool TOBTrigg = false;
    EventTrackCKF++;  
    
    // Basic Track Parameters 
    
    
    //(*iCKF).innerMomentum().phi();
    //(*iCKF).innerMomentum().eta();
    //(*iCKF).recHitsSize();
    //(*iCKF).chi2();
    

    // ************ Track Trigger
    for (trackingRecHit_iterator itCkf = iCKF->recHitsBegin();  itCkf != iCKF->recHitsEnd(); itCkf++){

  if (DEBUG)      cout << "starting rechit iterator" << endl;      

      if ((*itCkf)->isValid()){
	
	uint iidd3 = (*itCkf)->geographicalId().rawId();
	//cout << "setting first location iidd3 = " << iidd3 << endl;
	StripSubdetector strip=StripSubdetector(iidd3);
	unsigned int subid=strip.subdetId();
	//if (subid != StripSubdetector::TOB && subid != StripSubdetector::TIB) {
	//  continue;
	//}
	if (subid ==  StripSubdetector::TOB) { 
	  TOBDetId tobid(iidd3);
	  uint laytob =tobid.layer();
	  uint rod = tobid.rod()[1];
  if (DEBUG)	  cout << "hit from ckf track in layer = " << laytob+4 << "  in rod = " << rod << "  plus/minus = " << tobid.rod()[0] << endl;
	  // Trigger on TOB
	  if ( laytob == 6  ||  laytob == 5 ) TOBTrigg = true;
	}
	if (subid == StripSubdetector::TIB) {
	  TIBDetId tibid(iidd3);
	  uint laytib = tibid.layer();
  if (DEBUG)	  cout << "hit from ckf track in layer = " << laytib << endl;
	  if ( laytib == 3 || laytib == 4 ) TIBTrigg = true;
	}
      }
    }  //   ************ End Track Trigger
    
    int ContTRH = 0;      
    if (TIBTrigg && TOBTrigg) {
  if (DEBUG)      cout << "Passed HitRes trigger" << endl;      
      EventTriggCKF++; 
      
      const Trajectory traject = *(TrajectoryCollectionCKF.product()->begin());
      std::vector<TrajectoryMeasurement> TMeas=traject.measurements();
      vector<TrajectoryMeasurement>::iterator itm;
      double xloc = 0.;
      double yloc = 0.;
      double xErr = 0.;
      double yErr = 0.;
      double xglob,yglob,zglob;

  if (DEBUG)      cout << "lenght of TMeas = " << TMeas.size() << endl;      
      for (itm=TMeas.begin();itm!=TMeas.end();itm++){
  if (DEBUG)	cout << "starting TM iteration" << endl;
	ConstReferenceCountingPointer<TransientTrackingRecHit> theInHit;
	theInHit = (*itm).recHit();

	// can ask here if the recHit() is valid with an isValid()
	// should get invalid hits as well for layers where there is no found hit

	// should be able to ask TM for compatability with the recHit
	// TrackingTools/PatternTools/interface/TrajectoryMeasurement.h

	uint iidd = theInHit->geographicalId().rawId();

	//cout << "setting second location iidd = " << iidd << endl;
	
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
  if (DEBUG)	cout << "TKlayer from trajectory: " << TKlayers << "  from module = " << iidd <<  "   matched/stereo/rphi = " << ((iidd & 0x3)==0) << "/" << ((iidd & 0x3)==1) << "/" << ((iidd & 0x3)==2) << endl;

	// Modules Constraints
	TrajectoryAtValidHit*  TM = new TrajectoryAtValidHit(*itm,tkgeom);

	// --> Get trajectory from combinatedState 

	xloc = TM->localX();
	yloc = TM->localY();
	
	xErr =  TM->localErrorX();
	yErr =  TM->localErrorY();
	
	xglob = TM->globalX();
	yglob = TM->globalY();
	zglob = TM->globalZ();

	
	float YINTCons = 0.;
	if(TKlayers < 5) {
	  YINTCons = -1.;
	}
	if(TKlayers >= 5) {
	  YINTCons = 1.0;  // Bonding Region
	}

	
	if (layers != TKlayers) {    // Tracking position (just to draw) 
	  if (ContTRH < 200) {
	    TRHgpx[ContTRH] = xglob;
	    TRHgpy[ContTRH] = yglob;
	    TRHgpz[ContTRH] = zglob;
	    TRHLay[ContTRH] = TKlayers;
	    TRHMod[ContTRH] = iidd;
	  }
	  ContTRH++;
	}


 	if (layers == TKlayers) {   // Look at the layer not used to reconstruct the track
	  
  if (DEBUG)	  cout << "Looking at layer under study" << endl;
	  ModGlbX = 0.0; ModGlbY = 0.0; ModGlbZ = 0.0; trackAngle = 0.0; ModIsBad = 0; Id = 0; SiStripQualBad = 0; rod = 0; run = 0; event = 0;
	  ModLocX = 0.0; ModLocY = 0.0; ModLocErrX = 0.0; ModLocErrY = 0.0; Moddiscr1 = 0.0; Moddiscr2 = 0.0; Modres = 0.0; ResSig = 0.0;
	  // RPhi RecHit Efficiency 
	  if (vSiStripRecHit2D.size() > 0) {
  if (DEBUG)	    cout << "Checking rphi hits with size = " << vSiStripRecHit2D.size() << endl;
	    std::vector<const SiStripRecHit2D*>::const_iterator HitIterAll = vSiStripRecHit2D.begin();
	    int ContRPhi = 0;
	    std::vector< pair<float,float> > ResX;
	    for(; HitIterAll != vSiStripRecHit2D.end(); HitIterAll++) {
	      uint RPhiRecHitID = ((*HitIterAll)->geographicalId()).rawId();
	      //cout << "RecHit ID = " << RPhiRecHitID << "   iidd ID = " << iidd << endl;
	      if (RPhiRecHitID==369169604) cout << "modules from Christophe's list " << RPhiRecHitID << endl;
	      if (RPhiRecHitID==369169608) cout << "modules from Christophe's list " << RPhiRecHitID << endl;
	      if (RPhiRecHitID==369169612) cout << "modules from Christophe's list " << RPhiRecHitID << endl;
	      if (RPhiRecHitID == iidd) {
		float res = ((*HitIterAll)->localPosition().x()) - xloc;
		float sigma = checkConsistency((*HitIterAll), xloc, xErr);
		pair<float,float> res_sigma = pair<float,float>(res,sigma);
		ResX.push_back(res_sigma);
		ContRPhi++;
		if (DEBUG) cout << "Have ID match. residual = " << ResX.back().first << "  res sigma = " << ResX.back().second << endl;
	      }
	    }
	    float FinalRes = 1000.0;
	    float FinalResSig = 1000.0;
	    if (ContRPhi > 0) {
	      if (ContRPhi > 1) {
		//get the smallest one
		vector< pair<float,float> >::iterator ires;
		for (ires=ResX.begin(); ires!=ResX.end(); ires++){
		  //		  if ( abs((*ires).first) < abs(FinalRes) ) {
		  if ( abs((*ires).second) < abs(FinalResSig) ) {
		    FinalRes = (*ires).first;
		    FinalResSig = (*ires).second;
		  }
		  if (DEBUG) cout << "iresidual = " << (*ires).first << "  isigma = " << (*ires).second << "  and FinalRes = " << FinalRes << endl;
		}
	      }
	      else {
		FinalRes = ResX.at(0).first;
		FinalResSig = ResX.at(0).second;
	      }
	    }
	    
  if (DEBUG)	    cout << "Final rphi resolution = " << FinalRes << endl;

	    float discr1 = 0.;
	    float discr2 = 0.;
	    if (layers < 5) {
	      discr1 = 10.;
	      discr2 = fabs(5.45 - fabs(yloc))/yErr;
	    }
	    if (layers >= 5) {
	      discr1 = fabs(yloc)/yErr;
	      discr2 = fabs(9.4 - fabs(yloc))/yErr;
	    }  

  if (DEBUG)	    cout << "Checking location of rphi hit: abs(yloc) = " << abs(yloc) << "  abs(xloc) = " << abs(xloc) << "  discr1 = " << discr1 << "  discr2 = " << discr2 << endl;
	    
	    if ( abs(yloc) > YINTCons && abs(xloc) < 1500 && discr1 > 5 && discr2 > 5 ) { 
	      //cout << "activeLay.size() = " << ActiveLay.size() << endl;
	      
	      // fill ntuple varibles
	      //get global position from module id number iidd
	      ModGlbX = xglob;
	      ModGlbY = yglob;
	      ModGlbZ = zglob;	  
	      Id = iidd;
	      run = run_nr;
	      event = ev_nr;
	      if ( SiStripQuality_->IsModuleUsable(iidd) ) { 
		SiStripQualBad = 0; 
		cout << "strip qual good" << endl;
	      }	else { 
		SiStripQualBad = 1; 
		cout << "strip qual bad" << endl;
	      }
	      std::pair<float,float> monoAng = theAngleFinder->findtrackangle(*itm);
	      trackAngle = monoAng.first;
	      ModLocX = xloc;
	      ModLocY = yloc;
	      ModLocErrX = xErr;
	      ModLocErrY = yErr;
	      Moddiscr1 = discr1;
	      Moddiscr2 = discr2;
	      Modres = FinalRes;
	      ResSig = FinalResSig;
	      rod = (iidd>>5) & 0x7F;

	      if (DEBUG)	      cout << "before check good" << endl;
	      
	      if ( FinalResSig < 5.0) {  //require hit within 5sigma of track
		cout << "hit being counted as good" << endl;
		ModIsBad = 0;
		traj->Fill();
	      }
	      else {
		cout << "hit being counted as bad   ######### Invalid RPhi FinalResX " << FinalRes << " FinalRecHit " << iidd << "   TKlayers  "  <<  TKlayers  << " xloc " <<  xloc << " yloc  " << yloc << " module " << iidd << endl;
		ModIsBad = 1;
		traj->Fill();
		
		cout << " RPhi Error " << sqrt(xErr*xErr + yErr*yErr) << " ErrorX " << xErr << " yErr " <<  yErr <<  endl;
	      }
	      if (DEBUG) cout << "after check good" << endl;
	    }	    
	    if (DEBUG) cout << "after good location check" << endl;
	  }  // End RPhi
	  if (DEBUG) cout << "after rphi" << endl;
	} if (DEBUG) cout << "After layers=TKLayers if" << endl;
      } if (DEBUG) cout << "end TMeasurement loop" << endl;
    } //end loop counting triggered events
    if (DEBUG) cout << "end loop counting triggered events" << endl;
    TRHNum = ContTRH;
    if (TRHNum > 200 ) TRHNum = 199;
    rechits->Fill();   // Fill the tree only if there are tracks. 
  }
}

double HitEff::checkConsistency(const SiStripRecHit2D* rechit, double xx, double xerr)
{
  double error = sqrt(rechit->localPositionError().xx() + xerr*xerr);
  double separation = abs(rechit->localPosition().x() - xx);
  double consistency = separation/error;
  return consistency;
}

void HitEff::endJob(){
  rechits->GetDirectory()->cd();
  rechits->Write();
  traj->GetDirectory()->cd();
  traj->Write();  

  cout << " Events Analysed             " <<  events          << endl;
  cout << " Number Of Seeded events     " <<  EventSeedCKF    << endl;
  cout << " Number Of Tracked events    " <<  EventTrackCKF   << endl;
  cout << " Number Of Triggered events  " <<  EventTriggCKF   << endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HitEff);
