#include "TF1.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TLegend.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

const int kGreenHatch = 3654;
const int kBlueHatch = 3645;

//std::string _data="QCD";
std::string _data="PhotonJet";
std::string _mode="CaloGen";
//std::string _mode="GenPart";
//std::string _mode="PartPart";
const bool _fixfit = false;//true;

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


Double_t crystal_glueball(Double_t *xx, Double_t *p) {

  double x = *xx;
  double N = p[0];
  double mu = p[1];
  double s = p[2];
  double a1 = p[3];
  double n1 = p[4];
  double a2 = p[5];
  double n2 = p[6];

  double A1 = pow(n1/fabs(a1),n1)*exp(-a1*a1/2);
  double B1 = n1/fabs(a1) - fabs(a1);

  double A2 = pow(n2/fabs(a2),n2)*exp(-a2*a2/2);
  double B2 = n2/fabs(a2) - fabs(a2);

  double f = 0;
  if (x-mu < -a1*s)
    f = N*A1*pow(B1 - (x-mu)/s, -n1);
  else if (x-mu > a2*s)
    f = N*A2*pow(B2 + (x-mu)/s, -n2);
  else
    f = N*exp(-0.5*(x-mu)*(x-mu)/(s*s));

  return f;
}
Double_t crystal_glueball_x(Double_t *xx, Double_t *p) {
  return ((*xx)*crystal_glueball(xx, p));
}
Double_t crystal_glueball_x2(Double_t *xx, Double_t *p) {
  return ((*xx)*(*xx)*crystal_glueball(xx, p));
}

Double_t D0_function(Double_t *xx, Double_t *p) {

  double x = *xx;
  double N = p[0];
  double mu = p[1];
  double s = p[2];
  double P = p[3];
  double d0 = p[4];
  
  double f = N*(P*0.5*d0 * exp(d0*(x-mu+0.5*d0*s*s))
		* TMath::Erfc((x-mu+d0*s*s)/(sqrt(2.)*s))
		+ (1-P)*TMath::Gaus(x, mu, s, kTRUE));
  
  return f;
}

std::map<int, double> _binmap;
std::map<int, double> _binnorm;
const int _nbins = 14;
int _cnt = 0;

// CaloGen QCD
const int _npars0 = 13;
const double _pars0[_npars0+_nbins] =
/*{2.891, -0.437,
   8.385, 1.722, 0.02911,
   3.056, -0.8442, 0.1726, 2.361,
   3, 1.5, 0, 10,
   150.6, 777.7, 821.4, 515.5, 857.6, 830.1, 2136, 1834, 2129, 1921, 1922, 2666, 3354, 2276};*/
  {2.724, -0.4251,
   8.58, 1.724, 0.03181,
   3.114, -0.7166, 0.1012, 2.106,
   2.351, 0.8818, 0.003455, 130.1,
   502.1, 2486, 2627, 1610, 1332, 1276, 3271, 2792, 3221, 2877, 2868, 3984, 4880, 3205};
Double_t flrj_dptvg(Double_t *x, Double_t *p) {

  if (++_cnt%100000==0) cout << "." << flush;

  int bin = int((*x)/3);
  double pt = _binmap[bin];
  double r = (*x) - 3.*bin;
  double logpt = log(0.01*pt);
  double mu = 1. - p[0]*pow(pt, p[1]);
  double sig = sqrt(p[2]*p[2]/(pt*pt) + p[3]*p[3]/pt + p[4]*p[4]);
  double a = max(p[5] + logpt*(p[6] + logpt*p[7]),0.);
  double N = p[8];
  double a2 = max(p[9] + logpt*(p[10] + logpt*p[11]),0.);
  double N2 = p[12];
  double norm = p[_npars0+bin];

  // NB: sigma is relative sigma (sigma_pT/pT) so multiply by mu
  double pp[7] = {norm, mu, sig*mu, a, N, a2, N2};
  
  return crystal_glueball(&r, &pp[0]);
}

// PartPart QCD
const int _npars1 = 11;
const double _pars1[_npars1+_nbins] =
// Chi2/NDF = 4594.8/2446 (1.9)
/*{//0.9994, 
  1.134, -1.175,
  5.981, -0.04399, 0.0003798,
  0.7324, -0.4128, 0.08791,
  48.39, -29.15, 4.721,
  49.49, 354.8, 457.6, 313.4, 556.9, 554.3, 1467, 1291, 1536, 1454, 1581, 2445, 3557, 2825};*/
// Chi2/NDF = 4471.0/2445 (1.8)
/*{0.143, -0.7982,
   6.101, 0.05604, -0.0009505,
   0.7545, -0.4273, 0.09259,
   44.44, -26.43, 4.256,
   49.19, 353.4, 457.6, 314.1, 557.1, 553.8, 1466, 1290, 1537, 1458, 1586, 2453, 3560, 2804};*/
// Chi2/NDF = 4467.8/2445 (1.8)
/*  {0.1425, -0.7974,
   6.054, 0.008743, -0.0007619,
   0.7438, -0.4193, 0.08857,
   43.3, -25.43, 4.044,
   49.45, 354.7, 459, 315.2, 559.3, 556.3, 1473, 1295, 1541, 1459, 1586, 2456, 3585, 2840};*/
// Chi2/NDF = 4466.3/2445 (1.8)
  {0.1426, -0.7977,
   6.064, 0.0157, -0.000354,
   0.7467, -0.4187, 0.08768,
   40.88, -23.57, 3.689,
   49.37, 354.3, 458.8, 315.3, 560, 557.2, 1476, 1298, 1543, 1460, 1586, 2456, 3592, 2847};
// There's still a tad bit of asymmetry of low pT, but this could be due to
// - cut on pTsec/pTgamma < 0.10, which tends to favor high pTgamma
// - approximating <x1>/<x2> with <x1/x2> => ideally replace
//   <x1/x2> with <x1/pThat> and <x2/pThat> (need to find pThat)
//   => in the absence of pThat, could use '2(x1-x2)/(x1+x2)+1'
Double_t flgq_ptop(Double_t *x, Double_t *p) {
  
  if (++_cnt%100000==0) cout << "." << flush;

  int bin = int((*x)/3);
  double pt = _binmap[bin];
  double r = (*x) - 3.*bin;
  double logpt = log(0.01*pt);
  double mu = 1 - p[0]*pow(pt, p[1]);
  double sig = sqrt(p[2]*p[2]/(pt*pt) + p[3]*p[3]/pt + p[4]*p[4]);
  double a = max(p[5] + logpt*(p[6] + logpt*p[7]),0.);
  double N = max(p[8] + logpt*(p[9] + logpt*p[10]),0.);
  double norm = p[_npars1+bin];

  // NB: sigma is relative sigma (sigma_pT/pT) so multiply by mu
  double pp[5] = {norm, mu, sig*mu, a, N};
  
  return crystal_dipole(&r, &pp[0]);
}

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

int crystalball() {

  TDirectory *curdir = gDirectory;

  map<string, string> files;
  int nbins = 0;
  if (_data=="PhotonJet") {

    nbins = 7;
    files["0030_0050"] = "output_PhotonJets/output_PhotonJets_30_50.root";
    files["0050_0080"] = "output_PhotonJets/output_PhotonJets_50_80.root";
    files["0080_0120"] = "output_PhotonJets/output_PhotonJets_80_120.root";
    files["0120_0170"] = "output_PhotonJets/output_PhotonJets_120_170.root";
    files["0170_0300"] = "output_PhotonJets/output_PhotonJets_170_300.root";
    files["0300_0500"] = "output_PhotonJets/output_PhotonJets_300_500.root";
    files["0500_7000"] = "output_PhotonJets/output_PhotonJets_500_7000.root";
  }

  //TCanvas *cfits = new TCanvas("cfits","cfits",1200,600);
  //cfits->Divide(4,2);
  if (_data=="QCD") {

    // NB: these only work in compiled mode so call mk_crystalball.C
    nbins = 14;
    files["0030_0050"] = "output_QCD/output_QCD_30_50.root";
    files["0050_0080"] = "output_QCD/output_QCD_50_80.root";
    files["0080_0120"] = "output_QCD/output_QCD_80_120.root";
    files["0120_0170"] = "output_QCD/output_QCD_120_170.root";
    files["0170_0230"] = "output_QCD/output_QCD_170_230.root";
    files["0230_0300"] = "output_QCD/output_QCD_230_300.root";
    files["0300_0380"] = "output_QCD/output_QCD_300_380.root";
    files["0380_0470"] = "output_QCD/output_QCD_380_470.root";
    files["0470_0600"] = "output_QCD/output_QCD_470_600.root";
    files["0600_0800"] = "output_QCD/output_QCD_600_800.root";
    //
    files["0800_1000"] = "output_QCD/output_QCD_800_1000.root";
    files["1000_1400"] = "output_QCD/output_QCD_1000_1400.root";
    files["1400_1800"] = "output_QCD/output_QCD_1400_1800.root";
    files["1800_2200"] = "output_QCD/output_QCD_1800_2200.root";
  // pT histogram range used to only go up to 2000, so these are not useful
  //files["2200_2600"] = "output_QCD/output_QCD_2200_2600.root";
  //files["2600_3000"] = "output_QCD/output_QCD_2600_3000.root";
  //files["3000_3500"] = "output_QCD/output_QCD_3000_3500.root";
  //files["3500_inf"] = "output_QCD/output_QCD_3500_inf.root";
  }

  //const int ncanvas = 12;
  //const int nbins = 14;
  int  ncanvas = 0;
  TCanvas *cfits1 = 0, *cfits2 = 0;
  if (_data=="QCD") {

    ncanvas = 12;
    cfits1 = new TCanvas("cfits1","cfits1",1200,900);
    cfits1->Divide(4,3);
    cfits2 = new TCanvas("cfits2","cfits2",1200,600);
    cfits2->Divide(4,2);
  }
  if (_data=="PhotonJet") {
    
    ncanvas = 9;
    cfits1 = new TCanvas("cfits1","cfits1",900,900);
    cfits1->Divide(3,3);
  }

  TCanvas *cpars = new TCanvas("cpars","cpars",1200,800);
  cpars->Divide(3,2);

  TGraphErrors *gmean = new TGraphErrors();   gmean->SetName("mean");
  TGraphErrors *grms = new TGraphErrors();    grms->SetName("rms");
  TGraphErrors *gmean2 = new TGraphErrors();  gmean2->SetName("mean2");
  TGraphErrors *grms2 = new TGraphErrors();   grms2->SetName("rms2");

  TGraphErrors *gausmu = new TGraphErrors();  gausmu->SetName("gmu");
  TGraphErrors *gaussig = new TGraphErrors(); gaussig->SetName("gsig");

  TGraphErrors *gmu = new TGraphErrors();     gmu->SetName("mu");
  TGraphErrors *gsigma = new TGraphErrors();  gsigma->SetName("sigma");
  TGraphErrors *galpha = new TGraphErrors();  galpha->SetName("alpha");
  TGraphErrors *gpower = new TGraphErrors();  gpower->SetName("power");
  TGraphErrors *galpha2 = new TGraphErrors();  galpha->SetName("alpha2");
  TGraphErrors *gpower2 = new TGraphErrors();  gpower->SetName("power2");
  TGraphErrors *gchi2 = new TGraphErrors();   gchi2->SetName("chi2");
  TGraphErrors *gchi2cb = new TGraphErrors(); gchi2cb->SetName("chi2cb");

  TGraphErrors *gmux = new TGraphErrors();    gmux->SetName("mux");
  TGraphErrors *gsigmax = new TGraphErrors(); gsigmax->SetName("sigmax");

  TGraphErrors *g2d = new TGraphErrors();

  {
  int i = 0;
  for (map<string,string>::const_iterator it = files.begin();
       it != files.end(); ++it) {

    ++i;
    TCanvas *cfits = (i <= ncanvas - 2 ? cfits1 : cfits2);
    const int j = (i <= ncanvas - 2 ? i : i-ncanvas+2);

    TFile *f = new TFile(it->second.c_str(), "READ");
    assert(!f->IsZombie());
    TH1D *h = 0;
    //TH1D *h = (TH1D*)f->Get("Pkin/lrp_dptvg");
    //TH1D *h = (TH1D*)f->Get("Jkin/lrj_dptvg");
    //TH1D *h = (TH1D*)f->Get("Ekin/lrj_dptvq");
    //TH1D *h = (TH1D*)f->Get("EkinEM/lrj_dptvg");
    //if (_mode=="CaloGen") h = (TH1D*)f->Get("EkinSB/lrj_dptvg"); //nov20
    if (_mode=="CaloGen") h = (TH1D*)f->Get("EkinEM/lrj_ptop"); //nov20
    //TH1D *h = (TH1D*)f->Get("EkinSB/lgj_dptvq");
    if (_mode=="PartPart") h = (TH1D*)f->Get("EkinSB/lgq_ptop");
    //TH1D *h = (TH1D*)f->Get("EkinSB/lrj_dptvq");
    //TH1D *h = (TH1D*)f->Get("DkinSB/lrj_dptvq"); //nov14
    //TH1D *h = (TH1D*)f->Get("Ekin/lrp_dptvg");
    //TH1D *h = (TH1D*)f->Get("Ekin/lrj_dptvg");
    //TH1D *h = (TH1D*)f->Get("Ekin/lrj_dptvq");
    //TH1D *h = (TH1D*)f->Get("Ekin/lgq_ptop");
    //TH1D *h = (TH1D*)f->Get("Ekin/lrj_ptop");
    assert(h);
    //TH1D *hpt = (TH1D*)f->Get("Jkin/lrj_pt");
    //TH1D *hpt = (TH1D*)f->Get("Ekin/lrj_pt");
    TH1D *hpt = 0;
    //if (_data=="QCD") hpt = (TH1D*)f->Get("Ekin/lrp_pt"); //nov14
    if (_data=="QCD") hpt = (TH1D*)f->Get("EkinSB/lrp_pt"); //nov14
    if (_data=="PhotonJet") hpt = (TH1D*)f->Get("EkinEM/lrp_pt");
    assert(hpt);

    curdir->cd();

    //TCanvas *c = gROOT->FindObject("c1");
    //if (!c) c = new TCanvas("c1","c1");
    //c->SetLogy();
    cfits->cd(j);
    TPad *c = (TPad*)gPad;
    c->SetLogy();

    gStyle->SetOptStat(kTRUE);
    gStyle->SetOptFit(kTRUE);

    TH1D *h2 = (TH1D*)h->Clone(Form("h2_%s",it->first.c_str()));
    TH1D *h2r = (TH1D*)h->Clone(Form("h2r_%s",it->first.c_str()));
    if (_mode=="CaloGen" && _data=="QCD") h2r->Rebin(4);
    if (_mode=="CaloGen" && i<=4) h2r->Rebin(4);

    // Set truncated range xmin, xmax
    // Be careful to set those to the rebinned bin edges so that
    // later integrals of TF1s match the range exactly
    // (adjusted in the following loop over bins)
    double xmin=0., xmax=2.;
    if (_mode=="CaloGen") {
      xmin = 5./hpt->GetMean();
      xmin = max(xmin, 0.18);
      xmin = max(xmin, hpt->GetMean()/2500.);
      xmin = min(xmin, 0.6);
      xmax = 1.07;
    }
    if (_mode=="CaloPart") {
      xmin = 5./hpt->GetMean();
      xmin = max(xmin, 0.15);
      xmin = max(xmin, hpt->GetMean()/2500.);
      xmin = min(xmin, 0.6);
      xmax = 1.07;
    }
    if (_mode=="PartPart") {
      // May loose events if CaloJet fluctuates below 5 GeV
      // Uncorr threshold ~5 GeV, corr ~15 GeV, fluctuations ~30%
      // Hmm, just seems x<0.5 is strange, but has few events
      xmin = 0.5;//30./hpt->GetMean();
      xmax = 1.5;//2.;
    }

    for (int j = 1; j != h2->GetNbinsX()+1; ++j) {

      // Drop bin if either bin edge outside range
      int jr = h2r->FindBin(h2->GetBinCenter(j));

      if (h2r->GetBinLowEdge(jr)<xmin || h2r->GetBinLowEdge(jr+1)>xmax) {

	h2->SetBinContent(j, 0.);
	h2->SetBinError(j, 0.);

	// If one bin edge within range, reduce range
	if (h2r->GetBinLowEdge(jr)>xmin && h2r->GetBinLowEdge(jr)<xmax)
	  xmax = h2r->GetBinLowEdge(jr);
	if (h2r->GetBinLowEdge(jr+1)>xmin && h2r->GetBinLowEdge(jr+1)<xmax)
	  xmin = h2r->GetBinLowEdge(jr+1);
      }
    }
    h2->SetFillStyle(kGreenHatch);
    h2->SetFillColor(kGreen+100);
    h2->SetLineColor(kGreen+100);

    h->SetName(Form("%s_%s",h->GetName(),it->first.c_str()));
    h->SetFillStyle(kBlueHatch);
    h->SetFillColor(kBlue);

    int n = gmean->GetN();
    double pt = hpt->GetMean();
    double pt_err = hpt->GetMeanError();

    h2->GetXaxis()->SetRangeUser(xmin,xmax); // needed to get mean/RMS updated
    gmean2->SetPoint(n, pt, h2->GetMean());
    gmean2->SetPointError(n, pt_err, h2->GetMeanError());
    grms2->SetPoint(n, pt, h2->GetRMS() / h2->GetMean());
    grms2->SetPointError(n, pt_err, h2->GetRMSError());
    h2->GetXaxis()->SetRangeUser(0.,3.); // ...but no need to keep that?

    if (_mode=="CaloGen" && _data=="QCD") {
      h2->Rebin(4);
      h->Rebin(4);
    }
    if (_mode=="PartPart" && i<=4) {
      h2->Rebin(2);
      h->Rebin(2);
    }
    // Drop half-filled bins to avoid messing fits
    // Should be redundant now
    /*
    for (int j = 1; j != h->GetNbinsX()+1; ++j) {
      if (h2->GetBinContent(j)!=h->GetBinContent(j)) {
	h2->SetBinContent(j, 0.);
	h2->SetBinError(j, 0.);
      }
    }
    */

    h2->Draw("hist");
    h->Draw("sames");

    // Map 2D function into 1D to make fitting work
    // (still a problem in ROOT 5? Didn't work in ROOT4)
    // Do this after rebinning to reduce number of points needed
    // for fit, and for consitency with other fits.
    for (int j = 1; j != h->GetNbinsX()+1; ++j) {

      if (h2->GetBinContent(j)!=0) {

	int n = g2d->GetN();
	double xmap = 3*(i-1) + h2->GetBinCenter(j);
	//double xmap_err = 0.5*h2->GetBinWidth(j);
	double y = h2->GetBinContent(j);
	double y_err = h2->GetBinError(j);
	g2d->SetPoint(n, xmap, y);
	//g2d->SetPointError(n, xmap_err, y_err);
	g2d->SetPointError(n, 0., y_err); // Fit can't handle xmap_err!
      }
    }

    // Set up suitable fit functions
    TF1 *fgaus = new TF1(Form("fgaus_%d",i),"gaus",0.,2.);
    TF1 *pgaus = new TF1(Form("pgaus_%d",i),"gaus",0.,2.);
    TF1 *pgaus2 = new TF1(Form("pgaus2_%d",i),"gaus",0.,2.);
    //TF1 *fd0 = new TF1(Form("fd0_%d",i),D0_function,0.,2.,5);
    //fd0->SetLineColor(kBlue);
    //TF1 *fcb = new TF1(Form("fcb_%d",i),crystal_ball,0.,2.,5);
    TF1 *fcb = 0, *fcb2 = 0, *fcb_x = 0, *fcb_x2 = 0;
    if (_mode=="CaloGen") {
      fcb = new TF1(Form("fcb_%d",i),crystal_glueball,0.,2.,7);
      fcb2 = new TF1(Form("fcb2_%d",i),crystal_glueball,0.,2.,7);
      fcb_x = new TF1(Form("fcbx_%d",i),crystal_glueball_x,0.,2.,7);
      fcb_x2 = new TF1(Form("fcbx2_%d",i),crystal_glueball_x2,0.,2.,7);
    }
    if (_mode=="PartPart") {
      fcb = new TF1(Form("fcb_%d",i),crystal_dipole,0.,2.,5);
      fcb2 = new TF1(Form("fcb2_%d",i),crystal_dipole,0.,2.,5);
      fcb_x = new TF1(Form("fcbx_%d",i),crystal_dipole_x,0.,2.,5);
      fcb_x2 = new TF1(Form("fcbx2_%d",i),crystal_dipole_x2,0.,2.,5);
    }
    fcb->SetLineColor(kRed);
    
    // Initial fits with full Gaussian and Gaussian peak
    float ns = 1.0;
    //if (_mode=="PartPart") ns = 0.5;
    h->Fit(fgaus, "QRN");
    pgaus->SetRange(fgaus->GetParameter(1)-ns*fgaus->GetParameter(2),
		    fgaus->GetParameter(1)+ns*fgaus->GetParameter(2));
    h->Fit(pgaus, "QRN");
    pgaus->SetRange(pgaus->GetParameter(1)-ns*pgaus->GetParameter(2),
		    pgaus->GetParameter(1)+ns*pgaus->GetParameter(2));
    h->Fit(pgaus, "QR", "sames");
    // Could also use QR0 to not draw at this stage, then later
    // h->GetFunction("pgaus_%d")->ResetBit(kNowDraw);
    // h->Draw();
    
    pgaus2->SetParameters(pgaus->GetParameter(0),pgaus->GetParameter(1),
			  pgaus->GetParameter(2));
    pgaus2->SetLineWidth(2);
    pgaus2->SetLineStyle(kDashed);
    //pgaus2->Draw("same");
    h->GetListOfFunctions()->Add(pgaus2);

    /*
    fd0->SetParameters(fgaus->GetParameter(0),fgaus->GetParameter(1),
		       //fgaus->GetParameter(2), 0.1, 10.);
		       fgaus->GetParameter(2), 0.5, 10.);
    h->Fit(fd0, "QR+");
    */    

    // Initialize tails of the Crystal Ball fit
    if (_mode=="CaloGen") {

      TF1 *fm = new TF1("fm","1-[0]*pow(x,[1])",0.,10000.);
      fm->SetParameters(_pars0[0],_pars0[1]);
      TF1 *fs = new TF1("fs","sqrt([0]*[0]/(x*x)+[1]*[1]/x+[2]*[2])",
			0,10000.);
      fs->SetParameters(_pars0[2],_pars0[3],_pars0[4]);
      TF1 *fa = new TF1("fa","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			0.,10000.);
      fa->SetParameters(_pars0[5],_pars0[6],_pars0[7]);
      TF1 *fa2 = new TF1("fa2","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			 0.,10000.);
      fa2->SetParameters(_pars0[9],_pars0[10],_pars0[11]);
      double mu = fm->Eval(hpt->GetMean());
      double sig = fs->Eval(hpt->GetMean());
      double alpha = fa->Eval(hpt->GetMean());
      double npow = _pars0[8];//16.; // qcd parton
      double alpha2 = fa2->Eval(hpt->GetMean());
      double npow2 = _pars0[12];
      fcb->SetParameters(pgaus->GetParameter(0),pgaus->GetParameter(1),
			 pgaus->GetParameter(2), alpha, npow, alpha2, npow2);
      fcb->SetParLimits(4,2.,30.);
      fcb->SetRange(xmin, xmax);
      if (_fixfit) {
	fcb->FixParameter(1, mu);
	fcb->FixParameter(2, sig*mu);
	fcb->FixParameter(3, alpha);
	fcb->FixParameter(4, npow);
      }
      fcb->FixParameter(5, alpha2);
      fcb->FixParameter(6, npow2);
    }
    if (_mode=="PartPart") {

      TF1 *fm = new TF1("fm","1-[0]*pow(x,[1])",0.,10000.);
      fm->SetParameters(_pars1[0],_pars1[1]);
      TF1 *fs = new TF1("fs","sqrt([0]*[0]/(x*x)+[1]*[1]/x+[2]*[2])",
			0,10000.);
      fs->SetParameters(_pars1[2],_pars1[3],_pars1[4]);
      TF1 *fa = new TF1("fa","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			0.,10000.);
      fa->SetParameters(_pars1[5],_pars1[6],_pars1[7]);
      TF1 *fn = new TF1("fa","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			0.,10000.);
      fn->SetParameters(_pars1[8],_pars1[9],_pars1[10]);

      double mu = fm->Eval(hpt->GetMean());
      double sig = fs->Eval(hpt->GetMean());
      double alpha = fa->Eval(hpt->GetMean());
      double npow = fn->Eval(hpt->GetMean());
      fcb->SetParameters(pgaus->GetParameter(0),pgaus->GetParameter(1),
			 pgaus->GetParameter(2), alpha, npow);
      fcb->SetRange(xmin, xmax);
      if (_fixfit) {
	fcb->FixParameter(1, mu);
	fcb->FixParameter(2, sig*mu);
	fcb->FixParameter(3, alpha);
	fcb->FixParameter(4, npow);
      }
    }
    h->Fit(fcb, "QR+","sames");

    // Store mapping from index to pt and the
    // normalization for the Crystal Ball fit
    _binmap[i-1] = int(hpt->GetMean());
    _binnorm[i-1] = fcb->GetParameter(0);
    cout << Form("\nBin %d (pt%d): norm = %1.4g",
		 i, int(hpt->GetMean()), fcb->GetParameter(0)) << endl;

    // Copy fit parameters for a full range Crystal Ball and
    // two integrands for plotting and later arithmetic calculations
    for (int j = 0; j != fcb->GetNpar(); ++j) {
      fcb2->SetParameter(j, fcb->GetParameter(j));
      fcb_x->SetParameter(j, fcb->GetParameter(j));
      fcb_x2->SetParameter(j, fcb->GetParameter(j));
    }
    fcb2->SetLineWidth(2);
    fcb2->SetLineStyle(kDotted);
    fcb2->SetLineColor(kRed);
    h->GetListOfFunctions()->Add(fcb2);

    
    // Print out some diagnostics about fits (means, widths, normalization)
    cout << endl;
    cout << "Gaussian (full):" << endl;
    cout << "----------------" << endl;
    cout << Form("Chi2/NDF = %6.2f / %2d (%5.2f)",
		 fgaus->GetChisquare(),fgaus->GetNDF(),
		 fgaus->GetChisquare() / fgaus->GetNDF()) << endl;
    cout << Form("Mean = %5.3f;  Sigma = %6.4f;  Norm = %6.4g",
		 fgaus->GetParameter(1), fgaus->GetParameter(2),
		 fgaus->GetParameter(0)) << endl;
    
    cout << endl;
    cout << Form("Gaussian (peak +/- %1.1f*sigma):",ns) << endl;
    cout << "----------------" << endl;
    cout << Form("Chi2/NDF = %6.2f / %2d (%5.2f)",
		 pgaus->GetChisquare(),pgaus->GetNDF(),
		 pgaus->GetChisquare() / pgaus->GetNDF()) << endl;
    cout << Form("Mean = %5.3f;  Sigma = %6.4f;  Norm = %6.4g",
		 pgaus->GetParameter(1), pgaus->GetParameter(2),
		 pgaus->GetParameter(0)) << endl;
    
    /*
    cout << endl;
    cout << "D0-function:" << endl;
    cout << "----------------" << endl;
    cout << Form("Chi2/NDF = %6.2f / %2d (%5.2f)",
		 fd0->GetChisquare(), fd0->GetNDF(),
		 fd0->GetChisquare() / fd0->GetNDF()) << endl;
    cout << Form("Mean = %5.3f;  Sigma = %6.4f",
		 fd0->GetParameter(1), fd0->GetParameter(2)) << endl;
    */

    cout << endl;
    cout << "Crystal Ball:" << endl;
    cout << "----------------" << endl;
    cout << Form("Chi2/NDF = %6.2f / %2d (%5.2f)",
		 fcb->GetChisquare(), fcb->GetNDF(),
		 fcb->GetChisquare() / fcb->GetNDF()) << endl;
    cout << Form("Mean = %5.3f;  Sigma = %6.4f;  Norm = %6.4g",
		 fcb->GetParameter(1), fcb->GetParameter(2),
		 fcb->GetParameter(0)) << endl;

    // Store means, widths, chi2/ndf to graphs for later fits
    gmean->SetPoint(n, pt, h->GetMean());
    gmean->SetPointError(n, pt_err, h->GetMeanError());
    grms->SetPoint(n, pt, h->GetRMS() / h->GetMean());
    grms->SetPointError(n, pt_err, h->GetRMSError() / h->GetMean());

    gausmu->SetPoint(n, pt, pgaus->GetParameter(1));
    gausmu->SetPointError(n, pt_err, pgaus->GetParError(1));
    gaussig->SetPoint(n, pt, pgaus->GetParameter(2)/pgaus->GetParameter(1));
    gaussig->SetPointError(n, pt_err, pgaus->GetParError(2)
			   / pgaus->GetParameter(1));

    gmu->SetPoint(n, pt, fcb->GetParameter(1));
    gmu->SetPointError(n, pt_err, max(fcb->GetParError(1),0.0001));
    gsigma->SetPoint(n, pt, fcb->GetParameter(2) / fcb->GetParameter(1));
    gsigma->SetPointError(n, pt_err, fcb->GetParError(2)
			  / fcb->GetParameter(1));
    galpha->SetPoint(n, pt, fcb->GetParameter(3));
    galpha->SetPointError(n, pt_err, max(fcb->GetParError(3),0.01));
    galpha2->SetPoint(n, pt, fcb->GetParameter(5));
    galpha2->SetPointError(n, pt_err, max(fcb->GetParError(5),0.01));
    gpower->SetPoint(n, pt, fcb->GetParameter(4));
    gpower->SetPointError(n, pt_err, fcb->GetParError(4));
    gpower2->SetPoint(n, pt, fcb->GetParameter(6));
    gpower2->SetPointError(n, pt_err, fcb->GetParError(6));
    gchi2->SetPoint(n, pt, pgaus->GetChisquare()/pgaus->GetNDF());
    gchi2->SetPointError(n, pt_err, sqrt(2./pgaus->GetNDF()));
    gchi2cb->SetPoint(n, pt, fcb->GetChisquare()/fcb->GetNDF());
    gchi2cb->SetPointError(n, pt_err, sqrt(2./fcb->GetNDF()));

    // Calculate arithmetic mean and RMS from Crystal Ball fit
    // to compare to true arithmetic mean and RMS (in truncated range)
    double nx = fcb->Integral(xmin,xmax);
    double mux = fcb_x->Integral(xmin,xmax)/nx;
    double mux2 = fcb_x2->Integral(xmin,xmax)/nx;
    gmux->SetPoint(n, pt, mux);
    gmux->SetPointError(n, pt_err, max(fcb->GetParError(1),0.0001));
    gsigmax->SetPoint(n, pt, sqrt(mux2 - mux*mux) / mux);
    gsigmax->SetPointError(n, pt_err, fcb->GetParError(2) / mux);

    // Use an empty pad for a big legend for all pads
    if (i == ncanvas-2 || i == nbins) {

      cfits->cd(j+1);

      TLegend *leg = 0;
      if (_mode=="CaloGen")
	leg =  new TLegend(0.17,0.15,0.97,0.95,
			   "CaloJet p_{T} / PartonJet p_{T}","brNDC");
      if (_mode=="PartPart")
	leg =  new TLegend(0.17,0.15,0.97,0.95,
			   "Jet parton p_{T} / Photon parton p_{T}","brNDC");
      leg->SetFillStyle(kNone);
      leg->AddEntry(pgaus, "Gaussian peak fit #pm#sigma", "L");
      leg->AddEntry(pgaus2, "Gaussian extension", "L");
      leg->AddEntry(fcb, "Crystal Ball fit", "L");
      leg->AddEntry(fcb2, "Crystal Ball extension", "L");
      leg->AddEntry(h, "Full range", "F");
      leg->AddEntry(h2, "Truncated range", "F");
      leg->Draw();

      cfits->cd(j+2);
      gPad->SetLogx();

      TGraphErrors *g = (TGraphErrors*)gchi2->Clone("chi2_1");
      g->Draw("AP");
      g->GetXaxis()->SetTitle("p_{T} (GeV)");
      g->GetXaxis()->SetMoreLogLabels();
      g->GetXaxis()->SetNoExponent();
      g->GetYaxis()->SetTitle("#chi^{2} / NDF");
      g->GetYaxis()->SetRangeUser(0.,5.);

      TGraphErrors *g2 = (TGraphErrors*)gchi2cb->Clone("chi2cb_1");
      g2->SetLineColor(kRed);
      g2->Draw("SAME");
      
      cfits->cd(0);
    }

    //h2->Draw("hsame");
  } // for
  } // int i
  gaussig->GetXaxis()->SetRangeUser(30.,1000.);

  //cfits->cd(0);
  curdir->cd();


  // Power law fit of the mean
  TF1 *fpow1 = new TF1("fpow1","1-[0]*pow(x,[1]-1)",30.,2000.);
  fpow1->SetParameters(2.0, 0.6);
  fpow1->SetLineColor(kBlack);
  fpow1->SetLineWidth(1);
  TF1 *fpow2 = new TF1("fpow2","1-[0]*pow(x,[1]-1)",30.,2000.);
  fpow2->SetParameters(2.0, 0.6);
  fpow2->SetLineColor(kGreen+100);
  fpow2->SetLineWidth(1);
  TF1 *fpow3 = new TF1("fpow3","1-[0]*pow(x,[1]-1)",30.,2000.);
  fpow3->SetParameters(2.0, 0.6);
  fpow3->SetLineColor(kRed);
  fpow3->SetLineWidth(1);

  cpars->cd(1);
  gPad->SetLogx();

  gStyle->SetOptStat(0);
  TH1D *h1 = new TH1D("h1","",2000,0.,5000.);
  h1->Draw();
  h1->GetXaxis()->SetTitle("p_{T} (GeV)");
  h1->GetXaxis()->SetMoreLogLabels();
  h1->GetXaxis()->SetNoExponent();
  h1->GetXaxis()->SetRangeUser(30.,3000.);
  h1->GetYaxis()->SetTitle("mean #mu");
  if (_mode=="CaloGen") h1->GetYaxis()->SetRangeUser(0.3,1.05);
  if (_mode=="PartPart") h1->GetYaxis()->SetRangeUser(0.95,1.05);

  gausmu->Fit(fpow1,"QR");
  gausmu->Draw("P");
  gausmu->SetLineColor(kBlack);
  gausmu->SetLineWidth(2);
  gmean->SetLineColor(kBlue);
  gmean->SetLineWidth(2);
  gmean->Draw("P");
  gmux->Draw("P");
  gmux->SetLineColor(kRed);
  gmux->SetMarkerColor(kRed);
  gmux->SetMarkerStyle(kCircle);
  gmean2->Fit(fpow2,"QR");
  gmean2->Draw("P");
  gmean2->SetLineColor(kGreen+100);
  gmean2->SetLineWidth(2);
  gmu->Fit(fpow3, (_fixfit  ? "QRW" : "QR"));
  gmu->Draw("PSAMES");
  gmu->SetLineColor(kRed);
  gmu->SetLineWidth(2);


  // NSC fits of the widths
  TF1 *fsig1 = new TF1("fsig1","sqrt([0]*[0]/(x*x)+[1]*[1]/x+[2]*[2])",
		       30., 2000.);
  fsig1->SetParameters(1.,1.,0.05);
  fsig1->SetLineColor(kBlack);
  fsig1->SetLineWidth(1);
  TF1 *fsig2 = new TF1("fsig2","sqrt([0]*[0]/(x*x)+[1]*[1]/x+[2]*[2])",
		       30., 2000.);
  fsig2->SetParameters(1.,1.,0.05);
  fsig2->SetLineColor(kGreen+100);
  fsig2->SetLineWidth(1);
  TF1 *fsig3 = new TF1("fsig3","sqrt([0]*[0]/(x*x)+[1]*[1]/x+[2]*[2])",
		       30., 2000.);
  if (_mode=="CaloGen") fsig3->SetParameters(1.,1.,0.05);
  if (_mode=="PartPart") fsig3->SetParameters(6., 0.01, 0.);
  fsig3->SetLineColor(kRed);
  fsig3->SetLineWidth(1);

  cpars->cd(2);
  gPad->SetLogx();

  TH1D *h2 = new TH1D("h2","",2000,0.,5000.);
  h2->Draw();
  h2->GetXaxis()->SetTitle("p_{T} (GeV)");
  h2->GetXaxis()->SetMoreLogLabels();
  h2->GetXaxis()->SetNoExponent();
  h2->GetXaxis()->SetRangeUser(30.,3000.);
  h2->GetYaxis()->SetTitle("width (#sigma / #mu)");
  if (_mode=="CaloGen") h2->GetYaxis()->SetRangeUser(0.,0.5);
  if (_mode=="PartPart") h2->GetYaxis()->SetRangeUser(0.,0.25);

  gaussig->Fit(fsig1, "QR");
  gaussig->Draw("P");
  gaussig->SetLineColor(kBlack);
  gaussig->SetLineWidth(2);
  grms->Draw("P");
  grms->SetLineColor(kBlue);
  grms->SetLineWidth(2);
  gsigmax->Draw("P");
  gsigmax->SetLineColor(kRed);
  gsigmax->SetMarkerStyle(kCircle);
  gsigmax->SetMarkerColor(kRed);
  grms2->Draw("P");
  grms2->Fit(fsig2, "QR");
  grms2->SetLineColor(kGreen+100);
  grms2->SetLineWidth(2);
  gsigma->Fit(fsig3, (_fixfit ? "QRW" : "QR"));
  gsigma->Draw("PSAMES");
  gsigma->SetLineColor(kRed);
  gsigma->SetLineWidth(2);

  
  // Quadratic log fits of alpha parameter
  TF1 *falpha = new TF1("falpha","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			0.,10000.);
  falpha->SetParameters(1.683, -0.3506, 0.03486);

  TF1 *falpha2 = new TF1("falpha2","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			 0.,10000.);
  falpha2->SetParameters(1.683, 0.3506, -0.03486);
  falpha2->SetLineColor(kBlue);

  cpars->cd(3);
  gPad->SetLogx();

  galpha->Draw("AP");
  galpha->GetXaxis()->SetTitle("p_{T} (GeV)");
  galpha->GetXaxis()->SetNoExponent();
  galpha->GetXaxis()->SetMoreLogLabels();
  galpha->GetYaxis()->SetTitle("threshold #alpha ( (#bar{x}-x_{0})/#sigma )");
  if (_mode=="CaloGen") galpha->GetYaxis()->SetRangeUser(0.,5.);
  if (_mode=="PartPart") galpha->GetYaxis()->SetRangeUser(0.,2.5);

  galpha2->SetLineColor(kBlue);
  galpha2->Draw("SAMEP");

  galpha->Fit(falpha, (_fixfit ? "QRW" : "QR"));
  galpha2->Fit(falpha2, (_fixfit ? "QRW" : "QR"));


  // Single constant fits of the tail power parameter
  TF1 *fpower=0, *fpower2=0;
  if (_mode=="CalGen") {

    fpower = new TF1("fpower","[0]",0.,10000.);
    fpower->SetParameter(0,16.);
    fpower2 = new TF1("fpower2","[0]",0.,10000.);
    fpower2->SetParameter(0,16.);
    fpower2->SetLineColor(kBlue);
  }
  if (_mode=="PartPart") {
    fpower = new TF1("fpower","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
		     0.,10000.);
    fpower->SetParameters(40., -20., 4.);
  }
  
  cpars->cd(6);
  gPad->SetLogx();

  gpower->Draw("AP");
  gpower->GetXaxis()->SetTitle("p_{T} (GeV)");
  gpower->GetXaxis()->SetNoExponent();
  gpower->GetXaxis()->SetMoreLogLabels();
  gpower->GetYaxis()->SetTitle("power n ( [(x-#bar{x})/#sigma]^{-n} )");
  if (_mode=="CaloGen") gpower->GetYaxis()->SetRangeUser(0.,30.);
  if (_mode=="PartPart") gpower->GetYaxis()->SetRangeUser(0.,100.);

  gpower2->SetLineColor(kBlue);
  gpower2->Draw("SAMEP");

  gpower->Fit(fpower, (_fixfit ? "QRW" : "QR"));
  if (_mode=="CalGen") gpower2->Fit(fpower2, (_fixfit ? "QRW" : "QR"));

  
  // Ratio of means and the fit to Crystal Ball mu
  cpars->cd(4);
  gPad->SetLogx();

  TH1D *h4 = new TH1D("h4","",2000,0.,5000.);
  h4->Draw();
  h4->GetXaxis()->SetTitle("p_{T} (GeV)");
  h4->GetXaxis()->SetMoreLogLabels();
  h4->GetXaxis()->SetNoExponent();
  h4->GetXaxis()->SetRangeUser(30.,3000.);
  h4->GetYaxis()->SetTitle("mean #mu over fit");
  if (_mode=="CaloGen") h4->GetYaxis()->SetRangeUser(0.9,1.1);
  if (_mode=="PartPart") h4->GetYaxis()->SetRangeUser(0.95,1.05);

  TGraphErrors *gausmu_r = divide(gausmu, fpow3); gausmu_r->SetName("gmu_r");
  TGraphErrors *gmean_r = divide(gmean, fpow3);   gmean_r->SetName("mean_r");
  TGraphErrors *gmu_r = divide(gmu, fpow3);       gmu_r->SetName("mu_r");
  TGraphErrors *gmean2_r = divide(gmean2, fpow3); gmean2_r->SetName("mean2_r");
  TGraphErrors *gmux_r = divide(gmux, fpow3);     gmux_r->SetName("mux_r");

  gausmu_r->Draw("p");
  gausmu->GetYaxis()->SetTitle("mean #mu over fit");
  gausmu_r->GetYaxis()->SetRangeUser(0.9,1.1);
  gmean_r->Draw("p");
  gmu_r->Draw("p");
  gmux_r->Draw("p");
  gmean2_r->Draw("p");

  
  // Ratio of widths and the fit to Crystal Ball sigma
  cpars->cd(5);
  gPad->SetLogx();

  TH1D *h5 = new TH1D("h5","",2000,0.,5000.);
  h5->Draw();
  h5->GetXaxis()->SetTitle("p_{T} (GeV)");
  h5->GetXaxis()->SetMoreLogLabels();
  h5->GetXaxis()->SetNoExponent();
  h5->GetXaxis()->SetRangeUser(30.,3000.);
  h5->GetYaxis()->SetTitle("width (#sigma / #mu) over fit");
  h5->GetYaxis()->SetRangeUser(0.7,1.6);

  TGraphErrors *gsig_r = divide(gaussig, fsig3);  gsig_r->SetName("gsig_r");
  TGraphErrors *grms_r = divide(grms, fsig3);     grms_r->SetName("rms_r");
  TGraphErrors *gsigma_r = divide(gsigma, fsig3); gsigma_r->SetName("sigma_r");
  TGraphErrors *grms2_r = divide(grms2, fsig3);   grms2_r->SetName("rms2_r");
  TGraphErrors *gsigx_r = divide(gsigmax, fsig3); gsigx_r->SetName("sigx_r");

  gsig_r->Draw("p");
  grms_r->Draw("p");
  gsigma_r->Draw("p");
  gsigx_r->Draw("p");
  grms2_r->Draw("p");


  // Optional legend to print in one of the pads
  // but space is now taken by the tail power parameter
  cpars->cd(6);
  TLegend *leg = new TLegend(0.2,0.2,0.8,0.8,
			     "CaloJet p_{T} / PartonJet p_{T}","brNDC");
  leg->AddEntry(gaussig, "Gaussian #pm#sigma", "LP");
  leg->AddEntry(gsigma, "Crystal Ball", "LP");
  leg->AddEntry(grms, "Arithmetic", "LP");
  leg->AddEntry(grms2, "Truncated arithmetic", "LP");
  /*
  leg->AddEntry(pgaus, "Gaussian peak fit #pm#sigma", "L");
  leg->AddEntry(pgaus2, "Gaussian extension", "L");
  leg->AddEntry(fcb, "Crystal Ball fit", "L");
  leg->AddEntry(fcb2, "Crystal Ball extension", "L");
  leg->AddEntry(h, "Full range", "F");
  leg->AddEntry(h2, "Truncated range", "F");
  leg->Draw();
  */  

  cpars->cd(0);

  gROOT->ProcessLine(".! mkdir eps");
  gROOT->ProcessLine(".! mkdir eps/crystal");
  /*
  cfits1->SaveAs("eps/crystal/cfits1.eps");
  cfits2->SaveAs("eps/crystal/cfits2.eps");
  cpars->SaveAs("eps/crystal/cpars.eps");
  */
  if (_mode=="CaloGen") {
    cfits1->SaveAs("eps/crystal/djlrjvg_cfits1.eps");
    cfits2->SaveAs("eps/crystal/djlrjvg_cfits2.eps");
    cpars->SaveAs("eps/crystal/djlrjvg_cpars.eps");
  }
  if (_mode=="CaloPart") {
    cfits1->SaveAs("eps/crystal/djlrjvq_cfits1.eps");
    cfits2->SaveAs("eps/crystal/djlrjvq_cfits2.eps");
    cpars->SaveAs("eps/crystal/djlrjvq_cpars.eps");
  }
  if (_mode=="PartPart") {
    cfits1->SaveAs("eps/crystal/djlgqvq_cfits1.eps");
    cfits2->SaveAs("eps/crystal/djlgqvq_cfits2.eps");
    cpars->SaveAs("eps/crystal/djlgqvq_cpars.eps");
  }

  // Ok, we've got fits to all the parameters now. Let's do a global
  // fit to all the parameters at once
  TF1 *f2d = 0;
  int npars = 0;
  if (_mode=="CaloGen") {
    npars = _npars0 + _nbins;
    f2d = new TF1("f2d", flrj_dptvg, 0., 300., npars);
    if (false) {
      f2d->SetParameter(0, fpow3->GetParameter(0));
      f2d->SetParameter(1, fpow3->GetParameter(1)-1);
      f2d->SetParameter(2, fsig3->GetParameter(0));
      f2d->SetParameter(3, fsig3->GetParameter(1));
      f2d->SetParameter(4, fsig3->GetParameter(2));
      f2d->SetParameter(5, falpha->GetParameter(0));
      f2d->SetParameter(6, falpha->GetParameter(1));
      f2d->SetParameter(7, falpha->GetParameter(2));
      f2d->SetParameter(8, fpower->GetParameter(0));
      f2d->SetParameter(9, 3.);
      f2d->SetParameter(10, 1.);
      f2d->SetParameter(11, -0.2);
      f2d->SetParameter(12, 10.);
      for (int i = 0; i != _nbins; ++i) {
	f2d->SetParameter(_npars0+i, _binnorm[i]);
      }
    }
    else {
      for (int i = 0; i != _npars0; ++i) {
	f2d->SetParameter(i, _pars0[i]);
      }
      for (int i = 0; i != _nbins; ++i) {
	f2d->SetParameter(_npars0+i, _pars0[_npars0+i]);
      }
    }
  } // "CaloGen"
  if (_mode=="PartPart") {
    npars = _npars1 + _nbins;
    f2d = new TF1("f2d", flgq_ptop, 0., 300., npars);
    if (false) {
      f2d->SetParameter(0, 1.);
      f2d->SetParameter(0, -1.);
      f2d->SetParameter(1, 4.463);
      f2d->SetParameter(2, 0.4841);
      f2d->SetParameter(3, -0.009117);
      f2d->SetParameter(4, falpha->GetParameter(0));
      f2d->SetParameter(5, falpha->GetParameter(1));
      f2d->SetParameter(6, falpha->GetParameter(2));
      f2d->SetParameter(7, fpower->GetParameter(0));
      for (int i = 0; i != _nbins; ++i) {
	f2d->SetParameter(_npars1+i, _binnorm[i]);
      }
    }
    else {
      for (int i = 0; i != _npars1; ++i) {
	f2d->SetParameter(i, _pars1[i]);
      }
      for (int i = 0; i != _nbins; ++i) {
	f2d->SetParameter(_npars1+i, _pars1[_npars1+i]);
      }
    }
  } // "PartPart"

  // The big fit, may take some time
  gStyle->SetOptFit(100);
  g2d->Fit(f2d, "QR");

  cout << "Global fit:" << endl;
  cout << "-----------" << endl;
  cout << Form("Chi2/NDF = %1.1f/%d (%1.2g)\n",
	       f2d->GetChisquare(),f2d->GetNDF(),
	       f2d->GetChisquare()/f2d->GetNDF());
  if (_mode=="CaloGen") {
    cout << Form("mu = 1 - %1.4g*pow(pt,%1.4g)\n",
		 f2d->GetParameter(0),f2d->GetParameter(1));
    cout << Form("sig = sqrt(sqr(%1.4g/pt)+sqr(%1.4g)/pt + sqr(%1.4g))\n",
		 f2d->GetParameter(2),f2d->GetParameter(3),
		 f2d->GetParameter(4));
    cout << Form("alpha = %1.4g + logx*(%1.4g + logx*%1.4g)\n",
		 f2d->GetParameter(5),f2d->GetParameter(6),
		 f2d->GetParameter(7));
    cout << Form("power = %1.4g\n", f2d->GetParameter(8));
    cout << Form("alpha2 = %1.4g + logx*(%1.4g + logx*%1.4g)\n",
		 f2d->GetParameter(9),f2d->GetParameter(10),
		 f2d->GetParameter(11));
    cout << Form("power2 = %1.4g\n",f2d->GetParameter(12));
  }
  if (_mode=="PartPart") {
    cout << Form("mu = 1 - %1.4g*pow(pt,%1.4g)\n",
		 f2d->GetParameter(0),f2d->GetParameter(1));
    cout << Form("sig = sqrt(sqr(%1.4g/pt)+sqr(%1.4g)/pt + sqr(%1.4g))\n",
		 f2d->GetParameter(2),f2d->GetParameter(3),
		 f2d->GetParameter(4));
    cout << Form("alpha = %1.4g + logx*(%1.4g + logx*%1.4g)\n",
		 f2d->GetParameter(5),f2d->GetParameter(6),
		 f2d->GetParameter(7));
    //cout << Form("power = %1.4g\n", f2d->GetParameter(7));
    cout << Form("npow = %1.4g + logx*(%1.4g + logx*%1.4g)\n",
		 f2d->GetParameter(8),f2d->GetParameter(9),
		 f2d->GetParameter(10));
  }
  cout << endl << flush;

  // Print out a table of parameters
  for (int i = 0; i != npars; ++i) {
    cout << Form("%1.4g%s", f2d->GetParameter(i),
		 (i==npars-1 ? ";" : ", "));
    if (_mode=="CaloGen"
	&& (i==1 || i==4 || i==8 || i==12 || i==npars-1) ||
	_mode=="PartPart"
	&& (i==1 || i==4 || i==7 || i==10 || i==npars-1))
      cout << endl;
  }
  

  // Finally, draw the global graph and global fit for easy checking
  TCanvas *c4 = new TCanvas("c4","c4");
  g2d->Draw("AP");
  f2d->SetLineColor(kRed);
  f2d->SetNpx(10000);
  //f2d->Draw("same"); // already attached to g2d


  return 0;
}


// CaloGen for QCD, crystal_glueball:
// alphas quadlog, powers const 
// slightly questionable if fit fully converged for upper tail
/*
// Chi2/NDF = 825.5/508 (1.6)
const double _pars0[_npars+_nbins] =
  {2.891, -0.437,
   8.385, 1.722, 0.02911,
   3.056, -0.8442, 0.1726, 2.361,
   3, 1.5, 0, 10,
   150.6, 777.7, 821.4, 515.5, 857.6, 830.1, 2136, 1834, 2129, 1921, 1922, 2666, 3354, 2276};
*/
