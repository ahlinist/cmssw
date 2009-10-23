#include "TauAnalysis/DQMTools/plugins/DQMFileLoader.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

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

  scaleFactor_ = ( cfg.exists("scaleFactor") ) ? cfg.getParameter<double>("scaleFactor") : defaultScaleFactor;
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
    edm::LogError ("endJob") << " Error in Configuration ParameterSet" 
			     << " --> histograms will NOT be loaded !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore" 
			     << " --> histograms will NOT be loaded !!";
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
	edm::LogError ("endJob") << " Failed to open inputFile = " << inputFileName_full
				 << "--> histograms will NOT be loaded !!";
	return;
      }
 
      TObject* obj = inputFile->Get(dqmRootDirectory_inTFile.data());
      if ( TDirectory* directory = dynamic_cast<TDirectory*>(obj) ) {
	mapSubDirectoryStructure(directory, dqmRootDirectory, subDirectoryMap_[inputFileName_full]);
      } else {
	edm::LogError ("endJob") << " Failed to access " << dqmRootDirectory_inTFile << " in inputFile = " << inputFileName_full
				 << "--> histograms will NOT be loaded !!";
	return;
      }

      inputFile->Close();
      delete inputFile;
    }
  }

//--- load histograms from file
  //std::cout << "--> loading histograms from file..." << std::endl;
  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  for ( std::map<std::string, cfgEntryFileSet>::const_iterator fileSet = fileSets_.begin();
	fileSet != fileSets_.end(); ++fileSet ) {
    for ( vstring::const_iterator inputFileName = fileSet->second.inputFileNames_.begin();
	  inputFileName != fileSet->second.inputFileNames_.end(); ++inputFileName ) {
      std::string inputFileName_full;
      if ( inputFilePath_ != "" ) inputFileName_full += inputFilePath_;
      inputFileName_full += (*inputFileName);

      //if ( verbosity ) std::cout << " opening inputFile = " << inputFileName_full << std::endl;
      std::cout << " opening inputFile = " << inputFileName_full << std::endl; 
      dqmStore.open(inputFileName_full, true);

      if ( verbosity ) dqmStore.showDirStructure();
      
//--- if dqmDirectory_store specified in configuration parameters,
//    move histograms from dqmRootDirectory to dqmDirectory_store
//    (if the histograms are not moved, the histograms get overwritten,
//     the next time DQMStore::open is called)
      if ( fileSet->second.dqmDirectory_store_ != "" ) {
	std::string inputDirectory = dqmRootDirectory;
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
	    std::string outputDirName_full = dqmDirectoryName(outputDirectory).append(subDirName);

//--- load histograms contained in inputFile into inputDirectory;
//    when processing first inputFile, check that histograms in outputDirectory do not yet exist;
//    add histograms in inputFile to those in outputDirectory afterwards;
//    clear inputDirectory once finished processing all inputFiles.
	    int mode = ( inputFileName == fileSet->second.inputFileNames_.begin() ) ? 1 : 2;
	    dqmCopyRecursively(dqmStore, inputDirName_full, outputDirName_full, fileSet->second.scaleFactor_, mode, true);
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
