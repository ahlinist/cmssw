#include <iostream>
#include <string.h>
#include <fstream>
#include <cmath>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TMath.h>
#include "JetMETAnalysis/JetUtilities/interface/CommandLine.h"
using namespace std;

int main(int argc, char**argv)
{
  CommandLine c1;
  c1.parse(argc,argv);
  
  vector<string> InputFileNames = c1.getVector<string>("InputFileNames");
  vector<int> SampleIndex       = c1.getVector<int>("SampleIndex");
  string OutputFileName         = c1.getValue<string>("OutputFileName");  
  int NPtBins                   = c1.getValue<int>("NPtBins");
  int NETA                      = c1.getValue<int>("NETA");
   
  if (!c1.check()) return 0;
  c1.print();
  const int MAX_NETA = 82;
  const int MAX_NPtBins = 20;
  const int MAX_NSamples = 20; 
  int NSamples = InputFileNames.size();
  int NIndex = SampleIndex.size();
  if (NIndex != NPtBins)
    {
      cout<<"ERROR: SampleIndex vector size must be equal to the number of DijetPt Bins!!!!!"<<endl;
      exit(1);
    }  
  if (NETA>MAX_NETA)
    {
      cout<<"WARNING: too many eta bins!!!! Setting default value 82"<<endl;
      NETA = 82;
    } 
  if (NPtBins>MAX_NPtBins)
    {
      cout<<"WARNING: too many dijetPt bins!!!! Setting default value 20"<<endl;
      NPtBins = 20;
    }    
/////////////////////////////////////////////////////////////////////////
 
 char name[1024];
 int j,ptbin,etabin,ind;
 TFile *inf[MAX_NSamples];
 TFile *outf; 
 TH1F *h;
 outf = new TFile(OutputFileName.c_str(),"RECREATE");
 cout<<"Creating combined file: "<<OutputFileName<<endl;
 cout<<"Reading files: "<<endl; 
 for(j=0;j<NSamples;j++)
   {
     inf[j] = new TFile(InputFileNames[j].c_str(),"R");
     cout<<InputFileNames[j]<<" "<<endl;
   }
 for(ptbin=0;ptbin<NPtBins;ptbin++)
   {
     ind = SampleIndex[ptbin];
     cout<<"DijetPt bin:"<<ptbin<<", SampleIndex:"<<ind<<endl;
     sprintf(name,"RatioJet3_DijetPtBin%d",ptbin);
     h = (TH1F*)inf[ind]->Get(name);
     outf->cd();
     h->Write();
     sprintf(name,"RatioJet3Cuts_DijetPtBin%d",ptbin);
     h = (TH1F*)inf[ind]->Get(name);
     outf->cd();
     h->Write(); 
     sprintf(name,"PtJet3_DijetPtBin%d",ptbin);
     h = (TH1F*)inf[ind]->Get(name);
     outf->cd();
     h->Write();
     sprintf(name,"PtJet3Cuts_DijetPtBin%d",ptbin);
     h = (TH1F*)inf[ind]->Get(name);
     outf->cd();
     h->Write();  
     sprintf(name,"dPhiJet12_DijetPtBin%d",ptbin);
     h = (TH1F*)inf[ind]->Get(name);
     outf->cd();
     h->Write(); 
     sprintf(name,"dPhiJet12Cuts_DijetPtBin%d",ptbin);
     h = (TH1F*)inf[ind]->Get(name);
     outf->cd();
     h->Write();
     for(etabin=0;etabin<NETA;etabin++)
      { 
        sprintf(name,"B_DijetPtBin%d_EtaBin%d",ptbin,etabin);
        h = (TH1F*)inf[ind]->Get(name);
        outf->cd();
        h->Write();
        sprintf(name,"PtProbeJet_DijetPtBin%d_EtaBin%d",ptbin,etabin);
        h = (TH1F*)inf[ind]->Get(name);
        outf->cd();
        h->Write();
      } 
   }
 outf->Close();  
}
