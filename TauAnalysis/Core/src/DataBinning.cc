#include "TauAnalysis/Core/interface/DataBinning.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/Core/interface/binningAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TMath.h>

#include <iostream>
#include <iomanip>
 
const std::string meOptionsBinContent = std::string(meOptionsSeparator).append("a1").append(meOptionsSeparator).append("s1");
const std::string meOptionsBinError = std::string(meOptionsSeparator).append("a2").append(meOptionsSeparator).append("s1");

DataBinning::DataBinning()
{
  numBins_ = 0;
}

DataBinning::DataBinning(const edm::ParameterSet& cfg) 
  : BinningBase(cfg)
{
  //std::cout << "<DataBinning::DataBinning>:" << std::endl;

  numBins_ = binGrid_->numBins();
  //std::cout << "numBins = " << numBins_ << std::endl;

  for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
    binEntries_.push_back(binEntryType());
  }
}

DataBinning::DataBinning(const DataBinning& bluePrint)
  : BinningBase(bluePrint),
    binEntries_(bluePrint.binEntries_),
    numBins_(bluePrint.numBins_)
{}

DataBinning::DataBinning(const std::string& name, const BinGrid& binGrid, 
			 const std::vector<double>& binContents, const std::vector<double>& binSumw2)
  : BinningBase(name, binGrid)
{
  assert(binContents.size() == binSumw2.size());

  numBins_ = binEntries_.size();

  for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
    binEntryType& binEntry = binEntries_[iBin];

    binEntry.binContent_ = binContents[iBin];
    binEntry.binSumw2_ = binSumw2[iBin];
  }
}

DataBinning::~DataBinning()
{
//--- nothing to be done yet...
}

void DataBinning::bin(const std::vector<double>& x, double weight)
{
  int iBin = binGrid_->binNumber(x);
  if ( iBin >= 0 && iBin < (int)numBins_ ) {
    binEntryType& binEntry = binEntries_[iBin];

    binEntry.binContent_ += weight;
    binEntry.binSumw2_ += weight*weight;
  }
}

void DataBinning::print(std::ostream& stream) const
{
  stream << "<DataBinning::print>:" << std::endl;
  stream << " name = " << name_ << std::endl;
  
  double binContent_sum = 0.;
  double binSumw2_sum = 0.;
  for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
    const binEntryType& binEntry = binEntries_[iBin];

    if ( numBins_ > 1 ) {
      printBinCenterPosition(stream, binGrid_, iBin);
      stream << std::setprecision(3) << std::fixed << binEntry.binContent_ << " +/- " << TMath::Sqrt(binEntry.binSumw2_) << std::endl;
    }
    
    binContent_sum += binEntry.binContent_;
    binSumw2_sum += binEntry.binSumw2_;
  }

  stream << " sum: " << std::setprecision(3) << std::fixed << binContent_sum << " +/- " << TMath::Sqrt(binSumw2_sum) << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

std::vector<binResultType> DataBinning::getBinResults(unsigned binNumber) const
{
  std::vector<binResultType> binResults;
  
  if ( binNumber < numBins_ ) {
    const binEntryType& binEntry = binEntries_[binNumber];

    binResults.push_back(binResultType(binEntry.binContent_, binEntry.binSumw2_, "rec"));
  } else {
    edm::LogError ("DataBinning::getBinResults") << "Invalid binNumber = " << binNumber << " !!";
  }
  
  return binResults;
}

std::vector<binResultType> DataBinning::getBinResultsSum() const
{
  binEntryType binEntrySum;
  
  for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
    const binEntryType& binEntry = binEntries_[iBin];

    binEntrySum += binEntry;
  }

  std::vector<binResultType> binResultsSum;
  binResultsSum.push_back(binResultType(binEntrySum.binContent_, binEntrySum.binSumw2_, "rec"));
  
  return binResultsSum;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

std::vector<std::string> DataBinning::encodeStringRep() const
{
  std::vector<std::string> buffer = BinningBase::encodeStringRep();

  for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
    const binEntryType& binEntry = binEntries_[iBin];

    std::ostringstream meName_binContent;
    meName_binContent << "binContent_region" << (iBin + 1) << meOptionsBinContent;
    std::ostringstream meValue_binContent;
    meValue_binContent << std::setprecision(3) << std::fixed << binEntry.binContent_;
    std::string entry_binContent = encodeBinningStringRep(meName_binContent.str(), "float", meValue_binContent.str());
    buffer.push_back(entry_binContent);

    std::ostringstream meName_binError;
    meName_binError << "binError_region" << (iBin + 1) << meOptionsBinError;
    std::ostringstream meValue_binError;
    meValue_binError << std::setprecision(3) << std::fixed << TMath::Sqrt(binEntry.binSumw2_);
    std::string entry_binError = encodeBinningStringRep(meName_binError.str(), "float", meValue_binError.str());
    buffer.push_back(entry_binError);
  }

  return buffer;
}

void DataBinning::decodeStringRep(std::vector<std::string>& buffer)
{
  BinningBase::decodeStringRep(buffer);

  TPRegexp regexpParser_numBins("numBins");
  TPRegexp regexpParser_binContent_entry(std::string("binContent_region[[:digit:]]+").append(meOptionsBinContent));
  TPRegexp regexpParser_binContent_binNumber(std::string("binContent_region([[:digit:]]+)").append(meOptionsBinContent));
  TPRegexp regexpParser_binError_entry(std::string("binError_region[[:digit:]]+").append(meOptionsBinError));
  TPRegexp regexpParser_binError_binNumber(std::string("binError_region([[:digit:]]+)").append(meOptionsBinError));

  bool numBins_initialized = false;
  std::vector<bool> binContent_initialized;
  std::vector<bool> binSumw2_initialized;
  
  for ( std::vector<std::string>::const_iterator entry = buffer.begin();
	entry != buffer.end(); ++entry ) {
    std::string meName, meType, meValue;
    int error = 0;
    decodeBinningStringRep(*entry, meName, meType, meValue, error);

    if ( error ) {
      edm::LogError ("DataBinning::decodeStringRep") << " Error in parsing string = " << (*entry) << " --> skipping !!";
      continue;
    }

    TString meName_tstring = meName.data();
    
    bool binNumber_error = false;

    if ( regexpParser_numBins.Match(meName_tstring) == 1 ) {
      numBins_ = (unsigned)atoi(meValue.data());
      binEntries_.resize(numBins_);
      binContent_initialized.resize(numBins_);
      binSumw2_initialized.resize(numBins_);
      numBins_initialized = true;
    } else if ( regexpParser_binContent_entry.Match(meName_tstring) == 1 ) {
      if ( !numBins_initialized ) {
	edm::LogError ("DataBinning::decodeStringRep") << " Need to initialize numBins before setting binContents !!";
	continue;
      }
      
      TObjArray* subStrings = regexpParser_binContent_binNumber.MatchS(meName_tstring);
      if ( subStrings->GetEntries() == 2 ) {
	int binNumber = (unsigned)atoi(((TObjString*)subStrings->At(1))->GetString().Data()) - 1;
	float binContent = atof(meValue.data());
	
	if ( binNumber >= 0 && binNumber < (int)numBins_ ) {
	  binEntries_[binNumber].binContent_ = binContent;
	  binContent_initialized[binNumber] = true;
	} else {
	  edm::LogError ("DataBinning::decodeStringRep") << " Bin number = " << binNumber << " decoded from meName = " << meName
							 << " not within numBins = " << numBins_ << " range of binning object !!";
	  continue;
	}
      } else {
	binNumber_error = true;
      }
    } else if ( regexpParser_binError_entry.Match(meName_tstring) == 1 ) {
      if ( !numBins_initialized ) {
	edm::LogError ("DataBinning::decodeStringRep") << " Need to initialize numBins before setting binError !!";
	continue;
      }

      TObjArray* subStrings = regexpParser_binError_binNumber.MatchS(meName_tstring);
      if ( subStrings->GetEntries() == 2 ) {
	int binNumber = (unsigned)atoi(((TObjString*)subStrings->At(1))->GetString().Data()) - 1;
	float binError = atof(meValue.data());

	if ( binNumber >= 0 && binNumber < (int)numBins_ ) {
	  binEntries_[binNumber].binSumw2_ = binError*binError;
	  binSumw2_initialized[binNumber] = true;
	} else {
	  edm::LogError ("DataBinning::decodeStringRep") << " Bin number = " << binNumber << " decoded from meName = " << meName
							 << " not within numBins = " << numBins_ << " range of binning object !!";
	  continue;
	}
      } else {
	binNumber_error = true;
      }
    }

    if ( binNumber_error ) {
      edm::LogError ("DataBinning::decodeStringRep") << " Failed to decode bin number from meName = " << meName << " !!";
      continue;
    }
  }

//--- check that all data-members of DataBinning object 
//    have been initialized
  if ( numBins_initialized ) {
    for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
      if ( !binContent_initialized[iBin] ) edm::LogError ("decodeStringRep") << " Failed to decode binContents[" << iBin << "] !!";
      if ( !binSumw2_initialized[iBin] ) edm::LogError ("decodeStringRep") << " Failed to decode binError[" << iBin << "] !!";
    }
  } else {
    edm::LogError ("DataBinning::decodeStringRep") << " Failed to decode numBins !!";
  }
}


