#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"

#include <string>
#include <iostream>

using namespace std;

int plots(string file1, string file2, string dirs = "/", string dirb = "/",
	  string outdir = "eps", bool rebin = false) {

  if (dirs=="") dirs ="/";
  if (dirb=="") dirb ="/";
  TDirectory *curdir = gDirectory;

  TFile *f1 = new TFile(file1.c_str());
  assert(!f1->IsZombie());
  TFile *f2 = new TFile(file2.c_str());
  assert(!f2->IsZombie());

  curdir->cd();

  gROOT->ProcessLine(".x MyAnalysis/GammaJetAnalysis/test/d0_jes_style.C");
  gStyle->SetOptStat(111111);

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
      if (rebin && j!=0) {
	h1->Rebin(8); h1->Scale(1./8.);
	h2->Rebin(8); h2->Scale(1./8.);
      }

      c->Clear();
      c->SetLogx(kFALSE);
      if (j==0) c->SetLogx(kTRUE);
      c->SetLogy(kTRUE);
      if (j==4 || j==5) c->SetLogy(kFALSE);

      if (j==0)	h1->GetXaxis()->SetRangeUser(10,1000.);
      if (j==0) h1->GetXaxis()->SetTitle("p_{T} [GeV]");
      if (j==1) h1->GetXaxis()->SetRangeUser(0,3.);
      if (j==1) h1->GetXaxis()->SetTitle("p_{T} / p_{T}^{#gamma}");
      //if (j==2) h1->GetXaxis()->SetRangeUser(-5.,5.);
      //if (j==2) h1->GetXaxis()->SetTitle("#eta");
      //if (j==3) h1->GetXaxis()->SetRangeUser(0.,TMath::TwoPi());
      //if (j==3) h1->GetXaxis()->SetTitle("#phi");
      //if (j==4) h1->GetXaxis()->SetRangeUser(0.,TMath::Pi());
      if (j==4) h1->GetXaxis()->SetTitle("#Delta#phi(jet,#gamma)");
      //if (j==5) h1->GetXaxis()->SetRangeUser(0.,TMath::Pi());
      if (j==5) h1->GetXaxis()->SetTitle("#Delta#eta(jet,#gamma)");

      // Histogram drawn first determines the y-axis range
      if (j!=4) h1->Draw();
      else h2->Draw();

      h1->SetLineColor(kRed);
      h1->GetXaxis()->SetNoExponent();
      h1->GetXaxis()->SetMoreLogLabels();
      h1->Draw("same");
      c->Update(); // to get the new stats box
      TPaveStats *stats = (TPaveStats*)gPad->GetPrimitive("stats");
      //assert(stats);
      if (stats) {
	stats->SetName("stats_sig");
	stats->SetFillStyle(kNone);
	stats->SetLineColor(kRed);
	stats->SetTextColor(kRed);
	stats->SetX1NDC(max(stats->GetX1NDC(),stats->GetX2NDC()-0.36));
      }

      h2->SetLineColor(kBlue);
      h2->Draw("sames");
      c->Update(); // to get the new stats box
      TPaveStats *statb = (TPaveStats*)gPad->GetPrimitive("stats");
      //assert(statb);
//       if (statb) {
// 	statb->SetName("stats_bkg");
// 	statb->SetFillStyle(kNone);
// 	statb->SetLineColor(kBlue);
// 	statb->SetTextColor(kBlue);
// 	statb->SetX1NDC(max(stats->GetX1NDC()-0.39, 0.20));
// 	statb->SetX2NDC(min(stats->GetX2NDC()-0.39, stats->GetX1NDC()));
//       }

      c->SaveAs((outdir+"/"+fname+".eps").c_str());

    } // for j
  } // for i


  /*
  for (int i = 0; i != 4; ++i) { // PT

    string hname, fname;
    if (i==0) { hname="lrp_pt"; fname="LeadRecoPhot"; }
    if (i==1) { hname="lrj_pt"; fname="LeadRecoJet"; }
    if (i==2) { hname="srp_pt"; fname="SecRecoPhot"; }
    if (i==3) { hname="srj_pt"; fname="SecRecoJet"; }

    TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str());// assert(h1);
    TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str());// assert(h2);
    if (!h1) {
      cout << (file1+":"+dirs+"/"+hname) << " not found!" << endl; assert(h1);
    }
    if (!h2) {
      cout << (file2+":"+dirb+"/"+hname) << " not found!" << endl; assert(h2);
    }
    curdir->cd();

    double sum1 = h1->Integral();
    double sum2 = h2->Integral();
    if (sum1 && sum2) h2->Scale(sum1/sum2);
    //{ h2->Rebin(8); h2->Scale(1./8.); }

    c->Clear();
    c->SetLogx(kTRUE);
    c->SetLogy(kTRUE);

    h1->SetLineColor(kRed);
    h1->GetXaxis()->SetRangeUser(10,1000.);
    h1->GetXaxis()->SetTitle("p_{T} [GeV]");
    h1->GetXaxis()->SetNoExponent();
    h1->GetXaxis()->SetMoreLogLabels();
    h1->Draw();

    h2->SetLineColor(kBlue);
    h2->Draw("same");

    c->SaveAs((outdir+"/"+fname+"Pt.eps").c_str());
  }
  */
  /*
  for (int i = 1; i != 4; ++i) { // PT/PTGAM

    string hname, fname;
    if (i==1) { hname="lrj_ptop"; fname="LeadRecoJet"; }
    if (i==2) { hname="srp_ptop"; fname="SecRecoPhot"; }
    if (i==3) { hname="srj_ptop"; fname="SecRecoJet"; }

    TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str()); //assert(h1);
    TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str()); //assert(h2);
    if (!h1) {
      cout << (file1+":"+dirs+"/"+hname) << " not found!" << endl; assert(h1);
    }
    if (!h2) {
      cout << (file2+":"+dirb+"/"+hname) << " not found!" << endl; assert(h2);
    }
    curdir->cd();

    double sum1 = h1->Integral();
    double sum2 = h2->Integral();
    if (sum1 && sum2) h2->Scale(sum1/sum2);
    //if (i==3) 
    if (rebin) { h2->Rebin(8); h2->Scale(1./8.); }

    c->Clear();
    c->SetLogx(kFALSE);
    c->SetLogy(kTRUE);

    h1->SetLineColor(kRed);
    h1->GetXaxis()->SetRangeUser(0,3.);
    h1->GetXaxis()->SetTitle("p_{T} / p_{T}^{#gamma}");
    h1->GetXaxis()->SetNoExponent();
    h1->GetXaxis()->SetMoreLogLabels();
    h1->Draw(); 

    h2->SetLineColor(kBlue);
    h2->Draw("same");

    c->SaveAs((outdir+"/"+fname+"PtoGam.eps").c_str());
  }
  */

  /*
  for (int i = 0; i != 4; ++i) { // ETA

    string hname, fname;
    if (i==0) { hname="lrp_eta"; fname="LeadRecoPhot"; }
    if (i==1) { hname="lrj_eta"; fname="LeadRecoJet"; }
    if (i==2) { hname="srp_eta"; fname="SecRecoPhot"; }
    if (i==3) { hname="srj_eta"; fname="SecRecoJet"; }

    TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str()); //assert(h1);
    TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str()); //assert(h2);
    if (!h1) {
      cout << (file1+":"+dirs+"/"+hname) << " not found!" << endl; assert(h1);
    }
    if (!h2) {
      cout << (file2+":"+dirb+"/"+hname) << " not found!" << endl; assert(h2);
    }
    curdir->cd();

    double sum1 = h1->Integral();
    double sum2 = h2->Integral();
    if (sum1 && sum2) h2->Scale(sum1/sum2);
    //if (i==1 || i==3) 
    if (rebin) { h2->Rebin(8); h2->Scale(1./8.); }

    c->Clear();
    c->SetLogx(kFALSE);
    c->SetLogy(kTRUE);

    h1->SetLineColor(kRed);
    h1->Draw();

    h2->SetLineColor(kBlue);
    h2->Draw("same");

    c->SaveAs((outdir+"/"+fname+"Eta.eps").c_str());
  }
  */

  /*
  for (int i = 0; i != 4; ++i) { // PHI

    string hname, fname;
    if (i==0) { hname="lrp_phi"; fname="LeadRecoPhot"; }
    if (i==1) { hname="lrj_phi"; fname="LeadRecoJet"; }
    if (i==2) { hname="srp_phi"; fname="SecRecoPhot"; }
    if (i==3) { hname="srj_phi"; fname="SecRecoJet"; }

    TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str()); assert(h1);
    TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str()); assert(h2);
    if (!h1) {
      cout << (file1+":"+dirs+"/"+hname)<< " not found!" << endl; //assert(h1);
    }
    if (!h2) {
      cout << (file2+":"+dirb+"/"+hname)<< " not found!" << endl; //assert(h2);
    }
    curdir->cd();

    double sum1 = h1->Integral();
    double sum2 = h2->Integral();
    if (sum1 && sum2) h2->Scale(sum1/sum2);
    //if (i==1 || i==3) 
    if (rebin) { h2->Rebin(8); h2->Scale(1./8.); }

    c->Clear();
    c->SetLogx(kFALSE);
    c->SetLogy(kTRUE);

    h1->SetLineColor(kRed);
    h1->Draw();

    h2->SetLineColor(kBlue);
    h2->Draw("same");

    c->SaveAs((outdir+"/"+fname+"Phi.eps").c_str());
  }
  */

  /*
  for (int i = 1; i != 4; ++i) { // DELTAPHI

    string hname, fname;
    if (i==1) { hname="lrj_dphivp"; fname="LeadRecoJet"; }
    if (i==2) { hname="srp_dphivp"; fname="SecRecoPhot"; }
    if (i==3) { hname="srj_dphivp"; fname="SecRecoJet"; }

    TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str()); //assert(h1);
    TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str()); //assert(h2);
    if (!h1) {
      cout << (file1+":"+dirs+"/"+hname) << " not found!" << endl; assert(h1);
    }
    if (!h2) {
      cout << (file2+":"+dirb+"/"+hname) << " not found!" << endl; assert(h2);
    }
    curdir->cd();

    double sum1 = h1->Integral();
    double sum2 = h2->Integral();
    if (sum1 && sum2) h2->Scale(sum1/sum2);
    //if (i==1 || i==3) 
    if (rebin) { h2->Rebin(8); h2->Scale(1./8.); }

    c->Clear();
    c->SetLogx(kFALSE);
    c->SetLogy(kTRUE);

    h1->SetLineColor(kRed);
    h1->Draw(); h1->DrawClone("same");

    h2->SetLineColor(kBlue);
    h2->Draw("same");

    c->SaveAs((outdir+"/"+fname+"DeltaPhi.eps").c_str());
  }
  */

  /*
  for (int i = 1; i != 4; ++i) { // DELTAETA

    string hname, fname;
    if (i==1) { hname="lrj_detavp"; fname="LeadRecoJet"; }
    if (i==2) { hname="srp_detavp"; fname="SecRecoPhot"; }
    if (i==3) { hname="srj_detavp"; fname="SecRecoJet"; }

    TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str()); //assert(h1);
    TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str()); //assert(h2);
    if (!h1) {
      cout << (file1+":"+dirs+"/"+hname) << " not found!" << endl; assert(h1);
    }
    if (!h2) {
      cout << (file2+":"+dirb+"/"+hname) << " not found!" << endl; assert(h2);
    }
    curdir->cd();

    double sum1 = h1->Integral();
    double sum2 = h2->Integral();
    if (sum1 && sum2) h2->Scale(sum1/sum2);
    //if (i==1 || i==3) 
    if (rebin) { h2->Rebin(8); h2->Scale(1./8.); }

    c->Clear();
    c->SetLogx(kFALSE);
    c->SetLogy(kTRUE);

    h1->SetLineColor(kRed);
    h1->Draw();

    h2->SetLineColor(kBlue);
    h2->Draw("same");

    c->SaveAs((outdir+"/"+fname+"DeltaEta.eps").c_str());
  }
  */

  for (int i = 0; i != 2; ++i) { // PHOTON ID
    for (int j = 0; j != 6; ++j) {

      string hname, fname, epsname;
      if (i==0) { hname="lrp_"; fname="LeadRecoPhot"; }
      if (i==1) { hname="srp_"; fname="SecRecoPhot"; }
      
      if (j==0) { hname += "ntrkiso"; epsname = "NtrkIso"; }
      if (j==1) { hname += "ptiso"; epsname = "PtIso"; }
      if (j==2) { hname += "emf"; epsname = "EMF"; }
      if (j==3) { hname += "sMajMaj"; epsname = "ClusMajMaj";}
      if (j==4) { hname += "sMinMin"; epsname = "ClusMinMin"; }
      if (j==5) { hname += "ecaliso_new"; epsname = "EcalIso"; }
      
      TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str()); //assert(h1);
      TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str()); //assert(h2);
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
      if (j==1) h1->GetXaxis()->SetRangeUser(0.,0.2);
      if (j==2) h1->GetXaxis()->SetRangeUser(-0.3,0.3);
      if (j==3) h1->GetXaxis()->SetRangeUser(0.,2.0);
      if (j==4) h1->GetXaxis()->SetRangeUser(0.,1.0);
      if (j==5) h1->GetXaxis()->SetRangeUser(0.,0.2);

      c->Clear();
      c->SetLogx(kFALSE);
      c->SetLogy(kTRUE);
      
      h1->SetLineColor(kRed);
      h1->Draw();
      
      h2->SetLineColor(kBlue);
      h2->Draw("same");
      
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
      
      if (i==0) { 
	if (j==0) { hname += "dptvgg"; epsname = "DPtvGenPhot"; }
	if (j==1) { hname += "detavgg"; epsname = "DEtavGenPhot"; }
	if (j==2) { hname += "dphivgg"; epsname = "DPhivGenPhot"; }
	if (j==3) { hname += "drvgg"; epsname = "DRvGenPhot";}
      }
      if (i==1) { 
	if (j==0) { hname += "dptvg"; epsname = "DPtvParton"; }
	if (j==1) { hname += "detavg"; epsname = "DEtavParton"; }
	if (j==2) { hname += "dphivg"; epsname = "DPhivParton"; }
	if (j==3) { hname += "drvg"; epsname = "DRvParton";}
      }
      if (i==2) {
	if (j==0) { hname += "dptvg"; epsname = "DPtvGenJet"; }
	if (j==1) { hname += "detavg"; epsname = "DEtavGenJet"; }
	if (j==2) { hname += "dphivg"; epsname = "DPhivGenJet"; }
	if (j==3) { hname += "drvg"; epsname = "DRvGenJet";}
      }
      if (i==3) {
	if (j==0) { hname += "dptvq"; epsname = "DPtvParton"; }
	if (j==1) { hname += "detavq"; epsname = "DEtavParton"; }
	if (j==2) { hname += "dphivq"; epsname = "DPhivParton"; }
	if (j==3) { hname += "drvq"; epsname = "DRvParton";}
      }
      if (i==4) { 
	if (j==0) { hname += "dptvg"; epsname = "DPtvParton"; }
	if (j==1) { hname += "detavg"; epsname = "DEtavParton"; }
	if (j==2) { hname += "dphivg"; epsname = "DPhivParton"; }
	if (j==3) { hname += "drvg"; epsname = "DRvParton";}
      }      
      if (i==5) {
	if (j==0) { hname += "dptvq"; epsname = "DPtvParton"; }
	if (j==1) { hname += "detavq"; epsname = "DEtavParton"; }
	if (j==2) { hname += "dphivq"; epsname = "DPhivParton"; }
	if (j==3) { hname += "drvq"; epsname = "DRvParton";}
      }      

      TH1D *h1 = (TH1D*)f1->Get((dirs+"/"+hname).c_str()); //assert(h1);
      TH1D *h2 = (TH1D*)f2->Get((dirb+"/"+hname).c_str()); //assert(h2);
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
      if (rebin && (i!=0 || j==0)) { h2->Rebin(8); h2->Scale(1./8.); }
      
      //if (j==0) h1->GetXaxis()->SetRangeUser(-0.5,4.5);

      c->Clear();
      c->SetLogx(kFALSE);
      c->SetLogy(kTRUE);
      
      h1->SetLineColor(kRed);
      h1->Draw();
      
      h2->SetLineColor(kBlue);
      h2->Draw("same");
      
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
    if (rebin) { h2->Rebin(8); h2->Scale(1./8.); }

    c->Clear();
    c->SetLogx(kFALSE);
    c->SetLogy(kTRUE);

    h1->SetLineColor(kRed);
    h1->Draw();

    h2->SetLineColor(kBlue);
    h2->Draw("same");

    c->SaveAs((outdir+"/"+fname+"PtoGam.eps").c_str());
  }

  f1->Close();
  f2->Close();
  curdir->cd();

  //gROOT->Reset(); // loses also variables in mk_plots.C
  gROOT->Clear(); // important, otherwise mk_plots.C crashes in third loop

  return 0;
}
