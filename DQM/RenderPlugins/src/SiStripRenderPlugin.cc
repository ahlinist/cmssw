/*!
  \file SiStripRenderPlugin
  \brief Display Plugin for SiStrip DQM Histograms
  \author S. Dutta 
  \version $Revision: 1.1 $
  \date $Date: 2007/12/10 16:20:58 $
*/

#include <TProfile2D.h>

#include <TStyle.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TText.h>

#include "SiStripRenderPlugin.h"


void SiStripRenderPlugin::initialise( int argc, char **argv ) {


}

bool SiStripRenderPlugin::applies( const ObjInfo &o, const ImgInfo &i ) {
 
  if( o.name.find( "SiStrip/" ) == 0 ) {
    return true;
  } 

  return false;

}

void SiStripRenderPlugin::preDraw( TCanvas *c, const ObjInfo &o, const ImgInfo &i, RenderInfo &r ) {

  c->cd();

  if( dynamic_cast<TProfile*>( o.object ) ) {
    preDrawTProfile( c, o );
  }
  else if( dynamic_cast<TH2*>( o.object ) ) {
    preDrawTH2( c, o );
  }
  else if( dynamic_cast<TH1*>( o.object ) ) {
    preDrawTH1( c, o );
  }

}


void SiStripRenderPlugin::preDrawTProfile( TCanvas *c, const ObjInfo &o ) {

  return;

}

void SiStripRenderPlugin::preDrawTH2( TCanvas *c, const ObjInfo &o ) {

  TH2* obj = dynamic_cast<TH2*>( o.object );

  assert( obj );

  // This applies to all
  gStyle->SetCanvasBorderMode( 0 );
  gStyle->SetPadBorderMode( 0 );
  gStyle->SetPadBorderSize( 0 );
  //    (data->pad)->SetLogy( 0 );;
  gStyle->SetOptStat( 0 );
  obj->SetStats( kFALSE );

  TAxis* xa = obj->GetXaxis();
  TAxis* ya = obj->GetYaxis();

  xa->SetTitleOffset(0.7);
  xa->SetTitleSize(0.05);
  xa->SetLabelSize(0.04);

  ya->SetTitleOffset(0.7);
  ya->SetTitleSize(0.05);
  ya->SetLabelSize(0.04);

  
  if( o.name.find( "PedsEvolution" ) < o.name.size() ) {
    gStyle->SetOptStat( 1111 );
    obj->SetStats( kTRUE );
    obj->SetOption( "lego2" );
    return;
  } 
  if( o.name.find( "CMDistribution " ) < o.name.size() ){
    obj->GetXaxis()->LabelsOption("d");
    obj->SetOption( "lego2" );
    return;
  }
  if( o.name.find( "CMSlopeDistribution " ) < o.name.size() ) {
    obj->GetXaxis()->LabelsOption("d");
    obj->SetOption( "lego2" );
    return;
  }

  if( o.name.find( "PedestalDistribution " ) < o.name.size()) {
    obj->GetXaxis()->LabelsOption("d");
    obj->SetOption( "lego" );
    return;
  }

  return;
}

void SiStripRenderPlugin::preDrawTH1( TCanvas *c, const ObjInfo &o ) {

  TH1* obj = dynamic_cast<TH1*>( o.object );

  assert( obj );

  // This applies to all
  gStyle->SetOptStat(0111);
  if ( obj->GetMaximum(1.e5) > 0. ) {
    gPad->SetLogy(1);
  } else {
   gPad->SetLogy(0);
  }

}

void SiStripRenderPlugin::postDraw( TCanvas *c, const ObjInfo &o, const ImgInfo &i ) {

#ifdef DEBUG
  std::cout << "SiStripRenderPlugin:postDraw " << o.name << std::endl;
#endif

  c->cd();

  if( dynamic_cast<TProfile*>( o.object ) ) {
    postDrawTProfile( c, o );
  } else if( dynamic_cast<TH2*>( o.object ) ) {
    postDrawTH2( c, o );
  } else if( dynamic_cast<TH1*>( o.object ) ) {
    postDrawTH2( c, o );
  }


#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}

void SiStripRenderPlugin::postDrawTProfile( TCanvas *c, const ObjInfo &o ) {

  return;

}

void SiStripRenderPlugin::postDrawTH2( TCanvas *c, const ObjInfo &o ) {

  return;

}

void SiStripRenderPlugin::postDrawTH1( TCanvas *c, const ObjInfo &o ) {

  TText tt;
  tt.SetTextSize(0.12);
  if (o.flags == 0) return;
  else {
    if (FLAG_ERROR) {
      tt.SetTextColor(2);
      tt.DrawTextNDC(0.5, 0.5, "Error");
    } else if (FLAG_WARNING) {
      tt.SetTextColor(5);
      tt.DrawTextNDC(0.5, 0.5, "Warning");
    } else if (FLAG_REPORT) { 
      tt.SetTextColor(1);
      tt.DrawTextNDC(0.5, 0.5, "Other ");      
    } else {
      tt.SetTextColor(3);
      tt.DrawTextNDC(0.5, 0.5, "Ok ");
    }      
  }  

  return;

}

