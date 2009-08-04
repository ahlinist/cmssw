// $Id: EERenderPlugin.cc,v 1.132 2009/07/17 17:23:17 emanuele Exp $

/*!
  \file EERenderPlugin
  \brief Display Plugin for Quality Histograms
  \author G. Della Ricca
  \author B. Gobbo
  \version $Revision: 1.132 $
  \date $Date: 2009/07/17 17:23:17 $
*/

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

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
#include "TGraph.h"
#include "TLine.h"
#include <iostream>
#include <math.h>

class EERenderPlugin : public DQMRenderPlugin
{
  static const int ixSectorsEE[202];
  static const int iySectorsEE[202];
  static const int inTowersEE[400];

  TH2S* text1;
  TH2C* text3;
  TH2C* text4;
  TH2C* text6;
  TH2C* text7;
  TH2C* text8;
  TH2C* text9;
  TH2C* text10;
  TH2C* text11;
  TH2C* text12;
  TH2C* text13;

  int pCol3[7];
  int pCol4[10];
  int pCol5[10];
  int pCol6[10];

public:
  virtual void initialise( int, char ** )
    {
      float rgb[7][3]   = {{ 1.00,   0.00,   0.00},   { 0.00,   1.00,   0.00},
                           { 1.00,   0.96,   0.00},   { 0.50,   0.00,   0.00},
                           { 0.00,   0.40,   0.00},   { 0.94,   0.78,   0.00},
                           { 1.00,   1.00,   1.00}};

      float rgb2[10][3] = {{ 0.0000, 0.6510, 1.0000}, { 0.0000, 0.6135, 0.9455},
                           { 0.0000, 0.5760, 0.8911}, { 0.0000, 0.5386, 0.8366},
                           { 0.0000, 0.5011, 0.7821}, { 0.0000, 0.4636, 0.7277},
                           { 0.0000, 0.4261, 0.6732}, { 0.0000, 0.3887, 0.6187},
                           { 0.0000, 0.3512, 0.5643}, { 0.0000, 0.3137, 0.5098}};

      for( int i=0; i<6; i++ )
      {
        TColor* color = gROOT->GetColor( 301+i );
        if ( ! color ) color = new TColor( 301+i, 0, 0, 0, "");
        color->SetRGB( rgb[i][0], rgb[i][1], rgb[i][2] );
      }

      for( int i=0; i<10; i++ )
      {
        TColor* color = gROOT->GetColor( 401+i );
        if ( ! color ) color = new TColor( 401+i, 0, 0, 0, "");
        color->SetRGB( rgb2[i][0], rgb2[i][1], rgb2[i][2] );
      }

      for( int i=0; i<10; i++ )
      {
        TColor* color = gROOT->GetColor( 501+i );
        if ( ! color ) color = new TColor( 501+i, 0, 0, 0, "");
        color->SetRGB( rgb2[i][1], 0, 0 );
      }

      for( short i=0; i<7; i++ ) pCol3[i]  = i+301;
      for( short i=0; i<10; i++ ) pCol4[i] = i+401;
      for( short i=0; i<10; i++ ) pCol5[i] = i+501;
      pCol6[0]=2;
      pCol6[1]=10;
      for( short i=2; i<9; i++ ) pCol6[i] = i+1;
      pCol6[9]=1;

      text1 = new TH2S( "ee_text1", "text1", 100, -2., 98., 100, -2., 98.);
      text3 = new TH2C( "ee_text3", "text3", 10, 0,  10,  5,   0,  5 );
      text4 = new TH2C( "ee_text4", "text4",  2, 0,   2,  1,   0,  1 );
      text6 = new TH2C( "ee_text6", "text6", 10, 0., 100., 10, 0., 100. );
      text7 = new TH2C( "ee_text7", "text7", 10, 0., 100., 10, 0., 100. );
      text8 = new TH2C( "ee_text8", "text8", 10, -150., 150., 10, -150., 150. );
      text9 = new TH2C( "ee_text9", "text9", 10, -150., 150., 10, -150., 150. );
      text10 = new TH2C( "ee_text10", "text10", 20, 0., 40., 10, 0., 20. );
      text11 = new TH2C( "ee_text11", "text11", 20, 0., 200., 10, 0., 100. );
      text12 = new TH2C( "ee_text12", "text12", 10, 0., 20., 10, 0., 20. );
      text13 = new TH2C( "ee_text13", "text13", 10, 0., 20., 10, 0., 20. );

      text1->SetMinimum(  0.01 );
      text3->SetMinimum(  0.01 );
      text4->SetMinimum(  0.01 );
      text6->SetMinimum( -9.01 );
      text7->SetMinimum( +0.01 );
      text8->SetMinimum( -9.01 );
      text9->SetMinimum( +0.01 );
      text10->SetMinimum( -9.01 );
      text11->SetMinimum( -9.01 );
      text12->SetMinimum( -9.01 );
      text13->SetMinimum( +0.01 );

      text6->SetMaximum( -0.01 );
      text7->SetMaximum( +9.01 );
      text8->SetMaximum( -0.01 );
      text9->SetMaximum( +9.01 );
      text10->SetMaximum( -0.01 );
      text11->SetMaximum( -0.01 );
      text12->SetMinimum( -9.01 );
      text13->SetMinimum( +0.01 );

      for ( short j=0; j<400; j++ )
      {
        int x = 5*(1 + j%20);
        int y = 5*(1 + j/20);
        text1->SetBinContent(x, y, inTowersEE[j]);
      }

      for ( short i=1; i<=10; i++)
      {
        for ( int j=1; j<=10; j++)
        {
          text6->SetBinContent(i, j, -10);
          text7->SetBinContent(i, j, -10);
          text8->SetBinContent(i, j, -10);
          text9->SetBinContent(i, j, -10);
          text12->SetBinContent(i, j, -10);
          text13->SetBinContent(i, j, -10);
        }
      }

      for( short i=0; i<2; i++ )
      {
        text3->Fill( 2+i*5, 2, i+1+68 );
        text4->Fill( i, 0., i+1+68 );
      }

      text6->SetBinContent(2, 5, -3);
      text6->SetBinContent(2, 7, -2);
      text6->SetBinContent(4, 9, -1);
      text6->SetBinContent(7, 9, -9);
      text6->SetBinContent(9, 7, -8);
      text6->SetBinContent(9, 5, -7);
      text6->SetBinContent(8, 3, -6);
      text6->SetBinContent(6, 2, -5);
      text6->SetBinContent(3, 3, -4);

      text7->SetBinContent(2, 5, +3);
      text7->SetBinContent(2, 7, +2);
      text7->SetBinContent(4, 9, +1);
      text7->SetBinContent(7, 9, +9);
      text7->SetBinContent(9, 7, +8);
      text7->SetBinContent(9, 5, +7);
      text7->SetBinContent(8, 3, +6);
      text7->SetBinContent(5, 2, +5);
      text7->SetBinContent(3, 3, +4);

      text8->SetBinContent(2, 5, -3);
      text8->SetBinContent(2, 7, -2);
      text8->SetBinContent(4, 9, -1);
      text8->SetBinContent(7, 9, -9);
      text8->SetBinContent(9, 7, -8);
      text8->SetBinContent(9, 5, -7);
      text8->SetBinContent(8, 3, -6);
      text8->SetBinContent(6, 2, -5);
      text8->SetBinContent(3, 3, -4);

      text9->SetBinContent(2, 5, +3);
      text9->SetBinContent(2, 7, +2);
      text9->SetBinContent(4, 9, +1);
      text9->SetBinContent(7, 9, +9);
      text9->SetBinContent(9, 7, +8);
      text9->SetBinContent(9, 5, +7);
      text9->SetBinContent(8, 3, +6);
      text9->SetBinContent(5, 2, +5);
      text9->SetBinContent(3, 3, +4);

      text10->SetBinContent(2, 5, -3);
      text10->SetBinContent(2, 7, -2);
      text10->SetBinContent(4, 9, -1);
      text10->SetBinContent(7, 9, -9);
      text10->SetBinContent(9, 7, -8);
      text10->SetBinContent(9, 5, -7);
      text10->SetBinContent(8, 3, -6);
      text10->SetBinContent(6, 2, -5);
      text10->SetBinContent(3, 3, -4);

      text10->SetBinContent(10+2, 5, +3);
      text10->SetBinContent(10+2, 7, +2);
      text10->SetBinContent(10+4, 9, +1);
      text10->SetBinContent(10+7, 9, +9);
      text10->SetBinContent(10+9, 7, +8);
      text10->SetBinContent(10+9, 5, +7);
      text10->SetBinContent(10+8, 3, +6);
      text10->SetBinContent(10+5, 2, +5);
      text10->SetBinContent(10+3, 3, +4);

      text11->SetBinContent(2, 5, -3);
      text11->SetBinContent(2, 7, -2);
      text11->SetBinContent(4, 9, -1);
      text11->SetBinContent(7, 9, -9);
      text11->SetBinContent(9, 7, -8);
      text11->SetBinContent(9, 5, -7);
      text11->SetBinContent(8, 3, -6);
      text11->SetBinContent(6, 2, -5);
      text11->SetBinContent(3, 3, -4);

      text11->SetBinContent(10+2, 5, +3);
      text11->SetBinContent(10+2, 7, +2);
      text11->SetBinContent(10+4, 9, +1);
      text11->SetBinContent(10+7, 9, +9);
      text11->SetBinContent(10+9, 7, +8);
      text11->SetBinContent(10+9, 5, +7);
      text11->SetBinContent(10+8, 3, +6);
      text11->SetBinContent(10+5, 2, +5);
      text11->SetBinContent(10+3, 3, +4);

      text12->SetBinContent(2, 5, -3);
      text12->SetBinContent(2, 7, -2);
      text12->SetBinContent(4, 9, -1);
      text12->SetBinContent(7, 9, -9);
      text12->SetBinContent(9, 7, -8);
      text12->SetBinContent(9, 5, -7);
      text12->SetBinContent(8, 3, -6);
      text12->SetBinContent(6, 2, -5);
      text12->SetBinContent(3, 3, -4);

      text13->SetBinContent(2, 5, +3);
      text13->SetBinContent(2, 7, +2);
      text13->SetBinContent(4, 9, +1);
      text13->SetBinContent(7, 9, +9);
      text13->SetBinContent(9, 7, +8);
      text13->SetBinContent(9, 5, +7);
      text13->SetBinContent(8, 3, +6);
      text13->SetBinContent(5, 2, +5);
      text13->SetBinContent(3, 3, +4);

      text1->SetMarkerSize( 2 );
      text3->SetMarkerSize( 2 );
      text4->SetMarkerSize( 2 );
      text6->SetMarkerSize( 2 );
      text7->SetMarkerSize( 2 );
      text8->SetMarkerSize( 2 );
      text9->SetMarkerSize( 2 );
      text10->SetMarkerSize( 2 );
      text11->SetMarkerSize( 2 );
      text12->SetMarkerSize( 2 );
      text13->SetMarkerSize( 2 );
    }

  virtual bool applies( const DQMNet::CoreObject &o, const VisDQMImgInfo & )
    {
      if( o.name.find( "EcalEndcap/EE" ) != std::string::npos )
        return true;

      if( o.name.find( "EcalEndcap/EcalInfo" ) != std::string::npos )
        return true;

      if( o.name.find( "EcalEndcap/EventInfo" ) != std::string::npos )
        return true;

      if( o.name.find( "EcalEndcap/Run summary/EE" ) != std::string::npos )
        return true;

      if( o.name.find( "EcalEndcap/Run summary/EcalInfo" ) != std::string::npos )
        return true;

      if( o.name.find( "EcalEndcap/Run summary/EventInfo" ) != std::string::npos )
        return true;

      return false;
    }

  virtual void preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo &r )
    {
      c->cd();

      gStyle->Reset("Default");

      gStyle->SetCanvasColor(10);
      gStyle->SetPadColor(10);
      gStyle->SetFillColor(10);
      gStyle->SetFrameFillColor(10);
      gStyle->SetStatColor(10);
      gStyle->SetTitleFillColor(10);

      TGaxis::SetMaxDigits(4);

      gStyle->SetOptTitle(kTRUE);
      gStyle->SetTitleBorderSize(0);

      gStyle->SetOptStat(kFALSE);
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
  void preDrawTProfile2D( TCanvas *c, const DQMNet::CoreObject &o )
    {
      TProfile2D* obj = dynamic_cast<TProfile2D*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      int nbx = obj->GetNbinsX();
      int nby = obj->GetNbinsY();

      gStyle->SetPaintTextFormat();

      gStyle->SetOptStat(kFALSE);
      obj->SetStats(kFALSE);
      gPad->SetLogy(kFALSE);

      if( name.find( "EELT shape" ) != std::string::npos )
      {
        c->SetTheta(+30.);
        c->SetPhi(-60.);
        obj->GetXaxis()->SetTitleOffset(2.5);
        obj->GetYaxis()->SetTitleOffset(3.0);
        obj->GetZaxis()->SetTitleOffset(1.3);
        obj->SetOption("lego");
        return;
      }

      if( name.find( "EELDT shape" ) != std::string::npos )
      {
        c->SetTheta(+30.);
        c->SetPhi(-60.);
        obj->GetXaxis()->SetTitleOffset(2.5);
        obj->GetYaxis()->SetTitleOffset(3.0);
        obj->GetZaxis()->SetTitleOffset(1.3);
        obj->SetOption("lego");
        return;
      }

      if( name.find( "EETPT shape" ) != std::string::npos )
      {
        c->SetTheta(+30.);
        c->SetPhi(-60.);
        obj->GetXaxis()->SetTitleOffset(2.5);
        obj->GetYaxis()->SetTitleOffset(3.0);
        obj->GetZaxis()->SetTitleOffset(1.3);
        obj->SetOption("lego");
        return;
      }

      if( name.find( "EECLT" ) != std::string::npos )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(10, kFALSE);
        obj->GetYaxis()->SetNdivisions(10, kFALSE);
        obj->SetMinimum(0.0);
        gStyle->SetPalette(10, pCol4);
        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( nbx == 50 && nby == 50 )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(10);
        obj->GetYaxis()->SetNdivisions(10);
      }

      if( nbx == 100 && nby == 100 )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(10);
        obj->GetYaxis()->SetNdivisions(10);
      }

      if( nbx == 20 && nby == 20 &&
          name.find( "EESRT" ) != std::string::npos )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(10);
        obj->GetYaxis()->SetNdivisions(10);
      }

      obj->SetMinimum(0.0);
      gStyle->SetPalette(10, pCol4);
      obj->SetOption("colz");
      gPad->SetRightMargin(0.15);
    }

  void preDrawTProfile( TCanvas *, const DQMNet::CoreObject &o )
    {
      TProfile* obj = dynamic_cast<TProfile*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      gStyle->SetPaintTextFormat();

      gStyle->SetOptStat("e");
      obj->SetStats(kTRUE);
      gPad->SetLogy(kFALSE);
      obj->SetMinimum(0.0);

      if( name.find( "EEMM digi number profile" ) != std::string::npos ||
          name.find( "EELT laser L1" ) != std::string::npos ||
          ( name.find( "EETPT test pulse amplitude" ) != std::string::npos && name.find( "summary" ) != std::string::npos ) ||
          name.find( "EEPOT pedestal G12 mean" ) != std::string::npos || name.find( "EEPOT pedestal G12 rms" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.2);
        obj->GetXaxis()->LabelsOption("v");
      }

      if( name.find( "EEMM hit number profile" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.2);
        obj->GetXaxis()->LabelsOption("v");
      }

      if( name.find( "EEMM TP digi number profile" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.2);
        obj->GetXaxis()->LabelsOption("v");
      }

      if( name.find( "EESRT DCC event size" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.2);
        obj->GetXaxis()->LabelsOption("v");
      }
    }

  void preDrawTH3F( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH3F* obj = dynamic_cast<TH3F*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      gStyle->SetPaintTextFormat();

      gStyle->SetOptStat(kFALSE);
      obj->SetStats(kFALSE);
      gPad->SetLogy(kFALSE);
    }

  void preDrawTH2F( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      int nbx = obj->GetNbinsX();
      int nby = obj->GetNbinsY();

      gStyle->SetPaintTextFormat();

      gStyle->SetOptStat(kFALSE);
      obj->SetStats(kFALSE);
      gPad->SetLogy(kFALSE);

      if( name.find( "EESRT" ) != std::string::npos ||
          name.find( "EECLT SC energy vs seed crystal energy" ) != std::string::npos )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        if( name.find( "EESRT event size vs DCC" ) != std::string::npos )
        {
          gPad->SetLogy(kTRUE);
          gPad->SetBottomMargin(0.2);
          obj->GetXaxis()->SetNdivisions(18, kFALSE);
          obj->GetYaxis()->SetNdivisions(2, kFALSE);
          obj->GetXaxis()->LabelsOption("v");
          obj->GetYaxis()->SetRangeUser(0.1, 0.608*132);
        }
        obj->SetMinimum(0.0);

        std::string zAxisTitle(obj->GetZaxis()->GetTitle());

        if ( zAxisTitle.find("rate") != std::string::npos ) obj->SetMaximum(1.0);

        gStyle->SetPalette(1);
        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "EECLT" ) != std::string::npos )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(10, kFALSE);
        obj->GetYaxis()->SetNdivisions(10, kFALSE);
        obj->SetMinimum(0.0);
        gStyle->SetPalette(10, pCol4);
        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "EETMT timing vs amplitude" ) != std::string::npos )
      {
        if ( obj->GetMaximum() > 0. ) gPad->SetLogz(kTRUE);
        obj->SetMinimum(0.0);
        gStyle->SetPalette(10, pCol4);
        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        return;
      }

      if( nbx == 50 && nby == 50 )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(10);
        obj->GetYaxis()->SetNdivisions(10);
      }

      if( nbx == 20 && nby == 20 )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(20);
        obj->GetYaxis()->SetNdivisions(20);
      }

      if( nbx == 20 && nby == 20 &&
          name.find( "EESRT" ) != std::string::npos )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(10);
        obj->GetYaxis()->SetNdivisions(10);
      }

      if( nbx == 10 && ( nby == 1 || nby == 5 ) )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(10);
        obj->GetYaxis()->SetNdivisions(1);
      }

      if( nbx == 2 && nby == 1 )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(2);
        obj->GetYaxis()->SetNdivisions(1);
      }

      if( nbx == 100 && nby == 100 )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(10);
        obj->GetYaxis()->SetNdivisions(10);
      }

      if( nbx == 90 && nby == 20 )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(18);
        obj->GetYaxis()->SetNdivisions(2);
      }

      if( nbx == 40 && nby == 20 )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(20);
        obj->GetYaxis()->SetNdivisions(10);
      }

      if( nbx == 200 && nby == 100 )
      {
        gPad->SetGridx();
        gPad->SetGridy();
        obj->GetXaxis()->SetNdivisions(20);
        obj->GetYaxis()->SetNdivisions(10);
      }

      if( name.find( "reportSummaryMap" ) != std::string::npos )
      {
        dqm::utils::reportSummaryMapPalette(obj);
        obj->SetTitle("EcalEndcap Report Summary Map");
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "DAQSummaryMap" ) != std::string::npos )
      {
        dqm::utils::reportSummaryMapPalette(obj);
        obj->SetTitle("EcalEndcap DAQ Summary Map");
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "DCSSummaryMap" ) != std::string::npos )
      {
        dqm::utils::reportSummaryMapPalette(obj);
        obj->SetTitle("EcalEndcap DCS Summary Map");
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "CertificationSummaryMap" ) != std::string::npos )
      {
        dqm::utils::reportSummaryMapPalette(obj);
        obj->SetTitle("EcalEndcap Data Certification Summary Map");
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "EEIT" ) != std::string::npos &&
          name.find( "quality" ) ==std::string::npos )
      {
        obj->SetMinimum(0.0);
        gStyle->SetPalette(10, pCol5);
        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "EETTT" ) != std::string::npos &&
          name.find( "quality" ) ==std::string::npos )
      {
        if( name.find( "EBTTT Trigger Primitives Timing" ) != std::string::npos )
        {
          obj->SetMinimum(-1.0);
          obj->SetMaximum(6.0);
        }
        else
        {
          obj->SetMinimum(0.0);
        }

        if( name.find( "Error" ) ==std::string::npos )
        {
          if( name.find( "EBTTT Trigger Primitives Timing" ) != std::string::npos )
          {
            gStyle->SetPalette(7, pCol6);
          }
          else
          {
            gStyle->SetPalette(10, pCol4);
          }
        }
        else
        {
          gStyle->SetPalette(10, pCol5);
        }

        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "G12 RMS map" ) != std::string::npos )
      {
        obj->SetMinimum(1.0);
        obj->SetMaximum(4.0);
        gStyle->SetPalette(1);
        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "EEOT" ) != std::string::npos )
      {
        obj->SetMinimum(0.0);
        gStyle->SetPalette(10, pCol4);
        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "EESFT" ) != std::string::npos &&
          name.find( "summary" ) ==std::string::npos )
      {
        obj->SetMinimum(0.0);
        gStyle->SetPalette(10, pCol5);
        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "EESRT" ) != std::string::npos )
      {
        obj->SetMinimum(0.0);
        gStyle->SetPalette(10, pCol4);
        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "EECT" ) != std::string::npos )
      {
        obj->SetMinimum(0.0);
        gStyle->SetPalette(10, pCol4);
        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "summary" ) != std::string::npos &&
          name.find( "Trigger Primitives Timing" ) == std::string::npos )
      {
        obj->SetMinimum(-0.00000001);
        obj->SetMaximum(7.0);
        gStyle->SetPalette(7, pCol3);
        obj->SetOption("col");
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "quality" ) != std::string::npos )
      {
        obj->SetMinimum(-0.00000001);
        obj->SetMaximum(7.0);
        gStyle->SetPalette(7, pCol3);
        obj->SetOption("col");
        gStyle->SetPaintTextFormat("+g");
        return;
      }

      if( name.find( "EEMM event" ) != std::string::npos )
      {
        obj->SetMinimum(0.0);
        gStyle->SetPalette(10, pCol4);
        obj->SetOption("colz");
        gPad->SetRightMargin(0.15);
        gStyle->SetPaintTextFormat("+g");
        return;
      }
    }

  void preDrawTH1F( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      int nbx = obj->GetNbinsX();

      gStyle->SetPaintTextFormat();

      gStyle->SetOptStat("euomr");
      obj->SetStats(kTRUE);
      gPad->SetLogy(kFALSE);

      if ( obj->GetMaximum() > 0. ) gPad->SetLogy(kTRUE);
      if ( nbx == 10 || nbx == 850 ) {
        gPad->SetLogy(kFALSE);
        gStyle->SetOptStat("e");
        return;
      }

      if( name.find( "EVTTYPE" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.4);
        obj->GetXaxis()->LabelsOption("v");
      }

      if( name.find( "EEMM DCC" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.2);
        obj->GetXaxis()->LabelsOption("v");
      }

      if( name.find( "EEIT DCC" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.2);
        obj->GetXaxis()->LabelsOption("v");
      }

      if( name.find( "front-end status bits" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.25);
        obj->GetXaxis()->LabelsOption("v");
      }

      if( name.find( "EERDT" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.2);
        obj->GetXaxis()->LabelsOption("v");
      }

      if( name.find( "EERDT event type" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.4);
        obj->GetXaxis()->LabelsOption("v");
      }

      if( name.find( "front-end status errors summary" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.2);
        obj->GetXaxis()->LabelsOption("v");
      }

      if( name.find( "quality errors summary" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.2);
        obj->GetXaxis()->LabelsOption("v");
      }

      if( name.find( "EEOT digi occupancy summary 1D" ) != std::string::npos )
      {
        gPad->SetBottomMargin(0.2);
        obj->GetXaxis()->LabelsOption("v");
      }
    }

  void postDrawTProfile2D( TCanvas *c, const DQMNet::CoreObject &o )
    {
      TProfile2D* obj = dynamic_cast<TProfile2D*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      if( name.find( "EELT shape" ) != std::string::npos )
      {
        return;
      }

      if( name.find( "EELDT shape" ) != std::string::npos )
      {
        return;
      }

      if( name.find( "EETPT shape" ) != std::string::npos )
      {
        return;
      }

      int nbx = obj->GetNbinsX();
      int nby = obj->GetNbinsY();

      c->SetBit(TGraph::kClipFrame);
      TLine l;
      l.SetLineWidth(1);
      for ( int i=0; i<201; i=i+1)
      {
        if ( (ixSectorsEE[i]!=0 || iySectorsEE[i]!=0) && (ixSectorsEE[i+1]!=0 || iySectorsEE[i+1]!=0) )
        {
          if( name.find( "EESRT" ) != std::string::npos && nbx == 20 && nby == 20 )
          {
            l.DrawLine(0.2*ixSectorsEE[i], 0.2*iySectorsEE[i], 0.2*ixSectorsEE[i+1], 0.2*iySectorsEE[i+1]);
          }
          else if( name.find( "EECLT" ) != std::string::npos &&
                   name.find( "seed" ) == std::string::npos )
          {
            l.DrawLine(3.0*(ixSectorsEE[i]-50), 3.0*(iySectorsEE[i]-50), 3.0*(ixSectorsEE[i+1]-50), 3.0*(iySectorsEE[i+1]-50));
          }
          else if ( name.find( "EECLT SC energy vs seed crystal energy" ) == std::string::npos )
          {
            l.DrawLine(ixSectorsEE[i], iySectorsEE[i], ixSectorsEE[i+1], iySectorsEE[i+1]);
          }
        }
      }

      if( name.find( "EECLT" ) != std::string::npos &&
          name.find( "seed" ) == std::string::npos )
      {
        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text8->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text8->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text8->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text8->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text8->GetXaxis()->SetRange(x1, x2);
          text8->GetYaxis()->SetRange(y1, y2);
          text8->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text9->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text9->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text9->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text9->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text9->GetXaxis()->SetRange(x1, x2);
          text9->GetYaxis()->SetRange(y1, y2);
          text9->Draw("text,same");
        }
        return;
      }

      if ( name.find( "EBCLT SC energy vs seed crystal energy" ) != std::string::npos )
        return;

      if( name.find( "seed" ) != std::string::npos )
      {
        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text6->GetXaxis()->SetRange(x1, x2);
          text6->GetYaxis()->SetRange(y1, y2);
          text6->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text7->GetXaxis()->SetRange(x1, x2);
          text7->GetYaxis()->SetRange(y1, y2);
          text7->Draw("text,same");
        }
        return;
      }

      if( name.find( "EESRT") != std::string::npos )
      {
        if( nbx == 100 && nby == 100 )
        {
          if( name.find( "EE -" ) != std::string::npos )
          {
            int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
            int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
            int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
            int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
            text6->GetXaxis()->SetRange(x1, x2);
            text6->GetYaxis()->SetRange(y1, y2);
            text6->Draw("text,same");
          }

          if( name.find( "EE +" ) != std::string::npos )
          {
            int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
            int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
            int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
            int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
            text7->GetXaxis()->SetRange(x1, x2);
            text7->GetYaxis()->SetRange(y1, y2);
            text7->Draw("text,same");
          }
        }
        else if( nbx == 20 && nby == 20 )
        {
          if( name.find( "EE -" ) != std::string::npos )
          {
            int x1 = text12->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
            int x2 = text12->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
            int y1 = text12->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
            int y2 = text12->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
            text12->GetXaxis()->SetRange(x1, x2);
            text12->GetYaxis()->SetRange(y1, y2);
            text12->Draw("text,same");
          }

          if( name.find( "EE +" ) != std::string::npos )
          {
            int x1 = text13->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
            int x2 = text13->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
            int y1 = text13->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
            int y2 = text13->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
            text13->GetXaxis()->SetRange(x1, x2);
            text13->GetYaxis()->SetRange(y1, y2);
            text13->Draw("text,same");
          }
        }
        return;
      }

      int x1 = text1->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
      int x2 = text1->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
      int y1 = text1->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
      int y2 = text1->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
      text1->GetXaxis()->SetRange(x1, x2);
      text1->GetYaxis()->SetRange(y1, y2);
      text1->Draw("text,same");
    }

  void postDrawTH3F( TCanvas *, const DQMNet::CoreObject & )
    {
    }

  void postDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      if( name.find( "EETTT Et map" ) != std::string::npos )
        return;

      if( name.find( "EETMT timing vs amplitude" ) != std::string::npos )
        return;

      int nbx = obj->GetNbinsX();
      int nby = obj->GetNbinsY();

      c->SetBit(TGraph::kClipFrame);
      TLine l;
      l.SetLineWidth(1);
      for ( int i=0; i<201; i=i+1)
      {
        if ( (ixSectorsEE[i]!=0 || iySectorsEE[i]!=0) && (ixSectorsEE[i+1]!=0 || iySectorsEE[i+1]!=0) )
        {
          if( name.find( "reportSummaryMap") != std::string::npos && nbx == 40 && nby == 20 )
          {
            l.DrawLine(0.2*ixSectorsEE[i], 0.2*iySectorsEE[i], 0.2*ixSectorsEE[i+1], 0.2*iySectorsEE[i+1]);
            l.DrawLine(20+0.2*ixSectorsEE[i], 0.2*iySectorsEE[i], 20+0.2*ixSectorsEE[i+1], 0.2*iySectorsEE[i+1]);
          }
          else if( (name.find( "reportSummaryMap") != std::string::npos && nbx == 200 && nby == 100) ||
                   name.find( "DAQSummaryMap") != std::string::npos ||
                   name.find( "DCSSummaryMap") != std::string::npos ||
                   name.find( "CertificationSummaryMap") != std::string::npos )
          {
            l.DrawLine(ixSectorsEE[i], iySectorsEE[i], ixSectorsEE[i+1], iySectorsEE[i+1]);
            l.DrawLine(100+ixSectorsEE[i], iySectorsEE[i], 100+ixSectorsEE[i+1], iySectorsEE[i+1]);
          }
          else if( name.find( "EECLT" ) != std::string::npos &&
                   name.find( "seed" ) == std::string::npos )
          {
            l.DrawLine(3.0*(ixSectorsEE[i]-50), 3.0*(iySectorsEE[i]-50), 3.0*(ixSectorsEE[i+1]-50), 3.0*(iySectorsEE[i+1]-50));
          }
          else if( name.find( "EESRT" ) != std::string::npos && nbx == 20 && nby == 20 )
          {
            l.DrawLine(0.2*ixSectorsEE[i], 0.2*iySectorsEE[i], 0.2*ixSectorsEE[i+1], 0.2*iySectorsEE[i+1]);
          }
          else if( name.find( " PN " ) == std::string::npos &&
                   name.find( "EESRT event size vs DCC" ) == std::string::npos )
          {
            l.DrawLine(ixSectorsEE[i], iySectorsEE[i], ixSectorsEE[i+1], iySectorsEE[i+1]);
          }
        }
      }

      if( name.find( "EECLT" ) != std::string::npos &&
          name.find( "seed" ) == std::string::npos )
      {
        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text8->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text8->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text8->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text8->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text8->GetXaxis()->SetRange(x1, x2);
          text8->GetYaxis()->SetRange(y1, y2);
          text8->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text9->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text9->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text9->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text9->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text9->GetXaxis()->SetRange(x1, x2);
          text9->GetYaxis()->SetRange(y1, y2);
          text9->Draw("text,same");
        }
        return;
      }

      if( name.find( "seed" ) != std::string::npos )
      {
        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text6->GetXaxis()->SetRange(x1, x2);
          text6->GetYaxis()->SetRange(y1, y2);
          text6->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text7->GetXaxis()->SetRange(x1, x2);
          text7->GetYaxis()->SetRange(y1, y2);
          text7->Draw("text,same");
        }
        return;
      }

      if( name.find( "EEOT MEM" ) != std::string::npos )
      {
        int x1 = text3->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
        int x2 = text3->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
        int y1 = text3->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
        int y2 = text3->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
        text3->GetXaxis()->SetRange(x1, x2);
        text3->GetYaxis()->SetRange(y1, y2);
        text3->Draw("text,same");
        return;
      }

      if( name.find( "EEOT digi" ) != std::string::npos )
      {
        if( nbx == 50 && nby == 50 )
        {
          int x1 = text1->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text1->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text1->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text1->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text1->GetXaxis()->SetRange(x1, x2);
          text1->GetYaxis()->SetRange(y1, y2);
          text1->Draw("text,same");
          return;
        }

        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text6->GetXaxis()->SetRange(x1, x2);
          text6->GetYaxis()->SetRange(y1, y2);
          text6->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text7->GetXaxis()->SetRange(x1, x2);
          text7->GetYaxis()->SetRange(y1, y2);
          text7->Draw("text,same");
        }
        return;
      }

      if( name.find( "EEOT rec hit" ) != std::string::npos )
      {
        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text6->GetXaxis()->SetRange(x1, x2);
          text6->GetYaxis()->SetRange(y1, y2);
          text6->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text7->GetXaxis()->SetRange(x1, x2);
          text7->GetYaxis()->SetRange(y1, y2);
          text7->Draw("text,same");
        }
        return;
      }

      if( name.find( "EEOT TP digi" ) != std::string::npos )
      {
        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text6->GetXaxis()->SetRange(x1, x2);
          text6->GetYaxis()->SetRange(y1, y2);
          text6->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text7->GetXaxis()->SetRange(x1, x2);
          text7->GetYaxis()->SetRange(y1, y2);
          text7->Draw("text,same");
        }
        return;
      }

      if( name.find( "EEOT test pulse digi" ) != std::string::npos )
      {
        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text6->GetXaxis()->SetRange(x1, x2);
          text6->GetYaxis()->SetRange(y1, y2);
          text6->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text7->GetXaxis()->SetRange(x1, x2);
          text7->GetYaxis()->SetRange(y1, y2);
          text7->Draw("text,same");
        }
        return;
      }

      if( name.find( "EEOT laser digi" ) != std::string::npos )
      {
        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text6->GetXaxis()->SetRange(x1, x2);
          text6->GetYaxis()->SetRange(y1, y2);
          text6->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text7->GetXaxis()->SetRange(x1, x2);
          text7->GetYaxis()->SetRange(y1, y2);
          text7->Draw("text,same");
        }
        return;
      }

      if( name.find( "EEOT led digi" ) != std::string::npos )
      {
        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text6->GetXaxis()->SetRange(x1, x2);
          text6->GetYaxis()->SetRange(y1, y2);
          text6->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text7->GetXaxis()->SetRange(x1, x2);
          text7->GetYaxis()->SetRange(y1, y2);
          text7->Draw("text,same");
        }
        return;
      }

      if( name.find( "EEOT pedestal digi" ) != std::string::npos ||
          name.find( "G12 RMS map" ) != std::string::npos )
      {
        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text6->GetXaxis()->SetRange(x1, x2);
          text6->GetYaxis()->SetRange(y1, y2);
          text6->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text7->GetXaxis()->SetRange(x1, x2);
          text7->GetYaxis()->SetRange(y1, y2);
          text7->Draw("text,same");
        }
        return;
      }

      if( name.find( "EESRT") != std::string::npos )
      {
        if( nbx == 100 && nby == 100 )
        {
          if( name.find( "EE -" ) != std::string::npos )
          {
            int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
            int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
            int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
            int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
            text6->GetXaxis()->SetRange(x1, x2);
            text6->GetYaxis()->SetRange(y1, y2);
            text6->Draw("text,same");
          }

          if( name.find( "EE +" ) != std::string::npos )
          {
            int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
            int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
            int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
            int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
            text7->GetXaxis()->SetRange(x1, x2);
            text7->GetYaxis()->SetRange(y1, y2);
            text7->Draw("text,same");
          }
        }
        else if( nbx == 20 && nby == 20 )
        {
          if( name.find( "EE -" ) != std::string::npos )
          {
            int x1 = text12->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
            int x2 = text12->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
            int y1 = text12->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
            int y2 = text12->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
            text12->GetXaxis()->SetRange(x1, x2);
            text12->GetYaxis()->SetRange(y1, y2);
            text12->Draw("text,same");
          }

          if( name.find( "EE +" ) != std::string::npos )
          {
            int x1 = text13->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
            int x2 = text13->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
            int y1 = text13->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
            int y2 = text13->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
            text13->GetXaxis()->SetRange(x1, x2);
            text13->GetYaxis()->SetRange(y1, y2);
            text13->Draw("text,same");
          }
        }
        return;
      }

      if( name.find( "reportSummaryMap" ) != std::string::npos && nbx == 40 && nby == 20 )
      {
        int x1 = text10->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
        int x2 = text10->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
        int y1 = text10->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
        int y2 = text10->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
        text10->GetXaxis()->SetRange(x1, x2);
        text10->GetYaxis()->SetRange(y1, y2);
        text10->Draw("text,same");
        return;
      }

      if( (name.find( "reportSummaryMap" ) != std::string::npos && nbx == 200 && nby == 100) ||
          name.find( "DAQSummaryMap" ) != std::string::npos ||
          name.find( "DCSSummaryMap" ) != std::string::npos ||
          name.find( "CertificationSummaryMap" ) != std::string::npos )
      {
        int x1 = text11->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
        int x2 = text11->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
        int y1 = text11->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
        int y2 = text11->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
        text11->GetXaxis()->SetRange(x1, x2);
        text11->GetYaxis()->SetRange(y1, y2);
        text11->Draw("text,same");
        return;
      }

      if( nbx == 50 && nby == 50 )
      {
        int x1 = text1->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
        int x2 = text1->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
        int y1 = text1->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
        int y2 = text1->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
        text1->GetXaxis()->SetRange(x1, x2);
        text1->GetYaxis()->SetRange(y1, y2);
        text1->Draw("text,same");
        return;
      }

      if( nbx == 10 && ( nby == 1 || nby == 5 ) )
      {
        int x1 = text3->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
        int x2 = text3->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
        int y1 = text3->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
        int y2 = text3->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
        text3->GetXaxis()->SetRange(x1, x2);
        text3->GetYaxis()->SetRange(y1, y2);
        text3->Draw("text,same");
        return;
      }

      if( nbx == 2 && nby == 1 )
      {
        int x1 = text4->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
        int x2 = text4->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
        int y1 = text4->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
        int y2 = text4->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
        text4->GetXaxis()->SetRange(x1, x2);
        text4->GetYaxis()->SetRange(y1, y2);
        text4->Draw("text,same");
        return;
      }

      if( name.find( "summary" ) != std::string::npos )
      {
        if( name.find( "EE -" ) != std::string::npos )
        {
          int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text6->GetXaxis()->SetRange(x1, x2);
          text6->GetYaxis()->SetRange(y1, y2);
          text6->Draw("text,same");
        }

        if( name.find( "EE +" ) != std::string::npos )
        {
          int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
          int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
          int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
          int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
          text7->GetXaxis()->SetRange(x1, x2);
          text7->GetYaxis()->SetRange(y1, y2);
          text7->Draw("text,same");
        }
        return;
      }
    }

  void postDrawTH1F( TCanvas *, const DQMNet::CoreObject & )
    {
    }
};

const int EERenderPlugin::ixSectorsEE[202] = {
  61, 61, 60, 60, 59, 59, 58, 58, 57, 57, 55, 55, 45, 45, 43, 43, 42, 42,
  41, 41, 40, 40, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 45, 45, 55, 55,
  57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 0, 100,100, 97, 97, 95, 95, 92,
  92, 87, 87, 85, 85, 80, 80, 75, 75, 65, 65, 60, 60, 40, 40, 35, 35, 25,
  25, 20, 20, 15, 15, 13, 13,  8,  8,  5,  5,  3,  3,  0,  0,  3,  3,  5,
   5,  8,  8, 13, 13, 15, 15, 20, 20, 25, 25, 35, 35, 40, 40, 60, 60, 65,
  65, 75, 75, 80, 80, 85, 85, 87, 87, 92, 92, 95, 95, 97, 97,100,100,  0,
  61, 65, 65, 70, 70, 80, 80, 90, 90, 92,  0, 61, 65, 65, 90, 90, 97,  0,
  57, 60, 60, 65, 65, 70, 70, 75, 75, 80, 80,  0, 50, 50,  0, 43, 40, 40,
  35, 35, 30, 30, 25, 25, 20, 20,  0, 39, 35, 35, 10, 10,  3,  0, 39, 35,
  35, 30, 30, 20, 20, 10, 10,  8,  0, 45, 45, 40, 40, 35, 35,  0, 55, 55,
  60, 60, 65, 65
};

const int EERenderPlugin::iySectorsEE[202] = {
  50, 55, 55, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 60, 60, 59, 59, 58,
  58, 57, 57, 55, 55, 45, 45, 43, 43, 42, 42, 41, 41, 40, 40, 39, 39, 40,
  40, 41, 41, 42, 42, 43, 43, 45, 45, 50,  0, 50, 60, 60, 65, 65, 75, 75,
  80, 80, 85, 85, 87, 87, 92, 92, 95, 95, 97, 97,100,100, 97, 97, 95, 95,
  92, 92, 87, 87, 85, 85, 80, 80, 75, 75, 65, 65, 60, 60, 40, 40, 35, 35,
  25, 25, 20, 20, 15, 15, 13, 13,  8,  8,  5,  5,  3,  3,  0,  0,  3,  3,
   5,  5,  8,  8, 13, 13, 15, 15, 20, 20, 25, 25, 35, 35, 40, 40, 50,  0,
  45, 45, 40, 40, 35, 35, 30, 30, 25, 25,  0, 50, 50, 55, 55, 60, 60,  0,
  60, 60, 65, 65, 70, 70, 75, 75, 85, 85, 87,  0, 61,100,  0, 60, 60, 65,
  65, 70, 70, 75, 75, 85, 85, 87,  0, 50, 50, 55, 55, 60, 60,  0, 45, 45,
  40, 40, 35, 35, 30, 30, 25, 25,  0, 39, 30, 30, 15, 15,  5,  0, 39, 30,
  30, 15, 15,  5
};

const int EERenderPlugin::inTowersEE[400] = {
   0,  0,  0,  0,  0,  0,  0, 27, 37, 41, 17, 13,  3,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0, 21, 31, 29, 26, 36, 40, 16, 12,  2, 29, 31, 21,
   0,  0,  0,  0,  0,  0,  0, 21, 27, 30, 28, 25, 35, 39, 15, 11,  1, 28,
  30, 27, 21,  0,  0,  0,  0,  0, 14, 26, 25, 24, 23, 22, 34, 38, 14, 10,
  22, 23, 24, 25, 26, 14,  0,  0,  0, 14, 20, 19, 18, 17, 16, 15, 29, 33,
   9,  5, 15, 16, 17, 18, 19, 20, 14,  0,  0, 33, 13, 12, 11, 10,  9,  8,
  28, 32,  8,  4,  8,  9, 10, 11, 12, 13, 33,  0,  0, 30, 32, 31,  7,  6,
   5,  4, 33, 31,  7, 33,  4,  5,  6,  7, 31, 32, 30,  0, 34, 29, 28, 27,
  26, 25,  3,  2, 32, 30,  6, 32,  2,  3, 25, 26, 27, 28, 29, 34, 24, 23,
  22, 21, 20, 19, 18,  1, 21, 14, 21, 14,  1, 18, 19, 20, 21, 22, 23, 24,
  17, 16, 15, 14, 13, 12, 11, 10,  0,  0,  0,  0, 10, 11, 12, 13, 14, 15,
  16, 17,  9,  8,  7,  6,  5,  4,  3, 32,  0,  0,  0,  0, 32,  3,  4,  5,
   6,  7,  8,  9,  2,  1, 31, 30, 29, 28, 27, 26, 25,  3, 25,  3, 26, 27,
  28, 29, 30, 31,  1,  2, 25, 24, 23, 22, 21, 20, 19, 18, 16, 12, 12, 16,
  18, 19, 20, 21, 22, 23, 24, 25,  0, 17, 16, 15, 14, 13, 12, 33, 15, 11,
  11, 15, 33, 12, 13, 14, 15, 16, 17,  0,  0, 11, 10,  9,  8,  7, 32, 31,
  14, 10, 10, 14, 31, 32,  7,  8,  9, 10, 11,  0,  0, 25,  6,  5,  4, 29,
  28, 27, 13,  9,  9, 13, 27, 28, 29,  4,  5,  6, 25,  0,  0,  0,  3,  2,
   1, 26, 25, 24,  8,  4,  4,  8, 24, 25, 26,  1,  2,  3,  0,  0,  0,  0,
   0,  3, 23, 22, 21, 20,  7,  3,  3,  7, 20, 21, 22, 23,  3,  0,  0,  0,
   0,  0,  0,  0, 30, 19, 18, 17,  6,  2,  2,  6, 17, 18, 19, 30,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0, 30,  5,  1,  1,  5, 30,  0,  0,  0,
   0,  0,  0,  0 };

static EERenderPlugin instance;
