#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1SimOperations.h"


// ********************************
// *** Tracker Global Stubs *******
// ********************************

// 090122 SV Get Tracker Global Stubs
// begin
// Framework handles for the EVENT hits, digis, etc...

/****************************************************************************
  NOTICE: starting with 4_2_3 "global stubs" just means that stubs are given
  in terms of CMS (global) coordinates.
  I got rid of commented 3.3.6 statments. 
  Ignazio
*****************************************************************************/

using namespace cmsUpgrades;

void DTL1SimOperations::getTrackerGlobalStubs(edm::Event& event, 
		         		      const edm::EventSetup& eventSetup)
{
  // cout << "Checking tracker global stubs" << endl;
  StackedTrackerDetId id;
  /*
    retrieve global stubs and store into 
    map<DetId, vector<const L1TkStub_PixelDigi_ *> > DigiGlobalStubs 
  */ 
  edm::Handle<L1TkStub_PixelDigi_Collection> DigiGlobalStubsHandle; 
  try {
    event.getByLabel("L1TkStubsFromPixelDigis", DigiGlobalStubsHandle); 
    // Notice: just as it was in 3.3.6 !
  }
  catch(...) {
    cout << "exception event.getByLabel(\"L1TkStubsFromPixelDigis\", "
	 << "DigiGlobalStubsHandle) at event " << EvtCounter << endl;
  }
  const L1TkStub_PixelDigi_Collection *tempDigiGlobalStubs = 
    DigiGlobalStubsHandle.product();
  L1TkStub_PixelDigi_Collection::const_iterator tempDigiGlobalStubIter; 
  for( tempDigiGlobalStubIter = tempDigiGlobalStubs->begin();
       tempDigiGlobalStubIter != tempDigiGlobalStubs->end();
       ++tempDigiGlobalStubIter ) {
    id = tempDigiGlobalStubIter->getDetId(); 
    const L1TkStub_PixelDigi_ *aStackedTrackerGlobalStubDigi = 
      reinterpret_cast<const L1TkStub_PixelDigi_ *>(&(*tempDigiGlobalStubIter));
    DigiGlobalStubs[id].push_back(aStackedTrackerGlobalStubDigi);
  }
  if(debug_stubs)
    outAscii 
      << "\n" << (theStackedTracker->stacks().size()) << " stacks were found\n" 
      << endl;
  /* Loop on tracker digis to find DT-Tracker matches:
     first loop over all the stacks so to get their geometrical id; */
  for(StackedTrackerIterator = theStackedTracker->stacks().begin();
      StackedTrackerIterator != theStackedTracker->stacks().end();
      ++StackedTrackerIterator ) 
    {
      id = (*StackedTrackerIterator)->Id();
      /* then for each of the stacks search among DigiGlobalStubs. */
      int NGlobalDigiStubs = 0;
      if( DigiGlobalStubs.find( id ) != DigiGlobalStubs.end() )
	NGlobalDigiStubs = DigiGlobalStubs.find( id )->second.size();
      if(NGlobalDigiStubs) {
	if(debug_stubs) 
	  outAscii << id << " has " << NGlobalDigiStubs << " GlobalDigiStubs" 
		   << endl;
	for(vector<const L1TkStub_PixelDigi_ *>::iterator DigisIter = 
	      DigiGlobalStubs.find( id )->second.begin();
	    DigisIter != DigiGlobalStubs.find( id )->second.end();
	    ++DigisIter ) {
	  float x = (**DigisIter).getPosition().x(); 
	  float y = (**DigisIter).getPosition().y();
	  float z = (**DigisIter).getPosition().z();
	  float phi = (**DigisIter).getPosition().phi();
	  int MCid = (**DigisIter).getSimTrackId(); 
	  if( phi <= 0)
	    phi += (2.*TMath::Pi());
	  float theta = (**DigisIter).getPosition().theta();
	  // apply PT cut to stubs
	  double PtThreshold = 
	    pSet.getUntrackedParameter<double>("ptThreshold");
	  bool flagSTUB = ((*DigisIter)->getRoughPt() > PtThreshold); 	  
	  if(debug_stubs)
	    outAscii << " position" << (**DigisIter).getPosition()
		     << "\n  direction" << (**DigisIter).getDirection()
		     << "\n   phi " << phi << "  theta " << theta << " MC id" << MCid
		     << endl;
	  
	  // 090320 SV save Stubs and add in collection
	  TrackerStub* stub = 
	    new TrackerStub(id, x, y, z,(**DigisIter).getDirection(),
			    phi,theta,flagSTUB,MCid);
	  DTMatches->addStub(stub);
	}//end digi iteration
      }//end if: NGlobalDigiStubs
    }//end stacked tracker iterator
  DigiGlobalStubs.clear();
  // end Tracker Global Stubs
}
  

//-----------------------------------------------------------------------------------
//  The following is not needed any more after 3.3.6
//-----------------------------------------------------------------------------------
// begin
/// From Nicola Pozzobon: 3/5/2010
//// Check if a Stub can pass a X GeV Threshold 
bool DTL1SimOperations::
flagStubThreshold(const GlobalStubRefType& aStub, 
		  const cmsUpgrades::StackedTrackerGeometry* theStackedTracker, 
		  double mMagneticFieldStrength, 
		  double mPtThreshold ) 
{
  const double KGMS_C = 2.99792458e+8;
  double mCompatibilityScalingFactor = 
    (100.0 * 2.0e+9 * mPtThreshold)/(KGMS_C * mMagneticFieldStrength);
  mCompatibilityScalingFactor = 1.0/mCompatibilityScalingFactor;

  GlobalPoint innerHitPosition = 
    aStub.getCluster(0).getAveragePosition( &(*theStackedTracker) );
  GlobalPoint outerHitPosition = 
    aStub.getCluster(1).getAveragePosition( &(*theStackedTracker) );

  double outerPointRadius = outerHitPosition.perp();
  double innerPointRadius = innerHitPosition.perp();
  double outerPointPhi = outerHitPosition.phi();
  double innerPointPhi = innerHitPosition.phi();
  
  double deltaPhiThreshold = 
    (outerPointRadius - innerPointRadius) * mCompatibilityScalingFactor;
  double deltaPhi = outerPointPhi - innerPointPhi;
  if (deltaPhi<0) deltaPhi = -deltaPhi;
  if (deltaPhi > TMath::Pi()) deltaPhi = 2 * TMath::Pi() - deltaPhi; 
    
  // PLZ begin  
  double ox = outerHitPosition.x();
  double ix = innerHitPosition.x();
  double oy = outerHitPosition.y();
  double iy = innerHitPosition.y();
  double oR = outerPointRadius;
  double iR = innerPointRadius;
  double b = (oR*oR*ix-iR*iR*ox)/(ox*iy-oy*ix);
  double a = -(oR*oR-b*oy)/ox;
  double R =sqrt(a*a+b*b)/100;
  double Rthreshold = mPtThreshold/(0.2998*mMagneticFieldStrength);
  bool iokR = false;
  if ( R > Rthreshold ) iokR =true;
  bool iokdphi = false;
  if ( deltaPhi < deltaPhiThreshold ) iokdphi = true;
  /*
    outAscii << " raggio   " << R << " soglia " << Rthreshold << " " << iokR << endl;
    outAscii << " deltaphi " << deltaPhi << " soglia " 
             << deltaPhiThreshold << " " << iokdphi << endl;
  */  
  // PLZ end
    
  if ( deltaPhi < deltaPhiThreshold ) 
    return true;
  else return false;
}

//end



