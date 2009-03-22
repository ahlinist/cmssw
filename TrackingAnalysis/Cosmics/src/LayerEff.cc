// -*- C++ -*-
//
// Package:    LayerEff
// Class:      LayerEff
// 
/**\class LayerEff LayerEff.cc TrackingAnalysis/Cosmics/src/LayerEff.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  "Keith Ulmer"
//         Created:  Fri Mar 20 03:16:50 CDT 2009
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CalibTracker/Records/interface/SiStripQualityRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripQuality.h"

#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h" 

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"

#include "TFile.h"
#include "TTree.h"
//
// class decleration
//

class LayerEff : public edm::EDAnalyzer {
   public:
      explicit LayerEff(const edm::ParameterSet&);
      ~LayerEff();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

  // ----------member data ---------------------------
  edm::ParameterSet config_;
  edm::InputTag trackTag_;
  TTree* rootTree_;
  uint layer_;
  uint valid_;
  uint module_;
  uint qualBad_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
LayerEff::LayerEff(const edm::ParameterSet& iConfig) :
  config_(iConfig), rootTree_(0) 
{
   //now do what ever initialization is needed
    trackTag_ = iConfig.getParameter<edm::InputTag>("tracks");
}


LayerEff::~LayerEff()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
LayerEff::analyze(const edm::Event& e, const edm::EventSetup& es)
{
  using namespace edm;
  using namespace std;

  //  edm::ParameterSet config_;
  
  //CombinatoriaTrack
  Handle<reco::TrackCollection> trackCollectionCKF;
  edm::InputTag TkTagCKF = config_.getParameter<edm::InputTag>("tracks");
  e.getByLabel(TkTagCKF,trackCollectionCKF);

  // get the SiStripQuality records
  edm::ESHandle<SiStripQuality> SiStripQuality_;
  es.get<SiStripQualityRcd>().get(SiStripQuality_);

  // Tracking 
  const   reco::TrackCollection *tracksCKF=trackCollectionCKF.product();
  cout << "number ckf tracks found = " << tracksCKF->size() << endl;
  if (tracksCKF->size() == 1 ){
    cout << "starting checking good single track event" << endl;
    reco::TrackCollection::const_iterator iCKF=trackCollectionCKF.product()->begin();

    cout << "rechit size for track is " << iCKF->recHitsSize() << endl;

    uint previousLayer = 0;
    
    for (trackingRecHit_iterator iHit = iCKF->recHitsBegin(); iHit < iCKF->recHitsEnd(); iHit++) {

      uint iidd = (*iHit)->geographicalId().rawId();

      StripSubdetector strip=StripSubdetector(iidd);
      unsigned int subid=strip.subdetId();
      uint TKlayers = 0;
      if (subid ==  StripSubdetector::TIB) { 
	TIBDetId tibid(iidd);
	TKlayers = tibid.layer();
      }
      if (subid ==  StripSubdetector::TOB) { 
	TOBDetId tobid(iidd);
	TKlayers = tobid.layer() + 4 ; 
      }
      if (subid ==  StripSubdetector::TID) { 
	TIDDetId tidid(iidd);
	TKlayers = tidid.wheel() + 10;
      }
      if (subid ==  StripSubdetector::TEC) { 
	TECDetId tecid(iidd);
	TKlayers = tecid.wheel() + 13 ; 
      }

      if ( SiStripQuality_->getBadApvs(iidd)!=0 ) {
	qualBad_ = 1; 
      } else {
	qualBad_ = 0; 
      }
      
    
      cout << "rechit in layer = " << TKlayers << " with valid status = " << (*iHit)->isValid() << " and previous layer = " << previousLayer << endl;
      
      layer_ = TKlayers;
      valid_ = (*iHit)->isValid();
      module_ = iidd;
      
      if (TKlayers!=previousLayer)     rootTree_->Fill();
      
      previousLayer = TKlayers;
      
    }
    
  }
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
LayerEff::beginJob(const edm::EventSetup&)
{
  edm::Service<TFileService> fs;
  //
  // root output
  //
  rootTree_ = fs->make<TTree>("LayerEff","LayerEff");
  rootTree_->Branch("layer",&layer_,"layer/i");
  rootTree_->Branch("valid",&valid_,"valid/i");
  rootTree_->Branch("modules",&module_,"module/i");
  rootTree_->Branch("qualBad",&qualBad_,"qualBad/i");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
LayerEff::endJob() {
  if ( rootTree_ ) {
    rootTree_->GetDirectory()->cd();
    rootTree_->Write();
    delete rootTree_;
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(LayerEff);
