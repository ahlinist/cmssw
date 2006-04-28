/*
 *  Basic analyzer class which accesses CSCRecHits
 *  and compare them with muon simhits
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
  // Get the debug parameter for verbose output
  debug = pset.getUntrackedParameter<bool>("debug");
  rootFileName = pset.getUntrackedParameter<string>("rootFileName");
  simHitLabel = pset.getUntrackedParameter<string>("simHitLabel");
  recHitLabel = pset.getUntrackedParameter<string>("recHitLabel");
  minRechitChamber = pset.getUntrackedParameter<int>("minRechitPerChamber");
  maxRechitChamber = pset.getUntrackedParameter<int>("maxRechitPerChamber");
  
  if(debug) cout << "[CSCRecHitReader] Constructor called" << endl;
  
  // Create the root file
  theFile = new TFile(rootFileName.c_str(), "RECREATE");
  theFile->cd();
  
  // Book the histograms
  hRHPAll  = new H1DRecHit("ME_All");
  hRHPME1a = new H1DRecHit("ME_1_a");  // Innermost ME 1
  hRHPME11 = new H1DRecHit("ME_1_1");
  hRHPME12 = new H1DRecHit("ME_1_2");
  hRHPME13 = new H1DRecHit("ME_1_3");
  hRHPME21 = new H1DRecHit("ME_2_1");
  hRHPME22 = new H1DRecHit("ME_2_2");
  hRHPME31 = new H1DRecHit("ME_3_1");
  hRHPME32 = new H1DRecHit("ME_3_2");
  hRHPME4  = new H1DRecHit("ME_4");
}

// Destructor
CSCRecHitReader::~CSCRecHitReader(){
  
  if (debug) cout << "[CSCRecHitReader] Destructor called" << endl;
  
  // Write the histos to file
  theFile->cd();
  hRHPAll ->Write();
  hRHPME1a->Write();
  hRHPME11->Write();
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
  
  if (event.id().event()%100 == 0) {
    cout << " Event analysed #Run: " << event.id().run()
         << " #Event: " << event.id().event() << endl;
  }
  
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
  
  // Loop over rechits 
  
  // Build iterator for rechits and loop :
  CSCRecHit2DCollection::const_iterator recIt;
  for (recIt = recHits->begin(); recIt != recHits->end(); recIt++) {
    
    // Find chamber with rechits in CSC 
    CSCDetId idrec = (CSCDetId)(*recIt).cscDetId();
    
    // Store reco hit as a Local Point:
    LocalPoint rhitlocal = (*recIt).localPosition();  
    float xreco = rhitlocal.x();
    float yreco = rhitlocal.y();
    
    
    // Store dummy x and y position for sim hit:
    LocalPoint shitlocal(9999.,9999.,9999.);
    float xsimu = 9999.;
    float ysimu = 9999.;
    
    bool found_match = false;
    float delta_r = 9999.;
    
    // Build iterator for simHits:
    PSimHitContainer::const_iterator simIt;
    
    // Search for matching hit in layer/chamber/...  in simhits:
    for (simIt = simHits->begin(); simIt != simHits->end(); simIt++) {
      
      // Check that simHit is from a muon :
      if(abs((*simIt).particleType()) == 13) {
	
	// Find chamber where simhit is located
	CSCDetId id = (CSCDetId)(*simIt).detUnitId();
	
	// Matching chamber (sim/reco) ?
	if ((idrec.endcap() == id.endcap()) &&
	    (idrec.ring() == id.ring()) &&
	    (idrec.station() == id.station()) &&
	    (idrec.chamber() == id.chamber()) &&
	    (idrec.layer() == id.layer())) {
	  
	  found_match = true;  // found at least one match in chambers...
	  
	  
	  LocalPoint shitlocal_temp = (*simIt).localPosition();
	  
	  float xsimu_temp = (*simIt).localPosition().x();
	  float ysimu_temp = (*simIt).localPosition().y();
	  
	  float delta_r_temp = sqrt((xreco-xsimu_temp)*(xreco-xsimu_temp) 
				    + (yreco-ysimu_temp)*(yreco-ysimu_temp));
	  
	  // Is it the optimum simhit ?
	  if (delta_r_temp < delta_r) {
	    delta_r = delta_r_temp;
	    xsimu = xsimu_temp;
	    ysimu = ysimu_temp;
	    shitlocal = shitlocal_temp; // Store local Point for sim hit
	  }      
	}	
      }       
    }
    
    // Fill in best match:
    if (found_match) {
      float x_resol = xsimu - xreco;
      float y_resol = ysimu - yreco;
      
      // Get pointer to layer:
      const CSCLayer* csclayer = cscGeom->layer( idrec );      
      
      // Transform hit position from local chamber geometry to global CMS geom
      GlobalPoint rhitglobal= csclayer->toGlobal(rhitlocal);
      float grecphi = rhitglobal.phi();
      float grecx = rhitglobal.x();
      float grecy = rhitglobal.y();
      float grecr = sqrt(grecx*grecx + grecy*grecy); // No .r() in GlobalPoint?
      
      GlobalPoint shitglobal= csclayer->toGlobal(shitlocal);
      float gsimphi = shitglobal.phi();
      //    float gsimx = shitglobal.x();
      //    float gsimy = shitglobal.y();
      
      
      float PI = 3.1415927;
      float PIneg = -1 * PI;

      float dphi = grecphi - gsimphi;
      if (dphi > PI) dphi -= PI;
      if (dphi < PIneg) dphi += PI;
      float rdphi = grecr * dphi;
      
      if (debug) {
	cout << "matching chambers in sim/reco lists" << endl;
	cout << "Endcap : " << idrec.endcap() << " " 
	     << "Ring   : " << idrec.ring()   << " "
	     << "Station: " << idrec.station()<< " "
	     << "Chamber: " << idrec.chamber()<< " "
	     << "Layer  : " << idrec.layer()  << " " << endl;
	cout << "Distance between points : " << delta_r << " cm   " << endl;
	cout << "X resolution (local)    : " << x_resol << " cm   " << endl;
	cout << "Y resolution (local)    : " << y_resol << " cm   " << endl;
	cout << "Delta phi (global)      : " << dphi    << " rads " << endl;
	cout << "R cylindrical (global)  : " << grecr   << " cm   " << endl;
	cout << "R x Delta Phi (global)  : " << rdphi   << " cm   " << endl;

      }


      // Fill the histos
      H1DRecHit *histo = 0;
      histo = hRHPAll;
      histo->Fill(xreco, yreco, xsimu, ysimu, rdphi);
      

      
      
      if (!debug) {
      // if (idrec.endcap() == 1) // --> won't worry about +/- Z for now. 
      
      // Look at ME type first then determine inner/outer ring
      if (idrec.station() == 1) {
	if (idrec.ring() == 1) {
	  histo = hRHPME12;
	  histo->Fill(xreco, yreco, xsimu, ysimu, rdphi);
	}
	if (idrec.ring() == 2) {
	  histo = hRHPME13;
	  histo->Fill(xreco, yreco, xsimu, ysimu, rdphi);
	}
	if (idrec.ring() == 3) {
	  histo = hRHPME11;
	  histo->Fill(xreco, yreco, xsimu, ysimu, rdphi);
	}
	if (idrec.ring() == 4) {
	  histo = hRHPME1a;
	  histo->Fill(xreco, yreco, xsimu, ysimu, rdphi);
	}
      }
      if (idrec.station() == 2) {
	if (idrec.ring() == 1) {
	  histo = hRHPME21;
	  histo->Fill(xreco, yreco, xsimu, ysimu, rdphi);
	}
	if (idrec.ring() == 2) {
	  histo = hRHPME22;
	  histo->Fill(xreco, yreco, xsimu, ysimu, rdphi);
	}
      }
      if (idrec.station() == 3) {
	if (idrec.ring() == 1) {
	  histo = hRHPME31;
	  histo->Fill(xreco, yreco, xsimu, ysimu, rdphi);
	}
	if (idrec.ring() == 2) {
	  histo = hRHPME32;
	  histo->Fill(xreco, yreco, xsimu, ysimu, rdphi);
	}
      }
      if (idrec.station() == 4) {
	histo = hRHPME4;
	histo->Fill(xreco, yreco, xsimu, ysimu, rdphi);
	if (idrec.ring() == 2) cout << " invalid chamber !!! ";
      }
            }
      if (debug) cout << "Done filling the histograms !!! " << endl;
    }
  }
}



DEFINE_FWK_MODULE(CSCRecHitReader)

