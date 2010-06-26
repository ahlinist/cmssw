#include "TauAnalysis/Core/interface/BinGrid.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/Core/interface/binningAuxFunctions.h"

#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>

BinGrid::BinGrid()
{
  numDimensions_ = 0;
  numBinsTotal_ = 0;
}

BinGrid::BinGrid(const edm::ParameterSet& cfg)
{
  //std::cout << "<BinGrid::BinGrid>:" << std::endl;

  numDimensions_ = 0;
  
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgBinning = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfg1dObjVar = cfgBinning.begin(); 
	cfg1dObjVar != cfgBinning.end(); ++cfg1dObjVar ) {
    std::string objVarName = ( cfg1dObjVar->exists("branchName") ) ? 
      cfg1dObjVar->getParameter<std::string>("branchName") : "undefined";
    objVarNames_.push_back(objVarName);

    edm::ParameterSet cfg1dBinning = cfg1dObjVar->getParameter<edm::ParameterSet>("binning");
    
    double xMin = cfg1dBinning.getParameter<double>("min");
    double xMax = cfg1dBinning.getParameter<double>("max");

    typedef std::vector<double> vdouble;
    vdouble binBoundaries = ( cfg1dBinning.exists("boundaries") ) ? 
      cfg1dBinning.getParameter<vdouble>("boundaries") : vdouble();
    
    unsigned numBinBoundaries = binBoundaries.size();
    unsigned numBins_i = numBinBoundaries + 1;
    vdouble binEdges_i(numBins_i + 1);
    binEdges_i[0] = xMin;
    for ( unsigned iBin = 0; iBin < numBinBoundaries; ++iBin ) {
      binEdges_i[iBin + 1] = binBoundaries[iBin];
    }
    binEdges_i[numBins_i] = xMax;

    binEdges_.push_back(binEdges_i);
    ++numDimensions_;
  }

//--- initialize numBinsPerDimension and dimValues
//    data-members for encoding/decoding of bin numbers
  initialize();

  //print(std::cout);
}

BinGrid::BinGrid(const BinGrid& bluePrint)
  : numDimensions_(bluePrint.numDimensions_),
    objVarNames_(bluePrint.objVarNames_),
    binEdges_(bluePrint.binEdges_),
    numBinsPerDimension_(bluePrint.numBinsPerDimension_),
    numBinsTotal_(bluePrint.numBinsTotal_),
    dimValues_(bluePrint.dimValues_)
{}

BinGrid::~BinGrid()
{
//--- nothing to be done yet...
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void BinGrid::initialize()
{
  //std::cout << "<BinGrid::initialize>:" << std::endl;
  //std::cout << " numDimensions = " << numDimensions_ << std::endl;

  numBinsPerDimension_.resize(numDimensions_);
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    assert(binEdges_[iDimension].size() >= 2);
    numBinsPerDimension_[iDimension] = binEdges_[iDimension].size() - 1;
  }

//--- initialize "value" of each dimension i, defined by the product:
//
//      numBins(i + 1) * numBins(i + 2) * .. * numBins(numDimensions - 1)
//
  dimValues_.resize(numDimensions_);
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    dimValues_[iDimension] = getDimValue(iDimension);
  }

//--- compute total number of bins in multi-dimensional grid
  numBinsTotal_ = 1;
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    //std::cout << "iDimension = " << iDimension << ": numBinsPerDimension = " << numBinsPerDimension_[iDimension] << std::endl;
    numBinsTotal_ *= numBinsPerDimension_[iDimension];
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

unsigned BinGrid::encodeTotBin(const vunsigned& binIndices) const
{
  assert(binIndices.size() == numDimensions_);

  unsigned totBin = 0;
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    totBin += binIndices[iDimension] * dimValues_[iDimension];
  }

  return totBin;
}

std::vector<unsigned> BinGrid::decodeTotBin(unsigned totBin) const
{
  assert(totBin >= 0 && totBin < numBinsTotal_);

  vunsigned binIndices(numDimensions_);

  unsigned totBin_undecoded = totBin;
  for ( int iDimension = (numDimensions_ - 1); iDimension >= 0; --iDimension ) {
    unsigned dimValue_i = dimValues_[iDimension];
    unsigned binIndex = (totBin_undecoded / dimValue_i);
    binIndices[iDimension] = binIndex;
    totBin_undecoded -= (dimValue_i * binIndex);
  }

  assert(totBin_undecoded == 0);

  return binIndices;
}

unsigned BinGrid::getDimValue(unsigned i) const
{
  assert(i >= 0 && i < numDimensions_);

  unsigned dimValue = 1;
  for ( unsigned iDimension = 0; iDimension < i; ++iDimension ) {
    dimValue *= numBinsPerDimension_[iDimension];
  }

  return dimValue;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

int BinGrid::binNumber(const std::vector<double>& x) const
{
//--- check that vector x given as function argument
//    is of the same dimensionality as the bin-grid
  if ( x.size() != numDimensions_ ) {
    edm::LogError ("BinGrid::getBin") << " Invalid dimensionality = " << x.size() << " of vector x given as function argument," 
				      << " expected dimensionality = " << numDimensions_ << " !!";
    return -1;
  }

//--- compute bin value;
//    the bin value is computed as sum over values k_i calculated separately for each dimension i as:
//
//      k_i = bin_i * numBins(i + 1) * numBins(i + 2) * .. * numBins(numDimensions - 1)
//
//    where bin_i is taken as the index of the "left" bin-edge for which 
//
//      binEdges[i,k] < x[i] < binEdges[i,k + 1]
//
  vunsigned binIndices(numDimensions_);
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    const vdouble& binEdges_i = binEdges_[iDimension];

    bool xInRange = false;

    unsigned numBins_i = numBinsPerDimension_[iDimension];

    for ( unsigned iBin = 0; iBin < numBins_i; ++iBin ) {
      if ( x[iDimension] >= binEdges_i[iBin] && x[iDimension] < binEdges_i[iBin + 1] ) {
	binIndices[iDimension] = iBin;
	xInRange = true;
	break;
      }
    }

    if ( !xInRange ) {
      edm::LogWarning ("BinGrid::getBin") << " Value x[" << iDimension << "] = " << x[iDimension] << " outside Range"
					  << " [xMin, xMax[ = [" << binEdges_i[0] << ", " << binEdges_i[numBins_i] << "[ !!";
      return -1;
    }
  }

  return encodeTotBin(binIndices);
}
 
//
//-----------------------------------------------------------------------------------------------------------------------
//

std::vector<double> BinGrid::binCenter(unsigned totBin) const
{
  if ( totBin >= 0 && totBin < numBinsTotal_ ) {
    std::vector<double> binCenter(numDimensions_);

    vunsigned binIndices = decodeTotBin(totBin);
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      const vdouble& binEdges_i = binEdges_[iDimension];
      unsigned binIndex_i = binIndices[iDimension];
      binCenter[iDimension] = 0.5*(binEdges_i[binIndex_i] + binEdges_i[binIndex_i + 1]);
    } 

    return binCenter; 
  } else {
    edm::LogError ("BinGrid::binCenter") << " Invalid bin = " << totBin << " !!";
    return std::vector<double>(numDimensions_);
  }
}

std::vector<double> BinGrid::binEdges(unsigned dimension) const
{
  if ( dimension >= 0 && dimension < numDimensions_ ) {
    return binEdges_[dimension];
  } else {
    edm::LogError ("BinGrid::binCenter") << " Invalid dimension = " << dimension << " !!";
    return std::vector<double>();
  }
}

double BinGrid::binVolume(unsigned totBin) const
{
  if ( totBin >= 0 && totBin < numBinsTotal_ ) {
    double binVolume = 1.;

    vunsigned binIndices = decodeTotBin(totBin);
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      const vdouble& binEdges_i = binEdges_[iDimension];
      unsigned binIndex_i = binIndices[iDimension];
      double binWidth = binEdges_i[binIndex_i + 1] - binEdges_i[binIndex_i];
      binVolume *= binWidth;
    }

    return binVolume; 
  } else {
    edm::LogError ("BinGrid::binVolume") << " Invalid bin = " << totBin << " !!";
    return 0.;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void BinGrid::print(std::ostream& stream) const
{
  stream << "<BinGrid::print>:" << std::endl;

  stream << "numDimensions = " << numDimensions_ << std::endl;
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    stream << " dimension = " << iDimension << ": " << objVarNames_[iDimension] << std::endl;
    stream << "  numBins = " << numBinsPerDimension_[iDimension] << std::endl;
    stream << "  binEdges = { ";
    for ( unsigned iBin = 0; iBin < numBinsPerDimension_[iDimension]; ++iBin ) {
      stream << binEdges_[iDimension][iBin] << ", ";
    }
    stream << binEdges_[iDimension][numBinsPerDimension_[iDimension]] << " }" << std::endl;
  }

  stream << "numBins(total) = " << numBinsTotal_ << std::endl;
  for ( unsigned iBin = 0; iBin < numBinsTotal_; ++iBin ) {
    stream << " bin = " << iBin << std::endl;
    std::vector<double> binCenter_i = binCenter(iBin);
    stream << "  binCenter = { ";
    for ( unsigned iDimension = 0; iDimension < (numDimensions_ - 1); ++iDimension ) {
      stream << binCenter_i[iDimension] << ", ";
    }
    stream << binCenter_i[numDimensions_ - 1] << " }" << std::endl;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

std::vector<std::string> BinGrid::encodeStringRep() const
{
//--- save all data-members of BinGrid object as strings, 
//    so that data-members of BinGrids associated to different Binning objects 
//    do not get summed when adding MonitorElements during DQM harvesting

  std::vector<std::string> buffer;
  
  std::ostringstream meValue_dimensions;
  meValue_dimensions << numDimensions_;
  std::string entry_dimensions = encodeBinningStringRep("numDimensions", "string",  meValue_dimensions.str());
  buffer.push_back(entry_dimensions);

  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    std::ostringstream meName_binEdges;
    meName_binEdges << "binEdges" << (iDimension + 1);
    std::string meValue_binEdges = encodeVDoubleStringRep(binEdges_[iDimension]);
    std::string entry_binEdges = encodeBinningStringRep(meName_binEdges.str(), "string", meValue_binEdges);
    buffer.push_back(entry_binEdges);

    std::ostringstream meName_objVarName;
    meName_objVarName << "objVarName" << (iDimension + 1);
    std::string meValue_objVarName = objVarNames_[iDimension];
    std::string entry_objVarName = encodeBinningStringRep(meName_objVarName.str(), "string", meValue_objVarName);
    buffer.push_back(entry_objVarName);
  }

  std::ostringstream meValue_numBins;
  meValue_numBins << numBinsTotal_;
  std::string entry_numBins = encodeBinningStringRep("numBins", "string",  meValue_numBins.str());
  buffer.push_back(entry_numBins);

  return buffer;
}

void BinGrid::decodeStringRep(std::vector<std::string>& buffer)
{
  TPRegexp regexpParser_numDimensions("numDimensions");
  TPRegexp regexpParser_binEdges_entry("binEdges[[:digit:]]+");
  TPRegexp regexpParser_binEdges_binNumber("binEdges([[:digit:]]+)");
  TPRegexp regexpParser_objVarName_entry("objVarName[[:digit:]]+");
  TPRegexp regexpParser_objVarName_binNumber("objVarName([[:digit:]]+)");

  bool numDimensions_initialized = false;
  std::vector<bool> binEdges_initialized;
  std::vector<bool> objVarNames_initialized;
  
  for ( std::vector<std::string>::const_iterator entry = buffer.begin();
	entry != buffer.end(); ++entry ) {
    //std::cout << "entry = " << (*entry) << std::endl;
    std::string meName, meType, meValue;
    int error = 0;
    decodeBinningStringRep(*entry, meName, meType, meValue, error);

    if ( error ) {
      edm::LogError ("BinGrid::decodeStringRep") << " Error in parsing string = " << (*entry) << " --> skipping !!";
      continue;
    }

    TString meName_tstring = meName.data();
    //std::cout << "meName_tstring = " << meName_tstring << std::endl;
    
    bool binNumber_error = false;

    if ( regexpParser_numDimensions.Match(meName_tstring) == 1 ) {
      //std::cout << "--> matches numDimensions" << std::endl;
      unsigned numDimensions = (unsigned)atoi(meValue.data());
      numDimensions_ = numDimensions;
      binEdges_.resize(numDimensions_);
      objVarNames_.resize(numDimensions_);
      binEdges_initialized.resize(numDimensions_);
      objVarNames_initialized.resize(numDimensions_);
      numDimensions_initialized = true;
    } else if ( regexpParser_binEdges_entry.Match(meName_tstring) == 1 ) {
      //std::cout << "--> matches binEdges" << std::endl;
      if ( !numDimensions_initialized ) {
	edm::LogError ("BinGrid::decodeStringRep") << " Need to initialize numDimensions before setting binEdges !!";
	continue;
      }
      
      TObjArray* subStrings = regexpParser_binEdges_binNumber.MatchS(meName_tstring);
      if ( subStrings->GetEntries() == 2 ) {
	int dimension = (unsigned)atoi(((TObjString*)subStrings->At(1))->GetString().Data()) - 1;

	if ( !(dimension >= 0 && dimension < (int)numDimensions_) ) {
	  edm::LogError ("BinGrid::decodeStringRep") << " Dimension = " << dimension << " decoded from meName = " << meName
						     << " not within numDimensions = " << numDimensions_ << " range of binGrid object !!";
	  continue;
	}

	int error = 0;
	std::vector<double> binEdges = decodeVDoubleStringRep(meValue, error);

	if ( !error ) {
	  binEdges_[dimension] = binEdges;
	  binEdges_initialized[dimension] = true;
	} else {
	  edm::LogError ("BinGrid::decodeStringRep") << " Failed to decode meValue = " << meValue << " !!";
	  continue;
	}
      } else {
	binNumber_error = true;
      }
    } else if ( regexpParser_objVarName_entry.Match(meName_tstring) == 1 ) {
      //std::cout << "--> matches objVarName" << std::endl;
      if ( !numDimensions_initialized ) {
	edm::LogError ("BinGrid::decodeStringRep") << " Need to initialize numDimensions before setting objVarNames !!";
	continue;
      }

      TObjArray* subStrings = regexpParser_objVarName_binNumber.MatchS(meName_tstring);
      if ( subStrings->GetEntries() == 2 ) {
	int dimension = (unsigned)atoi(((TObjString*)subStrings->At(1))->GetString().Data()) - 1;

	if ( dimension >= 0 && dimension < (int)numDimensions_ ) {
	  objVarNames_[dimension] = meValue;
	  objVarNames_initialized[dimension] = true;
	} else {
          edm::LogError ("BinGrid::decodeStringRep") << " Dimension = " << dimension << " decoded from meName = " << meName
						     << " not within numDimensions = " << numDimensions_ << " range of binGrid object !!";
	  continue;
	}
      } else {
	binNumber_error = true;
      }
    }

    if ( binNumber_error ) {
      edm::LogError ("BinGrid::decodeStringRep") << " Failed to decode bin number from meName = " << meName << " !!";
      continue;
    }
  }

//--- check that all data-members of BinGrid object 
//    have been initialized
  if ( numDimensions_initialized ) {
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      if ( !binEdges_initialized[iDimension] ) {
	edm::LogError ("BinGrid::decodeStringRep") << " Failed to decode binEdges[" << iDimension << "] !!";
      }

      if ( !objVarNames_initialized[iDimension] ) {
	edm::LogError ("BinGrid::decodeStringRep") << " Failed to decode objVarName[" << iDimension << "] !!";
      }
    }
  } else {
    edm::LogError ("BinGrid::decodeStringRep") << " Failed to decode numDimensions !!";
  }

//--- initialize numBinsPerDimension and dimValues
//    data-members for encoding/decoding of bin numbers
  initialize();
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

std::vector<std::string>& operator<<(std::vector<std::string>& buffer, const BinGrid& binGrid)
{
  std::vector<std::string> buffer_ext = binGrid.encodeStringRep();

  unsigned numEntries = buffer_ext.size();
  for ( unsigned iEntry = 0; iEntry < numEntries; ++iEntry ) {
    buffer.push_back(buffer_ext[iEntry]);
  }

  return buffer;
}

std::vector<std::string>& operator>>(std::vector<std::string>& buffer, BinGrid& binGrid)
{
  binGrid.decodeStringRep(buffer);

  return buffer;
}


