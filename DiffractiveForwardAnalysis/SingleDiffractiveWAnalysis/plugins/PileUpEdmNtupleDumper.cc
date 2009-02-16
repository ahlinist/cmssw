#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_PileUpEdmNtupleDumper_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_PileUpEdmNtupleDumper_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class PileUpEdmNtupleDumper: public edm::EDProducer {
public:
  PileUpEdmNtupleDumper(const edm::ParameterSet&);
private:
  void produce(edm::Event&,const edm::EventSetup&);
}; 
#endif

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"

PileUpEdmNtupleDumper::PileUpEdmNtupleDumper(const edm::ParameterSet& conf) {
    produces<std::map<int,int> >();
}

void PileUpEdmNtupleDumper::produce(edm::Event& event, const edm::EventSetup& setup) {
    // Access CrossingFrame of HepMCProducts
    edm::Handle<CrossingFrame<edm::HepMCProduct> > crossingFrameHepMCH;
    event.getByLabel("mix","source",crossingFrameHepMCH);

    unsigned int nbunches = 0;
    std::auto_ptr<std::map<int,int> > pileUpMap(new std::map<int,int>);
    for(int ibunch = crossingFrameHepMCH->getBunchRange().first; ibunch <= crossingFrameHepMCH->getBunchRange().second; ++ibunch,++nbunches){
       int nrPileUp = crossingFrameHepMCH->getNrPileups(ibunch);
       pileUpMap->insert(std::make_pair(ibunch,nrPileUp));
       edm::LogVerbatim("Analysis") << " Number of added pile-up's for bunch " << ibunch << ": " << nrPileUp;
    }

    event.put(pileUpMap);
}

DEFINE_FWK_MODULE(PileUpEdmNtupleDumper);
 
