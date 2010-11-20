#include "TauAnalysis/Skimming/plugins/EventCountAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

EventCountAnalyzer::EventCountAnalyzer(const edm::ParameterSet& cfg)
{
  //std::cout << "<EventCountAnalyzer::EventCountAnalyzer>:" << std::endl;
  numEventsProcessed_ = 0;
}

EventCountAnalyzer::~EventCountAnalyzer()
{
  edm::LogInfo("~EventCountAnalyzer") << " Number of Events processed: >>> " << numEventsProcessed_ << " <<<";
  //std::cout << "<EventCountAnalyzer::~EventCountAnalyzer>:" << std::endl;
  //std::cout << " Number of Events processed: >>> " << numEventsProcessed_ << " <<<" << std::endl;
}

void EventCountAnalyzer::beginJob()
{
// nothing to be done yet...
}

void EventCountAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  //std::cout << "<EventCountAnalyzer::analyze>:" << std::endl;
  ++numEventsProcessed_;
}

void EventCountAnalyzer::endJob()
{
// nothing to be done yet...
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(EventCountAnalyzer);


