#include "TauAnalysis/Core/interface/ModelBinning.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/Core/interface/binningAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TMath.h>

#include <iostream>
#include <iomanip>
 
const std::string meOptionsBinContent = std::string(meOptionsSeparator).append("a1").append(meOptionsSeparator).append("s1");
const std::string meOptionsBinError = std::string(meOptionsSeparator).append("a2").append(meOptionsSeparator).append("s1");

ModelBinning::ModelBinning()
{
  numBins_ = 0;
}

ModelBinning::ModelBinning(const edm::ParameterSet& cfg) 
  : BinningBase(cfg)
{
  //std::cout << "<ModelBinning::ModelBinning>:" << std::endl;

  numBins_ = binGrid_->numBins();
  //std::cout << "numBins = " << numBins_ << std::endl;

  for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
    binEntries_.push_back(binEntryType_model());
  }
}

ModelBinning::~ModelBinning()
{
//--- nothing to be done yet...
}

void ModelBinning::bin(const std::vector<double>& x, double weight)
{
  edm::LogError ("ModelBinning::bin") << " Invalid function call to bin method with one argument, need to call customized version !!";
}

void ModelBinning::bin(const std::vector<double>& xRec, bool passesRec, double weightRec,
		       const std::vector<double>& xGen, bool passesGen, double weightGen)
{
  //std::cout << "<ModelBinning::bin>:" << std::endl;
  //std::cout << " passesRec = " << passesRec << std::endl;
  //std::cout << " weightRec = " << weightRec << std::endl;
  //std::cout << " passesGen = " << passesGen << std::endl;
  //std::cout << " weightGen = " << weightGen << std::endl;

  int recBin = binGrid_->binNumber(xRec);
  bool inRec = ( recBin >= 0 && recBin < (int)numBins_ ) ? true : false;
  //std::cout << " recBin = " << recBin << std::endl;
  //std::cout << " inRec = " << inRec << std::endl;

  int genBin = binGrid_->binNumber(xGen);
  bool inGen = ( genBin >= 0 && genBin < (int)numBins_ ) ? true : false;
  //std::cout << " genBin = " << genBin << std::endl;
  //std::cout << " inGen = " << inGen << std::endl;

  if ( passesRec && inRec ) binEntries_[recBin].rec_ += weightRec;  
  if ( passesGen && inGen ) binEntries_[genBin].gen_ += weightGen;

  if ( passesGen && inGen && passesRec && inRec ) {
    if ( recBin == genBin ) {
      binEntries_[genBin].stay_ += weightRec;
    } else {
      binEntries_[recBin].smearIn_ += weightRec;
      binEntries_[genBin].smearOut_ += weightRec;
    }
  } else if ( passesGen && inGen && passesRec && (!inRec) ) {
    binEntries_[genBin].smearOut_ += weightRec;
  } else if ( passesGen && inGen && (!passesRec) ) {
    binEntries_[genBin].lost_ += weightRec;
  } else if ( passesGen && (!inGen) && passesRec && inRec ) {
    binEntries_[recBin].smearIn_ += weightRec;
  } else if ( (!passesGen) && passesRec && inRec ) {
    binEntries_[recBin].smearIn_ += weightRec;
  }
}

void ModelBinning::print(std::ostream& stream) const
{
  stream << "<ModelBinning::print>:" << std::endl;
  stream << " name = " << name_ << std::endl;
  
  if ( numBins_ > 1 ) {
    for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
      printBinCenterPosition(stream, binGrid_, iBin);
      stream << std::endl;
      printBinEntry(stream, binEntries_[iBin]);
    }
  }

  binEntryType_model binEntrySum;  
  
  for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
    const binEntryType_model& binEntry = binEntries_[iBin];
    binEntrySum += binEntry;
  }
  
  stream << " sum:" << std::endl;
  printBinEntry(stream, binEntrySum);
}

void printBinEntry_element(std::ostream& stream, const char* label, double value, double err2)
{
  stream << "  " << label << ":"
	 << " " << std::setprecision(4) << std::fixed << value
	 << " +/- " << std::setprecision(4) << std::fixed << TMath::Sqrt(err2) << " (stat.)" << std::endl;  
}

void ModelBinning::printBinEntry(std::ostream& stream, const binEntryType_model& binEntry) const
{
  printBinEntry_element(stream, "gen", binEntry.gen_.binContent_, binEntry.gen_.binSumw2_);
  printBinEntry_element(stream, "rec", binEntry.rec_.binContent_, binEntry.rec_.binSumw2_);

  double acceptance, acceptanceErr2;
  computeAcceptance(binEntry, acceptance, acceptanceErr2);
  printBinEntry_element(stream, "acceptance", acceptance, acceptanceErr2);

  double purity, purityErr2;
  computePurity(binEntry, purity, purityErr2);
  printBinEntry_element(stream, "purity", purity, purityErr2);

  double stability, stabilityErr2;
  computeStability(binEntry, stability, stabilityErr2);
  printBinEntry_element(stream, "stability", stability, stabilityErr2);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

std::vector<binResultType> makeBinResults(const binEntryType_model& binEntry)
{
  std::vector<binResultType> binResults;

  binResults.push_back(binResultType(binEntry.gen_.binContent_, binEntry.gen_.binSumw2_, "gen"));
  binResults.push_back(binResultType(binEntry.rec_.binContent_, binEntry.rec_.binSumw2_, "rec"));
  binResults.push_back(binResultType(binEntry.stay_.binContent_, binEntry.stay_.binSumw2_, "stay"));
  binResults.push_back(binResultType(binEntry.lost_.binContent_, binEntry.lost_.binSumw2_, "lost"));
  binResults.push_back(binResultType(binEntry.smearIn_.binContent_, binEntry.smearIn_.binSumw2_, "smearIn"));
  binResults.push_back(binResultType(binEntry.smearOut_.binContent_, binEntry.smearOut_.binSumw2_, "smearOut"));
  
  double acceptance, acceptanceErr2;
  computeAcceptance(binEntry, acceptance, acceptanceErr2);
  binResults.push_back(binResultType(acceptance, acceptanceErr2, "acceptance"));
  
  double purity, purityErr2;
  computePurity(binEntry, purity, purityErr2);
  binResults.push_back(binResultType(purity, purityErr2, "purity"));
  
  double stability, stabilityErr2;
  computeStability(binEntry, stability, stabilityErr2);
  binResults.push_back(binResultType(stability, stabilityErr2, "stability"));

  return binResults;
}

std::vector<binResultType> ModelBinning::getBinResults(unsigned binNumber) const
{
  if ( binNumber >= 0 && binNumber < numBins_ ) {
    const binEntryType_model& binEntry = binEntries_[binNumber];

    return makeBinResults(binEntry);
  } else {
    edm::LogError ("DataBinning::getBinResults") << "Invalid binNumber = " << binNumber << " !!";
    return std::vector<binResultType>();
  }
}

std::vector<binResultType> ModelBinning::getBinResultsSum() const
{
  binEntryType_model binEntrySum;
  
  for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
    const binEntryType_model& binEntry = binEntries_[iBin];

    binEntrySum += binEntry;
  }

  return makeBinResults(binEntrySum);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DataBinning makeDataBinning(const std::string& name, const BinGrid& binGrid, 
			    std::vector<binEntryType_model> binEntries, void (*compFunction)(const binEntryType_model&, double&, double&))
{
  unsigned numBins = binEntries.size();

  std::vector<double> binContents(numBins);
  std::vector<double> binSumw2(numBins);

  for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {
    const binEntryType_model& binEntry = binEntries[iBin];
    
    double value, err2;
    compFunction(binEntry, value, err2);

    binContents[iBin] = value;
    binSumw2[iBin] = err2;
  }

  return DataBinning(name, binGrid, binContents, binSumw2);
}

DataBinning ModelBinning::getAcceptance() const
{
  std::string name_acceptance = std::string(name_).append("_acceptance");
  return makeDataBinning(name_acceptance, *binGrid_, binEntries_, &computeAcceptance);
}

DataBinning ModelBinning::getPurity() const
{
  std::string name_purity = std::string(name_).append("_purity");
  return makeDataBinning(name_purity, *binGrid_, binEntries_, &computePurity);
}

DataBinning ModelBinning::getStability() const
{
  std::string name_stability = std::string(name_).append("_purity");
  return makeDataBinning(name_stability, *binGrid_, binEntries_, &computeStability);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void ModelBinning::encodeBinEntryStringRep(std::vector<std::string>& buffer, 
					   unsigned binNumber, const char* label, const binEntryType& binEntry) const
{
  std::ostringstream meName_binContent;
  meName_binContent << "binContent_region" << (binNumber + 1) << "_" << label << meOptionsBinContent;
  std::ostringstream meValue_binContent;
  meValue_binContent << std::setprecision(4) << std::fixed << binEntry.binContent_;
  std::string entry_binContent = encodeBinningStringRep(meName_binContent.str(), "float", meValue_binContent.str());
  buffer.push_back(entry_binContent);
      
  std::ostringstream meName_binError;
  meName_binError << "binError_region" << (binNumber + 1) << "_" << label << meOptionsBinError;
  std::ostringstream meValue_binError;
  meValue_binError << std::setprecision(4) << std::fixed << TMath::Sqrt(binEntry.binSumw2_);
  std::string entry_binError = encodeBinningStringRep(meName_binError.str(), "float", meValue_binError.str());
  buffer.push_back(entry_binError);
}

std::vector<std::string> ModelBinning::encodeStringRep() const
{
  std::vector<std::string> buffer = BinningBase::encodeStringRep();

  for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
    const binEntryType_model& binEntry = binEntries_[iBin];

    encodeBinEntryStringRep(buffer, iBin, "gen", binEntry.gen_);
    encodeBinEntryStringRep(buffer, iBin, "rec", binEntry.rec_);
    encodeBinEntryStringRep(buffer, iBin, "stay", binEntry.stay_);
    encodeBinEntryStringRep(buffer, iBin, "lost", binEntry.lost_);
    encodeBinEntryStringRep(buffer, iBin, "smearIn", binEntry.smearIn_);
    encodeBinEntryStringRep(buffer, iBin, "smearOut", binEntry.smearOut_);
  }

  return buffer;
}

binEntryType* ModelBinning::findBinEntry(unsigned binNumber, const std::string& label)
{
  if ( binNumber >= 0 && binNumber < numBins_ ) {
    binEntryType_model& binEntry = binEntries_[binNumber];

    if ( label == "gen"      ) return &binEntry.gen_;
    if ( label == "rec"      ) return &binEntry.rec_;
    if ( label == "stay"     ) return &binEntry.stay_;
    if ( label == "lost"     ) return &binEntry.lost_;
    if ( label == "smearIn"  ) return &binEntry.smearIn_;
    if ( label == "smearOut" ) return &binEntry.smearOut_;

    edm::LogError ("ModelBinning::findBinEntry") << " Invalid label = " << label << " !!";
  } else {
    edm::LogError ("ModelBinning::findBinEntry") << " Invalid bin number = " << binNumber << ", numBins = " << numBins_ << " !!";
  }

  return 0;
}

void ModelBinning::decodeStringRep(std::vector<std::string>& buffer)
{
  BinningBase::decodeStringRep(buffer);

  TPRegexp regexpParser_numBins("numBins");
  TPRegexp regexpParser_binContents_entry(std::string("binContent_region[[:digit:]]+_[[:alnum:]]+").append(meOptionsBinContent));
  TPRegexp regexpParser_binContents_elements(std::string("binContent_region([[:digit:]]+)_([[:alnum:]]+)").append(meOptionsBinContent));
  TPRegexp regexpParser_binError_entry(std::string("binError_region[[:digit:]]+_[[:alnum:]]+").append(meOptionsBinError));
  TPRegexp regexpParser_binError_elements(std::string("binError_region([[:digit:]]+)_([[:alnum:]]+)").append(meOptionsBinError));

  bool numBins_initialized = false;
  std::vector<std::map<std::string, bool> > binContents_initialized;
  std::vector<std::map<std::string, bool> > binSumw2_initialized;
  
  for ( std::vector<std::string>::const_iterator entry = buffer.begin();
	entry != buffer.end(); ++entry ) {
    std::string meName, meType, meValue;
    int error = 0;
    decodeBinningStringRep(*entry, meName, meType, meValue, error);

    if ( error ) {
      edm::LogError ("ModelBinning::decodeStringRep") << " Error in parsing string = " << (*entry) << " --> skipping !!";
      continue;
    }

    TString meName_tstring = meName.data();
    
    bool binNumber_error = false;

    if ( regexpParser_numBins.Match(meName_tstring) == 1 ) {
      numBins_ = (unsigned)atoi(meValue.data());
      binEntries_.resize(numBins_);
      binContents_initialized.resize(numBins_);
      binSumw2_initialized.resize(numBins_);
      numBins_initialized = true;
    } else if ( regexpParser_binContents_entry.Match(meName_tstring) == 1 ) {
      if ( !numBins_initialized ) {
	edm::LogError ("decodeStringRep") << " Need to initialize numBins before setting binContents !!";
	continue;
      }
      
      TObjArray* subStrings = regexpParser_binContents_elements.MatchS(meName_tstring);
      if ( subStrings->GetEntries() == 3 ) {
	int binNumber = (unsigned)atoi(((TObjString*)subStrings->At(1))->GetString().Data()) - 1;
	std::string label = ((TObjString*)subStrings->At(2))->GetString().Data();
	float binContent = atof(meValue.data());
	
	if ( binNumber >= 0 && binNumber < (int)numBins_ ) {
	  binEntryType* binEntry = findBinEntry(binNumber, label);	  
	  if ( binEntry ) {
	    binEntry->binContent_ = binContent;
	    binContents_initialized[binNumber][label] = true;
	  } 
	} else {
	  edm::LogError ("decodeStringRep") << " Bin number = " << binNumber << " decoded from meName = " << meName
					    << " not within numBins = " << numBins_ << " range of binning object !!";
	  continue;
	}
      } else {
	binNumber_error = true;
      }
    } else if ( regexpParser_binError_entry.Match(meName_tstring) == 1 ) {
      if ( !numBins_initialized ) {
	edm::LogError ("ModelBinning::decodeStringRep") << " Need to initialize numBins before setting binError !!";
	continue;
      }

      TObjArray* subStrings = regexpParser_binError_elements.MatchS(meName_tstring);
      if ( subStrings->GetEntries() == 3 ) {
	int binNumber = (unsigned)atoi(((TObjString*)subStrings->At(1))->GetString().Data()) - 1;
	std::string label = ((TObjString*)subStrings->At(2))->GetString().Data();
	float binError = atof(meValue.data());

	if ( binNumber >= 0 && binNumber < (int)numBins_ ) {
	  binEntryType* binEntry = findBinEntry(binNumber, label);	  
	  if ( binEntry ) {
	    binEntry->binSumw2_ = binError*binError;
	    binSumw2_initialized[binNumber][label] = true;
	  } 
	} else {
	  edm::LogError ("decodeStringRep") << " Bin number = " << binNumber << " decoded from meName = " << meName
					    << " not within numBins = " << numBins_ << " range of binning object !!";
	  continue;
	}
      } else {
	binNumber_error = true;
      }
    }

    if ( binNumber_error ) {
      edm::LogError ("ModelBinning::decodeStringRep") << " Failed to decode bin number from meName = " << meName << " !!";
      continue;
    }
  }

//--- check that all data-members of ModelBinning object 
//    have been initialized
  if ( numBins_initialized ) {
    std::vector<std::string> defLabels;
    defLabels.push_back("gen");
    defLabels.push_back("rec");
    defLabels.push_back("stay");
    defLabels.push_back("lost");
    defLabels.push_back("smearIn");
    defLabels.push_back("smearOut");
    for ( unsigned iBin = 0; iBin < numBins_; ++iBin ) {
      for ( std::vector<std::string>::const_iterator label = defLabels.begin();
	    label != defLabels.end(); ++label ) {
	if ( !binContents_initialized[iBin][*label] ) 
	  edm::LogError ("decodeStringRep") << " Failed to decode binContents[" << iBin << "], label = " << (*label) << " !!";
	if ( !binSumw2_initialized[iBin][*label] ) 
	  edm::LogError ("decodeStringRep") << " Failed to decode binError[" << iBin << "], label = " << (*label) << " !!";
      }
    }
  } else {
    edm::LogError ("ModelBinning::decodeStringRep") << " Failed to decode numBins !!";
  }
}


