

/*!
  \file DTLocalRecoDisplayPlugins
  \brief Display Plugin for Data Local Reconstruction Histograms (2D)
  \author G. Mila 
  \version $Revision: 1.1 $
  \date $Date: 2007/04/03 09:28:46 $
*/

#include "DQM/DTMonitorDisplayPlugins/src/DTLocalRecoDisplayPlugins.h"
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



DTLocalRecoDisplayPlugins::DTLocalRecoDisplayPlugins () {

}

bool DTLocalRecoDisplayPlugins::isLocalRecoME (std::string name) {

  if( name.find( "h4DSeg" ) == 0 ) {
    return true;
  }

  if( name.find( "hResDist" ) == 0 ) {
    return true;
  }

  if( name.find( "Efficiency" ) == 0 ) {
    return true;
  }
  
  if( name.find( "UnassEfficiency" ) == 0 ) {
    return true;
  }
  
  return false;

}


std::string DTLocalRecoDisplayPlugins::preDraw( VisDQMDisplayPlugin::DisplayData *data ) {

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


std::string DTLocalRecoDisplayPlugins::preDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTLocalRecoDisplayPlugins::preDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTLocalRecoDisplayPlugins::preDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

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

std::string DTLocalRecoDisplayPlugins::preDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

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


void DTLocalRecoDisplayPlugins::postDraw( VisDQMDisplayPlugin::DisplayData *data ) {

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

void DTLocalRecoDisplayPlugins::postDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTLocalRecoDisplayPlugins::postDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTLocalRecoDisplayPlugins::postDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTLocalRecoDisplayPlugins::postDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

