#ifndef LooperClusterRemover_H
#define LooperClusterRemover_H


// -*- C++ -*-
//
// Package:    LooperClusterRemover
// Class:      LooperClusterRemover
// 
/**\class LooperClusterRemover LooperClusterRemover.cc RecoLocalTracker/LooperClusterRemover/src/LooperClusterRemover.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jean-Roch Vlimant,40 3-A28,+41227671209,
//         Created:  Wed Aug  3 12:12:46 CEST 2011
// $Id: LooperClusterRemover.h,v 1.3 2012/02/07 15:19:48 vlimant Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit1D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"

#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/Common/interface/ContainerMask.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//
// class declaration
//

namespace LooperClusterRemoverMethod {
  class Method;
}

class LooperClusterRemover : public edm::EDProducer {
   public:
      typedef edm::ContainerMask<edmNew::DetSetVector<SiPixelCluster> > PixelMaskContainer;
      typedef edm::ContainerMask<edmNew::DetSetVector<SiStripCluster> > StripMaskContainer;

      explicit LooperClusterRemover(const edm::ParameterSet&);
      ~LooperClusterRemover();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

      struct products{

	std::vector<bool> collectedPixels;
	std::vector<bool> collectedStrips;
	std::auto_ptr<TrackCandidateCollection> tcOut;
	std::auto_ptr<reco::TrackCollection> tOut;
	std::auto_ptr<reco::TrackExtraCollection> teOut;
	std::auto_ptr<TrackingRecHitCollection> trhOut;
      };

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      void reset(edm::Event&);
      void put(edm::Event& iEvent);

      products prod_;
      LooperClusterRemoverMethod::Method * method_;
      edm::InputTag pixelClusters_,stripClusters_;

};

namespace LooperClusterRemoverMethod {
  class Method{
  public:
    Method(){};
    Method(const edm::ParameterSet&){};
    virtual void run(edm::Event&, const edm::EventSetup&,
		     LooperClusterRemover::products &)=0;
  };
}


#endif
