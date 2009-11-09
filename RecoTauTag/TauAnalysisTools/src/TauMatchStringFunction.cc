#include "RecoTauTag/TauAnalysisTools/interface/TauMatchStringFunction.h"

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

