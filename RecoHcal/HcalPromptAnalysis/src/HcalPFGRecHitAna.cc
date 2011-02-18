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

  HEposEne     = fs->make<TH1F>( "HEposEne" , "HEposEne" , 12000, -20, 100 );
  HEposTime    = fs->make<TH1F>( "HEposTime", "HEposTime", 200, -200, 200 );
  HEnegEne     = fs->make<TH1F>( "HEnegEne" , "HEnegEne" , 12000, -20, 100 );
  HEnegTime    = fs->make<TH1F>( "HEnegTime", "HEnegTime", 200, -200, 200 );

  HBposEne     = fs->make<TH1F>( "HBposEne" , "HBposEne" , 12000, -20, 100 );
  HBposTime    = fs->make<TH1F>( "HBposTime", "HBposTime", 200, -200, 200 );
  HBnegEne     = fs->make<TH1F>( "HBnegEne" , "HBnegEne" , 12000, -20, 100 );
  HBnegTime    = fs->make<TH1F>( "HBnegTime", "HBnegTime", 200, -200, 200 );

  HFLongEne    = fs->make<TH1F>( "HFLongEne"  , "HFLongEne"  , 12000, -20, 100 );
  HFLongTime   = fs->make<TH1F>( "HFLongTime" , "HFLongTime" , 200, -200, 200 );
  HFShortEne   = fs->make<TH1F>( "HFShortEne" , "HFShortEne" , 12000, -20, 100 );
  HFShortTime  = fs->make<TH1F>( "HFShortTime", "HFShortTime", 200, -200, 200 );

  HOrm0Ene     = fs->make<TH1F>( "HOrm0Ene" , "HOrm0Ene" , 12000, -20, 100 );
  HOrm0Time    = fs->make<TH1F>( "HOrm0Time", "HOrm0Time", 200, -200, 200 );
  HOrp0Ene     = fs->make<TH1F>( "HOrp0Ene" , "HOrp0Ene" , 12000, -20, 100 );
  HOrp0Time    = fs->make<TH1F>( "HOrp0Time", "HOrp0Time", 200, -200, 200 );

  HOrm1Ene     = fs->make<TH1F>( "HOrm1Ene" , "HOrm1Ene" , 12000, -20, 100 );
  HOrm1Time    = fs->make<TH1F>( "HOrm1Time", "HOrm1Time", 200, -200, 200 );
  HOrp1Ene     = fs->make<TH1F>( "HOrp1Ene" , "HOrp1Ene" , 12000, -20, 100 );
  HOrp1Time    = fs->make<TH1F>( "HOrp1Time", "HOrp1Time", 200, -200, 200 );

  HOsipmEne    = fs->make<TH1F>( "HOsipmEne" , "HOsipmEne" , 12000, -20, 100 );
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
	    HEnegEne->Fill(j->energy()); 
	    HEnegTime->Fill(j->time()); 
	  } else {
	    HEposEne->Fill(j->energy()); 
	    HEposTime->Fill(j->time()); 
	  }
        }
	else
	{
	  
	  // Check whether rechits are being made for a channel that we 
	  // know should be masked.
	  if(j->id().ieta()==14 && j->id().iphi()==31 && j->id().depth()==1 )
	  {
	    std::cout << "PROBLEM :: RecHit Made for 14, 31, 1" << std::endl;
	  }
	  

	  if (j->id().ieta()<0) {
	    HBnegEne->Fill(j->energy()); 
	    HBnegTime->Fill(j->time()); 
	  } else {
	    HBposEne->Fill(j->energy()); 
	    HBposTime->Fill(j->time()); 
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
}

// ***********************************
// ***********************************
void HcalPFGRecHitAna::endJob() {

}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HcalPFGRecHitAna);
