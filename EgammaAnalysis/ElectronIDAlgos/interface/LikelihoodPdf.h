// -*- C++ -*-
//-----------------------------------------------------------------------
//
// Package:    
//      EgammaAnalysis/ElectronIDAlgos
// Description:
//      Class LikelihoodPdf
//      class defining a pdf (eventually splitted by electron class)
//      taking the shape from an histogram
//      
// Original Authors:  Emanuele Di Marco, 
//                    Chiara Ilaria Rovelli, 
//                    Paolo Meridiani
// Universita' di Roma "La Sapienza" and INFN Roma
//
// Created:  Fri Jun  1 13:37:18 CEST 2007
//
//-----------------------------------------------------------------------

#ifndef LikelihoodPdf_H
#define LikelihoodPdf_H

#include <TH1F.h>
#include <TDirectory.h>
#include <string>
#include <vector>
#include <map>

class LikelihoodPdf {
 public:
  LikelihoodPdf() {};
  LikelihoodPdf(const LikelihoodPdf& pdf) {}; 
  LikelihoodPdf(const char* name, const char* species);
  virtual ~LikelihoodPdf();
  
  // initialize the pdf reading histograms from file
  void initFromFile(TDirectory *dir);
  // split the pdf by category if splitPdf is true. split map is: <"class",classFraction>
  // if splitPdf is false, pdf is splitted, but they are all equal
  void split(std::map<std::string,float> splitFractions, bool splitPdf = false);
  // get Value of pdf at point x for class catName
  float getVal(float x, std::string catName="NOSPLIT", bool normalized = true);

 private:
  
  int findBin(float x, TH1F* histo);
  std::string _name;
  std::string _species;
  
  std::map<std::string,TH1F*> _splitPdf;
  std::map<std::string,std::string> _splitRule;

  TDirectory* _tdirectory;

};

#endif
