#ifndef SusyAnalysis_EventSelectorAND_h_
#define SusyAnalysis_EventSelectorAND_h_
/** Combination of selectors by logical AND.
 */
// Original author: W. Adam, 10/4/08

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "SusyAnalysis/EventSelector/interface/SelectorSequence.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include <vector>

class SelectorSequence;

class EventSelectorAND : public SusyEventSelector {
public:
  EventSelectorAND ();
  EventSelectorAND (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~EventSelectorAND () {}
private:
  SelectorSequence sequence_;
};
#endif
