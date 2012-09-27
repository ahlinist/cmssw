#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1SimOperations.h"


// ********************************
// *** Tracker Tracklets *******
// ********************************

// 110606 PLZ Get Tracker Tracklets
// begin
// Framework handles for the EVENT hits, digis, etc...

using namespace cmsUpgrades;

void DTL1SimOperations::getTrackerTracklets(edm::Event& event, 
					    const edm::EventSetup& eventSetup)
{
  //  outAscii << "Checking tracker tracklets " << endl;
  
  edm::Handle<L1TkTracklet_PixelDigi_Collection> DigiTrackletHandle;
  try {
    event.getByLabel("L1TkTrackletsFromPixelDigis",
//		     "ShortTrackletsVtx00HelFit", 
		     "ShortTrackletsVtx00StdFit", 
		     DigiTrackletHandle);
  }
  catch(...) {
    cout << "exception event.getByLabel(\"TrackletsFromPixelDigis\", "
	 << "DigiTrackletsHandle) at event " << EvtCounter << endl;
  }
  const L1TkTracklet_PixelDigi_Collection *DigiTracklets = 
    DigiTrackletHandle.product();
  L1TkTracklet_PixelDigi_Collection::const_iterator DigiTrackletIter;
  
  for( DigiTrackletIter = DigiTracklets->begin();
       DigiTrackletIter != DigiTracklets->end();
       ++DigiTrackletIter ) {
    // Select only Hermetic Tracklets 
    // if ( !(DigiTrackletIter->isHermetic()) ) continue; 
    // 3.3.6 above; the following should be the 4.2.3 equivalent:
    edm::Ptr< L1TkStub< Ref_PixelDigi_ > > const innerStubRef = 
      DigiTrackletIter->getStubRef(0);
    edm::Ptr< L1TkStub< Ref_PixelDigi_ > > const outerStubRef = 
      DigiTrackletIter->getStubRef(1);
    if( innerStubRef->getStack()%2 == 1 || 
	innerStubRef->getLadderPhi() != outerStubRef->getLadderPhi() )
      continue;
    // end of "hermetism" requirement, that is compliance with FNAL design.
    /// Beamspot flag
    /// Methods are removed since studies show the result is not influenced 
    /// by the choice.
    // if ( (DigiTrackletIter->isBeamSpot00()) ) continue;

    unsigned int superlayer = DigiTrackletIter->getDoubleStack();
    if (superlayer == 2 || superlayer == 3) 
      continue; /// Reject tracklets in layers 4/5 and 6/7 (not for barrel)

    float pt = 
      static_cast<float>(DigiTrackletIter->getMomentum().perp());

    float rho = 
      DigiTrackletIter->getStubRefs().rbegin()->second->getPosition().perp();

    float theta1 = 
      DigiTrackletIter->getStubRefs().begin()->second->getPosition().theta();

    float theta2 = 
      DigiTrackletIter->getStubRefs().rbegin()->second->getPosition().theta();
    float theta = (theta1+theta2)/2;

    float phi = 
      DigiTrackletIter->getStubRefs().rbegin()->second->getPosition().phi();
    if( phi <= 0)
      phi += (2.*TMath::Pi());
    /*    
    if(debug_stubs)
      outAscii 
	<< "Tracklet in SL " << superlayer<<"(" << layer << "," << layer0 << ")"
	<< " pt " << pt << " rho " << rho << " phi " << phi << " theta " 
	<< theta << endl;
    */   
    //  save Tracklets and add in collection
    
    if(superlayer == 4) superlayer = 2; // save some memory
    bool ptFlag = false;
    float TrackletptThreshold = 10.;
    if (pt > TrackletptThreshold) ptFlag = true;
    TrackerTracklet* tracklet = 
      new TrackerTracklet(superlayer,rho,phi,theta,pt,ptFlag);
    DTMatches->addTracklet(tracklet);
    
    /*    
	  const Tracklet_PixelDigi_* aStackedTrackerTrackletDigi =
	  reinterpret_cast<const Tracklet_PixelDigi_*>(&(*DigiTrackletIter));
	  DigiTracklets[superlayer].push_back(aStackedTrackerTrackletDigi);
    */
  }
}
  
