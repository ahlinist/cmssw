/*!
  \file HcalRenderPlugin.cc
  \brief Display Plugin for Hcal DQM Histograms
  \author J. Temple
  \version $Revision: 1.24 $
  \date $Date: 2009/10/18 15:44:28 $
  \\
  \\ Code shamelessly borrowed from S. Dutta's SiStripRenderPlugin.cc code,
  \\ G. Della Ricca and B. Gobbo's EBRenderPlugin.cc, and other existing
  \\ subdetector plugins
  \\ preDraw and postDraw methods now check whether histogram was a TH1
  \\ or TH2, and call a private method appropriate for the histogram type
*/

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TColor.h"
#include "TText.h"
#include "TLine.h"
#include "TPaletteAxis.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>

#define HCAL_ETAMIN -44.5
#define HCAL_ETAMAX 44.5
#define HCAL_PHIMIN -0.5
#define HCAL_PHIMAX 73.5

//---define parameters for HTR/Channel Plots
#define MARGIN  1
#define FEDS2_X0 3
#define FEDS3_X0 4
#define SPIG2_Y0 3
#define SPIG3_Y0 4
#define CHNS2_X0 3
#define XS 2
#define YS 4
////#define _XBINS (24*ERIC_HDP_XS+ERIC_HDP_X0)
////#define _YBINS (15*ERIC_HDP_YS+ERIC_HDP_Y0)

using std::cout;
using std::endl;

class HcalRenderPlugin : public DQMRenderPlugin
{
  // Color schemes
  int summaryColors[40];
  int errorFracColors[20];
  int standardColors[20];
  int hcalErrorColors[100];
  int hcalRainbowColors[100];
  Int_t NRGBs_rainbow;
  Int_t NCont_rainbow;
  Int_t NRGBs_hcalError;
  Int_t NCont_hcalError;
  Int_t defNCont_;  // default number of contours

public:
  virtual void initialise (int, char **)
  {
    // Error fraction colors, and standard colors
    // (error fractions colors are the reverse of standard colors)
    //  should probably make error colors run from yellow to red, don't include green?

    float rgb[20][3];

    for( int i=0; i<20; ++i )
      {
        if (i<2)
          summaryColors[i]=17;
        else summaryColors[i]=0;

        if ( i < 17 )
	  {
	    rgb[i][0] = 0.80+0.01*i;
	    rgb[i][1] = 0.00+0.03*i;
	    rgb[i][2] = 0.00;
	  }
        else if ( i < 19 )
	  {
	    rgb[i][0] = 0.80+0.01*i;
	    rgb[i][1] = 0.00+0.03*i+0.15+0.10*(i-17);
	    rgb[i][2] = 0.00;
	  }
        else if ( i == 19 )
	  {
	    rgb[i][0] = 0.00;
	    rgb[i][1] = 0.80;
	    rgb[i][2] = 0.00;
	  }
        // flip colors from standard values (1=bad, 0=good)
        errorFracColors[19-i] = 901+i;
        standardColors[i]=901+i;
        summaryColors[20+i]=901+i;
        TColor* color = gROOT->GetColor( 901+i );
        if( ! color ) color = new TColor( 901+i, 0, 0, 0, "" );
        color->SetRGB( rgb[i][0], rgb[i][1], rgb[i][2] );
      }

    // Make rainbow colors.  Assign colors positions 1101-1200;
    NRGBs_rainbow = 5; // specify number of RGB boundaries for rainbow
    NCont_rainbow = 100; // specify number of contours for rainbow
    Double_t stops_rainbow[] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red_rainbow[]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green_rainbow[] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue_rainbow[]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    Int_t nColorsGradient=0;

    int highestIndex=0;
    for (int g=1;g<NRGBs_rainbow;++g)
      {
        nColorsGradient = (Int_t) (floor(NCont_rainbow*stops_rainbow[g]) - floor(NCont_rainbow*stops_rainbow[g-1])); // specify number of gradients between stops (g-1) and (g)
        for (int c = 0; c < nColorsGradient; c++)
	  {
	    hcalRainbowColors[highestIndex]=1101+highestIndex;
	    TColor* color = gROOT->GetColor(1101+highestIndex);
	    // Make new color only if old color does not exist
	    if (!color)
	      color = new TColor(1101+highestIndex,
				 red_rainbow[g-1] + c * (red_rainbow[g] - red_rainbow[g-1])/ nColorsGradient,
				 green_rainbow[g-1] + c * (green_rainbow[g] - green_rainbow[g-1])/ nColorsGradient,
				 blue_rainbow[g-1] + c * (blue_rainbow[g] - blue_rainbow[g-1])/ nColorsGradient,
				 "  ");
	    highestIndex++;
	  }
      }

    // repeat for hcal error colors.  Assign color positions 1201-1300;
    NRGBs_hcalError = 6; // specify number of RGB boundaries for hcalError
    NCont_hcalError = 100; // specify number of contours for hcalError
    Double_t stops_hcalError[] = { 0.00, 0.05, 0.40, 0.75, 0.95, 1.00};
    Double_t red_hcalError[]   = { 0.00, 1.00, 1.00, 1.00, 1.00, 0.60};
    Double_t green_hcalError[] = { 1.00, 1.00, 0.67, 0.33, 0.00, 0.00};
    Double_t blue_hcalError[]  = { 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
    nColorsGradient=0;
    highestIndex=0;
    for (int g=1;g<NRGBs_hcalError;++g)
      {
        nColorsGradient = (Int_t) (floor(NCont_hcalError*stops_hcalError[g]) - floor(NCont_hcalError*stops_hcalError[g-1])); // specify number of gradients between stops (g-1) and (g)
        for (int c = 0; c < nColorsGradient; c++)
	  {
	    hcalErrorColors[highestIndex]=1201+highestIndex;
	    TColor* color = gROOT->GetColor(1201+highestIndex);
	    // Make new color only if old color does not exist
	    if (!color)
	      color = new TColor(1201+highestIndex,
				 red_hcalError[g-1] + c * (red_hcalError[g] - red_hcalError[g-1])/ nColorsGradient,
				 green_hcalError[g-1] + c * (green_hcalError[g] - green_hcalError[g-1])/ nColorsGradient,
				 blue_hcalError[g-1] + c * (blue_hcalError[g] - blue_hcalError[g-1])/ nColorsGradient,
				 "  ");

	    highestIndex++;
	  }
      }
  }

  virtual bool applies(const DQMNet::CoreObject &o, const VisDQMImgInfo &)
  {
    // determine whether core object is an Hcal object
    if (o.name.find( "Hcal/" ) != std::string::npos || o.name.find( "HcalCalib/" ) != std::string::npos)
      return true;

    return false;
  }

  virtual void preDraw (TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo &)
  {
    c->cd();

    gStyle->Reset("Default");
    defNCont_=gStyle->GetNumberContours(); // test this later

    gStyle->SetCanvasColor(10);
    gStyle->SetPadColor(10);
    gStyle->SetFillColor(10);
    gStyle->SetStatColor(10);
    gStyle->SetTitleFillColor(10);

    TGaxis::SetMaxDigits(4);

    gStyle->SetOptTitle(kTRUE);
    gStyle->SetTitleBorderSize(0);

    gStyle->SetOptStat(kFALSE);
    gStyle->SetStatBorderSize(1);

    gROOT->ForceStyle();

    // object is TH2 histogram
    if( dynamic_cast<TH2*>( o.object ) )
      {
        preDrawTH2( c, o );
      }
    // object is TH1 histogram
    else if( dynamic_cast<TH1*>( o.object ) )
      {
        preDrawTH1( c, o );
      }
  }

  virtual void postDraw (TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &)
  {
    // object is TH2 histogram
    if( dynamic_cast<TH2*>( o.object ) )
      {
        postDrawTH2( c, o );
      }
    // object is TH1 histogram
    else if( dynamic_cast<TH1*>( o.object ) )
      {
        postDrawTH1( c, o );
      }

    // reset number of contours -- to be tested
    gStyle->SetNumberContours(defNCont_);
    gStyle->UseCurrentStyle();
  }

private:
  void preDrawTH1 ( TCanvas *, const DQMNet::CoreObject &o )
  {
    // Do we want to do anything special yet with TH1 histograms?
    TH1* obj = dynamic_cast<TH1*>( o.object );
    assert (obj); // checks that object indeed exists

    // o.name is a std::string object
    // Add in list of names of histograms for which we want log plotting here.
    if (  (o.name.find("DataFormatMonitor/Corruption/03 OrN Difference")               !=std::string::npos) ||
	  (o.name.find("DataFormatMonitor/Corruption/04 HTR BCN when")                 !=std::string::npos) ||
	  (o.name.find("DataFormatMonitor/Corruption/05 BCN Difference")               !=std::string::npos) ||
	  (o.name.find("DataFormatMonitor/Corruption/06 EvN Difference")               !=std::string::npos) ||
	  (o.name.find("DataFormatMonitor/Data Flow/BCN from")                         !=std::string::npos) ||
	  (o.name.find("DataFormatMonitor/Data Flow/DCC Data Block Size Distribution") !=std::string::npos) ||
	  (o.name.find("DataFormatMonitor/Data Flow/DCC Event Counts")                 !=std::string::npos) ||
	  (o.name.find("DataFormatMonitor/Diagnostics/HTR Fiber Orbit")                !=std::string::npos) ||
	  (o.name.find("DataFormatMonitor/Diagnostics/HTR Status Word H")              !=std::string::npos) ||
	  (o.name.find("DataFormatMonitor/Diagnostics/HTR UnSupp")                     !=std::string::npos) ||
	  (o.name.find("RecHitMonitor_Hcal/rechit_1D_plots")                           !=std::string::npos) 
	  ) 
      if (obj->GetMaximum()>0) gPad->SetLogy(1);

    if (  (o.name.find("DataFormatMonitor/Data Flow/DCC Data Block Size Distribution")!=std::string::npos) ||
	  (o.name.find("DataFormatMonitor/Diagnostics/HTR Fiber Orbit")!=std::string::npos)  
	  ) 
      if (obj->GetMaximum()>0) 
	gPad->SetLogx(1);
    
    if (o.name.find("Problem_Total_DeadCells_") != std::string::npos ||
	o.name.find("rechit_1D_plots")!= std::string::npos)
      {
        gStyle->SetOptStat("iourmen");
        obj->SetStats( kTRUE );
      }

    if (  (o.name.find("Digi Shape - over thresh")!=std::string::npos)  )
      obj->SetMinimum(0);

    if (  ((o.name.find("DigiMonitor_Hcal/digi_info/") !=std::string::npos) &&
	   (o.name.find(" CapID") != std::string::npos)) ||
	  ((o.name.find("DigiMonitor_Hcal/digi_info/") !=std::string::npos) &&
	   (o.name.find(" Digi Shape") != std::string::npos))
	  )
      {
        obj->SetMinimum(0.);
      }

    if (o.name.find(" Capid 1st Time Slice") !=std::string::npos)
      if (obj->GetMaximum()>0) gPad->SetLogy(1);

    if (o.name.find("DigiMonitor_Hcal/digi_info/# of Digis") !=std::string::npos)
      {
        if (obj->GetMaximum()>0) gPad->SetLogy(1);
        gStyle->SetOptStat("iourmen");
        obj->SetStats(kTRUE);
      }
  }

  void preDrawTH2 ( TCanvas *c, const DQMNet::CoreObject &o )
  {
    TH2* obj = dynamic_cast<TH2*>( o.object );
    assert( obj );

    gStyle->SetCanvasBorderMode( 0 );
    gStyle->SetPadBorderMode( 0 );
    gStyle->SetPadBorderSize( 0 );

    // Do we want to set stats to 0 for Hcal?
    gStyle->SetOptStat( 0 );
    obj->SetStats( kFALSE );

    // Default coloring scheme
    setRainbowColor(); // sets to rainbow color with finer gradations than setPalette(1)
    obj->SetOption("colz");

    // Set default color scheme

    // Always use most up-to-date color scheme for reportSummaryMap,
    // so that it's consistent with other maps.
    // Set reportSummary Map color with dqm::utils::reportSummaryMapPalette(obj);

    // I don't think we want to do this yet, because reportSummaryMap doesn't
    // yet provide useful colors when value = -1?
    // Instead, use the summaryColors defined within this code
    // green when =1, red when = 0, grey when = -1

    if (o.name.find("reportSummaryMap" ) != std::string::npos)
      {
        gStyle->SetNumberContours(40); // should maintain reportSummaryMap color scheme, but allow for additional colors (from -1->0)
        gStyle->SetPalette(40,summaryColors);
	gStyle->SetPaintTextFormat("5.4g"); // set to %5.4f  text format in cells
	obj->SetMarkerSize(3); // set font size to 3x normal
        obj->SetOption("text90colz"); // draw marker at 90 degrees
	// Set min, max values to -1, 1
	obj->SetMinimum(-1.);
	obj->SetMaximum(1.);
      }
    else if (o.name.find("advancedReportSummaryMap" ) != std::string::npos)
      {
        gStyle->SetNumberContours(40);
        gStyle->SetPalette(40,summaryColors);
        obj->SetOption("colz");
	obj->SetMinimum(-1.);
	obj->SetMaximum(1.);
      }

    // Overall pedestal plots should always be colored yellow-red (no green allowed; ZS doesn't let us normalize to Nevents properly (yet) )
    else if (   (o.name.find("BaselineMonitor_Hcal/ ProblemPedestals")!=std::string::npos) ||
		(o.name.find("BaselineMonitor_Hcal/problem_pedestals/")!=std::string::npos) 
		)
      {
	double scale = obj->GetBinContent(0,0);
	obj->Scale(1./scale);
	obj->SetMinimum(0.);
	obj->SetMaximum(1.); 
	// Make special color scheme for pedestals at some point?
      }
      
      
    // Overall problem hot cells are plotted with error Fraction colors (0 = green, 1 = red)
    else if ( (o.name.find("RecHitMonitor_Hcal/ ProblemRecHits")!= std::string::npos ) ||
	      (o.name.find("RecHitMonitor_Hcal/problem_rechits/")!= std::string::npos ) ||
	      (o.name.find("DigiMonitor_Hcal/ ProblemDigis")!= std::string::npos ) ||
	      (o.name.find("DigiMonitor_Hcal/problem_digis/")!= std::string::npos ) ||
	      (o.name.find("HotCellMonitor_Hcal/ ProblemHotCells")!= std::string::npos ) ||
	      (o.name.find("HotCellMonitor_Hcal/problem_hotcells/") != std::string::npos) ||
	      (o.name.find("DeadCellMonitor_Hcal/ ProblemDeadCells")!= std::string::npos ) ||
	      (o.name.find("DeadCellMonitor_Hcal/problem_deadcells/")!= std::string::npos )
	      )
		
      {
	c->SetFrameFillColor(18);
	gStyle->SetFrameFillColor(18);
        double scale = obj->GetBinContent(0,0);
        if (scale>0) // problem histograms don't have underflow bins filled any more
	  {
	    obj->Scale(1./scale);
	    obj->SetMinimum(0.);
	    obj->SetMaximum(1.);
	    //Set error palette just for normalized histograms?
	    //gStyle->SetPalette(20, errorFracColors);
	    setErrorColor();
	  }
	else 
	  {
	    obj->SetMinimum(0.);
	    obj->SetMaximum(1.);
	    setErrorColor();
	  }
        obj->SetOption("colz");
      }

    // Do we want these plots to show error fractions (0-1) as well?
    // I think it's useful to have raw number of events plotted here; don't renormalize (yet)

    else if (// Hot Cell subdirectories
	     (o.name.find("HotCellMonitor_Hcal/hot_pedestaltest/") != std::string::npos) ||
	     (o.name.find("HotCellMonitor_Hcal/hot_rechit_above_threshold/") != std::string::npos) ||
	     (o.name.find("HotCellMonitor_Hcal/hot_rechit_always_above_threshold/") != std::string::npos) ||
	     (o.name.find("HotCellMonitor_Hcal/hot_neighbortest/") != std::string::npos)  ||
	     // Dead Cell subdirectories
	     (o.name.find("DeadCellMonitor_Hcal/dead_digi_often_missing/")!= std::string::npos ) ||
	     (o.name.find("DeadCellMonitor_Hcal/dead_digi_never_present/")!= std::string::npos ) ||
	     (o.name.find("DeadCellMonitor_Hcal/dead_energytest/")!= std::string::npos )
	     )
      {
	gPad->SetGridx();
	gPad->SetGridy();
        double scale = obj->GetBinContent(0,0);
        if (scale>0)
	  {
	    obj->SetMaximum(obj->GetBinContent(0,0));
	    obj->SetMinimum(0.);
	    if (o.name.find("dead_digi_never_present/") != std::string::npos && scale > 1000) 
	      {
		gStyle->SetOptStat(0);
		obj->SetStats(0);
	      }
	    else if (scale>10000)
	      {
		gStyle->SetOptStat(0);
		obj->SetStats(0);
	      }
	  }
        setErrorColor();
        //gStyle->SetPalette(20, errorFracColors);
        obj->SetOption("colz");
      }

    else if (
	     (
	      (o.name.find("BaselineMonitor_Hcal/") != std::string::npos) &&
	      (o.name.find("ADC") != std::string::npos)
	      )
	     &&
	     (o.name.find("Subtracted") == std::string::npos)
	     )
      {
        // ADC pedestals should be centered at 3; set maximum to 2*3=6
        obj->SetMinimum(0.);
        if ( (obj->GetMaximum()<6.) &&
	     ((o.name.find("Pedestal Mean Map") != std::string::npos) ||
	      (o.name.find("Pedestal Values Map") != std::string::npos))
	     )
	  obj->SetMaximum(6.);

        else if ( (obj->GetMaximum()<2.) &&
		  ((o.name.find("Pedestal RMS Map") != std::string::npos) ||
		   (o.name.find("Pedestal Widths Map") != std::string::npos))
		  )
	  obj->SetMaximum(2.);

        setRainbowColor(); // sets to rainbow color with finer gradations than setPalette(1)
        obj->SetOption("colz");
      }

    else if ((o.name.find("HTR Plots/ Channel Data Integrity/FED 7") != std::string::npos )            ||
	     (o.name.find("HTR Plots/Channel Integrity Summarized by Spigot") != std::string::npos )   ||
	     (o.name.find("HTR Plots/Half-HTR DataIntegrity Check") != std::string::npos )               )
      {
        gStyle->SetPalette(1);
        obj->SetOption("colz");
        c->SetBottomMargin(0.200);
        if (obj->GetMaximum()>0) gPad->SetLogz();
      }


    else   // default color is rainbow
      {
        gStyle->SetPalette(1);
        //gStyle->SetPalette(20, errorFracColors);
        obj->SetOption("colz");

      }
  }

  void postDrawTH1( TCanvas *, const DQMNet::CoreObject &o )
  {
    TH1* obj = dynamic_cast<TH1*>( o.object ); 
    assert( obj ); 
    
    // Dead Cell check -- warn that plots need many events before they update
    if (o.name.find("DeadCellMonitor_Hcal/" )!=std::string::npos)
      {
	if ( (o.name.find("DeadCellMonitor_Hcal/dead_digi_often_missing")!=std::string::npos) ||
	     (o.name.find("DeadCellMonitor_Hcal/dead_energytest")!=std::string::npos) ||
	     (o.name.find("DeadCellMonitor_Hcal/problem_deadcells") != std::string::npos) ||
	     (o.name.find("DeadCellMonitor_Hcal/Problem_") != std::string::npos) 
	     )
	  {
	    if (obj->GetBinContent(0)<10000)
	      {
		TText t;
		t.DrawTextNDC(0.05,0.01,"Histogram updates every 10,000 events");
		gStyle->SetOptStat(1111);
		obj->SetStats(1111); 
	      }
	    else
	      {
		gStyle->SetOptStat(0);
		obj->SetStats(0);
	      }
	  } 
	else if ( (o.name.find("Problem_NeverPresentCells_") != std::string::npos) 		
		  ) 
	  {
	    if (obj->GetBinContent(0)<1000)
	      {
		TText t;
		t.DrawTextNDC(0.05,0.01,"Histogram updates every 1,000 events");
		
	      }
	    else
	      {
		gStyle->SetOptStat(0);
		obj->SetStats(0);
		gPad->SetGridx();
		gPad->SetGridy(); 
	      }
	  }
      } // if (o.name.find("DeadCellMonitor_Hcal/"...))

    if ( (o.name.find("DataFormatMonitor/Corruption")      != std::string::npos) &&
	 (obj->GetEntries() == 0) )
      {
        TText t;
        t.SetTextSize( 0.1);
	if (o.name.find("BCN when OrN") !=std::string::npos)
	  t.DrawText(0, 0, "Empty == OK"); 
	else 
	  t.DrawText(-1*(obj->GetNbinsX() * 0.5), 0, "Empty == OK"); 

      }

  }

  void postDrawTH2( TCanvas *c, const DQMNet::CoreObject &o )
  {
    TH2* obj = dynamic_cast<TH2*>( o.object );
    assert( obj );

    // Move histogram to accomodate colz column
    TPaletteAxis *pal = (TPaletteAxis*)obj->GetListOfFunctions()->FindObject("palette");
    if (pal!=0)
      {
	c->SetRightMargin(0.15);
	pal->SetX1NDC(0.85);
	pal->SetX2NDC(0.90);
      }
      

    // Want to move colz palette, but this crashes code, and does not move the palette.  Hmm...
    obj->GetYaxis()->SetTickLength(0.0);
    obj->GetXaxis()->SetTickLength(0.0);

    if (o.name.find("DataFormatMonitor/Data Flow/DCC Data Block Size Each FED")!=std::string::npos)
      c->SetLogy();

    // in the future, we can add text output based on error status,
    // or set bin range based on filled histograms, etc.
    if ( (o.name.find("DataFormatMonitor/Corruption")      != std::string::npos) ||
	 (o.name.find("DataFormatMonitor/Corruption/F")      != std::string::npos) )
      {
        TText t;
        t.SetTextSize( 0.1);
        if (obj->GetEntries() == 0)
	  t.DrawText(1, 1, "Empty == OK"); 
      }

    if ( (o.name.find("Corruption/Chan") != std::string::npos )         )
      {
        c->SetBottomMargin(0.200);
        TLine line;
        line.SetLineWidth(1);
        for (int i=0; i<24; i++)
	  {   // x-axis:24 channels
	    for (int j=0; j<15; j++)
	      { // y-axis:15 spigots
		line.DrawLine(MARGIN+(i*CHNS2_X0), MARGIN+(j*SPIG2_Y0),
			      ((i+1) *  CHNS2_X0), MARGIN+(j*SPIG2_Y0)    );
		line.DrawLine(MARGIN+(i*CHNS2_X0), MARGIN+(j*SPIG2_Y0)+2,
			      ((i+1) *  CHNS2_X0), MARGIN+(j*SPIG2_Y0)+2  );

		line.DrawLine(MARGIN+(i*CHNS2_X0)  , MARGIN+(j*SPIG2_Y0),
			      MARGIN+(i*CHNS2_X0)  , ((j+1) *  SPIG2_Y0)    );
		line.DrawLine(MARGIN+(i*CHNS2_X0)+2, MARGIN+(j*SPIG2_Y0),
			      MARGIN+(i*CHNS2_X0)+2, ((j+1) *  SPIG2_Y0)    );
	      }
	  }
        // Draw a legend above the plot
        line.DrawLine((CHNS2_X0*20)  , (SPIG2_Y0*16)  ,
                      (CHNS2_X0*21)-1, (SPIG2_Y0*16)  );
        line.DrawLine((CHNS2_X0*20)  , (SPIG2_Y0*17)-1,
                      (CHNS2_X0*21)-1, (SPIG2_Y0*17)-1);

        line.DrawLine((CHNS2_X0*20)  , (SPIG2_Y0*16)  ,
                      (CHNS2_X0*20)  , (SPIG2_Y0*17)-1);
        line.DrawLine((CHNS2_X0*21)-1, (SPIG2_Y0*16)  ,
                      (CHNS2_X0*21)-1, (SPIG2_Y0*17)-1);
        TText tx;
        tx.SetTextSize( 0.02);
        tx.DrawText((CHNS2_X0*20)-6, (SPIG2_Y0*16)     , "DigiSize");
        tx.DrawText((CHNS2_X0*20)-4, (SPIG2_Y0*17)-1.75, "!DV"     );

        tx.DrawText((CHNS2_X0*21)  , (SPIG2_Y0*16)    , "CapRotat" );
        tx.DrawText((CHNS2_X0*21)  , (SPIG2_Y0*17)-1.75,"Er"      );

	obj->SetMaximum(1);
        return;
      }
    else if ( (o.name.find("Data Flow/01") != std::string::npos )         )
      {
	obj->SetMaximum(1);
        c->SetBottomMargin(0.200);
        TLine line;
        line.SetLineWidth(1);
        for (int i=0; i<32; i++)
	  {   // x-axis:32 DCCs
	    for (int j=0; j<15; j++)
	      { // y-axis:15 spigots
		line.DrawLine(MARGIN+(i*FEDS2_X0), MARGIN+(j*SPIG3_Y0),
			      ((i+1) *  FEDS2_X0), MARGIN+(j*SPIG3_Y0)    );
		line.DrawLine(MARGIN+(i*FEDS2_X0), MARGIN+(j*SPIG3_Y0)+3,
			      ((i+1) *  FEDS2_X0), MARGIN+(j*SPIG3_Y0)+3  );

		line.DrawLine(MARGIN+(i*FEDS2_X0)  , MARGIN+(j*SPIG3_Y0),
			      MARGIN+(i*FEDS2_X0)  , ((j+1) *  SPIG3_Y0)    );
		line.DrawLine(MARGIN+(i*FEDS2_X0)+2, MARGIN+(j*SPIG3_Y0),
			      MARGIN+(i*FEDS2_X0)+2, ((j+1) *  SPIG3_Y0)    );
	      }
	  }
        // Draw a legend above the plot
        line.DrawLine((FEDS2_X0*28)-2, (SPIG3_Y0*16)-1,
                      (FEDS2_X0*31)-1, (SPIG3_Y0*16)-1);
        line.DrawLine((FEDS2_X0*28)-2, (SPIG3_Y0*17)  ,
                      (FEDS2_X0*31)-1, (SPIG3_Y0*17)  );

        line.DrawLine((FEDS2_X0*28)-2, (SPIG3_Y0*16)-1,
                      (FEDS2_X0*28)-2, (SPIG3_Y0*17)  );
        line.DrawLine((FEDS2_X0*31)-1, (SPIG3_Y0*16)-1,
                      (FEDS2_X0*31)-1, (SPIG3_Y0*17)  );
        TText tx;
        tx.SetTextSize( 0.02);
        tx.DrawText((FEDS2_X0*29)-7, (SPIG3_Y0*16)-2.0, "CE");

        tx.DrawText((FEDS2_X0*29)-4, (SPIG3_Y0*16)+2.5, "OW");
	tx.DrawText((FEDS2_X0*29)-4, (SPIG3_Y0*16)+1  , "BZ");
        tx.DrawText((FEDS2_X0*29)-4, (SPIG3_Y0*16)-0.5, "EE");

        tx.DrawText((FEDS2_X0*29)+0, (SPIG3_Y0*16)+2.5, "OFW");
	tx.DrawText((FEDS2_X0*29)+0, (SPIG3_Y0*16)+1  , "BSY");
        tx.DrawText((FEDS2_X0*29)+0, (SPIG3_Y0*16)-0.5, "L1A");
        return;
      }
    else if ( (o.name.find("Corruption/09") != std::string::npos )         )
      {
	obj->SetMaximum(1);
        c->SetBottomMargin(0.200);
        TLine line;
        line.SetLineWidth(1);
        for (int i=0; i<32; i++)
	  {   // x-axis:32 DCCs
	    for (int j=0; j<15; j++)
	      { // y-axis:15 spigots
		line.DrawLine(MARGIN+(i*FEDS2_X0), MARGIN+(j*SPIG2_Y0),
			      ((i+1) *  FEDS2_X0), MARGIN+(j*SPIG2_Y0)    );
		line.DrawLine(MARGIN+(i*FEDS2_X0), MARGIN+(j*SPIG2_Y0)+2,
			      ((i+1) *  FEDS2_X0), MARGIN+(j*SPIG2_Y0)+2  );

		line.DrawLine(MARGIN+(i*FEDS2_X0)  , MARGIN+(j*SPIG2_Y0),
			      MARGIN+(i*FEDS2_X0)  , ((j+1) *  SPIG2_Y0)    );
		line.DrawLine(MARGIN+(i*FEDS2_X0)+2, MARGIN+(j*SPIG2_Y0),
			      MARGIN+(i*FEDS2_X0)+2, ((j+1) *  SPIG2_Y0)    );
	      }
	  }
        // Draw a legend above the plot
        line.DrawLine((FEDS2_X0*30)  , (SPIG2_Y0*16)  ,
                      (FEDS2_X0*31)-1, (SPIG2_Y0*16)  );
        line.DrawLine((FEDS2_X0*30)  , (SPIG2_Y0*17)-1,
                      (FEDS2_X0*31)-1, (SPIG2_Y0*17)-1);

        line.DrawLine((FEDS2_X0*30)  , (SPIG2_Y0*16)  ,
                      (FEDS2_X0*30)  , (SPIG2_Y0*17)-1);
        line.DrawLine((FEDS2_X0*31)-1, (SPIG2_Y0*16)  ,
                      (FEDS2_X0*31)-1, (SPIG2_Y0*17)-1);
        TText tx;
        tx.SetTextSize( 0.02);
        tx.DrawText((FEDS2_X0*30)-6, (SPIG2_Y0*16)     , "DigiSize");
        tx.DrawText((FEDS2_X0*30)-4, (SPIG2_Y0*17)-1.75, "!DV"     );

        tx.DrawText((FEDS2_X0*31)  , (SPIG2_Y0*16)    , "CapRotat" );
        tx.DrawText((FEDS2_X0*31)  , (SPIG2_Y0*17)-1.75,"Er"      );
        return;
      }

    if ( (o.name.find("DataFormatMonitor/Corruption/01") != std::string::npos) ||
	 (o.name.find("DataFormatMonitor/Corruption/02") != std::string::npos) ||
	 (o.name.find("DataFormatMonitor/Diagnostics/DCC Stat") != std::string::npos)    )
      {
        obj->SetStats(0);
        c->SetLeftMargin( 0.350); // in fractions of a TCanvas... ?
      }
    else  if ( (o.name.find("DataFormatMonitor/Diagnostics/HTR Status Word") != std::string::npos)    )
      {
        obj->SetStats(0);
        c->SetLeftMargin( 0.250); // in fractions of a TCanvas... ?
      }
    else if ( (o.name.find("Corruption/07") != std::string::npos ) ||
	      (o.name.find("Corruption/08") != std::string::npos )   )
      {
	obj->SetMaximum(1);
        TLine line;
        line.SetLineWidth(1);
        for (int i=0; i<32; i++)
	  {    // x-axis:32 DCC's (FEDs 700:731)
	    for (int j=0; j<15; j++)
	      {  // y-axis:15 spigots
		line.DrawLine(MARGIN+(i*FEDS3_X0), MARGIN+(j*SPIG3_Y0),
			      ((i+1) *  FEDS3_X0), MARGIN+(j*SPIG3_Y0)    );
		line.DrawLine(MARGIN+(i*FEDS3_X0), MARGIN+(j*SPIG3_Y0)+3,
			      ((i+1) *  FEDS3_X0), MARGIN+(j*SPIG3_Y0)+3    );

		line.DrawLine(MARGIN+(i*FEDS3_X0)  , MARGIN+(j*SPIG3_Y0),
			      MARGIN+(i*FEDS3_X0)  , ((j+1) *  SPIG3_Y0)    );
		line.DrawLine(MARGIN+(i*FEDS3_X0)+3, MARGIN+(j*SPIG3_Y0),
			      MARGIN+(i*FEDS3_X0)+3, ((j+1) *  SPIG3_Y0)    );
	      }}
        // Draw a legend above the plot
        line.DrawLine(FEDS3_X0*28.5, (SPIG3_Y0*16)-1,
                      FEDS3_X0*32  , (SPIG3_Y0*16)-1);
        line.DrawLine(FEDS3_X0*28.5, (SPIG3_Y0*17)+0,
                      FEDS3_X0*32  , (SPIG3_Y0*17)+0);
						 
        line.DrawLine(FEDS3_X0*28.5, (SPIG3_Y0*16)-1,
                      FEDS3_X0*28.5, (SPIG3_Y0*17)+0);
        line.DrawLine(FEDS3_X0*32  , (SPIG3_Y0*16)-1,
                      FEDS3_X0*32  , (SPIG3_Y0*17)+0);
        TText tx;
        tx.SetTextSize( 0.02);
        if (o.name.find("LRB") != std::string::npos )
	  {
	    tx.DrawText((FEDS3_X0*30)-8, (SPIG3_Y0*17)-6  , "T");

	    tx.DrawText((FEDS3_X0*30)-5, (SPIG3_Y0*17)-2  , "E!P");
	    tx.DrawText((FEDS3_X0*30)-5, (SPIG3_Y0*17)-3.5, "ND" );
	    tx.DrawText((FEDS3_X0*30)-5, (SPIG3_Y0*17)-5  , "CRC");

	    tx.DrawText((FEDS3_X0*30)+0, (SPIG3_Y0*17)-2  , "UE");
	    tx.DrawText((FEDS3_X0*30)+0, (SPIG3_Y0*17)-3.5, "OV");
	    tx.DrawText((FEDS3_X0*30)+0, (SPIG3_Y0*17)-5  , "ST");

	    tx.DrawText((FEDS3_X0*30)+4, (SPIG3_Y0*17)-2  , "TR");
	    tx.DrawText((FEDS3_X0*30)+4, (SPIG3_Y0*17)-3.5, "ID");
	    tx.DrawText((FEDS3_X0*30)+4, (SPIG3_Y0*17)-5  , "ODD");}
        if (o.name.find("Half-HTR") != std::string::npos )
	  {
	    tx.DrawText((FEDS3_X0*30)-5, (SPIG3_Y0*17)-2  , "CT");
	    tx.DrawText((FEDS3_X0*30)-5, (SPIG3_Y0*17)-3.5, "HM");
	    tx.DrawText((FEDS3_X0*30)-5, (SPIG3_Y0*17)-5  , "TM");

	    tx.DrawText((FEDS3_X0*30)+0, (SPIG3_Y0*17)-2  , "BE");
	    tx.DrawText((FEDS3_X0*30)+0, (SPIG3_Y0*17)-3.5, "15");
	    tx.DrawText((FEDS3_X0*30)+0, (SPIG3_Y0*17)-5  , "CK");

	    tx.DrawText((FEDS3_X0*30)+4 , (SPIG3_Y0*17)-2  , "LW");
	    tx.DrawText((FEDS3_X0*30)+4 , (SPIG3_Y0*17)-3.5, "WW");
	    tx.DrawText((FEDS3_X0*30)+4 , (SPIG3_Y0*17)-5  , "IW");}
        return;
      }

    else if ( (o.name.find("DigiMonitor_Hcal/ ProblemDigis")!= std::string::npos ) ||
	      (o.name.find("DigiMonitor_Hcal/problem_digis")!= std::string::npos ) ||
	      (o.name.find("BaselineMonitor_Hcal/ ProblemPedestals")!=std::string::npos) ||
	      (o.name.find("BaselineMonitor_Hcal/problem_pedestals")!=std::string::npos) ||
	      (o.name.find("HotCellMonitor_Hcal/ ProblemHotCells")!= std::string::npos ) ||
	      (o.name.find("HotCellMonitor_Hcal/problem_hotcells/") != std::string::npos) ||
	      (o.name.find("DeadCellMonitor_Hcal/ ProblemDeadCells")!= std::string::npos ) ||
	      (o.name.find("DeadCellMonitor_Hcal/problem_deadcells")!= std::string::npos ) ||
	      (o.name.find("RecHitMonitor_Hcal/ ProblemRecHits")!= std::string::npos ) ||
	      (o.name.find("RecHitMonitor_Hcal/problem_rechits")!= std::string::npos ) ||
	      (o.name.find("DataFormatMonitor/ HardwareWatchCells") != std::string::npos) ||
	      ((o.name.find("DataFormatMonitor") !=std::string::npos) && (o.name.find("Hardware Watch Cells") !=std::string::npos))
	      )
      {
        if ((obj->GetEntries()==0) ||
            ((obj->GetEntries()-obj->GetBinContent(0,0)) == 0) //underflow bin (0,0) stores ievt_; ignore this when checking for empty histograms?  nope, still doesn't work.  Dang!
	    )
	  {
	    gStyle->SetOptStat(1111);
	    obj->SetStats(1111);
	    TText t;
	    t.DrawText(1,1,"No News is Good News."); }
        else
	  {
	    gPad->SetGridx();
	    gPad->SetGridy();
	  }
      }
      
    // Dead Cell check -- warn that plots need many events before they update
    if (o.name.find("DeadCellMonitor_Hcal/" )!=std::string::npos)
      {
	if ( (o.name.find("DeadCellMonitor_Hcal/dead_digi_often_missing")!=std::string::npos) ||
	     (o.name.find("DeadCellMonitor_Hcal/dead_energytest")!=std::string::npos) ||
	     (o.name.find("DeadCellMonitor_Hcal/problem_deadcells") != std::string::npos) ||
	     (o.name.find("DeadCellMonitor_Hcal/Problem_") != std::string::npos) ||
	     (o.name.find("DeadCellMonitor_Hcal/dead_digi_often_missing/Problem_") != std::string::npos) 
	     )
	  {
	    if (obj->GetBinContent(0)<10000)
	      {
		TText t;
		t.DrawTextNDC(0.05,0.01,"Histogram updates every 10,000 events");
		gStyle->SetOptStat(1111);
		obj->SetStats(1111); 
	      }
	    else
	      {
		gStyle->SetOptStat(0);
		obj->SetStats(0);
		gPad->SetGridx();
		gPad->SetGridy(); 
	      }
	  } 
	else if ( (o.name.find("Problem_NeverPresentCells_") != std::string::npos) ||
		  (o.name.find("DeadCellMonitor_Hcal/ ProblemDeadCells") != std::string::npos) ||
		  (o.name.find("DeadCellMonitor_Hcal/problem_deadcells/") != std::string::npos) ||
		  (o.name.find("DeadCellMonitor_Hcal/dead_digi_never_present") != std::string::npos)
		  )
	  {
	    if (obj->GetBinContent(0)<1000)
	      {
		TText t;
		t.DrawTextNDC(0.05,0.01,"Histogram updates every 1,000 events");
	      }
	    else
	      {
		gStyle->SetOptStat(0);
		obj->SetStats(0);
		gPad->SetGridx();
		gPad->SetGridy(); 
	      }
	  }
      } // if (o.name.find(" DeadCellMonitor_Hcal/"...)
      
    //drawEtaPhiLines(obj);

  }

  void setRainbowColor(void)
  {
    gStyle->SetNumberContours(NCont_rainbow); // will this affect other histograms that don't use this palette?  Shouldn't -- Ncontours gets reset to default in post
    gStyle->SetPalette(NCont_rainbow,hcalRainbowColors);
  }

  void setErrorColor(void)
  {
    gStyle->SetNumberContours(NCont_hcalError);
    gStyle->SetPalette(NCont_hcalError,hcalErrorColors);
  }

  void drawEtaPhiLines(TH2* obj)
  {
    // I don't think we want these; it's easier just to zoom in from onlineDQM
    TAxis *xaxis =obj->GetXaxis();
    TAxis *yaxis=obj->GetYaxis();
    if (xaxis->GetXmax()!=HCAL_ETAMAX) return;
    if (xaxis->GetXmin()!=HCAL_ETAMIN) return;
    if (yaxis->GetXmax()!=HCAL_PHIMAX) return;
    if (yaxis->GetXmin()!=HCAL_PHIMIN) return;

    TLine* vert=0;
    TLine* horiz=0;
    // Draw vertical lines

    for (int xx=int(xaxis->GetXmin());
	 xx<=int(xaxis->GetXmax()); ++xx)
      {
        if (xx<-42 || xx >= 42) continue;
        vert = new TLine(xx+0.5,0.5,xx+0.5,72.5);
        vert->SetLineStyle(3);
        vert->Draw("same");
      }

    // Draw horizontal lines
    for (int yy=int(yaxis->GetXmin()); yy<int(yaxis->GetXmax());++yy)
      {
        if (yy%4==0)
          horiz = new TLine(-41.5,yy+0.5,41.5,yy+0.5);
        else if (yy%2==0)
          horiz = new TLine(-39.5,yy+0.5,39.5,yy+0.5);
        else
          horiz = new TLine(-20.5,yy+0.5,20.5,yy+0.5);
        horiz->SetLineStyle(3);
        horiz->Draw("same");
      }
  }
}; // HcalRenderPlugin class

static HcalRenderPlugin instance;
