// -*- C++ -*-
//
// Package:    TrackDifferences
// Class:      TrackDifferences
// 
/**\class TrackDifferences TrackDifferences.cc RecoLocalTracker/TrackDifferences/src/TrackDifferences.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jean-Roch Vlimant,40 3-A28,+41227671209,
//         Created:  Fri Aug 12 16:44:46 CEST 2011
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"

#include <fstream>

//
// class declaration
//

class TrackDifferences : public edm::EDProducer {
   public:
      explicit TrackDifferences(const edm::ParameterSet&);
      ~TrackDifferences();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

  edm::InputTag track1_,track2_;
  float deltaR_;
      // ----------member data ---------------------------
  bool match(const reco::Track & t1,const reco::Track & t2){
    return (deltaR(t1.eta(),t1.phi(),t2.eta(),t2.phi()) < deltaR_);
  }

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
TrackDifferences::TrackDifferences(const edm::ParameterSet& iConfig)
{
   //register your products
  produces<reco::TrackCollection>("common");
  produces<reco::TrackCollection>("in1notin2");
  produces<reco::TrackCollection>("in2notin1");
  track1_ = iConfig.getParameter<edm::InputTag>("track1");
  track2_ = iConfig.getParameter<edm::InputTag>("track2");

  deltaR_ = iConfig.getParameter<double>("deltaR");
}


TrackDifferences::~TrackDifferences()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
TrackDifferences::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  std::fstream mask("maskGoodTracks.txt",std::fstream::out);
  uint maxCount=100;
  uint count=0;
  mask<<maxCount<<std::endl;
  edm::Handle<reco::TrackCollection> track1;
  edm::Handle<reco::TrackCollection> track2;
  iEvent.getByLabel(track1_,track1);
  iEvent.getByLabel(track2_,track2);
  
  std::auto_ptr<reco::TrackCollection> common(new reco::TrackCollection());
  std::auto_ptr<reco::TrackCollection> in1notin2(new reco::TrackCollection());
  std::auto_ptr<reco::TrackCollection> in2notin1(new reco::TrackCollection());

  edm::ESHandle<TransientTrackingRecHitBuilder> builder;
  iSetup.get<TransientRecHitRecord>().get("WithTrackAngle",builder);

  std::cout<<"getting :"<<track1->size()<<" and "<<track2->size()<<" tracks in input "<<std::endl;

  for (unsigned int i1=0;i1!=track1->size();++i1){
    const reco::Track & t1=(*track1)[i1];
    bool matched=false;
    for (unsigned int i2=0;i2!=track2->size();++i2){
      const reco::Track & t2=(*track2)[i2];
      if (match(t1,t2)){
	matched=true;
	common->push_back(t1);
	break;
      }
      //      if (matched) break;
    }
    if (!matched) in1notin2->push_back(t1);
  }
  
  for (unsigned int i2=0;i2!=track2->size();++i2){
    const reco::Track & t2=(*track2)[i2];
    bool matched=false;
    for (unsigned int i1=0;i1!=track1->size();++i1){
      const reco::Track & t1=(*track1)[i1];
      if (match(t2,t1)){
        matched=true;
	// no need to push in the common collection, otherwise there is double counting
	//        common->push_back(t2);	
	//	std::cout<<"one track: "<<t2.pt()<<" "<<t2.eta()<<" "<<t2.phi()<<std::endl;
	for (trackingRecHit_iterator iHit=t2.recHitsBegin();
	     iHit!=t2.recHitsEnd();++iHit){
	  if (!((*iHit)->isValid())) continue;
	  TransientTrackingRecHit::RecHitPointer tHit = builder->build(&(**iHit));
	  GlobalPoint tPoint=tHit->globalPosition();
	  //	  std::cout<<(*iHit)->geographicalId()<<"\t"<<tPoint.x()<<"\t"<<tPoint.y()<<"\t"<<tPoint.z()<<std::endl;
	  count++;
	  if (count<=maxCount) mask<<(*iHit)->geographicalId()<<"\t"<<tPoint.x()<<"\t"<<tPoint.y()<<"\t"<<tPoint.z()<<std::endl;
	}
	break;
      }
      //      if (matched) break;
    }
    if (!matched) in2notin1->push_back(t2);
  }
  

  //inspection of notin2 and the cluster masking
  // check whether the track "lost" have clusters from the mask

  mask.close();
  std::cout<<common->size()<<" in common "<<in1notin2->size()<<" not in the second "<<in2notin1->size()<<" not in the first"<<std::endl;
  iEvent.put(common,"common");
  iEvent.put(in1notin2,"in1notin2");
  iEvent.put(in2notin1,"in2notin1");
}

// ------------ method called once each job just before starting event loop  ------------
void 
TrackDifferences::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TrackDifferences::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
TrackDifferences::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
TrackDifferences::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
TrackDifferences::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
TrackDifferences::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TrackDifferences::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackDifferences);
