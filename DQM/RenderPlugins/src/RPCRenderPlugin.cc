#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TProfile2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include <iostream>
#include <cassert>
#include "TLine.h"
#include "TText.h"
#include "TPaletteAxis.h"
#include "TH2F.h"
#include "TList.h"

class RPCRenderPlugin : public DQMRenderPlugin
{
public:
  virtual bool applies( const DQMNet::CoreObject &o, const VisDQMImgInfo & )
    {
      if (o.name.find("RPC/") == std::string::npos)
        return false;

      if( (o.name.find( "/RecHits/" ) != std::string::npos )||
          (o.name.find("/EventInfo") != std::string::npos))
        return true;

      return false;
    }

  virtual void preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo & )
    {
      c->cd();

      if( dynamic_cast<TH2*>( o.object ) )
      {
        preDrawTH2( c, o );
      }
    }

  virtual void postDraw(TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &)
    {
      c->cd();

      TText tt;
      tt.SetTextSize(0.12);

      //  if(o.name.find("Endcap") != std::string::npos && o.name.find("Barrel") == std::string::npos  ) {
      //     tt.SetTextColor(3);
      //     tt.DrawTextNDC(0.5, 0.5, "OK");
      //     return;
      //   }
      if(dynamic_cast<TH2*>( o.object ) ) postDrawTH2(c,o);
    }

private:
  void preDrawTH2( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      // This applies to all
      gStyle->SetCanvasBorderMode( 0 );
      gStyle->SetCanvasColor(kWhite);
      gStyle->SetPadBorderMode( 0 );
      gStyle->SetPadBorderSize( 0 );
      gStyle->SetOptStat( 10 );
      gStyle->SetPalette( 1 );
      obj->SetOption( "colz" );
      obj->SetStats( kFALSE );

      if(o.name.find("reportSummaryMap") != std::string::npos)
      {
        dqm::utils::reportSummaryMapPalette(obj);
        // gStyle->SetPaintTextFormat("%.2f");
        // obj->SetOption("colztext");
        return;
      }

      if( o.name.find("Occupancy") != std::string::npos )
      {
        obj->SetStats( kTRUE );
        return;
      }

      if( o.name.find("AfterPulse")!= std::string::npos)
      {
        obj->SetStats( kTRUE );
        obj->SetOption( "SCAT" );
        return;
      }
    }

  void postDrawTH2(TCanvas *c, const DQMNet::CoreObject &o)
    {
      TH2* obj = dynamic_cast<TH2*>( o.object );
      assert( obj );

      if(o.name.find("Roll_vs_Sector") != std::string::npos)
      {//roll vs sector 2Dhisto
        obj->GetXaxis()->SetNdivisions(-510);
        obj->GetYaxis()->SetNdivisions(-510);
        obj->GetXaxis()->CenterLabels();
        obj->GetYaxis()->CenterLabels();
        c->SetGridx();
        c->SetGridy();
      }

      if(o.name.find("SummaryMap") != std::string::npos)
      {//report summary map
        obj->GetXaxis()->SetNdivisions(-510);
        obj->GetYaxis()->SetNdivisions(-510);
        obj->GetXaxis()->CenterLabels();
        obj->GetYaxis()->CenterLabels();
        c->SetGridx();
        c->SetGridy();

        TLine line;// draw lines to delimitate Barrel and Endcaps
        line.SetLineWidth(1);
        line.DrawLine(-3.5, 0.5, -3.5, 6.5);
        line.DrawLine(-7.5, 6.5,-3.5, 6.5 );
        line.DrawLine(-2.5, 0.5, -2.5, 12.5);
        line.DrawLine(2.5, 0.5, 2.5, 12.5);
        line.DrawLine(-2.5, 12.5, 2.5, 12.5);

        line.DrawLine(3.5, 0.5, 3.5, 6.5);
        line.DrawLine(3.5, 6.5,7.5, 6.5 );
        line.DrawLine(7.5, 0.5,7.5, 6.5 );
        return;
      }

      if(o.name.find("Occupancy") != std::string::npos && o.name.find("SummaryBySectors") != std::string::npos && o.name.find("Wheel") != std::string::npos  )
      {
        //sector occupancy plots
        TLine line;
        line.SetLineWidth(2);// Draw lines to delimitate the end of the roll
        //rb1in
        line.DrawLine(91, 0.5, 91, 2.5);
        line.DrawLine(91, 2.5, 85, 2.5);
        //rb1out
        line.DrawLine(85, 2.5, 85, 4.5);
        line.DrawLine(85, 4.5, 91, 4.5);
        //rb2in X 3
        line.DrawLine(91, 4.5, 91, 7.5);
        line.DrawLine(91, 7.5, 85, 7.5);
        //rb2out
        line.DrawLine(85, 7.5, 85, 9.5);
        line.DrawLine(85, 9.5, 43, 9.5);
        //rb3
        line.DrawLine(43, 9.5, 43, 13.5);

        gPad->Update();
        TPaletteAxis *palette;
        palette = (TPaletteAxis*)obj->GetListOfFunctions()->FindObject("palette");
        palette->GetAxis()->SetLabelSize(0.024);
        gPad->Update();
      }

      if(o.name.find("Occupancy") != std::string::npos)
      {          //summary occupancy plots
        obj->GetXaxis()->SetNdivisions(-510);
        obj->GetYaxis()->SetNdivisions(-510);
        obj->GetXaxis()->CenterLabels();
        obj->GetYaxis()->CenterLabels();
        c->SetGridx();
        c->SetGridy();
        //    return;
      }

      if(o.name.find("OccupancyNormByGeoAndRPCEvents")!= std::string::npos)
      {
        obj->SetMaximum(0.2);
        return;
      }

      if( o.name.find("AfterPulse")!= std::string::npos)
      {//afterpulse 2D plots
        obj->GetXaxis()->LabelsOption("v");
        obj->GetXaxis()->SetLabelSize(0.03);
        obj->GetXaxis()->SetLabelOffset(0.005);
        obj->GetXaxis()->SetNdivisions(-510);
        obj->GetYaxis()->SetLabelSize(0.03);
        obj->GetYaxis()->SetLabelOffset(0.005);
        obj->GetYaxis()->SetNdivisions(-510);
        return;
      }

      if(o.name.find("ClusterSizeIn1Bin_Roll_vs_Sector") != std::string::npos)
      {
        obj->SetMinimum(-1.e-15);
        obj->SetMaximum(1.0);

        int colorPalette[20];
        for(int i=0; i<15; i++)
        {
          colorPalette[i]=416;
        }
        colorPalette[15]= 400; // Yallow
        colorPalette[16]= 807; // Orange
        colorPalette[17]= 632; // Red
        colorPalette[18]= 632;
        colorPalette[19]= 632;

        gStyle->SetPalette(20, colorPalette);
        return;
      }

      if(o.name.find("AsymmetryLeftRight_Roll_vs_Sector") != std::string::npos)
      {
        obj->SetMinimum(-1.e-15);
        obj->SetMaximum(1.0);

        int colorPalette1[10];

        colorPalette1[0]=416;
        colorPalette1[1]=416;
        colorPalette1[2]=416;

        colorPalette1[3]= 400; // Yallow
        colorPalette1[4]= 400; // Yallow

        colorPalette1[5]= 807; // Orange
        colorPalette1[6]= 807; // Orange

        colorPalette1[7]= 632; // Red
        colorPalette1[8]= 632;
        colorPalette1[9]= 632;

        gStyle->SetPalette(10, colorPalette1);
        return;
      }

      if(o.name.find("DeadChannelFraction_Roll_vs_Sector") != std::string::npos)
      {
        obj->SetMinimum(-1.e-15);
        obj->SetMaximum(1.0);

        int colorPalette2[10];

        colorPalette2[0]=416;
        colorPalette2[1]=416;
        colorPalette2[2]=416;

        colorPalette2[3]= 400; // Yallow
        colorPalette2[4]= 400; // Yallow
        colorPalette2[5]= 400; // Yallow

        colorPalette2[6]= 807; // Orange
        colorPalette2[7]= 807; // Orange

        colorPalette2[8]= 632; // Red
        colorPalette2[9]= 632;

        gStyle->SetPalette(10, colorPalette2);
        return;
      }

      if(o.name.find("RPCChamberQuality") != std::string::npos)
      {
        obj->SetMinimum(0.5);
        obj->SetMaximum(7.5);

        int colorPalette3[7];

        // colorPalette3[0]=416;
        colorPalette3[0]=416; // Gren OK
        colorPalette3[1]=860; // Blue OFF
        colorPalette3[2]= 400; // Yallow Noisily Strip
        colorPalette3[3]= 807; // Orange Noisily Chamber
        colorPalette3[4]= 616;  // Pink Partly Dead
        colorPalette3[5]= 632; // red Dead
        colorPalette3[6]= 432;

        gStyle->SetPalette(7, colorPalette3);

        c->cd();
        gPad->Update();

        TPaletteAxis *palette;
        palette = (TPaletteAxis*)obj->GetListOfFunctions()->FindObject("palette");
        palette->GetAxis()->SetLabelSize(0);
        //   palette->GetAxis()->SetTitle("OK        OFF     NoisSt    NoisCh     Dead    ");
        palette->GetAxis()->SetTitle("OK        OFF        Nois.St       Nois.Ch   Part.Dead       Dead    Bad.Shape");
        palette->SetTitleOffset(0.3);
        palette->SetTitleSize(0.025);
        //    gPad->Update();
        return;
      }

      if(o.name.find("VStatus_Wheel") != std::string::npos)
      {
        obj->SetMinimum(0.5);
        obj->SetMaximum(3.5);

        int colorPalette4[3];

        colorPalette4[0]=416; // Blue OFF
        colorPalette4[1]=860; // Green ON
        colorPalette4[2]= 400; // Yallow Error
        gStyle->SetPalette(3, colorPalette4);
        return;
      }

      if(o.name.find("NumberOfDigi_Mean_Roll_vs_Sector") != std::string::npos)
      {
        obj->SetMinimum(0.5);
        obj->SetMaximum(10.5);

        int colorPalette5[5];

        colorPalette5[0] = 416; // G
        colorPalette5[1] = 416; // G
        colorPalette5[2] = 400; // Y
        colorPalette5[3] = 807; // O
        colorPalette5[4] = 632; // R

        gStyle->SetPalette(5, colorPalette5);
        return;
      }
    }
};

static RPCRenderPlugin instance;
