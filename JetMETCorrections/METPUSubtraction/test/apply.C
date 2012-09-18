#include "TMath.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "GBRTrainer.h"
#include "GBRForest.h"
#include "Cintex/Cintex.h"


void apply(std::string iName,std::string iTrain="../data/gbrmet_53_data_mc.root",bool iPhi=false) {
  ROOT::Cintex::Cintex::Enable();

  TFile *lFForest = 0; 
  if(iPhi)  lFForest = new TFile(iTrain.c_str());
  if(!iPhi) lFForest = new TFile(iTrain.c_str());
  
  const GBRForest *lForest = (GBRForest*)lFForest->Get("PhiCorrection");
  if(lForest == 0) lForest = (GBRForest*)lFForest->Get("U1Correction");
  const std::vector<std::string> *lVars = (std::vector<std::string>*)lFForest->Get("varlist");
  int lN = lVars->size();
  
  for (int ivar = 0; ivar<lN; ++ivar) {
    printf("%i: %s\n",ivar, lVars->at(ivar).c_str());
  }
  
  TString  lName         = iName;
  TFile *lInput = TFile::Open(lName); //v10
  TTree   *lTree       = (TTree*)lInput    ->Get("Flat");

  TTreeFormula **lFVars = new TTreeFormula*[lN];
  for(int i0 = 0; i0 < lN; i0++) { 
    TString pExpress = lVars->at(i0); 
    TString pExpr1(pExpress);
    lFVars[i0] = new TTreeFormula(pExpr1,pExpr1,lTree);
  }
  
  //float lNPU    = 0;
  float lU      = 0;
  float lUPhi   = 0;  
  float lPtVis  = 0;
  float lPhiVis = 0;
  //float lPhiZ   = 0;
  //if(iPhi)  lTree->SetBranchAddress( "nopuu"     ,&lNPU);
  if(iPhi)  lTree->SetBranchAddress( "pfu"       ,&lU);
  if(iPhi)  lTree->SetBranchAddress( "pfuphi"    ,&lUPhi);
  if(!iPhi) lTree->SetBranchAddress( "u_mva"     ,&lU);
  if(!iPhi) lTree->SetBranchAddress( "uphi_mva"  ,&lUPhi);

  lTree->SetBranchAddress( "pt_z"       ,&lPtVis);
  lTree->SetBranchAddress( "phi_z"      ,&lPhiVis);
  //lTree->SetBranchAddress( "phi_z"      ,&lPhiZ);

  float lMetMVA    = 0;
  float lMetPhiMVA = 0;
  float lUMVA      = 0;
  float lUPhiMVA   = 0;
  float lU1MVA     = 0;
  float lU2MVA     = 0;

  TFile *lOFile = 0;
  if(iPhi)  lOFile = new TFile( "Output.root","RECREATE");
  if(!iPhi) lOFile = new TFile( "Output.root","RECREATE");

  TTree *lOTree = lTree->CloneTree(0);
  if(iPhi)  lOTree->Branch("met_mva"     ,&lMetMVA    ,"lMetMVA/F");
  if(!iPhi) lOTree->Branch("metx_mva"    ,&lMetMVA    ,"lMetMVA/F");

  if(iPhi)  lOTree->Branch("metphix_mva" ,&lMetPhiMVA ,"lMetPhiMVA/F");
  if(!iPhi) lOTree->Branch("metphi_mva"  ,&lMetPhiMVA ,"lMetPhiMVA/F");

  if(iPhi)  lOTree->Branch("uphi_mva"    ,&lUPhiMVA   ,"lUPhiMVA/F");
  if(!iPhi) lOTree->Branch("uphix_mva"   ,&lUPhiMVA   ,"lUPhiMVA/F");

  if(iPhi)  lOTree->Branch("u_mva"       ,&lUMVA      ,"lUMVA/F");
  if(!iPhi) lOTree->Branch("ux_mva"      ,&lUMVA      ,"lUMVA/F");

  if(iPhi)  lOTree->Branch("u1_mva"      ,&lU1MVA     ,"lU1MVA/F");
  if(!iPhi) lOTree->Branch("u1x_mva"     ,&lU1MVA     ,"lU1MVA/F");

  if(iPhi)  lOTree->Branch("u2_mva"      ,&lU2MVA     ,"lU2MVA/F");
  if(!iPhi) lOTree->Branch("u2x_mva"     ,&lU2MVA     ,"lU2MVA/F");
  
  Float_t *lVals = new Float_t[lN];
  ROOT::Cintex::Cintex::Enable();   
  int lNEvents = lTree->GetEntries();
  for (Long64_t i0=0; i0<lNEvents;i0++) {
    if (i0 % 10000 == 0) std::cout << "--- ... Processing event: " << double(i0)/double(lNEvents) << std::endl;
    lTree->GetEntry(i0);
    for(int i1 = 0; i1 < lN; i1++) { 
      lVals[i1] = lFVars[i1]->EvalInstance();
    }
    double lVal = lForest->GetResponse(lVals);
    lUPhiMVA = lUPhi; if(iPhi)  lUPhiMVA  += lVal;
    lUMVA    = lU;    if(!iPhi) lUMVA     *= lVal;
    
      
    TLorentzVector lVec;    lVec    .SetPtEtaPhiM(lUMVA,0,lUPhiMVA,0);
    if(!iPhi && lVal < 0) lVec.RotateZ(TMath::Pi());

    TLorentzVector lVis;    lVis    .SetPtEtaPhiM(lPtVis,0,lPhiVis,0);
    lVec      -= lVis;
    lMetMVA    = lVec.Pt();
    lMetPhiMVA = lVec.Phi();
    
    lVec += lVis;
    lVec.RotateZ(-lPhiVis);
    lU1MVA = -lVec.Px();
    lU2MVA = -lVec.Py();
    
    lOTree->Fill();
  }
  lOTree->Write();
  lOFile->Close();
}
