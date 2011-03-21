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
// $Id: DTL1slhcPlots.h,v 1.1 2010/03/03 13:09:38 arose Exp $
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
#include <TF1.h>
#include <TVector3.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TTree.h>
#include <TBranchElement.h>
#include <TMath.h>

#include "SimDataFormats/SLHC/interface/DTStubMatchesCollection.h"
#include "SimDataFormats/SLHC/interface/DTSeededTracklet.h"


using namespace std;
using namespace edm;


// Radius of tracker layers in cm
static const float TrackerLayerR[] = { 34., 38., 50., 54., 100., 104. }; 



class DTL1slhcPlots : public edm::EDAnalyzer {

public:
  explicit DTL1slhcPlots(const edm::ParameterSet&);
  ~DTL1slhcPlots();
  
  void make_StubStubDephi_plots(Handle<DTStubMatchesCollection> &, int);
  void make_MuStubStub_plots(Handle<DTStubMatchesCollection> &, int);
  void make_StubStubStub_plots(Handle<DTStubMatchesCollection> &, int);
  void make_test_plots(const edm::Event& event,
		       Handle<DTStubMatchesCollection> &, int i);
  void make_LinStubs_plots(const edm::Event& event,
			   Handle<DTStubMatchesCollection> &, int i);
  void make_StubStubDephi_canvases();
  void make_MuStubStub_canvases();
  void make_StubStubStub_canvases();
  void make_test_canvases();
  void make_LinStubs_canvases();

  void stubstubdephi_invPt_profile_with_linfit(TCanvas*, TProfile*, 
					       string, Int_t, bool);
  void stack_of_stubstubdephi_invPt_profile_with_linfits();
  void getOptimalRange(TH1* h, float entries[]);
  void update_histo(TH1F *histo, float store[], const double val);
  
  void magnetic_field_global_map();
  void magnetic_field_barrelYaxis_profile();

  void setIStyle(); // Style for Root


private:
  const edm::ParameterSet pSet;
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  bool optimize_plot;
  size_t RangeCalibrationSampleSize;
  int breath;
  double desert;

  std::vector<DTSeededTracklet*> DTSeededTrackletVector;

  // Counters
  size_t EvNo;
  size_t AtLeastOneDTStaubMatchingMuonEvNo;
  
  // ----------member data ---------------------------

  edm::InputTag DataSource;

  string RootFileNamePlots;
  TFile *OutTFilePlots;

  TStyle* IStyle;

  const MagneticField* theField;

  // histograms

  TH1F* dephiL0L2;    float tmp_dephiL0L2[30000];
  TH1F* dephiL0L3;    float tmp_dephiL0L3[30000];
  TH1F* dephiL1L2;    float tmp_dephiL1L2[30000];
  TH1F* dephiL1L3;    float tmp_dephiL1L3[30000];

  TH1F* dephiL0L8;    float tmp_dephiL0L8[30000];
  TH1F* dephiL0L9;    float tmp_dephiL0L9[30000];
  TH1F* dephiL1L8;    float tmp_dephiL1L8[30000];
  TH1F* dephiL1L9;    float tmp_dephiL1L9[30000];

  TH1F* dephiL2L8;    float tmp_dephiL2L8[30000];
  TH1F* dephiL2L9;    float tmp_dephiL2L9[30000];
  TH1F* dephiL3L8;    float tmp_dephiL3L8[30000];
  TH1F* dephiL3L9;    float tmp_dephiL3L9[30000];

  TProfile* profile_dephiL0L2_vs_invGenPt;
  TProfile* profile_dephiL0L3_vs_invGenPt;
  TProfile* profile_dephiL1L2_vs_invGenPt;
  TProfile* profile_dephiL1L3_vs_invGenPt;

  TProfile* profile_dephiL0L8_vs_invGenPt;
  TProfile* profile_dephiL0L9_vs_invGenPt;
  TProfile* profile_dephiL1L8_vs_invGenPt;
  TProfile* profile_dephiL1L9_vs_invGenPt;

  TProfile* profile_dephiL2L8_vs_invGenPt;
  TProfile* profile_dephiL2L9_vs_invGenPt;
  TProfile* profile_dephiL3L8_vs_invGenPt;
  TProfile* profile_dephiL3L9_vs_invGenPt;

  TH1F* Pt_dephiL0L2;    float tmp_Pt_dephiL0L2[30000];
  TH1F* Pt_dephiL0L3;    float tmp_Pt_dephiL0L3[30000];
  TH1F* Pt_dephiL1L2;    float tmp_Pt_dephiL1L2[30000];
  TH1F* Pt_dephiL1L3;    float tmp_Pt_dephiL1L3[30000];

  TH1F* Pt_dephiL0L8;    float tmp_Pt_dephiL0L8[30000];
  TH1F* Pt_dephiL0L9;    float tmp_Pt_dephiL0L9[30000];
  TH1F* Pt_dephiL1L8;    float tmp_Pt_dephiL1L8[30000];
  TH1F* Pt_dephiL1L9;    float tmp_Pt_dephiL1L9[30000];

  TH1F* Pt_dephiL2L8;    float tmp_Pt_dephiL2L8[30000];
  TH1F* Pt_dephiL2L9;    float tmp_Pt_dephiL2L9[30000];
  TH1F* Pt_dephiL3L8;    float tmp_Pt_dephiL3L8[30000];
  TH1F* Pt_dephiL3L9;    float tmp_Pt_dephiL3L9[30000];

  TH1F     *h_Pt_IMu_3_V,        *h_invPt_IMu_3_V;
  float    Pt_IMu_3_V[30000],    invPt_IMu_3_V[3000];

  TH1F     *h_Pt_IMu_3_0_St1,    *h_Pt_IMu_3_0_St2;
  float    Pt_IMu_3_0_St1[3000], Pt_IMu_3_0_St2[3000];
  TH1F     *h_Pt_IMu_3_0,        *h_invPt_IMu_3_0;
  float    Pt_IMu_3_0[3000],     invPt_IMu_3_0[3000];

  TH1F     *h_Pt_Mu_3_0_St1,     *h_Pt_Mu_3_0_St2;
  float    Pt_Mu_3_0_St1[3000],  Pt_Mu_3_0_St2[3000];
  TH1F     *h_Pt_Mu_3_0,         *h_invPt_Mu_3_0;
  float    Pt_Mu_3_0[3000],      invPt_Mu_3_0[3000];

  TH1F     *h_Pt_Stubs_9_3_0,    *h_invPt_Stubs_9_3_0;
  float    Pt_Stubs_9_3_0[3000], invPt_Stubs_9_3_0[3000];
  TH1F     *h_Pt_Stubs_9_3_V,    *h_invPt_Stubs_9_3_V;
  float    Pt_Stubs_9_3_V[3000], invPt_Stubs_9_3_V[3000];

  TH1F     *h_Pt_LinStubs_9_3_0, *h_invPt_LinStubs_9_3_0;
  float    Pt_LinStubs_9_3_0[3000], invPt_LinStubs_9_3_0[3000];
  TH1F     *h_deltaAlpha0_LinStubs_9_3_0, *h_deltad_LinStubs_9_3_0;
  float    deltaAlpha0_LinStubs_9_3_0[3000], deltad_LinStubs_9_3_0[3000];
  TH1F     *h_Pt_LinStubs_9_1_0,    *h_invPt_LinStubs_9_1_0;
  float    Pt_LinStubs_9_1_0[3000], invPt_LinStubs_9_1_0[3000];
  TH1F     *h_deltaAlpha0_LinStubs_9_1_0, *h_deltad_LinStubs_9_1_0;
  float    deltaAlpha0_LinStubs_9_1_0[3000], deltad_LinStubs_9_1_0[3000];

  TProfile* profile_RTilde_vs_genPt_st1;
  TProfile* profile_RTilde_vs_genPt_st2;
  TH2F* scatter_RTilde_vs_genPt_st1;
  TH2F* scatter_RTilde_vs_genPt_st2;
  TProfile* profile_RTilde_vs_TkLayer_genPt10_st1;
  TProfile* profile_RTilde_vs_TkLayer_genPt10_st2;
  TProfile* profile_RTilde_vs_TkLayer_genPt50_st1;
  TProfile* profile_RTilde_vs_TkLayer_genPt50_st2;

  TH1F      *hIbendingDT_st1, *hIbendingDT_st2;
  TH1F      *DeltaAlphaDT_st1, *DeltaAlphaDT_st2;
  TH1F      *DeltaPhiDT_st1, *DeltaPhiDT_st2;
  TH1F      *DeltaBendingDT_st1, *DeltaBendingDT_st2;
  TH1F      *h_DTTracklets_Pt_size;

};

#endif


