/*!
  \file HcalRenderPlugin.cc
  \brief Display Plugin for Hcal DQM Histograms
  \author J. Temple
  \version $Revision: 1.2 $
  \date $Date: 2009/12/06 21:48:33 $
  \\
  \\ Code shamelessly borrowed from S. Dutta's SiStripRenderPlugin.cc code,
  \\ G. Della Ricca and B. Gobbo's EBRenderPlugin.cc, and other existing
  \\ subdetector plugins
  \\ preDraw and postDraw methods now check whether histogram was a TH1
  \\ or TH2, and call a private method appropriate for the histogram type

  // DQMNet::CoreObject replaced by VisDQMObject in Lassi's Oct. 31 revision 
  */

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TProfile2D.h"
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

class HcalCalibRenderPlugin : public DQMRenderPlugin
{
  // Color Schemes
  // seach scheme needs an array of color indices, 
  // the number of contours to be used, and the number of RGB stop points
  Int_t summaryColors[100];
  Int_t NRGBs_summary;
  Int_t NCont_summary;

  Int_t pedestalColors[40];
  Int_t NRGBs_pedestal;
  Int_t NCont_pedestal;

public:
  virtual void initialise (int, char **)
  {

    Int_t nColorsGradient=0; // stores number of gradients in each 'step' between colors
    int highestIndex=0;

    //make summaryColors, with a value of -1 indicated in gray, values >-1 and <0 in white,
    // values 0-0.98 scaling from red to yellow, and values > 0.98 in green.
    NRGBs_summary=7;
    NCont_summary=100;
    Double_t stops_summary[] = {0.00,0.025,0.4999,0.50,0.98,0.981,1.00}; // set limits for color transitions
    Double_t red_summary[]   = {0.6,1.00,1.00,1.00,1.00,0.00,0.00};
    Double_t green_summary[] = {0.6,1.00,1.00,0.00,1.00,0.80,0.80};
    Double_t blue_summary[]  = {0.6,1.00,1.00,0.00,0.00,0.00,0.00};
    nColorsGradient=0;
    highestIndex=0; // set starting index for new color (to avoid existing root colors)
    for (int g=1;g<NRGBs_summary;++g)
      {
	nColorsGradient = (Int_t) (floor(NCont_summary*stops_summary[g]) - floor(NCont_summary*stops_summary[g-1])); // specify number of gradients between stops (g-1) and (g)
        for (int c = 0; c < nColorsGradient; c++)
	  {
	    summaryColors[highestIndex]=1001+highestIndex;
	    TColor* color = gROOT->GetColor(1001+highestIndex);
	    // Make new color only if old color does not exist
	    if (!color)
	      color = new TColor(1001+highestIndex,
				 red_summary[g-1] + c * (red_summary[g] - red_summary[g-1])/ nColorsGradient,
				 green_summary[g-1] + c * (green_summary[g] - green_summary[g-1])/ nColorsGradient,
				 blue_summary[g-1] + c * (blue_summary[g] - blue_summary[g-1])/ nColorsGradient,
				 "  ");

	    highestIndex++;
	  }
      }

    // Sample pedestal color scheme; need to set something better at some point
    //make pedestalColors, with a value of -1 indicated in gray, values >-1 and <0 in white,
    // values 0-0.98 scaling from red to yellow, and values > 0.98 in green.
    NRGBs_pedestal=11;
    NCont_pedestal=40;
    Double_t stops_pedestal[] = {0.00,0.10,0.20,0.30,0.35,0.50,0.65,0.70,0.80,0.90,1.00}; // set limits for color transitions
    Double_t red_pedestal[]   = {0.60,1.00,0.50,0.00,0.00,0.00,0.00,0.75,1.00,1.00,1.00};
    Double_t green_pedestal[] = {0.60,0.00,0.00,0.50,0.90,1.00,0.90,0.75,0.50,0.25,0.00};
    Double_t blue_pedestal[]  = {0.60,1.00,1.00,0.75,0.10,0.00,0.10,0.00,0.00,0.00,0.00};
    nColorsGradient=0;
    highestIndex=0; // set starting index for new color (to avoid existing root colors)
    for (int g=1;g<NRGBs_pedestal;++g)
      {
	nColorsGradient = (Int_t) (floor(NCont_pedestal*stops_pedestal[g]) - floor(NCont_pedestal*stops_pedestal[g-1])); // specify number of gradients between stops (g-1) and (g)
        for (int c = 0; c < nColorsGradient; c++)
	  {
	    pedestalColors[highestIndex]=1101+highestIndex;
	    TColor* color = gROOT->GetColor(1101+highestIndex);
	    // Make new color only if old color does not exist
	    if (!color)
	      color = new TColor(1101+highestIndex,
				 red_pedestal[g-1] + c * (red_pedestal[g] - red_pedestal[g-1])/ nColorsGradient,
				 green_pedestal[g-1] + c * (green_pedestal[g] - green_pedestal[g-1])/ nColorsGradient,
				 blue_pedestal[g-1] + c * (blue_pedestal[g] - blue_pedestal[g-1])/ nColorsGradient,
				 "  ");

	    highestIndex++;
	  }
      }


  }

  virtual bool applies(const VisDQMObject &o, const VisDQMImgInfo &)
  {
    // determine whether core object is an Hcal object
    if (o.name.find( "HcalCalib/" ) != std::string::npos || o.name.find( "HcalCalib/" ) != std::string::npos)
      return true;

    return false;
  }

  virtual void preDraw (TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo &, VisDQMRenderInfo &)
  {
    c->cd();

    gStyle->Reset("Default");
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
    
    // Ay yi yi -- need to remember that other histogram types
    // inherit from TH1, so we can't do the TH1 check first.
    if( dynamic_cast<TProfile2D*>( o.object ) )
      {      }
    else if ( dynamic_cast<TProfile*>( o.object ) )
      preDrawTProfile( c, o );

    // object is TH2 histogram
    else if( dynamic_cast<TH2*>( o.object ) )
      preDrawTH2( c, o );
    // object is TH1 histogram
    else if( dynamic_cast<TH1*>( o.object ) )
      preDrawTH1( c, o );
  }

  virtual void postDraw (TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo &)
  {
    if( dynamic_cast<TProfile2D*>( o.object ) )
      {      }
    else if( dynamic_cast<TProfile*>( o.object ) )
      {      }
    // object is TH2 histogram
    else if( dynamic_cast<TH2*>( o.object ) )
      {
        postDrawTH2( c, o );
      }
    // object is TH1 histogram
    else if( dynamic_cast<TH1*>( o.object ) )
      {
        postDrawTH1( c, o );
      }
  }

private:
  void preDrawTH1 ( TCanvas * c, const VisDQMObject &o )
  {
    c->cd();
    // Do we want to do anything special yet with TH1 histograms?
    TH1* obj = dynamic_cast<TH1*>( o.object );
    assert (obj); // checks that object indeed exists
    
    // For now, all 1D plots have stats on.  Disable in future?
    gStyle->SetOptStat("iourmen");
    obj->SetStats(kTRUE);
    
  } // void preDrawTH1(...)


  void preDrawTH2 ( TCanvas *c, const VisDQMObject &o )
  {
    c->cd();
    TH2* obj = dynamic_cast<TH2*>( o.object );
    assert( obj );

    gStyle->SetCanvasBorderMode( 0 );
    gStyle->SetPadBorderMode( 0 );
    gStyle->SetPadBorderSize( 0 );

    // default coloring scheme
    gStyle->SetPalette(1);
    obj->SetOption("colz");
    //c->SetRightMargin(2*c->GetRightMargin()); // double right margin
    gPad->SetGridx();
    gPad->SetGridy();
    if (o.name.find("HcalCalib/EventInfo/reportSummaryMap")!=std::string::npos)
      {
	obj->SetOption("text90colz"); // draw marker at 90 degrees
	setColorScheme(obj,NCont_summary,summaryColors);
      }
    // Set Pedestal mean maximum to 2x nominal (3 ADC counts)
    if (o.name.find("HcalCalib/HcalDetDiagPedestalMonitor/Summary Plots/HBHEHF pedestal mean map") !=std::string::npos)
      {
	//setColorScheme(obj,NCont_pedestal,pedestalColors);
	obj->SetMinimum(0);
	if (obj->GetMaximum()<6)
	  obj->SetMaximum(6);
      }
    // Set Pedestal RMS maximum to 2x nominal
    if (o.name.find("HcalCalib/HcalDetDiagPedestalMonitor/Summary Plots/HBHEHF pedestal rms map") !=std::string::npos)
      {
	//setColorScheme(obj,NCont_pedestal,pedestalColors);
	obj->SetMinimum(0);
	if (obj->GetMaximum()<2)
	  obj->SetMaximum(2);
      }
  } // void preDrawTH2

  void preDrawTProfile ( TCanvas *, const VisDQMObject &o )
  {
    TProfile* obj = dynamic_cast<TProfile*>( o.object ); 
    assert( obj ); 
    // Set TProfile boundaries to first/last bins that have non-zero content
    bool foundfirst=false;
    int firstnonzerobin=1;
    int lastnonzerobin=1;
    for (int i=1;i<=obj->GetNbinsX();++i)
      {
	if (foundfirst==false && obj->GetBinContent(i)!=0)
	  {
	    foundfirst=true;
	    firstnonzerobin=i;
	  }
	if (obj->GetBinContent(i)!=0)
	  lastnonzerobin=i+1;
      }
    if (lastnonzerobin-firstnonzerobin>1)
      obj->GetXaxis()->SetRange(firstnonzerobin,lastnonzerobin-1);
    return;
  }

  void postDrawTH1( TCanvas *, const VisDQMObject &o )
  {
    TH1* obj = dynamic_cast<TH1*>( o.object ); 
    assert( obj ); 
  }

  void postDrawTH2( TCanvas *c, const VisDQMObject &o )
  {
    c->cd();
    TH2* obj = dynamic_cast<TH2*>( o.object );
    assert( obj );
  }

  void setColorScheme(TH2* obj, Int_t cont, Int_t* Colors)
  {
    obj->SetContour(cont);
    gStyle->SetPalette(cont, Colors);
  }


}; // HcalCalibRenderPlugin class

static HcalCalibRenderPlugin instance;

