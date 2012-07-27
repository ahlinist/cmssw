#include "RecoTracker/Loopers/interface/LooperClusterRemover.h"

#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "RecoTracker/TkSeedGenerator/interface/SeedCreator.h"
#include "RecoTracker/TkSeedGenerator/interface/SeedCreatorFactory.h"
#include <fstream>


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
    edm::InputTag pixelRecHits_,stripRecHits_;
    double fractionOfTruth_;
    FractionOfTruth(const edm::ParameterSet& iConfig){
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
      useUnmatched_=iConfig.getParameter<bool>("useUnmatched");
      rphiRecHits_=iConfig.getParameter<edm::InputTag>("rphiRecHits");
      stereoRecHits_=iConfig.getParameter<edm::InputTag>("stereoRecHits");
      collectorConf_=iConfig.getParameter<edm::ParameterSet>("collector");
      makeTC_=iConfig.getParameter<bool>("makeTrackCandidates");
      makeT_=iConfig.getParameter<bool>("makeTrack");
      pxlClusterCharge_=iConfig.getParameter<double>("pxlClusterCharge");
      if (makeTC_){
	const edm::ParameterSet & sC=iConfig.getParameter<edm::ParameterSet>("SeedCreatorPSet");
	aCreator = SeedCreatorFactory::get()->create( sC.getParameter<std::string>("ComponentName"),
						      sC);
	maskWithNoTC_=iConfig.getParameter<bool>("maskWithNoTC");
      }else {
	aCreator=0;						      
	maskWithNoTC_=false;
      }
    };
    void run(edm::Event&, const edm::EventSetup&,
	     LooperClusterRemover::products &);
    
    bool useUnmatched_;
    edm::InputTag pixelRecHits_,stripRecHits_,rphiRecHits_,stereoRecHits_;
    edm::ParameterSet collectorConf_;
    bool makeTC_;
    bool makeT_;
    bool maskWithNoTC_;
    double pxlClusterCharge_;

    SeedCreator * aCreator;
    ~LooperMethod(){
      if (makeTC_) delete aCreator;
    }
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
