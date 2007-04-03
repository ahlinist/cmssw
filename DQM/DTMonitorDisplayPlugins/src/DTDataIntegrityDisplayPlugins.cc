// $Id: DTDataIntegrityDisplayPlugins.cc,v 1.1 2007/03/29 09:10:24 gmasetti Exp $

/*!
  \file DTDataIntegrityDisplayPlugins
  \brief Display Plugin for Data Integrity Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.1 $
  \date $Date: 2007/03/29 09:10:24 $
*/

#include "DQM/DTMonitorDisplayPlugins/src/DTDataIntegrityDisplayPlugins.h"
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



DTDataIntegrityDisplayPlugins::DTDataIntegrityDisplayPlugins () {

}

bool DTDataIntegrityDisplayPlugins::isDataIntegrityME (std::string name) {

  if( name.find( "FED770" ) == 0 ) {
    return true;
  }

  return false;

}


std::string DTDataIntegrityDisplayPlugins::preDraw( VisDQMDisplayPlugin::DisplayData *data ) {

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


std::string DTDataIntegrityDisplayPlugins::preDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTDataIntegrityDisplayPlugins::preDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTDataIntegrityDisplayPlugins::preDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

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

std::string DTDataIntegrityDisplayPlugins::preDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

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


      if ( name.find( "FED770_DDUChannel" ) == 0 ) {
	
	(data->pad)->SetLogy( 1 );
	
	obj->GetXaxis()->SetBinLabel(1,"ERR1");
	obj->GetXaxis()->SetBinLabel(2,"ERR2");
	obj->GetXaxis()->SetBinLabel(3,"ERR3");
	obj->GetXaxis()->SetBinLabel(4,"ERR4");
	obj->GetXaxis()->SetBinLabel(5,"ERR5");
	obj->GetXaxis()->SetBinLabel(6,"ERR6");
	obj->GetXaxis()->SetBinLabel(7,"ERR7");
	obj->GetXaxis()->SetBinLabel(8,"ERR8");

      }
  }
  
  return "";    

}


void DTDataIntegrityDisplayPlugins::postDraw( VisDQMDisplayPlugin::DisplayData *data ) {

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

void DTDataIntegrityDisplayPlugins::postDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTDataIntegrityDisplayPlugins::postDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTDataIntegrityDisplayPlugins::postDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTDataIntegrityDisplayPlugins::postDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

