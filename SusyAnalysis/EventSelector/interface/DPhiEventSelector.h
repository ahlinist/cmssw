#ifndef SusyAnalysis_DPhiEventSelector_h_
#define SusyAnalysis_DPhiEventSelector_h_
///
/// DPhiEventSelector
///
/// Computes the delta phi variable in di-jet events
/// 
/// \author jjones - Tue Jun  3 15:42:09 CEST 2008
///
/// $Id: DPhiSelector.h,v 1.1 2008/06/04 13:39:39 jaj99 Exp $
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"

class DPhiEventSelector : public SusyEventSelector {
public:
  DPhiEventSelector (const edm::ParameterSet&);
  virtual bool select(const edm::Event&) const;
  virtual ~DPhiEventSelector () {}

 private:
  // Define parameters here
  edm::InputTag jetTag_;
  double maxDPhi_;
  double localPi;
};
#endif
