// - Test conversion of flags TXT <-> XML
//
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include <iostream>

#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/getFlagTree.h"

void TXTtoXML() {
  std::cout << "--[ \033[1mTXTtoXML\033[0m ]----------------" << std::endl;

  std::cout << "\033[1mTxt\033[0m Flags Tree: " << std::endl;
  GlobalFlagTxt oFlagTxt( getTXTFlagTree());
  std::cout << "\033[1;32m" << oFlagTxt << "\033[0m" << std::endl;
  std::cout << std::endl;

  std::cout << "\033[1mXML\033[0m Flags Tree: " << std::endl;
  GlobalFlagXML oFlagXML( &oFlagTxt);
  std::cout << "\033[1;32m" << oFlagXML << "\033[0m" << std::endl;

  std::cout << std::endl;
}

void XMLtoTXT() {
  std::cout << "--[ \033[1mXMLtoTXT\033[0m ]----------------" << std::endl;

  std::cout << "\033[1mXML\033[0m Flags Tree: " << std::endl;
  GlobalFlagXML oFlagXML( getXMLFlagTree());
  std::cout << "\033[1;32m" << oFlagXML << "\033[0m" << std::endl;
  std::cout << std::endl;

  std::cout << "\033[1mTxt\033[0m Flags Tree: " << std::endl;
  GlobalFlagTxt oFlagTxt( &oFlagXML);
  std::cout << "\033[1;32m" << oFlagTxt << "\033[0m" << std::endl;

  std::cout << std::endl;
}

int main( int nArgc, char *pcArgv[]) {
  XMLtoTXT();
  TXTtoXML();

  return 0;
}
