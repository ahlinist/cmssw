#include "TProfile2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include <iostream>
#include <cassert>
#include "TLine.h"
#include "TText.h"

#include "DQM/RenderPlugins/src/RPCRenderPlugin.h"
#include "DQM/RenderPlugins/src/utils.h"

bool RPCRenderPlugin::applies( const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

   if( (o.name.find( "RPC/RecHits/SummaryHistograms" ) == 0 )||
       (o.name.find("RPC/EventInfo")!= std::string::npos)) return true;

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

void RPCRenderPlugin::postDraw(TCanvas *c,const DQMNet::CoreObject &o,const VisDQMImgInfo &i){
  c->cd();

  TText tt;
  tt.SetTextSize(0.12);

  if(o.name.find("Endcap") != std::string::npos && o.name.find("Barrel") == std::string::npos  ){
    tt.SetTextColor(3);
    tt.DrawTextNDC(0.5, 0.5, "OK-Not in Run");
    return;
  }


  if(dynamic_cast<TH2*>( o.object ) ) postDrawTH2(c,o); 

}

void RPCRenderPlugin::preDrawTH2( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH2F* obj = dynamic_cast<TH2F*>( o.object );

  assert( obj );

  // This applies to all
  gStyle->SetCanvasBorderMode( 0 );
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetPadBorderMode( 0 );
  gStyle->SetPadBorderSize( 0 );
  gStyle->SetOptStat( 0 );
  gStyle->SetPalette( 1 );
  obj->SetStats( kFALSE );

  if(o.name.find("reportSummaryMap") != std::string::npos){
    dqm::utils::reportSummaryMapPalette(obj);
    gStyle->SetOptStat( 10 );
    gStyle->SetPaintTextFormat("%.2f");
    obj->SetOption("colztext");
    obj->SetStats( kTRUE );
    return;
  }

  //obj->SetOption( "box" );
  gStyle->SetPalette(1);
  obj->SetOption( "colz" );

  if( o.name.find("Occupancy") < o.name.size() && o.name.find("_Sector")!= std::string::npos) {
    gStyle->SetOptStat( 10 );
    gStyle->SetPalette( 1 );
    obj->SetStats( kTRUE );
    return;
  }
  return;
}

void  RPCRenderPlugin::postDrawTH2(TCanvas *c, const DQMNet::CoreObject &o){
 TH2* obj = dynamic_cast<TH2*>( o.object );
 assert( obj );

 if(o.name.find("reportSummaryMap") != std::string::npos){
   TLine line;
   line.SetLineWidth(1);

   line.DrawLine(0.5, 0.5, 0.5, 6.5);
   line.DrawLine(4.5, 0.5, 4.5, 6.5);
   line.DrawLine(5.5, 0.5, 5.5, 12.5);
   line.DrawLine(10.5, 0.5, 10.5, 12.5);
   line.DrawLine(11.5, 0.5, 11.5, 6.5);
   line.DrawLine(15.5, 0.5, 15.5, 6.5);

   line.DrawLine(0.5, 6.5, 4.5, 6.5);
   line.DrawLine(5.5, 12.5, 10.5, 12.5);
   line.DrawLine(11.5, 6.5, 15.5, 6.5);
 }
 return;
}
