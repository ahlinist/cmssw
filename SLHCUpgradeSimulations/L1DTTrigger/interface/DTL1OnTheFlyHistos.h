// Stuff from PL Zotto, restructured by Ignazio and Zotto... si incazzera`!!!
#ifndef __DTL1OnTheFlyHistos__
#define __DTL1OnTheFlyHistos__

class TFile;
class TH1F;
class TH2F;
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace std;

class DTL1OnTheFlyHistos
{
 public:
  TFile* theFile;
  TH1F *xvertex;
  TH1F *yvertex;
  TH1F *zvertex;
  TH1F *eta_stub;
  TH1F *dtheta_trig_vx  ;
  TH1F *dtheta_bti_vx ;
  TH1F *dtheta_ts_vx  ;
  TH1F *dtheta_trig_st ;	
  TH1F *nbti_trig ;
  TH1F *nts_trig ;
  TH1F* eff_counters;	
  TH1F* eta_gen;
  TH1F* eta_gen_DTmatch;
  TH1F* eta_gen_DTmatch_GQ;	
  TH1F* phi_gen;
  TH1F* phi_gen_DT;
  TH1F* phi_gen_DT_GQ;
  TH1F* eta_gen_omi;
  TH1F* eta_gen_mi;
  TH1F *ndt_match1;
  TH1F *ndt_match2;
  TH1F *ndt_match;
  TH1F *ndt_match_BXok;
  TH1F *ndt_match_BXrj;
  TH1F *dphi_DTMatch;
  TH1F *dtheta_DTMatch;	
  TH1F *dphib_DTMatch;	
  TH1F *phib1_DT;	
  TH1F *phib2_DT;
  TH1F *sigma_phi_1;
  TH1F *sigma_phi_2;
  TH1F *PT_match1;
  TH1F *PT_match2;
  TH1F *PT_match;
  TH1F *invPT_match1;
  TH1F *invPT_match2;
  TH1F *invPT_match;
  TH1F *invPT_track;
  TH1F *invPT_stubs;
  TH1F *invPT_stubsV;
  TH1F *invPT_stubs1;
  TH1F *invPT_stubsV1;
  TH1F *dtmatchcode;
	TH1F *ptcut_average;
	TH1F *ptcut_average_tracklet;
	TH1F *ptcut_majority_tracklet;
	TH1F *ptcut_mixedmode_tracklet;
	TH1F *ptcut_majority;
	TH1F *ptcut_mixedmode;
  TH1F *ptcut_majority_Full;
  TH1F *ptcut_encoder;
  TH1F *ptcut_DTTF;
  TH1F *ptcut_L1_track;
	TH1F *ptval_average;
	TH1F *ptval_average_tracklet;
	TH1F *ptval_majority_tracklet;
	TH1F *ptval_mixedmode_tracklet;
	TH1F *ptval_majority;
	TH1F *ptval_mixedmode;
  TH1F *ptval_majority_Full;
  TH1F *ptval_encoder;
  TH1F *ptval_DTTF;
  TH1F *ptval_L1_track;
  TH2F *ptval_phib;
  TH2F *ptval_code;
  TH2F *phi_rho_ST1;
  TH2F *phi_rho_ST2;
  TH2F *mu_Pt_vs_phib;
  TH2F *ptbinvsnstubs;
  TH1F *R_stub;
  TH1F *Ntot_stubs[10];
  TH1F *n_stubs[10];
  TH1F *n1_stubs[10];
  TH1F *n2_stubs[10];
  TH1F *n1_sigphi[10];
  TH1F *n2_sigphi[10];
  TH1F *n_tracklets[10];
  TH1F *n_tracks;
  TH1F *dphigen1_[10];
  TH1F *dphigen2_[10];
  TH1F *dph_1_[50];
  TH1F *dph_2_[50];
  TH1F *dthet_1_[50];
  TH1F *dthet_2_[50];
  TH1F *nsph_1_[50];
  TH1F *nsph_2_[50];
  TH1F *nsthet_1_[50];
  TH1F *nsthet_2_[50];
  TH1F *phib_1_[5];
  TH1F *phib_2_[5];
  TH1F *dph_12_0;
  TH1F *dthet_12_0;

  bool some_more_debug;

  DTL1OnTheFlyHistos(){some_more_debug = false;}
  void book_histos();
  void write_out_histos(string& theRootFileNameHis); 
  ~DTL1OnTheFlyHistos(){}
};

#endif
