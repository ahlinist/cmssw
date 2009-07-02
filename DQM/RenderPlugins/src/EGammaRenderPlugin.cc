#include "TProfile2D.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include <cassert>

#include "DQM/RenderPlugins/src/EGammaRenderPlugin.h"
#include "DQM/RenderPlugins/src/utils.h"

bool EGammaRenderPlugin::applies( const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

  if (o.name.find( "PhotonAnalyzer/" )   == std::string::npos) return false;

  if( o.name.find( "/Efficiencies/" ) != std::string::npos ) {
    return true;
  } 

  if( o.name.find( "/AllPhotons/" ) != std::string::npos ) {
    return true;
  } 

  if( o.name.find( "/GoodCandidatePhotons/" ) != std::string::npos ) {
    return true;
  } 

  if( o.name.find( "/BackgroundPhotons/" ) != std::string::npos ) {
    return true;
  } 

  if( o.name.find( "/InvMass/" ) != std::string::npos ) {
    return true;
  }


  return false;

}

void EGammaRenderPlugin::preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo &r ) {

  c->cd();

  if( dynamic_cast<TH2F*>( o.object ) ) {
    preDrawTH2F( c, o );
  }
  else if( dynamic_cast<TH1F*>( o.object ) ) {
    preDrawTH1F( c, o );
  }
  else if( dynamic_cast<TProfile*>( o.object ) ) {
    preDrawTProfile( c, o );
  }

}


void EGammaRenderPlugin::preDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH2F* obj = dynamic_cast<TH2F*>( o.object );
  assert( obj );


  gStyle->SetPalette(1);
  gStyle->SetOptStat("e");
  obj->SetOption( "colz" );

  return;
}

void EGammaRenderPlugin::preDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH1F* obj = dynamic_cast<TH1F*>( o.object );
  assert( obj );


  gStyle->SetOptStat("emr");

  if( o.name.find( "nPho" )  != std::string::npos) {
    gStyle->SetOptStat("em");
  }
  if( o.name.find( "nConv" )  != std::string::npos) {
    gStyle->SetOptStat("em");
  }
  if( o.name.find( "nIsoTracks" )  != std::string::npos) {
    gStyle->SetOptStat("em");
  }
  if( o.name.find( "phoEta" )  != std::string::npos) {
    gStyle->SetOptStat("e");
  }
  if( o.name.find( "phoConvEta" )  != std::string::npos) {
    gStyle->SetOptStat("e");
  }
  if( o.name.find( "phoPhi" )  != std::string::npos) {
    gStyle->SetOptStat("e");
  }
  if( o.name.find( "phoConvPhi" )  != std::string::npos) {
    gStyle->SetOptStat("e");
  }
  if( o.name.find( "VsEta" )  != std::string::npos) {
    gStyle->SetOptStat("e");
  }


  return;
}


void EGammaRenderPlugin::preDrawTProfile( TCanvas *c, const DQMNet::CoreObject &o ) {


  TProfile* obj = dynamic_cast<TProfile*>( o.object );
  assert( obj );


  gStyle->SetOptStat("em");


  return;
}



void EGammaRenderPlugin::postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

  c->cd();
  
  if( dynamic_cast<TH1F*>( o.object ) ) {
    postDrawTH1F( c, o );
  }

  if( dynamic_cast<TH2F*>( o.object ) ) {
    postDrawTH2F( c, o );
  }

}

void EGammaRenderPlugin::postDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH1F* obj = dynamic_cast<TH1F*>( o.object );
  assert( obj );
  //gStyle->SetOptStat(11);
  obj->SetMinimum(0);

  if( o.name.find( "Filters" ) != std::string::npos ) {    
    c->SetBottomMargin(0.25);
    c->SetRightMargin(0.35);
    obj->SetStats(kFALSE);
    obj->SetMaximum(1.05);
  }

  if( o.name.find( "hOverE" )  != std::string::npos) {
    c->SetLogy(1);
    obj->SetMinimum(0.5);
  }
  if( o.name.find( "h1OverE" )  != std::string::npos) {
    c->SetLogy(1);
    obj->SetMinimum(0.5);
  }
  if( o.name.find( "h2OverE" )  != std::string::npos) {
    c->SetLogy(1);
    obj->SetMinimum(0.5);
  }


  return;

}


void EGammaRenderPlugin::postDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH2F* obj = dynamic_cast<TH2F*>( o.object );
  assert( obj );
  //gStyle->SetOptStat("e");





  return;

}



