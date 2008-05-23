#ifndef SusyAnalysis_EventSelector_ChargedFractionSelector_h_
#define SusyAnalysis_EventSelector_ChargedFractionSelector_h_
///
/// Selection based on charged fraction.
///
/// Calculates the total charge fraction from all jets
///
/// $Id: ChargedFractionSelector.h,v 1.3 2008/05/22 08:32:26 fronga Exp $

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class ChargedFractionSelector : public SusyEventSelector {
public:
  ChargedFractionSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~ChargedFractionSelector () {}

private:
  edm::InputTag jetTag_; ///< tag for input collection
  float minFraction_;    ///< lower charge fraction cut
  float maxEta_;         ///< upper |eta| cut on all jets
  float minPt_;          ///< lower pt cut on all jets
  unsigned int minTracks_; ///< minimum number of associated tracks

};
#endif
