#include <TFile.h>
#include <TTree.h>
//#include <TH1F.h>
//#include <TH1I.h>
#include <TCut.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <utility>
#include <TROOT.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCut.h>
#include <TGraphAsymmErrors.h>
#include <TBranch.h>
#include <math.h>
#include <map>
#include <iomanip>
#include "TH1.h"
#include "TH2.h"
#include <TLine.h>
#include "TProfile.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TVectorF.h"

int main (){
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("./pixelhitdata.root");
      if (!f) {
         std::cout <<"Did not find pixelhitdata.root file\n";
         f = new TFile("./pixelhitdata.root");
      }
      f->cd("SLHCPixelHitAnalyzer");
      std::cout <<"Looking for PixNtuple tree\n";
      TTree* tree = (TTree*)gDirectory->Get("PixNtuple");
      if (!tree) {
         std::cout <<"Did not find PixNtuple tree\n";
      }
  Long64_t nentries = tree->GetEntriesFast();
  std::cout <<"There are NEntries = "<<nentries<<"\n";

  TCanvas MyCanvas("MyCanvas", "New Geometry");
  gStyle->SetErrorX(0);

  // Create and fill the 2d histograms of hit residuals vs abs(eta)

  // For BPIX layer 1 only
  TH2D *bh2x = new TH2D("bh2x", "BPIX x hit resolution vs eta", 20, 0.0, 2.5, 200, -200., 200.);
  TH2D *bh2y = new TH2D("bh2y", "BPIX y hit resolution vs eta", 20, 0.0, 2.5, 200, -200., 200.);
  tree->Draw("(sim.localX-RecHit.localX)*10000.0:abs(sim.globalEta) >> bh2x", 
             "Cluster.edgeHitX==0&&Cluster.edgeHitY==0&&det.layer==1&&sim.globalEta>-5000", 0, nentries);
  tree->Draw("(sim.localY-RecHit.localY)*10000.0:abs(sim.globalEta) >> bh2y", 
             "Cluster.edgeHitX==0&&Cluster.edgeHitY==0&&det.layer==1&&sim.globalEta>-5000", 0, nentries);

  bh2x->SetYTitle("Hit Resolution (#mum)");
  bh2x->SetXTitle("#eta");
  bh2x->SetTitle("Hit Resolution vs eta");
  bh2y->SetYTitle("Hit Resolution (#mum)");
  bh2y->SetXTitle("#eta");
  bh2y->SetTitle("Hit Resolution vs eta");

  // For FPIX disk 1 only
  TH2D *fh2x = new TH2D("fh2x", "FPIX x hit resolution vs eta", 20, 0.0, 2.5, 200, -200., 200.);
  TH2D *fh2y = new TH2D("fh2y", "FPIX y hit resolution vs eta", 20, 0.0, 2.5, 200, -200., 200.);
  tree->Draw("(sim.localX-RecHit.localX)*10000.0:abs(sim.globalEta) >> fh2x", 
             "Cluster.edgeHitX==0&&Cluster.edgeHitY==0&&det.disk==1&&sim.globalEta>-5000", 0, nentries);
  tree->Draw("(sim.localY-RecHit.localY)*10000.0:abs(sim.globalEta) >> fh2y", 
             "Cluster.edgeHitX==0&&Cluster.edgeHitY==0&&det.disk==1&&sim.globalEta>-5000", 0, nentries);

  fh2x->SetYTitle("Hit Resolution (#mum)");
  fh2x->SetXTitle("#eta");
  fh2x->SetTitle("Hit Resolution vs eta");
  fh2y->SetYTitle("Hit Resolution (#mum)");
  fh2y->SetXTitle("#eta");
  fh2y->SetTitle("Hit Resolution vs eta");

  //
  // get slices in eta and fit residuals then plot each histo and fit
  //
  std::vector<TH1D*> bpix_x, bpix_y;
  std::vector<float> bpix_x_fitmean, bpix_x_fitsigma;
  std::vector<float> bpix_y_fitmean, bpix_y_fitsigma;
  std::vector<float> bpix_x_fitmeanerr, bpix_x_fitsigmaerr;
  std::vector<float> bpix_y_fitmeanerr, bpix_y_fitsigmaerr;
  std::vector<float> bpix_x_mean, bpix_x_rms;
  std::vector<float> bpix_x_meanerr, bpix_x_rmserr;
  std::vector<float> bpix_y_mean, bpix_y_rms;
  std::vector<float> bpix_y_meanerr, bpix_y_rmserr;
  int color = 2;
  int num_xbins_inx = bh2x->GetNbinsX();
  TVectorF etamean(num_xbins_inx), etaerr(num_xbins_inx);
  std::cout <<"Fitting BPix x hit residuals \n";
  for (int i=1; i<=num_xbins_inx; i++) {
    bpix_x.push_back(new TH1D(*(bh2x->ProjectionY(" ",i,i))));
    bpix_x[i-1]->SetXTitle("RecHit X Residual");
    bpix_x[i-1]->Fit("gaus","Q");
    bpix_x[i-1]->GetFunction("gaus")->SetLineWidth(1);
    bpix_x[i-1]->GetFunction("gaus")->SetLineColor(color);
    bpix_x_fitmean.push_back(bpix_x[i-1]->GetFunction("gaus")->GetParameter(1));
    bpix_x_fitsigma.push_back(bpix_x[i-1]->GetFunction("gaus")->GetParameter(2));
    bpix_x_fitmeanerr.push_back(bpix_x[i-1]->GetFunction("gaus")->GetParError(1));
    bpix_x_fitsigmaerr.push_back(bpix_x[i-1]->GetFunction("gaus")->GetParError(2));
    bpix_x_mean.push_back(bpix_x[i-1]->GetMean());
    bpix_x_meanerr.push_back(bpix_x[i-1]->GetMeanError());
    bpix_x_rms.push_back(bpix_x[i-1]->GetRMS());
    bpix_x_rmserr.push_back(bpix_x[i-1]->GetRMSError());
    etamean[i-1]=bh2x->GetBinCenter(i);
    etaerr[i-1]=0.0;
    std::cout <<"BPix X eta bin "<<i<<" fit mean " << bpix_x_fitmean[i-1] 
              << " fit sigma = " << bpix_x_fitsigma[i-1]<<std::endl;
  }
  MyCanvas.Clear();
  MyCanvas.Divide(7,3);
  int ipad = 1;
  for (int i=1; i<=num_xbins_inx; i++) {
    MyCanvas.cd(ipad);bpix_x[i-1]->Draw();
    ++ipad;
    //std::cout <<"Drawing "<<i<<" mean " << bpix_x[i-1]->GetMean() << " error " << bpix_x[i-1]->GetRMS()<<std::endl;
    if(i%21 == 0) {
      MyCanvas.Print("bpix_xhitpulls_0.eps");
      MyCanvas.Clear();
      MyCanvas.Divide(7,3);
      ipad = 1;
    }
  }
  MyCanvas.Print("bpix_xhitpulls.eps");

/* ---------------------------------------------------- */
  // get plots of hit resolution vs eta, fit sigmas and RMS

  MyCanvas.Clear();
  TVectorF bx_resmean(bpix_x_fitsigma.size(), &bpix_x_fitsigma[0]);
  TVectorF bx_reserr(bpix_x_fitsigmaerr.size(), &bpix_x_fitsigmaerr[0]);
  TGraphErrors bpix_tgx(etamean, bx_resmean, etaerr, bx_reserr);
  //bpix_tgx.Print();
  bpix_tgx.SetTitle("BPIX Transverse Hit Resolution vs eta");
  bpix_tgx.SetMarkerColor(4);
  bpix_tgx.SetMarkerStyle(20);
  bpix_tgx.GetYaxis()->SetRangeUser(0.0, 30.0);
  bpix_tgx.Draw("AP");
  // plot RMS on the same plot
  TVectorF bx_resrms(bpix_x_rms.size(), &bpix_x_rms[0]);
  TVectorF bx_resrmserr(bpix_x_rmserr.size(), &bpix_x_rmserr[0]);
  TGraphErrors bpix_rmstgx(etamean, bx_resrms, etaerr, bx_resrmserr);
  //bpix_tgx.Print();
  bpix_rmstgx.SetTitle("BPIX Transverse Hit Resolution vs eta");
  bpix_rmstgx.SetMarkerColor(2);
  bpix_rmstgx.SetMarkerStyle(21);
  bpix_rmstgx.Draw("P");
  //
  TLegend *leg = new TLegend(0.55,0.75,0.85,0.85);
  bpix_tgx.GetHistogram()->SetMarkerColor(4);
  bpix_tgx.GetHistogram()->SetMarkerStyle(20);
  bpix_rmstgx.GetHistogram()->SetMarkerColor(2);
  bpix_rmstgx.GetHistogram()->SetMarkerStyle(21);
  leg->AddEntry(bpix_tgx.GetHistogram(),"BPIX Hit Resolution (fit sigma)","p");
  leg->AddEntry(bpix_rmstgx.GetHistogram(),"BPIX Hit Resolution (RMS)","p");
  leg->Draw();
  MyCanvas.Print("bpix_xhitres.eps");

/* ---------------------------------------------------- */
  MyCanvas.Clear();

  int num_ybins_inx = bh2y->GetNbinsX();
  std::cout <<"Fitting BPix y hit residuals \n";
  for (int i=1; i<=num_ybins_inx; i++) {
    bpix_y.push_back(new TH1D(*(bh2y->ProjectionY(" ",i,i))));
    bpix_y[i-1]->SetXTitle("RecHit Y Residual");
    bpix_y[i-1]->Fit("gaus","Q");
    bpix_y[i-1]->GetFunction("gaus")->SetLineWidth(1);
    bpix_y[i-1]->GetFunction("gaus")->SetLineColor(color);
    bpix_y_fitmean.push_back(bpix_y[i-1]->GetFunction("gaus")->GetParameter(1));
    bpix_y_fitsigma.push_back(bpix_y[i-1]->GetFunction("gaus")->GetParameter(2));
    bpix_y_fitmeanerr.push_back(bpix_y[i-1]->GetFunction("gaus")->GetParError(1));
    bpix_y_fitsigmaerr.push_back(bpix_y[i-1]->GetFunction("gaus")->GetParError(2));
    bpix_y_mean.push_back(bpix_y[i-1]->GetMean());
    bpix_y_meanerr.push_back(bpix_y[i-1]->GetMeanError());
    bpix_y_rms.push_back(bpix_y[i-1]->GetRMS());
    bpix_y_rmserr.push_back(bpix_y[i-1]->GetRMSError());
    std::cout <<"BPix X eta bin "<<i<<" fit mean " << bpix_y_fitmean[i-1] 
              << " fit sigma = " << bpix_y_fitsigma[i-1]<<std::endl ; //<< "\n";
  }
  MyCanvas.Clear();
  MyCanvas.Divide(7,3);
  ipad = 1;
  for (int i=1; i<=num_ybins_inx; i++) {
    MyCanvas.cd(ipad);bpix_y[i-1]->Draw();
    ++ipad;
    //std::cout <<"Drawing "<<i<<" mean " << bpix_y[i-1]->GetMean() << " error " << bpix_y[i-1]->GetRMS()<<std::endl;
    if(i%21 == 0) {
      MyCanvas.Print("bpix_yhitpulls_0.eps");
      MyCanvas.Clear();
      MyCanvas.Divide(7,3);
      ipad = 1;
    }
  }
  MyCanvas.Print("bpix_yhitpulls.eps");

/* ---------------------------------------------------- */
  // get plots of hit resolution vs eta

  MyCanvas.Clear();
  TVectorF by_resmean(bpix_y_fitsigma.size(), &bpix_y_fitsigma[0]);
  TVectorF by_reserr(bpix_y_fitsigmaerr.size(), &bpix_y_fitsigmaerr[0]);
  TGraphErrors bpix_tgy(etamean, by_resmean, etaerr, by_reserr);
  //bpix_tgy.Print();
  bpix_tgy.SetTitle("BPIX Longitudinal Hit Resolution vs eta");
  bpix_tgy.SetMarkerColor(4);
  bpix_tgy.SetMarkerStyle(20);
  bpix_tgy.GetYaxis()->SetRangeUser(0.0, 45.0);
  bpix_tgy.Draw("AP");
  // plot RMS on the same plot
  TVectorF by_resrms(bpix_y_rms.size(), &bpix_y_rms[0]);
  TVectorF by_resrmserr(bpix_y_rmserr.size(), &bpix_y_rmserr[0]);
  TGraphErrors bpix_rmstgy(etamean, by_resrms, etaerr, by_resrmserr);
  bpix_rmstgy.SetTitle("BPIX Longitudinal Hit Resolution vs eta");
  bpix_rmstgy.SetMarkerColor(2);
  bpix_rmstgy.SetMarkerStyle(21);
  bpix_rmstgy.Draw("P");
  //
  delete leg;
  leg = new TLegend(0.55,0.75,0.85,0.85);
  bpix_tgy.GetHistogram()->SetMarkerColor(4);
  bpix_tgy.GetHistogram()->SetMarkerStyle(20);
  bpix_rmstgy.GetHistogram()->SetMarkerColor(2);
  bpix_rmstgy.GetHistogram()->SetMarkerStyle(21);
  leg->AddEntry(bpix_tgy.GetHistogram(),"BPIX Hit Resolution (fit sigma)","p");
  leg->AddEntry(bpix_rmstgy.GetHistogram(),"BPIX Hit Resolution (RMS)","p");
  leg->Draw();
  MyCanvas.Print("bpix_yhitres.eps");

/* ---------------------------------------------------- */
  // get slices in eta and fit residuals then plot each histo and fit
  std::vector<TH1D*> fpix_x, fpix_y;
  std::vector<float> fpix_x_fitmean, fpix_x_fitsigma;
  std::vector<float> fpix_y_fitmean, fpix_y_fitsigma;
  std::vector<float> fpix_x_fitmeanerr, fpix_x_fitsigmaerr;
  std::vector<float> fpix_y_fitmeanerr, fpix_y_fitsigmaerr;
  std::vector<float> fpix_x_mean, fpix_x_rms;
  std::vector<float> fpix_x_meanerr, fpix_x_rmserr;
  std::vector<float> fpix_y_mean, fpix_y_rms;
  std::vector<float> fpix_y_meanerr, fpix_y_rmserr;
  color = 2;
  num_xbins_inx = fh2x->GetNbinsX();
  int num_good = 0;
  int i_start = -1;
  TVectorF fetamean(num_xbins_inx), fetaerr(num_xbins_inx);
  std::cout <<"Fitting FPix x hit residuals \n";
  for (int i=1; i<=num_xbins_inx; i++) {
    fpix_x.push_back(new TH1D(*(fh2x->ProjectionY(" ",i,i))));
    // check if empty
    if(fpix_x[i-1]->GetEntries() > 20) {
      fpix_x[i-1]->SetXTitle("RecHit X Residual");
      fpix_x[i-1]->Fit("gaus","Q");
      fpix_x[i-1]->GetFunction("gaus")->SetLineWidth(1);
      fpix_x[i-1]->GetFunction("gaus")->SetLineColor(color);
      fpix_x_fitmean.push_back(fpix_x[i-1]->GetFunction("gaus")->GetParameter(1));
      fpix_x_fitsigma.push_back(fpix_x[i-1]->GetFunction("gaus")->GetParameter(2));
      fpix_x_fitmeanerr.push_back(fpix_x[i-1]->GetFunction("gaus")->GetParError(1));
      fpix_x_fitsigmaerr.push_back(fpix_x[i-1]->GetFunction("gaus")->GetParError(2));
      fpix_x_mean.push_back(fpix_x[i-1]->GetMean());
      fpix_x_meanerr.push_back(fpix_x[i-1]->GetMeanError());
      fpix_x_rms.push_back(fpix_x[i-1]->GetRMS());
      fpix_x_rmserr.push_back(fpix_x[i-1]->GetRMSError());
      std::cout <<"FPix X eta bin "<<i<<" fit mean " << fpix_x_fitmean[i-1] 
                << " fit sigma = " << fpix_x_fitsigma[i-1]<<std::endl;
      fetamean[num_good] = fh2x->GetBinCenter(i);
      fetaerr[num_good] = 0.0;
      ++num_good;
      if(i_start == -1) i_start = i-1;
    } else {
      fpix_x_fitmean.push_back(-9999.0);
      fpix_x_fitsigma.push_back(-9999.0);
      fpix_x_fitmeanerr.push_back(-9999.0);
      fpix_x_fitsigmaerr.push_back(-9999.0);
      fpix_x_mean.push_back(-9999.0);
      fpix_x_meanerr.push_back(-9999.0);
      fpix_x_rms.push_back(-9999.0);
      fpix_x_rmserr.push_back(-9999.0);
    }
  }
  MyCanvas.Clear();
  MyCanvas.Divide(4,3);
  int ican = 0;
  for (int i=1; i<=num_xbins_inx; i++) {
    if(fpix_x[i-1]->GetEntries() > 20) {
      ++ican;
      MyCanvas.cd(ican);fpix_x[i-1]->Draw();
      //std::cout <<"Drawing "<<ican<<" mean " << fpix_x[i-1]->GetMean() << " error " << fpix_x[i-1]->GetRMS()<<std::endl;
    }
  }
  MyCanvas.Print("fpix_xhitpulls.eps");

/* ---------------------------------------------------- */
  // get plots of hit resolution vs eta

  MyCanvas.Clear();
  // dummy histogram to plot axis I want
  TH1F dummyh1("dummyh1","FPIX Transverse Hit Resolution vs eta",20,0.0,2.5);
  dummyh1.GetYaxis()->SetRangeUser(0.0, 30.0);
  dummyh1.GetXaxis()->SetRangeUser(0.0, 2.5);
  dummyh1.SetStats(0);
  dummyh1.Draw();
  TVectorF fx_resmean(num_good, &fpix_x_fitsigma[i_start]);
  TVectorF fx_reserr(num_good, &fpix_x_fitsigmaerr[i_start]);
  TGraphErrors fpix_tgx(fetamean, fx_resmean, fetaerr, fx_reserr);
  //fpix_tgx.Print();
  fpix_tgx.SetTitle("FPIX Transverse Hit Resolution vs eta");
  fpix_tgx.SetMarkerColor(4);
  fpix_tgx.SetMarkerStyle(20);
  fpix_tgx.GetYaxis()->SetRangeUser(0.0, 30.0);
  fpix_tgx.GetXaxis()->SetRangeUser(0.0, 2.5);
  fpix_tgx.Draw("P");
  // plot RMS on the same plot
  TVectorF fx_resrms(num_good, &fpix_x_rms[i_start]);
  TVectorF fx_resrmserr(num_good, &fpix_x_rmserr[i_start]);
  TGraphErrors fpix_rmstgx(fetamean, fx_resrms, fetaerr, fx_resrmserr);
  fpix_rmstgx.SetTitle("FPIX Transverse Hit Resolution vs eta");
  fpix_rmstgx.SetMarkerColor(2);
  fpix_rmstgx.SetMarkerStyle(21);
  fpix_rmstgx.Draw("P");
  //
  bpix_tgx.SetMarkerColor(7);
  bpix_tgx.SetMarkerStyle(22);
  bpix_tgx.Draw("P");
  //
  delete leg;
  leg = new TLegend(0.55,0.75,0.85,0.85);
  fpix_tgx.GetHistogram()->SetMarkerColor(4);
  fpix_tgx.GetHistogram()->SetMarkerStyle(20);
  fpix_rmstgx.GetHistogram()->SetMarkerColor(2);
  fpix_rmstgx.GetHistogram()->SetMarkerStyle(21);
  bpix_tgx.GetHistogram()->SetMarkerColor(7);
  bpix_tgx.GetHistogram()->SetMarkerStyle(22);
  leg->AddEntry(fpix_tgx.GetHistogram(),"FPIX Hit Resolution (fit sigma)","p");
  leg->AddEntry(fpix_rmstgx.GetHistogram(),"FPIX Hit Resolution (RMS)","p");
  leg->AddEntry(bpix_tgx.GetHistogram(),"BPIX Hit Resolution (fit sigma)","p");
  leg->Draw();
  MyCanvas.Print("fpix_xhitres.eps");

/* ---------------------------------------------------- */
  MyCanvas.Clear();

  num_ybins_inx = fh2y->GetNbinsX();
  num_good = 0;
  i_start = -1;
  std::cout <<"Fitting FPix y hit residuals \n";
  for (int i=1; i<=num_ybins_inx; i++) {
    fpix_y.push_back(new TH1D(*(fh2y->ProjectionY(" ",i,i))));
    // check if empty
    if(fpix_y[i-1]->GetEntries() > 20) {
      fpix_y[i-1]->SetXTitle("RecHit Y Residual");
      fpix_y[i-1]->Fit("gaus","Q");
      fpix_y[i-1]->GetFunction("gaus")->SetLineWidth(1);
      fpix_y[i-1]->GetFunction("gaus")->SetLineColor(color);
      fpix_y_fitmean.push_back(fpix_y[i-1]->GetFunction("gaus")->GetParameter(1));
      fpix_y_fitsigma.push_back(fpix_y[i-1]->GetFunction("gaus")->GetParameter(2));
      fpix_y_fitmeanerr.push_back(fpix_y[i-1]->GetFunction("gaus")->GetParError(1));
      fpix_y_fitsigmaerr.push_back(fpix_y[i-1]->GetFunction("gaus")->GetParError(2));
      fpix_y_mean.push_back(fpix_y[i-1]->GetMean());
      fpix_y_meanerr.push_back(fpix_y[i-1]->GetMeanError());
      fpix_y_rms.push_back(fpix_y[i-1]->GetRMS());
      fpix_y_rmserr.push_back(fpix_y[i-1]->GetRMSError());
      std::cout <<"FPix X eta bin "<<i<<" fit mean " << fpix_y_fitmean[i-1] 
                << " fit sigma = " << fpix_y_fitsigma[i-1] <<std::endl; //<< "\n";
      fetamean[num_good] = fh2y->GetBinCenter(i);
      fetaerr[num_good] = 0.0;
      ++num_good;
      if(i_start == -1) i_start = i-1;
    } else {
      fpix_y_fitmean.push_back(-9999.0);
      fpix_y_fitsigma.push_back(-9999.0);
      fpix_y_fitmeanerr.push_back(-9999.0);
      fpix_y_fitsigmaerr.push_back(-9999.0);
      fpix_y_mean.push_back(-9999.0);
      fpix_y_meanerr.push_back(-9999.0);
      fpix_y_rms.push_back(-9999.0);
      fpix_y_rmserr.push_back(-9999.0);
    }
  }
  MyCanvas.Clear();
  MyCanvas.Divide(4,3);
  ican = 0;
  for (int i=1; i<=num_ybins_inx; i++) {
    if(fpix_y[i-1]->GetEntries() > 20) {
      ++ican;
      MyCanvas.cd(ican);fpix_y[i-1]->Draw();
      //std::cout <<"Drawing "<<ican<<" mean " << fpix_y[i-1]->GetMean() << " error " << fpix_y[i-1]->GetRMS()<<std::endl;
    }
  }
  MyCanvas.Print("fpix_yhitpulls.eps");

/* ---------------------------------------------------- */
  // get plots of hit resolution vs eta

  MyCanvas.Clear();
  // dummy histogram to plot axis I want
  dummyh1.SetTitle("FPIX Longitudinal Hit Resolution vs eta");
  dummyh1.GetYaxis()->SetRangeUser(0.0, 45.0);
  dummyh1.GetXaxis()->SetRangeUser(0.0, 2.5);
  dummyh1.SetStats(0);
  dummyh1.Draw();
  TVectorF fy_resmean(num_good, &fpix_y_fitsigma[i_start]);
  TVectorF fy_reserr(num_good, &fpix_y_fitsigmaerr[i_start]);
  TGraphErrors fpix_tgy(fetamean, fy_resmean, fetaerr, fy_reserr);
  //fpix_tgy.Print();
  fpix_tgy.SetTitle("FPIX Transverse Hit Resolution vs eta");
  fpix_tgy.SetMarkerColor(4);
  fpix_tgy.SetMarkerStyle(20);
  fpix_tgy.GetYaxis()->SetRangeUser(0.0, 45.0);
  fpix_tgy.GetXaxis()->SetRangeUser(0.0, 2.5);
  fpix_tgy.Draw("P");
  // plot RMS on the same plot
  TVectorF fy_resrms(num_good, &fpix_y_rms[i_start]);
  TVectorF fy_resrmserr(num_good, &fpix_y_rmserr[i_start]);
  TGraphErrors fpix_rmstgy(fetamean, fy_resrms, fetaerr, fy_resrmserr);
  fpix_rmstgy.SetTitle("FPIX Longitudinal Hit Resolution vs eta");
  fpix_rmstgy.SetMarkerColor(2);
  fpix_rmstgy.SetMarkerStyle(21);
  fpix_rmstgy.Draw("P");
  //
  bpix_tgy.SetMarkerColor(7);
  bpix_tgy.SetMarkerStyle(22);
  bpix_tgy.Draw("P");
  //
  delete leg;
  leg = new TLegend(0.55,0.75,0.85,0.85);
  fpix_tgy.GetHistogram()->SetMarkerColor(4);
  fpix_tgy.GetHistogram()->SetMarkerStyle(20);
  fpix_rmstgy.GetHistogram()->SetMarkerColor(2);
  fpix_rmstgy.GetHistogram()->SetMarkerStyle(21);
  bpix_tgy.GetHistogram()->SetMarkerColor(7);
  bpix_tgy.GetHistogram()->SetMarkerStyle(22);
  leg->AddEntry(fpix_tgy.GetHistogram(),"FPIX Hit Resolution (fit sigma)","p");
  leg->AddEntry(fpix_rmstgy.GetHistogram(),"FPIX Hit Resolution (RMS)","p");
  leg->AddEntry(bpix_tgy.GetHistogram(),"BPIX Hit Resolution (fit sigma)","p");
  leg->Draw();
  MyCanvas.Print("fpix_yhitres.eps");

// Now plot the hit residual fit sigmas (hit resolution) vs abs(eta)

std::cout << "\nMy program ran!\n\a";
return 1;
}

