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
//    gStyle->SetOptTitle(0); // for PAS/AN
//    gStyle->SetPadLeftMargin(0.13); // for Noise-only
//    gStyle->SetPadRightMargin(0.07); // for Noise-only
//    gStyle->SetStatFont(42);
//    gStyle->SetTitleFont(42);
//    gStyle->SetTitleFont(42, "XYZ");
//    gStyle->SetLabelFont(42, "XYZ");
}

void overlay_trend_plots(const string& fFile0, const string& fFile1, const string& fPlot, const string& fTitle, const string& fName) {
  
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
     
     h[0]->SetBinContent(i+1,yAll);
     h[0]->SetBinError(i+1,eyAll);
     
     if(xAll==xGood) {
        h[1]->SetBinContent(i+1,yGood);
        h[1]->SetBinError(i+1,eyGood);
      
        j++;
     }
     
     h[0]->GetXaxis()->SetBinLabel(i+1,Form("Run %d",(Int_t)xAll));
   }
   
   h[0]->SetTitle(fTitle.c_str());
   h[0]->GetYaxis()->SetTitle(g[0]->GetYaxis()->GetTitle());
   h[0]->GetXaxis()->SetLabelSize(0.035);
   h[0]->SetLineStyle(2);
   h[0]->SetLineWidth(2);
   h[0]->SetLineColor(kRed);
   h[0]->Draw("EL");
   h[1]->SetLineWidth(2);
   h[1]->SetLineColor(kGreen+2);
   h[1]->Draw("ELsame");
   
   TLegend *legend = new TLegend(.7,.15,.85,.25);
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


void makePlots() {
   // turn on/off batch mode
   gROOT->SetBatch(kTRUE);
   // set ROOT style
   style();
   //********************************************
   // root files
   //********************************************
   string all_runs_900 = "data/output/rootFileDATA900GeV_AllRuns.root";
   string good_runs_900 = "data/output/rootFileDATA900GeV_GoodRuns.root";
   
   string all_runs = "data/output/rootFileDATA_AllRuns.root";
   string good_runs = "data/output/rootFileDATA_GoodRuns.root";
   //********************************************
   // make plots
   //********************************************
//    overlay_trend_plots(all_runs_900, good_runs_900, "g_caloSumetMean_vs_run", "CaloSumET Mean vs. Run (900 GeV)", "caloSumetMean_vs_run_900GeV.png");
//    overlay_trend_plots(all_runs_900, good_runs_900, "g_caloSumetRMS_vs_run", "CaloSumET RMS vs. Run (900 GeV)", "caloSumetRMS_vs_run_900GeV.png");
//    overlay_trend_plots(all_runs_900, good_runs_900, "g_calometPtMean_vs_run", "CaloMET Mean vs. Run (900 GeV)", "calometPtMean_vs_run_900GeV.png");
//    overlay_trend_plots(all_runs_900, good_runs_900, "g_calometPtRMS_vs_run", "CaloMET RMS vs. Run (900 GeV)", "calometPtRMS_vs_run_900GeV.png");
//    overlay_trend_plots(all_runs_900, good_runs_900, "g_calometPxMean_vs_run", "CaloMETx  #mu vs. Run (900 GeV)", "calometPxMean_vs_run_900GeV.png");
//    overlay_trend_plots(all_runs_900, good_runs_900, "g_calometPxSigma_vs_run", "CaloMETx  #sigma vs. Run (900 GeV)", "calometPxSigma_vs_run_900GeV.png");
//    overlay_trend_plots(all_runs_900, good_runs_900, "g_calometPyMean_vs_run", "CaloMETy  #mu vs. Run (900 GeV)", "calometPyMean_vs_run_900GeV.png");
//    overlay_trend_plots(all_runs_900, good_runs_900, "g_calometPySigma_vs_run", "CaloMETy  #sigma vs. Run (900 GeV)", "calometPySigma_vs_run_900GeV.png");

   overlay_trend_plots(all_runs, good_runs, "g_caloSumetMean_vs_run", "CaloSumET Mean vs. Run", "caloSumetMean_vs_run.eps");
   overlay_trend_plots(all_runs, good_runs, "g_caloSumetRMS_vs_run", "CaloSumET RMS vs. Run", "caloSumetRMS_vs_run.eps");
   overlay_trend_plots(all_runs, good_runs, "g_calometPtMean_vs_run", "CaloMET Mean vs. Run", "calometPtMean_vs_run.eps");
   overlay_trend_plots(all_runs, good_runs, "g_calometPtRMS_vs_run", "CaloMET RMS vs. Run", "calometPtRMS_vs_run.eps");
   overlay_trend_plots(all_runs, good_runs, "g_calometPxMean_vs_run", "CaloMETx #mu vs. Run", "calometPxMean_vs_run.eps");
   overlay_trend_plots(all_runs, good_runs, "g_calometPxSigma_vs_run", "CaloMETx #sigma vs. Run", "calometPxSigma_vs_run.eps");
   overlay_trend_plots(all_runs, good_runs, "g_calometPyMean_vs_run", "CaloMETy #mu vs. Run", "calometPyMean_vs_run.eps");
   overlay_trend_plots(all_runs, good_runs, "g_calometPySigma_vs_run", "CaloMETy #sigma vs. Run", "calometPySigma_vs_run.eps");
}
