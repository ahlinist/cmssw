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

template<typename Obj>
double
TauMatchStringFunction<Obj>::compute(const CandBaseRef &c) const
{
   assert(handler_);
   const reco::CandViewMatchMap& matchMap = **handler_;
   // Get the matched object
   reco::CandViewMatchMap::const_iterator matched = matchMap.find(c);
   // Check to make sure a truth object exists for this input
   if( matched == matchMap.end() )
   {
      throw cms::Exception("TauMatrchStringFunctionKeyError") << " the TauMatchStringFunciton " <<
         this->name() << " has been passed an object that is not matched to any in the input colleciton";
   }
   // Get the matched object
   const CandBaseRef &matchedCand = matched->val;
   // Compute our funtion on this object
   return this->evaluate(matchedCand);
}


#endif
