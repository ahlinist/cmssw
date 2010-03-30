void style() {
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
  tdrStyle->SetErrorX(0.);
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

  tdrStyle->SetOptTitle(0);
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

void overlay_plots(const string& fFile0, const string& fFile1, const string& fPlot, const double fXmin, const double fXmax, const string& fXAxisLabel, const string& fYAxisLabel, const string& fName, const int logY, const int FillColor) {
  
   TH1F *h[2];
 
   TFile file0(fFile0.c_str());
   h[0] = (TH1F*)file0.Get(fPlot.c_str()); 

   TFile file1(fFile1.c_str());
   h[1] = (TH1F*)file1.Get(fPlot.c_str()); 

   TCanvas *c = new TCanvas("c");
   c->cd();

   h[0]->UseCurrentStyle();
   h[1]->UseCurrentStyle();
   //   cout<<h[1]->GetTitle()<<" "<<h[1]->Integral()<<endl;
   
//    double ymax = (h[0]->GetMaximum()>h[1]->GetMaximum()) ? h[0]->GetMaximum() : h[1]->GetMaximum();
   
   h[0]->GetXaxis()->SetTitle(fXAxisLabel.c_str());
   h[0]->GetYaxis()->SetTitle(fYAxisLabel.c_str());
   h[0]->GetXaxis()->SetRangeUser(fXmin,fXmax);

//    gPad->DrawFrame(fXmin, fXmax+100, 0.1, 1000000);
//    h[0]->SetTitleOffset(1.2,"X");
//    h[0]->GetXaxis()->SetTitleSize(0.04);
//    h[0]->GetYaxis()->SetTitleSize(0.04);
   
   double scale = h[1]->GetEntries()/h[0]->GetEntries();
   h[0]->Scale(scale);

//    h[1]->SetBinContent(40,1);
//    h[1]->SetBinError(40, 1.);

   h[0]->SetLineWidth(3);
   h[0]->SetLineWidth(1);
//    h[0]->SetLineColor(kYellow);
   h[0]->SetFillColor(FillColor);
//    h[0]->SetMarkerSize(.6);
//    h[0]->SetMarkerStyle(26);
//    h[0]->SetMarkerColor(kRed);
   h[0]->Draw("hist");
   h[1]->SetLineWidth(3);
//    h[1]->SetLineStyle(3);
   h[1]->SetLineColor(kBlack);
   h[1]->SetMarkerSize(1.0);
   h[1]->SetMarkerStyle(20);
   h[1]->SetMarkerColor(kBlack);
   h[1]->Draw("sames");
   
   //update the current pad, needed to modify statboxes
   //   gPad->Update();
   
//    // get the statboxes and set color
//    TPaveStats *st1 = (TPaveStats*)h[0]->GetListOfFunctions()->FindObject("stats");
//    st1->SetTextColor(kRed);
//    st1->SetLineColor(kRed);
//    st1->SetOptStat(1111111);
//    TPaveStats *st2 = (TPaveStats*)h[1]->GetListOfFunctions()->FindObject("stats");
//    st2->SetTextColor(kBlack);
//    st2->SetLineColor(kBlack);
//    st2->SetOptStat(1111111);

//    // set the position of the statboxes
//    double x1 = st1->GetX1NDC();
//    double y1 = st1->GetY1NDC();
//    double x2 = st1->GetX2NDC();
//    double y2 = st1->GetY2NDC();
   //double xx = x2-x1;
//    double yy = y2-y1;
//    st2->SetX1NDC(x1);
//    st2->SetY1NDC(y1-yy);
//    st2->SetX2NDC(x2);
//    st2->SetY2NDC(y1);
//    gPad->Modified();
   
//    TLegend *legend = new TLegend(.7,.8,.92,.92);
//   TLegend *legend = new TLegend(.62,.77,.92,.92); //Position for METx,y plots, or #sqrt{s}=2360 GeV
   TLegend *legend = new TLegend(.62,.6,.92,.75); //Position for MET ans SumET, or #sqrt{s}=2360 GeV
   legend->SetTextFont(42);
   legend->SetTextFont(42);
   legend->SetBorderSize(1);
   legend->SetFillColor(0);
   //    legend->SetFillStyle(0);
   legend->AddEntry(h[1],"Data","lp");
   legend->AddEntry(h[0],"Simulation","f");
   legend->Draw();
   
   TLatex l;
   l.SetTextAlign(12);
   l.SetTextSize(0.04);
   l.SetTextFont(62);
   l.SetNDC();
//    l.DrawLatex(0.15,0.88," #splitline{CMS Preliminary 2009}{#sqrt{s}=900 GeV}"); //Position for METx,y plots, or #sqrt{s}=2360 GeV
     l.DrawLatex(0.55,0.88," #splitline{CMS Preliminary 2010}{#sqrt{s}=7 TeV}"); //Position for MET and SumET plots, or #sqrt{s}=2360 GeV

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
    string MC = "$PWD/MC7TeV_MinBias-START3X_V25B/root/MC7TeV_MinBias-START3X_V25B.root";
//    // *** data ***
    string data = "$PWD/MC7TeV_MinBias-START3X_V25B_v2/root/MC7TeV_MinBias-START3X_V25B_v2.root";
   //********************************************
   // make plots
   //********************************************

    //Inclusive MET plots CaloMET
    overlay_plots(MC, data, "h_caloSumet", 0, 300, "Calo #SigmaE_{T} [GeV]", "Number of Events / GeV", "h_caloSumet.eps",1, 5);
    overlay_plots(MC, data, "h_calometPt", 0, 50, "Calo #slash{E}_{T} [GeV]", "Number of Events / GeV", "h_calometPt.eps",1, 5);
    overlay_plots(MC, data, "h_calometPxy", -50, 50, "Calo #slash{E}_{x\,y} [GeV]", "2xNumber of Events / GeV", "h_calometPxy.eps",1, 5);
    overlay_plots(MC, data, "h_caloMetOSumet", 0, 1, "Calo #slash{E}_{T}/#SigmaE_{T}", "Number of Events", "h_caloMetOSumet.eps",1, 5);

    //CaloMET plots in dijets: Loose jet ID
    overlay_plots(MC, data, "h_dijetLoose_caloSumet", 0, 300, "Calo #SigmaE_{T} [GeV]", "Number of Events / GeV", "h_dijetLoose_caloSumet.eps",1, 5);
    overlay_plots(MC, data, "h_dijetLoose_calometPt", 0, 50, "Calo #slash{E}_{T} [GeV]", "Number of Events / GeV", "h_dijetLoose_calometPt.eps",1, 5);
    overlay_plots(MC, data, "h_dijetLoose_calometPxy", -50, 50, "Calo #slash{E}_{x\,y} [GeV]", "2xNumber of Events / GeV", "h_dijetLoose_calometPxy.eps",1, 5);
    overlay_plots(MC, data, "h_dijetLoose_caloMetOSumet", 0, 1, "Calo #slash{E}_{T}/#SigmaE_{T}", "Number of Events", "h_dijetLoose_caloMetOSumet.eps",1, 5);
	
    //CaloMET plots in dijets: Tight jet ID
    overlay_plots(MC, data, "h_dijetTight_caloSumet", 0, 300, "Calo #SigmaE_{T} [GeV]", "Number of Events / GeV", "h_dijetTight_caloSumet.eps",1, 5);
    overlay_plots(MC, data, "h_dijetTight_calometPt", 0, 50, "Calo #slash{E}_{T} [GeV]", "Number of Events / GeV", "h_dijetTight_calometPt.eps",1, 5);
    overlay_plots(MC, data, "h_dijetTight_calometPxy", -50, 50, "Calo #slash{E}_{x\,y} [GeV]", "2xNumber of Events / GeV", "h_dijetTight_calometPxy.eps",1, 5);
    overlay_plots(MC, data, "h_dijetTight_caloMetOSumet", 0, 1, "Calo #slash{E}_{T}/#SigmaE_{T}", "Number of Events", "h_dijetTight_caloMetOSumet.eps",1, 5);

    //Inclusive MET plots tcMET
    overlay_plots(MC, data, "h_tcSumet", 0, 400, "Tc #SigmaE_{T} [GeV]", "Number of Events / GeV", "h_tcSumet.eps",1, 6);
    overlay_plots(MC, data, "h_tcmetPt", 0, 50, "Tc #slash{E}_{T} [GeV]", "Number of Events / GeV", "h_tcmetPt.eps",1, 6);
    overlay_plots(MC, data, "h_tcmetPxy", -50, 50, "Tc #slash{E}_{x\,y} [GeV]", "2xNumber of Events / GeV", "h_tcmetPxy.eps",1, 5);
    overlay_plots(MC, data, "h_tcMetOSumet", 0, 1, "Tc #slash{E}_{T}/#SigmaE_{T}", "Number of Events", "h_tcMetOSumet.eps",1, 5);

    //TcMET plots in dijets: Loose jet ID
    overlay_plots(MC, data, "h_dijetLoose_tcSumet", 0, 400, "Tc #SigmaE_{T} [GeV]", "Number of Events / GeV", "h_dijetLoose_tcSumet.eps",1, 6);
    overlay_plots(MC, data, "h_dijetLoose_tcmetPt", 0, 50, "Tc #slash{E}_{T} [GeV]", "Number of Events / GeV", "h_dijetLoose_tcmetPt.eps",1, 6);
    overlay_plots(MC, data, "h_dijetLoose_tcmetPxy", -50, 50, "Tc #slash{E}_{x\,y} [GeV]", "2xNumber of Events / GeV", "h_dijetLoose_tcmetPxy.eps",1, 5);
    overlay_plots(MC, data, "h_dijetLoose_tcMetOSumet", 0, 1, "Tc #slash{E}_{T}/#SigmaE_{T}", "Number of Events", "h_dijetLoose_tcMetOSumet.eps",1, 5);

    //TcMET plots in dijets: Tight jet ID
    overlay_plots(MC, data, "h_dijetTight_tcSumet", 0, 400, "Tc #SigmaE_{T} [GeV]", "Number of Events / GeV", "h_dijetTight_tcSumet.eps",1, 6);
    overlay_plots(MC, data, "h_dijetTight_tcmetPt", 0, 50, "Tc #slash{E}_{T} [GeV]", "Number of Events / GeV", "h_dijetTight_tcmetPt.eps",1, 6);
    overlay_plots(MC, data, "h_dijetTight_tcmetPxy", -50, 50, "Tc #slash{E}_{x\,y} [GeV]", "2xNumber of Events / GeV", "h_dijetTight_tcmetPxy.eps",1, 5);
    overlay_plots(MC, data, "h_dijetTight_tcMetOSumet", 0, 1, "Tc #slash{E}_{T}/#SigmaE_{T}", "Number of Events", "h_dijetTight_tcMetOSumet.eps",1, 5);

    //Inclusive MET plots PFMet
    overlay_plots(MC, data, "h_pfSumet", 0, 400, "Pf #SigmaE_{T} [GeV]", "Number of Events / GeV", "h_pfSumet.eps",1, 9);
    overlay_plots(MC, data, "h_pfmetPt", 0, 50, "Pf #slash{E}_{T} [GeV]", "Number of Events / GeV", "h_pfmetPt.eps",1, 9);
    overlay_plots(MC, data, "h_pfmetPxy", -50, 50, "Pf #slash{E}_{x\,y} [GeV]", "2xNumber of Events / GeV", "h_pfmetPxy.eps",1, 5);
    overlay_plots(MC, data, "h_pfMetOSumet", 0, 1, "Pf #slash{E}_{T}/#SigmaE_{T}", "Number of Events", "h_pfMetOSumet.eps",1, 5);

    //PFMET plots in dijets: Loose jet ID
    overlay_plots(MC, data, "h_dijetLoose_pfSumet", 0, 400, "Pf #SigmaE_{T} [GeV]", "Number of Events / GeV", "h_dijetLoose_pfSumet.eps",1, 9);
    overlay_plots(MC, data, "h_dijetLoose_pfmetPt", 0, 50, "Pf #slash{E}_{T} [GeV]", "Number of Events / GeV", "h_dijetLoose_pfmetPt.eps",1, 9);
    overlay_plots(MC, data, "h_dijetLoose_pfmetPxy", -50, 50, "Pf #slash{E}_{x\,y} [GeV]", "2xNumber of Events / GeV", "h_dijetLoose_pfmetPxy.eps",1, 5);
    overlay_plots(MC, data, "h_dijetLoose_pfMetOSumet", 0, 1, "Pf #slash{E}_{T}/#SigmaE_{T}", "Number of Events", "h_dijetLoose_pfMetOSumet.eps",1, 5);

    //PFMET plots in dijets: Tight jet ID
    overlay_plots(MC, data, "h_dijetTight_pfSumet", 0, 400, "Pf #SigmaE_{T} [GeV]", "Number of Events / GeV", "h_dijetTight_pfSumet.eps",1, 9);
    overlay_plots(MC, data, "h_dijetTight_pfmetPt", 0, 50, "Pf #slash{E}_{T} [GeV]", "Number of Events / GeV", "h_dijetTight_pfmetPt.eps",1, 9);
    overlay_plots(MC, data, "h_dijetTight_pfmetPxy", -50, 50, "Pf #slash{E}_{x\,y} [GeV]", "2xNumber of Events / GeV", "h_dijetTight_pfmetPxy.eps",1, 5);
    overlay_plots(MC, data, "h_dijetTight_pfMetOSumet", 0, 1, "Pf #slash{E}_{T}/#SigmaE_{T}", "Number of Events", "h_dijetTight_pfMetOSumet.eps",1, 5);

}
