/*!
  \file L1TRPCTFRenderPlugin.cc
  \\
  \\ Code shamelessly taken from, which was shamelessly borrowed 
  \\ from J. Temple's HcalRenderPlugin.cc code, which was shamelessly 
  \\ borrowed from S. Dutta's SiStripRenderPlugin.cc  code, G. Della Ricca 
  \\ and B. Gobbo's EBRenderPlugin.cc, and other existing subdetector plugins
*/

#include "DQM/RenderPlugins/src/L1TRPCTFRenderPlugin.h" 

#include "TProfile2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include "TLine.h"

#include <cassert>
#include "DQM/RenderPlugins/src/utils.h"

void L1TRPCTFRenderPlugin::initialise (int argc, char ** argv)
{
  // same as RenderPlugin default for now (no special action taken)
  return;
}



bool L1TRPCTFRenderPlugin::applies(const DQMNet::CoreObject &o,
				  const VisDQMImgInfo &i)
{
  // determine whether core object is an L1TEMU object
#ifdef DEBUG 
  std::cout << "L1TRPCTFRenderPlugin:applies " << o.name << std::endl; 
#endif 
  if (o.name.find( "L1TRPCTF/" ) != std::string::npos )
    {
      return true;
    }
  
  return false;
}



void L1TRPCTFRenderPlugin::preDraw (TCanvas * c,
			      const DQMNet::CoreObject &o,
			      const VisDQMImgInfo &i,
			      VisDQMRenderInfo &r)
{

#ifdef DEBUG 
  std::cout << "L1TRPCTFRenderPlugin:preDraw " << o.name << std::endl; 
#endif 
  c->cd(); 
 
  // object is TH2 histogram
  if( dynamic_cast<TH2F*>( o.object ) )
    { 
      preDrawTH2F( c, o ); 
    } 

  // object is TH1 histogram
  else if( dynamic_cast<TH1F*>( o.object ) ) 
    { 
      preDrawTH1F( c, o ); 
    } 

  else
#ifdef DEBUG 
    std::cout << "L1TRPCTFRenderPlugin:preDraw  -- Cannot identify object " << o.name << std::endl; 
#endif 

  return;
} // L1TRPCTFRenderPlugin::preDraw(...)


void L1TRPCTFRenderPlugin::postDraw (TCanvas * c,
			       const DQMNet::CoreObject & o,
			       const VisDQMImgInfo & i)
{

#ifdef DEBUG 
  std::cout << "L1TRPCTFRenderPlugin:postDraw " << o.name << std::endl; 
#endif 

  // object is TH2 histogram
  if( dynamic_cast<TH2F*>( o.object ) )
    { 
      postDrawTH2F( c, o ); 
    } 

  // object is TH1 histogram
  else if( dynamic_cast<TH1F*>( o.object ) ) 
    { 
      postDrawTH1F( c, o ); 
    } 

  else
#ifdef DEBUG 
    std::cout << "L1TRenderPlugin:postDraw  -- Cannot identify object " << o.name << std::endl; 
#endif 

  return;
} // L1TRenderPlugin::postDraw(...)



////////////////////////////////////////////////////////////////////////

//  private functions


void L1TRPCTFRenderPlugin::preDrawTH1F ( TCanvas *c, const DQMNet::CoreObject &o )
{
  // Do we want to do anything special yet with TH1F histograms?

  TH1F* obj = dynamic_cast<TH1F*>( o.object ); 
  assert (obj); // checks that object indeed exists


  return;

} // preDrawTH1(...)




void L1TRPCTFRenderPlugin::preDrawTH2F ( TCanvas *c, const DQMNet::CoreObject &o )
{
  

  TH2F* obj = dynamic_cast<TH2F*>( o.object ); 
  assert( obj ); 

  //put in preDrawTH2F  
  if( o.name.find( "muons_eta_phi" )  != std::string::npos) {
    obj->SetStats( kFALSE );

   
  }

  return;
} // preDrawTH2F(...)




void L1TRPCTFRenderPlugin::postDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o )
{

  // Add error/warning text to 1-D histograms.  Do we want this at this time?
  return;

} // postDrawTH1F(...)




void L1TRPCTFRenderPlugin::postDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o )
{

  TH2F* obj = dynamic_cast<TH2F*>( o.object );
  assert( obj );


  if( o.name.find( "muons_eta_phi" )  != std::string::npos) {

      TLine line;
      line.SetLineWidth(1);
      for (int tc=0;tc<12;++tc) {
        int phi = tc*12+2;
        //line.DrawLine(-3.5, 0.5, -3.5, 6.5);
        line.DrawLine(-16.5, phi, 16.5, phi);
      }

      for (int tb=0;tb<8;++tb) {
        float eta = -12.5;
        if (tb==1) eta = -8.5;
        else if (tb==2) eta = -4.5;
        else if (tb==3) eta = -1.5;
        else if (tb==4) eta = 1.5;
        else if (tb==5) eta = 4.5;
        else if (tb==6) eta = 8.5;
        else if (tb==7) eta = 12.5;
         
        line.DrawLine(eta, 0, eta, 143);

      }
     
  }



  return;

} // postDrawTH2F(...)
