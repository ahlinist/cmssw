
void plotHitResAlpha(){
  setTDRStyle();
  gROOT->SetStyle("tdrStyle");
  gStyle->SetOptFit(111111);
  // load the code before executing the macro:
  gROOT->LoadMacro("TrackingAnalysis/Cosmics/test/OverlapHistos.C+");

  ol = new OverlapHistos();
  ol->Loop(); 
  
  TGraphErrors* alpha2VsAngle_TOB2 = new TGraphErrors();
  TGraphErrors* alpha2VsAngle_TOB3 = new TGraphErrors();
  TGraphErrors* alpha2VsAngle_TOB4 = new TGraphErrors();
  TGraphErrors* alpha2VsAngle_allTOB = new TGraphErrors();
  
  const int predScaleFactor(1.);  // scaling of predicted errors for
                                  // evaluation of systematics

  int n_tot = layer->GetEntries();
  int n_layer2 = 0;
  int n_layer3 = 0;
  int n_layer4 = 0;
  int n_TOB = 0;
  float dxdzm, dxdze;
  float widthm, widthe;
  float predm, prede;
  float hitm, hite;
  float alpha, alphae;
  
  for (int i = 1; i<=n_tot; i++){
    if ( dxdz->GetBinContent(i) > 0 ) { 
      dxdzm = dxdz->GetBinContent(i); 
    } else {
      dxdzm = -dxdz->GetBinContent(i);
    }
    
    dxdze = (dxdz->GetBinError(i));
    widthm = (width->GetBinContent(i));
    widthe = (width->GetBinError(i));
    predm = sigMean->GetBinContent(i);
    prede = sigMean->GetBinError(i);
    hitm = hitSigX->GetBinContent(i);
    hite = hitSigX->GetBinError(i);

    predm *= predScaleFactor;   // systematics
    prede *= predScaleFactor;
    alpha = (widthm*widthm-predm*predm)/(hitm*hitm);
//    alphae = (2/(hitm*hitm))*(widthm*widthe+predm*prede+(widthm*widthm-predm*predm)*(hite/hitm));
    alphae = widthm*widthm*widthe*widthe;
    alphae += predm*predm*prede*prede;
    alphae += alpha*alpha*hitm*hitm*hite*hite;
    alphae = 2/(hitm*hitm)*sqrt(alphae);

    if (layer->GetBinContent(i)==6){
      alpha2VsAngle_TOB2->SetPoint(n_layer2,dxdzm, alpha);
      alpha2VsAngle_TOB2->SetPointError(n_layer2,dxdze, alphae);
      n_layer2++;
    }
    if (layer->GetBinContent(i)==7){
      alpha2VsAngle_TOB3->SetPoint(n_layer3,dxdzm, alpha);
      alpha2VsAngle_TOB3->SetPointError(n_layer3,dxdze, alphae);
      n_layer3++;
    }
    if (layer->GetBinContent(i)==8){
      alpha2VsAngle_TOB4->SetPoint(n_layer4,dxdzm, alpha);
      alpha2VsAngle_TOB4->SetPointError(n_layer4,dxdze,alphae);
      //error on alpha**2 is (2/x**2)*(we*w+pe*p+(w**2-p**2)*(xe/x))
      n_layer4++;
    }
    if (layer->GetBinContent(i)==5 || layer->GetBinContent(i)==6 
	|| layer->GetBinContent(i)==7 || layer->GetBinContent(i)==8 
	|| layer->GetBinContent(i)==9 || layer->GetBinContent(i)==10 ) {
      alpha2VsAngle_allTOB->SetPoint(n_TOB,dxdzm, alpha);
      alpha2VsAngle_allTOB->SetPointError(n_TOB,dxdze, alphae);
      n_TOB++;
    }

  }

  c1 = new TCanvas("c1","alpha squared",0,0,900,600);
  c1->Divide(2,2);
 
  c1->cd(1);
  plot(alpha2VsAngle_TOB2, "TOB2 only");
  //TPaveText tx(0.0,0.0,205.0,8.0);
  //TText *t=tx.AddText("a");
  //tx.Draw();
  TLatex* taga = new TLatex(.22, .20, "(a)");
  taga->SetNDC(kTRUE);  taga->SetTextSize(0.05);  taga->Draw();
  c1->cd(2);
  plot(alpha2VsAngle_TOB3, "TOB3 only");
  TLatex* tagb = new TLatex(.22, .85, "(b)");
  tagb->SetNDC(kTRUE);  tagb->SetTextSize(0.05);  tagb->Draw();
  c1->cd(3);
  plot(alpha2VsAngle_TOB4, "TOB4 only");
  TLatex* tagc = new TLatex(.22, .85, "(c)");
  tagc->SetNDC(kTRUE);  tagc->SetTextSize(0.05);  tagc->Draw();
  c1->cd(4);
  plot(alpha2VsAngle_allTOB);
  TLatex* tagd = new TLatex(.22, .85, "(d)");
  tagd->SetNDC(kTRUE);  tagd->SetTextSize(0.05);  tagd->Draw();
  c1->SaveAs("pass4skim_alpha2.eps");
  //  c1->Print("pass4skim_alpha2.pdf","pdf");
}

void plot(TGraphErrors* tg, string n)
{
  tg->SetTitle(TString(n));
  tg->SetMarkerColor(4);
  tg->SetMarkerStyle(21);
  tg->GetXaxis()->SetTitle("|dx/dz|");
  tg->GetYaxis()->SetTitle("#alpha^{2}");
  tg->Fit("pol1","Q");
  tg->Draw("AP");  
}

void plot(TGraphErrors* tg)
{
  tg->SetTitle(TString("All TOB layers"));
  tg->SetMarkerColor(4);
  tg->SetMarkerStyle(21);
  tg->GetXaxis()->SetTitle("|dx/dz|");
  tg->GetYaxis()->SetTitle("#alpha^{2}");
  tg->Fit("pol1","Q");
  tg->Draw("AP");  
}



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
   // tdrStyle->SetHistFillColor(1);
   // tdrStyle->SetHistFillStyle(0);
   //tdrStyle->SetHistLineColor(1);
   //tdrStyle->SetHistLineStyle(0);
   tdrStyle->SetHistLineWidth(3);
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
   tdrStyle->SetPadLeftMargin(0.16);
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
   tdrStyle->SetTitleYOffset(1.25);
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
