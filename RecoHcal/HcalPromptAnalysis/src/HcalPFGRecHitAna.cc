// HcalPFGRecHitAna
// Description:  Plot RecHit Energy and Time from Reco 
// Author: Jim Hirschauer
// Date:  Feb 2011
// Compiled and run in CMSSW_3_11_0

#include "RecoHcal/HcalPromptAnalysis/interface/HcalPFGRecHitAna.h"


#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalSourcePositionData.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "FWCore/Framework/interface/Event.h"
// #include "FWCore/Framework/interface/EventSetup.h"
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TCanvas.h>
#include <cmath>

#include <sys/time.h>
#include <time.h>


using namespace edm;
using namespace reco;
using namespace std;

// Get the algorithm of the jet collections we will read from the .cfg file 
// which defines the value of the strings CaloJetAlgorithm and GenJetAlgorithm.

HcalPFGRecHitAna::HcalPFGRecHitAna( const ParameterSet & cfg ){
}


void HcalPFGRecHitAna::beginJob( ) {
  
  edm::Service<TFileService> fs;

  HEPEne     = fs->make<TH1F>( "HEPEne" , "HEPEne" , 12000, -20 , 500 );
  HEPTime    = fs->make<TH1F>( "HEPTime", "HEPTime", 200  , -200, 200 );
  HEMEne     = fs->make<TH1F>( "HEMEne" , "HEMEne" , 12000, -20 , 500 );
  HEMTime    = fs->make<TH1F>( "HEMTime", "HEMTime", 200  , -200, 200 );

  HEM15BEne     = fs->make<TH1F>( "HEM15BEne"   , "HEM15BEne"    , 12000, -20 , 500 );
  HEM15BTime    = fs->make<TH1F>( "HEM15BTime"  , "HEM15BTime"   , 200  , -200, 200 );
  HEM15AEne     = fs->make<TH1F>( "HEM15AEne"   , "HEM15AEne"    , 12000, -20 , 500 );
  HEM15ATime    = fs->make<TH1F>( "HEM15ATime"  , "HEM15ATime"   , 200  , -200, 200 );
  HEMOtherEne   = fs->make<TH1F>( "HEMOtherEne" , "HEMOtherEne"  , 12000, -20 , 500 );
  HEMOtherTime  = fs->make<TH1F>( "HEMOtherTime", "HEMOtherTime" , 200  , -200, 200 );

  HBP14BEne     = fs->make<TH1F>( "HBP14BEne"   , "HBP14BEne"    , 12000, -20 , 500 );
  HBP14BTime    = fs->make<TH1F>( "HBP14BTime"  , "HBP14BTime"   , 200  , -200, 200 );
  HBP14AEne     = fs->make<TH1F>( "HBP14AEne"   , "HBP14AEne"    , 12000, -20 , 500 );
  HBP14ATime    = fs->make<TH1F>( "HBP14ATime"  , "HBP14ATime"   , 200  , -200, 200 );
  HBPOtherEne   = fs->make<TH1F>( "HBPOtherEne" , "HBPOtherEne"  , 12000, -20 , 500 );
  HBPOtherTime  = fs->make<TH1F>( "HBPOtherTime", "HBPOtherTime" , 200  , -200, 200 );

  HEM15BEneVTime     = fs->make<TH2F>( "HEM15BEneVTime"   , "HEM15BEneVTime"   , 12000, -20 , 500, 200, -200, 200 );
  HEM15AEneVTime     = fs->make<TH2F>( "HEM15AEneVTime"   , "HEM15AEneVTime"   , 12000, -20 , 500, 200, -200, 200 );
  HEMOtherEneVTime   = fs->make<TH2F>( "HEMOtherEneVTime" , "HEMOtherEneVTime" , 12000, -20 , 500, 200, -200, 200 );
  HBP14BEneVTime     = fs->make<TH2F>( "HBP14BEneVTime"   , "HBP14BEneVTime"   , 12000, -20 , 500, 200, -200, 200 );
  HBP14AEneVTime     = fs->make<TH2F>( "HBP14AEneVTime"   , "HBP14AEneVTime"   , 12000, -20 , 500, 200, -200, 200 );
  HBPOtherEneVTime   = fs->make<TH2F>( "HBPOtherEneVTime" , "HBPOtherEneVTime" , 12000, -20 , 500, 200, -200, 200 );

  HBPEne     = fs->make<TH1F>( "HBPEne" , "HBPEne" , 12000, -20, 500 );
  HBPTime    = fs->make<TH1F>( "HBPTime", "HBPTime", 200, -200, 200 );
  HBMEne     = fs->make<TH1F>( "HBMEne" , "HBMEne" , 12000, -20, 500 );
  HBMTime    = fs->make<TH1F>( "HBMTime", "HBMTime", 200, -200, 200 );

  HFLongEne    = fs->make<TH1F>( "HFLongEne"  , "HFLongEne"  , 12000, -20, 500 );
  HFLongTime   = fs->make<TH1F>( "HFLongTime" , "HFLongTime" , 200, -200, 200 );
  HFShortEne   = fs->make<TH1F>( "HFShortEne" , "HFShortEne" , 12000, -20, 500 );
  HFShortTime  = fs->make<TH1F>( "HFShortTime", "HFShortTime", 200, -200, 200 );

  HOrm0Ene     = fs->make<TH1F>( "HOrm0Ene" , "HOrm0Ene" , 12000, -20, 500 );
  HOrm0Time    = fs->make<TH1F>( "HOrm0Time", "HOrm0Time", 200, -200, 200 );
  HOrp0Ene     = fs->make<TH1F>( "HOrp0Ene" , "HOrp0Ene" , 12000, -20, 500 );
  HOrp0Time    = fs->make<TH1F>( "HOrp0Time", "HOrp0Time", 200, -200, 200 );

  HOrm1Ene     = fs->make<TH1F>( "HOrm1Ene" , "HOrm1Ene" , 12000, -20, 500 );
  HOrm1Time    = fs->make<TH1F>( "HOrm1Time", "HOrm1Time", 200, -200, 200 );
  HOrp1Ene     = fs->make<TH1F>( "HOrp1Ene" , "HOrp1Ene" , 12000, -20, 500 );
  HOrp1Time    = fs->make<TH1F>( "HOrp1Time", "HOrp1Time", 200, -200, 200 );

  HOsipmEne    = fs->make<TH1F>( "HOsipmEne" , "HOsipmEne" , 12000, -20, 500 );
  HOsipmTime   = fs->make<TH1F>( "HOsipmTime", "HOsipmTime", 200, -200, 200 );
}

// ************************
// ************************
void HcalPFGRecHitAna::analyze( const edm::Event& evt, const edm::EventSetup& es ) {
 
  using namespace edm;

  try {
    std::vector<edm::Handle<HBHERecHitCollection> > colls;
    evt.getManyByType(colls);
    std::vector<edm::Handle<HBHERecHitCollection> >::iterator i;
    for (i=colls.begin(); i!=colls.end(); i++) {
      for (HBHERecHitCollection::const_iterator j=(*i)->begin(); j!=(*i)->end(); j++) {
        if (j->id().subdet() == HcalEndcap) {
	  if (j->id().ieta()<0) {
	    HEMEne ->Fill(j->energy()); 
	    HEMTime->Fill(j->time()); 

	    // Make plots for HEM15 for RecHits w/ E>10GeV:
	    bool inHEM15B(0);
	    bool inHEM15A(0);

	    if ( j->id().iphi()==55 || j->id().iphi()==56) inHEM15B = 1;
	    else if ( j->id().iphi()==57 || j->id().iphi()==58) inHEM15A = 1;

	    if (inHEM15B)
	    {
	      HEM15BEne     ->Fill(j->energy());
	      HEM15BEneVTime->Fill(j->energy() , j->time());
	      if (j->energy()>10.){
		HEM15BTime    ->Fill(j->time());
	      }
	    }
	    else if (inHEM15A) 
	    {
	      HEM15AEne ->Fill(j->energy());
	      HEM15AEneVTime->Fill(j->energy() , j->time());
	      if (j->energy()>10.){
		HEM15ATime->Fill(j->time());
	      }
	    }
	    else
	    {
	      HEMOtherEne ->Fill(j->energy());
	      HEMOtherEneVTime->Fill(j->energy() , j->time());
	      if (j->energy()>10.){
		HEMOtherTime->Fill(j->time()); 
	      } 
	    }
	  } else {
	    HEPEne->Fill(j->energy()); 
	    HEPTime->Fill(j->time()); 
	  }
        }
	else
	{
	  
	  // Check whether rechits are being made for a channel that we 
	  // know should be masked.
	  // 	  if(j->id().ieta()==14 && j->id().iphi()==31 && j->id().depth()==1 )
	  // 	  {
	  // 	    std::cout << "PROBLEM :: RecHit Made for 14, 31, 1" << std::endl;
	  // 	  }
	  

	  if (j->id().ieta()<0) {
	    HBMEne->Fill(j->energy()); 
	    HBMTime->Fill(j->time()); 
	  } else {
	    HBPEne->Fill(j->energy()); 
	    HBPTime->Fill(j->time()); 

	    bool inHBP14B(0);
	    bool inHBP14A(0);
	    
	    if ( j->id().iphi()==51 || j->id().iphi()==52) inHBP14B = 1;
	    else if ( j->id().iphi()==53 || j->id().iphi()==54) inHBP14A = 1;

	    if (inHBP14B)
	    {
	      HBP14BEne ->Fill(j->energy());
	      HBP14BEneVTime->Fill(j->energy() , j->time());
	      if (j->energy()>10.){
		HBP14BTime->Fill(j->time());
	      }
	    }
	    else if (inHBP14A) 
	    {
	      HBP14AEne ->Fill(j->energy());
	      HBP14AEneVTime->Fill(j->energy() , j->time());
	      if (j->energy()>10.){
		HBP14ATime->Fill(j->time()); 
	      }
	    }
	    else
	    {
	      HBPOtherEne ->Fill(j->energy());
	      HBPOtherEneVTime->Fill(j->energy() , j->time());
	      if (j->energy()>10.){
		HBPOtherTime->Fill(j->time()); 
	      }
	    }
	  }
	}
      }
    }
  } catch (...) {
    cout << "No HB/HE RecHits." << endl;
  }


  try {
    std::vector<edm::Handle<HFRecHitCollection> > colls;
    evt.getManyByType(colls);
    std::vector<edm::Handle<HFRecHitCollection> >::iterator i;
    for (i=colls.begin(); i!=colls.end(); i++) {
      for (HFRecHitCollection::const_iterator j=(*i)->begin(); j!=(*i)->end(); j++) {
        if (j->id().subdet() == HcalForward) {
	  // Long and short fibers
	  if (j->id().depth() == 1){
	    HFLongEne->Fill(j->energy()); 
	    HFLongTime->Fill(j->time());
	  } else {
	    HFShortEne->Fill(j->energy()); 
	    HFShortTime->Fill(j->time());
	  }
        }
      }
    }
  } catch (...) {
    cout << "No HF RecHits." << endl;
  }


  try {
    std::vector<edm::Handle<HORecHitCollection> > colls;
    evt.getManyByType(colls);
    std::vector<edm::Handle<HORecHitCollection> >::iterator i;
    for (i=colls.begin(); i!=colls.end(); i++) {
      for (HORecHitCollection::const_iterator j=(*i)->begin(); j!=(*i)->end(); j++) {
        if (j->id().subdet() == HcalOuter) {
	  // Separate SiPMs and HPDs:
	  if (((j->id().iphi()>=59 && j->id().iphi()<=70 && 
		j->id().ieta()>=11 && j->id().ieta()<=15) || 
	       (j->id().iphi()>=47 && j->id().iphi()<=58 && 
		j->id().ieta()>=5 && j->id().ieta()<=10)))
	  {  
	    HOsipmEne->Fill(j->energy());
	    HOsipmTime->Fill(j->time());
	  } 
	  else 
	  {
	    // Separate rings -1,-2,0,1,2 in HPDs:
	    if (j->id().ieta()>= -10 && j->id().ieta() <= -5) {
	      HOrm1Ene->Fill(j->energy());
	      HOrm1Time->Fill(j->time());
	    } else if (j->id().ieta()>= -4 && j->id().ieta() <= 0) {
	      HOrm0Ene->Fill(j->energy());
	      HOrm0Time->Fill(j->time());
	    } else if (j->id().ieta()>= 0 && j->id().ieta() <= 4) {
	      HOrp0Ene->Fill(j->energy());
	      HOrp0Time->Fill(j->time());
	    } else if (j->id().ieta()>= 5 && j->id().ieta() <= 10) {
	      HOrp1Ene->Fill(j->energy());
	      HOrp1Time->Fill(j->time());
	    } else {
	      std::cout << "Finding events that are in no ring !?!" << std::endl;
	      std::cout << "eta = " << j->id().ieta() << std::endl;
	    }
	  }
        }
        //      std::cout << *j << std::endl;
      }
    }
  } catch (...) {
    cout << "No HO RecHits." << endl;
  }

  //   try {
  //     std::vector<edm::Handle<CaloJetCollection> > colls;
  //     evt.getByLabel("ak5CaloJets",colls);
  //     std::vector<edm::Handle<CaloJetCollection> >::iterator i;
  //     for (i=colls.begin(); i!=colls.end(); i++) {
  //       for (CaloJetCollection::const_iterator j=(*i)->begin(); j!=(*i)->end(); j++) {
  
  //       }
  //     }
  //   } catch (...) {
  //     cout << "No CaloJets." << endl;
  //  }
  
}

// ***********************************
// ***********************************
void HcalPFGRecHitAna::endJob() {

}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HcalPFGRecHitAna);
