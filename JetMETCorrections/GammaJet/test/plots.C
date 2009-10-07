#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TMath.h"
#include "TROOT.h"

#include <string>
#include <iostream>

using namespace std;

const bool drawStats = false;
const int kRedHatch = 3654;
const int kBlueHatch = 3645;

const bool _pre09 = false; // Summer09
const bool _octx = true; // Summer09 October exercise

// Draw histo with max of h1, h2 in each bin
// (helps automatic y-range setting)
void DoubleDraw(const TH1D *h1, const TH1D *h2);

int plots(string file1, string file2, string dirs = "/", string dirb = "/",
	  string outdir = "eps", bool rebin = false) {

  if (dirs=="") dirs ="/";
  if (dirb=="") dirb ="/";
  TDirectory *curdir = gDirectory;

  TFile *f1 = new TFile(file1.c_str());
  assert(!f1->IsZombie());
  TFile *f2 = new TFile(file2.c_str());
  assert(!f2->IsZombie());

  string legs = "#gammma jet";
  string legb = "QCD";
  if (_pre09) {
    legs = "prev09-v3";
    legb = "summer08";
  }
  if (_octx) {
    legs = "OctX";
    legb = "summer08";
  }

  // Figure out binning from the file name so can be put on the plots
  double ptmin = -1;
  double ptmax = -1;
  if (TString(file1.c_str()).Contains("_30.root")) {
    ptmin = 30.;
    ptmax = 50;
  }
  if (TString(file1.c_str()).Contains("_80.root")) {
    ptmin = 80.;
    ptmax = 120;
  }
  if (TString(file1.c_str()).Contains("_300.root")) {
    ptmin = 300.;
    ptmax = 470;
  }
  if (TString(file1.c_str()).Contains("x500.root")) {
    ptmin = 500.;
    ptmax = 800;
  }
  if (TString(file1.c_str()).Contains("x80.root")) {
    ptmin = 80.;
    ptmax = 120;
  }

  curdir->cd();

  gROOT->ProcessLine(".x cms_jes_style.C");
  if (drawStats) gStyle->SetOptStat(111111);
  else gStyle->SetOptStat(0);

  TCanvas *c = new TCanvas("c","c",600,600);

  for (int i = 0; i != 4; ++i) {
    for (int j = 0; j != 6; ++j) {
      
      string hname, fname;
      if (i==0) { hname="lrp_"; fname="LeadRecoPhot"; }
      if (i==1) { hname="lrj_"; fname="LeadRecoJet"; }
      if (i==2) { hname="srp_"; fname="SecRecoPhot"; }
      if (i==3) { hname="srj_"; fname="SecRecoJet"; }

      if (j==0) { hname+="pt"; fname+="Pt"; }
      if (j==1) { hname+="ptop"; fname+="PtoGam"; }
      if (j==2) { hname+="eta"; fname+="Eta"; }
      if (j==3) { hname+="phi"; fname+="Phi"; }
      if (j==4) { hname+="dphivp"; fname+="DeltaPhi"; }
      if (j==5) { hname+="detavp"; fname+="DeltaEta"; }

      if (i==0 && (j==1 || j==4 || j==5)) continue;

      string title;
      if (j==0) { title = "p_{T}(%s) [GeV/c]"; }
      if (j==1) { title = "p_{T}(%s) / p_{T}(calo-#gamma)"; }
      if (j==2) { title = "#eta(%s)"; }
      if (j==3) { title = "#phi(%s)"; }
      if (j==4) { title = "#Delta#phi(%s, calo-#gamma)"; }
      if (j==5) { title = "#Delta#eta(%s, calo-#gamma)"; }

      if (i==0) title = Form(title.c_str(),"calo-#gamma");
      if (i==1) title = Form(title.c_str(),"calojet");
      if (i==2) title = Form(title.c_str(),"2nd calo-#gamma");
      if (i==3) title = Form(title.c_str(),"2nd calojet");

      TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str());
      TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str());
      if (!h1) {
	cout << (file1+":"+dirs+"/"+hname) << " not found!" << endl;
	assert(h1);
      }
      if (!h2) {
	cout << (file2+":"+dirb+"/"+hname) << " not found!" << endl;
	//assert(h2);
	continue;
      }
      curdir->cd();

      double sum1 = h1->Integral();
      double sum2 = h2->Integral();
      if (sum1 && sum2) h2->Scale(sum1/sum2);
      if (rebin) { // && j!=0) {
	//h1->Rebin(8); h1->Scale(1./8.);
	//h2->Rebin(8); h2->Scale(1./8.);
	h1->Rebin(4); h1->Scale(1./4.);
	h2->Rebin(4); h2->Scale(1./4.);
      }

      c->Clear();
      c->SetLogx(kFALSE);
      if (j==0) c->SetLogx(kTRUE);
      c->SetLogy(kTRUE);
      if (j==4 || j==5) c->SetLogy(kFALSE);

      //TLatex *tex = new TLatex(0.57, 0.85, Form("%1.0f<#hat{p}_{T}<%1.0f GeV",
      TLatex *tex = new TLatex(0.57, 0.85, Form("#hat{p}_{T} %1.0f-%1.0f GeV",
						ptmin, ptmax));
      tex->SetNDC();
      tex->SetTextSize(0.05);

      TLegend *leg = new TLegend(0.70, 0.65, 0.95, 0.80, "", "brNDC");
      leg->SetBorderSize(0);
      leg->SetFillStyle(kNone);
      leg->AddEntry(h1, legs.c_str(), "F");
      leg->AddEntry(h2, legb.c_str(), "F");
      if (j==4) { leg->SetX1(0.45); leg->SetX2(0.70); }

      if (j==0)	h1->GetXaxis()->SetRangeUser(10,1000.);
      //if (j==0) h1->GetXaxis()->SetTitle("p_{T} [GeV/c]");
      if (j==1) h1->GetXaxis()->SetRangeUser(0,3.);
      //if (j==1) h1->GetXaxis()->SetTitle("p_{T} / p_{T}^{#gamma}");
      if (j==2) h1->GetXaxis()->SetRangeUser(-5.,5.);
      //if (j==2) h1->GetXaxis()->SetTitle("#eta");
      if (j==3) h1->GetXaxis()->SetRangeUser(0.,TMath::TwoPi()+0.0001);
      //if (j==3) h1->GetXaxis()->SetTitle("#phi");
      if (j==4) h1->GetXaxis()->SetRangeUser(0.,TMath::Pi()+0.0001);
      //if (j==4) h1->GetXaxis()->SetTitle("#Delta#phi(jet,#gamma)");
      if (j==5) h1->GetXaxis()->SetRangeUser(-3.,2.);
      //if (j==5) h1->GetXaxis()->SetTitle("#Delta#eta(jet,#gamma)");

      h1->GetXaxis()->SetTitle(title.c_str());
      h2->GetXaxis()->SetTitle(title.c_str());

      // Histogram drawn first determines the y-axis range
      //if (j!=4) h1->Draw("HF");
      //else h2->Draw("HF");
      // Draw max of the two in each bin for proper y-axis range
      DoubleDraw(h1, h2); 

      h1->SetLineColor(kRed);
      h1->SetFillColor(kRed);
      h1->SetFillStyle(kRedHatch);
      h1->GetXaxis()->SetNoExponent();
      h1->GetXaxis()->SetMoreLogLabels();
      h1->Draw("HFsame");
      c->Update(); // to get the new stats box
      TPaveStats *stats = (TPaveStats*)gPad->GetPrimitive("stats");
      //assert(stats);
      if (stats && drawStats) {
	stats->SetName("stats_sig");
	stats->SetFillStyle(kNone);
	stats->SetLineColor(kRed);
	stats->SetTextColor(kRed);
	stats->SetX1NDC(max(stats->GetX1NDC(),stats->GetX2NDC()-0.36));
      }

      h2->SetLineColor(kBlue);
      h2->SetFillColor(kBlue);
      h2->SetFillStyle(kBlueHatch);
      h2->Draw("HFsames");
      c->Update(); // to get the new stats box
      TPaveStats *statb = (TPaveStats*)gPad->GetPrimitive("stats");
      //assert(statb);
      if (statb && drawStats && false) {
 	statb->SetName("stats_bkg");
 	statb->SetFillStyle(kNone);
 	statb->SetLineColor(kBlue);
 	statb->SetTextColor(kBlue);
 	statb->SetX1NDC(max(stats->GetX1NDC()-0.39, 0.20));
 	statb->SetX2NDC(min(stats->GetX2NDC()-0.39, stats->GetX1NDC()));
      }

      if (!drawStats) leg->Draw();
      if (ptmin!=-1) tex->Draw();

      c->SaveAs((outdir+"/"+fname+".eps").c_str());

    } // for j
  } // for i


  for (int i = 0; i != 2; ++i) { // PHOTON ID
    for (int j = 0; j != 6; ++j) {

      string hname, hname2, fname, epsname;
      if (i==0) { hname="lrp_"; hname2="lrp_"; fname="LeadRecoPhot"; }
      if (i==1) { hname="srp_"; hname2="lrp_"; fname="SecRecoPhot"; }
      
      if (j==0) { hname += "ntrkiso"; epsname = "NtrkIso"; }
      if (j==1) { hname += "ptiso"; epsname = "PtIso"; }
      if (j==2) { hname += "emf"; hname2 += "hcaliso"; epsname = "HcalIso"; }
      if (j==3) { hname += "sMajMaj"; epsname = "ClusMajMaj";}
      if (j==4) { hname += "sMinMin"; epsname = "ClusMinMin"; }
      //if (j==5) { hname += "ecaliso_new"; epsname = "EcalIso"; }
      if (j==5) { hname += "ecaliso"; epsname = "EcalIso"; }

      TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str()); //assert(h1);
      if (!h1) h1 = (TH1D*)f1->Get((dirs+"/"+hname2).c_str()); //assert(h1);
      TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str()); //assert(h2);
      if (!h2) h2 = (TH1D*)f2->Get((dirb+"/"+hname2).c_str()); //assert(h1);
      if (!h1) {
	
	cout << (file1+":"+dirs+"/"+hname)<< " not found!" << endl;
	assert(h1);
      }
      if (!h2) {
	cout << (file2+":"+dirb+"/"+hname)<< " not found!" << endl;
	//assert(h2);
	continue;
      }
      curdir->cd();
      
      double sum1 = h1->Integral();
      double sum2 = h2->Integral();
      if (sum1 && sum2) h2->Scale(sum1/sum2);
      //if (i==1 || i==3) { h2->Rebin(8); h2->Scale(1./8.); }
      
      if (j==0) h1->GetXaxis()->SetRangeUser(-0.5,4.5);
      if (j==0) h1->GetXaxis()->SetTitle("N_{tracks}");
      if (j==1) h1->GetXaxis()->SetRangeUser(0.,0.2);
    //if (j==1) h1->GetXaxis()->SetTitle("#sum_{tracks} p_{T} / p_{T,#gamma}");
      if (j==1) h1->GetXaxis()->SetTitle("track isolation / p_{T,#gamma}");
      if (j==2) h1->GetXaxis()->SetRangeUser(-0.3,0.3);
      if (j==2) h1->GetXaxis()->SetTitle("HCAL isolation / p_{T,#gamma}");
      if (j==3) h1->GetXaxis()->SetRangeUser(0.,2.0);
      if (j==3) h1->GetXaxis()->SetTitle("Cluster major axis #sigma^{2}");
      if (j==4) h1->GetXaxis()->SetRangeUser(0.,1.0);
      if (j==4) h1->GetXaxis()->SetTitle("Cluster minor axis #sigma^{2}");
      if (j==5) h1->GetXaxis()->SetRangeUser(0.,0.2);
      if (j==5) h1->GetXaxis()->SetTitle("ECAL isolation / p_{T,#gamma}");

      c->Clear();
      c->SetLogx(kFALSE);
      c->SetLogy(kTRUE);

      //TLatex *tex = new TLatex(0.57, 0.85, Form("%1.0f<#hat{p}_{T}<%1.0f GeV",
      TLatex *tex = new TLatex(0.57, 0.85, Form("#hat{p}_{T} %1.0f-%1.0f GeV",
						ptmin, ptmax));
      tex->SetNDC();
      tex->SetTextSize(0.05);
    
      TLegend *leg = new TLegend(0.70, 0.65, 0.95, 0.80, "", "brNDC");
      leg->SetBorderSize(0);
      leg->SetFillStyle(kNone);
      leg->AddEntry(h1, legs.c_str(), "F");
      leg->AddEntry(h2, legb.c_str(), "F");
      
      DoubleDraw(h1, h2);

      h1->SetLineColor(kRed);
      h1->SetFillColor(kRed);
      h1->SetFillStyle(kRedHatch);
      h1->Draw("HFsame");
      
      h2->SetLineColor(kBlue);
      h2->SetFillColor(kBlue);
      h2->SetFillStyle(kBlueHatch);
      h2->Draw("HFsame");
      
      leg->Draw();
      if (ptmin!=-1) tex->Draw();

      c->SaveAs((outdir+"/"+fname+epsname+".eps").c_str());
    } // for j
  } // for i


  for (int i = 0; i != 6; ++i) { // MC MATCH VS PARTON
    for (int j = 0; j != 4; ++j) {

      string hname, fname, epsname;
      if (i==0 || i==1) { hname="lrp_"; fname="LeadRecoPhot"; }
      if (i==2 || i==3) { hname="lrj_"; fname="LeadRecoJet"; }
      if (i==4) { hname="lgg_"; fname="LeadGenPhot"; }
      if (i==5) { hname="lgj_"; fname="LeadGenJet"; }
      
      string title;
      if (j==0) title = "#Deltap_{T} / p_{T}";
      if (j==1) title = "#Delta#eta";
      if (j==2) title = "#Delta#phi";
      if (j==3) title = "#DeltaR";

      if (i==0) { 
	title += "(calo-#gamma, gen-#gamma)";
	if (j==0) title = "p_{T}(calo-#gamma) / p_{T}(gen-#gamma)";
	if (j==0) { hname += "dptvgg"; epsname = "DPtvGenPhot"; }
	if (j==1) { hname += "detavgg"; epsname = "DEtavGenPhot"; }
	if (j==2) { hname += "dphivgg"; epsname = "DPhivGenPhot"; }
	if (j==3) { hname += "drvgg"; epsname = "DRvGenPhot";}
      }
      if (i==1) { 
	title += "(calo-#gamma, parton-#gamma)";
	if (j==0) title = "p_{T}(calo-#gamma) / p_{T}(parton-#gamma)";
	if (j==0) { hname += "dptvg"; epsname = "DPtvParton"; }
	if (j==1) { hname += "detavg"; epsname = "DEtavParton"; }
	if (j==2) { hname += "dphivg"; epsname = "DPhivParton"; }
	if (j==3) { hname += "drvg"; epsname = "DRvParton";}
      }
      if (i==2) {
	title += "(calojet, genjet)";
	if (j==0) title = "p_{T}(calojet) / p_{T}(genjet)";
	if (j==0) { hname += "dptvg"; epsname = "DPtvGenJet"; }
	if (j==1) { hname += "detavg"; epsname = "DEtavGenJet"; }
	if (j==2) { hname += "dphivg"; epsname = "DPhivGenJet"; }
	if (j==3) { hname += "drvg"; epsname = "DRvGenJet";}
      }
      if (i==3) {
	title += "(calojet, jet parton)";
	if (j==0) title = "p_{T}(calojet) / p_{T}(jet parton)";
	if (j==0) { hname += "dptvq"; epsname = "DPtvParton"; }
	if (j==1) { hname += "detavq"; epsname = "DEtavParton"; }
	if (j==2) { hname += "dphivq"; epsname = "DPhivParton"; }
	if (j==3) { hname += "drvq"; epsname = "DRvParton";}
      }
      if (i==4) { 
	title += "(gen-#gamma, parton-#gamma)";
	if (j==0) title = "p_{T}(gen-#gamma) / p_{T}(parton-#gamma)";
	if (j==0) { hname += "dptvg"; epsname = "DPtvParton"; }
	if (j==1) { hname += "detavg"; epsname = "DEtavParton"; }
	if (j==2) { hname += "dphivg"; epsname = "DPhivParton"; }
	if (j==3) { hname += "drvg"; epsname = "DRvParton";}
      }      
      if (i==5) {
	title += "(genjet, jet parton)";
	if (j==0) title = "p_{T}(genjet) / p_{T}(jet parton)";
	if (j==0) { hname += "dptvq"; epsname = "DPtvParton"; }
	if (j==1) { hname += "detavq"; epsname = "DEtavParton"; }
	if (j==2) { hname += "dphivq"; epsname = "DPhivParton"; }
	if (j==3) { hname += "drvq"; epsname = "DRvParton";}
      }      

      TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str()); //assert(h1);
      TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str()); //assert(h2);
      // NB: replace GenPhot with Parton for signal
      if (hname=="lrp_dptvgg" && !(_pre09 || _octx)) {
	h1 = (TH1D*)f1->Get((dirs+"/lrp_dptvg").c_str());
	if (_pre09 || _octx) h2 = (TH1D*)f2->Get((dirb+"/lrp_dptvg").c_str());
      }
      if (hname=="lgg_dptvg" && !(_pre09 || _octx)) {
	h1 = (TH1D*)f1->Get((dirs+"/lrp_dptvg").c_str());
	if (_pre09 || _octx) h2 = (TH1D*)f2->Get((dirb+"/lrp_dptvg").c_str());
      }
      if (!h1) {
	cout << (file1+":"+dirs+"/"+hname)<< " not found!" << endl;
	assert(h1);
      }
      if (!h2) {
	cout << (file2+":"+dirb+"/"+hname)<< " not found!" << endl;
	//assert(h2);
	continue;
      }
      curdir->cd();
      
      double sum1 = h1->Integral();
      double sum2 = h2->Integral();
      if (sum1 && sum2) h2->Scale(sum1/sum2);
      //if (i==1 || i==3) 
      //if (rebin && (i!=0 || j==0)) { h2->Rebin(8); h2->Scale(1./8.); }
      if (rebin) {
	h2->Rebin(4); h2->Scale(1./4.);
	// Watch out for multiple rebins
	if (h1->GetNbinsX() > h2->GetNbinsX())
	  h1->Rebin(4); h1->Scale(1./4.);
      }
      
      //if (j==0) h1->GetXaxis()->SetRangeUser(-0.5,4.5);

      c->Clear();
      c->SetLogx(kFALSE);
      c->SetLogy(kTRUE);
      
      //TLatex *tex = new TLatex(0.57, 0.85, Form("%1.0f<#hat{p}_{T}<%1.0f GeV",
      TLatex *tex = new TLatex(0.57, 0.85, Form("#hat{p}_{T} %1.0f-%1.0f GeV",
						ptmin, ptmax));
      tex->SetNDC();
      tex->SetTextSize(0.05);

      TLegend *leg = new TLegend(0.70, 0.65, 0.95, 0.80, "", "brNDC");
      leg->SetBorderSize(0);
      leg->SetFillStyle(kNone);
      leg->AddEntry(h1, legs.c_str(), "F");
      leg->AddEntry(h2, legb.c_str(), "F");

      h1->GetXaxis()->SetTitle(title.c_str());
      h2->GetXaxis()->SetTitle(title.c_str());

      DoubleDraw(h1,h2);

      h1->SetLineColor(kRed);
      h1->SetFillColor(kRed);
      h1->SetFillStyle(kRedHatch);
      h1->Draw("HFsame");
      
      h2->SetLineColor(kBlue);
      h2->SetFillColor(kBlue);
      h2->SetFillStyle(kBlueHatch);
      h2->Draw("HFsame");

      leg->Draw();
      if (ptmin!=-1) tex->Draw();
      
      if (i==2 && j==0) {
	TLatex *tcms = new TLatex(0.58, 0.50, "CMS preliminary");
	tcms->SetNDC();
	tcms->SetTextSize(0.05);
	tcms->Draw();
      }

      c->SaveAs((outdir+"/"+fname+epsname+".eps").c_str());
    } // for j
  } // for i


  for (int i = 0; i != 3; ++i) { // EVENT TOPOLOGY

    string hname, fname;
    if (i==0) { hname="lrj_ptop"; fname="LeadRecoJet"; }
    if (i==1) { hname="lgj_ptop"; fname="LeadGenJet"; }
    if (i==2) { hname="lgq_ptop"; fname="LeadQuark"; }

    TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str()); //assert(h1);
    TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str()); //assert(h2);
    if (!h1) {
      cout << (file1+":"+dirs+"/"+hname) << " not found!" << endl;
      assert(h1);
    }
    if (!h2) {
      cout << (file2+":"+dirb+"/"+hname) << " not found!" << endl;
      //assert(h2);
      continue;
    }
    curdir->cd();

    double sum1 = h1->Integral();
    double sum2 = h2->Integral();
    if (sum1 && sum2) h2->Scale(sum1/sum2);
    //if (i==1 || i==3) 
    //if (rebin) { h2->Rebin(8); h2->Scale(1./8.); }
    if (rebin) {
      h1->Rebin(4); h1->Scale(1./4.); 
      h2->Rebin(4); h2->Scale(1./4.);
    }

    c->Clear();
    c->SetLogx(kFALSE);
    c->SetLogy(kTRUE);

    //TLatex *tex = new TLatex(0.57, 0.85, Form("%1.0f<#hat{p}_{T}<%1.0f GeV",
    TLatex *tex = new TLatex(0.57, 0.85, Form("#hat{p}_{T} %1.0f-%1.0f GeV",
					      ptmin, ptmax));
    tex->SetNDC();
    tex->SetTextSize(0.05);

    TLegend *leg = new TLegend(0.70, 0.65, 0.95, 0.80, "", "brNDC");
    leg->SetBorderSize(0);
    leg->SetFillStyle(kNone);
    leg->AddEntry(h1, legs.c_str(), "F");
    leg->AddEntry(h2, legb.c_str(), "F");

    if (i==0) h1->GetXaxis()->SetTitle("p_{T}(calojet) / p_{T}(calo-#gamma)");
    if (i==1) h1->GetXaxis()->SetTitle("p_{T}(genjet) / p_{T}(calo-#gamma)");
    if (i==2) h1->GetXaxis()->SetTitle("p_{T}(jet parton) / p_{T}(calo-#gamma)");

    DoubleDraw(h1,h2);

    h1->SetLineColor(kRed);
    h1->SetFillColor(kRed);
    h1->SetFillStyle(kRedHatch);
    h1->Draw("HFsame");

    h2->SetLineColor(kBlue);
    h2->SetFillColor(kBlue);
    h2->SetFillStyle(kBlueHatch);
    h2->Draw("HFsame");

    leg->Draw();
    if (ptmin!=-1) tex->Draw();

    c->SaveAs((outdir+"/"+fname+"PtoGam.eps").c_str());
  }

  f1->Close();
  f2->Close();
  curdir->cd();

  //gROOT->Reset(); // loses also variables in mk_plots.C
  gROOT->Clear(); // important, otherwise mk_plots.C crashes in third loop

  return 0;
}


// Draw histo with max or min of h1, h2 in each bin
// (helps automatic y-range setting)
void DoubleDraw(const TH1D *h1, const TH1D *h2) {

  TH1D *h3 = (TH1D*)h1->Clone(Form("h3_%s_%s",h1->GetName(),h2->GetName()));
  double lo = 1e9;
  double hi = -1e9;
  for (int i = 1; i != h1->GetNbinsX()+1; ++i) {
    if (h1->GetBinContent(i)>hi) hi = h1->GetBinContent(i);
    if (h1->GetBinContent(i)<lo && h1->GetBinContent(i)>0)
      lo = h1->GetBinContent(i);
  }
  for (int i = 1; i != h2->GetNbinsX()+1; ++i) {
    if (h2->GetBinContent(i)>hi) hi = h2->GetBinContent(i);
    if (h2->GetBinContent(i)<lo && h2->GetBinContent(i)>0)
    lo = h2->GetBinContent(i);
  }
  for (int i = 1; i != h3->GetNbinsX()+1; ++i) {
    if (i%3==0) h3->SetBinContent(i, hi);
    if (i%3==1) h3->SetBinContent(i, lo);
    //if (i%3==2) h3->SetBinContent(i, 0);
  }
  /*
  for (int i = 0; i != h1->GetNbinsX()+1; ++i) {
    h3->SetBinContent(i, max(h1->GetBinContent(i), h2->GetBinContent(i)));
  }
  */

  h3->SetFillStyle(kNone);
  h3->SetLineColor(kWhite);
  h3->Draw("H");

  return;
}
