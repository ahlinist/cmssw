// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include <iostream>
#include <fstream>
#include <sstream>

#include "CalibTracker/SiStripRunSummary/DQMFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/TIBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/TOBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/SerializeTxt.h"

const char  *pcFILE_BIN = "archive.bin";
std::string  oFlagsStr  = "";

DQMFlagTxt getFlagTree() {
  DQMFlagTxt oDQMFlag;
  oDQMFlag.setComment( "This is DQM Flag TXT");

  if( FlagTxt *poFlag = oDQMFlag.createChild<TIBFlagTxt>()) {
    poFlag->setState( Flag::OK);
    poFlag->setComment( "DQMFlagTxt -> TIBFlagTxt");
  }

  if( FlagTxt *poFlag = oDQMFlag.createChild<TOBFlagTxt>()) {
    poFlag->setState( Flag::OK);
    poFlag->setComment( "DQMFlagTxt -> TOBFlagTxt");
  }

  return oDQMFlag;
}

void BINtoFile() {
  std::cout << "--[ \033[1mBINtoFile\033[0m ]----------------" << std::endl;
  DQMFlagTxt oFlag( getFlagTree());

  std::ofstream oSFileOut( pcFILE_BIN);

  SerializeBin oSerializeBin;
  if( oSerializeBin.write( oSFileOut, oFlag)) {
    std::cout << "Flags Tree wrote to file: \033[1m" << pcFILE_BIN 
              << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not write Flags Tree to file: "
              << "\033[1m" << pcFILE_BIN << "\033[0m" << std::endl;
  }
  std::cout << std::endl;
}

void BINtoStr() {
  std::cout << "--[ \033[1mBINtoStr\033[0m ]----------------" << std::endl;
  DQMFlagTxt oFlag( getFlagTree());

  std::ostringstream oSStrOut;

  SerializeBin oSerializeBin;
  if( oSerializeBin.write( oSStrOut, oFlag)) {
    oFlagsStr = oSStrOut.str();

    std::cout << "BIN String representing Falgs Tree..."
              << std::endl
              << "\033[1;32m"
              << oFlagsStr << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not write Flags to string "
              << std::endl;
  }

  std::cout << std::endl;
}

void FiletoBIN() {
  std::cout << "--[ \033[1mFiletoBIN\033[0m ]----------------" << std::endl;

  std::ifstream oSFileIn( pcFILE_BIN);

  SerializeBin oSerializeBin;
  if( DQMFlagTxt *poFlag = oSerializeBin.read<DQMFlagTxt>( oSFileIn)) {
    std::cout << "Read Flags Tree from file: \033[1m"
              << pcFILE_BIN << "\033[0m" << std::endl;
    std::cout << "\033[1;32m" << *poFlag << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not read Flags Tree from file: "
              << "\033[1m" << pcFILE_BIN << "\033[0m" << std::endl;
  }

  std::cout << std::endl;
}

void StrtoBIN() {
  std::cout << "--[ \033[1mStrtoBIN\033[0m ]----------------" << std::endl;

  std::istringstream oSStrIn( oFlagsStr);

  SerializeBin oSerializeBin;
  if( DQMFlagTxt *poFlag = oSerializeBin.read<DQMFlagTxt>( oSStrIn)) {
    std::cout << "Read Flags Tree from string" << std::endl;
    std::cout << "\033[1;32m" << *poFlag << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not read Flags from string "
              << std::endl;
  }

  std::cout << std::endl;
}

int main( int nArgc, char *pcArgv[]) {
  BINtoFile();
  BINtoStr ();
  FiletoBIN();
  StrtoBIN ();

  return 0;
}
