#ifndef UTILITIES_H
#define UTILITIES_H
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TNamed.h"
#include <vector>
#include <map>
using namespace std;

namespace
{
void fillHist(const TString& histName,map<TString, TH1*> HistNames, const double& x);
void fillHist(const TString& histName,map<TString, TH2*> HistNames, const double& x, const double& y);
void BinNormalization(TH1F *h);
int  getBin(double x, vector<double> boundaries);
//////////////////////////////////////////////////////////////////////////////////////////
void FillHist(const TString& histName, map<TString, TH1*> HistNames, const double& x) 
{
  map<TString, TH1*>::iterator hid = HistNames.find(histName);
  if (hid==HistNames.end())
    cout << "%fillHist -- Could not find histogram with name: " << histName << endl;
  else
    hid->second->Fill(x);
}
//////////////////////////////////////////////////////////////////////////////////////////
void FillHist(const TString& histName, map<TString, TH2*> HistNames, const double& x, const double& y) 
{
  map<TString, TH2*>::iterator hid = HistNames.find(histName);
  if (hid==HistNames.end())
    cout << "%fillHist -- Could not find histogram with name: " << histName << endl;
  else
    hid->second->Fill(x,y);
}
//////////////////////////////////////////////////////////////////////////////////////////
void BinNormalization(TH1F *h)
{
  int i;
  double x,dx,e;
  for(i=0;i<h->GetNbinsX();i++)
    {
      x = h->GetBinContent(i+1);
      e = h->GetBinError(i+1);
      dx = h->GetBinWidth(i+1);
      h->SetBinContent(i+1,x/dx);
      h->SetBinError(i+1,e/dx);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
int getBin(double x, std::vector<double> boundaries)
{
  int i;
  int n = boundaries.size()-1;
  if (x<boundaries[0] || x>=boundaries[n])
    return -1;
  for(i=0;i<n;i++)
   {
     if (x>=boundaries[i] && x<boundaries[i+1])
       return i;
   }
  return 0; 
}
}
#endif
