#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_PileUpNumberFilter_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_PileUpNumberFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class PileUpNumberFilter : public edm::EDFilter {
    public:
       explicit PileUpNumberFilter(const edm::ParameterSet&);
       ~PileUpNumberFilter();

       virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
       edm::InputTag pileUpInfoTag_;
       unsigned int numberPU_;
};

#endif

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

PileUpNumberFilter::PileUpNumberFilter(const edm::ParameterSet& pset){
        pileUpInfoTag_ = pset.getParameter<edm::InputTag>("PileUpInfoTag"); 
        numberPU_ = pset.getParameter<unsigned int>("NumberOfPileUpEvents");
}

PileUpNumberFilter::~PileUpNumberFilter(){}

bool PileUpNumberFilter::filter(edm::Event& event, const edm::EventSetup& setup){
        edm::Handle<std::map<int,int> > pileUpMap;
        event.getByLabel("pileUpInfo",pileUpMap);
        std::map<int,int>::const_iterator bx0Iter = pileUpMap->find(0);
        if(bx0Iter == pileUpMap->end()){
           edm::LogError("Analysis") << ">>> Pile-up info does not contain bunch crossing 0 ..skipping";
           return false;
        }
        int nPileUpBx0 = bx0Iter->second;
        //edm::LogVerbatim("Analysis") << "  Number of pile-up events in bunch crossing 0: " << nPileUpBx0;

        bool accept = (nPileUpBx0 == numberPU_);

        return accept;
}

DEFINE_FWK_MODULE(PileUpNumberFilter);

