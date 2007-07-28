// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include <iostream>
#include <fstream>
#include <sstream>

#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/SerializeXML.h"
#include "CalibTracker/SiStripRunSummary/interface/getFlagTree.h"

const char  *pcFILE_XML = "archive.xml";
std::string  oFlagsStr  = "";

void XMLtoFile() {
  std::cout << "--[ \033[1mXMLtoFile\033[0m ]----------------" << std::endl;
  GlobalFlagXML oFlag( getXMLFlagTree());

  std::ofstream oSFileOut( pcFILE_XML);

  SerializeXML oSerializeXML;
  if( oSerializeXML.write( oSFileOut, oFlag)) {
    std::cout << "Flags Tree wrote to file: \033[1m" << pcFILE_XML 
              << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not write Flags Tree to file: "
              << "\033[1m" << pcFILE_XML << "\033[0m" << std::endl;
  }
  std::cout << std::endl;
}

void XMLtoStr() {
  std::cout << "--[ \033[1mXMLtoStr\033[0m ]----------------" << std::endl;
  GlobalFlagXML oFlag( getXMLFlagTree());

  std::ostringstream oSStrOut;

  SerializeXML oSerializeXML;
  if( oSerializeXML.write( oSStrOut, oFlag)) {
    oFlagsStr = oSStrOut.str();

    std::cout << "XML String representing Falgs Tree..."
              << std::endl
              << "\033[1;32m"
              << oFlagsStr << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not write Flags to string "
              << std::endl;
  }

  std::cout << std::endl;
}

void FiletoXML() {
  std::cout << "--[ \033[1mFiletoXML\033[0m ]----------------" << std::endl;

  std::ifstream oSFileIn( pcFILE_XML);

  SerializeXML oSerializeXML;
  if( GlobalFlagXML *poFlag = oSerializeXML.read<GlobalFlagXML>( oSFileIn)) {
    std::cout << "Read Flags Tree from file: \033[1m"
              << pcFILE_XML << "\033[0m" << std::endl;
    std::cout << "\033[1;32m" << *poFlag << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not read Flags Tree from file: "
              << "\033[1m" << pcFILE_XML << "\033[0m" << std::endl;
  }

  std::cout << std::endl;
}

void StrtoXML() {
  std::cout << "--[ \033[1mStrtoXML\033[0m ]----------------" << std::endl;

  std::istringstream oSStrIn( oFlagsStr);

  SerializeXML oSerializeXML;
  if( GlobalFlagXML *poFlag = oSerializeXML.read<GlobalFlagXML>( oSStrIn)) {
    std::cout << "Read Flags Tree from string" << std::endl;
    std::cout << "\033[1;32m" << *poFlag << "\033[0m" << std::endl;
  } else {
    std::cout << "[\033[31merror\033[0m] Could not read Flags from string "
              << std::endl;
  }

  std::cout << std::endl;
}


int main( int nArgc, char *pcArgv[]) {
  XMLtoFile();
  XMLtoStr ();
  FiletoXML();
  StrtoXML ();

  return 0;
}
