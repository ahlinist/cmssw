#include "TauAnalysis/Skimming/plugins/EwkTauValidation.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"

const std::string dqmSeparator = "/";

std::string dqmDirectoryName(const std::string& dqmRootDirectory, const std::string& dqmSubDirectory)
{
//--- concatenate names of dqmRootDirectory and dqmSubDirectory;
//    add "/" separator inbetween if necessary
  std::string dirName = dqmRootDirectory;
  if ( dirName != "" && dirName.find_last_of(dqmSeparator) != (dirName.length() - 1) )  dirName.append(dqmSeparator);
  dirName.append(dqmSubDirectory);
  return dirName;
}

EwkTauValidation::EwkTauValidation(const edm::ParameterSet& cfg)
  : dqmDirectory_(cfg.getParameter<std::string>("dqmDirectory")),
    dqmError_(0)
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("EwkTauValidation") << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!";
    dqmError_ = 1;
    return;
  }

  DQMStore* dqmStore = &(*edm::Service<DQMStore>());

  maxNumWarnings_ = cfg.exists("maxNumWarnings") ? cfg.getParameter<int>("maxNumWarnings") : 1;

  edm::ParameterSet cfgChannels = cfg.getParameter<edm::ParameterSet>("channels");

  edm::ParameterSet cfgElecTauChannel = cfgChannels.getParameter<edm::ParameterSet>("elecTauChannel");
  std::string dqmSubDirectoryElecTauChannel = cfgElecTauChannel.getParameter<std::string>("dqmSubDirectory");  
  cfgElecTauChannel.addParameter<std::string>("dqmDirectory", dqmDirectoryName(dqmDirectory_, dqmSubDirectoryElecTauChannel));
  cfgElecTauChannel.addParameter<int>("maxNumWarnings", maxNumWarnings_);
  elecTauHistManager_ = new EwkElecTauValHistManager(cfgElecTauChannel, dqmStore);

  edm::ParameterSet cfgMuTauChannel = cfgChannels.getParameter<edm::ParameterSet>("muTauChannel");
  std::string dqmSubDirectoryMuTauChannel = cfgMuTauChannel.getParameter<std::string>("dqmSubDirectory");
  cfgMuTauChannel.addParameter<std::string>("dqmDirectory", dqmDirectoryName(dqmDirectory_, dqmSubDirectoryMuTauChannel));
  cfgMuTauChannel.addParameter<int>("maxNumWarnings", maxNumWarnings_);
  muTauHistManager_ = new EwkMuTauValHistManager(cfgMuTauChannel, dqmStore);
}

EwkTauValidation::~EwkTauValidation()
{
  delete elecTauHistManager_;
  delete muTauHistManager_;
}

void EwkTauValidation::beginJob()
{
  if ( dqmError_ ) return;
 
  elecTauHistManager_->bookHistograms();
  muTauHistManager_->bookHistograms();
}

void EwkTauValidation::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  if ( dqmError_ ) return;

  elecTauHistManager_->fillHistograms(evt, es);
  muTauHistManager_->fillHistograms(evt, es);
}

void EwkTauValidation::endJob()
{
  if ( dqmError_ ) return;

  elecTauHistManager_->finalizeHistograms();
  muTauHistManager_->finalizeHistograms();
}

