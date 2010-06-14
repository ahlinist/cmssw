/**
 * This script must be compiled with ACLiC!
 *
 * Example:
 * $ root -l -b
 * root [0] .L plotL1Efficiency.cxx+
 * root [1] plotL1Efficiency()
 */


// The class definition in TTEffTree.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("TTEffTree.C")
// Root > T->Process("TTEffTree.C","some options")
// Root > T->Process("TTEffTree.C+")
//

#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TChain.h>
#include <TTree.h>
#include <TFile.h>
#include <TSelector.h>
#include <TGraphAsymmErrors.h>
#include <TPad.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TNamed.h>
#include <TEntryList.h>
#include <TF1.h>
#include <TLatex.h>
#include <TPaveStats.h>
#include <TStopwatch.h>

#include<vector>
#include<string>
#include<iostream>

#include "tdrstyle.cxx"

class Data;

TLegend *createTLegend(float x1, float y1, float width, float height) {
  TLegend *leg = new TLegend(x1, y1, x1+width, y1+height);
  leg->SetFillColor(kWhite);
  //leg->SetLineWidth(1);
  //leg->SetLineColor(kBlack);
  //leg->SetFillStyle(4000); // transparent
  //leg->SetShadowColor(kWhite);
  leg->SetBorderSize(1);
  leg->SetMargin(0.1);
  //leg->SetTextSize(0.02);
  return leg;
}

class TTEffTree : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   UInt_t          event;
   UInt_t          run;
   UInt_t          lumi;
   Float_t         PFTauPt;
   Float_t         PFTauInvPt;
   Float_t         PFTauEt;
   Float_t         PFTauEta;
   Float_t         PFTauPhi;
   Float_t         PFTauProng;
   Float_t         PFTauIso;
   Float_t         PFTauIsoSum;
   Float_t         PFTauEnergy;
   Float_t         PFClusterEtaRMS;
   Float_t         PFClusterPhiRMS;
   Float_t         PFClusterDrRMS;
   Int_t           PFTauMatch;
   Int_t           PFMuonMatch;
   Int_t           PFElectronMatch;
   Float_t         PFJetChargedEmEnergy;
   Float_t         PFJetChargedEmEnergyFraction;
   Float_t         PFJetChargedHadronEnergy;
   Float_t         PFJetChargedHadronEnergyFraction;
   Float_t         PFJetNeutralEmEnergy;
   Float_t         PFJetNeutralEmEnergyFraction;
   Float_t         PFJetNeutralHadronEnergy;
   Float_t         PFJetNeutralHadronEnergyFraction;
   Int_t           MCMatch;
   Float_t         MCTauEt;
   Float_t         MCTauE;
   Float_t         MCTauEta;
   Float_t         MCTauPhi;
   Float_t         L1JetPt;
   Float_t         L1JetEt;
   Float_t         L1JetEta;
   Float_t         L1JetPhi;
   Float_t         L1JetMatchDR;
   Bool_t          L1TauVeto;
   Bool_t          hasMatchedL1Jet;
   Bool_t          hasMatchedL1TauJet;
   Bool_t          hasMatchedL1CenJet;
   Float_t         L1MET;
   Float_t         L1MHT;
   Int_t           L1IsolationRegions_1GeV;
   Int_t           L1IsolationRegions_2GeV;
   Int_t           L1IsolationRegions_3GeV;
   Int_t           L1IsolationRegions_4GeV;
   Bool_t          hasTriggeredAndMatchedL1TauJet;
   Bool_t          hasTriggeredAndMatchedL1CenJet;
   Float_t         L2ECALIsolationEt;
   Float_t         L2TowerIsolationEt;
   Float_t         L2ClusterEtaRMS;
   Float_t         L2ClusterPhiRMS;
   Float_t         L2ClusterDeltaRRMS;
   Int_t           L2NClustersInAnnulus;
   Float_t         L2SeedTowerEt;
   Float_t         L2JetEt;
   Float_t         L2JetEta;
   Float_t         L2JetPhi;
   Float_t         L2HadEnergyFraction;
   Int_t           L2NTowers60;
   Int_t           L2NTowers90;
   Int_t           hasMatchedL2Jet;
   Float_t         PFEGIsolEt;
   Float_t         PFECALIsolationEt;
   Float_t         PFHCALTowIsolationEt;
   Float_t         PFECALTowIsolationEt;
   Int_t           PFNEGammaCandsAnnulus;
   Int_t           PFNHadCandsAnnulus;
   Float_t         PFHighestClusterEt;
   Float_t         PFEGammaClusterEtaRMS;
   Float_t         PFEGammaClusterPhiRMS;
   Float_t         PFEGammaClusterDeltaRRMS;
   Float_t         l25Eta;
   Float_t         l25Phi;
   Float_t         l25Et;
   Float_t         l25Pt;
   Float_t         l25PtLdgLoose;
   Float_t         l25EtaLdgLoose;
   Float_t         l25PhiLdgLoose;
   Float_t         l25PtLdgJetDRLoose;
   Float_t         l25InvPt;
   Char_t          l25Depth;
   Char_t          leadDepth1;
   Char_t          leadDepth2;
   Float_t         minDR;
   Float_t         bareEt;
   Char_t          l25DefDisc13;
   Char_t          l25DefDisc23;
   Char_t          l25DefDisc21;
   Char_t          l25LooseDisc13;
   Char_t          l25LooseDisc23;
   Char_t          l25LooseDisc21;
   Bool_t          L1_DoubleEG05_TopBottom;
   Bool_t          L1_DoubleEG1;
   Bool_t          L1_DoubleEG5;
   Bool_t          L1_DoubleHfBitCountsRing1_P1N1;
   Bool_t          L1_DoubleHfBitCountsRing2_P1N1;
   Bool_t          L1_DoubleHfRingEtSumsRing1_P200N200;
   Bool_t          L1_DoubleHfRingEtSumsRing1_P4N4;
   Bool_t          L1_DoubleHfRingEtSumsRing2_P200N200;
   Bool_t          L1_DoubleHfRingEtSumsRing2_P4N4;
   Bool_t          L1_DoubleJet30;
   Bool_t          L1_DoubleMu3;
   Bool_t          L1_DoubleMuOpen;
   Bool_t          L1_DoubleMuTopBottom;
   Bool_t          L1_DoubleTauJet14;
   Bool_t          L1_ETM20;
   Bool_t          L1_ETM30;
   Bool_t          L1_ETM80;
   Bool_t          L1_ETT60;
   Bool_t          L1_HTT100;
   Bool_t          L1_HTT200;
   Bool_t          L1_IsoEG10_Jet6_ForJet6;
   Bool_t          L1_MinBias_HTT10;
   Bool_t          L1_Mu3QE8_Jet6;
   Bool_t          L1_Mu5QE8_Jet6;
   Bool_t          L1_QuadJet6;
   Bool_t          L1_SingleEG1;
   Bool_t          L1_SingleEG10;
   Bool_t          L1_SingleEG12;
   Bool_t          L1_SingleEG15;
   Bool_t          L1_SingleEG2;
   Bool_t          L1_SingleEG20;
   Bool_t          L1_SingleEG5;
   Bool_t          L1_SingleEG8;
   Bool_t          L1_SingleHfBitCountsRing1_1;
   Bool_t          L1_SingleHfBitCountsRing2_1;
   Bool_t          L1_SingleHfRingEtSumsRing1_200;
   Bool_t          L1_SingleHfRingEtSumsRing1_4;
   Bool_t          L1_SingleHfRingEtSumsRing2_200;
   Bool_t          L1_SingleHfRingEtSumsRing2_4;
   Bool_t          L1_SingleIsoEG10;
   Bool_t          L1_SingleIsoEG12;
   Bool_t          L1_SingleIsoEG15;
   Bool_t          L1_SingleIsoEG5;
   Bool_t          L1_SingleIsoEG8;
   Bool_t          L1_SingleJet20;
   Bool_t          L1_SingleJet30;
   Bool_t          L1_SingleJet40;
   Bool_t          L1_SingleJet50;
   Bool_t          L1_SingleJet6;
   Bool_t          L1_SingleJet60;
   Bool_t          L1_SingleMu0;
   Bool_t          L1_SingleMu10;
   Bool_t          L1_SingleMu14;
   Bool_t          L1_SingleMu20;
   Bool_t          L1_SingleMu3;
   Bool_t          L1_SingleMu5;
   Bool_t          L1_SingleMu7;
   Bool_t          L1_SingleMuBeamHalo;
   Bool_t          L1_SingleMuOpen;
   Bool_t          L1_SingleTauJet10;
   Bool_t          L1_SingleTauJet20;
   Bool_t          L1_SingleTauJet30;
   Bool_t          L1_SingleTauJet50;
   Bool_t          L1_TripleJet14;
   Bool_t          L1_ZeroBias;
   Bool_t          tauFilter;

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_PFTauPt;   //!
   TBranch        *b_PFTauInvPt;   //!
   TBranch        *b_PFTauEt;   //!
   TBranch        *b_PFTauEta;   //!
   TBranch        *b_PFTauPhi;   //!
   TBranch        *b_PFTauProng;   //!
   TBranch        *b_PFTauIso;   //!
   TBranch        *b_PFTauIsoSum;   //!
   TBranch        *b_PFTauEnergy;   //!
   TBranch        *b_PFClusterEtaRMS;   //!
   TBranch        *b_PFClusterPhiRMS;   //!
   TBranch        *b_PFClusterDrRMS;   //!
   TBranch        *b_PFTauMatch;   //!
   TBranch        *b_PFMuonMatch;   //!
   TBranch        *b_PFElectronMatch;   //!
   TBranch        *b_PFJetChargedEmEnergy;   //!
   TBranch        *b_PFJetChargedEmEnergyFraction;   //!
   TBranch        *b_PFJetChargedHadronEnergy;   //!
   TBranch        *b_PFJetChargedHadronEnergyFraction;   //!
   TBranch        *b_PFJetNeutralEmEnergy;   //!
   TBranch        *b_PFJetNeutralEmEnergyFraction;   //!
   TBranch        *b_PFJetNeutralHadronEnergy;   //!
   TBranch        *b_PFJetNeutralHadronEnergyFraction;   //!
   TBranch        *b_MCMatch;   //!
   TBranch        *b_MCTauEt;   //!
   TBranch        *b_MCTauE;   //!
   TBranch        *b_MCTauEta;   //!
   TBranch        *b_MCTauPhi;   //!
   TBranch        *b_L1JetPt;   //!
   TBranch        *b_L1JetEt;   //!
   TBranch        *b_L1JetEta;   //!
   TBranch        *b_L1JetPhi;   //!
   TBranch        *b_L1JetMatchDR;   //!
   TBranch        *b_L1TauVeto;   //!
   TBranch        *b_hasMatchedL1Jet;   //!
   TBranch        *b_hasMatchedL1TauJet;   //!
   TBranch        *b_hasMatchedL1CenJet;   //!
   TBranch        *b_L1MET;   //!
   TBranch        *b_L1MHT;   //!
   TBranch        *b_L1IsolationRegions_1GeV;   //!
   TBranch        *b_L1IsolationRegions_2GeV;   //!
   TBranch        *b_L1IsolationRegions_3GeV;   //!
   TBranch        *b_L1IsolationRegions_4GeV;   //!
   TBranch        *b_hasTriggeredAndMatchedL1TauJet;   //!
   TBranch        *b_hasTriggeredAndMatchedL1CenJet;   //!
   TBranch        *b_L2ECALIsolationEt;   //!
   TBranch        *b_L2TowerIsolationEt;   //!
   TBranch        *b_L2ClusterEtaRMS;   //!
   TBranch        *b_L2ClusterPhiRMS;   //!
   TBranch        *b_L2ClusterDeltaRRMS;   //!
   TBranch        *b_L2NClustersInAnnulus;   //!
   TBranch        *b_L2SeedTowerEt;   //!
   TBranch        *b_L2JetEt;   //!
   TBranch        *b_L2JetEta;   //!
   TBranch        *b_L2JetPhi;   //!
   TBranch        *b_L2HadEnergyFraction;   //!
   TBranch        *b_L2NTowers60;   //!
   TBranch        *b_L2NTowers90;   //!
   TBranch        *b_hasMatchedL2Jet;   //!
   TBranch        *b_PFEGIsolEt;   //!
   TBranch        *b_PFECALIsolationEt;   //!
   TBranch        *b_PFHCALTowIsolationEt;   //!
   TBranch        *b_PFECALTowIsolationEt;   //!
   TBranch        *b_PFNEGammaCandsAnnulus;   //!
   TBranch        *b_PFNHadCandsAnnulus;   //!
   TBranch        *b_PFHighestClusterEt;   //!
   TBranch        *b_PFEGammaClusterEtaRMS;   //!
   TBranch        *b_PFEGammaClusterPhiRMS;   //!
   TBranch        *b_PFEGammaClusterDeltaRRMS;   //!
   TBranch        *b_l25Eta;   //!
   TBranch        *b_l25Phi;   //!
   TBranch        *b_l25Et;   //!
   TBranch        *b_l25Pt;   //!
   TBranch        *b_l25PtLdgLoose;   //!
   TBranch        *b_l25EtaLdgLoose;   //!
   TBranch        *b_l25PhiLdgLoose;   //!
   TBranch        *b_l25PtLdgJetDRLoose;   //!
   TBranch        *b_l25InvPt;   //!
   TBranch        *b_l25Depth;   //!
   TBranch        *b_leadDepth1;   //!
   TBranch        *b_leadDepth2;   //!
   TBranch        *b_minDR;   //!
   TBranch        *b_bareEt;   //!
   TBranch        *b_l25DefDisc13;   //!
   TBranch        *b_l25DefDisc23;   //!
   TBranch        *b_l25DefDisc21;   //!
   TBranch        *b_l25LooseDisc13;   //!
   TBranch        *b_l25LooseDisc23;   //!
   TBranch        *b_l25LooseDisc21;   //!
   TBranch        *b_L1_DoubleEG05_TopBottom;   //!
   TBranch        *b_L1_DoubleEG1;   //!
   TBranch        *b_L1_DoubleEG5;   //!
   TBranch        *b_L1_DoubleHfBitCountsRing1_P1N1;   //!
   TBranch        *b_L1_DoubleHfBitCountsRing2_P1N1;   //!
   TBranch        *b_L1_DoubleHfRingEtSumsRing1_P200N200;   //!
   TBranch        *b_L1_DoubleHfRingEtSumsRing1_P4N4;   //!
   TBranch        *b_L1_DoubleHfRingEtSumsRing2_P200N200;   //!
   TBranch        *b_L1_DoubleHfRingEtSumsRing2_P4N4;   //!
   TBranch        *b_L1_DoubleJet30;   //!
   TBranch        *b_L1_DoubleMu3;   //!
   TBranch        *b_L1_DoubleMuOpen;   //!
   TBranch        *b_L1_DoubleMuTopBottom;   //!
   TBranch        *b_L1_DoubleTauJet14;   //!
   TBranch        *b_L1_ETM20;   //!
   TBranch        *b_L1_ETM30;   //!
   TBranch        *b_L1_ETM80;   //!
   TBranch        *b_L1_ETT60;   //!
   TBranch        *b_L1_HTT100;   //!
   TBranch        *b_L1_HTT200;   //!
   TBranch        *b_L1_IsoEG10_Jet6_ForJet6;   //!
   TBranch        *b_L1_MinBias_HTT10;   //!
   TBranch        *b_L1_Mu3QE8_Jet6;   //!
   TBranch        *b_L1_Mu5QE8_Jet6;   //!
   TBranch        *b_L1_QuadJet6;   //!
   TBranch        *b_L1_SingleEG1;   //!
   TBranch        *b_L1_SingleEG10;   //!
   TBranch        *b_L1_SingleEG12;   //!
   TBranch        *b_L1_SingleEG15;   //!
   TBranch        *b_L1_SingleEG2;   //!
   TBranch        *b_L1_SingleEG20;   //!
   TBranch        *b_L1_SingleEG5;   //!
   TBranch        *b_L1_SingleEG8;   //!
   TBranch        *b_L1_SingleHfBitCountsRing1_1;   //!
   TBranch        *b_L1_SingleHfBitCountsRing2_1;   //!
   TBranch        *b_L1_SingleHfRingEtSumsRing1_200;   //!
   TBranch        *b_L1_SingleHfRingEtSumsRing1_4;   //!
   TBranch        *b_L1_SingleHfRingEtSumsRing2_200;   //!
   TBranch        *b_L1_SingleHfRingEtSumsRing2_4;   //!
   TBranch        *b_L1_SingleIsoEG10;   //!
   TBranch        *b_L1_SingleIsoEG12;   //!
   TBranch        *b_L1_SingleIsoEG15;   //!
   TBranch        *b_L1_SingleIsoEG5;   //!
   TBranch        *b_L1_SingleIsoEG8;   //!
   TBranch        *b_L1_SingleJet20;   //!
   TBranch        *b_L1_SingleJet30;   //!
   TBranch        *b_L1_SingleJet40;   //!
   TBranch        *b_L1_SingleJet50;   //!
   TBranch        *b_L1_SingleJet6;   //!
   TBranch        *b_L1_SingleJet60;   //!
   TBranch        *b_L1_SingleMu0;   //!
   TBranch        *b_L1_SingleMu10;   //!
   TBranch        *b_L1_SingleMu14;   //!
   TBranch        *b_L1_SingleMu20;   //!
   TBranch        *b_L1_SingleMu3;   //!
   TBranch        *b_L1_SingleMu5;   //!
   TBranch        *b_L1_SingleMu7;   //!
   TBranch        *b_L1_SingleMuBeamHalo;   //!
   TBranch        *b_L1_SingleMuOpen;   //!
   TBranch        *b_L1_SingleTauJet10;   //!
   TBranch        *b_L1_SingleTauJet20;   //!
   TBranch        *b_L1_SingleTauJet30;   //!
   TBranch        *b_L1_SingleTauJet50;   //!
   TBranch        *b_L1_TripleJet14;   //!
   TBranch        *b_L1_ZeroBias;   //!
   TBranch        *b_tauFilter;   //!

  Data *fDataPt; //! pointer to data structure to be filled
  Data *fDataEta; //! pointer to data structure to be filled
  Data *fDataPhi; //! pointer to data structure to be filled

  bool mc;
  Float_t tauCut;
  Float_t cenCut;
  
  TTEffTree(TTree * /*tree*/ =0): mc(0), tauCut(14), cenCut(30) { }
   virtual ~TTEffTree() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

  void setMC(bool f=true) {
    mc = f;
  }

  void setTauCut(float c) {
    tauCut = c;
  }

  void setCenCut(float c) {
    cenCut = c;
  }

  void resetTauCenCut() {
    tauCut = 14;
    cenCut = 30;
  }

   ClassDef(TTEffTree,0);
};

void TTEffTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);


   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("PFTauPt", &PFTauPt, &b_PFTauPt);
   fChain->SetBranchAddress("PFTauInvPt", &PFTauInvPt, &b_PFTauInvPt);
   fChain->SetBranchAddress("PFTauEt", &PFTauEt, &b_PFTauEt);
   fChain->SetBranchAddress("PFTauEta", &PFTauEta, &b_PFTauEta);
   fChain->SetBranchAddress("PFTauPhi", &PFTauPhi, &b_PFTauPhi);
   fChain->SetBranchAddress("PFTauProng", &PFTauProng, &b_PFTauProng);
   fChain->SetBranchAddress("PFTauIso", &PFTauIso, &b_PFTauIso);
   fChain->SetBranchAddress("PFTauIsoSum", &PFTauIsoSum, &b_PFTauIsoSum);
   fChain->SetBranchAddress("PFTauEnergy", &PFTauEnergy, &b_PFTauEnergy);
   fChain->SetBranchAddress("PFClusterEtaRMS", &PFClusterEtaRMS, &b_PFClusterEtaRMS);
   fChain->SetBranchAddress("PFClusterPhiRMS", &PFClusterPhiRMS, &b_PFClusterPhiRMS);
   fChain->SetBranchAddress("PFClusterDrRMS", &PFClusterDrRMS, &b_PFClusterDrRMS);
   fChain->SetBranchAddress("PFTauMatch", &PFTauMatch, &b_PFTauMatch);
   fChain->SetBranchAddress("PFMuonMatch", &PFMuonMatch, &b_PFMuonMatch);
   fChain->SetBranchAddress("PFElectronMatch", &PFElectronMatch, &b_PFElectronMatch);
   fChain->SetBranchAddress("PFJetChargedEmEnergy", &PFJetChargedEmEnergy, &b_PFJetChargedEmEnergy);
   fChain->SetBranchAddress("PFJetChargedEmEnergyFraction", &PFJetChargedEmEnergyFraction, &b_PFJetChargedEmEnergyFraction);
   fChain->SetBranchAddress("PFJetChargedHadronEnergy", &PFJetChargedHadronEnergy, &b_PFJetChargedHadronEnergy);
   fChain->SetBranchAddress("PFJetChargedHadronEnergyFraction", &PFJetChargedHadronEnergyFraction, &b_PFJetChargedHadronEnergyFraction);
   fChain->SetBranchAddress("PFJetNeutralEmEnergy", &PFJetNeutralEmEnergy, &b_PFJetNeutralEmEnergy);
   fChain->SetBranchAddress("PFJetNeutralEmEnergyFraction", &PFJetNeutralEmEnergyFraction, &b_PFJetNeutralEmEnergyFraction);
   fChain->SetBranchAddress("PFJetNeutralHadronEnergy", &PFJetNeutralHadronEnergy, &b_PFJetNeutralHadronEnergy);
   fChain->SetBranchAddress("PFJetNeutralHadronEnergyFraction", &PFJetNeutralHadronEnergyFraction, &b_PFJetNeutralHadronEnergyFraction);
   fChain->SetBranchAddress("MCMatch", &MCMatch, &b_MCMatch);
   fChain->SetBranchAddress("MCTauEt", &MCTauEt, &b_MCTauEt);
   fChain->SetBranchAddress("MCTauE", &MCTauE, &b_MCTauE);
   fChain->SetBranchAddress("MCTauEta", &MCTauEta, &b_MCTauEta);
   fChain->SetBranchAddress("MCTauPhi", &MCTauPhi, &b_MCTauPhi);
   fChain->SetBranchAddress("L1JetPt", &L1JetPt, &b_L1JetPt);
   fChain->SetBranchAddress("L1JetEt", &L1JetEt, &b_L1JetEt);
   fChain->SetBranchAddress("L1JetEta", &L1JetEta, &b_L1JetEta);
   fChain->SetBranchAddress("L1JetPhi", &L1JetPhi, &b_L1JetPhi);
   fChain->SetBranchAddress("L1JetMatchDR", &L1JetMatchDR, &b_L1JetMatchDR);
   fChain->SetBranchAddress("L1TauVeto", &L1TauVeto, &b_L1TauVeto);
   fChain->SetBranchAddress("hasMatchedL1Jet", &hasMatchedL1Jet, &b_hasMatchedL1Jet);
   fChain->SetBranchAddress("hasMatchedL1TauJet", &hasMatchedL1TauJet, &b_hasMatchedL1TauJet);
   fChain->SetBranchAddress("hasMatchedL1CenJet", &hasMatchedL1CenJet, &b_hasMatchedL1CenJet);
   fChain->SetBranchAddress("L1MET", &L1MET, &b_L1MET);
   fChain->SetBranchAddress("L1MHT", &L1MHT, &b_L1MHT);
   fChain->SetBranchAddress("L1IsolationRegions_1GeV", &L1IsolationRegions_1GeV, &b_L1IsolationRegions_1GeV);
   fChain->SetBranchAddress("L1IsolationRegions_2GeV", &L1IsolationRegions_2GeV, &b_L1IsolationRegions_2GeV);
   fChain->SetBranchAddress("L1IsolationRegions_3GeV", &L1IsolationRegions_3GeV, &b_L1IsolationRegions_3GeV);
   fChain->SetBranchAddress("L1IsolationRegions_4GeV", &L1IsolationRegions_4GeV, &b_L1IsolationRegions_4GeV);
   fChain->SetBranchAddress("hasTriggeredAndMatchedL1TauJet", &hasTriggeredAndMatchedL1TauJet, &b_hasTriggeredAndMatchedL1TauJet);
   fChain->SetBranchAddress("hasTriggeredAndMatchedL1CenJet", &hasTriggeredAndMatchedL1CenJet, &b_hasTriggeredAndMatchedL1CenJet);
   fChain->SetBranchAddress("L2ECALIsolationEt", &L2ECALIsolationEt, &b_L2ECALIsolationEt);
   fChain->SetBranchAddress("L2TowerIsolationEt", &L2TowerIsolationEt, &b_L2TowerIsolationEt);
   fChain->SetBranchAddress("L2ClusterEtaRMS", &L2ClusterEtaRMS, &b_L2ClusterEtaRMS);
   fChain->SetBranchAddress("L2ClusterPhiRMS", &L2ClusterPhiRMS, &b_L2ClusterPhiRMS);
   fChain->SetBranchAddress("L2ClusterDeltaRRMS", &L2ClusterDeltaRRMS, &b_L2ClusterDeltaRRMS);
   fChain->SetBranchAddress("L2NClustersInAnnulus", &L2NClustersInAnnulus, &b_L2NClustersInAnnulus);
   fChain->SetBranchAddress("L2SeedTowerEt", &L2SeedTowerEt, &b_L2SeedTowerEt);
   fChain->SetBranchAddress("L2JetEt", &L2JetEt, &b_L2JetEt);
   fChain->SetBranchAddress("L2JetEta", &L2JetEta, &b_L2JetEta);
   fChain->SetBranchAddress("L2JetPhi", &L2JetPhi, &b_L2JetPhi);
   fChain->SetBranchAddress("L2HadEnergyFraction", &L2HadEnergyFraction, &b_L2HadEnergyFraction);
   fChain->SetBranchAddress("L2NTowers60", &L2NTowers60, &b_L2NTowers60);
   fChain->SetBranchAddress("L2NTowers90", &L2NTowers90, &b_L2NTowers90);
   fChain->SetBranchAddress("hasMatchedL2Jet", &hasMatchedL2Jet, &b_hasMatchedL2Jet);
   fChain->SetBranchAddress("PFEGIsolEt", &PFEGIsolEt, &b_PFEGIsolEt);
   fChain->SetBranchAddress("PFECALIsolationEt", &PFECALIsolationEt, &b_PFECALIsolationEt);
   fChain->SetBranchAddress("PFHCALTowIsolationEt", &PFHCALTowIsolationEt, &b_PFHCALTowIsolationEt);
   fChain->SetBranchAddress("PFECALTowIsolationEt", &PFECALTowIsolationEt, &b_PFECALTowIsolationEt);
   fChain->SetBranchAddress("PFNEGammaCandsAnnulus", &PFNEGammaCandsAnnulus, &b_PFNEGammaCandsAnnulus);
   fChain->SetBranchAddress("PFNHadCandsAnnulus", &PFNHadCandsAnnulus, &b_PFNHadCandsAnnulus);
   fChain->SetBranchAddress("PFHighestClusterEt", &PFHighestClusterEt, &b_PFHighestClusterEt);
   fChain->SetBranchAddress("PFEGammaClusterEtaRMS", &PFEGammaClusterEtaRMS, &b_PFEGammaClusterEtaRMS);
   fChain->SetBranchAddress("PFEGammaClusterPhiRMS", &PFEGammaClusterPhiRMS, &b_PFEGammaClusterPhiRMS);
   fChain->SetBranchAddress("PFEGammaClusterDeltaRRMS", &PFEGammaClusterDeltaRRMS, &b_PFEGammaClusterDeltaRRMS);
   fChain->SetBranchAddress("l25Eta", &l25Eta, &b_l25Eta);
   fChain->SetBranchAddress("l25Phi", &l25Phi, &b_l25Phi);
   fChain->SetBranchAddress("l25Et", &l25Et, &b_l25Et);
   fChain->SetBranchAddress("l25Pt", &l25Pt, &b_l25Pt);
   fChain->SetBranchAddress("l25PtLdgLoose", &l25PtLdgLoose, &b_l25PtLdgLoose);
   fChain->SetBranchAddress("l25EtaLdgLoose", &l25EtaLdgLoose, &b_l25EtaLdgLoose);
   fChain->SetBranchAddress("l25PhiLdgLoose", &l25PhiLdgLoose, &b_l25PhiLdgLoose);
   fChain->SetBranchAddress("l25PtLdgJetDRLoose", &l25PtLdgJetDRLoose, &b_l25PtLdgJetDRLoose);
   fChain->SetBranchAddress("l25InvPt", &l25InvPt, &b_l25InvPt);
   fChain->SetBranchAddress("l25Depth", &l25Depth, &b_l25Depth);
   fChain->SetBranchAddress("leadDepth1", &leadDepth1, &b_leadDepth1);
   fChain->SetBranchAddress("leadDepth2", &leadDepth2, &b_leadDepth2);
   fChain->SetBranchAddress("minDR", &minDR, &b_minDR);
   fChain->SetBranchAddress("bareEt", &bareEt, &b_bareEt);
   fChain->SetBranchAddress("l25DefDisc13", &l25DefDisc13, &b_l25DefDisc13);
   fChain->SetBranchAddress("l25DefDisc23", &l25DefDisc23, &b_l25DefDisc23);
   fChain->SetBranchAddress("l25DefDisc21", &l25DefDisc21, &b_l25DefDisc21);
   fChain->SetBranchAddress("l25LooseDisc13", &l25LooseDisc13, &b_l25LooseDisc13);
   fChain->SetBranchAddress("l25LooseDisc23", &l25LooseDisc23, &b_l25LooseDisc23);
   fChain->SetBranchAddress("l25LooseDisc21", &l25LooseDisc21, &b_l25LooseDisc21);
   /*
   fChain->SetBranchAddress("L1_DoubleEG05_TopBottom", &L1_DoubleEG05_TopBottom, &b_L1_DoubleEG05_TopBottom);
   fChain->SetBranchAddress("L1_DoubleEG1", &L1_DoubleEG1, &b_L1_DoubleEG1);
   fChain->SetBranchAddress("L1_DoubleEG5", &L1_DoubleEG5, &b_L1_DoubleEG5);
   fChain->SetBranchAddress("L1_DoubleHfBitCountsRing1_P1N1", &L1_DoubleHfBitCountsRing1_P1N1, &b_L1_DoubleHfBitCountsRing1_P1N1);
   fChain->SetBranchAddress("L1_DoubleHfBitCountsRing2_P1N1", &L1_DoubleHfBitCountsRing2_P1N1, &b_L1_DoubleHfBitCountsRing2_P1N1);
   fChain->SetBranchAddress("L1_DoubleHfRingEtSumsRing1_P200N200", &L1_DoubleHfRingEtSumsRing1_P200N200, &b_L1_DoubleHfRingEtSumsRing1_P200N200);
   fChain->SetBranchAddress("L1_DoubleHfRingEtSumsRing1_P4N4", &L1_DoubleHfRingEtSumsRing1_P4N4, &b_L1_DoubleHfRingEtSumsRing1_P4N4);
   fChain->SetBranchAddress("L1_DoubleHfRingEtSumsRing2_P200N200", &L1_DoubleHfRingEtSumsRing2_P200N200, &b_L1_DoubleHfRingEtSumsRing2_P200N200);
   fChain->SetBranchAddress("L1_DoubleHfRingEtSumsRing2_P4N4", &L1_DoubleHfRingEtSumsRing2_P4N4, &b_L1_DoubleHfRingEtSumsRing2_P4N4);
   fChain->SetBranchAddress("L1_DoubleJet30", &L1_DoubleJet30, &b_L1_DoubleJet30);
   fChain->SetBranchAddress("L1_DoubleMu3", &L1_DoubleMu3, &b_L1_DoubleMu3);
   fChain->SetBranchAddress("L1_DoubleMuOpen", &L1_DoubleMuOpen, &b_L1_DoubleMuOpen);
   fChain->SetBranchAddress("L1_DoubleMuTopBottom", &L1_DoubleMuTopBottom, &b_L1_DoubleMuTopBottom);
   fChain->SetBranchAddress("L1_DoubleTauJet14", &L1_DoubleTauJet14, &b_L1_DoubleTauJet14);
   fChain->SetBranchAddress("L1_ETM20", &L1_ETM20, &b_L1_ETM20);
   fChain->SetBranchAddress("L1_ETM30", &L1_ETM30, &b_L1_ETM30);
   fChain->SetBranchAddress("L1_ETM80", &L1_ETM80, &b_L1_ETM80);
   fChain->SetBranchAddress("L1_ETT60", &L1_ETT60, &b_L1_ETT60);
   fChain->SetBranchAddress("L1_HTT100", &L1_HTT100, &b_L1_HTT100);
   fChain->SetBranchAddress("L1_HTT200", &L1_HTT200, &b_L1_HTT200);
   fChain->SetBranchAddress("L1_IsoEG10_Jet6_ForJet6", &L1_IsoEG10_Jet6_ForJet6, &b_L1_IsoEG10_Jet6_ForJet6);
   fChain->SetBranchAddress("L1_MinBias_HTT10", &L1_MinBias_HTT10, &b_L1_MinBias_HTT10);
   fChain->SetBranchAddress("L1_Mu3QE8_Jet6", &L1_Mu3QE8_Jet6, &b_L1_Mu3QE8_Jet6);
   fChain->SetBranchAddress("L1_Mu5QE8_Jet6", &L1_Mu5QE8_Jet6, &b_L1_Mu5QE8_Jet6);
   fChain->SetBranchAddress("L1_QuadJet6", &L1_QuadJet6, &b_L1_QuadJet6);
   fChain->SetBranchAddress("L1_SingleEG1", &L1_SingleEG1, &b_L1_SingleEG1);
   fChain->SetBranchAddress("L1_SingleEG10", &L1_SingleEG10, &b_L1_SingleEG10);
   fChain->SetBranchAddress("L1_SingleEG12", &L1_SingleEG12, &b_L1_SingleEG12);
   fChain->SetBranchAddress("L1_SingleEG15", &L1_SingleEG15, &b_L1_SingleEG15);
   fChain->SetBranchAddress("L1_SingleEG2", &L1_SingleEG2, &b_L1_SingleEG2);
   fChain->SetBranchAddress("L1_SingleEG20", &L1_SingleEG20, &b_L1_SingleEG20);
   fChain->SetBranchAddress("L1_SingleEG5", &L1_SingleEG5, &b_L1_SingleEG5);
   fChain->SetBranchAddress("L1_SingleEG8", &L1_SingleEG8, &b_L1_SingleEG8);
   fChain->SetBranchAddress("L1_SingleHfBitCountsRing1_1", &L1_SingleHfBitCountsRing1_1, &b_L1_SingleHfBitCountsRing1_1);
   fChain->SetBranchAddress("L1_SingleHfBitCountsRing2_1", &L1_SingleHfBitCountsRing2_1, &b_L1_SingleHfBitCountsRing2_1);
   fChain->SetBranchAddress("L1_SingleHfRingEtSumsRing1_200", &L1_SingleHfRingEtSumsRing1_200, &b_L1_SingleHfRingEtSumsRing1_200);
   fChain->SetBranchAddress("L1_SingleHfRingEtSumsRing1_4", &L1_SingleHfRingEtSumsRing1_4, &b_L1_SingleHfRingEtSumsRing1_4);
   fChain->SetBranchAddress("L1_SingleHfRingEtSumsRing2_200", &L1_SingleHfRingEtSumsRing2_200, &b_L1_SingleHfRingEtSumsRing2_200);
   fChain->SetBranchAddress("L1_SingleHfRingEtSumsRing2_4", &L1_SingleHfRingEtSumsRing2_4, &b_L1_SingleHfRingEtSumsRing2_4);
   fChain->SetBranchAddress("L1_SingleIsoEG10", &L1_SingleIsoEG10, &b_L1_SingleIsoEG10);
   fChain->SetBranchAddress("L1_SingleIsoEG12", &L1_SingleIsoEG12, &b_L1_SingleIsoEG12);
   fChain->SetBranchAddress("L1_SingleIsoEG15", &L1_SingleIsoEG15, &b_L1_SingleIsoEG15);
   fChain->SetBranchAddress("L1_SingleIsoEG5", &L1_SingleIsoEG5, &b_L1_SingleIsoEG5);
   fChain->SetBranchAddress("L1_SingleIsoEG8", &L1_SingleIsoEG8, &b_L1_SingleIsoEG8);
   fChain->SetBranchAddress("L1_SingleJet20", &L1_SingleJet20, &b_L1_SingleJet20);
   fChain->SetBranchAddress("L1_SingleJet30", &L1_SingleJet30, &b_L1_SingleJet30);
   fChain->SetBranchAddress("L1_SingleJet40", &L1_SingleJet40, &b_L1_SingleJet40);
   fChain->SetBranchAddress("L1_SingleJet50", &L1_SingleJet50, &b_L1_SingleJet50);
   fChain->SetBranchAddress("L1_SingleJet6", &L1_SingleJet6, &b_L1_SingleJet6);
   fChain->SetBranchAddress("L1_SingleJet60", &L1_SingleJet60, &b_L1_SingleJet60);
   fChain->SetBranchAddress("L1_SingleMu0", &L1_SingleMu0, &b_L1_SingleMu0);
   fChain->SetBranchAddress("L1_SingleMu10", &L1_SingleMu10, &b_L1_SingleMu10);
   fChain->SetBranchAddress("L1_SingleMu14", &L1_SingleMu14, &b_L1_SingleMu14);
   fChain->SetBranchAddress("L1_SingleMu20", &L1_SingleMu20, &b_L1_SingleMu20);
   fChain->SetBranchAddress("L1_SingleMu3", &L1_SingleMu3, &b_L1_SingleMu3);
   fChain->SetBranchAddress("L1_SingleMu5", &L1_SingleMu5, &b_L1_SingleMu5);
   fChain->SetBranchAddress("L1_SingleMu7", &L1_SingleMu7, &b_L1_SingleMu7);
   fChain->SetBranchAddress("L1_SingleMuBeamHalo", &L1_SingleMuBeamHalo, &b_L1_SingleMuBeamHalo);
   fChain->SetBranchAddress("L1_SingleMuOpen", &L1_SingleMuOpen, &b_L1_SingleMuOpen);
   fChain->SetBranchAddress("L1_SingleTauJet10", &L1_SingleTauJet10, &b_L1_SingleTauJet10);
   fChain->SetBranchAddress("L1_SingleTauJet20", &L1_SingleTauJet20, &b_L1_SingleTauJet20);
   fChain->SetBranchAddress("L1_SingleTauJet30", &L1_SingleTauJet30, &b_L1_SingleTauJet30);
   fChain->SetBranchAddress("L1_SingleTauJet50", &L1_SingleTauJet50, &b_L1_SingleTauJet50);
   fChain->SetBranchAddress("L1_TripleJet14", &L1_TripleJet14, &b_L1_TripleJet14);
   fChain->SetBranchAddress("L1_ZeroBias", &L1_ZeroBias, &b_L1_ZeroBias);
   //fChain->SetBranchAddress("tauFilter", &tauFilter, &b_tauFilter);
   */
}
Bool_t TTEffTree::Notify() { return kTRUE; }

class Data: public TNamed {
public:
  Data(TH1 *origin, const char *name, const char *xlab):
    TNamed(Form("data_%s", name), "title"),
    total(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h1", name)))),
    passed_L1Jet(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h2", name)))),
    passed_L1TauJet(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h3", name)))),
    passed_L1CenJet(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h4", name)))),
    passed_L1Jet_TauVeto(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h5", name)))),
    passed_L1Jet_TauVeto_Isolation(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h6", name)))),
    passed_L1Jet_TauVeto_Isolation2(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h7", name)))),
    passed_L1Jet_TauVeto_Isolation3(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h8", name)))),
    passed_L1Jet_TauVeto_Isolation4(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h9", name)))),
    passed_L1Jet_EtCut(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h10", name)))),
    passed_L1Jet_EtCut2(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h11", name)))),
    xlabel(xlab)
  {}

  TH1 *total;
  TH1 *passed_L1Jet;
  TH1 *passed_L1TauJet;
  TH1 *passed_L1CenJet;

  TH1 *passed_L1Jet_TauVeto;
  TH1 *passed_L1Jet_TauVeto_Isolation;
  TH1 *passed_L1Jet_TauVeto_Isolation2;
  TH1 *passed_L1Jet_TauVeto_Isolation3;
  TH1 *passed_L1Jet_TauVeto_Isolation4;
  
  TH1 *passed_L1Jet_EtCut;
  TH1 *passed_L1Jet_EtCut2;

  std::string xlabel;

  ClassDef(Data, 0);
};


void TTEffTree::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void TTEffTree::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   fDataPt = dynamic_cast<Data *>(fInput->FindObject("data_pt"));
   fDataEta = dynamic_cast<Data *>(fInput->FindObject("data_eta"));
   fDataPhi = dynamic_cast<Data *>(fInput->FindObject("data_phi"));

}

Bool_t TTEffTree::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either TTEffTree::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  if(entry % 1000000 == 0)
    std::cout << "Entry " << entry << std::endl;

  // Preselection cuts
  if(!mc) {
    b_run->GetEntry(entry);
    if(run < 132658)
      return kTRUE;
  }

  b_PFTauIso->GetEntry(entry);
  if(PFTauIso < 0.5)
    return kTRUE;


  b_PFTauProng->GetEntry(entry);
  if(PFTauProng < 1)
    return kTRUE;

  b_PFTauInvPt->GetEntry(entry);
  if(PFTauInvPt >= 1./3.)
    return kTRUE;


  // pt and eta cuts
  b_PFTauPt->GetEntry(entry);
  b_PFTauEta->GetEntry(entry);
  b_PFTauPhi->GetEntry(entry);

  bool pass_pt = PFTauPt > 15.;
  bool pass_eta = TMath::Abs(PFTauEta) < 2.5;

  if(!pass_pt && !pass_eta)
    return kTRUE;

  if(!pass_eta)
    return kTRUE;

  // All jets
  if(pass_eta) fDataPt->total->Fill(PFTauPt);
  if(pass_pt)  fDataEta->total->Fill(PFTauEta);
  if(pass_eta && pass_pt) fDataPhi->total->Fill(PFTauPhi);

  b_hasMatchedL1TauJet->GetEntry(entry);
  b_hasMatchedL1CenJet->GetEntry(entry);

  // Matching to L1 jet
  if(!hasMatchedL1TauJet && !hasMatchedL1CenJet)
    return kTRUE;
  /*
  b_L1JetMatchDR->GetEntry(entry);
  if(L1JetMatchDR > 0.3)
    return kTRUE;
  */

  if(pass_eta) fDataPt->passed_L1Jet->Fill(PFTauPt);
  if(pass_pt)  fDataEta->passed_L1Jet->Fill(PFTauEta);
  if(pass_eta && pass_pt) fDataPhi->passed_L1Jet->Fill(PFTauPhi);

  if(hasMatchedL1TauJet) {
    if(pass_eta) fDataPt->passed_L1TauJet->Fill(PFTauPt);
    if(pass_pt)  fDataEta->passed_L1TauJet->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1TauJet->Fill(PFTauPhi);
  }
  else {
    if(pass_eta) fDataPt->passed_L1CenJet->Fill(PFTauPt);
    if(pass_pt)  fDataEta->passed_L1CenJet->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1CenJet->Fill(PFTauPhi); 
 }

  // Separation of L1 jets to tau and central jets
  bool tau = false;
  bool tau2 = false;

  b_L1TauVeto->GetEntry(entry);
  if(L1TauVeto == 0) {
    if(pass_eta) fDataPt->passed_L1Jet_TauVeto->Fill(PFTauPt);
    if(pass_pt)  fDataEta->passed_L1Jet_TauVeto->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_TauVeto->Fill(PFTauPhi);

    b_L1IsolationRegions_1GeV->GetEntry(entry);
    b_L1IsolationRegions_2GeV->GetEntry(entry);
    b_L1IsolationRegions_3GeV->GetEntry(entry);
    b_L1IsolationRegions_4GeV->GetEntry(entry);

    if(L1IsolationRegions_1GeV >= 7) {
      if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation->Fill(PFTauPt);
      if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation->Fill(PFTauEta);
      if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_TauVeto_Isolation->Fill(PFTauPhi);
      tau = true;
    }
    if(L1IsolationRegions_2GeV >= 7) {
      if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation2->Fill(PFTauPt);
      if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation2->Fill(PFTauEta);
      if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_TauVeto_Isolation2->Fill(PFTauPhi);
      tau2 = true;
    }
    if(L1IsolationRegions_3GeV >= 7) {
      if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation3->Fill(PFTauPt);
      if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation3->Fill(PFTauEta);
      if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_TauVeto_Isolation3->Fill(PFTauPhi);
    }
    if(L1IsolationRegions_4GeV >= 7) {
      if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation4->Fill(PFTauPt);
      if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation4->Fill(PFTauEta);
      if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_TauVeto_Isolation4->Fill(PFTauPhi);
    }
  }

  // L1 overall efficiency
  b_L1JetEt->GetEntry(entry);
  if( (tau && L1JetEt > tauCut) || (!tau && L1JetEt > cenCut) ) {
    if(pass_eta) fDataPt->passed_L1Jet_EtCut->Fill(PFTauPt);
    if(pass_pt)  fDataEta->passed_L1Jet_EtCut->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_EtCut->Fill(PFTauPhi);
  }
  if( (tau2 && L1JetEt > tauCut) || (!tau2 && L1JetEt > cenCut) ) {
    if(pass_eta) fDataPt->passed_L1Jet_EtCut2->Fill(PFTauPt);
    if(pass_pt)  fDataEta->passed_L1Jet_EtCut2->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_EtCut2->Fill(PFTauPhi);
  }


  /*
  if(PFTauProng >= 1 && PFTauInvPt < 1./3. && PFTauIso > 0.5 && (mc || run >= 132658)) {
    if(pass_eta) fDataPt->total->Fill(PFTauPt);
    if(pass_pt)  fDataEta->total->Fill(PFTauEta);

    if(hasMatchedL1Jet) {
      if(pass_eta) fDataPt->passed_L1Jet->Fill(PFTauPt);
      if(pass_pt)  fDataEta->passed_L1Jet->Fill(PFTauEta);

      if(hasMatchedL1TauJet) {
        if(pass_eta) fDataPt->passed_L1TauJet->Fill(PFTauPt);
        if(pass_pt)  fDataEta->passed_L1TauJet->Fill(PFTauEta);
      }
      else if(hasMatchedL1CenJet) {
        if(pass_eta) fDataPt->passed_L1CenJet->Fill(PFTauPt);
        if(pass_pt)  fDataEta->passed_L1CenJet->Fill(PFTauEta);
      }

      bool tau = false;
      bool tau2 = false;

      if(L1TauVeto == 0) {
        if(pass_eta) fDataPt->passed_L1Jet_TauVeto->Fill(PFTauPt);
        if(pass_pt)  fDataEta->passed_L1Jet_TauVeto->Fill(PFTauEta);

        if(L1IsolationRegions_1GeV >= 7) {
          if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation->Fill(PFTauPt);
          if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation->Fill(PFTauEta);
          tau = true;
        }
        if(L1IsolationRegions_2GeV >= 7) {
          if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation2->Fill(PFTauPt);
          if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation2->Fill(PFTauEta);
          tau2 = true;
        }
        if(L1IsolationRegions_3GeV >= 7) {
          if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation3->Fill(PFTauPt);
          if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation3->Fill(PFTauEta);
        }
        if(L1IsolationRegions_4GeV >= 7) {
          if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation4->Fill(PFTauPt);
          if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation4->Fill(PFTauEta);
        }
      }

      if( (tau && L1JetEt > tauCut) || (!tau && L1JetEt > cenCut) ) {
        if(pass_eta) fDataPt->passed_L1Jet_EtCut->Fill(PFTauPt);
        if(pass_pt)  fDataEta->passed_L1Jet_EtCut->Fill(PFTauEta);
      }
      if( (tau2 && L1JetEt > tauCut) || (!tau2 && L1JetEt > cenCut) ) {
        if(pass_eta) fDataPt->passed_L1Jet_EtCut2->Fill(PFTauPt);
        if(pass_pt)  fDataEta->passed_L1Jet_EtCut2->Fill(PFTauEta);
      }

    }
  }
  */

   return kTRUE;
}

void TTEffTree::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

  fOutput->AddLast(fDataPt);
  fOutput->AddLast(fDataEta);
  fOutput->AddLast(fDataPhi);
  fDataPt = 0;
  fDataEta = 0;
  fDataPhi = 0;

}

void TTEffTree::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}


struct Drawer {
  explicit Drawer(TString pd):
    plotDir(pd) {
    // Returns true if path does NOT exist!
    if(gSystem->AccessPathName(plotDir.Data()))
      gSystem->mkdir(plotDir.Data());
  }

  void addFormat(const std::string& format) {
    formats.push_back(format);
  }

  void draw(const char *fname, TGraphAsymmErrors *p1, const char *legend1=0, TLegend *leg=0) const {
    p1->SetMarkerColor(kBlack);
    if(legend1) {
      if(!leg)
        leg = createTLegend(0.4, 0.17, 0.7, 0.33);
      leg->AddEntry(p1, legend1, "p");
    }
    p1->Draw("AP");
    if(leg)
      leg->Draw();
    save(fname);
  }

  void draw(const char *fname,
            TGraphAsymmErrors *p1, const char *legend1,
            TGraphAsymmErrors *p2, const char *legend2,
            TLegend *leg=0) const {
    if(!leg)
      leg = createTLegend(0.4, 0.17, 0.7, 0.33);
    p1->SetMarkerColor(kBlack);
    p2->SetMarkerColor(kRed);
   leg->AddEntry(p1, legend1, "p");
    leg->AddEntry(p2, legend2, "p");
    p1->Draw("AP");
    p2->Draw("P same");
    leg->Draw();
    save(fname);
  }

  void drawMC(const char *fname,
            TGraphAsymmErrors *p1, const char *legend1,
            TGraphAsymmErrors *p2, const char *legend2,
            TLegend *leg=0) const {
    if(!leg)
      leg = createTLegend(0.4, 0.17, 0.7, 0.33);
    p1->SetMarkerColor(kBlack);
    p2->SetMarkerColor(kRed);
    leg->AddEntry(p1, legend1, "p");
    leg->AddEntry(p2, legend2, "p");
    p1->Draw("AP");
    p2->Draw("P same");
    p1->Draw("P same");
    /*
    p1->Draw("A");
    p2->Draw("PX same");
    p2->Draw("P same"); // workaround for error bars being drawn on top of markers
    p1->Draw("P same");
    */
    leg->Draw();
    save(fname);
  }


  void draw(const char *fname,
            TGraphAsymmErrors *p1, const char *legend1,
            TGraphAsymmErrors *p2, const char *legend2,
            TGraphAsymmErrors *p3, const char *legend3,
            TLegend *leg=0) const {
    if(!leg)
      leg = createTLegend(0.4, 0.17, 0.7, 0.33);
    p1->SetMarkerColor(kBlack);
    p2->SetMarkerColor(kRed);
    p3->SetMarkerColor(kBlue);
    leg->AddEntry(p1, legend1, "p");
    leg->AddEntry(p2, legend2, "p");
    leg->AddEntry(p3, legend3, "p");
    p1->Draw("AP");
    p2->Draw("P same");
    p3->Draw("P same");
    leg->Draw();
    save(fname);
  }

  void save(const char *fname) const {
    TLatex l;
    l.SetNDC();
    l.DrawLatex(0.5, 0.96, "CMS preliminary 2010");
    l.DrawLatex(0.2, 0.96, "#sqrt{s} = 7 TeV");

    for(size_t i=0; i<formats.size(); ++i) {
      gPad->SaveAs(plotDir + Form("/%s%s", fname, formats[i].c_str()));
    }
  }


  TString plotDir;
  std::vector<std::string> formats;
};

class Fitter {
public:
  enum Function {kInvalid, kFreq, kFreqGauss};

private:
  struct FitFunc {
    FitFunc() {}
    virtual ~FitFunc() {}
    virtual TF1 *function(float min, float max) const = 0;
    virtual TString functionName(const char *xvar) const = 0;
    virtual void drawStatsLatex(TF1 *func, TLatex& l, float x, float y) const = 0;
    virtual void drawStatsPave(TPaveStats *pv, float x, float y) const = 0;
    void drawStats(TGraph *graph, TF1 *func, TLatex& l, float x, float y) const {
      TList *lst = graph->GetListOfFunctions();
      if(!lst) {
        drawStatsLatex(func, l, x, y);
        return;
      }
      
      TPaveStats *pv = dynamic_cast<TPaveStats *>(lst->FindObject("stats"));
      if(pv) {
        pv->SetTextSize(0.03);
        drawStatsPave(pv, x, y);
      }
      else
        drawStatsLatex(func, l, x, y);
    }
  };


  struct FreqFit: public FitFunc {
    FreqFit(): FitFunc() {}
    virtual ~FreqFit() {}
    virtual TF1 *function(float min, float max) const {
      TF1 *myfit = new TF1("myfit","[0]*(TMath::Freq((sqrt(x)-sqrt([1]))/(2*[2])))", min, max);
      myfit->SetParameters(1., 10., 1.); //,.1,2,0.1);
      return myfit;
    }
    virtual TString functionName(const char *xvar) const {
      TString ret;
      ret.Form("p_{0} #times TMath::Freq  #left(#frac{ #sqrt{%s}- #sqrt{p_{1}}}{2p_{2}}#right)", xvar);
      return ret;
    }
    virtual void drawStatsLatex(TF1 *func, TLatex& l, float x, float y) const {
      l.DrawLatex(x, y, Form("p_{0} = %8.5f  (plateau)", func->GetParameter(0))); y -= 0.04;
      l.DrawLatex(x, y, Form("p_{1} = %8.5f  (turn on)", func->GetParameter(1))); y -= 0.04; 
      l.DrawLatex(x, y, Form("p_{2} = %8.5f  (turn on)", func->GetParameter(2))); y -= 0.04;
    }
    virtual void drawStatsPave(TPaveStats *pv, float x, float y) const {
      pv->SetX1NDC(x);
      pv->SetY1NDC(y-0.2);

      pv->SetX2NDC(x+0.3);
      pv->SetY2NDC(y);
    }
  };

  FitFunc *getFunction(Function func) const {
    switch(func) {
    case kFreq:
      return new FreqFit();
    case kFreqGauss:
      std::cout << "kFreqGauss is not yet implemented!" << std::endl;
      return 0;
    case kInvalid:
      std::cout << "Invalid fit function!" << std::endl;
      return 0;
    }
    return 0;
  }

public:
  explicit Fitter(const Drawer& d): draw(d) {
    l.SetTextSize(0.03);
    l.SetNDC(kTRUE);
    l.SetTextColor(kRed);
  }

  void fit(const char *fname, TGraphAsymmErrors *p, Function func, const char *xvar, float min, float max,
           const char *legend=0, TLegend *leg=0) const {
    draw.draw(fname, p, legend, leg);

    FitFunc *ff = getFunction(func);
    TF1 *myfit = ff->function(min, max);
    p->Fit("myfit", "QR+");
    myfit->Draw("same");
    
    float x = 0.45;
    float y = 0.58;
    l.DrawLatex(x, y, ff->functionName(xvar)); y -= 0.07;
    l.DrawLatex(x, y, Form("Fit from %.1f to %.1f", min, max)); y-=0.07;
    gPad->Update();
    ff->drawStats(p, myfit, l, x, y);

    draw.save(fname);
  }

  const Drawer& draw;
  mutable TLatex l;
};

struct Efficiency {
  Efficiency(const Drawer& d, const Data *data):
    draw(d),
    L1Jet(create(data->passed_L1Jet, data->total, data->xlabel)),
    L1TauJet(create(data->passed_L1TauJet, data->total, data->xlabel)),
    L1CenJet(create(data->passed_L1CenJet, data->total, data->xlabel)),
    L1TauJet_TauVeto_plus_Isolation(create(data->passed_L1TauJet, data->passed_L1Jet, data->xlabel)),

    L1Jet_TauVeto(create(data->passed_L1Jet_TauVeto, data->passed_L1Jet, data->xlabel)),
    L1Jet_TauVeto_Isolation (create(data->passed_L1Jet_TauVeto_Isolation,  data->passed_L1Jet_TauVeto, data->xlabel)),
    L1Jet_TauVeto_Isolation2(create(data->passed_L1Jet_TauVeto_Isolation2, data->passed_L1Jet_TauVeto, data->xlabel)),
    L1Jet_TauVeto_Isolation3(create(data->passed_L1Jet_TauVeto_Isolation3, data->passed_L1Jet_TauVeto, data->xlabel)),
    L1Jet_TauVeto_Isolation4(create(data->passed_L1Jet_TauVeto_Isolation4, data->passed_L1Jet_TauVeto, data->xlabel)),
    L1Jet_TauVeto_plus_Isolation(create(data->passed_L1Jet_TauVeto_Isolation, data->passed_L1Jet, data->xlabel)),
    L1Jet_TauVeto_plus_Isolation2(create(data->passed_L1Jet_TauVeto_Isolation2, data->passed_L1Jet, data->xlabel)),
 
    L1Jet_EtCut(create(data->passed_L1Jet_EtCut, data->passed_L1Jet, data->xlabel)),
    L1Jet_EtCut2(create(data->passed_L1Jet_EtCut2, data->passed_L1Jet, data->xlabel)),

    L1Jet_Overall(create(data->passed_L1Jet_EtCut, data->total, data->xlabel)),
    L1Jet_Overall2(create(data->passed_L1Jet_EtCut2, data->total, data->xlabel))
  {}

  static TGraphAsymmErrors *create(const TH1 *passed, const TH1 *total, const std::string& xlabel) {
    TGraphAsymmErrors *gr = new TGraphAsymmErrors(passed, total);
    gr->GetYaxis()->SetTitle("L1 Efficiency");
    gr->GetXaxis()->SetTitle(xlabel.c_str());
    gr->SetMaximum(1.1);
    gr->SetMinimum(0.0);
    return gr;
  }

  void drawL1Jet(const char *prefix, TLegend *leg) const {
    draw.draw(Form("%s_L1Jet", prefix), L1Jet, "L1 jet", leg);
  }
  void drawL1Jet_Cen_Tau(const char *prefix, TLegend *leg) const {
    draw.draw(Form("%s_L1Jet_Cen_Tau", prefix), L1Jet, "L1 jet", L1TauJet, "L1 tau jet", L1CenJet, "L1 central jet", leg);
  }
  void drawL1Jet_TauVeto(const char *prefix, TLegend *leg) const {
    draw.draw(Form("%s_L1Jet_TauVeto", prefix), L1Jet_TauVeto, "TauVeto", leg);
  }
  void drawL1Jet_TauVeto_Isolation(const char *prefix, TLegend *leg) const {
    draw.draw(Form("%s_L1Jet_TauVeto_Isolation", prefix), L1Jet_TauVeto_Isolation, "1 GeV isolation", L1Jet_TauVeto_Isolation2, "2 GeV isolation", leg);
  }
  void drawL1Jet_EtCut(const char *prefix, TLegend *leg) const {
    draw.draw(Form("%s_L1Jet_EtCut", prefix), L1Jet_EtCut, "1 GeV isolation", L1Jet_EtCut2, "2 GeV isolation", leg);
  }
  void drawL1Jet_Overall(const char *prefix, TLegend *leg) const {
    draw.draw(Form("%s_L1Jet_Overall2", prefix), L1Jet_Overall2);
    draw.draw(Form("%s_L1Jet_Overall", prefix), L1Jet_Overall, "1 GeV isolation", L1Jet_Overall2, "2 GeV isolation", leg);
  }
  void drawL1Jet_TauVeto_plus_Isolation(const char *prefix, TLegend *leg) const {
    draw.draw(Form("%s_L1Jet_TauVeto_plus_Isolation",  prefix), L1TauJet_TauVeto_plus_Isolation, "L1 Tau jet", L1Jet_TauVeto_plus_Isolation, "TauVeto + Isolation (1 GeV)", dynamic_cast<TLegend *>(leg->Clone()));
    draw.draw(Form("%s_L1Jet_TauVeto_plus_Isolation2", prefix), L1TauJet_TauVeto_plus_Isolation, "L1 Tau jet", L1Jet_TauVeto_plus_Isolation2, "TauVeto + Isolation (2 GeV)", dynamic_cast<TLegend *>(leg->Clone()));
  }


  void cmp_drawL1Jet(const char *prefix, const Efficiency& cmp, TLegend *leg, const char *thisLabel, const char *cmpLabel) const {
    draw.drawMC(Form("%s_L1Jet", prefix), L1Jet, thisLabel, cmp.L1Jet, cmpLabel, leg);
  }
  void cmp_drawL1Jet_TauVeto(const char *prefix, const Efficiency& cmp, TLegend *leg, const char *thisLabel, const char *cmpLabel) const {
    draw.drawMC(Form("%s_L1Jet_TauVeto", prefix), L1Jet_TauVeto, thisLabel, cmp.L1Jet_TauVeto, cmpLabel, leg);
  }
  void cmp_drawL1Jet_TauVeto_Isolation(const char *prefix, const Efficiency& cmp, TLegend *leg, const char *thisLabel, const char *cmpLabel) const {
    draw.drawMC(Form("%s_L1Jet_TauVeto_Isolation", prefix), L1Jet_TauVeto_Isolation, thisLabel, cmp.L1Jet_TauVeto_Isolation, cmpLabel, dynamic_cast<TLegend *>(leg->Clone()));
    draw.drawMC(Form("%s_L1Jet_TauVeto_Isolation2", prefix), L1Jet_TauVeto_Isolation2, thisLabel, cmp.L1Jet_TauVeto_Isolation2, cmpLabel, dynamic_cast<TLegend *>(leg->Clone()));
  }
  void cmp_drawL1Jet_EtCut(const char *prefix, const Efficiency& cmp, TLegend *leg, const char *thisLabel, const char *cmpLabel) const {
    draw.drawMC(Form("%s_L1Jet_EtCut", prefix), L1Jet_EtCut, thisLabel, cmp.L1Jet_EtCut, cmpLabel, dynamic_cast<TLegend *>(leg->Clone()));
    draw.drawMC(Form("%s_L1Jet_EtCut2", prefix), L1Jet_EtCut2, thisLabel, cmp.L1Jet_EtCut2, cmpLabel, dynamic_cast<TLegend *>(leg->Clone()));
  }
  void cmp_drawL1Jet_Overall(const char *prefix, const Efficiency& cmp, TLegend *leg, const char *thisLabel, const char *cmpLabel) const {
    draw.drawMC(Form("%s_L1Jet_Overall", prefix), L1Jet_Overall, thisLabel, cmp.L1Jet_Overall, cmpLabel, dynamic_cast<TLegend *>(leg->Clone()));
    draw.drawMC(Form("%s_L1Jet_Overall2", prefix), L1Jet_Overall2, thisLabel, cmp.L1Jet_Overall2, cmpLabel, dynamic_cast<TLegend *>(leg->Clone()));
  }

  

  const Drawer& draw;

  TGraphAsymmErrors *L1Jet;
  TGraphAsymmErrors *L1TauJet;
  TGraphAsymmErrors *L1CenJet;
  TGraphAsymmErrors *L1TauJet_TauVeto_plus_Isolation;

  TGraphAsymmErrors *L1Jet_TauVeto;
  TGraphAsymmErrors *L1Jet_TauVeto_Isolation;
  TGraphAsymmErrors *L1Jet_TauVeto_Isolation2;
  TGraphAsymmErrors *L1Jet_TauVeto_Isolation3;
  TGraphAsymmErrors *L1Jet_TauVeto_Isolation4;
  TGraphAsymmErrors *L1Jet_TauVeto_plus_Isolation;
  TGraphAsymmErrors *L1Jet_TauVeto_plus_Isolation2;

  TGraphAsymmErrors *L1Jet_EtCut;
  TGraphAsymmErrors *L1Jet_EtCut2;

  TGraphAsymmErrors *L1Jet_Overall;
  TGraphAsymmErrors *L1Jet_Overall2;
};

void dummyFunc(TTEffTree *analyser) {
}

void setMC(TTEffTree *analyser) {
  analyser->setMC(true);
}

struct SetCuts {
  SetCuts(float tauCut, float cenCut, bool mc=false):
    fTauCut(tauCut), fCenCut(cenCut), fMc(mc) {}

  void operator()(TTEffTree *analyser) const {
    analyser->setTauCut(fTauCut);
    analyser->setCenCut(fCenCut);
    analyser->setMC(fMc);
  }
  
  float fTauCut;
  float fCenCut;
  bool fMc;
};

template <typename T>
TList *analyse(TChain *chain, TList *inputList, int maxEvents, const T& func) {
  TTEffTree *analyser = new TTEffTree();
  analyser->SetInputList(inputList);

  func(analyser);

  if(maxEvents >= 0)
    chain->Process(analyser, "", maxEvents);
  else {
    chain->Process(analyser);
  }

  return analyser->GetOutputList();
}

TList *analyse(TChain *chain, TList *inputList, int maxEvents=-1) {
  return analyse(chain, inputList, maxEvents, dummyFunc);
}

void plotL1Efficiency() {
  setTDRStyle();

  Drawer draw("l1plots");
  draw.addFormat(".png");
  draw.addFormat(".eps");
  //draw.addFormat(".svg");

  Fitter fit(draw);

  //float bins[] = {0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 15., 25.}; 
  //float bins[] = {0., 1., 2., 3., 4., 6., 10., 25.};
  //float bins[] = {0., 1., 2., 3., 4., 5., 6., 8., 10., 15.};  //float bins[] = {0., 2., 4., 6., 8., 10., 12., 14., 16., 18., 20.,
  //                25., 30., 40, 60., 80., 100.};
  float bins[] = {0., 2., 4., 6., 8., 10., 12., 14., 16., 18., 20.
                  , 25., 30., 35., 40, 45., 50., 55., 60., 65., 70., 75., 80.
                  //, 85., 90., 95., 100.
                  //, 105., 110., 115., 120.
                  //, 125., 150.
                  
  };
  size_t nbins = sizeof(bins)/sizeof(float)-1;

  TH1F *histo_pt = new TH1F("foo1", "foo", nbins, bins);
  TH1F *histo_eta = new TH1F("foo2", "foo", 30, -3, 3.);
  TH1F *histo_phi = new TH1F("foo3", "foo", 35, -3.5, 3.5);

  TList *inputList = new TList();
  inputList->Add(new Data(histo_pt, "pt", "PF-#tau p_{T} (GeV/c)"));
  inputList->Add(new Data(histo_eta, "eta", "PF-#tau #eta"));
  inputList->Add(new Data(histo_phi, "phi", "PF-#tau #phi"));

  //TString path("/home/mjkortel/data/tautrigger/");
  TString path("/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/");
  //TString path("/tmp/mkortela/");

  TChain *chain = new TChain("TTEffTree");
  // Skim preproduction ntuple
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10-PromptReco_preproduction_CS_Tau-v1_RAW-RECO_tagV00-06-04.root");
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10-PromptReco_preproduction_CS_Tau-v2_RAW-RECO_tagV00-06-04.root");

  // Latest GOODCOLL ntuple
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132440-132716_tagV00-06-04.root");
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132717-133511_tagV00-06-04.root");
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132717-133511_tagV00-06-07.root");
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132440-135175_tagV00-06-07.root"); std::string label("V00-06-07");
  chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132440-135175_tagV00-06-08.root"); std::string label("MinBias (V00-06-08)");
  //chain->Add(path + "tteffAnalysis-pftau_JetMETTau_Run2010A-CS_Tau-v2_RAW-RECO_tagV00-06-08.root");     std::string label("CS_Tau (V00-06-08)");

  float leg_width = 0.2;
  float leg_height_1 = 0.07;
  float leg_height_2 = leg_height_1*2;
  float leg_height_3 = leg_height_1*3;

  bool prev=false;
  prev=true;

  bool mc=false;
  mc=true;

  TStopwatch clock;
  clock.Start();

  TList *list = analyse(chain, inputList);
  Data *data_pt = dynamic_cast<Data *>(list->FindObject("data_pt"));
  Data *data_eta = dynamic_cast<Data *>(list->FindObject("data_eta"));
  Data *data_phi = dynamic_cast<Data *>(list->FindObject("data_phi"));

  Efficiency eff_pt(draw, data_pt);
  const char *prefix = "data_PFTauPt";
  eff_pt.drawL1Jet(prefix, createTLegend(0.6, 0.2, leg_width, leg_height_1));
  eff_pt.drawL1Jet_Cen_Tau(prefix, createTLegend(0.7, 0.5, leg_width, leg_height_2));
  eff_pt.drawL1Jet_TauVeto(prefix, createTLegend(0.2, 0.2, leg_width, leg_height_1));
  eff_pt.drawL1Jet_TauVeto_Isolation(prefix, createTLegend(0.2, 0.2, leg_width+0.05, leg_height_2));
  eff_pt.drawL1Jet_EtCut(prefix, createTLegend(0.6, 0.2, leg_width+0.05, leg_height_2));
  eff_pt.drawL1Jet_Overall(prefix, createTLegend(0.6, 0.2, leg_width+0.05, leg_height_2));
  eff_pt.drawL1Jet_TauVeto_plus_Isolation(prefix, createTLegend(0.2, 0.2, leg_width+0.05, leg_height_2));
  
  Efficiency eff_eta(draw, data_eta);
  prefix = "data_PFTauEta";
  eff_eta.drawL1Jet(prefix, createTLegend(0.6, 0.2, leg_width, leg_height_1));
  eff_eta.drawL1Jet_Cen_Tau(prefix, createTLegend(0.4, 0.2, leg_width, leg_height_2));
  eff_eta.drawL1Jet_TauVeto(prefix, createTLegend(0.2, 0.2, leg_width, leg_height_1));
  eff_eta.drawL1Jet_TauVeto_Isolation(prefix, createTLegend(0.2, 0.2, leg_width+0.05, leg_height_2));
  //eff_eta.drawL1Jet_EtCut(prefix, createTLegend(0.6, 0.2, leg_width+0.05, leg_height_2));
  eff_eta.drawL1Jet_Overall(prefix, createTLegend(0.6, 0.2, leg_width+0.05, leg_height_2));
  eff_eta.drawL1Jet_TauVeto_plus_Isolation(prefix, createTLegend(0.2, 0.2, leg_width+0.05, leg_height_2));

  Efficiency eff_phi(draw, data_phi);
  prefix = "data_PFTauPhi";
  eff_phi.drawL1Jet(prefix, createTLegend(0.6, 0.2, leg_width, leg_height_1));
  eff_phi.drawL1Jet_Cen_Tau(prefix, createTLegend(0.4, 0.2, leg_width, leg_height_2));
  eff_phi.drawL1Jet_TauVeto(prefix, createTLegend(0.2, 0.2, leg_width, leg_height_1));
  eff_phi.drawL1Jet_TauVeto_Isolation(prefix, createTLegend(0.2, 0.2, leg_width+0.05, leg_height_2));
  //eff_phi.drawL1Jet_EtCut(prefix, createTLegend(0.6, 0.2, leg_width+0.05, leg_height_2));
  eff_phi.drawL1Jet_Overall(prefix, createTLegend(0.6, 0.2, leg_width+0.05, leg_height_2));
  eff_phi.drawL1Jet_TauVeto_plus_Isolation(prefix, createTLegend(0.2, 0.2, leg_width+0.05, leg_height_2));

  if(prev) {
    inputList = new TList();
    inputList->Add(new Data(histo_pt, "pt", "PF-#tau p_{T} (GeV/c)"));
    inputList->Add(new Data(histo_eta, "eta", "PF-#tau #eta"));
    inputList->Add(new Data(histo_phi, "phi", "PF-#tau #phi"));

    TChain *chain = new TChain("TTEffTree");
    //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132717-133511_tagV00-06-04.root");     std::string prevLabel("V00-06-04");
    chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132440-135175_tagV00-06-07.root");     std::string prevLabel("MinBias (V00-06-07)");
    //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132440-135175_tagV00-06-08.root"); std::string prevLabel("MinBias (V00-06-08");
    //chain->Add(path + "tteffAnalysis-pftau_JetMETTau_Run2010A-CS_Tau-v2_RAW-RECO_tagV00-06-08.root");     std::string prevLabel("CS_Tau (V00-06-08)");

    leg_width += 0.1;

    //list = analyse(chain, inputList, -1, SetCuts(22, 40));
    list = analyse(chain, inputList);
    Data *prev_pt = dynamic_cast<Data *>(list->FindObject("data_pt"));
    Data *prev_eta = dynamic_cast<Data *>(list->FindObject("data_eta"));
    Data *prev_phi = dynamic_cast<Data *>(list->FindObject("data_phi"));

    Efficiency eff_prev_pt(draw, prev_pt);
    prefix = "data_prev_PFTauPt";
    eff_pt.cmp_drawL1Jet(prefix, eff_prev_pt, createTLegend(0.6, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    eff_pt.cmp_drawL1Jet_TauVeto(prefix, eff_prev_pt, createTLegend(0.2, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    eff_pt.cmp_drawL1Jet_TauVeto_Isolation(prefix, eff_prev_pt, createTLegend(0.2, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    eff_pt.cmp_drawL1Jet_EtCut(prefix, eff_prev_pt, createTLegend(0.6, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    eff_pt.cmp_drawL1Jet_Overall(prefix, eff_prev_pt, createTLegend(0.6, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());

    eff_prev_pt.drawL1Jet_TauVeto_plus_Isolation("prev_PFTauPt", createTLegend(0.2, 0.2, leg_width+0.05, leg_height_2));

    Efficiency eff_prev_eta(draw, prev_eta);
    prefix = "data_prev_PFTauEta";
    eff_eta.cmp_drawL1Jet(prefix, eff_prev_eta, createTLegend(0.6, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    eff_eta.cmp_drawL1Jet_TauVeto(prefix, eff_prev_eta, createTLegend(0.6, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    eff_eta.cmp_drawL1Jet_TauVeto_Isolation(prefix, eff_prev_eta, createTLegend(0.6, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    //eff_eta.cmp_drawL1Jet_EtCut(prefix, eff_prev_eta, createTLegend(0.6, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    eff_eta.cmp_drawL1Jet_Overall(prefix, eff_prev_eta, createTLegend(0.6, 0.8, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());

    Efficiency eff_prev_phi(draw, prev_phi);
    prefix = "data_prev_PFTauPhi";
    eff_phi.cmp_drawL1Jet(prefix, eff_prev_phi, createTLegend(0.6, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    eff_phi.cmp_drawL1Jet_TauVeto(prefix, eff_prev_phi, createTLegend(0.6, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    eff_phi.cmp_drawL1Jet_TauVeto_Isolation(prefix, eff_prev_phi, createTLegend(0.6, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    //eff_phi.cmp_drawL1Jet_EtCut(prefix, eff_prev_phi, createTLegend(0.6, 0.2, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());
    eff_phi.cmp_drawL1Jet_Overall(prefix, eff_prev_phi, createTLegend(0.6, 0.8, leg_width, leg_height_2), label.c_str(), prevLabel.c_str());

    leg_width -= 0.1;
  }


  if(mc) {
    inputList = new TList();
    inputList->Add(new Data(histo_pt, "pt", "PF-#tau p_{T} (GeV/c)"));
    inputList->Add(new Data(histo_eta, "eta", "PF-#tau #eta"));
    inputList->Add(new Data(histo_phi, "phi", "PF-#tau #phi"));

    path = "/castor/cern.ch/user/e/eluiggi/ttEff/";

    std::string dataLabel("Data");
    //std::string dataLabel = label; leg_width += 0.1;
    //std::string mcLabel("MC");
    std::string mcLabel("Simulation");

    chain = new TChain("TTEffTree");
    //chain->Add(path + "tteffAnalyzerMinBiasMC7TeV357-PFTau.root");
    chain->Add(path + "tteffAnalyzerMinBiasMC7TeV357L1Extra-PFTau.root");

    list = analyse(chain, inputList, -1, setMC);
    //list = analyse(chain, inputList, -1, SetCuts(22, 40, true));
    Data *mc_pt = dynamic_cast<Data *>(list->FindObject("data_pt"));
    Data *mc_eta = dynamic_cast<Data *>(list->FindObject("data_eta"));
    Data *mc_phi = dynamic_cast<Data *>(list->FindObject("data_phi"));

    Efficiency eff_mc_pt(draw, mc_pt);
    prefix = "data_mc_PFTauPt";
    eff_pt.cmp_drawL1Jet(prefix, eff_mc_pt, createTLegend(0.6, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    eff_pt.cmp_drawL1Jet_TauVeto(prefix, eff_mc_pt, createTLegend(0.2, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    eff_pt.cmp_drawL1Jet_TauVeto_Isolation(prefix, eff_mc_pt, createTLegend(0.2, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    eff_pt.cmp_drawL1Jet_EtCut(prefix, eff_mc_pt, createTLegend(0.6, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    eff_pt.cmp_drawL1Jet_Overall(prefix, eff_mc_pt, createTLegend(0.6, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());

    Efficiency eff_mc_eta(draw, mc_eta);
    prefix = "data_mc_PFTauEta";
    eff_eta.cmp_drawL1Jet(prefix, eff_mc_eta, createTLegend(0.6, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    eff_eta.cmp_drawL1Jet_TauVeto(prefix, eff_mc_eta, createTLegend(0.6, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    eff_eta.cmp_drawL1Jet_TauVeto_Isolation(prefix, eff_mc_eta, createTLegend(0.6, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    //eff_eta.cmp_drawL1Jet_EtCut(prefix, eff_mc_eta, createTLegend(0.6, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    eff_eta.cmp_drawL1Jet_Overall(prefix, eff_mc_eta, createTLegend(0.6, 0.8, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());

    Efficiency eff_mc_phi(draw, mc_phi);
    prefix = "data_mc_PFTauPhi";
    eff_phi.cmp_drawL1Jet(prefix, eff_mc_phi, createTLegend(0.6, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    eff_phi.cmp_drawL1Jet_TauVeto(prefix, eff_mc_phi, createTLegend(0.6, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    eff_phi.cmp_drawL1Jet_TauVeto_Isolation(prefix, eff_mc_phi, createTLegend(0.6, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    //eff_phi.cmp_drawL1Jet_EtCut(prefix, eff_mc_phi, createTLegend(0.6, 0.2, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
    eff_phi.cmp_drawL1Jet_Overall(prefix, eff_mc_phi, createTLegend(0.6, 0.8, leg_width, leg_height_2), dataLabel.c_str(), mcLabel.c_str());
  }


  fit.fit("data_fit_PFTauPt_L1Jet", eff_pt.L1Jet, Fitter::kFreq, "p_{T}", 5., 100.);

  clock.Stop();
  clock.Print();

}
