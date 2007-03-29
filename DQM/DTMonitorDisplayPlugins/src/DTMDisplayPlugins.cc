// $Id: DTMDisplayPlugins.cc,v 1.1 2007/03/29 15:24:00 $

/*!
  \file DTMDisplayPlugins
  \brief Display Plugin for Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.1 $
  \date $Date: 2007/03/29 15:24:00 $
*/

#include "DQM/DTMonitorDisplayPlugins/interface/DTMDisplayPlugins.h"
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



DTMDisplayPlugins::DTMDisplayPlugins( IgState *state ) : VisDQMDisplayPlugin( state ) {

}

bool DTMDisplayPlugins::applies( DisplayData *data ) {
 
   name = (data->object)->GetName();

   if( name.find( "Occupancy" ) == 0 ) {
     return true;
   }
   if( name.find( "TimeBoxes" ) == 0 ) {
     return true;
   }

   if( name.find( "FED770" ) == 0 ) {
     return true;
   }

   if( name.find( "tTrigTest" ) == 0 ) {
     return true;
   }


   return false;


}

std::string DTMDisplayPlugins::preDraw( DisplayData *data ) {

  if( dynamic_cast<TH2F*>( data->object ) ) {
    return preDrawTH2F( data );
  }
  
  if( dynamic_cast<TH1F*>( data->object ) ) {
    return preDrawTH1F( data );
  }
  
  return "";
  
}


std::string DTMDisplayPlugins::preDrawTH2F( DisplayData *data ) {

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


std::string DTMDisplayPlugins::preDrawTH1F( DisplayData *data ) {

  TH1F* obj = dynamic_cast<TH1F*>( data->object );

  name = (data->object)->GetName();

  if( obj ) {

      //     // This applies to all
      gStyle->SetCanvasBorderMode( 0 );
      gStyle->SetPadBorderMode( 0 );
      gStyle->SetPadBorderSize( 0 );
      //      (data->pad)->SetLogy( 1 );
      gStyle->SetOptStat( 0 );
      obj->SetStats( kFALSE );
      
    if ( name.find( "FED770_DDUChannel" ) == 0 ) {

//       //     // This applies to all
//       gStyle->SetCanvasBorderMode( 0 );
//       gStyle->SetPadBorderMode( 0 );
//       gStyle->SetPadBorderSize( 0 );
      (data->pad)->SetLogy( 1 );
//       gStyle->SetOptStat( 0 );
//       obj->SetStats( kFALSE );
      
      //     obj->SetOption( "box" );
      
      obj->GetXaxis()->SetBinLabel(1,"ERR1");
      obj->GetXaxis()->SetBinLabel(2,"ERR2");
      obj->GetXaxis()->SetBinLabel(3,"ERR3");
      obj->GetXaxis()->SetBinLabel(4,"ERR4");
      obj->GetXaxis()->SetBinLabel(5,"ERR5");
      obj->GetXaxis()->SetBinLabel(6,"ERR6");
      obj->GetXaxis()->SetBinLabel(7,"ERR7");
      obj->GetXaxis()->SetBinLabel(8,"ERR8");

    }

    if ( name.find( "tTrigTest" ) == 0 ) {
      
      TAttLine *line = dynamic_cast<TAttLine *> (data->object);

      obj->GetXaxis()->SetBinLabel(1,"SL1");
      obj->GetXaxis()->SetBinLabel(2,"SL2");
      obj->GetXaxis()->SetBinLabel(3,"SL3");

      if (line) {

	MonitorElement* me = data->me;

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

    }

  }

  return "";    

}



void DTMDisplayPlugins::postDraw( DisplayData *data ) {

    return;    

}

