#ifndef DATASET_H
#define DATASET_H

#include "TH1F.h"
#include "TString.h"
#include "TFile.h"

#include <iomanip>
#include <iostream>
using namespace std;

class DataSet {
 public:
  
  DataSet(TString RootFilePath, TString ProcessName, float CrossSection, float Luminosity, TString ReferenceHistogramPath);
  ~DataSet();
  
  void LoadFile();

  //Setters
  void SetCrossSection(float x) { CrossSection_ = x ;}
  void SetLuminosity( float x) { Luminosity_  = x ;}
  void SetNumberOfGeneratedEvents(float x) { NumberOfGenEvents_ = x;}
  void SetProcessName( TString x ) { ProcessName_ = x ; }
  void SetRootFilePath(TString x ) { RootFilePath_ =x ;}
  void SetScaleFactor( float x ) { ScaleFactor_ = x ;}
  

  //Getters
  TString RootFilePath(){ return RootFilePath_ ; }
  TString ProcessName() { return ProcessName_ ; }
  float NumberOfGeneratedEvents(){ return NumberOfGenEvents_ ;}
  float NumberOfScaledEvent(){return NumberOfScaledEvents_ ;}
  float CrossSection() { return CrossSection_ ;}
  float Luminosity() {return Luminosity_ ;}
  float ScaleFactor() { return ScaleFactor_ ;}

  TFile F;

 private:

  void SetNumberOfScaledEvents(float x ){ NumberOfScaledEvents_ = x ;}
  TString RootFilePath_;
  TString ProcessName_;
  TString ReferenceHistogramPath_;
  float Luminosity_;
  float CrossSection_;
  
  float NumberOfGenEvents_;
  float NumberOfScaledEvents_;
  float ScaleFactor_;

};


#endif
