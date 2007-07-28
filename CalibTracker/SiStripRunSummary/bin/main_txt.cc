// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include <iostream>
#include <fstream>
#include <sstream>

#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/SerializeTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/getFlagTree.h"

const char  *pcFILE_TXT = "archive.txt";
std::string  oFlagsStr  = "";

void TXTtoFile() {
  std::cout << "--[ \033[1mTXTtoFile\033[0m ]----------------" << std::endl;
  GlobalFlagTxt oFlag( getTXTFlagTree());

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
  GlobalFlagTxt oFlag( getTXTFlagTree());

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
  if( GlobalFlagTxt *poFlag = oSerializeTxt.read<GlobalFlagTxt>( oSFileIn)) {
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
  if( GlobalFlagTxt *poFlag = oSerializeTxt.read<GlobalFlagTxt>( oSStrIn)) {
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
