#ifndef SusyAnalysis_EventSelector_HTEventSelector_h_
#define SusyAnalysis_EventSelector_HTEventSelector_h_
/// Selector for HT
///
/// Computes HT from all jets and cuts on it
///
/// $Id: HTEventSelector.h,v 1.2 2008/05/22 08:32:26 fronga Exp $

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class HTEventSelector : public SusyEventSelector {
public:
  HTEventSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~HTEventSelector () {}

private:
  edm::InputTag jetTag_; ///< tag for input collection
  float minHT_;          ///< lower Et cuts (defines also min. #jets)
};
#endif
