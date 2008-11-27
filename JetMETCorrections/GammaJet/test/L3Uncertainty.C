// Adapted from D0 Experiment jetcorr/macros/RjetUncertainty.C
//
// Draw the uncertainty plots for L3 absolute response correction
// The uncertainties are stored in L3Corr.cpp/hpp

#include "TCanvas.h"
#include "TGraph.h"
#include "TH1.h"
#include "TLine.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TROOT.h"

//#include "src/PhotonRjetCorr.cpp"
//#include "L3Corr.cpp" 
#include "L3Corr.hpp" // => Load .cpp in mk_L3Uncertainty.C 

#include <string>

using namespace std;

const double maxpt = 900.;//3000.;

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
  TLatex* tStat = new TLatex(xmin+0.09, ymin, str);
  tStat->SetNDC();
  tStat->SetTextSize(0.045);
  tStat->Draw();
}
void DrawFill(TH1 *h, double xmin, double ymin, const char *str) {

    TLine *l1 = new TLine();
    l1->SetLineStyle(1);
    l1->SetLineWidth(25);
    l1->SetLineColor(h->GetFillColor());
    l1->DrawLineNDC(xmin, ymin+0.01, xmin+0.07, ymin+0.01);
    TLatex* tTot = new TLatex(xmin+0.09, ymin, str);
    tTot->SetNDC();
    tTot->SetTextSize(0.045);
    tTot->Draw();
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

  double erangemin = (plotLog) ? 15 : 0.01;
  double erangemax = (plotLog) ? maxpt : maxpt;
  double emin = (plotLog) ? 20 : 20;
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
  }
  hErr->SetFillStyle(1001);
  hErr->SetFillColor(5);
  hErr->SetLineColor(hErr->GetFillColor());
  hErr->SetMarkerColor(hErr->GetFillColor());
  hErr->SetMarkerSize(0);

  statUp->SetLineStyle(kDashed);
  statUp->SetLineWidth(1);
  statDown->SetLineStyle(kDashed);
  statDown->SetLineWidth(1);
  phUp->SetLineStyle(kSolid);
  phUp->SetLineWidth(1);
  phDown->SetLineStyle(kSolid);
  phDown->SetLineWidth(1);
  qcdUp->SetLineStyle(kDotted);
  qcdUp->SetLineWidth(1);
  qcdDown->SetLineStyle(kDotted);
  qcdDown->SetLineWidth(1);
  partUp->SetLineStyle(kDashDotted);
  partUp->SetLineWidth(1);
  partDown->SetLineStyle(kDashDotted);
  partDown->SetLineWidth(1);
  gluUp->SetLineStyle(kDashed);
  gluUp->SetLineWidth(1);
  gluUp->SetLineColor(kBlue);
  gluDown->SetLineStyle(kDashed);
  gluDown->SetLineWidth(1);
  gluDown->SetLineColor(kBlue);

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
    HE2->SetMinimum(-15.99);//-3.99);
    HE2->SetMaximum(15.99);//3.99);
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

    DrawLine(statUp, 0.18, 0.31, "stat. 10 pb^{-1}");
    DrawLine(phUp, 0.18, 0.25, "photon scale");
    DrawLine(gluUp, 0.18, 0.19, "second jet");
    DrawLine(partUp, 0.54, 0.31, "parton corr.");
    DrawLine(qcdUp, 0.54, 0.25, "QCD background");
    DrawFill(hErr, 0.54, 0.19, "total err.");

    C->Update();

    gROOT->ProcessLine(".! mkdir eps");
    gROOT->ProcessLine(".! mkdir eps/jec_plots");
    C->SaveAs(("eps/jec_plots/" + cName + ".eps").c_str());
  }

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
  TGraph* jetUp = 0;
  TGraph* jetDown = 0;
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
    double jetup[ndiv];
    double jetdown[ndiv];

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

      err = rjet._SystPurity2ndJet(x[i], phoID);
      jetup[i] = 100 * err / P;
      jetdown[i] = -100 * err / P;
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
    jetUp = new TGraph(ndiv, x, jetup);
    jetDown = new TGraph(ndiv, x, jetdown);
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
  jetUp->SetLineStyle(kDashDotted);
  jetUp->SetLineWidth(1);
  jetDown->SetLineStyle(kDashDotted);
  jetDown->SetLineWidth(1);

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
    jetUp->Draw("L");
    jetDown->Draw("L");

    DrawLine(statUp, 0.18, 0.25, "stat.");
    DrawLine(idUp, 0.18, 0.19, "photon ID");
    DrawLine(jetUp, 0.54, 0.31, "second jet");
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
  for (int j = 0; j != ne; ++j) {
    eUp[j]->SetLineStyle(style[j]);
    eUp[j]->SetLineColor(color[j]);
    eUp[j]->SetLineWidth(1);
    eDown[j]->SetLineStyle(style[j]);
    eDown[j]->SetLineColor(color[j]);
    eDown[j]->SetLineWidth(1);
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
    HE3->SetMinimum(-0.30);
    HE3->SetMaximum(0.30);
    HE3->SetStats(kFALSE);
    HE3->GetXaxis()->SetTitle("p_{T} [GeV]");
    HE3->GetXaxis()->SetTitleOffset(1.1);
    HE3->GetXaxis()->SetMoreLogLabels(kTRUE);
    HE3->GetXaxis()->SetNoExponent(kTRUE);
    HE3->GetYaxis()->SetTitle("Background #DeltaC");
    HE3->GetYaxis()->SetTitleOffset(1.1);//0.9);
    
    HE3->Draw("AXIS");
    hErr2->Draw("SAME E4");
    hErr1->Draw("SAME HIST L");
    hErr2->Draw("SAME HIST L");
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
