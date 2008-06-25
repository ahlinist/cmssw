#include<iostream>
#include "TFile.h"
#include "TTree.h" 
using namespace std; 

int main(){

//TFile hfile("histo.root"); 
TFile *f = new TFile("histo_pt50_1.root");
TTree* RecJet = (TTree*) f->Get("RecJet"); 

Float_t et_jet_centr, eta_jet_centr;
Float_t et_jet_add;  
Float_t e_jet_had;  
Int_t nTows; 
Int_t towId[5000]; 
Float_t towE[5000]; 

//RecJet->SetBranchAddress("et_jet_centr", &et_jet_centr); 
//RecJet->SetBranchAddress("eta_jet_centr", &eta_jet_centr); 
RecJet->SetBranchAddress("et_jet_add", &et_jet_add); 
RecJet->SetBranchAddress("e_jet_had", &e_jet_had);
RecJet->SetBranchAddress("nTows", &nTows);
RecJet->SetBranchAddress("towId", towId);
RecJet->SetBranchAddress("towE", towE);
 
float Et_thresh = 100; 

Int_t nentries = (Int_t) RecJet->GetEntries(); 
cout << "nentries = " << nentries << endl;  
for(Int_t i=0; i<nentries; i++){
  RecJet->GetEntry(i); 
  if(et_jet_add<Et_thresh){
  cout << e_jet_had << endl; 
  cout << nTows << endl; 
  for(Int_t j=0; j<nTows; j++){
   cout << towId[j] << " " << towE[j] << endl; 
  }
  }
}
}



