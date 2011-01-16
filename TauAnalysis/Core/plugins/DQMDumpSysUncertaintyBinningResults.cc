#include "TauAnalysis/Core/plugins/DQMDumpSysUncertaintyBinningResults.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/SysUncertaintyService.h"

#include "TauAnalysis/Core/interface/binningAuxFunctions.h"
#include "TauAnalysis/Core/interface/sysUncertaintyAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"
  
#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TMath.h>

typedef std::vector<std::string> vstring;

enum { kUndefined, kSimple, kPDF };

DQMDumpSysUncertaintyBinningResults::sysUncertaintyEntryType::sysUncertaintyEntryType(const edm::ParameterSet& cfg)
  : method_(kSimple),
    cfgError_(0)
{
  //std::cout << "<sysUncertaintyEntryType::sysUncertaintyEntryType>:" << std::endl;

  sysCentralValue_ = ( cfg.exists("sysCentralValue") ) ?
    cfg.getParameter<std::string>("sysCentralValue") : SysUncertaintyService::getNameCentralValue();
  //std::cout << " sysCentralValue = " << sysCentralValue_ << std::endl;

  vstring cfgSystematics = cfg.getParameter<vstring>("sysNames");
  for ( vstring::const_iterator sysName = cfgSystematics.begin();
	sysName != cfgSystematics.end(); ++sysName ) {
    vstring sysNames_expanded = expandSysName(*sysName);
    for ( vstring::const_iterator sysName_expanded = sysNames_expanded.begin();
	  sysName_expanded != sysNames_expanded.end(); ++sysName_expanded ) {
      if ( (*sysName_expanded) != sysCentralValue_ ) sysNames_.push_back(*sysName_expanded);
    }
  }

  //std::cout << " sysNames = " << format_vstring(sysNames_) << std::endl;

  sysTitle_ = cfg.getParameter<std::string>("sysTitle");
  //std::cout << " sysTitle = " << sysTitle_ << std::endl;

  if ( cfg.exists("method") ) {
    std::string method_string = cfg.getParameter<std::string>("method");
    //std::cout << " method = " << method_string << std::endl;
    
    if ( method_string == "simple" ) method_ = kSimple;
    else if ( method_string == "pdf" ) method_ = kPDF;
    else {
      edm::LogError("sysUncertaintyEntryType") << " Invalid method Parameter = " << method_string << " !!";
      method_ = kUndefined;
      cfgError_ = 1;
    }
  }

  binningServiceType_ = cfg.getParameter<std::string>("pluginType");
  //std::cout << " binningServiceType = " << binningServiceType_ << std::endl;
}

DQMDumpSysUncertaintyBinningResults::sysUncertaintyEntryType::~sysUncertaintyEntryType()
{
  for ( std::map<std::string, BinningBase*>::iterator it = binningPlugins_.begin();
	it != binningPlugins_.end(); ++it ) {
    delete it->second;
  }
}

void DQMDumpSysUncertaintyBinningResults::sysUncertaintyEntryType::loadBinningResults(const std::string& dqmDirectory)
{
  //std::cout << "<sysUncertaintyEntryType::loadBinningResults>:" << std::endl;

  vstring binningPluginNames = sysNames_;
  binningPluginNames.push_back(sysCentralValue_);

  for ( vstring::const_iterator binningPluginName = binningPluginNames.begin();
	binningPluginName != binningPluginNames.end(); ++binningPluginName ) {
    edm::ParameterSet cfgBinningService;
    BinningServiceBase* binningService = BinningServicePluginFactory::get()->create(binningServiceType_, cfgBinningService);

    std::string dqmDirectory_systematic = dqmDirectoryName(dqmDirectory).append(*binningPluginName);
    //std::cout << "dqmDirectory_systematic = " << dqmDirectory_systematic << std::endl;

    binningPlugins_[*binningPluginName] = binningService->loadBinningResults(dqmDirectory_systematic);
    
    delete binningService;
  }
}

binResultType getBinResult(const std::vector<binResultType>& binResults, const std::string& resultTypeName)
{
  for ( std::vector<binResultType>::const_iterator binResult = binResults.begin();
	binResult != binResults.end(); ++binResult ) {
    if ( binResult->name_  == resultTypeName ) return (*binResult);
  }
  
  edm::LogError ("getBinResult") 
    << " No binning result with name = " << resultTypeName << " found !!";
  return binResultType(0., 0., "undefined");
}

std::map<std::string, binResultType> 
DQMDumpSysUncertaintyBinningResults::sysUncertaintyEntryType::getBinResults_systematic(unsigned binNumber, const std::string& resultTypeName) const
{
  std::map<std::string, binResultType> binResults_systematic;
  
  for ( std::map<std::string, BinningBase*>::const_iterator binningPlugin = binningPlugins_.begin();
	binningPlugin != binningPlugins_.end(); ++binningPlugin ) {
    if ( binningPlugin->first == SysUncertaintyService::getNameCentralValue() ||
	 binningPlugin->first == sysCentralValue_                            ) continue;

    binResultType binResult_systematic = getBinResult(binningPlugin->second->getBinResults(binNumber), resultTypeName);

    binResults_systematic.insert(std::pair<std::string, binResultType>(binningPlugin->first, binResult_systematic));
  }

  return binResults_systematic;
}
 
std::map<std::string, binResultType> 
DQMDumpSysUncertaintyBinningResults::sysUncertaintyEntryType::getBinResultsSum_systematic(const std::string& resultTypeName) const
{
  std::map<std::string, binResultType> binResultsSum_systematic;

  for ( std::map<std::string, BinningBase*>::const_iterator binningPlugin = binningPlugins_.begin();
	binningPlugin != binningPlugins_.end(); ++binningPlugin ) {
    if ( binningPlugin->first == SysUncertaintyService::getNameCentralValue() ||
	 binningPlugin->first == sysCentralValue_                            ) continue;

    binResultType binResultSum_systematic = getBinResult(binningPlugin->second->getBinResultsSum(), resultTypeName);

    binResultsSum_systematic.insert(std::pair<std::string, binResultType>(binningPlugin->first, binResultSum_systematic));
  }

  return binResultsSum_systematic;
}

binResultType
DQMDumpSysUncertaintyBinningResults::sysUncertaintyEntryType::getBinResults_sysCentralValue(unsigned binNumber, const std::string& resultTypeName) const
{
  std::map<std::string, BinningBase*>::const_iterator binningPlugin_sysCentralValue 
    = binningPlugins_.find(sysCentralValue_);
 
  if ( binningPlugin_sysCentralValue == binningPlugins_.end() ) {
    edm::LogError ("getBinResults_sysCentralValue") 
      << " No binning results defined for sys. central value = " << sysCentralValue_ << " !!";
    return binResultType(0., 0., "undefined");
  }

  return getBinResult(binningPlugin_sysCentralValue->second->getBinResults(binNumber), resultTypeName);
}

binResultType 
DQMDumpSysUncertaintyBinningResults::sysUncertaintyEntryType::getBinResultsSum_sysCentralValue(const std::string& resultTypeName) const
{
  std::map<std::string, BinningBase*>::const_iterator binningPlugin_sysCentralValue 
    = binningPlugins_.find(sysCentralValue_);
 
  if ( binningPlugin_sysCentralValue == binningPlugins_.end() ) {
    edm::LogError ("getBinResults_sysCentralValue") 
      << " No binning results defined for sys. central value = " << sysCentralValue_ << " !!";
    return binResultType(0., 0., "undefined");
  }

  return getBinResult(binningPlugin_sysCentralValue->second->getBinResultsSum(), resultTypeName);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMDumpSysUncertaintyBinningResults::processEntryType::processEntryType(const std::string& processName, const std::string& dqmDirectory)
  : processName_(processName), 
    dqmDirectory_(dqmDirectory)
{
//--- nothing to be done yet...
}

DQMDumpSysUncertaintyBinningResults::processEntryType::~processEntryType()
{
//--- nothing to be done yet...
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMDumpSysUncertaintyBinningResults::DQMDumpSysUncertaintyBinningResults(const edm::ParameterSet& cfg)
  : cfgError_(0)
{
  //std::cout << "<DQMDumpSysUncertaintyBinningResults::DQMDumpSysUncertaintyBinningResults>:" << std::endl;

  edm::ParameterSet cfgProcessEntries = cfg.getParameter<edm::ParameterSet>("dqmDirectories");
  vstring processNames = cfgProcessEntries.getParameterNamesForType<std::string>();
  for ( vstring::const_iterator processName = processNames.begin(); 
	processName != processNames.end(); ++processName ) {
    std::string dqmDirectory = cfgProcessEntries.getParameter<std::string>(*processName);
    
    processEntries_.push_back(processEntryType(*processName, dqmDirectory));
  }
  
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgSysUncertainties = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfgSysUncertainty = cfgSysUncertainties.begin();
	cfgSysUncertainty != cfgSysUncertainties.end(); ++cfgSysUncertainty ) {
    for ( std::vector<processEntryType>::iterator processEntry = processEntries_.begin();
	  processEntry != processEntries_.end(); ++processEntry ) {
      sysUncertaintyEntryType sysUncertaintyEntry(*cfgSysUncertainty);
      if ( sysUncertaintyEntry.cfgError_ ) cfgError_ = 1;
      processEntry->sysUncertaintyEntries_.push_back(sysUncertaintyEntry);
    }
  }

  resultTypeNames_ = cfg.getParameter<vstring>("resultTypes");
  
  if ( processEntries_.size() == 0 ) {
    edm::LogError("DQMDumpSysUncertaintyBinningResults") << " Configuration Parameter dqmDirectories contains no Entries --> skipping !!";
    cfgError_ = 1;
  }
}

DQMDumpSysUncertaintyBinningResults::~DQMDumpSysUncertaintyBinningResults() 
{
//--- nothing to be done yet...
}

void DQMDumpSysUncertaintyBinningResults::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

bool containsSysName(const std::vector<std::string>& sysNames_skip, const std::string& sysName)
{
  for ( std::vector<std::string>::const_iterator sysName_skip = sysNames_skip.begin(); 
	sysName_skip != sysNames_skip.end(); ++sysName_skip ) {
    if ( sysName == (*sysName_skip) ) return true;
  }

  return false;
}

double getSysShift(const std::map<std::string, binResultType>& binResults_systematic, const std::string& sysName, double binCentralValue)
{
  //std::cout << "<getSysShift>:" << std::endl;
  //std::cout << " sysName = " << sysName << std::endl;
  //std::cout << " binCentralValue = " << binCentralValue << std::endl;

  std::map<std::string, binResultType>::const_iterator binResult_systematic = binResults_systematic.find(sysName);
 
  if ( binResult_systematic == binResults_systematic.end() ) {
    edm::LogError ("getSysShift") 
      << " No binning results defined for systematic = " << sysName << " !!";
    return 0.;
  }

  double binShiftedValue = binResult_systematic->second.binContent_;
  //std::cout << " binShiftedValue = " << binShiftedValue << std::endl;

  double sysShift = ( binCentralValue != 0. ) ? (binShiftedValue - binCentralValue)/binCentralValue : 0.;
  //std::cout << " sysShift = " << sysShift << std::endl;

  return sysShift;
}

void printBinResult(const std::string& sysTitle, const binResultType& binResult_sysCentralValue,
		    const std::map<std::string, binResultType>& binResults_systematic, int method)
{
  static TPRegexp regexpParser_bidirectional_entry("[[:alnum:]]+(Up|Down)");
  static TPRegexp regexpParser_bidirectional_name("([[:alnum:]]+)(Up|Down)");
  static TPRegexp regexpParser_array_entry("[[:alnum:]]+\\([[:digit:]]+\\)");
  static TPRegexp regexpParser_array_name("([[:alnum:]]+)\\([[:digit:]]+\\)");

  vstring sysNames_skip;

  for ( std::map<std::string, binResultType>::const_iterator binResult_systematic = binResults_systematic.begin();
	binResult_systematic != binResults_systematic.end(); ++binResult_systematic ) {
    const std::string sysName = binResult_systematic->first;
    //std::cout << "sysName = " << (*sysName) << std::endl;

    TString sysName_tstring = sysName.data();
    
    bool parseError = false;

    double binCentralValue = binResult_sysCentralValue.binContent_;    
    
    if ( regexpParser_bidirectional_entry.Match(sysName_tstring) >= 1 ) {
      TObjArray* subStrings = regexpParser_bidirectional_name.MatchS(sysName_tstring);
      
      if ( subStrings->GetEntries() == 3 ) {
	std::string sysName_bidirectional = ((TObjString*)subStrings->At(1))->GetString().Data();
	
	if ( containsSysName(sysNames_skip, sysName_bidirectional) ) continue;
	      
	double sysShift_up = getSysShift(binResults_systematic, std::string(sysName_bidirectional).append("Up"), binCentralValue);
	double sysShift_down = getSysShift(binResults_systematic, std::string(sysName_bidirectional).append("Down"), binCentralValue);
	
	std::cout << " " << std::setw(20) << sysTitle << ":"
		  << " up = " << std::setprecision(4) << std::fixed << sysShift_up*100. << "%,"
		  << " down = " << std::setprecision(4) << std::fixed << sysShift_down*100. << "%" << std::endl;
	
	sysNames_skip.push_back(sysName_bidirectional);
      } else {
	parseError = true;
      }
    } else if ( regexpParser_array_entry.Match(sysName_tstring) == 1 ){
      TObjArray* subStrings = regexpParser_array_name.MatchS(sysName_tstring);
      
      if ( subStrings->GetEntries() == 2 ) {
	std::string sysName_array = ((TObjString*)subStrings->At(1))->GetString().Data();
	
	if ( containsSysName(sysNames_skip, sysName_array) ) continue;
	
	TPRegexp regexpParser_element_entry(std::string(sysName_array).append("\\([[:digit:]]+\\)").data());

	vstring sysNames;
	for ( std::map<std::string, binResultType>::const_iterator binResult_systematic = binResults_systematic.begin();
	      binResult_systematic != binResults_systematic.end(); ++binResult_systematic ) {
	  if ( regexpParser_element_entry.Match(TString(binResult_systematic->first.data())) == 1 ) {
	    sysNames.push_back(binResult_systematic->first);
	  } else {
	    edm::LogError ("printBinResult") 
	      << " Invalid sysName = " << binResult_systematic->first << " within Array !!";
	    continue;
	  }
	}

	if ( method == kSimple ) {
//--- simply sum in quadrature difference of all array elements
//    with respect to central value

	  double sysShiftsSum2 = 0.;

	  for ( vstring::const_iterator sysName_element = sysNames.begin();
		sysName_element != sysNames.end(); ++sysName_element ) {
	    double sysShift_element = getSysShift(binResults_systematic, *sysName_element, binCentralValue);
	    sysShiftsSum2 += (sysShift_element*sysShift_element);
	  }
	
	  std::cout << " " << std::setw(20) << sysTitle << ":"
	  	    << " " << std::setprecision(4) << std::fixed << TMath::Sqrt(sysShiftsSum2)*100. << "%" << std::endl;
	  
	} else if ( method == kPDF ) {
//--- estimation of (PDF) uncertainties
//    according to "Master Equations" in hep-ph/0605240

	  double sysShiftsSum2_plus = 0.;
	  double sysShiftsSum2_minus = 0.;

//--- check that sysNames can be paired
	  unsigned numSysNames = sysNames.size();
	  if ( (numSysNames % 2) != 0 ) {
	    edm::LogError ("printBinResult") 
	      << " Invalid list of sysNames = " << format_vstring(sysNames) << "; failed to find Pairs !!";
	    continue;
	  }
	  
	  unsigned numSysNamePairs = (numSysNames / 2);
	  for ( unsigned iSysNamePair = 0; iSysNamePair < numSysNamePairs; ++iSysNamePair ) {
	    const std::string sysName_up = sysNames[iSysNamePair];
	    double sysShift_up = getSysShift(binResults_systematic, sysName_up, binCentralValue);
	    
	    const std::string sysName_down = sysNames[iSysNamePair + 1];
	    double sysShift_down = getSysShift(binResults_systematic, sysName_down, binCentralValue);
	    
	    double sysShift_plus = TMath::Max(sysShift_up, sysShift_down);
	    if ( sysShift_plus > 0. ) sysShiftsSum2_plus += sysShift_plus*sysShift_plus;
	    
	    double sysShift_minus = TMath::Min(sysShift_up, sysShift_down);
	    if ( sysShift_minus < 0. ) sysShiftsSum2_minus += sysShift_minus*sysShift_minus;
	  }
	  
	  std::cout << " " << std::setw(20) << sysTitle << ":"
		    << " plus " << std::setprecision(4) << std::fixed << TMath::Sqrt(sysShiftsSum2_plus)*100. << "%,"
		    << " minus " << std::setprecision(4) << std::fixed << TMath::Sqrt(sysShiftsSum2_minus)*100. << "%" 
		    << " (PDF set central value = " << binCentralValue << ")" << std::endl;
	}

	sysNames_skip.push_back(sysName_array);
      } else {
	parseError = true;
      }
    } else {
      double sysShift = getSysShift(binResults_systematic, sysName, binCentralValue);
	
      std::cout << " " << std::setw(20) << sysName << ":"
		<< " " << std::setprecision(4) << std::fixed << sysShift*100. << "%" << std::endl;
    }
    
    if ( parseError ) {
      edm::LogError ("printBinResult") << " Failed to decode sysName = " << sysName << " !!";
    }
  }
}

void DQMDumpSysUncertaintyBinningResults::endJob()
{
  //std::cout << "<DQMDumpSysUncertaintyBinningResults::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endjob") 
      << " Error in Configuration ParameterSet --> binning results will NOT be printed-out !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") 
      << " Failed to access dqmStore --> binning results will NOT be printed-out !!";
    return;
  }

//--- load objects created by SysUncertaintyBinner class from DQM directories
  for ( std::vector<processEntryType>::iterator processEntry = processEntries_.begin();
	processEntry != processEntries_.end(); ++processEntry ) {
    for ( std::vector<sysUncertaintyEntryType>::iterator sysUncertaintyEntry = processEntry->sysUncertaintyEntries_.begin();
	  sysUncertaintyEntry != processEntry->sysUncertaintyEntries_.end(); ++sysUncertaintyEntry ) {
      sysUncertaintyEntry->loadBinningResults(processEntry->dqmDirectory_);
    }
  }

//--- print objects created by SysUncertaintyBinner class
  for ( std::vector<processEntryType>::const_iterator processEntry = processEntries_.begin();
	processEntry != processEntries_.end(); ++processEntry ) {
    for ( vstring::const_iterator resultTypeName = resultTypeNames_.begin();
	  resultTypeName != resultTypeNames_.end(); ++resultTypeName ) {

      std::cout << "Systematic Uncertainties on resultTypeName = " << (*resultTypeName) 
		<< " for process = " << processEntry->processName_ << ":" << std::endl;

//--- get binning from first systematic uncertainty;
//    assume that same binning has been used for all systematic uncertainties
      std::vector<sysUncertaintyEntryType>::const_iterator sysUncertaintyEntry_ref = processEntry->sysUncertaintyEntries_.begin();
      if ( sysUncertaintyEntry_ref != processEntry->sysUncertaintyEntries_.end() ) {
	std::map<std::string, BinningBase*>::const_iterator binningCentralValue_ref = 
	  sysUncertaintyEntry_ref->binningPlugins_.find(sysUncertaintyEntry_ref->sysCentralValue_);

	if ( binningCentralValue_ref == sysUncertaintyEntry_ref->binningPlugins_.end() ) {
	  edm::LogError ("endJob") 
	    << " Failed to find reference central value = " << sysUncertaintyEntry_ref->sysCentralValue_ << " !!";
	  continue;
	}
	
	unsigned numBins = binningCentralValue_ref->second->binGrid()->numBins();
	if ( numBins > 1 ) {
	  for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {
//--- print bin-center position
	    printBinCenterPosition(std::cout, binningCentralValue_ref->second->binGrid(), iBin);
	    std::cout << std::endl;

//--- print systematic uncertainties on number of events in bin
	    for ( std::vector<sysUncertaintyEntryType>::const_iterator sysUncertaintyEntry = processEntry->sysUncertaintyEntries_.begin();
		  sysUncertaintyEntry != processEntry->sysUncertaintyEntries_.end(); ++sysUncertaintyEntry ) {
	      binResultType binResult_sysCentralValue = sysUncertaintyEntry->getBinResults_sysCentralValue(iBin, *resultTypeName);
	      std::map<std::string, binResultType> binResults_systematic 
		= sysUncertaintyEntry->getBinResults_systematic(iBin, *resultTypeName);
	      printBinResult(sysUncertaintyEntry->sysTitle_, binResult_sysCentralValue, binResults_systematic, sysUncertaintyEntry->method_);
	    }
	  }
	}
	
//--- print systematic uncertainties on total number of events
//    (sum of all bins)
	std::cout << " sum: " << std::endl;
	
	for ( std::vector<sysUncertaintyEntryType>::const_iterator sysUncertaintyEntry = processEntry->sysUncertaintyEntries_.begin();
	      sysUncertaintyEntry != processEntry->sysUncertaintyEntries_.end(); ++sysUncertaintyEntry ) {
	  binResultType binResultSum_sysCentralValue = sysUncertaintyEntry->getBinResultsSum_sysCentralValue(*resultTypeName);
	  std::map<std::string, binResultType> binResultsSum_systematic 
	    = sysUncertaintyEntry->getBinResultsSum_systematic(*resultTypeName);
	  printBinResult(sysUncertaintyEntry->sysTitle_, binResultSum_sysCentralValue, binResultsSum_systematic, sysUncertaintyEntry->method_);
	}
      }
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMDumpSysUncertaintyBinningResults);
