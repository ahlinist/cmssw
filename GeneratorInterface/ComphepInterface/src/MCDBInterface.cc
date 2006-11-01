//************************************
//* Generators interface with the LCG MCDB
//*
//* Hector Naves Sordo 
//* 
//* First version: 25/10/06 
//* 
//************************************
#include "GeneratorInterface/MadGraphInterface/interface/MCDBInterface.h"


// Makes a local copy of a CASTOR file.
// This code is a modified version of 
// Utilities/StorageFactory/test/any.cpp by Vincenzo Innocente
//
void mcdbGetInputFile(string  &madGraphInputFile, int &mcdbArticleID) {
  
  // Parse the filename from the MCDB CASTOR PATH
  // Needed for the local name.
  //
  int mcdbPathLength = madGraphInputFile.size();
  int mcdbStartFromHere = madGraphInputFile.rfind("/",mcdbPathLength);
  string mcdbLocalFileName = madGraphInputFile.substr(mcdbStartFromHere + 1);
  
  // Parse the MCDB Article Number from the MCDB CASTOR PATH
  //
  int mcdbStartArticleIDFromHere =  
    madGraphInputFile.rfind("/", mcdbStartFromHere - 1);
  string mcdbArticleIDFromPath = 
    madGraphInputFile.substr(mcdbStartArticleIDFromHere + 1, (mcdbStartFromHere - mcdbStartArticleIDFromHere) - 1);
  
  cout << "MCDBInterface: MCDB input file..." << endl;
  
  //***
  // Makes the local copy of the CASTOR file 
  //
  PluginManager::get ()->initialise ();
  
  IOOffset    mcdbFileSize = -1;
  StorageFactory::get ()->enableAccounting(true);
  bool mcdbFileExists = StorageFactory::get ()->check(madGraphInputFile, &mcdbFileSize);
  
  cout << "MCDB Article ID: " << mcdbArticleIDFromPath << endl;
  cout << "Retrieving file: " << madGraphInputFile << endl;
  cout << "           Size: " << mcdbFileSize << endl;
  
  if (!mcdbFileExists) {
    cerr << "Error: Cannot open MCDB input file" << endl;
    throw edm::Exception(edm::errors::Configuration,"OpenMCDBFileError")
      <<" Cannot open MCDB input file, check file name and path.";
  }
  
  Storage  *mcdbFile =  StorageFactory::get ()->open(madGraphInputFile);
  char mcdbBuf [1024];
  IOSize mcdbNSize;
  
  ofstream  mcdbLocalFileCopy;
  mcdbLocalFileCopy.open(mcdbLocalFileName.c_str());
  while ((mcdbNSize = mcdbFile->read (mcdbBuf, sizeof (mcdbBuf))))
    mcdbLocalFileCopy.write(mcdbBuf, mcdbNSize);
  
  mcdbLocalFileCopy.close();
  cout << "MCDBInterface: MCDB stats:\n"
      << StorageAccount::summaryText () <<endl;
  cout << endl;
  //
  //***

  // Once the local copy of the file is done, we only need the filename
  // and the Article ID...
  //
  madGraphInputFile = mcdbLocalFileName;
  mcdbArticleID = atoi(mcdbArticleIDFromPath.c_str());

}
