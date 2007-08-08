// -*- C++ -*-
//-----------------------------------------------------------------------
//
// Package:    
//      EgammaAnalysis/ElectronIDAlgos
// Description:
//      Class LikelihoodSpecies
//      class defining a species (hypotesis) for a GsfLikelihood
//      species are: electrons, pions, kaons...
//      
// Original Authors: Emanuele Di Marco, 
//                   Chiara Ilaria Rovelli,
//                   Paolo Meridiani
//
// Universita' di Roma "La Sapienza" and INFN Roma
//
// Created:  Fri Jun  1 13:37:18 CEST 2007
//
//-----------------------------------------------------------------------

#ifndef LikelihoodSpecies_h
#define LikelihoodSpecies_h

#include "EgammaAnalysis/ElectronIDAlgos/interface/LikelihoodPdf.h"
#include <vector>
#include <string>
#include <map>

class LikelihoodSpecies {
 public:
  LikelihoodSpecies() {};
  LikelihoodSpecies(const char* name, float prior);

  virtual ~LikelihoodSpecies();

  // modifiers
  void setName(const char* name);
  void addPdf(LikelihoodPdf* pdf);
  void setPrior(float prior);
  void setSplitFraction(std::pair<std::string,float> splitfrac);

  // methods
  std::vector<LikelihoodPdf*> getListOfPdfs();
  const char* getName();
  float getPrior();
  std::map<std::string,float> getSplitFractions();

 private:
  std::vector<LikelihoodPdf*> _pdfList;
  std::string _name;
  float _prior;
  std::map<std::string,float> _splitFractions;

};
#endif
