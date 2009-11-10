#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TLeaf.h"
#include "TBranch.h"
#include <fstream>
#include <string>
#include <map>
#include <math.h>
#include "TChain.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TChain.h"

using namespace std;

int analyze(const char *infile,const char *outname){
  
  TFile *infile = new TFile(infile);
  TTree *intree = (TTree*)infile->Get("demo/calotree");
  
  if(TTree *intree = (TTree *)infile->Get("demo/calotree"))
    cout << "Analyzeing file: " << infile << endl;
  else {
    cout << "File has no TTree: " << endl;
    return 0;
  }

  TH2F * TowEtaPhi = new TH2F ("TowEtaPhi","TowEtaPhi", 82, -4., 4., 72, -3.2, 3.2);

  TH1F * PhiJ1J2   = new TH1F ("PhiJ1J2","PhiJ1J2", 64, 0., 3.2);
  TH1F * J1Et      = new TH1F ("J1Et","J1Et", 500, 0., 1000.);
  TH1F * J1Eta     = new TH1F ("J1Eta","J1Eta", 64, -4., 4.);
  TH1F * J1Phi     = new TH1F ("J1Phi","J1Phi", 64, -3.2, 3.2);
  TH1F * MET       = new TH1F ("MET","MET", 500, 0, 1000);

  TCanvas * c1 = new TCanvas( "c1", "PX", 300, 300);
  c1->Draw();

  Int_t nentries = (Int_t)intree->GetEntries();

  //CaloTowers
  const Int_t MAXCTOW = 5904;
  int nCTower;
  float TowEn[MAXCTOW];
  float TowEt[MAXCTOW];
  float TowEta[MAXCTOW];
  float TowPhi[MAXCTOW];

  const Int_t MAXJETS = 1000;
  Int_t nJets;
  Float_t JetPt[MAXJETS];
  Float_t JetEta[MAXJETS];
  Float_t JetPhi[MAXJETS];
  Float_t JetEMFR[MAXJETS];
  Int_t JetID[MAXJETS];
  Int_t JetIDTight[MAXJETS];
     
  Int_t HcalNoiseFilterTight;
  Int_t HcalNoiseFilter;

  TBranch *pmet  = (TBranch*)intree->GetBranch("calomet");

  intree->SetBranchAddress("nTowers",&nCTower);
  intree->SetBranchAddress("TowEn",&TowEn);
  intree->SetBranchAddress("TowEt",&TowEt);
  intree->SetBranchAddress("TowEta",&TowEta);
  intree->SetBranchAddress("TowPhi",&TowPhi);

  intree->SetBranchAddress("nJets",&nJets);
  intree->SetBranchAddress("JetsPT",&JetPt);
  intree->SetBranchAddress("JetsEta",&JetEta);
  intree->SetBranchAddress("JetsPhi",&JetPhi);
  intree->SetBranchAddress("JetsEMFR",&JetEMFR);     
  intree->SetBranchAddress("JetsID",&JetID);
  intree->SetBranchAddress("JetsIDTight",&JetIDTight);
   
  intree->SetBranchAddress("HcalNoiseFilter", &HcalNoiseFilter);    
  intree->SetBranchAddress("HcalNoiseFilterTight", &HcalNoiseFilterTight);
     
  cout<<"Total number of events: "<<nentries<<endl;
  for (Int_t i=0;i<nentries; i++)
    {
      if (i%100000 == 0) printf ("Entry: %d\n", i);

      intree->GetEntry(i);
      
      cout<<nCTower<<endl;
      for (Int_t j = 0; j< nCTower; j++)
	{
	  // 	  if(TowEt[j]>0.)
	  TowEtaPhi ->Fill(TowPhi[j], TowEta[j]);
	}
      
      PhiJ1J2->Fill(acos(cos(TowPhi[0]-TowPhi[1])));
      
      for (Int_t j = 0; j< nJets; j++)
	{
	  {
	    J1Eta->Fill(JetEta[j]);
	    J1Phi->Fill(JetPhi[j]);
	  }
	}
     
      Float_t met = pmet->GetLeaf("met")->GetValue();
      
    }
  
  //////////////////
  //Save the output
  //////////////////

  TFile *outfile;
  char name[200];
  sprintf(name,"%s.root",outname);
  outfile = new TFile(name,"RECREATE","ROOT file with histograms");
  outfile->Close();

  outfile = new TFile(name,"UPDATE","ROOT file with histograms");
  TowEtaPhi->Write();
  PhiJ1J2->Write();
  J1Eta->Write();
  J1Phi->Write();
  J1Et->Write();
  outfile->Close();
  
  delete infile;

  return 0;
}
