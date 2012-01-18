#ifndef SusyAnalysis_WeightSelector_h_
#define SusyAnalysis_WeightSelector_h_
///
/// WeightSelector
///
/// \author csander - Tue Oct 18 08:50:55 CEST 2011
///
/// $Id: $
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
//#include "FWCore/ParameterSet/interface/InputTag.h"

class WeightSelector: public SusyEventSelector {
   public:
      WeightSelector(const edm::ParameterSet&);
      virtual bool select(const edm::Event&) const;
      virtual ~WeightSelector() {
      }

   private:
      // Define parameters here
      edm::InputTag weightTag_; ///< tag for input collection
};
#endif
