#include "EgammaAnalysis/ElectronIDAlgos/interface/LikelihoodPdf.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <iostream>

LikelihoodPdf::LikelihoodPdf(const char* name, const char* species) {
  _name = std::string(name);
  _species = std::string(species);
}

LikelihoodPdf::~LikelihoodPdf() {
  //  delete _tdirectory;
}

void LikelihoodPdf::split(std::map<std::string,float> splitFractions, bool splitPdf) {
  if(splitFractions.size()>0 && splitPdf) {
    std::map<std::string,float>::const_iterator splitCatItr;
    for(splitCatItr=splitFractions.begin();splitCatItr!=splitFractions.end();splitCatItr++) {
      std::string totPdfName = _name + "_" + _species + "_" + splitCatItr->first;
      _splitRule.insert( std::make_pair(splitCatItr->first,totPdfName) );
    }
  }
  else if(splitFractions.size()>0) {
    std::map<std::string,float>::const_iterator splitCatItr;
    for(splitCatItr=splitFractions.begin();splitCatItr!=splitFractions.end();splitCatItr++) {
      std::string totPdfName = _name + "_" + _species;
      _splitRule.insert( std::make_pair(splitCatItr->first,totPdfName) );
    }
  }
  else {
    std::string totPdfName = _name + "_" + _species;
    _splitRule.insert( std::make_pair("NOSPLIT",totPdfName) );
  }
}

void LikelihoodPdf::initFromFile(TDirectory *dir) {
  _tdirectory = dir;
  std::map<std::string,std::string>::const_iterator ruleItr;
  for(ruleItr=_splitRule.begin();ruleItr!=_splitRule.end();ruleItr++) {
    TH1F *histo = (TH1F*)_tdirectory->Get(ruleItr->second.c_str());
    if(!histo) {
      throw cms::Exception("LikelihoodPdf") << "LikelihoodPdf ERROR\thisto " << ruleItr->first.c_str() << " not found";
    }
    else {
      _splitPdf.insert( std::make_pair(ruleItr->first,histo) );
    }
  }
}

float LikelihoodPdf::getVal(float x, std::string gsfClass, bool normalized) {
  _tdirectory->cd();
  TH1F *thePdf=0;
  if(_splitPdf.size()>1) {
    edm::LogInfo("LikelihoodPdf") << "The PDF is SPLITTED by GsfClass";
    thePdf=_splitPdf.find(gsfClass)->second;
  }
  else {
    edm::LogInfo("Gsf") << "It is UNSPLITTED";
    thePdf=_splitPdf.find("NOSPLIT")->second;
  }
  
  float prob=-1;
  if(normalized)
    prob=thePdf->GetBinContent(findBin(x,thePdf))/thePdf->Integral();
  else
    prob=thePdf->GetBinContent(findBin(x,thePdf));

  edm::LogInfo("LikelihoodPdf") << "sanity check: PDF name = " << _name
			 << " for species = " << _species
			 << " for class = " << gsfClass 
			 << " bin content = " << thePdf->GetBinContent(findBin(x,thePdf))
			 << " prob = " << prob;

  return prob;
}

int LikelihoodPdf::findBin(float x, TH1F* histo) {
  if (x<histo->GetXaxis()->GetXmin())
    return 0;
  else if (x>histo->GetXaxis()->GetXmax())
    return (histo->GetNbinsX()+1);
  else
    return histo->GetXaxis()->FindFixBin(x);
}


