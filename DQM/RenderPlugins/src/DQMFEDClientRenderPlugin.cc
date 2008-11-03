/*!
  \file DQMFEDCLientRenderPlugin.cc
  \\
  \\ Code shamelessly borrowed from J. Temple's HcalRenderPlugin.cc code, 
  \\ which was shamelessly borrowed from S. Dutta's SiStripRenderPlugin.cc 
  \\ code, G. Della Ricca and B. Gobbo's EBRenderPlugin.cc, and other existing
  \\ subdetector plugins
  \\ preDraw and postDraw methods now check whether histogram was a TH1
  \\ or TH2, and call a private method appropriate for the histogram type
*/

#include "DQM/RenderPlugins/src/DQMFEDClientRenderPlugin.h"

#include "TProfile2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"

#include <cassert>
#include "DQM/RenderPlugins/src/utils.h"

// ------------------------------------------------------------------
void DQMFEDClientRenderPlugin::initialise (int argc, char ** argv) {
  return;
}

// -------------------------------------------------------------------------------------------
bool DQMFEDClientRenderPlugin::applies(const DQMNet::CoreObject &o, const VisDQMImgInfo &i) {

  // determine whether core object is an L1T object
#ifdef DEBUG
  std::cout << "DQMFEDClientRenderPlugin:applies " << o.name << std::endl; 
#endif
  if(o.name.find( "FED/" ) != std::string::npos) {
    return true;
  }

  return false;

}


// ------------------------------------------------------------------------------------------------------------------------------
void DQMFEDClientRenderPlugin::preDraw (TCanvas * c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo &r) {

#ifdef DEBUG 
  std::cout << "DQMFEDClientRenderPlugin:preDraw " << o.name << std::endl; 
#endif 
  c->cd(); 

  // object is TH2 histogram
  if( dynamic_cast<TH2F*>( o.object ) ) {
    preDrawTH2F( c, o ); 
  }

  // object is TH1 histogram
  else if( dynamic_cast<TH1F*>( o.object ) ) {
    preDrawTH1F( c, o ); 
  }

  else
#ifdef DEBUG
    std::cout << "DQMFEDClientRenderPlugin:preDraw  -- Cannot identify object " << o.name << std::endl; 
#endif

  return;

}

// ----------------------------------------------------------------------------------------------------------------------------
void DQMFEDClientRenderPlugin::postDraw (TCanvas * c, const DQMNet::CoreObject & o, const VisDQMImgInfo & i) {

#ifdef DEBUG
  std::cout << "DQMFEDClientRenderPlugin:postDraw " << o.name << std::endl; 
#endif

  // object is TH2 histogram
  if( dynamic_cast<TH2F*>( o.object ) ) {
    postDrawTH2F( c, o ); 
  }

  // object is TH1 histogram
  else if( dynamic_cast<TH1F*>( o.object ) ) {
    postDrawTH1F( c, o ); 
  }

  else
#ifdef DEBUG
    std::cout << "DQMFEDClientRenderPlugin:postDraw  -- Cannot identify object " << o.name << std::endl; 
#endif

  return;

}


// private functions

// --------------------------------------------------------------------------------------
void DQMFEDClientRenderPlugin::preDrawTH1F ( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

// --------------------------------------------------------------------------------------
void DQMFEDClientRenderPlugin::preDrawTH2F ( TCanvas *c, const DQMNet::CoreObject &o ) {  

  TH2F* obj = dynamic_cast<TH2F*>( o.object ); 
  assert( obj ); 

  //put in preDrawTH2F
  if( o.name.find( "reportSummaryMap" )  != std::string::npos) {

    obj->SetStats( kFALSE );
    dqm::utils::reportSummaryMapPalette(obj);
    obj->SetOption("colz");
    obj->SetTitle("FED Report Summary Map");

    obj->GetXaxis()->SetNdivisions(1,true);
    obj->GetYaxis()->SetNdivisions(7,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    
    gPad->SetGrid(1,1);

    return;

  }

  gStyle->SetCanvasBorderMode( 0 );
  gStyle->SetPadBorderMode( 0 );
  gStyle->SetPadBorderSize( 0 );

  return;

}

// --------------------------------------------------------------------------------------
void DQMFEDClientRenderPlugin::postDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

// --------------------------------------------------------------------------------------
void DQMFEDClientRenderPlugin::postDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}
