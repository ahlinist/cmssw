// Adapted from D0 Experiment jetcorr/macros/RjetUncertainty.C
//
// Draw the uncertainty plots for L3 absolute response correction
// The uncertainties are stored in L3Corr.cpp/hpp

#include "TCanvas.h"
#include "TGraph.h"
#include "TH1.h"
#include "TLine.h"
#include "TPave.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TROOT.h"

//#include "src/PhotonRjetCorr.cpp"
//#include "L3Corr.cpp" 
#include "L3Corr.hpp" // => Load .cpp in mk_L3Uncertainty.C 

#include <string>
#include <fstream>

using namespace std;

const double maxpt = 900.;//3000.;

const int kBeige = 41;
const int kRightHatch = 3654;
const int kLeftHatch = 3645;
const int kVertHatch = 3699;
const int kHorixHatch = 3600;
const int kCrossHatch = 3644;

// Helper routine to do much the same as TLegend
void DrawLine(TGraph *g, double xmin, double ymin, const char *str) {

  TLine* l1 = new TLine();
  l1->SetLineStyle(g->GetLineStyle());
  l1->SetLineColor(g->GetLineColor());
  l1->SetLineWidth(g->GetLineWidth());
  l1->DrawLineNDC(xmin, ymin+0.01, xmin+0.07, ymin+0.01);
  TLatex* tStat = new TLatex(xmin+0.09, ymin, str);
  tStat->SetNDC();
  tStat->SetTextSize(0.045);
  tStat->Draw();
}
void DrawLine(TH1 *h, double xmin, double ymin, const char *str) {

  TLine* l1 = new TLine();
  l1->SetLineStyle(h->GetLineStyle());
  l1->SetLineColor(h->GetLineColor());
  l1->SetLineWidth(h->GetLineWidth());
  l1->DrawLineNDC(xmin, ymin+0.01, xmin+0.07, ymin+0.01);
  
  if (str!="") {
    TLatex* tStat = new TLatex(xmin+0.09, ymin, str);
    tStat->SetNDC();
    tStat->SetTextSize(0.045);
    tStat->Draw();
  }
}
void DrawFill(TH1 *h, double xmin, double ymin, const char *str) {

  /*
    TLine *l1 = new TLine();
    l1->SetLineStyle(1);
    l1->SetLineWidth(25);
    l1->SetLineColor(h->GetFillColor());
    l1->DrawLineNDC(xmin, ymin+0.01, xmin+0.07, ymin+0.01);
  */
  
  TPave *b1 = new TPave(xmin, ymin+0.01-0.025, xmin+0.07, ymin+0.01+0.025,
			1, "NDC");
  b1->SetFillStyle(h->GetFillStyle());
  b1->SetFillColor(h->GetFillColor());
  b1->SetLineStyle(1);
  b1->SetLineColor(h->GetLineColor());
  b1->Draw("L");
  
  if (str!="") {
    TLatex* tTot = new TLatex(xmin+0.09, ymin, str);
    tTot->SetNDC();
    tTot->SetTextSize(0.045);
    tTot->Draw();
  }
}

void L3Uncertainty() {

  // Improve some settings for this
  gStyle->SetPadLeftMargin(0.12);

  const bool plotLog = true;
  L3Corr::JetAlg jetAlg = L3Corr::IC5_DATA;
  //L3Corr::JetAlg jetAlg = L3Corr::IC5_MC;

  jec::ErrorTypes all = jec::kAll;//(jec::ErrorTypes)
    //(jec::kL3Stat | jec::kL3PhotonES | jec::kL3QCDBackground
    //| jec::kL3Parton | jec::kL3HardGluon);

  L3Corr rjet(jetAlg, all);
  L3Corr rjetStat(jetAlg, jec::kL3Stat);
  L3Corr rjetPhoton(jetAlg, jec::kL3PhotonES);
  L3Corr rjetQCD(jetAlg, jec::kL3QCDBackground);
  L3Corr rjetParton(jetAlg, jec::kL3Parton);
  L3Corr rjetGluon(jetAlg, jec::kL3HardGluon);
  L3Corr rjetFlavor(jetAlg, jec::kL3Flavor);

  //double erangemin = (plotLog) ? 15 : 0.01;
  double erangemin = (plotLog) ? 10 : 0.01;
  double erangemax = (plotLog) ? maxpt : maxpt;
  //double emin = (plotLog) ? 20 : 20;
  //double emin = (plotLog) ? 12 : 12;
  double emin = (plotLog) ? 15 : 15;
  double emax = (plotLog) ? maxpt*0.8 : maxpt*0.95;

  TGraph* statUp = 0;
  TGraph* statDown = 0;
  TGraph* phUp = 0;
  TGraph* phDown = 0;
  TGraph* qcdUp = 0;
  TGraph* qcdDown = 0;
  TGraph* partUp = 0;
  TGraph* partDown = 0;
  TGraph* gluUp = 0;
  TGraph* gluDown = 0;
  TGraph* flavorUp = 0;
  TGraph* flavorDown = 0;
  TH1D* hErr = 0;
  {
    const int ndiv = 100;
    double x[ndiv];
    double yup[ndiv];
    double ydown[ndiv];
    double phup[ndiv];
    double phdown[ndiv];
    double qcdup[ndiv];
    double qcddown[ndiv];
    double partup[ndiv];
    double partdown[ndiv];
    double gluup[ndiv];
    double gludown[ndiv];
    double flavorup[ndiv];
    double flavordown[ndiv];

    double xbins[ndiv+1];
    const double dx = (plotLog ?
		       pow(emax / emin, 1. / (ndiv - 1)) : 
		       (emax - emin) / (double) (ndiv - 1));
    xbins[0] = emin;
    for (int i = 1; i != ndiv+1; ++i) {

      if (plotLog) xbins[i] = xbins[i-1] * dx;
      else         xbins[i] = xbins[i-1] + dx;
    }

    //hErr = new TH1D("hErr", "hErr", ndiv, currx - dx/2, 
    //	    currx + dx * (ndiv - 0.5));
    hErr = new TH1D("hErr", "hErr", ndiv, xbins);

    for(int i = 0; i != ndiv; ++i) {

      x[i] = 0.5*(xbins[i]+xbins[i+1]);//currx;
      double err = 0;

      double r = rjetStat.Rjet(x[i],err);
      yup[i] = 100 * err / r;
      ydown[i] = -100 * err / r;

      r = rjetPhoton.Rjet(x[i], err);
      phup[i] = 100 * err / r;
      phdown[i] = -100 * err / r;


      r = rjetQCD.Rjet(x[i], err);
      qcdup[i] = 100 * err / r;
      qcddown[i] = -100 * err / r;

      r = rjetParton.Rjet(x[i], err);
      partup[i] = 100 * err / r;
      partdown[i] = -100 * err / r;

      r = rjetGluon.Rjet(x[i], err);
      gluup[i] = 100 * err / r;
      gludown[i] = -100 * err / r;

      r = rjetFlavor.Rjet(x[i], err);
      flavorup[i] = 100 * err / r;
      flavordown[i] = -100 * err / r;
    }

    for (int ibin = 1; ibin != hErr->GetNbinsX()+1; ++ibin) {

      double x = hErr->GetBinCenter(ibin);
      double err = 0;
      double r = rjet.Rjet(x, err);
      hErr->SetBinContent(ibin, 0);
      hErr->SetBinError(ibin, 100 * err / r);
    }

    statUp = new TGraph(ndiv, x, yup);
    statDown = new TGraph(ndiv, x, ydown);
    phUp = new TGraph(ndiv, x, phup);
    phDown = new TGraph(ndiv, x, phdown);
    qcdUp = new TGraph(ndiv, x, qcdup);
    qcdDown = new TGraph(ndiv, x, qcddown);
    partUp = new TGraph(ndiv, x, partup);
    partDown = new TGraph(ndiv, x, partdown);
    gluUp = new TGraph(ndiv, x, gluup);
    gluDown = new TGraph(ndiv, x, gludown);
    flavorUp = new TGraph(ndiv, x, flavorup);
    flavorDown = new TGraph(ndiv, x, flavordown);
  }
  hErr->SetFillStyle(1001);
  hErr->SetFillColor(kGray);//5);
  hErr->SetLineColor(hErr->GetFillColor());
  hErr->SetMarkerColor(hErr->GetFillColor());
  hErr->SetMarkerSize(0);

  statUp->SetLineStyle(kDashed);
  statUp->SetLineWidth(2);
  statUp->SetLineColor(kRed);
  statDown->SetLineStyle(kDashed);
  statDown->SetLineWidth(2);
  statDown->SetLineColor(kRed);

  phUp->SetLineStyle(kSolid);
  phUp->SetLineWidth(2);
  phDown->SetLineStyle(kSolid);
  phDown->SetLineWidth(2);

  qcdUp->SetLineStyle(kDotted);
  qcdUp->SetLineWidth(2);
  qcdUp->SetLineColor(kGreen+2);
  qcdDown->SetLineStyle(kDotted);
  qcdDown->SetLineWidth(2);
  qcdDown->SetLineColor(kGreen+2);

  partUp->SetLineStyle(kDashDotted);
  partUp->SetLineWidth(2);
  partDown->SetLineStyle(kDashDotted);
  partDown->SetLineWidth(2);

  gluUp->SetLineStyle(kDashed);
  gluUp->SetLineWidth(3);
  gluUp->SetLineColor(kBlue);
  gluDown->SetLineStyle(kDashed);
  gluDown->SetLineWidth(3);
  gluDown->SetLineColor(kBlue);

  flavorUp->SetLineStyle(kSolid);
  flavorUp->SetLineWidth(3);
  flavorUp->SetLineColor(kBlue);
  flavorDown->SetLineStyle(kSolid);
  flavorDown->SetLineWidth(3);
  flavorDown->SetLineColor(kBlue);

  {
    string cName = "L3Uncert_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C->SetLogx();

    TH1F* HE2 = new TH1F((cName + "_E2").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE2->SetMinimum(-29.99);//-19.99);//-15.99);//-3.99);
    HE2->SetMaximum(29.99);//19.99);//15.99);//3.99);
    HE2->SetStats(kFALSE);
    HE2->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE2->GetXaxis()->SetTitleOffset(1.1);//0.8);
    HE2->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE2->GetXaxis()->SetNoExponent(kTRUE);
    HE2->GetYaxis()->SetTitle("rel. error [%]");
    HE2->GetYaxis()->SetTitleOffset(0.9);//0.7);
    HE2->Draw("AXIS");

    hErr->Draw("SAME E3");
    HE2->Draw("SAME AXIS");

    statUp->Draw("L");
    statDown->Draw("L");
    phUp->Draw("L");
    phDown->Draw("L");
    qcdUp->Draw("L");
    qcdDown->Draw("L");
    partUp->Draw("L");
    partDown->Draw("L");
    gluUp->Draw("L");
    gluDown->Draw("L");
    flavorUp->Draw("L");
    flavorDown->Draw("L");

    DrawLine(statUp, 0.18, 0.31, "stat. 10 pb^{-1}");
    DrawLine(phUp, 0.18, 0.25, "photon scale");
    //DrawLine(gluUp, 0.18, 0.19, "second jet");
    DrawLine(partUp, 0.18, 0.19, "parton corr");
    DrawLine(flavorUp, 0.54, 0.37, "flavor mapping");
    //DrawLine(partUp, 0.54, 0.31, "parton corr.");
    DrawLine(gluUp, 0.54, 0.31, "second jet");
    DrawLine(qcdUp, 0.54, 0.25, "QCD background");
    DrawFill(hErr, 0.54, 0.19, "total err.");

    C->Update();

    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

  {
    string cName = "L3UncertUp_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C->SetLogx();

    TH1F* HE2 = new TH1F((cName + "_E2").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE2->SetMinimum(0.);//-4.99);
    HE2->SetMaximum(39.99);//19.99);
    HE2->SetStats(kFALSE);
    HE2->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE2->GetXaxis()->SetTitleOffset(0.9);
    HE2->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE2->GetXaxis()->SetNoExponent(kTRUE);
    HE2->GetYaxis()->SetTitle("relative uncertainty [%]");
    HE2->GetYaxis()->SetTitleOffset(0.9);
    HE2->Draw("AXIS");

    TH1D *hErrUp = (TH1D*)hErr->Clone("hErrUp");
    for (int i = 1; i != hErrUp->GetNbinsX()+1; ++i) {
      hErrUp->SetBinContent(i, 0.5*hErr->GetBinError(i));
      hErrUp->SetBinError(i, 0.5*hErr->GetBinError(i));
    }

    hErrUp->Draw("SAME E3");
    HE2->Draw("SAME AXIS");

    statUp->Draw("L");
    phUp->Draw("L");
    qcdUp->Draw("L");
    partUp->Draw("L");
    gluUp->Draw("L");
    flavorUp->Draw("L");

    /*
    DrawLine(statUp, 0.18, 0.87, "stat. 10 pb^{-1}");
    DrawLine(phUp, 0.18, 0.81, "photon scale");
    DrawLine(partUp, 0.18, 0.75, "parton corr.");
    DrawLine(flavorUp, 0.54, 0.87, "flavor mapping");
    DrawLine(gluUp, 0.54, 0.81, "second jet");
    DrawLine(qcdUp, 0.54, 0.75, "QCD background");
    DrawFill(hErrUp, 0.54, 0.69, "total err.");
    */
    DrawFill(hErrUp, 0.45, 0.81, "total uncertainty");
    DrawLine(flavorUp, 0.45, 0.75, "flavor mapping");
    DrawLine(gluUp, 0.45, 0.69, "second jet cut");
    DrawLine(partUp, 0.45, 0.63, "parton correction");
    DrawLine(qcdUp, 0.45, 0.57, "QCD background");
    DrawLine(statUp, 0.45, 0.51, "statistics / 10 pb^{-1}");
    DrawLine(phUp, 0.45, 0.45, "photon scale");

    TLatex *tcms = new TLatex(0.45, 0.87, "CMS preliminary");
    tcms->SetNDC();
    tcms->SetTextSize(0.05);
    tcms->Draw();

    C->Update();

    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());

    HE2->GetXaxis()->SetRangeUser(25.,752.);
    C->Update();
    C->SaveAs(("eps/jec_plots/" + cName + "_trunc.eps").c_str());
  }

  // Produce ASCII file of the uncertainties for CMSSW implementation
  ofstream fout("L3Uncertainty.txt");
  double mineta = -5;
  double maxeta = 5;
  int npar = 3*hErr->GetNbinsX();
  fout << Form("%1.1f %1.1f %d", mineta, maxeta, npar);
  for (int i = 1; i != hErr->GetNbinsX()+1; ++i)
    fout << Form(" %1.1f %1.3g %1.3g ", hErr->GetBinCenter(i),
		 0.01*hErr->GetBinError(i), 0.01*hErr->GetBinError(i));
  fout << endl;

}


void PurityUncertainty() {

  // Improve some settings for this
  gStyle->SetPadLeftMargin(0.12);

  const bool plotLog = true;
  L3Corr::JetAlg jetAlg = L3Corr::IC5_DATA;
  //L3Corr::JetAlg jetAlg = L3Corr::IC5_MC;
  L3Corr::PhotonID phoID = L3Corr::kMedium;

  jec::ErrorTypes all = jec::kAll;//(jec::ErrorTypes)
    //(jec::kL3Stat | jec::kL3PhotonES | jec::kL3QCDBackground
    //| jec::kL3Parton | jec::kL3HardGluon);

  L3Corr rjet(jetAlg, all);

  double erangemin = (plotLog) ? 15 : 0.01;
  double erangemax = (plotLog) ? maxpt : maxpt;
  double emin = (plotLog) ? 20 : 20;
  double emax = (plotLog) ? maxpt*0.8 : maxpt*0.95;//2500. : 2900.;

  TGraph* statUp = 0;
  TGraph* statDown = 0;
  TGraph* idUp = 0;
  TGraph* idDown = 0;
  TGraph* xsecUp = 0;
  TGraph* xsecDown = 0;
  //TGraph* jetUp = 0;
  //TGraph* jetDown = 0;
  TH1D* hErr = 0, *hErr1, *hErr2, *hErr3;
  {
    const int ndiv = 100;
    double x[ndiv];
    double statup[ndiv];
    double statdown[ndiv];
    double idup[ndiv];
    double iddown[ndiv];
    double xsecup[ndiv];
    double xsecdown[ndiv];
    //double jetup[ndiv];
    //double jetdown[ndiv];

    double xbins[ndiv+1];
    const double dx = (plotLog ?
		       pow(emax / emin, 1. / (ndiv - 1)) : 
		       (emax - emin) / (double) (ndiv - 1));
    xbins[0] = emin;
    for (int i = 1; i != ndiv+1; ++i) {

      if (plotLog) xbins[i] = xbins[i-1] * dx;
      else         xbins[i] = xbins[i-1] + dx;
    }

    hErr = new TH1D("hErr", "hErr", ndiv, xbins);
    hErr1 = new TH1D("hErr1", "hErr1", ndiv, xbins);
    hErr2 = new TH1D("hErr2", "hErr2", ndiv, xbins);
    hErr3 = new TH1D("hErr3", "hErr3", ndiv, xbins);

    for(int i = 0; i != ndiv; ++i) {

      x[i] = 0.5 * (xbins[i] + xbins[i+1]);
      double err = 0;
      double P = rjet._purity(x[i], phoID);

      err = rjet._StatPurity(x[i], phoID);
      statup[i] = 100 * err / P;
      statdown[i] = -100 * err / P;

      err = rjet._SystPurityID(x[i], phoID);
      idup[i] = 100 * err / P;
      iddown[i] = -100 * err / P;

      err = rjet._SystPurityXsec(x[i], phoID);
      xsecup[i] = 100 * err / P;
      xsecdown[i] = -100 * err / P;

      //err = rjet._SystPurity2ndJet(x[i], phoID);
      //jetup[i] = 100 * err / P;
      //jetdown[i] = -100 * err / P;
    }

    for (int ibin = 1; ibin != hErr->GetNbinsX()+1; ++ibin) {

      double x = hErr->GetBinCenter(ibin);
      double P = rjet._purity(x, phoID);
      double err = rjet._SystPurity(x, phoID);
      hErr->SetBinContent(ibin, 0);
      hErr->SetBinError(ibin, 100 * err / P);

      P = rjet._purity(x, L3Corr::kLoose);
      err = rjet._SystPurity(x, L3Corr::kLoose);
      hErr1->SetBinContent(ibin, P);
      hErr1->SetBinError(ibin, err);
      P = rjet._purity(x, L3Corr::kMedium);
      err = rjet._SystPurity(x, L3Corr::kMedium);
      hErr2->SetBinContent(ibin, P);
      hErr2->SetBinError(ibin, err);
      P = rjet._purity(x, L3Corr::kTight);
      err = rjet._SystPurity(x, L3Corr::kTight);
      hErr3->SetBinContent(ibin, P);
      hErr3->SetBinError(ibin, err);
    }

    statUp = new TGraph(ndiv, x, statup);
    statDown = new TGraph(ndiv, x, statdown);
    idUp = new TGraph(ndiv, x, idup);
    idDown = new TGraph(ndiv, x, iddown);
    xsecUp = new TGraph(ndiv, x, xsecup);
    xsecDown = new TGraph(ndiv, x, xsecdown);
    //jetUp = new TGraph(ndiv, x, jetup);
    //jetDown = new TGraph(ndiv, x, jetdown);
  }

  hErr->SetFillStyle(1001);
  hErr->SetFillColor(5);
  hErr->SetLineColor(hErr->GetFillColor());
  hErr->SetMarkerColor(hErr->GetFillColor());
  hErr->SetMarkerSize(0);

  hErr1->SetLineStyle(kDashed);
  hErr1->SetLineWidth(2);
  hErr2->SetLineStyle(kSolid);
  hErr2->SetLineWidth(2);
  hErr2->SetFillStyle(1001);
  hErr2->SetFillColor(kYellow);
  hErr2->SetLineColor(kBlack);//hErr2->GetFillColor());
  hErr2->SetMarkerColor(hErr2->GetFillColor());
  hErr2->SetMarkerSize(0);
  hErr3->SetLineStyle(kDotted);
  hErr3->SetLineWidth(2);

  statUp->SetLineStyle(kDashed);
  statUp->SetLineWidth(1);
  statDown->SetLineStyle(kDashed);
  statDown->SetLineWidth(1);
  idUp->SetLineStyle(kSolid);
  idUp->SetLineWidth(1);
  idDown->SetLineStyle(kSolid);
  idDown->SetLineWidth(1);
  xsecUp->SetLineStyle(kDotted);
  xsecUp->SetLineWidth(1);
  xsecDown->SetLineStyle(kDotted);
  xsecDown->SetLineWidth(1);
  //jetUp->SetLineStyle(kDashDotted);
  //jetUp->SetLineWidth(1);
  //jetDown->SetLineStyle(kDashDotted);
  //jetDown->SetLineWidth(1);

  {
    string cName = "L3PurityUncert_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C->SetLogx();

    TH1F* HE2 = new TH1F((cName + "_E2").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE2->SetMinimum(-35.99);
    HE2->SetMaximum(35.99);
    HE2->SetStats(kFALSE);
    HE2->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE2->GetXaxis()->SetTitleOffset(1.1);
    HE2->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE2->GetXaxis()->SetNoExponent(kTRUE);
    HE2->GetYaxis()->SetTitle("rel. error on purity [%]");
    HE2->GetYaxis()->SetTitleOffset(0.9);
    HE2->Draw("AXIS");

    hErr->Draw("SAME E3");
    HE2->Draw("SAME AXIS");

    statUp->Draw("L");
    statDown->Draw("L");
    idUp->Draw("L");
    idDown->Draw("L");
    xsecUp->Draw("L");
    xsecDown->Draw("L");
    //jetUp->Draw("L");
    //jetDown->Draw("L");

    DrawLine(statUp, 0.18, 0.25, "stat.");
    DrawLine(idUp, 0.18, 0.19, "photon ID");
    //DrawLine(jetUp, 0.54, 0.31, "second jet");
    DrawLine(xsecUp, 0.54, 0.25, "cross sections");
    DrawFill(hErr, 0.54, 0.19, "total err.");

    C->Update();

    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

  // Draw plots of the different purity factors
  {
    string cName = "L3Purity_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C2 = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C2->SetLogx();

    // Improve some settings for this
    C2->SetLeftMargin(0.15);
    
    TH1F* HE3 = new TH1F((cName + "_E3").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE3->SetMinimum(0.00);
    HE3->SetMaximum(1.00);
    HE3->SetStats(kFALSE);
    HE3->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE3->GetXaxis()->SetTitleOffset(1.1);
    HE3->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE3->GetXaxis()->SetNoExponent(kTRUE);
    HE3->GetYaxis()->SetTitle("Purity");
    HE3->GetYaxis()->SetTitleOffset(1.1);//0.9);

    TH1D *hErr2b = (TH1D*)hErr2->Clone("HErr2b");
    hErr2b->SetFillStyle(kNone);
    hErr2b->SetFillColor(0);
    
    HE3->Draw("AXIS");
    hErr2->Draw("SAME E4");
    hErr1->Draw("SAME HIST L");
    hErr2b->Draw("SAME HIST L");
    hErr3->Draw("SAME HIST L");
    
    DrawLine(hErr1, 0.54, 0.31, "loose");
    DrawFill(hErr2, 0.54, 0.25, "medium");
    DrawLine(hErr2, 0.54, 0.25, "");
    DrawLine(hErr3, 0.54, 0.19, "tight");

    C2->Update();
    
    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C2->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

}


void DeltaCUncertainty() {

  // Improve some settings for this
  gStyle->SetPadLeftMargin(0.12);

  const bool plotLog = true;
  L3Corr::JetAlg jetAlg = L3Corr::IC5_DATA;
  //L3Corr::JetAlg jetAlg = L3Corr::IC5_MC;
  L3Corr::PhotonID phoID = L3Corr::kMedium;

  jec::ErrorTypes all = jec::kAll;//(jec::ErrorTypes)
    //(jec::kL3Stat | jec::kL3PhotonES | jec::kL3QCDBackground
    //| jec::kL3Parton | jec::kL3HardGluon);

  L3Corr rjet(jetAlg, all);

  double erangemin = (plotLog) ? 15 : 0.01;
  double erangemax = (plotLog) ? maxpt : maxpt;
  double emin = (plotLog) ? 20 : 20;
  double emax = (plotLog) ? maxpt*0.8 : maxpt*0.95;//2500. : 2900.;

  const int ne = 6;//5;
  TGraph* eUp[ne];
  TGraph* eDown[ne];
  TH1D *hErr = 0, *hErr1, *hErr2, *hErr3;
  {
    const int ndiv = 100;
    double x[ndiv];
    double eup[ne][ndiv];
    double edown[ne][ndiv];

    double xbins[ndiv+1];
    const double dx = (plotLog ?
		       pow(emax / emin, 1. / (ndiv - 1)) : 
		       (emax - emin) / (double) (ndiv - 1));
    xbins[0] = emin;
    for (int i = 1; i != ndiv+1; ++i) {

      if (plotLog) xbins[i] = xbins[i-1] * dx;
      else         xbins[i] = xbins[i-1] + dx;
    }

    hErr = new TH1D("hErr", "hErr", ndiv, xbins);
    hErr1 = new TH1D("hErr1", "hErr1", ndiv, xbins);
    hErr2 = new TH1D("hErr2", "hErr2", ndiv, xbins);
    hErr3 = new TH1D("hErr3", "hErr3", ndiv, xbins);

    for(int i = 0; i != ndiv; ++i) {

      x[i] = 0.5 * (xbins[i] + xbins[i+1]);
      double err = 0;
      double syserr[ne];
      rjet._deltaC(x[i], err, phoID, &syserr);

      for (int j = 0; j != ne; ++j) {
	eup[j][i] = 100 * syserr[j];
	edown[j][i] = -100 * syserr[j];
      }
    } // for i

    for (int ibin = 1; ibin != hErr->GetNbinsX()+1; ++ibin) {

      double x = hErr->GetBinCenter(ibin);
      double err = 0;
      rjet._deltaC(x, err, phoID);
      hErr->SetBinContent(ibin, 0);
      hErr->SetBinError(ibin, 100 * err);

      double dC1 = rjet._deltaC(x, err, L3Corr::kLoose);
      hErr1->SetBinContent(ibin, dC1);
      hErr1->SetBinError(ibin, (1+dC1)*err);
      double dC2 = rjet._deltaC(x, err, L3Corr::kMedium);
      hErr2->SetBinContent(ibin, dC2);
      hErr2->SetBinError(ibin, (1+dC2)*err);
      double dC3 = rjet._deltaC(x, err, L3Corr::kTight);
      hErr3->SetBinContent(ibin, dC3);
      hErr3->SetBinError(ibin, (1+dC3)*err);
    } // for ibin

    for (int j = 0; j != ne; ++j) {
      eUp[j] = new TGraph(ndiv, x, eup[j]);
      eDown[j] = new TGraph(ndiv, x, edown[j]);
    }
  }

  hErr->SetFillStyle(1001);
  hErr->SetFillColor(kYellow);
  hErr->SetLineColor(hErr->GetFillColor());
  hErr->SetMarkerColor(hErr->GetFillColor());
  hErr->SetMarkerSize(0);

  hErr1->SetLineStyle(kDashed);
  hErr1->SetLineWidth(2);
  hErr2->SetLineStyle(kSolid);
  hErr2->SetLineWidth(2);
  hErr2->SetFillStyle(1001);
  hErr2->SetFillColor(kYellow);
  hErr2->SetLineColor(kBlack);//hErr2->GetFillColor());
  hErr2->SetMarkerColor(hErr2->GetFillColor());
  hErr2->SetMarkerSize(0);
  hErr3->SetLineStyle(kDotted);
  hErr3->SetLineWidth(2);

  int style[ne] = {kDashed, kDotted, kSolid, kDashed, kSolid, kSolid};
  int color[ne] = {kBlack, kBlack, kBlack, kBlue, kBlue, kBlue};
  int width[ne] = {1, 1, 1, 2, 2, 2};
  for (int j = 0; j != ne; ++j) {
    eUp[j]->SetLineStyle(style[j]);
    eUp[j]->SetLineColor(color[j]);
    eUp[j]->SetLineWidth(width[j]);
    eDown[j]->SetLineStyle(style[j]);
    eDown[j]->SetLineColor(color[j]);
    eDown[j]->SetLineWidth(width[j]);
  }

  {
    string cName = "L3DeltaCUncert_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C->SetLogx();

    TH1F* HE2 = new TH1F((cName + "_E2").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE2->SetMinimum(-35.99);
    HE2->SetMaximum(35.99);
    HE2->SetStats(kFALSE);
    HE2->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE2->GetXaxis()->SetTitleOffset(1.1);
    HE2->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE2->GetXaxis()->SetNoExponent(kTRUE);
    HE2->GetYaxis()->SetTitle("rel. error on #DeltaC [%]");
    HE2->GetYaxis()->SetTitleOffset(0.9);

    HE2->Draw("AXIS");
    hErr->Draw("SAME E3");
    //HE2->Draw("SAME AXIS");

    // Skip #4, jet showering
    for (int j = 0; j != ne; ++j) {
      if (j!=4) {
	eUp[j]->Draw("L");
	eDown[j]->Draw("L");
      }
    }

    // Check mappings from L3Corr.cpp::_deltaC
    DrawLine(eUp[0], 0.18, 0.31, "stat.");
    DrawLine(eUp[1], 0.18, 0.25, "EM response");
    DrawLine(eUp[3], 0.18, 0.19, "jet response");
    //DrawLine(eUp[2], 0.54, 0.31, "EM rel.showering");
    //DrawLine(eUp[4], 0.54, 0.25, "jet showering");
    DrawLine(eUp[2], 0.54, 0.31, "EM rel.showering");
    DrawLine(eUp[5], 0.54, 0.25, "EM sh. tails"); // mean vs peak
    DrawFill(hErr, 0.54, 0.19, "total err.");

    /*
    TLine *l1 = new TLine();
    l1->SetLineStyle(1);
    l1->SetLineWidth(25);
    l1->SetLineColor(hErr->GetFillColor());
    l1->DrawLineNDC(0.54, 0.20, 0.61, 0.20);
    TLatex* tTot = new TLatex(0.63, 0.19, "total err.");
    tTot->SetNDC();
    tTot->SetTextSize(0.045);
    tTot->Draw();
    */

    C->Update();

    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

  // Draw plots of the different DeltaC factors
  {
    string cName = "L3DeltaC_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C2 = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C2->SetLogx();

    // Improve some settings for this
    C2->SetLeftMargin(0.15);
    
    TH1F* HE3 = new TH1F((cName + "_E3").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE3->SetMinimum(-0.3499);//-0.30);
    HE3->SetMaximum(0.1999);//0.30);
    HE3->SetStats(kFALSE);
    HE3->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE3->GetXaxis()->SetTitleOffset(1.1);
    HE3->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE3->GetXaxis()->SetNoExponent(kTRUE);
    HE3->GetYaxis()->SetTitle("Background #DeltaC");
    HE3->GetYaxis()->SetTitleOffset(1.1);//0.9);
    
    HE3->Draw("AXIS");
    hErr2->Draw("SAME E4");
    hErr2->Draw("SAME HIST L");
    hErr1->Draw("SAME HIST L");
    hErr3->Draw("SAME HIST L");
    
    DrawLine(hErr1, 0.54, 0.31, "loose");
    DrawFill(hErr2, 0.54, 0.25, "medium");
    DrawLine(hErr2, 0.54, 0.25, "");
    DrawLine(hErr3, 0.54, 0.19, "tight");

    C2->Update();
    
    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C2->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }
  
}


void BackgroundUncertainty() {

  // Improve some settings for this
  gStyle->SetPadLeftMargin(0.12);

  const bool plotLog = true;
  L3Corr::JetAlg jetAlg = L3Corr::IC5_DATA;
  //L3Corr::JetAlg jetAlg = L3Corr::IC5_MC;

  jec::ErrorTypes all = jec::kAll;

  L3Corr rjet(jetAlg, all);
  L3Corr rjetLoose(jetAlg, jec::kL3QCDBackground, L3Corr::kLoose);
  L3Corr rjetMedium(jetAlg, jec::kL3QCDBackground, L3Corr::kMedium);
  L3Corr rjetTight(jetAlg, jec::kL3QCDBackground, L3Corr::kTight);

  double erangemin = (plotLog) ? 15 : 0.01;
  double erangemax = (plotLog) ? maxpt : maxpt;
  double emin = (plotLog) ? 20 : 20;
  double emax = (plotLog) ? maxpt*0.8 : maxpt*0.95;

  TGraph* looseUp = 0;
  TGraph* looseDown = 0;
  TGraph* mediumUp = 0;
  TGraph* mediumDown = 0;
  TGraph* tightUp = 0;
  TGraph* tightDown = 0;
  TH1D* hErr = 0, *hErr1, *hErr2, *hErr3;
  {
    const int ndiv = 100;
    double x[ndiv];
    double looseup[ndiv];
    double loosedown[ndiv];
    double mediumup[ndiv];
    double mediumdown[ndiv];
    double tightup[ndiv];
    double tightdown[ndiv];

    double xbins[ndiv+1];
    const double dx = (plotLog ?
		       pow(emax / emin, 1. / (ndiv - 1)) : 
		       (emax - emin) / (double) (ndiv - 1));
    xbins[0] = emin;
    for (int i = 1; i != ndiv+1; ++i) {

      if (plotLog) xbins[i] = xbins[i-1] * dx;
      else         xbins[i] = xbins[i-1] + dx;
    }

    hErr = new TH1D("hErr", "hErr", ndiv, xbins);
    hErr1 = new TH1D("hErr1", "hErr1", ndiv, xbins);
    hErr2 = new TH1D("hErr2", "hErr2", ndiv, xbins);
    hErr3 = new TH1D("hErr3", "hErr3", ndiv, xbins);

    for(int i = 0; i != ndiv; ++i) {

      x[i] = 0.5 * (xbins[i] + xbins[i+1]);
      double err = 0.;

      double r = rjetLoose.Rjet(x[i], err);
      looseup[i] = 100 * err / r;
      loosedown[i] = -100 * err / r;

      r = rjetMedium.Rjet(x[i], err);
      mediumup[i] = 100 * err / r;
      mediumdown[i] = -100 * err / r;

      r = rjetTight.Rjet(x[i], err);
      tightup[i] = 100 * err / r;
      tightdown[i] = -100 * err / r;

    }

    for (int ibin = 1; ibin != hErr->GetNbinsX()+1; ++ibin) {

      double x = hErr->GetBinCenter(ibin);
      double err = 0.;
      double r = rjet.Rjet(x, err);
      hErr->SetBinContent(ibin, 0);
      hErr->SetBinError(ibin, 100. * err / r);

      r = rjetLoose.Rjet(x, err);
      double dr = rjetLoose._deltaRjet(x, L3Corr::kLoose);
      hErr1->SetBinContent(ibin, dr);
      hErr1->SetBinError(ibin, err / r * (1 + dr));

      r = rjetMedium.Rjet(x, err);
      dr = rjetMedium._deltaRjet(x, L3Corr::kMedium);
      hErr2->SetBinContent(ibin, dr);
      hErr2->SetBinError(ibin, err / r * (1 + dr));

      r = rjetTight.Rjet(x, err);
      dr = rjetTight._deltaRjet(x, L3Corr::kTight);
      hErr3->SetBinContent(ibin, dr);
      hErr3->SetBinError(ibin, err / r * (1 + dr));
    }

    looseUp = new TGraph(ndiv, x, looseup);
    looseDown = new TGraph(ndiv, x, loosedown);
    mediumUp = new TGraph(ndiv, x, mediumup);
    mediumDown = new TGraph(ndiv, x, mediumdown);
    tightUp = new TGraph(ndiv, x, tightup);
    tightDown = new TGraph(ndiv, x, tightdown);
  }

  hErr->SetFillStyle(1001);
  hErr->SetFillColor(5);
  hErr->SetLineColor(hErr->GetFillColor());
  hErr->SetMarkerColor(hErr->GetFillColor());
  hErr->SetMarkerSize(0);

  hErr1->SetLineStyle(kDashed);
  hErr1->SetLineWidth(2);
  hErr1->SetFillStyle(kLeftHatch);
  hErr1->SetFillColor(kBlue);
  hErr1->SetLineColor(kBlue);
  hErr1->SetMarkerColor(hErr1->GetFillColor());
  hErr1->SetMarkerSize(0);

  hErr2->SetLineStyle(kSolid);
  hErr2->SetLineWidth(2);
  hErr2->SetFillStyle(1001);
  hErr2->SetFillColor(kYellow);
  hErr2->SetLineColor(kBlack);
  hErr2->SetMarkerColor(hErr2->GetFillColor());
  hErr2->SetMarkerSize(0);

  hErr3->SetLineStyle(kDashDotted);
  hErr3->SetLineWidth(2);
  hErr3->SetFillStyle(kRightHatch);
  hErr3->SetFillColor(kRed);
  hErr3->SetLineColor(kRed);
  hErr3->SetMarkerColor(hErr3->GetFillColor());
  hErr3->SetMarkerSize(0);

  looseUp->SetLineStyle(kDashed);
  looseUp->SetLineWidth(2);
  looseDown->SetLineStyle(kDashed);
  looseDown->SetLineWidth(2);
  mediumUp->SetLineStyle(kSolid);
  mediumUp->SetLineWidth(2);
  mediumDown->SetLineStyle(kSolid);
  mediumDown->SetLineWidth(2);
  tightUp->SetLineStyle(kDotted);
  tightUp->SetLineWidth(2);
  tightDown->SetLineStyle(kDotted);
  tightDown->SetLineWidth(2);

  {
    string cName = "L3BackgroundUncert_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C->SetLogx();

    TH1F* HE2 = new TH1F((cName + "_E2").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE2->SetMinimum(-15.99);
    HE2->SetMaximum(15.99);
    HE2->SetStats(kFALSE);
    HE2->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE2->GetXaxis()->SetTitleOffset(1.1);
    HE2->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE2->GetXaxis()->SetNoExponent(kTRUE);
    HE2->GetYaxis()->SetTitle("rel. error #DeltaR_{jet} [%]");
    HE2->GetYaxis()->SetTitleOffset(0.9);
    HE2->Draw("AXIS");

    hErr->Draw("SAME E3");
    HE2->Draw("SAME AXIS");

    looseUp->Draw("L");
    looseDown->Draw("L");
    mediumUp->Draw("L");
    mediumDown->Draw("L");
    tightUp->Draw("L");
    tightDown->Draw("L");

    DrawLine(looseUp, 0.54, 0.37, "loose ID");
    DrawLine(mediumUp, 0.54, 0.31, "medium ID");
    DrawLine(tightUp, 0.54, 0.25, "tight ID");
    DrawFill(hErr, 0.54, 0.19, "total L3 err.");

    C->Update();

    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

  // Draw plots of the different background corrections
  {
    string cName = "L3Background_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C2 = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C2->SetLogx();

    // Improve some settings for this
    C2->SetLeftMargin(0.15);
    
    TH1F* HE3 = new TH1F((cName + "_E3").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE3->SetMinimum(-0.1999);//-0.15999);
    HE3->SetMaximum(0.09999);//0.15999);
    HE3->SetStats(kFALSE);
    HE3->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE3->GetXaxis()->SetTitleOffset(1.1);
    HE3->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE3->GetXaxis()->SetNoExponent(kTRUE);
    HE3->GetYaxis()->SetTitle("Background #DeltaR_{jet}");
    HE3->GetYaxis()->SetTitleOffset(1.2);

    TH1D *hErr1b = (TH1D*)hErr1->Clone("HErr1b");
    hErr1b->SetFillStyle(kNone);
    hErr1b->SetFillColor(0);

    TH1D *hErr2b = (TH1D*)hErr2->Clone("HErr2b");
    hErr2b->SetFillStyle(kNone);
    hErr2b->SetFillColor(0);

    TH1D *hErr3b = (TH1D*)hErr3->Clone("HErr3b");
    hErr3b->SetFillStyle(kNone);
    hErr3b->SetFillColor(0);
    
    HE3->Draw("AXIS");
    hErr2->Draw("SAME E4");
    hErr2b->Draw("SAME HIST L");
    hErr1->Draw("SAME E4");
    hErr1b->Draw("SAME HIST L");
    hErr3->Draw("SAME E4");
    hErr3b->Draw("SAME HIST L");
    
    DrawFill(hErr1, 0.54, 0.31, "loose ID");
    DrawLine(hErr1b, 0.54, 0.31, "");
    DrawFill(hErr2, 0.54, 0.25, "medium ID");
    DrawLine(hErr2b, 0.54, 0.25, "");
    DrawFill(hErr3, 0.54, 0.19, "tight ID");
    DrawLine(hErr3b, 0.54, 0.19, "");

    C2->Update();
    
    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C2->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

}


void PartonUncertainty() {

  // Improve some settings for this
  gStyle->SetPadLeftMargin(0.12);

  const bool plotLog = true;
  L3Corr::JetAlg jetAlg = L3Corr::IC5_DATA;
  //L3Corr::JetAlg jetAlg = L3Corr::IC5_MC;

  L3Corr kjetParton(jetAlg, jec::kL3Parton);
  L3Corr kjetFrag(jetAlg, jec::kL3PartonFrag);
  L3Corr kjetUE(jetAlg, jec::kL3PartonUE);

  double erangemin = (plotLog) ? 15 : 0.01;
  double erangemax = (plotLog) ? maxpt : maxpt;
  double emin = (plotLog) ? 20 : 20;
  double emax = (plotLog) ? maxpt*0.8 : maxpt*0.95;

  TGraph* fragUp = 0;
  TGraph* fragDown = 0;
  TGraph* ueUp = 0;
  TGraph* ueDown = 0;
  TH1D* hErr = 0, *hErr1, *hErr2, *hErr3;
  {
    const int ndiv = 100;
    double x[ndiv];
    double fragup[ndiv];
    double fragdown[ndiv];
    double ueup[ndiv];
    double uedown[ndiv];

    double xbins[ndiv+1];
    const double dx = (plotLog ?
		       pow(emax / emin, 1. / (ndiv - 1)) : 
		       (emax - emin) / (double) (ndiv - 1));
    xbins[0] = emin;
    for (int i = 1; i != ndiv+1; ++i) {

      if (plotLog) xbins[i] = xbins[i-1] * dx;
      else         xbins[i] = xbins[i-1] + dx;
    }

    hErr = new TH1D("hErr", "hErr", ndiv, xbins);
    hErr1 = new TH1D("hErr1", "hErr1", ndiv, xbins);
    hErr2 = new TH1D("hErr2", "hErr2", ndiv, xbins);
    hErr3 = new TH1D("hErr3", "hErr3", ndiv, xbins);

    for(int i = 0; i != ndiv; ++i) {

      x[i] = 0.5 * (xbins[i] + xbins[i+1]);
      double err = 0;
      //double k = kjet._partonFrag(x[i]) * kjet._partonUE(x[i]);

      double r = kjetFrag.Rjet(x[i], err);
      fragup[i] = 100 * err / r;
      fragdown[i] = -100 * err / r;

      r = kjetUE.Rjet(x[i], err);
      ueup[i] = 100 * err / r;
      uedown[i] = -100 * err / r;
    }

    for (int ibin = 1; ibin != hErr->GetNbinsX()+1; ++ibin) {

      double x = hErr->GetBinCenter(ibin);
      double err = 0;
      double r = kjetParton.Rjet(x, err);
      hErr->SetBinContent(ibin, 0);
      hErr->SetBinError(ibin, 100 * err / r);

      double k = kjetParton._partonFrag(x) * kjetParton._partonUE(x);
      r = kjetParton.Rjet(x, err);
      hErr1->SetBinContent(ibin, k);
      hErr1->SetBinError(ibin, err / r * k);
 
      k = kjetFrag._partonFrag(x);
      r = kjetFrag.Rjet(x, err);
      hErr2->SetBinContent(ibin, k);
      hErr2->SetBinError(ibin, err / r * k);

      k = kjetUE._partonUE(x);
      r = kjetUE.Rjet(x, err);
      hErr3->SetBinContent(ibin, k);
      hErr3->SetBinError(ibin, err / r * k);
    }

    fragUp = new TGraph(ndiv, x, fragup);
    fragDown = new TGraph(ndiv, x, fragdown);
    ueUp = new TGraph(ndiv, x, ueup);
    ueDown = new TGraph(ndiv, x, uedown);
  }

  hErr->SetFillStyle(1001);
  hErr->SetFillColor(kYellow);//5);
  hErr->SetLineColor(hErr->GetFillColor());
  hErr->SetMarkerColor(hErr->GetFillColor());
  hErr->SetMarkerSize(0);

  hErr1->SetLineStyle(kDashed);
  hErr1->SetLineWidth(2);
  hErr1->SetFillStyle(kCrossHatch);//3013);//3004);
  hErr1->SetFillColor(kRed);//Black);
  hErr1->SetLineColor(kBlack);//kRed);
  hErr1->SetMarkerColor(hErr1->GetFillColor());
  hErr1->SetMarkerSize(0);

  hErr2->SetLineStyle(kSolid);
  hErr2->SetLineWidth(2);
  hErr2->SetFillStyle(1001);
  hErr2->SetFillColor(kYellow);
  hErr2->SetLineColor(kBlack);//kYellow);
  hErr2->SetMarkerColor(hErr2->GetFillColor());
  hErr2->SetMarkerSize(0);

  hErr3->SetLineStyle(kDotted);
  hErr3->SetLineWidth(2);
  hErr3->SetFillStyle(1001);
  hErr3->SetFillColor(kBeige);// kYellow);
  hErr3->SetLineColor(kBlack);//kBeige);
  hErr3->SetMarkerColor(hErr3->GetFillColor());
  hErr3->SetMarkerSize(0);

  fragUp->SetLineStyle(kDashed);
  fragUp->SetLineWidth(1);
  fragDown->SetLineStyle(kDashed);
  fragDown->SetLineWidth(1);
  ueUp->SetLineStyle(kSolid);
  ueUp->SetLineWidth(1);
  ueDown->SetLineStyle(kSolid);
  ueDown->SetLineWidth(1);

  {
    string cName = "L3PartonUncert_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C->SetLogx();

    TH1F* HE2 = new TH1F((cName + "_E2").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE2->SetMinimum(-6.99);//-5.99);
    HE2->SetMaximum(6.99);//5.99);
    HE2->SetStats(kFALSE);
    HE2->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE2->GetXaxis()->SetTitleOffset(1.1);
    HE2->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE2->GetXaxis()->SetNoExponent(kTRUE);
    HE2->GetYaxis()->SetTitle("rel. error on parton corr. [%]");
    HE2->GetYaxis()->SetTitleOffset(0.9);
    HE2->Draw("AXIS");

    hErr->Draw("SAME E3");
    HE2->Draw("SAME AXIS");

    fragUp->Draw("L");
    fragDown->Draw("L");
    ueUp->Draw("L");
    ueDown->Draw("L");

    DrawLine(fragUp, 0.45, 0.31, "hadronization");
    DrawLine(ueUp, 0.45, 0.25, "underlying event");
    DrawFill(hErr, 0.45, 0.19, "total error");

    C->Update();

    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

  // Draw plot of the parton correction with error bands
  {
    string cName = "L3Parton_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C2 = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C2->SetLogx();

    // Improve some settings for this
    C2->SetLeftMargin(0.15);
    
    TH1F* HE3 = new TH1F((cName + "_E3").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    //HE3->SetMinimum(0.94001);
    //HE3->SetMaximum(1.05999);
    HE3->SetMinimum(0.85001);//0.89001);
    HE3->SetMaximum(1.14999);//1.10999);
    HE3->SetStats(kFALSE);
    HE3->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE3->GetXaxis()->SetTitleOffset(1.1);
    HE3->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE3->GetXaxis()->SetNoExponent(kTRUE);
    HE3->GetYaxis()->SetTitle("physics showering");
    HE3->GetYaxis()->SetTitleOffset(1.2);

    TH1D *hErr1b = (TH1D*)hErr1->Clone("HErr1b");
    hErr1b->SetFillStyle(kNone);
    hErr1b->SetFillColor(0);

    TH1D *hErr2b = (TH1D*)hErr2->Clone("HErr2b");
    hErr2b->SetFillStyle(kNone);
    hErr2b->SetFillColor(0);

    TH1D *hErr3b = (TH1D*)hErr3->Clone("HErr3b");
    hErr3b->SetFillStyle(kNone);
    hErr3b->SetFillColor(0);
    
    HE3->Draw("AXIS");
    hErr2->Draw("SAME E4");
    hErr2b->SetLineStyle(kDashed);
    hErr2b->SetLineColor(kBlack);
    hErr2b->Draw("SAME HIST L");

    hErr3->Draw("SAME E4");
    hErr3b->SetLineStyle(kDashDotted);
    hErr3b->SetLineColor(kBlack);
    hErr3b->Draw("SAME HIST L");

    hErr1->Draw("SAME E4");
    hErr1b->SetLineStyle(kSolid);
    hErr1b->SetLineColor(kRed);//kBlack);
    hErr1b->Draw("SAME HIST L");
    
    DrawFill(hErr2, 0.45, 0.31, "");
    DrawLine(hErr2b, 0.45, 0.31, "hadronization");
    DrawFill(hErr3, 0.45, 0.25, "");
    DrawLine(hErr3b, 0.45, 0.25, "underlying event");
    DrawFill(hErr1, 0.45, 0.19, "");
    //DrawLine(hErr1b, 0.45, 0.19, "total parton corr.");
    DrawLine(hErr1b, 0.45, 0.19, "physics showering");

    C2->Update();
    
    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C2->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

}


void FlavorUncertainty() {

  // Improve some settings for this
  gStyle->SetPadLeftMargin(0.12);

  const bool plotLog = true;
  L3Corr::JetAlg jetAlg = L3Corr::IC5_DATA;
  //L3Corr::JetAlg jetAlg = L3Corr::IC5_MC;

  jec::ErrorTypes other = jec::kAll & ~jec::kL3Flavor;
  jec::ErrorTypes any = jec::kAll | jec::kL3Flavor;

  //L3Corr rjet(jetAlg, jec::kAll);
  L3Corr rjet(jetAlg, other);
  L3Corr rjetFlavor(jetAlg, jec::kL3Flavor);
  L3Corr rjetAny(jetAlg, any);

  double erangemin = (plotLog) ? 15 : 0.01;
  double erangemax = (plotLog) ? maxpt : maxpt;
  double emin = (plotLog) ? 20 : 20;
  double emax = (plotLog) ? maxpt*0.8 : maxpt*0.95;

  TGraph* flavorUp = 0;
  TGraph* flavorDown = 0;
  TGraph* allUp = 0;
  TGraph* allDown = 0;
  TH1D* hErr = 0, *hErr1;
  {
    const int ndiv = 100;
    double x[ndiv];
    double flavorup[ndiv];
    double flavordown[ndiv];
    double allup[ndiv];
    double alldown[ndiv];

    double xbins[ndiv+1];
    const double dx = (plotLog ?
		       pow(emax / emin, 1. / (ndiv - 1)) : 
		       (emax - emin) / (double) (ndiv - 1));
    xbins[0] = emin;
    for (int i = 1; i != ndiv+1; ++i) {

      if (plotLog) xbins[i] = xbins[i-1] * dx;
      else         xbins[i] = xbins[i-1] + dx;
    }

    hErr = new TH1D("hErr", "hErr", ndiv, xbins);
    hErr1 = new TH1D("hErr1", "hErr1", ndiv, xbins);

    for(int i = 0; i != ndiv; ++i) {

      x[i] = 0.5 * (xbins[i] + xbins[i+1]);
      double err = 0;

      double r = rjetFlavor.Rjet(x[i], err);
      flavorup[i] = 100 * err / r;
      flavordown[i] = -100 * err / r;

      r = rjet.Rjet(x[i], err);
      allup[i] = 100 * err / r;
      alldown[i] = -100 * err / r;
    }

    for (int ibin = 1; ibin != hErr->GetNbinsX()+1; ++ibin) {

      double x = hErr->GetBinCenter(ibin);
      double err = 0;
      double r = rjetAny.Rjet(x, err);
      hErr->SetBinContent(ibin, 0);
      hErr->SetBinError(ibin, 100 * err / r);

      double f = rjetFlavor._flavorMap(x);
      r = rjetFlavor.Rjet(x, err);
      hErr1->SetBinContent(ibin, f);
      hErr1->SetBinError(ibin, err / r * f);
    }

    flavorUp = new TGraph(ndiv, x, flavorup);
    flavorDown = new TGraph(ndiv, x, flavordown);
    allUp = new TGraph(ndiv, x, allup);
    allDown = new TGraph(ndiv, x, alldown);
  }

  hErr->SetFillStyle(1001);
  hErr->SetFillColor(kYellow);//5);
  hErr->SetLineColor(hErr->GetFillColor());
  hErr->SetMarkerColor(hErr->GetFillColor());
  hErr->SetMarkerSize(0);

  hErr1->SetLineStyle(kDashed);
  hErr1->SetLineWidth(2);
  hErr1->SetFillStyle(1001);
  hErr1->SetFillColor(kYellow);
  hErr1->SetLineColor(kBlack);
  hErr1->SetMarkerColor(hErr1->GetFillColor());
  hErr1->SetMarkerSize(0);

  flavorUp->SetLineStyle(kSolid);
  flavorUp->SetLineWidth(2);
  flavorDown->SetLineStyle(kSolid);
  flavorDown->SetLineWidth(2);

  allUp->SetLineStyle(kDashed);
  allUp->SetLineWidth(2);
  allDown->SetLineStyle(kDashed);
  allDown->SetLineWidth(2);

  {
    string cName = "L3FlavorUncert_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C->SetLogx();

    TH1F* HE2 = new TH1F((cName + "_E2").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE2->SetMinimum(-15.99);
    HE2->SetMaximum(15.99);
    HE2->SetStats(kFALSE);
    HE2->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE2->GetXaxis()->SetTitleOffset(1.1);
    HE2->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE2->GetXaxis()->SetNoExponent(kTRUE);
    HE2->GetYaxis()->SetTitle("relative error [%]");
    HE2->GetYaxis()->SetTitleOffset(0.9);
    HE2->Draw("AXIS");

    hErr->Draw("SAME E3");
    HE2->Draw("SAME AXIS");

    flavorUp->Draw("L");
    flavorDown->Draw("L");
    allUp->Draw("L");
    allDown->Draw("L");

    DrawLine(flavorUp, 0.45, 0.34, "flavor correction");
    //DrawLine(allUp, 0.45, 0.28, "L3 corr.");
    DrawLine(allUp, 0.45, 0.28, "other sources");
    //DrawFill(hErr, 0.45, 0.22, "total L3+flavor err.");
    DrawFill(hErr, 0.45, 0.22, "total error");

    C->Update();

    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

  // Draw plot of the flavor correction with error band
  {
    string cName = "L3Flavor_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C2 = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C2->SetLogx();

    // Improve some settings for this
    C2->SetLeftMargin(0.15);
    
    TH1F* HE3 = new TH1F((cName + "_E3").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE3->SetMinimum(0.60001);
    HE3->SetMaximum(1.05999);
    HE3->SetStats(kFALSE);
    HE3->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE3->GetXaxis()->SetTitleOffset(1.1);
    HE3->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE3->GetXaxis()->SetNoExponent(kTRUE);
    HE3->GetYaxis()->SetTitle("Flavor correction");
    HE3->GetYaxis()->SetTitleOffset(1.2);

    TH1D *hErr1b = (TH1D*)hErr1->Clone("HErr1b");
    hErr1b->SetFillStyle(kNone);
    hErr1b->SetFillColor(0);

    HE3->Draw("AXIS");
    hErr1->Draw("SAME E4");
    hErr1b->SetLineStyle(kSolid);
    hErr1b->SetLineColor(kBlack);
    hErr1b->Draw("SAME HIST L");
    
    DrawFill(hErr1, 0.45, 0.22, "");
    DrawLine(hErr1b, 0.45, 0.22, "flavor correction");

    C2->Update();
    
    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C2->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

}


void BiasCorrections() {

  // Improve some settings for this
  gStyle->SetPadLeftMargin(0.12);

  const bool plotLog = true;
  L3Corr::JetAlg jetAlg = L3Corr::IC5_DATA;
  //L3Corr::JetAlg jetAlg = L3Corr::IC5_MC;

  L3Corr::PhotonID id = L3Corr::kMedium;
  
  jec::ErrorTypes bias = //jec::kAll & ~jec::kL3Flavor;
    //jec::kL3QCDBackground | jec::kL3Parton | jec::kL3HardGluon;
    jec::kL3PhotonES | jec::kL3HardGluon | jec::kL3Parton;
  jec::ErrorTypes bkg = jec::kL3QCDBackground;
  jec::ErrorTypes photon = jec::kL3PhotonES;
  jec::ErrorTypes topo = jec::kL3HardGluon;
  jec::ErrorTypes parton = jec::kL3Parton;

  L3Corr rjet(jetAlg, bias);
  L3Corr rjetBkg(jetAlg, bkg);
  L3Corr rjetTopo(jetAlg, topo);
  L3Corr rjetParton(jetAlg, parton);
  L3Corr rjetPeak(jetAlg, parton);
  L3Corr rjetBalance(jetAlg, parton);
  L3Corr rjetPhoton(jetAlg, photon);

  double erangemin = (plotLog) ? 15 : 0.01;
  double erangemax = (plotLog) ? maxpt : maxpt;
  double emin = (plotLog) ? 20 : 20;
  double emax = (plotLog) ? maxpt*0.8 : maxpt*0.95;

  TGraph* bkgUp = 0;
  TGraph* bkgDown = 0;
  TGraph* topoUp = 0;
  TGraph* topoDown = 0;
  TGraph* partonUp = 0;
  TGraph* partonDown = 0;
  TGraph* peakUp = 0;
  TGraph* peakDown = 0;
  TGraph* balanceUp = 0;
  TGraph* balanceDown = 0;
  TGraph* photonUp = 0;
  TGraph* photonDown = 0;
  TGraph* allUp = 0;
  TGraph* allDown = 0;
  TH1D* hErr = 0, *hErr1, *hErr2, *hErr3, *hErr4, *hErr5, *hErr6, *hAll;
  {
    const int ndiv = 100;
    double x[ndiv];
    double bkgup[ndiv];
    double bkgdown[ndiv];
    double topoup[ndiv];
    double topodown[ndiv];
    double peakup[ndiv];
    double peakdown[ndiv];
    double partonup[ndiv];
    double partondown[ndiv];
    double balanceup[ndiv];
    double balancedown[ndiv];
    double photonup[ndiv];
    double photondown[ndiv];
    double allup[ndiv];
    double alldown[ndiv];

    double xbins[ndiv+1];
    const double dx = (plotLog ?
		       pow(emax / emin, 1. / (ndiv - 1)) : 
		       (emax - emin) / (double) (ndiv - 1));
    xbins[0] = emin;
    for (int i = 1; i != ndiv+1; ++i) {

      if (plotLog) xbins[i] = xbins[i-1] * dx;
      else         xbins[i] = xbins[i-1] + dx;
    }

    hErr = new TH1D("hErr", "hErr", ndiv, xbins);
    hErr1 = new TH1D("hErr1", "hErr1", ndiv, xbins);
    hErr2 = new TH1D("hErr2", "hErr2", ndiv, xbins);
    hErr3 = new TH1D("hErr3", "hErr3", ndiv, xbins);
    hErr4 = new TH1D("hErr4", "hErr4", ndiv, xbins);
    hErr5 = new TH1D("hErr5", "hErr5", ndiv, xbins);
    hErr6 = new TH1D("hErr6", "hErr6", ndiv, xbins);
    hAll = new TH1D("hAll", "hAll", ndiv, xbins);

    for(int i = 0; i != ndiv; ++i) {

      x[i] = 0.5 * (xbins[i] + xbins[i+1]);
      double err = 0;

      double r = rjetBkg.Rjet(x[i], err);
      bkgup[i] = 100 * err / r;
      bkgdown[i] = -100 * err / r;

      rjetTopo.Rjet(x[i], err);
      topoup[i] = 100 * err / r;
      topodown[i] = -100 * err / r;

      rjetParton.Rjet(x[i], err);
      partonup[i] = 100 * err / r;
      partondown[i] = -100 * err / r;

      rjetPeak.Rjet(x[i], err);
      peakup[i] = 0.;//100 * err / r;
      peakdown[i] = 0.;//-100 * err / r;

      rjetBalance.Rjet(x[i], err);
      balanceup[i] = 0.;//100 * err / r;
      balancedown[i] = 0.;//-100 * err / r;

      rjetPhoton.Rjet(x[i], err);
      photonup[i] = 100 * err / r;
      photondown[i] = -100 * err / r;

      rjet.Rjet(x[i], err);
      allup[i] = 100 * err / r;
      alldown[i] = -100 * err / r;
    }

    for (int ibin = 1; ibin != hErr->GetNbinsX()+1; ++ibin) {

      double x = hErr->GetBinCenter(ibin);
      double err = 0.;
      double r = rjet.Rjet(x, err);
      hErr->SetBinContent(ibin, 0);
      hErr->SetBinError(ibin, 100. * err / r);

      r = rjetBkg.Rjet(x, err);
      double dr = rjetBkg._RbiasBkg(x, id);
      hErr1->SetBinContent(ibin, dr);
      hErr1->SetBinError(ibin, err / r * dr);

      r = rjetTopo.Rjet(x, err);
      dr = rjetTopo._RbiasTopo(x, id);
      hErr2->SetBinContent(ibin, dr);
      hErr2->SetBinError(ibin, err / r * dr);

      r = rjetParton.Rjet(x, err);
      dr = rjetParton._RbiasParton(x, id); // _parton(x);
      hErr3->SetBinContent(ibin, dr);
      hErr3->SetBinError(ibin, err / r * dr);

      r = rjetPeak.Rjet(x, err);
      dr = rjetPeak._RbiasPeak(x);
      hErr4->SetBinContent(ibin, dr);
      hErr4->SetBinError(ibin, 1e-4);//err / r * (1 + dr));

      r = rjetBalance.Rjet(x, err);
      dr = rjetBalance._RbiasBalance(x);
      hErr5->SetBinContent(ibin, dr);
      hErr5->SetBinError(ibin, 1e-4);//err / r * (1 + dr));

      r = rjetPhoton.Rjet(x, err);
      dr = rjetPhoton._RbiasPhot(x, id);
      hErr6->SetBinContent(ibin, dr);
      //hErr6->SetBinError(ibin, err / r * (1 + dr));
      hErr6->SetBinError(ibin, err / r * dr);

      r = rjet.Rjet(x, err);
      dr = //rjetBkg._RbiasBkg(x,id) * rjetTopo._RbiasTopo(x, id)
	rjetPhoton._RbiasPhot(x, id) * rjetTopo._RbiasTopo(x, id)
	//* rjetParton._parton(x) * rjetPeak._RbiasPeak(x)
	* rjetParton._RbiasParton(x, id) * rjetBalance._RbiasBalance(x)
	* rjetPeak._RbiasPeak(x);
      hAll->SetBinContent(ibin, dr);
      hAll->SetBinError(ibin, err / r * dr);
    }

    bkgUp = new TGraph(ndiv, x, bkgup);
    bkgDown = new TGraph(ndiv, x, bkgdown);
    topoUp = new TGraph(ndiv, x, topoup);
    topoDown = new TGraph(ndiv, x, topodown);
    partonUp = new TGraph(ndiv, x, partonup);
    partonDown = new TGraph(ndiv, x, partondown);
    peakUp = new TGraph(ndiv, x, peakup);
    peakDown = new TGraph(ndiv, x, peakdown);
    balanceUp = new TGraph(ndiv, x, balanceup);
    balanceDown = new TGraph(ndiv, x, balancedown);
    photonUp = new TGraph(ndiv, x, photonup);
    photonDown = new TGraph(ndiv, x, photondown);
    allUp = new TGraph(ndiv, x, allup);
    allDown = new TGraph(ndiv, x, alldown);
  }

  hErr->SetFillStyle(1001);
  hErr->SetFillColor(kYellow);
  hErr->SetLineColor(hErr->GetFillColor());
  hErr->SetMarkerColor(hErr->GetFillColor());
  hErr->SetMarkerSize(0);

  // background
  hErr1->SetLineStyle(kSolid);
  hErr1->SetLineWidth(2);
  hErr1->SetFillStyle(kLeftHatch);
  hErr1->SetFillColor(kRed);
  hErr1->SetLineColor(kBlack);
  hErr1->SetMarkerColor(hErr1->GetFillColor());
  hErr1->SetMarkerSize(0);

  // topology
  hErr2->SetLineStyle(kDashed);
  hErr2->SetLineWidth(2);
  hErr2->SetFillStyle(kVertHatch);
  hErr2->SetFillColor(kBlue);
  hErr2->SetLineColor(kBlack);
  hErr2->SetMarkerColor(hErr2->GetFillColor());
  hErr2->SetMarkerSize(0);

  // physics shower
  hErr3->SetLineStyle(kDotted);
  hErr3->SetLineWidth(2);
  hErr3->SetFillStyle(kRightHatch);
  hErr3->SetFillColor(kGreen);
  hErr3->SetLineColor(kBlack);
  hErr3->SetMarkerColor(hErr3->GetFillColor());
  hErr3->SetMarkerSize(0);

  // peak vs mean
  hErr4->SetLineStyle(kDashDotted);
  hErr4->SetLineWidth(2);
  hErr4->SetFillStyle(1001);
  hErr4->SetFillColor(kBlue);
  hErr4->SetLineColor(kBlack);
  hErr4->SetMarkerColor(hErr4->GetFillColor());
  hErr4->SetMarkerSize(0);

  // balance between partons
  hErr5->SetLineStyle(kDashDotted);
  hErr5->SetLineWidth(2);
  hErr5->SetFillStyle(1001);
  hErr5->SetFillColor(kBlue);
  hErr5->SetLineColor(kBlack);
  hErr5->SetMarkerColor(hErr5->GetFillColor());
  hErr5->SetMarkerSize(0);

  // photon scale
  hErr6->SetLineStyle(kDashDotted);//kSolid);
  hErr6->SetLineWidth(2);
  hErr6->SetFillStyle(kLeftHatch);
  hErr6->SetFillColor(kRed);
  hErr6->SetLineColor(kBlack);
  hErr6->SetMarkerColor(hErr6->GetFillColor());
  hErr6->SetMarkerSize(0);

  hAll->SetLineStyle(kSolid);
  hAll->SetLineWidth(2);
  hAll->SetFillStyle(1001);
  hAll->SetFillColor(kYellow);
  hAll->SetLineColor(kBlack);
  hAll->SetMarkerColor(hAll->GetFillColor());
  hAll->SetMarkerSize(0);

  bkgUp->SetLineStyle(kSolid);
  bkgUp->SetLineWidth(2);
  bkgDown->SetLineStyle(kSolid);
  bkgDown->SetLineWidth(2);

  topoUp->SetLineStyle(kDashed);
  topoUp->SetLineWidth(2);
  topoDown->SetLineStyle(kDashed);
  topoDown->SetLineWidth(2);

  partonUp->SetLineStyle(kDotted);
  partonUp->SetLineWidth(2);
  partonDown->SetLineStyle(kDotted);
  partonDown->SetLineWidth(2);

  peakUp->SetLineStyle(kDashDotted);
  peakUp->SetLineWidth(2);
  peakDown->SetLineStyle(kDashDotted);
  peakDown->SetLineWidth(2);

  balanceUp->SetLineStyle(kDashDotted);
  balanceUp->SetLineWidth(2);
  balanceDown->SetLineStyle(kDashDotted);
  balanceDown->SetLineWidth(2);

  photonUp->SetLineStyle(kSolid);
  photonUp->SetLineWidth(2);
  photonDown->SetLineStyle(kSolid);
  photonDown->SetLineWidth(2);

  allUp->SetLineStyle(kDashed);
  allUp->SetLineWidth(2);
  allDown->SetLineStyle(kDashed);
  allDown->SetLineWidth(2);

  {
    string cName = "L3BiasUncert_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C->SetLogx();

    TH1F* HE2 = new TH1F((cName + "_E2").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE2->SetMinimum(-15.99);
    HE2->SetMaximum(15.99);
    HE2->SetStats(kFALSE);
    HE2->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE2->GetXaxis()->SetTitleOffset(1.1);
    HE2->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE2->GetXaxis()->SetNoExponent(kTRUE);
    HE2->GetYaxis()->SetTitle("rel. error on bias [%]");
    HE2->GetYaxis()->SetTitleOffset(0.9);
    HE2->Draw("AXIS");

    hErr->Draw("SAME E3");
    HE2->Draw("SAME AXIS");

    //bkgUp->Draw("L");
    //bkgDown->Draw("L");
    topoUp->Draw("L");
    topoDown->Draw("L");
    partonUp->Draw("L");
    partonDown->Draw("L");
    //peakUp->Draw("L");
    //peakDown->Draw("L");
    //balanceUp->Draw("L");
    //balanceDown->Draw("L");
    photonUp->Draw("L");
    photonDown->Draw("L");

    DrawFill(hErr, 0.54, 0.19, "total bias");
    //DrawLine(bkgUp, 0.18, 0.25, "background");
    DrawLine(photonUp, 0.18, 0.25, "photon scale");
    DrawLine(topoUp, 0.18, 0.19, "second jet");//"topology");
    //DrawLine(partonUp, 0.54, 0.25, "physics shower"); // was 0.54, 0.31
    DrawLine(partonUp, 0.54, 0.25, "parton correction"); // was 0.54, 0.31
    //DrawLine(peakUp, 0.54, 0.25, "peak vs mean");
    //DrawLine(peakUp, 0.54, 0.25, "parton balance");

    C->Update();

    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

  // Draw plots of the different bias corrections
  {
    string cName = "L3Bias_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C2 = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C2->SetLogx();

    // Improve some settings for this
    C2->SetLeftMargin(0.15);
    
    TH1F* HE3 = new TH1F((cName + "_E3").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE3->SetMinimum(0.60);//0.70);//0.60);//0.90);
    HE3->SetMaximum(1.10);
    HE3->SetStats(kFALSE);
    HE3->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE3->GetXaxis()->SetTitleOffset(1.1);
    HE3->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE3->GetXaxis()->SetNoExponent(kTRUE);
    HE3->GetYaxis()->SetTitle("Response bias");
    HE3->GetYaxis()->SetTitleOffset(1.2);

    TH1D *hErr1b = (TH1D*)hErr1->Clone("HErr1b");
    hErr1b->SetFillStyle(kNone);
    hErr1b->SetFillColor(0);

    TH1D *hErr2b = (TH1D*)hErr2->Clone("HErr2b");
    hErr2b->SetFillStyle(kNone);
    hErr2b->SetFillColor(0);

    TH1D *hErr3b = (TH1D*)hErr3->Clone("HErr3b");
    hErr3b->SetFillStyle(kNone);
    hErr3b->SetFillColor(0);

    TH1D *hErr4b = (TH1D*)hErr4->Clone("HErr4b");
    hErr4b->SetFillStyle(kNone);
    hErr4b->SetFillColor(0);

    TH1D *hErr5b = (TH1D*)hErr5->Clone("HErr5b");
    hErr5b->SetFillStyle(kNone);
    hErr5b->SetFillColor(0);

    TH1D *hErr6b = (TH1D*)hErr6->Clone("HErr6b");
    hErr6b->SetFillStyle(kNone);
    hErr6b->SetFillColor(0);

    TH1D *hAllb = (TH1D*)hAll->Clone("HAllb");
    hAllb->SetFillStyle(kNone);
    hAllb->SetFillColor(0);
    
    HE3->Draw("AXIS");
    hAll->Draw("SAME E4");
    hAllb->Draw("SAME HIST L");
    //hErr1->Draw("SAME E4");
    //hErr1b->Draw("SAME HIST L");
    hErr2->Draw("SAME E4");
    hErr2b->Draw("SAME HIST L");
    hErr3->Draw("SAME E4");
    hErr3b->Draw("SAME HIST L");
    //hErr4->Draw("SAME E4");
    //hErr4b->Draw("SAME HIST L");
    //hErr5->Draw("SAME E4");
    //hErr5b->Draw("SAME HIST L");
    hErr6->Draw("SAME E4");
    hErr6b->Draw("SAME HIST L");    

    //DrawFill(hErr1, 0.18, 0.25, "background");
    //DrawLine(hErr1b, 0.18, 0.25, "");
    DrawFill(hErr6, 0.18, 0.25, "photon scale");
    DrawLine(hErr6b, 0.18, 0.25, "");
    DrawFill(hErr2, 0.18, 0.19, "second jet");//topology");
    DrawLine(hErr2b, 0.18, 0.19, "");
    DrawFill(hErr3, 0.54, 0.25, "parton correction"); // was 0.54, 0.31
    DrawLine(hErr3b, 0.54, 0.25, ""); // was 0.54, 0.31
    //DrawFill(hErr4, 0.54, 0.25, "peak vs mean");
    //DrawLine(hErr4b, 0.54, 0.25, "");
    //DrawFill(hErr5, 0.54, 0.25, "parton balance");
    //DrawLine(hErr5b, 0.54, 0.25, "");
    DrawFill(hAll, 0.54, 0.19, "total");
    DrawLine(hAllb, 0.54, 0.19, "");

    C2->Update();
    
    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C2->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

}


void TopoUncertainty() {

  // Improve some settings for this
  gStyle->SetPadLeftMargin(0.12);

  const bool plotLog = true;
  L3Corr::JetAlg jetAlg = L3Corr::IC5_DATA;
  //L3Corr::JetAlg jetAlg = L3Corr::IC5_MC;

  jec::ErrorTypes all = jec::kAll;

  L3Corr rjet(jetAlg, all);
  L3Corr rjetLoose(jetAlg, jec::kL3HardGluon, L3Corr::kMedium005);
  L3Corr rjetMedium(jetAlg, jec::kL3HardGluon, L3Corr::kMedium010);
  L3Corr rjetTight(jetAlg, jec::kL3HardGluon, L3Corr::kMedium020);

  double erangemin = (plotLog) ? 15 : 0.01;
  double erangemax = (plotLog) ? maxpt : maxpt;
  double emin = (plotLog) ? 20 : 20;
  double emax = (plotLog) ? maxpt*0.8 : maxpt*0.95;

  TGraph* looseUp = 0;
  TGraph* looseDown = 0;
  TGraph* mediumUp = 0;
  TGraph* mediumDown = 0;
  TGraph* tightUp = 0;
  TGraph* tightDown = 0;
  TH1D* hErr = 0, *hErr0, *hErr1, *hErr2, *hErr3;
  {
    const int ndiv = 100;
    double x[ndiv];
    double looseup[ndiv];
    double loosedown[ndiv];
    double mediumup[ndiv];
    double mediumdown[ndiv];
    double tightup[ndiv];
    double tightdown[ndiv];

    double xbins[ndiv+1];
    const double dx = (plotLog ?
		       pow(emax / emin, 1. / (ndiv - 1)) : 
		       (emax - emin) / (double) (ndiv - 1));
    xbins[0] = emin;
    for (int i = 1; i != ndiv+1; ++i) {

      if (plotLog) xbins[i] = xbins[i-1] * dx;
      else         xbins[i] = xbins[i-1] + dx;
    }

    hErr = new TH1D("hErr", "hErr", ndiv, xbins);
    hErr0 = new TH1D("hErr0", "hErr0", ndiv, xbins);
    hErr1 = new TH1D("hErr1", "hErr1", ndiv, xbins);
    hErr2 = new TH1D("hErr2", "hErr2", ndiv, xbins);
    hErr3 = new TH1D("hErr3", "hErr3", ndiv, xbins);

    for(int i = 0; i != ndiv; ++i) {

      x[i] = 0.5 * (xbins[i] + xbins[i+1]);
      double err = 0.;

      double r = rjetLoose.Rjet(x[i], err);
      looseup[i] = 100 * err / r;
      loosedown[i] = -100 * err / r;

      r = rjetMedium.Rjet(x[i], err);
      mediumup[i] = 100 * err / r;
      mediumdown[i] = -100 * err / r;

      r = rjetTight.Rjet(x[i], err);
      tightup[i] = 100 * err / r;
      tightdown[i] = -100 * err / r;

    }

    for (int ibin = 1; ibin != hErr->GetNbinsX()+1; ++ibin) {

      double x = hErr->GetBinCenter(ibin);
      double err = 0.;
      double ctopo = rjet._RbiasTopo(x, L3Corr::kMedium);
      double r = rjet.Rjet(x, err);
      hErr->SetBinContent(ibin, 0);
      hErr->SetBinError(ibin, 100. * err / r);
      hErr0->SetBinContent(ibin, ctopo);
      hErr0->SetBinError(ibin, err / r * ctopo);

      r = rjetLoose.Rjet(x, err);
      ctopo = rjetLoose._RbiasTopo(x, L3Corr::kMedium020);
      hErr1->SetBinContent(ibin, ctopo);
      hErr1->SetBinError(ibin, err / r * ctopo);

      r = rjetMedium.Rjet(x, err);
      ctopo = rjetMedium._RbiasTopo(x, L3Corr::kMedium010);
      hErr2->SetBinContent(ibin, ctopo);
      hErr2->SetBinError(ibin, err / r * ctopo);

      r = rjetTight.Rjet(x, err);
      ctopo = rjetTight._RbiasTopo(x, L3Corr::kMedium005);
      hErr3->SetBinContent(ibin, ctopo);
      hErr3->SetBinError(ibin, err / r * ctopo);
    }

    looseUp = new TGraph(ndiv, x, looseup);
    looseDown = new TGraph(ndiv, x, loosedown);
    mediumUp = new TGraph(ndiv, x, mediumup);
    mediumDown = new TGraph(ndiv, x, mediumdown);
    tightUp = new TGraph(ndiv, x, tightup);
    tightDown = new TGraph(ndiv, x, tightdown);
  }

  hErr->SetFillStyle(1001);
  hErr->SetFillColor(5);
  hErr->SetLineColor(hErr->GetFillColor());
  hErr->SetMarkerColor(hErr->GetFillColor());
  hErr->SetMarkerSize(0);

  hErr0->SetLineStyle(kSolid);
  hErr0->SetLineWidth(2);
  hErr0->SetFillStyle(1001);
  hErr0->SetFillColor(kYellow);
  hErr0->SetLineColor(kBlack);
  hErr0->SetMarkerColor(hErr2->GetFillColor());
  hErr0->SetMarkerSize(0);

  hErr1->SetLineStyle(kDashed);
  hErr1->SetLineWidth(2);
  hErr1->SetFillStyle(kLeftHatch);
  hErr1->SetFillColor(kBlue);
  hErr1->SetLineColor(kBlue);
  hErr1->SetMarkerColor(hErr1->GetFillColor());
  hErr1->SetMarkerSize(0);

  hErr2->SetLineStyle(kSolid);
  hErr2->SetLineWidth(2);
  hErr2->SetFillStyle(1001);
  hErr2->SetFillColor(kYellow);
  hErr2->SetLineColor(kBlack);
  hErr2->SetMarkerColor(hErr2->GetFillColor());
  hErr2->SetMarkerSize(0);

  hErr3->SetLineStyle(kDashDotted);
  hErr3->SetLineWidth(2);
  hErr3->SetFillStyle(kRightHatch);
  hErr3->SetFillColor(kRed);
  hErr3->SetLineColor(kRed);
  hErr3->SetMarkerColor(hErr3->GetFillColor());
  hErr3->SetMarkerSize(0);

  looseUp->SetLineStyle(kDashed);
  looseUp->SetLineWidth(2);
  looseDown->SetLineStyle(kDashed);
  looseDown->SetLineWidth(2);
  mediumUp->SetLineStyle(kSolid);
  mediumUp->SetLineWidth(2);
  mediumDown->SetLineStyle(kSolid);
  mediumDown->SetLineWidth(2);
  tightUp->SetLineStyle(kDotted);
  tightUp->SetLineWidth(2);
  tightDown->SetLineStyle(kDotted);
  tightDown->SetLineWidth(2);

  {
    string cName = "L3TopoUncert_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C->SetLogx();

    TH1F* HE2 = new TH1F((cName + "_E2").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE2->SetMinimum(-15.99);
    HE2->SetMaximum(15.99);
    HE2->SetStats(kFALSE);
    HE2->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE2->GetXaxis()->SetTitleOffset(1.1);
    HE2->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE2->GetXaxis()->SetNoExponent(kTRUE);
    HE2->GetYaxis()->SetTitle("rel. error topology bias [%]");
    HE2->GetYaxis()->SetTitleOffset(0.9);
    HE2->Draw("AXIS");

    hErr->Draw("SAME E3");
    HE2->Draw("SAME AXIS");

    looseUp->Draw("L");
    looseDown->Draw("L");
    mediumUp->Draw("L");
    mediumDown->Draw("L");
    tightUp->Draw("L");
    tightDown->Draw("L");

    DrawLine(looseUp, 0.44, 0.37, "loose topo (20%)");
    DrawLine(mediumUp, 0.44, 0.31, "medium topo (10%)");
    DrawLine(tightUp, 0.44, 0.25, "tight topo (5%)");
    DrawFill(hErr, 0.44, 0.19, "total L3 err.");

    C->Update();

    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

  // Draw plots of the different background corrections
  {
    string cName = "L3Topo_";
    switch (jetAlg) {
    case L3Corr::IC5_DATA: cName += "ICone5_Data";
      break;
    case L3Corr::IC5_MC: cName += "ICone5_MC";
      break;
    default:
      exit(0);
      break;
    };
    if (plotLog) cName += "_Log";
    
    TCanvas* C2 = new TCanvas(cName.c_str(), cName.c_str(), 600, 600);
    if (plotLog) C2->SetLogx();

    // Improve some settings for this
    C2->SetLeftMargin(0.15);
    
    TH1F* HE3 = new TH1F((cName + "_E3").c_str(), (char*) 0, 100, 
			 erangemin, erangemax);
    HE3->SetMinimum(0.60);//-0.15999);
    HE3->SetMaximum(1.1);//0.15999);
    HE3->SetStats(kFALSE);
    HE3->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE3->GetXaxis()->SetTitleOffset(1.1);
    HE3->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE3->GetXaxis()->SetNoExponent(kTRUE);
    HE3->GetYaxis()->SetTitle("Topology bias");
    HE3->GetYaxis()->SetTitleOffset(1.2);

    TH1D *hErr0b = (TH1D*)hErr0->Clone("HErr0b");
    hErr0b->SetFillStyle(kNone);
    hErr0b->SetFillColor(0);
    hErr0b->SetLineStyle(kSolid);

    TH1D *hErr1b = (TH1D*)hErr1->Clone("HErr1b");
    hErr1b->SetFillStyle(kNone);
    hErr1b->SetFillColor(0);

    TH1D *hErr2b = (TH1D*)hErr2->Clone("HErr2b");
    hErr2b->SetFillStyle(kNone);
    hErr2b->SetFillColor(0);
    hErr2b->SetLineStyle(kDotted);
    hErr2b->SetLineColor(kGreen+2);

    TH1D *hErr3b = (TH1D*)hErr3->Clone("HErr3b");
    hErr3b->SetFillStyle(kNone);
    hErr3b->SetFillColor(0);
    
    HE3->Draw("AXIS");
    //hErr2->Draw("SAME E4");
    //hErr2b->Draw("SAME HIST L");
    ////hErr1->Draw("SAME E4");
    //hErr1b->Draw("SAME HIST L");
    ////hErr3->Draw("SAME E4");
    //hErr3b->Draw("SAME HIST L");
    hErr0->Draw("SAME E4");
    hErr1b->Draw("SAME HIST L");
    hErr2b->Draw("SAME HIST L");
    hErr3b->Draw("SAME HIST L");
    hErr0b->Draw("SAME HIST L");

    DrawFill(hErr0, 0.44, 0.37, "default + unc. (10%)");
    DrawLine(hErr0b, 0.44, 0.37, "");

    //DrawFill(hErr1, 0.44, 0.31, "loose topo (20%)");
    //DrawLine(hErr1b, 0.44, 0.31, "");
    DrawLine(hErr1b, 0.44, 0.31, "loose topo (20%)");
    //DrawFill(hErr2, 0.44, 0.25, "medium + unc. (10%)");
    //DrawLine(hErr2b, 0.44, 0.25, "");
    DrawLine(hErr2b, 0.44, 0.25, "medium topo (10%)");
    //DrawFill(hErr3, 0.44, 0.19, "tight topo (5%)");
    //DrawLine(hErr3b, 0.44, 0.19, "");
    DrawLine(hErr3b, 0.44, 0.19, "tight topo (5%)");

    C2->Update();
    
    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C2->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

}
