

// $Id: DTLocalTriggerDisplayPlugins.cc,v 1.2 2007/06/01 15:23:02 mzanetti Exp $

/*!
  \file DTLocalTriggerDisplayPlugins
  \brief Display Plugin for LocalTrigger Task Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.2 $
  \date $Date: 2007/06/01 15:23:02 $
*/

#include "DQM/DTMonitorDisplayPlugins/src/DTLocalTriggerDisplayPlugins.h"
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



DTLocalTriggerDisplayPlugins::DTLocalTriggerDisplayPlugins () {

}

bool DTLocalTriggerDisplayPlugins::isLocalTriggerME (std::string name) {

  if( name.find( "BXvsQual" ) == 4 ) {
    return true;
  }

  if( name.find( "ThetaBXvsQual" ) == 4 ) {
    return true;
  }

  if( name.find( "Flag1" ) == 4 ) {
    return true;
  }
   
  if( name.find( "Qualvs" ) == 4 ) {
    return true;
  }

  if( name.find( "Positionvs" ) == 4 ) {
    return true;
  }

  if( name.find( "TrigEff" ) == 0 ) {
    return true;
  }
  
  if( name.find( "CorrectBX" ) == 0 ) {
    return true;
  }

  if( name.find( "CorrFrac" ) == 0 ) {
    return true;
  }

  if( name.find( "2ndFrac" ) == 0 ) {
    return true;
  }

  if( name.find( "HFrac" ) == 0 ) {
    return true;
  }

  return false;

}


std::string DTLocalTriggerDisplayPlugins::preDraw( VisDQMDisplayPlugin::DisplayData *data ) {

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


std::string DTLocalTriggerDisplayPlugins::preDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTLocalTriggerDisplayPlugins::preDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTLocalTriggerDisplayPlugins::preDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

  TH2F* obj = dynamic_cast<TH2F*>( data->object );

  name = (data->object)->GetName();

  if( obj ) {

    // This applies to all
    gStyle->SetCanvasBorderMode( 0 );
    gStyle->SetPadBorderMode( 0 );
    gStyle->SetPadBorderSize( 0 );
    (data->pad)->SetLogy( 0 );
    //    gStyle->SetOptStat( 0 );
    obj->SetStats(kFALSE );
    obj->SetLineColor(2);
    gStyle->SetPalette(1);
    

//     TColor* color;
//     const Int_t n = 100;
//     int pCol4[n];
//     for( Int_t i=0; i<n; i++ ) {
//       Float_t grey = Float_t((i*100.)/(10000));
//       color = new TColor(401+i,grey,1.-grey,0.,"");
//       pCol4[i] = i+401;
//     }
//     gStyle->SetPalette( n, pCol4 );
    obj->SetOption( "colz" );
    //    obj->SetOption( "lego" );

  if( name.find( "BXvs" ) == 4 ) {
    (data->pad)->SetLogz( 1 );

    //    return true;
  }
  if( name.find( "Theta" ) == 4 ) {
    (data->pad)->SetLogz( 1 );

    //    return true;
  }


  }

  return "";    

}

std::string DTLocalTriggerDisplayPlugins::preDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

  TH1F* obj = dynamic_cast<TH1F*>( data->object );

  name = (data->object)->GetName();

  if( obj ) {

      //     // This applies to all
      gStyle->SetCanvasBorderMode( 0 );
      gStyle->SetPadBorderMode( 0 );
      gStyle->SetPadBorderSize( 0 );
      //      (data->pad)->SetLogy( 1 );
      gStyle->SetOptStat( 0 );
      obj->SetStats(kFALSE);
      obj->SetLineColor(4);
      obj->GetXaxis()->LabelsOption("hd");
      
  }
  
  if( name.find("TrigEff")==0 ) {
    obj->GetYaxis()->SetRangeUser(0.,1.1);
    
  }

  return "";    

}


void DTLocalTriggerDisplayPlugins::postDraw( VisDQMDisplayPlugin::DisplayData *data ) {

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

void DTLocalTriggerDisplayPlugins::postDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTLocalTriggerDisplayPlugins::postDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTLocalTriggerDisplayPlugins::postDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTLocalTriggerDisplayPlugins::postDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

