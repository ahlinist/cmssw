#include "DataSet.h"



DataSet::DataSet(TString RootFilePath, TString ProcessName, float XSection, float Lum, TString ReferenceHistogramPath)
{
  SetProcessName( ProcessName) ;
  SetCrossSection(XSection);
  SetLuminosity(Lum);
  ReferenceHistogramPath_  = ReferenceHistogramPath;
  RootFilePath_ = RootFilePath;

  SetScaleFactor(1);
  SetNumberOfGeneratedEvents(0);
  SetNumberOfScaledEvents(0);
}

void DataSet::LoadFile()
{
  TFile F(RootFilePath_);
  
  //  F.ls();
  TH1F *RefHistogram = (TH1F*) F.Get(ReferenceHistogramPath_);
  
  SetNumberOfGeneratedEvents( RefHistogram->GetEntries()) ;
  
  if(NumberOfGeneratedEvents())
    SetScaleFactor( Luminosity() * CrossSection() / NumberOfGeneratedEvents() );
  else
    std::cout << "ERROR :  No Events/Entries in Reference Histogram with name " << ReferenceHistogramPath_ << endl;


  SetNumberOfScaledEvents( NumberOfGeneratedEvents() * ScaleFactor() );
  F.Close();


}

 
DataSet::~DataSet(){}


