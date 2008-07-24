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

   if( (o.name.find( "RPC/RecHits/" ) == 0 )||
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
    tt.DrawTextNDC(0.5, 0.5, "OK");
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
  gStyle->SetOptStat( 10 );
  gStyle->SetPalette( 1 );
  obj->SetOption( "colz" );  
  obj->SetStats( kFALSE );

  if(o.name.find("reportSummaryMap") != std::string::npos){
    dqm::utils::reportSummaryMapPalette(obj);
    gStyle->SetPaintTextFormat("%.2f");
    obj->SetOption("colztext");
    obj->SetStats( kTRUE );
    return;
  }

  if( o.name.find("Occupancy") < o.name.size() ) {
    obj->SetStats( kTRUE );
    return;
  }
  return;
}

void  RPCRenderPlugin::postDrawTH2(TCanvas *c, const DQMNet::CoreObject &o){
 TH2* obj = dynamic_cast<TH2*>( o.object );
 assert( obj );

 if(o.name.find("reportSummaryMap") != std::string::npos){

   obj->GetXaxis()->SetNdivisions(16,true);
   obj->GetYaxis()->SetNdivisions(13,true);
   obj->GetXaxis()->CenterLabels();
   obj->GetYaxis()->CenterLabels();
   c->SetGrid(1,1);
   
   TLine line;
   line.SetLineWidth(1);
   line.DrawLine(-3.5, 0.5, -3.5, 6.5);
   line.DrawLine(-7.5, 6.5,-3.5, 6.5 );
   line.DrawLine(-2.5, 0.5, -2.5, 12.5);
   line.DrawLine(2.5, 0.5, 2.5, 12.5);
   line.DrawLine(-2.5, 12.5, 2.5, 12.5);

   line.DrawLine(3.5, 0.5, 3.5, 6.5);
   line.DrawLine(3.5, 6.5,7.5, 6.5 );
   line.DrawLine(7.5, 0.5,7.5, 6.5 );
 }

if(o.name.find("Occupancy") != std::string::npos){
   TLine line;
   line.SetLineWidth(2);
   //rb1in
   line.DrawLine(91, 0.5, 91, 2.5);
   line.DrawLine(91, 2.5, 85, 2.5);
   //rb1out
   line.DrawLine(85, 2.5, 85, 4.5);
   line.DrawLine(85, 4.5, 91, 4.5);
   //rb2in X 3
   line.DrawLine(91, 4.5, 91, 7.5);
   line.DrawLine(91, 7.5, 85, 7.5);

   //rb2out
   line.DrawLine(85, 7.5, 85, 9.5);
   line.DrawLine(85, 9.5, 43, 9.5);

   //rb3
   line.DrawLine(43, 9.5, 43, 13.5);


 }

 return;
}
