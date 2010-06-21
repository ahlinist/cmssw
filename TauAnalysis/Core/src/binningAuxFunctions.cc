#include "TauAnalysis/Core/interface/binningAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TMath.h>

#include <iostream>
#include <iomanip>

void printBinCenterPosition(std::ostream& stream, const BinGrid* binGrid, unsigned binNumber, bool addNewLine)
{
  unsigned numBins = binGrid->numBins();
  if ( binNumber >= 0 && binNumber < numBins ) {
    stream << " bin " << std::setw(2) << binNumber << " (center: ";

    std::vector<double> binCenter = binGrid->binCenter(binNumber);

    const std::vector<std::string>& objVarNames = binGrid->objVarNames();
    if ( binCenter.size() != objVarNames.size() ) {
      edm::LogError ("printBinCenterPosition") << "Invalid dimension of bin-center vector !!";
      return;
    }
    
    unsigned numObjVarNames = objVarNames.size();
    for ( unsigned iObjVar = 0; iObjVar < numObjVarNames; ++iObjVar ) {
      stream << objVarNames[iObjVar] << " = " << std::setprecision(3) << std::fixed << binCenter[iObjVar];
      if ( iObjVar < (numObjVarNames - 1) ) stream << ", ";
    }
    
    stream << "): ";
    if ( addNewLine ) stream << std::endl;
  } else {
    edm::LogError ("printBinCenterPosition") << "Invalid binNumber = " << binNumber << " !!";
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

double getBinContent(const std::vector<binResultType>& binResult, const char* name)
{
  for ( std::vector<binResultType>::const_iterator binResult_i = binResult.begin();
	binResult_i != binResult.end(); ++binResult_i ) {
    if ( binResult_i->name_ == name ) return binResult_i->binContent_;
  }

  edm::LogError ("getBinContent") 
    << "binResult passed as function argument does not contain an entry of name = " << name << " !!";
  return 0.;
}

double getBinSumw2(const std::vector<binResultType>& binResult, const char* name)
{
  for ( std::vector<binResultType>::const_iterator binResult_i = binResult.begin();
	binResult_i != binResult.end(); ++binResult_i ) {
    if ( binResult_i->name_ == name ) return binResult_i->binSumw2_;
  }

  edm::LogError ("getBinSumw2") 
    << "binResult passed as function argument does not contain an entry of name = " << name << " !!";
  return 0.;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

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

//
//-----------------------------------------------------------------------------------------------------------------------
//

void computeAcceptance(const binEntryType_model& binEntry, double& acceptance, double& acceptanceErr2)
{
  double gen = binEntry.gen_.binContent_;
  double rec = binEntry.rec_.binContent_;
  double stay = binEntry.stay_.binContent_;
  double stayErr2 = binEntry.stay_.binSumw2_;
  double lost = binEntry.lost_.binContent_;
  double lostErr2 = binEntry.lost_.binSumw2_;
  double smearIn = binEntry.smearIn_.binContent_;
  double smearInErr2 = binEntry.smearIn_.binSumw2_;
  double smearOut = binEntry.smearOut_.binContent_;
  double smearOutErr2 = binEntry.smearOut_.binSumw2_;
  
  if ( gen != 0. ) {
    acceptance = rec/gen;
    
    double denominator = TMath::Power(stay + smearOut + lost, -2.);
    
    acceptanceErr2 = TMath::Power((smearOut + lost - smearIn)*denominator, 2.)*stayErr2
                    + denominator*smearInErr2 + TMath::Power(rec*denominator, 2.)*(lostErr2 + smearOutErr2);
  } else {
    acceptance = 0.;
    acceptanceErr2 = 0.;
  }
}

void computePurity(const binEntryType_model& binEntry, double& purity, double& purityErr2)
{
  double rec = binEntry.rec_.binContent_;
  double stay = binEntry.stay_.binContent_;
  double stayErr2 = binEntry.stay_.binSumw2_;
  double smearIn = binEntry.smearIn_.binContent_;
  double smearInErr2 = binEntry.smearIn_.binSumw2_;
  
  if ( rec != 0. ) {
    purity = stay/rec;

    double denominator = TMath::Power(stay + smearIn, -2.);

    purityErr2 = TMath::Power(smearIn*denominator, 2.)*stayErr2
                + TMath::Power(stay*denominator, 2.)*smearInErr2;
  } else {
    purity = 0.;
    purityErr2 = 0.;
  }
}

void computeStability(const binEntryType_model& binEntry, double& stability, double& stabilityErr2)
{
  double stay = binEntry.stay_.binContent_;
  double stayErr2 = binEntry.stay_.binSumw2_;
  double smearOut = binEntry.smearOut_.binContent_;
  double smearOutErr2 = binEntry.smearOut_.binSumw2_;
    
  if ( (stay + smearOut) != 0. ) {
    stability = stay/(stay + smearOut);

    double denominator = TMath::Power(stay + smearOut, -2.);
    
    stabilityErr2 = TMath::Power(smearOut*denominator, 2.)*stayErr2
                   + TMath::Power(stay*denominator, 2.)*smearOutErr2;
  } else {
    stability = 0.;
    stabilityErr2 = 0.;
  }
}

