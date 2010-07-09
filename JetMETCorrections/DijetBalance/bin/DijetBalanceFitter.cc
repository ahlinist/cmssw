#include <iostream>
#include <string.h>
#include <fstream>
#include <cmath>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TKey.h>
#include <TROOT.h>
#include <TMath.h>
#include "JetMETAnalysis/JetUtilities/interface/CommandLine.h"
#include "JetMETCorrections/DijetBalance/interface/JetUtil.h"
#include "JetMETCorrections/DijetBalance/interface/Utilities.h"
using namespace std;

int main(int argc, char**argv)
{
 
/////////////////////////////////////////////////////////////////////////
  CommandLine c1;
  c1.parse(argc,argv);
  
  string HistoFilename      = c1.getValue<string> ("HistoFilename");
  string FitterFilename     = c1.getValue<string> ("FitterFilename");
  bool ETA_SYMMETRY         = c1.getValue<bool>   ("ETA_SYMMETRY",false);
  vector<string> JetAlgos   = c1.getVector<string>("JetAlgos","");
  vector<double> DijetPtBnd = c1.getVector<double>("DijetPtBoundaries");
  vector<double> EtaBnd     = c1.getVector<double>("EtaBoundaries");
  if (!c1.check()) return 0;
  c1.print();
   
  char name[1024];
  int ptbin,etabin,i;
  double mPt,ePt,sPt,mB,eB,sB,r,e;
  const int MAX_NETA = 83;
  const int MAX_NPT = 21;
  int NPT = DijetPtBnd.size()-1;
  if (!ETA_SYMMETRY) {
    cout<<"ETA_SYMMETRY is FALSE: unfolding eta bins...."<<endl;
    vector<double> newEtaBnd;
    for(unsigned i=EtaBnd.size()-1;i>0;i--)
      newEtaBnd.push_back(-1*EtaBnd[i]);
    for(unsigned i=0;i<EtaBnd.size();i++)
      newEtaBnd.push_back(EtaBnd[i]);
    EtaBnd.clear();
    for(unsigned i=0;i<newEtaBnd.size();i++)
      EtaBnd.push_back(newEtaBnd[i]);   
  }
  int NETA = EtaBnd.size()-1;
  double pt_vec[MAX_NPT],eta_vec[MAX_NETA];
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
  for(i=0;i<=NPT;i++)
    pt_vec[i] = DijetPtBnd[i];
  for(i=0;i<=NETA;i++)
    eta_vec[i] = EtaBnd[i];        
  TFile *inf;
  TFile *outf; 
  TH1F *h;
  inf = new TFile(HistoFilename.c_str(),"r");  
  outf = new TFile(FitterFilename.c_str(),"RECREATE");
TIter next(inf->GetListOfKeys());
  TKey* key(0);

while ((key=(TKey*)next())) {
  if (strcmp(key->GetClassName(),"TDirectoryFile")!=0) continue;
    
  TDirectoryFile* idir = (TDirectoryFile*)key->ReadObj();
  string alg(idir->GetName());
  if (JetAlgos.size()>0&&!contains(JetAlgos,alg)) continue;
  cout<<alg<<" ... "<<endl;
   cout<<"Booking histograms"<<endl;
  TDirectoryFile* odir = (TDirectoryFile*)outf->mkdir(alg.c_str());
  odir->cd();
  TH1F *MeanPt[MAX_NETA];
  TH1F *RelativeResponse[MAX_NPT];

  for(etabin=0;etabin<NETA;etabin++)
    { 
      sprintf(name,"MeanPt_Eta%d",etabin);
      MeanPt[etabin] = new TH1F(name,name,NPT,pt_vec);
    }
  for (ptbin=0; ptbin<NPT; ptbin++)//loop over DijetPt bins
    { 
      sprintf(name,"RelativeResponse_Pt%d",ptbin); 
      RelativeResponse[ptbin] = new TH1F(name,name,NETA,eta_vec);
      for(etabin=0;etabin<NETA;etabin++)
        {
          //////////////////////////////////////////////////////////////
          sprintf(name,"B_DijetPt%d_Eta%d",ptbin,etabin);
          h = (TH1F*)idir->Get(name);
          GetMEAN(h,mB,eB,sB);
          //GetMPV(h,outf,mB,eB,sB,seB);
          r = (2.+mB)/(2.-mB);
          e = (4.*eB)/pow(2-mB,2);
          RelativeResponse[ptbin]->SetBinContent(etabin+1,r);
          RelativeResponse[ptbin]->SetBinError(etabin+1,e);
	  /////////////////////////////////////////////////////////////
          sprintf(name,"PtProbe_DijetPt%d_Eta%d",ptbin,etabin);
          h = (TH1F*)idir->Get(name);
          GetMEAN(h,mPt,ePt,sPt);
          MeanPt[etabin]->SetBinContent(ptbin+1,mPt);
          MeanPt[etabin]->SetBinError(ptbin+1,ePt);
        }//end of eta loop 
    }// end of Pt loop  
  ////////////////////// writing ///////////////////////////////
  odir->cd();
  for(etabin=0;etabin<NETA;etabin++)
    MeanPt[etabin]->Write();
  for(ptbin=0;ptbin<NPT;ptbin++)
    RelativeResponse[ptbin]->Write();  
}// algo loop  
  gROOT->GetListOfFiles()->Remove(outf);
  outf->Close();
  delete outf;
  inf->Close();
  delete inf;
  return 0;  
}
