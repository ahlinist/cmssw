#include "TauAnalysis/Core/interface/BinningBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/Core/interface/binningAuxFunctions.h"

#include <TPRegexp.h>
#include <TString.h>

BinningBase::BinningBase()
{
  name_ = "undefined";
  binGrid_ = 0;
}

BinningBase::BinningBase(const edm::ParameterSet& cfg)
{
  name_ = cfg.getParameter<std::string>("name");

  binGrid_ = new BinGrid(cfg);
}

BinningBase::~BinningBase()
{
  delete binGrid_;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

std::vector<std::string> BinningBase::encodeStringRep() const
{
  std::vector<std::string> buffer;

  buffer << (*binGrid_);

  std::string entry_name = encodeBinningStringRep("name", "string", name_);
  buffer.push_back(entry_name);

  return buffer;
}

void BinningBase::decodeStringRep(std::vector<std::string>& buffer)
{
  if ( binGrid_ ) delete binGrid_;
  binGrid_ = new BinGrid();
  buffer >> (*const_cast<BinGrid*>(binGrid_));

  TPRegexp regexpParser_name("name");

  bool name_initialized = false;
  
  for ( std::vector<std::string>::const_iterator entry = buffer.begin();
	entry != buffer.end(); ++entry ) {
    std::string meName, meType, meValue;
    int error = 0;
    decodeBinningStringRep(*entry, meName, meType, meValue, error);

    if ( error ) {
      edm::LogError ("BinningBase::decodeStringRep") << " Error in parsing string = " << (*entry) << " --> skipping !!";
      continue;
    }

    TString meName_tstring = meName.data();
    
    if ( regexpParser_name.Match(meName_tstring) == 1 ) {
      name_ = meValue;
      name_initialized = true;
    }
  }

//--- check that all data-members of BinningBase object 
//    have been initialized
  if ( !name_initialized ) {
    edm::LogError ("BinningBase::decodeStringRep") << " Failed to decode name !!";
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

std::vector<std::string>& operator<<(std::vector<std::string>& buffer, const BinningBase& binning)
{
  std::vector<std::string> buffer_ext = binning.encodeStringRep();

  unsigned numEntries = buffer_ext.size();
  for ( unsigned iEntry = 0; iEntry < numEntries; ++iEntry ) {
    buffer.push_back(buffer_ext[iEntry]);
  }

  return buffer;
}

std::vector<std::string>& operator>>(std::vector<std::string>& buffer, BinningBase& binning)
{
  binning.decodeStringRep(buffer);

  return buffer;
}


