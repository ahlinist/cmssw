#ifndef SusyAnalysis_EventSelector_MyMHTEventSelector_h_
#define SusyAnalysis_EventSelector_MyMHTEventSelector_h_
/// Selector for MHT
///
/// Computes HT from jets after selection on pT and |eta|
///   and cuts on it. HT is defined as scalar sum of the 
///   the transverse jet energies.
///
/// $Id: HTEventSelector.h,v 1.4 2008/10/03 14:34:06 adamwo Exp $

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class MyMHTEventSelector : public SusyEventSelector {
public:
  MyMHTEventSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~MyMHTEventSelector () {}

private:
  edm::InputTag jetTag_; ///< tag for input collection
  float minMHT_;          ///< lower HT cut 
  float minPt_;          ///< minimum Pt of jets taken into account
  float maxEta_;         ///< maximum Eta of jets taken into account
};
#endif
