{

gROOT->Reset();



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



TFile DATA900GeV("output_DATA900GeV.root");
//TFile MC900GeV("output_MC900GeV.root");
TFile DATA2360GeV("output_DATA2360GeV.root");
//TFile MC2360GeV("output_MC2360GeV.root");


//900 GeV

TCanvas c_calomet_900GeV;

c_calomet_900GeV.SetLogy();
// c_calomet_900GeV.SetGridx();
// c_calomet_900GeV.SetGridy();

DATA900GeV.cd();
h_calometPt_baseSel->GetXaxis()->SetRangeUser(0,80);

h_calometPt_baseSel->SetTitle("");
//h_calometPt_baseSel_HFfilter->SetTitle("");

h_calometPt_baseSel->SetLineColor(kBlack);
h_calometPt_baseSel_HFfilter->SetLineColor(kRed);
h_calometPt->SetLineColor(kGreen+1);
h_calometPt->SetLineWidth(2);

h_calometPt_baseSel->SetMarkerColor(kBlack);
h_calometPt_baseSel_HFfilter->SetMarkerColor(kRed);
h_calometPt->SetMarkerColor(kGreen+1);

h_calometPt_baseSel->SetFillColor(kBlack);
h_calometPt_baseSel_HFfilter->SetFillColor(kRed);
//h_calometPt->SetFillColor(kGreen+1);


h_calometPt_baseSel->SetMarkerStyle(20);
h_calometPt_baseSel_HFfilter->SetMarkerStyle(26);
h_calometPt->SetMarkerStyle(23);

h_calometPt_baseSel->SetMarkerSize(0.8);
h_calometPt_baseSel_HFfilter->SetMarkerSize(0.8);
h_calometPt->SetMarkerSize(0.8);

h_calometPt_baseSel->GetXaxis()->SetTitle("#slash{E}_{T} [GeV]");

h_calometPt_baseSel->Draw("");
h_calometPt_baseSel_HFfilter->Draw("sames");
h_calometPt->Draw("sameshist");
//h_calometPt_baseSel->Draw("same");

gPad->RedrawAxis();

TLegend *legend_900 = new TLegend(0.456376,0.449301,0.901007,0.708042);
legend_900->SetFillColor(0);
legend_900->AddEntry(h_calometPt_baseSel,"No cleaning","pl");
legend_900->AddEntry(h_calometPt_baseSel_HFfilter,"After HF noise cleaning","pl");
legend_900->AddEntry(h_calometPt,"After HF+ECAL noise cleaning","l");
legend_900->SetBorderSize(0);
//Draw legend
legend_900->Draw();


TLatex l_900;
l_900.SetTextAlign(12);
l_900.SetTextSize(0.04);
l_900.SetTextFont(62);
l_900.SetNDC();
l_900.DrawLatex(0.533557,0.872378," #splitline{CMS 2009 Preliminary}{#sqrt{s}=900 GeV}");

c_calomet_900GeV.SaveAs("calometPt_afterFilters_900.eps");



//2360 GeV

TCanvas c_calomet_2360GeV;

c_calomet_2360GeV.SetLogy();
// c_calomet_2360GeV.SetGridx();
// c_calomet_2360GeV.SetGridy();

DATA2360GeV.cd();
h_calometPt_baseSel->GetXaxis()->SetRangeUser(0,80);

h_calometPt_baseSel->SetTitle("");
//h_calometPt_baseSel_HFfilter->SetTitle("");

h_calometPt_baseSel->SetLineColor(kBlack);
h_calometPt_baseSel_HFfilter->SetLineColor(kRed);
h_calometPt->SetLineColor(kGreen+1);
h_calometPt->SetLineWidth(2);

h_calometPt_baseSel->SetMarkerColor(kBlack);
h_calometPt_baseSel_HFfilter->SetMarkerColor(kRed);
h_calometPt->SetMarkerColor(kGreen+1);

h_calometPt_baseSel->SetFillColor(kBlack);
h_calometPt_baseSel_HFfilter->SetFillColor(kRed);
//h_calometPt->SetFillColor(kGreen+1);


h_calometPt_baseSel->SetMarkerStyle(20);
h_calometPt_baseSel_HFfilter->SetMarkerStyle(26);
h_calometPt->SetMarkerStyle(23);

h_calometPt_baseSel->SetMarkerSize(0.8);
h_calometPt_baseSel_HFfilter->SetMarkerSize(0.8);
h_calometPt->SetMarkerSize(0.8);

h_calometPt_baseSel->GetXaxis()->SetTitle("#slash{E}_{T} [GeV]");

h_calometPt_baseSel->Draw("");
h_calometPt_baseSel_HFfilter->Draw("sames");
h_calometPt->Draw("sameshist");
//h_calometPt_baseSel->Draw("same");

gPad->RedrawAxis();

TLegend *legend_2360 = new TLegend(0.456376,0.449301,0.901007,0.708042);
legend_2360->SetFillColor(0);
legend_2360->AddEntry(h_calometPt_baseSel,"No cleaning","pl");
legend_2360->AddEntry(h_calometPt_baseSel_HFfilter,"After HF noise cleaning","pl");
legend_2360->AddEntry(h_calometPt,"After HF+ECAL noise cleaning","l");
legend_2360->SetBorderSize(0);
//Draw legend
legend_2360->Draw();


TLatex l_2360;
l_2360.SetTextAlign(12);
l_2360.SetTextSize(0.04);
l_2360.SetTextFont(62);
l_2360.SetNDC();
l_2360.DrawLatex(0.533557,0.872378," #splitline{CMS 2009 Preliminary}{#sqrt{s}=2360 GeV}");

c_calomet_2360GeV.SaveAs("calometPt_afterFilters_2360.eps");

}
