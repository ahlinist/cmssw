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
  
  string InputFileName  = c1.getValue<string> ("InputFileName");
  vector<int> HLTIndex  = c1.getVector<int>   ("HLTIndex");
  string OutputFileName = c1.getValue<string> ("OutputFileName");  
  int NPT               = c1.getValue<int>    ("NPT");
  int NETA              = c1.getValue<int>    ("NETA");
     
  if (!c1.check()) return 0;
  c1.print();
  const int MAX_NETA = 82;
  const int MAX_NPT  = 20;
    
  int NIndex = HLTIndex.size();
  if (NIndex != NPT)
    {
      cout<<"ERROR: SampleIndex vector size must be equal to the number of DijetPt Bins!!!!!"<<endl;
      exit(1);
    }  
  if (NETA>MAX_NETA)
    {
      cout<<"WARNING: too many eta bins!!!! Setting default value 82"<<endl;
      NETA = 82;
    } 
  if (NPT>MAX_NPT)
    {
      cout<<"WARNING: too many dijetPt bins!!!! Setting default value 20"<<endl;
      NPT = 20;
    }    
/////////////////////////////////////////////////////////////////////////
 
 char name[1024];
 int ptbin,etabin,ind;
 TFile *inf;
 TFile *outf; 
 TH1F *h;
 outf = new TFile(OutputFileName.c_str(),"RECREATE");
 cout<<"Creating combined file: "<<OutputFileName<<endl;
 cout<<"Reading file: "<<endl; 
 inf = new TFile(InputFileName.c_str(),"R");
 cout<<InputFileName<<" "<<endl;
 for(ptbin=0;ptbin<NPT;ptbin++)
   {
     ind = HLTIndex[ptbin];
     cout<<"DijetPt bin:"<<ptbin<<", HLTIndex:"<<ind<<endl;
     sprintf(name,"Ratio_DijetPt%d_HLT%d",ptbin,ind);
     h = (TH1F*)inf->Get(name);
     outf->cd();
     sprintf(name,"Ratio_DijetPt%d",ptbin);
     h->Write(name);
     sprintf(name,"RatioCuts_DijetPt%d_HLT%d",ptbin,ind);
     h = (TH1F*)inf->Get(name);
     outf->cd();
     sprintf(name,"RatioCuts_DijetPt%d",ptbin);
     h->Write(name); 
     sprintf(name,"PtJet3_DijetPt%d_HLT%d",ptbin,ind);
     h = (TH1F*)inf->Get(name);
     outf->cd();
     sprintf(name,"PtJet3_DijetPt%d",ptbin);
     h->Write(name);
     sprintf(name,"PtJet3Cuts_DijetPt%d_HLT%d",ptbin,ind);
     h = (TH1F*)inf->Get(name);
     outf->cd();
     sprintf(name,"PtJet3Cuts_DijetPt%d",ptbin);
     h->Write(name);  
     sprintf(name,"DPhi_DijetPt%d_HLT%d",ptbin,ind);
     h = (TH1F*)inf->Get(name);
     outf->cd();
     sprintf(name,"DPhi_DijetPt%d",ptbin); 
     h->Write(name); 
     sprintf(name,"DPhiCuts_DijetPt%d_HLT%d",ptbin,ind);
     h = (TH1F*)inf->Get(name);
     outf->cd();
     sprintf(name,"DPhiCuts_DijetPt%d",ptbin);
     h->Write(name);
     for(etabin=0;etabin<NETA;etabin++)
      { 
        sprintf(name,"B_DijetPt%d_Eta%d_HLT%d",ptbin,etabin,ind);
        h = (TH1F*)inf->Get(name);
        outf->cd();
        sprintf(name,"B_DijetPt%d_Eta%d",ptbin,etabin);
        h->Write(name);
        sprintf(name,"PtProbe_DijetPt%d_Eta%d_HLT%d",ptbin,etabin,ind);
        h = (TH1F*)inf->Get(name);
        outf->cd();
        sprintf(name,"PtProbe_DijetPt%d_Eta%d",ptbin,etabin); 
        h->Write(name);
      } 
   }
 outf->Close();  
}
