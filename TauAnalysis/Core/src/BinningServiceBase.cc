#include "TauAnalysis/Core/interface/BinningServiceBase.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/Core/interface/binningAuxFunctions.h"

#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>

#include <iostream>
#include <iomanip>
#include <algorithm>

const std::string meNameSeparator = ". :";
const std::string meNameOrder = "meNameOrder";

BinningServiceBase::BinningServiceBase(const edm::ParameterSet&)
{}

BinningServiceBase::~BinningServiceBase()
{
//--- nothing to be done yet...
}

bool operator<(const class BinningServiceBase::meEntry& entry1, const class BinningServiceBase::meEntry& entry2)
{
  return entry1.id_ < entry2.id_;
} 

BinningBase* BinningServiceBase::loadBinningResults(const std::string& dqmDirectory) const
{
//--- check if DQMStore is available;
//    print an error message and return if not
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("BinningServiceBase::loadBinningResults") << " Failed to access dqmStore !!";
    return 0;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  dqmStore.setCurrentFolder(dqmDirectory);

  std::vector<meEntry> meEntries;

//--- load from DQMStore auxiliary MonitorElement 
//    in which order of MonitorElements is encoded,
//    so that MonitorElements can be loaded from DQMStore
//    in the order in which they have been saved
  std::string meNameOrder_full = dqmDirectoryName(dqmDirectory).append(meNameOrder);
  //std::cout << " meNameOrder_full = " <<  meNameOrder_full << std::endl;

  MonitorElement* meOrder = dqmStore.get(meNameOrder_full);
  //std::cout << " meOrder = " << meOrder << std::endl;
  if ( !meOrder ) {
    edm::LogError ("BinningServiceBase::loadBinningResults") << " Failed to access meName = " << meNameOrder << " in DQMStore" 
					                     << " --> skipping !!";
    return 0;
  }

//--- decode order of MonitorElements from auxiliary MonitorElement 
  TString pattern_entry = "[[:alnum:]]+[,[[:space:]]*[[:alnum:]]+]*";
  TPRegexp regexpParser_entry(pattern_entry);

  TString meValueOrder_tstring = meOrder->getStringValue();
  //std::cout << "meValueOrder_tstring = " << meValueOrder_tstring.Data() << std::endl;

  if ( regexpParser_entry.Match(meValueOrder_tstring) != 1 ) {
    edm::LogError ("BinningServiceBase::loadBinningResults") << " Failed to decode meName order = " 
							     << meValueOrder_tstring.Data() << " !!";
    return 0;
  }
  
  TObjArray* subStrings = meValueOrder_tstring.Tokenize(",");
  //std::cout << "subStrings->GetEntries() = " << subStrings->GetEntries() << std::endl;

  unsigned numEntries = subStrings->GetEntries();
  for ( unsigned iEntry = 0; iEntry < numEntries; ++iEntry ) {
    std::string meName = ((TObjString*)subStrings->At(iEntry))->GetString().ReplaceAll(" ", "").Data();

    std::string meName_full = dqmDirectoryName(dqmDirectory).append(meName);
    //std::cout << " meName_full = " <<  meName_full << std::endl;

    dqmStore.setCurrentFolder(dqmDirectory);
    MonitorElement* me = dqmStore.get(meName_full);
    //std::cout << " me = " << me << std::endl;
    if ( !me ) {
      edm::LogError ("BinningServiceBase::loadBinningResults") << " Failed to access meName = " << meName << " in DQMStore" 
							       << " --> skipping !!";
      continue;
    }

//--- skip "invalid" MonitorElements
    if ( me->kind() == MonitorElement::DQM_KIND_INVALID ) {
      edm::LogWarning ("loadBinningResults") << " MonitorElement meName = " << meName << " marked as invalid" 
					     << " --> skipping !!";
      continue;
    }

    std::string meType, meValue;

    if ( me->kind() == MonitorElement::DQM_KIND_STRING ) {
      meType = "string";
      meValue = me->getStringValue();
      //std::cout << "meValue(string) = " << meValue << std::endl;
    } else if ( me->kind() == MonitorElement::DQM_KIND_REAL ) {
      meType = "float";
      std::ostringstream meValue_ostringstream;
      meValue_ostringstream << std::setprecision(3) << std::fixed << me->getFloatValue();
      meValue = meValue_ostringstream.str();
      //std::cout << "meValue(float) = " << meValue << std::endl;
    } else if ( me->kind() == MonitorElement::DQM_KIND_INT ) {
      meType = "int";
      std::ostringstream meValue_ostringstream;
      meValue_ostringstream << me->getIntValue();
      meValue = meValue_ostringstream.str();
      //std::cout << "meValue(int) = " << meValue << std::endl;
    } else {
      edm::LogError ("BinningServiceBase::loadBinningResults") << " MonitorElement meName = " << meName << " of unsupported type" 
							       << " --> skipping !!";
      continue;
    } 

    int meId = iEntry;
    meEntries.push_back(meEntry(meId, meName, meType, meValue));
  }

  delete subStrings;

//--- sort meEntry objects representing MonitorElements
//    in the order by which MonitorElements have been saved
  std::sort(meEntries.begin(), meEntries.end()); 

//--- fill meEntry objects into buffer in sorted order
  std::vector<std::string> buffer;
  for ( std::vector<meEntry>::const_iterator meEntry_i = meEntries.begin();
	meEntry_i != meEntries.end(); ++meEntry_i ) {
    std::string entry = encodeBinningStringRep(meEntry_i->name_, meEntry_i->type_, meEntry_i->value_);
    //std::cout << "entry = " << entry << std::endl;
    buffer.push_back(entry);
  }

//--- create binning object and initialize with content of buffer
  BinningBase* binning = createBinning();
  buffer >> (*binning);

  return binning;
}

void BinningServiceBase::saveBinningResults(const std::string& dqmDirectory, const BinningBase* binning) const
{
//--- check if DQMStore is available;
//    print an error message and return if not
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("BinningServiceBase::saveBinningResults") << " Failed to access dqmStore" 
							     << " --> binning results will NOT be saved !!";
    return;
  }
  
  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  dqmStore.setCurrentFolder(dqmDirectory);

  std::vector<std::string> buffer;
  buffer << (*binning);

  std::vector<std::string> meNames_ordered;
  
  for ( std::vector<std::string>::const_iterator entry = buffer.begin();
	entry != buffer.end(); ++entry ) {
    std::string meName, meType, meValue;
    int error = 0;
    decodeBinningStringRep(*entry, meName, meType, meValue, error);

    if ( error ) {
      edm::LogError ("BinningServiceBase::saveBinningResults") << " Error in parsing string = " << (*entry) << " --> skipping !!";
      continue;
    }

    if ( meType == "string" ) {
      dqmStore.bookString(meName, meValue);
    } else if ( meType == "float" ) {
      MonitorElement* me = dqmStore.bookFloat(meName);
      me->Fill(atof(meValue.data()));
    } else if ( meType == "int" ) {
      MonitorElement* me = dqmStore.bookInt(meName);
      me->Fill(atoi(meValue.data()));
    } else {
      edm::LogError ("BinningServiceBase::saveBinningResults") << " Undefined meType = " << meType << " --> skipping !!";
      continue;
    }

    meNames_ordered.push_back(meName);
  }
  
//--- encode order of MonitorElements into auxiliary MonitorElement,
//    so that MonitorElements can be loaded from DQMStore
//    in the order in which they have been saved
  std::ostringstream meValueOrder;
  
  unsigned numEntries = meNames_ordered.size();
  for ( unsigned iEntry = 0; iEntry < numEntries; ++iEntry ) {
    std::string meName = meNames_ordered[iEntry];
    meValueOrder << meName;
    if ( iEntry < (numEntries - 1) ) meValueOrder << ", ";
  }

  dqmStore.bookString(meNameOrder, meValueOrder.str());
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(BinningServicePluginFactory, "BinningServicePluginFactory");
