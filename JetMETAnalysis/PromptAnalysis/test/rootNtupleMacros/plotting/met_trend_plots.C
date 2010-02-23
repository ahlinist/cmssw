#include "TStyle.h"

void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  // For the histo:
  tdrStyle->SetHistFillColor(63);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

//  tdrStyle->SetEndErrorSize(0);
//   tdrStyle->SetErrorX(0.);
//  tdrStyle->SetErrorMarker(20);
  
  tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.13);
  tdrStyle->SetPadRightMargin(0.05);

  // For the Global title:

  //  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

  // For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.05);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  // tdrStyle->SetPaperSize(15.,15.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();
}


void setMyStyle() {

   gStyle->Reset("Default");
   gStyle->SetCanvasColor(0);
   gStyle->SetPadColor(0);
   gStyle->SetTitleFillColor(10);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetStatColor(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
   gStyle->SetPadTickY(1);
   gStyle->SetFrameBorderMode(0);
   gStyle->SetPalette(1);
     
   gStyle->SetOptStat(kFALSE);
   gStyle->SetOptTitle(1); // for PAS/AN
//    gStyle->SetPadLeftMargin(0.13);
//    gStyle->SetPadRightMargin(0.07);
//    gStyle->SetStatFont(42);
//    gStyle->SetTitleFont(42);
//    gStyle->SetTitleFont(42, "XYZ");
//    gStyle->SetLabelFont(42, "XYZ");
}

void overlay_trend_plots(const string& fFile0, const string& fFile1, const string& fPlot, const string& fTitle, const string& fYTitle, const string& fName) {
  
   TGraphErrors *g[2];
 
   TFile file0(fFile0.c_str());
   g[0] = (TGraphErrors*)file0.Get(fPlot.c_str()); 

   TFile file1(fFile1.c_str());
   g[1] = (TGraphErrors*)file1.Get(fPlot.c_str());
   
   TCanvas *c = new TCanvas("c","",1200,800);
   c->cd();
  
   g[0]->SetLineWidth(2);
//    g[0]->SetLineStyle(4);
   g[0]->SetLineColor(kRed);
//    g[0]->SetMarkerSize(.6);
//    g[0]->SetMarkerStyle(26);
//    g[0]->SetMarkerColor(kRed);
   g[0]->Draw("ALP");
   g[1]->SetLineWidth(2);
   g[1]->SetLineStyle(7);
   g[1]->SetLineColor(kGreen+2);
//    g[1]->SetMarkerSize(.8);
//    g[1]->SetMarkerStyle(20);
   g[1]->SetMarkerColor(kGreen+2);
   g[1]->Draw("LP");
   
   
   string fileName = fName;
//    c->SaveAs(("g_"+fileName).c_str());
   
   Int_t n = g[0]->GetN();
   
   TH1F *h[2];
   h[0]= new TH1F("h0","h0",n,0,(Float_t)n);
   h[1] = new TH1F("h1","h1",n,0,(Float_t)n);
   
   Int_t j = 0;
   Double_t xAll, yAll, eyAll, xGood, yGood, eyGood;

   for(Int_t i=0; i<n; i++) {

     g[0]->GetPoint(i, xAll, yAll);
     eyAll = g[0]->GetErrorY(i);
     
     g[1]->GetPoint(j, xGood, yGood);
     eyGood = g[1]->GetErrorY(j);
     
     if(xAll==xGood) {
        h[0]->SetBinContent(i+1,yGood);
        h[0]->SetBinError(i+1,eyGood);
      
        h[1]->SetBinContent(i+1,yGood);
        h[1]->SetBinError(i+1,eyGood);
      
        j++;
     } else {
        h[0]->SetBinContent(i+1,yAll);
        h[0]->SetBinError(i+1,eyAll);
     }
     
     h[0]->GetXaxis()->SetBinLabel(i+1,Form("%d",(Int_t)xAll));
   }
   
   h[0]->SetTitle(fTitle.c_str());
   h[0]->GetYaxis()->SetTitle(fYTitle.c_str());
   h[0]->GetXaxis()->SetTitle("Run");
   h[0]->GetXaxis()->SetLabelSize(0.045);
   h[0]->SetLineStyle(2);
   h[0]->SetLineWidth(2);
   h[0]->SetLineColor(kRed);
   h[0]->Draw("EL");
   h[1]->SetLineWidth(2);
   h[1]->SetLineColor(kGreen+2);
   h[1]->Draw("ELsame");
   
   TLegend *legend = new TLegend(.55,.75,.7,.85);
   legend->SetBorderSize(1);
   legend->SetFillColor(0);
   //    legend->SetFillStyle(0);
   legend->AddEntry(h[0],"Bad Runs","l");
   legend->AddEntry(h[1],"Good Runs","l");
   legend->Draw();
   
   TLatex l;
   l.SetTextAlign(12);
   l.SetTextSize(0.04);
   l.SetTextFont(62);
   l.SetNDC();
   l.DrawLatex(0.13,0.83,"CMS 2009 Preliminary");
   
   c->SaveAs(("h_"+fileName).c_str());
   
   delete legend;
   delete c;
}


void trend_plot(const string& fFile0, const string& fPlot, const string& fTitle, const string& fYTitle, const string& fName) {
  
   TGraphErrors *g;
 
   TFile file0(fFile0.c_str());
   g = (TGraphErrors*)file0.Get(fPlot.c_str());
   
//    TCanvas *c = new TCanvas("c","",1200,800);
   TCanvas *c = new TCanvas("c");
   c->cd();
  
   g->SetLineWidth(2);
//    g[0]->SetLineStyle(4);
   g->SetLineColor(kGreen+2);
//    g[0]->SetMarkerSize(.6);
//    g[0]->SetMarkerStyle(26);
//    g[0]->SetMarkerColor(kRed);
//    g->Draw("ALP");
   
   string fileName = fName;
//    c->SaveAs(("g_"+fileName).c_str());
   
   Int_t n = g->GetN()-1;
   
   TH1F *h = new TH1F("h","h",n,0,(Float_t)n);
   
   Double_t xGood, yGood, eyGood;

   for(Int_t i=0; i<n; i++) {

     g->GetPoint(i, xGood, yGood);
     eyGood = g->GetErrorY(i);
     
     h->SetBinContent(i+1,yGood);
     h->SetBinError(i+1,eyGood);
     
     h->GetXaxis()->SetBinLabel(i+1,Form("%d",(Int_t)xGood));
   }
   
   h->SetTitle(fTitle.c_str());
   h->GetYaxis()->SetTitle(fYTitle.c_str());
   h->GetXaxis()->SetTitle("Run Number");
   h->GetXaxis()->SetLabelSize(0.05);
   h->SetTitleOffset(1.,"X");
   h->SetTitleOffset(1.15,"Y");
//    h->SetLineStyle(2);
   h->SetLineWidth(2);
   h->SetLineColor(kGreen+3);
   h->SetMinimum(0.5);
   h->SetMaximum(3.2);
   h->Draw("EL");
   
//    TLegend *legend = new TLegend(.55,.75,.7,.85);
//    legend->SetBorderSize(1);
//    legend->SetFillColor(0);
//    //    legend->SetFillStyle(0);
//    legend->AddEntry(h,"Good Runs","l");
//    legend->Draw();
   
   TLatex l;
   l.SetTextAlign(12);
   l.SetTextSize(0.05);
   l.SetTextFont(62);
   l.SetNDC();
   l.DrawLatex(0.17,0.9,"CMS Preliminary 2009");
   l.DrawLatex(0.17,0.85,"#sqrt{s}=900 GeV");
   
   c->SaveAs(("h_"+fileName).c_str());
   
//    delete legend;
   delete c;
}


void makePlots() {
   // turn on/off batch mode
   gROOT->SetBatch(kTRUE);
   // set ROOT style
//    setMyStyle();
   setTDRStyle();
   gStyle->SetOptTitle(0);
   gStyle->SetPadLeftMargin(0.13);
   gStyle->SetPadRightMargin(0.07);
   //********************************************
   // root files
   //********************************************
   string all_runs = "data/output/Jan23rdReReco_files/output_DATA_AllRuns.root";
   string good_runs = "data/output/Jan23rdReReco_files/output_DATA.root";
   //********************************************
   // make plots
   //********************************************
//    overlay_trend_plots(all_runs, good_runs, "g_caloSumetMean_vs_run", "#scale[0.8]{#sum}E_{T} Mean vs. Run", "#scale[0.8]{#sum}E_{T} Mean [GeV]", "caloSumetMean_vs_run.eps");
//    overlay_trend_plots(all_runs, good_runs, "g_caloSumetRMS_vs_run", "#scale[0.8]{#sum}E_{T} RMS vs. Run", "#scale[0.8]{#sum}E_{T} RMS [GeV]","caloSumetRMS_vs_run.eps");
//    overlay_trend_plots(all_runs, good_runs, "g_calometPtMean_vs_run", "#slash{E}_{T} Mean vs. Run", "#slash{E}_{T} Mean [GeV]", "calometPtMean_vs_run.eps");
//    overlay_trend_plots(all_runs, good_runs, "g_calometPtRMS_vs_run", "#slash{E}_{T} RMS vs. Run", "#slash{E}_{T} RMS [GeV]", "calometPtRMS_vs_run.eps");
//    overlay_trend_plots(all_runs, good_runs, "g_calometPxMean_vs_run", "#mu(#slash{E}_{x}) vs. Run", "#mu(#slash{E}_{x}) [GeV]", "calometPxMean_vs_run.eps");
//    overlay_trend_plots(all_runs, good_runs, "g_calometPxSigma_vs_run", "#sigma(#slash{E}_{x}) vs. Run", "#sigma(#slash{E}_{x}) [GeV]", "calometPxSigma_vs_run.eps");
//    overlay_trend_plots(all_runs, good_runs, "g_calometPyMean_vs_run", "#mu(#slash{E}_{y}) vs. Run", "#mu(#slash{E}_{y}) [GeV]", "calometPyMean_vs_run.eps");
//    overlay_trend_plots(all_runs, good_runs, "g_calometPySigma_vs_run", "#sigma(#slash{E}_{y}) vs. Run", "#sigma(#slash{E}_{y}) [GeV]", "calometPySigma_vs_run.eps");
   
   trend_plot(good_runs, "g_calometPtMean_vs_run", "#slash{E}_{T} Mean vs. Run", "#slash{E}_{T} Mean [GeV]", "calometPtMean_vs_run.eps");
   trend_plot(good_runs, "g_calometPtRMS_vs_run", "#slash{E}_{T} RMS vs. Run", "#slash{E}_{T} RMS [GeV]", "calometPtRMS_vs_run.eps");
}                                              
                                                                                                                                                                                                                                                                                          