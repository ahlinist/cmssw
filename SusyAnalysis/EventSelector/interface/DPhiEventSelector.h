#ifndef SusyAnalysis_DPhiEventSelector_h_
#define SusyAnalysis_DPhiEventSelector_h_
///
/// DPhiEventSelector
///
/// Computes the delta phi variable in di-jet events
///
/// \author jjones - Tue Jun  3 15:42:09 CEST 2008
///
/// $Id: DPhiEventSelector.h,v 1.2 2010/05/28 08:01:48 csander Exp $
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"

class DPhiEventSelector: public SusyEventSelector {
public:
   DPhiEventSelector(const edm::ParameterSet&);
   virtual bool select(const edm::Event&) const;
   virtual ~DPhiEventSelector() {
   }

private:
   // Define parameters here
   edm::InputTag jetTag_;
   double maxDPhi_;
   double localPi;
};
#endif
