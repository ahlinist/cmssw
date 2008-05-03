// -*- C++ -*-
//
// Package:     RenderPlugins
// Class  :     HLXRenderPlugin
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  
//         Created:  Sat Apr 19 20:02:57 CEST 2008
// $Id: HLXRenderPlugin.cc,v 1.1 2008/05/03 19:03:28 neadam Exp $
//

// system include files
#include <iostream>

// user include files
#include "DQM/RenderPlugins/src/HLXRenderPlugin.h"

#include "TH1F.h"
#include "TProfile.h"

#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"

#include "TGraph.h"
#include "TLine.h"
#include "TROOT.h"


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// member functions
//
void HLXRenderPlugin::initialise( int argc, char **argv ) 
{

}

bool HLXRenderPlugin::applies( const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) 
{
  std::cerr << "HLX Render plugin in applies!!!!" << std::endl;
 
  if( o.name.find( "/HLX/HFPlus/Wedge" )  == 0 ){
    return true;
  }
  if( o.name.find( "/HLX/HFMinus/Wedge" ) == 0 ){
    return true;
  }
  if( o.name.find( "/HLX/HFCompare" )     == 0 ){
    return true;
  }
  if( o.name.find( "/HLX/Average" )       == 0 ){ 
    return true;
  }
  return false;
}

void HLXRenderPlugin::preDraw( TCanvas *c, const DQMNet::CoreObject &o, 
			       const VisDQMImgInfo &i, VisDQMRenderInfo &r ) 
{


   c->cd();

   gStyle->Reset("Plain");
   gStyle->SetErrorX(0);
   gStyle->SetEndErrorSize(10);
   gStyle->SetCanvasColor(10);
   gStyle->SetPadColor(10);
   gStyle->SetFillColor(10);
   gStyle->SetOptTitle(kTRUE);
   gStyle->SetTitleBorderSize(0);
   gStyle->SetOptStat(kTRUE);
   gStyle->SetStatBorderSize(1);
   gStyle->SetStatColor(kWhite);
   gStyle->SetTitleFillColor(kWhite);

   gROOT->ForceStyle();

   c->GetPad(0)->SetTicks(1,1);

   if( dynamic_cast<TProfile*>( o.object ) ) {
      preDrawTProfile( c, o );
   }
   else if( dynamic_cast<TH1F*>( o.object ) ) {
      preDrawTH1F( c, o );
   }

   r.drawOptions = "";


}

void HLXRenderPlugin::preDrawTProfile( TCanvas *c, const DQMNet::CoreObject &o ) 
{

   TProfile* obj = dynamic_cast<TProfile*>( o.object );

   assert( obj );

   // Average plots for Etsum/Tower Occupancy
   obj->SetStats(kTRUE);

   return;
}

void HLXRenderPlugin::preDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o ) 
{

  TH1F* obj = dynamic_cast<TH1F*>( o.object );

  assert( obj );

  // Average plots for Etsum/Tower Occupancy
  obj->SetStats(kTRUE);
  
//   if( o.name.find("ETSum") == 0 && o.name.find("HFCompare") != 0 )
//   {
//     gPad->SetLogy();
//   }


  return;
}

// Anything that needs to be done after the histogram is drawn
void HLXRenderPlugin::postDraw( TCanvas *c, const DQMNet::CoreObject &o, 
			       const VisDQMImgInfo &i ) 
{


   // Do nothing here for now ...

//    c->cd();
//    if( dynamic_cast<TProfile*>( o.object ) ) {
//       postDrawTProfile2D( c, o );
//    }
//    else if( dynamic_cast<TH1F*>( o.object ) ) {
//       postDrawTH1F( c, o );
//    }

#ifdef DEBUG
   std::cerr << "done" << std::endl;
#endif

}

//
// const member functions
//

//
// static member functions
//
