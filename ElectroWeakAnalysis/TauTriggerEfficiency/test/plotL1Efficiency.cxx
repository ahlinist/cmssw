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
#include <TCanvas.h>
#include <TTreeFormula.h>
#include <TProfile.h>
#include <THStack.h>

#include<vector>
#include<string>
#include<iostream>
#include<functional>

#include "tdrstyle.cxx"

class Data;
class DistData;
class Drawer;

struct Metadata {
  enum Dir { kDown, kRight };
};

template <typename T>
class Plot {
public:

  Plot(T *obj):
    fPlot(obj),
    fLegendX(0.6), fLegendY(0.2), fLegendWidth(0.2), fLegendHeight(0.07),
    fMetaX(0.2), fMetaY(0.89), fLumi(""), fDir(Metadata::kDown)
  {}
  Plot(T *obj, const std::string& lumi):
    fPlot(obj),
    fLegendX(0.6), fLegendY(0.2), fLegendWidth(0.2), fLegendHeight(0.07),
    fMetaX(0.2), fMetaY(0.89), fLumi(lumi), fDir(Metadata::kDown)
  {}
  ~Plot() { delete fPlot; }

  TLegend *createLegend() const {
    TLegend *leg = new TLegend(fLegendX, fLegendY, fLegendX+fLegendWidth, fLegendY+fLegendHeight);
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

  void drawMetadata() const {
    TLatex l;
    l.SetNDC();
    l.SetTextSize(0.04);

    if(fDir == Metadata::kDown) {
      float y = fMetaY;
      
      l.DrawLatex(fMetaX, y, "CMS preliminary 2010"); y-=0.05;
      l.DrawLatex(fMetaX, y, "#sqrt{s} = 7 TeV"); y-=0.05;
      if(!fLumi.empty())
        l.DrawLatex(fMetaX, y, fLumi.c_str()); y-=0.05;
    }
    else if(fDir == Metadata::kRight) {
      if(fLumi.empty())
        l.DrawLatex(fMetaX, fMetaY, "CMS preliminary 2010, #sqrt{s}=7TeV");
      else
        l.DrawLatex(fMetaX, fMetaY, ("CMS preliminary 2010, #sqrt{s}=7TeV, "+fLumi).c_str());
    }
  }

  Plot& setLegendX(float x) {
    fLegendX = x;
    return *this;
  }
  Plot& setLegendY(float y) {
    fLegendY = y;
    return *this;
  }
  Plot& setLegendWidth(float w) {
    fLegendWidth = w;
    return *this;
  }
  Plot& setLegendHeight(float h) {
    fLegendHeight = h;
    return *this;
  }
  Plot& setLegendPos(float x, float y) {
    setLegendX(x);
    setLegendY(y);
    return *this;
  }
  Plot& setLegend(float x, float y, float w, float h) {
    setLegendPos(x, y);
    setLegendWidth(w);
    setLegendHeight(h);
    return *this;
  }

  Plot& setMetaX(float x) {
    fMetaX = x;
    return *this;
  }
  Plot& setMetaY(float y) {
    fMetaY = y;
    return *this;
  }
  Plot& setMetaPos(float x, float y) {
    setMetaX(x);
    setMetaY(y);
    return *this;
  }
  Plot& setMetaDir(Metadata::Dir dir) {
    fDir = dir;
    return *this;
  }
  Plot& setLumi(const std::string& lumi) {
    fLumi = lumi;
    return *this;
  }

  template <typename T2>
  Plot& takeConf(const Plot<T2>& pl) {
    setLegend(pl.fLegendX, pl.fLegendY, pl.fLegendWidth, pl.fLegendHeight);
    setMetaPos(pl.fMetaX, pl.fMetaY);
    setMetaDir(pl.fDir);
    setLumi(pl.fLumi);
    return *this;
  }

  const T *getPlot() const {
    return fPlot;
  }
  
  T *operator->() {
    return fPlot;
  }

  // Intentional break in the const-correctness :(
  // Needed because style changes require that the TH1/TGraph are non-const
  T *operator->() const {
    return fPlot;
  }

private:
  // disable copy construction and assignment
  Plot(const Plot&);            // NOT IMPLEMENTED
  Plot& operator=(const Plot&); // NOT IMPLEMENTED


  T *fPlot;

  float fLegendX;
  float fLegendY;
  float fLegendWidth;
  float fLegendHeight;

  float fMetaX;
  float fMetaY;
  std::string fLumi;
  Metadata::Dir fDir;
};

struct PlotLegendHeightWidth {
  PlotLegendHeightWidth(float h, float w): height(h), width(w) {}

  template <typename T>
  void operator()(T& p) const {
    p.setLegendHeight(height).setLegendWidth(width);
  }

  const float height;
  const float width;
};

struct PlotLegend {
  PlotLegend(float x, float y, float h, float w): fX(x), fY(y), fH(h), fW(w) {}

  template <typename T>
  void operator()(T &p) const {
    p.setLegend(fX, fY, fH, fW);
  }

  const float fX;
  const float fY;
  const float fH;
  const float fW;
};

struct PlotMetaPos {
  PlotMetaPos(float x, float y): fX(x), fY(y) {}

  template <typename T>
  void operator()(T& p) const {
    p.setMetaPos(fX, fY);
  }

  const float fX;
  const float fY;
};

struct PlotLumi {
  PlotLumi(const std::string& l): fLumi(l) {}

  template <typename T>
  void operator()(T& p) const {
    p.setLumi(fLumi);
  }

  const std::string fLumi;
};




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

  std::string fSelectionString;
  TTreeFormula *fSelection; //! pointer to common selection formula

  DistData *fDist; //! pointer to data structure to be filled

  Data *fDataPt; //! pointer to data structure to be filled
  Data *fDataEta; //! pointer to data structure to be filled
  Data *fDataPhi; //! pointer to data structure to be filled

  Float_t tauCut;
  Float_t cenCut;

  Long64_t allEntries;
  Long64_t selectedEntries;
  Long64_t selectedL1JetEntries;
  Long64_t selectedL1JetEtEntries;
  
  //TTEffTree(TTree * /*tree*/ =0): mc(0), tauCut(14), cenCut(30) { }
  TTEffTree(const std::string& selection): fSelectionString(selection), fSelection(0), tauCut(14), cenCut(30),
                                           allEntries(0), selectedEntries(0),
                                           selectedL1JetEntries(0), selectedL1JetEtEntries(0)
  {}
  virtual ~TTEffTree() { delete fSelection; }
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

   if(!fSelectionString.empty())
     fSelection = new TTreeFormula(fSelectionString.c_str(), fSelectionString.c_str(), fChain);

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
Bool_t TTEffTree::Notify() { 
  if(fSelection)
    fSelection->UpdateFormulaLeaves();
  return kTRUE; 
}

class Data: public TNamed {
public:
  Data(const TH1 *origin, const char *name, const std::string& xlab):
    TNamed(Form("data_%s", name), "title"),
    total(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h1", name)))),
    passed_L1Jet(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h2", name)))),
    passed_L1TauJet(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h3", name)))),
    passed_L1CenJet(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h4", name)))),
    passed_L1TauJet_EtCut(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h5", name)))),
    passed_L1Jet_TauVeto(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h6", name)))),
    passed_L1Jet_TauVeto_Isolation(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h7", name)))),
    passed_L1Jet_TauVeto_Isolation2(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h8", name)))),
    passed_L1Jet_TauVeto_Isolation3(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h9", name)))),
    passed_L1Jet_TauVeto_Isolation4(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h10", name)))),
    passed_L1Jet_Tau_EtCut2(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h11", name)))),
    passed_L1Jet_Cen_EtCut2(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h12", name)))),
    passed_L1Jet_EtCut(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h13", name)))),
    passed_L1Jet_EtCut2(dynamic_cast<TH1 *>(origin->Clone(Form("%s_h14", name)))),
    xlabel(xlab)
  {}

  TH1 *total;
  TH1 *passed_L1Jet;
  TH1 *passed_L1TauJet;
  TH1 *passed_L1CenJet;
  TH1 *passed_L1TauJet_EtCut;

  TH1 *passed_L1Jet_TauVeto;
  TH1 *passed_L1Jet_TauVeto_Isolation;
  TH1 *passed_L1Jet_TauVeto_Isolation2;
  TH1 *passed_L1Jet_TauVeto_Isolation3;
  TH1 *passed_L1Jet_TauVeto_Isolation4;
  
  TH1 *passed_L1Jet_Tau_EtCut2;
  TH1 *passed_L1Jet_Cen_EtCut2;

  TH1 *passed_L1Jet_EtCut;
  TH1 *passed_L1Jet_EtCut2;

  std::string xlabel;

  ClassDef(Data, 0);
};

class NumberData {
public:
  NumberData(Long64_t all, Long64_t sel, Long64_t selJet, Long64_t selJetEt):
    allEntries(all), selectedEntries(sel),
    selectedL1JetEntries(selJet), selectedL1JetEtEntries(selJetEt)
  {}

  Long64_t allEntries;
  Long64_t selectedEntries;
  Long64_t selectedL1JetEntries;
  Long64_t selectedL1JetEtEntries;
};

class DistData: public TNamed {
public:
  struct Histos {
    static TH2 *createResolution(const TH1 *etplot, const char *prefix) {
      const Double_t *binarray = etplot->GetXaxis()->GetXbins()->GetArray();
      if(binarray)
        return new TH2F(Form("%s_dist_h8", prefix), "", etplot->GetNbinsX(), binarray,
                        50, 0., 2.);
      else
        return new TH2F(Form("%s_dist_h8", prefix), "", etplot->GetNbinsX(),
                        etplot->GetXaxis()->GetXmin(), etplot->GetXaxis()->GetXmax(),
                        50, 0., 2.);
    }

    Histos(const TH1 *etplot, const char *prefix):
      hasMatchedL1Jet(new TH1F(Form("%s_dist_h1", prefix), "", 2, 0., 2.)),
      l1TauVeto(new TH1F(Form("%s_dist_h2", prefix), "", 2, 0., 2.)),
      l1IsolationRegions2(new TH1F(Form("%s_dist_h3", prefix), "", 9, 0., 9.)),
      l1JetEt(new TH1F(Form("%s_dist_h4", prefix),"", 25, 0., 50.)),
      l1TauJetEt(dynamic_cast<TH1 *>(l1JetEt->Clone(Form("%s_dist_h5", prefix)))),
      l1CenJetEt(dynamic_cast<TH1 *>(l1JetEt->Clone(Form("%s_dist_h6", prefix)))),
      l1JetEtResolution(new TH1F(Form("%s_dist_h7", prefix), "", 20, -1., 1.)),
      l1JetEtResolution_PFTauEt(createResolution(etplot, prefix))
      //l1JetEtResolution_PFTauEt(new TH2F(Form("%s_dist_h8", prefix), "", etplot->GetNbinsX(), //,
                                           //                                   50, 0., 2.))
    {
      hasMatchedL1Jet->GetXaxis()->SetTitle("hasMatchedL1Jet");
      hasMatchedL1Jet->GetYaxis()->SetTitle("Occurrences");
      //hasMatchedL1Jet->GetXaxis()->SetBinLabel(1, "false");
      //hasMatchedL1Jet->GetXaxis()->SetBinLabel(2, "true");

      l1TauVeto->GetXaxis()->SetTitle("TauVeto");
      l1TauVeto->GetYaxis()->SetTitle("Occurrences");
      //l1TauVeto->GetXaxis()->SetBinLabel(1, "0");
      //l1TauVeto->GetXaxis()->SetBinLabel(2, "1");

      l1IsolationRegions2->GetXaxis()->SetTitle("L1 isolation regions");
      l1IsolationRegions2->GetYaxis()->SetTitle("Occurrences");

      l1JetEt->GetXaxis()->SetTitle("L1 jet E_{T} (GeV)");
      l1JetEt->GetYaxis()->SetTitle("Occurrences");

      l1TauJetEt->GetXaxis()->SetTitle("L1 tau jet E_{T} (GeV)");
      l1TauJetEt->GetYaxis()->SetTitle("Occurrences");

      l1CenJetEt->GetXaxis()->SetTitle("L1 central jet E_{T} (GeV)");
      l1CenJetEt->GetYaxis()->SetTitle("Occurrences");

      l1JetEtResolution->GetXaxis()->SetTitle("#DeltaE_{T} / PF-#tau E_{T}");
      l1JetEtResolution->GetYaxis()->SetTitle("Occurrences");
      
      l1JetEtResolution_PFTauEt->GetXaxis()->SetTitle("PF-#tau E_{T} (GeV)");
      l1JetEtResolution_PFTauEt->GetYaxis()->SetTitle("|#DeltaE_{T}| / PF-#tau E_{T}");
    };

    void normalize(double norm) {
      hasMatchedL1Jet->Scale(norm);
      l1TauVeto->Scale(norm);
      l1IsolationRegions2->Scale(norm);
      l1JetEt->Scale(norm);
      l1TauJetEt->Scale(norm);
      l1CenJetEt->Scale(norm);
      l1JetEtResolution->Scale(norm);
    }

    void takeConf(const Histos& h) {
      hasMatchedL1Jet.takeConf(h.hasMatchedL1Jet);
      l1TauVeto.takeConf(h.l1TauVeto);
      l1IsolationRegions2.takeConf(h.l1IsolationRegions2);
      l1JetEt.takeConf(h.l1JetEt);
      l1TauJetEt.takeConf(h.l1TauJetEt);
      l1CenJetEt.takeConf(h.l1CenJetEt);
      l1JetEtResolution.takeConf(h.l1JetEtResolution);
      l1JetEtResolution_PFTauEt.takeConf(h.l1JetEtResolution_PFTauEt);
    }

    template <typename F>
    void forEach(F func) {
      func(hasMatchedL1Jet);
      func(l1TauVeto);
      func(l1IsolationRegions2);
      func(l1JetEt);
      func(l1TauJetEt);
      func(l1CenJetEt);
      func(l1JetEtResolution);
      func(l1JetEtResolution_PFTauEt);
    }

    Plot<TH1> hasMatchedL1Jet;
    Plot<TH1> l1TauVeto;
    Plot<TH1> l1IsolationRegions2;
    Plot<TH1> l1JetEt;
    Plot<TH1> l1TauJetEt;
    Plot<TH1> l1CenJetEt;
    Plot<TH1> l1JetEtResolution;
    Plot<TH2> l1JetEtResolution_PFTauEt;
  };


  DistData(const TH1 *etplot):
    TNamed("dist", ""), number(0), histos(etplot, ""), histos_raw(etplot, "raw")
  {
  }

  void normalize(const DistData *d) {
    //double norm = d->number->allEntries / static_cast<double>(number->allEntries);
    //double norm = d->number->selectedEntries / static_cast<double>(number->selectedEntries);
    double norm = d->number->selectedL1JetEtEntries / static_cast<double>(number->selectedL1JetEtEntries);
    histos.normalize(norm);

    norm = d->number->selectedL1JetEntries / static_cast<double>(number->selectedL1JetEntries);
    histos_raw.normalize(norm);
  }



  NumberData *number;
  Histos histos;
  Histos histos_raw;

  ClassDef(DistData, 0);
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

   fDist = new DistData(fDataPt->total /*, fDataPt->xlabel.c_str()*/);
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

  const float cut_pt = 15;
  const float cut_eta = 2.4;

  if(entry % 1000000 == 0)
    std::cout << "Entry " << entry << std::endl;

  // PFTau selection
  ++allEntries;
  if(fSelection && fSelection->EvalInstance(0) < 0.5)
    return kTRUE;

  ++selectedEntries;


  // pt and eta cuts
  b_PFTauPhi->GetEntry(entry);
  b_PFTauEta->GetEntry(entry);

  b_PFTauPt->GetEntry(entry);
  b_PFTauEt->GetEntry(entry);
  //float ptvar = PFTauPt;
  float ptvar = PFTauEt;

  bool pass_pt = ptvar > cut_pt;
  bool pass_eta = TMath::Abs(PFTauEta) < cut_eta;

  if(!pass_pt && !pass_eta)
    return kTRUE;

  b_L1JetEt->GetEntry(entry);
  bool pass_l1et = L1JetEt > tauCut;

  // All jets
  if(pass_eta) fDataPt->total->Fill(ptvar);
  if(pass_pt)  fDataEta->total->Fill(PFTauEta);
  if(pass_eta && pass_pt) fDataPhi->total->Fill(PFTauPhi);

  b_hasMatchedL1TauJet->GetEntry(entry);
  b_hasMatchedL1CenJet->GetEntry(entry);

  // Matching to L1 jet
  if(!hasMatchedL1TauJet && !hasMatchedL1CenJet) {
    fDist->histos_raw.hasMatchedL1Jet->Fill(0);
    if(pass_l1et) fDist->histos.hasMatchedL1Jet->Fill(0);
    return kTRUE;
  }
  fDist->histos_raw.hasMatchedL1Jet->Fill(1);
  if(pass_l1et) fDist->histos.hasMatchedL1Jet->Fill(1);

  ++selectedL1JetEntries;
  if(pass_l1et) ++selectedL1JetEtEntries;

  /*
  b_L1JetMatchDR->GetEntry(entry);
  if(L1JetMatchDR > 0.3)
    return kTRUE;
  */

  if(pass_eta) fDataPt->passed_L1Jet->Fill(ptvar);
  if(pass_pt)  fDataEta->passed_L1Jet->Fill(PFTauEta);
  if(pass_eta && pass_pt) fDataPhi->passed_L1Jet->Fill(PFTauPhi);

  if(hasMatchedL1TauJet) {
    if(pass_eta) fDataPt->passed_L1TauJet->Fill(ptvar);
    if(pass_pt)  fDataEta->passed_L1TauJet->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1TauJet->Fill(PFTauPhi);
  }
  else {
    if(pass_eta) fDataPt->passed_L1CenJet->Fill(ptvar);
    if(pass_pt)  fDataEta->passed_L1CenJet->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1CenJet->Fill(PFTauPhi); 
 }

  // Separation of L1 jets to tau and central jets
  bool tau = false;   // is L1 tau with 1 GeV isolation
  bool tau2 = false;  // is L1 tau with 2 GeV isolation

  b_L1TauVeto->GetEntry(entry);
  fDist->histos_raw.l1TauVeto->Fill(L1TauVeto);
  if(pass_l1et) fDist->histos.l1TauVeto->Fill(L1TauVeto);

  b_L1IsolationRegions_2GeV->GetEntry(entry);
  fDist->histos_raw.l1IsolationRegions2->Fill(L1IsolationRegions_2GeV);
  if(pass_l1et) fDist->histos.l1IsolationRegions2->Fill(L1IsolationRegions_2GeV);

  if(L1TauVeto == 0) {
    if(pass_eta) fDataPt->passed_L1Jet_TauVeto->Fill(ptvar);
    if(pass_pt)  fDataEta->passed_L1Jet_TauVeto->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_TauVeto->Fill(PFTauPhi);

    b_L1IsolationRegions_1GeV->GetEntry(entry);
    b_L1IsolationRegions_3GeV->GetEntry(entry);
    b_L1IsolationRegions_4GeV->GetEntry(entry);

    if(L1IsolationRegions_1GeV >= 7) {
      if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation->Fill(ptvar);
      if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation->Fill(PFTauEta);
      if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_TauVeto_Isolation->Fill(PFTauPhi);
      tau = true;
    }
    if(L1IsolationRegions_2GeV >= 7) {
      if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation2->Fill(ptvar);
      if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation2->Fill(PFTauEta);
      if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_TauVeto_Isolation2->Fill(PFTauPhi);
      tau2 = true;
    }
    if(L1IsolationRegions_3GeV >= 7) {
      if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation3->Fill(ptvar);
      if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation3->Fill(PFTauEta);
      if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_TauVeto_Isolation3->Fill(PFTauPhi);
    }
    if(L1IsolationRegions_4GeV >= 7) {
      if(pass_eta) fDataPt->passed_L1Jet_TauVeto_Isolation4->Fill(ptvar);
      if(pass_pt)  fDataEta->passed_L1Jet_TauVeto_Isolation4->Fill(PFTauEta);
      if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_TauVeto_Isolation4->Fill(PFTauPhi);
    }
  }

  fDist->histos_raw.l1JetEt->Fill(L1JetEt);
  if(pass_l1et) fDist->histos.l1JetEt->Fill(L1JetEt);
  if(tau2) {
    fDist->histos_raw.l1TauJetEt->Fill(L1JetEt);
    if(pass_l1et) fDist->histos.l1TauJetEt->Fill(L1JetEt);
  }
  else {
    fDist->histos_raw.l1CenJetEt->Fill(L1JetEt);
    if(pass_l1et) fDist->histos.l1CenJetEt->Fill(L1JetEt);
  }

  // Resolution
  float resolution = (L1JetEt - PFTauEt) / PFTauEt;
  fDist->histos_raw.l1JetEtResolution->Fill(resolution);
  fDist->histos_raw.l1JetEtResolution_PFTauEt->Fill(PFTauEt, TMath::Abs(resolution));
  if(pass_l1et) {
    fDist->histos.l1JetEtResolution->Fill(resolution);
    fDist->histos.l1JetEtResolution_PFTauEt->Fill(PFTauEt, TMath::Abs(resolution));
  }

  // Tau and Central jet turn-ons
  if(tau2 && L1JetEt > tauCut) {
    if(pass_eta) fDataPt->passed_L1Jet_Tau_EtCut2->Fill(ptvar);
    if(pass_pt)  fDataEta->passed_L1Jet_Tau_EtCut2->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_Tau_EtCut2->Fill(PFTauPhi);
  }
  if(hasMatchedL1TauJet && L1JetEt > tauCut) {
    if(pass_eta) fDataPt->passed_L1TauJet_EtCut->Fill(ptvar);
    if(pass_pt)  fDataEta->passed_L1TauJet_EtCut->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1TauJet_EtCut->Fill(PFTauPhi);
  }
  if(!tau2 && L1JetEt > cenCut) {
    if(pass_eta) fDataPt->passed_L1Jet_Cen_EtCut2->Fill(ptvar);
    if(pass_pt)  fDataEta->passed_L1Jet_Cen_EtCut2->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_Cen_EtCut2->Fill(PFTauPhi);
  }

  // L1 overall efficiency
  if( (tau && L1JetEt > tauCut) || (!tau && L1JetEt > cenCut) ) {
    if(pass_eta) fDataPt->passed_L1Jet_EtCut->Fill(ptvar);
    if(pass_pt)  fDataEta->passed_L1Jet_EtCut->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_EtCut->Fill(PFTauPhi);
  }
  if( (tau2 && L1JetEt > tauCut) || (!tau2 && L1JetEt > cenCut) ) {
    if(pass_eta) fDataPt->passed_L1Jet_EtCut2->Fill(ptvar);
    if(pass_pt)  fDataEta->passed_L1Jet_EtCut2->Fill(PFTauEta);
    if(pass_eta && pass_pt) fDataPhi->passed_L1Jet_EtCut2->Fill(PFTauPhi);
  }

  return kTRUE;
}

void TTEffTree::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

  fDist->number = new NumberData(allEntries, selectedEntries, selectedL1JetEntries, selectedL1JetEtEntries);
  fOutput->AddLast(fDataPt);
  fOutput->AddLast(fDataEta);
  fOutput->AddLast(fDataPhi);
  fOutput->AddLast(fDist);
  fDataPt = 0;
  fDataEta = 0;
  fDataPhi = 0;
  allEntries = 0;
  selectedEntries = 0;

}

void TTEffTree::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}


struct Drawer {
  explicit Drawer(TString pd):
    fMin(0), fMax(0), c(0), frame(0), plotDir(pd) {
    // Returns true if path does NOT exist!
    if(gSystem->AccessPathName(plotDir.Data()))
      gSystem->mkdir(plotDir.Data());
  }

  void setMinMax(float xmin, float xmax) {
    fMin = xmin;
    fMax = xmax;
  }

  void addFormat(const std::string& format) {
    formats.push_back(format);
  }

  void createCanvas(const TH1 *h1, const TH1 *h2, const TH1 *h3=0) const {
    c = new TCanvas();
    int h1maxbin = h1->GetMaximumBin();
    const TAxis *axis = h1->GetXaxis();
    Double_t ymax = std::max(h1->GetBinContent(h1maxbin)+h1->GetBinError(h1maxbin), h2->GetMaximum());
    if(h3)
      ymax = std::max(ymax, h3->GetMaximum());
    /*
    // Hack for testing DrawNormalized
    if(strcmp(h1->GetName(), "_dist_h4") == 0)
      ymax = 0.3;
    if(strcmp(h1->GetName(), "raw_dist_h4") == 0)
      ymax = 0.2;
    */

    frame = c->DrawFrame(axis->GetXmin(), 0.,
                         axis->GetXmax(), 1.1*ymax);
    frame->GetXaxis()->SetTitle(h1->GetXaxis()->GetTitle());
    frame->GetYaxis()->SetTitle(h1->GetYaxis()->GetTitle());
  }

  void createCanvas(const TGraph *gr) const {
    c = new TCanvas();
    frame = c->DrawFrame(fMin, 0, fMax, 1.1);
    if(gr) {
      frame->GetXaxis()->SetTitle(gr->GetXaxis()->GetTitle());
      frame->GetYaxis()->SetTitle(gr->GetYaxis()->GetTitle());
    }
  }

  void draw(const char *fname,
            const Plot<TH1>& p1, const char *legend1,
            const Plot<TH1>& p2, const char *legend2) const {
    TLegend *leg = p2.createLegend();
    p1->SetMarkerColor(kBlack);
    p2->SetLineColor(kRed);
    p2->SetFillColor(kRed);
    p2->SetFillStyle(3005);
    leg->AddEntry(p1.getPlot(), legend1, "p");
    leg->AddEntry(p2.getPlot(), legend2, "f");
    //p2->DrawNormalized("same", 1);
    //p1->DrawNormalized("P same", 1);
    p2->Draw("same");
    p1->Draw("EP same");
    leg->Draw();
    p2.drawMetadata();
    save(fname);
  }

  void draw(const char *fname,
            const Plot<TH1>& p1, const char *legend1,
            const Plot<TH1>& p2, const char *legend2,
            const Plot<TH1>& p3, const char *legend3) const {
    createCanvas(p1.getPlot(), p2.getPlot(), p3.getPlot());
    TLegend *leg = p3.createLegend();
    p1->SetMarkerColor(kBlack);
    p2->SetLineColor(kRed);
    p2->SetFillColor(kRed);
    p2->SetFillStyle(3005);
    p3->SetLineColor(kBlue-7);
    p3->SetFillColor(kBlue-7);
    p3->SetFillStyle(3006);
    leg->AddEntry(p1.getPlot(), legend1, "p");
    leg->AddEntry(p2.getPlot(), legend2, "f");
    leg->AddEntry(p3.getPlot(), legend3, "f");
    //p2->DrawNormalized("same", 1);
    //p3->DrawNormalized("same", 1);
    //p1->DrawNormalized("P same");
    p2->Draw("same");
    p3->Draw("same");
    p1->Draw("EP same");
    leg->Draw();
    p3.drawMetadata();
    save(fname);
  }

  void draw(const char *fname,
            const Plot<TProfile>& p1, const char *legend1,
            const Plot<TProfile>& p2, const char *legend2) const {
    createCanvas(p1.getPlot(), p2.getPlot());
    TLegend *leg = p2.createLegend();
    p1->SetMarkerColor(kBlack);
    p2->SetMarkerColor(kRed);
    leg->AddEntry(p1.getPlot(), legend1, "p");
    leg->AddEntry(p2.getPlot(), legend2, "p");
    p2->Draw("PE1 same");
    p1->Draw("PE1 same");
    leg->Draw();
    p2.drawMetadata();
    save(fname);
  }

  void draw(const char *fname,
            const Plot<TProfile>& p1, const char *legend1,
            const Plot<TProfile>& p2, const char *legend2,
            const Plot<TProfile>& p3, const char *legend3) const {
    createCanvas(p1.getPlot(), p2.getPlot(), p3.getPlot());
    TLegend *leg = p3.createLegend();
    p1->SetMarkerColor(kBlack);
    p2->SetMarkerColor(kRed);
    p3->SetMarkerColor(kBlue-7);
    leg->AddEntry(p1.getPlot(), legend1, "p");
    leg->AddEntry(p2.getPlot(), legend2, "p");
    leg->AddEntry(p3.getPlot(), legend3, "p");
    p2->Draw("PE1 same");
    p3->Draw("PE1 same");
    p1->Draw("PE1 same");
    leg->Draw();
    p3.drawMetadata();
    save(fname);
  }

  void draw(const char *fname, const Plot<TGraphAsymmErrors>& p1, const char *legend1=0) const {
    createCanvas(p1.getPlot());
    p1->SetMarkerColor(kBlack);
    p1->Draw("P same");
    if(legend1) {
      TLegend *leg = p1.createLegend();
      leg->AddEntry(p1.getPlot(), legend1, "p");
      leg->Draw();
    }
    p1.drawMetadata();
    save(fname);
  }

  void draw(const char *fname,
            const Plot<TGraphAsymmErrors>& p1, const char *legend1,
            const Plot<TGraphAsymmErrors>& p2, const char *legend2) const {
    createCanvas(p1.getPlot());
    TLegend *leg = p2.createLegend();
    p1->SetMarkerColor(kBlack);
    p2->SetMarkerColor(kRed);
    leg->AddEntry(p1.getPlot(), legend1, "p");
    leg->AddEntry(p2.getPlot(), legend2, "p");
    p1->Draw("P same");
    p2->Draw("P same");
    leg->Draw();
    p2.drawMetadata();
    save(fname);
  }

  void drawMC(const char *fname,
              const Plot<TGraphAsymmErrors>& p1, const char *legend1,
              const Plot<TGraphAsymmErrors>& p2, const char *legend2) const {
    createCanvas(p1.getPlot());
    TLegend *leg = p2.createLegend();
    p1->SetMarkerColor(kBlack);
    p1->SetMarkerSize(0.75);
    p2->SetMarkerColor(kRed);
    leg->AddEntry(p1.getPlot(), legend1, "p");
    leg->AddEntry(p2.getPlot(), legend2, "p");
    p1->Draw("P same");
    p2->Draw("P same");
    p1->Draw("P same");
    leg->Draw();
    p2.drawMetadata();
    save(fname);
  }


  void draw(const char *fname,
            const Plot<TGraphAsymmErrors>& p1, const char *legend1,
            const Plot<TGraphAsymmErrors>& p2, const char *legend2,
            const Plot<TGraphAsymmErrors>& p3, const char *legend3) const {
    createCanvas(p1.getPlot());
    TLegend *leg = p3.createLegend();
    p1->SetMarkerColor(kBlack);
    p2->SetMarkerColor(kRed);
    p3->SetMarkerColor(kBlue-7);
    leg->AddEntry(p1.getPlot(), legend1, "p");
    leg->AddEntry(p2.getPlot(), legend2, "p");
    leg->AddEntry(p3.getPlot(), legend3, "p");
    p2->Draw("P same");
    p3->Draw("P same");
    p1->Draw("P same");
    leg->Draw();
    p3.drawMetadata();
    save(fname);
  }

  void save(const char *fname) const {
    for(size_t i=0; i<formats.size(); ++i) {
      c->SaveAs(plotDir + Form("/%s%s", fname, formats[i].c_str()));
    }
  }

  float fMin;
  float fMax;
  mutable TCanvas *c;
  mutable TH1 *frame;
  TString plotDir;
  std::vector<std::string> formats;
};

void draw_DistDataHistos(const char *prefix, const Drawer& draw,
                         DistData::Histos *d1, DistData::Histos *d2,
                         const std::string& label1, const std::string& label2,
                         const std::string& lumi) {
  d1->forEach(PlotLegend(0.65, 0.6, 0.25, 0.14));
  d1->forEach(PlotMetaPos(0.55, 0.89));
  d1->forEach(PlotLumi(lumi));

  d1->hasMatchedL1Jet.setLegendPos(0.2, 0.6).setMetaPos(0.2, 0.89);
  d1->l1IsolationRegions2.takeConf(d1->hasMatchedL1Jet);

  d2->takeConf(*d1);

  draw.draw(Form("%s_hasMatchedL1Jet", prefix), d1->hasMatchedL1Jet, label1.c_str(), d2->hasMatchedL1Jet, label2.c_str());
  draw.draw(Form("%s_L1TauVeto", prefix), d1->l1TauVeto, label1.c_str(), d2->l1TauVeto, label2.c_str());
  draw.draw(Form("%s_L1IsolationRegions2", prefix), d1->l1IsolationRegions2, label1.c_str(), d2->l1IsolationRegions2, label2.c_str());
  draw.draw(Form("%s_L1JetEt", prefix), d1->l1JetEt, label1.c_str(), d2->l1JetEt, label2.c_str());
  draw.draw(Form("%s_L1TauJetEt", prefix), d1->l1TauJetEt, label1.c_str(), d2->l1TauJetEt, label2.c_str());
  draw.draw(Form("%s_L1CenJetEt", prefix), d1->l1CenJetEt, label1.c_str(), d2->l1CenJetEt, label2.c_str());
  draw.draw(Form("%s_L1JetEtResolution", prefix), d1->l1JetEtResolution, label1.c_str(), d2->l1JetEtResolution, label2.c_str());

  Plot<TProfile> res_p1(dynamic_cast<TProfile *>(d1->l1JetEtResolution_PFTauEt->ProfileX()->Clone("res_p1")), lumi);
  Plot<TProfile> res_p2(dynamic_cast<TProfile *>(d2->l1JetEtResolution_PFTauEt->ProfileX()->Clone("res_p2")), lumi);
  res_p1->GetYaxis()->SetTitle(d1->l1JetEtResolution_PFTauEt->GetYaxis()->GetTitle());
  res_p2->GetYaxis()->SetTitle(d2->l1JetEtResolution_PFTauEt->GetYaxis()->GetTitle());
  res_p1.takeConf(d1->l1TauVeto);
  res_p1.setLegendPos(0.2, 0.2);
  res_p2.takeConf(res_p1);
  draw.draw(Form("%s_L1JetEtResolution_PFTauEt", prefix), res_p1, label1.c_str(), res_p2, label2.c_str());
}

void draw_DistDataHistos(const char *prefix, const Drawer& draw,
                         DistData::Histos *d1, DistData::Histos *d2, DistData::Histos *d3,
                         const std::string& label1, const std::string& label2, const std::string& label3,
                         const std::string& lumi) {
  d1->forEach(PlotLegend(0.65, 0.6, 0.25, 0.14));
  d1->forEach(PlotMetaPos(0.55, 0.89));
  d1->forEach(PlotLumi(lumi));

  d1->hasMatchedL1Jet.setLegendPos(0.2, 0.6).setMetaPos(0.2, 0.89);
  d1->l1IsolationRegions2.takeConf(d1->hasMatchedL1Jet);

  d2->takeConf(*d1);
  d3->takeConf(*d1);

  draw.draw(Form("%s_hasMatchedL1Jet", prefix), d1->hasMatchedL1Jet, label1.c_str(), d2->hasMatchedL1Jet, label2.c_str(), d3->hasMatchedL1Jet, label3.c_str());
  draw.draw(Form("%s_L1TauVeto", prefix), d1->l1TauVeto, label1.c_str(), d2->l1TauVeto, label2.c_str(), d3->l1TauVeto, label3.c_str());
  draw.draw(Form("%s_L1IsolationRegions2", prefix), d1->l1IsolationRegions2, label1.c_str(), d2->l1IsolationRegions2, label2.c_str(), d3->l1IsolationRegions2, label3.c_str());
  draw.draw(Form("%s_L1JetEt", prefix), d1->l1JetEt, label1.c_str(), d2->l1JetEt, label2.c_str(), d3->l1JetEt, label3.c_str());
  draw.draw(Form("%s_L1TauJetEt", prefix), d1->l1TauJetEt, label1.c_str(), d2->l1TauJetEt, label2.c_str(), d3->l1TauJetEt, label3.c_str());
  draw.draw(Form("%s_L1CenJetEt", prefix), d1->l1CenJetEt, label1.c_str(), d2->l1CenJetEt, label2.c_str(), d3->l1CenJetEt, label3.c_str());
  draw.draw(Form("%s_L1JetEtResolution", prefix), d1->l1JetEtResolution, label1.c_str(), d2->l1JetEtResolution, label2.c_str(), d3->l1JetEtResolution, label3.c_str());

  Plot<TProfile> res_p1(dynamic_cast<TProfile *>(d1->l1JetEtResolution_PFTauEt->ProfileX()->Clone("res_p1")), lumi);
  Plot<TProfile> res_p2(dynamic_cast<TProfile *>(d2->l1JetEtResolution_PFTauEt->ProfileX()->Clone("res_p2")), lumi);
  Plot<TProfile> res_p3(dynamic_cast<TProfile *>(d3->l1JetEtResolution_PFTauEt->ProfileX()->Clone("res_p3")), lumi);
  res_p1->GetYaxis()->SetTitle(d1->l1JetEtResolution_PFTauEt->GetYaxis()->GetTitle());
  res_p2->GetYaxis()->SetTitle(d2->l1JetEtResolution_PFTauEt->GetYaxis()->GetTitle());
  res_p3->GetYaxis()->SetTitle(d3->l1JetEtResolution_PFTauEt->GetYaxis()->GetTitle());
  res_p1.takeConf(d1->l1TauVeto);
  res_p1.setLegendPos(0.2, 0.2);
  res_p2.takeConf(res_p1);
  res_p3.takeConf(res_p1);
  draw.draw(Form("%s_L1JetEtResolution_PFTauEt", prefix), res_p1, label1.c_str(), res_p2, label2.c_str(), res_p3, label3.c_str());
}

void draw_DistData(const char *prefix, const Drawer& draw,
                   DistData *d1, DistData *d2,
                   const std::string& label1, const std::string& label2,
                   const std::string& lumi) {
  draw_DistDataHistos(prefix, draw, &(d1->histos), &(d2->histos), label1, label2, lumi);
  std::string tmp(prefix);
  tmp += "_raw";
  draw_DistDataHistos(tmp.c_str(), draw, &(d1->histos_raw), &(d2->histos_raw), label1, label2, lumi);
}

void draw_DistData(const char *prefix, const Drawer& draw,
                   DistData *d1, DistData *d2, DistData *d3,
                   const std::string& label1, const std::string& label2, const std::string& label3,
                   const std::string& lumi) {
  draw_DistDataHistos(prefix, draw, &(d1->histos), &(d2->histos), &(d3->histos), label1, label2, label3, lumi);
  std::string tmp(prefix);
  tmp += "_raw";
  draw_DistDataHistos(tmp.c_str(), draw, &(d1->histos_raw), &(d2->histos_raw), label1, label2, lumi);
  draw_DistDataHistos(tmp.c_str(), draw, &(d1->histos_raw), &(d2->histos_raw), &(d3->histos_raw), label1, label2, label3, lumi);
}

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
    void drawStats(const TGraph *graph, TF1 *func, TLatex& l, float x, float y) const {
      const TList *lst = graph->GetListOfFunctions();
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

  void fit(const char *fname, const Plot<TGraphAsymmErrors>& p, Function func, const char *xvar, float min, float max,
           const char *legend=0) const {
    draw.draw(fname, p, legend);

    FitFunc *ff = getFunction(func);
    TF1 *myfit = ff->function(min, max);
    p->Fit("myfit", "QR+");
    myfit->Draw("same");
    
    float x = 0.45;
    float y = 0.58;
    l.DrawLatex(x, y, ff->functionName(xvar)); y -= 0.07;
    l.DrawLatex(x, y, Form("Fit from %.1f to %.1f", min, max)); y-=0.07;
    gPad->Update();
    ff->drawStats(p.getPlot(), myfit, l, x, y);

    draw.save(fname);
  }

  const Drawer& draw;
  mutable TLatex l;
};

struct Efficiency {
  typedef Plot<TGraphAsymmErrors> Pl;

  Efficiency(Drawer& dr, const Data *data, const std::string& lumi):
    d(dr),
    L1Jet(create(data->passed_L1Jet, data->total, data->xlabel), lumi),
    L1TauJet(create(data->passed_L1TauJet, data->total, data->xlabel), lumi),
    L1CenJet(create(data->passed_L1CenJet, data->total, data->xlabel), lumi),
    L1TauJet_TauVeto_plus_Isolation(create(data->passed_L1TauJet, data->passed_L1Jet, data->xlabel), lumi),
    L1TauJet_EtCut(create(data->passed_L1TauJet_EtCut, data->total, data->xlabel), lumi),

    L1Jet_TauVeto(create(data->passed_L1Jet_TauVeto, data->passed_L1Jet, data->xlabel), lumi),
    L1Jet_TauVeto_Isolation(create(data->passed_L1Jet_TauVeto_Isolation,  data->passed_L1Jet_TauVeto, data->xlabel), lumi),
    L1Jet_TauVeto_Isolation2(create(data->passed_L1Jet_TauVeto_Isolation2, data->passed_L1Jet_TauVeto, data->xlabel), lumi),
    L1Jet_TauVeto_Isolation3(create(data->passed_L1Jet_TauVeto_Isolation3, data->passed_L1Jet_TauVeto, data->xlabel), lumi),
    L1Jet_TauVeto_Isolation4(create(data->passed_L1Jet_TauVeto_Isolation4, data->passed_L1Jet_TauVeto, data->xlabel), lumi),
    L1Jet_TauVeto_plus_Isolation(create(data->passed_L1Jet_TauVeto_Isolation, data->passed_L1Jet, data->xlabel), lumi),
    L1Jet_TauVeto_plus_Isolation2(create(data->passed_L1Jet_TauVeto_Isolation2, data->passed_L1Jet, data->xlabel), lumi),

    L1Jet_Tau_EtCut2(create(data->passed_L1Jet_Tau_EtCut2, data->total, data->xlabel), lumi),
    L1Jet_Cen_EtCut2(create(data->passed_L1Jet_Cen_EtCut2, data->total, data->xlabel), lumi),
 
    L1Jet_EtCut(create(data->passed_L1Jet_EtCut, data->passed_L1Jet, data->xlabel), lumi),
    L1Jet_EtCut2(create(data->passed_L1Jet_EtCut2, data->passed_L1Jet, data->xlabel), lumi),

    L1Jet_Overall(create(data->passed_L1Jet_EtCut, data->total, data->xlabel), lumi),
    L1Jet_Overall2(create(data->passed_L1Jet_EtCut2, data->total, data->xlabel), lumi)
  {
    TAxis *axis = data->total->GetXaxis();
    d.setMinMax(axis->GetBinLowEdge(axis->GetFirst()), axis->GetBinUpEdge(axis->GetLast()));
  }

  static TGraphAsymmErrors *create(const TH1 *passed, const TH1 *total, const std::string& xlabel) {
    TGraphAsymmErrors *gr = new TGraphAsymmErrors(passed, total);
    gr->GetYaxis()->SetTitle("L1 Efficiency");
    gr->GetXaxis()->SetTitle(xlabel.c_str());
    //gr->SetMaximum(1.1);
    //gr->SetMinimum(0.0);

    return gr;
  }

  void takeConf(const Efficiency& eff) {
    L1Jet.takeConf(eff.L1Jet);
    L1TauJet.takeConf(eff.L1TauJet);
    L1CenJet.takeConf(eff.L1CenJet);
    L1TauJet_TauVeto_plus_Isolation.takeConf(eff.L1TauJet_TauVeto_plus_Isolation);
    L1TauJet_EtCut.takeConf(eff.L1TauJet_EtCut);

    L1Jet_TauVeto.takeConf(eff.L1Jet_TauVeto);
    L1Jet_TauVeto_Isolation.takeConf(eff.L1Jet_TauVeto_Isolation);
    L1Jet_TauVeto_Isolation2.takeConf(eff.L1Jet_TauVeto_Isolation2);
    L1Jet_TauVeto_Isolation3.takeConf(eff.L1Jet_TauVeto_Isolation3);
    L1Jet_TauVeto_Isolation4.takeConf(eff.L1Jet_TauVeto_Isolation4);
    L1Jet_TauVeto_plus_Isolation.takeConf(eff.L1Jet_TauVeto_plus_Isolation);
    L1Jet_TauVeto_plus_Isolation2.takeConf(eff.L1Jet_TauVeto_plus_Isolation2);

    L1Jet_Tau_EtCut2.takeConf(eff.L1Jet_Tau_EtCut2);
    L1Jet_Cen_EtCut2.takeConf(eff.L1Jet_Cen_EtCut2);

    L1Jet_EtCut.takeConf(eff.L1Jet_EtCut);
    L1Jet_EtCut2.takeConf(eff.L1Jet_EtCut2);
    
    L1Jet_Overall.takeConf(eff.L1Jet_Overall);
    L1Jet_Overall2.takeConf(eff.L1Jet_Overall2);
  }

  template <typename F>
  void forEach(F func) {
    func(L1Jet);
    func(L1TauJet);
    func(L1CenJet);
    func(L1TauJet_TauVeto_plus_Isolation);
    func(L1TauJet_EtCut);

    func(L1Jet_TauVeto);
    func(L1Jet_TauVeto_Isolation);
    func(L1Jet_TauVeto_Isolation2);
    func(L1Jet_TauVeto_Isolation3);
    func(L1Jet_TauVeto_Isolation4);
    func(L1Jet_TauVeto_plus_Isolation);
    func(L1Jet_TauVeto_plus_Isolation2);

    func(L1Jet_Tau_EtCut2);
    func(L1Jet_Cen_EtCut2);

    func(L1Jet_EtCut);
    func(L1Jet_EtCut2);

    func(L1Jet_Overall);
    func(L1Jet_Overall2);
  }

  void drawL1Jet_TauCen_EtCut(const char *prefix) {
    d.drawMC(Form("%s_L1Jet_TauCen_EtCut2", prefix), L1Jet_Tau_EtCut2, "L1 tau jet", L1Jet_Cen_EtCut2, "L1 central jet");
  }

  void drawL1Jet_TauVeto_plus_Isolation(const char *prefix) {
    d.draw(Form("%s_L1Jet_TauVeto_plus_Isolation",  prefix), L1TauJet_TauVeto_plus_Isolation, "L1 Tau jet", L1Jet_TauVeto_plus_Isolation, "TauVeto + Isolation (1 GeV)");
    d.draw(Form("%s_L1Jet_TauVeto_plus_Isolation2", prefix), L1TauJet_TauVeto_plus_Isolation, "L1 Tau jet", L1Jet_TauVeto_plus_Isolation2, "TauVeto + Isolation (2 GeV)");
  }

  void draw(const char *prefix) {
    d.draw(Form("%s_L1Jet", prefix), L1Jet);

    d.draw(Form("%s_L1Jet_Cen_Tau", prefix), L1Jet, "L1 jet", L1TauJet, "L1 tau jet", L1CenJet, "L1 central jet");

    d.draw(Form("%s_L1Jet_TauVeto", prefix), L1Jet_TauVeto);

    d.draw(Form("%s_L1Jet_TauVeto_Isolation", prefix), L1Jet_TauVeto_Isolation, "1 GeV isolation", L1Jet_TauVeto_Isolation2, "2 GeV isolation");

    //d.draw(Form("%s_L1Jet_Cen_EtCut2", prefix), L1Jet_Cen_EtCut2);

    //d.draw(Form("%s_L1Jet_Tau_EtCut2", prefix), L1Jet_Tau_EtCut2);
    d.drawMC(Form("%s_L1Jet_TauJet_EtCut", prefix), L1Jet_Tau_EtCut2, "Custom L1 tau", L1TauJet_EtCut, "L1 tau jet");

    //d.draw(Form("%s_L1Jet_EtCut", prefix), L1Jet_EtCut, "1 GeV isolation", L1Jet_EtCut2, "2 GeV isolation");

    //d.draw(Form("%s_L1Jet_Overall2", prefix), L1Jet_Overall2);
    d.draw(Form("%s_L1Jet_Overall", prefix), L1Jet_Overall, "1 GeV isolation", L1Jet_Overall2, "2 GeV isolation");

    drawL1Jet_TauCen_EtCut(prefix);
    drawL1Jet_TauVeto_plus_Isolation(prefix);
  }

  void drawCmp(const char *prefix, const Efficiency& cmp, const std::string& thisLabel, const std::string& cmpLabel) const {
    d.drawMC(Form("%s_L1Jet", prefix), L1Jet, thisLabel.c_str(), cmp.L1Jet, cmpLabel.c_str());
    
    d.drawMC(Form("%s_L1Jet_TauVeto", prefix), L1Jet_TauVeto, thisLabel.c_str(), cmp.L1Jet_TauVeto, cmpLabel.c_str());

    //d.drawMC(Form("%s_L1Jet_TauVeto_Isolation", prefix), L1Jet_TauVeto_Isolation, thisLabel.c_str(), cmp.L1Jet_TauVeto_Isolation, cmpLabel.c_str());
    d.drawMC(Form("%s_L1Jet_TauVeto_Isolation2", prefix), L1Jet_TauVeto_Isolation2, thisLabel.c_str(), cmp.L1Jet_TauVeto_Isolation2, cmpLabel.c_str());

    d.drawMC(Form("%s_L1Jet_Tau_EtCut2", prefix), L1Jet_Tau_EtCut2, thisLabel.c_str(), cmp.L1Jet_Tau_EtCut2, cmpLabel.c_str());
    d.drawMC(Form("%s_L1Jet_Cen_EtCut2", prefix), L1Jet_Cen_EtCut2, thisLabel.c_str(), cmp.L1Jet_Cen_EtCut2, cmpLabel.c_str());

    //d.drawMC(Form("%s_L1Jet_EtCut", prefix), L1Jet_EtCut, thisLabel.c_str(), cmp.L1Jet_EtCut, cmpLabel.c_str());
    //d.drawMC(Form("%s_L1Jet_EtCut2", prefix), L1Jet_EtCut2, thisLabel.c_str(), cmp.L1Jet_EtCut2, cmpLabel.c_str());

    d.drawMC(Form("%s_L1Jet_Overall", prefix), L1Jet_Overall, thisLabel.c_str(), cmp.L1Jet_Overall, cmpLabel.c_str());
    d.drawMC(Form("%s_L1Jet_Overall2", prefix), L1Jet_Overall2, thisLabel.c_str(), cmp.L1Jet_Overall2, cmpLabel.c_str());
  }

  void drawCmp(const char *prefix, const Efficiency& cmp, const Efficiency& cmp2, const std::string& thisLabel, const std::string& cmpLabel, const std::string& cmp2Label) const {
    d.draw(Form("%s_L1Jet", prefix), L1Jet, thisLabel.c_str(), cmp.L1Jet, cmpLabel.c_str(), cmp2.L1Jet, cmp2Label.c_str());
    
    d.draw(Form("%s_L1Jet_TauVeto", prefix), L1Jet_TauVeto, thisLabel.c_str(), cmp.L1Jet_TauVeto, cmpLabel.c_str(), cmp2.L1Jet_TauVeto, cmp2Label.c_str());

    //d.draw(Form("%s_L1Jet_TauVeto_Isolation", prefix), L1Jet_TauVeto_Isolation, thisLabel.c_str(), cmp.L1Jet_TauVeto_Isolation, cmpLabel.c_str(), cmp2.L1Jet_TauVeto_Isolation, cmp2Label.c_str());
    d.draw(Form("%s_L1Jet_TauVeto_Isolation2", prefix), L1Jet_TauVeto_Isolation2, thisLabel.c_str(), cmp.L1Jet_TauVeto_Isolation2, cmpLabel.c_str(), cmp2.L1Jet_TauVeto_Isolation2, cmp2Label.c_str());

    d.draw(Form("%s_L1Jet_Tau_EtCut2", prefix), L1Jet_Tau_EtCut2, thisLabel.c_str(), cmp.L1Jet_Tau_EtCut2, cmpLabel.c_str(), cmp2.L1Jet_Tau_EtCut2, cmp2Label.c_str());
    d.draw(Form("%s_L1Jet_Cen_EtCut2", prefix), L1Jet_Cen_EtCut2, thisLabel.c_str(), cmp.L1Jet_Cen_EtCut2, cmpLabel.c_str(), cmp2.L1Jet_Cen_EtCut2, cmp2Label.c_str());

    //d.draw(Form("%s_L1Jet_EtCut", prefix), L1Jet_EtCut, thisLabel.c_str(), cmp.L1Jet_EtCut, cmpLabel.c_str(), cmp2.L1Jet_EtCut, cmp2Label.c_str());
    //d.draw(Form("%s_L1Jet_EtCut2", prefix), L1Jet_EtCut2, thisLabel.c_str(), cmp.L1Jet_EtCut2, cmpLabel.c_str(), cmp2.L1Jet_EtCut2, cmp2Label.c_str());

    d.draw(Form("%s_L1Jet_Overall", prefix), L1Jet_Overall, thisLabel.c_str(), cmp.L1Jet_Overall, cmpLabel.c_str(), cmp2.L1Jet_Overall, cmp2Label.c_str());
    d.draw(Form("%s_L1Jet_Overall2", prefix), L1Jet_Overall2, thisLabel.c_str(), cmp.L1Jet_Overall2, cmpLabel.c_str(), cmp2.L1Jet_Overall2, cmp2Label.c_str());
  }

  Drawer& d;

  Pl L1Jet;
  Pl L1TauJet;
  Pl L1CenJet;
  Pl L1TauJet_TauVeto_plus_Isolation;
  Pl L1TauJet_EtCut;

  Pl L1Jet_TauVeto;
  Pl L1Jet_TauVeto_Isolation;
  Pl L1Jet_TauVeto_Isolation2;
  Pl L1Jet_TauVeto_Isolation3;
  Pl L1Jet_TauVeto_Isolation4;
  Pl L1Jet_TauVeto_plus_Isolation;
  Pl L1Jet_TauVeto_plus_Isolation2;

  Pl L1Jet_Tau_EtCut2;
  Pl L1Jet_Cen_EtCut2;

  Pl L1Jet_EtCut;
  Pl L1Jet_EtCut2;

  Pl L1Jet_Overall;
  Pl L1Jet_Overall2;
};

void dummyFunc(TTEffTree *analyser) {
}

struct SetCuts {
  SetCuts(float tauCut, float cenCut):
    fTauCut(tauCut), fCenCut(cenCut) {}

  void operator()(TTEffTree *analyser) const {
    analyser->setTauCut(fTauCut);
    analyser->setCenCut(fCenCut);
  }
  
  float fTauCut;
  float fCenCut;
};

template <typename T>
TList *analyse(const std::string& selection, TChain *chain, TList *inputList, int maxEvents, const T& func) {
  TTEffTree *analyser = new TTEffTree(selection);
  analyser->SetInputList(inputList);

  func(analyser);

  if(maxEvents >= 0)
    chain->Process(analyser, "", maxEvents);
  else {
    chain->Process(analyser);
  }

  return analyser->GetOutputList();
}

TList *analyse(const std::string& selection, TChain *chain, TList *inputList, int maxEvents=-1) {
  return analyse(selection, chain, inputList, maxEvents, dummyFunc);
}

void plotL1Efficiency() {
  setTDRStyle();

  Drawer draw("l1plots");
  draw.addFormat(".png");
  draw.addFormat(".eps");
  draw.addFormat(".C");
  //draw.addFormat(".svg");

  Fitter fit(draw);

  //float bins[] = {0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 15., 25.}; 
  //float bins[] = {0., 1., 2., 3., 4., 6., 10., 25.};
  //float bins[] = {0., 1., 2., 3., 4., 5., 6., 8., 10., 15.};  //float bins[] = {0., 2., 4., 6., 8., 10., 12., 14., 16., 18., 20.,
  //                25., 30., 40, 60., 80., 100.};
  float bins[] = {//0., 2., 4., 6., 8., 10., 12., 14., 16., 18., 20.
                  //, 25., 30., 35., 40, 45., 50., 55., 60., 65., 70., 75., 80.
                  10., 15., 17.5, 20.,
                  25., 30., 35., 40., 50., 60., 70., 80., 100., 105.
                  //, 85., 90., 95., 100.
                  //, 105., 110., 115., 120.
                  //, 125., 150.
                  
  };
  size_t nbins = sizeof(bins)/sizeof(float)-1;

  TH1F *histo_pt = new TH1F("foo1", "foo", 19, 10., 105.); // 5 GeV/c bins
  //TH1F *histo_pt = new TH1F("foo1", "foo", 47, 10., 104.); // 2 GeV/c bins
  //TH1F *histo_pt = new TH1F("foo1", "foo", 16, 0., 80.);
  //TH1F *histo_pt = new TH1F("foo1", "foo", nbins, bins);
  TH1F *histo_eta = new TH1F("foo2", "foo", 30, -3, 3.);
  TH1F *histo_phi = new TH1F("foo3", "foo", 35, -3.5, 3.5);

  TList *inputList = new TList();
  
  std::string ptlabel ("PF-#tau E_{T} (GeV)"); inputList->Add(new Data(histo_pt, "pt", ptlabel)); 
  //std::string ptlabel ("PF-#tau p_{T} (GeV/c)"); inputList->Add(new Data(histo_pt, "pt", ptlabel));
  inputList->Add(new Data(histo_eta, "eta", "PF-#tau #eta"));
  inputList->Add(new Data(histo_phi, "phi", "PF-#tau #phi"));

  //TString path("/home/mjkortel/data/tautrigger/");
  TString path("/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/");
  //TString path("/tmp/mkortela/");

  std::string lumi = "";

  TChain *chain = new TChain("TTEffTree");
  // Skim preproduction ntuple
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10-PromptReco_preproduction_CS_Tau-v1_RAW-RECO_tagV00-06-04.root");
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10-PromptReco_preproduction_CS_Tau-v2_RAW-RECO_tagV00-06-04.root");

  // Latest GOODCOLL ntuple
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132440-132716_tagV00-06-04.root");
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132717-133511_tagV00-06-04.root");
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132717-133511_tagV00-06-07.root");
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132440-135175_tagV00-06-07.root"); std::string label("V00-06-07");
  //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132440-135175_tagV00-06-08.root"); std::string label("MinBias (V00-06-08)");
  //chain->Add(path + "tteffAnalysis-pftau_JetMETTau_Run2010A-CS_Tau-v2_RAW-RECO_tagV00-06-08.root");     std::string label("CS_Tau (V00-06-08)");
  //chain->Add(path + "tteffAnalysis-pftau.root"); std::string label("MinBias (V00-06-08"); lumi = "L_{int}=1.92 nb^{-1}";
  //chain->Add(path + "tteffAnalysis-pftau_cstau.root"); std::string label("CS_Tau (V00-06-10)"); lumi = "L_{int}=6.91 nb^{-1}";
  //chain->Add(path + "test-pftau.root"); std::string label("Test");

  chain->Add(path + "tteffAnalysis-pftau_JetMETTau_Run2010A-CS_Tau-v2_RAW-RECO_tagV00-06-08.root");     std::string label("JetMET (Jun 7)");
  //chain->Add(path + "tteffAnalysis-pftau_cstau.root"); std::string label("JetMET (Jun 22)"); lumi = "L_{int}=6.91 nb^{-1}";

  std::string pfTauSelection("PFTauEt > 15 && abs(PFTauEta) < 2.4 && PFTauIsoSum < 1. && (PFTauProng == 1 || PFTauProng == 3) && PFTauInvPt < 1/5. && PFMuonMatch == 1");
  std::string runSelection("run >= 132658");

  bool prev=false;
  //prev=true;

  bool mc=false;
  //mc=true;

  bool mc2=false;
  //mc2=true;

  bool ploteta=false;
  bool plotphi=false;
  //ploteta=true;
  //plotphi=true;

  TStopwatch clock;
  clock.Start();

  TList *list = analyse(runSelection + "&&" + pfTauSelection , chain, inputList);
  Data *data_pt = dynamic_cast<Data *>(list->FindObject("data_pt"));
  Data *data_eta = dynamic_cast<Data *>(list->FindObject("data_eta"));
  Data *data_phi = dynamic_cast<Data *>(list->FindObject("data_phi"));
  DistData *data_dist = dynamic_cast<DistData *>(list->FindObject("dist"));

  Efficiency eff_pt(draw, data_pt, lumi);
  eff_pt.L1Jet.setMetaPos(0.55, 0.4);
  eff_pt.L1CenJet.setLegend(0.7, 0.6, 0.2, 0.14).setMetaDir(Metadata::kRight);
  eff_pt.L1Jet_TauVeto.setLegendPos(0.17, 0.2).setMetaPos(0.2, 0.4);
  eff_pt.L1Jet_TauVeto_Isolation.setLegend(0.17, 0.2, 0.25, 0.14).setMetaPos(0.6, 0.9);
  eff_pt.L1Jet_TauVeto_Isolation2.takeConf(eff_pt.L1Jet_TauVeto_Isolation);
  eff_pt.L1Jet_TauVeto_Isolation3.takeConf(eff_pt.L1Jet_TauVeto_Isolation);
  eff_pt.L1Jet_TauVeto_Isolation4.takeConf(eff_pt.L1Jet_TauVeto_Isolation);
  eff_pt.L1Jet_Cen_EtCut2.setLegend(0.2, 0.65, 0.2, 0.11);
  eff_pt.L1Jet_Tau_EtCut2.setLegend(0.25, 0.17, 0.2, 0.14);
  eff_pt.L1TauJet_EtCut.takeConf(eff_pt.L1Jet_Tau_EtCut2);
  eff_pt.L1Jet_EtCut.setLegendWidth(0.25).setLegendHeight(0.14);
  eff_pt.L1Jet_EtCut2.takeConf(eff_pt.L1Jet_EtCut);
  eff_pt.L1Jet_Overall.takeConf(eff_pt.L1Jet_EtCut);
  eff_pt.L1Jet_Overall2.takeConf(eff_pt.L1Jet_Overall);
  eff_pt.L1Jet_TauVeto_plus_Isolation.setLegendWidth(0.25).setMetaDir(Metadata::kRight);
  eff_pt.L1Jet_TauVeto_plus_Isolation2.takeConf(eff_pt.L1Jet_TauVeto_plus_Isolation);
  eff_pt.draw("data_PFTauPt");


  Efficiency eff_eta(draw, data_eta, lumi);
  eff_eta.takeConf(eff_pt);
  eff_eta.L1CenJet.setLegendX(0.4);
  eff_eta.L1Jet_Overall.setLegendX(0.6);
  eff_eta.L1Jet_Overall2.takeConf(eff_eta.L1Jet_Overall);
  if(ploteta) {
    eff_eta.draw("data_PFTauEta");
  }

  Efficiency eff_phi(draw, data_phi, lumi);
  eff_phi.takeConf(eff_eta);
  eff_phi.L1CenJet.setLegendY(0.2);
  if(plotphi) {
    eff_phi.draw("data_PFTauPhi");
  }

  if(prev) {
    inputList = new TList();
    inputList->Add(new Data(histo_pt, "pt", ptlabel));
    inputList->Add(new Data(histo_eta, "eta", "PF-#tau #eta"));
    inputList->Add(new Data(histo_phi, "phi", "PF-#tau #phi"));

    TChain *chain = new TChain("TTEffTree");
    //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132717-133511_tagV00-06-04.root");     std::string prevLabel("V00-06-04");
    chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132440-135175_tagV00-06-07.root");     std::string prevLabel("MinBias (V00-06-07)");
    //chain->Add(path + "tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_runs_132440-135175_tagV00-06-08.root"); std::string prevLabel("MinBias (V00-06-08");
    //chain->Add(path + "tteffAnalysis-pftau_JetMETTau_Run2010A-CS_Tau-v2_RAW-RECO_tagV00-06-08.root");     std::string prevLabel("CS_Tau (V00-06-08)");

    //list = analyse("", chain, inputList, -1, SetCuts(22, 40));
    list = analyse(runSelection + "&&" + pfTauSelection, chain, inputList);
    Data *prev_pt = dynamic_cast<Data *>(list->FindObject("data_pt"));
    Data *prev_eta = dynamic_cast<Data *>(list->FindObject("data_eta"));
    Data *prev_phi = dynamic_cast<Data *>(list->FindObject("data_phi"));
    DistData *prev_dist = dynamic_cast<DistData *>(list->FindObject("dist"));

    PlotLegendHeightWidth modify(0.14, 0.3);

    Efficiency eff_prev_pt(draw, prev_pt, lumi);
    eff_prev_pt.takeConf(eff_pt);
    eff_prev_pt.forEach(modify);
    //eff_prev_pt.drawL1Jet_TauVeto_plus_Isolation("prev_PFTauPt", modify(pm_Isolation).addLegendWidth(0.05));
    eff_pt.drawCmp("data_prev_PFTauPt", eff_prev_pt, label, prevLabel);

    if(ploteta) {
      Efficiency eff_prev_eta(draw, prev_eta, lumi);
      eff_prev_eta.takeConf(eff_eta);
      eff_prev_eta.forEach(modify);
      eff_eta.drawCmp("data_prev_PFTauEta", eff_prev_eta, label, prevLabel);
    }

    if(plotphi) {
      Efficiency eff_prev_phi(draw, prev_phi, lumi);
      eff_prev_phi.takeConf(eff_phi);
      eff_prev_phi.forEach(modify);
      eff_phi.drawCmp("data_prev_PFTauPhi", eff_prev_phi, label, prevLabel);
    }
  }

  if(mc) {
    inputList = new TList();
    inputList->Add(new Data(histo_pt, "pt", ptlabel));
    inputList->Add(new Data(histo_eta, "eta", "PF-#tau #eta"));
    inputList->Add(new Data(histo_phi, "phi", "PF-#tau #phi"));

    path = "/castor/cern.ch/user/e/eluiggi/ttEff/";

    std::string dataLabel("Data "); dataLabel += label;
    //std::string dataLabel = label; leg_width += 0.1;
    //std::string mcLabel("MC");
    std::string mcLabel("Simulation");

    PlotLegendHeightWidth modify(0.14, 0.2);

    chain = new TChain("TTEffTree");
    //chain->Add(path + "tteffAnalyzerMinBiasMC7TeV357-PFTau.root");
    chain->Add(path + "tteffAnalyzerMinBiasMC7TeV357L1Extra-PFTau.root"); mcLabel = "MC MinBias";
    //chain->Add(path + "zttRelValTTEffAnalysis362-pftau.root"); mcLabel = "MC Z#rightarrow#tau#tau";

    list = analyse(pfTauSelection, chain, inputList);
    //list = analyse(pfTauSelection, chain, inputList, -1, SetCuts(22, 40));
    Data *mc_pt = dynamic_cast<Data *>(list->FindObject("data_pt"));
    Data *mc_eta = dynamic_cast<Data *>(list->FindObject("data_eta"));
    Data *mc_phi = dynamic_cast<Data *>(list->FindObject("data_phi"));
    DistData *mc_dist = dynamic_cast<DistData *>(list->FindObject("dist"));
    mc_dist->normalize(data_dist);

    if(!mc2)
      draw_DistData("data_mc", draw, data_dist, mc_dist, dataLabel, mcLabel, lumi);

    Efficiency eff_mc_pt(draw, mc_pt, lumi);
    eff_mc_pt.takeConf(eff_pt);
    eff_mc_pt.forEach(modify);
    eff_mc_pt.L1Jet.setMetaPos(0.4, 0.5);
    eff_mc_pt.L1Jet_TauVeto.setMetaPos(0.2, 0.5);
    eff_mc_pt.L1Jet_TauVeto_Isolation2.setMetaDir(Metadata::kRight).setMetaPos(0.2, 0.89);
    eff_mc_pt.L1Jet_Cen_EtCut2.setLegendPos(0.2, 0.6);
    eff_mc_pt.L1Jet_Overall.setLegendPos(0.3, 0.2).setMetaPos(0.55, 0.4);
    eff_mc_pt.L1Jet_Overall2.takeConf(eff_mc_pt.L1Jet_Overall);

    if(!mc2)
      eff_pt.drawCmp("data_mc_PFTauPt", eff_mc_pt, dataLabel, mcLabel);

    eff_mc_pt.forEach(PlotLumi(""));
    eff_mc_pt.L1Jet_Tau_EtCut2.takeConf(eff_pt.L1Jet_Tau_EtCut2);
    eff_mc_pt.L1Jet_Cen_EtCut2.takeConf(eff_pt.L1Jet_Cen_EtCut2);
    eff_mc_pt.L1Jet_Cen_EtCut2.setLegendPos(0.8, 0.55);
    eff_mc_pt.drawL1Jet_TauCen_EtCut("mc_PFTauPt");
    //eff_mc_pt.drawL1Jet_TauVeto_plus_Isolation("mc_PFTauPt");
    eff_mc_pt.forEach(PlotLumi(lumi));

    Efficiency eff_mc_eta(draw, mc_eta, lumi);
    eff_mc_eta.takeConf(eff_eta);
    eff_mc_eta.forEach(modify);
    eff_mc_eta.L1Jet_Overall.setLegendY(0.8);
    eff_mc_eta.L1Jet_Overall2.takeConf(eff_mc_eta.L1Jet_Overall);
    if(ploteta && !mc2) {
      eff_mc_eta.drawCmp("data_mc_PFTauEta", eff_mc_eta, dataLabel, mcLabel);
    }

    Efficiency eff_mc_phi(draw, mc_phi, lumi);
    eff_mc_phi.takeConf(eff_phi);
    eff_mc_phi.forEach(modify);
    eff_mc_phi.L1Jet_Overall.setLegendY(0.8);
    eff_mc_phi.L1Jet_Overall2.takeConf(eff_mc_phi.L1Jet_Overall);
    if(plotphi && !mc2) {
      eff_mc_phi.drawCmp("data_mc_PFTauPhi", eff_mc_phi, dataLabel, mcLabel);
    }

    if(mc2) {
      inputList = new TList();
      inputList->Add(new Data(histo_pt, "pt", ptlabel));
      inputList->Add(new Data(histo_eta, "eta", "PF-#tau #eta"));
      inputList->Add(new Data(histo_phi, "phi", "PF-#tau #phi"));

      std::string mc2Label("Simulation 2");

      chain = new TChain("TTEffTree");
      //chain->Add(path + "tteffAnalyzerMinBiasMC7TeV357-PFTau.root");
      //chain->Add(path + "tteffAnalyzerMinBiasMC7TeV357L1Extra-PFTau.root"); mc2Label = "MC MinBias";
      chain->Add(path + "zttRelValTTEffAnalysis362-pftau.root"); mc2Label = "MC Z#rightarrow#tau#tau";
      list = analyse(pfTauSelection, chain, inputList);

      Data *mc2_pt = dynamic_cast<Data *>(list->FindObject("data_pt"));
      Data *mc2_eta = dynamic_cast<Data *>(list->FindObject("data_eta"));
      Data *mc2_phi = dynamic_cast<Data *>(list->FindObject("data_phi"));
      DistData *mc2_dist = dynamic_cast<DistData *>(list->FindObject("dist"));
      mc2_dist->normalize(data_dist);

      draw_DistData("data_mc", draw, data_dist, mc_dist, mc2_dist, dataLabel, mcLabel, mc2Label, lumi);

      Efficiency eff_mc2_pt(draw, mc2_pt, lumi);
      eff_mc2_pt.takeConf(eff_mc_pt);
      eff_pt.drawCmp("data_mc_PFTauPt", eff_mc_pt, eff_mc2_pt, dataLabel, mcLabel, mc2Label);

      eff_mc2_pt.forEach(PlotLumi(""));
      eff_mc2_pt.L1Jet_Tau_EtCut2.takeConf(eff_mc_pt.L1Jet_Tau_EtCut2);
      eff_mc2_pt.L1Jet_Cen_EtCut2.takeConf(eff_mc_pt.L1Jet_Cen_EtCut2);
      eff_mc2_pt.L1Jet_Cen_EtCut2.setLegendPos(0.25, 0.25);
      eff_mc2_pt.drawL1Jet_TauCen_EtCut("mc2_PFTauPt");
      //eff_mc2_pt.drawL1Jet_TauVeto_plus_Isolation("mc2_PFTauPt");
      eff_mc2_pt.forEach(PlotLumi(lumi));


      Efficiency eff_mc2_eta(draw, mc2_eta, lumi);
      eff_mc2_eta.takeConf(eff_mc_eta);
      if(ploteta) {
        eff_mc_eta.drawCmp("data_mc_PFTauEta", eff_mc_eta, eff_mc2_eta, dataLabel, mcLabel, mc2Label);
      }

      Efficiency eff_mc2_phi(draw, mc2_phi, lumi);
      eff_mc2_phi.takeConf(eff_phi);
      if(plotphi) {
        eff_mc_phi.drawCmp("data_mc_PFTauPhi", eff_mc_phi, eff_mc2_phi, dataLabel, mcLabel, mc2Label);
      }
    }
  }

  //eff_pt.L1Jet.setMetaPos(0.2, 0.89).setMetaDir(Metadata::kRight);
  //fit.fit("data_fit_PFTauPt_L1Jet", eff_pt.L1Jet, Fitter::kFreq, "p_{T}", 5., 100.);

  clock.Stop();
  clock.Print();

}
