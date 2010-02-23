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
   TDirectoryFile *subDir = (TDirectoryFile*)file0.Get("offsetAnalysis");
   h[0] = (TH1F*)subDir->Get(fPlot.c_str());

   TFile file1(fFile1.c_str());
   subDir = (TDirectoryFile*)file1.Get("offsetAnalysis");
   h[1] = (TH1F*)subDir->Get(fPlot.c_str());

//    double ymax = (h[0]->GetMaximum()>h[1]->GetMaximum()) ? h[0]->GetMaximum() : h[1]->GetMaximum();
   
   h[1]->GetXaxis()->SetTitle(fXAxisLabel.c_str());
   h[1]->GetXaxis()->SetRangeUser(fXmin,fXmax);
//    h[0]->GetYaxis()->SetRangeUser(0.5,1.25*ymax);

   h[1]->SetTitleOffset(1.2,"X");
   h[1]->GetXaxis()->SetTitleSize(0.04);
   h[1]->GetYaxis()->SetTitleSize(0.04);
   
   TCanvas *c = new TCanvas("c","",800,800);
   c->cd();
  
   h[0]->SetLineWidth(3);
//    h[0]->SetLineStyle(4);
   h[0]->SetLineColor(kRed);
   h[0]->SetFillColor(kRed);
//    h[0]->SetMarkerSize(.6);
//    h[0]->SetMarkerStyle(26);
//    h[0]->SetMarkerColor(kRed);
//    h[0]->Draw("hist");
   h[1]->SetMinimum(0.5);
   h[1]->SetLineWidth(3);
//    h[1]->SetLineStyle(3);
   h[1]->SetLineColor(kBlack);
//    h[1]->SetMarkerSize(.8);
   h[1]->SetMarkerStyle(20);
   h[1]->SetMarkerColor(kBlack);
   h[1]->Draw("");
   h[0]->Draw("histsames");
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
   l.DrawLatex(0.45,0.15,"CMS 2009 Preliminary");

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
//    string MC_noise = "data/output/RecHitValidation_NoiseOnly_STARTUP3X_V8I.root";
   string MC_noise = "data/output/RecHitValidation_NoiseOnly_STARTUP3X_V8N.root";
   // *** data ***
//    string data_ZB = "data/output/RecHitValidation_ZeroBias_R124022_900GeV_Dec14thReReco.root";
   string data_ZB = "data/output/RecHitValidation_ZeroBias_R124022_900GeV_Jan23rdReReco.root";
   //********************************************
   // make plots
   //********************************************

   overlay_plots(MC_noise, data_ZB, "h_RecHitEinEB", -0.35, 0.35, "EB RecHit Energy [GeV]", "h_RecHitEinEB.png",1);
   overlay_plots(MC_noise, data_ZB, "h_RecHitEinEE", -1.5, 1.5, "EE RecHit Energy [GeV]", "h_RecHitEinEE.png",1);
   overlay_plots(MC_noise, data_ZB, "h_RecHitEinHB", -2, 2, "HB RecHit Energy [GeV]", "h_RecHitEinHB.png",1);
   overlay_plots(MC_noise, data_ZB, "h_RecHitEinHE", -2, 2, "HE RecHit Energy [GeV]", "h_RecHitEinHE.png",1);
   overlay_plots(MC_noise, data_ZB, "h_RecHitEinHO", -4.5, 4.5, "HO RecHit Energy [GeV]", "h_RecHitEinHO.png",1);
   overlay_plots(MC_noise, data_ZB, "h_RecHitEinHFl", -4, 4, "HF Long RecHit Energy [GeV]", "h_RecHitEinHFl.png",1);
   overlay_plots(MC_noise, data_ZB, "h_RecHitEinHFs", -4, 4, "HF Short RecHit Energy [GeV]", "h_RecHitEinHFs.png",1);
}
