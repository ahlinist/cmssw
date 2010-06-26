/*!
  \file DQMInfoRenderPlugin.cc
  \\
  \\ Code shamelessly borrowed from J. Temple's HcalRenderPlugin.cc code,
  \\ which was shamelessly borrowed from S. Dutta's SiStripRenderPlugin.cc
  \\ code, G. Della Ricca and B. Gobbo's EBRenderPlugin.cc, and other existing
  \\ subdetector plugins
  \\ preDraw and postDraw methods now check whether histogram was a TH1
  \\ or TH2, and call a private method appropriate for the histogram type
*/

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include <cassert>
#include "TROOT.h"

class DQMInfoRenderPlugin : public DQMRenderPlugin
{
public:
  // These functions may be different that parent version
  virtual bool applies(const VisDQMObject &o, const VisDQMImgInfo &)
    {
      // determine whether core object is an Info object
      if ( o.name.find( "Info/EventInfo/reportSummaryMap" ) != std::string::npos
        || o.name.find( "Info/LhcInfo/") != std::string::npos )
        return true;
      return false;
    }

  virtual void preDraw (TCanvas * c, const VisDQMObject &o, const VisDQMImgInfo &, VisDQMRenderInfo &)
    {

      c->cd();
      gPad->SetLogy(0);
      
      if ( o.name.find( "Info/EventInfo/reportSummaryMap" ) != std::string::npos )
      {
        // object is TH2 histogram
        if ( dynamic_cast<TH2F*>( o.object ) ) 
        {
           gPad->SetLogy(0);
           preDrawTH2F( c, o );
        }
      }
      else if ( o.name.find( "Info/LhcInfo/") != std::string::npos )
        if ( dynamic_cast<TH1F*>( o.object ) )
	{
	   gPad->SetLogy(0);
	   preDrawTH1F( c, o);
	}
    }
 
  virtual void postDraw( TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo & )
    {
    
      c->cd();
      if ( o.name.find( "Info/EventInfo/reportSummaryMap" ) != std::string::npos )
      {
         // object is TH2 histogram
	 TH2F* obj = dynamic_cast<TH2F*>( o.object );
	 for ( int i = obj->GetNbinsX(); i > 0; --i )
	 {
	   if ( obj->GetBinContent(i,28) == 1 )
	   {
             char s[25];
  	     sprintf (s,"Stable Beams");
             TText tt;
	     tt.SetTextSize(0.1);
	     tt.SetTextColor(38);
	     tt.DrawTextNDC(0.25,0.6,const_cast<char*>(s));
	   }
	   if ( obj->GetBinContent(i,27) == 1 )
	   {
             char s[25];
  	     sprintf (s,"7 TeV");
             TText tt;
	     tt.SetTextSize(0.1);
	     tt.SetTextColor(38);
	     tt.DrawTextNDC(0.4,0.5,const_cast<char*>(s));
	   }
         }
      }
      else if ( o.name.find( "Info/LhcInfo/beamMode") != std::string::npos )
      {
	 TH1F* obj = dynamic_cast<TH1F*>( o.object );
	 assert( obj );

	 std::string smode[22] = 
	 {
	  "",
	  "no mode",
	  "setup",
	  "inj pilot",
	  "inj intr",
	  "inj nomn",
	  "pre ramp",
	  "ramp",
	  "flat top",
	  "squeeze",
	  "adjust",
	  "stable beams",
	  "unstable",
	  "beam dump",
	  "ramp down",
	  "recovery",
	  "inj dump",
	  "circ dump",
	  "abort",
	  "cycling",
	  "warn beam dump",
	  "no beam"
	 };
	 
	 int ibin=0;
	 for ( int i = obj->GetNbinsX(); i > 0; --i )
	 {
	   if ( obj->GetBinContent(i) != 0 )
	   {
	      ibin = (int)obj->GetBinContent(i);
	      break;
	   }
	 }

         if (ibin!=0)
	 {
           const char* s = smode[ibin].c_str();
           TText tt;
	   tt.SetTextSize(0.08);
           tt.SetTextColor(2);
	   if (ibin == 11)
	     tt.SetTextColor(3);
	   tt.DrawTextNDC(0.3,0.5,s);
	 }
      }
      else if ( o.name.find( "Info/LhcInfo/lhcFill") != std::string::npos )
      {
	 TH1F* obj = dynamic_cast<TH1F*>( o.object );
	 assert( obj );

	 int y=0;
	 for ( int i = obj->GetNbinsX(); i > 0; --i )
	 {
	   if ( obj->GetBinContent(i) != 0 )
	   {
	      y = (int)obj->GetBinContent(i);
	      break;
	   }
	 }

         if (y!=0 && y<9999) 
	 {
           char s[25];
	   sprintf (s,"LHC Fill: %d",y);
           TText tt;
	   tt.SetTextSize(0.06);
	   tt.SetTextColor(4);
	   tt.DrawTextNDC(0.2,0.5,const_cast<char*>(s));
	 }
      }
      else if ( o.name.find( "Info/LhcInfo/momentum") != std::string::npos )
      {
	 TH1F* obj = dynamic_cast<TH1F*>( o.object );
	 assert( obj );

	 int y=0;
	 for ( int i = obj->GetNbinsX(); i > 0; --i )
	 {
	   if ( obj->GetBinContent(i) != 0 )
	   {
	      y = (int)obj->GetBinContent(i);
	      break;
	   }
	 }

         if (y!=0 && y<9999)
	 {
           char s[25];
	   sprintf (s,"Beam Energy: %d GeV",y);
           TText tt;
	   tt.SetTextSize(0.06);
	   tt.SetTextColor(4);
	   tt.DrawTextNDC(0.2,0.5,const_cast<char*>(s));
	 }
      }

    } 
private:
  void preDrawTH2F ( TCanvas *, const VisDQMObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      
      int topBin = 29;
      int nbins = obj->GetNbinsX();
      int maxRange = nbins;
      for ( int i = nbins; i > 0; --i )
      {
	if ( obj->GetBinContent(i,topBin) != 0 )
	{
	   maxRange = i;
	   break;
	}
      }
      obj->GetXaxis()->SetRange(1,maxRange);
      //obj->GetYaxis()->SetRange(1,topBin-1);
      obj->GetYaxis()->SetRange(1,topBin);
      gPad->SetGrid(1,1);
      gPad->SetLeftMargin(0.12);

      obj->SetStats( kFALSE );

      int pcol[2];
      float rgb[2][2];
      rgb[0][0] = 0.80;
      rgb[0][1] = 0.00;
      rgb[1][0] = 0.00;
      rgb[1][1] = 0.80;

      for( int i=0; i<2; i++ )
      {
	pcol[i] = 801+i;
	TColor* color2 = gROOT->GetColor( 801+i );
	if( ! color2 ) color2 = new TColor( 801+i, 0, 0, 0, "" );
	color2->SetRGB( rgb[i][0], rgb[i][1], 0. );
      }

      gStyle->SetPalette(2, pcol);
      obj->SetMinimum(-1.e-15);
      obj->SetMaximum(1.0);
      obj->SetOption("colz");

      return;

    }

  void preDrawTH1F ( TCanvas *, const VisDQMObject &o )
    {
      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert( obj );

      int nbins = obj->GetNbinsX();
      int maxRange = nbins;
      for ( int i = nbins; i > 0; --i )
      {
	if ( obj->GetBinContent(i) != 0 )
	{
	   maxRange = i+1;
	   break;
	}
      }
      obj->GetXaxis()->SetRange(1,maxRange);
      obj->SetStats( kFALSE );
      obj->SetMinimum(-1.e-15);

      if ( o.name.find( "Info/LhcInfo/beamMode") != std::string::npos )
      {
        obj->SetMaximum(22.);
        obj->SetMinimum(0.);
        gPad->SetLeftMargin(0.15);
        gPad->SetGrid(1,1);
      }
      
      // FIXME: put in last value as text
      
      return;

    }
};

static DQMInfoRenderPlugin instance;
