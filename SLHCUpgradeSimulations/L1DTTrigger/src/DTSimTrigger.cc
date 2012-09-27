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

#include "SimDataFormats/SLHC/interface/DTTSPhiTrigger.h"
#include "SimDataFormats/SLHC/interface/DTTSThetaTrigger.h"
#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1SimOperations.h"
#include "SimDataFormats/SLHC/src/DTUtils.h"

//PLZ begin
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "L1Trigger/GlobalMuonTrigger/test/L1MuGMTDump.h"
//PLZ end

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

using namespace std;
using namespace edm;
using namespace reco;

/*
edm::Ref<reco::GenParticleCollection> 
PATGenCandsFromSimTracksProducer::generatorRef_(const SimTrack &st, 
						const GlobalContext &g) const {
  assert(st.genpartIndex() != -1);        
  // Note that st.genpartIndex() is the barcode, not the index within 
  // GenParticleCollection, so I have to search the particle
  std::vector<int>::const_iterator it;
  if (g.barcodesAreSorted) {
    it = std::lower_bound(g.genBarcodes->begin(), 
			  g.genBarcodes->end(), 
			  st.genpartIndex());
  } else {
    it = std::find(g.genBarcodes->begin(), 
		   g.genBarcodes->end(), 
		   st.genpartIndex());
  }
 
  // Check that I found something
  // I need to check '*it == st.genpartIndex()' because lower_bound just finds 
  // the right spot for an item in a sorted list, not the item
  if ((it != g.genBarcodes->end()) && (*it == st.genpartIndex())) {
    return reco::GenParticleRef(g.gens, it - g.genBarcodes->begin());
  } else {
    return reco::GenParticleRef();
  }
}
*/ 





void DTL1SimOperations::getDTSimTrigger(edm::Event& event, 
					const edm::EventSetup& eventSetup)
{	
  // PLZ begin xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  float etagen = NAN;
  
  // Ignazio begin --------------------------------------------------------------
  Handle<GenParticleCollection> genParticles;
  try { event.getByLabel("genParticles", genParticles); }
  catch(...) {
    cout << "\nException from event.getByLabel(\"genParticles\", genParticles)"
	 << endl;
    exit(1); 
  }

  Handle<std::vector<int> > genBarcodes;
  try { event.getByLabel("genParticles", genBarcodes); }
  catch(...) {
    cout << "\nException from event.getByLabel(\"DTL1slhcPlots\", genBarcodes)"
	 << endl;
    exit(1); 
  }
  if (genParticles->size() != genBarcodes->size()) 
    throw cms::Exception("Corrupt data") 
      << "Barcodes not of the same size as GenParticles!\n";

  // The following is for debug purposes
  for(size_t i = 0; i < genParticles->size(); ++ i) {
    const GenParticle& p = (*genParticles)[i];
    int id = p.pdgId();
    // int st = p.status();
    if(debug)
      cout << id << " ---> barcode: " << (*genBarcodes)[i] << endl;
    if(p.numberOfMothers()) {
      const Candidate* mom = p.mother(); //p.mother(i);
      if(debug)
	cout << "\nnumberOfMothers " << p.numberOfMothers()
	     << "; first mom pdgId " << mom->pdgId() << endl;
    }
  }
  // Ignazio end ----------------------------------------------------------------

  // Get simtracks
  edm::Handle<edm::SimTrackContainer> simtracks_handle;
  try { event.getByLabel("g4SimHits",simtracks_handle); }
  catch(...) {
    cout << "\nException from event.getByLabel(\"g4SimHits\", simtracks_handle)"
	 << endl;
    exit(1); 
  }
  edm::SimTrackContainer const* simtracks = simtracks_handle.product();

  edm::SimTrackContainer *simtracksSorted = new SimTrackContainer(*simtracks);
  std::sort(simtracksSorted->begin(), simtracksSorted->end(), LessById());

  // Get the associated vertices
  Handle<SimVertexContainer> simVertices;
  try { event.getByLabel("g4SimHits", simVertices); }
  catch(...) {
    cout << "\nException from event.getByLabel(\"g4SimHits\", simVertices)"
	 << endl;
    exit(1); 
  }

  edm::SimTrackContainer::const_iterator isimtrk;
  int igen = 0;
  for(isimtrk=simtracks->begin(); isimtrk!=simtracks->end(); isimtrk++) {
    if(abs((*isimtrk).type())==13) { // muon!
      /*
	pxgen[igen]=(*isimtr).momentum().px();
	pygen[igen]=(*isimtr).momentum().py();
	pzgen[igen]=(*isimtr).momentum().pz();
	ptgen[igen]=(*isimtr).momentum().pt();
      */
      etagen=(*isimtrk).momentum().eta();
      /*
	phigen[igen]=(*isimtr).momentum().phi()>0 ? 
	(*isimtr).momentum().phi() : (*isimtr).momentum().phi()+2*3.14159265359;
	chagen[igen]=(*isimtr).type()>0 ? -1 : 1 ;
	vxgen[igen]=(*simvertices)[(*isimtr).vertIndex()].position().x();
	vygen[igen]=(*simvertices)[(*isimtr).vertIndex()].position().y();
	vzgen[igen]=(*simvertices)[(*isimtr).vertIndex()].position().z();
	pargen[igen]=-1;
      */
      igen++;

      /// Ignazio begin ---------------------------------------------------------
      /// SimTrack::genpartIndex() is the index of the corresponding Generator 
      /// particle in the Event container (-1 if no Genpart) 
      if(isimtrk->genpartIndex() == -1) 
	continue;
      std::vector<int>::const_iterator barcode;
      barcode = std::find(genBarcodes->begin(), 
			  genBarcodes->end(), 
			  isimtrk->genpartIndex());
      // Check that I found the right thing before getting a GenParticleRef
      edm::Ref<reco::GenParticleCollection> muRef;
      edm::Ref<reco::GenParticleCollection> mumumRef;
      if((barcode != genBarcodes->end()) && (*barcode == isimtrk->genpartIndex())) 
	muRef = reco::GenParticleRef(genParticles, barcode - genBarcodes->begin());
      else
	muRef = reco::GenParticleRef();
      // I want to identify the muon mum:
      if(isimtrk->noVertex() && debug) 
	cout << "isimtrk->noVertex()" << endl;
      if(!isimtrk->noVertex()) {	
	// Pick the vertex (NB: isimtrk.vertIndex() is really an index)
	const SimVertex &vtx = (*simVertices)[isimtrk->vertIndex()];
        if(debug)
	  cout << "simtrk for this muon has vertexId " << vtx.vertexId() << endl; 
	// Check if the vertex has a parent track (otherwise, we're lost)
	if (vtx.noParent() && debug) 
	  cout << "no parent!" << endl;
	if (!vtx.noParent()) {
	  // Now note that vtx.parentIndex() is NOT an index, it's a track id, 
	  // so I have to search for it 
	  unsigned int mumSimTkId = vtx.parentIndex();
	  if(debug)
	    cout << "vtx.parentIndex() " << mumSimTkId << endl;
	  SimTrackContainer::const_iterator 
	    mumSimTk = std::lower_bound(simtracksSorted->begin(), 
					simtracksSorted->end(), 
					mumSimTkId, 
					LessById());
	  if((mumSimTk != simtracksSorted->end()) && 
	     (mumSimTk->trackId() == mumSimTkId)) { 
	    // "mumSimTk" points indeed to the parent SimTrack we were searching for!
	    if (mumSimTk->genpartIndex() != -1) {
	      std::vector<int>::const_iterator mum_barcode;
	      mum_barcode = std::find(genBarcodes->begin(), 
				      genBarcodes->end(), 
				      mumSimTk->genpartIndex());
	      // Check that I found something
	      // I need to check '*mum_barcode == mumSimTk->genpartIndex()' because 
	      // lower_bound just finds the right spot for an item in a sorted list, 
	      // not the item.
	      if ((mum_barcode != genBarcodes->end()) && 
		  (*mum_barcode == mumSimTk->genpartIndex())) {
		// Ok, we have the mum barcode.
		unsigned int mumidx = mum_barcode - genBarcodes->begin();
		if(debug)
		  cout << "mumidx " << mumidx << endl;
		mumumRef = reco::GenParticleRef(genParticles, mumidx);
	      }
	      else
		mumumRef = reco::GenParticleRef();
	    } // if it->genpartIndex() != -1
	  } // end if found trackId() is first parent one indeed
	} // end if vertex has (track) parent
      } // end if track has associated vertex
    } // end if track belongs to muon
   /// Ignazio end --------------------------------------------------------------      
  } 
  
  /// Ignazio: per ricordare di  generalizzare al caso di eventi con piu` muoni
  if(igen > 1) cout << "muons from SimTrackContainer " << igen << endl;
  /// end Ignazio ***************************************************************

  if(fabs(etagen) > 1) 
    return;

  float thetagen = 2.*atan(exp(-etagen));	
  // PLZ end xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  if((debug_bti || debug_tsphi || debug_tstheta) && theAsciiFileName != "") 
    outAscii << "\nTrigger block\n-------------" << endl;
  
  // L1 Local Trigger Block -----------------------------------------------------
  bool BTI_done     = false;
  bool TSTHETA_done = false;
  
  // BTI
  vector<DTBtiTrigData> btitrigs = theDTTrigger->BtiTrigs();
  vector<DTBtiTrigData>::const_iterator pbti;
  int ibti = 0;
  if(debug_bti && theAsciiFileName != "") 
    outAscii << "\n[DTTrigger]: " << btitrigs.size() << " BTI triggers found" 
	     << endl;
  DTBtiTrigger* aDTBti;
  for( pbti = btitrigs.begin(); pbti != btitrigs.end(); pbti++ ) 
    {
      Global3DPoint pos = theDTTrigger->CMSPosition(&(*pbti));
      Global3DVector dir = theDTTrigger->CMSDirection(&(*pbti));
      aDTBti = new DTBtiTrigger(*pbti, pos, dir);
      BtiTrigs->push_back(*aDTBti);
      ++ibti;
      if(debug_bti && ( ibti < 100 ) && (theAsciiFileName != "")) {
	outAscii << ibti << ")" << endl; 
	outAscii << aDTBti->sprint();
      }
      delete aDTBti;
    }
  BTI_done = true;
 
  if(theRootFileNameHis != "") { // Ignazio
    // PLZ begin	
    int nbti=0;
    float th1 = -1;
    float th2 = -1;
    for ( pbti = btitrigs.begin(); pbti != btitrigs.end(); pbti++ ) {
      int bstat = pbti->station();
      int bwh = pbti->wheel();
      int bsect = pbti->sector();
      int bnum = pbti->btiNumber();
      int bbx = pbti->step();
      int bsl = pbti->btiSL();
      GlobalPoint pos = theDTTrigger->CMSPosition(&(*pbti));
      float thetatrig = 
	atan( sqrt(pos.x()*pos.x() + pos.y()*pos.y())/pos.z() );
      if(pos.z() < 0) thetatrig += TMath::Pi();
      if(bstat == 1 && bbx == 16 && bsl == 2) {th1 = thetatrig;}
      if(bstat == 2 && bbx == 16 && bsl == 2) {th2 = thetatrig;}           
      if(bstat == 1 && bbx == 16 && bsl == 2) {
	nbti++;	
	DTBtiId id = DTBtiId(bwh, bstat, bsect, 2, bnum);
	DTChamberId chaid = DTChamberId(bwh, bstat, bsect);
	const DTChamber* chamb = muonGeom->chamber(chaid);
	DTTrigGeom* _geom = new DTTrigGeom(const_cast<DTChamber*>(chamb), false);
	GlobalPoint  gpbti = _geom->CMSPosition(DTBtiId(chaid, 2, bnum)); 
	float thposx = gpbti.x();
	float thposy = gpbti.y();
	float thposz = gpbti.z(); 
	float thetabti = atan(sqrt( thposx*thposx + thposy*thposy)/thposz );
	if(thposz<0) thetabti += TMath::Pi();
	float dth = thetatrig-thetagen;
	dtheta_trig_vx -> Fill(dth);
	dth = thetabti-thetagen;
	dtheta_bti_vx -> Fill(dth);	
      }
    } 
    if (nbti >0) nbti_trig->Fill(nbti);
    if(th2 > 0 && th1 > 0) {
      float dth = th2-th1;
      dtheta_trig_st -> Fill(dth);
    }
    //PLZ end  
  }

  // TSTheta
  vector<DTChambThSegm> theTSThTrigs = theDTTrigger->TSThTrigs();
  vector<DTChambThSegm>::const_iterator tsTheta;
  if(debug_tstheta && theAsciiFileName != "")
    outAscii << "\n[DTTrigger]: " << theTSThTrigs.size() 
	     << " TSTheta triggers found" 
	     << endl;
  int itstheta = 0;
  for (tsTheta = theTSThTrigs.begin(); tsTheta != theTSThTrigs.end(); tsTheta++) {
    TSThetaTrigs->push_back(*tsTheta);
    if(debug_tstheta && tsTheta->ChamberId().station() < 3 && theAsciiFileName != "")
      outAscii << " Trigger # " << (++itstheta) 
	       << " on Station " << tsTheta->ChamberId().station() 
	       << " bx " << tsTheta->step() << endl;
  } 
  TSTHETA_done = true;  
  
  // TSPhi and  TSphi-TStheta matching ------------------------------------------
  vector<DTChambPhSegm> theTSPhTrigs = theDTTrigger->TSPhTrigs();
  vector<DTChambPhSegm>::const_iterator tsphi;
  DTTSPhiTrigger* aTSphiTrig;
  if(debug_tsphi && theAsciiFileName != "")
    outAscii << "\n[DTTrigger]: " << theTSPhTrigs.size() 
	     << " TSPhi triggers found" 
	     << endl;
  int itsphi = 0;
  for (tsphi = theTSPhTrigs.begin(); tsphi != theTSPhTrigs.end(); tsphi++) {
    Global3DPoint pos = theDTTrigger->CMSPosition(&(*tsphi)); 
    Global3DVector dir = theDTTrigger->CMSDirection(&(*tsphi));  
    // float rho = sqrt(pos.x()*pos.x()+pos.y()*pos.y());
    // cout << tsphi-> station()<< " " << tsphi->code() << " " 
    //      << tsphi->phi() << " " << rho << endl;    
    aTSphiTrig = new DTTSPhiTrigger(*tsphi, pos, dir);
    TSPhiTrigs->push_back(*aTSphiTrig);
    if(debug_tsphi && tsphi->ChamberId().station() < 3 && theAsciiFileName != "") {
      outAscii << " Trigger # " <<(++itsphi) << endl;
      outAscii << aTSphiTrig->sprint();
    }    
    // *********************************
    // *** match TSphi-BTItheta (SV) *** 
    // *********************************
    bool bti_match_found = false;
    if( ! use_TSTheta && BTI_done &&  
	(tsphi->station()==1 || tsphi->station()==2) ) {
      for(BtiTrigsCollection::iterator bti = BtiTrigs->begin();
	  bti != BtiTrigs->end(); bti++) {
	if( match(*bti, *tsphi) ) {
	  // Add DTMatch to DTMatchesCollection.
	  // To use "position" as given by a BTI, that is the station center:
          // DTMatches->addDT(*bti, *tsphi, debug_dttrackmatch_extrapolation);
	  // To use "position" as given instead by Phi server:
	  DTMatches->addDT(*bti, *aTSphiTrig, debug_dttrackmatch);
	  bti_match_found = true;
	}
      }
    }
    // *** end match TSphi-BTItheta (SV) ***
    //
    // *********************************
    // *** match TSphi-TStheta (PLZ) *** 
    // *********************************
    bool theta_match_found = false;
    if( use_TSTheta && TSTHETA_done && 
	(tsphi->station()==1 || tsphi->station()==2) ) {
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
	      DTTrigGeom* _geom = 
		new DTTrigGeom(const_cast<DTChamber*>(chamb), false);
	      GlobalPoint  gpbti = _geom->CMSPosition(DTBtiId(chaid, 2, bti_id)); 
	      GlobalVector gdbti = GlobalVector(); // ?????????????????????
	      float thposx = gpbti.x();
	      float thposy = gpbti.y();
	      float thposz = gpbti.z();  
	      int wh   = tsphi->wheel();
	      int st   = tsphi->station();
	      int se   = tsphi->sector();
	      int bx   = tsphi->step();
	      int code = tsphi->code()*4 + thqual; 
	      // code 0,1=L, 2,3=H, 4=LL, 5=HL, 6=HH 
	      if(tsphi->station() == 1) code = code + 2;
	      int phi  = tsphi->phi(); 
	      int phib = tsphi->phiB();
	      float theta = atan(sqrt( thposx*thposx + thposy*thposy)/thposz );
	      if(thposz<0) theta += TMath::Pi();
	      bool flagBxOK = false;
	      if(bx == 16) flagBxOK = true;
	      GlobalPoint pos_match(pos.x(),pos.y(),gpbti.z());
	      DTMatch* aDTMatch = 
		new DTMatch(wh, st, se, bx, code, phi, phib, theta, 
				pos_match, dir, flagBxOK);
	      DTMatches->addDT(aDTMatch); 
	      theta_match_found =true;	      
	    } // end if tstheta->code(i) > 0
	  } // end loop over i to get bti_id = (i+1)*8 - 3	  
	  int n0 = DTMatches->numDt();
	  int n1 = DTMatches->numDt(1);
	  int n2 = DTMatches->numDt(2);
	  if(debug_tstheta && theAsciiFileName != "") 
	    outAscii << " DTMatches all, station 1, station 2: " 
		     << n0 << " " << n1 << " " << n2 <<endl;
	} // end if match *tstheta, *tsphi
      } // loop over TS theta's
    } // end if use_TSTheta
    // *** end match TSphi-BTItheta (PLZ) ***
    
    /*
      Case where no Theta Trigger was found: 
      this method is allowed only for phi trigger code >= 2 !!
    */
    // Get chamber center and allow error on whole chamber width 
    bool match_found = false;
    if(bti_match_found || theta_match_found)   match_found = true;
    if( !match_found 
	&& (tsphi->station()==1 || tsphi->station()==2) 
	&& tsphi->code() >= 2 ) {
      if(debug_tstheta && theAsciiFileName != "") 
	outAscii << " theta match not found for this phi trigger " << endl;
      int wh   = tsphi->wheel();
      int st   = tsphi->station();
      int se   = tsphi->sector(); 
      int bx   = tsphi->step();
      int code = tsphi->code()*4 ; 
      // code 0,1=L, 2,3=H, 4=LL, 5=HL, 6=HH : theta quality set to 0
            if(tsphi->station() == 1) code = code + 2; 
      // force lower rank assuming it is like station 2  
      if( use_roughTheta ) {
	int phi  = tsphi->phi(); 
	int phib = tsphi->phiB();
	// Theta is set at station center with half a chamber window
	// DTChamberId chaid = DTChamberId(wh, st, se);
	// const DTChamber* chamb = muonGeom->chamber(chaid);
	// DTTrigGeom* _geom = new DTTrigGeom(const_cast<DTChamber*>(chamb), false);
	DTWireId wireId = DTWireId(wh,st,se,2,1,1);
	const DTLayer* layer = muonGeom->layer(wireId.layerId()); 
	const DTTopology& tp=layer->specificTopology();
	float  posX=tp.wirePosition(tp.firstChannel());
	LocalPoint posInLayer(posX,0.,0.);
	GlobalPoint pos_first = layer->surface().toGlobal(posInLayer);  
	int ncells = layer->specificTopology().channels();
	posX = posX + static_cast<float>(ncells) * 4.2;
	LocalPoint posInLayer2(posX,0.,0.);;
	GlobalPoint pos_last = layer->toGlobal(posInLayer2);
	float theta = (pos_first.theta()+pos_last.theta())/2.;
	posX = posX - static_cast<float>(ncells) * 2.1;
	LocalPoint posInLayer3(posX,0.,0.);
	GlobalPoint pos_center = layer->toGlobal(posInLayer3);
	GlobalVector gdbti = GlobalVector(); // ????????????????????? Dummy direction
	bool flagBxOK = false;
	if(bx == 16) flagBxOK = true;
	      GlobalPoint pos_match(pos.x(),pos.y(),pos_center.z());
	DTMatch* aDTMatch = 
	  new DTMatch(wh, st, se, bx, code, phi, phib, theta, 
			  pos_match, gdbti, flagBxOK);
	DTMatches->addDT(aDTMatch); 
	// set needed data for correct extrapolation search and flag for missing theta
	float delta_theta = fabs((pos_first.theta()-pos_last.theta())/2.);
	aDTMatch ->setTheta(delta_theta); 
      } // end if use_roughTheta
    } // end if not match_found ... 
   
    delete aTSphiTrig;  
  }
  if(theRootFileNameHis != "") {
    //PLZ begin 
    int numDTOK = 0;
    ndt_match1 -> Fill(DTMatches->numDt(1));
    ndt_match2 -> Fill(DTMatches->numDt(2));
    ndt_match -> Fill(DTMatches->numDt());
    for (int idtmatch = 0; idtmatch < DTMatches->numDt(); idtmatch++){
      if(DTMatches->dtmatch(idtmatch)->bx() == 16) numDTOK++;
    }
    if (numDTOK > 0) ndt_match_BXok -> Fill(numDTOK);
    //PLZ end
  }
  if(debug_dtmatch && theAsciiFileName != "") {
    outAscii 
      << "\n=========================================================="
      << "\nNumber of DTMatches: " <<  DTMatches->numDt()
      << "; in station 1: " << DTMatches->numDt(1)
      << "; in station 2: " << DTMatches->numDt(2)
      << endl;
    for (int idtmatch = 0; idtmatch < DTMatches->numDt(); idtmatch++){
      GlobalPoint pos = DTMatches->dtmatch(idtmatch)->CMS_Position();
      float rho = sqrt(pos.x()*pos.x()+pos.y()*pos.y());
      outAscii 
	<< " " << idtmatch << ")"
	<< " bx " << DTMatches->dtmatch(idtmatch)->bx()
	<< " st " << DTMatches->dtmatch(idtmatch)->station()
	<< " wh " << DTMatches->dtmatch(idtmatch)->wheel()
	<< " se " << DTMatches->dtmatch(idtmatch)->sector()
	<< " code " << DTMatches->dtmatch(idtmatch)->code()
	<< "\n    phi_glo " <<  DTMatches->dtmatch(idtmatch)->phi_glo()
	<< " phi_ts " <<  DTMatches->dtmatch(idtmatch)->phi_ts()
	<< " phib " <<  DTMatches->dtmatch(idtmatch)->phib_ts()
	<< " theta " << DTMatches->dtmatch(idtmatch)->theta_ts()
	<< " rho " << rho
	<< endl;
      outAscii 
	<< "==========================================================" 
	<< endl;
    }
  }
  return;  
  
  //----------------------------------------------------------------------------
  // Track Finder 
  //----------------------------------------------------------------------------
  // To get tracks as segment collections from DTTF muon sorter
  // edm::Handle<L1MuDTSegmentedTrackContainer>  DTTFSegmentedTracks_handle;
  // edm::Handle< const vector<const L1MuDTTrack*> > DTTFSegmentedTracks_handle;
  edm::Handle< const L1DTTracksCollection > DTTFTracks_handle;
  event.getByLabel( "simDttfDigis", "DTTF", DTTFTracks_handle);
  if (! DTTFTracks_handle.isValid()) {
    cout << "Invalid L1DTTracksCollection at event " << EvtCounter << endl;
    if(debug_dttf && theAsciiFileName != "") 
      outAscii 
	<< "Invalid L1DTTracksCollection at event " << EvtCounter << endl;
  }
  else {
    //
    edm::ESHandle< L1MuTriggerScales > triggerscales_handle;
    eventSetup.get< L1MuTriggerScalesRcd >().get( triggerscales_handle );
    const L1MuTriggerScales* theDTTriggerScales = triggerscales_handle.product();
    edm::ESHandle< L1MuTriggerPtScale > triggerPtScales_handle;
    eventSetup.get< L1MuTriggerPtScaleRcd >().get( triggerPtScales_handle );
    const L1MuTriggerPtScale* theDTTriggerPtScale = triggerPtScales_handle.product();
    //
    const vector<const L1MuDTTrack*>* DTTFTracks = DTTFTracks_handle.product();
    vector<const L1MuDTTrack*>::const_iterator track;
    for ( track = DTTFTracks->begin(); 
	  track != DTTFTracks->end(); track++ ) {
      if ( *track ) {
	L1MuDTTrack* anL1MuDTTrack = new L1MuDTTrack(*(*track));
	float phi = theDTTriggerScales->getPhiScale()->getLowEdge((*track)->phi());
	phi = (phi > TMath::Pi())? phi - 2.*TMath::Pi(): phi;
	anL1MuDTTrack->setPhiValue(phi);
	float eta = theDTTriggerScales->getRegionalEtaScale((*track)->type_idx())->
	  getCenter((*track)->eta());
	anL1MuDTTrack->setEtaValue(eta);
	float pt = theDTTriggerPtScale->getPtScale()->getLowEdge((*track)->pt());
	anL1MuDTTrack->setPtValue(pt);
	//
	L1MuDTTracks->push_back(anL1MuDTTrack);
	delete anL1MuDTTrack;
	if(debug_dttf && theAsciiFileName != "") {
	  outAscii 
	    << "\n********** DT SegmentedTracks print ********************************" 
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
  if(debug_dttf && theAsciiFileName != "") 
    outAscii 
      << "\n********** L1MuDTTracks: ********************************" 
      << endl;
  edm::Handle< L1MuDTTrackContainer > L1MuDttfDigis_handle;
  event.getByLabel("simDttfDigis", "DTTF", L1MuDttfDigis_handle);
  if (! L1MuDttfDigis_handle.isValid()) {
    // edm::LogInfo("DataNotFound") << "can't find L1MuDTTrackContainer with label "
    //				    << dttpgSource_.label() ;
    cout << "Invalid L1MuDTTrackContainer at event " << EvtCounter << endl;
    if(debug_dttf && theAsciiFileName != "") 
      outAscii << "Invalid L1MuDTTrackContainer at event " << EvtCounter << endl;
  }
  else {
    edm::ESHandle< L1MuTriggerScales > triggerscales_handle;
    eventSetup.get< L1MuTriggerScalesRcd >().get( triggerscales_handle );
    const L1MuTriggerScales* theDTTriggerScales = triggerscales_handle.product();
    edm::ESHandle< L1MuTriggerPtScale > triggerPtScales_handle;
    eventSetup.get< L1MuTriggerPtScaleRcd >().get( triggerPtScales_handle );
    const L1MuTriggerPtScale* theDTTriggerPtScale = triggerPtScales_handle.product();
    //
    L1MuDTTrackContainer::TrackContainer* 
      dtTracks = L1MuDttfDigis_handle->getContainer();
    for(L1MuDTTrackContainer::TrackContainer::iterator track = dtTracks->begin(); 
	track != dtTracks->end(); 
	++track ) {    
      float phi = theDTTriggerScales->getPhiScale()->getLowEdge(track->phi_packed());
      phi = ( phi > TMath::Pi() )? phi - 2.*TMath::Pi(): phi;
      track->setPhiValue(phi);
      track->setEtaValue(theDTTriggerScales->getRegionalEtaScale(track->type_idx())->
			 getCenter(track->eta_packed()));
      track->setPtValue(theDTTriggerPtScale->getPtScale()->getLowEdge(track->pt_packed()));
      if(debug_dttf && theAsciiFileName != "")
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
  if(debug_dttf && theAsciiFileName != "") 
    outAscii << "\n" << endl;  


  return;



}
