//************************************
//* Generators interface with the LCG MCDB
//*
//* Hector Naves Sordo 
//* 
//* First version: 25/10/06 
//* 
//************************************
#include "GeneratorInterface/MadGraphInterface/interface/MCDBInterface.h"
#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/standard.h"

// Makes a local copy of a CASTOR file.
// This code is a modified version of 
// Utilities/StorageFactory/test/any.cpp by Vincenzo Innocente
void mcdbGetInputFile(std::string  &madGraphInputFile, int &mcdbArticleID) {
  
  // Parse the filename from the MCDB CASTOR PATH
  // Needed for the local name.
  int mcdbPathLength = madGraphInputFile.size();
  int mcdbStartFromHere = madGraphInputFile.rfind("/",mcdbPathLength);
  std::string mcdbLocalFileName = madGraphInputFile.substr(mcdbStartFromHere + 1);
  
  // Parse the MCDB Article Number from the MCDB CASTOR PATH
  int mcdbStartArticleIDFromHere =  madGraphInputFile.rfind("/", mcdbStartFromHere - 1);
  std::string mcdbArticleIDFromPath = madGraphInputFile.substr(mcdbStartArticleIDFromHere + 1, (mcdbStartFromHere - mcdbStartArticleIDFromHere) - 1);
  
  std::cout << "MCDBInterface: MCDB input file..." << std::endl;
  
  // Makes the local copy of the CASTOR file 
  edmplugin::PluginManager::configure(edmplugin::standard::config());
  
  seal::IOOffset    mcdbFileSize = -1;
  StorageFactory::get()->enableAccounting(true);
  bool mcdbFileExists = StorageFactory::get()->check(madGraphInputFile, &mcdbFileSize);
  
  std::cout << "MCDB Article ID: " << mcdbArticleIDFromPath << std::endl;
  std::cout << "Retrieving file: " << madGraphInputFile << std::endl;
  std::cout << "           Size: " << mcdbFileSize << std::endl;
  
  if (!mcdbFileExists) {
    std::cerr << "Error: Cannot open MCDB input file" << std::endl;
    throw edm::Exception(edm::errors::Configuration,"OpenMCDBFileError")
      <<" Cannot open MCDB input file, check file name and path.";
  }
  
  seal::Storage  *mcdbFile =  StorageFactory::get()->open(madGraphInputFile);
  char mcdbBuf [1024];
  seal::IOSize mcdbNSize;
  
  std::ofstream  mcdbLocalFileCopy;
  mcdbLocalFileCopy.open(mcdbLocalFileName.c_str());
  while ((mcdbNSize = mcdbFile->read (mcdbBuf, sizeof (mcdbBuf))))
    mcdbLocalFileCopy.write(mcdbBuf, mcdbNSize);
  
  mcdbLocalFileCopy.close();
  std::cout << "MCDBInterface: MCDB stats:\n" << StorageAccount::summaryText () <<std::endl <<std::endl;

  // Once the local copy of the file is done, we only need the filename and the Article ID...
  madGraphInputFile = mcdbLocalFileName;
  mcdbArticleID = atoi(mcdbArticleIDFromPath.c_str());
}
