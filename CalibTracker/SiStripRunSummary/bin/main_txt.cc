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

const char  *pcFILE_TXT = "archive.txt";
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

void TXTtoFile() {
  std::cout << "--[ \033[1mTXTtoFile\033[0m ]----------------" << std::endl;
  DQMFlagTxt oFlag( getFlagTree());

  std::ofstream oSFileOut( pcFILE_TXT);

  SerializeTxt oSerializeTxt;
  if( oSerializeTxt.write( oSFileOut, oFlag)) {
    std::cout << "Flags Tree wrote to file: \033[1m" << pcFILE_TXT 
              << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not write Flags Tree to file: "
              << "\033[1m" << pcFILE_TXT << "\033[0m" << std::endl;
  }
  std::cout << std::endl;
}

void TXTtoStr() {
  std::cout << "--[ \033[1mTXTtoStr\033[0m ]----------------" << std::endl;
  DQMFlagTxt oFlag( getFlagTree());

  std::ostringstream oSStrOut;

  SerializeTxt oSerializeTxt;
  if( oSerializeTxt.write( oSStrOut, oFlag)) {
    oFlagsStr = oSStrOut.str();

    std::cout << "TXT String representing Falgs Tree..."
              << std::endl
              << "\033[1;32m"
              << oFlagsStr << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not write Flags to string "
              << std::endl;
  }

  std::cout << std::endl;
}

void FiletoTXT() {
  std::cout << "--[ \033[1mFiletoTXT\033[0m ]----------------" << std::endl;

  std::ifstream oSFileIn( pcFILE_TXT);

  SerializeTxt oSerializeTxt;
  if( DQMFlagTxt *poFlag = oSerializeTxt.read<DQMFlagTxt>( oSFileIn)) {
    std::cout << "Read Flags Tree from file: \033[1m"
              << pcFILE_TXT << "\033[0m" << std::endl;
    std::cout << "\033[1;32m" << *poFlag << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not read Flags Tree from file: "
              << "\033[1m" << pcFILE_TXT << "\033[0m" << std::endl;
  }

  std::cout << std::endl;
}

void StrtoTXT() {
  std::cout << "--[ \033[1mStrtoTXT\033[0m ]----------------" << std::endl;

  std::istringstream oSStrIn( oFlagsStr);

  SerializeTxt oSerializeTxt;
  if( DQMFlagTxt *poFlag = oSerializeTxt.read<DQMFlagTxt>( oSStrIn)) {
    std::cout << "Read Flags Tree from string" << std::endl;
    std::cout << "\033[1;32m" << *poFlag << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not read Flags from string "
              << std::endl;
  }

  std::cout << std::endl;
}

int main( int nArgc, char *pcArgv[]) {
  TXTtoFile();
  TXTtoStr ();
  FiletoTXT();
  StrtoTXT ();

  return 0;
}
