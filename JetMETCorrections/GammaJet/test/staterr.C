#include "TF1.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TGraphErrors.h"
#include "TRandom3.h"
#include "TTimeStamp.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMinuit.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TMath.h"
#include "TList.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TFile.h"

#include <iostream>

using namespace std;


int kDarkYellow = kYellow+100;
int kDarkGreen = kGreen+100;
const int kRightHatch = 3454;//3654;
const int kLeftHatch = 3445;//3645;
const int kVertHatch = 3499;//3645;

const bool useBinnedXsec = true;
TH1D *h_xsec = 0;

Double_t cross_section(Double_t *xx, Double_t *p) {
  double x = (*xx);
  if (x<p[3] || x>=p[4]) return 0.;
  if (useBinnedXsec && h_xsec) {
    return h_xsec->Interpolate(x);
  }
  return (p[0]*pow(x,p[1])*exp(p[2]*x));
}
Double_t cross_section_x(Double_t *xx, Double_t *p) {
  return ( (*xx) * cross_section(xx, p) );
}

// Divide TGraphErrors/TH1 by the function TF1
// If TGraphErrors/TH1 has functions associated to it, divide them, too
TGraphErrors *divide(const TGraphErrors *g, const TF1 *f);
TH1D *divide(const TH1D *g, const TF1 *f);

// Create uncertainty band (histogram) for a given function and error matrix
// in the range of the function
TH1D *getBand(const TF1 *f, TMatrixD const& m, int npx = 100);

void staterr() {

#if defined (__CINT__)
  cout << "This program will only run (not crash) in compiled mode!" << endl
       << "Call 'root -l staterr.C+'" << endl;
  exit(-1);
#endif

  gROOT->ProcessLine(".x cms_jes_style.C");
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetTitleOffset(1.0,"y");

  bool _randstat = true; // randomize statistics also
  bool _notemode = true;
  bool _fptalk = true;

  double lumi = 1.;//10.;//10.;
  //int XBINS = 27;
  double xmin = 25.;
  double xmax = 700.;
  const double xbins0[] =
    //{25, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300,
    //325, 350, 375, 400, 425, 450, 475, 500, 525, 550, 575, 600,
    //625, 650, 675, 700};
    // for 10 pb-1
    //{25, 30, 36, 43, 52, 62, 75, 90, 107, 129, 155, 186, 225, 270, 500, 700};
    // for 100 pb-1
    //{25, 30, 36, 43, 52, 62, 75, 90, 107, 129, 155, 186, 225, 270,
    //320, 385, 460, 560, 800};
    // for 10 pb-1, sqrt(s)=10 TeV
    {25, 30, 36, 43, 52, 62, 75, 90, 107, 129, 155, 186, 225, 460, 700};
  const int XBINS = sizeof(xbins0)/sizeof(xbins0[0])-1;
  const double* xbins = &xbins0[0];
  int YBINS = 27;
  double ymin = 0.;
  double ymax = 2.;
  const double ns = 1.5; // Gaussian peak fit range

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  TCanvas *c0 = new TCanvas("c0","c0",600,600);
  c0->SetLogx();
  c0->SetLogy();

  if (useBinnedXsec) {

    TFile *f = new TFile("output/output_mixed_medium_ite.root","READ");
    assert(!f->IsZombie());
    h_xsec = (TH1D*)f->Get("ptphot_sig");
    TH1D *h_xsec_bkg = (TH1D*)f->Get("ptphot_bkg");
    assert(h_xsec);
    // Normalize cross section units to 1./(25 GeV * fb)
    for (int i = 1; i != h_xsec->GetNbinsX()+1; ++i)
      h_xsec->SetBinContent(i, h_xsec->GetBinContent(i)
			    / h_xsec->GetBinWidth(i) * 25.);
    for (int i = 1; i != h_xsec_bkg->GetNbinsX()+1; ++i)
      h_xsec_bkg->SetBinContent(i, h_xsec_bkg->GetBinContent(i)
				/ h_xsec_bkg->GetBinWidth(i) * 25.);
    // Correct for pTphot>25 threshold
    if (h_xsec_bkg->GetBinLowEdge(1)==15. && h_xsec_bkg->GetBinLowEdge(2)==30.)
      h_xsec_bkg->SetBinContent(1, h_xsec_bkg->GetBinContent(1)*15./5.);
    cout << "Using binned cross section" << endl;

    TH1D *h0 = new TH1D("h0","h0",1000,0.,1000.);
    h0->SetMinimum(0.1);//1.);
    h0->SetMaximum(1e10);//1e8);//1e7);
    h0->GetXaxis()->SetTitle("p_{T}^{#gamma} (GeV)");
    h0->GetXaxis()->SetMoreLogLabels();
    h0->GetXaxis()->SetNoExponent();
    h0->GetXaxis()->SetRangeUser(25.,900.);
    h0->GetYaxis()->SetTitle("events / 25 GeV / fb^{-1}");
    h0->Draw();

    TLegend *l0 = new TLegend(0.50,0.70,0.90,0.90);
    l0->SetFillStyle(kNone);
    l0->SetBorderSize(0);
    l0->AddEntry(h_xsec, "#gamma + jet", "P");
    l0->AddEntry(h_xsec_bkg, "QCD background", "P");
    l0->Draw();

    h_xsec_bkg->SetLineWidth(2);
    h_xsec_bkg->SetLineColor(kBlack);
    h_xsec_bkg->SetMarkerStyle(kFullCircle);
    h_xsec_bkg->SetMarkerColor(kBlack);
    h_xsec_bkg->Draw("SAME");

    h_xsec->SetLineWidth(2);
    h_xsec->SetLineColor(kRed);
    h_xsec->SetMarkerStyle(kCircle);
    h_xsec->SetMarkerColor(kRed);
    h_xsec->Draw("SAME");

    c0->SaveAs("eps/toymc/xsec.eps");
  }
  else
    cout << "Using parameterized cross section" << endl;

  // Photon+jets after photonID and kinematic cuts
  // events/25 GeV/fb-1
  // NB: first three parameters are meaningless if h_xsec is provided
  TF1 *xsec = new TF1("xsec", cross_section, xmin, xmax, 5);
  xsec->SetParameters(7.729e+10, -3.104, -0.003991, xmin, xmax);
  xsec->SetNpx(1000);
  TF1 *xsec_x = new TF1("xsec_x", cross_section_x, xmin, xmax, 5);
  xsec_x->SetParameters(7.729e+10, -3.104, -0.003991, xmin, xmax);
  xsec_x->SetNpx(1000);

  TF1 *mean = new TF1("mean", "1 - [0] * pow(x, [1])", xmin, xmax);
  //mean->SetParameters(2.112, -0.3986); // CSA07 ( 1-1.881*pow(x,0.6198-1) )
  mean->SetParameters(2.136,0.5801-1); // Summer08
  //TF1 *rjet2 = new TF1("rjet2", "[2] - [0] * pow(x, [1])", xmin, xmax);
  TF1 *rjet2 = new TF1("rjet2", "[2] - [0] * pow(0.01*x, [1]-1)", xmin, xmax);
  //rjet2->SetParameters(2.112*0.1, -0.3986+1, 1.);
  rjet2->SetParameters(2.112*pow(100.,-0.3986), -0.3986+1, 1.);
  //TF1 *rjet4 = new TF1("rjet4", "[2] - [0] * pow(x, [1])", xmin, xmax);
  TF1 *rjet4 = new TF1("rjet4", "[2] - [0] * pow(0.01*x, [1]-1)", xmin, xmax);
  rjet4->SetParameters(2.112*pow(100.,-0.3986), -0.3986+1, 1.);
		      
  TF1 *res = new TF1("res", "sqrt([0]*[0]/(x*x) + [1]*[1]/x + [2]*[2])",
		     xmin, xmax);
  res->SetParameters(11.64, 1.740, 0.07823);
  

  TF1 *rjet2a = new TF1("rjet2a", "1 - [0] * pow(0.01*x, [1]-1)", xmin, xmax);
  rjet2a->SetParameters(2.112*0.1, -0.3986+1);

  TF1 *rjet2b = new TF1("rjet2b","[0]-[1]/(pow(log10(x),[2])+[3])+[4]/x",
			xmin, xmax);
  rjet2b->SetParameter(0,1.);
  rjet2b->SetParameter(1,5.);
  rjet2b->SetParameter(2,5.);
  rjet2b->SetParameter(3,2.);
  rjet2b->SetParameter(4,1.);
  rjet2b->SetParameter(5,1.);
  //rjet2b->SetParLimits(2,0.,10000.);
  //rjet2b->SetParLimits(3,0.,10000.);
  
  //TF1 *gaus = new TF1("fg", "TMath::Gaus(x,[0],[1],0)", 0., 2.); // slow!
  TF1 *gaus = new TF1("fg", "gaus", 0., 2.);
  TF1 *gaus2 = new TF1("fg2", "gaus", 0., 2.);
  TF1 *gaus_x = new TF1("fg_x", "gaus(0)*x", 0., 2.);

  //responsevspt_sig = new TH2D("responsevspt_sig", "responsevspt_sig",
  //		      XBINS,xmin,xmax, YBINS,ymin,ymax);
  //responsevspt_sig->Sumw2();
  TH1D *temphist_sig = new TH1D("temphist_sig","", YBINS, ymin, ymax);
  temphist_sig->Sumw2();
  //temphist_sig->SetMarkerStyle(8);
  //temphist_sig->SetMarkerColor(kBlue);
  //temphist_sig->SetMarkerSize(.7);
  //temphist_sig->SetStats(0);
  //temphist_sig->SetXTitle("p_{T}(jet) / p_{T}(#gamma)");
  TH1D *tempprof_sig = new TProfile("tempprof_sig", "", //XBINS, xmin, xmax);
				    XBINS, xbins);

  TH1D *ptmean_sig = new TProfile("ptmean_sig","ptmean_sig",//XBINS,xmin,xmax);
				  XBINS, xbins);
  TH1D *gmean_sig = new TH1D("gmean_sig","gmean",//XBINS,xmin,xmax);
			     XBINS, xbins);
  
  TGraphErrors *gr_sig = new TGraphErrors(0);
  TGraphErrors *gr_sig2 = new TGraphErrors(0);
  TGraphErrors *gr_sig3 = new TGraphErrors(0);
  TGraphErrors *gr_sig4 = new TGraphErrors(0);
  TGraphErrors *gr_sig5 = new TGraphErrors(0);
  
  // Set a random seed for the random number sequence
  //TTimer t;
  //gRandom->SetSeed(t.GetTime());
  TTimeStamp t;
  TRandom3 *rand = new TRandom3();
  //rand->SetSeed(t.GetNanoSec()/1000);//123456);
  //grand->SetSeed(624161); // note default (summer08 - AN2009-012/v4)
  rand->SetSeed(771156000); // note default
  //rand->SetSeed(293658000); // very pretty curves with chi2/NDF=1.053
  //rand->SetSeed(63900000); // representative for stat uncertainty (note)
  gRandom = rand; // can we replace this?

  cout << Form("// TRandom3 seed %d", gRandom->GetSeed()) << endl;

  int cnt = 0;
  for (int i = 1; i != gmean_sig->GetNbinsX()+1; ++i) {
    
    // Estimate average pT and number of events in bin
    double binmin = gmean_sig->GetBinLowEdge(i);
    double binmax = gmean_sig->GetBinLowEdge(i+1);
    double pt = xsec_x->Integral(binmin,binmax)/xsec->Integral(binmin,binmax);
    double nev = xsec->Integral(binmin, binmax) / 25. / 1000. * lumi;
    int np = (_randstat ? gRandom->Poisson(nev) : int(nev));

    // Generate toy MC
    double m = mean->Eval(pt);
    double s = res->Eval(pt) * m;
    //gaus->SetParameters(m, s);
    gaus->SetParameters(np/(s*sqrt(TMath::TwoPi())), m, s);
    gaus2->SetParameters(np/(s*sqrt(TMath::TwoPi())), m, s);
    gaus2->FixParameter(2, s);
    //gaus_x->SetParameters(np/(s*sqrt(TMath::TwoPi())), m, s);

    cout << Form("ptbin %1.0f-%1.0f av %1.1f, nav=%1.1f / np=%d"
		 " (mu=%1.2f, sig=%1.3f)",
		 binmin, binmax, pt, nev, np, m, s/m) << endl;

    temphist_sig->Reset();
    for (int j = 0; j != np; ++j) {

      double pt = xsec_x->GetRandom(binmin, binmax);
      ptmean_sig->Fill(pt, pt);
      double r = gaus->GetRandom(ymin, ymax);
      //gmean_sig->Fill(pt, r);
      temphist_sig->Fill(r);
      tempprof_sig->Fill(pt, r);
      //responsevspt_sig->Fill(pt, r);

      if (++cnt%100000==0) cout << "." << flush;
      if (++cnt%8000000==0) cout << endl;
    }
    if (np==0) continue;

    temphist_sig->Fit(gaus, (np>10 ? "QRN" : "QRNL"));
    temphist_sig->Fit(gaus2, (np>10 ? "QRN" : "QRNL"));

    // Repeat Gaussian fit for the peak +/- 1.5 sigma
    gaus->SetRange(gaus->GetParameter(1) - ns*gaus->GetParameter(2),
		   gaus->GetParameter(1) + ns*gaus->GetParameter(2));
    temphist_sig->Fit(gaus, (np>10 ? "QRN" : "QRNL"));
    gaus->SetRange(gaus->GetParameter(1) - ns*gaus->GetParameter(2),
		   gaus->GetParameter(1) + ns*gaus->GetParameter(2));
    temphist_sig->Fit(gaus, (np>10 ? "QRN" : "QRNL"));
    gaus->SetRange(0., 2.);

    // Low pT tails being cut out by r>0. cause 0.02/0.44~4% bias at pT=25 GeV
    // Calculate a correction factor for the fraction being cut out
    gaus_x->SetParameters(gaus->GetParameter(0),gaus->GetParameter(1),
			  gaus->GetParameter(2));
    double corr = (gaus_x->Integral(-1,3)/gaus->Integral(-1,3))
      /(gaus_x->Integral(0.,2.)/gaus->Integral(0.,2.));

    double x = ptmean_sig->GetBinContent(i);
    double ex = ptmean_sig->GetBinError(i);
    //double y = gmean_sig->GetBinContent(i);
    //double ey = gmean_sig->GetBinError(i);
    double y = temphist_sig->GetMean() * corr;
    double y2 = temphist_sig->GetMean() * corr;
    double y3 = tempprof_sig->GetBinContent(i) * corr;
    double y4 = gaus->GetParameter(1);
    double y5 = gaus2->GetParameter(1);
    double ey = temphist_sig->GetMeanError() * corr;
    double ey2 = s/sqrt(np) * corr;//s/sqrt(nev);
    double ey3 = tempprof_sig->GetBinError(i) * corr;
    double ey4 = gaus->GetParError(1);
    double ey5 = gaus2->GetParError(1);

    if (x != 0 && ex != 0 && y != 0 && ey != 0) {
      int n = gr_sig->GetN();
      gr_sig->SetPoint(n, x, y);
      gr_sig->SetPointError(n, ex, ey);
    }
    if (x != 0 && ex != 0 && y2 != 0 && y<=2 && ey2 != 0) {
      int n = gr_sig2->GetN();
      gr_sig2->SetPoint(n, x, y2);
      gr_sig2->SetPointError(n, ex, ey2);
    }
    if (x != 0 && ex != 0 && y3 != 0 && ey3 != 0) {
      int n = gr_sig3->GetN();
      gr_sig3->SetPoint(n, x, y3);
      gr_sig3->SetPointError(n, ex, ey3);
    }
    if (x != 0 && ex != 0 && y4 != 0 && ey4 != 0) {
      int n = gr_sig4->GetN();
      gr_sig4->SetPoint(n, x, y4);
      gr_sig4->SetPointError(n, ex, ey4);
    }
    if (x != 0 && ex != 0 && y5 != 0 && ey5 != 0) {
      int n = gr_sig5->GetN();
      gr_sig5->SetPoint(n, x, y5);
      gr_sig5->SetPointError(n, ex, ey5);
    }
  } // for i


  TCanvas *c1 = new TCanvas();
  if (_fptalk) {
    delete c1; c1 = new TCanvas("c1","c1",600,600);
  }
  c1->SetLogx();
  
  TH1D *h1 = new TH1D("h1", "", 2000, 20., 700.);
  h1->GetXaxis()->SetMoreLogLabels();
  h1->GetXaxis()->SetNoExponent();
  h1->GetXaxis()->SetTitle("p_{T} (GeV)");
  h1->GetYaxis()->SetRangeUser(0.3, 1.1);
  h1->GetYaxis()->SetTitle(Form("ToyMC jet response (%1.0f pb^{-1})",lumi));
  if (_fptalk) {
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetTitleOffset(1.2);
  }

  rjet2->SetLineColor(_notemode ? kBlack : kBlue);
  rjet2->SetLineWidth(1);
  gr_sig2->Fit(rjet2, "QR");

  // Retrieve and factorize correlation matrix for L3Corr.cpp
  const int ndim2 = rjet2->GetNpar();
  TMatrixD emat2(ndim2, ndim2);
  gMinuit->mnemat(&emat2[0][0], ndim2);

  TH1D *he_sig2 = getBand(rjet2, emat2);

  /*
  // Alternative fit with 2-parameter powerlaw
  rjet2a->SetLineColor(kDarkGreen);
  gr_sig2->Fit(rjet2a,"QR+");

  const int ndima = rjet2a->GetNpar();
  TMatrixD emata(ndima, ndima);
  gMinuit->mnemat(&emata[0][0], ndima);

  TH1D *he_sig2a = getBand(rjet2a, emata);

  // Another alternative fit with 5-parameter function from Kostas/Daniele
  rjet2b->SetLineColor(kBlue);
  gr_sig2->Fit(rjet2b,"QR+");//"MR");

  const int ndimb = rjet2b->GetNpar();
  TMatrixD ematb(ndimb, ndimb);
  gMinuit->mnemat(&ematb[0][0], ndimb);

  TH1D *he_sig2b = getBand(rjet2b, ematb);
  */

  /*
  TVectorD eigvs(ndim);
  TMatrixD eigs(emat.EigenVectors(eigvs));

  // Print out the signal fit parameters to be used in L3Corr.cpp
  cout << Form("// Signal sample (toyMC %d pb-1):", lumi) << endl;
  cout << Form("_pstat[0] = %1.4g; _pstat[1] = %1.4g; _pstat[2] = %1.4g;"
	       " // [2]+/-%1.3f",
	       rjet2->GetParameter(0), rjet2->GetParameter(1),
	       rjet2->GetParameter(2), rjet2->GetParError(2)) << endl;

  cout << Form("_chi2 = %1.4g; // %1.1f/%d",
	       rjet2->GetChisquare()/rjet2->GetNDF(),
	       rjet2->GetChisquare(),rjet2->GetNDF()) << endl;

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
  */

  gr_sig2->SetName("gr_sig2");
  gr_sig2->SetMarkerStyle(kFullCircle);
  gr_sig2->SetMarkerSize(1.);
  gr_sig2->SetMarkerColor(kBlue);
  gr_sig2->SetLineColor(kBlue);
  gr_sig2->SetLineWidth(2);
  
  gr_sig->SetName("gr_sig");
  gr_sig->SetMarkerStyle(kOpenCircle);
  gr_sig->SetMarkerSize(1.);
  gr_sig->SetMarkerColor(kRed);
  gr_sig->SetLineColor(kRed);
  gr_sig->SetLineWidth(2);

  // Basically the same as gr_sig
  // Apparently histograms keep unbinned mean and uncertainty
  gr_sig3->SetName("gr_sig3");
  gr_sig3->SetMarkerStyle(kOpenSquare);
  gr_sig3->SetMarkerSize(1.);
  gr_sig3->SetMarkerColor(kDarkYellow);
  gr_sig3->SetLineColor(kDarkYellow);

  /*
  // Same uncertainty as with free sigma in the fit
  gr_sig5->SetName("gr_sig4");
  gr_sig5->SetMarkerStyle(kOpenDiamond);
  gr_sig5->SetMarkerSize(1.);
  gr_sig5->SetMarkerColor(kBlack);
  gr_sig5->SetLineColor(kBlack);
  gr_sig5->Draw("SAMEP");http://indico.cern.ch/getFile.py/access?contribId=92&sessionId=6&resId=1&materia
  */

  rjet4->SetLineColor(_notemode ? kRed : kDarkGreen);
  rjet4->SetLineWidth(1);
  gr_sig4->Fit(rjet4, "QR");

  const int ndim4 = rjet4->GetNpar();
  TMatrixD emat4(ndim4, ndim4);
  gMinuit->mnemat(&emat4[0][0], ndim4);
  TH1D *he_sig4 = getBand(rjet4, emat4);

  // Solve eigenvectors for arithmetic mean (emat2)
  // Optional would be for gaussian mean (emat4)
  TVectorD eigvs(ndim2);
  TMatrixD eigs(emat2.EigenVectors(eigvs));

  // Print out the signal fit parameters to be used in L3Corr.cpp
  //cout << Form("// Signal sample (toyMC %1.0f pb-1, %1.1f-sigma peak):",
  //	       lumi, ns) << endl;
  cout << Form("// Signal sample (toyMC %1.0f pb-1, %s, arithmetic mean):",
	       lumi, (useBinnedXsec ? "binned xsec" : "fitted xsec")) << endl;
  cout << Form("_pstat[0] = %1.4g; _pstat[1] = %1.4g; _pstat[2] = %1.4g;"
	       " // [2]+/-%1.3f",
	       rjet2->GetParameter(0), rjet2->GetParameter(1),
	       rjet2->GetParameter(2), rjet2->GetParError(2)) << endl;

  cout << Form("_chi2 = %1.4g; // %1.1f/%d",
	       rjet2->GetChisquare()/rjet2->GetNDF(),
	       rjet2->GetChisquare(),rjet2->GetNDF()) << endl;

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

  gr_sig4->SetName("gr_sig4");
  gr_sig4->SetMarkerStyle(kOpenCircle);
  gr_sig4->SetMarkerSize(1.);
  gr_sig4->SetMarkerColor(kDarkGreen);
  gr_sig4->SetLineColor(kDarkGreen);

  if (_notemode) {
    rjet2->SetLineColor(kBlack);
    gr_sig2->SetLineColor(kBlack);
    gr_sig2->SetMarkerColor(kBlack);
    he_sig2->SetLineColor(kBlack);
    he_sig2->SetFillColor(kBlack);
    he_sig2->SetFillStyle(kRightHatch);
    rjet4->SetLineColor(kRed);
    gr_sig4->SetLineColor(kRed);
    gr_sig4->SetLineWidth(2);
    gr_sig4->SetMarkerColor(kRed);
    he_sig4->SetLineColor(kRed);
    he_sig4->SetFillColor(kRed);
    he_sig4->SetFillStyle(kLeftHatch);
  }
  if (_fptalk) {
    mean->SetLineStyle(kDashed);
  }

  h1->Draw();
  mean->Draw("same");
  he_sig2->Draw("same e4");
  if (!_fptalk) he_sig4->Draw("same e4");
  gr_sig2->Draw("SAMEP");
  if (!_notemode) {
    gr_sig->Draw("SAMEP");  
    gr_sig3->Draw("SAMEP");
  }
  if (!_fptalk) gr_sig4->Draw("SAMEP");

  if (_notemode) {

    //TLegend *leg = new TLegend(0.16, 0.52, 0.45, 0.92, "", "brNDC");
    TLegend *leg = new TLegend(0.16, 0.57, 0.55, 0.92, "", "brNDC");
    if (_fptalk) {
      delete leg; leg = new TLegend(0.16, 0.71, 0.65, 0.92, "", "brNDC");
    }
    leg->SetBorderSize(0);
    leg->SetFillStyle(kNone);
    leg->AddEntry(mean, "Input response" , "L");
    leg->AddEntry(gr_sig2, "Arithmetic mean", "LP");
    if (!_fptalk) leg->AddEntry(gr_sig4, "Gaussian peak", "LP");
    leg->AddEntry(he_sig2, "Fit unc. on mean", "F");
    if (!_fptalk) leg->AddEntry(he_sig4, "Fit unc. on peak", "F");
    //leg->AddEntry(he_sig2, "Arithmetic fit error", "F");
    //leg->AddEntry(he_sig4, "Gaussian fit error", "F");
    leg->Draw();

    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/toymc");
    c1->SaveAs(Form("eps/toymc/staterr%1.0fpb.eps",lumi));
  }


  TCanvas *c2 = new TCanvas();
  if (_fptalk) {
    delete c2; c2 = new TCanvas("c2","c2",600,600);
    c2->SetLeftMargin(0.14); c2->SetRightMargin(0.02);
  }
  c2->SetLogx();
  
  TH1D *h2 = new TH1D("h2", "", 2000, 20., 700.);
  h2->GetXaxis()->SetMoreLogLabels();
  h2->GetXaxis()->SetNoExponent();
  h2->GetXaxis()->SetTitle("p_{T} (GeV)");
  //h2->GetYaxis()->SetRangeUser(0.85, 1.15);
  h2->GetYaxis()->SetRangeUser(0.90, 1.10);
  h2->GetYaxis()->SetTitle(Form("ToyMC over input (%1.0f pb^{-1})",lumi));
  if (_fptalk) {
    h2->GetYaxis()->SetTitleSize(0.05);
    h2->GetYaxis()->SetTitleOffset(1.4);//1.2);
  }
  h2->Draw();

  TGraphErrors *gr2_r = divide(gr_sig2, mean);
  TGraphErrors *gr4_r = divide(gr_sig4, mean);

  TH1D *he2_r = divide(he_sig2, mean);
  he2_r->SetFillStyle(kRightHatch);
  TH1D *he4_r = divide(he_sig4, mean);
  he4_r->SetFillStyle(kLeftHatch);

  /*
  if (_notemode) {
    he2_r->SetFillColor(kBlack);
    he4_r->SetFillColor(kRed);
  }
  */

  he2_r->Draw("SAME E4");
  if (!_fptalk) he4_r->Draw("SAME E4");
  gr2_r->Draw("SAMEP");
  if (!_fptalk) gr4_r->Draw("SAMEP");

  if (_notemode) {

    TLegend *leg = new TLegend(0.16, 0.67, 0.45, 0.93, "", "brNDC");
    if (_fptalk) {
      delete leg; leg = new TLegend(0.16, 0.78, 0.65, 0.92, "", "brNDC");
    }
    leg->SetBorderSize(0);
    leg->SetFillStyle(kNone);
    leg->AddEntry(gr2_r, "Arithmetic mean", "LP");
    if (!_fptalk) leg->AddEntry(gr4_r, "Gaussian peak", "LP");
    leg->AddEntry(he2_r, "Fit unc. on mean", "F");
    if (!_fptalk) leg->AddEntry(he4_r, "Fit unc. on peak", "F");
    //leg->AddEntry(he2_r, "Arithmetic fit error", "F");
    //leg->AddEntry(he4_r, "Gaussian fit error", "F");
    leg->Draw();
    
    c2->SaveAs(Form("eps/toymc/staterr%1.0fpb_r.eps",lumi));
  }


  TCanvas *c3 = new TCanvas();
  c3->SetLogx();
  
  TH1D *h3 = new TH1D("h3", "", 2000, 20., 700.);
  h3->GetXaxis()->SetMoreLogLabels();
  h3->GetXaxis()->SetNoExponent();
  h3->GetXaxis()->SetTitle("p_{T} (GeV)");
  h3->GetYaxis()->SetRangeUser(0.90, 1.10);
  h3->GetYaxis()->SetTitle(Form("ToyMC over input (%1.0f pb^{-1})",lumi));
  h3->Draw();

  // Alternative fit with 2-parameter powerlaw
  rjet2a->SetLineColor(kDarkGreen);
  gr_sig2->Fit(rjet2a,"QR+");

  const int ndima = rjet2a->GetNpar();
  TMatrixD emata(ndima, ndima);
  gMinuit->mnemat(&emata[0][0], ndima);

  TH1D *he_sig2a = getBand(rjet2a, emata);

  // Another alternative fit with 5-parameter function from Kostas/Daniele
  rjet2b->SetLineColor(kBlue);
  gr_sig2->Fit(rjet2b,"QR+");//"MR");

  const int ndimb = rjet2b->GetNpar();
  TMatrixD ematb(ndimb, ndimb);
  gMinuit->mnemat(&ematb[0][0], ndimb);

  TH1D *he_sig2b = getBand(rjet2b, ematb);

  // Redo ratio to get extra fits in
  TGraphErrors *gr2_r3 = divide(gr_sig2, mean);

  TH1D *he2a_r = divide(he_sig2a, mean);
  he2a_r->SetFillStyle(kLeftHatch);
  he2a_r->SetFillColor(kRed);
  TH1D *he2b_r = divide(he_sig2b, mean);
  he2b_r->SetFillStyle(kVertHatch);
  he2b_r->SetFillColor(kBlue);

  if (_notemode) {
    he2a_r->SetLineColor(kRed);
    if (gr2_r3->GetFunction("rjet2a_over_mean"))
      gr2_r3->GetFunction("rjet2a_over_mean")->SetLineColor(kRed);
    if (gr2_r3->GetFunction("rjet2_over_mean"))
      gr2_r3->GetFunction("rjet2_over_mean")->SetLineWidth(2);
  }

  h3->Draw();
  he2b_r->Draw("SAME E4");
  he2_r->Draw("SAME E4");
  he2a_r->Draw("SAME E4");
  gr2_r3->Draw("SAME P");

  if (_notemode) {

    TLegend *leg = new TLegend(0.16, 0.67, 0.45, 0.93, "", "brNDC");
    leg->SetBorderSize(0);
    leg->SetFillStyle(kNone);
    leg->AddEntry(gr2_r, "Arithmetic mean", "LP");
    leg->AddEntry(he2a_r, "2-p powerlaw", "F");
    leg->AddEntry(he2_r, "3-p powerlaw", "F");
    leg->AddEntry(he2b_r, "5-p JEC fit", "F");
    leg->Draw();
    
    c3->SaveAs(Form("eps/toymc/staterr%1.0fpb_rfits.eps",lumi));
  }
  
} // staterr


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

    while ( (obj = it.Next()) ) {

      TF1 *f1 = (TF1*)obj;
      string formula = Form("(%s)/(%s)",&f1->GetExpFormula("p")[0],
			    &f->GetExpFormula("p")[0]);
      double xmin, xmax;
      f1->GetRange(xmin, xmax);
      TF1 *f2 = new TF1(Form("%s_over_%s",f1->GetName(),f->GetName()),
			formula.c_str(),xmin,xmax);
      f2->SetLineWidth(f1->GetLineWidth());
      f2->SetLineColor(f1->GetLineColor());
      ng->GetListOfFunctions()->Add(f2);
    } // while
  }

  ng->SetName(Form("%s_over_%s", g->GetName(), f->GetName()));

  return ng;
}

// Divide TH1D by the function TF1
// If TH1D has functions associated to it, divide them, too
TH1D *divide(const TH1D *g, const TF1 *f) {

  TH1D *ng = (TH1D*)g->Clone(Form("%s_over_%s", g->GetName(), f->GetName()));
 
  for (int i = 1; i != g->GetNbinsX()+1; ++i) {

    double x = g->GetBinCenter(i);
    double y = g->GetBinContent(i);
    double ey = g->GetBinError(i);
    double fy = f->Eval(x);
    ng->SetBinContent(i, y/fy);
    ng->SetBinError(i, ey/fy);
  }

  if (ng->GetListOfFunctions()) {

    ng->GetListOfFunctions()->Clear();
    TListIter it(g->GetListOfFunctions());
    TObject *obj;

    while ( (obj = it.Next()) ) {

      TF1 *f1 = (TF1*)obj;
      string formula = Form("(%s)/(%s)",&f1->GetExpFormula("p")[0],
			    &f->GetExpFormula("p")[0]);
      double xmin, xmax;
      f1->GetRange(xmin, xmax);
      TF1 *f2 = new TF1(Form("%s_over_%s",f1->GetName(),f->GetName()),
			formula.c_str(),xmin,xmax);
      f2->SetLineWidth(f1->GetLineWidth());
      f2->SetLineColor(f1->GetLineColor());
      ng->GetListOfFunctions()->Add(f2);
    } // while
  }

  return ng;
}

// Create uncertainty band (histogram) for a given function and error matrix
// in the range of the function
TH1D *getBand(const TF1 *f, TMatrixD const& m, int npx) {

  Bool_t islog = (gPad ? gPad->GetLogx() : kFALSE);
  double xmin = f->GetXmin();
  double xmax = f->GetXmax();
  int npar = f->GetNpar();
  TString formula = f->GetExpFormula();

  // Create binning (linear or log)
  vector<double> xvec(npx+1);
  xvec[0] = xmin;
  double dx = (islog ? pow(xmax/xmin, 1./npx) : (xmax-xmin)/npx);
  for (int i = 0; i != npx; ++i) {
    xvec[i+1] = (islog ? xvec[i]*dx : xvec[i]+dx);
  }

  // Specialized error function for powerlaw fit
  // (For now, ONLY for powerlaw)
  assert(npar==2 || npar==3 || npar==5);
  //assert(formula=="1-([0]*(x^[1]))" ||
  // formula=="[2]-([0]*(x^[1]))");
  // df/d[0] = -x^[1]
  // df/d[1] = -[0]*log(x)*x^[1]
  // df/d[2] = 1
  assert(formula=="1-([0]*((0.01*x)^([1]-1)))" ||
	 formula=="[2]-([0]*((0.01*x)^([1]-1)))" ||
	 formula=="([0]-([1]/((log10(x)^[2])+[3])))+([4]/x)");
  TF1 *fe = 0;
  if (npar==2 && formula=="1-([0]*((0.01*x)^([1]-1)))") {
    // df/d[0] = -(0.01*x)^([1]-1)
    // df/d[1] = -[0]*log(0.01*x)*(0.01*x)^([1]-1)
    // df/d[2] = 1
    fe = new TF1(Form("fe_%s",f->GetName()),
		 "pow(0.01*x,([1]-1))*sqrt([2]+2*[0]*log(0.01*x)*[3]"
		 "+[0]*[0]*log(0.01*x)*log(0.01*x)*[4])",
		 xmin, xmax);
    fe->SetParameters(f->GetParameter(0), f->GetParameter(1),
		      m[0][0], m[0][1], m[1][1]);
  }
  if (npar==3 && formula=="[2]-([0]*((0.01*x)^([1]-1)))") {
    // df/d[0] = -(0.01*x)^([1]-1)
    // df/d[1] = -[0]*log(0.01*x)*(0.01*x)^([1]-1)
    // df/d[2] = 1
    fe = new TF1(Form("fe_%s",f->GetName()),
		 "sqrt(pow(0.01*x,2*[1]-2)*[2]"
		 "+2*pow(0.01*x,2*[1]-2)*[0]*log(0.01*x)*[3]"
		 "-2*pow(0.01*x,[1]-1)*[4]"
		 "+pow(0.01*x,2*[1]-2)*[0]*[0]*log(0.01*x)*log(0.01*x)*[5]"
		 "-2*pow(0.01*x,[1]-1)*[0]*log(0.01*x)*[6]"
		 "+[7])",
		 xmin, xmax);
    fe->SetParameters(f->GetParameter(0), f->GetParameter(1),
		      m[0][0], m[0][1], m[0][2],
		      m[1][1], m[1][2], m[2][2]);
  }
  if (npar==5 && formula=="([0]-([1]/((log10(x)^[2])+[3])))+([4]/x)") {
    // df/d[0] = 1.
    // df/d[1] = -1./((log10(x)^[2])+[3]))
    // df/d[2] = [1]/((log10(x)^[2])+[3]))^2*log(log10(x))*log10(x)^[2]
    // df/d[3] = [1]/((log10(x)^[2])+[3]))^2
    // df/d[4] = 1./x
    fe =
      new TF1(Form("fe_%s",f->GetName()),
	      "sqrt("
	      "  [5]" // m00
	      "- 2/(pow(log10(x),[2])+[3])*[6]" // m01
	      "+ 2*[1]/pow(pow(log10(x),[2])+[3],2)*log(log10(x))" // m02
	      "   *pow(log10(x),[2])*[7]"
	      "+ 2*[1]/pow(pow(log10(x),[2])+[3],2)*[8]" // m03
	      "+ 2/x*[9]" // m04
	      "+ pow(1/(pow(log10(x),[2])+[3]),2)*[10]" // m11
	      "- 2*[1]/pow(pow(log10(x),[2])+[3],3)*log(log10(x))" // m12
	      "   *pow(log10(x),[2])*[11]"
	      "- 2*[1]/pow(pow(log10(x),[2])+[3],3)*[12]" // m13
	      "- 2/((pow(log10(x),[2])+[3])*x)*[13]" // m14
	      "+ pow([1]/pow(pow(log10(x),[2])+[3],2)*log(log10(x))"
	      "   *pow(log10(x),[2]),2)*[14]" // m22
	      "+ 2*[1]*[1]/pow(pow(log10(x),[2])+[3],4)*log(log10(x))"
	      "   *pow(log10(x),[2])*[15]" // m23
	      "+ 2*[1]/(pow(pow(log10(x),[2])+[3],2)*x)*log(log10(x))"
	      "   *pow(log10(x),[2])*[16]" // m24
	      "+ pow([1]/pow(pow(log10(x),[2])+[3],2),2)*[17]" // m33
	      "+ 2*[1]/(pow(pow(log10(x),[2])+[3],2)*x)*[18]" // m34
	      "+ 1/(x*x)*[19])", // m44
	      xmin, xmax);
    for (int i = 0, k = 0; i != 5; ++i) {
      fe->SetParameter(i, f->GetParameter(i));
      for (int j = i; j != 5; ++j, ++k) {
	fe->SetParameter(k+5, m[i][j]);
	cout << Form("%d:%d,%d; ",k+5,i,j);
      }
    }
    cout << endl;
  }
  assert(fe);
  
  TH1D *h = new TH1D(Form("he_%s",f->GetName()), "", npx, &xvec[0]);

  for (int i = 1; i != h->GetNbinsX()+1; ++i) {

    double x = h->GetBinCenter(i);
    double y = f->Eval(x);
    double ey = fe->Eval(x);
    h->SetBinContent(i, y);
    h->SetBinError(i, ey);
  }

  h->SetLineColor(f->GetLineColor());
  h->SetFillColor(f->GetLineColor());
  h->SetFillStyle(3001);

  return h;
}
