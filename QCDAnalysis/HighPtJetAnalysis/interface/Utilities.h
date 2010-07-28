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
typedef struct 
{
  int   passLooseID;
  int   nTrkVtx;
  int   nTrkCalo;
  int   n90;
  int   n90hits;
  float rawPt;
  float corPt;
  float eta;
  float etaD;
  float y;
  float phi;
  float rawE;
  float corE;
  float trkVtxPt;
  float trkCaloPt;
  float fHPD;
  float fRBX;
  float shapeVar;
  float sigmaEta;
  float sigmaPhi;
  float emf;
  float chf;
  float gendR;
  float genPt;
  float genEta;
  float genPhi;
  float corRsp;
  float rawRsp;
} JET_VAR;
typedef struct 
{
  int   runNo;
  int   evtNo;
  int   lumi;
  int   bunch;
  int   njets;
  int   njetsHt;
  int   passLooseHcalNoise;
  int   passTightHcalNoise;
  int   nPV;
  int   PVntracks;
  float PVz;
  float PVx;
  float PVy;
  float PVndof;
  float PVchi2;
  float PVnormalizedChi2;
  float corMass;
  float rawMass; 
  float met;
  float metNoHF;
  float sumet;
  float sumetNoHF;
  float met_over_sumet; 
  float met_over_sumet_NoHF;
  float ht;
  float pthat;
  float weight;
} EVENT_VAR;
//  Each trigger path gets a branch with these guys in them
//  fired should be: -1 if something bad happened (i.e. path doesn't exist)
//                    0 existed but didn't fire
//                    1 fired.
//  prescale will be filled soon -- easy access exists but not yet part of
//           a release yet.  Currently hardwired to 1.
typedef struct 
{
  int prescale;
  int fired;
} TrigStruct;
void fillHist(const TString& histName,map<TString, TH1*> HistNames, const double& x);
void FillHist(const TString& histName, map<TString, TH1*> HistNames, const double& x, const double& wt);
void fillHist(const TString& histName,map<TString, TH2*> HistNames, const double& x, const double& y);
void BinNormalization(TH1F *h);
int  getBin(double x, vector<double> boundaries);
int  getBin(float x, vector<float> boundaries);
bool findName(string ss, vector<string> vv);
//////////////////////////////////////////////////////////////////////////////////////////
bool FindName(string ss, vector<string> vv)
{
  for(unsigned i=0;i<vv.size();i++)
    {
      if (vv[i] == ss)
        return true;
    }
  return false;
}
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
void FillHist(const TString& histName, map<TString, TH1*> HistNames, const double& x, const double& wt)
{
  map<TString, TH1*>::iterator hid = HistNames.find(histName);
  if (hid==HistNames.end())
    cout << "%fillHist -- Could not find histogram with name: " << histName << endl;
  else
    hid->second->Fill(x,wt);
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
//////////////////////////////////////////////////////////////////////////////////////////
int getBin(float x, std::vector<float> boundaries)
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
