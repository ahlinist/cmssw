#include "TauAnalysis/RecoTools/plugins/PATTriggerEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerAlgorithm.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

PATTriggerEventSelector::hltAcceptPathType::hltAcceptPathType(const edm::ParameterSet& cfg)
  : hltAcceptEventRange_(0)
{
  hltPathName_ = cfg.getParameter<std::string>("hltAcceptPath");
  if ( cfg.exists("runrange") )
    hltAcceptEventRange_ = new edm::EventRange(cfg.getParameter<edm::EventRange>("runrange"));

  print();
}

PATTriggerEventSelector::hltAcceptPathType::hltAcceptPathType(const std::string& hltPathName)
  : hltPathName_(hltPathName),
    hltAcceptEventRange_(0)
{
  print();
}

PATTriggerEventSelector::hltAcceptPathType::~hltAcceptPathType()
{
  delete hltAcceptEventRange_;
}

bool PATTriggerEventSelector::hltAcceptPathType::isInRange(const edm::Event& evt) const
{
  if ( !hltAcceptEventRange_ ) return true;

  edm::RunNumber_t runNumber = evt.id().run();
  //edm::LuminosityBlockNumber_t lumiSectionNumber = evt.id().luminosityBlock();
  edm::EventNumber_t eventNumber = evt.id().event();
  
  if ( (runNumber > hltAcceptEventRange_->startRun() || 
	(runNumber == hltAcceptEventRange_->startRun() && 
	 // CV: edm::EventRange->startEvent = 1 means that startEvent parameter has been set to 'MIN'
	 (eventNumber >= hltAcceptEventRange_->startEvent() || hltAcceptEventRange_->startEvent() == 1))) &&
       (runNumber < hltAcceptEventRange_->endRun() ||
	(runNumber == hltAcceptEventRange_->endRun() && 
	 // CV: edm::EventRange->endEvent = 0 means that endEvent parameter has been set to 'MAX'
	 (eventNumber <= hltAcceptEventRange_->endEvent() || hltAcceptEventRange_->endEvent() == 0))) ) return true;
  return false;
}

void PATTriggerEventSelector::hltAcceptPathType::print() const
{
  std::cout << "<hltAcceptPathType::print>:" << std::endl;
  std::cout << " hltPathName = " << hltPathName_ << std::endl;
  if ( hltAcceptEventRange_ ) {
    std::cout << " hltAcceptEventRange:" << std::endl;
    std::cout << "  startRun = " << hltAcceptEventRange_->startRun() << "," 
	      << " startEvent = " << hltAcceptEventRange_->startEvent() << std::endl;
    std::cout << "  endRun = " << hltAcceptEventRange_->endRun() << "," 
	      << " endEvent = " << hltAcceptEventRange_->endEvent() << std::endl;
  } 
}

//
//-------------------------------------------------------------------------------
//

PATTriggerEventSelector::PATTriggerEventSelector(const edm::ParameterSet& cfg)
{
  //std::cout << "<PATTriggerEventSelector::PATTriggerEventSelector>:" << std::endl;
  
  src_ = cfg.getParameter<edm::InputTag>("src"); 

  if ( cfg.exists("config") ) {
//--- run-range dependent configuration for data
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgAcceptPaths = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgAcceptPath = cfgAcceptPaths.begin();
	  cfgAcceptPath != cfgAcceptPaths.end(); ++cfgAcceptPath ) {
      hltAcceptPathType* hltAcceptPath = new hltAcceptPathType(*cfgAcceptPath);
      hltAcceptPaths_.push_back(hltAcceptPath);
    }
  } else {
//--- run-range independent configuration for Monte Carlo
    typedef std::vector<std::string> vstring;
    vstring hltAcceptPathNames = cfg.getParameter<vstring>("hltAcceptPaths");
    for ( vstring::const_iterator hltAcceptPathName = hltAcceptPathNames.begin();
	  hltAcceptPathName != hltAcceptPathNames.end(); ++hltAcceptPathName ) {
      hltAcceptPathType* hltAcceptPath = new hltAcceptPathType(*hltAcceptPathName);
      hltAcceptPaths_.push_back(hltAcceptPath);
    }
  }

  maxWarnings_ = cfg.exists("maxWarnings") ? cfg.getParameter<int>("maxWarnings") : 1;
}

PATTriggerEventSelector::~PATTriggerEventSelector()
{
  for ( std::vector<hltAcceptPathType*>::iterator it = hltAcceptPaths_.begin();
	it != hltAcceptPaths_.end(); ++it ) {
    delete (*it);
  }
}

bool PATTriggerEventSelector::operator()(edm::Event& evt, const edm::EventSetup& es) 
{
  edm::Handle<pat::TriggerEvent> patTriggerEvent;
  evt.getByLabel(src_, patTriggerEvent);
  
  for ( std::vector<hltAcceptPathType*>::const_iterator hltAcceptPath = hltAcceptPaths_.begin();
	hltAcceptPath != hltAcceptPaths_.end(); ++hltAcceptPath ) {

    if ( !(*hltAcceptPath)->isInRange(evt) ) continue;
    
//--- check for wildcard character "*" that accepts all events
    if ( (*hltAcceptPath)->hltPathName_ == "*" ) return true;

//--- get pat::TriggerAlgorithm object
//    containing information for specified HLT path
    const pat::TriggerPath* hltPath = patTriggerEvent->path((*hltAcceptPath)->hltPathName_);
    if ( !hltPath ) {
      if ( numWarnings_[(*hltAcceptPath)->hltPathName_] < maxWarnings_ ) {
	edm::LogError("PATTriggerEventSelector::operator()") 
	  << " HLT Trigger path = " << (*hltAcceptPath)->hltPathName_ << " not found in Trigger menu !!";
	std::cout << "available HLT paths:" << std::endl;
	const pat::TriggerPathCollection* paths =  patTriggerEvent->paths();
	for ( pat::TriggerPathCollection::const_iterator path = paths->begin();
	      path != paths->end(); ++path ) {
	  std::cout << " " << path->name() << ":" 
		    << " prescale = " << path->prescale() << std::endl;
	  std::cout << "(modules = " << format_vstring(path->modules()) << ")" << std::endl;
	}
      }
      ++numWarnings_[(*hltAcceptPath)->hltPathName_];
      continue;
    }
    
    if ( hltPath->wasAccept() ) return true;
  }
  
  return false;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATTriggerEventSelector, "PATTriggerEventSelector");
