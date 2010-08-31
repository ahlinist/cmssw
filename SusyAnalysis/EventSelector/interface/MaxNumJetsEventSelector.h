#ifndef SusyAnalysis_MaxNumJetsEventSelector_h_
#define SusyAnalysis_MaxNumJetsEventSelector_h_
///
/// MaxNumJetsEventSelector
///
/// Selects on number (n) of jets with ET cut on N+1 jet
///
/// \author jjones - Tue Jun  3 15:42:09 CEST 2008
///
/// $Id: MaxNumJetsEventSelector.h,v 1.2 2010/05/28 08:01:48 csander Exp $
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
//#include "FWCore/ParameterSet/interface/InputTag.h"
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"

class MaxNumJetsEventSelector: public SusyEventSelector {
public:
   MaxNumJetsEventSelector(const edm::ParameterSet&);
   virtual bool select(const edm::Event&) const;
   virtual ~MaxNumJetsEventSelector() {
   }

private:
   // Define parameters here
   edm::InputTag jetTag_;
   double maxEt_; ///< Et cuts on n+1 jet
   unsigned int maxNumJets_; // number of jets allowed
};
#endif
