#ifndef SusyAnalysis_EventSelector_PrimaryVertexEventSelector_h_
#define SusyAnalysis_EventSelector_PrimaryVertexEventSelector_h_

/// Selector for presence of a primary vertex.
///
/// Just checks that there is (at least) one primary vertex.
///
/// $Id: PrimaryVertexEventSelector.h,v 1.2 2008/05/22 08:32:26 fronga Exp $

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class PrimaryVertexEventSelector : public SusyEventSelector {
public:
  PrimaryVertexEventSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~PrimaryVertexEventSelector () {}
private:
  edm::InputTag vertexTag_; ///< tag for input collection
};

#endif
