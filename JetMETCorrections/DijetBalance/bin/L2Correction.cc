#include <iostream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TKey.h>
#include <TROOT.h>
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
  
  string FitterFilename   = c1.getValue<string> ("FitterFilename");
  string L2ROOTFilename   = c1.getValue<string> ("L2ROOTFilename");
  string BiasROOTFilename = c1.getValue<string> ("BiasROOTFilename","");
  string L2TXTFilename    = c1.getValue<string> ("L2TXTFilename");
  bool ETA_SYMMETRY       = c1.getValue<bool>   ("ETA_SYMMETRY",false);
  int NPtBins             = c1.getValue<int>    ("NPT");
  vector<string> JetAlgos = c1.getVector<string>("JetAlgos","");
  vector<double> EtaBnd   = c1.getVector<double>("EtaBoundaries");
  if (!c1.check()) return 0;
  c1.print();
  bool CORRECT_BIAS(false);
  if (BiasROOTFilename != "")
    CORRECT_BIAS = true;
  const int MAX_NETA = 82;
  const int MAX_NPtBins = 20;
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
 double resp,e_resp,respBias(1),e_respBias(1),xPt,exPt,e_cor,e_corBias;
 double MinCaloPt[MAX_NETA],MaxCaloPt[MAX_NETA];
 double correction_x[MAX_NPtBins],correction_ex[MAX_NPtBins];
 double correction_y[MAX_NPtBins],correction_ey[MAX_NPtBins],correctionBias_y[MAX_NPtBins],correctionBias_ey[MAX_NPtBins];
 double L2_cor[10];
 
 TFile *inf     = new TFile(FitterFilename.c_str(),"r");
 TFile *infBias(0);
 if (CORRECT_BIAS)
   infBias = new TFile(BiasROOTFilename.c_str(),"r"); 
 TFile *outf    = new TFile(L2ROOTFilename.c_str(),"RECREATE");
 TH1F *hresponse[MAX_NPtBins];
 TH1F *hresponseBias[MAX_NPtBins]; 
 TH1F *h;    
 TF1 *L2Correction[MAX_NETA];
 TF1 *L2CorrectionBias[MAX_NETA]; 
 ofstream L2File;
 TIter next(inf->GetListOfKeys());
 TKey* key(0);
 while ((key=(TKey*)next())) {
    if (strcmp(key->GetClassName(),"TDirectoryFile")!=0) continue;
    TDirectoryFile* idir = (TDirectoryFile*)key->ReadObj();
    string alg(idir->GetName());
    if (JetAlgos.size()>0&&!contains(JetAlgos,alg)) continue;
    cout<<alg<<" ... "<<endl;
    TDirectoryFile* idirBias(0);
    if (CORRECT_BIAS)   
      idirBias = (TDirectoryFile*)infBias->Get(alg.c_str());
    TDirectoryFile* odir = (TDirectoryFile*)outf->mkdir(alg.c_str());
    odir->cd();
    TGraphErrors *g_L2Correction[MAX_NETA];
    TGraphErrors *g_L2CorrectionBias[MAX_NETA];
    TMatrixD *COV[MAX_NETA];
    TMatrixD *COVBias[MAX_NETA];
    /////////////////// Calculating the L2 Correction //////////////////////////////
    for (ptbin=0;ptbin<NPtBins;ptbin++) 
      {
        sprintf(name,"RelativeResponse_Pt%d",ptbin);
        hresponse[ptbin] = (TH1F*)idir->Get(name);

        if (CORRECT_BIAS)
          hresponseBias[ptbin] = (TH1F*)idirBias->Get(name);
      }
    for (etabin=0;etabin<NETA;etabin++)
      {
        sprintf(name,"MeanPt_Eta%d",etabin);
        h = (TH1F*)idir->Get(name);
        ///////////// Correction calculation for every etabin  //////////  
        auxi = 0;
        for (ptbin=1;ptbin<NPtBins;ptbin++)
          { 
            resp   = hresponse[ptbin]->GetBinContent(etabin+1);
            e_resp = hresponse[ptbin]->GetBinError(etabin+1);
            if (CORRECT_BIAS)
              { 
                respBias   = hresponseBias[ptbin]->GetBinContent(etabin+1);
                e_respBias = hresponseBias[ptbin]->GetBinError(etabin+1);
              }
            xPt   = h->GetBinContent(ptbin+1);//average Probe Pt for the etabin
            exPt  = h->GetBinError(ptbin+1);
            if (resp>0 && e_resp>0.0001 && xPt>0)
              {
                e_cor = e_resp/pow(resp,2);     
                if (e_cor<0.8 && e_cor>0.0001)
                  {
                    correction_x[auxi]  = xPt;
                    correction_ex[auxi] = exPt;
                    correction_y[auxi]  = 1./resp;
                    correction_ey[auxi] = e_cor;
                    if (CORRECT_BIAS)
                      {
                        e_corBias = (respBias/resp)*sqrt(pow(e_respBias/respBias,2)+pow(e_resp/resp,2));
                        correctionBias_y[auxi]  = respBias/resp;
                        correctionBias_ey[auxi] = e_corBias;
                      } 
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
            npar = 1;
            if (auxi>30)
              //sprintf(func,"[0]+[1]*log10(x)+[2]*pow(log10(x),2)");
              //sprintf(func,"[0]+[1]*pow(x,-[2])"); 
              //sprintf(func,"[0]+[1]*x");
              sprintf(func,"[0]");
            else
              {
                //sprintf(func,"[0]+[1]*log10(x)");
                //sprintf(func,"[0]+[1]*x"); 
                sprintf(func,"[0]");
                npar = 1;
              }
            L2Correction[etabin] = new TF1(name,func,MinCaloPt[etabin],MaxCaloPt[etabin]);
            L2Correction[etabin]->SetParameter(0,0.);
            L2Correction[etabin]->SetParameter(1,0.);
            L2Correction[etabin]->SetParameter(2,0.);
            g_L2Correction[etabin]->Fit(name,"RQ");  
            TVirtualFitter *fitter = TVirtualFitter::GetFitter();
            COV[etabin] = new TMatrixD(npar,npar,fitter->GetCovarianceMatrix());
            if (CORRECT_BIAS)
              { 
                g_L2CorrectionBias[etabin] = new TGraphErrors(auxi,correction_x,correctionBias_y,correction_ex,correctionBias_ey);
                sprintf(name,"L2CorrectionBias%d",etabin);
                sprintf(func,"[0]"); 
                npar = 1;
                L2CorrectionBias[etabin] = new TF1(name,func,MinCaloPt[etabin],MaxCaloPt[etabin]);
                L2CorrectionBias[etabin]->SetParameter(0,0.);
                L2CorrectionBias[etabin]->SetParameter(1,0.);
                L2CorrectionBias[etabin]->SetParameter(2,0.);
                g_L2CorrectionBias[etabin]->Fit(name,"RQ"); 
                TVirtualFitter *fitter = TVirtualFitter::GetFitter();
                COVBias[etabin] = new TMatrixD(npar,npar,fitter->GetCovarianceMatrix()); 
              }
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
            correctionBias_y[0] = respBias;
            correctionBias_y[1] = respBias;
	    correction_ex[0] =0.;
	    correction_ex[1] = 0.;
	    correction_ey[0] = 0.;
	    correction_ey[1] = 0.;
            correctionBias_ey[0] = 0.;
            correctionBias_ey[1] = 0.;
            g_L2Correction[etabin] = new TGraphErrors(2,correction_x,correction_y,correction_ex,correction_ey);
            sprintf(name,"L2Correction%d",etabin);
            //sprintf(func,"[0]+[1]*log10(x)");
            //sprintf(func,"[0]+[1]*x");
            sprintf(func,"[0]");
            npar = 1;
            L2Correction[etabin] = new TF1(name,func,MinCaloPt[etabin],MaxCaloPt[etabin]);
            L2Correction[etabin]->SetParameter(0,0.);
            L2Correction[etabin]->FixParameter(1,0.);
            L2Correction[etabin]->SetParameter(2,0.);
            g_L2Correction[etabin]->Fit(name,"RQ");  
            TVirtualFitter *fitter = TVirtualFitter::GetFitter();
            COV[etabin] = new TMatrixD(npar,npar,fitter->GetCovarianceMatrix());
            if (CORRECT_BIAS)
              {
                g_L2CorrectionBias[etabin] = new TGraphErrors(2,correction_x,correctionBias_y,correction_ex,correctionBias_ey);
                sprintf(name,"L2CorrectionBias%d",etabin);
                sprintf(func,"[0]");
                npar = 1;
                L2CorrectionBias[etabin] = new TF1(name,func,MinCaloPt[etabin],MaxCaloPt[etabin]);
                L2CorrectionBias[etabin]->SetParameter(0,0.);
                L2CorrectionBias[etabin]->FixParameter(1,0.);
                L2CorrectionBias[etabin]->SetParameter(2,0.);
                g_L2CorrectionBias[etabin]->Fit(name,"RQ");
                TVirtualFitter *fitter = TVirtualFitter::GetFitter();
                COVBias[etabin] = new TMatrixD(npar,npar,fitter->GetCovarianceMatrix());
              }
            cout<<name<<" fitted....."<<endl;
          }	                    
      }//end of etabin loop  
    //////////////////////// Writing //////////////////////////////
    odir->cd();
    L2File.open(L2TXTFilename.c_str());
    L2File.setf(ios::right);
    L2File << "{ 1 JetEta 1 JetPt "<<func<<" Correction L2Relative} \n";
    for(etabin=0;etabin<NETA;etabin++)
      {
        int ieta = NETA-1-etabin;
        for(i=0;i<3;i++)
          {
            if (CORRECT_BIAS)
              L2_cor[i] = L2CorrectionBias[ieta]->GetParameter(i);
            else
              L2_cor[i] = L2Correction[ieta]->GetParameter(i);
          } 
        L2File    << setw(11) << -1.*EtaBnd[ieta+1]
             << setw(11) << -1.*EtaBnd[ieta]
             << setw(11) << (int)5
             << setw(12) << 4 //MinCaloPt[etabin]
             << setw(12) << 1000 //MaxCaloPt[etabin]
             << setw(13) << L2_cor[0]
             << setw(13) << L2_cor[1]
             << setw(13) << L2_cor[2]
             << "\n";
       }  
    for(etabin=0;etabin<NETA;etabin++)
      {
        for(i=0;i<3;i++)
          {
            if (CORRECT_BIAS) 
              L2_cor[i] = L2CorrectionBias[etabin]->GetParameter(i);
            else
              L2_cor[i] = L2Correction[etabin]->GetParameter(i);
          }
        L2File    << setw(11) << EtaBnd[etabin]
                 << setw(11) << EtaBnd[etabin+1]
                 << setw(11) << (int)5
                 << setw(12) << 4 //MinCaloPt[etabin]
                 << setw(12) << 1000 //MaxCaloPt[etabin]
                 << setw(13) << L2_cor[0]
                 << setw(13) << L2_cor[1]
                 << setw(13) << L2_cor[2]
                 << "\n";
      } 
   L2File.close();
   cout<<L2TXTFilename<<" written...."<<endl;
   //outf = new TFile(L2ROOTFilename.c_str(),"RECREATE"); 
   for(etabin=0;etabin<NETA;etabin++)
     {
       sprintf(name,"L2Correction_Eta%d",etabin);
       g_L2Correction[etabin]->Write(name);
       sprintf(name,"CovMatrix_Eta%d",etabin);
       COV[etabin]->Write(name);
       if (CORRECT_BIAS)
         {
           sprintf(name,"L2CorrectionNoBias_Eta%d",etabin);
           g_L2CorrectionBias[etabin]->Write(name);
           sprintf(name,"CovMatrixNoBias_Eta%d",etabin);
           COVBias[etabin]->Write(name);
         } 
     }
 }// algo loop
 gROOT->GetListOfFiles()->Remove(outf);
 outf->Close();
 delete outf;
 inf->Close();
 delete inf;
 return 0; 
}
