// - Test conversion of flags TXT <-> XML
//
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include <iostream>

#include "CalibTracker/SiStripRunSummary/TIBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/TIBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/TOBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/TOBFlagXML.h"

#include "CalibTracker/SiStripRunSummary/DQMFlagXML.h"
#include "CalibTracker/SiStripRunSummary/DQMFlagTxt.h"

DQMFlagTxt getTXTFlagTree() {
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

DQMFlagXML getXMLFlagTree() {
  DQMFlagXML oDQMFlag;
  oDQMFlag.setComment( "This is DQM Flag XML");

  if( FlagXML *poFlag = oDQMFlag.createChild<TIBFlagXML>()) {
    poFlag->setState( Flag::OK);
    poFlag->setComment( "DQMFlagXML -> TIBFlagXML");
  }

  if( FlagXML *poFlag = oDQMFlag.createChild<TOBFlagXML>()) {
    poFlag->setState( Flag::OK);
    poFlag->setComment( "DQMFlagXML -> TOBFlagXML");
  }

  return oDQMFlag;
}

void TXTtoXML() {
  std::cout << "--[ \033[1mTXTtoXML\033[0m ]----------------" << std::endl;

  std::cout << "\033[1mTxt\033[0m Flags Tree: " << std::endl;
  DQMFlagTxt oFlagTxt( getTXTFlagTree());
  std::cout << "\033[1;32m" << oFlagTxt << "\033[0m" << std::endl;
  std::cout << std::endl;

  std::cout << "\033[1mXML\033[0m Flags Tree: " << std::endl;
  DQMFlagXML oFlagXML( &oFlagTxt);
  std::cout << "\033[1;32m" << oFlagXML << "\033[0m" << std::endl;

  std::cout << std::endl;
}

void XMLtoTXT() {
  std::cout << "--[ \033[1mXMLtoTXT\033[0m ]----------------" << std::endl;

  std::cout << "\033[1mXML\033[0m Flags Tree: " << std::endl;
  DQMFlagXML oFlagXML( getXMLFlagTree());
  std::cout << "\033[1;32m" << oFlagXML << "\033[0m" << std::endl;
  std::cout << std::endl;

  std::cout << "\033[1mTxt\033[0m Flags Tree: " << std::endl;
  DQMFlagTxt oFlagTxt( &oFlagXML);
  std::cout << "\033[1;32m" << oFlagTxt << "\033[0m" << std::endl;

  std::cout << std::endl;
}

int main( int nArgc, char *pcArgv[]) {
  XMLtoTXT();
  TXTtoXML();

  return 0;
}
