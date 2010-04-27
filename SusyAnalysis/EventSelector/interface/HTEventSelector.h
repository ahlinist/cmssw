#ifndef SusyAnalysis_EventSelector_HTEventSelector_h_
#define SusyAnalysis_EventSelector_HTEventSelector_h_
/// Selector for HT
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

class HTEventSelector : public SusyEventSelector {
public:
  HTEventSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~HTEventSelector () {}

private:
  edm::InputTag jetTag_; ///< tag for input collection
  float minHT_;          ///< lower HT cut 
  float minPt_;          ///< minimum Pt of jets taken into account
  float maxEta_;         ///< maximum Eta of jets taken into account
  double minFem_;
  double maxFem_;
  int minN90_;
  double minfHPD_;
};
#endif
