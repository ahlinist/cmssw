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

using namespace std;

void doRatio(const TGraphErrors *a, const TGraphErrors *b, TGraphErrors *c,
	     double xmatch=0.1);

int mcfactors(vector<string> sigfiles, vector<string> bkgfiles,
	      const string idtype) {

  TDirectory *curdir = gDirectory;
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);

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

  // Parton to genjet correction mapped vs ptgen (!ptgamma)
  TGraphErrors *gkjets = new TGraphErrors(); gkjets->SetName("gkjets");
  TGraphErrors *gkjetb = new TGraphErrors(); gkjetb->SetName("gkjetb");
  TGraphErrors *gkjetr = new TGraphErrors(); gkjetr->SetName("gkjetr");

  // reco vs parton correction mapped vs ptgen (!ptgamma)
  TGraphErrors *grkjets = new TGraphErrors(); grkjets->SetName("grkjets");
  TGraphErrors *grkjetb = new TGraphErrors(); grkjetb->SetName("grkjetb");
  TGraphErrors *grkjetr = new TGraphErrors(); grkjetr->SetName("grkjetr");


  // Parton to photon correction mapped vs ptgamma
  TGraphErrors *grphos = new TGraphErrors(); grphos->SetName("grphos");
  TGraphErrors *grphob = new TGraphErrors(); grphob->SetName("grphob");
  TGraphErrors *grphor = new TGraphErrors(); grphor->SetName("grphor");

  // Parton to genphoton correction mapped vs ptgamma
  TGraphErrors *gkphos = new TGraphErrors(); gkphos->SetName("gkphos");
  TGraphErrors *gkphob = new TGraphErrors(); gkphob->SetName("gkphob");
  TGraphErrors *gkphor = new TGraphErrors(); gkphor->SetName("gkphor");

  // Reco gamma vs parton correction mapped vs ptgamma
  TGraphErrors *grkphos = new TGraphErrors(); grkphos->SetName("grkphos");
  TGraphErrors *grkphob = new TGraphErrors(); grkphob->SetName("grkphob");
  TGraphErrors *grkphor = new TGraphErrors(); grkphor->SetName("grkphor");


  // Direct method imbalance pTgenjet/pTrecogamma vs ptgamma
  TGraphErrors *gkgams = new TGraphErrors(); gkgams->SetName("gkgams");
  TGraphErrors *gkgamb = new TGraphErrors(); gkgamb->SetName("gkgamb");
  TGraphErrors *gkgamr = new TGraphErrors(); gkgamr->SetName("gkgamr");

  // Parton to genjet correction mapped vs ptgen (!ptgamma)
  TGraphErrors *gktopos = new TGraphErrors(); gktopos->SetName("gktopos");
  TGraphErrors *gktopob = new TGraphErrors(); gktopob->SetName("gktopob");
  TGraphErrors *gktopor = new TGraphErrors(); gktopor->SetName("gktopor");

  // Parton to genjet correction mapped vs ptgamma
  TGraphErrors *grmeass = new TGraphErrors(); grmeass->SetName("grmeass");
  TGraphErrors *grmeasb = new TGraphErrors(); grmeasb->SetName("grmeasb");
  TGraphErrors *grmeasr = new TGraphErrors(); grmeasr->SetName("grmeasr");


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
    }

    // Genjet vs parton correction mapped vs ptgen

     dir = (i < sigfiles.size() ? dirs : dirb);
     TH1D *hkjet = (TH1D*)f->Get((dir+"/lgj_dptvq").c_str());

     assert(hkjet);

     // Replace with Gaussian fit later
     double kjet = hkjet->GetMean();
     double kjet_err = max(0.05,hkjet->GetRMS())
       / sqrt(max(1.,hkjet->Integral()));

     if (kjet && ptphot && ptgen) {
       TGraphErrors *gkjet = (i < sigfiles.size() ? gkjets : gkjetb);
       int n = gkjet->GetN();
       gkjet->SetPoint(n, ptgen, kjet);
       gkjet->SetPointError(n, ptgen_err, kjet_err);
     }

    // Reco jet vs parton correction mapped vs ptgen

     dir = (i < sigfiles.size() ? dirs : dirb);
     TH1D *hrkjet = (TH1D*)f->Get((dir+"/lrj_dptvq").c_str());

     assert(hrkjet);

     // Replace with Gaussian fit later
     double rkjet = hrkjet->GetMean();
     double rkjet_err = max(0.05,hrkjet->GetRMS())
       / sqrt(max(1.,hrkjet->Integral()));

     if (rkjet && ptphot && ptgen) {
       TGraphErrors *grkjet = (i < sigfiles.size() ? grkjets : grkjetb);
       int n = grkjet->GetN();
       grkjet->SetPoint(n, ptgen, rkjet);
       grkjet->SetPointError(n, ptgen_err, rkjet_err);
     }


     // Reco photon vs genphoton correction mapped vs ptgamma

     dir = (i < sigfiles.size() ? dirsp : dirbp);
     TH1D *hrpho = (TH1D*)f->Get((dir+"/lrp_dptvgg").c_str());

     if (hrpho) {

       // Replace with Gaussian fit later
       double rpho = hrpho->GetMean();
       double rpho_err = max(0.05,hrpho->GetRMS())
	 / sqrt(max(1.,hrpho->Integral()));
       
       if (rpho && ptphot) {
	 TGraphErrors *grpho = (i < sigfiles.size() ? grphos : grphob);
	 int n = grpho->GetN();
	 grpho->SetPoint(n, ptphot, rpho);
	 grpho->SetPointError(n, ptphot_err, rpho_err);
       }
     } // if hrpho

    // Genphoton vs parton correction mapped vs ptgamma

     dir = (i < sigfiles.size() ? dirsp : dirbp); // needed?
     TH1D *hkpho = (TH1D*)f->Get((dir+"/lgg_dptvg").c_str());

     if (hkpho) {

       // Replace with Gaussian fit later
       double kpho = hkpho->GetMean();
       double kpho_err = max(0.05,hkpho->GetRMS())
	 / sqrt(max(1.,hkpho->Integral()));
       
       if (kpho && ptphot && ptgen) {
	 TGraphErrors *gkpho = (i < sigfiles.size() ? gkphos : gkphob);
	 int n = gkpho->GetN();
	 gkpho->SetPoint(n, ptphot, kpho);
	 gkpho->SetPointError(n, ptphot_err, kpho_err);
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
       
       if (rkpho && ptphot && ptgen) {
	 TGraphErrors *grkpho = (i < sigfiles.size() ? grkphos : grkphob);
	 int n = grkpho->GetN();
	 grkpho->SetPoint(n, ptphot, rkpho);
	 grkpho->SetPointError(n, ptphot_err, rkpho_err);
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

     if (ktopo && ptphot && ptgen) {
       TGraphErrors *gktopo = (i < sigfiles.size() ? gktopos : gktopob);
       int n = gktopo->GetN();
       gktopo->SetPoint(n, ptgen, ktopo);
       gktopo->SetPointError(n, ptgen_err, ktopo_err);
     }

     // Direct method imbalance pTgenjet/pTrecogamma

     dir = (i < sigfiles.size() ? dirs : dirb);
     TH1D *hkgam = (TH1D*)f->Get((dir+"/lgj_ptop").c_str());

     assert(hkgam);

     // Replace with Gaussian fit later
     double kgam = hkgam->GetMean();
     double kgam_err = max(0.05,hkgam->GetRMS())
       / sqrt(max(1.,hkgam->Integral()));

     if (kgam && ptphot) {
       TGraphErrors *gkgam = (i < sigfiles.size() ? gkgams : gkgamb);
       int n = gkgam->GetN();
       gkgam->SetPoint(n, ptphot, kgam);
       gkgam->SetPointError(n, ptphot_err, kgam_err);
     }

    // Measured response vs ptgamma

     dir = (i < sigfiles.size() ? dirsp : dirbp);
     TH1D *hrmeas = (TH1D*)f->Get((dir+"/lrj_ptop").c_str());

     if (hrmeas) {

       // Replace with Gaussian fit later
       double rmeas = hrmeas->GetMean();
       double rmeas_err = max(0.01,hrmeas->GetRMS())
	 / sqrt(max(1.,hrmeas->Integral()));
       
       if (rmeas && ptphot && ptgen) {
	 TGraphErrors *grmeas = (i < sigfiles.size() ? grmeass : grmeasb);
	 int n = grmeas->GetN();
	 grmeas->SetPoint(n, ptphot, rmeas);
	 grmeas->SetPointError(n, ptphot_err, rmeas_err);
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

  TCanvas *crjet = new TCanvas("crjet","crjet",600,600);
  crjet->SetLogx();

  TH1D *hrjet = new TH1D("hrjet","hrjet",1000,0.,2000.);
  hrjet->GetXaxis()->SetTitle("p_{T} (GeV)");
  hrjet->GetXaxis()->SetMoreLogLabels();
  hrjet->GetXaxis()->SetNoExponent();
  hrjet->GetXaxis()->SetRangeUser(30.,2000.);
  hrjet->GetYaxis()->SetTitle("R_{jet} = p_{T,calojet} / p_{T,genjet}");
  hrjet->GetYaxis()->SetRangeUser(0.40,1.10);

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

  TF1 *frjetb = new TF1("frjetb","1-[0]*pow(x,[1]-1)",10.,14000.);
  frjetb->SetParNames("a","m");
  frjetb->SetLineColor(kBlue);
  frjetb->SetRange(10.,14000.);
  frjetb->SetParameters(frjets->GetParameter(0),frjets->GetParameter(1));
  grjetb->Fit(frjetb,"QRN");
  frjetb->SetRange(10.,14000.);
  hrjet2->GetListOfFunctions()->Add(frjetb);

  TLegend *lrjet = new TLegend(0.65,0.2,0.95,0.4,_algoname.c_str(),"brNDC");
  lrjet->SetBorderSize(0);
  lrjet->SetFillStyle(kNone);
  lrjet->SetTextSize(0.06);
  grjets->SetMarkerStyle(kCircle);
  grjets->SetMarkerColor(kRed);
  grjets->SetMarkerSize(0.8);
  grjetb->SetMarkerStyle(kOpenSquare);
  grjetb->SetMarkerColor(kBlue);
  grjetb->SetMarkerSize(0.8);
  lrjet->AddEntry(grjets, "#gamma jet", "LP");
  lrjet->AddEntry(grjetb, "QCD", "LP");

  hrjet->Draw();
  hrjet2->Draw("SAMES");
  frjets->Draw("SAME");
  frjetb->Draw("SAME");
  grjets->SetLineColor(kRed);
  grjets->Draw("SAME P");
  grjetb->SetLineColor(kBlue);
  grjetb->Draw("SAME P");
  lrjet->Draw();

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
  crjet->SaveAs("eps/mcfactors_rjet.eps");


  TCanvas *ckjet = new TCanvas("ckjet","ckjet",600,600);
  ckjet->SetLogx();

  TH1D *hkjet = new TH1D("hkjet","hkjet",2000,0.,2000.);
  hkjet->GetXaxis()->SetTitle("p_{T} (GeV)");
  hkjet->GetXaxis()->SetMoreLogLabels();
  hkjet->GetXaxis()->SetNoExponent();
  hkjet->GetXaxis()->SetRangeUser(30.,2000.);
  hkjet->GetYaxis()->SetTitle("k_{jet} = p_{T,genjet} / p_{T,parton-jet}");
  hkjet->GetYaxis()->SetRangeUser(0.85,1.10);

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

  TF1 *fkjetb = new TF1("fkjetb","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			10.,14000.);
  fkjetb->SetParNames("p0","p1","p2");
  fkjetb->SetLineColor(kBlue);
  fkjetb->SetRange(10.,14000.);
  fkjetb->SetParameters(0.92, 0.04,-0.005);
  gkjetb->Fit(fkjetb,"QRN");
  fkjetb->SetRange(10.,14000.);
  hkjet2->GetListOfFunctions()->Add(fkjetb);

  TLegend *lkjet = new TLegend(0.65,0.2,0.95,0.4,_algoname.c_str(),"brNDC");
  lkjet->SetBorderSize(0);
  lkjet->SetFillStyle(kNone);
  lkjet->SetTextSize(0.06);
  gkjets->SetMarkerStyle(kCircle);
  gkjets->SetMarkerColor(kRed);
  gkjets->SetMarkerSize(0.8);
  gkjetb->SetMarkerStyle(kOpenSquare);
  gkjetb->SetMarkerColor(kBlue);
  gkjetb->SetMarkerSize(0.8);
  lkjet->AddEntry(gkjets, "#gamma jet", "LP");
  lkjet->AddEntry(gkjetb, "QCD", "LP");

  hkjet->Draw("");
  hkjet2->Draw("SAMES");
  fkjets->Draw("SAME");
  fkjetb->Draw("SAME");
  gkjets->SetLineColor(kRed);
  gkjets->Draw("SAME P");
  gkjetb->SetLineColor(kBlue);
  gkjetb->Draw("SAME P");
  lkjet->Draw();

  gPad->Update(); // update the canvas to create the stats boxes
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

  TF1 *fktopob = new TF1("fktopob","[0]+log(0.01*x)*([1]+log(0.01*x)*[2])",
			 10.,14000.);
  fktopob->SetParNames("p0","p1","p2");
  fktopob->SetLineColor(kBlue);
  fktopob->SetRange(10.,14000.);
  fktopob->SetParameters(1., 0., 0.);
  gktopob->Fit(fktopob,"QRN");
  fktopob->SetRange(10.,14000.);
  hktopo2->GetListOfFunctions()->Add(fktopob);

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

  gPad->Update(); // update the canvas to create the stats boxes
  //TPaveStats *stats, *statb;
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

  TF1 *frphob = new TF1("frphob","1-[0]*pow(x,[1]-1)",10.,14000.);
  frphob->SetParNames("a","m");
  frphob->SetLineColor(kBlue);
  frphob->SetRange(10.,14000.);
  frphob->SetParameters(0.3*frjetb->GetParameter(0),frjetb->GetParameter(1));
  frphob->FixParameter(1,frjetb->GetParameter(1));
  grphob->Fit(frphob,"QRN");
  frphob->SetRange(10.,14000.);
  hrpho2->GetListOfFunctions()->Add(frphob);

  // Expected R_gamma for a background jet
  TF1 *fjet = new TF1("fjet","(1-[0]*pow(x,[1]-1))",0.,14000.);
  fjet->SetParameters(frjetb->GetParameter(0),frjetb->GetParameter(1));

  TLegend *lrpho = new TLegend(0.69,0.18,0.93,0.48,_algoname.c_str(),"brNDC");
  lrpho->SetBorderSize(0);
  lrpho->SetFillStyle(kNone);
  lrpho->SetTextSize(0.06);

  TLatex *trpho = new TLatex(0.22,0.74,(idtype+"ID").c_str());
  trpho->SetNDC();
  trpho->SetTextSize(0.06);

  grphos->SetMarkerStyle(kCircle);
  grphos->SetMarkerColor(kRed);
  grphos->SetMarkerSize(0.8);
  grphob->SetMarkerStyle(kOpenSquare);
  grphob->SetMarkerColor(kBlue);
  grphob->SetMarkerSize(0.8);
  lrpho->AddEntry(grphos, "#gamma jet", "LP");
  lrpho->AddEntry(grphob, "QCD", "LP");
  lrpho->AddEntry(fjet, "R_{jet}^{QCD}", "L");

  hrpho->Draw("");
  hrpho2->Draw("SAMES");
  frphos->Draw("SAME"); // draw on top of stats box
  frphob->Draw("SAME"); // draw on top of stats box
  fjet->Draw("SAME");
  grphos->SetLineColor(kRed);
  grphos->Draw("SAME P");
  grphob->SetLineColor(kBlue);
  grphob->Draw("SAME P");
  lrpho->Draw();
  trpho->Draw();

  gPad->Update(); // update the canvas to create the stats boxes
  //TPaveStats *stats, *statb;
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

  gPad->Update(); // update the canvas to create the stats boxes
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
  fkqcd_prk->SetLineWidth(2);
  grkphor->SetMarkerStyle(kCircle);
  grkphor->SetMarkerColor(kRed);
  grkphor->SetMarkerSize(0.8);
  grkphor->SetLineColor(kRed);
  grkphor->SetLineWidth(2);
  grkphor->SetLineStyle(kDashed);
  //
  fkqcd_jr->SetLineColor(kBlue);
  fkqcd_jr->SetLineStyle(kDotted);
  fkqcd_jr->SetLineWidth(1);
  fkqcd_jrk->SetLineColor(kBlue);
  fkqcd_jrk->SetLineStyle(kDotted);
  fkqcd_jrk->SetLineWidth(2);
  grkjetr->SetMarkerStyle(kOpenSquare);
  grkjetr->SetMarkerColor(kBlue);
  grkjetr->SetMarkerSize(0.8);
  grkjetr->SetLineColor(kBlue);
  grkjetr->SetLineWidth(2);
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
  grmeasr->Draw("same p");
  lkqcd1->Draw();
  lkqcd2->Draw();
  tkqcd->Draw();

  ckqcd->SaveAs("eps/mcfactors_deltac.eps");


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
  
