// $Id: EBRenderPlugin.cc,v 1.5 2007/10/11 06:19:27 dellaric Exp $

/*!
  \file EBRenderPlugin
  \brief Display Plugin for Quality Histograms
  \author G. Della Ricca
  \author B. Gobbo 
  \version $Revision: 1.5 $
  \date $Date: 2007/10/11 06:19:27 $
*/

#include <TProfile2D.h>

#include <TStyle.h>
#include <TCanvas.h>
#include <TColor.h>

#include "DQMServices/Core/interface/MonitorElement.h"

#include "DQM/EcalCommon/interface/ColorPalette.h"
#include <DQM/EcalCommon/interface/Numbers.h>
#include "EBRenderPlugin.h"

static bool  first = true;
static TH2C* t1 = new TH2C( "eb_text1", "text1", 85, 0,  85, 20,   0, 20 );
static TH2C* t2 = new TH2C( "eb_text2", "text2", 17, 0,  17,  4,   0,  4 );
static TH2C* t3 = new TH2C( "eb_text3", "text3", 10, 0,  10,  5,   0,  5 );
static TH2C* t4 = new TH2C( "eb_text4", "text4",  2, 0,   2,  1,   0,  1 );
static TH2C* t6 = new TH2C( "eb_text6", "text6", 18, 0, 360,  2, -85, 85 );
static TH2C* t7 = new TH2C( "eb_text7", "text7", 18, -M_PI*(9+1.5)/9, M_PI*(9-1.5)/9, 2, -1.479, 1.479);

static TGaxis* ax1 = new TGaxis(-M_PI*(9+1.5)/9, -1.479, M_PI*(9-1.5)/9, -1.479, -M_PI*(9+1.5)/9, M_PI*(9-1.5)/9, 40306, "N");

void EBRenderPlugin::initialise( int argc, char **argv, DaqMonitorBEInterface *bei ) {

  if( ! first ) return;

  first = false;

  for( short i=0; i<6; i++ ) {
    TColor* color;
    if( ! gROOT->GetColor( 301+i )) {
      color = new TColor( 301+i, ecdqm::rgb[i][0], ecdqm::rgb[i][1], ecdqm::rgb[i][2], "" );
    }
    else {
      color = gROOT->GetColor( 301+i );
      color->SetRGB( ecdqm::rgb[i][0], ecdqm::rgb[i][1], ecdqm::rgb[i][2] );
    }
  }
  for( short i=0; i<6; i++ ) pCol3[i]  = i+301;

  for( short i=0; i<10; i++ ) {
    TColor* color;
    if( ! gROOT->GetColor( 401+i )) {
      color = new TColor( 401+i, ecdqm::rgb2[i][0], ecdqm::rgb2[i][1], ecdqm::rgb2[i][2], "" );
    }
    else {
      color = gROOT->GetColor( 401+i );
      color->SetRGB( ecdqm::rgb2[i][0], ecdqm::rgb2[i][1], ecdqm::rgb2[i][2] );
    }
  }
  for( short i=0; i<10; i++ ) pCol4[i] = i+401;

  text1 = t1;
  text2 = t2;
  text3 = t3;
  text4 = t4;
  text6 = t6;
  text7 = t7;

  text1->SetMinimum(   0.1 );
  text2->SetMinimum(   0.1 );
  text3->SetMinimum(   0.1 );
  text4->SetMinimum(   0.1 );
  text6->SetMinimum( -18.01 );
  text7->SetMinimum( -18.01 );

  for( short i=0; i<68; i++ ) {
    text1->Fill( 2+(i/4)*5, 2+(i%4)*5, i+1 );
    text2->Fill( i/4, i%4, i+1 );
  }
  for( short i=0; i<2; i++ ) {
    text3->Fill( 2+i*5, 2, i+1+68 );
    text4->Fill( i, 0., i+1+68 );
  }
  for ( short i=0; i<36; i++ ) {
    int x = 1 + i%18;
    int y = 1 + i/18;
    text6->SetBinContent(x, y, Numbers::iEB(i+1));
  }
  for ( short i=0; i<36; i++ ) {
    int x = 1 + i%18;
    int y = 2 - i/18;
    int z = x + 8;
    if ( z > 18 ) z = z - 18;
    if ( y == 1 ) {
      text7->SetBinContent(x, y, -z);
    } else {
      text7->SetBinContent(x, y, +z);
    }
  }

  text1->SetMarkerSize( 2 );
  text2->SetMarkerSize( 2 );
  text3->SetMarkerSize( 2 );
  text4->SetMarkerSize( 2 );
  text6->SetMarkerSize( 2 );
  text7->SetMarkerSize( 2 );

}

bool EBRenderPlugin::applies( const ObjInfo &o, const ImgInfo &i ) {
 
  if( o.name.find( "EcalBarrel/EB" ) == 0 ) {
    return true;
  } 

  return false;

}

void EBRenderPlugin::preDraw( TCanvas *c, const ObjInfo &o, const ImgInfo &i, RenderInfo &r ) {

#ifdef DEBUG
  std::cout << "EBRenderPlugin:preDraw " << o.name << std::endl;
#endif

  c->cd();

//  gPad->SetFrameFillColor(10);
//  if (o.error) gPad->SetFillColor(2);
//  if (o.warning) gPad->SetFillColor(5);
//  if (o.other) gPad->SetFillColor(16);

  if( dynamic_cast<TProfile2D*>( o.object ) ) {
    preDrawTProfile2D( c, o );
  }
  else if( dynamic_cast<TProfile*>( o.object ) ) {
    preDrawTProfile( c, o );
  }
  else if( dynamic_cast<TH2*>( o.object ) ) {
    preDrawTH2( c, o );
  }
  else if( dynamic_cast<TH1*>( o.object ) ) {
    preDrawTH1( c, o );
  }

#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}

void EBRenderPlugin::preDrawTProfile2D( TCanvas *c, const ObjInfo &o ) {

  TProfile2D* obj = dynamic_cast<TProfile2D*>( o.object );

  assert( obj );

  gStyle->SetPaintTextFormat();

  gStyle->SetOptStat(0);
  obj->SetStats(kFALSE);

  if( o.name.find( "EBPDT" ) < o.name.size() || 
      o.name.find( "EBLT shape" ) < o.name.size() || 
      o.name.find( "EBTPT shape" ) < o.name.size() ) {
    obj->GetXaxis()->SetDrawOption("u");
    obj->GetYaxis()->SetDrawOption("u");
    obj->GetXaxis()->SetNdivisions(0);
    obj->GetYaxis()->SetNdivisions(0);
    obj->SetOption("axis");
    return;
  }

  // Occupancy-like (10 x grays) plots
  if( o.name.find( "EBCLT" ) < o.name.size() ) {
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(40118, kFALSE);
    obj->GetYaxis()->SetNdivisions(170102, kFALSE);
    gStyle->SetPalette(10, pCol4);
    obj->SetOption("colz");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  // Occupancy-like (10 x grays) plots
  gPad->SetGridx();
  gPad->SetGridy();
  obj->GetXaxis()->SetNdivisions(17);
  obj->GetYaxis()->SetNdivisions(4);
  gStyle->SetPalette(10, pCol4);
  obj->SetOption("colz");
  return;

}

void EBRenderPlugin::preDrawTProfile( TCanvas *c, const ObjInfo &o ) {

  TProfile* obj = dynamic_cast<TProfile*>( o.object );

  assert( obj );

  gStyle->SetOptStat(0);
  obj->SetStats( kFALSE );
  gStyle->SetOptStat("euomr");
  obj->SetStats(kTRUE);
  return;

}

void EBRenderPlugin::preDrawTH2( TCanvas *c, const ObjInfo &o ) {

  TH2* obj = dynamic_cast<TH2*>( o.object );

  assert( obj );

  gStyle->SetPaintTextFormat();

  gStyle->SetOptStat(0);
  obj->SetStats( kFALSE );
  int nbx = obj->GetNbinsX();
  int nby = obj->GetNbinsY();

  gPad->SetLogy(0);

  if( nbx == 85 && nby == 20 ) {
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(17);
    obj->GetYaxis()->SetNdivisions(4);
  }

  if( nbx == 17 && nby == 4 ) {
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(17);
    obj->GetYaxis()->SetNdivisions(4);
  }

  if( nbx == 10 && nby == 5 ) {
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(10);
    obj->GetYaxis()->SetNdivisions(1);
  }

  if( nbx == 2 && nby == 1 ) {
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(2);
    obj->GetYaxis()->SetNdivisions(1);
  }

  if( nbx == 360 && nby == 170 ) { 
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(18, kFALSE);
    obj->GetYaxis()->SetNdivisions(2);
  }

  if( nbx == 90 && nby == 20 ) { 
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(18, kFALSE);
    obj->GetYaxis()->SetNdivisions(2, kFALSE);
  }

  // Occupancy-like (10 x grays) plots
  if( o.name.find( "EBIT" ) < o.name.size() &&
      o.name.find( "quality" ) >= o.name.size() ) {
    obj->SetMinimum(0.0);
    gStyle->SetPalette(4, pCol4);
    obj->SetOption("colz");
    return;
  }

  // Occupancy-like (10 x grays) plots
  if( o.name.find( "EBOT" ) < o.name.size() &&
      o.name.find( "quality" ) >= o.name.size() ) {
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(40118, kFALSE);
    obj->GetYaxis()->SetNdivisions(170102, kFALSE);
    gStyle->SetPalette(10, pCol4);
    obj->SetMinimum(0.0);
    obj->SetOption("colz");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  // Quality-like (green, yellow, red) plots
  if( o.name.find( "summary" ) < o.name.size() ) {
    gStyle->SetOptStat(" ");
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(6.0);
    gStyle->SetPalette(6, pCol3);
    obj->SetOption("col");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  // Quality-like (green, yellow, red) plots
  if( o.name.find( "quality" ) < o.name.size() ) {
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(6.0);
    gStyle->SetPalette(6, pCol3);
    obj->SetOption("col");
    return;
  }

  // Occupancy-like (10 x grays) plots
  if( o.name.find( "EBMM event" ) < o.name.size() ) {
    gStyle->SetPalette(10, pCol4);
    obj->SetOption("colz");
    return;
  }

  // Occupancy-like (10 x grays) plots
  if( o.name.find( "EBCLT" ) < o.name.size() ) {
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(40118, kFALSE);
    obj->GetYaxis()->SetNdivisions(170102, kFALSE);
    gStyle->SetPalette(10, pCol4);
    obj->SetOption("colz");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

}

void EBRenderPlugin::preDrawTH1( TCanvas *c, const ObjInfo &o ) {

  TH1* obj = dynamic_cast<TH1*>( o.object );

  assert( obj );

  obj->SetStats(kTRUE);
  if ( obj->GetMaximum(1.e15) > 0. ) {
    gPad->SetLogy(1);
  } else {
   gPad->SetLogy(0);
  }

  gStyle->SetOptStat("euomr");
  return;

}

void EBRenderPlugin::postDraw( TCanvas *c, const ObjInfo &o, const ImgInfo &i ) {

#ifdef DEBUG
  std::cout << "EBRenderPlugin:postDraw " << o.name << std::endl;
#endif

  c->cd();

  if( dynamic_cast<TProfile2D*>( o.object ) ) {
    postDrawTProfile2D( c, o );
  }
  else if( dynamic_cast<TH2*>( o.object ) ) {
    postDrawTH2( c, o );
  }

#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}

void EBRenderPlugin::postDrawTProfile2D( TCanvas *c, const ObjInfo &o ) {

  TProfile2D* obj = dynamic_cast<TProfile2D*>( o.object );

  assert( obj );

  if( o.name.find( "EBPDT" ) < o.name.size() || 
      o.name.find( "EBLT shape" ) < o.name.size() || 
      o.name.find( "EBTPT shape" ) < o.name.size() ) {
    TH1D* obj1 = (TH1D*) gROOT->FindObject("shape");
    if( obj1) obj1->Delete();
    obj1 = obj->ProjectionY("shape", 1, 1, "e");
    gStyle->SetOptStat("euomr");
    obj1->SetStats(kTRUE);
    obj1->SetMinimum(0.0);
    gPad->SetGridx(0);
    gPad->SetGridy(0);
    obj1->GetXaxis()->SetNdivisions(510);
    obj1->GetYaxis()->SetNdivisions(510);
    obj1->GetXaxis()->SetDrawOption("+");
    obj1->GetYaxis()->SetDrawOption("+");
    obj1->Draw();
    return;
  }

  if( o.name.find( "EBCLT" ) < o.name.size() ) {
    text7->Draw( "text,same" );
    //ax1->Draw();
    return;
  }

  text1->Draw( "text,same" );
  return;

}

void EBRenderPlugin::postDrawTH2( TCanvas *c, const ObjInfo &o ) {

  TH2* obj = dynamic_cast<TH2*>( o.object );

  assert( obj );

  int nbx = obj->GetNbinsX();
  int nby = obj->GetNbinsY();

  if( nbx == 85 && nby == 20 ) {
    text1->Draw("text,same");
    return;
  }

  if( nbx == 17 && nby == 4 ) {
    text2->Draw("text,same");
    return;
  }

  if( nbx == 10 && nby == 5 ) {
    text3->Draw("text,same");
    return;
  }

  if( nbx == 2 && nby == 1 ) {
    text4->Draw("text,same");
    return;
  }

  if( o.name.find( "summary" ) < o.name.size() ) {
    text6->Draw("text,same");
    return;
  }

  if( o.name.find( "EBCLT" ) < o.name.size() ) {
    text7->Draw("text,same");
    //ax1->Draw();
    return;
  }

}

