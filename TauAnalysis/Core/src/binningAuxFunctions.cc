#include "TauAnalysis/Core/interface/binningAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>

#include <iostream>
#include <iomanip>

std::string encodeBinningStringRep(const std::string& meName, const std::string& meType, const std::string& meValue)
{
  std::ostringstream entry;
  entry << "meName = " << meName << ";"
	<< " meType = " << meType << ";" 
	<< " meValue = " << meValue;

  return entry.str();
}

void decodeBinningStringRep(const std::string& entry, std::string& meName, std::string& meType, std::string& meValue, int& error)
{
  //std::cout << "<decodeBinningStringRep>:" << std::endl;
  //std::cout << " entry = " << entry << std::endl;
  
  TString pattern_entry = "meName[[:space:]]*=[[:space:]]*[[:alnum:]_\\(\\)";
  pattern_entry.Append(meOptionsSeparator).Append("]+");
  pattern_entry.Append("; meType[[:space:]]*=[[:space:]]*[[:alnum:]]+");
  pattern_entry.Append("; meValue[[:space:]]*=[[:space:]]*[\\+\\-[:alnum:].]+[,[:space:]\\+\\-[:alnum:].]*"); 
  TPRegexp regexpParser_entry(pattern_entry);
  TString pattern_items = "meName[[:space:]]*=[[:space:]]*([[:alnum:]_\\(\\)";
  pattern_items.Append(meOptionsSeparator).Append("]+)");
  pattern_items.Append("; meType[[:space:]]*=[[:space:]]*([[:alnum:]]+)");
  pattern_items.Append("; meValue[[:space:]]*=[[:space:]]*([\\+\\-[:alnum:].]+[,[:space:]\\+\\-[:alnum:].]*)");
  TPRegexp regexpParser_items(pattern_items);

  meName = "";
  meType = "";
  meValue = "";

  bool parseError = false;

  TString entry_tstring = entry.data();

//--- check if line matches 
//      meName = ..; meType = ..; meValue = ..
//    format; in which case require four matches for the items (first match refers to entire binning_string)
//    and match individually meName, meType and meValue
  if ( regexpParser_entry.Match(entry_tstring) == 1 ) {
    TObjArray* subStrings = regexpParser_items.MatchS(entry_tstring);
    //std::cout << "subStrings->GetEntries() = " << subStrings->GetEntries() << std::endl;

    if ( subStrings->GetEntries() == 4 ) {
      meName = ((TObjString*)subStrings->At(1))->GetString().Data();
      meType = ((TObjString*)subStrings->At(2))->GetString().Data();
      meValue = ((TObjString*)subStrings->At(3))->GetString().Data();
      
      if ( !(meType == "string" || meType == "float" || meType == "int") ) {
	edm::LogError ("decodeBinningStringRep") << " Undefined meType = " << meType << " !!";
	error = 1;
      }
    } else {
      parseError = true;
    }
  } else {
    parseError = true;
  }
    
  if ( parseError ) {
    edm::LogError ("decodeBinningStringRep") << " Error in parsing string = " << entry << " !!";
    error = 1;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

std::string encodeVDoubleStringRep(const std::vector<double>& elements)
{
  std::ostringstream entry;

  unsigned numElements = elements.size();
  for ( unsigned iElement = 0; iElement < numElements; ++iElement ) {
    entry << std::setprecision(3) << std::fixed << elements[iElement];
    if ( iElement < (numElements - 1) ) entry << ", ";
  }

  return entry.str();
}

std::vector<double> decodeVDoubleStringRep(const std::string& entry, int& error)
{
  const std::string pattern_double = "[+|-]*[[:digit:].]+[eE\\+\\-.[:digit:]]*";

  TString pattern_entry = pattern_double.data();
  pattern_entry.Append("[,[:space:]*").Append(pattern_double.data()).Append("]*");
  TPRegexp regexpParser_entry(pattern_entry);

  TString entry_tstring = entry.data();
  //std::cout << "entry_tstring = " << entry_tstring << std::endl;

  std::vector<double> elements;

  if ( regexpParser_entry.Match(entry_tstring) == 1 ) { 
//--- iterate over all vector elements (separated by ",")
    TObjArray* subStrings = entry_tstring.Tokenize(",");
    unsigned numElements = subStrings->GetEntries();
    for ( unsigned iElement = 0; iElement < numElements; ++iElement ) {
      std::string element_string = ((TObjString*)subStrings->At(iElement))->GetString().ReplaceAll(" ", "").Data();
      //std::cout << " element_string = " << element_string << std::endl;
      double element = atof(element_string.data());
      //std::cout << " element = " << element << std::endl;
      elements.push_back(element);
    }
    delete subStrings;
  } else {
    edm::LogError ("decodeVDoubleStringRep") << " Error in parsing string = " << entry << " !!";
    error = 1;
  }
  
  return elements;
}

std::string encodeVStringStringRep(const std::vector<std::string>& elements)
{
  std::ostringstream entry;

  unsigned numElements = elements.size();
  for ( unsigned iElement = 0; iElement < numElements; ++iElement ) {
    entry << elements[iElement];
    if ( iElement < (numElements - 1) ) entry << ", ";
  }
  
  return entry.str();
}

std::vector<std::string> decodeVStringStringRep(const std::string& entry, int& error)
{
  const std::string pattern_string = "[[:alnum:]]+";

  TString pattern_entry = pattern_string.data();
  pattern_entry.Append("[,[:space:]*").Append(pattern_string.data()).Append("]*");
  TPRegexp regexpParser_entry(pattern_entry);

  TString entry_tstring = entry.data();
  //std::cout << "entry_tstring = " << entry_tstring << std::endl;

  std::vector<std::string> elements;

  if ( regexpParser_entry.Match(entry_tstring) == 1 ) { 
//--- iterate over all vector elements (separated by ",")
    TObjArray* subStrings = entry_tstring.Tokenize(",");
    unsigned numElements = subStrings->GetEntries();
    for ( unsigned iElement = 0; iElement < numElements; ++iElement ) {
      std::string element = ((TObjString*)subStrings->At(iElement))->GetString().ReplaceAll(" ", "").Data();
      //std::cout << " element = " << element << std::endl;
      elements.push_back(element);
    }
    delete subStrings;
  } else {
    edm::LogError ("decodeVStringStringRep") << " Error in parsing string = " << entry << " !!";
    error = 1;
  }
  
  return elements;
}
