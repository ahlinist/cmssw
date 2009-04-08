#define gjet_response_cxx
#include "gjet_response.h"
#include <TF1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TStyle.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TMinuit.h>
#include <TMath.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

//using std::string;
using namespace std;

bool drawStats = false;

inline float gjet_response::delta_phi(float phi1, float phi2) {

  float dphi = fabs(phi1 - phi2);
  return (dphi <= TMath::Pi())? dphi : TMath::TwoPi() - dphi;
}

inline float gjet_response::delta_eta(float eta1, float eta2) {

  return (eta2 >= 0 ? eta1 - eta2 : eta2 - eta1);
}

bool gjet_response::NNID(float nnval) {

  bool nnphotcut(false);
  if (ptphot<60)                 nnphotcut = nnval > 1.007;
  if (ptphot<100 && ptphot>=60)  nnphotcut = nnval > 0.999;
  if (ptphot<200 && ptphot>=100) nnphotcut = nnval > 0.980;
  if (ptphot>=200)               nnphotcut = nnval > 0.999;

  return nnphotcut;
}

// Return signal response multiplied by DeltaC
// Can be directly used with TF1
bkgbias *bkgobj;
Double_t bkgresp(const Double_t *ptgamma, const Double_t *p) {

  double x = *ptgamma;
  double r = p[2] - p[0]*pow(0.01*x, p[1]-1);
  double dc = bkgobj->deltac(ptgamma); 
  
  return (r*dc);
}

// Calculate low pT bias for given Gaussian and threshold
void calcLowPtBias(TF1 *gaus, const double xmin,
		   double& bm, double& bs) {
 
  TF1 *gaus_x = new TF1("gaus_x","x*gaus",0.,2.);
  TF1 *gaus_xx = new TF1("gaus_xx","x*x*gaus",0.,2.);
  for (int ipar = 0; ipar != 3; ++ipar) {
    gaus_x->SetParameter(ipar, gaus->GetParameter(ipar));
    gaus_xx->SetParameter(ipar, gaus->GetParameter(ipar));
  }
  bm = (gaus_x->Integral(0.,2.) / gaus->Integral(0.,2.))
    / (gaus_x->Integral(xmin,2.) / gaus->Integral(xmin,2.));
  bs = (gaus_xx->Integral(0.,2.) / gaus->Integral(0.,2.))
    / (gaus_xx->Integral(xmin,2.) / gaus->Integral(xmin,2.));

  gaus_x->Delete();
  gaus_xx->Delete();

  return;
}

void gjet_response::Configure(const char *cfg) {

  cafe::Config config(cfg);

  // Test that file is available
  string test = config.get("AlgoName", "test");
  if (test=="test") {
    cout << "* WARNING: No configuration found for 'AlgoName' in '"
	 << cfg << "'" << endl;
    cout << "* Check that CAFE_CONFIG environment variable" << endl
	 << "* is set, file exists and configuration is set." << endl;
    cout << "* Set the environmet variable like this:" << endl
	 << "  gSystem->Setenv(\"CAFE_CONFIG\", \"gjettree.config\");" << endl;
    cout << "* Set the configuration variable in the file like this:" << endl
	 << "  "<<cfg<<".AlgoName:  IterativeCone0.5" << endl;
    exit(10);
  }

  // Algorithm name
  _algoname = config.get("AlgoName", "");
  drawStats = config.get("DrawStats", drawStats);

  // Topological cuts

  // Maximum photon eta
  _photetacut = config.get("PhotEtaCut", 1.3);
  // Minimum photon pT
  _photptcut = config.get("PhotPtCut", 25.);
  // Maximum jet eta
  _jetetacut = config.get("JetEtaCut", 1.3);
  // Minimum pT for second jet
  _jet2_minpt = config.get("Jet2_minPt", 10.);
  // Maximum fraction of photon energy for second jet
  _jet2_maxfrac = config.get("Jet2_maxFrac", 0.10);
  // Minimum DeltaPhi(leadjet, leadphoton), 0<DeltaPhi<pi
  _deltaphi = config.get("DeltaPhi", TMath::Pi()-0.2);
  // Maximum Abs(DeltaEta(leadjet, leadphoton))
  _deltaeta = config.get("DeltaEta", 1.0);

  // MC truth function
  _mctruthfunc = config.get("MCTruthFunc","1 - 2.316*pow(x,0.6005-1)");
  _mctruthcsa07 = config.get("MCTruth_CSA07","");

  // For low pT bias correction, effective low pT threshold
  // NB: not same as real reco threshold, need to also consider
  //      the even inefficiencies caused by fake and MB jets
  _lowptreco = config.get("LowPtReco",5.5);
  _lowptbias = (_lowptreco>0);
  _fitptmin = config.get("FitPtMin",30.);

  // Graph x-axis limits
  _xmin = config.get("Xmin", 25.);
  _xmax = config.get("Xmax", 700.);
  // Rebin purity, efficiency etc. plots
  _rebin = config.get("Rebin", 3);

  return;
} // Configure


void gjet_response::DumpConfig() {

  cout << "Running gjet_response.C with the following configuration:" << endl
       << endl
       << "Topological cuts:" << endl
       << "--------------------" << endl
       << "PhotEtaCut:   " << _photetacut << endl
       << "PhotPtCut:    " << _photptcut << endl
       << "JetEtaCut:    " << _jetetacut << endl
       << "Jet2_minPt:   " << _jet2_minpt << endl
       << "Jet2_maxFrac: " << _jet2_maxfrac << endl
       << "DeltaPhi:     " << _deltaphi << endl
       << "DeltaEta:     " << _deltaeta << endl
       << endl
       << "MC truth function:" << endl
       << "--------------------" << endl
       << "MCTruth_CSA07:  " << _mctruthcsa07 << endl
       << "MCTruthFunc:  " << _mctruthfunc << endl
       << endl
       << "Graph X-axis limits and other settings" << endl
       << "--------------------" << endl
       << "Xmin:         " << _xmin << endl
       << "Xmax:         " << _xmax << endl
       << "Rebin:         " << _rebin << endl
       << "***** Remember to add a switch for fit vs. mean! ****" << endl
       << endl;

} // DumpConfig


void gjet_response::Loop(const string idtype)
{
//   In a ROOT session, you can do:
//      Root > .L gjet_response.C
//      Root > gjet_response t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

   bkgobj = new bkgbias(idtype);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   cout << "Looping over "<<nentries<<" entries" << flush;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
      if (jentry%100000==1) cout << "." << flush;
      if (jentry%5000000==1 || jentry==nentries-1) cout << endl << flush;

      ptphot_nocut->Fill(ptphot,weight);
      response_nocut->Fill(ptjet/ptphot,weight);
      responsevspt_nocut->Fill(ptphot,ptjet/ptphot,weight);

      // Topological cuts
      bool ptcut = (ptphot > _photptcut);
      bool ptjetcut = (ptjet > _lowptreco);
      bool pt2jetcut = (pt2jet < _jet2_maxfrac*ptphot || pt2jet < _jet2_minpt);
      //bool pt2photcut = true;
      bool etacut = (fabs(etaphot) < _photetacut);
      bool etajetcut = (fabs(etajet) < _jetetacut);
      bool deltaphicut = (delta_phi(phijet,phiphot) > _deltaphi);
      bool deltaetacut = (fabs(delta_eta(etajet,etaphot)) < _deltaeta);
      bool topocuts = ptcut && etacut && ptjetcut && etajetcut && pt2jetcut
	&& deltaphicut && deltaetacut;

      // PhotonID cuts
      bool idcuts = false;
      _idtype = idtype;
      if (idtype=="NN")     { idcuts = NNID(nniso); }
      if (idtype=="loose")  { idcuts = (photonid>=1); }
      if (idtype=="medium") { idcuts = (photonid>=10); }
      if (idtype=="tight")  { idcuts = (photonid>=100); }
      assert(idtype=="loose" || idtype=="medium"
	     || idtype=="tight" || idtype=="NN");

      bool allcuts = topocuts && idcuts;

      if (isgamma) {

	ptphot_nocut_sig->Fill(ptphot,weight);
	response_nocut_sig->Fill(ptjet/ptphot,weight);
	responsevspt_nocut_sig->Fill(ptphot,ptjet/ptphot,weight);
	
	if (ptphot<60)
	  response_nocut_sig_reg1->Fill(ptjet/ptphot,weight);
	if (ptphot<100 && ptphot>=60)
	  response_nocut_sig_reg2->Fill(ptjet/ptphot,weight);
	if (ptphot<200 && ptphot>=100)
	  response_nocut_sig_reg3->Fill(ptjet/ptphot,weight);
	if (ptphot>=200)
	  response_nocut_sig_reg4->Fill(ptjet/ptphot,weight);

	if (topocuts) {
	  if (ptphot<60)                 nn_reg1_sig->Fill(nniso,weight);
	  if (ptphot<100 && ptphot>=60)  nn_reg2_sig->Fill(nniso,weight);
	  if (ptphot<200 && ptphot>=100) nn_reg3_sig->Fill(nniso,weight);
	  if (ptphot>=200)               nn_reg4_sig->Fill(nniso,weight);
	}

      } else if (!isgamma) {

	ptphot_nocut_bkg->Fill(ptphot,weight);
	response_nocut_bkg->Fill(ptjet/ptphot,weight);
	responsevspt_nocut_bkg->Fill(ptphot,ptjet/ptphot,weight);

	if (topocuts) {
	  if (ptphot<60)                 nn_reg1_bkg->Fill(nniso,weight);
	  if (ptphot<100 && ptphot>=60)  nn_reg2_bkg->Fill(nniso,weight);
	  if (ptphot<200 && ptphot>=100) nn_reg3_bkg->Fill(nniso,weight);
	  if (ptphot>=200)               nn_reg4_bkg->Fill(nniso,weight);
	}
      }

      if (topocuts) {
	if (isgamma)
	  ptphot_topocut_sig->Fill(ptphot,weight);
	else if (!isgamma)
	  ptphot_topocut_bkg->Fill(ptphot,weight);
      }

      if (idcuts) {
	if (isgamma)
	  ptphot_idcut_sig->Fill(ptphot,weight);
	else if (!isgamma)
	  ptphot_idcut_bkg->Fill(ptphot,weight);
      }
      
      if (allcuts){

	ptmean_mix->Fill(ptphot,ptphot,weight);

	ptphot_mix->Fill(ptphot,weight);
	response_mix->Fill(ptjet/ptphot,weight);
	responsevspt_mix->Fill(ptphot,ptjet/ptphot,weight);

	if (ptphot<60)
	  response_mix_reg1->Fill(ptjet/ptphot,weight);
	if (ptphot<100 && ptphot>=60)
	  response_mix_reg2->Fill(ptjet/ptphot,weight);
	if (ptphot<200 && ptphot>=100)
	  response_mix_reg3->Fill(ptjet/ptphot,weight);
	if (ptphot>=200)
	  response_mix_reg4->Fill(ptjet/ptphot,weight);
	
	if (isgamma) {

	  ptmean_sig->Fill(ptphot,ptphot,weight);

	  ptphot_sig->Fill(ptphot,weight);
	  response_sig->Fill(ptjet/ptphot,weight);
	  responsevspt_sig->Fill(ptphot,ptjet/ptphot,weight);	

	  if (ptphot<60)
	    response_sig_reg1->Fill(ptjet/ptphot,weight);
	  if (ptphot<100 && ptphot>=60)
	    response_sig_reg2->Fill(ptjet/ptphot,weight);
	  if (ptphot<200 && ptphot>=100)
	    response_sig_reg3->Fill(ptjet/ptphot,weight);
	  if (ptphot>=200)
	    response_sig_reg4->Fill(ptjet/ptphot,weight);

	} else if (!isgamma) {

	  ptmean_bkg->Fill(ptphot,ptphot,weight);

	  ptphot_bkg->Fill(ptphot,weight);
	  response_bkg->Fill(ptjet/ptphot,weight);
	  responsevspt_bkg->Fill(ptphot,ptjet/ptphot,weight);
	  
	  if (ptphot<60)
	    response_bkg_reg1->Fill(ptjet/ptphot,weight);
	  if (ptphot<100 && ptphot>=60)
	    response_bkg_reg2->Fill(ptjet/ptphot,weight);
	  if (ptphot<200 && ptphot>=100)
	    response_bkg_reg3->Fill(ptjet/ptphot,weight);
	  if (ptphot>=200)
	    response_bkg_reg4->Fill(ptjet/ptphot,weight);
	}
      }
   }
}

void gjet_response::BookHistos()
{

  double xmin = _xmin;//25.;
  double xmax = _xmax;//700.;

  int rb = _rebin; // Rebin some plots to have wider bins

  // create linear or logarithmic binning
  bool _logbins = true;

  vector<double> xbins(XBINS+1);
  for (int i = 0; i != XBINS+1; ++i) {
    xbins[i] = (_logbins ?
		xmin * pow(xmax / xmin, double(i) / XBINS) : 
		xmin + i * (xmax - xmin) / XBINS);
  }

  vector<double> xbins_w(XBINS/rb+1);
  for (int i = 0; i != XBINS/rb+1; ++i) {
    xbins_w[i] = (_logbins ?
		  xmin * pow(xmax / xmin, double(i) / (XBINS/rb)) : 
		  xmin + i * (xmax - xmin) / (XBINS/rb));
  }


  ptmean_mix = new TProfile("ptmean_mix","ptmean_mix",XBINS,&xbins[0]);
  ptmean_sig = new TProfile("ptmean_sig","ptmean_sig",XBINS,&xbins[0]);
  ptmean_bkg = new TProfile("ptmean_bkg","ptmean_bkg",XBINS,&xbins[0]);
  
  ptphot_nocut = new TH1D("ptphot_nocut","ptphot_nocut",XBINS/rb,&xbins_w[0]);
  ptphot_nocut->Sumw2();
  ptphot_mix = new TH1D("ptphot_mix","ptphot",XBINS/rb,&xbins_w[0]);
  ptphot_mix->Sumw2();
  response_nocut = new TH1D("response_nocut","response_nocut",XBINS,0.,2.);
  response_nocut->Sumw2();
  response_mix = new TH1D("response_mix","response",XBINS,0.,2.);
  response_mix->Sumw2();
  responsevspt_nocut = new TH2D("responsevspt_nocut","responsevspt_nocut",
				XBINS,&xbins[0],YBINS,0.,2.);
  responsevspt_nocut->Sumw2();
  responsevspt_mix = new TH2D("responsevspt_mix","responsevspt",
			      XBINS,&xbins[0],YBINS,0.,2.);
  responsevspt_mix->Sumw2();
  response_mix_reg1 = new TH1D("response_mix_reg1","response",XBINS,0.,2.);
  response_mix_reg1->Sumw2();
  response_mix_reg2 = new TH1D("response_mix_reg2","response",XBINS,0.,2.);
  response_mix_reg2->Sumw2();
  response_mix_reg3 = new TH1D("response_mix_reg3","response",XBINS,0.,2.);
  response_mix_reg3->Sumw2();
  response_mix_reg4 = new TH1D("response_mix_reg4","response",XBINS,0.,2.);
  response_mix_reg4->Sumw2();
  
  ptphot_nocut_sig = new TH1D("ptphot_nocut_sig","ptphot_nocut_sig",
			      XBINS/rb,&xbins_w[0]);
  ptphot_nocut_sig->Sumw2();
  ptphot_sig = new TH1D("ptphot_sig","ptphot_sig",XBINS/rb,&xbins_w[0]);
  ptphot_sig->Sumw2();
  response_nocut_sig = new TH1D("response_nocut_sig","response_nocut_sig",
				XBINS,0.,2.);
  response_nocut_sig->Sumw2();
  response_sig = new TH1D("response_sig","response_sig", XBINS,0.,2.);
  response_sig->Sumw2();
  responsevspt_nocut_sig = new TH2D("responsevspt_nocut_sig",
				    "responsevspt_nocut_sig",XBINS,&xbins[0],
				    YBINS,0.,2.);
  responsevspt_nocut_sig->Sumw2();
  responsevspt_sig = new TH2D("responsevspt_sig",
			      "responsevspt_sig",XBINS,&xbins[0],
			      YBINS,0.,2.);
  responsevspt_sig->Sumw2();
  response_nocut_sig_reg1 = new TH1D("response_nocut_sig_reg1",
				     "response_nocut",XBINS,0.,2.);
  response_nocut_sig_reg1->Sumw2();
  response_nocut_sig_reg2 = new TH1D("response_nocut_sig_reg2",
				     "response_nocut",XBINS,0.,2.);
  response_nocut_sig_reg2->Sumw2();
  response_nocut_sig_reg3 = new TH1D("response_nocut_sig_reg3",
				     "response_nocut",XBINS,0.,2.);
  response_nocut_sig_reg3->Sumw2();
  response_nocut_sig_reg4 = new TH1D("response_nocut_sig_reg4",
				     "response_nocut",XBINS,0.,2.);
  response_nocut_sig_reg4->Sumw2();
  response_sig_reg1 = new TH1D("response_sig_reg1",
			       "response",XBINS,0.,2.);
  response_sig_reg1->Sumw2();
  response_sig_reg2 = new TH1D("response_sig_reg2",
			       "response",XBINS,0.,2.);
  response_sig_reg2->Sumw2();
  response_sig_reg3 = new TH1D("response_sig_reg3",
			       "response",XBINS,0.,2.);
  response_sig_reg3->Sumw2();
  response_sig_reg4 = new TH1D("response_sig_reg4",
			       "response",XBINS,0.,2.);
  response_sig_reg4->Sumw2();
  
  ptphot_nocut_bkg = new TH1D("ptphot_nocut_bkg","ptphot_nocut_bkg",
			      XBINS/rb,&xbins_w[0]);
  ptphot_nocut_bkg->Sumw2();
  ptphot_bkg = new TH1D("ptphot_bkg","ptphot_bkg",
			XBINS/rb,&xbins_w[0]);
  ptphot_bkg->Sumw2();
  response_nocut_bkg = new TH1D("response_nocut_bkg","response_nocut_bkg",
				XBINS,0.,2.);
  response_nocut_bkg->Sumw2();
  response_bkg = new TH1D("response_bkg","response_bkg",
			  XBINS,0.,2.);
  response_bkg->Sumw2();
  responsevspt_nocut_bkg = new TH2D("responsevspt_nocut_bkg",
				    "responsevspt_nocut_bkg",XBINS,&xbins[0],
				    YBINS,0.,2.);
  responsevspt_nocut_bkg->Sumw2();
  responsevspt_bkg = new TH2D("responsevspt_bkg",
			      "responsevspt_bkg",XBINS,&xbins[0],
			      YBINS,0.,2.);
  responsevspt_bkg->Sumw2();
  response_bkg_reg1 = new TH1D("response_bkg_reg1",
			       "response",XBINS,0.,2.);
  response_bkg_reg1->Sumw2();
  response_bkg_reg2 = new TH1D("response_bkg_reg2",
			       "response",XBINS,0.,2.);
  response_bkg_reg2->Sumw2();
  response_bkg_reg3 = new TH1D("response_bkg_reg3",
			       "response",XBINS,0.,2.);
  response_bkg_reg3->Sumw2();
  response_bkg_reg4 = new TH1D("response_bkg_reg4",
			       "response",XBINS,0.,2.);
  response_bkg_reg4->Sumw2();
  

  // For photon id efficiencies
  ptphot_topocut_sig = new TH1D("ptphot_topocut_sig","ptphot_topocut_sig",
				XBINS/rb,&xbins_w[0]);
  ptphot_topocut_sig->Sumw2();
  ptphot_topocut_bkg = new TH1D("ptphot_topocut_bkg","ptphot_topocut_bkg",
				XBINS/rb,&xbins_w[0]);
  ptphot_topocut_bkg->Sumw2();
  ptphot_idcut_sig = new TH1D("ptphot_idcut_sig","ptphot_idcut_sig",
				XBINS/rb,&xbins_w[0]);
  ptphot_idcut_sig->Sumw2();
  ptphot_idcut_bkg = new TH1D("ptphot_idcut_bkg","ptphot_idcut_bkg",
				XBINS/rb,&xbins_w[0]);
  ptphot_idcut_bkg->Sumw2();

  sob_vs_eff_1 = new TH2D("sob_vs_eff_1","sob_vs_eff_1",100,0,1.,100,0.,5.);
  sob_vs_eff_2 = new TH2D("sob_vs_eff_2","sob_vs_eff_2",100,0,1.,100,0.,5.);
  sob_vs_eff_3 = new TH2D("sob_vs_eff_3","sob_vs_eff_3",100,0,1.,100,0.,5.);
  sob_vs_eff_4 = new TH2D("sob_vs_eff_4","sob_vs_eff_4",100,0,1.,100,0.,5.);
  
  nn_reg1_sig = new TH1D("nn_reg1_sig","nn_reg1_sig",400,-1.,1.1);
  nn_reg2_sig = new TH1D("nn_reg2_sig","nn_reg2_sig",400,-1.,1.1);
  nn_reg3_sig = new TH1D("nn_reg3_sig","nn_reg3_sig",400,-1.,1.1);
  nn_reg4_sig = new TH1D("nn_reg4_sig","nn_reg4_sig",400,-1.,1.1);
  nn_reg1_bkg = new TH1D("nn_reg1_bkg","nn_reg1_bkg",400,-1.,1.1);
  nn_reg2_bkg = new TH1D("nn_reg2_bkg","nn_reg2_bkg",400,-1.,1.1);
  nn_reg3_bkg = new TH1D("nn_reg3_bkg","nn_reg3_bkg",400,-1.,1.1);
  nn_reg4_bkg = new TH1D("nn_reg4_bkg","nn_reg4_bkg",400,-1.,1.1);

}


void gjet_response::ResetHistos()
{
  ptmean_mix->Reset();
  ptmean_sig->Reset();
  ptmean_bkg->Reset();

  ptphot_nocut->Reset();
  ptphot_mix->Reset();
  response_nocut->Reset();
  response_mix->Reset();
  responsevspt_nocut->Reset();
  responsevspt_mix->Reset();
  response_mix_reg1->Reset();
  response_mix_reg2->Reset();
  response_mix_reg3->Reset();
  response_mix_reg4->Reset();

  ptphot_nocut_sig->Reset();
  ptphot_sig->Reset();
  response_nocut_sig->Reset();
  response_sig->Reset();
  responsevspt_nocut_sig->Reset();
  responsevspt_sig->Reset();
  response_nocut_sig_reg1->Reset();
  response_nocut_sig_reg2->Reset();
  response_nocut_sig_reg3->Reset();
  response_nocut_sig_reg4->Reset();
  response_sig_reg1->Reset();
  response_sig_reg2->Reset();
  response_sig_reg3->Reset();
  response_sig_reg4->Reset();

  ptphot_nocut_bkg->Reset();
  ptphot_bkg->Reset();
  response_nocut_bkg->Reset();
  response_bkg->Reset();
  responsevspt_nocut_bkg->Reset();
  responsevspt_bkg->Reset();
  response_bkg_reg1->Reset();
  response_bkg_reg2->Reset();
  response_bkg_reg3->Reset();
  response_bkg_reg4->Reset();

  ptphot_topocut_sig->Reset();
  ptphot_topocut_bkg->Reset();
  ptphot_idcut_sig->Reset();
  ptphot_idcut_bkg->Reset();
}

void gjet_response::Optimum()
{
  BookHistos();
  for (int j = 0; j != 11; ++j){
    for (int i = 0; i != 21; ++i){

      cut2jet = 0. + double(j)*3./100;    
      cutnn = 0.9 + double(i)/200;    
      ResetHistos();
      Loop();
      double S1_bef = response_nocut_sig_reg1->Integral();
      double S2_bef = response_nocut_sig_reg2->Integral();
      double S3_bef = response_nocut_sig_reg3->Integral();
      double S4_bef = response_nocut_sig_reg4->Integral();
      double S1 = response_sig_reg1->Integral();
      double B1 = response_bkg_reg1->Integral();
      double S2 = response_sig_reg2->Integral();
      double B2 = response_bkg_reg2->Integral();
      double S3 = response_sig_reg3->Integral();
      double B3 = response_bkg_reg3->Integral();
      double S4 = response_sig_reg4->Integral();
      double B4 = response_bkg_reg4->Integral();
      double eff1 = S1/S1_bef;
      double eff2 = S2/S2_bef;
      double eff3 = S3/S3_bef;
      double eff4 = S4/S4_bef;
      sob_vs_eff_1->Fill(eff1,S1/B1);
      sob_vs_eff_2->Fill(eff2,S2/B2);
      sob_vs_eff_3->Fill(eff3,S3/B3);
      sob_vs_eff_4->Fill(eff4,S4/B4);
      cout << "cutnn = " <<  cutnn << "; cut2jet = " << cut2jet << "; cutetajet = " << cutetajet << endl;
      cout << "S1/B1 = " << S1/B1 << "   " << "; eff1 = " << eff1 << endl;
      cout << "S2/B2 = " << S2/B2 << "   " << "; eff2 = " << eff2 << endl;
      cout << "S3/B3 = " << S3/B3 << "   " << "; eff3 = " << eff3 << endl;
      cout << "S4/B4 = " << S4/B4 << "   " << "; eff4 = " << eff4 << endl;
      cout << endl;
    }
  }
}

void gjet_response::Fit(bool arithmetic)
{
  gROOT->SetStyle("Plain"); // load cms_jes_style.C instead
  
  gStyle->SetPalette(1);
  if (drawStats) {
    gStyle->SetOptStat(1111111);  // Show overflow, underflow + SumOfWeights 
    gStyle->SetOptFit(111110); 
  } else {
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
  }
  //  gStyle->SetStatStyle(0); // for a completely transparent stat box

  gStyle->SetOptFile(1); 
  
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(0.3);
  gStyle->SetMarkerColor(1);

  // Where to put tick marks
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  
  gStyle->SetTitleBorderSize(0);  // no border around histogram title (font size can't be calculated anyways ...)
  
  gROOT->ForceStyle();
  gStyle->SetOptStat(0);
  char name[100];
  char nameout[100];
  sprintf(name,"%s","responsevspt");
  TCanvas *c0 = new TCanvas("c0","--c0--",472,0,800,800);
  TCanvas *c1 = new TCanvas("c1","--sig+mix--",472,0,800,800);
  TCanvas *c2 = new TCanvas("c2","--sig+bkg--",472,0,800,800);
  TCanvas *c3 = new TCanvas("c3","--purity--",472,0,800,800);
  TCanvas *c3s = new TCanvas("c3s","--purityStat--",472,0,800,800);
  TCanvas *c3b = new TCanvas("c3b","--S/B--",472,0,800,800);
  TCanvas *c4 = new TCanvas("c4","--efficiency--",472,0,800,800);
  TCanvas *c4b = new TCanvas("c4b","--effid--",472,0,800,800);
  TCanvas *c5 = new TCanvas("c5","--rejection--",472,0,800,800);
  TAxis *xaxis =  responsevspt_sig->GetXaxis();
  TAxis *yaxis =  responsevspt_sig->GetYaxis();
  int nbinx = responsevspt_sig->GetNbinsX();
  int nbiny = responsevspt_sig->GetNbinsY();
  
  TH1D *temphist_sig = new TH1D("temphist_sig","",
				nbiny,yaxis->GetXmin(),yaxis->GetXmax());
  temphist_sig->Sumw2();
  temphist_sig->SetMarkerStyle(8);
  temphist_sig->SetMarkerColor(kBlue);
  temphist_sig->SetMarkerSize(.7);
  temphist_sig->SetStats(0);
  temphist_sig->SetXTitle("p_{T}(jet) / p_{T}(#gamma)");
  
  TH1D *temphist_bkg = new TH1D("temphist_bkg","",
				nbiny,yaxis->GetXmin(),yaxis->GetXmax());
  temphist_bkg->Sumw2();
  temphist_bkg->SetMarkerStyle(8);
  temphist_bkg->SetMarkerColor(kBlue);
  temphist_bkg->SetMarkerSize(.7);
  temphist_bkg->SetStats(0);
  temphist_bkg->SetXTitle("p_{T}(jet) / p_{T}(#gamma)");
  
  TH1D *temphist_mix = new TH1D("temphist_mix","",
				nbiny,yaxis->GetXmin(),yaxis->GetXmax());
  temphist_mix->Sumw2();
  temphist_mix->SetMarkerStyle(8);
  temphist_mix->SetMarkerColor(kBlue);
  temphist_mix->SetMarkerSize(.7);
  temphist_mix->SetStats(0);
  temphist_mix->SetXTitle("p_{T}(jet) / p_{T}(#gamma)");
  
  const Double_t *xbins = xaxis->GetXbins()->GetArray();
  TH1D *gmean_sig = new TH1D("gmean_sig","gmean",nbinx,&xbins[0]);
  TH1D *gsigma_sig = new TH1D("gsigma_sig","gsigma",nbinx,&xbins[0]);
  TH1D *distrsum_sig = new TH1D("distrsum_sig","distrsum",nbinx,&xbins[0]);
  
  TH1D *gmean_bkg = new TH1D("gmean_bkg","gmean",nbinx,&xbins[0]);
  TH1D *gsigma_bkg = new TH1D("gsigma_bkg","gsigma",nbinx,&xbins[0]);
  
  TH1D *gmean_mix = new TH1D("gmean_mix","gmean",nbinx,&xbins[0]);
  TH1D *gsigma_mix = new TH1D("gsigma_mix","gsigma",nbinx,&xbins[0]);

  // Add the MC truth curve at the back and use for initial fit parameters
  TF1 *mctruth = new TF1("mctruth", _mctruthfunc.c_str(),_fitptmin,1000.);
  mctruth->SetLineWidth(1);
  mctruth->SetLineColor(14);
  mctruth->SetLineStyle(kDashed);

  // Old CSA07 truth
  TF1 *mctruth07 = 0;
  if (_mctruthcsa07!="") {
    mctruth07 = new TF1("mctruth07", _mctruthcsa07.c_str(),_fitptmin,1000.);
    mctruth07->SetLineWidth(1);
    mctruth07->SetLineColor(kBlue);
    mctruth07->SetLineStyle(kDotted);
  }

  
  TF1 *mcreso = new TF1("mcreso","sqrt([0]*[0]/(x*x)+[1]*[1]/x+[2]*[2])",
			_fitptmin,1000.);
  //mcreso->SetParameters(0., 1.239, 0.08923);
  mcreso->SetParameters(0., 1.164, 0.1017);

  for (int j = 1; j != nbinx+1; ++j){
    
    for(int i = 1; i != nbiny+1; ++i){
      
      double binvalue = responsevspt_sig->GetBinContent(j,i);
      double binerror = responsevspt_sig->GetBinError(j,i);
      temphist_sig->SetBinContent(i,binvalue);          
      temphist_sig->SetBinError(i,binerror);          
      binvalue = responsevspt_bkg->GetBinContent(j,i);
      binerror = responsevspt_bkg->GetBinError(j,i);
      temphist_bkg->SetBinContent(i,binvalue);          
      temphist_bkg->SetBinError(i,binerror);          
      binvalue = responsevspt_mix->GetBinContent(j,i);
      binerror = responsevspt_mix->GetBinError(j,i);
      temphist_mix->SetBinContent(i,binvalue);          
      temphist_mix->SetBinError(i,binerror);          
    }
    double ptmin = responsevspt_sig->GetBinLowEdge(j);
    double ptmax = responsevspt_sig->GetBinLowEdge(j+1);

    double pt = ptmean_sig->GetBinContent(j);
    double xmin = _lowptreco / ptmin;
    double xav = _lowptreco / pt;

    Double_t params_sig[3] = {1000.,1.,.1};
    Double_t params_bkg[3] = {1000.,1.,.1};
    Double_t params_mix[3] = {1000.,1.,.1};
    TF1 *gaus_sig = new TF1(Form("gaus_sig_%04d_%04d",int(ptmin),int(ptmax)),
			    "gaus",xmin,2.);
    gaus_sig->SetParameters(params_sig);
    gaus_sig->SetLineColor(kRed);
    //gaus_sig->SetParLimits(0, 0., 10000.);
    gaus_sig->SetParLimits(1, 0., 2.);
    gaus_sig->SetParLimits(2, 0.00, 2.);
    gaus_sig->SetParLimits(0, 0.,temphist_sig->GetSum()/5.);
    temphist_sig->Fit(gaus_sig,"mllq","pe");    
    temphist_sig->Fit(gaus_sig,"mllq","pe",
		      gaus_sig->GetParameter(1)-gaus_sig->GetParameter(2),
		      gaus_sig->GetParameter(1)+gaus_sig->GetParameter(2));
    temphist_sig->Fit(gaus_sig,"mllq","pe",
		      gaus_sig->GetParameter(1)-gaus_sig->GetParameter(2),
		      gaus_sig->GetParameter(1)+2*gaus_sig->GetParameter(2));
    temphist_sig->Fit(gaus_sig,"mllq","pe",
		      gaus_sig->GetParameter(1)-gaus_sig->GetParameter(2),
		      gaus_sig->GetParameter(1)+3*gaus_sig->GetParameter(2));

    TF1 *gsig = new TF1(Form("gsig_%04d_%04d",int(ptmin),int(ptmax)),
			"gaus",xmin,2.);
    double Rexp = mctruth->Eval(pt);
    double Sexp = mcreso->Eval(pt);
    gsig->SetParameters(1000., Rexp, Sexp);
    gsig->SetParLimits(1, Rexp*0.9, Rexp*1.1);
    gsig->SetParLimits(2, Sexp*0.8, Sexp*1.2);
    temphist_sig->Fit(gsig,"MLLQRB+");

    curdir = gDirectory;

    c0->cd();
    temphist_sig->Draw();
    sprintf(nameout,"%s%d%s","tempfits/tempfit_sig_",j,".eps");
    c0->SaveAs(nameout); 

    hFitDir->cd();
    temphist_sig->Write(Form("Sig_%04d_%04d",int(ptmin),int(ptmax)));
    curdir->cd();
    {
      double mean = gaus_sig->GetParameter(1);
      double sigma = gaus_sig->GetParameter(2);
      double errmean = gaus_sig->GetParError(1);
      double errsigma = gaus_sig->GetParError(2);

      if (arithmetic) {

	mean = temphist_sig->GetMean();
	errmean = temphist_sig->GetMeanError();
	sigma = temphist_sig->GetRMS();
	errsigma = temphist_sig->GetRMSError();

	if (_lowptbias && xav > -0.9) {
	  
	  double bm, bs;
	  //calcLowPtBias(gaus_sig, xav, bm, bs);
	  calcLowPtBias(gsig, xav, bm, bs);
	  
	  cout << Form("pt %d-%d: bm=%1.3g "
		       "(rms=%1.3g mean=%1.3g / sig=%1.3g mu=%1.3g)\n",
		       int(ptmin),int(ptmax),bm,sigma,mean,
		       gaus_sig->GetParameter(2),gaus_sig->GetParameter(1));
	  mean *= bm;
	  errmean *= max(1., bm);
	  sigma = sqrt((sigma*sigma + mean/bm*mean/bm)*bs*bs - mean*mean);
	  errsigma *= max(1., bs);
	  cout << Form("pt %d-%d: bs=%1.3g "
		       "(rms=%1.3g mean=%1.3g / sig=%1.3g mu=%1.3g)\n",
		       int(ptmin),int(ptmax),bs,sigma,mean,
		       gaus_sig->GetParameter(2),gaus_sig->GetParameter(1));
	}
      } // if (arithmetic)

      gmean_sig->SetBinContent(j,mean);
      gsigma_sig->SetBinContent(j,sigma);
      gmean_sig->SetBinError(j,errmean);
      gsigma_sig->SetBinError(j,errsigma);
    }

    double numevents = temphist_sig->Integral();
    distrsum_sig->SetBinContent(j,numevents);

    /*
    if(temphist_sig->Integral()>0)
      cout << temphist_sig->Integral()<< "   "
	   << gaus_sig->GetParameter(2)/sqrt(temphist_sig->Integral()) << "   "
	   << gaus_sig->GetParError(1) << endl;
    */
    gaus_sig->Delete();
    
    TF1 *gaus_mix = new TF1(Form("gaus_mix_%04d_%04d",int(ptmin),int(ptmax)),
			    "gaus",xmin,2.);//-1.,1.);
    gaus_mix->SetParameters(params_mix);
    gaus_mix->SetLineColor(kRed);
    gaus_mix->SetParLimits(0, 0., temphist_mix->GetSum()*2.);
    gaus_mix->SetParLimits(1, 0., 2.);
    gaus_mix->SetParLimits(2, 0.00, 2.);
    gaus_mix->SetParameter(0,temphist_mix->GetSum()/10.);
    temphist_mix->Fit(gaus_mix,"mq","pe"); // Need to drop LL to avoid 'nan'
    //temphist_mix->Fit(gaus_mix,"mllq","pe");    
    temphist_mix->Fit(gaus_mix,"mllq","pe",
		      gaus_mix->GetParameter(1)-gaus_mix->GetParameter(2),
		      gaus_mix->GetParameter(1)+gaus_mix->GetParameter(2));
    temphist_mix->Fit(gaus_mix,"mllq","pe",
		      gaus_mix->GetParameter(1)-gaus_mix->GetParameter(2),
		      gaus_mix->GetParameter(1)+2*gaus_mix->GetParameter(2));
    temphist_mix->Fit(gaus_mix,"mllq","pe",
		      gaus_mix->GetParameter(1)-gaus_mix->GetParameter(2),
		      gaus_mix->GetParameter(1)+3*gaus_mix->GetParameter(2));

    TF1 *gmix = new TF1(Form("gmix_%04d_%04d",int(ptmin),int(ptmax)),
			"gaus",xmin,2.);
    gmix->SetParameters(1000., Rexp, Sexp);
    gmix->SetParLimits(1, Rexp*0.9, Rexp*1.1);
    gmix->SetParLimits(2, Sexp*0.8, Sexp*1.2);
    temphist_mix->Fit(gmix,"MLLQRB+");

    curdir = gDirectory;

    c0->cd();
    temphist_mix->Draw();
    sprintf(nameout,"%s%d%s","tempfits/tempfit_mix_",j,".eps");
    c0->SaveAs(nameout); 

    hFitDir->cd();
    temphist_mix->Write(Form("Mix_%04d_%04d",int(ptmin),int(ptmax)));

    curdir->cd();
    {
      double mean = gaus_mix->GetParameter(1);
      double sigma = gaus_mix->GetParameter(2);
      double errmean = gaus_mix->GetParError(1);
      double errsigma = gaus_mix->GetParError(2);

      if (arithmetic) {

	mean = temphist_mix->GetMean();
	errmean = temphist_mix->GetMeanError();
	sigma = temphist_mix->GetRMS();
	errsigma = temphist_mix->GetRMSError();
	
	if (_lowptbias && xav > -0.9) {

	  double bm, bs;
	  //calcLowPtBias(gaus_mix, xav, bm, bs);
	  calcLowPtBias(gmix, xav, bm, bs);

	  cout << Form("pt %d-%d: bm=%1.3g "
		       "(rms=%1.3g mean=%1.3g / sig=%1.3g mu=%1.3g)\n",
		       int(ptmin),int(ptmax),bm,sigma,mean,
		       gaus_mix->GetParameter(2),gaus_mix->GetParameter(1));
	  mean *= bm;
	  errmean *= max(1., bm);
	  sigma = sqrt((sigma*sigma + mean/bm*mean/bm)*bs*bs - mean*mean);
	  errsigma *= max(1., bs);
	  cout << Form("pt %d-%d: bs=%1.3g "
		       "(rms=%1.3g mean=%1.3g / sig=%1.3g mu=%1.3g)\n",
		       int(ptmin),int(ptmax),bs,sigma,mean,
		       gaus_mix->GetParameter(2),gaus_mix->GetParameter(1));
	}
      } // if (arithmetic)

      gmean_mix->SetBinContent(j,mean);
      gsigma_mix->SetBinContent(j,sigma);
      gmean_mix->SetBinError(j,errmean);
      gsigma_mix->SetBinError(j,errsigma);
    }
    gaus_mix->Delete();

    TF1 *gaus_bkg = new TF1(Form("gaus_bkg_%04d_%04d",int(ptmin),int(ptmax)),
			    "gaus",xmin,2.);//-1.,1.);
    gaus_bkg->SetParameters(params_bkg);
    gaus_bkg->SetLineColor(kRed);
    gaus_bkg->SetParLimits(0, 0., temphist_bkg->GetSum()*2.);
    gaus_bkg->SetParLimits(1,0 , 2.);
    gaus_bkg->SetParLimits(2, 0.00, 2.);
    gaus_bkg->SetParameter(0,temphist_bkg->GetSum()/10.);
    temphist_bkg->Fit(gaus_bkg,"mllq","pe");

    curdir = gDirectory;

    c0->cd();
    temphist_bkg->Draw();
    sprintf(nameout,"%s%d%s","tempfits/tempfit_bkg_",j,".eps");
    c0->SaveAs(nameout); 

    hFitDir->cd();
    temphist_mix->Write(Form("Bkg_%04d_%04d",int(ptmin),int(ptmax)));
    curdir->cd();
    {
      double mean = gaus_bkg->GetParameter(1);
      double sigma = gaus_bkg->GetParameter(2);
      double errmean = gaus_bkg->GetParError(1);
      double errsigma = gaus_bkg->GetParError(2);
      
      if (arithmetic) {

	double err = temphist_bkg->GetMeanError();
	double n = temphist_bkg->Integral();
	if (n < 3) err = 0.3/sqrt(max(n,1.));
	if (err<0.01) err = 1./sqrt(ptmin);
	mean = temphist_bkg->GetMean();
	errmean =  err;
	sigma = temphist_bkg->GetRMS();
	errsigma = temphist_bkg->GetRMSError();

	if (_lowptbias && xav > -0.9) {
	  
	  double bm, bs;
	  calcLowPtBias(gaus_bkg, xav, bm, bs);
	  
	  mean *= bm;
	  errmean *= max(1., bm);
	  sigma = sqrt((sigma*sigma + mean/bm*mean/bm)*bs*bs - mean*mean);
	  errsigma *= max(1., bs);
	}
      } // if (arithmetic)

      gmean_bkg->SetBinContent(j,mean);
      gsigma_bkg->SetBinContent(j,sigma);
      gmean_bkg->SetBinError(j,errmean);
      gsigma_bkg->SetBinError(j,errsigma);
    }
    gaus_bkg->Delete();
    
  }

  // Create graphs with correct bin x-axis means (<p_T^phot>)
  TGraphErrors *gr_sig = new TGraphErrors(0);
  for (int i = 1; i != gmean_sig->GetNbinsX()+1; ++i) {

    double x = ptmean_sig->GetBinContent(i);
    double ex = ptmean_sig->GetBinError(i);
    double y = gmean_sig->GetBinContent(i);
    double ey = gmean_sig->GetBinError(i);
    int n = gr_sig->GetN();
    if (x !=0 && ex != 0 && y != 0 && ey != 0) {
      gr_sig->SetPoint(n, x, y);
      gr_sig->SetPointError(n, ex, ey);
    }
  }
  gr_sig->SetName("gr_sig");
  gr_sig->SetMarkerStyle(kOpenCircle);
  gr_sig->SetMarkerSize(1.);
  gr_sig->SetMarkerColor(kRed);
  gr_sig->SetLineColor(kRed);

  TGraphErrors *gr_bkg = new TGraphErrors(0);
  for (int i = 1; i != gmean_bkg->GetNbinsX()+1; ++i) {

    double x = ptmean_bkg->GetBinContent(i);
    double ex = ptmean_bkg->GetBinError(i);
    double y = gmean_bkg->GetBinContent(i);
    double ey = gmean_bkg->GetBinError(i);
    int n = gr_bkg->GetN();
    if (x !=0 && ex != 0 && y != 0 && ey != 0) {
      gr_bkg->SetPoint(n, x, y);
      gr_bkg->SetPointError(n, ex, ey);
    }
  }
  gr_bkg->SetName("gr_bkg");
  gr_bkg->SetMarkerStyle(kFullSquare);
  gr_bkg->SetMarkerSize(1.);
  gr_bkg->SetMarkerColor(kBlue);
  gr_bkg->SetLineColor(kBlue);

  TGraphErrors *gr_mix = new TGraphErrors(0);
  for (int i = 1; i != gmean_mix->GetNbinsX()+1; ++i) {

    double x = ptmean_mix->GetBinContent(i);
    double ex = ptmean_mix->GetBinError(i);
    double y = gmean_mix->GetBinContent(i);
    double ey = gmean_mix->GetBinError(i);
    int n = gr_mix->GetN();
    if (x !=0 && ex != 0 && y != 0 && ey != 0) {
      gr_mix->SetPoint(n, x, y);
      gr_mix->SetPointError(n, ex, ey);
    }
  }
  gr_mix->SetName("gr_mix");
  gr_mix->SetMarkerStyle(kFullCircle);
  gr_mix->SetMarkerSize(1.);
  gr_mix->SetMarkerColor(kBlack);
  gr_mix->SetLineColor(kBlack);
  

  // Start drawing results
  //c0->SetLogx();
  //c1->SetLogx();
  c0->cd();
  
  //  gmean_sig->SetAxisRange(40.,700.);
  gmean_sig->SetTitle("");
  gmean_sig->SetMinimum(0.3);
  gmean_sig->SetMaximum(1.1);
  gmean_sig->SetMarkerStyle(24);
  gmean_sig->SetMarkerSize(1.);
  gmean_sig->SetMarkerColor(kRed);
  gmean_sig->SetLineColor(kRed);
  gmean_sig->SetXTitle("p_{T} [GeV/c]");
  gmean_sig->GetXaxis()->SetNoExponent();
  gmean_sig->GetXaxis()->SetMoreLogLabels();
  string title = "p_{T}(jet) / p_{T}(#gamma) ("+_idtype+")"; 
  //gmean_sig->SetYTitle("#frac{p_{T}(jet)}{p_{T}(#gamma)}");
  gmean_sig->SetYTitle(title.c_str());
  gmean_sig->SetTitleOffset(.8,"Y");
  gmean_sig->Draw("pe");
  responsevspt_sig->Draw("same");
  gmean_sig->Draw("pesame");

  c0->SaveAs(arithmetic ? "ameanvspt_overlap_sig.eps"
	     : "gmeanvspt_overlap_sig.eps");
  gmean_sig->Draw("pe");
  c0->SaveAs(arithmetic ? "ameanvspt_sig.eps" : "gmeanvspt_sig.eps");

  TF1 *fres = new TF1("fres","sqrt([0]*[0]/(x*x)+[1]*[1]/x+[2]*[2])",
		      _fitptmin,1000.);
  fres->SetParameters(1.,1.,0.1);
  gsigma_sig->Fit(fres,"QR");

  gsigma_sig->SetMinimum(0);
  gsigma_sig->SetMaximum(0.5);//2.);
  gsigma_sig->SetMarkerStyle(8);
  gsigma_sig->SetMarkerColor(kRed);
  gsigma_sig->GetXaxis()->SetNoExponent();
  gsigma_sig->GetXaxis()->SetMoreLogLabels();
  gsigma_sig->Draw("pe");
  fres->Draw("same");

  c0->SaveAs(arithmetic ? "armsvspt_sig.eps" : "gsigmavspt_sig.eps");


  c0->SetLogy(1);

  TF1 *fx = new TF1("fx","[0]*pow(x,-[1])*exp(-[2]*x)",
		    30.,700.);
  fx->SetParameters(7e10,3.,4e-3);
  //distrsum_sig->Fit(fx, "QRN");
  ptphot_sig->Fit(fx, "QRN");
  
  cout << Form("%1.4g*pow(x,-%1.4g)*exp(-%1.4g*x)", fx->GetParameter(0),
	       fx->GetParameter(1), fx->GetParameter(2)) << endl;

  distrsum_sig->SetTitle("");
  distrsum_sig->SetXTitle("p_{T} [GeV/c]");
  distrsum_sig->GetXaxis()->SetNoExponent();
  distrsum_sig->GetXaxis()->SetMoreLogLabels();
  distrsum_sig->SetYTitle("Nev/fb^{-1}");
  distrsum_sig->SetTitleOffset(1.3,"Y");
  distrsum_sig->Draw("pe");
  fx->Draw("same");

  c0->SaveAs("xsecvspt_sig.eps");

  c0->SetLogx(1);
  c0->SaveAs("xsecvspt_sig_log.eps");
  c0->SetLogx(0);


  c0->SetLogy(0);
  c1->cd();
  c1->SetLeftMargin(0.15);
  c1->SetRightMargin(0.05);
  gmean_mix->SetTitle("");
  gmean_mix->SetMinimum(0.3);
  gmean_mix->SetMaximum(1.1);
  gmean_mix->SetMarkerStyle(8);
  gmean_mix->SetMarkerSize(1.);
  gmean_mix->SetXTitle("p_{T} [GeV/c]");
  gmean_mix->GetXaxis()->SetNoExponent();
  gmean_mix->GetXaxis()->SetMoreLogLabels();
  //gmean_mix->SetYTitle("p_{T}(jet) / p_{T}(#gamma)");
  gmean_mix->SetYTitle(title.c_str());
  gmean_mix->SetTitleOffset(1.5,"Y");//.8,"Y");
  gmean_mix->Draw("pe");

  c1->SaveAs(arithmetic ? "ameanvspt_mix.eps" : "gmeanvspt_mix.eps");

  // SIGNAL + MIX

  TLegendEntry *legge;
  TLegend *leg;
  //leg = new TLegend(0.45,0.15,0.8,0.35);
  leg = new TLegend(0.45,0.15,0.8,0.45, _algoname.c_str());
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.05);
  leg->SetFillColor(0);
  legge = leg->AddEntry(gr_sig, "#gamma jet only", "p");
  legge = leg->AddEntry(gr_mix, "#gamma jet + QCD bkg", "p");
  legge = leg->AddEntry(mctruth, "MC truth", "l");
  if (mctruth07) leg->AddEntry(mctruth07, "CSA07 truth", "l");
  // Do fits of response to these points
  TF1 *fsig = new TF1("fsig","[2]-[0]*pow(0.01*x,[1]-1)",_fitptmin,1000.);
  fsig->SetLineColor(kRed);
  fsig->SetLineWidth(1);
  fsig->SetParNames("a","m","c");
  fsig->SetParameters(0.62, 0.8, 1.);
  //gmean_sig->Fit(fsig, "QR");
  gr_sig->Fit(fsig, "QR");//"QRN");

  const int npar = 3;
  TMatrixD emats(npar, npar);
  gMinuit->mnemat(&emats[0][0], npar);

  //gmean_sig->GetListOfFunctions()->Add(fsig);
  TF1 *fmix = new TF1("fmix","[2]-[0]*pow(0.01*x,[1]-1)",_fitptmin,1000.);
  fmix->SetLineColor(kBlack);
  fmix->SetLineWidth(1);
  fmix->SetParNames("a","m","c");
  fmix->SetParameters(0.62, 0.8, 1.);
  //gmean_mix->Fit(fmix, "QR");
  gr_mix->Fit(fmix, "QR");//"QRN");
  //gmean_mix->GetListOfFunctions()->Add(fmix);
  //

  // Retrieve and factorize correlation matrix for L3Corr.cpp
  TMatrixD emat(npar, npar);
  gMinuit->mnemat(&emat[0][0], npar);

  TVectorD eigvs(npar);
  TMatrixD eigs(emats.EigenVectors(eigvs));

  // Print out the signal fit parameters to be used in L3Corr.cpp
  cout << "Signal sample:" << endl;
  cout << Form("_pstat[0] = %1.4g; _pstat[1] = %1.4g; _pstat[2] = %1.4g;",
	       fsig->GetParameter(0), fsig->GetParameter(1),
	       fsig->GetParameter(2)) << endl;

  cout << Form("_chi2 = %1.4g;", fsig->GetChisquare()/fsig->GetNDF()) << endl;

  double vs = sqrt(fabs(eigvs[0]));
  cout << Form( "_eig[0][0] = %1.4g; _eig[1][0] = %1.4g; _eig[2][0]  = %1.4g;",
		vs*eigs[0][0], vs*eigs[1][0], vs*eigs[2][0]) << endl;
  vs = sqrt(fabs(eigvs[1]));
  cout << Form( "_eig[0][1] = %1.4g; _eig[1][1] = %1.4g; _eig[2][1]  = %1.4g;",
		vs*eigs[0][1], vs*eigs[1][1], vs*eigs[2][1]) << endl;
  vs = sqrt(fabs(eigvs[2]));
  cout << Form( "_eig[0][2] = %1.4g; _eig[1][2] = %1.4g; _eig[2][2]  = %1.4g;",
		vs*eigs[0][2], vs*eigs[1][2], vs*eigs[2][2]) << endl;
  cout << endl;

  TVectorD eigv(npar);
  TMatrixD eig(emat.EigenVectors(eigv));
  //cout << "Eigenvectors" << endl;
  //eig.Print();
  //cout << "Eigenvalues" << endl;
  //eigv.Print();
  //cout << "Is eigenmatrix symmetric? " << eig.IsSymmetric() << endl;

  // Print out the mixed fit parameters to be used in L3Corr.cpp
  cout << "Mixed sample:" << endl;
  cout << Form("_pstat[0] = %1.4g; _pstat[1] = %1.4g; _pstat[2] = %1.4g;",
	       fmix->GetParameter(0), fmix->GetParameter(1),
	       fmix->GetParameter(2)) << endl;

  cout << Form("_chi2 = %1.4g;", fmix->GetChisquare()/fmix->GetNDF()) << endl;

  double v = sqrt(fabs(eigv[0]));
  cout << Form( "_eig[0][0] = %1.4g; _eig[1][0] = %1.4g; _eig[2][0]  = %1.4g;",
		v*eig[0][0], v*eig[1][0], v*eig[2][0]) << endl;
  v = sqrt(fabs(eigv[1]));
  cout << Form( "_eig[0][1] = %1.4g; _eig[1][1] = %1.4g; _eig[2][1]  = %1.4g;",
		v*eig[0][1], v*eig[1][1], v*eig[2][1]) << endl;
  v = sqrt(fabs(eigv[2]));
  cout << Form( "_eig[0][2] = %1.4g; _eig[1][2] = %1.4g; _eig[2][2]  = %1.4g;",
		v*eig[0][2], v*eig[1][2], v*eig[2][2]) << endl;
  cout << endl;

  if (drawStats) gStyle->SetOptFit();
  else gStyle->SetOptFit(0);
  gmean_mix->Draw("axis");//"pe");
  mctruth->Draw("same");
  if (mctruth07) mctruth07->Draw("same");
  gr_mix->Draw(drawStats ? "pesames" : "pesame");
  c1->Update(); // to get the new stats box
  TPaveStats *stats;
  if (drawStats) {
    stats = (TPaveStats*)gPad->GetPrimitive("stats");
    assert(stats);
    stats->SetName("stats_mix");
  }
  //gmean_sig->Draw("pesames");
  gr_sig->Draw(drawStats ? "pesames" : "pesame");
  if (drawStats) {
    c1->Update(); // to get the new stats box
    stats = (TPaveStats*)gPad->GetPrimitive("stats");
    assert(stats);
    stats->SetLineColor(kRed);
    stats->SetTextColor(kRed);
    stats->SetX1NDC(stats->GetX1NDC()-0.4);
    stats->SetX2NDC(stats->GetX2NDC()-0.4);
    stats->SetName("stats_sig");
  }
  leg->Draw();

  c1->SaveAs(arithmetic ? "ameanvspt_sig+mix.eps" : "gmeanvspt_sig+mix.eps");
  // Same in log scale
  gmean_mix->SetMaximum(1.0);
  c1->SetLogx();
  c1->Update();
  c1->SaveAs(arithmetic ? "ameanvspt_sig+mix_log.eps"
	     : "gmeanvspt_sig+mix_log.eps");
  gmean_mix->SetMaximum(1.1);
  c0->cd();

  // SIGNAL + BACKGROUND
  c2->cd();
  c2->SetLeftMargin(0.15);
  c2->SetRightMargin(0.05);
  TLegend *legsb;
  //legsb = new TLegend(0.45,0.15,0.8,0.35);
  legsb = new TLegend(0.45,0.15,0.8,0.45, _algoname.c_str());
  legsb->SetFillStyle(kNone); legsb->SetBorderSize(0);
  legsb->SetTextSize(0.05);
  legsb->AddEntry(gr_sig, "#gamma jet", "p");
  legsb->AddEntry(gr_bkg, "QCD bkg", "p");
  legsb->AddEntry(mctruth, "MC truth", "l");
  // Do fit of response to the background (signal already done)
  TF1 *fbkg = new TF1("fbkg","[2]-[0]*pow(0.01*x,[1]-1)",_fitptmin,1000.);
  fbkg->SetLineColor(kBlue);
  fbkg->SetLineWidth(1);
  fbkg->SetParNames("a","m","c");
  fbkg->SetParameters(0.62, 0.8, 1.);
  fbkg->SetParLimits(1,0,10);
  gr_bkg->Fit(fbkg, (drawStats ? "QR" : "QRN"));
  // 
  // Background bias defined in deltac.C
  // I wish this would have worked directly... only available since ROOT 5.XX
  //TF1 *rbkg = new TF1("rbkg", bkgobj, &bkgbias::bkgresp,
  //	      50., 1000., 3,"bkgbias","bkgresp");
  // Now need to make both bkgresp and bkgobj globally visible, ugh.
  TF1 *rbkg = new TF1("rbkg", bkgresp, _fitptmin, 1000., 3);
  rbkg->SetParameters(fsig->GetParameter(0),fsig->GetParameter(1),
		      fsig->GetParameter(2));
  rbkg->SetLineColor(kBlue);
  //
  if (drawStats) gStyle->SetOptFit();
  gmean_mix->Draw("axis");
  mctruth->Draw("same");
  gr_bkg->Draw(drawStats ? "pesames" : "pesame");
  rbkg->Draw("same");
  if (drawStats) {
    c2->Update(); // to get the new stats box
    stats = (TPaveStats*)gPad->GetPrimitive("stats"); assert(stats);
    stats->SetLineColor(kBlue);
    stats->SetTextColor(kBlue);
    stats->SetName("stats_bkg");
  }
  gr_sig->Draw("pesames");
  // NB: stats_sig was already moved to the left earlier, no need to repeat
  legsb->Draw();

  c2->SaveAs(arithmetic ? "ameanvspt_sig+bkg.eps" : "gmeanvspt_sig+bkg.eps");
  c0->cd();

  gmean_bkg->SetMinimum(.5);
  gmean_bkg->SetMaximum(2.);
  gmean_bkg->SetMarkerStyle(8);
  gmean_bkg->SetMarkerColor(kRed);
  gmean_bkg->SetLineColor(kRed);
  gmean_bkg->GetXaxis()->SetNoExponent();
  gmean_bkg->GetXaxis()->SetMoreLogLabels();
  gmean_bkg->Draw("pe");
  c0->SaveAs(arithmetic ? "ameanvspt_bkg.eps" : "gmeanvspt_bkg.eps");

  // PURITY
  c3->cd();
  c3->SetLeftMargin(0.15);
  c3->SetRightMargin(0.05);
  purity = (TH1D*)ptphot_mix->Clone("purity");
  purity->Divide(ptphot_sig, ptphot_mix, 1., 1., "");//"B");
  purity->SetMinimum(0.);
  purity->SetMaximum(1.);
  purity->SetTitle("");
  purity->SetXTitle("p_{T} [GeV/c]");
  purity->GetXaxis()->SetMoreLogLabels();
  purity->GetXaxis()->SetNoExponent();
  purity->SetYTitle("#gamma jet purity");
  purity->GetYaxis()->SetTitleOffset(1.5);

  TF1 *fpurity = new TF1("fpurity","[0] + log(0.01*x)*([1]+log(0.01*x)*[2])",
			 0., 670.);//1000.);
  fpurity->SetParameters(0.5,0.2,-0.01);
  //fpurity->FixParameter(2, 0.);
  purity->Fit(fpurity,"QRN");

  purity->Draw();
  if (drawStats) fpurity->Draw("same");
  else {
    purity->SetLineWidth(2);
    purity->SetLineColor(kBlack);
  }

  TMatrixD pemat(3,3);
  gMinuit->mnemat(&pemat[0][0],3);
  cout << "Purity:" << endl;
  cout << Form("P = %1.4g + x*(%1.4g + x*%1.4g);",
	       fpurity->GetParameter(0), fpurity->GetParameter(1),
	       fpurity->GetParameter(2)) << endl;
  cout << Form("stat = sqrt(%1.4g + 2*%1.4g*x + 2*%1.4g*x*x\n"
		"+ %1.4g*x*x + 2*%1.4g*x*x*x\n"
	       "+ %1.4g*x*x*x*x);\n",
	       pemat[0][0],pemat[0][1],pemat[0][2],
	       pemat[1][1],pemat[1][2],pemat[2][2]) << endl;

  c3->SaveAs("purity.eps");

  c3->SetLogx();
  c3->SaveAs("purity_log.eps");

  // Wider bins
  int nwide = 3;
  TH1D *purity_w = (TH1D*)purity->Clone(Form("%s_w",purity->GetName()));
  TH1D *ptphot_sig_w = (TH1D*)ptphot_sig->Clone("w1");
  TH1D *ptphot_mix_w = (TH1D*)ptphot_mix->Clone("w2");
  ptphot_sig_w->Rebin(nwide);
  ptphot_mix_w->Rebin(nwide);
  purity_w->Rebin(nwide);
  purity_w->Divide(ptphot_sig_w, ptphot_mix_w, 1., 1., "");//"B");
  delete ptphot_sig_w;
  delete ptphot_mix_w;

  purity_w->SetMinimum(0.);
  purity_w->SetMaximum(1.);
  purity_w->Draw();
  c3->SetLogx(0);
  c3->Update();
  c3->SaveAs("purity_wide.eps");
  c0->cd();

  // Purity statistical uncertainty from the top of the error bars
  c3s->cd();
  TGraphErrors *puritystat = new TGraphErrors(0);
  for (int i = 1; i != purity->GetNbinsX()+1; ++i) {
    puritystat->SetPoint(i-1, purity->GetBinCenter(i),
			 purity->GetBinError(i));
    double err = ptphot_bkg->GetBinError(i) / ptphot_bkg->GetBinContent(i);
    //double neff = 1. / (err * err);
    //err = fabs(neff / (neff + 1.) - 1.);
    puritystat->SetPointError(i-1, 0.5*purity->GetBinWidth(i), err);
  }
  //TF1 *fps = new TF1("fpuritystat","sqrt([0]+log(0.01*x)*([1]"
  //	     "+log(0.01*x)*([2]+log(0.01*x)*([3]"
  //	     "+log(0.01*x)*[4]))))",0.,670.);
  //fps->SetParameters(pemat[0][0], 2*pemat[0][1], 2*pemat[0][2]+pemat[1][1],
  //		     2*pemat[1][2], pemat[2][2]);
//fps->FixParameter(4,0.);
//fps->FixParameter(3,0.);
  TF1 *fps = new TF1("fpuritystat", "[0]", 0., 670.);
  puritystat->Fit(fps, "QRNW");
  //cout << Form("stat = sqrt(%1.4g + %1.4g*x + %1.4g*x*x\n"
  //       "+ %1.4g*x*x*x + %1.4g*x*x*x*x); // top of err.bars\n",
  //       fps->GetParameter(0),fps->GetParameter(1),fps->GetParameter(2),
  //       fps->GetParameter(3),fps->GetParameter(4));
  cout <<  Form("stat = %1.4g; // top of err.bars\n",
		fps->GetParameter(0)) << endl;

  puritystat->Draw("axis p");
  puritystat->GetYaxis()->SetRangeUser(0., 0.30);
  fps->Draw("same");
  c3s->Update();
  c3s->SaveAs("puritystat.eps");
  c0->cd();

  // S/B
  c3b->cd();
  c3b->SetLeftMargin(0.15);
  c3b->SetRightMargin(0.05);
  c3b->SetLogy();
  sovb = (TH1D*)ptphot_bkg->Clone("sovb");
  sovb->Divide(ptphot_sig, ptphot_bkg, 1., 1., "");
  sovb->SetMinimum(1e-1);//0.01);
  sovb->SetMaximum(1e2);//10000.);
  sovb->SetTitle("");
  sovb->SetXTitle("p_{T} [GeV/c]");
  sovb->GetXaxis()->SetMoreLogLabels();
  sovb->GetXaxis()->SetNoExponent();
  sovb->SetYTitle("#gamma jet S/B");
  sovb->GetYaxis()->SetTitleOffset(1.5);

  TF1 *fsovb = new TF1("fsb","[0] + log(0.01*x)*([1]+log(0.01*x)*[2])",
		       0., 670.);//1000.);
  fsovb->SetParameters(10.,1.,-0.1);
  sovb->Fit(fsovb,"QRN");

  sovb->Draw();
  if (drawStats) fsovb->Draw("same");
  else {
    sovb->SetLineWidth(2);
    sovb->SetLineColor(kBlack);
  }

  c3b->SaveAs("sovb.eps");

  c3b->SetLogx();
  c3b->SaveAs("sovb_log.eps");

  // Wider bins
  //int nwide = 3;
  TH1D *sovb_w = (TH1D*)sovb->Clone(Form("%s_w", sovb->GetName()));
  ptphot_sig_w = (TH1D*)ptphot_sig->Clone("w1");
  TH1D *ptphot_bkg_w = (TH1D*)ptphot_bkg->Clone("w2");
  ptphot_sig_w->Rebin(nwide);
  ptphot_bkg_w->Rebin(nwide);
  sovb_w->Rebin(nwide);
  sovb_w->Divide(ptphot_sig_w, ptphot_bkg_w, 1., 1., "");//"B");
  delete ptphot_sig_w;
  delete ptphot_bkg_w;

  sovb_w->SetMinimum(1e-1);
  sovb_w->SetMaximum(1e2);
  sovb_w->Draw();
  c3b->SetLogx(0);
  c3b->Update();
  c3b->SaveAs("sovb_wide.eps");
  c0->cd();

  // EFFICIENCY
  c4->cd();
  c4->SetLeftMargin(0.15);
  c4->SetRightMargin(0.05);
  efficiency = (TH1D*)ptphot_sig->Clone("efficiency");
  efficiency->Divide(ptphot_sig, ptphot_nocut_sig, 1., 1., "B");
  efficiency->SetMinimum(0.);
  efficiency->SetMaximum(0.5);//1.);
  efficiency->SetTitle("");
  efficiency->SetXTitle("p_{T} [GeV/c]");
  efficiency->GetXaxis()->SetMoreLogLabels();
  efficiency->GetXaxis()->SetNoExponent();
  efficiency->SetYTitle("#gamma jet efficiency");
  efficiency->GetYaxis()->SetTitleOffset(1.5);

  TF1 *feff = new TF1("feff","[0] + log(0.01*x)*([1]+log(0.01*x)*[2])",
		      0., 1000.);
  feff->SetParameters(0.2,0.1,-0.01);
  //feff->FixParameter(2,0.);
  efficiency->Fit(feff,"QRN");

  efficiency->Draw();
  if (drawStats) feff->Draw("same");
  else {
    efficiency->SetLineColor(kRed);
    efficiency->SetLineWidth(2);
  }

  c4->SaveAs("efficiency.eps");

  c4->SetLogx();
  c4->SaveAs("efficiency_log.eps");

  // Wider bins
  //int nwide = 3;
  TH1D *efficiency_w =
    (TH1D*)efficiency->Clone(Form("%s_w", efficiency->GetName()));
  ptphot_sig_w = (TH1D*)ptphot_sig->Clone("w1");
  TH1D *ptphot_nocut_sig_w = (TH1D*)ptphot_nocut_sig->Clone("w2");
  ptphot_sig_w->Rebin(nwide);
  ptphot_nocut_sig_w->Rebin(nwide);
  efficiency_w->Rebin(nwide);
  efficiency_w->Divide(ptphot_sig_w, ptphot_nocut_sig_w, 1., 1., "");//"B");
  delete ptphot_sig_w;
  delete ptphot_nocut_sig_w;

  efficiency_w->SetMinimum(0.);
  efficiency_w->SetMaximum(0.5);
  efficiency_w->Draw();
  c4->SetLogx(0);
  c4->Update();
  c4->SaveAs("efficiency_wide.eps");
  c0->cd();



  // EFFICIENCY (PHOTON ID ONLY)
  c4b->cd();
  c4b->SetLeftMargin(0.15);
  c4b->SetRightMargin(0.05);
  effid1 = (TH1D*)ptphot_sig->Clone("effid1");
  effid1->Divide(ptphot_idcut_sig, ptphot_nocut_sig, 1., 1., "B");
  effid1->SetMinimum(0.);
  effid1->SetMaximum(1.);
  effid1->SetTitle("");
  effid1->SetXTitle("p_{T} [GeV/c]");
  string effid_title = _idtype + " photon ID efficiency w.r.t. loose|NN";
  effid1->SetYTitle(effid_title.c_str());
  effid1->GetYaxis()->SetTitleOffset(1.5);
  effid2 = (TH1D*)ptphot_sig->Clone("effid2");
  effid2->Divide(ptphot_sig, ptphot_topocut_sig, 1., 1., "B");

  TF1 *feffid = new TF1("feffid","[0] + log(0.01*x)*([1]+log(0.01*x)*[2])",
		      0., 1000.);
  feffid->SetParameters(0.2,0.1,-0.01);
  effid1->Fit(feffid,"QRN");

  TLegend *legid = new TLegend(0.5,0.3,0.9,0.5,"","brNDC");
  legid->SetBorderSize(0);
  legid->SetFillStyle(kNone);
  legid->AddEntry(effid1,"before topo cuts", "L");
  legid->AddEntry(effid2,"after topo cuts", "L");

  effid1->Draw();
  effid2->Draw("same");
  if (drawStats) feffid->Draw("same");
  else {
    effid1->SetLineColor(kRed);
    effid1->SetLineWidth(2);
  }
  legid->Draw("same");

  c4b->SaveAs("effid.eps");
  c0->cd();


  // REJECTION
  c5->cd();
  c5->SetLeftMargin(0.15);
  c5->SetRightMargin(0.05);
  c5->SetLogy();
  rejection = (TH1D*)ptphot_bkg->Clone("rejection");
  rejection->Divide(ptphot_nocut_bkg, ptphot_bkg, 1., 1., "");//"B");
  rejection->SetMinimum(1e2);
  rejection->SetMaximum(1e7);
  rejection->SetTitle("");
  rejection->SetXTitle("p_{T} [GeV/c]");
  rejection->GetXaxis()->SetMoreLogLabels();
  rejection->GetXaxis()->SetNoExponent();
  rejection->SetYTitle("QCD rejection");
  rejection->GetYaxis()->SetTitleOffset(1.5);
  //rejection->GetYaxis()->SetMoreLogLabels();
  rejection->Draw();
  if (!drawStats) {
    rejection->SetLineColor(kBlue);
    rejection->SetLineWidth(2);
  }

  c5->SaveAs("rejection.eps");

  c5->SetLogx();
  c5->SaveAs("rejection_log.eps");

  // Wider bins
  //int nwide = 3;
  TH1D *rejection_w =
    (TH1D*)rejection->Clone(Form("%s_w", rejection->GetName()));
  TH1D *ptphot_nocut_bkg_w = (TH1D*)ptphot_nocut_bkg->Clone("w1");
  ptphot_bkg_w = (TH1D*)ptphot_bkg->Clone("w2");
  ptphot_nocut_bkg_w->Rebin(nwide);
  ptphot_bkg_w->Rebin(nwide);
  rejection_w->Rebin(nwide);
  rejection_w->Divide(ptphot_nocut_bkg_w, ptphot_bkg_w, 1., 1., "");//"B");
  delete ptphot_nocut_bkg_w;
  delete ptphot_bkg_w;

  rejection_w->SetMinimum(1e2);
  rejection_w->SetMaximum(1e7);
  rejection_w->Draw();
  c5->SetLogx(0);
  c5->Update();
  c5->SaveAs("rejection_wide.eps");
  c0->cd();

  // Save some debug histos
  TCanvas *ctemp = new TCanvas();
  ctemp->cd();
  //gStyle->SetOptStat();
  ptphot_sig->Draw();
  ctemp->SaveAs("tempfits/ptphot_allcut_sig.eps");
  ptphot_bkg->Draw();
  ctemp->SaveAs("tempfits/ptphot_allcut_bkg.eps");
  response_sig->Draw();
  ctemp->SaveAs("tempfits/response_sig.eps");
  response_bkg->Draw();
  ctemp->SaveAs("tempfits/response_bkg.eps");
  c0->cd();

  // Save histograms
  // NB: saving to file is not automatic, because the histograms are now
  //     created in the ROOT work directory so that it is possible to
  //     edit the canvases after the script exits and the file is closed
  hOutputFile->cd();

  ptmean_mix->Write();
  ptmean_sig->Write();
  ptmean_bkg->Write();
  purity->Write();
  efficiency->Write();
  rejection->Write();

  ptphot_nocut->Write();
  ptphot_mix->Write();
  response_nocut->Write();
  response_mix->Write();
  responsevspt_nocut->Write();
  responsevspt_mix->Write();
  response_mix_reg1->Write();
  response_mix_reg2->Write();
  response_mix_reg3->Write();
  response_mix_reg4->Write();

  ptphot_nocut_sig->Write();
  ptphot_sig->Write();
  response_nocut_sig->Write();
  response_sig->Write();
  responsevspt_nocut_sig->Write();
  responsevspt_sig->Write();
  response_nocut_sig_reg1->Write();
  response_nocut_sig_reg2->Write();
  response_nocut_sig_reg3->Write();
  response_nocut_sig_reg4->Write();
  response_sig_reg1->Write();
  response_sig_reg2->Write();
  response_sig_reg3->Write();
  response_sig_reg4->Write();

  ptphot_nocut_bkg->Write();
  ptphot_bkg->Write();
  response_nocut_bkg->Write();
  response_bkg->Write();
  responsevspt_nocut_bkg->Write();
  responsevspt_bkg->Write();
  response_bkg_reg1->Write();
  response_bkg_reg2->Write();
  response_bkg_reg3->Write();
  response_bkg_reg4->Write();

  ptphot_topocut_sig->Write();
  ptphot_topocut_bkg->Write();
  ptphot_idcut_sig->Write();
  ptphot_idcut_bkg->Write();

  curdir->cd();

  if (gROOT->IsBatch()) {
    gROOT->Clear();
  }
}

TChain * getchain(char *thechain) {

  TChain *chain = new TChain("AnaTree");
  std::cout << "Chaining ... " << thechain << std::endl;
  char pName[2000];
  char buffer[200];
  sprintf(buffer, "%s", thechain);
  ifstream is(buffer);
  cout << "files " << buffer <<  endl;
  while(is.getline(buffer, 200, '\n')){
    //    if (buffer[0] == '#') continue;
    sscanf(buffer, "%s", pName);
    std::cout << "   Add: " << buffer << std::endl;
    chain->Add(pName);
  }
  is.close();
  return chain;

}
