#include "SusyAnalysis/EventSelector/interface/HLTEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"

HLTEventSelector::HLTEventSelector (const edm::ParameterSet& pset) :
  SusyEventSelector(pset) {
  // input collection
  triggerResults_ = pset.getParameter<edm::InputTag>("triggerResults");
  // trigger path names
  pathNames_ = pset.getParameter< std::vector<std::string> >("pathNames");
  pathNamesConv_.reserve( pathNames_.size() );

  // Add all path names as variables (making sure underscores are removed)
  for ( std::vector<std::string>::const_iterator i=pathNames_.begin();
	i!=pathNames_.end(); ++i )
    {
      std::string pathConv;
      removeUnderscores_( *i, pathConv );
      pathNamesConv_.push_back(pathConv);
      defineVariable( pathConv );
    }

  edm::LogInfo("HLTEventSelector") << "constructed with \n"
				   << "  source     = " << triggerResults_ << "\n"
				   << "  #pathnames = " << pathNames_.size();
}

bool
HLTEventSelector::select (const edm::Event& event) const
{
  // Reset cached variables
  resetVariables();

  // Get the trigger results and check validity
  edm::Handle<edm::TriggerResults> hltHandle;
  event.getByLabel(triggerResults_, hltHandle);
  if ( !hltHandle.isValid() ) {
    edm::LogWarning("HLTEventSelector") << "No trigger results for InputTag " << triggerResults_;
    return false;
  }

  // Get results
  edm::TriggerNames trgNames;
  trgNames.init(*hltHandle);
  bool result(false);

  // Example for OR of all specified triggers
  for ( unsigned int i=0; i<pathNames_.size(); ++i ) {
    // Get index
    unsigned int index = trgNames.triggerIndex(pathNames_[i]);
    if ( index==trgNames.size() ) {
      edm::LogWarning("HLTEventSelector") << "Unknown trigger name " << pathNames_[i];
      continue;
    }
    if ( hltHandle->accept(index) ) {
      LogDebug("HLTEventSelector") << "Event selected by " << pathNames_[i];
      result = true;
      setVariable( pathNamesConv_[i], true );
    } else {
      setVariable( pathNamesConv_[i], false );
    }
  }

  if (!result) LogDebug("HLTEventSelector") << "Event rejected";

  return result;
}


//________________________________________________________________________________________
void
HLTEventSelector::removeUnderscores_( const std::string& iname, std::string& oname ) const {
  oname.reserve(iname.size());
  for ( unsigned int i = 0; i<iname.size(); ++i )
    if ( iname[i] != '_' ) oname.push_back(iname[i]);
}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, HLTEventSelector, "HLTEventSelector");
