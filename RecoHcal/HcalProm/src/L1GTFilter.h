// Module to filter events with particular trigger bits
// can dump trigger table as well
// F.Ratnikov UMd, May 29, 2008
// $Id:$

#ifndef L1GTFilter_H
#define L1GTFilter_H

#include <string>

#include "FWCore/Framework/interface/EDFilter.h"

class L1GtTriggerMenu;

class L1GTFilter: public edm::EDFilter {
 public:
  L1GTFilter(const edm::ParameterSet&);
  ~L1GTFilter() {}
  virtual bool filter (edm::Event&, const edm::EventSetup&);
 private:
  std::string mTriggerName;
  int mTriggerBit;
  unsigned long long mCacheId;
  bool mDumpTriggerTable;
  const L1GtTriggerMenu* mMenu;
};

#endif
