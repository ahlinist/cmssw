#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void CSM(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S.root");
  TGraphAsymmErrors *S1;
  S1 = (TGraphAsymmErrors*)gFile->Get("Ups1S");
  
  TString ytitle = TString::Format("d^{2}#sigma/dp_{T}d|y| #times BR(#mu#mu)  (nb/(GeV/c))");
  
  float scale=1.0/0.5; //the feed down fraction factor 

  float Nlo_pt[8]={3,5,10,15,20,25,30,35};
  float Nlo_pt_err[8]={0,0,0,0,0,0,0,0};
  float Nlo_high[8]={0.34,0.205, 0.024,0.0033,0.00066,0.000135,0.000045,0.000014};
  float Nlo_low[8] ={0.135,0.083,0.011,0.00175, 0.00033, 0.000075,0.000025,0.0000085};
  float Nlo_mean[8];
  float Nlo_error[8];
  for (int i=0; i<8; i++){
    Nlo_mean[i]=(Nlo_high[i]+Nlo_low[i])/2.0;
    Nlo_error[i]=(Nlo_high[i]-Nlo_low[i])/2.0;
    Nlo_mean[i]=Nlo_mean[i]*scale;
    Nlo_error[i]=Nlo_error[i]*scale;
  }
  TGraphErrors* NLO = new TGraphErrors(8,Nlo_pt,Nlo_mean,Nlo_pt_err,Nlo_error);
  
  float NNlo_pt[7]={5,10,15,20,25,30,35};
  float NNlo_pt_err[7]={0,0,0,0,0,0,0};
  float NNlo_high[7]={0.51, 0.104, 0.0189, 0.0048, 0.00128,  0.00039, 0.000138};
  float NNlo_low[7] ={0.087,0.0132,0.0027,0.00062,0.000163,0.000061,0.0000217};
  float NNlo_mean[7];
  float NNlo_error[7];
  for (int i=0; i<7; i++){
    NNlo_mean[i]=(NNlo_high[i]+NNlo_low[i])/2.0;
    NNlo_error[i]=(NNlo_high[i]-NNlo_low[i])/2.0;
    NNlo_mean[i]=NNlo_mean[i]*scale;
    NNlo_error[i]=NNlo_error[i]*scale;
  }
  TGraphErrors* NNLO = new TGraphErrors(7,NNlo_pt,NNlo_mean,NNlo_pt_err,NNlo_error);
  
  TCanvas *c2 = new TCanvas();//"c1","title",800,600);
  TH1F *frame = gPad->DrawFrame(0,0.000005,35,2);
  frame->SetStats(0);
  frame->GetXaxis()->SetTitle("p_{T}^{Y} (GeV/c)");
  frame->GetYaxis()->SetTitle("d#sigma/dp_{T} * BR (nb/(GeV/c))");
  frame->GetXaxis()->SetTitle("p_{T}^{Y} (GeV/c)");
  frame->GetYaxis()->SetTitle(ytitle);
  frame->GetYaxis()->SetTitleOffset(1.1);
  frame->Draw();
  
  c2->cd();
  c2->SetLeftMargin(0.15);
  c2->SetLogy();
  frame->Draw();

  S1->Draw("P");
  NLO->SetFillColor(2);
  NLO->SetLineColor(2);
  NLO->Draw("3");

  NNLO->SetFillColor(4);
  NNLO->SetFillStyle(3244);
  NNLO->SetLineColor(4);
  NNLO->Draw("same 3");
  
  TLegend *leg = new TLegend(0.65,0.65,0.89,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);

  leg->AddEntry(NLO, "NLO prediction", "FL");
  leg->AddEntry(NNLO,"NNLO* prediction", "FL");
  leg->Draw();

  
  c1->SaveAs("CSM_NLO_0_2.pdf");
}

void NRQCD(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S.root");
  TGraphAsymmErrors *S1;
  S1 = (TGraphAsymmErrors*)gFile->Get("Ups1S");
  
  TString ytitle = TString::Format("d^{2}#sigma/dp_{T}d|y| #times BR(#mu#mu)  (nb/(GeV/c))");
  
  float Nrqcd_pt[7]={8., 10., 14., 18., 22., 26., 30.};
  float Nrqcd_pt_err[7]={0,0,0,0,0,0,0};
  float Nrqcd_high[7]={0.5814, 0.3467, 0.0991, 0.0326, 0.0131, 0.0064, 0.0035};
  float Nrqcd_low[7] ={0.2704, 0.1824, 0.0515, 0.0127, 0.0040, 0.0015, 0.0007};
  float Nrqcd_mean[7];
  float Nrqcd_error[7];
  for (int i=0; i<7; i++){
    Nrqcd_mean[i]=(Nrqcd_high[i]+Nrqcd_low[i])/2.0;
    Nrqcd_error[i]=(Nrqcd_high[i]-Nrqcd_low[i])/2.0;
  }
  TGraphErrors* Nrqcd = new TGraphErrors(7,Nrqcd_pt,Nrqcd_mean,Nrqcd_pt_err,Nrqcd_error);
  
  TCanvas *c2 = new TCanvas();//"c1","title",800,600);
  TH1F *frame = gPad->DrawFrame(0,0.000005,35,2);
  frame->SetStats(0);
  frame->GetXaxis()->SetTitle("p_{T}^{Y} (GeV/c)");
  frame->GetYaxis()->SetTitle("d#sigma/dp_{T} * BR (nb/(GeV/c))");
  frame->GetXaxis()->SetTitle("p_{T}^{Y} (GeV/c)");
  frame->GetYaxis()->SetTitle(ytitle);
  frame->GetYaxis()->SetTitleOffset(1.1);
  frame->Draw();
  
  c2->cd();
  c2->SetLeftMargin(0.15);
  c2->SetLogy();
  frame->Draw();

  Nrqcd->SetFillColor(3);
  Nrqcd->SetLineColor(3);
  Nrqcd->Draw("3");
  S1->Draw("P");
  TLegend *leg = new TLegend(0.65,0.65,0.89,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);

  leg->AddEntry(Nrqcd, "NRQCD prediction", "FL");
  leg->Draw();

  
  c1->SaveAs("COM_NRQCD_0_2.pdf");
}

float sum_eta20_25[3], sum_eta25_30[3], sum_eta30_35[3], sum_eta35_40[3], sum_eta40_45[3];

void calculateLHCb(){
  ifstream in; 
  in.open("LHCb.dat");
  float eta20_25[15][3], eta25_30[15][3], eta30_35[15][3], eta35_40[15][3], eta40_45[15][3];
  for (int i=0; i<15; i++){
    for (int j=0; j<3; j++){
      in >> eta20_25[i][j];
    }   
    for (int j=0; j<3; j++){
      in >> eta25_30[i][j];
    }   
    for (int j=0; j<3; j++){
      in >> eta30_35[i][j];
    }   
    for (int j=0; j<3; j++){
      in >> eta35_40[i][j];
    }   
    for (int j=0; j<3; j++){
      in >> eta40_45[i][j];
    }   
  }   
  for (int i=0; i<15; i++){
    for (int j=0; j<3; j++){    
      cout<< " " << eta20_25[i][j] << " " << eta25_30[i][j] << " " << eta30_35[i][j] << " " << eta35_40[i][j] << " " << eta40_45[i][j] << " " << endl;
    }   
  }   
  for (int i=0; i<15; i++){
    sum_eta20_25[0] += eta20_25[i][0];
    sum_eta25_30[0] += eta25_30[i][0];
    sum_eta30_35[0] += eta30_35[i][0];
    sum_eta35_40[0] += eta35_40[i][0];
    sum_eta40_45[0] += eta40_45[i][0];
    
    sum_eta20_25[1] += eta20_25[i][1];
    sum_eta25_30[1] += eta25_30[i][1];
    sum_eta30_35[1] += eta30_35[i][1];
    sum_eta35_40[1] += eta35_40[i][1];
    sum_eta40_45[1] += eta40_45[i][1];
    
    sum_eta20_25[2] += eta20_25[i][2];
    sum_eta25_30[2] += eta25_30[i][2];
    sum_eta30_35[2] += eta30_35[i][2];
    sum_eta35_40[2] += eta35_40[i][2];
    sum_eta40_45[2] += eta40_45[i][2];
    
  }   
  for (int i=0; i<3; i++){
    cout << endl << " " << sum_eta20_25[i] << " " << sum_eta25_30[i] << " " << sum_eta30_35[i] << " " << sum_eta35_40[i] << " " << sum_eta40_45[i] << " " << endl;
  }
}

void DrawLHCb(){
	
	TCanvas *c2 = new TCanvas();//"c1","title",800,600);
	TH1F *frame = gPad->DrawFrame(0,0,4.5,130);
	frame->SetStats(0);
	frame->GetXaxis()->SetTitle("|y|^{Y(1S)}");
	frame->GetYaxis()->SetTitle("d#sigma/dy (nb)");
	frame->GetYaxis()->SetTitleOffset(1.1);
	frame->Draw();

	//CMS
	
	
	//LHCb

	calculateLHCb();
	float eta[5]={2.25,2.75,3.25,3.75,4.25};
	float eta_err[5]={0.25,0.25,0.25,0.25,0.25};
	float y[5]={sum_eta20_25[0],sum_eta25_30[0],sum_eta30_35[0],sum_eta35_40[0],sum_eta40_45[0]};
	float y_err_stat[5]={sum_eta20_25[1],sum_eta25_30[1],sum_eta30_35[1],sum_eta35_40[1],sum_eta40_45[1]};
	float y_err_syst[5]={sum_eta20_25[2],sum_eta25_30[2],sum_eta30_35[2],sum_eta35_40[2],sum_eta40_45[2]};
	float y_err[5];
	for (int i=0; i<3; i++){
		y_err[i]=y_err_stat[i]+y_err_syst[i];
	}
	TGraphErrors *LHCb = new TGraphErrors(5, eta, y, eta_err, y_err_syst);
	LHCb->SetMarkerColor(kGreen+1);
	LHCb->SetMarkerStyle(21);
	LHCb->Draw("samePZ");

	leg = new TLegend(0.35,0.75,0.9,0.9);
	leg->AddEntry(LHCb, "LHCb 32.4 pb^{-1} (p_{T}<15 GeV/c)", "PLE");
	leg->Draw();
	TLatex latex1, latex2;
	latex1.DrawLatex(0.5,15,"#Upsilon(1S)");
	latex2.DrawLatex(0.5,30, "#sqrt{s} = 7 TeV");
	c2->SaveAs("LHCb.pdf");
}

void Tevatron(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Final1S.root");
  TGraphAsymmErrors *S1;
  S1 = (TGraphAsymmErrors*)gFile->Get("Ups1S");
  TFile *f = new TFile("Final2S.root");
  TGraphAsymmErrors *S2;
  S2 = (TGraphAsymmErrors*)gFile->Get("Ups2S");  
  TFile *f = new TFile("Final3S.root");
  TGraphAsymmErrors *S3;
  S3 = (TGraphAsymmErrors*)gFile->Get("Ups3S");
  
  // Tevatron
  TFile *f = new TFile("TevatronResults.root");
  TGraphAsymmErrors *CDF1s;  CDF1s = (TGraphAsymmErrors*)gFile->Get("CDF_Y1S");
  TGraphAsymmErrors *D01s;  D01s = (TGraphAsymmErrors*)gFile->Get("D0_Y1S");
  TGraphAsymmErrors *CDF2s;  CDF2s = (TGraphAsymmErrors*)gFile->Get("CDF_Y2S");
  TGraphAsymmErrors *CDF3s;  CDF3s = (TGraphAsymmErrors*)gFile->Get("CDF_Y3S");
  
  double sigma1s(8.339); double sigma2s(2.151); double sigma3s(1.138); 
  double y1s[10]; double y1s_errh[10]; double y1s_errl[10];
  double y2s[10]; double y2s_errh[10]; double y2s_errl[10];
  double y3s[10]; double y3s_errh[10]; double y3s_errl[10];
  for (int i = 0; i < S1->GetN(); ++i) {
    y1s[i] = S1->GetY()[i]/sigma1s; 
    y1s_errh[i] = S1->GetErrorYhigh(i)/sigma1s;
    y1s_errl[i] = S1->GetErrorYlow(i)/sigma1s;
    
    y2s[i] = S2->GetY()[i]/sigma2s; 
    y2s_errh[i] = S2->GetErrorYhigh(i)/sigma2s;
    y2s_errl[i] = S2->GetErrorYlow(i)/sigma2s;
    
    y3s[i] = S3->GetY()[i]/sigma3s; 
    y3s_errh[i] = S3->GetErrorYhigh(i)/sigma3s;
    y3s_errl[i] = S3->GetErrorYlow(i)/sigma3s;
  }
  
  double xbin[10] = {1., 3., 6., 9., 11.5, 14.5, 18., 22.5, 27.5, 40.};
  double xh[10] = {1., 1., 2., 1., 1.5, 1.5, 2., 2.5, 2.5, 10.};
  double xl[10] = {1., 1., 2., 1., 1.5, 1.5, 2., 2.5, 2.5, 10.};
  S1_norm = new TGraphAsymmErrors(10,xbin,y1s,xl,xh,y1s_errl,y1s_errh);
  S2_norm = new TGraphAsymmErrors(10,xbin,y2s,xl,xh,y2s_errl,y2s_errh);
  S3_norm = new TGraphAsymmErrors(10,xbin,y3s,xl,xh,y3s_errl,y3s_errh);
  
  TCanvas *c1 = new TCanvas("c1", "c1", 800,600);
  c1->SetLogy();
  CDF1s->GetXaxis()->SetTitleOffset(0.85);
  CDF1s->GetYaxis()->SetTitleOffset(0.95);
  CDF1s->GetXaxis()->SetTitle("p_{T}^{#Upsilon}");
  CDF1s->GetYaxis()->SetTitle("(d#sigma/dp_{T})/#sigma_{TOT}");
  CDF1s->SetTitle("");
  CDF1s->SetMinimum(0.001);
  CDF1s->SetMaximum(0.2);
  S1_norm->SetMarkerColor(2); CDF1s->SetMarkerColor(4); D01s->SetMarkerColor(3); 
  S1_norm->SetMarkerStyle(21); CDF1s->SetMarkerStyle(21); D01s->SetMarkerStyle(21);
  CDF1s->Draw("AP");
  D01s->Draw("P");
  S1_norm->Draw("P");
  legg = new TLegend(0.5,0.7,0.7,0.9);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("         #Upsilon(1S)");
  legge = legg->AddEntry(S1_norm, "CMS, |y| < 2.4, #sqrt{s} = 7 TeV" ,"p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(CDF1s,   "CDF, |y| < 0.4, #sqrt{s} = 1.96 TeV" ,"p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(D01s,    "D0 , |y| < 1.8, #sqrt{s} = 1.8 TeV" ,"p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c1->SaveAs("Tevatron1S.pdf");
  
  TCanvas *c2 = new TCanvas("c2", "c2", 800,600);
  c2->SetLogy();
  CDF2s->GetXaxis()->SetTitleOffset(0.85);
  CDF2s->GetYaxis()->SetTitleOffset(0.95);
  CDF2s->GetXaxis()->SetTitle("p_{T}^{#Upsilon}");
  CDF2s->GetYaxis()->SetTitle("(d#sigma/dp_{T})/#sigma_{TOT}");
  CDF2s->SetTitle("");
  CDF2s->SetMinimum(0.001);
  CDF2s->SetMaximum(0.2);
  S2_norm->SetMarkerColor(2); CDF2s->SetMarkerColor(4); 
  S2_norm->SetMarkerStyle(21); CDF2s->SetMarkerStyle(21);
  CDF2s->Draw("AP");
  S2_norm->Draw("P");
  legg = new TLegend(0.5,0.7,0.7,0.9);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("         #Upsilon(2S)");
  legge = legg->AddEntry(S1_norm, "CMS, |y| < 2.4, #sqrt{s} = 7 TeV" ,"p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(CDF1s,   "CDF, |y| < 0.4, #sqrt{s} = 1.96 TeV" ,"p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c2->SaveAs("Tevatron2S.pdf");
  
  TCanvas *c3 = new TCanvas("c3", "c3", 800,600);
  c3->SetLogy();
  CDF3s->GetXaxis()->SetTitleOffset(0.85);
  CDF3s->GetYaxis()->SetTitleOffset(0.95);
  CDF3s->GetXaxis()->SetTitle("p_{T}^{#Upsilon}");
  CDF3s->GetYaxis()->SetTitle("(d#sigma/dp_{T})/#sigma_{TOT}");
  CDF3s->SetTitle("");
  CDF3s->SetMinimum(0.001);
  CDF3s->SetMaximum(0.2);
  S3_norm->SetMarkerColor(2); CDF3s->SetMarkerColor(4); 
  S3_norm->SetMarkerStyle(21); CDF3s->SetMarkerStyle(21);
  CDF3s->Draw("AP");
  S3_norm->Draw("P");
  legg = new TLegend(0.5,0.7,0.7,0.9);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("         #Upsilon(3S)");
  legge = legg->AddEntry(S1_norm, "CMS, |y| < 2.4, #sqrt{s} = 7 TeV" ,"p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(CDF1s,   "CDF, |y| < 0.4, #sqrt{s} = 1.96 TeV" ,"p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c2->SaveAs("Tevatron3S.pdf"); 
  
  
}

