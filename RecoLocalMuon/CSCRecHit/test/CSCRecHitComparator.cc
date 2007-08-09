/*
 *  Basic analyzer class which accesses CSCRecHits
 *  and compare them with muon simhits and/or digis.  
 *
 *
 *  Author: D. Fortin - UC Riverside
 */

#include "CSCRecHitComparator.h"

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
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"
 
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
CSCRecHitComparator::CSCRecHitComparator(const edm::ParameterSet& pset){

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
CSCRecHitComparator::~CSCRecHitComparator(){
  
  if (debug) cout << "[CSCRecHitReader] Destructor called" << endl;

  cout << "Rechit reconstruction efficiency" << endl;        
  int ibin = 0;
  heff0    = new TH1F("h0", "rechit efficiency", segMap1.size()*2 + 2, 0, segMap1.size()*2 + 2); 
  for (map<string,int>::const_iterator it = segMap1.begin(); it != segMap1.end(); it++) {
    ibin++;
    float eff = (float)it->second/(float)chaMap1[it->first]; 
    heff0->SetBinContent(ibin*2, eff);
    heff0->GetXaxis()->SetBinLabel(ibin*2, (it->first).c_str());
    cout << it->first << ": " << it->second << " " << chaMap1[it->first] 
         << "  "      << eff  << endl;
  }

  
  // Write the histos to file
  theFile->cd();
  heff0->Write();
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
void CSCRecHitComparator::analyze(const Event & event, const EventSetup& eventSetup){
  
  // Get the CSC Geometry :
  ESHandle<CSCGeometry> cscGeom;
  eventSetup.get<MuonGeometryRecord>().get(cscGeom);
  
  // Get the SimHit collection :
  Handle<PSimHitContainer> simHits;
  event.getByLabel(simHitLabel, "MuonCSCHits", simHits);
  if (debug) cout << "   #SimHits: " << simHits->size() << endl;

  /*  
   * // Get the RecHits collection :
   * Handle<CSCRecHit2DCollection> recHits; 
   * event.getByLabel(recHitLabel, recHits);  
   * if (debug) cout << "   #RecHits: " << recHits->size() << endl;
   */


  std::vector<edm::Handle<CSCRecHit2DCollection> > VrecHits;
  event.getManyByType(VrecHits);

  const edm::Provenance *theProvenance = 0;
  unsigned int theRecProduct = 0;
  bool provFound = false;
  unsigned int i = 0;
  while (!provFound && (i < VrecHits.size())) {
    theProvenance = VrecHits.at(i).provenance();
    if (theProvenance->processName() == "testing") {  // CSCRecHitB
      theRecProduct = i;
      provFound = true;
    }
    i++;
  }

  if (!provFound) {
    std::cout << "******* rechit collection using process name testing not found" << std::endl;
  }

  CSCRecHit2DCollection recHits(*(VrecHits.at(theRecProduct)));


  theProvenance = 0;
  theRecProduct = 0;
  provFound = false;
  while (!provFound && (i < VrecHits.size())) {
    theProvenance = VrecHits.at(i).provenance();
    if (theProvenance->processName() == "testold") {  // CSCRecHit
      theRecProduct = i;
      provFound = true;
    }
    i++;
  }

  CSCRecHit2DCollection oldHits(*(VrecHits.at(theRecProduct)));
  
  
  // Loop over old rechits, and search for matching new rechits
  
  // Build iterator for rechits and loop :
  CSCRecHit2DCollection::const_iterator simIt;


  // Search for matching hit in layer/chamber/...  in simhits:
  for (simIt = oldHits.begin(); simIt != oldHits.end(); simIt++) {
    
    
    // Find chamber where old rechit is located
    CSCDetId id = (CSCDetId)(*simIt).cscDetId();

      
    // Store sim hit as a Local Point:
    LocalPoint shitlocal = (*simIt).localPosition();  

    float xsimu = shitlocal.x();
    float ysimu = shitlocal.y();
      
    // Find out the corresponding wiregroup / strip #
    const CSCLayer* csclayer = cscGeom->layer( id );
    const CSCLayerGeometry* geom = csclayer->geometry();
    const CSCChamber* chamber = cscGeom->chamber( id );
    int wire_shit = geom->nearestWire(shitlocal);
    int wiregrp_shit = geom->wireGroup(wire_shit);
    float stripWidth = geom->stripPitch(shitlocal);

    // Find wire hit position and wire properties
    int nWires    = geom->numberOfWiresPerGroup(wiregrp_shit); 
    double wSpace = geom->wirePitch();
    float wAngle  = geom->wireAngle();
    float dy      = wSpace * (nWires) / sqrt(12.);    

    // Find strip hit position and strip properties
    int strip_shit = geom->nearestStrip(shitlocal);
    float sAngle   = geom->stripAngle(strip_shit);
    double sPhiPitch = geom->stripPhiPitch();

    // Initialize some parameters which we will use to find best rechit 
    // candidates and count how many (if any) matching rechits we have
    bool found_match = false;
    float delta_r = maxRechitDistance;
    int rechit_count = 0;
      
    // Store dummy x and y position for rechit:
    LocalPoint rhitlocal(9999.,9999.,9999.);
    float xreco = 9999.;
    float yreco = 9999.;
    float sigma_xreco = 0.001;
    float sigma_yreco = 0.001;
    float sigma_xyreco = 0.0001;
    double chi2 = -1;
    int rwiregrp = 0; 
    int stripnum = 0; 
      

    chaMap1[chamber->specs()->chamberTypeName()]++; 


    // Loop over rechits 
      
    // Build iterator for rechits and loop :
    CSCRecHit2DCollection::const_iterator recIt;
      
    for (recIt = recHits.begin(); recIt != recHits.end(); recIt++) {
	
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
          sigma_xreco = (*recIt).localPositionError().xx();
          sigma_yreco = (*recIt).localPositionError().yy();
          sigma_xyreco = (*recIt).localPositionError().xy();
          chi2 =  (*recIt).chi2();

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
    if ( id.station() == 1 && id.ring() == 4 ) {
      // Then account for the ganged strips which gives you x 3 hits
      if ( rechit_count > maxRechitLayerGanged ) found_match = false;
    } else {
      // Other chambers don't have ganged strips:
      if ( rechit_count > maxRechitLayer ) found_match = false;
    }


    // With best match, compute the various quantities needed:
    if (found_match) {
      

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

      histo->FillHaveMatch(1);
  
      segMap1[chamber->specs()->chamberTypeName()]++;


      float x_resol = xreco - xsimu;
      float y_resol = yreco - ysimu;
              
      // Get pointer to layer:
      const CSCLayer* csclayer = cscGeom->layer( id );
      // Transform hit position from local chamber geometry to global CMS geom
      GlobalPoint rhitglobal= csclayer->toGlobal(rhitlocal);
      float grecphi = rhitglobal.phi();
      float grecx = rhitglobal.x();
      float grecy = rhitglobal.y();
      float grecz = rhitglobal.z();

      if (grecx == 0) grecx = 0.0001;
      if (grecy == 0) grecy = 0.0001;

      // Now disantangle local x and y errors to get error in strip frame
      double sinangdif  = sin(sAngle - wAngle);
      double sin2angdif = sinangdif * sinangdif;
      double wcoss      = dy * cos(sAngle);
      double scosw2     = sigma_xreco * sin2angdif - wcoss*wcoss;
      double dstrip     = sin(sAngle) * sqrt(scosw2) / cos(wAngle);

      sigma_xreco = sqrt(sigma_xreco);
      sigma_yreco = sqrt(sigma_yreco);

      GlobalPoint shitglobal= csclayer->toGlobal(shitlocal);
      float gsimphi = shitglobal.phi();
      float gsimx = shitglobal.x();
      float gsimy = shitglobal.y();
      float gsimz = shitglobal.z();
	
      // We're dealing with a Global point, so we need to compute the radial distance by hand:
      float gsimr = sqrt(gsimx*gsimx + gsimy*gsimy); 
	
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
        cout << "Delta X (local)    : " << x_resol << " cm   " << endl;
        cout << "Delta Y (local)    : " << y_resol << " cm   " << endl;
        cout << "Delta phi (global)      : " << dphi    << " rads " << endl;
        cout << "R x Delta Phi (global)  : " << rdphi   << " cm   " << endl;
        cout << "Delta z (global)        : " << grecz-gsimz    << " cm      " << endl;
      }
	
      // Fill the histograms according to which segment we are using:
	  
      // Look at ME type first then determine inner/outer ring
      if (id.station() == 1) {
        if (id.ring() == 1) {
          histo = hRHPME1b;
        }
        if (id.ring() == 2) {
          histo = hRHPME12;
	}
        if (id.ring() == 3) {
          histo = hRHPME13;
	}
	if (id.ring() == 4) {
	  histo = hRHPME1a;
	}
      }
      if (id.station() == 2) {
        if (id.ring() == 1) {
          histo = hRHPME21;
        }
        if (id.ring() == 2) {
          histo = hRHPME22;
	}
      }
      if (id.station() == 3) {
        if (id.ring() == 1) {
          histo = hRHPME31;
	}
	if (id.ring() == 2) {
	  histo = hRHPME32;
	}
      }
      if (id.station() == 4) {
        histo = hRHPME4;
	if (id.ring() != 1) cout << " invalid ring in ME 4 !!! ";
      }
      histo->Fill(xreco, yreco, xsimu, ysimu, grecphi, gsimphi, gsimr, sigma_xreco, sigma_yreco, stripWidth, dstrip, sPhiPitch, chi2);

    } else {
          
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
          
      histo->FillHaveMatch(0);
      histo->FillNoMatch(xsimu, ysimu );
    }
  }
}
       



DEFINE_FWK_MODULE(CSCRecHitComparator);

