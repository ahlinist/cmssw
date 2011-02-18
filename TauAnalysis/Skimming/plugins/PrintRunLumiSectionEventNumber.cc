#include "TauAnalysis/Skimming/plugins/PrintRunLumiSectionEventNumber.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"

PrintRunLumiSectionEventNumber::PrintRunLumiSectionEventNumber(const edm::ParameterSet& cfg)
  : cfgError_(0)
{
  //std::cout << "<PrintRunLumiSectionEventNumber::PrintRunLumiSectionEventNumber>:" << std::endl;

  outputStream_ = getOutputOptions(cfg, isOutputFile_, cfgError_);

  separator_ = cfg.exists("separator") ? 
    cfg.getParameter<std::string>("separator") : ":";
}

PrintRunLumiSectionEventNumber::~PrintRunLumiSectionEventNumber()
{
//--- close output file
  if ( isOutputFile_ ) delete outputStream_;
}

void PrintRunLumiSectionEventNumber::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("analyze") << " Error in Configuration ParameterSet --> skipping !!";
    return;
  }

//--- retrieve run and event numbers from the event
  edm::RunNumber_t runNumber = evt.id().run();
  edm::LuminosityBlockNumber_t lumiSectionNumber = evt.luminosityBlock();
  edm::EventNumber_t eventNumber = evt.id().event();

  *outputStream_ << runNumber << separator_ << lumiSectionNumber << separator_ << eventNumber << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PrintRunLumiSectionEventNumber);

