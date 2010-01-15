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

void overlay_plots(const string& fFile0, const string& fFile1, const string& fPlot, const double fXmin, const double fXmax, const string& fXAxisLabel, const string& fName, const int logY) {
  
   TH1F *h[2];
 
   TFile file0(fFile0.c_str());
   h[0] = (TH1F*)file0.Get(fPlot.c_str()); 

   TFile file1(fFile1.c_str());
   h[1] = (TH1F*)file1.Get(fPlot.c_str()); 
   
//    double ymax = (h[0]->GetMaximum()>h[1]->GetMaximum()) ? h[0]->GetMaximum() : h[1]->GetMaximum();
   
   h[0]->GetXaxis()->SetTitle(fXAxisLabel.c_str());
   h[0]->GetXaxis()->SetRangeUser(fXmin,fXmax);
//    h[0]->GetYaxis()->SetRangeUser(0.5,1.25*ymax);

   h[0]->SetTitleOffset(1.2,"X");
   h[0]->GetXaxis()->SetTitleSize(0.04);
   h[0]->GetYaxis()->SetTitleSize(0.04);
   
   double scale = h[1]->GetEntries()/h[0]->GetEntries();
   h[0]->Scale(scale);
   
   TCanvas *c = new TCanvas("c","",800,800);
   c->cd();
  
   h[0]->SetLineWidth(3);
//    h[0]->SetLineStyle(4);
   h[0]->SetLineColor(kRed);
   h[0]->SetFillColor(kRed);
//    h[0]->SetMarkerSize(.6);
//    h[0]->SetMarkerStyle(26);
//    h[0]->SetMarkerColor(kRed);
   h[0]->Draw("hist");
   h[1]->SetLineWidth(3);
//    h[1]->SetLineStyle(3);
   h[1]->SetLineColor(kBlack);
//    h[1]->SetMarkerSize(.8);
   h[1]->SetMarkerStyle(20);
   h[1]->SetMarkerColor(kBlack);
   h[1]->Draw("sames");
   
   //update the current pad, needed to modify statboxes
   gPad->Update();
   
   // get the statboxes and set color
   TPaveStats *st1 = (TPaveStats*)h[0]->GetListOfFunctions()->FindObject("stats");
   st1->SetTextColor(kRed);
   st1->SetLineColor(kRed);
   st1->SetOptStat(1111111);
   TPaveStats *st2 = (TPaveStats*)h[1]->GetListOfFunctions()->FindObject("stats");
   st2->SetTextColor(kBlack);
   st2->SetLineColor(kBlack);
   st2->SetOptStat(1111111);

   // set the position of the statboxes
   double x1 = st1->GetX1NDC();
   double y1 = st1->GetY1NDC();
   double x2 = st1->GetX2NDC();
   double y2 = st1->GetY2NDC();
   //double xx = x2-x1;
   double yy = y2-y1;
   st2->SetX1NDC(x1);
   st2->SetY1NDC(y1-yy);
   st2->SetX2NDC(x2);
   st2->SetY2NDC(y1);
   gPad->Modified();
   
   TLegend *legend = new TLegend(.4,.91,.75,.99);
   legend->SetBorderSize(1);
   legend->SetFillColor(0);
   //    legend->SetFillStyle(0);
   legend->AddEntry(h[0],"MC","l");
   legend->AddEntry(h[1],"Data","l");
   legend->Draw();
   
   TLatex l;
   l.SetTextAlign(12);
   l.SetTextSize(0.04);
   l.SetTextFont(62);
   l.SetNDC();
   l.DrawLatex(0.40,0.15,"CMS 2009 Preliminary");

   if(logY==1)
     c->SetLogy();
   
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
   string MC = "output_MC.root";
   // *** data ***
   string data = "output_DATA.root";
   //********************************************
   // make plots
   //********************************************

   overlay_plots(MC, data, "h_caloSumet", 0, 100, "CaloSumET [GeV]", "h_caloSumet.png",1);
   overlay_plots(MC, data, "h_calometPt", 0, 100, "CaloMET [GeV]", "h_calometPt.png",1);
   overlay_plots(MC, data, "h_calometPx", -50, 50, "CaloMETX [GeV]", "h_calometPx.png",1);
   overlay_plots(MC, data, "h_calometPy", -50, 50, "CaloMETY [GeV]", "h_calometPy.png",1);
   overlay_plots(MC, data, "h_calometPhi", -4, 4, "CaloMETPhi (rad.)", "h_calometPhi.png",1);
   overlay_plots(MC, data, "h_calometSig", 0, 10, "CaloMET/sqrt(SumET)", "h_calometSig.png",1);

   overlay_plots(MC, data, "h_caloSumetEB", 0, 100, "CaloSumET - EB [GeV]", "h_caloSumetEB.png",1);
   overlay_plots(MC, data, "h_caloSumetEE", 0, 100, "CaloSumET - EE [GeV]", "h_caloSumetEE.png",1);
   overlay_plots(MC, data, "h_caloSumetHB", 0, 100, "CaloSumET - HB [GeV]", "h_caloSumetHB.png",1);
   overlay_plots(MC, data, "h_caloSumetHE", 0, 100, "CaloSumET - HE [GeV]", "h_caloSumetHE.png",1);
   overlay_plots(MC, data, "h_caloSumetHadHF", 0, 100, "CaloSumET - had HF [GeV]", "h_caloSumetHadHF.png",1);
   overlay_plots(MC, data, "h_caloSumetEmHF", 0, 100, "CaloSumET - em HF [GeV]", "h_caloSumetEmHF.png",1);

}
