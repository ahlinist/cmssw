#include "TauAnalysis/FittingTools/plugins/DQMExportAnalysisResults.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/histogramAuxFunctions.h"
#include "TauAnalysis/Core/interface/SysUncertaintyService.h"

#include <TMath.h>
#include <TH1.h>

#include <iostream>
#include <iomanip>
#include <fstream>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

const std::string systematicsDirKeyword = "#SYSTEMATICSDIR#";
const std::string channelOutputFileNameKeyword = "#CHANNEL_OUTPUTFILENAME#";
const std::string outputFileNameSeparator = "/";

mode_t mkdirAccessPermissions = (S_IRWXU | S_IRWXG | S_IRWXO);

const double epsilon = 1.e-2;

typedef std::vector<std::string> vstring;
typedef std::vector<edm::ParameterSet> vParameterSet;

DQMExportAnalysisResults::DQMExportAnalysisResults(const edm::ParameterSet& cfg)
{
  std::cout << "<DQMExportAnalysisResults::DQMExportAnalysisResults>:" << std::endl;

  vParameterSet cfgChannels = cfg.getParameter<vParameterSet>("channels");
  unsigned channelIndex = 0;
  for ( vParameterSet::const_iterator cfgChannel = cfgChannels.begin();
	cfgChannel != cfgChannels.end(); ++cfgChannel ) {
    channelEntryType* channelEntry = new channelEntryType(channelIndex, *cfgChannel);
    channels_.push_back(channelEntry);
    ++channelIndex;
  }

  outputFilePath_ = cfg.getParameter<std::string>("outputFilePath");

  edm::ParameterSet cfgProcesses = cfg.getParameter<edm::ParameterSet>("processes");
  vstring processNames = cfgProcesses.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator processName = processNames.begin();
	processName != processNames.end(); ++processName ) {
    edm::ParameterSet cfgProcess = cfgProcesses.getParameter<edm::ParameterSet>(*processName);
    processEntryType* processEntry = new processEntryType(*processName, cfgProcess);
    processes_.push_back(processEntry);
  }

  if ( cfg.exists("systematics") ) {
    edm::ParameterSet cfgSystematics = cfg.getParameter<edm::ParameterSet>("systematics");
    vstring systematicNames = cfgSystematics.getParameterNamesForType<edm::ParameterSet>();
    for ( vstring::const_iterator systematicName = systematicNames.begin();
	  systematicName != systematicNames.end(); ++systematicName ) {
      edm::ParameterSet cfgSystematic = cfgSystematics.getParameter<edm::ParameterSet>(*systematicName);      
      systematicEntryType* systematicEntry = new systematicEntryType(*systematicName, cfgSystematic);
      systematics_.push_back(systematicEntry);
    }
  }
}

DQMExportAnalysisResults::~DQMExportAnalysisResults()
{
  for ( std::vector<channelEntryType*>::iterator it = channels_.begin();
	it != channels_.end(); ++it ) {
    delete (*it);
  }

  for ( std::vector<processEntryType*>::iterator it = processes_.begin();
	it != processes_.end(); ++it ) {
    delete (*it);
  }

  for ( std::vector<systematicEntryType*>::iterator it = systematics_.begin();
	it != systematics_.end(); ++it ) {
    delete (*it);
  }
}

void createSubDirectories(const std::string& outputFileName, int& errorFlag)
{
  //std::cout << "<createSubDirectories>:" << std::endl;
  //std::cout << " outputFileName = " << outputFileName << std::endl;

  size_t index = outputFileName.find(outputFileNameSeparator, 1);
  while ( index < outputFileName.length() ) {
    std::string outputFilePath = std::string(outputFileName, 0, index);
    //std::cout << " outputFilePath = " << outputFilePath << std::endl;

    struct stat statBuffer;
    int statError = stat(outputFilePath.data(), &statBuffer);
    if ( statError != 0 ) {
      if ( errno == ENOENT ) { // directory does not exist --> create it
	std::cout << "--> directory = " << outputFilePath << " does not yet exist, creating it..." << std::endl;
	int mkdirError = mkdir(outputFilePath.data(), mkdirAccessPermissions);
	if ( mkdirError != 0 ) {
	  edm::LogError ("createSubDirectories") 
	    << " Failed to create directory = " << outputFilePath << " !!";
	  errorFlag = 1;
	}
      } else if ( errno == ENOENT ) { // directory/file marked as "bad" --> print error message
	edm::LogError ("createSubDirectories") 
	  << " Directory = " << outputFilePath << " marked as 'bad' !!";
	errorFlag = 1;
      }
    } else { // directory/file exists, check that it is indeed a directory
      if ( !(statBuffer.st_mode & S_IFDIR) ) {
	edm::LogError ("createSubDirectories") 
	  << " Invalid function argument outputFileName = " << outputFileName << ":" 
	  << " outputFilePath = " << outputFilePath << " is not a directory !!";
	errorFlag = 1;
      }
    }

    if ( errorFlag ) return;

    index = outputFileName.find(outputFileNameSeparator, index + 1);
  }
}

int getFirstBin(TH1* histogram, const std::string& axis)
{
  return 0; // return 0/1 to include/exclude underflow bin
}

int getLastBin(TH1* histogram, const std::string& axis)
{
  int numBins = 0;
  if      ( axis == "x" || axis == "X" ) numBins = histogram->GetNbinsX();
  else if ( axis == "y" || axis == "Y" ) numBins = histogram->GetNbinsY();
  else if ( axis == "z" || axis == "Z" ) numBins = histogram->GetNbinsZ();
  else {
    edm::LogError ("getLastBin")
      << " Invalid function argument axis = " << axis << " !!";
    return -1;
  }
  
  return numBins + 1; // return numBins/(numBins + 1) to include/exclude overflow bin
}

int getNumBins(TH1* histogram, const std::string& axis)
{
  return (getLastBin(histogram, axis) - getFirstBin(histogram, axis)) + 1;
}

int getNumBins(TH1* histogram)
{
  int numBins = getNumBins(histogram, "X");
  if ( histogram->GetDimension() >= 2 ) numBins *= getNumBins(histogram, "Y");
  if ( histogram->GetDimension() >= 3 ) numBins *= getNumBins(histogram, "Z");
  return numBins;
}

double getSumBinContents(TH1* histogram, int firstBinX, int lastBinX, int firstBinY, int lastBinY, int firstBinZ, int lastBinZ)
{
  double sumBinContents = 0.;
  for ( int iBinX = firstBinX; iBinX <= lastBinX; ++iBinX ) {
    for ( int iBinY = firstBinY; iBinY <= lastBinY; ++iBinY ) {
      for ( int iBinZ = firstBinZ; iBinZ <= lastBinZ; ++iBinZ ) {
	double binContent = 0.;
	if      ( histogram->GetDimension() == 1 ) binContent = histogram->GetBinContent(iBinX);
	else if ( histogram->GetDimension() == 2 ) binContent = histogram->GetBinContent(iBinX, iBinY);
	else if ( histogram->GetDimension() == 3 ) binContent = histogram->GetBinContent(iBinX, iBinY, iBinZ);
	else assert(0);
	
	sumBinContents += binContent;
      }
    }
  }

  return sumBinContents;
}

double getSumBinErrors2(TH1* histogram, int firstBinX, int lastBinX, int firstBinY, int lastBinY, int firstBinZ, int lastBinZ)
{
  double sumBinErrors2 = 0.;
  for ( int iBinX = firstBinX; iBinX <= lastBinX; ++iBinX ) {
    for ( int iBinY = firstBinY; iBinY <= lastBinY; ++iBinY ) {
      for ( int iBinZ = firstBinZ; iBinZ <= lastBinZ; ++iBinZ ) {
	double binError = 0.;
	if      ( histogram->GetDimension() == 1 ) binError = histogram->GetBinError(iBinX);
	else if ( histogram->GetDimension() == 2 ) binError = histogram->GetBinError(iBinX, iBinY);
	else if ( histogram->GetDimension() == 3 ) binError = histogram->GetBinError(iBinX, iBinY, iBinZ);
	else assert(0);
	
	sumBinErrors2 += (binError*binError);
      }
    }
  }

  return sumBinErrors2;
}

void exportAnalysisResults(
       DQMStore& dqmStore, 
       const std::string& meNameTemplate, const std::string& meNameNumEventsProcessed, const std::string& meNameNumEventsPassed, 
       double xSection, double dataIntLumi,
       unsigned numChannels, unsigned binOffset, unsigned numBinsTotal,
       const std::string& outputFileName, bool failSilent = false)
{
  std::cout << "<exportAnalysisResults>:" << std::endl;
  std::cout << " meNameTemplate = " << meNameTemplate << std::endl;
  std::cout << " xSection = " << xSection << std::endl;
  std::cout << " dataIntLumi = " << dataIntLumi << std::endl;
  std::cout << " outputFileName = " << outputFileName << std::endl;
  std::cout << " binOffset = " << binOffset << std::endl;

  std::vector<int> binContents(numBinsTotal);
  for ( unsigned iBin = 0; iBin < numBinsTotal; ++iBin ) {
    binContents[iBin] = 0;		
  }

  double numEventsProcessed, numEventsPassed;
  if ( meNameTemplate == "" && meNameNumEventsProcessed == "" && meNameNumEventsPassed == "" ) { // create "empty" (dummy) file
    numEventsProcessed = 1000.;
    numEventsPassed = 0.;
  } else {
    MonitorElement* me = dqmStore.get(meNameTemplate);
    TH1* histogram = ( me ) ? me->getTH1() : NULL;
    if ( histogram == NULL ) {
      if ( !failSilent ) 
	edm::LogError ("exportAnalysisResults") 
	  << " Failed to access dqmMonitorElement = " << meNameTemplate 
	  << " --> analysis results will NOT be exported !!";
      return;
    }

    bool error = false;
    numEventsProcessed = getValue(dqmStore, meNameNumEventsProcessed, error);
    std::cout << " numEventsProcessed = " << numEventsProcessed << std::endl;
    numEventsPassed = getValue(dqmStore, meNameNumEventsPassed, error);
    std::cout << " numEventsPassed = " << numEventsPassed << std::endl;
    assert(!error);

//--- correct for skimming efficiency
//   (by scaling-up number of processed events such that
//    number of processed events = integrated luminosity of analyzed dataset * cross-section)
    if ( xSection > 0. && dataIntLumi > 0. ) {
      double effXsection = numEventsProcessed/dataIntLumi;
      std::cout << "--> scaling number of processed events up by factor = " << (xSection/effXsection) << ","
		<< " in order to account for skimming efficiencies !!" << std::endl;
      std::cout << " xSection = " << xSection << std::endl;
      std::cout << " effXsection = " << effXsection << std::endl;
      numEventsProcessed *= (xSection/effXsection);
    } 

//--- read bin-contents, bin-errors
    int firstBinX = getFirstBin(histogram, "X");
    int lastBinX = getLastBin(histogram, "X");

    int firstBinY, lastBinY;
    if ( histogram->GetDimension() >= 2 ) {
      firstBinX = getFirstBin(histogram, "Y");
      lastBinY = getLastBin(histogram, "Y");
    } else {
      firstBinY = 1;
      lastBinY = 1;
    }

    int firstBinZ, lastBinZ;
    if ( histogram->GetDimension() >= 3 ) {
      firstBinZ = getFirstBin(histogram, "Z");
      lastBinZ = getLastBin(histogram, "Z");
    } else {
      firstBinZ = 1;
      lastBinZ = 1;
    }

    double sumBinContents = getSumBinContents(histogram, firstBinX, lastBinX, firstBinY, lastBinY, firstBinZ, lastBinZ);
    //std::cout << " sumBinContents = " << sumBinContents << std::endl;

    double sumBinErrors2 = getSumBinErrors2(histogram, firstBinX, lastBinX, firstBinY, lastBinY, firstBinZ, lastBinZ);
    //std::cout << " sumBinErrors2 = " << sumBinErrors2 << std::endl;

//--- scale (weighted) number of events expected for luminosity of analyzed dataset
//    to "effective" number of events Neff for which 
//      sumBinContents/sqrt(sumBinErrors2) = sqrt(Neff)
//    corresponding to number of events needed to reach same level of statistical precision
//    in case all events would have unit weight
    double scaleFactor = sumBinContents/sumBinErrors2;
    std::cout << " scaleFactor = " << scaleFactor << std::endl;

    int binIndex = 0;
    for ( int iBinX = firstBinX; iBinX <= lastBinX; ++iBinX ) {
      for ( int iBinY = firstBinY; iBinY <= lastBinY; ++iBinY ) {
	for ( int iBinZ = firstBinZ; iBinZ <= lastBinZ; ++iBinZ ) {
	  double binContent = 0.;
	  if      ( histogram->GetDimension() == 1 ) binContent = histogram->GetBinContent(iBinX);
	  else if ( histogram->GetDimension() == 2 ) binContent = histogram->GetBinContent(iBinX, iBinY);
	  else if ( histogram->GetDimension() == 3 ) binContent = histogram->GetBinContent(iBinX, iBinY, iBinZ);
	  else assert(0);
	  
	  binContent *= scaleFactor;
	  
	  binContents[binIndex + binOffset] = TMath::Nint(binContent);
	  ++binIndex;
	}
      }
    }

    std::cout << " binContents = " << format_vint(binContents) << std::endl;

    numEventsProcessed *= scaleFactor;
    numEventsPassed *= scaleFactor;

    double consistencyCheckNumerator = TMath::Abs(numEventsPassed - sumBinContents*scaleFactor);
    double consistencyCheckDenominator = 0.5*(TMath::Abs(numEventsPassed) + TMath::Abs(sumBinContents*scaleFactor));
    if ( consistencyCheckDenominator != 0. && (consistencyCheckNumerator/consistencyCheckDenominator) > epsilon ) {
      edm::LogWarning ("exportAnalysisResults")
	<< " Mismatch between number of events passing cuts and sum of entries in histogram !!";
      std::cout << "(num. events passed (meName = " << meNameNumEventsPassed << "): " << numEventsPassed << ","
		<< " sum of entries in histogram (meName = " << meNameTemplate << "): " << sumBinContents*scaleFactor << ")" 
		<< std::endl;
      edm::LogInfo ("exportAnalysisResults")
	<< " Assuming that histograms were summed by 'hadd' instead of harvested properly using DQM tools"
        << " --> scaling numbers to match integral of histogram !!";
      numEventsProcessed *= (sumBinContents*scaleFactor/numEventsPassed);
      numEventsPassed *= (sumBinContents*scaleFactor/numEventsPassed);
    }
  }

  int errorFlag = 0;
  createSubDirectories(outputFileName, errorFlag);
  if ( errorFlag ) {
    if ( !failSilent )
      edm::LogError ("exportAnalysisResults") 
	<< " Failed to create directory structure" 
	<< " --> analysis results will NOT be exported !!";
    return;
  }

  ostream* outputFile = new std::ofstream(outputFileName.data(), std::ios::out);

  unsigned width = 8;           // 8 characters per number, right justified
  unsigned numbersPerLine = 10; // max. 10 numbers per line

  unsigned numBinsPerChannel = (numBinsTotal / numChannels);

  (*outputFile) << " " << std::setw(width) << std::setfill(' ') << numChannels;
  (*outputFile) << " " << std::setw(width) << std::setfill(' ') << numBinsPerChannel;
  (*outputFile) << " " << std::setw(width) << std::setfill(' ') << TMath::Nint(numEventsProcessed);
  (*outputFile) << " " << std::setw(width) << std::setfill(' ') << TMath::Nint(numEventsPassed);
  (*outputFile) << std::endl;

  bool isEndLineTerminated = true;
  for ( unsigned iChannel = 0; iChannel < numChannels; ++iChannel ) {
    for ( unsigned iBin = 0; iBin < numBinsPerChannel; ++iBin ) {
      (*outputFile) << " " << std::setw(width) << std::setfill(' ') << binContents[iChannel*numBinsPerChannel + iBin];
      isEndLineTerminated = false;
      if ( ((iBin + 1) % numbersPerLine) == 0 ) {
	(*outputFile) << std::endl;
	isEndLineTerminated = true;
      }
    }
    
    if ( !isEndLineTerminated ) {
      (*outputFile) << std::endl;
      isEndLineTerminated = true;
    }
  }
  
  delete outputFile;
}

std::string getMEname_full(const std::string& meName_channel, const std::string& systematic)
{
  std::string meName_full = meName_channel;
  int errorFlag = 0;
  meName_full = replace_string(meName_full, systematicsDirKeyword, systematic, 0, 1, errorFlag);
  meName_full = replace_string(meName_full, "//", "/", 0, 1000, errorFlag);
  return meName_full;
}

void DQMExportAnalysisResults::endJob()
{
  std::cout << "<DQMExportAnalysisResults::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") 
      << " Failed to access dqmStore" 
      << " --> histograms will NOT be exported !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());  

  numChannels_ = channels_.size();
  std::cout << " numChannels = " << numChannels_ << std::endl;

//--- check that binning is compatible for all channels
  TH1* refHistogramBinning = 0;
  for ( std::vector<channelEntryType*>::const_iterator channel = channels_.begin();
	channel != channels_.end(); ++channel ) {
    std::string meNameBinning = getMEname_full((*channel)->meNameBinning_, SysUncertaintyService::getNameCentralValue());		
    bool dqmError = false;
    (*channel)->histogramBinning_ = getHistogram(dqmStore, meNameBinning, dqmError);
    if ( dqmError ) {
      edm::LogError ("endJob") 
	<< " Failed to access MonitorElement name = " << meNameBinning
	<< " --> histograms will NOT be exported !!";
      return;
    }

    if ( !refHistogramBinning ) {
      refHistogramBinning = (*channel)->histogramBinning_;
    } else {
      if ( !isCompatibleBinning(refHistogramBinning, (*channel)->histogramBinning_) ) {
	edm::LogError ("endJob")
	  << " Template histograms for different channels have incompatible binning" 
	  << " --> histograms will NOT be exported !!";
      }
    }

    unsigned numBins_channel = getNumBins((*channel)->histogramBinning_);

    binOffsets_.insert(std::pair<unsigned, unsigned>((*channel)->index_, numBinsTotal_));
    numBinsTotal_ += numBins_channel;
  }

  std::cout << " binOffsets = " << "{";
  for ( std::map<unsigned, unsigned>::const_iterator binOffset = binOffsets_.begin();
	binOffset != binOffsets_.end(); ++binOffset ) {
    if ( binOffset != binOffsets_.begin() ) std::cout << ",";
    std::cout << " " << binOffset->first << ":" << binOffset->second;
  }
  std::cout << " }" << std::endl;
  
  for ( std::vector<processEntryType*>::iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    for ( std::vector<channelEntryType*>::iterator channel = channels_.begin();
	  channel != channels_.end(); ++channel ) {
      int errorFlag = 0;

//--- export "central values" 
//   (analysis results with no systematic shifts applied)
      std::string outputFileName_channel = std::string(outputFilePath_).append("/");
      outputFileName_channel.append((*process)->outputFilePath_).append("/");
      outputFileName_channel.append((*process)->outputFileName_);
      outputFileName_channel = 
	replace_string(outputFileName_channel, channelOutputFileNameKeyword, (*channel)->shortName_, 0, 1, errorFlag);
      outputFileName_channel = replace_string(outputFileName_channel, "//", "/", 0, 1000, errorFlag);
      std::cout << " outputFileName_channel = " << outputFileName_channel << std::endl;

//--- check if process is signal/background for channel:
//    if it is, export distribution; else create "empty" (dummy) file
      distributionEntryType* distribution = 0;
      if ( (*process)->distributions_.find((*channel)->name_) != (*process)->distributions_.end() ) {
	distribution = (*process)->distributions_.find((*channel)->name_)->second;

	std::string meNameTemplate_channel = 
	  getMEname_full(distribution->meNameTemplate_, SysUncertaintyService::getNameCentralValue());
	std::cout << " meNameTemplate_channel = " << meNameTemplate_channel << std::endl;

	std::string meNameNumEventsProcessed_channel = getMEname_full(distribution->meNameNumEventsProcessed_, "");
	std::cout << " meNameNumEventsProcessed_channel = " << meNameNumEventsProcessed_channel << std::endl;
        std::string meNameNumEventsPassed_channel = 
          getMEname_full(distribution->meNameNumEventsPassed_, SysUncertaintyService::getNameCentralValue());
	std::cout << " meNameNumEventsPassed_channel = " << meNameNumEventsPassed_channel << std::endl;

	exportAnalysisResults(dqmStore, 
			      meNameTemplate_channel, meNameNumEventsProcessed_channel, meNameNumEventsPassed_channel, 
			      (*process)->xSection_, (*channel)->dataIntLumi_,
			      numChannels_, binOffsets_[(*channel)->index_], numBinsTotal_, outputFileName_channel, false);
      } else {
	exportAnalysisResults(dqmStore, 
			      "", "", "", 
			      (*process)->xSection_, (*channel)->dataIntLumi_,
			      numChannels_, binOffsets_[(*channel)->index_], numBinsTotal_, outputFileName_channel, false);
      }

//--- export systematic uncertainties
      for ( std::vector<systematicEntryType*>::iterator systematic = systematics_.begin();
	    systematic != systematics_.end(); ++systematic ) {

	std::string outputFileName_systematic = std::string(outputFilePath_).append("/");
	outputFileName_systematic.append((*systematic)->outputFilePath_).append("/");
	outputFileName_systematic.append((*process)->outputFilePath_).append("/");
	outputFileName_systematic.append((*process)->outputFileName_);
	outputFileName_systematic = 
	  replace_string(outputFileName_systematic, channelOutputFileNameKeyword, (*channel)->shortName_, 0, 1, errorFlag);
	outputFileName_systematic = replace_string(outputFileName_systematic, "//", "/", 0, 1000, errorFlag);
	std::cout << " outputFileName_systematic = " << outputFileName_systematic << std::endl;

	if ( distribution && distribution->systematics_.find((*systematic)->name_) != distribution->systematics_.end() ) {

	  std::string meNameTemplate_systematic = 
	    getMEname_full(distribution->meNameTemplate_, (*systematic)->dqmDirectory_);
	  std::cout << " meNameTemplate_systematic = " << meNameTemplate_systematic << std::endl;

	  std::string meNameNumEventsProcessed_systematic = 
	    getMEname_full(distribution->meNameNumEventsProcessed_, (*systematic)->dqmDirectory_);
	  std::cout << " meNameNumEventsProcessed_systematic = " << meNameNumEventsProcessed_systematic << std::endl;
	  std::string meNameNumEventsPassed_systematic = 
	    getMEname_full(distribution->meNameNumEventsPassed_, (*systematic)->dqmDirectory_);
	  std::cout << " meNameNumEventsPassed_systematic = " << meNameNumEventsPassed_systematic << std::endl;

	  exportAnalysisResults(dqmStore, meNameTemplate_systematic, 
				meNameNumEventsProcessed_systematic, meNameNumEventsPassed_systematic, 
				(*process)->xSection_, (*channel)->dataIntLumi_,
				numChannels_, binOffsets_[(*channel)->index_], numBinsTotal_, outputFileName_systematic, false);
	} else {
	  exportAnalysisResults(dqmStore, 
				"", "", "", 
				(*process)->xSection_, (*channel)->dataIntLumi_,
				numChannels_, binOffsets_[(*channel)->index_], numBinsTotal_, outputFileName_systematic, false);
	}
      }
    }
  }

  std::cout << "done." << std::endl;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMExportAnalysisResults);
