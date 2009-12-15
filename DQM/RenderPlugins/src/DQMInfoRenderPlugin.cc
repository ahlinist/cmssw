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

#include "TProfile2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include <cassert>

class DQMInfoRenderPlugin : public DQMRenderPlugin
{
public:
  // These functions may be different that parent version
  virtual bool applies(const VisDQMObject &o, const VisDQMImgInfo &)
    {
      // determine whether core object is an Info object
      if( o.name.find( "Info/EventInfo/reportSummary" ) != std::string::npos)
        return true;
      return false;
    }

  virtual void preDraw (TCanvas * c, const VisDQMObject &o, const VisDQMImgInfo &, VisDQMRenderInfo &)
    {
      c->cd();
      gPad->SetLogy(0);

      // object is TH2 histogram
      if ( dynamic_cast<TH2F*>( o.object ) ) 
      {
         gPad->SetLogy(0);
         preDrawTH2F( c, o );
      }
    }

private:
  void preDrawTH2F ( TCanvas *, const VisDQMObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      
      int topBin = 26;
      int nbins = obj->GetNbinsX();
      int maxRange = nbins;
      for ( int i = nbins; i > 0; --i )
      {
	if ( obj->GetBinContent(i,topBin) != 0 )
	{
	   maxRange = i+1;
	   break;
	}
      }
      obj->GetXaxis()->SetRange(1,maxRange);
      obj->GetYaxis()->SetRange(1,topBin-1);
      gPad->SetGrid(1,1);
      gPad->SetLeftMargin(0.12);

      obj->SetStats( kFALSE );
      dqm::utils::redGreenPalette(obj);
      obj->SetOption("colz");

      return;

    }
};

static DQMInfoRenderPlugin instance;
