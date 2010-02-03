// Package:    DTL1SimOperation
// Class:      DTL1SimOperation
//
// Original Author:  Ignazio Lazzizzera
//         Created:  Thu Jul 30 11:56:13 CEST 2009
// $Id: DTPrimitivesToTrackerStubsMatches.cc,v 1.1 2010/02/02 18:44:30 arose Exp $
//
//
#include <algorithm>
#include <vector>

#include "SLHCUpgradeSimulations/L1Trigger/interface/DTL1SimOperation.h"
//#include "SLHCUpgradeSimulations/L1Trigger/interface/DTSeededTracklet.h"
#include "SimDataFormats/SLHC/interface/DTSeededTracklet.h"


void  DTL1SimOperation::getDTPrimitivesToTrackerStubsMatches()
// ************************************************************
// *** 090320 SV find DT primitives - Tracker Stubs matches ***
// ************************************************************
{
  // loop on dtmatches
  if(debug_dttrackmatch) outAscii 
    << "\nLooking in " << DTStubMatches->numDt() 
    << " TSPhi-TSTheta matched DT BTI's; flagBxOK ones are:" << endl;
  // for the moment just consider only the best trigger primitive at right bx 
  // (still missing "doubles" cancellation and track trigger time stamp) 
  size_t Nr_of_flagBxOK_dtmatches = 0;
  for (int dm = 0; dm < DTStubMatches->numDt(); dm++) {
    if(	DTStubMatches->dtmatch(dm)->flagBxOK() 
	//&& DTStubMatches->dtmatch(dm)->trig_order()==1 
	//&& DTStubMatches->dtmatch(dm)->code()>=16 
	//&& ! DTStubMatches->dtmatch(dm)->flagReject() // Ignazio
	) {// begin if: DTMatch selection  
      // get extrapolation to each tracker layer
      ++Nr_of_flagBxOK_dtmatches;
      for(int lay = 0; lay<StackedLayersInUseTotal; lay++) {
	if(debug_dttrackmatch) outAscii 
	  << "----------------------------------------------------------------\n"
	  << "DTmatch " << dm 
	  << " station " << DTStubMatches->dtmatch(dm)->station() 
	  << " (flagBxOK " << DTStubMatches->dtmatch(dm)->flagBxOK()
	  << ", predPhi " << DTStubMatches->dtmatch(dm)->predPhi(lay) 
	  << ", predTheta " << DTStubMatches->dtmatch(dm)->predTheta() 
	  << ", trig order " << DTStubMatches->dtmatch(dm)->trig_order() 
	  << ")\nhas matching stabs in stacked tracker layer " 
	  <<  our_to_tracker_lay_Id(lay) << ":" 
	  << endl;    
	// 090203 SV try to match tracker stubs with predicted positions: 
	// get DT data	
	int dtphi    = DTStubMatches->dtmatch(dm)->predPhi(lay);
	int dttheta  = DTStubMatches->dtmatch(dm)->predTheta();
	int sdtphi   = DTStubMatches->dtmatch(dm)->predSigmaPhi(lay);
	int sdttheta = DTStubMatches->dtmatch(dm)->predSigmaTheta(lay);
	// get and store closest stub	 
	TrackerStub* ClosestStub = DTStubMatches->getClosestStub(dtphi,dttheta,lay);
	if( ! ClosestStub->valid() ) {
	  // it is the case that no matching stub is on the layer !!
	  if(debug_dttrackmatch) 
	    outAscii 
	      << "    no matching stub on this layer!" << endl;
	  continue;
	}
	int sphi = static_cast<int>(ClosestStub->phi()*4096.);
	int stheta = static_cast<int>(ClosestStub->theta()*4096.);	
	if( ! DTStubMatches->dtmatch(dm)->isMatched(lay) ) {
	  //DTStubMatches->dtmatch(dm)->setMatchStub(lay,sphi,stheta);
	  DTStubMatches->dtmatch(dm)->setMatchStub(lay, sphi, stheta,
						   ClosestStub->position(),
						   ClosestStub->direction());
	  DTStubMatches->dtmatch(dm)->insertMatchingStubObj(ClosestStub);
	  if(debug_dttrackmatch) {
	    float nsphi = 
	      static_cast<float>(dtphi-sphi)/static_cast<float>(sdtphi);
	    float nstheta = 
	      static_cast<float>(dttheta-stheta)/static_cast<float>(sdttheta);
	    outAscii << "    closest stub " << "phi " << sphi 
		     << " at " << nsphi << " sigmas ; theta " << stheta 
		     << " at " << nstheta << " sigmas" 
		     << endl;
	    outAscii << "    x = " << ClosestStub->x() 
		     << ", y = " << ClosestStub->y() 
		     << ", z = " << ClosestStub->z() << endl; 
	    //outAscii << "    direction =" << ClosestStub->direction() << endl;
	  }
	}// end if:! DTStubMatches->dtmatch(dm)->isMatched(lay)	
      }// end lay loop
      DTStubMatches->dtmatch(dm)->setPt(pSet);
     if(debug_dttrackmatch) outAscii 
	<< "----------------------------------------------------------------\n"
	<< ">> DTMatch " << dm 
	<< " has stub matches on " 
	<< DTStubMatches->dtmatch(dm)->matchingStubsTotal() 
	<< " layers" << endl;
    }// end if: DTMatch selection
  }// end DTMatch loop
  if(debug_dttrackmatch)
    outAscii 
      << "----------------------------------------------------------------\n"
      << "Total of flagBxOK DTStubMatches this event " 
      << Nr_of_flagBxOK_dtmatches 
      << "\nTotal of BTI - TSPhi/TSTheta DT matches this event " 
      << DTStubMatches->numDt() 
      << "\n----------------------------------------------------------------"
      << endl; 

  /********************************************************************
   ********************************************************************
   *                                                                  *
   *  Next we search for DTMatch objects that have stubs in common.   *
   *                                                                  *
   ********************************************************************
   ********************************************************************/
  DTSeededTracklet::reset_DTSeededTrackletsCollectionSize();
  DTTracklet dttracklet;
  StubTracklet *first_stubtracklet, *core_stubtracklet, *stubtracklet;
  StubTracklet *new_core_stubtracklet, *new_stubtracklet;
  if(DTStubMatches->numDt() == 0) {
    // cout << "DTStubMatches->numDt() " << DTStubMatches->numDt() << endl;
    return;
  }
  DTStubMatch* aDTStubMatch = new DTStubMatch();
  int dm0 = 0; 
  /*
    Looping over the DTStubMatchesCollection to search for a DTStubMatch 
    object to be the first DTSeededTracklet:
  */
  for( ; dm0 < DTStubMatches->numDt(); dm0++ ) {
    if(! DTStubMatches->dtmatch(dm0)->flagBxOK()) 
      continue; 
    delete aDTStubMatch;
    aDTStubMatch = new DTStubMatch(*DTStubMatches->dtmatch(dm0));
    if( ! aDTStubMatch->getMatchingStubs().empty() ) {
      if(debug_dttrackmatch)
	outAscii << aDTStubMatch->writeMatchingStubs(dm0) << endl;
      break;
    }
  }
  if( dm0 == DTStubMatches->numDt() )
    return;
  DTSeededTracklet* aDTSeededTracklet = 
    new DTSeededTracklet(aDTStubMatch);
  DTSeededTracklets->push_back(aDTSeededTracklet);
  /*
    Next loop over the remaing DTStubMatch objects and merge them into 
    that of the already set DTSeededTracklet objects, having a minimum 
    number of stubs in common; if such minimum number of common stubs are
    not found, set a new a DTSeededTracklet object. 
  */
  ++dm0;
  for (int dm = dm0; dm < DTStubMatches->numDt(); dm++) {
    if(!DTStubMatches->dtmatch(dm)->flagBxOK()) continue;
    if(!DTStubMatches->dtmatch(dm)->getMatchingStubs().empty() 
       && debug_dttrackmatch)
      outAscii << DTStubMatches->dtmatch(dm)->writeMatchingStubs(dm) << endl;    
    bool brand_new_tracklet = true;
    first_stubtracklet = 
      // to see if this new DTSeededTracklet object can be merged into any 
      // already available one
      new StubTracklet(DTStubMatches->dtmatch(dm)->getMatchingStubs());
    /*
    if(debug_dttrackmatch) {
      outAscii << "first_stubtracklet" << endl;
      StubTracklet::const_iterator ist;
      for(ist = first_stubtracklet->begin(); ist != first_stubtracklet->end(); ist++)
	outAscii << (*ist)->id() << endl;
    }
    */
    size_t new_DTSeededTracklets_size = DTSeededTracklets->size();

    // loop over the already available DTSeededTracklet objects
    for (size_t j=0; j<new_DTSeededTracklets_size; j++) {
      dttracklet = (*DTSeededTracklets)[j]->theDTTracklet();
      core_stubtracklet = 
	new StubTracklet((*DTSeededTracklets)[j]->theCoreStubTracklet());
      /*
      if(debug_dttrackmatch) {  
	outAscii << "core_stubtracklet" << endl;
	StubTracklet::const_iterator ist;
	for(ist = core_stubtracklet->begin(); ist != core_stubtracklet->end(); ist++)
	  outAscii << (*ist)->id() << endl;
      }
      */
      stubtracklet = 
	new StubTracklet((*DTSeededTracklets)[j]->theStubTracklet());
      /*
      if(debug_dttrackmatch) {
	outAscii << "stubtracklet" << endl;
	StubTracklet::const_iterator ist;
	for(ist = stubtracklet->begin(); ist != stubtracklet->end(); ist++)
	  outAscii << (*ist)->id() << endl;
      }
      */
      new_core_stubtracklet = new StubTracklet();
      set_intersection(first_stubtracklet->begin(), first_stubtracklet->end(), 
		       core_stubtracklet->begin(), core_stubtracklet->end(), 
		       inserter(*new_core_stubtracklet, 
				new_core_stubtracklet->begin()),
		       lt_stub());
      //outAscii << "intersection size = " << new_core_stubtracklet->size() << endl;
      new_stubtracklet = new StubTracklet();
      set_union(first_stubtracklet->begin(), first_stubtracklet->end(), 
		stubtracklet->begin(), stubtracklet->end(), 
		inserter(*new_stubtracklet, 
			 new_stubtracklet->begin()),
		lt_stub());
      //outAscii << "union size = " << new_stubtracklet->size() << endl;
      if(new_core_stubtracklet->size() >= 3) {
	brand_new_tracklet = false;
	aDTStubMatch = new DTStubMatch(*DTStubMatches->dtmatch(dm));
	(*DTSeededTracklets)[j]->update(aDTStubMatch, 
					new_stubtracklet, 
					new_core_stubtracklet);
	if(debug_dttrackmatch) outAscii 
	  << "tracklet update from DTStubMatch, Core and Tracklet " 
	  << "sizes " << dttracklet.size() << ", " << stubtracklet->size() 
	  << ", " << core_stubtracklet->size() << " to " 
	  << (*DTSeededTracklets)[j]->theDTTracklet().size() << ", " 
	  << (*DTSeededTracklets)[j]->theCoreStubTracklet().size() << ", " 
	  << (*DTSeededTracklets)[j]->theStubTracklet().size()
	  << endl;
	delete stubtracklet; 
	delete core_stubtracklet; 
	break; 
      }
    } // end of loop over already set DTSeededTracklet objects  
    delete first_stubtracklet;

    if(brand_new_tracklet && DTStubMatches->dtmatch(dm)->matchingStubsTotal()) {
      outAscii << "Brand new DTSeededTracklet  DTStubMatch " << dm << endl;
      aDTStubMatch = new DTStubMatch(*DTStubMatches->dtmatch(dm));
      DTSeededTracklet* aDTSeededTracklet = 
	new DTSeededTracklet(aDTStubMatch);
      DTSeededTracklets->push_back(aDTSeededTracklet);
    } // end if brand_new_tracklet

  } // end of loop over DTStubMatch objects

  if(debug_dttrackmatch) outAscii
    << "----------------------------------------------------------------------------"
    << endl;
  size_t N = DTSeededTracklets->size();

  for (size_t j=0; j<N; j++) {
    (*DTSeededTracklets)[j]->set_theDTTracklet_size();
    (*DTSeededTracklets)[j]->set_theStubTracklet_size();
    (*DTSeededTracklets)[j]->set_theCoreStubTracklet_size();

    if(debug_dttrackmatch) outAscii 
      << "tracklet final: DTTracklet, CoreStubTracklet and StubTracklet " 
      << "sizes " << (*DTSeededTracklets)[j]->theDTTracklet_size()
      << ", " << (*DTSeededTracklets)[j]->theCoreStubTracklet_size()
      <<	", " << (*DTSeededTracklets)[j]->theStubTracklet_size()
      << endl;

    /*
    cout << endl;
    for(size_t d=0; d<(*DTSeededTracklets)[j]->theDTTracklet_size(); d++)
      cout << "(" << ((*DTSeededTracklets)[j]->theDTTracklet())[d]->station()
	   << ", " 
	   << ((*DTSeededTracklets)[j]->theDTTracklet())[d]->trig_order()  << ") " 
	   << flush;
    cout << endl;
    */
    (*DTSeededTracklets)[j]->sort();
    /*
    for(size_t d=0; d<(*DTSeededTracklets)[j]->theDTTracklet_size(); d++)
      cout << "(" << ((*DTSeededTracklets)[j]->theDTTracklet())[d]->station()
	   << ", " 
	   << ((*DTSeededTracklets)[j]->theDTTracklet())[d]->trig_order()  << ") " 
	   << flush;
    cout << endl;
    */
    (*DTSeededTracklets)[j]->setPt(pSet); 
  }

  if(debug_dttrackmatch)
    outAscii
      << "DTSeededTrackletCollectionSize = "
      << DTSeededTracklet::getDTSeededTrackletsCollectionSize() << endl;
  if(debug_dttrackmatch) outAscii << endl;

}


