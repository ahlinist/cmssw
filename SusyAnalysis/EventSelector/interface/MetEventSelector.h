#ifndef SusyAnalysis_MetEventSelector_h_
#define SusyAnalysis_MetEventSelector_h_
/// Trivial example for a MET selector.
/// To be modified for analysis!
///
/// Original author: W. Adam, 10/4/08
///
/// $Id: METEventSelector.h,v 1.3 2008/05/22 08:32:26 fronga Exp $

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include <vector>
#include <string>

class MetEventSelector : public SusyEventSelector {
public:
  MetEventSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~MetEventSelector () {}
private:
  edm::InputTag metTag_;      ///< tag for input collection
  float minMet_;              ///< lower cut on MET
};
#endif
