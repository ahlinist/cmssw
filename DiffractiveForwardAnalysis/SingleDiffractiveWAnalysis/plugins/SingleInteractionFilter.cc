#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_SingleInteractionFilter_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_SingleInteractionFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class SingleInteractionFilter : public edm::EDFilter {
    public:
       explicit SingleInteractionFilter(const edm::ParameterSet&);
       ~SingleInteractionFilter();

       virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
       edm::InputTag pileUpInfoTag_;
};

#endif

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

SingleInteractionFilter::SingleInteractionFilter(const edm::ParameterSet& pset){
        pileUpInfoTag_ = pset.getParameter<edm::InputTag>("PileUpInfoTag"); 
}

SingleInteractionFilter::~SingleInteractionFilter(){}

bool SingleInteractionFilter::filter(edm::Event& event, const edm::EventSetup& setup){
        edm::Handle<std::map<int,int> > pileUpMap;
        event.getByLabel("pileUpInfo",pileUpMap);
        std::map<int,int>::const_iterator bx0Iter = pileUpMap->find(0);
        if(bx0Iter == pileUpMap->end()){
           edm::LogError("Analysis") << ">>> Pile-up info does not contain bunch crossing 0 ..skipping";
           return false;
        }
        int nPileUpBx0 = bx0Iter->second;
        //edm::LogVerbatim("Analysis") << "  Number of pile-up events in bunch crossing 0: " << nPileUpBx0;

        bool singleInteraction = (nPileUpBx0 == 0);

        return singleInteraction;
}

DEFINE_FWK_MODULE(SingleInteractionFilter);

