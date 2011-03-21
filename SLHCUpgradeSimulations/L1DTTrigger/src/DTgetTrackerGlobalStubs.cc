#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1SimOperations.h"


// ********************************
// *** Tracker Global Stubs *******
// ********************************

// 090122 SV Get Tracker Global Stubs
// begin
// Framework handles for the EVENT hits, digis, etc...

using namespace cmsUpgrades;

void DTL1SimOperations::getTrackerGlobalStubs(edm::Event& event, 
		         		      const edm::EventSetup& eventSetup)
{
  // cout << "Checking tracker global stubs" << endl;
  StackedTrackerDetId id;
  /*
    retrieve global stubs and store into 
    map< DetId, vector<const GlobalStub_PixelDigi_ *> > DigiGlobalStubs
  */ 
  edm::Handle<GlobalStub_PixelDigi_Collection> DigiGlobalStubsHandle;
  try {
    event.getByLabel("GlobalStubsFromPixelDigis", DigiGlobalStubsHandle);
  }
  catch(...) {
    cout << "exception event.getByLabel(\"GlobalStubsFromPixelDigis\", "
	 << "DigiGlobalStubsHandle) at event " << EvtCounter << endl;
  }
  const GlobalStub_PixelDigi_Collection *tempDigiGlobalStubs = 
    DigiGlobalStubsHandle.product();
  GlobalStub_PixelDigi_Collection::const_iterator tempDigiGlobalStubIter;
  for( tempDigiGlobalStubIter = tempDigiGlobalStubs->begin();
       tempDigiGlobalStubIter != tempDigiGlobalStubs->end();
       ++tempDigiGlobalStubIter ) {
    id = tempDigiGlobalStubIter->Id();
    const GlobalStub_PixelDigi_* aStackedTrackerGlobalStubDigi =
      reinterpret_cast<const GlobalStub_PixelDigi_*>(&(*tempDigiGlobalStubIter));
    DigiGlobalStubs[id].push_back(aStackedTrackerGlobalStubDigi);
  }
  if(debug_stubs)
    outAscii 
      << "\n" << (theStackedTracker->stacks().size()) << " stacks were found\n" << endl;
  // Loop on tracker digis to find DT-Tracker matches:
  // first loop over all the stacks so to get their geometrical id;...
  for(StackedTrackerIterator = theStackedTracker->stacks().begin();
      StackedTrackerIterator != theStackedTracker->stacks().end();
      ++StackedTrackerIterator ) 
    {
      id = (*StackedTrackerIterator)->Id();
      // ... and for each of the stacks search among DigiGlobalStubs.
      int NGlobalDigiStubs = 0;
      if( DigiGlobalStubs.find( id ) != DigiGlobalStubs.end() )
	NGlobalDigiStubs = DigiGlobalStubs.find( id )->second.size();
      if(NGlobalDigiStubs) {
	if(debug_stubs) 
	  outAscii 
	    << id << " has " << NGlobalDigiStubs << " GlobalDigiStubs" << endl;
	for(vector<const GlobalStub_PixelDigi_*>::iterator DigisIter =
	      DigiGlobalStubs.find( id )->second.begin();
	    DigisIter != DigiGlobalStubs.find( id )->second.end();
	    ++DigisIter ) {
	  float x = (**DigisIter).position().x();
	  float y = (**DigisIter).position().y();
	  float z = (**DigisIter).position().z();
	  float phi = (**DigisIter).position().phi();
	  if( phi <= 0)
	    phi += (2.*TMath::Pi());
	  float theta = (**DigisIter).position().theta();
	  // apply PT cut to stubs
	  double MagneticFieldStrength = 
	    pSet.getUntrackedParameter<double>("magneticFieldStrength");
	  double PtThreshold = 
	    pSet.getUntrackedParameter<double>("ptThreshold");
	  bool flagSTUB = flagStubThreshold(**DigisIter, theStackedTracker, 
					    MagneticFieldStrength, PtThreshold);
	  /*
	  if(debug_stubs)
	    outAscii << " position" << (**DigisIter).position()
		     << "\n  direction" << (**DigisIter).direction()
		     << "\n   phi " << phi << "  theta " << theta
		     << endl;
	  */
	  // 090320 SV save Stubs and add in collection
	  TrackerStub* stub = 
	    new TrackerStub(id, x, y, z,(**DigisIter).direction(),phi,theta,flagSTUB);
	  DTStubMatches->addStub(stub);
	}//end digi iteration
      }//end if: NGlobalDigiStubs
    }//end stacked tracker iterator
  DigiGlobalStubs.clear();
  // end Tracker Global Stubs
}
  



//-----------------------------------------------------------------------------------
//begin
// From Nicola Pozzobon: 3/5/2010
/// Check if a Stub can pass a X GeV Threshold 
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
  const LocalStub<cmsUpgrades::Ref_PixelDigi_> *refLocStub = aStub.localStub();
  GlobalPoint innerHitPosition = 
    refLocStub->averagePosition( &(*theStackedTracker) , 0);
  GlobalPoint outerHitPosition = 
    refLocStub->averagePosition( &(*theStackedTracker) , 1);
  double outerPointRadius = outerHitPosition.perp();
  double innerPointRadius = innerHitPosition.perp();
  double outerPointPhi = outerHitPosition.phi();
  double innerPointPhi = innerHitPosition.phi();
  
  double deltaPhiThreshold = 
    (outerPointRadius - innerPointRadius) * mCompatibilityScalingFactor;
  double deltaPhi = outerPointPhi - innerPointPhi;
  if (deltaPhi<0) deltaPhi = -deltaPhi;
  if (deltaPhi > TMath::Pi()) deltaPhi = 2 *
    TMath::Pi() - deltaPhi; 
  if ( deltaPhi < deltaPhiThreshold ) 
    return true;
  else return false;
}
//end



