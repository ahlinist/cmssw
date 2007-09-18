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

#include "CondFormats/BTauObjects/interface/CalibratedHistogram.h"
#include "CondFormats/EgammaObjects/interface/ElectronLikelihoodCalibration.h"
#include <TH1F.h>
#include <TDirectory.h>
#include <string>
#include <vector>
#include <map>

class LikelihoodPdf {

 public:
  LikelihoodPdf() {};
  LikelihoodPdf(const LikelihoodPdf& pdf) {}; 
  LikelihoodPdf(const char* name, const char* species, int ecalsubdet, int ptbin);
  virtual ~LikelihoodPdf();
  
  //! initialize PDFs from CondDB
  void initFromDB(const ElectronLikelihoodCalibration *calibration);

  //! split the pdf by category if splitPdf is true. split map is: <"class",classFraction>
  //! if splitPdf is false, pdf is splitted, but they are all equal (but allowing different priors)
  void split(std::map<std::string,float> splitFractions, bool splitPdf = false);

  //! get Value of pdf at point x for class catName
  float getVal(float x, std::string catName="NOSPLIT", bool normalized = true);

  //! get PDF name
  std::string getName() { return _name; }

  //! get PDF species
  std::string getSpecies() { return _species; }



 private:
  
  std::string _name;
  std::string _species;
  int _ecalsubdet;
  int _ptbin;

  std::map<std::string,const CalibratedHistogram*> _splitPdf;
  std::map<std::string,std::string> _splitRule;

};

#endif
