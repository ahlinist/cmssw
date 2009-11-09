#include "RecoTauTag/TauAnalysisTools/interface/TauDiscriminatorFunction.h"

template<class TauType, class DiscriminatorType>
bool
TauDiscriminatorFunction<TauType, DiscriminatorType>::isValid() const
{
   if( handle_ && handle_->isValid())
   {
      return true;
   }
   else 
      return false;
}

template<class TauType, class DiscriminatorType>
double
TauDiscriminatorFunction<TauType, DiscriminatorType>::compute(const CandBaseRef &c) const
{
   tau_ref specificRef = c.castTo<tau_ref>();
   //return (*handle_)[specificRef];
   double result = (**handle_)[specificRef];
   return result;
}


