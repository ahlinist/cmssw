// Package:    DTL1SimOperations
// Class:      DTL1SimOperations
//
// Original Author:  Ignazio Lazzizzera
//         Created:  Thu Jul 30 11:56:13 CEST 2009
// $Id: DTPrimitivesToTrackerObjectsMatches.cc,v 1.1 2010/03/03 13:09:39 arose Exp $
//
//
#include <algorithm>
#include <vector>

#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1SimOperations.h"
#include "SimDataFormats/SLHC/interface/DTSeededStubTrack.h"
#include "SimDataFormats/SLHC/src/DTUtils.h"

void  DTL1SimOperations::getDTPrimitivesToTrackerObjectsMatches()
// ****************************************************************
// *** 090320 SV find DT primitives - Tracker Stubs matches     ***
// *** 110608 PLZ add DT primitives - Tracker Tracklets matches ***
// **************************************************************** 
{
  if(debug_dttrackmatch) outAscii 
    << "\nLooking in " << DTMatches->numDt() 
    << " TSPhi-TSTheta matched DT BTI's; flagBxOK ones are:" << endl;
  // for the moment just consider only the best trigger primitive at right bx 
  // (still missing "doubles" cancellation and track trigger time stamp) 
  size_t Nr_of_flagBxOK_dtmatches = 0;
  
  for (int dm = 0; dm < DTMatches->numDt(); dm++) {
    double datum = pSet.getUntrackedParameter<double>("singleMuonPt", 0);
    if(datum)
      DTMatches->dtmatch(dm)->setGunFiredSingleMuPt(static_cast<float>(datum));
    
    if( debug_dttrackmatch
	&&  DTMatches->dtmatch(dm)->flagBxOK() 
	&&  DTMatches->dtmatch(dm)->flagReject() ) 
      outAscii
	<< "----------------------------------------------------------------\n"
	<< "DTmatch " << dm << " was rejected " << endl; 
    if( debug_dttrackmatch
	&&  !DTMatches->dtmatch(dm)->flagBxOK() )  
      outAscii
	<< "----------------------------------------------------------------\n"
	<< "DTmatch " << dm << " is at wrong bx: not considered " << endl; 
    
    if( DTMatches->dtmatch(dm)->flagBxOK() 
	//&& DTMatches->dtmatch(dm)->trig_order()==1 
	//&& DTMatches->dtmatch(dm)->code()>=16 
	&& ! DTMatches->dtmatch(dm)->flagReject() ) {
      // begin if: DTMatch selection  
      // get extrapolation to each tracker layer
      ++Nr_of_flagBxOK_dtmatches;
      
      getClosestTrackerStub(DTMatches->dtmatch(dm), dm);
      getClosestTrackerTracklet(DTMatches->dtmatch(dm), dm);     
      getMatchingTrackerTrack(DTMatches->dtmatch(dm), dm);

      for(int L1=0; L1<StackedLayersInUseTotal; L1++) {
	for(int L2=0; L2<L1; L2++) {
	  if( DTMatches->dtmatch(dm)->isMatched(L1) 
	      && DTMatches->dtmatch(dm)->isMatched(L2) ) {
	    // double deltaPhi(double phi1, double phi2) defined in 
	    // SimDataFormats/SLHC/src/DTUtils.cc
	    float dephi = deltaPhi(DTMatches->dtmatch(dm)->stub_phiCMS(L1),
				   DTMatches->dtmatch(dm)->stub_phiCMS(L2));
	    DTMatches->dtmatch(dm)->set_stubstub_dephi(L1, L2, dephi);
	  } // end if tracker layers matched
	}
      }
 
      /// for neural networks ****************************************************
      double amplf = 
	pSet.getUntrackedParameter<double>("pattern_out_ampl_factor", 1.);
      double datum = 
	pSet.getUntrackedParameter<double>("singleMuonPt", 0);
      if(datum) {
	DTMatches->dtmatch(dm)->setRTSdata(15, static_cast<int>(amplf/datum));
      }
      patternAscii << *(DTMatches->dtmatch(dm)) << endl; 
      /// end for neural networks ************************************************

      DTMatches->dtmatch(dm)->setPt(pSet);     
      // priority encoding of Mu_x_y Pt calculation
      // set PtValue, flagPt and bin
      DTMatches->dtmatch(dm)->encoderPt();
      DTMatches->dtmatch(dm)->assign_encoderPtBin();      
      // average of Mu_x_y Pt calculation
      // set PtValue, flagPtPT and bin of Mu_x_y Pt calculation
      DTMatches->dtmatch(dm)->averagePtTracklet();
      DTMatches->dtmatch(dm)->assign_averagePtBinTracklet();   
	  //the same for tracklets
	  DTMatches->dtmatch(dm)->averagePt();
	  DTMatches->dtmatch(dm)->assign_averagePtBin();   
      // set majority bin of Mu_x_y Pt calculation - inner layers only
      DTMatches->dtmatch(dm)->assign_majorityPtBin();    
      // set majority bin of Mu_x_y Pt calculation -Full longbarrel
      DTMatches->dtmatch(dm)->assign_majorityPtBinFull();
      // set majority bin of Mu_x_y Pt calculation -Tracklets
		DTMatches->dtmatch(dm)->assign_majorityPtBinTracklet();
		// set majority+average bin of Mu_x_y Pt calculation -Inner longbarrel layers
		DTMatches->dtmatch(dm)->assign_mixedmodePtBin();
		// set majority+average bin of Mu_x_y Pt calculation -Tracklets
		DTMatches->dtmatch(dm)->assign_mixedmodePtBinTracklet();
		
      // begin debug
      if(debug_dttrackmatch) {
	outAscii 
	  << "----------------------------------------------------------------\n"
	  << ">> DTMatch " << dm 
	  << " has stub matches on " 
	  << DTMatches->dtmatch(dm)->matchingStubsTotal() 
	  << " layers\n"
	  << DTMatches->dtmatch(dm)->writePhiStubToPredictedDistance();
	if(DTMatches->dtmatch(dm)->flagPt()) {
	  float Pt = DTMatches->dtmatch(dm)->Pt_encoder();	  
	  float encoder_Ptbin = DTMatches->dtmatch(dm)->Pt_encoder_bin();
	  float average_Pt = DTMatches->dtmatch(dm)->Pt_average();
	  float average_Ptbin = DTMatches->dtmatch(dm)->Pt_average_bin();
	  float majority_Ptbin = DTMatches->dtmatch(dm)->Pt_majority_bin();
	  outAscii 
	    << " assigning Pt: encoder " << Pt << " to bin " << encoder_Ptbin 
	    << " average " << average_Pt << " to bin " << average_Ptbin 
	    << " majority in bin " << majority_Ptbin 
	    << endl;
	} // end if flagPt
      } // end if debug_dttrackmatch
    } // end if for DTMatch selection by flagBxOK() && !flagReject() 
  }// end DTMatch loop
  if(debug_dttrackmatch)
    outAscii 
      << "----------------------------------------------------------------\n"
      << "Total of flagBxOK DTMatches this event " 
      << Nr_of_flagBxOK_dtmatches 
      << "\nTotal of BTI - TSPhi/TSTheta DT matches this event " 
      << DTMatches->numDt() 
      << "\n----------------------------------------------------------------"
      << endl; 
  
  setDTSeededStubTracks();
  return;
}






void  DTL1SimOperations::getClosestTrackerStub(DTMatch* dtmatch, int dm) 
{

  if(debug_dttrackmatch) 
    outAscii 
      << "----------------------------------------------------------------\n"
      << "DTmatch " << dm 
      << " station " << dtmatch->station() 
      << ", trig order " << dtmatch->trig_order() 
      << endl;
  
  for(int lay = 0; lay<StackedLayersInUseTotal; lay++) {
    if(debug_dttrackmatch) outAscii 
      << "Looking for matching stubs in stacked tracker layer " 
      <<  our_to_tracker_lay_Id(lay) << ":" 	  
      << " predPhi " << dtmatch->predPhi(lay) 
      << ", predTheta " << dtmatch->predTheta() 
      << endl;  
    // 090203 SV try to match tracker stubs with predicted positions: 
    // get DT data	
    int dtphi = dtmatch->predPhi(lay);
    float DTphiCMS = dtmatch->phiCMS();
    int dttheta  = dtmatch->predTheta();
    int sdtphi   = dtmatch->predSigmaPhi(lay);
    int sdttheta = dtmatch->predSigmaTheta(lay);
    if(!dtmatch->flagTheta()) 
      sdttheta = sdttheta + dtmatch->deltaTheta();
    // get and store closest stub	 
    TrackerStub* ClosestStub = 
      DTMatches->getClosestStub(dtphi, dttheta, sdtphi, sdttheta, lay);
    if( ! ClosestStub->valid() ) {
      // it is the case that no matching stub is on the layer !!
      if(debug_dttrackmatch) 
	outAscii 
	  << "    no matching stub on this layer!" << endl;
      continue;
    }

    int sphi = static_cast<int>(ClosestStub->phi()*4096.);
    int dist_phi = deltaPhi(dtphi, sphi);
    float fstub_dephi = deltaPhi(ClosestStub->phi(), DTphiCMS);

    float ffstub_dephi = fabs(ClosestStub->phi()-DTphiCMS);
    if( ffstub_dephi > TMath::Pi() ) {
      if( ClosestStub->phi() > TMath::Pi() )
	ffstub_dephi = fabs(ClosestStub->phi() - DTphiCMS - 2.*TMath::Pi());
      else if( DTphiCMS > TMath::Pi() )
	ffstub_dephi = fabs(ClosestStub->phi() - DTphiCMS + 2.*TMath::Pi());
    }
    if(ffstub_dephi != fstub_dephi) cout << "cazzo" << endl;
    short stub_dephi = 
      static_cast<short>(0.5 + fstub_dephi*2048./TMath::Pi());


    int stheta = static_cast<int>(ClosestStub->theta()*4096.);
    int dist_theta = abs(dttheta-stheta);
    float nsphi = 
      static_cast<float>(dist_phi)/static_cast<float>(sdtphi);
    float nstheta =  
      static_cast<float>(dist_theta)/static_cast<float>(sdttheta);

    // RTS
    /* 
       (Ignazio) To build Neural Network data we store "predicted" tracker 
       stub hit for this layer for the case we have not real matching stub:
       to notice such case, we set "nsigma" to the exagerated value of 1000.
       We also want exploit invariance for rotations around the collition
       axis, so that stub hits are given relative to DT hit.  
       Notice that I scale the 0 --> 2*TMath::Pi() interval to 0 --> 4096.
    */
    short pred_phi = 
      static_cast<short>(0.5 + (dtphi/4096. - DTphiCMS)*2048./TMath::Pi());
    pred_phi = abs(pred_phi);
    dtmatch->setRTSdata((3 + 2*lay), pred_phi);
    dtmatch->setRTSdata((4 + 2*lay), 1000);
    short NSphi = static_cast<short>(100. * (0.005 + nsphi));	
    // RTS end
    
    if( ! dtmatch->isMatched(lay) ) {
      dtmatch->set_stub_dephi(lay, fstub_dephi);
      dtmatch->setMatchingStub(lay, sphi, stheta,
			    ClosestStub->position(),
			    ClosestStub->direction());
      dtmatch->insertMatchingStubObj(ClosestStub);
      dtmatch->setRTSdata((3 + 2*lay), stub_dephi);
      dtmatch->setRTSdata( (4 + 2*lay), NSphi );
      if(debug_dttrackmatch) {
	outAscii << "    closest stub " << "phi " << sphi 
		 << " at " << nsphi << " sigmas ; theta " << stheta 
		 << " at " << nstheta << " sigmas" 
		 << endl;
	outAscii << "    x = " << ClosestStub->x() 
		 << ", y = " << ClosestStub->y() 
		 << ", z = " << ClosestStub->z() 
		 << " MC Track " << ClosestStub->MCid() << endl; 
      }

    }// end if:! dtmatch->isMatched(lay)	
  }// end lay loop
}    





void  DTL1SimOperations::getClosestTrackerTracklet(DTMatch* dtmatch, int dm) 
{

  // find closest Tracklet inside window in each tracker superlayer
  for(int superlay = 0; superlay<3; superlay++) {
    int lay =superlay*2+1;
    if(debug_dttrackmatch) 
      outAscii << endl;     
    if(debug_dttrackmatch) 
      outAscii 
	<< "Looking for matching Tracklets in tracker superlayer " 
	<<  superlay << ":" 
	<< " predPhi " << dtmatch->predPhi(lay) 
	<< ", predTheta " << dtmatch->predTheta() 
	<< endl;  
    // 110600 PLZ try to match tracker Tracklets with predicted positions: 
    // get DT data	
    int dtphi    = dtmatch->predPhi(lay);
    int dttheta  = dtmatch->predTheta();
    int sdtphi   = dtmatch->predSigmaPhi(lay);
    int sdttheta = dtmatch->predSigmaTheta(lay);
    if(!dtmatch->flagTheta()) 
      sdttheta = sdttheta + dtmatch->deltaTheta();
    // get and store closest Tracklet	 
    TrackerTracklet* ClosestTracklet = 
      DTMatches->getClosestTracklet(dtphi, dttheta, sdtphi, sdttheta, superlay);
    if( ! ClosestTracklet->valid() ) {
      // it is the case that no matching Tracklet is on the layer !!
      if(debug_dttrackmatch) 
	outAscii 
	  << "    no matching Tracklet on this layer!" << endl;
      continue;
    }
    dtmatch->setMatchingTkTracklet(superlay, ClosestTracklet);
    int sphi = static_cast<int>(ClosestTracklet->phi()*4096.);
    int dist_phi = deltaPhi(dtphi, sphi);
    int stheta = static_cast<int>(ClosestTracklet->theta()*4096.);
    int dist_theta = abs(dttheta-stheta);
    float nsphi = 
      static_cast<float>(dist_phi)/static_cast<float>(sdtphi);
    float nstheta =  
      static_cast<float>(dist_theta)/static_cast<float>(sdttheta);	
    
    if(debug_dttrackmatch) {
      outAscii << "    closest Tracklet " << "phi " << sphi 
	       << " at " << nsphi << " sigmas ; theta " << stheta 
	       << " at " << nstheta << " sigmas" 
	       << endl; 
    }
    //PLZ	}// end if:! dtmatch->isMatched(lay)	
  }// end superlay loop
}



void  DTL1SimOperations::getMatchingTrackerTrack(DTMatch* dtmatch, int dm) 
{

  // get Tracks in Matching window    
//

    if(debug_dttrackmatch)
     outAscii << " Looking for Tracker tracks matching dtmatch " << dm << endl;
     
     int dtphi    = DTMatches->dtmatch(dm)->predPhiVx();
     int dttheta  = DTMatches->dtmatch(dm)->predTheta();
     int sdtphi   = DTMatches->dtmatch(dm)->predSigmaPhiVx();
     int sdttheta = DTMatches->dtmatch(dm)->predSigmaThetaVx();
     if(!DTMatches->dtmatch(dm)->flagTheta()) 
     sdttheta = sdttheta + DTMatches->dtmatch(dm)->deltaTheta();
     vector<TrackerTrack*> Tracks_in_window;
     int ntracks = 0;
     DTMatches->getAllTracksInWindow(dtphi, dttheta,sdtphi,sdttheta,Tracks_in_window, ntracks);
       
/*     for(unsigned int i=0; i<Tracks_in_window.size(); i++) 
       outAscii << " Matching Track " << i << 
               " pt " << Tracks_in_window[i]->pt() <<
               " phi " << Tracks_in_window[i]->phi() <<
               " theta " << Tracks_in_window[i]->theta() 
	       << endl  ;*/
 
  // Check Pt Match
	       
    int st = DTMatches->dtmatch(dm)->station();
    int wh = DTMatches->dtmatch(dm)->wheel();
    int phib = DTMatches->dtmatch(dm)->phib_ts();
    int Mu_PT = DTMatches->dtmatch(dm)->DTMatch_PT(st,wh,fabs(static_cast<float>(phib)));
    int Mu_PTMin = DTMatches->dtmatch(dm)->DTMatch_PTMin(st,wh,fabs(static_cast<float>(phib))); 
    int Mu_PTMax = DTMatches->dtmatch(dm)->DTMatch_PTMax(st,wh,fabs(static_cast<float>(phib)));
        
    if(debug_dttrackmatch)
    outAscii << " phib " << phib << " PT " << Mu_PT << " PTMin " << Mu_PTMin << " PTMax " << Mu_PTMax << endl;
    
     int diff_pt_min = 1000;
     int best_track = -1;
     for(unsigned int i=0; i<Tracks_in_window.size(); i++){    
    if(debug_dttrackmatch)
       outAscii << " Matching Track " << i << 
               " pt " << Tracks_in_window[i]->pt() <<
               " phi " << Tracks_in_window[i]->phi() <<
               " theta " << Tracks_in_window[i]->theta() 
	       << endl  ;
       int track_pt = static_cast<int>(Tracks_in_window[i]->pt());
       if((track_pt >= Mu_PTMin) &&(track_pt <= Mu_PTMax)){
       int diff_pt = abs(Mu_PT-track_pt);
         if (diff_pt < diff_pt_min) {
           diff_pt_min =diff_pt;
           best_track = i;
         }	  
       }
     if(best_track >=0) {
     DTMatches->dtmatch(dm)-> setPtMatchingTrack(Tracks_in_window[best_track]->pt());
//     DTMatches->dtmatch(dm)-> setPtbinTrack(Tracks_in_window[best_track]->ptbin());
     }
     else {DTMatches->dtmatch(dm)-> setPtMatchingTrack(10000.);}
     }
}




void  DTL1SimOperations::setDTSeededStubTracks()
{ 
  /********************************************************************
   ********************************************************************
   *                                                                  *
   *  Ssearch for DTMatch objects that have stubs in common.          *
   *                                                                  *
   ********************************************************************
   ********************************************************************/
  // cout << "\nNext we search for DTMatch objects that have stubs in common." << endl; 
  DTSeededStubTrack::reset_DTSeededStubTracksCollectionSize();
  DTMatchesVector dtMatches;
  DTMatchingStubSet *first_stubset, *core_stubset, *stubset;
  DTMatchingStubSet *new_core_stubset, *new_stubset;
  if(DTMatches->numDt() == 0) {
    // cout << "DTMatches->numDt() " << DTMatches->numDt() << endl;
    return;
  }
  // cout << "DTMatch* aDTMatch = new DTMatch();" << endl;
  DTMatch* aDTMatch = new DTMatch();
  int dm0 = 0; 
  /*
    Looping over the DTMatchesCollection to search for a DTMatch 
    object to be the first DTSeededStubTrack:
  */
  for( ; dm0 < DTMatches->numDt(); dm0++ ) {
    if(! DTMatches->dtmatch(dm0)->flagBxOK()) 
      continue; 
    delete aDTMatch;
    aDTMatch = new DTMatch(*DTMatches->dtmatch(dm0));
    if( ! aDTMatch->getMatchingStubs().empty() ) {
      if(debug_dttrackmatch)
	outAscii << aDTMatch->writeMatchingStubs(dm0) << endl;
      break;
    }
  }
  if( dm0 == DTMatches->numDt() )
    return;
  DTSeededStubTrack* aDTSeededStubTrack =  new DTSeededStubTrack(aDTMatch);
  DTSeededStubTracks->push_back(aDTSeededStubTrack);
  /*
    Next loop over the remaing DTMatch objects and merge them into 
    that of the already set DTSeededStubTrack objects, having a minimum 
    number of stubs in common; if such minimum number of common stubs are
    not found, set a new a DTSeededStubTrack object. 
  */
  ++dm0;
  for (int dm = dm0; dm < DTMatches->numDt(); dm++) {
    if(!DTMatches->dtmatch(dm)->flagBxOK()) continue;
    if(!DTMatches->dtmatch(dm)->getMatchingStubs().empty() 
       && debug_dttrackmatch)
      outAscii << DTMatches->dtmatch(dm)->writeMatchingStubs(dm) << endl;    
    bool brand_new_tracklet = true;
    first_stubset = 
      // to see if this new DTSeededStubTrack object can be merged into any 
      // already available one
      new DTMatchingStubSet(DTMatches->dtmatch(dm)->getMatchingStubs());
    /*
    if(debug_dttrackmatch) {
      outAscii << "first_stubset" << endl;
      DTMatchingStubSet::const_iterator ist;
      for(ist = first_stubset->begin(); ist != first_stubset->end(); ist++)
	outAscii << (*ist)->id() << endl;
    }
    */
    size_t new_DTSeededStubTracks_size = DTSeededStubTracks->size();

    // loop over the already available DTSeededStubTrack objects
    for (size_t j=0; j<new_DTSeededStubTracks_size; j++) {
      dtMatches = (*DTSeededStubTracks)[j]->theDTMatchesVector();
      core_stubset = 
	new DTMatchingStubSet((*DTSeededStubTracks)[j]->theCoreDTMatchingStubSet());
      /*
      if(debug_dttrackmatch) {  
	outAscii << "core_stubset" << endl;
	DTMatchingStubSet::const_iterator ist;
	for(ist = core_stubset->begin(); ist != core_stubset->end(); ist++)
	  outAscii << (*ist)->id() << endl;
      }
      */
      stubset = 
	new DTMatchingStubSet((*DTSeededStubTracks)[j]->theDTMatchingStubSet());
      /*
      if(debug_dttrackmatch) {
	outAscii << "stubset" << endl;
	DTMatchingStubSet::const_iterator ist;
	for(ist = stubset->begin(); ist != stubset->end(); ist++)
	outAscii << (*ist)->id() << endl;
	}
      */
      new_core_stubset = new DTMatchingStubSet();
      set_intersection(first_stubset->begin(), first_stubset->end(), 
		       core_stubset->begin(), core_stubset->end(), 
		       inserter(*new_core_stubset, 
				new_core_stubset->begin()),
		       lt_stub());
      //outAscii << "intersection size = " << new_core_stubset->size() << endl;
      new_stubset = new DTMatchingStubSet();
      set_union(first_stubset->begin(), first_stubset->end(), 
		stubset->begin(), stubset->end(), 
		inserter(*new_stubset, 
			 new_stubset->begin()),
		lt_stub());
      //outAscii << "union size = " << new_stubset->size() << endl;
      if(new_core_stubset->size() >= 3) {
	brand_new_tracklet = false;
	aDTMatch = new DTMatch(*DTMatches->dtmatch(dm));
	(*DTSeededStubTracks)[j]->update(aDTMatch, 
					new_stubset, 
					new_core_stubset);
	if(debug_dttrackmatch) outAscii 
	  << "tracklet update from DTMatch, Core and Tracklet " 
	  << "sizes " << dtMatches.size() << ", " << stubset->size() 
	  << ", " << core_stubset->size() << " to " 
	  << (*DTSeededStubTracks)[j]->theDTMatchesVector().size() << ", " 
	  << (*DTSeededStubTracks)[j]->theCoreDTMatchingStubSet().size() << ", " 
	  << (*DTSeededStubTracks)[j]->theDTMatchingStubSet().size()
	  << endl;
	delete stubset; 
	delete core_stubset; 
	break; 
      }
    } // end of loop over already set DTSeededStubTrack objects  
    delete first_stubset;

    if(brand_new_tracklet && DTMatches->dtmatch(dm)->matchingStubsTotal()) {
      outAscii << "Brand new DTSeededStubTrack  DTMatch " << dm << endl;
      aDTMatch = new DTMatch(*DTMatches->dtmatch(dm));
      DTSeededStubTrack* aDTSeededStubTrack = 
	new DTSeededStubTrack(aDTMatch);
      DTSeededStubTracks->push_back(aDTSeededStubTrack);
    } // end if brand_new_tracklet
  } // end of loop over DTMatch objects

  if(debug_dttrackmatch) outAscii
    << "----------------------------------------------------------------------------"
    << endl;
  size_t N = DTSeededStubTracks->size();
  for (size_t j=0; j<N; j++) {
    (*DTSeededStubTracks)[j]->set_theDTMatchesVector_size();
    (*DTSeededStubTracks)[j]->set_theDTMatchingStubSet_size();
    (*DTSeededStubTracks)[j]->set_theCoreDTMatchingStubSet_size();

    if(debug_dttrackmatch) 
      outAscii 
	<< "tracklet final: "
	<< "DTMatchesVector, CoreDTMatchingStubSet and DTMatchingStubSet " 
	<< "sizes " << (*DTSeededStubTracks)[j]->theDTMatchesVector_size()
	<< ", " << (*DTSeededStubTracks)[j]->theCoreDTMatchingStubSet_size()
	<<	", " << (*DTSeededStubTracks)[j]->theDTMatchingStubSet_size()
	<< endl;
    
    /*
    cout << endl;
    for(size_t d=0; d<(*DTSeededStubTracks)[j]->theDTMatchesVector_size(); d++)
      cout << "(" << ((*DTSeededStubTracks)[j]->theDTMatchesVector())[d]->station()
	   << ", " 
	   << ((*DTSeededStubTracks)[j]->theDTMatchesVector())[d]->trig_order()  << ") " 
	   << flush;
    cout << endl;
    */

    // order according DT trigger quality coding
    (*DTSeededStubTracks)[j]->sort();
    /*
    for(size_t d=0; d<(*DTSeededStubTracks)[j]->theDTMatchesVector_size(); d++)
      cout << "(" << ((*DTSeededStubTracks)[j]->theDTMatchesVector())[d]->station()
	   << ", " 
	   << ((*DTSeededStubTracks)[j]->theDTMatchesVector())[d]->trig_order()  << ") " 
	   << flush;
    cout << endl;
    */
    (*DTSeededStubTracks)[j]->setPt(pSet);
  }

  if(debug_dttrackmatch)
    outAscii
      << "DTSeededStubTrackCollectionSize = "
      << DTSeededStubTrack::getDTSeededStubTracksCollectionSize() << endl;
  if(debug_dttrackmatch) outAscii << endl;
}
