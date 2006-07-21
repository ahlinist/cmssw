#ifndef RPCFileProcessor_h
#define RPCFileProcessor_h

/** \class RPCFileProcessor
 *   Class for the processing of the Link Board data file
 *
 *  \author Ilaria Segoni
 */
#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <DataFormats/MuonDetId/interface/RPCDetId.h>
#include <IORawData/RPCLocalDAQ/interface/Mapping.h>
#include <DataFormats/RPCDigi/interface/RPCDigiCollection.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <bitset>


class RPCFileProcessor {
 public:
  RPCFileProcessor(std::vector<std::string> inputFiles, std::string lastWord, std::string firstWord);
  ~RPCFileProcessor(){}
  bool nextEvent(std::auto_ptr<RPCDigiCollection> & pDigis);
 
  enum processStatus {
        Initialization = 1, 
    	Event      = 2,
    	EndOfFile  = 3
  };

 private:
  bool initializeFile();
  RPCDetId makeDetId(std::string,std::string,std::string,std::string,std::string);
  bool readEvent(std::auto_ptr<RPCDigiCollection> & pDigis);
  
  
 private:
  enum processStatus currentStatus;
  std::vector<std::string> dataFiles;
  int maxLBs;
  unsigned int currentFileNumber;
  std::string wordBeforeEvent;
  std::string wordAfterEvent;
 
  std::ifstream *  currentFile;
  std::map<int, RPCDetId>    theDetIds;
  std::map<int, std::string> theLBIds;
  Mapping * mapWheel1;
  Mapping * mapWheel2;
};
#endif
