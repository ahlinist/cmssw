#include <iostream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TKey.h>
#include <TROOT.h>
#include <TMath.h>
#include <TTree.h>
#include "JetMETAnalysis/JetUtilities/interface/CommandLine.h"
#include "JetMETCorrections/DijetBalance/interface/JetUtil.h"
#include "JetMETCorrections/DijetBalance/interface/Utilities.h"
using namespace std;

typedef struct 
{
  int prescale;
  int fired;
} TrigStruct;
int main(int argc, char**argv)
{
  CommandLine c1;
  c1.parse(argc,argv);
  //----------------------------------------------------------------------------
  string         TreeFilename   = c1.getValue<string> ("TreeFilename");
  string         HistoFilename  = c1.getValue<string> ("HistoFilename");
  bool           IsMCarlo       = c1.getValue<bool>   ("IsMCarlo",false);
  bool           ApplyWeights   = c1.getValue<bool>   ("ApplyWeights",false);  
  double         DPHI_CUT       = c1.getValue<double> ("DPHI_CUT");
  double         RATIO_CUT      = c1.getValue<double> ("RATIO_CUT");
  double         PTJET3_CUT     = c1.getValue<double> ("PTJET3_CUT");  
  vector<string> HLTbit         = c1.getVector<string>("HLTbit","");
  vector<string> JetAlgos       = c1.getVector<string>("JetAlgos","");
  vector<double> PtHatBnd       = c1.getVector<double>("PtHatBoundaries","0,1");
  vector<double> PtHatXsec      = c1.getVector<double>("PtHatXsec","0,0");
  vector<double> PtHatNevt      = c1.getVector<double>("PtHatEvents","0,0");
  //----------------------------------------------------------------------------
  if (!c1.check()) return 0;
  c1.print();
  unsigned int NHLT = HLTbit.size();
  const unsigned int MAX_NHLT = 10;
  if (NHLT>MAX_NHLT)
    {
      cout<<"WARNING: too many HLT bits!!!! Setting default value 10"<<endl;
      NHLT = 10;
    }
  //----------------------------------------------------------------------------
  TFile *inf;      
  inf = TFile::Open(TreeFilename.c_str(),"R");
  TFile *outf = new TFile(HistoFilename.c_str(),"RECREATE");   
  char name[1024];
  int i;
  int passLooseIdBarrel,passLooseIdProbe,passLooseIdJet3;
  bool cut_dphi,cut_ratio,cut_jet3,cut_id;
  float dphi,ratio,ptJet3,dijetPt,pthat,wt;
  TrigStruct HLTTrigResults[5];
  ///////////////////////////////////////////////////////////////  
  TIter next(inf->GetListOfKeys());
  TKey* key(0);
while ((key=(TKey*)next())) {
  if (strcmp(key->GetClassName(),"TDirectoryFile")!=0) continue;
    
  TDirectoryFile* idir = (TDirectoryFile*)key->ReadObj();
  string alg(idir->GetName());
  if (JetAlgos.size()>0&&!contains(JetAlgos,alg)) continue;
  cout<<alg<<" ... "<<endl;

  TTree *tr = (TTree*)idir->Get("DiJetTree");
  TBranch *brDphi = (TBranch*)tr->GetBranch("dphi");
  brDphi->SetAddress(&dphi);
  TBranch *brPtDijet = (TBranch*)tr->GetBranch("dijetPt");
  brPtDijet->SetAddress(&dijetPt);
  TBranch *brPtJet3 = (TBranch*)tr->GetBranch("ptJet3");
  brPtJet3->SetAddress(&ptJet3);
  TBranch *brIdBarrel = (TBranch*)tr->GetBranch("passLooseIdBarrel");
  TBranch *brIdProbe = (TBranch*)tr->GetBranch("passLooseIdProbe");
  TBranch *brIdJet3 = (TBranch*)tr->GetBranch("passLooseIdJet3");
  brIdBarrel->SetAddress(&passLooseIdBarrel);
  brIdProbe->SetAddress(&passLooseIdProbe);
  brIdJet3->SetAddress(&passLooseIdJet3); 
  TBranch *b_hltResults[5]; 
  for(unsigned int k=0;k<NHLT;k++) 
    {
      b_hltResults[k] = (TBranch*)tr->GetBranch(HLTbit[k].c_str());
      b_hltResults[k]->SetAddress(&HLTTrigResults[k]);
    }
  if (IsMCarlo)
    {
      TBranch *brPtHat = (TBranch*)tr->GetBranch("pthat");
      brPtHat->SetAddress(&pthat);
    }
  ///////////////////////////////////////////////////////////////
  cout<<"Booking histograms"<<endl;
  TDirectoryFile* odir = (TDirectoryFile*)outf->mkdir(alg.c_str());
  odir->cd();
  TH1F *hDijetPtEff[MAX_NHLT];
  TH1F *hDijetPtEffBase[MAX_NHLT];
  for(unsigned int k=0;k<NHLT;k++)
    {
      sprintf(name,"DijetPtEff_HLT%d",k);
      hDijetPtEff[k] = new TH1F(name,name,500,0,500);

      sprintf(name,"DijetPtEffBase_HLT%d",k);
      hDijetPtEffBase[k] = new TH1F(name,name,500,0,500);
    } 
  ///////////////////////////////////////////////////////////////
  cout<<"Found "<<tr->GetEntries()<<" entries in: "<<TreeFilename<<endl;
  cout<<"Filling histograms"<<endl;
  for(i=0;i<tr->GetEntries();i++)
    {
      if (i % 500000 == 0)
        cout<<i<<endl;
      tr->GetEntry(i);
      wt = 1.;
      if (IsMCarlo)
        {
          int ipthat = getBin(pthat,PtHatBnd);
          double xsec = PtHatXsec[ipthat];
          double nevt = PtHatNevt[ipthat];
          if (ApplyWeights) 
            wt = xsec/nevt; 
        }
      ratio     = ptJet3/dijetPt;
      cut_dphi  = (fabs(dphi)>DPHI_CUT);
      cut_ratio = (ratio<RATIO_CUT);
      // need to investigate the PFJet ID
      //cut_id    = (passLooseIdBarrel == 1 && passLooseIdProbe == 1);
      cut_id = true;
 
      //if (passLooseIdJet3 == 0)//jet3 coming probabaly from noise. Accept the event.
        //cut_jet3 = true;
      //else 
      cut_jet3  = (cut_ratio || (ptJet3<PTJET3_CUT));
      if (!cut_id) continue;
      for(unsigned int k=0;k<NHLT-1;k++)
        {
          if (cut_dphi && cut_jet3)
            {
              if (HLTTrigResults[k].fired == 1)
                hDijetPtEffBase[k+1]->Fill(dijetPt,wt);  
              if (HLTTrigResults[k].fired == 1 && HLTTrigResults[k+1].fired == 1)
                hDijetPtEff[k+1]->Fill(dijetPt,wt);
            }
        }
    }
  }// algo loop
  outf->Write();
  gROOT->GetListOfFiles()->Remove(outf);
  outf->Close();
  delete outf;
  inf->Close();
  delete inf;
  return 0;
}
