//************************************
//* Generators interface with the LCG MCDB
//* Hector Naves Sordo 
//* First version: 25/10/06
//
// Sandro Fonseca de Souza:20/11/08
// Fixed a MCDB
//  
//************************************
#include "GeneratorInterface/ComphepInterface/interface/MCDBInterface.h"
#include "FWCore/PluginManager/interface/standard.h"
#include "Utilities/StorageFactory/interface/IOTypes.h"
#include "Utilities/StorageFactory/interface/Storage.h"

// Makes a local copy of a CASTOR file.
// This code is a modified version of 
// Utilities/StorageFactory/test/any.cpp by Vincenzo Innocente
void mcdbGetInputFile(std::string  &compHEPInputFile, int &mcdbArticleID) {
  
  // Parse the filename from the MCDB CASTOR PATH
  // Needed for the local name.
  int mcdbPathLength = compHEPInputFile.size();
  int mcdbStartFromHere = compHEPInputFile.rfind("/",mcdbPathLength);
  std::string mcdbLocalFileName = compHEPInputFile.substr(mcdbStartFromHere + 1);
  
  // Parse the MCDB Article Number from the MCDB CASTOR PATH
  int mcdbStartArticleIDFromHere =  compHEPInputFile.rfind("/", mcdbStartFromHere - 1);
  std::string mcdbArticleIDFromPath = compHEPInputFile.substr(mcdbStartArticleIDFromHere + 1, (mcdbStartFromHere - mcdbStartArticleIDFromHere) - 1);
  
  std::cout << "MCDBInterface: MCDB input file... "<< compHEPInputFile<< std::endl;
  
  // Makes the local copy of the CASTOR file
  // bug https://twiki.cern.ch/twiki/bin/view/LCG/CMSSWInterface 
  //  edmplugin::PluginManager::configure(edmplugin::standard::config());
  
  IOOffset    mcdbFileSize = -1;
  StorageFactory::get()->enableAccounting(true);
  bool mcdbFileExists = StorageFactory::get()->check(compHEPInputFile, &mcdbFileSize);
  
  std::cout << "MCDB Article ID: " << mcdbArticleIDFromPath << std::endl;
  std::cout << "Retrieving file: " << compHEPInputFile << std::endl;
  std::cout << "           Size: " << mcdbFileSize << std::endl;
  
  if (!mcdbFileExists) {
    std::cerr << "Error: Cannot open MCDB input file" << std::endl;
    throw edm::Exception(edm::errors::Configuration,"OpenMCDBFileError")
      <<" Cannot open MCDB input file, check file name and path.";
  }
  
  Storage  *mcdbFile =  StorageFactory::get()->open(compHEPInputFile);
  char mcdbBuf [1024];
  IOSize mcdbNSize;
  
  std::ofstream  mcdbLocalFileCopy;
  mcdbLocalFileCopy.open(mcdbLocalFileName.c_str());
  while ((mcdbNSize = mcdbFile->read (mcdbBuf, sizeof (mcdbBuf))))
    mcdbLocalFileCopy.write(mcdbBuf, mcdbNSize);
  
  mcdbLocalFileCopy.close();
  std::cout << "MCDBInterface: MCDB stats:\n" << StorageAccount::summaryText () <<std::endl <<std::endl;

  // Once the local copy of the file is done, we only need the filename and the Article ID...
  compHEPInputFile = mcdbLocalFileName;
  mcdbArticleID = atoi(mcdbArticleIDFromPath.c_str());
}


