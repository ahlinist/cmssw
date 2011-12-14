#include "RecoTracker/Loopers/interface/LooperClusterRemover.h"

#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#include <fstream>
#include "DataFormats/JetReco/interface/Jet.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"

namespace LooperClusterRemoverMethod {

  class PerJet : public Method{
  public:
    PerJet(const edm::ParameterSet& iConfig){
      pixelRecHits_=iConfig.getParameter<edm::InputTag>("pixelRecHits");
      stripRecHits_=iConfig.getParameter<edm::InputTag>("stripRecHits");
      previousMask_=iConfig.getParameter<edm::InputTag>("previousMask");
      jets_=iConfig.getParameter<edm::InputTag>("jets");
      coneSize_=iConfig.getParameter<double>("coneSize");
    }
    void run(edm::Event&, const edm::EventSetup&,
	     LooperClusterRemover::products &);
    bool inZone(edm::Handle<edm::View<reco::Candidate> > &, TransientTrackingRecHit::RecHitPointer &);
    edm::InputTag pixelRecHits_,stripRecHits_;
    edm::InputTag previousMask_,jets_;
    double coneSize_;
  };
  class FractionOfTruth : public Method{
  public:
    //takes in list of rechits, associated to MC truth and rejects a fraction of loopers.
    //    edm::ParameterSet associatorConf_;
    //    TrackerHitAssociator  * associator_;
    edm::InputTag pixelRecHits_,stripRecHits_;
    double fractionOfTruth_;
    FractionOfTruth(const edm::ParameterSet& iConfig){
      //      associatorConf_ = iConfig.getParameter<edm::ParameterSet>("associatorConf");
      pixelRecHits_=iConfig.getParameter<edm::InputTag>("pixelRecHits");
      stripRecHits_=iConfig.getParameter<edm::InputTag>("stripRecHits");
      fractionOfTruth_=iConfig.getParameter<double>("fraction");
    }
    void run(edm::Event&, const edm::EventSetup&,
	     LooperClusterRemover::products &);
  };


  class LooperMethod : public Method{
  public:
    LooperMethod(const edm::ParameterSet& iConfig){
      pixelRecHits_=iConfig.getParameter<edm::InputTag>("pixelRecHits");
      stripRecHits_=iConfig.getParameter<edm::InputTag>("stripRecHits");
      collectorConf_=iConfig.getParameter<edm::ParameterSet>("collector");
      makeTC_=iConfig.getParameter<bool>("makeTrackCandidates");
    };
    void run(edm::Event&, const edm::EventSetup&,
	     LooperClusterRemover::products &);
    
    edm::InputTag pixelRecHits_,stripRecHits_;
    edm::ParameterSet collectorConf_;
    bool makeTC_;
  };

  class ReadFileIn : public Method{
  public:
    ReadFileIn(const edm::ParameterSet& iConfig);
    void run(edm::Event&, const edm::EventSetup&,
	     LooperClusterRemover::products &);
    fstream file_;
    bool withDetId_;
    edm::InputTag pixelRecHits_,stripRecHits_;
    double epsilon_;
  };


  class EveryNMethod : public Method{
  public:
    EveryNMethod(const edm::ParameterSet& iConfig){
      pixelRecHits_=iConfig.getParameter<edm::InputTag>("pixelRecHits");
      stripRecHits_=iConfig.getParameter<edm::InputTag>("stripRecHits");
      everyNPixel_ =iConfig.getParameter<unsigned int>("everyNPixel");
    }
    unsigned int everyNPixel_;
    edm::InputTag pixelRecHits_,stripRecHits_;
    void run(edm::Event&, const edm::EventSetup&,
	     LooperClusterRemover::products &);
  };


} //namespace
