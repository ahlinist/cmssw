#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1OnTheFlyHistos.h"
#include <iostream>

using namespace std;

void DTL1OnTheFlyHistos::book_histos() {
  if(some_more_debug) 
    cout << "Booking histos... " << flush;

  //book histos
  char* name = new char[100];
  /*
  // These are not filled anywhere!  
  xvertex  = new TH1F("xvertex","x vertex gen",100,-1.,1.);
  yvertex  = new TH1F("yvertex","y vertex gen",100,-1.,1.);
  zvertex  = new TH1F("zvertex","z vertex gen",100,-20.,20.);
  eta_stub  = new TH1F("eta_stub","eta gen",120,-1.1,1.1);*/
    
  dtheta_trig_vx  = new TH1F("dtheta_trig_vx","dtheta  trig ST1-gen",
			     400,-0.15,0.15);
  dtheta_bti_vx  = new TH1F("dtheta_bti_vx","dtheta  bti ST1-gen",
			    400,-0.15,0.15);
  /*
  // These are not filled anywhere!
  dtheta_ts_vx  = new TH1F("dtheta_ts_vx","dtheta  ts ST1-gen",
			   400,-0.15,0.15);
  */
  dtheta_trig_st  = new TH1F("dtheta_trig_st","dtheta  trig ST1- trig ST2",
			     400,-0.02,0.02);
  
  nbti_trig = new TH1F("nbti_trig","number BTI  trig ST1",10,0,10);
  /*
  // These are not filled anywhere!
  nts_trig =  new TH1F("nts_trig","number  TS trig ST1",10,0,10);
  */
  eff_counters  = new TH1F("eff_counters","eff_counters",20,-0.5,19.5);
  eta_gen  = new TH1F("eta_gen","eta gen",120,-1.5,1.5);
  /*
  // These are not filled anywhere!
  eta_gen_DTmatch  = new TH1F("eta_gen_DTmatch","eta gen",120,-1.5,1.5);
  eta_gen_DTmatch_GQ  = new TH1F("eta_gen_DTmatch_GQ","eta gen",120,-1.5,1.5);
  eta_gen_omi  = new TH1F("eta_gen_omi","eta gen",120,-1.5,1.5);
  eta_gen_mi  = new TH1F("eta_gen_mi","eta gen",120,-1.5,1.5);
  */
  phi_gen  = new TH1F("phi_gen","phi gen",120,0.0,6.3);
  /*
  // These are not filled anywhere!
  phi_gen_DT  = new TH1F("phi_gen_DT","phi gen",120,-3.15,3.15);
  phi_gen_DT_GQ  = new TH1F("phi_gen_DT_GQ","phi gen",120,-3.15,3.15);
  */
  ndt_match1 = new TH1F("ndt_match1","ST 1 - number of DT phi-eta matches",
			20,0.,20.);
  ndt_match2 = new TH1F("ndt_match2","ST 2 - number of DT phi-eta matches",
			20,0.,20.);
  ndt_match_BXok = 
    new TH1F("ndt_match_BXok",
	     "number of DT phi-eta matches at right bx before cancellation",
			    20,0.,20.);
  ndt_match_BXrj = 
    new TH1F("ndt_match_BXrj",
	     "number of DT phi-eta matches at right bx after cancellation",
			    20,0.,20.);
  /*
  // These are not filled anywhere! 
  dphi_DTMatch = new TH1F("dphi_DTMatch"," lay = 2 - pred phi diff ST1 - ST2",
			  800,-800,800);
  dtheta_DTMatch = new TH1F("dtheta_DTMatch"," pred theta diff ST1 - ST2",
			    100,-400,400);
  dphib_DTMatch = new TH1F("dphib_DTMatch"," pred phib diff ST1 - ST2",
			   200,-100,100);
  phib1_DT = new TH1F("phib1_DT","  phib  ST1",400,-200,200);
  phib2_DT = new TH1F("phib2_DT"," phib  ST2",400,-200,200);
  sigma_phi_1 = new TH1F("sigma_phi_1","  phi window  ST1",800,0,400);
  sigma_phi_2 = new TH1F("sigma_phi_2","  phi window  ST2",800,0,400);
  */
  
  ndt_match = new TH1F("ndt_match", "ST 1 or ST 2 - number of DT phi-eta matches",
		       20,0.,20);
  PT_match1  = new TH1F("PT_match1","ST 1 - PT of highest quality DT primitive",
			200,-1.,199.);
  PT_match2  = new TH1F("PT_match2","ST 2 - PT of highest quality DT primitive",
			200,-1.,199.);
  PT_match   = new TH1F("PT_match","ST1 or ST 2 - PT of highest quality DT primitive",
			200,-1.,199);
  invPT_match1  = new TH1F("invPT_match1",
			   "ST 1 - invPT of highest quality DT primitive",
			   600,0.0,0.4);
  invPT_match2  = new TH1F("invPT_match2",
			   "ST 2 - invPT of highest quality DT primitive",
			   600,0.0,0.4);
  invPT_match   = new TH1F("invPT_match",
			   "ST1 or ST 2 - invPT of highest quality DT primitive",
			   600,0.0,0.4);
  invPT_track   = new TH1F("invPT_track",
			   " invPT of matching tracker L1 Track",
			   600,0.0,0.4);
  /*
  // These are not filled anywhere!
  invPT_stubs   = new TH1F("invPT_stubs",
			   "5-3-0 - invPT of highest quality DT primitive",
			   1600,0.0,0.3);
  invPT_stubsV   = new TH1F("invPT_stubsV",
			    "5-3-V - invPT of highest quality DT primitive",
			    1600,0.0,0.3);;
  invPT_stubs1   = new TH1F("invPT_stubs1",
			    "3-1-0 - invPT of highest quality DT primitive",
			    1600,0.0,0.3);
  invPT_stubsV1   = new TH1F("invPT_stubsV1",
			     "3-0-V - invPT of highest quality DT primitive",
			     1600,0.0,0.3);
  dtmatchcode  = new TH1F("dtmachcode","ST 1 - number of DT phi-eta matches",
			  40,-2.,38.);
  */
  ptcut_average  = new TH1F("ptcut_average","classificazione soglie average",
							30,0.,30.);
	ptcut_average_tracklet  = new TH1F("ptcut_average_tracklet","classificazione soglie average",
							  30,0.,30.);
	ptcut_majority_tracklet  = new TH1F("ptcut_majority_tracklet","classificazione soglie majority",
										30,0.,30.);
	ptcut_mixedmode_tracklet  = new TH1F("ptcut_mixedmode_tracklet","classificazione soglie mixedmode",
										30,0.,30.);
  ptcut_majority  = new TH1F("ptcut_majority","classificazione soglie majority inner only",
							 30,0.,30.);
	ptcut_mixedmode  = new TH1F("ptcut_mixedmode","classificazione soglie mixedmode inner only",
							   30,0.,30.);	
  ptcut_majority_Full  = new TH1F("ptcut_majority_Full","classificazione soglie majority Full longbarrel",
			30,0.,30.);
  ptcut_encoder  = new TH1F("ptcut_encoder","classificazione soglie priority encoder",
		       30,0.,30.);
  ptcut_DTTF  = new TH1F("ptcut_DTTF","DTTF classificazione trigger",
		       30,0.,30.);
  ptcut_L1_track  = new TH1F("ptcut_L1_track","L1 tracker tracks classificazione trigger",
		       30,0.,30.);
  ptval_encoder  = new TH1F("ptval_encoder"," priority encoder - classificazione trigger",
		       150,0.,150.);
  ptval_average  = new TH1F("ptval_average","average classificazione trigger",
							150,0.,150.);
	ptval_average_tracklet  = new TH1F("ptval_average_tracklet","average classificazione trigger",
							  150,0.,150.);
	ptval_majority_tracklet  = new TH1F("ptval_majority_tracklet","majority classificazione trigger",
										150,0.,150.);
	ptval_mixedmode_tracklet  = new TH1F("ptval_mixedmode_tracklet","mixedmode classificazione trigger",
										150,0.,150.);
  ptval_majority  = new TH1F("ptval_majority","majority classificazione trigger inner only",
							 150,0.,150.);
	ptval_mixedmode  = new TH1F("ptval_mixedmode","mixedmode classificazione trigger inner only",
							   150,0.,150.);
  ptval_majority_Full  = new TH1F("ptval_majority_Full","majority classificazione trigger Full longbarrel",
			150,0.,150.);
  ptval_L1_track  = new TH1F("ptval_L1_track","L1 Tracker tracks classificazione trigger",
			150,0.,150.);
  ptval_DTTF  = new TH1F("ptval_DTTF","DTTF classificazione trigger",
			150,0.,150.);
  ptval_phib  = new TH2F("ptval_phib"," best - classificazione trigger vs phib",
			 400,-200,200,150,0.,150.);
  ptval_code  = new TH2F("ptval_code"," best - classificazione trigger vs code",
			 40,-0,40,150,0.,150.);
  
  phi_rho_ST1  = new TH2F("phi_rho_ST1"," phi vs rho ST1 ",400,0,6.3,400,400.,460.);
  phi_rho_ST2  = new TH2F("phi_rho_ST2"," phi vs rho ST2 ",400,0,6.3,400,500.,560.);
  /*
  // These are not filled anywhere!  
  mu_Pt_vs_phib  = new TH2F("mu_Pt_vs_phib","mu_pt_vs_phib",250,0.,500.,200, 0., 200.);
  */
  ptbinvsnstubs   = new TH2F("ptbinvsnstubs","ptbinvsnstubs",100,0.,100.,25, 0., 25.);
  /*
  // These are not filled anywhere!
  R_stub = new TH1F("R_stub","R - stub position",260,0.,130.);
  */
  // These are not filled anywhere!
    n_tracks = new TH1F("n_tracks","number of tracks  from ST 1 or ST 2",20,-1.,19);
    
  for ( int i = 0; i < 6; i++ ) {
    /*
    sprintf(name,"Ntot_stubs%i",i);
    Ntot_stubs[i] = new TH1F(name,"number of stubs per layer from ST 1",20,-1.,19);
    */
    sprintf(name,"n_stubs%i",i);
    n_stubs[i] = new TH1F(name,"number of stubs per layer from ST 1 or ST 2",20,-1.,19);
    /*
    sprintf(name,"n1_stubs%i",i);
    n1_stubs[i] = new TH1F(name,"number of stubs per layer from ST 1",20,-1.,19);
    sprintf(name,"n2_stubs%i",i);
    n2_stubs[i] = new TH1F(name,"number of stubs per layer from ST 2",20,-1.,19);
    sprintf(name,"n1_sigphi%i",i);
    n1_sigphi[i] = new TH1F(name,"number of sigmas of closest stub per layer from ST 1",
			    100,-20.,20);
    sprintf(name,"n2_sigphi%i",i);
    n2_sigphi[i] = new TH1F(name,"number of sigmas of closest stub per layer from ST 2",
			    100,-20.,20);
    */ 
    if (i <3) {
    sprintf(name,"n_tracklets%i",i);
    n_tracklets[i] = new TH1F(name,"number of tracklets per superlayer from ST 1 or ST 2",20,-1.,19);
    }

    if (i <5) {
    sprintf(name,"dphigen1_%i",i);   
    dphigen1_[i] = new TH1F(name,"phi diff ST1 - vertex",1600,-0.8,0.8);
    sprintf(name,"dphigen2_%i",i);   
    dphigen2_[i] = new TH1F(name,"phi diff ST2 - vertex",1600,-0.8,0.8); 
    }
   
  }
    
  /*
  // These are not filled anywhere!
  for ( int i = 0; i <50; i++ ) {
    if(i%10<6){
      //histos for extrapolation functions calculation
      sprintf(name,"dph_1_%i",i);
      dph_1_[i] = new TH1F(name,"phi diff ST1 - stub",1500,-3000,3000);
      sprintf(name,"dthet_1_%i",i);
      dthet_1_[i] = new TH1F(name,"theta diff ST1 - stub ",300,-2000.,2000.);
      sprintf(name,"dph_2_%i",i);
      dph_2_[i] = new TH1F(name,"phi diff ST2 - stub",1500,-3000,3000);
      sprintf(name,"dthet_2_%i",i);
      dthet_2_[i] = new TH1F(name,"theta diff ST2 - stub ",300,-2000.,2000.);      
      // histos for extrapolation functions check
      sprintf(name,"nsph_1_%i",i);
      nsph_1_[i] = new TH1F(name,"nsigmaphi diff ST1 pred - stub",50,-10.,10.);
      sprintf(name,"nsthet_1_%i",i);
      nsthet_1_[i] = new TH1F(name,"nsigmatheta diff ST1 pred - stub ",50,-10.,10.);
      sprintf(name,"nsph_2_%i",i);
      nsph_2_[i] = new TH1F(name,"nsigmaphi diff ST2 pred - pstub",50,-10.,10.);
      sprintf(name,"nsthet_2_%i",i);
      nsthet_2_[i] = new TH1F(name,"nsigmatheta diff ST2 pred - stub ",50,-10.,10.);
    }
  }
  */
  
  // These are not filled anywhere!
  for ( int i = 0; i < 5; i++ ) {
    sprintf(name,"phib_1_%i",i);
    phib_1_[i] = new TH1F(name,"phi bending ST1 ",500,-200,200);
    sprintf(name,"phib_2_%i",i);
    phib_2_[i] = new TH1F(name,"phi bending ST2 ",500,-200,200);
  }
  
  /*
   // These are not filled anywhere!
   dph_12_0 = new TH1F("dph_12_0","phi diff ST1 ST2 ",300,-0.15,0.15);
   dthet_12_0 = new TH1F("dthet_12_0","theta diff ST1 ST2 ",300,-0.15,0.15);
  */
  if(some_more_debug) 
    cout << "done!" << endl;
}


//-----------------------------------------------------------------------------  
void DTL1OnTheFlyHistos::write_out_histos(string& theRootFileNameHis) {
  if(some_more_debug) 
    cout << "\nWriting out additional histos... " << flush;

  theFile = new TFile(theRootFileNameHis.c_str(), "RECREATE");
  theFile->cd();
  /*
  // These are not filled anywhere!  
  xvertex->Write();
  yvertex->Write();
  zvertex->Write();
  eta_stub->Write();*/
    
  dtheta_trig_vx  ->Write();
  dtheta_bti_vx ->Write();
  /*
  // These are not filled anywhere!
  dtheta_ts_vx  ->Write();
  */
  dtheta_trig_st ->Write();
  nbti_trig ->Write();
  /*
  // These are not filled anywhere!
  nts_trig ->Write();
  */
  eff_counters->Write();	
  eta_gen->Write();
  /*
  // These are not filled anywhere!
  eta_gen_DTmatch->Write();
  eta_gen_DTmatch_GQ->Write();
  eta_gen_omi->Write();
  eta_gen_mi->Write();
  */ 	
  phi_gen->Write();
  /*
  // These are not filled anywhere!
  phi_gen_DT->Write();
  phi_gen_DT_GQ->Write();
  */
  ndt_match1->Write();
  ndt_match2->Write();
  ndt_match->Write();
  ndt_match_BXok -> Write();
  ndt_match_BXrj -> Write();
  /*
  // These are not filled anywhere!  
  dphi_DTMatch ->Write();
  dtheta_DTMatch ->Write();
  dphib_DTMatch ->Write();
  phib1_DT ->Write();
  phib2_DT ->Write();
  sigma_phi_1 ->Write();
  sigma_phi_2 ->Write();
  */
  PT_match1->Write();
  PT_match2->Write();
  PT_match->Write();
  invPT_match1->Write();
  invPT_match2->Write();
  invPT_match->Write();
  invPT_track->Write();
  /*
  // These are not filled anywhere!
  invPT_stubs->Write();
  invPT_stubsV->Write();
  invPT_stubs1->Write();
  invPT_stubsV1->Write();
  dtmatchcode  -> Write();
  */
	ptcut_average->Write();
	ptcut_average_tracklet->Write();
	ptcut_majority_tracklet->Write();
	ptcut_mixedmode_tracklet->Write();
	ptcut_mixedmode->Write();
	ptcut_majority->Write();
  ptcut_majority_Full->Write();
  ptcut_encoder->Write();
  ptcut_DTTF->Write();
  ptcut_L1_track->Write();
	ptval_average->Write();
	ptval_average_tracklet->Write();
	ptval_majority_tracklet->Write();
	ptval_mixedmode_tracklet->Write();
	ptval_majority->Write();
	ptval_mixedmode->Write();
  ptval_majority_Full->Write();
  ptval_encoder->Write();
  ptval_DTTF->Write();
  ptval_L1_track->Write();
  ptval_phib->Write();
  ptval_code->Write();
  phi_rho_ST1->Write();
  phi_rho_ST2->Write();
  /*
  // These are not filled anywhere!
  mu_Pt_vs_phib->Write();
  */
  ptbinvsnstubs->Write();
  //  R_stub->Write();
  
  // These are not filled anywhere!
    n_tracks->Write();
  for ( int i = 0; i < 6; i++ ){
 //   Ntot_stubs[i]->Write();
    n_stubs[i]->Write();
/*    n1_stubs[i]->Write();
    n2_stubs[i]->Write();
    n1_sigphi[i]->Write();
    n2_sigphi[i]->Write();*/
    if(i<3) n_tracklets[i]->Write();
    if(i<5) dphigen1_[i]->Write();
    if(i<5) dphigen2_[i]->Write();
    
  }
   
  /*
  // These are not filled anywhere!
  for ( int i = 0; i < 50; i++ ){
    if(i%10<6){
      dph_1_[i] ->Write();
      dph_2_[i] ->Write();
      dthet_1_[i] ->Write();
      dthet_2_[i] ->Write();
      nsph_1_[i] ->Write();
      nsph_2_[i] ->Write();
      nsthet_1_[i] ->Write();
      nsthet_2_[i] ->Write();
    }
  } 
  */
  
  // These are not filled anywhere!
  for ( int i = 0; i < 5; i++ ){
    phib_1_[i] ->Write();
    phib_2_[i] ->Write();
  } 
   
  // dph_12_0 ->Write();
  // dthet_12_0 ->Write();

  if(some_more_debug)
    cout << "done!" << endl;

  theFile->Close();
  /*
    When a TFile is closed, all histograms in memory associated with this file 
    are automatically deleted.
  */
  delete theFile;
}


