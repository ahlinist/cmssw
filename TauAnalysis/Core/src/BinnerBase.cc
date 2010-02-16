#include "TauAnalysis/Core/interface/BinnerBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

BinnerBase::BinnerBase()
  : objValExtractor_(0),
    binning_(0),
    binningService_(0)
{
//--- nothing to be done yet...
}

BinnerBase::BinnerBase(const edm::ParameterSet& cfg)
{
  //std::cout << "<BinnerBase::BinnerBase>:" << std::endl; 

  edm::ParameterSet cfgBinning = cfg.getParameter<edm::ParameterSet>("binning");
  objValExtractor_ = ObjValVectorExtractorPluginFactory::get()->create("MultiObjValExtractor", cfgBinning);

  binning_ = 0;
  
  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;

  edm::ParameterSet cfgBinningService = cfg.getParameter<edm::ParameterSet>("binningService");
  std::string binningService_pluginType = cfgBinningService.getParameter<std::string>("pluginType");
  binningService_ = BinningServicePluginFactory::get()->create(binningService_pluginType, cfgBinningService);
}

BinnerBase::~BinnerBase()
{
  delete objValExtractor_;
  delete binning_;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void BinnerBase::analyze(const edm::Event& evt, const edm::EventSetup& es, double evtWeight) 
{ 
  bin(evt, es, evtWeight); 
}

void BinnerBase::endJob() 
{ 
  if ( binning_ ) {
    binning_->print(std::cout); 

    binningService_->saveBinningResults(dqmDirectory_store_, binning_);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void BinnerBase::bin(const edm::Event& evt, const edm::EventSetup& es, double evtWeight) 
{
  if ( !binning_ ) {
    edm::LogError ("BinnerBase::bin") << " No binning object defined --> skipping !!";
    return;
  }

  std::vector<double> x = (*objValExtractor_)(evt);

  binning_->bin(x, evtWeight);
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(BinnerPluginFactory, "BinnerPluginFactory");


