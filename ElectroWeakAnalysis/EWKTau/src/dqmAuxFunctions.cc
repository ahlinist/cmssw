#include "ElectroWeakAnalysis/EWKTau/interface/dqmAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

std::string replace_string(const std::string& src, const std::string& keyword, const std::string& parameter, int& errorFlag)
{
  std::string modSrc = src;
  unsigned numReplacements = 0;
  while ( modSrc.find(keyword) != std::string::npos ) {
    modSrc.replace(modSrc.find(keyword), keyword.length(), parameter);
    ++numReplacements;
  }
  if ( numReplacements != 1 ) {
    edm::LogError ("replace_string") << " Failed to replace parameter = " << parameter << " in src = " << src << ","
				     << " numReplacements = " << numReplacements << " !!";
    errorFlag = 1;
  }
  return modSrc;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void separateHistogramFromDirectoryName(const std::string& histogramAndDirectoryName, std::string& histogramName, std::string& directoryName)
{
  //std::cout << "<separateHistogramFromDirectoryName>:" << std::endl;

  std::string tempName = histogramAndDirectoryName;

//--- remove DQM prefix from histogram name
  std::string::size_type dqmPrefixPos = tempName.find(dqmPrefix);
  if ( dqmPrefixPos != std::string::npos ) {  
    tempName.replace(dqmPrefixPos, dqmPrefix.size(), "");  
  }  

  //std::cout << " tempName = " << tempName << std::endl;

//--- extract directory from histogram name
  std::string::size_type lastPos;
  std::string::size_type nextPos = tempName.find(dqmSeparator);  
  do {
    lastPos = nextPos;
    nextPos = tempName.find(dqmSeparator, lastPos + 1);
  } while ( nextPos != std::string::npos );

  histogramName = ( lastPos != std::string::npos ) ? std::string(tempName, lastPos + 1, tempName.length()) : tempName;
  directoryName = ( lastPos != std::string::npos ) ? std::string(tempName, 0, lastPos) : "";

  //std::cout << " histogramName = " << histogramName << std::endl;
  //std::cout << " directoryName = " << directoryName << std::endl;
}

