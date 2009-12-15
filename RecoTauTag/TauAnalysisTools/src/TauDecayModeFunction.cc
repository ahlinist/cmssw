#include "RecoTauTag/TauAnalysisTools/interface/TauDecayModeFunction.h"

bool
TauDecayModeFunction::isValid() const
{
   if( handle_ && handle_->isValid())
   {
      return true;
   }
   else 
      return false;
}

double
TauDecayModeFunction::compute(const CandBaseRef &c) const
{
   tau_ref specificRef = c.castTo<tau_ref>();
   //return (*handle_)[specificRef];
   const reco::PFTauDecayMode &my_decay_mode = (**handle_)[specificRef];
   return evaluate(my_decay_mode);
}


