#ifndef RecoTauTag_TauMatchStringFunction_h
#define RecoTauTag_TauMatchStringFunction_h

/*
 * TauMatchStringFunction
 *
 * Author: Evan K. Friis (UC Davis)
 *
 * Extension of TauStringFunction which takes in the constructor
 * a handle to a CandViewMatchMap.  When TauStringFunction() is called 
 * with a matched object (generally RECO), the corresponsing MC object
 * is retrieved and the string function is evaluated on it.
 * 
 */

#include "RecoTauTag/TauAnalysisTools/interface/TauStringFunction.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"


template<typename Obj>
class TauMatchStringFunction : public TauStringFunction<Obj> {
   public:
      // For some reason this needs to be redefined...
      typedef edm::RefToBase<reco::Candidate> CandBaseRef;
      TauMatchStringFunction(const std::string &expr, 
            const edm::Handle<reco::CandViewMatchMap> *handle):TauStringFunction<Obj>(expr),handler_(handle){};
      virtual ~TauMatchStringFunction(){};
   protected:
      virtual double compute(const CandBaseRef &c) const;
   private:
      const edm::Handle<reco::CandViewMatchMap> *handler_;
};

// Templated implementation in 
// RecoTauTag/TauAnalysisTools/src/TauMatchStringFunction.cc


#endif
