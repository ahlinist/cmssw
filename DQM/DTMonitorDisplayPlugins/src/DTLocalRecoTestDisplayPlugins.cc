
/*!
  \file DTLocalRecoTestDisplayPlugins
  \brief Display Plugin for Local Recontruction Client test Quality Histograms
  \author G. Mila
  \version $Revision: 1.1 $
  \date $Date: 2007/04/03 09:30:52 $
*/

#include "DQM/DTMonitorDisplayPlugins/src/DTLocalRecoTestDisplayPlugins.h"
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



DTLocalRecoTestDisplayPlugins::DTLocalRecoTestDisplayPlugins () {

}

bool DTLocalRecoTestDisplayPlugins::isLocalRecoTestME (std::string name) {


  if( name.find( "MeanTest" ) == 0 ) {
    return true;
  }

  if( name.find( "SigmaTest" ) == 0 ) {
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


std::string DTLocalRecoTestDisplayPlugins::preDraw( VisDQMDisplayPlugin::DisplayData *data ) {

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


std::string DTLocalRecoTestDisplayPlugins::preDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTLocalRecoTestDisplayPlugins::preDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTLocalRecoTestDisplayPlugins::preDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";    

}

std::string DTLocalRecoTestDisplayPlugins::preDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

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
    
    TAttLine *line = dynamic_cast<TAttLine *> (data->object);
    
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
  
  return "";    
  
}


void DTLocalRecoTestDisplayPlugins::postDraw( VisDQMDisplayPlugin::DisplayData *data ) {

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

void DTLocalRecoTestDisplayPlugins::postDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTLocalRecoTestDisplayPlugins::postDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTLocalRecoTestDisplayPlugins::postDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTLocalRecoTestDisplayPlugins::postDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

