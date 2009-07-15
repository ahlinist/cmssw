//Convert DQM algorithm rate histograms into trees for use in the Fitter 
//
//partially ripped from CDF: /Consumer/XMon/SL1DMonitor.cc

#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

using namespace std;

void DQMToXMonFitter() {

  //grab DQM histogram file
  //TFile * fin = new TFile("~/public/Playback_V0001_L1T_R000081156.root");
  TFile * fin = new TFile("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/Online/096/994/DQM_V0013_R000096994_R000096994.root");
  if (fin->IsZombie()) {
    printf("DQM root file does not exist.");
    return;
  }

  //read trigger rate histograms
  TH1F *hRate[128];
  char hpath[200];
  //char hname[40];

  // number of lines actually used
  int m_numL1Used;
  // instantaneous luminosity as calcuated for each level
  Float_t m_currL1InstLum;

  // current cross sections
  Float_t m_l1_xsecs[128];
  char m_reallyLongString[300];
  int m_currentLineIndex;

  //tree of trigger names and indices
  TTree *L1nametree = new TTree("l1names", "L1 Trigger Names");
  L1nametree->Branch("index", &m_currentLineIndex, "index/I");
  L1nametree->Branch("name", m_reallyLongString, "name/C");

  //tree of lum, num, and xsec
  TTree *L1tree = new TTree("l1xsec", "L1 Cross Sections");
  L1tree->Branch("num", &m_numL1Used, "num/I");
  L1tree->Branch("lum", &m_currL1InstLum, "lum/F");
  L1tree->Branch("xsec", m_l1_xsecs, "xsec[num]/F");

  for(int i = 0; i<128; i++){
    //TH1F *h1 = new TH1F(*((TH1F*)fin->Get("DQMData/Run 81156/L1T/Run Summary/L1TScalersSCAL/L1TriggerScalers/AlgorithmRates/Rate_AlgoBit_000")));
    //TH1F *h1 = (TH1F*)fin->Get("DQMData/Run 81156/L1T/Run summary/L1TScalersSCAL/L1TriggerScalers/AlgorithmRates/Rate_AlgoBit_000");
    //sprintf(hpath,"DQMData/Run 81156/L1T/Run summary/L1TScalersSCAL/L1TriggerScalers/AlgorithmRates/Rate_AlgoBit_%03d",i);
    sprintf(hpath,"DQMData/Run 96994/L1T/Run summary/L1TScalersSCAL/L1TriggerScalers/AlgorithmRates/Rate_AlgoBit_%03d",i);
    hRate[i] = (TH1F*)fin->Get(hpath);
    if(!hRate[i]) cout << "error reading histo for bit " << i << endl;
  }
  // cout << "h1 = " << h1 << endl;
  //if(!h1) {cout << "ERROR! can't read histogram" << endl; return;}

  //calculate cross-sections

  //get number of lumisections
  //cout << "number of lumisections = " << hRate[0]->GetXaxis()->GetNbins()  << endl;  
  //cout << "bin 1 center = " << hRate[0]->GetXaxis()->GetBinCenter(4)  << endl;
  Int_t nBins = hRate[0]->GetXaxis()->GetNbins();
  Int_t nLumisections = 0;
  Float_t lumi = 0;
  for(int i =0; i < nBins; i++){
    if(hRate[1]->GetBinContent(i)){
      nLumisections++;
    }
  }
  cout << "number of lumisections = " << nLumisections << endl; 

  m_numL1Used = 128;

  for(int i = 0; i < nBins; i++){
    if(hRate[1]->GetBinContent(i)){
      lumi = hRate[1]->GetXaxis()->GetBinCenter(i);
      cout << "lumi = " << lumi << endl;
      if(lumi){
	for (int iTrig = 0; iTrig < 128; ++iTrig){
	  m_l1_xsecs[iTrig] = hRate[iTrig]->GetBinContent(i)/lumi;
	}
	m_currL1InstLum = lumi;
	L1tree->Fill();
      }
    }
  }

  //fill trigger names
  for (int index = 0; index < 128; ++index){
    m_currentLineIndex = index;
    //cout << "about to check " << index << endl;
    //cout << "name " << hRate[index]->GetTitle() << endl;
 
    //sprintf (m_reallyLongString, "L1_%s", hRate[index]->GetTitle());
    sprintf (m_reallyLongString, "L1_%d", index);

    L1nametree->Fill();
    //cout << "filling "  << index << " with " << m_reallyLongString << endl;
  } // for index

  //fill lumi and xsec info
 

  //make output file for fitter
  //TFile *fout = new TFile ("TXMon_txsec081156.root", "RECREATE");
  TFile *fout = new TFile ("TXMon_txsec096994.root", "RECREATE");

  //h1->Write();

  L1nametree->Write();

  L1tree->Write();

  fin->Close();
  fout->Close();

}
