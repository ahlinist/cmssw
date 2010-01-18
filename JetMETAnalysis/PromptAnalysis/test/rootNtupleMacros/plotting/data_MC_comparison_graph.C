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
     
//    gStyle->SetOptStat(kFALSE);
   gStyle->SetOptTitle(0); // for PAS/AN
//    gStyle->SetPadLeftMargin(0.13); // for Noise-only
//    gStyle->SetPadRightMargin(0.07); // for Noise-only
//    gStyle->SetStatFont(42);
//    gStyle->SetTitleFont(42);
//    gStyle->SetTitleFont(42, "XYZ");
//    gStyle->SetLabelFont(42, "XYZ");
}

void overlay_plots(const string& fFile0, const string& fFile1, const string& fPlot, 
		   const double fXmin, const double fXmax,
		   const double fYmin, const double fYmax,
		   const string& fXAxisLabel, const string& fYAxisLabel, const string& fName) {
  
  // TGraphErrors *calometPtRMS_vs_ieta = new TGraphErrors(n_ieta,x_ieta,yR_ieta,ex_ieta,eyR_ieta);

   TGraphErrors *g[2];
 
   TFile file0(fFile0.c_str());
   g[0] = (TGraphErrors*)file0.Get(fPlot.c_str()); 

   TFile file1(fFile1.c_str());
   g[1] = (TGraphErrors*)file1.Get(fPlot.c_str()); 
   
   //    double ymax = (g[0]->GetMaximum()>g[1]->GetMaximum()) ? g[0]->GetMaximum() : g[1]->GetMaximum();
   
   g[0]->GetXaxis()->SetTitle(fXAxisLabel.c_str());
   g[0]->GetYaxis()->SetTitle(fYAxisLabel.c_str());
   g[0]->GetXaxis()->SetRangeUser(fXmin,fXmax);
   g[0]->GetYaxis()->SetRangeUser(fYmin,fYmax);
   g[0]->GetXaxis()->SetLabelSize(0.03);
   g[0]->GetYaxis()->SetLabelSize(0.03);

   //g[0]->SetTitleOffset(1.2,"X");
   g[0]->GetXaxis()->SetTitleSize(0.03);
   g[0]->GetYaxis()->SetTitleSize(0.03);

   g[0]->GetYaxis()->SetTitleOffset(1.5);
   

   //    double scale = g[1]->GetEntries()/g[0]->GetEntries();
   //    g[0]->Scale(scale);
   
   TCanvas *c = new TCanvas("c","",800,800);
   c->cd();
  
   g[0]->SetLineWidth(3);
   //    g[0]->SetLineStyle(4);
   g[0]->SetLineColor(kRed);
   g[0]->SetFillColor(kRed);
   //    g[0]->SetMarkerSize(.6);
   g[0]->SetMarkerStyle(26);
   g[0]->SetMarkerColor(kRed);
   g[0]->Draw("ap");
   g[1]->SetLineWidth(3);
   //    g[1]->SetLineStyle(3);
   g[1]->SetLineColor(kBlack);
   //    g[1]->SetMarkerSize(.8);
   g[1]->SetMarkerStyle(20);
   g[1]->SetMarkerSize(0.5);
   g[1]->SetMarkerColor(kBlack);
   g[1]->Draw("psames");
   
   //update the current pad, needed to modify statboxes
   gPad->Update();
   
   // get the statboxes and set color
   //    TPaveStats *st1 = (TPaveStats*)g[0]->GetListOfFunctions()->FindObject("stats");
   //    st1->SetTextColor(kRed);
   //    st1->SetLineColor(kRed);
   //    st1->SetOptStat(1111111);
   //    TPaveStats *st2 = (TPaveStats*)g[1]->GetListOfFunctions()->FindObject("stats");
   //    st2->SetTextColor(kBlack);
   //    st2->SetLineColor(kBlack);
   //    st2->SetOptStat(1111111);

   // set the position of the statboxes
   //    double x1 = st1->GetX1NDC();
   //    double y1 = st1->GetY1NDC();
   //    double x2 = st1->GetX2NDC();
   //    double y2 = st1->GetY2NDC();
   //    //double xx = x2-x1;
   //    double yy = y2-y1;
   //    st2->SetX1NDC(x1);
   //    st2->SetY1NDC(y1-yy);
   //    st2->SetX2NDC(x2);
   //    st2->SetY2NDC(y1);
   gPad->Modified();
   
   TLegend *legend = new TLegend(.4,.91,.75,.99);
   legend->SetBorderSize(1);
   legend->SetFillColor(0);
   //    legend->SetFillStyle(0);
   legend->AddEntry(g[0],"MC","lp");
   legend->AddEntry(g[1],"Data","lp");
   legend->Draw();
   
   TLatex l;
   l.SetTextAlign(12);
   l.SetTextSize(0.04);
   l.SetTextFont(62);
   l.SetNDC();
   //l.DrawLatex(0.20,0.15,"CMS 2009 Preliminary - 900 GeV");
   l.DrawLatex(0.20,0.15,"CMS 2009 Preliminary - 2360 GeV");

   //c->SetLogy();
   c->SetGridx();
   c->SetGridy();
   
   string fileName = fName;
   c->SaveAs(fileName.c_str());
   
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
   // *** MC ***
   //string MC = "output_MC900GeV.root";
   string MC = "output_MC2360GeV.root";
   // *** data ***
   //string data = "output_DATA900GeV.root";
   string data = "output_DATA2360GeV.root";
   //********************************************
   // make plots
   //********************************************

   /*
   overlay_plots(MC, data, "g_calometPtMean_vs_ieta", -41, 41, -0.1, 0.7, "Calotower ieta", "CaloMET Mean [GeV]", "g_calometPtMean_vs_ieta_900.png");
   overlay_plots(MC, data, "g_calometPtRMS_vs_ieta", -41, 41, 0, 0.7, "Calotower ieta", "CaloMET RMS [GeV]", "g_calometPtRMS_vs_ieta_900.png");

   overlay_plots(MC, data, "g_caloSumetMean_vs_ieta", -41, 41, -0.1, 0.7,"Calotower ieta", "CaloSumET [GeV]", "g_caloSumetMean_vs_ieta_900.png");
   overlay_plots(MC, data, "g_caloSumetRMS_vs_ieta", -41, 41, 0, 0.7,"Calotower ieta", "CaloSumET RMS [GeV]", "g_caloSumetRMS_vs_ieta_900.png");

   overlay_plots(MC, data, "g_calometPxMean_vs_ieta", -41, 41, -0.1, 0.1, "Calotower ieta", "CaloMETPx Mean [GeV]", "g_calometPxMean_vs_ieta_900.png");
   overlay_plots(MC, data, "g_calometPxRMS_vs_ieta", -41, 41, 0, 0.7, "Calotower ieta", "CaloMETPx RMS [GeV]", "g_calometPxRMS_vs_ieta_900.png");

   overlay_plots(MC, data, "g_calometPyMean_vs_ieta", -41, 41, -0.1, 0.1, "Calotower ieta", "CaloMETPy Mean [GeV]", "g_calometPyMean_vs_ieta_900.png");
   overlay_plots(MC, data, "g_calometPyRMS_vs_ieta", -41, 41, 0., 0.7, "Calotower ieta", "CaloMETPy RMS [GeV]", "g_calometPyRMS_vs_ieta_900.png");
   */

   
   overlay_plots(MC, data, "g_calometPtMean_vs_ieta", -41, 41, -0.1, 0.7, "Calotower ieta", "CaloMET Mean [GeV]", "g_calometPtMean_vs_ieta_2360.png");
   overlay_plots(MC, data, "g_calometPtRMS_vs_ieta", -41, 41, 0, 0.7, "Calotower ieta", "CaloMET RMS [GeV]", "g_calometPtRMS_vs_ieta_2360.png");

   overlay_plots(MC, data, "g_caloSumetMean_vs_ieta", -41, 41, -0.1, 0.7,"Calotower ieta", "CaloSumET [GeV]", "g_caloSumetMean_vs_ieta_2360.png");
   overlay_plots(MC, data, "g_caloSumetRMS_vs_ieta", -41, 41, 0, 0.7,"Calotower ieta", "CaloSumET RMS [GeV]", "g_caloSumetRMS_vs_ieta_2360.png");

   overlay_plots(MC, data, "g_calometPxMean_vs_ieta", -41, 41, -0.1, 0.1, "Calotower ieta", "CaloMETPx Mean [GeV]", "g_calometPxMean_vs_ieta_2360.png");
   overlay_plots(MC, data, "g_calometPxRMS_vs_ieta", -41, 41, 0, 0.7, "Calotower ieta", "CaloMETPx RMS [GeV]", "g_calometPxRMS_vs_ieta_2360.png");

   overlay_plots(MC, data, "g_calometPyMean_vs_ieta", -41, 41, -0.1, 0.1, "Calotower ieta", "CaloMETPy Mean [GeV]", "g_calometPyMean_vs_ieta_2360.png");
   overlay_plots(MC, data, "g_calometPyRMS_vs_ieta", -41, 41, 0., 0.7, "Calotower ieta", "CaloMETPy RMS [GeV]", "g_calometPyRMS_vs_ieta_2360.png");
   

}
