/*
 *  Basic analyzer class which accesses CSCRecHits
 *  and compare them with muon simhits.  
 *
 *  The output histograms are dealt with in CSCRecHitHistograms.h
 *
 *  Author: D. Fortin - UC Riverside
 */

#include "CSCRecHitReader.h"

#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
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
  recHitLabel      = pset.getUntrackedParameter<string>("recHitLabel");
  maxRechitDistance= pset.getUntrackedParameter<double>("maxRechitDistance");
  maxRechitLayer   = pset.getUntrackedParameter<int>("maxRechitPerLayer");
  maxSimhitLayer   = pset.getUntrackedParameter<int>("maxSimhitPerLayer");
  WhichEndCap      = pset.getUntrackedParameter<int>("WhichEndCap");

  // ME_1_a/b are ganged in 3 --> 3 reco hits /hit  so account for it !
  maxRechitLayerGanged = 3 * maxRechitLayer;  

  
  if(debug) cout << "[CSCRecHitReader] Constructor called" << endl;
  
  // Create the root file
  theFile = new TFile(rootFileName.c_str(), "RECREATE");
  theFile->cd();
  
  // Book the histograms
  hRHPAll  = new H2DRecHit("ME_All");
  hRHPME1a = new H2DRecHit("ME_1_a");  // Innermost ME 1_1
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
  hRHPAll ->Write();
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
    int matched_simhit = 1;

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
    // Are there more simhits in this layer than required ?  If so reject these hits.
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
      
      
      // Initialize some parameters which we will use to find best rechit candidates and
      // count how many (if any) matching rechits we have
      bool found_match = false;
      float delta_r = maxRechitDistance;
      int rechit_count = 0;
      
      // Store dummy x and y position for rec hit:
      LocalPoint rhitlocal(9999.,9999.,9999.);
      float xreco = 9999.;
      float yreco = 9999.;
 

      // Loop over rechits 
      
      // Build iterator for rechits and loop :
      CSCRecHit2DCollection::const_iterator recIt;
      for (recIt = recHits->begin(); recIt != recHits->end(); recIt++) {
	
	// Find chamber with rechits in CSC 
	CSCDetId idrec = (CSCDetId)(*recIt).cscDetId();
	
	
	// Matching chamber (sim/reco) ?  Also test if it's in the right EndCap: either (0), 1 or 2 ?
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
	  if (delta_r_temp < delta_r) {
	    delta_r = delta_r_temp;
	    xreco = xreco_temp;
	    yreco = yreco_temp;
	    rhitlocal = rhitlocal_temp; // Store local Point for this rec hit
	    found_match = true;         // found at least one match in chambers...
	  }  
	}
      }
    
      // Check if exceeded the number of rechits per layer :

      // 1) if we are dealing with  ME_1_a or b
      //      if ((id.station() == 1 && id.ring() == 1) || // ME_1_b
      //	  (id.station() == 1 && id.ring() == 4))   // ME_1_a

      // Tim: Only ME1a has ganged strips...

      // 1) if we are dealing with ME_1_a
      if ( id.station() == 1 && id.ring() == 4 )   // ME_1_a
      {
        // Then account for the ganged strips which gives you x 3 hits
	if ( rechit_count > maxRechitLayerGanged ) found_match = false;

      } else {
      // 2) Other chambers don't have ganged strips:
	  if ( rechit_count > maxRechitLayer ) found_match = false;
      }
    
      
      // With best match, compute the various quantities needed:
      if (found_match) {

	float x_resol = xreco - xsimu;
	float y_resol = yreco - ysimu;
	
	// Get pointer to layer:
	const CSCLayer* csclayer = cscGeom->layer( id );      
	
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
	if (dphi > PI) dphi -= PI;
	if (dphi < PIneg) dphi += PI;
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
	
	if (!debug) {
	  
	  // Fill the histograms according to which segment we are using:
	  
	  H2DRecHit *histo = 0;
	  histo = hRHPAll;
	  histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta);
	  
	  
	// Look at ME type first then determine inner/outer ring
	  if (id.station() == 1) {
	    if (id.ring() == 1) {
	      histo = hRHPME1b;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta);
	    }
	    if (id.ring() == 2) {
	      histo = hRHPME12;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta);
	    }
	    if (id.ring() == 3) {
	      histo = hRHPME13;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta);
	    }
	    if (id.ring() == 4) {
	      histo = hRHPME1a;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta);
	    }
	  }
	  if (id.station() == 2) {
	    if (id.ring() == 1) {
	      histo = hRHPME21;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta);
	    }
	    if (id.ring() == 2) {
	      histo = hRHPME22;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta);
	    }
	  }
	  if (id.station() == 3) {
	    if (id.ring() == 1) {
	      histo = hRHPME31;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta);
	    }
	    if (id.ring() == 2) {
	      histo = hRHPME32;
	      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta);
	    }
	  }
	  if (id.station() == 4) {
	    histo = hRHPME4;
	    histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, rdphi, greceta, gsimeta, deta);
	    if (id.ring() != 1) cout << " invalid ring in ME 4 !!! ";
	  }
	}
	if (debug) cout << "When debugging, you do not fill histograms !!! " << endl;
      }
    }
    idx_sim++;
  }
}



DEFINE_FWK_MODULE(CSCRecHitReader)

