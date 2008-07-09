// $Id: EBRenderPlugin.cc,v 1.88 2008/06/10 09:44:16 dellaric Exp $

/*!
  \file EBRenderPlugin
  \brief Display Plugin for Quality Histograms
  \author G. Della Ricca
  \author B. Gobbo 
  \version $Revision: 1.88 $
  \date $Date: 2008/06/10 09:44:16 $
*/

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
#include <math.h>

#include "DQM/EcalCommon/interface/ColorPalette.h"
#include "DQM/EcalCommon/interface/Numbers.h"

#include "DQM/RenderPlugins/src/utils.h"

#include "DQM/RenderPlugins/src/EBRenderPlugin.h"

static bool first = true;

void EBRenderPlugin::initialise( int argc, char **argv ) {

  if( ! first ) return;

  first = false;

  for( int i=0; i<6; i++ ) {
    TColor* color = gROOT->GetColor( 301+i );
    if ( ! color ) color = new TColor( 301+i, 0, 0, 0, "");
    color->SetRGB( ecdqm::rgb[i][0], ecdqm::rgb[i][1], ecdqm::rgb[i][2] );
  }

  for( int i=0; i<10; i++ ) {
    TColor* color = gROOT->GetColor( 401+i );
    if ( ! color ) color = new TColor( 401+i, 0, 0, 0, "");
    color->SetRGB( ecdqm::rgb2[i][0], ecdqm::rgb2[i][1], ecdqm::rgb2[i][2] );
  }
  
  for( int i=0; i<10; i++ ) {
    TColor* color = gROOT->GetColor( 501+i );
    if ( ! color ) color = new TColor( 501+i, 0, 0, 0, "");
    color->SetRGB( ecdqm::rgb2[i][1], 0, 0 );
  }

  for( short i=0; i<6; i++ ) pCol3[i]  = i+301;
  for( short i=0; i<10; i++ ) pCol4[i] = i+401;
  for( short i=0; i<10; i++ ) pCol5[i] = i+501;

  text1 = new TH2C( "eb_text1", "text1", 85, 0,  85, 20,   0, 20 );
  text2 = new TH2C( "eb_text2", "text2", 17, 0,  17,  4,   0,  4 );
  text3 = new TH2C( "eb_text3", "text3", 10, 0,  10,  5,   0,  5 );
  text4 = new TH2C( "eb_text4", "text4",  2, 0,   2,  1,   0,  1 );
  text6 = new TH2C( "eb_text6", "text6", 18, 0, 360,  2, -85, 85 );
  text7 = new TH2C( "eb_text7", "text7", 18, -M_PI*(9+1.5)/9, M_PI*(9-1.5)/9, 2, -1.479, 1.479);
  text8 = new TH2C( "eb_text8", "text8", 18, 0., 72., 2, -17., 17. );
  text9 = new TH2C( "eb_text9", "text9", 18, 0., 72., 2, 0., 34. );

  text1->SetMinimum(   0.10 );
  text2->SetMinimum(   0.10 );
  text3->SetMinimum(   0.10 );
  text4->SetMinimum(   0.10 );
  text6->SetMinimum( -18.01 );
  text7->SetMinimum( -18.01 );
  text8->SetMinimum( -18.01 );
  text9->SetMinimum( -18.01 );

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
  for ( short i=0; i<36; i++ ) {
    int x = 1 + i%18;
    int y = 1 + i/18;
    text8->SetBinContent(x, y, Numbers::iEB(i+1));
    text9->SetBinContent(x, y, Numbers::iEB(i+1));
  }

  text1->SetMarkerSize( 2 );
  text2->SetMarkerSize( 2 );
  text3->SetMarkerSize( 2 );
  text4->SetMarkerSize( 2 );
  text6->SetMarkerSize( 2 );
  text7->SetMarkerSize( 2 );
  text8->SetMarkerSize( 2 );
  text9->SetMarkerSize( 2 );

}

bool EBRenderPlugin::applies( const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

#ifdef DEBUG
  std::cout << "EBRenderPlugin:applies " << o.name << std::endl;
#endif

  if( o.name.find( "EcalBarrel/EB" ) < o.name.size() ) {
    return true;
  }

  if( o.name.find( "EcalBarrel/EcalInfo" ) < o.name.size() ) {
    return true;
  }

  if( o.name.find( "EcalBarrel/EventInfo" ) < o.name.size() ) {
    return true;
  }

  if( o.name.find( "EcalBarrel/Run summary/EB" ) < o.name.size() ) {
    return true;
  }

  if( o.name.find( "EcalBarrel/Run summary/EcalInfo" ) < o.name.size() ) {
    return true;
  }

  if( o.name.find( "EcalBarrel/Run summary/EventInfo" ) < o.name.size() ) {
    return true;
  }

  return false;

}

void EBRenderPlugin::preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo &r ) {

#ifdef DEBUG
  std::cout << "EBRenderPlugin:preDraw " << o.name << std::endl;
#endif

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

  gStyle->SetOptFit(kFALSE);

  gROOT->ForceStyle();

  if( dynamic_cast<TProfile2D*>( o.object ) ) {
    preDrawTProfile2D( c, o );
  }
  else if( dynamic_cast<TProfile*>( o.object ) ) {
    preDrawTProfile( c, o );
  }
  else if( dynamic_cast<TH3F*>( o.object ) ) {
    preDrawTH3F( c, o );
  }
  else if( dynamic_cast<TH2F*>( o.object ) ) {
    preDrawTH2F( c, o );
  }
  else if( dynamic_cast<TH1F*>( o.object ) ) {
    preDrawTH1F( c, o );
  }

  r.drawOptions = "";

#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}

void EBRenderPlugin::preDrawTProfile2D( TCanvas *c, const DQMNet::CoreObject &o ) {

  TProfile2D* obj = dynamic_cast<TProfile2D*>( o.object );

  assert( obj );

  std::string name = o.name.substr(o.name.rfind("/")+1);

  gStyle->SetPaintTextFormat();

  gStyle->SetOptStat(kFALSE);
  obj->SetStats(kFALSE);
  gPad->SetLogy(kFALSE);

  if( name.find( "EBLT shape" ) < name.size() ) {
    c->SetTheta(+30.);
    c->SetPhi(-60.);
    obj->GetXaxis()->SetTitleOffset(2.5);
    obj->GetYaxis()->SetTitleOffset(3.0);
    obj->GetZaxis()->SetTitleOffset(1.3);
    obj->SetOption("lego");
    return;
  }

  if( name.find( "EBTPT shape" ) < name.size() ) {
    c->SetTheta(+30.);
    c->SetPhi(-60.);
    obj->GetXaxis()->SetTitleOffset(2.5);
    obj->GetYaxis()->SetTitleOffset(3.0);
    obj->GetZaxis()->SetTitleOffset(1.3);
    obj->SetOption("lego");
    return;
  }

  if( name.find( "EBCLT" ) < name.size() ) {
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(40118, kFALSE);
    obj->GetYaxis()->SetNdivisions(170102, kFALSE);
    obj->SetMinimum(0.0);
    gStyle->SetPalette(10, pCol4);
    obj->SetOption("colz");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  int nbx = obj->GetNbinsX();
  int nby = obj->GetNbinsY();

  if( nbx == 72 && nby == 34 ) {
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(18, kFALSE);
    obj->GetYaxis()->SetNdivisions(2, kFALSE);
  }

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

  obj->SetMinimum(0.0);
  gStyle->SetPalette(10, pCol4);
  obj->SetOption("colz");
  return;

}

void EBRenderPlugin::preDrawTProfile( TCanvas *c, const DQMNet::CoreObject &o ) {

  TProfile* obj = dynamic_cast<TProfile*>( o.object );

  assert( obj );

  std::string name = o.name.substr(o.name.rfind("/")+1);

  gStyle->SetPaintTextFormat();

  gStyle->SetOptStat("euomr");
  obj->SetStats(kTRUE);
  gPad->SetLogy(kFALSE);

  if( name.find( "EBMM digi number profile" ) < name.size() ) {
   gPad->SetBottomMargin(0.2);
   obj->GetXaxis()->LabelsOption("v");
  }

  if( name.find( "EBMM hit number profile" ) < name.size() ) {
   gPad->SetBottomMargin(0.2);
   obj->GetXaxis()->LabelsOption("v");
  }

  if( name.find( "EBMM TP digi number profile" ) < name.size() ) {
   gPad->SetBottomMargin(0.2);
   obj->GetXaxis()->LabelsOption("v");
  }

  return;

}

void EBRenderPlugin::preDrawTH3F( TCanvas *c, const DQMNet::CoreObject &o ) {
  
  TH3F* obj = dynamic_cast<TH3F*>( o.object );
  
  assert( obj );

  std::string name = o.name.substr(o.name.rfind("/")+1);

  gStyle->SetPaintTextFormat();

  gStyle->SetOptStat(kFALSE);
  obj->SetStats(kFALSE);
  gPad->SetLogy(kFALSE);

  return;
  
} 

void EBRenderPlugin::preDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH2F* obj = dynamic_cast<TH2F*>( o.object );

  assert( obj );

  std::string name = o.name.substr(o.name.rfind("/")+1);

  gStyle->SetPaintTextFormat();

  gStyle->SetOptStat(kFALSE);
  obj->SetStats(kFALSE);
  gPad->SetLogy(kFALSE);

  if( name.find( "EBCLT" ) < name.size() ) {
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(40118, kFALSE);
    obj->GetYaxis()->SetNdivisions(170102, kFALSE);
    obj->SetMinimum(0.0);
    gStyle->SetPalette(10, pCol4);
    obj->SetOption("colz");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  if( name.find( "EBTMT timing vs amplitude" ) < name.size() ) {
    if ( obj->GetMaximum() > 0. ) gPad->SetLogz(kTRUE);
    obj->SetMinimum(0.0);
    gStyle->SetPalette(10, pCol4);
    obj->SetOption("colz");
    return;
  }

  int nbx = obj->GetNbinsX();
  int nby = obj->GetNbinsY();

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
    obj->GetYaxis()->SetNdivisions(2, kFALSE);
  }

  if( nbx == 90 && nby == 20 ) { 
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(18, kFALSE);
    obj->GetYaxis()->SetNdivisions(2, kFALSE);
  }

  if( nbx == 72 && nby == 34 ) {
    gPad->SetGridx();
    gPad->SetGridy();
    obj->GetXaxis()->SetNdivisions(18, kFALSE);
    obj->GetYaxis()->SetNdivisions(2, kFALSE);
  }

  if( name.find( "reportSummaryMap" ) < name.size() ) {
    dqm::utils::reportSummaryMapPalette(obj);
    obj->SetTitle("EcalBarrel Report Summary Map");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  if( name.find( "EBIT" ) < name.size() &&
      name.find( "quality" ) >= name.size() ) {
    obj->SetMinimum(0.0);
    gStyle->SetPalette(10, pCol5);
    obj->SetOption("colz");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  if( name.find( "EBTTT" ) < name.size() &&
      name.find( "quality" ) >= name.size() ) {
    obj->SetMinimum(0.0);
    if( name.find( "Error" ) >= name.size() ) {
      gStyle->SetPalette(10, pCol4);
    } else {
      gStyle->SetPalette(10, pCol5);
    }
    obj->SetOption("colz");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  if( name.find( "EBSFT" ) < name.size() &&
      name.find( "summary" ) >= name.size() ) { 
    obj->SetMinimum(0.0); 
    gStyle->SetPalette(10, pCol5);
    obj->SetOption("colz");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  if( name.find( "EBOT" ) < name.size() ) {
    obj->SetMinimum(0.0);
    gStyle->SetPalette(10, pCol4);
    obj->SetOption("colz");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  if( name.find( "EBCT" ) < name.size() ) {
    obj->SetMinimum(0.0);
    gStyle->SetPalette(10, pCol4);
    obj->SetOption("colz");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  if( name.find( "summary" ) < name.size() ) {
    gStyle->SetOptStat(" ");
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(6.0);
    gStyle->SetPalette(6, pCol3);
    obj->SetOption("col");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  if( name.find( "quality" ) < name.size() ) {
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(6.0);
    gStyle->SetPalette(6, pCol3);
    obj->SetOption("col");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

  if( name.find( "EBMM event" ) < name.size() ) {
    obj->SetMinimum(0.0);
    gStyle->SetPalette(10, pCol4);
    obj->SetOption("colz");
    gStyle->SetPaintTextFormat("+g");
    return;
  }

}

void EBRenderPlugin::preDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH1F* obj = dynamic_cast<TH1F*>( o.object );

  assert( obj );

  std::string name = o.name.substr(o.name.rfind("/")+1);

  gStyle->SetPaintTextFormat();

  gStyle->SetOptStat("euomr");
  obj->SetStats(kTRUE);
  gPad->SetLogy(kFALSE);

  int nbx = obj->GetNbinsX();

  if ( obj->GetMaximum() > 0. ) gPad->SetLogy(kTRUE);

  if ( nbx == 10 ) gPad->SetLogy(kFALSE);
  if ( nbx == 1700 ) gPad->SetLogy(kFALSE);

  if( name.find( "EVTTYPE" ) < name.size() ) {
   gPad->SetBottomMargin(0.4);
   obj->GetXaxis()->LabelsOption("v");
  }

  if( name.find( "EBMM DCC" ) < name.size() ) {
   gPad->SetBottomMargin(0.2);
   obj->GetXaxis()->LabelsOption("v");
  }

  if( name.find( "EBIT DCC" ) < name.size() ) {
   gPad->SetBottomMargin(0.2);
   obj->GetXaxis()->LabelsOption("v");
  }

  if( name.find( "front-end status bits" ) < name.size() ) {
   gPad->SetBottomMargin(0.25);
   obj->GetXaxis()->LabelsOption("v");
  }

  if( name.find( "front-end status errors summary" ) < name.size() ) {
   gPad->SetBottomMargin(0.2);
   obj->GetXaxis()->LabelsOption("v");
  }

  if( name.find( "quality errors summary" ) < name.size() ) {
   gPad->SetBottomMargin(0.2);
   obj->GetXaxis()->LabelsOption("v");
  }

  if( name.find( "EBOT digi occupancy summary 1D" ) < name.size() ) {
   gPad->SetBottomMargin(0.2);
   obj->GetXaxis()->LabelsOption("v");
  }

  return;

}

void EBRenderPlugin::postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

#ifdef DEBUG
  std::cout << "EBRenderPlugin:postDraw " << o.name << std::endl;
#endif

  c->cd();

  if( dynamic_cast<TProfile2D*>( o.object ) ) {
    postDrawTProfile2D( c, o );
  }
  else if( dynamic_cast<TH3F*>( o.object ) ) {
    postDrawTH3F( c, o );
  }
  else if( dynamic_cast<TH2F*>( o.object ) ) {
    postDrawTH2F( c, o );
  }
  else if( dynamic_cast<TH1F*>( o.object ) ) {
    postDrawTH1F( c, o );
  }

#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}

void EBRenderPlugin::postDrawTProfile2D( TCanvas *c, const DQMNet::CoreObject &o ) {

  TProfile2D* obj = dynamic_cast<TProfile2D*>( o.object );

  assert( obj );

  std::string name = o.name.substr(o.name.rfind("/")+1);

  if( name.find( "EBLT shape" ) < name.size() ) {
    return;
  }

  if( name.find( "EBTPT shape" ) < name.size() ) {
    return;
  }

  if( name.find( "EBCLT" ) < name.size() ) {
    int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text7->GetXaxis()->SetRange(x1, x2);
    text7->GetYaxis()->SetRange(y1, y2);
    text7->Draw("text,same");
    return;
  }

  int nbx = obj->GetNbinsX();
  int nby = obj->GetNbinsY();

  if( nbx == 17 && nby == 4 ) {
    int x1 = text2->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text2->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text2->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text2->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text2->GetXaxis()->SetRange(x1, x2);
    text2->GetYaxis()->SetRange(y1, y2);
    text2->Draw("text,same");
    return;
  }

  if( nbx == 72 && nby == 34 ) {
    int x1 = text8->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text8->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text8->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text8->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text8->GetXaxis()->SetRange(x1, x2);
    text8->GetYaxis()->SetRange(y1, y2);
    text8->Draw("text,same");
    return;
  }

  int x1 = text1->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
  int x2 = text1->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
  int y1 = text1->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
  int y2 = text1->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
  text1->GetXaxis()->SetRange(x1, x2);
  text1->GetYaxis()->SetRange(y1, y2);
  text1->Draw("text,same");
  return;

}

void EBRenderPlugin::postDrawTH3F( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH3F* obj = dynamic_cast<TH3F*>( o.object );

  assert( obj );

  std::string name = o.name.substr(o.name.rfind("/")+1);

  return;

}

void EBRenderPlugin::postDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH2F* obj = dynamic_cast<TH2F*>( o.object );

  assert( obj );

  std::string name = o.name.substr(o.name.rfind("/")+1);

  if( name.find( "EBCLT" ) < name.size() ) {
    int x1 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text7->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text7->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text7->GetXaxis()->SetRange(x1, x2);
    text7->GetYaxis()->SetRange(y1, y2);
    text7->Draw("text,same");
    return;
  }

  if( name.find( "EBOT MEM" ) < name.size() ) {
    int x1 = text3->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text3->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text3->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text3->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text3->GetXaxis()->SetRange(x1, x2);
    text3->GetYaxis()->SetRange(y1, y2);
    text3->Draw("text,same");
    return;
  }

  if( name.find( "EBOT digi" ) < name.size() ) {
    int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text6->GetXaxis()->SetRange(x1, x2);
    text6->GetYaxis()->SetRange(y1, y2);
    text6->Draw("text,same");
    return;
  }

  if( name.find( "EBOT rec hit" ) < name.size() ) {
    int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text6->GetXaxis()->SetRange(x1, x2);
    text6->GetYaxis()->SetRange(y1, y2);
    text6->Draw("text,same");
    return;
  }

  if( name.find( "EBOT TP digi" ) < name.size() ) {
    int x1 = text8->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text8->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text8->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text8->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text8->GetXaxis()->SetRange(x1, x2);
    text8->GetYaxis()->SetRange(y1, y2);
    text8->Draw("text,same");
    return;
  }

  if( name.find( "EBOT test pulse digi" ) < name.size() ) {
    int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text6->GetXaxis()->SetRange(x1, x2);
    text6->GetYaxis()->SetRange(y1, y2);
    text6->Draw("text,same");
    return;
  }

  if( name.find( "EBOT laser digi" ) < name.size() ) {
    int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text6->GetXaxis()->SetRange(x1, x2);
    text6->GetYaxis()->SetRange(y1, y2);
    text6->Draw("text,same");
    return;
  }

  if( name.find( "EBOT pedestal digi" ) < name.size() ) {
    int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text6->GetXaxis()->SetRange(x1, x2);
    text6->GetYaxis()->SetRange(y1, y2);
    text6->Draw("text,same");
    return;
  }

  if( name.find( "reportSummaryMap" ) < name.size() ) {
    int x1 = text9->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text9->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text9->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text9->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text9->GetXaxis()->SetRange(x1, x2);
    text9->GetYaxis()->SetRange(y1, y2);
    text9->Draw("text,same");
    return;
  }

  int nbx = obj->GetNbinsX();
  int nby = obj->GetNbinsY();

  if( nbx == 85 && nby == 20 ) {
    int x1 = text1->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text1->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text1->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text1->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text1->GetXaxis()->SetRange(x1, x2);
    text1->GetYaxis()->SetRange(y1, y2);
    text1->Draw("text,same");
    return;
  }

  if( nbx == 17 && nby == 4 ) {
    int x1 = text2->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text2->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text2->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text2->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text2->GetXaxis()->SetRange(x1, x2);
    text2->GetYaxis()->SetRange(y1, y2);
    text2->Draw("text,same");
    return;
  }

  if( nbx == 10 && nby == 5 ) {
    int x1 = text3->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text3->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text3->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text3->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text3->GetXaxis()->SetRange(x1, x2);
    text3->GetYaxis()->SetRange(y1, y2);
    text3->Draw("text,same");
    return;
  }

  if( nbx == 2 && nby == 1 ) {
    int x1 = text4->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text4->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text4->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text4->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text4->GetXaxis()->SetRange(x1, x2);
    text4->GetYaxis()->SetRange(y1, y2);
    text4->Draw("text,same");
    return;
  }

  if( nbx == 72 && nby == 34 ) {
    int x1 = text8->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text8->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text8->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text8->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text8->GetXaxis()->SetRange(x1, x2);
    text8->GetYaxis()->SetRange(y1, y2);
    text8->Draw("text,same");
    return;
  }

  if( name.find( "summary" ) < name.size() ) {
    int x1 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmin());
    int x2 = text6->GetXaxis()->FindFixBin(obj->GetXaxis()->GetXmax());
    int y1 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmin());
    int y2 = text6->GetYaxis()->FindFixBin(obj->GetYaxis()->GetXmax());
    text6->GetXaxis()->SetRange(x1, x2);
    text6->GetYaxis()->SetRange(y1, y2);
    text6->Draw("text,same");
    return;
  }

}

void EBRenderPlugin::postDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH1F* obj = dynamic_cast<TH1F*>( o.object );

  assert( obj );

  std::string name = o.name.substr(o.name.rfind("/")+1);

  return;

}

