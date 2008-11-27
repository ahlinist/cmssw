#include "TH1D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveStats.h"
#include "TStyle.h"

#include "Config.hpp"

#include <string>
#include <iostream>

#include "deltac.C"

// Current ROOT version (5.14) doesn't yet support member functions
// with TF1 so need to make these things global, ugh 
/*
bkgbias *bkgobj;
Double_t bkgbias::deltac(const Double_t *ptgamma, const Double_t *p) {
  return bkgobj->deltac;
}
*/

using namespace std;

const bool drawBars = false;
const bool drawStats = true;//false;
const bool drawPeak = true;

void doRatio(const TGraphErrors *a, const TGraphErrors *b, TGraphErrors *c,
	     double xmatch=0.1);

double FindPeak(TH1D* h, double &err);

TGraphErrors* divide(const TGraphErrors *g, const TF1 *f);

int mcfactors(vector<string> sigfiles, vector<string> bkgfiles,
	      const string idtype) {

  TDirectory *curdir = gDirectory;
  gStyle->SetOptStat(0);
  if (drawStats) gStyle->SetOptFit(1);
  else gStyle->SetOptFit(0);

  cafe::Config config("factor");
  string _algoname = config.get("AlgoName", "");
  
  string dirs = "Ekin";
  string dirsp = "EkinID";
  string dirb = "EkinSB";
  string dirbp = "EkinID";
  if (idtype=="loose") {
    dirsp = "EkinEM";
    dirbp = "EkinEM";
  }
  if (idtype=="medium") {
    dirsp = "EkinID";
    dirbp = "EkinID";
  }
  if (idtype=="tight") {
    dirsp = "EkinTC";
    dirbp = "EkinTC";
  }
  if (idtype=="NN") {
    dirsp = "EkinNN";
    dirbp = "EkinNN";
  }

  vector<string> files(sigfiles.size()+bkgfiles.size());
  for (unsigned int i = 0; i != sigfiles.size(); ++i)
    files[i] = sigfiles[i];
  for (unsigned int i = 0; i != bkgfiles.size(); ++i)
    files[i+sigfiles.size()] = bkgfiles[i];

  // MC truth jet response mapped vs ptgen (!ptgamma)
  TGraphErrors *grjets = new TGraphErrors(); grjets->SetName("grjets");
  TGraphErrors *grjetb = new TGraphErrors(); grjetb->SetName("grjetb");
  TGraphErrors *grjetr = new TGraphErrors(); grjetr->SetName("grjetr");
  // Same with peak (most probable) values
  TGraphErrors *prjets = new TGraphErrors(); prjets->SetName("prjets");
  TGraphErrors *prjetb = new TGraphErrors(); prjetb->SetName("prjetb");
  TGraphErrors *prjetr = new TGraphErrors(); prjetr->SetName("prjetr");

  // Parton to genjet correction mapped vs ptgen (!ptgamma)
  TGraphErrors *gkjets = new TGraphErrors(); gkjets->SetName("gkjets");
  TGraphErrors *gkjetb = new TGraphErrors(); gkjetb->SetName("gkjetb");
  TGraphErrors *gkjetr = new TGraphErrors(); gkjetr->SetName("gkjetr");
  // Same with peak (most probable) values
  TGraphErrors *pkjets = new TGraphErrors(); pkjets->SetName("pkjets");
  TGraphErrors *pkjetb = new TGraphErrors(); pkjetb->SetName("pkjetb");
  TGraphErrors *pkjetr = new TGraphErrors(); pkjetr->SetName("pkjetr");

  // reco vs parton correction mapped vs ptgen (!ptgamma)
  TGraphErrors *grkjets = new TGraphErrors(); grkjets->SetName("grkjets");
  TGraphErrors *grkjetb = new TGraphErrors(); grkjetb->SetName("grkjetb");
  TGraphErrors *grkjetr = new TGraphErrors(); grkjetr->SetName("grkjetr");
  // Same with peak (most probable) values
  TGraphErrors *prkjets = new TGraphErrors(); prkjets->SetName("prkjets");
  TGraphErrors *prkjetb = new TGraphErrors(); prkjetb->SetName("prkjetb");
  TGraphErrors *prkjetr = new TGraphErrors(); prkjetr->SetName("prkjetr");


  // Parton to photon correction mapped vs ptgamma
  TGraphErrors *grphos = new TGraphErrors(); grphos->SetName("grphos");
  TGraphErrors *grphob = new TGraphErrors(); grphob->SetName("grphob");
  TGraphErrors *grphor = new TGraphErrors(); grphor->SetName("grphor");
  // Same with peak (most probable) values
  TGraphErrors *prphos = new TGraphErrors(); prphos->SetName("prphos");
  TGraphErrors *prphob = new TGraphErrors(); prphob->SetName("prphob");
  TGraphErrors *prphor = new TGraphErrors(); prphor->SetName("prphor");

  // Parton to genphoton correction mapped vs ptgamma
  TGraphErrors *gkphos = new TGraphErrors(); gkphos->SetName("gkphos");
  TGraphErrors *gkphob = new TGraphErrors(); gkphob->SetName("gkphob");
  TGraphErrors *gkphor = new TGraphErrors(); gkphor->SetName("gkphor");
  // Same with peak (most probable) values
  TGraphErrors *pkphos = new TGraphErrors(); pkphos->SetName("pkphos");
  TGraphErrors *pkphob = new TGraphErrors(); pkphob->SetName("pkphob");
  TGraphErrors *pkphor = new TGraphErrors(); pkphor->SetName("pkphor");

  // Reco gamma vs parton correction mapped vs ptgamma
  TGraphErrors *grkphos = new TGraphErrors(); grkphos->SetName("grkphos");
  TGraphErrors *grkphob = new TGraphErrors(); grkphob->SetName("grkphob");
  TGraphErrors *grkphor = new TGraphErrors(); grkphor->SetName("grkphor");
  // Same with peak (most probable) values
  TGraphErrors *prkphos = new TGraphErrors(); prkphos->SetName("prkphos");
  TGraphErrors *prkphob = new TGraphErrors(); prkphob->SetName("prkphob");
  TGraphErrors *prkphor = new TGraphErrors(); prkphor->SetName("prkphor");


  // Direct method imbalance pTgenjet/pTrecogamma vs ptgamma
  TGraphErrors *gkgams = new TGraphErrors(); gkgams->SetName("gkgams");
  TGraphErrors *gkgamb = new TGraphErrors(); gkgamb->SetName("gkgamb");
  TGraphErrors *gkgamr = new TGraphErrors(); gkgamr->SetName("gkgamr");
  // Same with peak (most probable) values
  TGraphErrors *pkgams = new TGraphErrors(); pkgams->SetName("pkgams");
  TGraphErrors *pkgamb = new TGraphErrors(); pkgamb->SetName("pkgamb");
  TGraphErrors *pkgamr = new TGraphErrors(); pkgamr->SetName("pkgamr");

  // Parton to genjet correction mapped vs ptgen (!ptgamma)
  TGraphErrors *gktopos = new TGraphErrors(); gktopos->SetName("gktopos");
  TGraphErrors *gktopob = new TGraphErrors(); gktopob->SetName("gktopob");
  TGraphErrors *gktopor = new TGraphErrors(); gktopor->SetName("gktopor");
  // Same with peak (most probable) values
  TGraphErrors *pktopos = new TGraphErrors(); pktopos->SetName("pktopos");
  TGraphErrors *pktopob = new TGraphErrors(); pktopob->SetName("pktopob");
  TGraphErrors *pktopor = new TGraphErrors(); pktopor->SetName("pktopor");

  // Parton to genjet correction mapped vs ptgamma
  TGraphErrors *grmeass = new TGraphErrors(); grmeass->SetName("grmeass");
  TGraphErrors *grmeasb = new TGraphErrors(); grmeasb->SetName("grmeasb");
  TGraphErrors *grmeasr = new TGraphErrors(); grmeasr->SetName("grmeasr");
  // Same with peak (most probable) values
  TGraphErrors *prmeass = new TGraphErrors(); prmeass->SetName("prmeass");
  TGraphErrors *prmeasb = new TGraphErrors(); prmeasb->SetName("prmeasb");
  TGraphErrors *prmeasr = new TGraphErrors(); prmeasr->SetName("prmeasr");


  for (unsigned int i = 0; i != files.size(); ++i) {

    TFile *f = new TFile(files[i].c_str(), "READ");
    assert(!f->IsZombie());

    // Get the average genjet pt and reco photon pt

    string dir = (i < sigfiles.size() ? dirs : dirb);
    TH1D *hptphot = (TH1D*)f->Get((dir+"/lrp_pt").c_str());
    TH1D *hptgen = (TH1D*)f->Get((dir+"/lgj_pt").c_str());
    assert(hptphot);
    assert(hptgen);

    // MC truth jet response (reco jet vs genjet) mapped vs ptgen
    
     dir = (i < sigfiles.size() ? dirs : dirb);
    TH1D *hrjet = (TH1D*)f->Get((dir+"/lrj_dptvg").c_str());

    if (!hrjet) cout << files[i].c_str() << endl;
    assert(hrjet);

    // Replace with Gaussian fit later
    double rjet = hrjet->GetMean();
    double rjet_err = max(0.1,hrjet->GetRMS())
      / sqrt(max(1.,hrjet->Integral()));
    double rjet2_err = 0.;
    double rjet2 = FindPeak(hrjet, rjet2_err);

    double ptphot = hptphot->GetMean();
    double ptphot_err = max(0.1*ptphot, hptphot->GetRMS())
      / sqrt(max(1.,hptphot->Integral()));
    double ptgen = hptgen->GetMean();
    double ptgen_err = max(0.1,hptgen->GetRMS())
      / sqrt(max(1.,hptgen->Integral()));

    if (rjet && ptphot && ptgen) {
      TGraphErrors *grjet = (i < sigfiles.size() ? grjets : grjetb);
      int n = grjet->GetN();
      grjet->SetPoint(n, ptgen, rjet);
      grjet->SetPointError(n, ptgen_err, rjet_err);
      TGraphErrors *prjet = (i < sigfiles.size() ? prjets : prjetb);
      n = prjet->GetN();
      prjet->SetPoint(n, ptgen, rjet2);
      prjet->SetPointError(n, ptgen_err, rjet2_err);
    }

    // Genjet vs parton correction mapped vs ptgen

     dir = (i < sigfiles.size() ? dirs : dirb);
     TH1D *hkjet = (TH1D*)f->Get((dir+"/lgj_dptvq").c_str());

     assert(hkjet);

     // Replace with Gaussian fit later
     double kjet = hkjet->GetMean();
     double kjet_err = max(0.05,hkjet->GetRMS())
       / sqrt(max(1.,hkjet->Integral()));
     double kjet2_err = 0.;
     double kjet2 = FindPeak(hkjet, kjet2_err);

     if (kjet && ptphot && ptgen) {
       TGraphErrors *gkjet = (i < sigfiles.size() ? gkjets : gkjetb);
       int n = gkjet->GetN();
       gkjet->SetPoint(n, ptgen, kjet);
       gkjet->SetPointError(n, ptgen_err, kjet_err);
       TGraphErrors *pkjet = (i < sigfiles.size() ? pkjets : pkjetb);
       n = pkjet->GetN();
       pkjet->SetPoint(n, ptgen, kjet2);
       pkjet->SetPointError(n, ptgen_err, kjet2_err);
     }

    // Reco jet vs parton correction mapped vs ptgen

     dir = (i < sigfiles.size() ? dirs : dirb);
     TH1D *hrkjet = (TH1D*)f->Get((dir+"/lrj_dptvq").c_str());

     assert(hrkjet);

     // Replace with Gaussian fit later
     double rkjet = hrkjet->GetMean();
     double rkjet_err = max(0.05,hrkjet->GetRMS())
       / sqrt(max(1.,hrkjet->Integral()));
     double rkjet2_err = 0.;
     double rkjet2 = FindPeak(hrkjet, rkjet2_err);

     if (rkjet && ptphot && ptgen) {
       TGraphErrors *grkjet = (i < sigfiles.size() ? grkjets : grkjetb);
       int n = grkjet->GetN();
       grkjet->SetPoint(n, ptgen, rkjet);
       grkjet->SetPointError(n, ptgen_err, rkjet_err);
       TGraphErrors *prkjet = (i < sigfiles.size() ? prkjets : prkjetb);
       n = prkjet->GetN();
       prkjet->SetPoint(n, ptgen, rkjet2);
       prkjet->SetPointError(n, ptgen_err, rkjet2_err);
     }


     // Reco photon vs genphoton correction mapped vs ptgamma

     dir = (i < sigfiles.size() ? dirsp : dirbp);
     TH1D *hrpho = (TH1D*)f->Get((dir+"/lrp_dptvgg").c_str());
     // NB: Replace genphoton with parton for signal
     if (i < sigfiles.size()) {
       hrpho = (TH1D*)f->Get((dir+"/lrp_dptvg").c_str());
     }

     if (hrpho) {

       // Replace with Gaussian fit later
       double rpho = hrpho->GetMean();
       double rpho_err = max(0.05,hrpho->GetRMS())
	 / sqrt(max(1.,hrpho->Integral()));
       double rpho2_err = 0.; // not used, not reliable error
       double rpho2 = FindPeak(hrpho, rpho2_err);
       
       if (rpho && ptphot) {
	 TGraphErrors *grpho = (i < sigfiles.size() ? grphos : grphob);
	 int n = grpho->GetN();
	 grpho->SetPoint(n, ptphot, rpho);
	 grpho->SetPointError(n, ptphot_err, rpho_err);
	 TGraphErrors *prpho = (i < sigfiles.size() ? prphos : prphob);
	 if (rpho2 > 0.5 && rpho2 < 1.1) {
	   // eliminate an outliers at 700GeV/0.49, 500GeV/1.2
	   n = prpho->GetN();
	   prpho->SetPoint(n, ptphot, rpho2);
	   prpho->SetPointError(n, ptphot_err, rpho_err);//rpho2_err);
	 }
       }
     } // if hrpho

    // Genphoton vs parton correction mapped vs ptgamma

     dir = (i < sigfiles.size() ? dirsp : dirbp); // needed?
     TH1D *hkpho = (TH1D*)f->Get((dir+"/lgg_dptvg").c_str());
     // NB: Replace genphoton with parton for signal, i.e. set hkpho:=1
     /*
     if (i < sigfiles.size()) {
       TH1D *tmp = hkpho;
       hkpho = (TH1D*)hkpho->Clone("lgg_dptvg_2");
       hkpho->Divide(tmp);
     }
     */

     if (hkpho) {

       // Replace with Gaussian fit later
       double kpho = hkpho->GetMean();
       double kpho_err = max(0.05,hkpho->GetRMS())
	 / sqrt(max(1.,hkpho->Integral()));
       double kpho2_err = 0.;
       double kpho2 = FindPeak(hkpho, kpho2_err);
       // NB: Replace genphoton with parton for signal, i.e. set hkpho:=1
       if (i < sigfiles.size()) kpho = 1.;
       if (i < sigfiles.size()) kpho2 = 1.;
       
       if (kpho && ptphot && ptgen) {
	 TGraphErrors *gkpho = (i < sigfiles.size() ? gkphos : gkphob);
	 int n = gkpho->GetN();
	 gkpho->SetPoint(n, ptphot, kpho);
	 gkpho->SetPointError(n, ptphot_err, kpho_err);
	 TGraphErrors *pkpho = (i < sigfiles.size() ? pkphos : pkphob);
	 n = pkpho->GetN();
	 pkpho->SetPoint(n, ptphot, kpho2);
	 pkpho->SetPointError(n, ptphot_err, kpho2_err);
       }
     } // if hkpho

    // Reco photon vs parton correction mapped vs ptgamma

     dir = (i < sigfiles.size() ? dirsp : dirbp);
     TH1D *hrkpho = (TH1D*)f->Get((dir+"/lrp_dptvg").c_str());

     if (hrkpho) {

       // Replace with Gaussian fit later
       double rkpho = hrkpho->GetMean();
       double rkpho_err = max(0.05,hrkpho->GetRMS())
	 / sqrt(max(1.,hrkpho->Integral()));
       double rkpho2_err = 0.;
       double rkpho2 = FindPeak(hrkpho, rkpho2_err);
       
       if (rkpho && ptphot && ptgen) {
	 TGraphErrors *grkpho = (i < sigfiles.size() ? grkphos : grkphob);
	 int n = grkpho->GetN();
	 grkpho->SetPoint(n, ptphot, rkpho);
	 grkpho->SetPointError(n, ptphot_err, rkpho_err);
	 TGraphErrors *prkpho = (i < sigfiles.size() ? prkphos : prkphob);
	 n = prkpho->GetN();
	 prkpho->SetPoint(n, ptphot, rkpho2);
	 prkpho->SetPointError(n, ptphot_err, rkpho2_err);
       }
     } // if hrkpho


    // Parton level balance mapped vs ptgen

     dir = (i < sigfiles.size() ? dirs : dirb);
     TH1D *hktopo = (TH1D*)f->Get((dir+"/lgq_ptop").c_str());

     assert(hktopo);

     // Replace with Gaussian fit later
     double ktopo = hktopo->GetMean();
     double ktopo_err = max(0.01,hktopo->GetRMS())
       / sqrt(max(1.,hktopo->Integral()));
     double ktopo2_err = 0.;
     double ktopo2 = FindPeak(hktopo, ktopo2_err);

     if (ktopo && ptphot && ptgen) {
       TGraphErrors *gktopo = (i < sigfiles.size() ? gktopos : gktopob);
       int n = gktopo->GetN();
       gktopo->SetPoint(n, ptgen, ktopo);
       gktopo->SetPointError(n, ptgen_err, ktopo_err);
       TGraphErrors *pktopo = (i < sigfiles.size() ? pktopos : pktopob);
       n = pktopo->GetN();
       pktopo->SetPoint(n, ptgen, ktopo2);
       pktopo->SetPointError(n, ptgen_err, ktopo2_err);
     }

     // Direct method imbalance pTgenjet/pTrecogamma

     dir = (i < sigfiles.size() ? dirs : dirb);
     TH1D *hkgam = (TH1D*)f->Get((dir+"/lgj_ptop").c_str());

     assert(hkgam);

     // Replace with Gaussian fit later
     double kgam = hkgam->GetMean();
     double kgam_err = max(0.05,hkgam->GetRMS())
       / sqrt(max(1.,hkgam->Integral()));
     double kgam2_err = 0.;
     double kgam2 = FindPeak(hkgam, kgam2_err);

     if (kgam && ptphot) {
       TGraphErrors *gkgam = (i < sigfiles.size() ? gkgams : gkgamb);
       int n = gkgam->GetN();
       gkgam->SetPoint(n, ptphot, kgam);
       gkgam->SetPointError(n, ptphot_err, kgam_err);
       TGraphErrors *pkgam = (i < sigfiles.size() ? pkgams : pkgamb);
       n = pkgam->GetN();
       pkgam->SetPoint(n, ptphot, kgam2);
       pkgam->SetPointError(n, ptphot_err, kgam2_err);
     }

    // Measured response vs ptgamma

     dir = (i < sigfiles.size() ? dirsp : dirbp);
     TH1D *hrmeas = (TH1D*)f->Get((dir+"/lrj_ptop").c_str());

     if (hrmeas) {

       // Replace with Gaussian fit later
       double rmeas = hrmeas->GetMean();
       double rmeas_err = max(0.01,hrmeas->GetRMS())
	 / sqrt(max(1.,hrmeas->Integral()));
       double rmeas2_err = 0.;
       double rmeas2 = FindPeak(hrmeas, rmeas2_err);
       
       if (rmeas && ptphot && ptgen) {
	 TGraphErrors *grmeas = (i < sigfiles.size() ? grmeass : grmeasb);
	 int n = grmeas->GetN();
	 grmeas->SetPoint(n, ptphot, rmeas);
	 grmeas->SetPointError(n, ptphot_err, rmeas_err);
	 TGraphErrors *prmeas = (i < sigfiles.size() ? prmeass : prmeasb);
	 n = prmeas->GetN();
	 prmeas->SetPoint(n, ptphot, rmeas2);
	 prmeas->SetPointError(n, ptphot_err, rmeas2_err);
       }
     } // if hrmeas

    f->Close();
  } // for i
  curdir->cd();


  // Do the ratio
  doRatio(grjetb,grjets,grjetr);
  doRatio(grkjetb,grkjets,grkjetr);
  doRatio(grkphos,grkphob,grkphor);
  doRatio(grmeasb,grmeass,grmeasr);

  doRatio(prjetb,prjets,prjetr);
  doRatio(prkjetb,prkjets,prkjetr);
  doRatio(prkphos,prkphob,prkphor);
  doRatio(prmeasb,prmeass,prmeasr);

  TCanvas *crjet = new TCanvas("crjet","crjet",600,600);
  crjet->SetLogx();

  TH1D *hrjet = new TH1D("hrjet","hrjet",1000,0.,2000.);
  hrjet->GetXaxis()->SetTitle("p_{T} (GeV)");
  hrjet->GetXaxis()->SetMoreLogLabels();
  hrjet->GetXaxis()->SetNoExponent();
  hrjet->GetXaxis()->SetRangeUser(30.,2000.);
  hrjet->GetYaxis()->SetTitle("R_{jet} = p_{T,calojet} / p_{T,genjet}");
  hrjet->GetYaxis()->SetRangeUser(0.40,1.10);
  //if (!drawStats) hrjet->GetYaxis()->SetRangeUser(0.40,1.05);

  // Ugly trick to get the second stats box
  // For some reason doesn't seem to be created for TGraphErrors
  TH1D* hrjet2 = (TH1D*)hrjet->Clone("hrjet2");

  TF1 *frjets = new TF1("frjets","1-[0]*pow(x,[1]-1)",10.,14000.);
  frjets->SetParNames("a","m");
  frjets->SetLineColor(kRed);
  frjets->SetRange(50.,14000.);
  frjets->SetParameters(2,0.6);
  grjets->Fit(frjets,"QRN");
  frjets->SetRange(10.,14000.);
  hrjet->GetListOfFunctions()->Add(frjets);

  TF1 *fprjets = new TF1("fprjets","1-[0]*pow(x,[1]-1)",10.,14000.);
  fprjets->SetParNames("a","m");
  fprjets->SetLineColor(kRed);
  fprjets->SetLineStyle(kDashed);
  fprjets->SetRange(50.,14000.);
  fprjets->SetParameters(2,0.6);
  prjets->Fit(fprjets,"QRN");
  fprjets->SetRange(10.,14000.);
  //hrjet->GetListOfFunctions()->Add(frjets);
 
  cout << Form("double rjets = 1 - %1.4g * pow(y, %1.4g);",
	       frjets->GetParameter(0),frjets->GetParameter(1)-1) << endl;

  TF1 *frjetb = new TF1("frjetb","1-[0]*pow(x,[1]-1)",10.,14000.);
  frjetb->SetParNames("a","m");
  frjetb->SetLineColor(kBlue);
  frjetb->SetRange(10.,14000.);
  frjetb->SetParameters(frjets->GetParameter(0),frjets->GetParameter(1));
  grjetb->Fit(frjetb,"QRN");
  frjetb->SetRange(10.,14000.);
  hrjet2->GetListOfFunctions()->Add(frjetb);

  TF1 *fprjetb = new TF1("fprjetb","1-[0]*pow(x,[1]-1)",10.,14000.);
  fprjetb->SetParNames("a","m");
  fprjetb->SetLineColor(kBlue);
  fprjetb->SetLineStyle(kDotted);
  fprjetb->SetRange(10.,14000.);
  fprjetb->SetParameters(fprjets->GetParameter(0),fprjets->GetParameter(1));
  prjetb->Fit(fprjetb,"QRN");
  fprjetb->SetRange(10.,14000.);

  cout << Form("double rjetb = 1 - %1.4g * pow(y, %1.4g);",
	       frjetb->GetParameter(0),frjetb->GetParameter(1)-1) << endl;

  TLegend *lrjet = (drawPeak ? 
		    new TLegend(0.60,0.2,0.95,0.5,_algoname.c_str(),"brNDC") :
		    new TLegend(0.65,0.2,0.95,0.4,_algoname.c_str(),"brNDC"));
  lrjet->SetBorderSize(0);
  lrjet->SetFillStyle(kNone);
  lrjet->SetTextSize(0.05);//drawPeak ? 0.05 : 0.06);
  grjets->SetMarkerStyle(kFullCircle);
  grjets->SetMarkerColor(kRed);
  grjets->SetMarkerSize(0.8);
  prjets->SetMarkerStyle(kCircle);
  prjets->SetMarkerColor(kRed);
  prjets->SetMarkerSize(0.8);
  grjetb->SetMarkerStyle(kFullSquare);
  grjetb->SetMarkerColor(kBlue);
  grjetb->SetMarkerSize(0.8);
  prjetb->SetMarkerStyle(kOpenSquare);
  prjetb->SetMarkerColor(kBlue);
  prjetb->SetMarkerSize(0.8);
  lrjet->AddEntry(grjets, "#gamma jet (mean)", "LP");
  if (drawPeak) lrjet->AddEntry(prjets, "#gamma jet (peak)", "LP");
  lrjet->AddEntry(grjetb, "QCD (mean)", "LP");
  if (drawPeak) lrjet->AddEntry(prjetb, "QCD (peak)", "LP");

  hrjet->Draw();
  hrjet2->Draw("SAMES");
  frjets->Draw("SAME");
  if (drawPeak) fprjets->Draw("SAME");
  frjetb->Draw("SAME");
  if (drawPeak) fprjetb->Draw("SAME");
  grjets->SetLineColor(kRed);
  grjets->Draw("SAME P");
  prjets->SetLineColor(kRed);
  prjets->SetLineStyle(kDashed);
  if (drawPeak) prjets->Draw("SAME P");
  grjetb->SetLineColor(kBlue);
  grjetb->Draw("SAME P");
  prjetb->SetLineColor(kBlue);
  prjetb->SetLineStyle(kDotted);
  if (drawPeak) prjetb->Draw("SAME P");
  lrjet->Draw();

  if (drawStats) {
    gPad->Update(); // update the canvas to create the stats boxes
    TPaveStats *stats, *statb;
    stats = (TPaveStats*)hrjet->GetListOfFunctions()->FindObject("stats");
    assert(stats);
    statb = (TPaveStats*)hrjet2->GetListOfFunctions()->FindObject("stats");
    assert(statb);
  
    stats->SetLineColor(kRed);
    stats->SetTextColor(kRed);
    stats->SetX1NDC(max(stats->GetX1NDC(),stats->GetX2NDC()-0.35));
    
    statb->SetLineColor(kBlue);
    statb->SetTextColor(kBlue);
    statb->SetX1NDC(max(stats->GetX1NDC()-0.39, 0.20));
    statb->SetX2NDC(min(stats->GetX2NDC()-0.39, stats->GetX1NDC()));
    statb->SetName("stats_bkg");
    gPad->Update(); // show changes in the interactive session
  }
  crjet->SaveAs("eps/mcfactors_rjet.eps");

  // Same plot, but divide by gamma+jet fit
  TCanvas *crjetz = new TCanvas("crjetz","crjetz",600,600);
  crjetz->SetLogx();

  TH1D *hrjetz = new TH1D("hrjetz","hrjetz",1000,0.,2000.);
  hrjetz->GetXaxis()->SetTitle("p_{T} (GeV)");
  hrjetz->GetXaxis()->SetMoreLogLabels();
  hrjetz->GetXaxis()->SetNoExponent();
  hrjetz->GetXaxis()->SetRangeUser(30.,2000.);
  //hrjetz->GetYaxis()->SetTitle("R_{jet} over fit");
  //hrjetz->GetYaxis()->SetRangeUser(0.95,1.25); // over fprjetb
hrjetz->GetYaxis()->SetTitle("R_{jet} over peak fits");
  hrjetz->GetYaxis()->SetRangeUser(0.95,1.10); // over fprjets,b

  
  if (!grjets->GetListOfFunctions()) {
    grjets->Fit("pol0","QR"); grjets->GetListOfFunctions()->Clear();
  }
  grjets->GetListOfFunctions()->Add(frjets);
  //hrjet->GetListOfFunctions()->Remove(frjets);
  if (!prjets->GetListOfFunctions()) {
    prjets->Fit("pol0","QR"); prjets->GetListOfFunctions()->Clear();
  }
  prjets->GetListOfFunctions()->Add(fprjets);
  if (!grjetb->GetListOfFunctions()) {
    grjetb->Fit("pol0","QR"); grjetb->GetListOfFunctions()->Clear();
  }
  grjetb->GetListOfFunctions()->Add(frjetb);
  //hrjet2->GetListOfFunctions()->Clear();
  if (!prjetb->GetListOfFunctions()) {
    prjetb->Fit("pol0","QR"); prjetb->GetListOfFunctions()->Clear();
  }
  prjetb->GetListOfFunctions()->Add(fprjetb);
  TGraphErrors *drjets  = divide(grjets, fprjets);
  TGraphErrors *dprjets = divide(prjets, fprjets);
  TGraphErrors *drjetb  = divide(grjetb, fprjetb);
  TGraphErrors *dprjetb = divide(prjetb, fprjetb);

  hrjetz->Draw();
  drjets->Draw("same P");
  if(drawPeak) dprjets->Draw("same P");
  drjetb->Draw("same P");
  if (drawPeak) dprjetb->Draw("same P");

  TLegend *lrjetz = (drawPeak ? 
		     new TLegend(0.60,0.5,0.95,0.8,_algoname.c_str(),"brNDC") :
		     new TLegend(0.65,0.5,0.95,0.9,_algoname.c_str(),"brNDC"));
  lrjetz->SetBorderSize(0);
  lrjetz->SetFillStyle(kNone);
  lrjetz->SetTextSize(0.05);
  lrjetz->AddEntry(grjets, "#gamma jet (mean)", "LP");
  if (drawPeak) lrjetz->AddEntry(prjets, "#gamma jet (peak)", "LP");
  lrjetz->AddEntry(grjetb, "QCD (mean)", "LP");
  if (drawPeak) lrjetz->AddEntry(prjetb, "QCD (peak)", "LP");
  lrjetz->Draw();

  int st = gStyle->GetOptFit();
  gStyle->SetOptFit(0);
  gPad->Update();
  crjetz->SaveAs("eps/mcfactors_rjetz.eps");
  gStyle->SetOptFit(st);


  TCanvas *ckjet = new TCanvas("ckjet","ckjet",600,600);
  ckjet->SetLogx();

  TH1D *hkjet = new TH1D("hkjet","hkjet",2000,0.,2000.);
  hkjet->GetXaxis()->SetTitle("p_{T} (GeV)");
  hkjet->GetXaxis()->SetMoreLogLabels();
  hkjet->GetXaxis()->SetNoExponent();
  hkjet->GetXaxis()->SetRangeUser(30.,2000.);
  hkjet->GetYaxis()->SetTitle("k_{jet} = p_{T,genjet} / p_{T,parton-jet}");
  hkjet->GetYaxis()->SetRangeUser(0.85,1.10);
  //if (!drawStats) hkjet->GetYaxis()->SetRangeUser(0.45,1.05);

  // Ugly trick to get the second stats box
  // For some reason doesn't seem to be created for TGraphErrors
  TH1D* hkjet2 = (TH1D*)hkjet->Clone("hkjet2");

  TF1 *fkjets = new TF1("fkjets","[2]-[0]*pow(x,[1]-1)",10.,14000.);
  fkjets->SetParNames("a","m","c");
  fkjets->SetLineColor(kRed);
  fkjets->SetRange(50.,14000.);
  fkjets->SetParameters(-0.2,0.6,1.0);
  gkjets->Fit(fkjets,"QRN");
  fkjets->SetRange(10.,14000.);
  hkjet->GetListOfFunctions()->Add(fkjets);

  TF1 *fpkjets = new TF1("fpkjets","[2]-[0]*pow(x,[1]-1)",10.,14000.);
  fpkjets->SetParNames("a","m","c");
  fpkjets->SetLineColor(kRed);
  fpkjets->SetLineStyle(kDashed);
  fpkjets->SetRange(50.,14000.);
  fpkjets->SetParameters(-0.2,0.6,1.0);
  pkjets->Fit(fpkjets,"QRN");
  fpkjets->SetRange(10.,14000.);
  //hkjet->GetListOfFunctions()->Add(fkjets);

  cout << Form("double kjets = %1.4g - %1.4g * pow(y, %1.4g);",
	       fkjets->GetParameter(2),fkjets->GetParameter(0),
	       fkjets->GetParameter(1)-1) << endl;
  cout << Form("double kjets_p = %1.4g - %1.4g * pow(y, %1.4g);",
	       fpkjets->GetParameter(2),fpkjets->GetParameter(0),
	       fpkjets->GetParameter(1)-1) << endl;

  TF1 *fkjetb = new TF1("fkjetb","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			10.,14000.);
  fkjetb->SetParNames("p0","p1","p2");
  fkjetb->SetLineColor(kBlue);
  fkjetb->SetRange(10.,14000.);
  fkjetb->SetParameters(0.92, 0.04,-0.005);
  gkjetb->Fit(fkjetb,"QRN");
  fkjetb->SetRange(10.,14000.);
  hkjet2->GetListOfFunctions()->Add(fkjetb);

  TF1 *fpkjetb = new TF1("fpkjetb","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			10.,14000.);
  fpkjetb->SetParNames("p0","p1","p2");
  fpkjetb->SetLineColor(kBlue);
  fpkjetb->SetLineStyle(kDotted);
  fpkjetb->SetRange(10.,14000.);
  fpkjetb->SetParameters(0.96, 0.04,-0.005);
  pkjetb->Fit(fpkjetb,"QRN");
  fpkjetb->SetRange(10.,14000.);
  //hkjet2->GetListOfFunctions()->Add(fkjetb);

  cout << Form("double kjetb = %1.4g + x * (%1.4g + x * %1.4g);",
	       fkjetb->GetParameter(0),fkjetb->GetParameter(1),
	       fkjetb->GetParameter(2)) << endl;
  cout << Form("double kjetb_p = %1.4g + x * (%1.4g + x * %1.4g);",
	       fpkjetb->GetParameter(0),fpkjetb->GetParameter(1),
	       fpkjetb->GetParameter(2)) << endl;

  TLegend *lkjet = (drawPeak ? 
		    new TLegend(0.60,0.2,0.95,0.5,_algoname.c_str(),"brNDC") :
		    new TLegend(0.65,0.2,0.95,0.4,_algoname.c_str(),"brNDC"));
  lkjet->SetBorderSize(0);
  lkjet->SetFillStyle(kNone);
  lkjet->SetTextSize(0.05);//drawPeak ? 0.05 : 0.06);
  gkjets->SetMarkerStyle(kFullCircle);
  gkjets->SetMarkerColor(kRed);
  gkjets->SetMarkerSize(0.8);
  pkjets->SetMarkerStyle(kCircle);
  pkjets->SetMarkerColor(kRed);
  pkjets->SetMarkerSize(0.8);
  gkjetb->SetMarkerStyle(kFullSquare);
  gkjetb->SetMarkerColor(kBlue);
  gkjetb->SetMarkerSize(0.8);
  pkjetb->SetMarkerStyle(kOpenSquare);
  pkjetb->SetMarkerColor(kBlue);
  pkjetb->SetMarkerSize(0.8);
  lkjet->AddEntry(gkjets, "#gamma jet (mean)", "LP");
  if (drawPeak) lkjet->AddEntry(pkjets, "#gamma jet (peak)", "LP");
  lkjet->AddEntry(gkjetb, "QCD (mean)", "LP");
  if (drawPeak) lkjet->AddEntry(pkjetb, "QCD (peak)", "LP");

  hkjet->Draw("");
  hkjet2->Draw("SAMES");
  if (drawPeak) fpkjets->Draw("SAME");
  fkjets->Draw("SAME");
  if (drawPeak) fpkjetb->Draw("SAME");
  fkjetb->Draw("SAME");
  gkjets->SetLineColor(kRed);
  gkjets->Draw("SAME P");
  pkjets->SetLineColor(kRed);
  pkjets->SetLineStyle(kDashed);
  if (drawPeak) pkjets->Draw("SAME P");
  gkjetb->SetLineColor(kBlue);
  gkjetb->Draw("SAME P");
  pkjetb->SetLineColor(kBlue);
  pkjetb->SetLineStyle(kDotted);
  if (drawPeak) pkjetb->Draw("SAME P");
  lkjet->Draw();

  if (drawStats) {
    gPad->Update(); // update the canvas to create the stats boxes
    TPaveStats *stats, *statb;
    stats = (TPaveStats*)hkjet->GetListOfFunctions()->FindObject("stats");
    assert(stats);
    statb = (TPaveStats*)hkjet2->GetListOfFunctions()->FindObject("stats");
    assert(statb);
    
    stats->SetLineColor(kRed);
    stats->SetTextColor(kRed);
    stats->SetX1NDC(max(stats->GetX1NDC(),stats->GetX2NDC()-0.36));
    
    statb->SetLineColor(kBlue);
    statb->SetTextColor(kBlue);
    statb->SetX1NDC(max(stats->GetX1NDC()-0.39, 0.20));
    statb->SetX2NDC(min(stats->GetX2NDC()-0.39, stats->GetX1NDC()));
    statb->SetName("stats_bkg");
    gPad->Update(); // show changes in the interactive session
  }
  ckjet->SaveAs("eps/mcfactors_kjet.eps");


  TCanvas *cktopo = new TCanvas("cktopo","cktopo",600,600);
  cktopo->SetLogx();

  TH1D *hktopo = new TH1D("hktopo","hktopo",2000,0.,2000.);
  hktopo->GetXaxis()->SetTitle("p_{T} (GeV)");
  hktopo->GetXaxis()->SetMoreLogLabels();
  hktopo->GetXaxis()->SetNoExponent();
  hktopo->GetXaxis()->SetRangeUser(30.,2000.);
  string title = "k_{topo} = p_{T,parton-jet} / p_{T,parton-#gamma}";
  hktopo->GetYaxis()->SetTitle(title.c_str());
  hktopo->GetYaxis()->SetRangeUser(0.9,1.10);
  //if (!drawStats) hktopo->GetYaxis()->SetRangeUser(0.45,1.05);

  // Ugly trick to get the second stats box
  // For some reason doesn't seem to be created for TGraphErrors
  TH1D* hktopo2 = (TH1D*)hktopo->Clone("hktopo2");

  TF1 *fktopos = new TF1("fktopos","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			 10.,14000.);
  fktopos->SetParNames("p0","p1","p2");
  fktopos->SetLineColor(kRed);
  fktopos->SetRange(50.,14000.);
  fktopos->SetParameters(1., 0., 0.);
  fktopos->FixParameter(2,0.);
  gktopos->Fit(fktopos,"QRN");
  fktopos->SetRange(10.,14000.);
  hktopo->GetListOfFunctions()->Add(fktopos);

  cout << Form("double ktopos = %1.4g + x * (%1.4g + x * %1.4g);",
	       fktopos->GetParameter(0),fktopos->GetParameter(1),
	       fktopos->GetParameter(2)) << endl;

  TF1 *fktopob = new TF1("fktopob","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			 10.,14000.);
  fktopob->SetParNames("p0","p1","p2");
  fktopob->SetLineColor(kBlue);
  fktopob->SetRange(10.,14000.);
  fktopob->SetParameters(1., 0., 0.);
  gktopob->Fit(fktopob,"QRN");
  fktopob->SetRange(10.,14000.);
  hktopo2->GetListOfFunctions()->Add(fktopob);

  cout << Form("double ktopob = %1.4g + x * (%1.4g + x * %1.4g);",
	       fktopob->GetParameter(0),fktopob->GetParameter(1),
	       fktopob->GetParameter(2)) << endl;

  TLegend *lktopo = new TLegend(0.65,0.2,0.95,0.4,_algoname.c_str(),"brNDC");
  lktopo->SetBorderSize(0);
  lktopo->SetFillStyle(kNone);
  lktopo->SetTextSize(0.06);

  TLatex *tktopo = new TLatex(0.25,0.34,(idtype+"ID").c_str());
  tktopo->SetNDC();
  tktopo->SetTextSize(0.06);

  gktopos->SetMarkerStyle(kCircle);
  gktopos->SetMarkerColor(kRed);
  gktopos->SetMarkerSize(0.8);
  gktopob->SetMarkerStyle(kOpenSquare);
  gktopob->SetMarkerColor(kBlue);
  gktopob->SetMarkerSize(0.8);
  lktopo->AddEntry(gktopos, "#gamma jet", "LP");
  lktopo->AddEntry(gktopob, "QCD", "LP");

  hktopo->Draw("");
  hktopo2->Draw("SAMES");
  fktopos->Draw("SAME");
  fktopob->Draw("SAME");
  gktopos->SetLineColor(kRed);
  gktopos->Draw("SAME P");
  gktopob->SetLineColor(kBlue);
  gktopob->Draw("SAME P");
  lktopo->Draw();
  tktopo->Draw();

  if (drawStats) {
    gPad->Update(); // update the canvas to create the stats boxes
    TPaveStats *stats, *statb;
    stats = (TPaveStats*)hktopo->GetListOfFunctions()->FindObject("stats");
    assert(stats);
    statb = (TPaveStats*)hktopo2->GetListOfFunctions()->FindObject("stats");
    assert(statb);
    
    stats->SetLineColor(kRed);
    stats->SetTextColor(kRed);
    stats->SetX1NDC(max(stats->GetX1NDC(),stats->GetX2NDC()-0.36));
    
    statb->SetLineColor(kBlue);
    statb->SetTextColor(kBlue);
    statb->SetX1NDC(max(stats->GetX1NDC()-0.39, 0.20));
    statb->SetX2NDC(min(stats->GetX2NDC()-0.39, stats->GetX1NDC()));
    statb->SetName("stats_bkg");
    gPad->Update(); // show changes in the interactive session
  }
  cktopo->SaveAs("eps/mcfactors_ktopo.eps");


  TCanvas *crpho = new TCanvas("crpho","crpho",600,600);
  crpho->SetLogx();

  TH1D *hrpho = new TH1D("hrpho","hrpho",1000,0.,2000.);
  hrpho->GetXaxis()->SetTitle("p_{T} (GeV)");
  hrpho->GetXaxis()->SetMoreLogLabels();
  hrpho->GetXaxis()->SetNoExponent();
  hrpho->GetXaxis()->SetRangeUser(30.,2000.);//500.);
  title = "R_{#gamma} = p_{T,calo-#gamma} / p_{T,gen-#gamma}";
  hrpho->GetYaxis()->SetTitle(title.c_str());
  hrpho->GetYaxis()->SetRangeUser(0.45,1.30);
  //if (!drawStats) hrpho->GetYaxis()->SetRangeUser(0.45,1.05);

  // Ugly trick to get the second stats box
  // For some reason doesn't seem to be created for TGraphErrors
  TH1D* hrpho2 = (TH1D*)hrpho->Clone("hrpho2");

  TF1 *frphos = new TF1("frphos","1-[0]*pow(x,[1]-1)",10.,14000.);
  frphos->SetParNames("a","m");
  frphos->SetLineColor(kRed);
  frphos->SetRange(10.,14000.);
  frphos->SetParameters(0.2,0.6);//,1.);
  grphos->Fit(frphos,"QRN");
  frphos->SetRange(10.,14000.);
  hrpho->GetListOfFunctions()->Add(frphos);

  //TF1 *fprphos = new TF1("fprphos","1-[0]*pow(x,[1]-1)",10.,14000.);
  TF1 *fprphos = new TF1("fprphos","1-[0]",10.,14000.);
  fprphos->SetParNames("a","m");
  fprphos->SetLineColor(kRed);
  fprphos->SetLineStyle(kDashed);
  fprphos->SetRange(10.,14000.);
  fprphos->SetParameter(0,0.2);//s(0.2,0.6);
  prphos->Fit(fprphos,"QRN");
  fprphos->SetRange(10.,14000.);
  //hrpho->GetListOfFunctions()->Add(fprphos);

  cout << Form("double rphos = 1 - %1.4g * pow(y, %1.4g);",
	       frphos->GetParameter(0),frphos->GetParameter(1)-1) << endl;

  TF1 *frphob = new TF1("frphob","1-[0]*pow(x,[1]-1)",10.,14000.);
  frphob->SetParNames("a","m");
  frphob->SetLineColor(kBlue);
  frphob->SetRange(10.,14000.);
  frphob->SetParameters(0.3*frjetb->GetParameter(0),frjetb->GetParameter(1));
  frphob->FixParameter(1,frjetb->GetParameter(1));
  grphob->Fit(frphob,"QRN");
  frphob->SetRange(10.,14000.);
  hrpho2->GetListOfFunctions()->Add(frphob);

  TF1 *fprphob = new TF1("fprphob","1-[0]*pow(x,[1]-1)",10.,14000.);
  fprphob->SetParNames("a","m");
  fprphob->SetLineColor(kBlue);
  fprphob->SetLineStyle(kDotted);
  fprphob->SetRange(10.,14000.);
  fprphob->SetParameters(0.3*frjetb->GetParameter(0),frjetb->GetParameter(1));
  fprphob->FixParameter(1,frjetb->GetParameter(1));// or fprjetb?
  prphob->Fit(fprphob,"QRN");
  fprphob->SetRange(10.,14000.);
  //hrpho2->GetListOfFunctions()->Add(fprphob);

  cout << Form("double rphob = 1 - %1.4g * pow(y, %1.4g);",
	       frphob->GetParameter(0),frphob->GetParameter(1)-1) << endl;

  // Expected R_gamma for a background jet
  TF1 *fjet = new TF1("fjet","(1-[0]*pow(x,[1]-1))",0.,14000.);
  fjet->SetParameters(frjetb->GetParameter(0),frjetb->GetParameter(1));

  TLegend *lrpho=(drawPeak ?
		  new TLegend(0.69,0.18,0.93,0.48,_algoname.c_str(),"brNDC"):
		  new TLegend(0.69,0.18,0.93,0.48,_algoname.c_str(),"brNDC"));
  lrpho->SetBorderSize(0);
  lrpho->SetFillStyle(kNone);
  lrpho->SetTextSize(drawPeak ? 0.04 : 0.06);

  TLatex *trpho = new TLatex(0.22,0.74,(idtype+"ID").c_str());
  trpho->SetNDC();
  trpho->SetTextSize(0.06);

  grphos->SetMarkerStyle(kFullCircle);
  grphos->SetMarkerColor(kRed);
  grphos->SetMarkerSize(0.8);
  prphos->SetMarkerStyle(kCircle);
  prphos->SetMarkerColor(kRed);
  prphos->SetMarkerSize(0.8);
  grphob->SetMarkerStyle(kFullSquare);
  grphob->SetMarkerColor(kBlue);
  grphob->SetMarkerSize(0.8);
  prphob->SetMarkerStyle(kOpenSquare);
  prphob->SetMarkerColor(kBlue);
  prphob->SetMarkerSize(0.8);
  lrpho->AddEntry(grphos, "#gamma jet (mean)", "LP");
  lrpho->AddEntry(prphos, "#gamma jet (peak)", "LP");
  lrpho->AddEntry(grphob, "QCD (mean)", "LP");
  lrpho->AddEntry(prphob, "QCD (peak)", "LP");
  //lrpho->AddEntry(fjet, "R_{jet}^{QCD} (mean)", "L");

  hrpho->Draw("");
  hrpho2->Draw("SAMES");
  frphos->Draw("SAME"); // draw on top of stats box
  if (drawPeak) fprphos->Draw("SAME");
  frphob->Draw("SAME"); // draw on top of stats box
  if (drawPeak) fprphob->Draw("SAME");
  //fjet->Draw("SAME");
  grphos->SetLineColor(kRed);
  grphos->Draw("SAME P");
  prphos->SetLineColor(kRed);
  prphos->SetLineStyle(kDashed);
  if (drawPeak) prphos->Draw("SAME P");
  grphob->SetLineColor(kBlue);
  grphob->Draw("SAME P");
  prphob->SetLineColor(kBlue);
  prphob->SetLineStyle(kBlue);
  if (drawPeak) prphob->Draw("SAME P");
  lrpho->Draw();
  trpho->Draw();

  if (drawStats) {
    gPad->Update(); // update the canvas to create the stats boxes
    TPaveStats *stats, *statb;
    stats = (TPaveStats*)hrpho->GetListOfFunctions()->FindObject("stats");
    assert(stats);
    statb = (TPaveStats*)hrpho2->GetListOfFunctions()->FindObject("stats");
    assert(statb);
    
    stats->SetLineColor(kRed);
    stats->SetTextColor(kRed);
    stats->SetX1NDC(max(stats->GetX1NDC(),stats->GetX2NDC()-0.36));
    
    statb->SetLineColor(kBlue);
    statb->SetTextColor(kBlue);
    statb->SetX1NDC(max(stats->GetX1NDC()-0.39, 0.20));
    statb->SetX2NDC(min(stats->GetX2NDC()-0.39, stats->GetX1NDC()));
    statb->SetName("stats_bkg");
    gPad->Update(); // show changes in the interactive session
  }
  hrpho->GetYaxis()->SetRangeUser(0.75,1.15);
  crpho->SaveAs("eps/mcfactors_rphoz.eps");
  
  lrpho->AddEntry(fjet, "R_{jet}^{QCD} (mean)", "L");
  fjet->Draw("SAME");
  hrpho->GetYaxis()->SetRangeUser(0.45,1.30);
  crpho->SaveAs("eps/mcfactors_rpho.eps");
  


  TCanvas *ckpho = new TCanvas("ckpho","ckpho",600,600);
  ckpho->SetLogx();

  TH1D *hkpho = new TH1D("hkpho","hkpho",1000,0.,2000.);
  hkpho->GetXaxis()->SetTitle("p_{T} (GeV)");
  hkpho->GetXaxis()->SetMoreLogLabels();
  hkpho->GetXaxis()->SetNoExponent();
  hkpho->GetXaxis()->SetRangeUser(30.,2000.);//500.);
  title = "k_{#gamma} = p_{T,gen-#gamma} / p_{T,parton-#gamma}";
  hkpho->GetYaxis()->SetTitle(title.c_str());
  hkpho->GetYaxis()->SetRangeUser(0.85,1.10);//0.45,1.30);
  //if (!drawStats) hkpho->GetYaxis()->SetRangeUser(0.45, 1.05);

  // Ugly trick to get the second stats box
  // For some reason doesn't seem to be created for TGraphErrors
  TH1D* hkpho2 = (TH1D*)hkpho->Clone("hkpho2");

  TF1 *fkphos = new TF1("fkphos","[2]-[0]*pow(x,[1]-1)",10.,14000.);
  fkphos->SetLineColor(kRed);
  fkphos->SetRange(10.,14000.);
  fkphos->SetParameters(0.2,0.6,1.);
  gkphos->Fit(fkphos,"QRN");
  fkphos->SetRange(10.,14000.);
  hkpho->GetListOfFunctions()->Add(fkphos);

  cout << Form("double kphos = %1.4g - %1.4g * pow(y, %1.4g);",
	       fkphos->GetParameter(2),fkphos->GetParameter(0),
	       fkphos->GetParameter(1)-1) << endl;

  TF1 *fkphob = new TF1("fkphob","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			10.,14000.);
  fkphob->SetLineColor(kBlue);
  fkphob->SetRange(10.,14000.);
  fkphob->SetParameters(fkjetb->GetParameter(0),fkjetb->GetParameter(1),
			fkjetb->GetParameter(2));
  fkphob->FixParameter(1,fkjetb->GetParameter(1));
  fkphob->FixParameter(2,fkjetb->GetParameter(2));
  gkphob->Fit(fkphob,"QRN");
  fkphob->SetRange(10.,14000.);
  hkpho2->GetListOfFunctions()->Add(fkphob);

  cout << Form("double kphob = %1.4g + x * (%1.4g + x * %1.4g);",
	       fkphob->GetParameter(0),fkphob->GetParameter(1),
	       fkphob->GetParameter(2)) << endl;

  // Expected k_gamma for a background jet
  TF1 *fpho = new TF1("fpho","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
		      0.,14000.);
  fpho->SetParameters(fkjetb->GetParameter(0),fkjetb->GetParameter(1),
		      fkjetb->GetParameter(2));

  TLegend *lkpho = new TLegend(0.75,0.18,0.95,0.45,_algoname.c_str(),"brNDC");
  lkpho->SetBorderSize(0);
  lkpho->SetFillStyle(kNone);
  lkpho->SetTextSize(0.05);

  TLatex *tkpho = new TLatex(0.22,0.74,(idtype+"ID").c_str());
  tkpho->SetNDC();
  tkpho->SetTextSize(0.05);

  gkphos->SetMarkerStyle(kCircle);
  gkphos->SetMarkerColor(kRed);
  gkphos->SetMarkerSize(0.8);
  gkphob->SetMarkerStyle(kOpenSquare);
  gkphob->SetMarkerColor(kBlue);
  gkphob->SetMarkerSize(0.8);
  lkpho->AddEntry(gkphos, "#gamma jet", "LP");
  lkpho->AddEntry(gkphob, "QCD", "LP");
  lkpho->AddEntry(fpho, "k_{jet}^{QCD}", "L");

  hkpho->Draw("");
  hkpho2->Draw("SAMES");
  fkphos->Draw("SAME"); // draw on top of stats box
  fkphob->Draw("SAME"); // draw on top of stats box
  fpho->Draw("SAME");
  gkphos->SetLineColor(kRed);
  gkphos->Draw("SAME P");
  gkphob->SetLineColor(kBlue);
  gkphob->Draw("SAME P");
  lkpho->Draw();
  tkpho->Draw();

  if (drawStats) {
    gPad->Update(); // update the canvas to create the stats boxes
    TPaveStats *stats, *statb;
    stats = (TPaveStats*)hkpho->GetListOfFunctions()->FindObject("stats");
    assert(stats);
    statb = (TPaveStats*)hkpho2->GetListOfFunctions()->FindObject("stats");
    assert(statb);
    
    stats->SetLineColor(kRed);
    stats->SetTextColor(kRed);
    stats->SetX1NDC(max(stats->GetX1NDC(),stats->GetX2NDC()-0.36));
    
    statb->SetLineColor(kBlue);
    statb->SetTextColor(kBlue);
    statb->SetX1NDC(max(stats->GetX1NDC()-0.39, 0.20));
    statb->SetX2NDC(min(stats->GetX2NDC()-0.39, stats->GetX1NDC()));
    statb->SetName("stats_bkg");
    gPad->Update(); // show changes in the interactive session
  }
  ckpho->SaveAs("eps/mcfactors_kpho.eps");

 
  TCanvas *ckgam = new TCanvas("ckgam","ckgam",600,600);
  ckgam->SetLogx();

  TH1D *hkgam = new TH1D("hkgam","hkgam",1000,0.,2000.);
  hkgam->GetXaxis()->SetTitle("p_{T} (GeV)");
  hkgam->GetXaxis()->SetMoreLogLabels();
  hkgam->GetXaxis()->SetNoExponent();
  hkgam->GetXaxis()->SetRangeUser(30.,2000.);
  title = "k_{#gamma+jet} = p_{T,genjet} / p_{T,calo-#gamma}";
  hkgam->GetYaxis()->SetTitle(title.c_str());
  hkgam->GetYaxis()->SetRangeUser(0.90,1.10);

  TLatex *tkgam = new TLatex(0.65,0.84,(idtype+"ID").c_str());
  tkgam->SetNDC();
  tkgam->SetTextSize(0.06);

  TF1 *fkgam = new TF1("fkgam","([2]-[0]*pow(x,[1]-1))" //kjet
		       "*([3]+log(0.01*x)*([4]+log(0.01*x)*[5]))" //ktopo
		       "/((1-[6]*pow(x,[7]-1))" //rpho
		       "*([10]-[8]*pow(x,[9]-1)))", //kpho
		       0,14000);
  fkgam->SetParameters(fkjets->GetParameter(0),fkjets->GetParameter(1),
		       fkjets->GetParameter(2),
		       fktopos->GetParameter(0),fktopos->GetParameter(1),
		       fktopos->GetParameter(2),
		       frphos->GetParameter(0),frphos->GetParameter(1),
		       fkphos->GetParameter(0),fkphos->GetParameter(1));
  fkgam->SetParameter(10,fkphos->GetParameter(2));
  fkgam->SetLineColor(kRed);

  hkgam->Draw();
  fkgam->Draw("same");
  gkgams->SetLineColor(kRed);
  gkgams->Draw("same");
  tkgam->Draw();

  ckgam->SaveAs("eps/mcfactors_kgam.eps");


  TCanvas *ckqcd = new TCanvas("ckqcd","ckqcd",600,600);
  ckqcd->SetLogx();

  TH1D *hkqcd = new TH1D("hkqcd","hkqcd",1000,0.,2000.);
  hkqcd->GetXaxis()->SetTitle("p_{T} (GeV)");
  hkqcd->GetXaxis()->SetMoreLogLabels();
  hkqcd->GetXaxis()->SetNoExponent();
  hkqcd->GetXaxis()->SetRangeUser(30.,2000.);//500.);
  title = "#DeltaC = #DeltaR_{jet} #Deltak_{jet} / (#DeltaR_{#gamma} #Deltak_{#gamma})";
  hkqcd->GetYaxis()->SetTitle(title.c_str());
  hkqcd->GetYaxis()->SetRangeUser(0.70,1.40);


  TF1 *fkqcd_jr = new TF1("fkqcd_jr",
			  "(1-[0]*pow(x,[1]-1))/(1-[2]*pow(x,[3]-1))",
			  10.,14000.);
  fkqcd_jr->SetParameters(frjetb->GetParameter(0),frjetb->GetParameter(1),
			  frjets->GetParameter(0),frjets->GetParameter(1));

  TF1 *fkqcd_jrk = new TF1("fkqcd_jrk",
			   "((1-[0]*pow(x,[1]-1))/(1-[2]*pow(x,[3]-1)))"
			   "*(([4]+log(0.01*x)*([5]+log(0.01*x)*[6]))"
			   "/([9]-[7]*pow(x,[8]-1)))",
			   10.,14000.);
  fkqcd_jrk->SetParameters(frjetb->GetParameter(0),frjetb->GetParameter(1),
			   frjets->GetParameter(0),frjets->GetParameter(1),
			   fkjetb->GetParameter(0),fkjetb->GetParameter(1),
			   fkjetb->GetParameter(2),
			   fkjets->GetParameter(0),fkjets->GetParameter(1),
			   fkjets->GetParameter(2));

  TF1 *fkqcd_pr = new TF1("fkqcd_pr",
			  "1./((1-[0]*pow(x,[1]-1))/(1-[2]*pow(x,[3]-1)))",
			  10.,14000.);
  fkqcd_pr->SetParameters(frphob->GetParameter(0),frphob->GetParameter(1),
			  frphos->GetParameter(0),frphos->GetParameter(1));

  TF1 *fkqcd_prk = new TF1("fkqcd_prk",
			   "1./(((1-[0]*pow(x,[1]-1))/(1-[2]*pow(x,[3]-1)))"
			   "*(([4]+log(0.01*x)*([5]+log(0.01*x)*[6]))"
			   "/([9]-[7]*pow(x,[8]-1))))",
			   10.,14000.);
  fkqcd_prk->SetParameters(frphob->GetParameter(0),frphob->GetParameter(1),
			   frphos->GetParameter(0),frphos->GetParameter(1),
			   fkphob->GetParameter(0),fkphob->GetParameter(1),
			   fkphob->GetParameter(2),
			   fkphos->GetParameter(0),fkphos->GetParameter(1),
			   fkphos->GetParameter(2));

  TF1 *fkqcd  = new TF1("fkqcd",
			"((1-[0]*pow(x,[1]-1))/(1-[2]*pow(x,[3]-1)))" //jr
			"*(([4]+log(0.01*x)*([5]+log(0.01*x)*[6]))"
			"/([9]-[7]*pow(x,[8]-1)))" //jk
			"/(((1-[10]*pow(x,[11]-1))/(1-[12]*pow(x,[13]-1)))"//pr
			"*(([14]+log(0.01*x)*([15]+log(0.01*x)*[16]))"
			"/([19]-[17]*pow(x,[18]-1))))", //pk
			10., 14000.);

  fkqcd->SetParameter(0,frjetb->GetParameter(0));
  fkqcd->SetParameter(1,frjetb->GetParameter(1));
  fkqcd->SetParameter(2,frjets->GetParameter(0));
  fkqcd->SetParameter(3,frjets->GetParameter(1));
  fkqcd->SetParameter(4,fkjetb->GetParameter(0));
  fkqcd->SetParameter(5,fkjetb->GetParameter(1));
  fkqcd->SetParameter(6,fkjetb->GetParameter(2));
  fkqcd->SetParameter(7,fkjets->GetParameter(0));
  fkqcd->SetParameter(8,fkjets->GetParameter(1));
  fkqcd->SetParameter(9,fkjets->GetParameter(2));
  fkqcd->SetParameter(10,frphob->GetParameter(0));
  fkqcd->SetParameter(11,frphob->GetParameter(1));
  fkqcd->SetParameter(12,frphos->GetParameter(0));
  fkqcd->SetParameter(13,frphos->GetParameter(1));
  fkqcd->SetParameter(14,fkphob->GetParameter(0));
  fkqcd->SetParameter(15,fkphob->GetParameter(1));
  fkqcd->SetParameter(16,fkphob->GetParameter(2));
  fkqcd->SetParameter(17,fkphos->GetParameter(0));
  fkqcd->SetParameter(18,fkphos->GetParameter(1));
  fkqcd->SetParameter(19,fkphos->GetParameter(2));
  
  fkqcd->SetLineColor(kBlack);
  fkqcd->SetLineStyle(kSolid);
  fkqcd->SetLineWidth(3);

  // Printout the parameters
  cout << "Parameters ("<<idtype<<"):" << endl;
  for (int i = 0; i != 20; ++i)
    cout << Form("%1.4g, ", fkqcd->GetParameter(i));
  cout << endl << endl;

  TLegend *lkqcd1 = new TLegend(0.69,0.60,0.92,0.93,_algoname.c_str(),"brNDC");
  lkqcd1->SetBorderSize(0);
  lkqcd1->SetFillStyle(kNone);
  lkqcd1->SetTextSize(0.04);
  TLegend *lkqcd2 = new TLegend(0.69,0.20,0.92,0.36,"","brNDC");
  lkqcd2->SetBorderSize(0);
  lkqcd2->SetFillStyle(kNone);
  lkqcd2->SetTextSize(0.04);
  TLatex *tkqcd = new TLatex(0.25,0.87,(idtype+"ID").c_str());
  tkqcd->SetNDC();
  tkqcd->SetTextSize(0.04);
  //
  fkqcd_pr->SetLineColor(kRed);
  fkqcd_pr->SetLineStyle(kDashed);
  fkqcd_pr->SetLineWidth(1);
  fkqcd_prk->SetLineColor(kRed);
  fkqcd_prk->SetLineStyle(kDashed);
  fkqcd_prk->SetLineWidth(3);//2);
  grkphor->SetMarkerStyle(kCircle);
  grkphor->SetMarkerColor(kRed);
  grkphor->SetMarkerSize(0.8);
  grkphor->SetLineColor(kRed);
  grkphor->SetLineWidth(3);//2);
  grkphor->SetLineStyle(kDashed);
  //
  fkqcd_jr->SetLineColor(kBlue);
  fkqcd_jr->SetLineStyle(kDotted);
  fkqcd_jr->SetLineWidth(1);
  fkqcd_jrk->SetLineColor(kBlue);
  fkqcd_jrk->SetLineStyle(kDotted);
  fkqcd_jrk->SetLineWidth(3);//2);
  grkjetr->SetMarkerStyle(kOpenSquare);
  grkjetr->SetMarkerColor(kBlue);
  grkjetr->SetMarkerSize(0.8);
  grkjetr->SetLineColor(kBlue);
  grkjetr->SetLineWidth(3);//2);
  grkjetr->SetLineStyle(kDotted);
  //
  grmeasr->SetMarkerStyle(kOpenDiamond);
  grmeasr->SetMarkerSize(1.2);
  grmeasr->SetLineWidth(2);
  lkqcd1->AddEntry(grkphor, "(#DeltaR_{#gamma} #Deltak_{#gamma})^{-1}", "LP");
  lkqcd1->AddEntry(fkqcd_pr, "(#DeltaR_{#gamma})^{-1}", "L");
  lkqcd1->AddEntry(fkqcd, "#DeltaC", "L");
  lkqcd2->AddEntry(fkqcd_jr, "#DeltaR_{jet}", "L");
  lkqcd2->AddEntry(grkjetr, "#DeltaR_{jet} #Deltak_{jet}", "LP");

  hkqcd->Draw();
  fkqcd_jr->Draw("same");
  fkqcd_jrk->Draw("same");
  fkqcd_pr->Draw("same");
  fkqcd_prk->Draw("same");
  fkqcd->Draw("same");
  // Uncomment these when statistics better
  //grkjetr->Draw("same p");
  //grkphor->Draw("same p");
  if (drawBars) grmeasr->Draw("same p");
  lkqcd1->Draw();
  lkqcd2->Draw();
  tkqcd->Draw();

  ckqcd->SaveAs("eps/mcfactors_deltac.eps");


  // Plot of combined background + photon scale/topology correction
  /*{
    TCanvas *ccmb = new TCanvas("ccomb","ccomb",600,600);
    
    // Generate bins evenly in log(x)
    double xmin = 40;
    double xmax = 700.;
    vector<double> bins(101.);
    double r = pow( (xmin-xmax)/xmin , 1./(bins.size()-1.) );
    bins[0] = xmin;
    for (unsigned int i = 1; i != bins.size(); ++i)
      bins[i] = r*bins[i-1];

    TH1D *hbkg = new TH1D("hbkg", bins.size()-1, &bins[0]);
  
    for (int i = 0; 

  } // plot of combined correction
  */

  if (gROOT->IsBatch()) {

    delete hrjet;
    delete hkjet;
    delete hktopo;
    delete hrpho;
    delete hkpho;
    delete hkgam;
    delete hkqcd;
  }

  return 0;
}



// Do ratio c = a/b for match |xa-xb|/(0.5*(xa+xb))<xmatch
void doRatio(const TGraphErrors *a, const TGraphErrors *b, TGraphErrors *c,
	     double xmatch) {

  c->Set(0);
  for (int i = 0; i != a->GetN(); ++i) {

    double xa,ya,xb,yb;
    a->GetPoint(i, xa, ya);
    double dptmin = 99999;
    int jmin = -1;
    for (int j = 0; j != b->GetN(); ++j) {
      b->GetPoint(j, xb, yb);
      if (fabs(xb-xa) < dptmin) {
	jmin = j;
	dptmin = fabs(xb-xa);
      }
    } // for j
    
    b->GetPoint(jmin, xb, yb);
    double dx = 0.5*fabs(xb-xa);
    double x = 0.5*fabs(xb+xa);
    double r = (yb ? ya/yb : 0.);
    if (dx < xmatch*x) {
      int n = c->GetN();
      c->SetPoint(n, x, r);
      double xaerr = a->GetErrorX(i);
      double yaerr = a->GetErrorY(i);
      double xberr = b->GetErrorX(jmin);
      double yberr = b->GetErrorY(jmin);
      double xerr = sqrt(xaerr*xaerr + xberr*xberr + dx*dx);
      double rerr = sqrt(yaerr*yaerr + (ya*ya*yberr*yberr)/(yb*yb)) / yb;
      c->SetPointError(n, xerr, rerr);
    }
  
  } // for i

  return;
} // doRatio
  

double FindPeak(TH1D* h, double &err) {

  TF1 *f = new TF1("g","gaus",0.,2.);
  h->Fit(f, "QRN");
  f->SetRange(f->GetParameter(1)-2.*f->GetParameter(2),
	      f->GetParameter(1)+2.*f->GetParameter(2));
  h->Fit(f, "QRN");
  f->SetRange(f->GetParameter(1)-1.5*f->GetParameter(2),
	      f->GetParameter(1)+1.5*f->GetParameter(2));
  h->Fit(f, "QRN");
  f->SetRange(f->GetParameter(1)-1.*f->GetParameter(2),
	      f->GetParameter(1)+1.*f->GetParameter(2));
  h->Fit(f, "QRN");
  // Could determine from mean-mu on which side the tails are
  // or both side, if mean-mu~0, but sigma<<RMS;
  // then fit with approriate version of Crystal Ball

  err = f->GetParError(1)
    * sqrt(max(1.,f->GetChisquare()/max(1,f->GetNDF())));

  return f->GetParameter(1);
} // FindPeak

// Divide TGraphErrors by the function TF1
// If TGraphErrors has functions associated to it, divide them, too
TGraphErrors *divide(const TGraphErrors *g, const TF1 *f) {

  TGraphErrors *ng = (TGraphErrors*)g->Clone();
 
  for (int i = 0; i != g->GetN(); ++i) {
    double x, y;
    g->GetPoint(i, x, y);
    double fy = f->Eval(x);
    ng->SetPoint(i, x, y/fy);
    ng->SetPointError(i, g->GetErrorX(i), g->GetErrorY(i)/fy);
  }

  if (ng->GetListOfFunctions()) {

    ng->GetListOfFunctions()->Clear();
    TListIter it(g->GetListOfFunctions());
    TObject *obj;

    while (obj = it.Next()) {

      TF1 *f1 = (TF1*)obj;
      string formula = Form("(%s)/(%s)",&f1->GetExpFormula("p")[0],
			    &f->GetExpFormula("p")[0]);
      double xmin, xmax;
      f1->GetRange(xmin, xmax);
      TF1 *f2 = new TF1(Form("%s_r",f1->GetName()),formula.c_str(),xmin,xmax);
      f2->SetLineWidth(f1->GetLineWidth());
      f2->SetLineColor(f1->GetLineColor());
      ng->GetListOfFunctions()->Add(f2);
    } // while
  }

  return ng;
}
