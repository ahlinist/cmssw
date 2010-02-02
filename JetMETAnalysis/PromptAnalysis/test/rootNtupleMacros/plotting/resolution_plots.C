void style() {

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
//    gStyle->SetOptTitle(0); // for PAS/AN
//    gStyle->SetPadLeftMargin(0.13); // for Noise-only
//    gStyle->SetPadRightMargin(0.07); // for Noise-only
//    gStyle->SetStatFont(42);
//    gStyle->SetTitleFont(42);
//    gStyle->SetTitleFont(42, "XYZ");
//    gStyle->SetLabelFont(42, "XYZ");
}

void overlay_plots_nodist(const string& fFile0, const string& fFile1, const string& fPlot, const double fXmin, const double fXmax, const double fYmin, const double fYmax,const string& fXAxisLabel, const string& fYAxisLabel, const string& fTitle, const string& fName, const int logY) {
  
   gStyle->SetOptStat(kFALSE);
 
   TH1F *h[2];
 
   TFile file0(fFile0.c_str());
   h[0] = (TH1F*)file0.Get(fPlot.c_str()); 

   TFile file1(fFile1.c_str());
   h[1] = (TH1F*)file1.Get(fPlot.c_str()); 
   
//    double ymax = (h[0]->GetMaximum()>h[1]->GetMaximum()) ? h[0]->GetMaximum() : h[1]->GetMaximum();
   
   h[0]->SetTitle(fTitle.c_str());
   h[0]->GetXaxis()->SetTitle(fXAxisLabel.c_str());
   h[0]->GetYaxis()->SetTitle(fYAxisLabel.c_str());
   h[0]->GetXaxis()->SetRangeUser(fXmin,fXmax);
   h[0]->GetYaxis()->SetRangeUser(fYmin,fYmax);

   h[0]->SetTitleOffset(1.2,"X");
   h[0]->GetXaxis()->SetTitleSize(0.04);
   h[0]->GetYaxis()->SetTitleSize(0.04);
   
   TCanvas *c = new TCanvas("c","",1200,800);
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
   
   TLegend *legend = new TLegend(.15,.75,.3,.85);
   legend->SetBorderSize(1);
   legend->SetFillColor(0);
   //    legend->SetFillStyle(0);
   legend->AddEntry(h[1],"Data","lp");
   legend->AddEntry(h[0],"MC","lp");
   legend->Draw();
   
   TLatex l;
   l.SetTextAlign(12);
   l.SetTextSize(0.04);
   l.SetTextFont(62);
   l.SetNDC();
   l.DrawLatex(0.6,0.15,"CMS 2009 Preliminary");

   if(logY==1)
     c->SetLogy();
   
   string fileName = fName;
   c->SaveAs(fileName.c_str());
   
   delete legend;
   delete c;
}

void perform_fit(const string& fFile, const string& fPlot, const double fXmin, const double fXmax, const double fYmin, const double fYmax, const string& fXAxisLabel, const string& fYAxisLabel, const string& fTitle, const string& fName) {

   TH1F *h;
 
   TFile file(fFile.c_str());
   h = (TH1F*)file.Get(fPlot.c_str()); 

   TCanvas *c = new TCanvas("c","",1200,800);
   c->cd();

   Int_t nBins = h->GetXaxis()->GetNbins();
   
   TH1F *h_metsigma2_sumet = new TH1F("h_metsigma2_sumet","h_metsigma2_sumet",nBins,h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());

   for(Int_t i=0; i<nBins; i++) {
     h_metsigma2_sumet->SetBinContent(i+1, h->GetBinContent(i+1)*h->GetBinContent(i+1) );
     h_metsigma2_sumet->SetBinError(i+1, 2*h->GetBinContent(i+1)*h->GetBinError(i+1) );
   }

   h_metsigma2_sumet->GetXaxis()->SetRangeUser(fXmin,fXmax);
   h_metsigma2_sumet->SetLineColor(kRed);
   h_metsigma2_sumet->SetMarkerStyle(26);
   h_metsigma2_sumet->SetMarkerColor(kRed);
   h_metsigma2_sumet->Draw();

   TF1 *fit = new TF1("fit","pow([0],2)+pow([1],2)*(x-[3])+pow([2]*(x-[3]),2)",fXmin,fXmax);
   fit->SetParameter(0, 0.5);
   fit->SetParLimits(0, 0.1, 2);
   fit->SetParameter(1, 0.5);
   fit->SetParLimits(1, 0, 1.5);
   fit->SetParameter(2, 0.02);
   fit->SetParLimits(2, 0, 0.2);
   fit->SetParameter(3, 3);
   fit->SetParLimits(3, 2, 5);
//    fit->FixParameter(3, 0);

   h_metsigma2_sumet->Fit("fit","R");
   
   c->SaveAs(("fit_"+fName).c_str());

   TF1 *final = new TF1("final","sqrt(fit)",fXmin,fXmax);

   h->SetTitle(fTitle.c_str());
   h->SetTitleOffset(1.2,"X");
   h->GetXaxis()->SetTitle(fXAxisLabel.c_str());
   h->GetYaxis()->SetTitle(fYAxisLabel.c_str());
   h->GetXaxis()->SetRangeUser(0,fXmax);
   h->GetYaxis()->SetRangeUser(fYmin,fYmax);
   h->SetLineColor(kRed);
   h->SetLineWidth(2);
   h->SetMarkerStyle(26);
   h->SetMarkerColor(kRed);
   h->Draw();
   final->Draw("same");

   TLatex l;
   l.SetTextAlign(12);
   l.SetTextSize(0.04);
   l.SetTextFont(62);
   l.SetNDC();
   l.DrawLatex(0.13,0.83,"CMS 2009 Preliminary");
   
   l.DrawLatex(0.55,0.4,"f(x)=#sqrt{A^{2}+B^{2}(x-D)+C^{2}(x-D)^{2}}");
   string sA = "A="; sA=sA+Form("%f",fit->GetParameter(0))+"#pm"+Form("%f",fit->GetParError(0))+" GeV";
   string sB = "B="; sB=sB+Form("%f",fit->GetParameter(1))+"#pm"+Form("%f",fit->GetParError(1))+" (GeV)^{1/2}";
   string sC = "C="; sC=sC+Form("%f",fit->GetParameter(2))+"#pm"+Form("%f",fit->GetParError(2));
   string sD = "D="; sD=sD+Form("%f",fit->GetParameter(3))+"#pm"+Form("%f",fit->GetParError(3))+ " GeV";;
   l.DrawLatex(0.55,0.3,sA.c_str());
   l.DrawLatex(0.55,0.25,sB.c_str());
   l.DrawLatex(0.55,0.2,sC.c_str());
   l.DrawLatex(0.55,0.15,sD.c_str());
   
   c->SaveAs(("final_"+fName).c_str());
   
   delete c;
}


void makePlots() {
   // turn on/off batch mode
   gROOT->SetBatch(kTRUE);
   // set ROOT style
   style();
   //********************************************
   // root files
   //********************************************
   // *** MC ***
   string MC_900 = "data/output/output_MC900GeV.root";
   string MC_2360 = "data/output/output_MC2360GeV.root";
   string QCD_FlatPt_15to3000 = "data/output/output_QCDFlat_Pt15to3000_Summer09-MC_31X_V9_7TeV.root";
   // *** data ***
   string data_900 = "data/output/output_DATA900GeV.root";
   string data_2360 = "data/output/output_DATA2360GeV.root";
   //********************************************
   // make plots
   //********************************************
   overlay_plots_nodist(MC_900, data_900, "h_metxsigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{x}) [GeV]", "900 GeV", "h_metxsigma_sumet_900.eps",0);
   overlay_plots_nodist(MC_900, data_900, "h_metysigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{y}) [GeV]", "900 GeV", "h_metysigma_sumet_900.eps",0);
   
   perform_fit(MC_900, "h_metxsigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{x}) [GeV]", "MC 900 GeV", "metxsigma_sumet_MC_900.eps");
   perform_fit(MC_900, "h_metysigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{y}) [GeV]", "MC 900 GeV", "metysigma_sumet_MC_900.eps");
   
   perform_fit(data_900, "h_metxsigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{x}) [GeV]", "DATA 900 GeV", "metxsigma_sumet_DATA_900.eps");
   perform_fit(data_900, "h_metysigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{y}) [GeV]", "DATA 900 GeV", "metysigma_sumet_DATA_900.eps");

   overlay_plots_nodist(MC_2360, data_2360, "h_metxsigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{x}) [GeV]", "2360 GeV", "h_metxsigma_sumet_2360.eps",0);
   overlay_plots_nodist(MC_2360, data_2360, "h_metysigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{y}) [GeV]", "2360 GeV", "h_metysigma_sumet_2360.eps",0);
   
   perform_fit(MC_2360, "h_metxsigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{x}) [GeV]", "MC 2360 GeV", "metxsigma_sumet_MC_2360.eps");
   perform_fit(MC_2360, "h_metysigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{y}) [GeV]", "MC 2360 GeV", "metysigma_sumet_MC_2360.eps");
   
   perform_fit(data_2360, "h_metxsigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{x}) [GeV]", "DATA 2360 GeV", "metxsigma_sumet_DATA_2360.eps");
   perform_fit(data_2360, "h_metysigma_sumet", 0, 39, 0, 6, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{y}) [GeV]", "DATA 2360 GeV", "metysigma_sumet_DATA_2360.eps");

//    overlay_plots_nodist(QCD_FlatPt_15to3000, QCD_FlatPt_15to3000, "h_metxsigma_sumet", 0, 1000, 0, 40, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{x}) [GeV]", "QCD FlatPt 15to3000", "h_metxsigma_sumet_QCD_FlatPt_15to3000.eps",0);
//    perform_fit(QCD_FlatPt_15to3000, "h_metxsigma_sumet", 0, 1000, 0, 40, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{x}) [GeV]", "MC QCD FlatPt 15to3000", "metxsigma_sumet_QCD_FlatPt_15to3000.eps");
   
   //    overlay_plots_nodist(QCD_FlatPt_15to3000, QCD_FlatPt_15to3000, "h_metysigma_sumet", 0, 1000, 0, 40, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{y}) [GeV]", "QCD FlatPt 15to3000", "h_metysigma_sumet_QCD_FlatPt_15to3000.eps",0);
//    perform_fit(QCD_FlatPt_15to3000, "h_metysigma_sumet", 0, 1000, 0, 40, "#scale[1.0]{#sum}E_{T} [GeV]", "#sigma(#slash{E}_{y}) [GeV]", "MC QCD FlatPt 15to3000", "metysigma_sumet_QCD_FlatPt_15to3000.eps");
}
