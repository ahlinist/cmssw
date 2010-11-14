#include "TauAnalysis/Skimming/plugins/EventCountFilter.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <TPRegexp.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>

#include <iostream>
#include <fstream>

const int noMatchRequired = -1;

EventCountFilter::EventCountFilter(const edm::ParameterSet& cfg)
{
  numEventsToBeSelected_ = cfg.getParameter<int>("numEvents");
  firstEventToBeSelected_ = cfg.exists("firstEvent") ?
    cfg.getParameter<int>("firstEvent") : 1;

  numEventsProcessed_ = 0;
}

EventCountFilter::~EventCountFilter()
{
// nothing to be done yet...
}

bool EventCountFilter::filter(edm::Event& evt, const edm::EventSetup& es)
{
  ++numEventsProcessed_;

  if ( numEventsProcessed_ >= firstEventToBeSelected_ && 
       numEventsProcessed_ < (firstEventToBeSelected_ + numEventsToBeSelected_) ) 
    return true;
  else
    return false;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(EventCountFilter);

