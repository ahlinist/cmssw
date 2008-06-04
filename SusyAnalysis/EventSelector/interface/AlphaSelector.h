#ifndef SusyAnalysis_AlphaSelector_h_
#define SusyAnalysis_AlphaSelector_h_
///
/// AlphaSelector
///
/// Computes the "alpha" variable in di-jet events
/// 
/// \author mstoye - Tue Jun  3 15:42:09 CEST 2008
///
/// $Id: $
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"

class AlphaSelector : public SusyEventSelector {
public:
  AlphaSelector (const edm::ParameterSet&);
  virtual bool select(const edm::Event&) const;
  virtual ~AlphaSelector () {}

 private:
  // Define parameters here
  edm::InputTag jetTag_;
  double minAlpha_;
};
#endif
