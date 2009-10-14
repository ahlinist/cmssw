/*!
  \file HLTRenderPlugin.cc
  \\ Current maintainer: slaunwhj
  \\ Based on HLTRenderPlugin
  \\


  $Id: HLTRenderPlugin.cc,v 1.8 2009/09/21 14:34:02 rekovic Exp $
  $Log: HLTRenderPlugin.cc,v $
  Revision 1.8  2009/09/21 14:34:02  rekovic
  Fix FourVector renders

  Revision 1.7  2009/05/22 19:09:33  lat
  Untabify.

  Revision 1.6  2009/05/22 19:05:23  lat
  Adapt to keeping render plug-ins outside server RPM. Clean up and harmonise code.

  Revision 1.5  2008/10/01 17:57:39  lorenzo
  changed EventInfo format

  Revision 1.4  2008/08/28 21:50:39  wittich
  Rate histos: Also put in low range minimums in case we start in the
  middle of a run
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

class HLTRenderPlugin : public DQMRenderPlugin
{
public:
  virtual bool applies(const DQMNet::CoreObject &o, const VisDQMImgInfo &)
    {
      // determine whether core object is an HLT object
      if (o.name.find( "HLT/Muon" ) != std::string::npos  )
        return true;

      return false;
    }

  virtual void preDraw (TCanvas * c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo &)
    {
      c->cd();

      // object is TH2 histogram
      if( dynamic_cast<TH2F*>( o.object ) )
      {
        preDrawTH2F( c, o );
      }
      // object is TH1 histogram
      else if( dynamic_cast<TH1F*>( o.object ) )
      {
        preDrawTH1F( c, o );
      }
    }

  virtual void postDraw (TCanvas * c, const DQMNet::CoreObject &o, const VisDQMImgInfo &)
    {
      // object is TH2 histogram
      if( dynamic_cast<TH2F*>( o.object ) )
      {
        postDrawTH2F( c, o );
      }
      // object is TH1 histogram
      else if( dynamic_cast<TH1F*>( o.object ) )
      {
        postDrawTH1F( c, o );
      }
    }

private:
  void preDrawTH1F ( TCanvas *, const DQMNet::CoreObject &o )
    {
      // Do we want to do anything special yet with TH1F histograms?
      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert (obj); // checks that object indeed exists


      // if this isn't a muon hlt plot, skip it
      if (o.name.find("HLT/Muon") == std::string::npos)
        return;


      // Do these for all your histos
      gStyle->SetOptStat(10);
      
      // FourVector eff histograms
      if ( o.name.find("recEff") != std::string::npos)
      {
        
        obj->SetMinimum(0);
        obj->SetMaximum(1.0);
        obj->GetYaxis()->SetTitle("_Eff_");

      }

      if (o.name.find("recPassPt") != std::string::npos)
        {
          
          gPad->SetLogy();
          
        }

    }

  void preDrawTH2F ( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      // if this isn't a muon hlt plot, skip it
      if (o.name.find("HLT/Muon") == std::string::npos)
        return;

      // You'll want to do all of these things 
      // Regardless of the histo
      
      gStyle->SetOptStat(10);
      gStyle->SetPalette(1,0);
      obj->SetOption("colz");
      gPad->SetGrid(1,1);
      
      //Handle the 2D eff histograms
      if( o.name.find( "recEffPhiVsEta" )  != std::string::npos)
        {
         
          obj->SetMinimum(0.0);
          obj->SetMaximum(1.0);
          
          return;
        }
      
      // if not a 2D eff, do nothing special
      return;

    }

  void postDrawTH1F( TCanvas *, const DQMNet::CoreObject & )
    {

      // No special actions necessary right now

      return;

    }

  void postDrawTH2F( TCanvas *, const DQMNet::CoreObject & )
    {
      // nothing to put here just yet
      // in the future, we can add text output based on error status,
      // or set bin range based on filled histograms, etc.
      // Maybe add a big "OK" sign to histograms with no entries (i.e., no errors)?
    }
};

static HLTRenderPlugin instance;
