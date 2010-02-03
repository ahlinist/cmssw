#include "CondFormats/DataRecord/interface/L1MuGMTScalesRcd.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerScalesRcd.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerScales.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerPtScaleRcd.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerPtScale.h"

#include "DataFormats/DTDigi/interface/DTDigiCollection.h"
#include "DataFormats/MuonDetId/interface/DTWireId.h"
#include "DataFormats/MuonDetId/interface/DTLayerId.h"
#include "DataFormats/MuonDetId/interface/DTBtiId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTTopology.h"

#include "L1Trigger/DTTrigger/interface/DTTrig.h"
#include "L1Trigger/DTUtilities/interface/DTTrigGeom.h"
#include "L1Trigger/DTSectorCollector/interface/DTSCTrigUnit.h"
#include "L1Trigger/DTTrackFinder/interface/L1MuDTTrack.h"
#include "L1Trigger/GlobalMuonTrigger/src/L1MuGMTConfig.h"

//#include "SLHCUpgradeSimulations/L1Trigger/interface/DTTSPhiTrigger.h"
//#include "SLHCUpgradeSimulations/L1Trigger/interface/DTTSThetaTrigger.h"
#include "SimDataFormats/SLHC/interface/DTTSPhiTrigger.h"
#include "SimDataFormats/SLHC/interface/DTTSThetaTrigger.h"
#include "SLHCUpgradeSimulations/L1Trigger/interface/DTL1SimOperation.h"
#include "SLHCUpgradeSimulations/L1Trigger/src/DTUtils.h"


void DTL1SimOperation::getDTSimTrigger(edm::Event& event, 
				       const edm::EventSetup& eventSetup)
{
  outAscii << "\nTrigger block\n-------------" << endl;

  // L1 Local Trigger Block ----------------------------------------------------
  bool BTI_done     = false;
  bool TSTHETA_done = false;

  // BTI
  vector<DTBtiTrigData> btitrigs = theTrigger->BtiTrigs();
  vector<DTBtiTrigData>::const_iterator pbti;
  int ibti = 0;
  if(debug_bti) 
    outAscii << "\n[DTTrigger]: " << btitrigs.size() << " BTI triggers found" 
	     << endl;
  DTBtiTrigger* aDTBti;
  for ( pbti = btitrigs.begin(); pbti != btitrigs.end(); pbti++ ) 
    {
      Global3DPoint pos = theTrigger->CMSPosition(&(*pbti));
      Global3DVector dir = theTrigger->CMSDirection(&(*pbti));
      aDTBti = new DTBtiTrigger(*pbti, pos, dir);
      BtiTrigs->push_back(*aDTBti);
      ++ibti;
      if(debug_bti && ( ibti < 100 ) ) {
	outAscii << ibti << ")" << endl; 
	outAscii << aDTBti->sprint();
      }
      delete aDTBti;
    }
  BTI_done = true;  
  

  // TSTheta
  vector<DTChambThSegm> theTSThTrigs = theTrigger->TSThTrigs();
  vector<DTChambThSegm>::const_iterator tsTheta;
  if(debug_tstheta)
    outAscii << "\n[DTTrigger]: " << theTSThTrigs.size() << " TSTheta triggers found" 
	     << endl;
  int itstheta = 0;
  for (tsTheta = theTSThTrigs.begin(); tsTheta != theTSThTrigs.end(); tsTheta++) {
    TSThetaTrigs->push_back(*tsTheta);
    if(debug_tstheta)
      outAscii << " Trigger # " << (++itstheta) 
	       << " on Station " << tsTheta->ChamberId().station() 
	       << " bx " << tsTheta->step() << endl;
  } 
  TSTHETA_done = true;  
  
  // TSPhi
  vector<DTChambPhSegm> theTSPhTrigs = theTrigger->TSPhTrigs();
  vector<DTChambPhSegm>::const_iterator tsphi;
  DTTSPhiTrigger* aTSphiTrig;
  if(debug_tsphi)
    outAscii << "\n[DTTrigger]: " << theTSPhTrigs.size() << " TSPhi triggers found" 
	     << endl;
  for (tsphi = theTSPhTrigs.begin(); tsphi != theTSPhTrigs.end(); tsphi++) {
    Global3DPoint pos = theTrigger->CMSPosition(&(*tsphi)); 
    Global3DVector dir = theTrigger->CMSDirection(&(*tsphi));
    aTSphiTrig = new DTTSPhiTrigger(*tsphi, pos, dir);
    TSPhiTrigs->push_back(*aTSphiTrig);
    int itsphi = 0;
    if(debug_tsphi) {
      outAscii << (++itsphi) << ")" << endl;
      outAscii << aTSphiTrig->sprint();
    }

    // *********************************
    // *** match TSphi-BTItheta (SV) *** 
    // *********************************
    if( !USE_TSTheta && BTI_done &&  (tsphi->station()==1 || tsphi->station()==2) ) {
      for(BtiTrigsCollection::iterator bti = BtiTrigs->begin();
	  bti != BtiTrigs->end(); bti++) {
	if( match(*bti, *tsphi) )
	  // Add DTMatch to DTStubCollection
	  // To "position" as given by a BTI, that is the station center:
          // DTStubMatches->addDT(*bti, *tsphi, debug_dttrackmatch_extrapolation);
	  // To use "position" as given by Phi server:
	  DTStubMatches->addDT(*bti, *aTSphiTrig, debug_dttrackmatch);
      }
    }
    // *** end match TSphi-BTItheta (SV) ***
    //
    // *********************************
    // *** match TSphi-TStheta (PLZ) *** 
    // *********************************
    if( USE_TSTheta && TSTHETA_done && (tsphi->station()==1 || tsphi->station()==2) ) {
      for(TSThetaTrigsCollection::iterator tstheta = TSThetaTrigs->begin();
	  tstheta != TSThetaTrigs->end(); tstheta++) {
	if( match(*tstheta, *tsphi) ) {
	  for(int i=0; i<7; i++) {
	    if(tstheta->code(i) > 0) {
	      int thwh = tstheta->ChamberId().wheel();
	      int thstat = tstheta->ChamberId().station();
	      int thsect = tstheta->ChamberId().sector();
	      int thqual= tstheta->quality(i);
	      int bti_id = (i+1)*8 - 3;
	      DTBtiId id = DTBtiId(thwh, thstat, thsect, 2, bti_id);
	      DTChamberId chaid = DTChamberId(thwh, thstat, thsect);
	      const DTChamber* chamb = muonGeom->chamber(chaid);
	      DTTrigGeom* _geom = new DTTrigGeom(const_cast<DTChamber*>(chamb), false);
	      GlobalPoint  gpbti = _geom->CMSPosition(DTBtiId(chaid, 2, bti_id)); 
	      GlobalVector gdbti = GlobalVector(); // ?????????????????????
	      float thposx = gpbti.x();
	      float thposy = gpbti.y();
	      float thposz = gpbti.z();  
	      int wh   = tsphi->wheel();
	      int st   = tsphi->station();
	      int se   = tsphi->sector();
	      int bx   = tsphi->step();
	      int code = tsphi->code()*4 + thqual; // code 0,1=L, 2,3=H, 4=LL, 5=HL, 6=HH 
	      if(tsphi->station() == 1) code = code + 2;
	      int phi  = tsphi->phi(); 
	      int phib = tsphi->phiB();
	      float theta = atan(sqrt( thposx*thposx + thposy*thposy)/thposz );
	      if(thposz<0) theta += TMath::Pi();
	      bool flagBxOK = false;
	      if(bx == 16) flagBxOK = true;
	      DTStubMatch* aDTStubMatch = 
		new DTStubMatch(wh, st, se, bx, code, phi, phib, theta, 
				gpbti, gdbti, flagBxOK);
	      DTStubMatches->addDT(aDTStubMatch); 
	    } // end if tstheta->code(i) > 0
	  } // end loop over i to get bti_id = (i+1)*8 - 3	  
	  int n0 = DTStubMatches->numDt();
	  int n1 = DTStubMatches->numDt(1);
	  int n2 = DTStubMatches->numDt(2);
	  outAscii << " n0,n1,n2 " << n0 << " " << n1 << " " << n2 <<endl;
	} // end if match *tstheta, *tsphi
      } // loop over TS theta's
    } // end if USE_TSTheta
    // *** end match TSphi-BTItheta (PLZ) ***
    
    delete aTSphiTrig; 

  }
  if(debug_dtmatch) {
    outAscii 
      << "\n=========================================================="
      << "\nNumber of DTMatch: " <<  DTStubMatches->numDt()
      << "; in station 1: " << DTStubMatches->numDt(1)
      << "; in station 2: " << DTStubMatches->numDt(2)
      << endl;
    for (int idtmatch = 0; idtmatch < DTStubMatches->numDt(); idtmatch++)
      outAscii 
	<< " " << idtmatch << ")"
	<< " bx " << DTStubMatches->dtmatch(idtmatch)->bx()
	<< " st " << DTStubMatches->dtmatch(idtmatch)->station()
	<< " wh " << DTStubMatches->dtmatch(idtmatch)->wheel()
	<< " se " << DTStubMatches->dtmatch(idtmatch)->sector()
	<< " code " << DTStubMatches->dtmatch(idtmatch)->code()
	<< "\n    phi_glo " <<  DTStubMatches->dtmatch(idtmatch)->phi_glo()
	<< " phi_ts " <<  DTStubMatches->dtmatch(idtmatch)->phi_ts()
	<< " phib " <<  DTStubMatches->dtmatch(idtmatch)->phib_ts()
	<< " theta " << DTStubMatches->dtmatch(idtmatch)->theta_ts()
	<< endl;
    outAscii 
      << "==========================================================" 
      << endl;
  }

  
  return;  



  //----------------------------------------------------------------------------
  // Track Finder 
  //----------------------------------------------------------------------------
  // To get tracks as segment collections from DTTF muon sorter
  // edm::Handle<L1MuDTSegmentedTrackContainer>  DTTFSegmentedTracks_handle;
  //edm::Handle< const vector<const L1MuDTTrack*> > DTTFSegmentedTracks_handle;
  edm::Handle< const L1DTTracksCollection > DTTFTracks_handle;
  try { event.getByLabel( "simDttfDigis", 
			  "DTTF", 
			  DTTFTracks_handle); }
  catch(...) {
    cout << "exeption event.getByLabel for DTTFTracks_handle) at event "
         << EvtCounter << endl;
  }
  if (! DTTFTracks_handle.isValid()) {
    cout << "Invalid L1DTTracksCollection at event " << EvtCounter << endl;
    if(debug_dttf) 
      outAscii 
	<< "Invalid L1DTTracksCollection at event " << EvtCounter << endl;
  }
  else {
    //
    edm::ESHandle< L1MuTriggerScales > triggerscales_handle;
    eventSetup.get< L1MuTriggerScalesRcd >().get( triggerscales_handle );
    const L1MuTriggerScales* theTriggerScales = triggerscales_handle.product();
    edm::ESHandle< L1MuTriggerPtScale > triggerPtScales_handle;
    eventSetup.get< L1MuTriggerPtScaleRcd >().get( triggerPtScales_handle );
    const L1MuTriggerPtScale* theTriggerPtScale = triggerPtScales_handle.product();
    //
    const vector<const L1MuDTTrack*>* DTTFTracks = DTTFTracks_handle.product();
    vector<const L1MuDTTrack*>::const_iterator track;
    for ( track = DTTFTracks->begin(); 
	  track != DTTFTracks->end(); track++ ) {
      if ( *track ) {
	L1MuDTTrack* anL1MuDTTrack = new L1MuDTTrack(*(*track));
	float phi = theTriggerScales->getPhiScale()->getLowEdge((*track)->phi());
	phi = (phi > TMath::Pi())? phi - 2.*TMath::Pi(): phi;
	anL1MuDTTrack->setPhiValue(phi);
	float eta = theTriggerScales->getRegionalEtaScale((*track)->type_idx())->
	  getCenter((*track)->eta());
	anL1MuDTTrack->setEtaValue(eta);
	float pt = theTriggerPtScale->getPtScale()->getLowEdge((*track)->pt());
	anL1MuDTTrack->setPtValue(pt);
	//
	L1MuDTTracks->push_back(anL1MuDTTrack);
	delete anL1MuDTTrack;
	if(debug_dttf) {
	  outAscii 
	    << "\n********** DT SegmentedTracks print **********************************" 
	    << endl; 
	  if( (*track)->ptValue()  == -10. ||
	      (*track)->etaValue() == -10. ||
	      (*track)->phiValue() == -10.   )
	    outAscii << "track to be set; available data:" << endl;
	  outAscii << *(*track) << endl;                                      
	  outAscii << "\nIn " << (*track)->spid() << "found:" << endl;  
	  vector<L1MuDTTrackSegPhi> vp = (*track)->getTSphi();    
	  outAscii << "L1MuDTTrackSegPhi vector size " << vp.size() << endl;
	  for(size_t i=0; i<vp.size(); i++)                    
	    outAscii << vp[i] << endl;                                                         
	  vector<L1MuDTTrackSegEta> ve = (*track)->getTSeta(); 
	  outAscii << "L1MuDTTrackSegEta vector size " << ve.size() << endl;
	  for(size_t i=0; i<ve.size(); i++)                      
	    outAscii << ve[i] << endl;                           
	  
	} // end if debug_dttf
      } // end if *track
    } // end loop over vector<const L1MuDTTrack*>
  } // if DTTFSorterTracks_handle.isValid()
  

  //----------------------------------------------------------------------------
  // To get muon regional candidates as delivered to L1MuGMT
  if(debug_dttf) 
    outAscii 
      << "\n********** L1MuDTTracks: *********************************************" 
      << endl;
  edm::Handle< L1MuDTTrackContainer > L1MuDttfDigis_handle;
  try { event.getByLabel("simDttfDigis", "DTTF", L1MuDttfDigis_handle); }
  catch(...) {
    cout << "exeption event.getByLabel(\"DTTF\"), L1MuDttfDigis_handle); at event "
         << EvtCounter << endl;
  }
  if (! L1MuDttfDigis_handle.isValid()) {
    // edm::LogInfo("DataNotFound") << "can't find L1MuDTTrackContainer with label "
    //				    << dttpgSource_.label() ;
    cout << "Invalid L1MuDTTrackContainer at event " << EvtCounter << endl;
    if(debug_dttf) 
      outAscii << "Invalid L1MuDTTrackContainer at event " << EvtCounter << endl;
  }
  else {
    edm::ESHandle< L1MuTriggerScales > triggerscales_handle;
    eventSetup.get< L1MuTriggerScalesRcd >().get( triggerscales_handle );
    const L1MuTriggerScales* theTriggerScales = triggerscales_handle.product();
    edm::ESHandle< L1MuTriggerPtScale > triggerPtScales_handle;
    eventSetup.get< L1MuTriggerPtScaleRcd >().get( triggerPtScales_handle );
    const L1MuTriggerPtScale* theTriggerPtScale = triggerPtScales_handle.product();
    //
    L1MuDTTrackContainer::TrackContainer* 
      dtTracks = L1MuDttfDigis_handle->getContainer();
    for(L1MuDTTrackContainer::TrackContainer::iterator track = dtTracks->begin(); 
	track != dtTracks->end(); 
	++track ) {    
      float phi = theTriggerScales->getPhiScale()->getLowEdge(track->phi_packed());
      phi = ( phi > TMath::Pi() )? phi - 2.*TMath::Pi(): phi;
      track->setPhiValue(phi);
      track->setEtaValue(theTriggerScales->getRegionalEtaScale(track->type_idx())->
			 getCenter(track->eta_packed()));
      track->setPtValue(theTriggerPtScale->getPtScale()->getLowEdge(track->pt_packed()));
      if(debug_dttf)
	outAscii 
	  << setiosflags(ios::showpoint | ios::fixed | ios::right | ios::adjustfield)
	  << "  bx = " << setw(3) << track->bx() 
	  << "\n  quality (packed) = " << setw(2) << setprecision(1) 
	  << biny(track->quality_packed(), false, 3) 
	  << "\n  pt      (packed) = " << setw(3) << biny(track->pt_packed(), false, 5) 
	  << "\n  pt  (GeV) = " << setw(6) << setprecision(1) << track->ptValue()
	  << "\n  phi     (packed) = " << setw(3) <<  biny(track->phi_packed(), false, 8) 
	  << "\n  phi (rad) = " << setw(6) << setprecision(3) 
	  << ((track->phiValue() > pi_greca)? (track->phiValue() - 2*pi_greca): 
	      track->phiValue())
	  << "\n  eta     (packed) = " << setw(3) << biny(track->eta_packed(), false, 6) 
	  << "\n  eta (rad) = " << setw(6) << setprecision(3) << track->etaValue()
	  << "\n  charge  (packed) = " << setw(3) <<  biny(track->charge_packed(), false, 1) 
	  << std::endl;
    }      
  }
  if(debug_dttf) 
    outAscii << "\n" << endl;  


  return;



}

