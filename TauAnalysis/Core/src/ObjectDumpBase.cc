#include "TauAnalysis/Core/interface/ObjectDumpBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"

ObjectDumpBase::ObjectDumpBase(const edm::ParameterSet& cfg)
  : outputStream_(0),
    cfgError_(false)
{
  bool nonStandAlone = ( cfg.exists("nonStandAlone") ) ? cfg.getParameter<bool>("nonStandAlone") : false;
  if ( !nonStandAlone ) outputStream_ = getOutputOptions(cfg, isOutputFile_, cfgError_);
}

ObjectDumpBase::~ObjectDumpBase()
{
//--- close output file
  if ( isOutputFile_ ) delete outputStream_;
}

void ObjectDumpBase::setOutputStream(std::ostream* outputStream)
{ 
  outputStream_ = outputStream; 
}

void ObjectDumpBase::analyze(const edm::Event& evt, const edm::EventSetup& es, double weight)
{
  print(evt, es); 
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(ObjectDumpPluginFactory, "ObjectDumpPluginFactory");

