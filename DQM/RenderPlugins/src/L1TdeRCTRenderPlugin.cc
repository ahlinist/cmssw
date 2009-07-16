// $Id: L1TdeRCTRenderPlugin.cc,v 1.13 2009/05/22 19:09:33 lat Exp $

/*!
  \file L1TdeRCTRenderPlugin
  \brief Display Plugin for Quality Histograms
  \author A.Savin
  \version $Revision: 1.13 $
  \date $Date: 2009/05/22 19:09:33 $
*/

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TColor.h"
#include "TROOT.h"
#include <iostream>
#include <cassert>

class L1TdeRCTRenderPlugin : public DQMRenderPlugin
{
  TH2F* dummybox;

  Int_t paletteSize;
  Int_t nContours;

  Int_t pEff    [100];
  Int_t pIneff  [100];
  Int_t pOvereff[100];

public:
  virtual void initialise(int, char **)
    {
      Float_t rgb[300] = {0};
      paletteSize = 100;
      nContours   = 100;

      for (Int_t i = 0; i < paletteSize; i++)
      {
        rgb[3 * i + 0] = 0.0;
        rgb[3 * i + 1] = 0.8;
        rgb[3 * i + 2] = 0.0;

        if (i <= 98)
        {
          rgb[3 * i + 0] = 0.5;
          rgb[3 * i + 1] = 1.0;
          rgb[3 * i + 2] = 0.0;
        }
        if (i <= 94)
        {
          rgb[3 * i + 0] = 1.0;
          rgb[3 * i + 1] = 1.0;
          rgb[3 * i + 2] = 0.0;
        }
        if (i <= 89)
        {
          rgb[3 * i + 0] = 1.0;
          rgb[3 * i + 1] = 0.5;
          rgb[3 * i + 2] = 0.0;
        }
        if (i <= 84)
        {
          rgb[3 * i + 0] = 1.0;
          rgb[3 * i + 1] = 0.0;
          rgb[3 * i + 2] = 0.0;
        }
        pEff [i] = TColor::GetColor (rgb[3 * i + 0], rgb[3 * i + 1], rgb[3 * i + 2]);
      }

      for (Int_t i = 0; i < paletteSize; i++)
      {
        rgb[3 * i + 0] = 1.0;
        rgb[3 * i + 1] = 0.0;
        rgb[3 * i + 2] = 0.0;

        if (i <= 14)
        {
          rgb[3 * i + 0] = 1.0;
          rgb[3 * i + 1] = 0.5;
          rgb[3 * i + 2] = 0.0;
        }
        if (i <= 9)
        {
          rgb[3 * i + 0] = 1.0;
          rgb[3 * i + 1] = 1.0;
          rgb[3 * i + 2] = 0.0;
        }
        if (i <= 4)
        {
          rgb[3 * i + 0] = 0.5;
          rgb[3 * i + 1] = 1.0;
          rgb[3 * i + 2] = 0.0;
        }
        if (i < 1)
        {
          rgb[3 * i + 0] = 0.0;
          rgb[3 * i + 1] = 0.8;
          rgb[3 * i + 2] = 0.0;
        }
        pIneff [i] = TColor::GetColor (rgb[3 * i + 0], rgb[3 * i + 1], rgb[3 * i + 2]);
      }

      for (Int_t i = 0; i < paletteSize; i++)
      {
        rgb[3 * i + 0] = 1.0;
        rgb[3 * i + 1] = 0.0;
        rgb[3 * i + 2] = 0.0;

        if (i <= 14)
        {
          rgb[3 * i + 0] = 1.0;
          rgb[3 * i + 1] = 0.5;
          rgb[3 * i + 2] = 0.0;
        }
        if (i <= 9)
        {
          rgb[3 * i + 0] = 1.0;
          rgb[3 * i + 1] = 1.0;
          rgb[3 * i + 2] = 0.0;
        }
        if (i <= 4)
        {
          rgb[3 * i + 0] = 0.5;
          rgb[3 * i + 1] = 1.0;
          rgb[3 * i + 2] = 0.0;
        }
        if (i < 1)
        {
          rgb[3 * i + 0] = 0.0;
          rgb[3 * i + 1] = 0.8;
          rgb[3 * i + 2] = 0.0;
        }
        pOvereff [i] = TColor::GetColor (rgb[3 * i + 0], rgb[3 * i + 1], rgb[3 * i + 2]);
      }

      dummybox = new  TH2F("dummy","",22,-0.5,21.5,18,-0.5,17.5);

      for(int i=0; i<22; i++)
      {
        for(int j=0; j<18; j++)
        {
          dummybox->Fill(i,j);
        }
      }
    }

  virtual bool applies( const DQMNet::CoreObject &o, const VisDQMImgInfo & )
    {
      if( o.name.find( "L1TdeRCT" ) != std::string::npos )
        return true;

/*
      if( o.name.find( "L1TdeRCT/IsoEm" ) != std::string::npos )
        return true;

      if( o.name.find( "L1TdeRCT/IsoEm/ServiceData" ) != std::string::npos )
        return true;

      if( o.name.find( "L1TdeRCT/NisoEm" ) != std::string::npos )
        return true;

      if( o.name.find( "L1TdeRCT/NisoEm/ServiceData" ) != std::string::npos )
        return true;

      if( o.name.find( "L1TdeRCT/EffCurves/NisoEm" ) != std::string::npos )
        return true;

      if( o.name.find( "L1TdeRCT/EffCurves/NisoEm/ServiceData" ) != std::string::npos )
        return true;

      if( o.name.find( "L1TdeRCT/RegionData" ) != std::string::npos )
        return true;

      if( o.name.find( "L1TdeRCT/RegionData/ServiceData" ) != std::string::npos )
        return true;

      if( o.name.find( "L1TdeRCT/BitData" ) != std::string::npos )
        return true;

      if( o.name.find( "L1TdeRCT/BitData/ServiceData" ) != std::string::npos )
        return true;
*/

      return false;
    }

  virtual void preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo &r )
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

      gStyle->SetOptStat(11);
      gStyle->SetStatBorderSize(1);

      gStyle->SetOptFit(kFALSE);

      gROOT->ForceStyle();

      if( dynamic_cast<TProfile2D*>( o.object ) )
      {
        preDrawTProfile2D( c, o );
      }
      else if( dynamic_cast<TProfile*>( o.object ) )
      {
        preDrawTProfile( c, o );
      }
      else if( dynamic_cast<TH3F*>( o.object ) )
      {
        preDrawTH3F( c, o );
      }
      else if( dynamic_cast<TH2F*>( o.object ) )
      {
        preDrawTH2F( c, o );
      }
      else if( dynamic_cast<TH1F*>( o.object ) )
      {
        preDrawTH1F( c, o );
      }
      r.drawOptions = "";
    }

  virtual void postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo & )
    {
      c->cd();

      if( dynamic_cast<TProfile2D*>( o.object ) )
      {
        postDrawTProfile2D( c, o );
      }
      else if( dynamic_cast<TH3F*>( o.object ) )
      {
        postDrawTH3F( c, o );
      }
      else if( dynamic_cast<TH2F*>( o.object ) )
      {
        postDrawTH2F( c, o );
      }
      else if( dynamic_cast<TH1F*>( o.object ) )
      {
        postDrawTH1F( c, o );
      }
    }

private:
  void preDrawTProfile2D( TCanvas *, const DQMNet::CoreObject & )
    {
    }

  void preDrawTProfile( TCanvas *, const DQMNet::CoreObject & )
    {
    }

  void preDrawTH3F( TCanvas *, const DQMNet::CoreObject & )
    {
    }

  void preDrawTH2F( TCanvas *, const DQMNet::CoreObject &o )
    {
      paletteSize = 100 ;

      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      gStyle->SetPaintTextFormat();

      gStyle->SetOptStat(11);
      gPad->SetLogy(kFALSE);

      gStyle->SetNumberContours (nContours);
      obj->GetXaxis()->SetTitle("GCT eta");
      obj->GetYaxis()->SetTitle("GCT phi");

      if( name.find( "rctInputTPGEcalOcc" ) != std::string::npos )
      {
        obj->GetXaxis()->SetTitle("CAL eta");
        obj->GetYaxis()->SetTitle("CAL phi");
        return;
      }

      if( name.find( "rctInputTPGHcalOcc" ) != std::string::npos )
      {
        obj->GetXaxis()->SetTitle("CAL eta");
        obj->GetYaxis()->SetTitle("CAL phi");
        return;
      }

      //--Iso stuff
      if( name.find( "rctIsoEmDataOcc" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctIsoEmEmulOcc" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctIsoEmEff1Occ" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctIsoEmEff2Occ" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctIsoEmIneffOcc" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctIsoEmOvereffOcc" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctIsoEmEff1" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.005);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctIsoEmEff2" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.0);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctIsoEmIneff" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pIneff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctIsoEmOvereff" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pOvereff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      //--Niso stuff
      if( name.find( "rctNisoEmDataOcc" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctNisoEmEmulOcc" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctNisoEmEff1Occ" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctNisoEmEff2Occ" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctNisoEmIneffOcc" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctNisoEmOvereffOcc" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctNisoEmEff1" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.005);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctNisoEmEff2" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.0);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctNisoEmIneff" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pIneff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctNisoEmOvereff" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pOvereff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      //--Regional stuff
      if( name.find( "rctRegDataOcc2D" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctRegEmulOcc2D" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctRegMatchedOcc2D" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctRegUnmatchedDataOcc2D" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctRegUnmatchedEmulOcc2D" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctRegDeltaEtOcc2D" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctRegEff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.005);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctRegSpEff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.0);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctRegIneff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pIneff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctRegOvereff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pOvereff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      // Eff Curves
      if( name.find( "trigEffThreshOcc" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "trigEffTriggThreshOcc" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "trigEffThresh" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.005);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      //--Bit data
      if( name.find( "rctBitEmul" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctBitData" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctBitMatched" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctBitUnmatched" ) != std::string::npos )
      {
        obj->SetOption("box");
        return;
      }

      if( name.find( "rctBitOverFlowEff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.005);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitOverFlowIneff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pIneff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitOverFlowOvereff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pOvereff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitTauVetoEff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.005);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitTauVetoIneff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pIneff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitTauVetoOvereff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pOvereff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitMipEff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.005);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitMipIneff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pIneff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitMipOvereff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pOvereff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitQuietEff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.005);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitQuietIneff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pIneff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitQuietOvereff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pOvereff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitHfPlusTauEff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pEff);
        obj->SetMinimum(0.005);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitHfPlusTauIneff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pIneff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }

      if( name.find( "rctBitHfPlusTauOvereff2D" ) != std::string::npos )
      {
        gStyle->SetPalette(paletteSize, pOvereff);
        obj->SetMinimum(0.0002);
        obj->SetMaximum(1.0);
        obj->SetStats(kFALSE);
        obj->SetOption("colz");
        return;
      }
    }

  void preDrawTH1F( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert( obj );
     
      std::string name = o.name.substr(o.name.rfind("/")+1);

      obj->GetXaxis()->SetTitle("Channnel number");
      gStyle->SetOptStat(11);

      if( name.find( "gtTriggerAlgoNumbers" ) != std::string::npos )
      {
        obj->GetXaxis()->SetTitle("GT AlgoNumber");
        return;
      }

      if( name.find( "rctInputTPGHcalSample" ) != std::string::npos )
      {
        obj->GetXaxis()->SetTitle("HCAL sample");
        return;
      }

      //--Iso stuff
      if( name.find( "rctIsoEmEff1oneD" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }

      if( name.find( "rctIsoEmEff2oneD" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }

      if( name.find( "rctIsoEmIneff1D" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }

      if( name.find( "rctIsoEmOvereff1D" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }

      //--Niso stuff

      if( name.find( "rctNisoEmEff1oneD" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }

      if( name.find( "rctNisoEmEff2oneD" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }

      if( name.find( "rctNisoEmIneff1D" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }

      if( name.find( "rctNisoEmOvereff1D" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }

      //--Regional stuff

      if( name.find( "rctRegEff1D" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }

      if( name.find( "rctRegSpEff1D" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }

      if( name.find( "rctRegIneff1D" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }

      if( name.find( "rctRegOvereff1D" ) != std::string::npos )
      {
        obj->SetStats(kFALSE);
        return;
      }


    }

  void postDrawTProfile2D( TCanvas *, const DQMNet::CoreObject & )
    {
    }

  void postDrawTH3F( TCanvas *, const DQMNet::CoreObject & )
    {
    }

  void postDrawTH2F( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      if( name.find( "rctIsoEmEff1" ) != std::string::npos &&
          name.find( "rctIsoEmEff1" ) != name.find( "rctIsoEmEff1Occ" ))
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctIsoEmEff2" ) != std::string::npos &&
          name.find( "rctIsoEmEff2" ) != name.find( "rctIsoEmEff2Occ" ) )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctIsoEmIneff" ) != std::string::npos &&
          name.find( "rctIsoEmIneff" ) != name.find( "rctIsoEmIneffOcc" ) )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctIsoEmOvereff" ) != std::string::npos &&
          name.find( "rctIsoEmOvereff" ) != name.find( "rctIsoEmOvereffOcc" ) )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctNisoEmEff1" ) != std::string::npos &&
          name.find( "rctNisoEmEff1" ) != name.find( "rctNisoEmEff1Occ") )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctNisoEmEff2" ) != std::string::npos &&
          name.find( "rctNisoEmEff2" ) != name.find( "rctNisoEmEff2Occ" ) )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctNisoEmIneff" ) != std::string::npos &&
          name.find( "rctNisoEmIneff" ) != name.find( "rctNisoEmIneffOcc" ) )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctNisoEmOvereff" ) != std::string::npos &&
          name.find( "rctNisoEmOvereff" ) != name.find( "rctNisoEmOvereffOcc" ) )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctRegEff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctRegIneff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctRegOvereff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctRegSpEff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "trigEffThresh" ) != std::string::npos &&
          name.find( "trigEffThresh" ) != name.find( "trigEffThreshOcc" ) )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitOverFlowEff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitOverFlowIneff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitOverFlowOvereff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitTauVetoEff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitTauVetoIneff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitTauVetoOvereff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitMipEff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitMipIneff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitMipOvereff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitQuietEff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitQuietIneff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitQuietOvereff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitHfPlusTauEff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitHfPlusTauIneff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }

      if( name.find( "rctBitHfPlusTauOvereff2D" ) != std::string::npos )
      {
        dummybox->Draw("box,same");
        return;
      }
    }

  void postDrawTH1F( TCanvas *, const DQMNet::CoreObject & )
    {
    }
};

static L1TdeRCTRenderPlugin instance;
