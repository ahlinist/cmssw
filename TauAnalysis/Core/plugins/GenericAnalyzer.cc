#include "TauAnalysis/Core/plugins/GenericAnalyzer.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include <algorithm>
#include <limits.h>
#include <iostream>

unsigned GenericAnalyzer::analysisSequenceEntry_filter::filterId_ = 1;

GenericAnalyzer::analysisSequenceEntry::analysisSequenceEntry(const std::string& name)
  : name_(name)
{}

GenericAnalyzer::analysisSequenceEntry::~analysisSequenceEntry()
{
// nothing to be done yet...
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

GenericAnalyzer::analysisSequenceEntry_filter::analysisSequenceEntry_filter(const std::string& name, const std::string& title,
									   const edm::ParameterSet& cfgFilter, int& cfgError)
  : analysisSequenceEntry(name)
{
  std::string filterType = cfgFilter.getParameter<std::string>("pluginType");

  if ( cfgFilter.exists("src_cumulative") &&
       cfgFilter.exists("src_individual") ) {
    edm::ParameterSet cfgFilter_cumulative = cfgFilter;
    cfgFilter_cumulative.addParameter<edm::InputTag>("src", cfgFilter.getParameter<edm::InputTag>("src_cumulative"));
    filterPlugin_cumulative_ = EventSelectorPluginFactory::get()->create(filterType, cfgFilter_cumulative);
    edm::ParameterSet cfgFilter_individual = cfgFilter;
    cfgFilter_individual.addParameter<edm::InputTag>("src", cfgFilter.getParameter<edm::InputTag>("src_individual"));
    filterPlugin_individual_ = EventSelectorPluginFactory::get()->create(filterType, cfgFilter_individual);
  } else {
    filterPlugin_cumulative_ = EventSelectorPluginFactory::get()->create(filterType, cfgFilter);
    filterPlugin_individual_ = EventSelectorPluginFactory::get()->create(filterType, cfgFilter);
  }

  ++filterId_;

  //print();
}

GenericAnalyzer::analysisSequenceEntry_filter::~analysisSequenceEntry_filter()
{
  delete filterPlugin_cumulative_;
  delete filterPlugin_individual_;
}

void GenericAnalyzer::analysisSequenceEntry_filter::print() const
{
  std::cout << "<GenericAnalyzer::analysisSequenceEntry_filter::print>:" << std::endl; 
  std::cout << " filterPlugin_cumulative = " << filterPlugin_cumulative_ << std::endl;
  std::cout << " filterPlugin_individual = " << filterPlugin_individual_ << std::endl;
  std::cout << " filterId = " << filterId_ << std::endl;
}

bool GenericAnalyzer::analysisSequenceEntry_filter::filter_cumulative(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::Event* evt_nonConst = const_cast<edm::Event*>(&evt);
  return (*filterPlugin_cumulative_)(*evt_nonConst, es);
}

bool GenericAnalyzer::analysisSequenceEntry_filter::filter_individual(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::Event* evt_nonConst = const_cast<edm::Event*>(&evt);
  return (*filterPlugin_individual_)(*evt_nonConst, es);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

GenericAnalyzer::analysisSequenceEntry_analyzer::analysisSequenceEntry_analyzer(const std::string& name , 
										const std::list<edm::ParameterSet>& cfgAnalyzers)
  : analysisSequenceEntry(name)
{
  for ( std::list<edm::ParameterSet>::const_iterator cfgAnalyzer = cfgAnalyzers.begin(); 
	cfgAnalyzer != cfgAnalyzers.end(); ++cfgAnalyzer ) {
    std::string analyzerType = cfgAnalyzer->getParameter<std::string>("pluginType");
    AnalyzerPluginBase* analyzer = AnalyzerPluginFactory::get()->create(analyzerType, *cfgAnalyzer);
    analyzerPlugins_.push_back(analyzer);
  }

  //print();
}

GenericAnalyzer::analysisSequenceEntry_analyzer::~analysisSequenceEntry_analyzer()
{
  for ( std::list<AnalyzerPluginBase*>::const_iterator analyzer = analyzerPlugins_.begin();
	analyzer != analyzerPlugins_.end(); ++analyzer ) {
    delete (*analyzer);
  }
}

void GenericAnalyzer::analysisSequenceEntry_analyzer::print() const
{
  std::cout << "<GenericAnalyzer::analysisSequenceEntry_analyzer::print>:" << std::endl;
}

void GenericAnalyzer::analysisSequenceEntry_analyzer::beginJob()
{
  for ( std::list<AnalyzerPluginBase*>::const_iterator analyzer = analyzerPlugins_.begin();
	analyzer != analyzerPlugins_.end(); ++analyzer ) {
    (*analyzer)->beginJob();
  }
}

void GenericAnalyzer::analysisSequenceEntry_analyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  for ( std::list<AnalyzerPluginBase*>::const_iterator analyzer = analyzerPlugins_.begin();
	analyzer != analyzerPlugins_.end(); ++analyzer ) {
    (*analyzer)->analyze(evt, es);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenericAnalyzer::addFilter(const std::string& filterName, const vstring& saveRunEventNumbers)
{
  std::map<std::string, edm::ParameterSet>::const_iterator it = cfgFilters_.find(filterName);
  if ( it != cfgFilters_.end() ) {
    edm::ParameterSet cfgFilter = it->second;
    
    std::string filterTitle = ( cfgFilter.exists("title") ) ? cfgFilter.getParameter<std::string>("title") : filterName;
    
    analysisSequenceEntry_filter* entry = new analysisSequenceEntry_filter(filterName, filterTitle, cfgFilter, cfgError_);
    analysisSequence_.push_back(entry);
  } else {
    edm::LogError("GenericAnalyzer::addFilter") << " Failed to access configuration parameter for filter = " << filterName
						<< " --> skipping !!";
    cfgError_ = 1;
  }
}

std::string capitalizeFirstCharacter(const std::string& input)
{
  std::string output = input;
  if ( output != "" ) std::transform(output.begin(), ++output.begin(), output.begin(), ::toupper);
  return output;
}

std::string dqmSubDirectoryName_filter(const std::string& afterFilterName, const std::string& beforeFilterName)
{
  std::string subDirName;
  if ( afterFilterName != "" ) subDirName.append("after").append(capitalizeFirstCharacter(afterFilterName));
  if ( afterFilterName != "" && beforeFilterName != "" ) subDirName.append("_");
  if ( beforeFilterName != "" ) subDirName.append("before").append(capitalizeFirstCharacter(beforeFilterName));
  return subDirName;
}

void GenericAnalyzer::addAnalyzers(const vstring& analyzerNames, 
				   const std::string& afterFilterName, const std::string& beforeFilterName,
				   const vstring& replaceCommands)
{
  std::list<edm::ParameterSet> cfgAnalyzers;

  for ( vstring::const_iterator analyzerName = analyzerNames.begin();
	analyzerName != analyzerNames.end(); ++analyzerName ) {

    std::map<std::string, edm::ParameterSet>::const_iterator it = cfgAnalyzers_.find(*analyzerName);
    if ( it != cfgAnalyzers_.end() ) {
      edm::ParameterSet cfgAnalyzer = it->second;
      
      for ( vstring::const_iterator replaceCommand = replaceCommands.begin();
	    replaceCommand != replaceCommands.end(); ++replaceCommand ) {
	int errorFlag;
	std::string replaceCommand_noWhiteSpace = replace_string(*replaceCommand, " ", "", 0, UINT_MAX, errorFlag);

	std::string keyword = std::string(*analyzerName).append(".");
	if ( replaceCommand_noWhiteSpace.find(keyword) != std::string::npos ) {
	  std::string replaceCommand_noKeyword = replace_string(replaceCommand_noWhiteSpace, keyword, "", 0, UINT_MAX, errorFlag);

	  size_t posAssignmentOperator = replaceCommand_noKeyword.find("=");
	  if ( posAssignmentOperator == std::string::npos ||
	       !(posAssignmentOperator >= 1 && posAssignmentOperator < (replaceCommand_noKeyword.length() - 1)) ) {
	    edm::LogError("GenericAnalyzer::addAnalyzers") << " Failed to parse replaceCommand = " << (*replaceCommand)
							   << " --> skipping !!";
	    cfgError_ = 1;
	    continue;
	  }
	
	  std::string leftHandSide = std::string(replaceCommand_noKeyword, 0, posAssignmentOperator);
	  std::string rightHandSide = std::string(replaceCommand_noKeyword, posAssignmentOperator + 1);

//--- check that parameter that is to be replaced
//    exists in configuration parameter set 
//    and is defined to be a supported type
//    (so far, only std::string and edm::InputTag are supported types
//     for configuration parameter replacements)
	  if ( cfgAnalyzer.existsAs<std::string>(leftHandSide) ) {
	    cfgAnalyzer.addParameter<std::string>(leftHandSide, rightHandSide);
	  } else if ( cfgAnalyzer.existsAs<edm::InputTag>(leftHandSide) ) {
	    edm::InputTag rightHandSide_inputTag = rightHandSide;
	    cfgAnalyzer.addParameter<edm::InputTag>(leftHandSide, rightHandSide_inputTag);
	  } else {
	    edm::LogError("GenericAnalyzer::addAnalyzers") << " Configuration parameter to be replaced = " << leftHandSide 
							   << " does either not exist or is not of a supported type"
							   << " --> skipping !!";
	    cfgError_ = 1;
	    continue;
	  }

//--- store modified configuration ParameterSet
//    (NOTE: by storing the modified configuration ParameterSet,
//           replace statements become cumulative)
	  cfgAnalyzers_[it->first] = cfgAnalyzer;
	}
      }

      cfgAnalyzer.addParameter<std::string>("afterFilterName", afterFilterName);
      cfgAnalyzer.addParameter<std::string>("beforeFilterName", beforeFilterName);

      std::string dqmDirectory_store = cfgAnalyzer.getParameter<std::string>("dqmDirectory_store");
      std::string dirName = dqmDirectoryName(name_).append(dqmSubDirectoryName_filter(afterFilterName, beforeFilterName));
      std::string dirName_full = dqmDirectoryName(dirName).append(dqmDirectory_store);
      cfgAnalyzer.addParameter<std::string>("dqmDirectory_store", dirName_full);

      cfgAnalyzers.push_back(cfgAnalyzer);
    } else {
      edm::LogError("GenericAnalyzer::addAnalyzers") << " Failed to access configuration parameter for analyzer = " << (*analyzerName)
						     << " --> skipping !!";
      cfgError_ = 1;
      return;
    }
  }

  std::string entryName = dqmDirectoryName(name_).append(dqmSubDirectoryName_filter(afterFilterName, beforeFilterName));
  analysisSequenceEntry_analyzer* entry = new analysisSequenceEntry_analyzer(entryName, cfgAnalyzers);
  analysisSequence_.push_back(entry);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

GenericAnalyzer::GenericAnalyzer(const edm::ParameterSet& cfg)
{
  //std::cout << "<GenericAnalyzer::GenericAnalyzer>:" << std::endl;

  cfgError_ = 0;

  name_ = cfg.getParameter<std::string>("name");

  typedef std::vector<edm::ParameterSet> vParameterSet;

//--- store configuration parameters for filters
  //std::cout << "--> storing configuration parameters for filters..." << std::endl;
  if ( cfg.exists("filters") ) {
    vParameterSet cfgFilters = cfg.getParameter<vParameterSet>("filters");
    for ( vParameterSet::const_iterator cfgFilter = cfgFilters.begin(); 
	  cfgFilter != cfgFilters.end(); ++cfgFilter ) {
      std::string cfgFilterName = cfgFilter->getParameter<std::string>("pluginName");
      cfgFilters_.insert(std::pair<std::string, edm::ParameterSet>(cfgFilterName, *cfgFilter)); 
    }
  }

//--- store configuration parameters for analyzers
  //std::cout << "--> storing configuration parameters for analyzers..." << std::endl;
  if ( cfg.exists("analyzers") ) {
    vParameterSet cfgAnalyzers = cfg.getParameter<vParameterSet>("analyzers");
    for ( vParameterSet::const_iterator cfgAnalyzer = cfgAnalyzers.begin(); 
	  cfgAnalyzer != cfgAnalyzers.end(); ++cfgAnalyzer ) {
      std::string cfgAnalyzerName = cfgAnalyzer->getParameter<std::string>("pluginName");
      cfgAnalyzers_.insert(std::pair<std::string, edm::ParameterSet>(cfgAnalyzerName, *cfgAnalyzer));
    }
  }

//--- configure analysisSequence
  //std::cout << "--> configuring analysisSequence..." << std::endl;
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgAnalysisSequenceEntries = cfg.getParameter<vParameterSet>("analysisSequence");
  std::string lastFilterName = "";
  for ( vParameterSet::const_iterator cfgAnalysisSequenceEntry = cfgAnalysisSequenceEntries.begin();
	cfgAnalysisSequenceEntry != cfgAnalysisSequenceEntries.end(); ++cfgAnalysisSequenceEntry ) {
    std::string filterName = ( cfgAnalysisSequenceEntry->exists("filter") ) ? 
      cfgAnalysisSequenceEntry->getParameter<std::string>("filter") : "";
    vstring analyzerNames = ( cfgAnalysisSequenceEntry->exists("analyzers") ) ? 
      cfgAnalysisSequenceEntry->getParameter<vstring>("analyzers") : vstring();

    if ( filterName == "" && analyzerNames.size() == 0 ) {
      edm::LogError("GenericAnalyzer") << " Either filter or analyzers must be specified for sequenceEntries !!";
      cfgError_ = 1;
      continue;
    }
    
    if ( filterName != "" && analyzerNames.size() != 0 ) {
      edm::LogError("GenericAnalyzer") << " Must not specify filter and analyzers for same sequenceEntry !!";
      cfgError_ = 1;
      continue;
    }

    if ( filterName != "" ) {
      vstring saveRunEventNumbers = cfgAnalysisSequenceEntry->exists("saveRunEventNumbers") ? 
	cfgAnalysisSequenceEntry->getParameter<vstring>("saveRunEventNumbers") : vstring();
      
      addFilter(filterName, saveRunEventNumbers);
      
      lastFilterName = filterName;
    }
    
    if ( analyzerNames.size() != 0 ) {
      std::string nextFilterName = "";
      for ( vParameterSet::const_iterator cfgAnalysisSequenceEntry_nextFilter = cfgAnalysisSequenceEntry;
	    cfgAnalysisSequenceEntry_nextFilter != cfgAnalysisSequenceEntries.end(); ++cfgAnalysisSequenceEntry_nextFilter ) {
	if (  cfgAnalysisSequenceEntry_nextFilter->exists("filter") ) {
	  nextFilterName = cfgAnalysisSequenceEntry_nextFilter->getParameter<std::string>("filter");
	  break;
	}
      }

      vstring replaceCommands = ( cfgAnalysisSequenceEntry->exists("replace") ) ? 
	cfgAnalysisSequenceEntry->getParameter<vstring>("replace") : vstring();
      //std::cout << " replaceCommands = " << format_vstring(replaceCommands) << std::endl;
      
      addAnalyzers(analyzerNames, lastFilterName, nextFilterName, replaceCommands);
    }
  }

  if ( cfgError_ ) return;

//--- configure filter statistics service
//    and create filter statistics table
  filterStatisticsService_ = new FilterStatisticsService();
  filterStatisticsService_dqmDirectory_ = dqmDirectoryName(name_).append("FilterStatistics");

  edm::ParameterSet cfgFilterStatisticsTable;
  cfgFilterStatisticsTable.addParameter<std::string>("name", std::string(name_).append("-").append("FilterStatisticsService"));
  vParameterSet filterStatisticsTable_config;
  for ( vParameterSet::const_iterator cfgAnalysisSequenceEntry = cfgAnalysisSequenceEntries.begin();
	cfgAnalysisSequenceEntry != cfgAnalysisSequenceEntries.end(); ++cfgAnalysisSequenceEntry ) {
    if ( cfgAnalysisSequenceEntry->exists("filter") ) {
      std::string filterName = cfgAnalysisSequenceEntry->getParameter<std::string>("filter");
      std::string filterTitle = ( cfgAnalysisSequenceEntry->exists("title") ) ? 
	cfgAnalysisSequenceEntry->getParameter<std::string>("title") : filterName;
          
      edm::ParameterSet config;
      config.addParameter<std::string>("filterName", filterName);
      config.addParameter<std::string>("filterTitle", filterTitle);
      
      filterStatisticsTable_config.push_back(config);
    }
  }
  cfgFilterStatisticsTable.addParameter<vParameterSet>("config", filterStatisticsTable_config);
  filterStatisticsTable_ = filterStatisticsService_->createFilterStatisticsTable(cfgFilterStatisticsTable);
 
//--- configure run & event number service
  edm::ParameterSet cfgRunEventNumberService;
  cfgRunEventNumberService.addParameter<std::string>("name", std::string(name_).append("-").append("RunEventNumberService"));
  cfgRunEventNumberService.addParameter<std::string>("dqmDirectory_store", dqmDirectoryName(name_).append("FilterStatistics"));
  vParameterSet runEventNumberService_config;
  for ( vParameterSet::const_iterator cfgAnalysisSequenceEntry = cfgAnalysisSequenceEntries.begin();
	cfgAnalysisSequenceEntry != cfgAnalysisSequenceEntries.end(); ++cfgAnalysisSequenceEntry ) {
    if ( cfgAnalysisSequenceEntry->exists("filter") ) {
      std::string filterName = cfgAnalysisSequenceEntry->getParameter<std::string>("filter");
      vstring saveRunEventNumbers = cfgAnalysisSequenceEntry->exists("saveRunEventNumbers") ? 
	cfgAnalysisSequenceEntry->getParameter<vstring>("saveRunEventNumbers") : vstring();
      
      edm::ParameterSet config;
      config.addParameter<std::string>("filterName", filterName);
      config.addParameter<vstring>("saveRunEventNumbers", saveRunEventNumbers);
      
      runEventNumberService_config.push_back(config);
    }
  }
  cfgRunEventNumberService.addParameter<vParameterSet>("config", runEventNumberService_config);
  runEventNumberService_ = new RunEventNumberService(cfgRunEventNumberService);

//--- configure eventDumps
  if ( cfg.exists("eventDumps") ) {
    vParameterSet cfgEventDumps = cfg.getParameter<vParameterSet>("eventDumps");
    for ( vParameterSet::const_iterator cfgEventDump = cfgEventDumps.begin(); 
	  cfgEventDump != cfgEventDumps.end(); ++cfgEventDump ) {
      std::string eventDumpName = cfgEventDump->getParameter<std::string>("pluginName");
      std::string eventDumpType = cfgEventDump->getParameter<std::string>("pluginType");
 
      EventDumpBase* entry = EventDumpPluginFactory::get()->create(eventDumpType, *cfgEventDump);
      eventDumps_.push_back(entry);
    }
  }
}

GenericAnalyzer::~GenericAnalyzer()
{
  for ( std::list<analysisSequenceEntry*>::const_iterator entry = analysisSequence_.begin();
	entry != analysisSequence_.end(); ++entry ) {
    delete (*entry);
  }

  delete filterStatisticsService_;
  delete filterStatisticsTable_;
  delete runEventNumberService_;
}

void GenericAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{  
  //std::cout << "<GenericAnalyzer::analyze>:" << std::endl; 

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError("GenericAnalyzer::analyze") << " Error in Configuration ParameterSet --> skipping !!";
    return;
  }

  double eventWeight = 1.; // event weights not implemented yet...

//--- call analyze method of each analyzerPlugin
//    (fill histograms, compute binning results,...)
  typedef std::vector<std::pair<std::string, bool> > filterResults_type;
  filterResults_type filterResults_cumulative;
  bool previousFiltersPassed = true;
  filterResults_type filterResults_individual;
  for ( std::list<analysisSequenceEntry*>::iterator entry = analysisSequence_.begin();
	entry != analysisSequence_.end(); ++entry ) {

    if ( (*entry)->type() == analysisSequenceEntry::kFilter ) {
      bool filterPassed_cumulative = (*entry)->filter_cumulative(evt, es);
      
      filterResults_cumulative.push_back(std::pair<std::string, bool>((*entry)->name_, filterPassed_cumulative));
      
      if ( !filterPassed_cumulative ) previousFiltersPassed = false;

      bool filterPassed_individual = (*entry)->filter_individual(evt, es);
      
      filterResults_individual.push_back(std::pair<std::string, bool>((*entry)->name_, filterPassed_individual));
    }

    if ( (*entry)->type() == analysisSequenceEntry::kAnalyzer ) {
      if ( previousFiltersPassed ) (*entry)->analyze(evt, es);
    }
  }

//--- update filter statistics table
  filterStatisticsTable_->update(filterResults_cumulative, filterResults_individual, eventWeight);

//--- save run & event numbers
  runEventNumberService_->update(evt.id().run(), evt.id().event(), evt.luminosityBlock(),
				 filterResults_cumulative, filterResults_individual, eventWeight);

//--- if requested, dump event information 
  for ( std::list<EventDumpBase*>::const_iterator it = eventDumps_.begin();
	it != eventDumps_.end(); ++it ) {
    EventDumpBase* eventDump = (*it);
    eventDump->analyze(evt, es, filterResults_cumulative, filterResults_individual, eventWeight);
  }
}

void GenericAnalyzer::beginJob(const edm::EventSetup&)
{
  //std::cout << "<GenericAnalyzer::beginJob>:" << std::endl;

  benchmark_.Start(std::string("GenericAnalyzer").append("-").append(name_).data());

//--- call beginJob method of each EDFilter/EDAnalyzer
  for ( std::list<analysisSequenceEntry*>::iterator entry = analysisSequence_.begin();
	entry != analysisSequence_.end(); ++entry ) {
    (*entry)->beginJob();
  }
}

void GenericAnalyzer::endJob()
{
  //std::cout << "<GenericAnalyzer::endJob>:" << std::endl;

  if ( cfgError_ ) return;

//--- call endJob method of each EDFilter/EDAnalyzer
  for ( std::list<analysisSequenceEntry*>::iterator entry = analysisSequence_.begin();
	entry != analysisSequence_.end(); ++entry ) {
    (*entry)->endJob();
  }

  benchmark_.Show(std::string("GenericAnalyzer").append("-").append(name_).data());

//--- print filter statistics (cut-flow) table
//    and save results in DQM file
  filterStatisticsTable_->print(std::cout);
  filterStatisticsService_->saveFilterStatisticsTable(filterStatisticsService_dqmDirectory_, filterStatisticsTable_);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(GenericAnalyzer);

