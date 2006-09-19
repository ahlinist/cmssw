/*
 *  Basic analyzer class which accesses CSCRecHits
 *  and compare them with muon simhits and/or digis.  
 *
 *  The output histograms are dealt with in CSCRecHitHistograms.h
 *
 *  Author: D. Fortin - UC Riverside
 */

#include "CSCRecHitReader.h"

#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include <Geometry/CSCGeometry/interface/CSCChamber.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include "Geometry/Vector/interface/GlobalPoint.h"
#include "Geometry/Vector/interface/GlobalVector.h"
#include "Geometry/Vector/interface/LocalPoint.h"
#include "Geometry/Vector/interface/LocalVector.h"
 
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "TFile.h"
#include "TVector3.h"

#include <iostream>
#include <map>

using namespace std;
using namespace edm;


// Constructor
CSCRecHitReader::CSCRecHitReader(const ParameterSet& pset){

  // Get the various input parameters
  debug            = pset.getUntrackedParameter<bool>("debug");
  rootFileName     = pset.getUntrackedParameter<string>("rootFileName");
  simHitLabel      = pset.getUntrackedParameter<string>("simHitLabel");
  stripdigiLabel   = pset.getUntrackedParameter<string>("CSCStripDigiProducer");
  wiredigiLabel    = pset.getUntrackedParameter<string>("CSCWireDigiProducer");
  recHitLabel      = pset.getUntrackedParameter<string>("recHitLabel");
  maxRechitDistance= pset.getUntrackedParameter<double>("maxRechitDistance");
  maxRechitLayer   = pset.getUntrackedParameter<int>("maxRechitPerLayer");
  maxSimhitLayer   = pset.getUntrackedParameter<int>("maxSimhitPerLayer");
  WhichEndCap      = pset.getUntrackedParameter<int>("WhichEndCap");


  // ME_1_a strips are ganged in 3 --> 3 reco hits /hit  so account for it !
  maxRechitLayerGanged = 3 * maxRechitLayer;  
  
  
  if(debug) cout << "[CSCRecHitReader] Constructor called" << endl;
  
  // Create the root file
  theFile = new TFile(rootFileName.c_str(), "RECREATE");
  theFile->cd();
  
  // Book the histograms
  hRHPME1a = new H2DRecHit("ME_1_a");  
  hRHPME1b = new H2DRecHit("ME_1_b");
  hRHPME12 = new H2DRecHit("ME_1_2");
  hRHPME13 = new H2DRecHit("ME_1_3");
  hRHPME21 = new H2DRecHit("ME_2_1");
  hRHPME22 = new H2DRecHit("ME_2_2");
  hRHPME31 = new H2DRecHit("ME_3_1");
  hRHPME32 = new H2DRecHit("ME_3_2");
  hRHPME4  = new H2DRecHit("ME_4_1");
}

// Destructor
CSCRecHitReader::~CSCRecHitReader(){
  
  if (debug) cout << "[CSCRecHitReader] Destructor called" << endl;
  
  // Write the histos to file
  theFile->cd();
  hRHPME1a->Write();
  hRHPME1b->Write();
  hRHPME12->Write();
  hRHPME13->Write();
  hRHPME21->Write();
  hRHPME22->Write();
  hRHPME31->Write();
  hRHPME32->Write();
  hRHPME4 ->Write();
  theFile->Close();
  if (debug) cout << "************* Finished writing histograms to file" << endl;
}

// The Analysis  (the main)
void CSCRecHitReader::analyze(const Event & event, const EventSetup& eventSetup){
  
  if (event.id().event()%100 == 0) cout << " Event analysed #Run: " << event.id().run()
					<< " #Event: " << event.id().event() << endl;
  
  // Get the CSC Geometry :
  ESHandle<CSCGeometry> cscGeom;
  eventSetup.get<MuonGeometryRecord>().get(cscGeom);
  
  // Get the SimHit collection :
  Handle<PSimHitContainer> simHits;
  event.getByLabel(simHitLabel, "MuonCSCHits", simHits);
  if (debug) cout << "   #SimHits: " << simHits->size() << endl;
  
  // Get the RecHits collection :
  Handle<CSCRecHit2DCollection> recHits; 
  event.getByLabel(recHitLabel, recHits);  
  if (debug) cout << "   #RecHits: " << recHits->size() << endl;
  
  // Get the CSC Digis :
  // Get collection of strip digis from event
  Handle<CSCStripDigiCollection> stripDigis;
  event.getByLabel(stripdigiLabel, "MuonCSCStripDigi", stripDigis);
  
  // Get the collection of wire digis from event
  Handle<CSCWireDigiCollection> wireDigis;
  event.getByLabel(wiredigiLabel,  "MuonCSCWireDigi",  wireDigis);
  
  
  // First loop over simhits and count how many simhits you have per chambers 
  
  // Array of type bool to see if save/throw away simhit:
  bool simhit_keep[200];
  
  if (simHits->size() > 200) {
    cout << "More simhits than allowed: " << simHits->size() << " > 200  --> skip event" << endl;
    return;
  }
  int idx_sim = 0;
  int j = 0;
  
  // Build iterator for simHits:
  PSimHitContainer::const_iterator simIt_1;
  
  // Search for matching hit in layer/chamber/...  in simhits:
  for (simIt_1 = simHits->begin(); simIt_1 != simHits->end(); simIt_1++) {
    
    // By default, keep this simhit
    simhit_keep[idx_sim] = true;
    
    // Initialize counter of hits in same layer
    int matched_simhit = 0;
    
    // Find chamber where simhit is located
    CSCDetId id_1 = (CSCDetId)(*simIt_1).detUnitId();
    
    // Not the most efficient way of doing it
    // but loop 2nd time to check for hits in same layer and save 
    // information into an array;
    
    // Build 2nd iterator for simHits:
    PSimHitContainer::const_iterator simIt_2;
    
    for (simIt_2 = simHits->begin(); simIt_2 != simHits->end(); simIt_2++) {
      
      // Find chamber where simhit 2 is located
      CSCDetId id_2 = (CSCDetId)(*simIt_2).detUnitId();
      
      // See if have matching pair of sim hits in same any chamber layer
      if ((idx_sim != j) &&
	  (id_1.endcap() == id_2.endcap()) &&
	  (id_1.ring() == id_2.ring()) &&
	  (id_1.station() == id_2.station()) &&
	  (id_1.chamber() == id_2.chamber()) &&
	  (id_1.layer() == id_2.layer())) {
	matched_simhit++;
      }
      j++;
    }
    // Are there more simhits in this layer than required ?  
    if (matched_simhit > maxSimhitLayer) simhit_keep[idx_sim] = false;
    idx_sim++;
  }
  
  // So, now we know which simhits to keep/throw away !
  
  // reset simhit index
  idx_sim=0;
  
  
  // Now, loop over simhits again, and search for matching rechits
  
  // Build iterator for simHits:
  PSimHitContainer::const_iterator simIt;
  
  // Search for matching hit in layer/chamber/...  in simhits:
  for (simIt = simHits->begin(); simIt != simHits->end(); simIt++) {
    
    
    // Check that simHit is from a muon and that we want to keep it:
    if (abs((*simIt).particleType()) == 13  && simhit_keep[idx_sim]) {
      
      // Find chamber where simhit is located
      CSCDetId id = (CSCDetId)(*simIt).detUnitId();

      
      // Store sim hit as a Local Point:
      LocalPoint shitlocal = (*simIt).localPosition();  
      float xsimu = shitlocal.x();
      float ysimu = shitlocal.y();
      

      // Find out the corresponding wiregroup / strip #
      const CSCLayer* csclayer = cscGeom->layer( id );
      const CSCLayerGeometry* geom = csclayer->geometry();
      int wire_shit = geom->nearestWire(shitlocal);
      int wiregrp_shit = geom->wireGroup(wire_shit);

      int strip_shit = geom->nearestStrip(shitlocal);
      if (id.station() == 1 && id.ring() == 4) {
      // ME-1-a strips are ganged:  48 strips --> 16 readouts
         strip_shit = strip_shit%16;
         if (strip_shit==0) strip_shit = 16;
      }

      
      // Initialize some parameters which we will use to find best rechit 
      // candidates and count how many (if any) matching rechits we have
      bool found_match = false;
      float delta_r = maxRechitDistance;
      int rechit_count = 0;
      
      // Store dummy x and y position for rechit:
      LocalPoint rhitlocal(9999.,9999.,9999.);
      float xreco = 9999.;
      float yreco = 9999.;
      int rwiregrp = 0; 
      int stripnum = 0; 
      
      // Loop over rechits 
      
      // Build iterator for rechits and loop :
      CSCRecHit2DCollection::const_iterator recIt;
      
      for (recIt = recHits->begin(); recIt != recHits->end(); recIt++) {
	
	// Find chamber with rechits in CSC 
	CSCDetId idrec = (CSCDetId)(*recIt).cscDetId();
	
	// Matching chamber (sim/reco) ?  
        // Also test if it's in the right EndCap: either (0), 1 or 2 ?
	if ((idrec.endcap() == id.endcap()) &&
	    (idrec.ring() == id.ring()) &&
	    (idrec.station() == id.station()) &&
	    (idrec.chamber() == id.chamber()) &&
	    (idrec.layer() == id.layer()) && 
	    ((idrec.endcap() == WhichEndCap) || WhichEndCap == 0)) { 
	  
	  
	  // Store reco hit as a Local Point:
	  LocalPoint rhitlocal_temp = (*recIt).localPosition();  
	  float xreco_temp = rhitlocal_temp.x();
	  float yreco_temp = rhitlocal_temp.y();
	  
	  float delta_r_temp = sqrt((xreco_temp - xsimu) * (xreco_temp - xsimu) 
				    + (yreco_temp - ysimu) * (yreco_temp - ysimu));
	  
	  rechit_count ++;
	  if (debug) cout << "found a rechit match for sim hit:" << rechit_count 
			  <<" at distance r = " << delta_r_temp << " cm" <<endl;
	  
	  
	  // Is it the optimum rechit/simhit pair ?
          // If so, store properties
	  if (delta_r_temp < delta_r) {
	    found_match = true;     
	    delta_r = delta_r_temp;
	    xreco = xreco_temp;
	    yreco = yreco_temp;
	    rhitlocal = rhitlocal_temp;

            // Find out the corresponding wiregroup 
            int wire_rhit = geom->nearestWire(rhitlocal);
            rwiregrp = geom->wireGroup(wire_rhit);

            // Find out the corresponding strip #
            stripnum = geom->nearestStrip(rhitlocal);
            if (id.station() == 1 && id.ring() == 4) {
	       // ME-1/a strips are ganged:  48 strips --> 16 readouts
               stripnum = stripnum%16;
   	       if (stripnum==0) stripnum = 16;
            }
	  }  
	}
      }
      
      // Check if exceeded the number of rechits per layer :      
      // 1) if we are dealing with ME_1_a
      if ( id.station() == 1 && id.ring() == 4 )   // ME-1/a
	{
	  // Then account for the ganged strips which gives you x 3 hits
	  if ( rechit_count > maxRechitLayerGanged ) found_match = false;  
	} else {
	  // 2) Other chambers don't have ganged strips:
	  if ( rechit_count > maxRechitLayer ) found_match = false;
	}
      
      
      // With best match, compute the various quantities needed:
      if (found_match) {
	
        // Look at wire digi and find best match for selected rechit (see if it exists):

        int wiregrp_digi = 999;

        CSCWireDigiCollection::DigiRangeIterator wdigiIt;
        for (wdigiIt = wireDigis->begin(); wdigiIt != wireDigis->end(); ++wdigiIt) {
	  
	  // Find chamber with rechits in CSC 
	  CSCDetId id_wdigi = (*wdigiIt).first;
	  
	  if ((id_wdigi.endcap() == id.endcap()) &&
	      (id_wdigi.ring() == id.ring()) &&
	      (id_wdigi.station() == id.station()) &&
	      (id_wdigi.chamber() == id.chamber()) &&
	      (id_wdigi.layer() == id.layer())) {	    

	    const CSCWireDigiCollection::Range rwired = wireDigis->get(id_wdigi );
	    for ( CSCWireDigiCollection::const_iterator it = rwired.first; it != rwired.second; ++it ) {
	      CSCWireDigi wdigi = *it;
	      int wiregrp_digi_temp = wdigi.getWireGroup();

              H2DRecHit *histo = 0;

               // Look at ME type first then determine inner/outer ring
              if (id.station() == 1) {
                 if (id.ring() == 1) histo = hRHPME1b;
                 if (id.ring() == 2) histo = hRHPME12;
                 if (id.ring() == 3) histo = hRHPME13;
                 if (id.ring() == 4) histo = hRHPME1a;
              }
              if (id.station() == 2) {
                 if (id.ring() == 1) histo = hRHPME21;
                 if (id.ring() == 2) histo = hRHPME22;
              }
              if (id.station() == 3) {
                 if (id.ring() == 1) histo = hRHPME31;
                 if (id.ring() == 2) histo = hRHPME32;
              }   
              if (id.station() == 4) histo = hRHPME4;

              histo->FillWDigi(wiregrp_digi_temp);
 
	      // Find best digi match according to selected rechit
	      if (abs(rwiregrp - wiregrp_digi) > abs(rwiregrp - wiregrp_digi_temp)) 
		wiregrp_digi = wiregrp_digi_temp;
	    }
	  }
        }
	
	if (debug) cout << "Best match:  w.g. digi = " 
			<< wiregrp_digi << "  w.g. reco = "
			<< rwiregrp << endl;

	
        // Look at strip digi:

	int stripnum_digi = 999;

        CSCStripDigiCollection::DigiRangeIterator sdigiIt;   
        for (sdigiIt = stripDigis->begin(); sdigiIt != stripDigis->end(); ++sdigiIt) {
	  
	  // Find chamber with rechits in CSC
	  CSCDetId id_sdigi = (*sdigiIt).first;
	  
	  if ((id_sdigi.endcap() == id.endcap()) &&   
	      (id_sdigi.ring() == id.ring()) &&
	      (id_sdigi.station() == id.station()) &&
	      (id_sdigi.chamber() == id.chamber()) &&
	      (id_sdigi.layer() == id.layer())) {
	    
	    const CSCStripDigiCollection::Range& rstrip = (*sdigiIt).second;
//	    const CSCStripDigiCollection::Range rstrip = stripDigis->get(id_sdigi );
	    for ( CSCStripDigiCollection::const_iterator it = rstrip.first; it != rstrip.second; ++it ) {
	      CSCStripDigi sdigi = *it;
	      int stripnum_digi_temp = sdigi.getStrip();

              H2DRecHit *histo = 0;

              // Look at ME type first then determine inner/outer ring
              if (id.station() == 1) {
                 if (id.ring() == 1) histo = hRHPME1b;
                 if (id.ring() == 2) histo = hRHPME12;
                 if (id.ring() == 3) histo = hRHPME13;
                 if (id.ring() == 4) histo = hRHPME1a;
              }
              if (id.station() == 2) {
                 if (id.ring() == 1) histo = hRHPME21;
                 if (id.ring() == 2) histo = hRHPME22;
              }
              if (id.station() == 3) {
                 if (id.ring() == 1) histo = hRHPME31;
                 if (id.ring() == 2) histo = hRHPME32;
              }
              if (id.station() == 4) histo = hRHPME4;

              histo->FillSDigi(stripnum_digi_temp);
	     

	      // Find best digi match according to selected rechit
	      if (abs(stripnum - stripnum_digi) > abs(stripnum - stripnum_digi_temp)) 
		stripnum_digi = stripnum_digi_temp;
	    }
	  }
        }
	if (debug) cout << "Match:  strip# digi = "
			<< stripnum_digi << "  strip# reco = "
			<< stripnum      << endl;	
	
	float x_resol = xreco - xsimu;
	float y_resol = yreco - ysimu;
	
	// Get pointer to layer:
        const CSCLayer* csclayer = cscGeom->layer( id );
        const CSCLayerGeometry* layergeom_ = csclayer->geometry();
        float apothem = layergeom_->length()/2.;	
	// Transform hit position from local chamber geometry to global CMS geom
	GlobalPoint rhitglobal= csclayer->toGlobal(rhitlocal);
	float grecphi = rhitglobal.phi();
	float greceta = rhitglobal.eta();
	//	float grecx = rhitglobal.x();
	//	float grecy = rhitglobal.y();
	float grecz = rhitglobal.z();
	
	GlobalPoint shitglobal= csclayer->toGlobal(shitlocal);
	float gsimphi = shitglobal.phi();
	float gsimeta = shitglobal.eta();
	float gsimx = shitglobal.x();
	float gsimy = shitglobal.y();
	float gsimz = shitglobal.z();
	
	// We're dealing with a Global point, so we need to compute the radial distance by hand:
	float gsimr = sqrt(gsimx*gsimx + gsimy*gsimy); 
	
	float deta = greceta -  gsimeta; 
	
	float PI = 3.1415927;
	float PIneg = -1 * PI;
	
	float dphi = grecphi - gsimphi;
	if (dphi > PI) dphi -= 2.0*PI;
	if (dphi < PIneg) dphi += 2.0*PI;
	float rdphi = gsimr * dphi;
	
	if (debug) {
	  cout << "matching chambers in sim/reco lists" << endl;
	  cout << "Endcap : " << id.endcap() << " " 
	       << "Ring   : " << id.ring()   << " "
	       << "Station: " << id.station()<< " "
	       << "Chamber: " << id.chamber()<< " "
	       << "Layer  : " << id.layer()  << " " << endl;
	  cout << "Distance between points : " << delta_r << " cm   " << endl;
	  cout << "X resolution (local)    : " << x_resol << " cm   " << endl;
	  cout << "Y resolution (local)    : " << y_resol << " cm   " << endl;
	  cout << "Delta phi (global)      : " << dphi    << " rads " << endl;
	  cout << "R cylindrical (global)  : " << gsimr   << " cm   " << endl;
	  cout << "R x Delta Phi (global)  : " << rdphi   << " cm   " << endl;
	  cout << "Delta eta (global)      : " << deta    << "      " << endl;
	  cout << "Delta z (global)        : " << grecz-gsimz    << " cm      " << endl;
	}
	
	  
	// Fill the histograms according to which segment we are using:
	  
	H2DRecHit *histo = 0;
	  
        // Look at ME type first then determine inner/outer ring
	  if (id.station() == 1) {
	    if (id.ring() == 1) {
	      histo = hRHPME1b;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta, rwiregrp, stripnum, wiregrp_digi, stripnum_digi, strip_shit, wiregrp_shit, gsimr, apothem);
	    }
	    if (id.ring() == 2) {
	      histo = hRHPME12;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta, rwiregrp, stripnum, wiregrp_digi, stripnum_digi, strip_shit, wiregrp_shit, gsimr, apothem);
	    }
	    if (id.ring() == 3) {
	      histo = hRHPME13;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta, rwiregrp,stripnum, wiregrp_digi, stripnum_digi, strip_shit, wiregrp_shit, gsimr, apothem);
	    }
	    if (id.ring() == 4) {
	      histo = hRHPME1a;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta, rwiregrp,stripnum, wiregrp_digi, stripnum_digi, strip_shit, wiregrp_shit, gsimr, apothem);
	    }
	  }
	  if (id.station() == 2) {
	    if (id.ring() == 1) {
	      histo = hRHPME21;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta, rwiregrp,stripnum, wiregrp_digi, stripnum_digi, strip_shit, wiregrp_shit, gsimr, apothem);
	    }
	    if (id.ring() == 2) {
	      histo = hRHPME22;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta, rwiregrp,stripnum, wiregrp_digi, stripnum_digi, strip_shit, wiregrp_shit, gsimr, apothem);
	    }
	  }
	  if (id.station() == 3) {
	    if (id.ring() == 1) {
	      histo = hRHPME31;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta, rwiregrp,stripnum, wiregrp_digi, stripnum_digi, strip_shit, wiregrp_shit, gsimr, apothem);
	    }
	    if (id.ring() == 2) {
	      histo = hRHPME32;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta, rwiregrp,stripnum, wiregrp_digi, stripnum_digi, strip_shit, wiregrp_shit, gsimr, apothem);
	    }
	  }
	  if (id.station() == 4) {
	    histo = hRHPME4;
	    histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta, rwiregrp,stripnum, wiregrp_digi, stripnum_digi, strip_shit, wiregrp_shit, gsimr, apothem);
	    if (id.ring() != 1) cout << " invalid ring in ME 4 !!! ";
	  }
       }
    }
    idx_sim++;
  }
}



DEFINE_FWK_MODULE(CSCRecHitReader)

