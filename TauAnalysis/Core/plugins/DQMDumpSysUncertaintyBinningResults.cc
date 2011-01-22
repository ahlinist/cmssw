#include "TauAnalysis/Core/plugins/DQMDumpSysUncertaintyBinningResults.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/SysUncertaintyService.h"
#include "TauAnalysis/Core/interface/BinGrid.h"

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

using namespace DQMDumpSysUncertaintyBinningResults_namespace;

DQMDumpSysUncertaintyBinningResults::sysUncertaintyBaseType::sysUncertaintyBaseType(const edm::ParameterSet& cfg)
  : sysTitle_(cfg.getParameter<std::string>("sysTitle")),
    binningServiceType_(cfg.getParameter<std::string>("pluginType")),
    cfgError_(0)
{
  std::cout << "<sysUncertaintyBaseType::sysUncertaintyBaseType>:" << std::endl;
  std::cout << " sysTitle = " << sysTitle_ << std::endl;
  std::cout << " binningServiceType = " << binningServiceType_ << std::endl;
}

DQMDumpSysUncertaintyBinningResults::sysUncertaintyBaseType::~sysUncertaintyBaseType()
{
  for ( std::map<std::string, BinningBase*>::iterator it = binningPlugins_.begin();
	it != binningPlugins_.end(); ++it ) {
    delete it->second;
  }
}

void DQMDumpSysUncertaintyBinningResults::sysUncertaintyBaseType::loadBinningResults(const std::string& dqmDirectory)
{
  //std::cout << "<sysUncertaintyBaseType::loadBinningResults>:" << std::endl;

  for ( vstring::const_iterator binningPluginName = binningPluginNames_.begin();
	binningPluginName != binningPluginNames_.end(); ++binningPluginName ) {
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

binResultsMapppingType
DQMDumpSysUncertaintyBinningResults::sysUncertaintyBaseType::getBinResults_systematic(
  const std::vector<std::string>& sysNames, unsigned binNumber, const std::string& resultTypeName) const
{
  binResultsMapppingType binResults_systematic;
  
  for ( vstring::const_iterator sysName = sysNames.begin();
	sysName != sysNames.end(); ++sysName ) {
    std::map<std::string, BinningBase*>::const_iterator binningPlugin_systematic = binningPlugins_.find(*sysName);
    assert(binningPlugin_systematic != binningPlugins_.end());

    binResultType binResult_systematic = getBinResult(binningPlugin_systematic->second->getBinResults(binNumber), resultTypeName);

    binResults_systematic.push_back(std::pair<std::string, binResultType>(*sysName, binResult_systematic));
  }

  return binResults_systematic;
}
 
binResultsMapppingType
DQMDumpSysUncertaintyBinningResults::sysUncertaintyBaseType::getBinResultsSum_systematic(
  const std::vector<std::string>& sysNames, const std::string& resultTypeName) const
{
  binResultsMapppingType binResultsSum_systematic;

  for ( vstring::const_iterator sysName = sysNames.begin();
	sysName != sysNames.end(); ++sysName ) {
    std::map<std::string, BinningBase*>::const_iterator binningPlugin_systematic = binningPlugins_.find(*sysName);
    assert(binningPlugin_systematic != binningPlugins_.end());

    binResultType binResultSum_systematic = getBinResult(binningPlugin_systematic->second->getBinResultsSum(), resultTypeName);

    binResultsSum_systematic.push_back(std::pair<std::string, binResultType>(*sysName, binResultSum_systematic));
  }

  return binResultsSum_systematic;
}

binResultType
DQMDumpSysUncertaintyBinningResults::sysUncertaintyBaseType::getBinResults_sysCentralValue(
  const std::string& sysCentralValue, unsigned binNumber, const std::string& resultTypeName) const
{
  std::map<std::string, BinningBase*>::const_iterator binningPlugin_sysCentralValue = binningPlugins_.find(sysCentralValue);
  assert(binningPlugin_sysCentralValue != binningPlugins_.end());

  return getBinResult(binningPlugin_sysCentralValue->second->getBinResults(binNumber), resultTypeName);
}

binResultType 
DQMDumpSysUncertaintyBinningResults::sysUncertaintyBaseType::getBinResultsSum_sysCentralValue(
  const std::string& sysCentralValue, const std::string& resultTypeName) const
{
  std::map<std::string, BinningBase*>::const_iterator binningPlugin_sysCentralValue = binningPlugins_.find(sysCentralValue);
  assert(binningPlugin_sysCentralValue != binningPlugins_.end());

  return getBinResult(binningPlugin_sysCentralValue->second->getBinResultsSum(), resultTypeName);
}

vstring expandSysNamesExclCentralValue(const vstring& cfgSystematics, const vstring& sysCentralValues)
{
  vstring retVal;

  for ( vstring::const_iterator sysName = cfgSystematics.begin();
	sysName != cfgSystematics.end(); ++sysName ) {
    vstring sysNames_expanded = expandSysName(*sysName);
    for ( vstring::const_iterator sysName_expanded = sysNames_expanded.begin();
	  sysName_expanded != sysNames_expanded.end(); ++sysName_expanded ) {
      bool isSysCentralValue = false;
      for ( vstring::const_iterator sysCentralValue = sysCentralValues.begin();
	    sysCentralValue != sysCentralValues.end(); ++sysCentralValue ) {
	if ( (*sysName_expanded) == (*sysCentralValue) ) isSysCentralValue = true;
      }
      if ( !isSysCentralValue ) retVal.push_back(*sysName_expanded);
    }
  }

  return retVal;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMDumpSysUncertaintyBinningResults::simpleUncertaintyType::simpleUncertaintyType(const edm::ParameterSet& cfg)
  : sysUncertaintyBaseType(cfg)
{
  sysCentralValue_ = cfg.getParameter<std::string>("sysCentralValue");
  vstring sysCentralValues;
  sysCentralValues.push_back(sysCentralValue_);
  //std::cout << " sysCentralValue = " << sysCentralValue_ << std::endl;

  vstring cfgSystematics = cfg.getParameter<vstring>("sysNames");
  sysNames_ = expandSysNamesExclCentralValue(cfgSystematics, sysCentralValues);
  //std::cout << " sysNames = " << format_vstring(sysNames_) << std::endl;

  binningPluginNames_.insert(binningPluginNames_.begin(), sysNames_.begin(), sysNames_.end());
  binningPluginNames_.push_back(sysCentralValue_);
}

DQMDumpSysUncertaintyBinningResults::simpleUncertaintyType::~simpleUncertaintyType()
{
//--- nothing to be done yet...
}

bool containsSysName(const vstring& sysNames_skip, const std::string& sysName)
{
  for ( vstring::const_iterator sysName_skip = sysNames_skip.begin(); 
	sysName_skip != sysNames_skip.end(); ++sysName_skip ) {
    if ( sysName == (*sysName_skip) ) return true;
  }

  return false;
}

double getSysShift(const binResultsMapppingType& binResults_systematic, 
		   const std::string& sysName, double binCentralValue)
{
  //std::cout << "<getSysShift>:" << std::endl;
  //std::cout << " sysName = " << sysName << std::endl;
  //std::cout << " binCentralValue = " << binCentralValue << std::endl;

  binResultsMapppingType::const_iterator binResult_systematic = binResults_systematic.find(sysName);
 
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

void DQMDumpSysUncertaintyBinningResults::simpleUncertaintyType::printBinResult(int bin, const std::string& resultTypeName) const
{
  double binCentralValue = 0.;
  binResultsMapppingType binResultsSys;
  if ( bin != -1 ) {
    binCentralValue = getBinResults_sysCentralValue(sysCentralValue_, bin, resultTypeName).binContent_;
    binResultsSys = getBinResults_systematic(sysNames_, bin, resultTypeName);
  } else {
    binCentralValue = getBinResultsSum_sysCentralValue(sysCentralValue_, resultTypeName).binContent_;
    binResultsSys = getBinResultsSum_systematic(sysNames_, resultTypeName);
  }

  //std::cout << " binCentralValue = " << binCentralValue << std::endl;

  static TPRegexp regexpParser_bidirectional_entry("[[:alnum:]]+(Up|Down)");
  static TPRegexp regexpParser_bidirectional_name("([[:alnum:]]+)(Up|Down)");
  static TPRegexp regexpParser_array_entry("[[:alnum:]]+\\([[:digit:]]+\\)");
  static TPRegexp regexpParser_array_name("([[:alnum:]]+)\\([[:digit:]]+\\)");

  vstring sysNamesToSkip;

  for ( vstring::const_iterator sysName = sysNames_.begin();
	sysName != sysNames_.end(); ++sysName ) {
    //std::cout << " sysName = " << (*sysName) << std::endl;

    TString sysName_tstring = sysName->data();
    
    bool parseError = false;
    
    if ( regexpParser_bidirectional_entry.Match(sysName_tstring) >= 1 ) {
      TObjArray* subStrings = regexpParser_bidirectional_name.MatchS(sysName_tstring);
      
      if ( subStrings->GetEntries() == 3 ) {
	std::string sysName_bidirectional = ((TObjString*)subStrings->At(1))->GetString().Data();
	
	if ( containsSysName(sysNamesToSkip, sysName_bidirectional) ) continue;
	      
	double sysShift_up   = getSysShift(binResultsSys, std::string(sysName_bidirectional).append("Up"),   binCentralValue);
	double sysShift_down = getSysShift(binResultsSys, std::string(sysName_bidirectional).append("Down"), binCentralValue);
	
	std::cout << " " << std::setw(20) << sysTitle_ << ":"
		  << " up = " << std::setprecision(4) << std::fixed << sysShift_up*100. << "%,"
		  << " down = " << std::setprecision(4) << std::fixed << sysShift_down*100. << "%" << std::endl;
	
	sysNamesToSkip.push_back(sysName_bidirectional);
      } else {
	parseError = true;
      }
    } else if ( regexpParser_array_entry.Match(sysName_tstring) == 1 ){
      TObjArray* subStrings = regexpParser_array_name.MatchS(sysName_tstring);
      
      if ( subStrings->GetEntries() == 2 ) {
	std::string sysName_array = ((TObjString*)subStrings->At(1))->GetString().Data();
	
	if ( containsSysName(sysNamesToSkip, sysName_array) ) continue;
	
	TPRegexp regexpParser_element_entry(std::string(sysName_array).append("\\([[:digit:]]+\\)").data());

	vstring sysNames;
	for ( binResultsMapppingType::const_iterator binResultSys = binResultsSys.begin();
	      binResultSys != binResultsSys.end(); ++binResultSys ) {
	  if ( regexpParser_element_entry.Match(TString(binResultSys->first.data())) == 1 ) {
	    sysNames.push_back(binResultSys->first);
	  } else {
	    edm::LogError ("printBinResult") 
	      << " Invalid sysName = " << binResultSys->first << " within Array !!";
	    continue;
	  }
	}

//--- simply sum in quadrature difference of all array elements
//    with respect to central value

	double sysShiftsSum2 = 0.;
	
	for ( vstring::const_iterator sysName_element = sysNames.begin();
	      sysName_element != sysNames.end(); ++sysName_element ) {
	  double sysShift_element = getSysShift(binResultsSys, *sysName_element, binCentralValue);
	  sysShiftsSum2 += (sysShift_element*sysShift_element);
	}
	
	std::cout << " " << std::setw(20) << sysTitle_ << ":"
		  << " " << std::setprecision(4) << std::fixed << TMath::Sqrt(sysShiftsSum2)*100. << "%" << std::endl;
      } else {
	parseError = true;
      }
    } else {
      double sysShift = getSysShift(binResultsSys, *sysName, binCentralValue);
      
      std::cout << " " << std::setw(20) << (*sysName) << ":"
		<< " " << std::setprecision(4) << std::fixed << sysShift*100. << "%" << std::endl;
    }

    if ( parseError ) {
      edm::LogError ("printBinResult") << " Failed to decode sysName = " << (*sysName) << " !!";
    }
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMDumpSysUncertaintyBinningResults::pdfUncertaintyType::pdfUncertaintyType(const edm::ParameterSet& cfg)
  : sysUncertaintyBaseType(cfg)
{
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgPDFsets = cfg.getParameter<vParameterSet>("pdfSets");
  for ( vParameterSet::const_iterator cfgPDFset = cfgPDFsets.begin();
	cfgPDFset != cfgPDFsets.end(); ++cfgPDFset ) {
    vstring sysCentralValues = cfgPDFset->getParameter<vstring>("sysCentralValues");
    //std::cout << " sysCentralValues = " << format_vstring(sysCentralValues) << std::endl;

    vstring cfgSystematics = cfgPDFset->getParameter<vstring>("sysNames");
    //std::cout << " cfgSystematics = " << format_vstring(cfgSystematics) << std::endl;

    vstring sysNames = expandSysNamesExclCentralValue(cfgSystematics, sysCentralValues);
    //std::cout << " sysNames = " << format_vstring(sysNames) << std::endl;

    pdfSetEntryType pdfSetEntry(sysCentralValues, sysNames);
    pdfSetEntries_.push_back(pdfSetEntry);

    binningPluginNames_.insert(binningPluginNames_.begin(), sysNames.begin(), sysNames.end());
    binningPluginNames_.insert(binningPluginNames_.begin(), sysCentralValues.begin(), sysCentralValues.end());
  }
}

DQMDumpSysUncertaintyBinningResults::pdfUncertaintyType::~pdfUncertaintyType()
{
//--- nothing to be done yet...
}

void DQMDumpSysUncertaintyBinningResults::pdfUncertaintyType::printBinResult(int bin, const std::string& resultTypeName) const
{
//--- estimation of (PDF) uncertainties
//    according to:
//   o VBTF analysis CMS EWK-10-002 ("abs")
//   o "Master Equations" in hep-ph/0605240 ("sum")

  double maxSysShift_plus   = 0.;
  double maxSysShift_minus  = 0.;

  double sysShiftsSum2_plus  = 0.;
  double sysShiftsSum2_minus = 0.;  

  for ( std::vector<pdfSetEntryType>::const_iterator pdfSetEntry = pdfSetEntries_.begin();
	pdfSetEntry != pdfSetEntries_.end(); ++pdfSetEntry ) {

//--- compute central value
    double binCentralValue = 0.;
    for ( vstring::const_iterator sysCentralValue = pdfSetEntry->sysCentralValues_.begin();
	  sysCentralValue != pdfSetEntry->sysCentralValues_.end(); ++sysCentralValue ) {
      if ( bin != -1 ) binCentralValue += getBinResults_sysCentralValue(*sysCentralValue, bin, resultTypeName).binContent_;
      else             binCentralValue += getBinResultsSum_sysCentralValue(*sysCentralValue, resultTypeName).binContent_;
    }

    binCentralValue /= pdfSetEntry->sysCentralValues_.size();
    std::cout << " PDF set central value(s) = " << format_vstring(pdfSetEntry->sysCentralValues_) << std::endl;
    std::cout << "--> taking " << binCentralValue << std::endl;

    binResultsMapppingType binResultsSys;
    if ( bin != -1 ) binResultsSys = getBinResults_systematic(pdfSetEntry->sysNames_, bin, resultTypeName);
    else             binResultsSys = getBinResultsSum_systematic(pdfSetEntry->sysNames_, resultTypeName);

    static TPRegexp regexpParser_array_entry("[[:alnum:]]+\\([[:digit:]]+\\)");
    static TPRegexp regexpParser_array_name("([[:alnum:]]+)\\([[:digit:]]+\\)");

    vstring sysNamesToSkip;
    
    for ( vstring::const_iterator sysName = pdfSetEntry->sysNames_.begin();
	  sysName != pdfSetEntry->sysNames_.end(); ++sysName ) {
      //std::cout << " sysName = " << (*sysName) << std::endl;
    
      TString sysName_tstring = sysName->data();
    
      bool parseError = false;

      if ( regexpParser_array_entry.Match(sysName_tstring) == 1 ){
	TObjArray* subStrings = regexpParser_array_name.MatchS(sysName_tstring);
      
	if ( subStrings->GetEntries() == 2 ) {
	  std::string sysName_array = ((TObjString*)subStrings->At(1))->GetString().Data();
	  
	  if ( containsSysName(sysNamesToSkip, sysName_array) ) continue;
	  
	  TPRegexp regexpParser_element_entry(std::string(sysName_array).append("\\([[:digit:]]+\\)").data());
	  
	  vstring sysNames;
	  for ( binResultsMapppingType::const_iterator binResultSys = binResultsSys.begin();
		binResultSys != binResultsSys.end(); ++binResultSys ) {
	    if ( regexpParser_element_entry.Match(TString(binResultSys->first.data())) == 1 ) {
	      sysNames.push_back(binResultSys->first);
	    } else {
	      edm::LogError ("printBinResult") 
		<< " Invalid sysName = " << binResultSys->first << " within Array !!";
	      continue;
	    }
	  }

//--- check that sysNames can be paired
	  unsigned numSysNames = sysNames.size();
	  if ( (numSysNames % 2) != 0 ) {
	    edm::LogError ("printBinResult") 
	      << " Invalid list of sysNames = " << format_vstring(sysNames) << "; failed to find Pairs !!";
	    continue;
	  }
	  
	  unsigned numSysNamePairs = (numSysNames / 2);
	  for ( unsigned iSysNamePair = 0; iSysNamePair < numSysNamePairs; ++iSysNamePair ) {
	    const std::string sysName_up = sysNames[2*iSysNamePair];
	    double sysShift_up = getSysShift(binResultsSys, sysName_up, binCentralValue);
	    std::cout << " sysName_up = " << sysName_up << ": sysShift_up = " << sysShift_up << std::endl;
	    
	    const std::string sysName_down = sysNames[2*iSysNamePair + 1];
	    double sysShift_down = getSysShift(binResultsSys, sysName_down, binCentralValue);
	    std::cout << " sysName_down = " << sysName_down << ": sysShift_down = " << sysShift_down << std::endl;
	    
	    double sysShift_plus = TMath::Max(sysShift_up, sysShift_down);
	    if ( TMath::Abs(sysShift_plus) > maxSysShift_plus ) maxSysShift_plus = TMath::Abs(sysShift_plus);
	    if ( sysShift_plus > 0. ) sysShiftsSum2_plus += sysShift_plus*sysShift_plus;
	    
	    double sysShift_minus = TMath::Min(sysShift_up, sysShift_down);
	    if ( TMath::Abs(sysShift_minus) > maxSysShift_minus ) maxSysShift_minus = TMath::Abs(sysShift_minus);
	    if ( sysShift_minus < 0. ) sysShiftsSum2_minus += sysShift_minus*sysShift_minus;
	  }

	  sysNamesToSkip.push_back(sysName_array);
	} else {
	  parseError = true;
	}
      } else {
	parseError = true;
      }

      if ( parseError ) {
	edm::LogError ("printBinResult") << " Failed to decode sysName = " << (*sysName) << " !!";
      }
    }

    std::cout << "--> sysShiftsSum2: plus = " << TMath::Sqrt(sysShiftsSum2_plus)*100. << "%," 
	      << " minus = " << TMath::Sqrt(sysShiftsSum2_minus)*100. << "%" << std::endl;
  }

  sysShiftsSum2_plus  /= pdfSetEntries_.size();
  sysShiftsSum2_minus /= pdfSetEntries_.size();

  std::cout << " " << std::setw(20) << sysTitle_ << ":" << std::endl;
  std::cout << " " << std::setw(10) << "plus:" 
	    << " 'abs' method = " << std::setprecision(4) << std::fixed << maxSysShift_plus*100. << "%,"
	    << " 'sum' method = " << std::setprecision(4) << std::fixed << TMath::Sqrt(sysShiftsSum2_plus)*100. << "%" 
	    << std::endl;
  std::cout << " " << std::setw(10) << "minus:" 
	    << " 'abs' method = " << std::setprecision(4) << std::fixed << maxSysShift_minus*100. << "%,"
	    << " 'sum' method = " << std::setprecision(4) << std::fixed << TMath::Sqrt(sysShiftsSum2_minus)*100. << "%" 
	    << std::endl;
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
  for ( std::vector<sysUncertaintyBaseType*>::iterator it = sysUncertaintyEntries_.begin();
	it != sysUncertaintyEntries_.end(); ++it ) {
    delete (*it);
  }
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
      sysUncertaintyBaseType* sysUncertaintyEntry = 0;
      std::string method = "simple";
      if ( cfgSysUncertainty->exists("method") ) method = cfgSysUncertainty->getParameter<std::string>("method");
      if      ( method == "simple" ) sysUncertaintyEntry = new simpleUncertaintyType(*cfgSysUncertainty);
      else if ( method == "pdf"    ) sysUncertaintyEntry = new pdfUncertaintyType(*cfgSysUncertainty);
      else {
	edm::LogError("DQMDumpSysUncertaintyBinningResults") 
	  << " Invalid Configuration Parameter 'method' = " << method << " --> skipping !!";
	cfgError_ = 1;
	continue;
      }

      if ( sysUncertaintyEntry->cfgError_ ) cfgError_ = 1;

      processEntry->sysUncertaintyEntries_.push_back(sysUncertaintyEntry);
    }
  }

  resultTypeNames_ = cfg.getParameter<vstring>("resultTypes");
  
  if ( processEntries_.size() == 0 ) {
    edm::LogError("DQMDumpSysUncertaintyBinningResults") 
      << " Configuration Parameter 'dqmDirectories' contains no Entries --> skipping !!";
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
    for ( std::vector<sysUncertaintyBaseType*>::iterator sysUncertaintyEntry = processEntry->sysUncertaintyEntries_.begin();
	  sysUncertaintyEntry != processEntry->sysUncertaintyEntries_.end(); ++sysUncertaintyEntry ) {
      (*sysUncertaintyEntry)->loadBinningResults(processEntry->dqmDirectory_);
    }
  }

//--- print objects created by SysUncertaintyBinner class
  for ( std::vector<processEntryType>::const_iterator processEntry = processEntries_.begin();
	processEntry != processEntries_.end(); ++processEntry ) {
    for ( vstring::const_iterator resultTypeName = resultTypeNames_.begin();
	  resultTypeName != resultTypeNames_.end(); ++resultTypeName ) {

      std::cout << "Systematic Uncertainties on resultTypeName = " << (*resultTypeName) 
		<< " for process = " << processEntry->processName_ << ":" << std::endl;

//--- get bin grid
//   (assume that same binning has been used for all systematic uncertainties)
      const BinGrid* binGrid = 0;
      std::vector<sysUncertaintyBaseType*>::const_iterator sysUncertaintyEntry_ref = processEntry->sysUncertaintyEntries_.begin();
      if ( sysUncertaintyEntry_ref != processEntry->sysUncertaintyEntries_.end() ) {
	std::map<std::string, BinningBase*>::const_iterator binningPlugins_ref = (*sysUncertaintyEntry_ref)->binningPlugins_.begin();
	if ( binningPlugins_ref != (*sysUncertaintyEntry_ref)->binningPlugins_.end() ) binGrid = binningPlugins_ref->second->binGrid();
      }
      
      if ( !binGrid ) {
	edm::LogError ("endJob") 
	  << " Failed to find bin-Grid --> skipping !!";
	continue;
      }

      unsigned numBins = binGrid->numBins();
      if ( numBins > 1 ) {
	for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {
//--- print bin-center position
	  printBinCenterPosition(std::cout, binGrid, iBin);
	  std::cout << std::endl;

//--- print systematic uncertainties on number of events in bin
	  for ( std::vector<sysUncertaintyBaseType*>::const_iterator sysUncertaintyEntry = processEntry->sysUncertaintyEntries_.begin();
		sysUncertaintyEntry != processEntry->sysUncertaintyEntries_.end(); ++sysUncertaintyEntry ) {
	    (*sysUncertaintyEntry)->printBinResult(iBin, *resultTypeName);
	  }
	}
      }
	
//--- print systematic uncertainties on total number of events
//    (sum of all bins)
      std::cout << " sum: " << std::endl;
	
      for ( std::vector<sysUncertaintyBaseType*>::const_iterator sysUncertaintyEntry = processEntry->sysUncertaintyEntries_.begin();
	    sysUncertaintyEntry != processEntry->sysUncertaintyEntries_.end(); ++sysUncertaintyEntry ) {
	(*sysUncertaintyEntry)->printBinResult(-1, *resultTypeName);
      }
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMDumpSysUncertaintyBinningResults);
