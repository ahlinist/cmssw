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
   gStyle->SetOptFit(1);
   gStyle->SetOptTitle(0);
//    gStyle->SetPadLeftMargin(0.13);
//    gStyle->SetPadRightMargin(0.07);
//    gStyle->SetStatFont(42);
//    gStyle->SetTitleFont(42);
//    gStyle->SetTitleFont(42, "XYZ");
//    gStyle->SetLabelFont(42, "XYZ");
}


void overlay_plots(const string& fFile0, const string& fFile1, const string& fPlot0, const string& fPlot1, const double fXmin, const double fXmax, const double fYmin, const double fYmax,const string& fXAxisLabel, const string& fYAxisLabel, const string& fTitle, const string& fName, const int doFit) {
  
   gStyle->SetOptStat(kFALSE);
 
   TH2F *h2[4];
   TH1F *h[2];
   
   TFile file0(fFile0.c_str());
   h2[0] = (TH2F*)file0.Get(fPlot0.c_str()); 
   h2[1] = (TH2F*)file0.Get(fPlot1.c_str()); 
   
   h2[0]->Add(h2[1],1.0);
   
   TFile file1(fFile1.c_str());
   h2[2] = (TH2F*)file1.Get(fPlot0.c_str()); 
   h2[3] = (TH2F*)file1.Get(fPlot1.c_str()); 
   
   h2[2]->Add(h2[3],1.0);
   
   h[0] = new TH1F("h0","h0",h2[0]->GetXaxis()->GetNbins(),h2[0]->GetXaxis()->GetXmin(),h2[0]->GetXaxis()->GetXmax());
   h[1] = new TH1F("h1","h1",h2[2]->GetXaxis()->GetNbins(),h2[2]->GetXaxis()->GetXmin(),h2[2]->GetXaxis()->GetXmax());
   
//    for(Int_t i=1; i<=h2[0]->GetNbinsX(); i++) {
     for(Int_t i=1; i<=14; i++) {

     TH1D* h_temp = h2[0]->ProjectionY("h_temp", i, i, "e");

     Int_t ngroup = 1;

     if( fTitle.find("900") != string::npos ) {
       if(i==10) ngroup = 2;
       else if(i==11) ngroup = 4;
       else if(i==12) ngroup = 4;
     } else if( fTitle.find("2360") != string::npos ) {
       if(i==10) ngroup = 4;
       else if(i==11) ngroup = 4;
       else if(i==12) ngroup = 4;
       else if(i==13) ngroup = 4;
       else if(i==14) ngroup = 8;
     }
     
     h_temp->Rebin(ngroup);
     
     TF1* f1= new TF1("f1", "gaus", h_temp->GetXaxis()->GetXmin(), h_temp->GetXaxis()->GetXmax());
     f1->SetParameters(h_temp->GetBinContent(h_temp->GetMaximumBin()),h_temp->GetMean(),h_temp->GetRMS());
     
     h_temp->Fit("f1","R", "",h_temp->GetXaxis()->GetXmin(),h_temp->GetXaxis()->GetXmax());
  
     h[0]->SetBinContent(i, f1->GetParameter(2));
     h[0]->SetBinError(i, f1->GetParError(2));
      
     delete f1;
     delete h_temp;
   }
   
//    for(Int_t i=1; i<=h2[2]->GetNbinsX(); i++) {
   for(Int_t i=1; i<=14; i++) {
    
     TH1D* h_temp = h2[2]->ProjectionY("h_temp", i, i, "e");
     
     Int_t ngroup = 1;

     if( fTitle.find("900") != string::npos ) {
       if(i==10) ngroup = 2;
       else if(i==11) ngroup = 4;
       else if(i==12) ngroup = 4;
     } else if( fTitle.find("2360") != string::npos ) {
       if(i==10) ngroup = 4;
       else if(i==11) ngroup = 4;
       else if(i==12) ngroup = 4;
       else if(i==13) ngroup = 4;
       else if(i==14) ngroup = 8;
     }
     
     h_temp->Rebin(ngroup);
  
     TF1* f1= new TF1("f1", "gaus", h_temp->GetXaxis()->GetXmin(), h_temp->GetXaxis()->GetXmax());
     f1->SetParameters(h_temp->GetBinContent(h_temp->GetMaximumBin()),h_temp->GetMean(),h_temp->GetRMS());
     h_temp->Fit("f1","R", "",h_temp->GetXaxis()->GetXmin(),h_temp->GetXaxis()->GetXmax());
  
     h[1]->SetBinContent(i, f1->GetParameter(2));
     h[1]->SetBinError(i, f1->GetParError(2));
      
     delete f1;
     delete h_temp;
   }
   
   h[1]->SetTitle(fTitle.c_str());
   h[1]->GetXaxis()->SetTitle(fXAxisLabel.c_str());
   h[1]->GetYaxis()->SetTitle(fYAxisLabel.c_str());
   h[1]->GetXaxis()->SetRangeUser(fXmin,fXmax);
   h[1]->GetYaxis()->SetRangeUser(fYmin,fYmax);

   h[1]->SetTitleOffset(1.,"X");
   h[1]->SetTitleOffset(1.,"Y");
//    h[1]->GetXaxis()->SetTitleSize(0.04);
//    h[1]->GetYaxis()->SetTitleSize(0.04);
   
   TCanvas *c = new TCanvas("c","",1200,800);
//    TCanvas *c = new TCanvas("c");
   c->cd();

   TF1 *fit = new TF1("fit","sqrt(pow([0],2)+pow([1],2)*(x-[3])+pow([2]*(x-[3]),2))",fXmin,(fXmax+1));
   fit->SetParName(0, "A");
   fit->SetParameter(0, 0.5);
   fit->SetParLimits(0, 0.1, 2);
   fit->SetParName(1, "B");
   fit->SetParameter(1, 0.5);
   fit->SetParLimits(1, 0, 1.5);
   fit->SetParName(2, "C");
   fit->SetParameter(2, 0.02);
   fit->SetParLimits(2, 0, 0.2);
   fit->SetParName(3, "D");
   fit->SetParameter(3, 3);
   fit->SetParLimits(3, 2, 5);
//    fit->FixParameter(3, 0);

   if(doFit==1) h[1]->Fit("fit","RV");

   h[0]->SetLineWidth(2);
   h[0]->SetLineStyle(2);
   h[0]->SetLineColor(kRed);
   h[0]->SetFillColor(kRed);
//    h[0]->SetMarkerSize(.6);
   h[0]->SetMarkerStyle(26);
   h[0]->SetMarkerColor(kRed);
   h[1]->SetLineWidth(2);
//    h[1]->SetLineStyle(3);
   h[1]->SetLineColor(kBlack);
//    h[1]->SetMarkerSize(.8);
   h[1]->SetMarkerStyle(20);
   h[1]->SetMarkerColor(kBlack);
   h[1]->Draw("");
   h[0]->Draw("same");
   if(doFit==1) fit->Draw("same");
   
   TLegend *legend = new TLegend(.65,.25,.95,.4);
   legend->SetBorderSize(0);
   legend->SetFillColor(0);
   legend->SetFillStyle(0);
   legend->SetTextFont(62);
   legend->AddEntry(h[1],"Data","lp");
   legend->AddEntry(h[0],"Simulation","lp");
   legend->Draw();
   
   TLatex l;
   l.SetTextAlign(12);
   l.SetTextSize(0.05);
   l.SetTextFont(62);
   l.SetNDC();
   l.DrawLatex(0.14,0.84,"CMS 2009 Preliminary");
   l.DrawLatex(0.14,0.79,("#sqrt{s}="+fTitle).c_str());
   l.DrawLatex(0.16,0.55,"f(x)=#sqrt{A^{2}+B^{2}(x-D)+C^{2}(x-D)^{2}}");

   string fileName = fName;
   c->SaveAs(fileName.c_str());
   
   delete legend;
   delete h[0];
   delete h[1];
   delete c;
}


void overlay_plotsPAS(const string& fFile0, const string& fFile1, const string& fPlot0, const string& fPlot1, const double fXmin, const double fXmax, const double fYmin, const double fYmax,const string& fXAxisLabel, const string& fYAxisLabel, const string& fTitle, const string& fName, const int doFit) {
  
   gStyle->SetOptStat(kFALSE);
 
   TH2F *h2[4];
   TH1F *h[2];
   
   TFile file0(fFile0.c_str());
   h2[0] = (TH2F*)file0.Get(fPlot0.c_str()); 
   h2[1] = (TH2F*)file0.Get(fPlot1.c_str()); 
   
   h2[0]->Add(h2[1],1.0);
   
   TFile file1(fFile1.c_str());
   h2[2] = (TH2F*)file1.Get(fPlot0.c_str()); 
   h2[3] = (TH2F*)file1.Get(fPlot1.c_str()); 
   
   h2[2]->Add(h2[3],1.0);
   
   h[0] = new TH1F("h0","h0",h2[0]->GetXaxis()->GetNbins(),h2[0]->GetXaxis()->GetXmin(),h2[0]->GetXaxis()->GetXmax());
   h[1] = new TH1F("h1","h1",h2[2]->GetXaxis()->GetNbins(),h2[2]->GetXaxis()->GetXmin(),h2[2]->GetXaxis()->GetXmax());
   
//    for(Int_t i=1; i<=h2[0]->GetNbinsX(); i++) {
     for(Int_t i=1; i<=14; i++) {

     TH1D* h_temp = h2[0]->ProjectionY("h_temp", i, i, "e");

     Int_t ngroup = 1;

     if( fTitle.find("900") != string::npos ) {
       if(i==10) ngroup = 2;
       else if(i==11) ngroup = 4;
       else if(i==12) ngroup = 4;
     } else if( fTitle.find("2360") != string::npos ) {
       if(i==10) ngroup = 4;
       else if(i==11) ngroup = 4;
       else if(i==12) ngroup = 4;
       else if(i==13) ngroup = 4;
       else if(i==14) ngroup = 8;
     }
     
     h_temp->Rebin(ngroup);
     
     TF1* f1= new TF1("f1", "gaus", h_temp->GetXaxis()->GetXmin(), h_temp->GetXaxis()->GetXmax());
     f1->SetParameters(h_temp->GetBinContent(h_temp->GetMaximumBin()),h_temp->GetMean(),h_temp->GetRMS());
     
     h_temp->Fit("f1","R", "",h_temp->GetXaxis()->GetXmin(),h_temp->GetXaxis()->GetXmax());
  
     h[0]->SetBinContent(i, f1->GetParameter(2));
     h[0]->SetBinError(i, f1->GetParError(2));
      
     delete f1;
     delete h_temp;
   }
   
//    for(Int_t i=1; i<=h2[2]->GetNbinsX(); i++) {
   for(Int_t i=1; i<=14; i++) {
    
     TH1D* h_temp = h2[2]->ProjectionY("h_temp", i, i, "e");
     
     Int_t ngroup = 1;

     if( fTitle.find("900") != string::npos ) {
       if(i==10) ngroup = 2;
       else if(i==11) ngroup = 4;
       else if(i==12) ngroup = 4;
     } else if( fTitle.find("2360") != string::npos ) {
       if(i==10) ngroup = 4;
       else if(i==11) ngroup = 4;
       else if(i==12) ngroup = 4;
       else if(i==13) ngroup = 4;
       else if(i==14) ngroup = 8;
     }
     
     h_temp->Rebin(ngroup);
  
     TF1* f1= new TF1("f1", "gaus", h_temp->GetXaxis()->GetXmin(), h_temp->GetXaxis()->GetXmax());
     f1->SetParameters(h_temp->GetBinContent(h_temp->GetMaximumBin()),h_temp->GetMean(),h_temp->GetRMS());
     h_temp->Fit("f1","R", "",h_temp->GetXaxis()->GetXmin(),h_temp->GetXaxis()->GetXmax());
  
     h[1]->SetBinContent(i, f1->GetParameter(2));
     h[1]->SetBinError(i, f1->GetParError(2));
      
     delete f1;
     delete h_temp;
   }
   
   h[0]->SetTitle(fTitle.c_str());
   h[0]->GetXaxis()->SetTitle(fXAxisLabel.c_str());
   h[0]->GetYaxis()->SetTitle(fYAxisLabel.c_str());
   h[0]->GetXaxis()->SetRangeUser(fXmin,fXmax);
   h[0]->GetYaxis()->SetRangeUser(fYmin,fYmax);

   h[0]->SetTitleOffset(1.,"X");
   h[0]->SetTitleOffset(1.,"Y");
//    h[0]->GetXaxis()->SetTitleSize(0.04);
//    h[0]->GetYaxis()->SetTitleSize(0.04);
   
//    TCanvas *c = new TCanvas("c","",1200,800);
   TCanvas *c = new TCanvas("c");
   c->cd();
  
   h[0]->SetLineWidth(2);
   h[0]->SetLineStyle(2);
   h[0]->SetLineColor(kRed);
   h[0]->SetFillColor(kRed);
//    h[0]->SetMarkerSize(.6);
   h[0]->SetMarkerStyle(26);
   h[0]->SetMarkerColor(kRed);
   h[0]->Draw("");
   h[1]->SetLineWidth(2);
//    h[1]->SetLineStyle(3);
   h[1]->SetLineColor(kBlack);
//    h[1]->SetMarkerSize(.8);
   h[1]->SetMarkerStyle(20);
   h[1]->SetMarkerColor(kBlack);
   h[1]->Draw("same");
   
   TLegend *legend = new TLegend(.65,.25,.95,.4);
   legend->SetBorderSize(0);
   legend->SetFillColor(0);
   legend->SetFillStyle(0);
   legend->SetTextFont(42);
   legend->AddEntry(h[1],"Data","lp");
   legend->AddEntry(h[0],"Simulation","lp");
   legend->Draw();
   
   TLatex l;
   l.SetTextAlign(12);
   l.SetTextSize(0.05);
   l.SetTextFont(62);
   l.SetNDC();
   l.DrawLatex(0.17,0.9,"CMS Preliminary 2009");
   l.DrawLatex(0.17,0.85,("#sqrt{s}="+fTitle).c_str());

   string fileName = fName;
   c->SaveAs(fileName.c_str());
   
   delete legend;
   delete h[0];
   delete h[1];
   delete c;
}


void makePlots() {
   // turn on/off batch mode
   gROOT->SetBatch(kTRUE);
   // set ROOT style
//    setMyStyle();
   setTDRStyle();
   gStyle->SetOptTitle(0);
   gStyle->SetFuncColor(kBlack);
   gStyle->SetFuncWidth(2);
   //********************************************
   // root files
   //********************************************
   // *** MC ***
   string MC_900 = "data/output/Jan23rdReReco_files/output_MC900GeV.root";
   string MC_2360 = "data/output/Jan23rdReReco_files/output_MC2360GeV.root";
   // *** data ***
   string data_900 = "data/output/Jan23rdReReco_files/output_DATA900GeV.root";
   string data_2360 = "data/output/Jan23rdReReco_files/output_DATA2360GeV.root";
   //********************************************
   // make plots
   //********************************************

//    overlay_plots(MC_900, data_900, "h2_metx_sumet", "h2_mety_sumet", 0, 39, 0, 6.5, "#SigmaE_{T} [GeV]", "#sigma(#slash{E}_{x,y}) [GeV]", "900 GeV", "h_metxysigma_sumet_900.eps",1);
   overlay_plotsPAS(MC_900, data_900, "h2_metx_sumet", "h2_mety_sumet", 0, 39, 0, 6, "#SigmaE_{T} [GeV]", "#sigma(#slash{E}_{x,y}) [GeV]", "900 GeV", "h_metxysigma_sumet_900.eps",0);

//    overlay_plots(MC_2360, data_2360, "h2_metx_sumet", "h2_mety_sumet", 0, 46, 0, 7, "#SigmaE_{T} [GeV]", "#sigma(#slash{E}_{x,y}) [GeV]", "2360 GeV", "h_metxysigma_sumet_2360.eps",1);
   overlay_plotsPAS(MC_2360, data_2360, "h2_metx_sumet", "h2_mety_sumet", 0, 45, 0, 6, "#SigmaE_{T} [GeV]", "#sigma(#slash{E}_{x,y}) [GeV]", "2360 GeV", "h_metxysigma_sumet_2360.eps",0);
}
