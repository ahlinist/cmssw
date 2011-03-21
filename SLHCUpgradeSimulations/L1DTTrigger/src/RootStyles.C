//#include "tdrstyle.h"
#include <TROOT.h>
#include "TStyle.h"
#include <TPad.h>

#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1slhcPlots.h"
#include "SLHCUpgradeSimulations/L1DTTrigger/interface/RootStyles.h"

/*****************
just after root

.L tdrstyle.C
gROOT->Reset();
setTDRStyle();

*****************/

using namespace std;

//-------------------------------------------------------------------------------
void palette()
{

  //  gStyle->SetPalette(1);

  // use the "second standard" above OR...

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

}



//-------------------------------------------------------------------------------
void DTL1slhcPlots::setIStyle() {

  // For the Global title:
  IStyle->SetOptTitle(1); // In general I want global title!
  IStyle->SetTitleFont(42);
  IStyle->SetTitleColor(1);
  IStyle->SetTitleTextColor(1);
  IStyle->SetTitleFillColor(10);
  IStyle->SetTitleFontSize(0.05);

  IStyle->SetCanvasBorderMode(0);
  IStyle->SetCanvasColor(kWhite);
  IStyle->SetCanvasDefH(400);     //Height of canvas
  IStyle->SetCanvasDefW(600);     //Width of canvas
  IStyle->SetPadBorderMode(0);
  IStyle->SetPadColor(kWhite);
  /*
  // Pad Margins:
  //-----------------------------
  IStyle->SetPadTopMargin(0.05);
  IStyle->SetPadBottomMargin(0.13);
  IStyle->SetPadLeftMargin(0.16);
  IStyle->SetPadRightMargin(0.02);
  */

  // For the frame:
  //-----------------------------
  IStyle->SetFrameBorderMode(0);
  IStyle->SetFrameBorderSize(1);
  IStyle->SetFrameFillColor(kWhite);
  IStyle->SetFrameFillStyle(0);
  IStyle->SetFrameLineColor(1);
  IStyle->SetFrameLineStyle(1);
  IStyle->SetFrameLineWidth(1);

  // For the histo:
  //-----------------------------
  IStyle->SetHistLineWidth(1);
  IStyle->SetHistLineColor(kBlack);
  IStyle->SetEndErrorSize(2);
  IStyle->SetErrorX(0.);
  // IStyle->SetMarkerStyle(kFullDotSmall); 
  // IStyle->SetMarkerStyle(kFullDotMedium); 
  IStyle->SetMarkerStyle(kFullDotLarge);
  // IStyle->SetMarkerStyle(kFullCircle);


  // For the statistics box:
  //-----------------------------
  /*
    gStyle->SetOptStat("option-string");

    Where option string can contain:-
    n ;  name of histogram is printed
    e ;  number of entries printed
    m ;  mean value printed
    r ;  rms printed
    u ;  number of underflows printed
    o ;  number of overflows printed
    i ;  integral of bins printed
    Example: gStyle->SetOptStat("ne");
    print only name of histogram and number of entries.

    To remove it altogther:-

  gStyle->SetOptStat("");
  */
  // IStyle->SetOptStat("emr");        
  /*
  The parameter mode can be = ksiourmen  (default = 000001111)
    k = 1;  kurtosis printed
    k = 2;  kurtosis and kurtosis error printed
    s = 1;  skewness printed
    s = 2;  skewness and skewness error printed
    i = 1;  integral of bins printed
    o = 1;  number of overflows printed
    u = 1;  number of underflows printed
    r = 1;  rms printed
    r = 2;  rms and rms error printed
    m = 1;  mean value printed
    m = 2;  mean and mean error values printed
    e = 1;  number of entries printed
    n = 1;  name of histogram is printed
  Example: gStyle->SetOptStat(11);
           print only name of histogram and number of entries.
           gStyle->SetOptStat(1101);  displays the name of histogram, mean value and RMS.
  WARNING: never call SetOptStat(000111); but SetOptStat(1111), 0001111 will
          be taken as an octal number !!
  WARNING: SetOptStat(1) is taken as SetOptStat(1111) (for back compatibility
           with older versions. If you want to print only the name of the histogram
           call SetOptStat(1000000001).
  NOTE that in case of 2-D histograms, when selecting just underflow (10000)
        or overflow (100000), the stats box will show all combinations
        of underflow/overflows and not just one single number!
  */

  //  IStyle->SetOptStat(1110);  
  IStyle->SetOptStat(0); 

  IStyle->SetStatColor(kWhite);
  IStyle->SetStatFont(42);
 
  // For fitting:
  //-----------------------------
  IStyle->SetFuncWidth(1);         // for the fit function 
  IStyle->SetFuncColor(kBlack);    // for the fit function 
  IStyle->SetOptFit(111);          // IStyle->SetOptFit(0) to get rid of
  IStyle->SetStatW(0.18);          // set width of stat box
  IStyle->SetStatFontSize(0.04);   // set the font size for stat box

  /*
  // Change for log plots:
  IStyle->SetOptLogx(0);
  IStyle->SetOptLogy(0);
  IStyle->SetOptLogz(0)
  */

  // IStyle->SetPalette(1);
  palette();
  IStyle->cd(); // this becomes now the current style gStyle

}



//-------------------------------------------------------------------------------
void setTDRStyle() {
  TStyle* tdrStyle = new TStyle("tdrStyle","Style for P-TDR");   

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
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);
  
  tdrStyle->SetEndErrorSize(2);
  //tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
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
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
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
  //  tdrStyle->SetPaperSize(20.,20.);                                      // Ignazio
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

/*
// tdrGrid: Turns the grid lines on (true) or off (false)

void tdrGrid(bool gridOn) {
  tdrStyle->SetPadGridX(gridOn);
  tdrStyle->SetPadGridY(gridOn);
}

// fixOverlay: Redraws the axis

void fixOverlay() {
  gPad->RedrawAxis();
}

*/
