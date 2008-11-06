// $Id: L1TdeRCTRenderPlugin.cc,v 1.5 2008/05/27 19:53:06 elmer Exp $

/*!
  \file L1TdeRCTRenderPlugin
  \brief Display Plugin for Quality Histograms
  \author A.Savin 
  \version $Revision: 1.5 $
  \date $Date: 2008/05/27 19:53:06 $
*/

#include <cassert>

#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TProfile2D.h"


#include "TStyle.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TColor.h"
#include "TROOT.h"

#include <iostream>

#include "DQM/RenderPlugins/src/L1TdeRCTRenderPlugin.h"

static bool first = true;

void L1TdeRCTRenderPlugin::initialise( int argc, char **argv ) {

  if( ! first ) return;

  first = false;

  paletteSize = 100 ;
  Float_t rgb[300] = {0} ;

   for (Int_t i = 0; i < paletteSize; ++i){
      rgb[i*3]=1.; rgb[i*3+1]=0.; rgb[i*3+2]=0.;
      if(i <= 15){
      rgb[i*3]=1.; rgb[i*3+1]=0.7; rgb[i*3+2]=0.1;
      }
      if(i <= 10){
      rgb[i*3]=1.; rgb[i*3+1]=1.; rgb[i*3+2]=0.;
      }
      if(i <= 5){
      rgb[i*3]=0.; rgb[i*3+1]=1.; rgb[i*3+2]=0.;
      }
      pIneff[i] = TColor::GetColor(rgb[i * 3], rgb[i * 3 + 1], rgb[i * 3 + 2]);
   }                                                                            

   for (Int_t i = 0; i < paletteSize; ++i){
      rgb[i*3]=1.; rgb[i*3+1]=0.; rgb[i*3+2]=0.;
      if(i <= 15){
      rgb[i*3]=1.; rgb[i*3+1]=0.7; rgb[i*3+2]=0.1;
      }
      if(i <= 10){
      rgb[i*3]=1.; rgb[i*3+1]=1.; rgb[i*3+2]=0.;
      }
      if(i <= 5){
      rgb[i*3]=0.; rgb[i*3+1]=1.; rgb[i*3+2]=0.;
      }
      pOvereff[i] = TColor::GetColor(rgb[i * 3], rgb[i * 3 + 1], rgb[i * 3 + 2]);
   }                                                                            

   for (Int_t i = 0; i < paletteSize; ++i){
      rgb[i*3]=1.; rgb[i*3+1]=0.; rgb[i*3+2]=0.;
      if(i >= 85){
      rgb[i*3]=1.; rgb[i*3+1]=0.7; rgb[i*3+2]=0.1;
      }
      if(i >= 90){
      rgb[i*3]=1.; rgb[i*3+1]=1.; rgb[i*3+2]=0.;
      }
      if(i >= 95){
      rgb[i*3]=0.; rgb[i*3+1]=1.; rgb[i*3+2]=0.;
      }
      pEff[i] = TColor::GetColor(rgb[i * 3], rgb[i * 3 + 1], rgb[i * 3 + 2]);
   }                                                                            

   dummybox = new  TH2F("dummy","",22,-0.5,21.5,18,-0.5,17.5);

   for(int i=0; i<22; i++){
    for(int j=0; j<18; j++){
    dummybox->Fill(i,j) ;
    }
   }

}

bool L1TdeRCTRenderPlugin::applies( const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

#ifdef DEBUG
  std::cout << "L1TdeRCTRenderPlugin:applies " << o.name << std::endl;
#endif

  if( o.name.find( "L1TEMU/L1TdeRCT/IsoEm" ) != std::string::npos ) {
    return true;
  }

  if( o.name.find( "L1TEMU/L1TdeRCT/IsoEm/ServiceData" ) != std::string::npos ) {
    return true;
  }

  if( o.name.find( "L1TEMU/L1TdeRCT/NisoEm" ) != std::string::npos ) {
    return true;
  }

  if( o.name.find( "L1TEMU/L1TdeRCT/NisoEm/ServiceData" ) != std::string::npos ) {
    return true;
  }

  return false;

}

void L1TdeRCTRenderPlugin::preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo &r ) {

#ifdef DEBUG
  std::cout << "L1TdeRCTRenderPlugin:preDraw " << o.name << std::endl;
#endif

  c->cd();

  gStyle->Reset("Default");
  
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(10);
  gStyle->SetFillColor(10);
  gStyle->SetStatColor(10);
  gStyle->SetTitleFillColor(10);
  
  TGaxis::SetMaxDigits(4);
  
  gStyle->SetOptTitle(kTRUE);
  gStyle->SetTitleBorderSize(0);

  gStyle->SetOptStat(kFALSE);
  gStyle->SetStatBorderSize(1);

  gStyle->SetOptFit(kFALSE);

  gROOT->ForceStyle();

  if( dynamic_cast<TProfile2D*>( o.object ) ) {
    preDrawTProfile2D( c, o );
  }
  else if( dynamic_cast<TProfile*>( o.object ) ) {
    preDrawTProfile( c, o );
  }
  else if( dynamic_cast<TH3F*>( o.object ) ) {
    preDrawTH3F( c, o );
  }
  else if( dynamic_cast<TH2F*>( o.object ) ) {
    preDrawTH2F( c, o );
  }
  else if( dynamic_cast<TH1F*>( o.object ) ) {
    preDrawTH1F( c, o );
  }

  r.drawOptions = "";

#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}

void L1TdeRCTRenderPlugin::preDrawTProfile2D( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void L1TdeRCTRenderPlugin::preDrawTProfile( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void L1TdeRCTRenderPlugin::preDrawTH3F( TCanvas *c, const DQMNet::CoreObject &o ) {
  
  return;
  
} 

void L1TdeRCTRenderPlugin::preDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o ) {

  paletteSize = 100 ;

  TH2F* obj = dynamic_cast<TH2F*>( o.object );

  assert( obj );

  std::string name = o.name.substr(o.name.rfind("/")+1);

  gStyle->SetPaintTextFormat();

  gStyle->SetOptStat(kFALSE);
  gPad->SetLogy(kFALSE);

//--Iso stuff

  if( name.find( "rctIsoEmDataOcc" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctIsoEmEmulOcc" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctIsoEmEff1Occ" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctIsoEmEff2Occ" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctIsoEmIneffOcc" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctIsoEmOvereffOcc" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctIsoEmEff1" ) != std::string::npos ) {
    gStyle->SetPalette(paletteSize, pEff);
    obj->SetStats(kFALSE);
    obj->SetOption("colz");
    return;
  }

  if( name.find( "rctIsoEmEff2" ) != std::string::npos ) {
    gStyle->SetPalette(paletteSize, pEff);
    obj->SetStats(kFALSE);
    obj->SetOption("colz");
    return;
  }

  if( name.find( "rctIsoEmIneff" ) != std::string::npos ) {
    gStyle->SetPalette(paletteSize, pIneff);
    obj->SetStats(kFALSE);
    obj->SetOption("colz");
    return;
  }

  if( name.find( "rctIsoEmOvereff" ) != std::string::npos ) {
    gStyle->SetPalette(paletteSize, pOvereff);
    obj->SetStats(kFALSE);
    obj->SetOption("colz");
    return;
  }

//--Niso stuff

  if( name.find( "rctNisoEmDataOcc" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctNisoEmEmulOcc" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctNisoEmEff1Occ" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctNisoEmEff2Occ" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctNisoEmIneffOcc" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctNisoEmOvereffOcc" ) != std::string::npos ) {
    obj->SetOption("box");
    return;
  }

  if( name.find( "rctNisoEmEff1" ) != std::string::npos ) {
    gStyle->SetPalette(paletteSize, pEff);
    obj->SetStats(kFALSE);
    obj->SetOption("colz");
    return;
  }

  if( name.find( "rctNisoEmEff2" ) != std::string::npos ) {
    gStyle->SetPalette(paletteSize, pEff);
    obj->SetStats(kFALSE);
    obj->SetOption("colz");
    return;
  }

  if( name.find( "rctNisoEmIneff" ) != std::string::npos ) {
    gStyle->SetPalette(paletteSize, pIneff);
    obj->SetStats(kFALSE);
    obj->SetOption("colz");
    return;
  }

  if( name.find( "rctNisoEmOvereff" ) != std::string::npos ) {
    gStyle->SetPalette(paletteSize, pOvereff);
    obj->SetStats(kFALSE);
    obj->SetOption("colz");
    return;
  }


}

void L1TdeRCTRenderPlugin::preDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void L1TdeRCTRenderPlugin::postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

#ifdef DEBUG
  std::cout << "L1TdeRCTRenderPlugin:postDraw " << o.name << std::endl;
#endif

  c->cd();

  if( dynamic_cast<TProfile2D*>( o.object ) ) {
    postDrawTProfile2D( c, o );
  }
  else if( dynamic_cast<TH3F*>( o.object ) ) {
    postDrawTH3F( c, o );
  }
  else if( dynamic_cast<TH2F*>( o.object ) ) {
    postDrawTH2F( c, o );
  }
  else if( dynamic_cast<TH1F*>( o.object ) ) {
    postDrawTH1F( c, o );
  }

#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}

void L1TdeRCTRenderPlugin::postDrawTProfile2D( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void L1TdeRCTRenderPlugin::postDrawTH3F( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void L1TdeRCTRenderPlugin::postDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH2F* obj = dynamic_cast<TH2F*>( o.object );

  assert( obj );

  std::string name = o.name.substr(o.name.rfind("/")+1);

  if( name.find( "rctIsoEmEff1" ) != std::string::npos &&
  name.find( "rctIsoEmEff1" ) != name.find( "rctIsoEmEff1Occ" )) {
    dummybox->Draw("box,same");
    return;
  }

  if( name.find( "rctIsoEmEff2" ) != std::string::npos &&
  name.find( "rctIsoEmEff2" ) != name.find( "rctIsoEmEff2Occ" ) ) {
    dummybox->Draw("box,same");
    return;
  }

  if( name.find( "rctIsoEmIneff" ) != std::string::npos &&
  name.find( "rctIsoEmIneff" ) != name.find( "rctIsoEmIneffOcc" ) ) {
    dummybox->Draw("box,same");
    return;
  }

  if( name.find( "rctIsoEmOvereff" ) != std::string::npos &&
  name.find( "rctIsoEmOvereff" ) != name.find( "rctIsoEmOvereffOcc" ) ) {
    dummybox->Draw("box,same");
    return;
  }

  if( name.find( "rctNisoEmEff1" ) != std::string::npos &&
  name.find( "rctNisoEmEff1" ) != name.find( "rctNisoEmEff1Occ") ) {
    dummybox->Draw("box,same");
    return;
  }

  if( name.find( "rctNisoEmEff2" ) != std::string::npos &&
  name.find( "rctNisoEmEff2" ) != name.find( "rctNisoEmEff2Occ" ) ) {
    dummybox->Draw("box,same");
    return;
  }

  if( name.find( "rctNisoEmIneff" ) != std::string::npos &&
  name.find( "rctNisoEmIneff" ) != name.find( "rctNisoEmIneffOcc" ) ) {
    dummybox->Draw("box,same");
    return;
  }

  if( name.find( "rctNisoEmOvereff" ) != std::string::npos &&
  name.find( "rctNisoEmOvereff" ) != name.find( "rctNisoEmOvereffOcc" ) ) {
    dummybox->Draw("box,same");
    return;
  }

}

void L1TdeRCTRenderPlugin::postDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

