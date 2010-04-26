#ifndef SusyAnalysis_EventSelector_MHTEventSelector_h_
#define SusyAnalysis_EventSelector_MHTEventSelector_h_
/// Selector for MHT
///
/// Computes MHT from all jets and cuts on it
///
/// $Id: MHTEventSelector.h,v 1.2 2009/06/05 18:11:04 xshi Exp $

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class MHTEventSelector : public SusyEventSelector {
public:
  MHTEventSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~MHTEventSelector () {}

private:
  edm::InputTag jetTag_; ///< tag for input collection
  edm::InputTag mhtTag_; ///< tag for input collection
  float minMHT_;          ///< lower MHT cut
  float maxMHT_;          ///< upper MHT cut
  float minPt_;          ///< minimum Pt of jets taken into account
};
#endif
