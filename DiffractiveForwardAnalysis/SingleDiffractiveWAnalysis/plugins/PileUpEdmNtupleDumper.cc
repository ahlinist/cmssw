#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_PileUpEdmNtupleDumper_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_PileUpEdmNtupleDumper_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>

class PileUpEdmNtupleDumper: public edm::EDProducer {
public:
  PileUpEdmNtupleDumper(const edm::ParameterSet&);
private:
  void produce(edm::Event&,const edm::EventSetup&);

  bool usePlayBackInfo_;
  std::vector<int> bunchRange_;
}; 
#endif

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfo.h"

PileUpEdmNtupleDumper::PileUpEdmNtupleDumper(const edm::ParameterSet& conf) {
    usePlayBackInfo_ = conf.getUntrackedParameter<bool>("AccessCrossingFramePlayBack",false);
    if(usePlayBackInfo_) bunchRange_ = conf.getParameter<std::vector<int> >("BunchCrossings");

    produces<std::map<int,int> >();
}

void PileUpEdmNtupleDumper::produce(edm::Event& event, const edm::EventSetup& setup) {
    std::auto_ptr<std::map<int,int> > pileUpMap(new std::map<int,int>);
    if(usePlayBackInfo_){
       // Access CrossingFramePlaybackInfo
       edm::Handle<CrossingFramePlaybackInfo>  playbackInfo;
       event.getByLabel("mix", playbackInfo);

       std::vector<edm::EventID> ids;
       std::vector<int> fileNrs;
       std::vector<unsigned int> nrEvents;
       const unsigned int s = 0;
       playbackInfo->getEventStartInfo(ids,fileNrs,nrEvents,s);

       if(bunchRange_.size() != nrEvents.size()){
          throw edm::Exception(edm::errors::Configuration,"PileUpEdmNtupleDumperError") << " Expecting different bunch ranges\n"; 
       }

       for(unsigned int ibunch = 0; ibunch < nrEvents.size(); ++ibunch){
          int nrPileUp = nrEvents[ibunch];
          pileUpMap->insert(std::make_pair(bunchRange_[ibunch],nrPileUp));
          edm::LogVerbatim("Analysis") << " Number of added pile-up's for bunch " << bunchRange_[ibunch] << ": " << nrEvents[ibunch];
       }
    } else{
       // Access CrossingFrame of HepMCProducts
       edm::Handle<CrossingFrame<edm::HepMCProduct> > crossingFrameHepMCH;
       event.getByLabel("mix","source",crossingFrameHepMCH);

       unsigned int nbunches = 0;
       //std::auto_ptr<std::map<int,int> > pileUpMap(new std::map<int,int>);
       for(int ibunch = crossingFrameHepMCH->getBunchRange().first; ibunch <= crossingFrameHepMCH->getBunchRange().second; ++ibunch,++nbunches){
          int nrPileUp = crossingFrameHepMCH->getNrPileups(ibunch);
          pileUpMap->insert(std::make_pair(ibunch,nrPileUp));
          edm::LogVerbatim("Analysis") << " Number of added pile-up's for bunch " << ibunch << ": " << nrPileUp;
       }
    }

    event.put(pileUpMap);
}

DEFINE_FWK_MODULE(PileUpEdmNtupleDumper);
 
