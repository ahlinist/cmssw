// $Id: EcalCalibRenderPlugin.cc,v 1.1 2011/08/11 18:33:05 yiiyama Exp $

/*!
  \file EcalCalibRenderPlugin
  \brief Display Plugin for Quality Histograms
  \author Y. Iiyama
  \version $Revision: 1.1 $
  \date $Date: 2011/08/11 18:33:05 $
*/

#include "DQM/DQMRenderPlugin.h"
#include "utils.h"

#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TProfile2D.h"

#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TROOT.h"
#include "TLine.h"
#include "TClass.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <math.h>
#include <cassert>

class EcalCalibRenderPlugin : public DQMRenderPlugin
{

  int pStatus[3]; // green, yellow, red

public:
  virtual void initialise( int, char ** )
    {

      pStatus[0] = kRed;
      pStatus[1] = kYellow;
      pStatus[2] = kGreen;

    }

  virtual bool applies( const VisDQMObject &o, const VisDQMImgInfo & )
    {
      if( o.name.find( "EcalCalibration/" ) != std::string::npos )
	return true;

      return false;
    }

  virtual void preDraw( TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo &, VisDQMRenderInfo &r )
    {
      c->cd();

      gStyle->Reset("Default");

      gStyle->SetCanvasColor(10);
      gStyle->SetPadColor(10);
      gStyle->SetFillColor(10);
      gStyle->SetFrameFillColor(10);
      gStyle->SetStatColor(10);
      gStyle->SetTitleFillColor(10);

      gStyle->SetOptTitle(kTRUE);
      gStyle->SetTitleBorderSize(0);

      gStyle->SetOptStat(kFALSE);
      gStyle->SetStatBorderSize(1);

      gStyle->SetOptFit(kFALSE);

      gROOT->ForceStyle();

      TObject *obj = o.object;

      if( obj->IsA() == TClass::GetClass("TProfile2D") )
      {
        preDrawTProfile2D( c, o, r );
      }
      else if( obj->IsA() == TClass::GetClass("TProfile") )
      {
        preDrawTProfile( c, o, r );
      }
      else if( obj->IsA() == TClass::GetClass("TH3F") )
      {
        preDrawTH3F( c, o, r );
      }
      else if( obj->IsA() == TClass::GetClass("TH2F") || obj->IsA() == TClass::GetClass("TH2D") )
      {
        preDrawTH2( c, o, r );
      }
      else if( obj->IsA() == TClass::GetClass("TH1F") || obj->IsA() == TClass::GetClass("TH1D") )
      {
        preDrawTH1( c, o, r );
      }
    }

  virtual void postDraw( TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo & )
    {
      c->cd();

      TObject *obj = o.object;

      if( obj->IsA() == TClass::GetClass("TProfile2D") )
      {
        postDrawTProfile2D( c, o );
      }
      else if( obj->IsA() == TClass::GetClass("TProfile") )
      {
        postDrawTProfile( c, o );
      }
      else if( obj->IsA() == TClass::GetClass("TH3F") )
      {
        postDrawTH3F( c, o );
      }
      else if( obj->IsA() == TClass::GetClass("TH2F") || obj->IsA() == TClass::GetClass("TH2D") )
      {
        postDrawTH2( c, o );
      }
      else if( obj->IsA() == TClass::GetClass("TH1F") || obj->IsA() == TClass::GetClass("TH1D") )
      {
        postDrawTH1( c, o );
      }
    }

private:
  void preDrawTProfile2D( TCanvas *, const VisDQMObject &, VisDQMRenderInfo & )
    {
    }

  void preDrawTProfile( TCanvas *, const VisDQMObject &o, VisDQMRenderInfo & )
    {
      TProfile *obj = dynamic_cast<TProfile *>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      if( name.find("EcalLaser") != std::string::npos ){

	if( name.find("L1 (blue) amplitude trend") != std::string::npos ){
	  obj->SetMinimum(0.);
	  obj->SetMaximum(1000.);
	}else if( name.find("L4 (red) amplitude trend") != std::string::npos ){
	  obj->SetMinimum(0.);
	  obj->SetMaximum(2000.);
	}else if( name.find("amplitude RMS trend") != std::string::npos ){
	  obj->SetMinimum(0.);
	  obj->SetMaximum(20.);
	}else if( name.find("jitter trend") != std::string::npos ){
	  obj->SetMinimum(0.);
	  obj->SetMaximum(20.);
	}else if( name.find("FWHM trend") != std::string::npos ){
	  obj->SetMinimum(20.);
	  obj->SetMaximum(100.);
	}else if( name.find("timing trend") != std::string::npos ){
	  obj->SetMinimum(1370.);
	  obj->SetMaximum(1560.);
	}else if( name.find("prepulse amplitude trend") != std::string::npos ){
	  obj->SetMinimum(-0.5);
	  obj->SetMaximum(2.);
	}else if( name.find("prepulse width trend") != std::string::npos ){
	  obj->SetMinimum(0.);
	  obj->SetMaximum(200.);
	}else if( name.find("error on") != std::string::npos ){
	  gPad->SetLogy(kTRUE);
	}else if( name.find("approximate trigger rate") != std::string::npos ){
	  gPad->SetLogy(kTRUE);
	}else if( name.find("number of") != std::string::npos ){
	  obj->SetMinimum(0.);
	  obj->SetMaximum(2500.);
	}else if( name.find("duration") != std::string::npos ){
	  obj->SetMinimum(0.);
	  obj->SetMaximum(16.);
	}

	return;
      }
    }

  void preDrawTH3F( TCanvas *, const VisDQMObject &, VisDQMRenderInfo & )
    {
    }

  void preDrawTH2( TCanvas *, const VisDQMObject &o, VisDQMRenderInfo &r )
    {
      TH2* obj = dynamic_cast<TH2*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      gStyle->SetPaintTextFormat();

      gStyle->SetOptStat(kFALSE);
      obj->SetStats(kFALSE);
      gPad->SetLogy(kFALSE);

      if( name.find( "EcalLaser" ) != std::string::npos ){

	if( name.find("sequence validation") != std::string::npos ){
	  obj->SetMinimum(0.);
	  obj->SetMaximum(3.);

	  gStyle->SetPalette(3,pStatus);

	  if( r.drawOptions.size() == 0 ) r.drawOptions = "col";
	}

	return;
      }
    }

  void preDrawTH1( TCanvas *, const VisDQMObject &o, VisDQMRenderInfo &r )
    {
      TH1* obj = dynamic_cast<TH1*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      if( name.find( "EcalLaser" ) != std::string::npos ){

	if( name.find( "duration max" ) != std::string::npos || name.find( "duration min" ) != std::string::npos ){
	  obj->SetMarkerStyle(20);
	  obj->SetMarkerSize(1);

	  if( r.drawOptions.size() == 0 ) r.drawOptions = "P";
	}
      }
    }

  void postDrawTProfile2D( TCanvas *, const VisDQMObject & )
    {
    }

  void postDrawTH3F( TCanvas *, const VisDQMObject & )
    {
    }

  void postDrawTProfile( TCanvas *, const VisDQMObject &o )
    {
      TProfile *obj = dynamic_cast<TProfile *>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      TLine l1, l2, l3;
      l1.SetLineWidth(2), l2.SetLineWidth(2), l3.SetLineWidth(2);

      if( name.find("EcalLaser") != std::string::npos ){

	float xmin = obj->GetXaxis()->GetXmin();
	float xmax = obj->GetXaxis()->GetXmax();

	l1.SetLineStyle(1), l2.SetLineStyle(1), l3.SetLineStyle(1);
	l1.SetLineColor(kRed), l2.SetLineColor(kOrange), l3.SetLineColor(kGreen);

	if( name.find("L1 (blue) amplitude trend") != std::string::npos ){
	  l1.DrawLine(xmin,100,xmax,100);
	  l2.DrawLine(xmin,200,xmax,200);
	}else if( name.find("L4 (red) amplitude trend") != std::string::npos ){
	  l1.DrawLine(xmin,200,xmax,200);
	  l2.DrawLine(xmin,400,xmax,400);
	}else if( name.find("amplitude RMS trend") != std::string::npos ){
	  l1.DrawLine(xmin,15,xmax,15);
	  l2.DrawLine(xmin,5,xmax,5);
	}else if( name.find("jitter trend") != std::string::npos ){
	  l1.DrawLine(xmin,15,xmax,15);
	  l2.DrawLine(xmin,10,xmax,10);
	}else if( name.find("FWHM trend") != std::string::npos ){
	  l1.DrawLine(xmin,75,xmax,75);
	  l2.DrawLine(xmin,50,xmax,50);
	}else if( name.find("timing trend") != std::string::npos ){
	  l1.DrawLine(xmin,1515,xmax,1515);
	  l1.DrawLine(xmin,1415,xmax,1415);
	  l2.DrawLine(xmin,1490,xmax,1490);
	  l2.DrawLine(xmin,1440,xmax,1440);
	  l3.DrawLine(xmin,1465,xmax,1465);
	}

	return;
      }
    }

  void postDrawTH2( TCanvas *, const VisDQMObject & )
    {
    }

  void postDrawTH1( TCanvas *, const VisDQMObject & )
    {
    }

};

static EcalCalibRenderPlugin instance;
