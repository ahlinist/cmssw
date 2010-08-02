// -*- C++ -*-
//
// Package:    PKAME
// Class:      PKAME
// 
/**\class PKAME PKAME.cc Filter/PKAME/src/PKAME.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Lea Caminada (PSI) [leac]
//         Created:  Thu Mar 18 11:19:51 CET 2010
// $Id$
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "FWCore/Framework/interface/ESHandle.h"


#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h" 

#include "CommonTools/TrackerMap/interface/TrackerMap.h"

#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h" 
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSRecHit2D.h" 
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "FWCore/Framework/interface/ESHandle.h"


#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h" 

#include "CommonTools/TrackerMap/interface/TrackerMap.h"

#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"



#include <TFile.h>
#include <TH1.h>

//
// class declaration
//

class PKAME : public edm::EDFilter {
   public:
      explicit PKAME(const edm::ParameterSet&);
      ~PKAME();

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

  std::string          fTracksLabel;
      
      // ----------member data ---------------------------
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
PKAME::PKAME(const edm::ParameterSet& iConfig):fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("generalTracks")))
{
   //now do what ever initialization is needed
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- Filter" << endl;
  cout << "--- " << fTracksLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;

}


PKAME::~PKAME()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
PKAME::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
// #ifdef THIS_IS_AN_EVENT_EXAMPLE
//    Handle<ExampleData> pIn;
//    iEvent.getByLabel("example",pIn);
// #endif

// #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
//    ESHandle<SetupData> pSetup;
//    iSetup.get<SetupRecord>().get(pSetup);
// #endif

  Handle<reco::TrackCollection> recTrks;
  iEvent.getByLabel(fTracksLabel.c_str(), recTrks); 

  int nhighpurity=0, ntot=0;
  for(reco::TrackCollection::const_iterator t=recTrks->begin(); t!=recTrks->end(); ++t){
    ntot++;
    if(t->quality(reco::TrackBase::highPurity)) nhighpurity++;
  }
  
  bool accept=true;
  if((nhighpurity<0.25*ntot)&&(ntot>10))
    accept=false;



   return accept;
}

// ------------ method called once each job just before starting event loop  ------------
void 
PKAME::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PKAME::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(PKAME);
