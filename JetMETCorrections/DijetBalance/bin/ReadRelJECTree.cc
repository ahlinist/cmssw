#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include "JetMETAnalysis/JetUtilities/interface/CommandLine.h"
#include "JetMETCorrections/DijetBalance/interface/Utilities.h"
#include <TROOT.h>
#include <TChain.h>

using namespace std;
int main(int argc, char**argv)
{
  ///////////////////  Read parameters  /////////////////////
  CommandLine c1;
  c1.parse(argc,argv);
  string InputFileName      = c1.getValue<string>  ("InputFileName");
  string OutputFileName     = c1.getValue<string>  ("OutputFileName");
  string ALGO               = c1.getValue<string>  ("ALGO");
  bool IsMCarlo             = c1.getValue<bool>    ("IsMCarlo",false); 
  double DR_MAX             = c1.getValue<double>  ("DR_MAX",0.25);
  vector<double> PtBnd      = c1.getVector<double> ("PtBoundaries");
  vector<double> PtHatBnd;
  vector<double> Xsec;
  vector<double> NEvents;
  if (IsMCarlo)
    {  
      PtHatBnd  = c1.getVector<double> ("PtHatBoundaries","0,15,20,30,50,80,120,170,230,300,380,470,600,800,1000,1400,1800,2200,2600,3000,3500");
      Xsec      = c1.getVector<double> ("Xsec","4.844e+10,5.794e+8,2.361e+8,5.311e+7,6.358e+6,7.849e+5,1.151e+5,2.014e+4,4.094e+3,9.346e+2,2.338e+2,7.021e+1,1.557e+1,1.843e+0,3.318e-1,1.086e-2,3.499e-4,7.549e-6,6.465e-8,6.295e-11");
      NEvents   = c1.getVector<double> ("NEvents","2197029,2256430,1034680,1161768,111289,606771,58888,51680,52894,64265,52207,20380,22448,26000,23956,20575,33970,22580,20644,23460"); 
    }
  /////////////////////////////////////////////////////////////  
  const int NPT_MAX = 20;
  unsigned int NPT = PtBnd.size();
  TFile *inf  = new TFile(InputFileName.c_str());
  TFile *outf = new TFile(OutputFileName.c_str(),"RECREATE");
  if (!c1.check()) return 0;
  c1.print(); // Printing the options
  //-------------- Leaf variables -----------------------
  int evtNo,runNo,lumi,bunch;
  float pthat;
  vector<float> *calodR(0),*caloE(0),*caloPt(0),*caloEta(0),*caloPhi(0),*caloRsp(0);
  vector<float> *jptdR(0),*jptE(0),*jptPt(0),*jptEta(0),*jptPhi(0),*jptRsp(0);
  vector<float> *energy(0),*pt(0),*eta(0),*phi(0);
  //-------------- Define the tree branches ------------- 
  TString alg(ALGO);
  TTree *tr = (TTree*)inf->Get(alg+"reljec/RelJECTree");
  TBranch *b_e         = 0;
  TBranch *b_pt         = 0;
  TBranch *b_eta        = 0;
  TBranch *b_phi        = 0;
  TBranch *b_EvtNo      = 0;
  TBranch *b_RunNo      = 0;
  TBranch *b_Lumi       = 0;
  TBranch *b_Bunch      = 0;
  TBranch *b_pthat      = 0;
  TBranch *b_calodR     = 0;
  TBranch *b_caloE      = 0;
  TBranch *b_caloPt     = 0;
  TBranch *b_caloEta    = 0;
  TBranch *b_caloPhi    = 0;
  TBranch *b_caloRsp    = 0;
  TBranch *b_jptdR      = 0;
  TBranch *b_jptE       = 0;
  TBranch *b_jptPt      = 0;
  TBranch *b_jptEta     = 0;
  TBranch *b_jptPhi     = 0;
  TBranch *b_jptRsp     = 0;
  cout<<"Branches defined"<<endl;
  //-------------- Set Branch Addresses -----------------
  tr->SetBranchAddress("energy",             &energy,             &b_e);
  tr->SetBranchAddress("pt",                 &pt,                 &b_pt);
  tr->SetBranchAddress("eta",                &eta,                &b_eta);
  tr->SetBranchAddress("phi",                &phi,                &b_phi);
  tr->SetBranchAddress("evtNo",              &evtNo,              &b_EvtNo);
  tr->SetBranchAddress("runNo",              &runNo,              &b_RunNo);
  tr->SetBranchAddress("lumi",               &lumi,               &b_Lumi);  
  tr->SetBranchAddress("bunch",              &bunch,              &b_Bunch);    
  tr->SetBranchAddress("pthat",              &pthat,              &b_pthat);
  tr->SetBranchAddress("caloMatchR",         &calodR,             &b_calodR);
  tr->SetBranchAddress("caloMatchE",         &caloE,              &b_caloE);
  tr->SetBranchAddress("caloMatchPt",        &caloPt,             &b_caloPt);
  tr->SetBranchAddress("caloMatchEta",       &caloEta,            &b_caloEta);
  tr->SetBranchAddress("caloMatchPhi",       &caloPhi,            &b_caloPhi);
  tr->SetBranchAddress("caloResponse",       &caloRsp,            &b_caloRsp);
  tr->SetBranchAddress("jptMatchR",          &jptdR,              &b_jptdR);
  tr->SetBranchAddress("jptMatchE",          &jptE,               &b_jptE);
  tr->SetBranchAddress("jptMatchPt",         &jptPt,              &b_jptPt);
  tr->SetBranchAddress("jptMatchEta",        &jptEta,             &b_jptEta);
  tr->SetBranchAddress("jptMatchPhi",        &jptPhi,             &b_jptPhi);
  tr->SetBranchAddress("jptResponse",        &jptRsp,             &b_jptRsp);
   
  cout<<"Branch addresses set"<<endl;
  //-------------- Read the tree ------------------------
  unsigned NEntries = tr->GetEntries();
  cout<<"Booking histograms"<<endl;
  TH1F *hPtCalo[NPT_MAX];
  TH1F *hPtPF[NPT_MAX];
  TH1F *hPtJPT[NPT_MAX];
  TH1F *hCaloRsp[NPT_MAX];
  TH1F *hCaloJPTRsp[NPT_MAX];
  TH1F *hJPTRsp[NPT_MAX];
  TH2F *hPFvsCalo  = new TH2F("PFvsCalo","PFvsCalo",500,0,1000,500,0,1000);
  TH2F *hPFvsJPT   = new TH2F("PFvsJPT","PFvsJPT",500,0,1000,500,0,1000);
  TH2F *hJPTvsCalo = new TH2F("JPTvsCalo","JPTvs Calo",500,0,1000,500,0,1000);
  
  char name[1024];
  for(unsigned int i=0;i<NPT;i++)
    {
      sprintf(name,"ptCalo_Pt%d",i);
      hPtCalo[i] = new TH1F(name,name,500,0,1000);

      sprintf(name,"ptJPT_Pt%d",i);
      hPtJPT[i] = new TH1F(name,name,500,0,1000);

      sprintf(name,"ptPF_Pt%d",i);
      hPtPF[i] = new TH1F(name,name,500,0,1000);
   
      sprintf(name,"caloRsp_Pt%d",i);
      hCaloRsp[i] = new TH1F(name,name,200,0,2);

      sprintf(name,"caloJPTRsp_Pt%d",i);
      hCaloJPTRsp[i] = new TH1F(name,name,200,0,2); 
  
      sprintf(name,"jptRsp_Pt%d",i);
      hJPTRsp[i] = new TH1F(name,name,200,0,2);
    }
  
  cout<<"Reading TREE: "<<NEntries<<" events"<<endl;
  int decade = 0;
  int ipt;
  for(unsigned i=0;i<NEntries;i++)
    {
      double progress = 10.0*i/(1.0*NEntries);
      int k = TMath::FloorNint(progress); 
      if (k > decade) 
        cout<<10*k<<" %"<<endl;
      decade = k;    
      tr->GetEntry(i);
      double wt(1.0);
      if (IsMCarlo)
        {
          int bin = getBin(pthat,PtHatBnd);
          wt = Xsec[bin]/(NEvents[bin]);
        }
      unsigned Njets = pt->size();
      if (
           (runNo == 138737 && evtNo == 44851731) ||
           (runNo == 138919 && evtNo == 32253996) ||
           (runNo == 139096 && evtNo == 247776848) ||
           (runNo == 138921 && evtNo == 16936769) ||
           (runNo == 139370 && evtNo == 132046118) ||
           (runNo == 139370 && evtNo == 511769839) ||
           (runNo == 139370 && evtNo == 403257286) ||
           (runNo == 139368 && evtNo ==  25590516) ||
           (runNo == 139347 && evtNo ==  252029032) ||
           (runNo == 139365 && evtNo ==  65727014)
         )
        {
          TLorentzVector P4[2],caloP4[2],jptP4[2];
          P4[0].SetPtEtaPhiE((*pt)[0],(*eta)[0],(*phi)[0],(*energy)[0]);
          P4[1].SetPtEtaPhiE((*pt)[1],(*eta)[1],(*phi)[1],(*energy)[1]);
          caloP4[0].SetPtEtaPhiE((*caloPt)[0],(*caloEta)[0],(*caloPhi)[0],(*caloE)[0]);
          caloP4[1].SetPtEtaPhiE((*caloPt)[1],(*caloEta)[1],(*caloPhi)[1],(*caloE)[1]);          
          jptP4[0].SetPtEtaPhiE((*jptPt)[0],(*jptEta)[0],(*jptPhi)[0],(*jptE)[0]);
          jptP4[1].SetPtEtaPhiE((*jptPt)[1],(*jptEta)[1],(*jptPhi)[1],(*jptE)[1]);
          double m     = (P4[0]+P4[1]).M();
          double mCalo = (caloP4[0]+caloP4[1]).M();
          double mJPT  = (jptP4[0]+jptP4[1]).M();
          cout<<runNo<<" "<<evtNo<<" "<<endl;
          cout<<"Calo: "<<(*caloPt)[0]<<" "<<(*caloPt)[1]<<" "<<mCalo<<endl;
          cout<<"PF:   "<<(*pt)[0]<<" "<<(*pt)[1]<<" "<<m<<endl;
          cout<<"JPT:  "<<(*jptPt)[0]<<" "<<(*jptPt)[1]<<" "<<mJPT<<endl;
        }
      for(unsigned j=0;j<Njets;j++) 
        {
          if (j<2)
            {
              ipt = getBin((*pt)[j],PtBnd);
              if (ipt > -1 && (*calodR)[j] < DR_MAX && (*jptdR)[j] < DR_MAX)
                {
                  hPtCalo[ipt]    ->Fill((*caloPt)[j],wt);
                  hPtJPT[ipt]     ->Fill((*jptPt)[j],wt);
                  hPtPF[ipt]      ->Fill((*pt)[j],wt);
                  hCaloRsp[ipt]   ->Fill((*caloRsp)[j],wt);
                  hCaloJPTRsp[ipt]->Fill((*caloPt)[j]/(*jptPt)[j],wt);
                  hJPTRsp[ipt]    ->Fill((*jptRsp)[j],wt);
                  hPFvsCalo       ->Fill((*pt)[j],(*caloPt)[j],wt);
                  hJPTvsCalo      ->Fill((*jptPt)[j],(*caloPt)[j],wt);
                  hPFvsJPT        ->Fill((*pt)[j],(*jptPt)[j],wt);                  
                }
            }
	}// end of jets loop
    } // end of tree loop
  cout<<"End of TREE"<<endl;
  cout<<"Saving tree"<<endl;
  outf->cd();
  outf->Write();
  cout<<"Closing output file"<<endl;
  outf->Close();
}
