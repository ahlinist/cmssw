{
  //--------------------------------------------------------------------
  //
  // File       : d0_jes_style.C
  // Author     : Jeroen Hegeman
  //              (jhegeman@fnal.gov)
  // Last change: 20060914T0822UTC
  // Version    : 1.4
  //
  // Purpose    : One common style for the D0 JES group.
  //
  // Usage      : Just add the following lines to your rootlogon.C:
  //              gROOT->ProcessLine(".x ./d0_jes_style.C");
  //              gROOT->ForceStyle();
  //
  //              NOTE: The 'ForceStyle()' trick will still result in
  //                    histograms that are read from file looking
  //                    slightly different from the ones that are
  //                    created with this style.
  //                    For example: it cannot move the positions
  //                    of statistics boxed in histograms that are read
  //                    from file.
  //
  //--------------------------------------------------------------------

  #include <iostream>

  using std::cout;
  using std::endl;

  cout << "=============================================" << endl;
  cout << endl;
  cout << "  Loading D0 JES Group ROOT Style (v1.3)" << endl;
  cout << "    Glued together by Jeroen Hegeman" << endl;
  cout << "    (jhegeman@fnal.gov)" << endl;
  cout << endl;
  cout << "  This style is a loose combination of the D0" << endl;
  cout << "  style and some things I picked up from" << endl;
  cout << "  several places on the web." << endl;
  cout << endl;
  cout << "=============================================" << endl;

  //------------------------

  TStyle* d0jesStyle= new TStyle;
  // Start from the builtin 'Plain' ROOT style.
  (*d0jesStyle) = *(gROOT->GetStyle("Plain"));
  d0jesStyle->SetName("d0jesStyle");
  d0jesStyle->SetTitle("D0 JES Group common style");
  gROOT->GetListOfStyles()->Add(d0jesStyle);
  gROOT->SetStyle("d0jesStyle");
  gROOT->ForceStyle();

  //------------------------

  // A bunch of color definitions.
  Color_t kColorOffset = kBlack;
  Color_t kResponse    = kRed;
  Color_t kShowering   = kBlue;

  // And style definitions.
  Style_t kLineStyleOffset    = kSolid;
  Style_t kLineStyleResponse  = kDashed;
  Style_t kLineStyleShowering = kDotted;

  Style_t kMarkerStyleOffset    = kPlus;
  Style_t kMarkerStyleResponse  = kPlus;
  Style_t kMarkerStyleShowering = kPlus;

  //------------------------

  // Line thickness.
  Width_t d0jesLineWidth      = 2.;
  Width_t d0jesFrameLineWidth = 1.;

  //------------------------

  // No frills.
  d0jesStyle->SetFrameBorderMode(0);
  d0jesStyle->SetCanvasBorderMode(0);
  d0jesStyle->SetPadBorderMode(0);

  //------------------------

  // White backgrounds.
  d0jesStyle->SetPadColor(kWhite);
  d0jesStyle->SetCanvasColor(kWhite);
  d0jesStyle->SetStatColor(kWhite);
  d0jesStyle->SetTitleColor(kWhite,"");
  //  d0jesStyle->SetFillColor(kWhite);

  //------------------------

  // Nicer colors.
  d0jesStyle->SetPalette(1,0);

  //------------------------

  // Paper sizes and margins.
  d0jesStyle->SetPaperSize(TStyle::kUSLetter);
  d0jesStyle->SetPadTopMargin(0.05);//0.1);
  d0jesStyle->SetPadBottomMargin(0.15);//0.14);
  d0jesStyle->SetPadLeftMargin(0.15);//0.17);//0.12);
  d0jesStyle->SetPadRightMargin(0.05);//0.03);//0.1);

  //------------------------

  // Font stuff.

  // Font: helvetica-bold-r-normal, precision 2.
  Font_t d0jesFont           = 42;
  Font_t d0jesFontBold       = 62;
  Float_t d0jesTextSize      = .08;
  Float_t d0jesTextSizeSmall = .04;
  Short_t d0jesTextAlign     = 12;

  d0jesStyle->SetTextFont(d0jesFont);
  d0jesStyle->SetTextSize(d0jesTextSize);

  d0jesStyle->SetTitleFont(d0jesFontBold);
  d0jesStyle->SetTitleSize(0.06,"xyz");
  //d0jesStyle->SetTitleOffset(0.9,"xyz");
  d0jesStyle->SetTitleOffset(1.0,"x");
  d0jesStyle->SetTitleOffset(1.0,"y");//1.3,"y");
  d0jesStyle->SetTitleOffset(0.9,"z");

  //------------------------

  // Line and marker styles.
  d0jesStyle->SetLineWidth(d0jesFrameLineWidth);
  d0jesStyle->SetFrameLineWidth(d0jesFrameLineWidth);
//d0jesStyle->SetHistLineWidth(d0jesLineWidth);
  d0jesStyle->SetFuncWidth(d0jesLineWidth);
  d0jesStyle->SetGridWidth(d0jesFrameLineWidth);
  d0jesStyle->SetLineStyleString(2,"[24 24]");
  d0jesStyle->SetLineStyleString(3,"[6 18]");
  d0jesStyle->SetLineStyleString(4,"[24 24 6 24]");
//d0jesStyle->SetMarkerStyle(kPlus);
//d0jesStyle->SetMarkerSize(1.5);
  d0jesStyle->SetMarkerSize(1.2);

  //------------------------

  // Label settings.
  //d0jesStyle->SetLabelFont(d0jesFont,"xyz");
  d0jesStyle->SetLabelFont(d0jesFontBold,"xyz");
  d0jesStyle->SetLabelOffset(0.015);
  d0jesStyle->SetLabelSize(0.045,"xyz");

  //------------------------

  // Histogram settings.

  // Histogram title not shown by default.
  d0jesStyle->SetOptTitle(0);

  // Statistics box shown by default.
  d0jesStyle->SetOptStat(1);  

  // Fit statistics box not shown by default.
  d0jesStyle->SetOptFit(0);

  //------------------------

  // Settings for the statistics box.
  d0jesStyle->SetStatBorderSize(1);
  d0jesStyle->SetStatFont(d0jesFont);
  d0jesStyle->SetStatFontSize(0.05);
  d0jesStyle->SetStatX(1.0-d0jesStyle->GetPadRightMargin());
  d0jesStyle->SetStatY(1.0-d0jesStyle->GetPadTopMargin());
  d0jesStyle->SetStatW(0.20);//0.25);
  d0jesStyle->SetStatH(0.15);

  //------------------------

  // Settings for the legend.
  //  d0jesStyle->SetLegendBorderSize(1);

  //------------------------

  // Where to put tick marks
  d0jesStyle->SetPadTickX(1);
  d0jesStyle->SetPadTickY(1);
  // and how often.
  d0jesStyle->SetNdivisions(510,"x");
  d0jesStyle->SetNdivisions(510,"y");

  // Fix the number of decimal places on axes.
  d0jesStyle->SetStripDecimals(kFALSE);

  //------------------------

  // Some (hopefully) useful thingies.

//   TPaveText* d0jesName = new TPaveText(0.65,0.8,0.9,0.9,"BRNDC");
//   d0jesName->SetFillColor(0);
//   d0jesName->SetTextAlign(d0jesTextAlign);
//   d0jesName->SetBorderSize(0);
//   d0jesName->AddText("D0 JES");

//   TText* d0jesLabel = new TText();
//   d0jesLabel->SetTextFont(d0jesFont);
//   d0jesLabel->SetTextColor(1);
//   d0jesLabel->SetTextSize(d0jesTextSizeSmall);
//   d0jesLabel->SetTextAlign(d0jesTextAlign);

//   TLatex* d0jesLatex = new TLatex();
//   d0jesLatex->SetTextFont(d0jesFont);
//   d0jesLatex->SetTextColor(1);
//   d0jesLatex->SetTextSize(d0jesTextSizeSmall);
//   d0jesLatex->SetTextAlign(d0jesTextAlign);

  float shiftX = 1.1*d0jesStyle->GetTickLength("X");
  float shiftY = 1.1*d0jesStyle->GetTickLength("Y");
  TLegend d0jesLegend(.65,.65,.9-shiftX,.9-shiftY,"D0 JES Preliminary");
  d0jesLegend.SetName("d0jesLegend");
  gROOT->Append(&d0jesLegend);
  d0jesLegend.SetTextFont(d0jesFont);
  d0jesLegend.SetTextColor(1);
  d0jesLegend.SetTextAlign(d0jesTextAlign);
  d0jesLegend.SetBorderSize(0);
  d0jesLegend.SetFillColor(19);

  //------------------------

  cout << "=============================================" << endl;
  cout << "  Style loaded" << endl;
  cout << "=============================================" << endl;
}
