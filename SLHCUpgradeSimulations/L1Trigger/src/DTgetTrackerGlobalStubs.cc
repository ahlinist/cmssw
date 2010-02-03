#include "SLHCUpgradeSimulations/L1Trigger/interface/DTL1SimOperation.h"


// ********************************
// *** Tracker Global Stubs *******
// ********************************

// 090122 SV Get Tracker Global Stubs
// begin
// Framework handles for the EVENT hits, digis, etc...

using namespace cmsUpgrades;

void DTL1SimOperation::getTrackerGlobalStubs(edm::Event& event, 
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
	  if( phi< 0)
	    phi += (2.*TMath::Pi());
	  float theta = (**DigisIter).position().theta();
	  /*
	  if(debug_stubs)
	    outAscii << " position" << (**DigisIter).position()
		     << "\n  direction" << (**DigisIter).direction()
		     << "\n   phi " << phi << "  theta " << theta
		     << endl;
	  */
	  // 090320 SV save Stubs and add in collection
	  TrackerStub* stub = 
	    new TrackerStub(id, x, y, z, (**DigisIter).direction(), phi, theta);
	  DTStubMatches->addStub(stub);
	}//end digi iteration
      }//end if: NGlobalDigiStubs
    }//end stacked tracker iterator
  DigiGlobalStubs.clear();
  // end Tracker Global Stubs
}
  


