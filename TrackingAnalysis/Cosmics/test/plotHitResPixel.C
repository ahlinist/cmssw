void plotHitResPixelPublication(char* name){
  setTDRStyle();
  gROOT->SetStyle("tdrStyle");
  gStyle->SetErrorX(0);
  gStyle->SetOptStat(0);  

  TString file = TString("overlaps_") + name;
  //  TFile* f = TFile::Open(file+"_hitXcut.root");
  //TFile* f = TFile::Open("overlaps_p5_0to30_pixelY_noEdge.root");
  TFile* f = TFile::Open("overlaps_p5_0to30X_pixel_V11.root");
  //TFile* f = TFile::Open("overlaps_p5_0to30Y_pixel_V11.root");

  int n_tot = width->GetEntries();
  cout << "n_tot = " << n_tot << endl;
  int n;

  float widthm, widthe;
  float predm, prede;
  float hitm, hite;
  float hitmdata, hitedata;
  float ratio, ratioe;

  float widthmY, widtheY;
  float predmY, predeY;
  float hitmY, hiteY;
  float hitmdataY, hitedataY;
  float ratioY, ratioeY;

  float slopeX, slopeY;
  float rangeX, rangeY;

  // loop through entries to see how many are cut by the misalignment requirement
  int i;
  int bins = 0;
  for (int i = 1; i<=n_tot; i++) {
    slopeX = ddXslope->GetBinContent(i);
    slopeY = ddYslope->GetBinContent(i);
    rangeX = x->GetBinError(i);
    rangeY = y->GetBinError(i);
    
    cout << "slopeX = " << slopeX << " and rangeX = " << rangeX << ", slopeY = " << slopeY << " and rangeY = " << rangeY << "               " << slopeX*rangeX << " and " << slopeY*rangeY <<   endl;
    
    if ( (slopeX*rangeX)>0.001 || (slopeY*rangeY)>0.001 ||  (slopeX*rangeX)<-0.001 || (slopeY*rangeY)<-0.001 ) {
      cout << "cutting from misalignment: " << slopeX*rangeX << " or " << slopeY*rangeY << endl;
      continue;
    }
    bins++;    
  }

  cout << "bins = " << bins << endl;

  TH1F* hitres = new TH1F("hitres", "hit resolution ", bins, -0.5, bins-0.5);
  TH1F* hitsigma = new TH1F("hitsig", "hit position uncertainty", bins, -0.5, bins-0.5);
  TH1F* ratio_h = new TH1F("ratio", "ratio ", bins, -0.5, bins-0.5);
  TH1F* dd = new TH1F("dd", "double difference width ", bins, -0.5, bins-0.5);
  TH1F* pred = new TH1F("pred", "mean pred uncertainty ", bins, -0.5, bins-0.5);

  TH1F* hitresY = new TH1F("hitresY", "hit resolution in y", bins, -0.5, bins-0.5);
  TH1F* hitsigmaY = new TH1F("hitsigY", "hit position uncertainty in Y", bins, -0.5, bins-0.5);
  TH1F* ratio_hY = new TH1F("ratioY", "ratio in Y", bins, -0.5, bins-0.5);
  TH1F* ddY = new TH1F("ddY", "double difference width in Y", bins, -0.5, bins-0.5);
  TH1F* predY = new TH1F("predY", "mean pred uncertainty in Y", bins, -0.5, bins-0.5);

  TH1F* hitres1D = new TH1F("hitres1D", "hit resolution", 10, 0, 50);
  TH1F* hitresY1D = new TH1F("hitresY1D", "hit resolution in y", 10, 20, 70);

  int currentbin = 0;
  for (int i = 1; i<=n_tot; i++){
    widthm = width->GetBinContent(i);
    widthe = width->GetBinError(i);
    predm = sigMean->GetBinContent(i);
    prede = sigMean->GetBinError(i);
    hitm = hitSigX->GetBinContent(i);
    hite = hitSigX->GetBinError(i);

    widthmY = widthY->GetBinContent(i);
    widtheY = widthY->GetBinError(i);
    predmY = sigMeanY->GetBinContent(i);
    predeY = sigMeanY->GetBinError(i);
    hitmY = hitSigY->GetBinContent(i);
    hiteY = hitSigY->GetBinError(i);

    slopeX = ddXslope->GetBinContent(i);
    slopeY = ddYslope->GetBinContent(i);
    rangeX = x->GetBinError(i);
    rangeY = y->GetBinError(i);

    if ( (slopeX*rangeX)>0.001 || (slopeY*rangeY)>0.001 ||  (slopeX*rangeX)<-0.001 || (slopeY*rangeY)<-0.001 ) {
      continue;
    }

    currentbin++;

    //add on an extra 5 microns of uncertainty to the fitted width because of the alignment
    widthe = sqrt(widthe*widthe+(5*5));
    
    if (widthm>predm) {
      hitmdata = sqrt(widthm*widthm-predm*predm);
      hitedata = sqrt(widthe*widthe+prede*prede);
    } else {
      cout << "BAD point: pred mean > dd width with entry = " << i << " and mean = "<< predm << " and widthm = " << widthm << endl;
    }

    if (widthmY>predmY) {
      hitmdataY = sqrt(widthmY*widthmY-predmY*predmY);
      hitedataY = sqrt(widtheY*widtheY+predeY*predeY);
    } else {
      cout << "BAD point: pred mean > dd width with entry = " << i << " and mean = "<< predm << " and widthm = " << widthm << endl;
    }
    
    ratio = hitmdata/hitm;
    ratioe = sqrt( ((hite/hitm)*(hite/hitm)) + ((hitedata/hitmdata)*(hitedata/hitmdata)) )*ratio;

    ratioY = hitmdataY/hitmY;
    ratioeY = sqrt( ((hiteY/hitmY)*(hiteY/hitmY)) + ((hitedataY/hitmdataY)*(hitedataY/hitmdataY)) )*ratioY;
    
    hitres->SetBinContent(currentbin,hitmdata/sqrt(2));
    hitres->SetBinError(currentbin,hitedata/sqrt(2));
    hitres1D->Fill(hitmdata/sqrt(2));
    hitsigma->SetBinContent(currentbin,hitm);
    hitsigma->SetBinError(currentbin,hite);
    ratio_h->SetBinContent(currentbin,ratio);
    ratio_h->SetBinError(currentbin,ratioe);
    dd->SetBinContent(currentbin,widthm);
    dd->SetBinError(currentbin,widthe);
    pred->SetBinContent(currentbin, predm);
    pred->SetBinError(currentbin, prede);
    
    hitresY->SetBinContent(currentbin,hitmdataY/sqrt(2));
    hitresY->SetBinError(currentbin,hitedataY/sqrt(2));
    hitresY1D->Fill(hitmdataY/sqrt(2));
    hitsigmaY->SetBinContent(currentbin,hitmY);
    hitsigmaY->SetBinError(currentbin,hiteY);
    ratio_hY->SetBinContent(currentbin,ratioY);
    ratio_hY->SetBinError(currentbin,ratioeY);
    ddY->SetBinContent(currentbin,widthmY);
    ddY->SetBinError(currentbin,widtheY);
    predY->SetBinContent(currentbin, predmY);
    predY->SetBinError(currentbin, predeY);
    
  }

  //first do plot to show how resolution is found
  //dd->SetLineColor(2);
  dd->GetYaxis()->SetTitle("Resolution [#mum]");
  dd->GetXaxis()->SetTitle("");
  dd->GetXaxis()->SetLabelSize(0);
  dd->GetXaxis()->SetTickLength(0);
  dd->SetMarkerStyle(24);    //20
  //dd->SetMarkerColor(2);
  //hitres->SetLineColor(4);
  hitres->SetMarkerStyle(25);  //21
  //hitres->SetMarkerColor(4);
  //pred->SetLineColor(1);
  pred->SetMarkerStyle(26);
  //pred->SetMarkerColor(1);
  //dd->SetMaximum(80);
  dd->SetMinimum(0);
  dd->Draw();
  pred->Draw("same");
  hitres->Draw("same");

  hitres1D->GetXaxis()->SetTitle("Pixel resolution in x");
  hitres1D->Draw();
  c1->SaveAs("hitresX1D.eps");

  hitresY1D->GetXaxis()->SetTitle("Pixel resolution in y");
  hitresY1D->Draw();
  c1->SaveAs("hitresY1D.eps");

  leg = new TLegend(0.30,0.72,0.64,0.91);
  leg->AddEntry(dd,"double difference width","p");
  leg->AddEntry(hitres,"Hit resolution from data","p");
  leg->AddEntry(pred,"<#sigma(#Deltax_{pred})>","p");
  leg->SetTextSize(0.055);
  leg->SetFillColor(0);
  //leg->Draw("same");

  c1->SaveAs("showResPixel.png");


  hitres->Fit("pol0");  
  hitres->GetFunction("pol0")->SetLineColor(1);
  hitres->SetLineColor(1);
  hitres->GetYaxis()->SetTitle("Resolution in x [#mum]");
  hitres->GetXaxis()->SetTitle("overlap measurement");
  hitres->GetXaxis()->SetLabelSize(0);
  hitres->GetXaxis()->SetTickLength(0);
  hitres->SetMarkerStyle(24);    //20
  //hitres->SetMarkerColor(2);
  //hitsigma->SetLineColor(4);
  hitsigma->SetMarkerStyle(25);  //21
  //hitsigma->SetMarkerColor(4);
  //hitres->SetMaximum(100);
  hitres->Draw();
  //hitsigma->Draw("same");
  
  leg_hist = new TLegend(0.55,0.76,0.85,0.90);
  leg_hist->AddEntry(hitres,"X resolution from data","p");
  //  leg_hist->AddEntry(hitsigma,"<calculated uncertainty on #Deltax>","p");
  leg_hist->SetTextSize(0.055);
  leg_hist->SetFillColor(0);
  //leg_hist->Draw("same");

  TText* txt=new TText(0.15, 0.88, "CMS 2008");
  txt->SetNDC(kTRUE); // <- use NDC coordinate
  txt->SetTextSize(0.06);
  txt->Draw("same");

  //TPaveStats *st1 = (TPaveStats*)hitres->FindObject("stats");
  //use wrong name to enlarge box by hand because I don't know how to
  //do it in the script!
  //st2->Draw("same");

  TString fileRes = TString("Resolution_") + name;
  TString fileRatio = TString("ratio_") + name;
  c1->SaveAs(fileRes+"PixelX.eps");
  c1->SaveAs(fileRes+"PixelX.png");
  c1->SaveAs(fileRes+"PixelX.pdf");
  c1->SaveAs(fileRes+"PixelX.C");  

  ratio_h->Fit("pol0");
  ratio_h->GetXaxis()->SetTitle("Ratio of resolution in x from data and calculation");
  ratio_h->GetXaxis()->SetLabelSize(0);
  ratio_h->GetXaxis()->SetTickLength(0);
  ratio_h->Draw();
  c1->SaveAs(fileRatio+"PixelX.eps");
  c1->SaveAs(fileRatio+"PixelX.png");

  hitresY->Fit("pol0");  
  hitresY->GetFunction("pol0")->SetLineColor(1);
  hitresY->SetLineColor(1);
  hitresY->GetYaxis()->SetTitle("Resolution in y [#mum]");
  hitresY->GetXaxis()->SetTitle("overlap measurement");
  hitresY->GetXaxis()->SetLabelSize(0);
  hitresY->GetXaxis()->SetTickLength(0);
  hitresY->SetMarkerStyle(24);    //20
  //hitresY->SetMarkerColor(2);
  //hitsigmaY->SetLineColor(4);
  hitsigmaY->SetMarkerStyle(25);  //21
  //hitsigmaY->SetMarkerColor(4);
  hitresY->SetMaximum(70);
  hitresY->Draw();
  //hitsigmaY->Draw("same");
  
  leg_histY = new TLegend(0.55,0.76,0.85,0.90);
  leg_histY->AddEntry(hitres,"Y resolution from data","p");
  //  leg_histY->AddEntry(hitsigma,"<calculated uncertainty on #Deltay>","p");
  leg_histY->SetTextSize(0.055);
  leg_histY->SetFillColor(0);
  //leg_histY->Draw("same");

  TText* txt2=new TText(0.15, 0.88, "CMS 2008");
  txt2->SetNDC(kTRUE); // <- use NDC coordinate
  txt2->SetTextSize(0.06);
  txt2->Draw("same");

  TPaveStats *st1 = (TPaveStats*)hitres->FindObject("stats");
  //use wrong name to enlarge box by hand because I don't know how to
  //do it in the script!
  //st2->Draw("same");

  TString fileResY = TString("Resolution_") + name;
  TString fileRatioY = TString("ratio_") + name;
  c1->SaveAs(fileResY+"PixelY.eps");
  c1->SaveAs(fileResY+"PixelY.png");
  c1->SaveAs(fileResY+"PixelY.pdf");
  c1->SaveAs(fileResY+"PixelY.C");

  ratio_hY->Fit("pol0");
  ratio_hY->GetXaxis()->SetTitle("Ratio of resolution in y from data and calculation");
  ratio_hY->GetXaxis()->SetLabelSize(0);
  ratio_hY->GetXaxis()->SetTickLength(0);
  ratio_hY->Draw();
  c1->SaveAs(fileRatio+"PixelY.eps");
  c1->SaveAs(fileRatio+"PixelY.png");

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
   //tdrStyle->SetOptFit(1);
   //tdrStyle->SetFitFormat("5.4g");
   //tdrStyle->SetFuncColor(2);
   //tdrStyle->SetFuncStyle(1);
   //tdrStyle->SetFuncWidth(1);

//For the date:
   //tdrStyle->SetOptDate(0);
   // tdrStyle->SetDateX(Float_t x = 0.01);
   // tdrStyle->SetDateY(Float_t y = 0.01);

// // For the statistics box:
//   tdrStyle->SetOptFile(0);
//   tdrStyle->SetOptStat(0); // To display the mean and RMS:    
//   tdrStyle->SetOptStat(000000);
//   tdrStyle->SetOptStat("mr");
   // tdrStyle->SetOptStat("");
   //tdrStyle->SetStatColor(kWhite);
   //tdrStyle->SetStatFont(42);
   //tdrStyle->SetStatFontSize(0.025);
   //tdrStyle->SetStatTextColor(1);
   //tdrStyle->SetStatFormat("6.4g");
   //tdrStyle->SetStatBorderSize(1);
   //tdrStyle->SetStatH(0.1);
   //tdrStyle->SetStatW(0.15);
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
   tdrStyle->SetTitleSize(0.08, "XYZ");
   // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
   // tdrStyle->SetTitleYSize(Float_t size = 0.02);
   //tdrStyle->SetTitleXOffset(0.9);
   tdrStyle->SetTitleXOffset(0.675);
   tdrStyle->SetTitleYOffset(0.60);
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
