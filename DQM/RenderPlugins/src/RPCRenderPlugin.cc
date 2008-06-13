#include "TProfile2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include <iostream>
#include <cassert>
//#include "DQMServices/Core/interface/MonitorElement.h"

#include "DQM/RenderPlugins/src/RPCRenderPlugin.h"


bool RPCRenderPlugin::applies( const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

   if( o.name.find( "RPC/RecHits/SummaryHistograms" ) == 0 ) {
    
     return true;
  } 

   return false;
  
}

void RPCRenderPlugin::preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo &r ) {

#ifdef DEBUG
  std::cout << "RPCRenderPlugin:preDraw " << o.name << std::endl;
#endif

  c->cd();

 
  if( dynamic_cast<TH2*>( o.object ) ) {
    preDrawTH2( c, o );
  }
  

#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}


void RPCRenderPlugin::preDrawTH2( TCanvas *c, const DQMNet::CoreObject &o ) {

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

  if( o.name.find("Occupancy") < o.name.size() ) {
    gStyle->SetOptStat( 10 );
    gStyle->SetPalette( 1 );
    obj->SetStats( kTRUE );
    return;
  }

  return;


}
