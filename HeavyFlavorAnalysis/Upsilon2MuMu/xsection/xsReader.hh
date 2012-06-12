#ifndef XSREADER_H
#define XSREADER_H

#include <iostream>

#include <TROOT.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>

#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaJet.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/PidTable.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/JSON.hh"

#include "treeReaderXS.hh"


#define DR      57.29577951
#define PIPMASS 0.13957
#define ELMASS  0.000511
#define MUMASS  0.10567


class xsReader: public treeReaderXS {
public:
  xsReader(TChain *tree, TString evtClassName);
  ~xsReader();

  void        bookHist();
  void        startAnalysis();
  void        eventProcessing();
  void        fillHist();
  void        fillCandHist(int mode);
  void        MCstudy();
  void        acceptance();
  void        UpsGun_acceptance();
  void        PathStudy();
  void        GenStudy();
  void        Detectability();
  bool        isPathPreScaled(TString path);
  bool        isPathFired_Match(TString path, TString label);
  bool        isPathFired(TString path);
  void        preSelEff();
  bool        MuIDCheck();
  bool        MomentumCorrection();
  bool        CowboyVeto(TAnaCand *pCand);
  bool        CowboyVeto_gen(TGenCand *genCand);
  void        getBinCenters(TGenCand *gCand, double &pt, double &rapidity);
  void        GetBinCenters(TAnaCand *pCand, double &pt, double &rapidity);
  void        GetBINCenters(TLorentzVector Cand, double &pt, double &rapidity);
  void        AnaEff(TAnaCand *pCand, int mode);
  void        calculateWeights(int mode);
  void        readCuts(TString filename, int dump = 1);
  void        candidateSelection(int mode);
  bool        isMatchedToTrig(TAnaTrack *pTag, TString Label);
  void        x_btest();
  void        TriggerComparisonStudy();
  void        trigEffCheck();
  void        freePointers();
  
  // -- Cut values
  int TYPE;
  int MODE;
  int MUTYPE1;
  int MUTYPE2;
  double CHI2;
  int RESTYPE;
  double PTHI;
  double PTLO;
  double ETAHI;
  double ETALO;
  double ETABARREL;
  double PTBARREL;
  double PTCAND;
  double RAPCAND;
  int BIN;
  double MASSLO;
  double MASSHI;
  int UPSTYPE;
  double DPHI;  
  double DETA;
  int BARREL;
  TString HLTPATH, HLTPATH1, HLTPATH2, HLTPATH3, HLTPATH4, HLTPATH5, HLTPATH6, HLTPATH7, HLTPATH8, HLTPATH9, HLTPATH10, HLTPATH11, HLTPATH12, HLTPATH13;
  TString HLTLABEL, HLTLABEL1, HLTLABEL2, HLTLABEL3;
    
  // -- Variables
  TAnaCand    *fpCand; 
  double      fCandPt, fCandMass, fCandY;
  TGenCand    *fgCand;
  double      fGenCandPt, fGenCandY;
  double      fGenMuon1Pt, fGenMuon1Eta, fMuon1Eta, fMuon1Pt;
  double      fGenMuon2Pt, fGenMuon2Eta, fMuon2Eta, fMuon2Pt;
  PidTable    *fPidTableMuIDPos, *fPidTableMuIDNeg; 
  PidTable    *fPidTableTrigPos, *fPidTableTrigNeg;
  PidTable    *fPidTableTrckEff;
  PidTable    *fPidTableTrigFit, *fPidTableMuidFit;
  
  double      fWeight;
  static const int  fNpt = 16;
  static const int  fNy = 12;
  double      fPTbin[fNpt+1], fYbin[fNy+1];
  int fBin;
  double fMassLow, fMassHigh;
  
  float um, uP, ue, up, gE, gP, ge, gp, dR, xbm;
  float m_um, m_uP, m_ue, m_up, m_gE, m_gP, m_ge, m_gp, m_dR, m_xbm;
  int m_xbid;
  float mbg_um, mbg_uP, mbg_ue, mbg_up, mbg_gE, mbg_gP, mbg_ge, mbg_gp, mbg_dR, mbg_xbm;
  
  vector<TAnaCand*> Cands;
  vector<TAnaCand*> Cands_ID;
  vector<TAnaCand*> Cands_TM;
  
  JSON   *fpJSON;
};


#endif
