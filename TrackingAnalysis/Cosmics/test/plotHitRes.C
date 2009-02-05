void plotHitRes(){
  setTDRStyle();
  gROOT->SetStyle("tdrStyle");

  // load the code before executing the macro:
  //gROOT->LoadMacro("TrackingAnalysis/Cosmics/test/OverlapHistos.C+");
  //ol = new OverlapHistos();
  //ol->Loop(); 

  TFile f("output_test.root");

  //summary histos written out to main file, histos/detid pair in root->ROOT Memory
  //new TBrowser()
  
  width->SetLineColor(2);
  sigMean->GetYaxis()->SetTitle("Uncertainty [#mum]");
  sigMean->GetXaxis()->SetTitle("");
  sigMean->GetXaxis()->SetLabelSize(0);
  sigMean->GetXaxis()->SetTickLength(0);
  sigMean->SetMinimum(0);
  sigMean->SetMaximum(150);
  width->SetMarkerStyle(24);    //20
  width->SetMarkerColor(2);
  sigMean->SetMarkerStyle(26);  //22
  hitSigX->SetLineColor(4);
  hitSigX->SetMarkerStyle(25);  //21
  hitSigX->SetMarkerColor(4);
  sigMean->Draw();
  width->Draw("same");
  hitSigX->Draw("same");
  
  leg_hist = new TLegend(0.60,0.72,0.94,0.91);
  leg_hist->AddEntry(width,"Fitted width (#Deltax_{hit}-#Deltax_{pred})","p");
  leg_hist->AddEntry(sigMean,"<#sigma(#Deltax_{pred})>","p");
  leg_hist->AddEntry(hitSigX,"<#sigma(#Deltax_{hit})>","p");
  leg_hist->SetTextSize(0.055);
  leg_hist->SetFillColor(0);
  leg_hist->Draw("same");

  TArrow ar1(0.0,0.0,88.4,0.0,0.05,"<|>");
  ar1.SetArrowSize(0.02); ar1.SetAngle(40);
  ar1.SetLineWidth(2); ar1.Draw();

  TArrow ar2(88.6,0.0,95.4.0,0.0,0.05,"<|>");
  ar2.SetArrowSize(0.02); ar2.SetAngle(40);
  ar2.SetLineWidth(2); ar2.Draw();

  TArrow ar4(95.6,0.0,119.4.0,0.0,0.05,"<|>");
  ar4.SetArrowSize(0.02); ar4.SetAngle(40);
  ar4.SetLineWidth(2); ar4.Draw();

  TArrow ar5(119.6,0.0,137.4,0.0,0.05,"<|>");
  ar5.SetArrowSize(0.02); ar5.SetAngle(40);
  ar5.SetLineWidth(2); ar5.Draw();

  TArrow ar6(137.6,0.0,174.4,0.0,0.05,"<|>");
  ar6.SetArrowSize(0.02); ar6.SetAngle(40);
  ar6.SetLineWidth(2); ar6.Draw();

  TArrow ar7(174.6,0.0,196.4.0,0.0,0.05,"<|>");
  ar7.SetArrowSize(0.02); ar7.SetAngle(40);
  ar7.SetLineWidth(2); ar7.Draw();

  TArrow ar8(196.6,0.0,232.4.0,0.0,0.05,"<|>");
  ar8.SetArrowSize(0.02); ar8.SetAngle(40);
  ar8.SetLineWidth(2); ar8.Draw();

  TLatex* taga = new TLatex(.24, .085, "TIB1-4");
  taga->SetNDC(kTRUE);  taga->SetTextSize(0.04);  taga->Draw();
  TLatex* tagg = new TLatex(.425, .085, "TOB1");
  tagg->SetNDC(kTRUE);  tagg->SetTextSize(0.04);  tagg->Draw();
  TLatex* tagb = new TLatex(.485, .085, "TOB2");
  tagb->SetNDC(kTRUE);  tagb->SetTextSize(0.04);  tagb->Draw();
  TLatex* tagc = new TLatex(.562, .085, "TOB3");
  tagc->SetNDC(kTRUE);  tagc->SetTextSize(0.04);  tagc->Draw();
  TLatex* tagd = new TLatex(.665, .085, "TOB4");
  tagd->SetNDC(kTRUE);  tagd->SetTextSize(0.04);  tagd->Draw();
  TLatex* tage = new TLatex(.777, .085, "TOB5");
  tage->SetNDC(kTRUE);  tage->SetTextSize(0.04);  tage->Draw();
  TLatex* tagf = new TLatex(.885, .085, "TOB6");
  tagf->SetNDC(kTRUE);  tagf->SetTextSize(0.04);  tagf->Draw();

  c1->SaveAs("Resolution.eps");
  //c1->SaveAs("Resolution.ps");
  //c1->Print("Resolution.pdf","pdf");
}



TArrow getHorAr(double start, double end){
  TArrow ar(start,0.0,end,0.0,0.05,"<|>");
  //TArrow ar(0.0,0.0,4.0,0.0,0.05,"<|>");
  ar.SetAngle(40);
  ar.SetLineWidth(2);
  return ar;
}


void setTDRStyle() {

   TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
   tdrStyle->SetCanvasBorderMode(0);
   tdrStyle->SetCanvasColor(kWhite);
   tdrStyle->SetCanvasDefH(600); //Height of canvas
   tdrStyle->SetCanvasDefW(1100); //Width of canvas
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
   // tdrStyle->SetHistFillColor(1);
   // tdrStyle->SetHistFillStyle(0);
   //tdrStyle->SetHistLineColor(1);
   //tdrStyle->SetHistLineStyle(0);
   tdrStyle->SetHistLineWidth(1.5);
   // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
   // tdrStyle->SetNumberContours(Int_t number = 20);

//   tdrStyle->SetEndErrorSize(2);
//   tdrStyle->SetErrorMarker(20);
//   tdrStyle->SetErrorX(0.);

   //tdrStyle->SetMarkerStyle(20);

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

// // For the statistics box:
//   tdrStyle->SetOptFile(0);
//   tdrStyle->SetOptStat(0); // To display the mean and RMS:    
//   tdrStyle->SetOptStat("mr");
   tdrStyle->SetOptStat("");
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
   tdrStyle->SetPadLeftMargin(0.10);
   tdrStyle->SetPadRightMargin(0.02);

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
   //tdrStyle->SetTitleSize(0.06, "XYZ");
   tdrStyle->SetTitleSize(0.052, "XYZ");
   // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
   // tdrStyle->SetTitleYSize(Float_t size = 0.02);
   //tdrStyle->SetTitleXOffset(0.9);
   tdrStyle->SetTitleXOffset(0.875);
   tdrStyle->SetTitleYOffset(0.90);
   //tdrStyle->SetTitleYOffset(1.05);
   // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the  Offset

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
   tdrStyle->SetPaperSize(20.,20.);
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
