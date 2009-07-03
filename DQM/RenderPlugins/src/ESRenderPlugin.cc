#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TColor.h"
#include "TGraph.h"
#include "TLine.h"

#include <math.h>


class ESRenderPlugin : public DQMRenderPlugin {

   public:


      virtual bool applies( const DQMNet::CoreObject &o, const VisDQMImgInfo &i );

      virtual void preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo&  r);

   private:

      void preDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o );

      double NEntries;

};


bool ESRenderPlugin::applies( const DQMNet::CoreObject &o, const VisDQMImgInfo & ) {

   if( o.name.find( "EcalPreshower" ) != std::string::npos ) {
      if( o.name.find( "ESOccupancyTask" ) != std::string::npos ){
	 return true;
      }
      if( o.name.find( "ESIntegrityTask" ) != std::string::npos ){
	 return true;
      }
      if( o.name.find( "ESIntegrityClient" ) != std::string::npos ){
	 return true;
      }
      if( o.name.find( "EventInfo" ) != std::string::npos ){
	 return true;
      }
   }

   return false;

}


void ESRenderPlugin::preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo & ) {

   c->cd();

   gStyle->Reset("Default");

   gStyle->SetCanvasColor(10);
   gStyle->SetPadColor(10);
   gStyle->SetFillColor(10);
   gStyle->SetFrameFillColor(10);
   gStyle->SetStatColor(10);
   gStyle->SetTitleFillColor(10);

   gStyle->SetOptTitle(kTRUE);
   gStyle->SetTitleBorderSize(0);

   gStyle->SetOptStat(kFALSE);
   gStyle->SetStatBorderSize(1);
   gStyle->SetOptFit(kFALSE);


   if( dynamic_cast<TH2F*>( o.object ) ) {
      preDrawTH2F( c, o );
   }

}


void ESRenderPlugin::preDrawTH2F( TCanvas *, const DQMNet::CoreObject &o ) {

   TH2F* obj = dynamic_cast<TH2F*>( o.object );

   assert( obj );

   std::string name = o.name.substr(o.name.rfind("/")+1);

   int colorbar[6] = {1,2,3,4,5,6};
   //int colorbar2[5] = {0,3,800,4,2};

   gStyle->SetPaintTextFormat();

   gStyle->SetOptStat(kFALSE);
   obj->SetStats(kFALSE);
   gPad->SetLogy(kFALSE);

   if( name.find( "Integrity Summary" ) != std::string::npos ) {
      gStyle->SetPalette(6,colorbar);
      obj->SetMinimum(0.5);
      obj->SetMaximum(6.5);
      obj->SetOption("colz");
      gPad->SetRightMargin(0.15);
      gStyle->SetPaintTextFormat("+g");
      return;
   }


   if( name.find( "RecHit 2D Occupancy" ) != std::string::npos ) {
      gStyle->SetPalette(1);
      obj->GetXaxis()->SetRange(0,39);
      obj->GetYaxis()->SetRange(0,39);
      NEntries = obj->GetBinContent(40,40);
      obj->Scale(1/NEntries);
      obj->SetMaximum(33);
      obj->SetOption("colz");
      gPad->SetRightMargin(0.15);
      gStyle->SetPaintTextFormat("+g");
      return;
   }

   if( name.find( "DigiHit 2D Occupancy" ) != std::string::npos ) {
      gStyle->SetPalette(1);
      obj->GetXaxis()->SetRange(0,39);
      obj->GetYaxis()->SetRange(0,39);
      NEntries = obj->GetBinContent(40,40);
      obj->Scale(1/NEntries);
      obj->SetOption("colz");
      gPad->SetRightMargin(0.15);
      gStyle->SetPaintTextFormat("+g");
      return;
   }

   if( name.find( "reportSummaryMap" ) != std::string::npos ) {
      gStyle->SetPalette(6,colorbar);
      obj->SetMinimum(-0.5);
      obj->SetMaximum(4.5);
      obj->SetOption("col");
      gPad->SetRightMargin(0.15);
      gStyle->SetPaintTextFormat("+g");
      return;
   }



}



static ESRenderPlugin instance;
