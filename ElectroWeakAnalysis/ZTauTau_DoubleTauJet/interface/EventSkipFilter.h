#ifndef EventSkipFilter_H
#define EventSkipFilter_H

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Provenance/interface/EventID.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/Makers.h"

#include <vector>

class EventSkipFilter: public edm::EDFilter {

public:
  explicit EventSkipFilter(const edm::ParameterSet&);
  ~EventSkipFilter() {}
  virtual bool filter(edm::Event&, const edm::EventSetup&);

private:

  int _debugFlg;
  std::vector<edm::EventID> _eventIDList;
};
#endif
