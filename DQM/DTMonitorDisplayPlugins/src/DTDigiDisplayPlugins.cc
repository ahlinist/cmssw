// $Id: DTDigiDisplayPlugins.cc,v 1.1 2007/03/29 09:10:24 gmasetti Exp $

/*!
  \file DTDigiDisplayPlugins
  \brief Display Plugin for Digi Task Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.1 $
  \date $Date: 2007/03/29 09:10:24 $
*/

#include "DQM/DTMonitorDisplayPlugins/src/DTDigiDisplayPlugins.h"
//#include "DQM/DTMonitorDisplayPlugins/interface/ColorPalette.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include <iostream>
#include <TROOT.h>
#include <TGraph.h>
#include <TObject.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TVirtualPad.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TColor.h>



DTDigiDisplayPlugins::DTDigiDisplayPlugins () {

}

bool DTDigiDisplayPlugins::isDigiME (std::string name) {

  if( name.find( "Occupancy" ) == 0 ) {
    return true;
  }

  if( name.find( "TimeBoxes" ) == 0 ) {
    return true;
  }

  return false;

}


std::string DTDigiDisplayPlugins::preDraw( VisDQMDisplayPlugin::DisplayData *data ) {

  if( dynamic_cast<TProfile2D*>( data->object ) ) {
    return preDrawTProfile2D( data );
  }

  if( dynamic_cast<TProfile*>( data->object ) ) {
    return preDrawTProfile( data );
  }

  if( dynamic_cast<TH2F*>( data->object ) ) {
    return preDrawTH2F( data );
  }
  
  if( dynamic_cast<TH1F*>( data->object ) ) {
    return preDrawTH1F( data );
  }
  
  return "";

}


std::string DTDigiDisplayPlugins::preDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTDigiDisplayPlugins::preDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTDigiDisplayPlugins::preDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

  TH2F* obj = dynamic_cast<TH2F*>( data->object );

  name = (data->object)->GetName();

  if( obj ) {

    // This applies to all
    gStyle->SetCanvasBorderMode( 0 );
    gStyle->SetPadBorderMode( 0 );
    gStyle->SetPadBorderSize( 0 );
    //    (data->pad)->SetLogy( 0 );
    gStyle->SetOptStat( 0 );
    obj->SetStats( kFALSE );

    obj->SetOption( "box" );

  }

  return "";    

}

std::string DTDigiDisplayPlugins::preDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

  TH1F* obj = dynamic_cast<TH1F*>( data->object );

  //name = (data->object)->GetName();

  if( obj ) {

      //     // This applies to all
      gStyle->SetCanvasBorderMode( 0 );
      gStyle->SetPadBorderMode( 0 );
      gStyle->SetPadBorderSize( 0 );
      //      (data->pad)->SetLogy( 1 );
      gStyle->SetOptStat( 0 );
      obj->SetStats( kFALSE );
      
  }

  return "";    

}


void DTDigiDisplayPlugins::postDraw( VisDQMDisplayPlugin::DisplayData *data ) {

  if( dynamic_cast<TProfile2D*>( data->object ) ) {
    return postDrawTProfile2D( data );
  }

  if( dynamic_cast<TProfile*>( data->object ) ) {
    return postDrawTProfile( data );
  }

  if( dynamic_cast<TH2F*>( data->object ) ) {
    return postDrawTH2F( data );
  }
  
  if( dynamic_cast<TH1F*>( data->object ) ) {
    return postDrawTH1F( data );
  }
  
  return ;

}

void DTDigiDisplayPlugins::postDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTDigiDisplayPlugins::postDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTDigiDisplayPlugins::postDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTDigiDisplayPlugins::postDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

