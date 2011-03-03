#include "TauAnalysis/Core/interface/sysUncertaintyAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>

#include <iostream>

std::vector<std::string> expandSysName(const std::string& sysName)
{
  //std::cout << "<expandSysName>:" << std::endl;

  static TPRegexp regexpParser_array_entry("[[:alnum:]]+\\([[:digit:]]+\\)");
  static TPRegexp regexpParser_array_elements("([[:alnum:]]+)\\(([[:digit:]]+)\\)");
  
  TString sysName_tstring = sysName.data();
  
  std::vector<std::string> sysNames_expanded;
  
  if ( regexpParser_array_entry.Match(sysName_tstring) == 1 ){
    TObjArray* subStrings = regexpParser_array_elements.MatchS(sysName_tstring);
    
    if ( subStrings->GetEntries() == 3 ) {
      std::string sysName_array = ((TObjString*)subStrings->At(1))->GetString().Data();
      //std::cout << " sysName_array = " << sysName_array << std::endl;

      unsigned numElements_array = (unsigned)atoi(((TObjString*)subStrings->At(2))->GetString().Data());
      //std::cout << " numElements_array = " << numElements_array << std::endl;
      
      for ( unsigned iElement = 0; iElement < numElements_array; ++iElement ) {
	std::ostringstream sysName_i;
	sysName_i << sysName_array;
	sysName_i << "(";
	sysName_i << iElement;
	sysName_i << ")";
	sysNames_expanded.push_back(sysName_i.str());
      }
    } else {
      edm::LogError ("expandSysName") << " Failed to decode sysName = " << sysName << " !!";
    }
  } else {
    sysNames_expanded.push_back(sysName);
  }

  //std::cout << " sysNames_expanded = " << format_vstring(sysNames_expanded) << std::endl;

  return sysNames_expanded;
}
