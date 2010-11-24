#include "TauAnalysis/DQMTools/plugins/DQMFileLoader.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/Core/interface/FilterStatisticsService.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <TFile.h>
#include <TList.h>
#include <TKey.h>
#include <TH1.h>

#include <iostream>

const std::string dqmRootDirectory_inTFile = "DQMData";

const double defaultScaleFactor = 1.;

const int verbosity = 0;

void mapSubDirectoryStructure(TDirectory* directory, std::string directoryName, std::set<std::string>& subDirectories)
{
  TList* subDirectoryNames = directory->GetListOfKeys();
  if ( !subDirectoryNames ) return;

  TIter next(subDirectoryNames);
  while ( TKey* key = dynamic_cast<TKey*>(next()) ) {
    TObject* obj = directory->Get(key->GetName());

    if ( TDirectory* subDirectory = dynamic_cast<TDirectory*>(obj) ) {
      std::string subDirectoryName = dqmDirectoryName(directoryName).append(key->GetName());
    
      subDirectories.insert(subDirectoryName);
    
      mapSubDirectoryStructure(subDirectory, subDirectoryName, subDirectories); 
    }
  }
} 

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMFileLoader::cfgEntryFileSet::cfgEntryFileSet(const std::string& name, const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMFileLoader::cfgEntryFileSet>" << std::endl;

  cfgError_ = 0;

  name_ = name;

  vstring inputFileList = cfg.getParameter<vstring>("inputFileNames");
  for ( vstring::const_iterator inputFile = inputFileList.begin();
	inputFile != inputFileList.end(); ++inputFile ) {
    if ( inputFile->find(rangeKeyword) != std::string::npos ) {
      size_t posRangeStart = inputFile->find(rangeKeyword) + rangeKeyword.length();
      size_t posRangeEnd = inputFile->find('#', posRangeStart); 
      
      size_t posRangeSeparator = inputFile->find('-', posRangeStart);

      if ( (posRangeEnd == std::string::npos) || 
	   (posRangeSeparator >= posRangeEnd) ) { 
	edm::LogError ("DQMFileLoader::cfgEntryFileSet") << " Invalid range specification in inputFile = " << (*inputFile) << " !!";
	continue;
      }

      std::string firstFile = std::string(*inputFile, posRangeStart, posRangeSeparator - posRangeStart);
      std::string lastFile = std::string(*inputFile, posRangeSeparator + 1, posRangeEnd - (posRangeSeparator + 1));

      if ( firstFile.length() != lastFile.length() ) {
	edm::LogError ("DQMFileLoader::cfgEntryFileSet") << " Invalid range specification in inputFile = " << (*inputFile) << " !!";
	continue;
      }

      int numFirstFile = atoi(firstFile.data());
      int numLastFile = atoi(lastFile.data());
      for ( int iFile = numFirstFile; iFile <= numLastFile; ++iFile ) {
	std::ostringstream fileName;
	fileName << std::string(*inputFile, 0, inputFile->find(rangeKeyword));
	fileName << std::setfill('0') << std::setw(firstFile.length()) << iFile;
	fileName << std::string(*inputFile, posRangeEnd + 1);
	inputFileNames_.push_back(fileName.str());
      }
    } else {
      inputFileNames_.push_back(*inputFile);
    }
  }

  
  // Determine if we want to automagically calculate the scaling factor
  autoscale_ = ( cfg.exists("autoscale") ) ? cfg.getParameter<bool>("autoscale") : false;

  if ( autoscale_ ) {
    totalExpectedEventsBeforeSkim_ = cfg.getParameter<unsigned>("totalExpectedEventsBeforeSkim");
    skimEfficiency_ = cfg.getParameter<double>("skimEfficiency");
    xSection_ = cfg.getParameter<double>("xSection");
    targetIntLumi_ = cfg.getParameter<double>("targetIntLumi");
    filterStatisticsLocation_ = dqmDirectoryName(cfg.getParameter<std::string>("filterStatisticsLocation"));
    filterToUse_ = cfg.getParameter<std::string>("filterToUse");
    scaleFactor_ = -1;
    // Warn if old scale factor is hanging around
    if( cfg.exists("scaleFactor") ) {
      edm::LogWarning("DQMFileLoader::cfgEntryFileSet") 
	<< "Both the <autoscale> and <scaleFactor> options are set. The <scaleFactor> specified "
	<< " in the parameter set will be ignored and the autoscale value will be used!"; 
    }
  } else {
    scaleFactor_ = ( cfg.exists("scaleFactor") ) ? cfg.getParameter<double>("scaleFactor") : defaultScaleFactor;
  }

  //std::cout << " scaleFactor = " << scaleFactor_ << std::endl;

  dqmDirectory_store_ = ( cfg.exists("dqmDirectory_store") ) ? cfg.getParameter<std::string>("dqmDirectory_store") : "";
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;

  if ( verbosity ) print();
}

void DQMFileLoader::cfgEntryFileSet::print() const
{
  std::cout << "<cfgEntryFileSet::print>:" << std::endl;
  std::cout << " name = " << name_ << std::endl;
  std::cout << " inputFileNames = " << format_vstring(inputFileNames_) << std::endl;
  std::cout << " scaleFactor = " << scaleFactor_ << std::endl;
  std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMFileLoader::DQMFileLoader(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMFileLoader::DQMFileLoader>:" << std::endl;

  cfgError_ = 0;

//--- configure fileSets  
  //std::cout << "--> configuring fileSets..." << std::endl;
  inputFilePath_ = ( cfg.exists("inputFilePath") ) ? cfg.getParameter<std::string>("inputFilePath") : "";
  if ( inputFilePath_ != "" && inputFilePath_[inputFilePath_.length() - 1] != '/' ) inputFilePath_ += '/';

  readCfgParameter<cfgEntryFileSet>(cfg, fileSets_);

//--- check for errors in reading configuration parameters of fileSets
  for ( std::map<std::string, cfgEntryFileSet>::const_iterator fileSet = fileSets_.begin();
	fileSet != fileSets_.end(); ++fileSet ) {
    if ( fileSet->second.cfgError_ ) {
      edm::LogError ("DQMFileLoader") << " Error in Configuration Parameters of fileSet = " << fileSet->second.name_ << " !!";
      cfgError_ = 1;
    }
  }

//--- check that dqmDirectory_store configuration parameters are specified and non-empty for all fileSets,
//    unless there is only one file to be loaded in the fileSet for which dqmDirectory_store has not been are specified 
//    (otherwise histograms contained in previous file get overwritten once histograms are loaded from the next files)
  for ( std::map<std::string, cfgEntryFileSet>::const_iterator fileSet = fileSets_.begin();
	fileSet != fileSets_.end(); ++fileSet ) {
    if ( fileSet->second.dqmDirectory_store_ == "" && fileSet->second.inputFileNames_.size() > 1 ) {
      edm::LogError ("DQMFileLoader") << " dqmDirectory_store undefined for fileSet = " << fileSet->second.name_ << " !!";
      cfgError_ = 1;
    }
  }

  //std::cout << "done." << std::endl;
}

DQMFileLoader::~DQMFileLoader() 
{
// nothing to be done yet...
}

void DQMFileLoader::analyze(const edm::Event&, const edm::EventSetup&)
{
// nothing to be done yet...
}

void DQMFileLoader::endJob()
{
  std::cout << "<DQMFileLoader::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endJob") 
      << " Error in Configuration ParameterSet --> histograms will NOT be loaded !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") 
      << " Failed to access dqmStore --> histograms will NOT be loaded !!";
    return;
  }

//--- check that inputFiles exist;
//    store list of directories existing in inputFile,
//    in order to separate histogram directories existing in the inputFile from directories existing in DQMStore
//    when calling recursive function dqmCopyRecursively
  for ( std::map<std::string, cfgEntryFileSet>::const_iterator fileSet = fileSets_.begin();
	fileSet != fileSets_.end(); ++fileSet ) {
    for ( vstring::const_iterator inputFileName = fileSet->second.inputFileNames_.begin();
	  inputFileName != fileSet->second.inputFileNames_.end(); ++inputFileName ) {
      std::string inputFileName_full;
      if ( inputFilePath_ != "" ) inputFileName_full += inputFilePath_;
      inputFileName_full += (*inputFileName);

      TFile* inputFile = TFile::Open(inputFileName_full.data());
      if ( inputFile->IsZombie() ) {
	edm::LogError ("endJob") 
	  << " Failed to open inputFile = " << inputFileName_full 
	  << " --> histograms will NOT be loaded !!";
	return;
      }
 
      TObject* obj = inputFile->Get(dqmRootDirectory_inTFile.data());
      if ( TDirectory* directory = dynamic_cast<TDirectory*>(obj) ) {
	mapSubDirectoryStructure(directory, "", subDirectoryMap_[inputFileName_full]);
      } else {
	edm::LogError ("endJob") 
	  << " Failed to access " << dqmRootDirectory_inTFile << " in inputFile = " << inputFileName_full
	  << " --> histograms will NOT be loaded !!";
	return;
      }

      inputFile->Close();
      delete inputFile;
    }
  }

//--- load histograms from file
  //std::cout << "--> loading histograms from file..." << std::endl;
  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  for ( std::map<std::string, cfgEntryFileSet>::iterator fileSet = fileSets_.begin();
	fileSet != fileSets_.end(); ++fileSet ) {
    cfgEntryFileSet& cfgFileSet = fileSet->second;
    if ( cfgFileSet.autoscale_ ) {
      std::cout << "************************** Autoscaler *****************************************" << std::endl;
      std::cout << " Looping over input files to find scale factor for sample " << fileSet->first << std::endl;
      // If using the autoscale option, first loop over the files to compute the total number
      // of events process
      unsigned int eventsProcessed = 0;
      for ( vstring::const_iterator inputFileName = cfgFileSet.inputFileNames_.begin();
	    inputFileName != cfgFileSet.inputFileNames_.end(); ++inputFileName ) {
	std::string inputFileName_full;
	if ( inputFilePath_ != "" ) inputFileName_full += inputFilePath_;
	inputFileName_full += (*inputFileName);
	//if ( verbosity ) std::cout << " opening inputFile = " << inputFileName_full << std::endl;
	std::cout << " opening inputFile = " << inputFileName_full << std::endl;  
	dqmStore.open(inputFileName_full, true);
	// Load filter statistics
	FilterStatisticsService statsService;
	FilterStatisticsTable* filterTable = statsService.loadFilterStatisticsTable(cfgFileSet.filterStatisticsLocation_);
	//filterTable->print(std::cout);
	int eventsInThisFile = 
	  filterTable->extractNumber(cfgFileSet.filterStatisticsLocation_.append(cfgFileSet.filterToUse_), "Processed", false);
	std::cout << " events in file: " << eventsInThisFile << std::endl;
	eventsProcessed += eventsInThisFile;
      }
      // Compute luminosity scale factor
      double luminosityNoJobFailures = 
	cfgFileSet.totalExpectedEventsBeforeSkim_ /cfgFileSet.xSection_;
      double expectedEvents = cfgFileSet.totalExpectedEventsBeforeSkim_*cfgFileSet.skimEfficiency_;
      double jobEfficiency = eventsProcessed/expectedEvents;
      double effectiveLumi = luminosityNoJobFailures*jobEfficiency;
      // Set the scale factor
      cfgFileSet.scaleFactor_ = cfgFileSet.targetIntLumi_/effectiveLumi;
      
      std::cout << " xSection: " << cfgFileSet.xSection_ << "pb" << std::endl;
      std::cout << " total event production: " << cfgFileSet.totalExpectedEventsBeforeSkim_ << std::endl;
      std::cout << " skim efficiency: " << cfgFileSet.skimEfficiency_ << std::endl;
      std::cout << " Int. lumi assuming no job failures: " << luminosityNoJobFailures << "pb-1" << std::endl;
      std::cout << " Actual/expected events processed: " << eventsProcessed << "/" << expectedEvents 
		<< " (" << jobEfficiency*100 << "%)" << std::endl;
      std::cout << " Effective int. lumi: " << luminosityNoJobFailures*jobEfficiency << "pb-1" << std::endl;
      std::cout << " Scale factor for target lumi " << cfgFileSet.targetIntLumi_  
		<< "pb-1: " << cfgFileSet.scaleFactor_ << std::endl;
      std::cout << " sanity check: " << fileSet->second.scaleFactor_ << std::endl;
    }
    
    for ( vstring::const_iterator inputFileName = fileSet->second.inputFileNames_.begin();
	  inputFileName != fileSet->second.inputFileNames_.end(); ++inputFileName ) {     
      std::string inputFileName_full;
      if ( inputFilePath_ != "" ) inputFileName_full += inputFilePath_;
      inputFileName_full += (*inputFileName);

//--- open input file only once
//   (input file has already been opened in case auto-scale option is used)
      if ( !cfgFileSet.autoscale_ ) {
	//if ( verbosity ) std::cout << " opening inputFile = " << inputFileName_full << std::endl;
	std::cout << " opening inputFile = " << inputFileName_full << std::endl; 
	dqmStore.open(inputFileName_full, true);
      }
      
      if ( verbosity ) dqmStore.showDirStructure();
      
//--- if dqmDirectory_store specified in configuration parameters,
//    move histograms from dqmRootDirectory to dqmDirectory_store
//    (if the histograms are not moved, the histograms get overwritten,
//     the next time DQMStore::open is called)
      if ( fileSet->second.dqmDirectory_store_ != "" ) {
	std::string inputDirectory = "";
	//std::cout << "inputDirectory = " << inputDirectory << std::endl;
	std::string outputDirectory = dqmDirectoryName(std::string(inputDirectory)).append(fileSet->second.dqmDirectory_store_);
	//std::cout << "outputDirectory = " << outputDirectory << std::endl;

	dqmStore.setCurrentFolder(inputDirectory);
	std::vector<std::string> dirNames = dqmStore.getSubdirs();
	for ( std::vector<std::string>::const_iterator dirName = dirNames.begin();
	      dirName != dirNames.end(); ++dirName ) {
	  std::string subDirName = dqmSubDirectoryName(inputDirectory, *dirName);
	  //std::cout << " subDirName = " << subDirName << std::endl;
	  
	  const sstring& subDirectories = subDirectoryMap_[inputFileName_full];
	  if ( subDirectories.find(subDirName) != subDirectories.end() ) {
	    std::string inputDirName_full = dqmDirectoryName(inputDirectory).append(subDirName);	    
	    //std::cout << " inputDirName_full = " << inputDirName_full << std::endl;
	    std::string outputDirName_full = dqmDirectoryName(outputDirectory).append(subDirName);
	    //std::cout << " outputDirName_full = " << outputDirName_full << std::endl;

//--- load histograms contained in inputFile into inputDirectory;
//    when processing first inputFile, check that histograms in outputDirectory do not yet exist;
//    add histograms in inputFile to those in outputDirectory afterwards;
//    clear inputDirectory once finished processing all inputFiles.
	    int mode = ( inputFileName == fileSet->second.inputFileNames_.begin() ) ? 1 : 2;
	    dqmCopyRecursively(dqmStore, inputDirName_full, outputDirName_full, fileSet->second.scaleFactor_, 0., mode, true);
	  }
	}
      }
    }
  }

  std::cout << "done." << std::endl; 
  if ( verbosity ) dqmStore.showDirStructure();
  //dqmStore.showDirStructure();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMFileLoader);
