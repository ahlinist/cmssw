// -*- C++ -*-
//-----------------------------------------------------------------------
//
// Package:    
//      EgammaAnalysis/ElectronIDAlgos
// Description:
//      Class LikelihoodPdfProduct
//      class defining a likelihood as a product of LikelihoodPdf's
//      a priori probability can be given to any hypotesis 
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

#ifndef LikelihoodPdfProduct_h
#define LikelihoodPdfProduct_h

#include "EgammaAnalysis/ElectronIDAlgos/interface/LikelihoodSpecies.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/LikelihoodPdf.h"
#include <TDirectory.h>
#include <string>
#include <vector>
#include <map>

class LikelihoodPdfProduct {
 public:
  LikelihoodPdfProduct(const char* name);
  ~LikelihoodPdfProduct();
  
  // initialize the Pdf's from file
  void initFromFile(TDirectory *dir);
  // add a species (hypothesis) to the likelihood, with a priori probability 
  void addSpecies(const char* name, float priorWeight=1.);
  // add a pdf for a species, splitted or not
  void addPdf(const char* specname, const char* name, bool splitPdf=false);
  // set the fraction of one category for a given species
  void setSplitFrac(const char* specname, const char* catName, float frac);
  // get the likelihood ratio p(a priori) * L(specName) / L_tot
  float getRatio(const char* specName, std::vector<float> measurements, std::string);

 private:

  float getSpeciesProb(const char* specName, std::vector<float> measurements, std::string gsfClass);
  std::string _name;
  TDirectory * _directory;
  std::vector<LikelihoodSpecies*> _specList;
  std::vector<float> _priorList;

};
#endif
    
