#include <iostream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TMath.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TVirtualFitter.h>
#include <TMatrixD.h>
#include "JetMETAnalysis/JetUtilities/interface/CommandLine.h"
#include "JetMETCorrections/DijetBalance/interface/JetUtil.h"
#include "JetMETCorrections/DijetBalance/interface/Utilities.h"
using namespace std;

int main(int argc, char**argv)
{
  CommandLine c1;
  c1.parse(argc,argv);
  
  string FitterFilename            = c1.getValue<string>("FitterFilename");
  string L2ROOTFilename            = c1.getValue<string>("L2ROOTFilename");
  string L2TXTFilename             = c1.getValue<string>("L2TXTFilename");
  vector<double> DijetPtBoundaries = c1.getVector<double>("DijetPtBoundaries");
  vector<double> EtaBoundaries     = c1.getVector<double>("EtaBoundaries");
  if (!c1.check()) return 0;
  c1.print();
  int NPtBins = DijetPtBoundaries.size()-1;
  int NETA = EtaBoundaries.size()-1;
  const int MAX_NETA = 82;
  const int MAX_NPtBins = 20;
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
 int i,auxi,etabin,ptbin,npar;
 char name[100],func[1024];
 double resp,e_resp,xPt,exPt,e_cor;
 double MinCaloPt[MAX_NETA],MaxCaloPt[MAX_NETA];
 double correction_x[MAX_NPtBins],correction_ex[MAX_NPtBins],correction_y[MAX_NPtBins],correction_ey[MAX_NPtBins];
 double L2_cor[10];
 
 TFile *inf;
 TFile *outf;
 TH1F *hresponse[MAX_NPtBins];
 TH1F *h;    
 TF1 *L2Correction[MAX_NETA];
 inf = new TFile(FitterFilename.c_str(),"r");
 ofstream L2File;
 TGraphErrors *g_L2Correction[MAX_NETA];
 TMatrixD *COV[MAX_NETA];
 /////////////////// Calculating the L2 Correction //////////////////////////////
 for (ptbin=0;ptbin<NPtBins;ptbin++) 
   {
     sprintf(name,"RelativeResponse_Pt%d",ptbin);
     hresponse[ptbin] = (TH1F*)inf->Get(name);
   }
 for (etabin=0;etabin<NETA;etabin++)
   {
     sprintf(name,"MeanPt_Eta%d",etabin);
     h = (TH1F*)inf->Get(name);
     ///////////// Correction calculation for every etabin  //////////  
     auxi = 0;
     for (ptbin=0;ptbin<NPtBins-1;ptbin++)
       { 
         resp = hresponse[ptbin]->GetBinContent(etabin+1);
         e_resp = hresponse[ptbin]->GetBinError(etabin+1);
         xPt = h->GetBinContent(ptbin+1);//average Probe Pt for the etabin
         exPt = h->GetBinError(ptbin+1);
         if (resp>0 && e_resp>0.0001 && xPt>0)
           {
             e_cor = e_resp/pow(resp,2);     
             if (e_cor<0.8 && e_cor>0.0001)
               {
                 correction_x[auxi]  = xPt;
                 correction_ex[auxi] = exPt;
                 correction_y[auxi]  = 1./resp;
                 correction_ey[auxi] = e_cor;
                 auxi++;          
               }
           }
       }
     if (auxi>1)
       {  
         MaxCaloPt[etabin]=correction_x[auxi-1];
         MinCaloPt[etabin]=correction_x[0];
         g_L2Correction[etabin] = new TGraphErrors(auxi,correction_x,correction_y,correction_ex,correction_ey);
         sprintf(name,"L2Correction%d",etabin);
         npar = 3;
         if (auxi>2)
           sprintf(func,"[0]+[1]*log10(x)+[2]*pow(log10(x),2)");
         else
           {
             sprintf(func,"[0]+[1]*log10(x)");
             npar = 2;
           }
         L2Correction[etabin] = new TF1(name,func,MinCaloPt[etabin],MaxCaloPt[etabin]);
         L2Correction[etabin]->SetParameter(0,0.);
         L2Correction[etabin]->SetParameter(1,0.);
         L2Correction[etabin]->SetParameter(2,0.);
         L2Correction[etabin]->SetParameter(3,0.);
         L2Correction[etabin]->SetParameter(4,0.);
         L2Correction[etabin]->SetParameter(5,0.);
         g_L2Correction[etabin]->Fit(name,"RQ");  
         TVirtualFitter *fitter = TVirtualFitter::GetFitter();
         COV[etabin] = new TMatrixD(npar,npar,fitter->GetCovarianceMatrix());
         cout<<name<<" fitted....."<<endl;
       }	
     else
       {  
         MaxCaloPt[etabin]= 1500.;
         MinCaloPt[etabin]= 1.;
	 correction_x[0] = MinCaloPt[etabin];
	 correction_x[1] = MaxCaloPt[etabin];
	 correction_y[0] = 1.;
	 correction_y[1] = 1.;
	 correction_ex[0] =0.;
	 correction_ex[1] = 0.;
	 correction_ey[0] = 0.;
	 correction_ey[1] = 0.;
         g_L2Correction[etabin] = new TGraphErrors(2,correction_x,correction_y,correction_ex,correction_ey);
         sprintf(name,"L2Correction%d",etabin);
         sprintf(func,"[0]+[1]*log10(x)");
         npar = 2;
         L2Correction[etabin] = new TF1(name,func,MinCaloPt[etabin],MaxCaloPt[etabin]);
         L2Correction[etabin]->SetParameter(0,0.);
         L2Correction[etabin]->SetParameter(1,0.);
         L2Correction[etabin]->SetParameter(2,0.);
         L2Correction[etabin]->SetParameter(3,0.);
         L2Correction[etabin]->SetParameter(4,0.);
         L2Correction[etabin]->SetParameter(5,0.);
         g_L2Correction[etabin]->Fit(name,"RQ");  
         TVirtualFitter *fitter = TVirtualFitter::GetFitter();
         COV[etabin] = new TMatrixD(npar,npar,fitter->GetCovarianceMatrix());
         cout<<name<<" fitted....."<<endl;
       }	                    
   }//end of etabin loop  

 //////////////////////// Writing //////////////////////////////
   L2File.open(L2TXTFilename.c_str());
   L2File.setf(ios::right);
   for(etabin=0;etabin<NETA;etabin++)
     {
       for(i=0;i<6;i++)
         L2_cor[i] = L2Correction[etabin]->GetParameter(i);
       L2File    << setw(11) << EtaBoundaries[etabin]
                 << setw(11) << EtaBoundaries[etabin+1]
                 << setw(11) << (int)8
                 << setw(12) << MinCaloPt[etabin]
                 << setw(12) << MaxCaloPt[etabin]
                 << setw(13) << L2_cor[0]
                 << setw(13) << L2_cor[1]
                 << setw(13) << L2_cor[2]
                 << setw(13) << L2_cor[3]
                 << setw(13) << L2_cor[4]
                 << setw(13) << L2_cor[5]
                 << "\n";
     } 
   L2File.close();
 cout<<L2TXTFilename<<" written...."<<endl;
 outf = new TFile(L2ROOTFilename.c_str(),"RECREATE"); 
 for(etabin=0;etabin<NETA;etabin++)
   {
     sprintf(name,"L2Correction_Eta%d",etabin);
     g_L2Correction[etabin]->Write(name);
     sprintf(name,"CovMatrix_Eta%d",etabin);
     COV[etabin]->Write(name);
   }
 outf->Close(); 
}
