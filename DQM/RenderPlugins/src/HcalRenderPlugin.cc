/*!
  \file HcalRenderPlugin.cc
  \brief Display Plugin for Hcal DQM Histograms
  \author J. Temple
  \version $Revision: 1.1 $
  \date $Date: 2008/04/26 07:43:42 $
  \\
  \\ Code shamelessly borrowed from S. Dutta's SiStripRenderPlugin.cc code,
  \\ G. Della Ricca and B. Gobbo's EBRenderPlugin.cc, and other existing
  \\ subdetector plugins
  \\ preDraw and postDraw methods now check whether histogram was a TH1
  \\ or TH2, and call a private method appropriate for the histogram type
*/

#include "DQM/RenderPlugins/src/HcalRenderPlugin.h" 

#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"

#include <cassert>
#include <fstream>

void
HcalRenderPlugin::initialise (int argc, char ** argv)
{
  // same as RenderPlugin default for now (no special action taken)
  return;
}



bool HcalRenderPlugin::applies(const DQMNet::CoreObject &o,
				  const VisDQMImgInfo &i)
{
  // determine whether core object is an Hcal object
#ifdef DEBUG 
  std::cout << "HcalRenderPlugin:applies " << o.name << std::endl; 
#endif 

  if (o.name.find( "Hcal/" ) != std::string::npos)
    {
      return true;
    }
  
  return false;
}



void
HcalRenderPlugin::preDraw (TCanvas * c,
			      const DQMNet::CoreObject &o,
			      const VisDQMImgInfo &i,
			      VisDQMRenderInfo &r)
{

#ifdef DEBUG 
  std::cout << "HcalRenderPlugin:preDraw " << o.name << std::endl; 
#endif 
  c->cd(); 
 
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

  else
#ifdef DEBUG 
    std::cout << "HcalRenderPlugin:preDraw  -- Cannot identify object " << o.name << std::endl; 
#endif 

  return;
} // HcalRenderPlugin::preDraw(...)


void
HcalRenderPlugin::postDraw (TCanvas * c,
			       const DQMNet::CoreObject & o,
			       const VisDQMImgInfo & i)
{

#ifdef DEBUG 
  std::cout << "HcalRenderPlugin:postDraw " << o.name << std::endl; 
#endif 

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

  else
#ifdef DEBUG 
    std::cout << "HcalRenderPlugin:postDraw  -- Cannot identify object " << o.name << std::endl; 
#endif 

  return;
} // HcalRenderPlugin::postDraw(...)



////////////////////////////////////////////////////////////////////////

//  private functions


void HcalRenderPlugin::preDrawTH1 ( TCanvas *c, const DQMNet::CoreObject &o )
{
  // Do we want to do anything special yet with TH1 histograms?

  TH1* obj = dynamic_cast<TH1*>( o.object ); 
  assert (obj); // checks that object indeed exists

  // Code used in SiStripRenderPlugin -- do we want similar defaults?

  /*
    gStyle->SetOptStat(0111); 
    if ( obj->GetMaximum(1.e5) > 0. ) { 
    gPad->SetLogy(1); 
    } else { 
    gPad->SetLogy(0); 
    } 
  */

  return;

} // preDrawTH1(...)




void HcalRenderPlugin::preDrawTH2 ( TCanvas *c, const DQMNet::CoreObject &o )
{
  
  TH2* obj = dynamic_cast<TH2*>( o.object ); 
  assert( obj ); 
  
  gStyle->SetCanvasBorderMode( 0 ); 
  gStyle->SetPadBorderMode( 0 ); 
  gStyle->SetPadBorderSize( 0 ); 
  
  // I don't think we want to set stats to 0 for Hcal
  gStyle->SetOptStat( 0 ); 
  obj->SetStats( kFALSE ); 

  // Use same labeling format as SiStripRenderPlugin.cc
  /* Axes formatting skipped at DQM request
  TAxis* xa = obj->GetXaxis(); 
  TAxis* ya = obj->GetYaxis(); 
  
  xa->SetTitleOffset(0.7); 
  xa->SetTitleSize(0.05); 
  xa->SetLabelSize(0.04); 
  
  ya->SetTitleOffset(0.7); 
  ya->SetTitleSize(0.05); 
  ya->SetLabelSize(0.04); 
  */
  // Now the important stuff -- set 2D hist drawing option to "colz"
  gStyle->SetPalette(1);
  obj->SetOption("colz");

  return;
} // preDrawTH2(...)




void HcalRenderPlugin::postDrawTH1( TCanvas *c, const DQMNet::CoreObject &o )
{

  // Add error/warning text to 1-D histograms.  Do we want this at this time?
  /*
  TText tt; 
  tt.SetTextSize(0.12); 

  if (o.flags == 0) return; 

  else 
    { 
      if (o.flags & DQMNet::DQM_FLAG_REPORT_ERROR) 
	{ 
	  tt.SetTextColor(2); // error color = RED 
	  tt.DrawTextNDC(0.5, 0.5, "Error"); 
	}  // DQM_FLAG_REPORT_ERROR

      else if (o.flags & DQMNet::DQM_FLAG_REPORT_WARNING) 
	{ 
	  tt.SetTextColor(5); 
	  tt.DrawTextNDC(0.5, 0.5, "Warning"); // warning color = YELLOW 
	} // DQM_FLAG_REPORT_WARNING

      else if (o.flags & DQMNet::DQM_FLAG_REPORT_OTHER) 
	{  
	  tt.SetTextColor(1); // other color = BLACK
	  tt.DrawTextNDC(0.5, 0.5, "Other ");       
	} // DQM_FLAG_REPORT_OTHER

      else 
	{ 
	  tt.SetTextColor(3); 
	  tt.DrawTextNDC(0.5, 0.5, "Ok "); 
	} //else
    } // else (  o.flags != 0  )
  */
  return;

} // postDrawTH1(...)




void HcalRenderPlugin::postDrawTH2( TCanvas *c, const DQMNet::CoreObject &o )
{
  // nothing to put here just yet
  // in the future, we can add text output based on error status,
  // or set bin range based on filled histograms, etc.  
  // Maybe add a big "OK" sign to histograms with no entries (i.e., no errors)?

  return;

} // postDrawTH2(...)
