#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include "TFile.h"
#include "TROOT.h" 
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TImage.h"
#include "TRandom.h"
#include "TGaxis.h"
#include "TAxis.h"
#include "TCut.h"
#include "TPostScript.h"
#include "TF1.h"
#include "TLeaf.h"
#include <TLegend.h>
#include <TKey.h>

using namespace std;
TLegend *leg;

void loop(const char *name1, const char *name2, char *outdirname)
{
  TFile *file1 = new TFile(name1);
  gStyle->SetOptStat(1);
  gStyle->SetStatY(1);
  gStyle->SetStatX(0.7);
  gStyle->SetStatTextColor(1);

  char buffer[300];

  TH1 *his1[400]; 
  TH1 *his2[400]; 
  int k = 0;
  int l = 0;
  TKey* key1;
  TObject *obj1;

  TIter nextkey1(file1->GetListOfKeys());

  while ((key1 = (TKey*)nextkey1())) 
    {
      obj1 = key1->ReadObj();
      if ((obj1->IsA()->InheritsFrom("TH1"))) 
        {
	  if(obj1->IsA()->InheritsFrom(TH2::Class())) continue;
          his1[k] = (TH1*)obj1; 
	  his1[k] ->SetMarkerStyle(8);
          his1[k] ->SetLineColor(1);
	  cout << his1[k]->GetName() << endl;
          his1[k]->Draw();
          gPad->Modified();
          gPad->Update(); 
        }
      k++;
    }

  TFile *file2 = new TFile(name2);
  TKey* key2;
  TObject *obj2;

  TIter nextkey2(file2->GetListOfKeys());

  gStyle->SetStatY(1);
  gStyle->SetStatX(0.9);
  gStyle->SetStatTextColor(2);

  while ((key2 = (TKey*)nextkey2())) 
    {
      obj2 = key2->ReadObj();
      if ((obj2->IsA()->InheritsFrom("TH1"))) 
        {
	  if(obj2->IsA()->InheritsFrom(TH2::Class())) continue;
          his2[l] = (TH1*)obj2;
          cout << his2[l]->GetName() << endl;
	  his2[l] ->SetLineColor(2);
 	  his2[l] ->SetFillColor(2);
	  his2[l] ->GetXaxis()->SetTitle(Form("%s",his2[l]->GetName()));
          his2[l]->Draw();
          gPad->Modified();
          gPad->Update(); 
        }
      l++;
    }

  TCanvas *VarCanvas[100];
  
  for (int i=0; i<l; i++) 
    {
      VarCanvas[i] = new TCanvas(Form("%s",his2[i]->GetName()),Form("%s",his2[i]->GetName()),0,0,600,600);
      VarCanvas[i]->Divide(1,1);
      VarCanvas[i]->SetFixedAspectRatio(kTRUE);
    }

  TFile *outfile;
  char name[200];
  sprintf(name,"%s.root",outdirname);

  outfile = new TFile(name,"RECREATE","ROOT file with histograms");
  outfile->Close();
  
  outfile = new TFile(name,"UPDATE","ROOT file with histograms");

  for (int s=0; s<k; s++)
    {
      if(his1[s]!=NULL && his2[s]!=NULL && his1[s]->GetSumOfWeights()!=0)
        {
	  VarCanvas[s]->Draw();
	  VarCanvas[s]->cd(1);
	  his2[s]->Scale(his1[s]->Integral()/his2[s]->Integral());
          his2[s]->Draw("hist");
          his1[s]->Draw("same");
          leg = new TLegend(0.7,0.7,0.8,0.8);
          leg->AddEntry(his1[s],"Data","l");
          leg->AddEntry(his2[s],"MinBias MC","l");
          leg->Draw();
          gPad->Modified();
          gPad->Update();
	  VarCanvas[s]->Write();
        }
    }
}
