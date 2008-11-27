// This example demonstrates that in the presence of non-Gaussian tails
// only the mean values of distributions are reliable for factorization,
// and the MPV (most probable values, peaks, Gaussian fits) are biased
// The tails and mean values are representative of those in full MC
//
// Also enables to calculate the expected statistical uncertainty of the
// response fit by returning mean and uncertainty.

#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"

#include <iostream>

using namespace std;

Double_t crystal_ball(Double_t *xx, Double_t *p) {

  double x = *xx;
  double N = p[0];
  double mu = p[1];
  double s = p[2];
  double a = p[3];
  double n = p[4];

  double A = pow(n/fabs(a),n)*exp(-a*a/2);
  double B = n/fabs(a) - fabs(a);

  double f = (x-mu > -a*s ?
	      N*exp(-0.5*(x-mu)*(x-mu)/(s*s)) : N*A*pow(B - (x-mu)/s, -n));

  return f;
}
Double_t crystal_ball_x(Double_t *xx, Double_t *p) {
  return ((*xx)*crystal_ball(xx, p));
}
Double_t crystal_ball_x2(Double_t *xx, Double_t *p) {
  return ((*xx)*(*xx)*crystal_ball(xx, p));
}

Double_t crystal_dipole(Double_t *xx, Double_t *p) {

  double x = *xx;
  double N = p[0];
  double mu = p[1];
  double s = p[2];
  double a = p[3];
  double n = p[4];

  double A = pow(n/fabs(a),n)*exp(-a*a/2);
  double B = n/fabs(a) - fabs(a);

  double f = 0;
  if (x-mu < -a*s)
    f = N*A*pow(B - (x-mu)/s, -n);
  else if (x-mu > a*s)
    f = N*A*pow(B + (x-mu)/s, -n);
  else
    f = N*exp(-0.5*(x-mu)*(x-mu)/(s*s));

  return f;
}
Double_t crystal_dipole_x(Double_t *xx, Double_t *p) {
  return ((*xx)*crystal_dipole(xx, p));
}
Double_t crystal_dipole_x2(Double_t *xx, Double_t *p) {
  return ((*xx)*(*xx)*crystal_dipole(xx, p));
}

Double_t cross_section(Double_t *xx, Double_t *p) {
  double x = (*xx);
  if (x<p[3] || x>=p[4]) return 0.;
  return (p[0]*pow(x,p[1])*exp(p[2]*x));
}
Double_t cross_section_x(Double_t *xx, Double_t *p) {
  double x = (*xx);
  if (x<p[3] || x>=p[4]) return 0.;
  return (x*p[0]*pow(x,p[1])*exp(p[2]*x));
}


void toymc(double ptmin = 290., double ptmax = 320., double lumi = 0,
	   double *nevts = 0, double *mean = 0, double *err = 0) {

  gStyle->SetOptStat(1110);
  gStyle->SetOptFit();

  TF1 *cb1 = new TF1("cb1",crystal_ball, 0., 2., 5);
  TF1 *cb1x = new TF1("cb1x",crystal_ball_x, 0., 2., 5);
  TF1 *g = new TF1("g","gaus",0.,2.);
  TF1 *cb2 = new TF1("cb2",crystal_ball, 0., 2., 5);
  TF1 *cb2x = new TF1("cb2x",crystal_ball_x, 0., 2., 5);
  TF1 *cb3 = new TF1("cb3",crystal_ball, 0., 2., 5);
  TF1 *cb3x = new TF1("cb3x",crystal_ball_x, 0., 2., 5);
  TF1 *cb4 = new TF1("cb4",crystal_dipole, 0., 2., 5);
  TF1 *cb4x = new TF1("cb4x",crystal_dipole_x, 0., 2., 5);

  double mu = 0.70; // mu1*mu2
  double mu1 = 0.95;
  double mu2 = mu/0.95;
  double mu3 = 1.00;
  double mu4 = 1.00;
  // Sigmas are relative widths so remember to multiply by mean
  double sig = 0.12; // sqrt(sig1**2 + sig2**2)
  double sig1 = 0.06;
  double sig2 = sqrt(sig*sig-sig1*sig1);
  double sig3 = 0.025;
  double sig4 = 0.030;
  double a1 = 0.7;//1.2;
  double n1 = 15.;
  double a2 = 1.7;
  double n2 = 15.;
  double a3 = 1.0;
  double n3 = 25.;
  double a4 = 0.8;
  double n4 = 35.;

  const bool photbin = false;//true; // Bin in calo photon pT, not parton pT
  double binmin = ptmin;//290;
  double binmax = ptmax;//320;
  double genmin = binmin*0.93;//0.9;
  double genmax = binmax*1.40;//1.20;

  //TF1 *xsec = new TF1("xsec","7.729e+10*pow(x,-3.104)*exp(-0.003991*x)",
  TF1 *xsec = new TF1("xsec", cross_section, 0., 14000., 5);
  xsec->SetParameters(7.729e+10, -3.104, -0.003991, genmin, genmax);
  xsec->SetNpx(1000);

  // Estimate average pT and number of events in bin
  TF1 *xsec_x = new TF1("xsec_x", cross_section_x, 0., 14000., 5);
  xsec_x->SetParameters(7.729e+10, -3.104, -0.003991, genmin, genmax);
  xsec_x->SetNpx(1000);
  double pt = xsec_x->Integral(binmin,binmax) / xsec->Integral(binmin,binmax);
  double nev = xsec->Integral(binmin, binmax) * lumi;
  if (nev==0) {
    nev = 5000000;
    lumi = nev / xsec->Integral(binmin, binmax);
  }
  cout << Form("Processing bin %1.0f-%1.0f with average pT=%1.0f"
	       " and lumi=%1.1f pb-1 (nev=%1.3g)",
	       binmin, binmax, pt, lumi, nev) << endl;

  cb1->SetParameters(1., mu1, sig1*mu1, a1, n1); // Gen/Part
  cb1x->SetParameters(1., mu1, sig1*mu1, a1, n1); // Gen/Part
  //g->SetParameters(1., mu2, sig2*mu2); // Calo/Gen
  cb2->SetParameters(1., mu2, sig2*mu2, a2, n2); // Calo/Part
  cb2x->SetParameters(1., mu2, sig2*mu2, a2, n2); // Calo/Part
  cb3->SetParameters(1., mu3, sig3*mu3, a3, n3); // Calo/Phot
  cb3x->SetParameters(1., mu3, sig3*mu3, a3, n3); // Calo/Phot
  cb4->SetParameters(1., mu4, sig4*mu4, a4, n4); // Part/Part
  cb4x->SetParameters(1., mu4, sig4*mu4, a4, n4); // Part/Part

  double mu1x = cb1x->Integral(0.,2.) / cb1->Integral(0.,2.);
  double mu2x = cb2x->Integral(0.,2.) / cb2->Integral(0.,2.);
  double mu3x = cb3x->Integral(0.,2.) / cb3->Integral(0.,2.);
  double mu4x = cb4x->Integral(0.,2.) / cb4->Integral(0.,2.);

  TH1D *xsec0 = new TH1D("xsec0","PtHat",1000,0.7*genmin,1.3*genmax);
  TH1D *xsec1 = new TH1D("xsec1","PartPtPho",1000,0.7*genmin,1.3*genmax);
  TH1D *xsec2 = new TH1D("xsec2","PhotPt",1000,0.7*genmin,1.3*genmax);
  TH1D *xsec3 = new TH1D("xsec3","PhotPt",1000,0.7*genmin,1.3*genmax);
  TH1D *h1 = new TH1D("h1","GenPart",400,0.,2.);
  TH1D *h2 = new TH1D("h2","CaloGen",400,0.,2.);
  TH1D *h3 = new TH1D("h3","Phot",400,0.,2.);
  TH1D *h4 = new TH1D("h4","Part",400,0.,2.);
  TH1D *h12 = new TH1D("h12","CaloPart",400,0.,2.);
  TH1D *h123 = new TH1D("h123","CaloPhot",400,0.,2.);
  TH1D *h1234 = new TH1D("h1234","RmeasSig",400,0.,2.);

  const double pthat0 = 300.;
  const int nit = 5000000;
  for (int i = 0; i != nit; ++i) {

    double pthat = pthat0;
    //if (photbin) 
    pthat = xsec->GetRandom(genmin, genmax);

    // Is the parton balancing model ok?
    // Should we rather simulate a "second jet", which either
    // the photon or jet can loose? (No gains allowed vs pthat)

    // parton balance
    double part = cb4->GetRandom();

    // jet side
    //double part1 = cb4->GetRandom();
    //double partpt1 = part1 * pthat;
    double partpt1 = (1.+min(part-1.,0.)) * pthat;
    double genpart = cb1->GetRandom();
    double genpt = genpart * partpt1;
    //double calogen = g->GetRandom();
    double calogen = cb2->GetRandom();
    double calopt = calogen * genpt;
    double calpart = calopt / partpt1;

    // photon side
    //double part2 = cb4->GetRandom();
    //double partpt2 = part2 * pthat;
    double partpt2 = (1.+min(1.-part,0.)) * pthat;
    double phot = cb3->GetRandom();
    double photpt1 = phot * partpt1; // parton pT's perfectly balanced
    double calphot1 = calopt / photpt1; // parton pT's perfectly balanced
    double photpt2 = phot * partpt2; // account for parton imbalance
    double calphot2 = calopt / photpt2; // account for parton imbalance

    xsec0->Fill(pthat);
    xsec1->Fill(partpt2);
    xsec2->Fill(photpt2);
    //if (photbin && 
    if (photpt2 >= binmin && photpt2 < binmax) {
      //if (true) {
      xsec3->Fill(photpt2);

      h1->Fill(genpart);
      h2->Fill(calogen);
      h3->Fill(phot);
      h4->Fill(partpt1/partpt2);//part2);
      h12->Fill(calpart);
      h123->Fill(calphot1);
      h1234->Fill(calphot2);
    }
  }

  double m1 = h1->GetMean();
  double m2 = h2->GetMean();
  double m3 = h3->GetMean();
  double m4 = h4->GetMean();
  double m12 = h12->GetMean();
  double m123 = h123->GetMean();
  double m1234 = h1234->GetMean();
  double s12 = h12->GetRMS() / m12;
  double s123 = h123->GetRMS() / m123;
  double s1234 = h1234->GetRMS() / m1234;

  g->SetRange(mu1*(1.-sig1),mu1*(1+sig1));
  h1->Fit(g, "QR");
  double p1 = g->GetParameter(1);

  g->SetRange(mu2*(1.-sig2),mu2*(1+sig2));
  h2->Fit(g, "QR");
  double p2 = g->GetParameter(1);

  g->SetRange(mu3*(1.-sig3),mu3*(1+sig3));
  h3->Fit(g, "QR");
  double p3 = g->GetParameter(1);

  g->SetRange(mu4*(1.-sig4),mu4*(1+sig4));
  h4->Fit(g, "QR");
  double p4 = g->GetParameter(1);

  g->SetRange(m12*(1.-s12),m12*(1+s12));
  h12->Fit(g, "QRN");
  g->SetRange(g->GetParameter(1)-g->GetParameter(2),
	      g->GetParameter(1)+g->GetParameter(2));
  h12->Fit(g, "QR");
  double p12 = g->GetParameter(1);

  g->SetRange(m123*(1.-s123),m123*(1+s123));
  h123->Fit(g, "QRN");
  g->SetRange(g->GetParameter(1)-g->GetParameter(2),
	      g->GetParameter(1)+g->GetParameter(2));
  h123->Fit(g, "QR");
  double p123 = g->GetParameter(1);

  g->SetRange(m1234*(1.-s1234),m1234*(1+s1234));
  h1234->Fit(g, "QRN");
  g->SetRange(g->GetParameter(1)-g->GetParameter(2),
	      g->GetParameter(1)+g->GetParameter(2));
  h1234->Fit(g, "QR");
  double p1234 = g->GetParameter(1);

  cout << Form("for GenPart,\n"
	       "mean_in = %1.3g, peak_in = %1.3g, diff = %+1.3f%%\n"
	       "mean_mc = %1.3g, peak_mc = %1.3g, diff = %+1.3f%%\n"
	       "diff  = %+1.3f%%, diff  = %+1.3f%%,(diff = %+1.3f%%)\n",
	       mu1x,              mu1,              100.*(mu1x/mu1-1.),
	       m1,                p1,               100.*(m1/p1-1.),
	       100.*(m1/mu1x-1.), 100.*(p1/mu1-1.), 100.*(p1/mu1x-1.))
       << endl;

  cout << Form("for CaloGen,\n"
	       "mean_in = %1.3g, peak_in = %1.3g, diff = %+1.3f%%\n"
	       "mean_mc = %1.3g, peak_mc = %1.3g, diff = %+1.3f%%\n"
	       "diff  = %+1.3f%%, diff  = %+1.3f%%,(diff = %+1.3f%%)\n",
	       mu2x,              mu2,              100.*(mu2x/mu2-1.),
	       m2,                p2,               100.*(m2/p2-1.),
	       100.*(m2/mu2x-1.), 100.*(p2/mu2-1.), 100.*(p2/mu2x-1.))
       << endl;

  double mean_in = mu1x*mu2x;
  double peak_in = mu1*mu2;
  cout << Form("for CaloPart,\n"
	       "mean_in = %1.3g, peak_in = %01.03g, diff = %+1.3f%%\n"
	       "mean_mc = %1.3g, peak_mc = %01.03g, diff = %+1.3f%%\n"
	       "diff  = %+1.3f%%, diff  = %+1.3f%%,(diff = %+1.3f%%)\n",
	       mean_in,           peak_in,    100.*(mean_in/peak_in-1.),
	       m12,               p12,        100.*(m12/p12-1.),
	       100.*(m12/mean_in-1.), 100.*(p12/peak_in-1.),
	       100.*(p12/mean_in-1.)) << endl;

  cout << "-------------------------------------" << endl << endl;

  cout << Form("for Phot,\n"
	       "mean_in = %1.3g, peak_in = %1.3g, diff = %+1.3f%%\n"
	       "mean_mc = %1.3g, peak_mc = %1.3g, diff = %+1.3f%%\n"
	       "diff  = %+1.3f%%, diff  = %+1.3f%%,(diff = %+1.3f%%)\n",
	       mu3x,              mu3,              100.*(mu3x/mu3-1.),
	       m3,                p3,               100.*(m3/p3-1.),
	       100.*(m3/mu3x-1.),100.*(p3/mu3-1.), 100.*(p3/mu3x-1.))
       << endl;

  mean_in = mu1x*mu2x/mu3x;
  peak_in = mu1*mu2/mu3;
  cout << Form("for CaloPhot,\n"
	       "mean_in = %1.3g, peak_in = %01.03g, diff = %+1.3f%%\n"
	       "mean_mc = %1.3g, peak_mc = %01.03g, diff = %+1.3f%%\n"
	       "diff  = %+1.3f%%, diff  = %+1.3f%%,(diff = %+1.3f%%)\n",
	       mean_in,           peak_in,    100.*(mean_in/peak_in-1.),
	       m123,              p123,       100.*(m123/p123-1.),
	       100.*(m123/mean_in-1.), 100.*(p123/peak_in-1.),
	       100.*(p123/mean_in-1.)) << endl;

  cout << "-------------------------------------" << endl << endl;

  cout << Form("for Part,\n"
	       "mean_in = %1.3g, peak_in = %1.3g, diff = %+1.3f%%\n"
	       "mean_mc = %1.3g, peak_mc = %1.3g, diff = %+1.3f%%\n"
	       "diff  = %+1.3f%%, diff  = %+1.3f%%,(diff = %+1.3f%%)\n",
	       mu4x,              mu4,              100.*(mu4x/mu4-1.),
	       m4,                p4,               100.*(m4/p4-1.),
	       100.*(m4/mu4x-1.),100.*(p4/mu4-1.), 100.*(p4/mu4x-1.))
       << endl;

  mean_in = mu1x*mu2x/(mu3x*mu4x);
  peak_in = mu1*mu2/(mu3/mu4);
  cout << Form("for RmeasSig,\n"
	       "mean_in = %1.3g, peak_in = %01.03g, diff = %+1.3f%%\n"
	       "mean_mc = %1.3g, peak_mc = %01.03g, diff = %+1.3f%%\n"
	       "diff  = %+1.3f%%, diff  = %+1.3f%%,(diff = %+1.3f%%)\n",
	       mean_in,           peak_in,    100.*(mean_in/peak_in-1.),
	       m1234,             p1234,      100.*(m1234/p1234-1.),
	       100.*(m1234/mean_in-1.), 100.*(p1234/peak_in-1.),
	       100.*(p1234/mean_in-1.)) << endl;

  //cout << Form("m1 = %1.3g, m2 = %1.3g", m1, m2) << endl;
  //cout << Form("m12 = %1.3g, mu = %1.3g, rel.diff = %1.3g%%",
  //       m12, mu, 100.*(m12/mu - 1.)) << endl;

  TCanvas *c = (TCanvas*)gROOT->FindObject("c1");
  if (c) delete c;
  c = new TCanvas("c1","c1",900,900);
  c->Divide(3,3);
  c->cd(1);
  gPad->SetLogy();
  h2->Draw();
  c->cd(2);
  gPad->SetLogy();
  h1->Draw();
  c->cd(3);
  gPad->SetLogy();
  h12->Draw();
  c->cd(4);
  gPad->SetLogy();
  xsec0->SetLineColor(kBlack);
  xsec0->Draw();
  xsec1->SetLineColor(kBlue);
  xsec1->Draw("SAME");
  xsec2->SetLineColor(kRed);
  xsec2->Draw("SAME");
  c->cd(5);
  gPad->SetLogy();
  h3->Draw();
  c->cd(6);
  gPad->SetLogy();
  h123->Draw();
  c->cd(7);
  TH1D *xsec1r = (TH1D*)xsec1->Clone("xsec1r");
  xsec1r->Divide(xsec1,xsec0);
  xsec1r->Draw();
  xsec1r->SetMaximum(1.1);
  TH1D *xsec2r1 = (TH1D*)xsec2->Clone("xsec2r1");
  xsec2r1->Divide(xsec2,xsec1);
  xsec2r1->Draw("SAME");
  TH1D *xsec2r0 = (TH1D*)xsec2->Clone("xsec2r0");
  xsec2r0->SetLineColor(kBlack);
  xsec2r0->Divide(xsec2,xsec0);
  xsec2r0->Draw("SAME");
  TH1D *xsec3r0 = (TH1D*)xsec3->Clone("xsec3r0");
  xsec3r0->Divide(xsec3,xsec0);
  xsec3r0->SetFillStyle(3001);
  xsec3r0->SetFillColor(kRed);
  xsec3r0->Draw("SAME");
  c->cd(8);
  gPad->SetLogy();
  h4->Draw();
  c->cd(9);
  gPad->SetLogy();
  h1234->Draw();
  c->cd(0);
  
}
