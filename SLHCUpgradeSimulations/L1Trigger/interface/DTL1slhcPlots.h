#ifndef __DTL1slhcAnalysis
#define __DTL1slhcAnalysis

// -*- C++ -*-
//
// Package:    DTL1slhcPlots
// Class:      DTL1slhcPlots
// 
/**\class DTL1slhcPlots

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ignazio Lazzizzera
//         Created:  Fri Oct 16 15:44:58 CEST 2009
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/GenericHandle.h"
#include "FWCore/Framework/interface/ESHandle.h"

// STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iterator>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <iomanip>


// ROOT
#include <TROOT.h>
#include <TApplication.h>
#include <TRint.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TFrame.h>
#include <TLegend.h>
#include <THStack.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TVector3.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TTree.h>
#include <TBranchElement.h>
#include <TMath.h>

//#include "SLHCUpgradeSimulations/L1Trigger/interface/DTSeededTracklet.h"
#include "SimDataFormats/SLHC/interface/DTSeededTracklet.h"


using namespace std;


class DTL1slhcPlots : public edm::EDAnalyzer {

public:
  explicit DTL1slhcPlots(const edm::ParameterSet&);
  ~DTL1slhcPlots();
  
  void getOptimalRange(TH1* h, float entries[]);
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  unsigned long int EvCounter;
  size_t GaugeSampleSize;
  int breath;
  double desert;
  
  // ----------member data ---------------------------

  edm::InputTag DataSource;

  string RootFileNamePlots;
  TFile *OutTFilePlots;
  // histohrams

  TProfile* profile_PtOverGenPt_vs_bendingDT;

  TH2F*     scatter_PtOverGenPt_vs_bendingDT;

  TH1F     *h_Pt_IMu_3_V,      *h_invPt_IMu_3_V;
  float    Pt_IMu_3_V[30000],  invPt_IMu_3_V[3000];
  TH1F     *h_Pt_Mu_3_V,       *h_invPt_Mu_3_V;
  float    Pt_Mu_3_V[3000],    invPt_Mu_3_V[3000];
  TH1F     *h_Pt_5_3_V,        *h_invPt_5_3_V;
  float    Pt_5_3_V[3000],     invPt_5_3_V[3000];
  TH1F     *h_Pt_only_Mu_V,    *h_invPt_only_Mu_V;
  float    Pt_only_Mu_V[3000], invPt_only_Mu_V[3000];

  TH1F     *h_DTTracklets_Pt_size;

};

#endif

