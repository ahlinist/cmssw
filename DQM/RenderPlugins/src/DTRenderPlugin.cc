// $Id: DTRenderPlugin.cc,v 1.1 2007/10/11 07:12:57 gmasetti Exp $

/*!
  \file EBRenderPlugin
  \brief Display Plugin for Quality Histograms
  \author G. Masetti
  \version $Revision: 1.1 $
  \date $Date: 2007/10/11 07:12:57 $
*/

#include <TProfile2D.h>

#include <TStyle.h>
#include <TCanvas.h>
#include <TColor.h>

#include "DQMServices/Core/interface/MonitorElement.h"

//#include "DQM/EcalCommon/interface/ColorPalette.h"
//#include <DQM/EcalCommon/interface/Numbers.h>
#include "DTRenderPlugin.h"


void DTRenderPlugin::initialise( int argc, char **argv, DaqMonitorBEInterface *bei ) {


}

bool DTRenderPlugin::applies( const ObjInfo &o, const ImgInfo &i ) {
 
  if( o.name.find( "DriftTubes/DT" ) == 0 ) {
    return true;
  } 

  return false;

}

void DTRenderPlugin::preDraw( TCanvas *c, const ObjInfo &o, const ImgInfo &i, RenderInfo &r ) {

#ifdef DEBUG
  std::cout << "DTRenderPlugin:preDraw " << o.name << std::endl;
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

void DTRenderPlugin::preDrawTProfile2D( TCanvas *c, const ObjInfo &o ) {

  return;

}

void DTRenderPlugin::preDrawTProfile( TCanvas *c, const ObjInfo &o ) {

  return;

}

void DTRenderPlugin::preDrawTH2( TCanvas *c, const ObjInfo &o ) {

  TH2* obj = dynamic_cast<TH2*>( o.object );

  assert( obj );

  // This applies to all
  gStyle->SetCanvasBorderMode( 0 );
  gStyle->SetPadBorderMode( 0 );
  gStyle->SetPadBorderSize( 0 );
  //    (data->pad)->SetLogy( 0 );
  gStyle->SetOptStat( 0 );
  gStyle->SetPalette( 1 );
  obj->SetStats( kFALSE );
  
  //obj->SetOption( "box" );
  gStyle->SetPalette(1);
  obj->SetOption( "colz" );

  if( o.name.find( "FED770_EventLenght" ) < o.name.size() ) {
    gStyle->SetOptStat( 1111111 );
    gStyle->SetPalette( 1 );
    obj->SetStats( kTRUE );
    return;
  }

  if( o.name.find( "FED770_ROS" ) < o.name.size() ) {
    obj->GetXaxis()->LabelsOption("d");
    obj->GetXaxis()->SetLabelSize(0.05);
    obj->GetYaxis()->SetLabelSize(0.05);
    return;
  }

  if( o.name.find( "SCTriggerBX" ) < o.name.size() ) {
    obj->GetYaxis()->SetLabelSize(0.1);
    obj->GetXaxis()->SetTitle("Trigger BX");
    return;
  }
  
  if( o.name.find( "SCTriggerQuality" ) < o.name.size() ) {
    obj->GetXaxis()->LabelsOption("h");
    obj->GetXaxis()->SetLabelSize(0.1);
    obj->GetYaxis()->SetLabelSize(0.1);
    return;
  }

  return;


}

void DTRenderPlugin::preDrawTH1( TCanvas *c, const ObjInfo &o ) {

  TH1* obj = dynamic_cast<TH1*>( o.object );

  assert( obj );

  // This applies to all
  gStyle->SetCanvasBorderMode( 0 );
  gStyle->SetPadBorderMode( 0 );
  gStyle->SetPadBorderSize( 0 );
  //      (data->pad)->SetLogy( 1 );
  gStyle->SetOptStat( 1111111 );
  obj->SetStats( kFALSE );

  if ( obj->GetMaximum(1.e15) > 0. ) {
    gPad->SetLogy(1);
  } else {
   gPad->SetLogy(0);
  }

  if( o.name.find( "FED770TTSValues_Percent" ) < o.name.size() ) {
    gPad->SetLogy( 1 );
    return;
  }

  if( o.name.find( "FED770_ROSList" ) < o.name.size() ) {
    obj->GetXaxis()->SetTitle("# of ROS");
    return;
  }

  if( o.name.find( "hResDist" ) < o.name.size() ) {
    
    TAttLine *line = dynamic_cast<TAttLine *> (o.object);
    assert (line);
    
    if (line) {
      
      MonitorElement* me = o.me;
      if (me->hasError()) {
	line->SetLineColor(TColor::GetColor("#CC0000"));
	//	  std::cout << name << " has error" << std::endl;
      }
      else if (me->hasWarning()) {
	line->SetLineColor(TColor::GetColor("#993300"));
	//	  std::cout << name << " has worning" << std::endl;
      }
      else if (me->hasOtherReport()) { 
	line->SetLineColor(TColor::GetColor("#FFCC00"));
	//	  std::cout << name << " has other report" << std::endl;
      }
      else {
	line->SetLineColor(TColor::GetColor("#000000"));
	//	  std::cout << name << " has nothing" << std::endl; 
      }  
    }   
    return;
  }
  
  return;

}

void DTRenderPlugin::postDraw( TCanvas *c, const ObjInfo &o, const ImgInfo &i ) {

#ifdef DEBUG
  std::cout << "DTRenderPlugin:postDraw " << o.name << std::endl;
#endif

  c->cd();

  if( dynamic_cast<TProfile2D*>( o.object ) ) {
    postDrawTProfile2D( c, o );
  }
  else if( dynamic_cast<TProfile*>( o.object ) ) {
    postDrawTProfile( c, o );
  }
  else if( dynamic_cast<TH2*>( o.object ) ) {
    postDrawTH2( c, o );
  }
  else if( dynamic_cast<TH1*>( o.object ) ) {
    postDrawTH2( c, o );
  }


#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}

void DTRenderPlugin::postDrawTProfile2D( TCanvas *c, const ObjInfo &o ) {

  return;

}

void DTRenderPlugin::postDrawTProfile( TCanvas *c, const ObjInfo &o ) {

  return;

}

void DTRenderPlugin::postDrawTH2( TCanvas *c, const ObjInfo &o ) {

  return;

}

void DTRenderPlugin::postDrawTH1( TCanvas *c, const ObjInfo &o ) {

  return;

}

