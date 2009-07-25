#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TColor.h"
#include "TText.h"
#include "TGraph.h"
#include "TLine.h"

#include <math.h>


class ESRenderPlugin : public DQMRenderPlugin 
{

   public:

      static const int ixES[346];
      static const int iyES[346];
      static const int lsES[54];
      static const int lwES[54];

      virtual bool applies( const DQMNet::CoreObject &o, const VisDQMImgInfo &i );

      virtual void preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo&  r);

      virtual void postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i );

   private:

      void preDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o );
      void preDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o );
      void postDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o );

      void drawBorders( int plane, float sx, float sy );

      double NEntries;

};

bool ESRenderPlugin::applies( const DQMNet::CoreObject &o, const VisDQMImgInfo & ) 
{

   if( o.name.find( "EcalPreshower" ) != std::string::npos ) {
      if( o.name.find( "ESOccupancyTask" ) != std::string::npos ){
	 return true;
      }
      if( o.name.find( "ESRawDataTask" ) != std::string::npos ){
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

void ESRenderPlugin::preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo & ) 
{

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


   if( dynamic_cast<TH1F*>( o.object ) ) {
      preDrawTH1F( c, o );
   }

   if( dynamic_cast<TH2F*>( o.object ) ) {
      preDrawTH2F( c, o );
   }

}

void ESRenderPlugin::postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo & )
{
   c->cd();

   if( dynamic_cast<TH2F*>( o.object ) )
   {
      postDrawTH2F( c, o );
   }
}


void ESRenderPlugin::preDrawTH1F( TCanvas *, const DQMNet::CoreObject &o ) 
{

   TH1F* obj = dynamic_cast<TH1F*>( o.object );

   assert( obj );

   std::string name = o.name.substr(o.name.rfind("/")+1);

   if( o.name.find("ESRawDataTask")!= std::string::npos)
   {
      obj->SetFillColor(kRed);
   }

   if( name.find( "Gain used for data taking" ) != std::string::npos )
   {
      obj->GetXaxis()->SetBinLabel(1,"LG");
      obj->GetXaxis()->SetBinLabel(2,"HG");
      obj->GetXaxis()->SetLabelSize(0.1);
   }

   if( name.find( "FEDs used for data taking" ) != std::string::npos )
   {
      obj->SetFillColor(kGreen);
   }

   if( name.find( "Z 1 P 1" ) != std::string::npos )
   {
      name.erase( name.find( "Z 1 P 1" ) , 7);
      name.insert( 2, "+F" );
      obj->SetTitle( name.c_str() );
   }
   else if( name.find( "Z -1 P 1" ) != std::string::npos )
   {
      name.erase( name.find( "Z -1 P 1" ) , 8);
      name.insert( 2, "-F" );
      obj->SetTitle( name.c_str() );
   }
   else if( name.find( "Z 1 P 2" ) != std::string::npos )
   {
      name.erase( name.find( "Z 1 P 2" ) , 7);
      name.insert( 2, "+R" );
      obj->SetTitle( name.c_str() );
   }
   else if( name.find( "Z -1 P 2" ) != std::string::npos )
   {
      name.erase( name.find( "Z -1 P 2" ) , 8);
      name.insert( 2, "-R" );
      obj->SetTitle( name.c_str() );
   }

}

void ESRenderPlugin::preDrawTH2F( TCanvas *, const DQMNet::CoreObject &o ) 
{

   TH2F* obj = dynamic_cast<TH2F*>( o.object );

   assert( obj );

   std::string name = o.name.substr(o.name.rfind("/")+1);

   int colorbar[8] = {0,2,3,4,5,6,7,800};

   const int NRGBs = 5;
   const int NCont = 255;

   double stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
   double red[NRGBs]   = { 0.87, 1.00, 0.12, 0.00, 0.00 };
   double green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
   double blue[NRGBs]  = { 0.00, 0.00, 0.00, 1.00, 0.87 };

   gStyle->SetPaintTextFormat();

   gStyle->SetOptStat(kFALSE);
   obj->SetStats(kFALSE);
   gPad->SetLogy(kFALSE);

   gStyle->SetPalette(1);
   obj->SetOption("colz");
   gPad->SetRightMargin(0.15);
   gStyle->SetPaintTextFormat("+g");


   if( name.find( "OptoRX" ) != std::string::npos||name.find( "KChip" ) != std::string::npos||name.find( "Fiber Status" ) != std::string::npos )
   {
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
      gStyle->SetNumberContours(NCont);
   }

   if( name.find( "Z 1 P 1" ) != std::string::npos )
   {
      name.erase( name.find( "Z 1 P 1" ) , 7);
      name.insert( 2, "+F" );
      obj->SetTitle( name.c_str() );
   }
   else if( name.find( "Z -1 P 1" ) != std::string::npos )
   {
      name.erase( name.find( "Z -1 P 1" ) , 8);
      name.insert( 2, "-F" );
      obj->SetTitle( name.c_str() );
   }
   else if( name.find( "Z 1 P 2" ) != std::string::npos )
   {
      name.erase( name.find( "Z 1 P 2" ) , 7);
      name.insert( 2, "+R" );
      obj->SetTitle( name.c_str() );
   }
   else if( name.find( "Z -1 P 2" ) != std::string::npos )
   {
      name.erase( name.find( "Z -1 P 2" ) , 8);
      name.insert( 2, "-R" );
      obj->SetTitle( name.c_str() );
   }

   if( name.find( "Integrity Summary" ) != std::string::npos ) 
   {
      gStyle->SetPalette(8,colorbar);
      obj->SetMinimum(0.5);
      obj->SetMaximum(8.5);
      return;
   }

   if( name.find( "Fiber Status" ) != std::string::npos )   
   {
      obj->GetYaxis()->SetBinLabel(1,"Bad");
      obj->GetYaxis()->SetBinLabel(2,"Good");
      obj->GetYaxis()->SetLabelSize(0.08);
      obj->GetYaxis()->SetTitle("");
   }

   /*   if( name.find( "ES OptoRX used for data taking" ) != std::string::npos )   
	{
	}
    */
   if( name.find( "RecHit 2D Occupancy" ) != std::string::npos ) 
   {
      gStyle->SetPalette(1);
      NEntries = obj->GetBinContent(40,40);
      obj->SetBinContent(40,40,0.);
      obj->Scale(1/NEntries);
      obj->SetMaximum(33);
      return;
   }

   if( name.find( "Digi 2D Occupancy" ) != std::string::npos ) 
   {
      gStyle->SetPalette(1);
      NEntries = obj->GetBinContent(40,40);
      obj->SetBinContent(40,40,0.);
      obj->Scale(1/NEntries);
      return;
   }

   if( name.find( "reportSummaryMap" ) != std::string::npos ) 
   {
      dqm::utils::reportSummaryMapPalette(obj);
      obj->SetTitle("EcalPreshower Report Summary Map");
      return;
   }


}

void ESRenderPlugin::postDrawTH2F( TCanvas *, const DQMNet::CoreObject &o )
{
   TH2F* obj = dynamic_cast<TH2F*>( o.object );
   assert( obj );

   std::string name = o.name.substr(o.name.rfind("/")+1);

   if( name.find( "Z 1 P 1" ) != std::string::npos ){
      drawBorders( 1, 0.5, 0.5 );
   }

   if( name.find( "Z -1 P 1" ) != std::string::npos ){
      drawBorders( 2, 0.5, 0.5 );
   }

   if( name.find( "Z 1 P 2" ) != std::string::npos ){
      drawBorders( 3, 0.5, 0.5 );
   }

   if( name.find( "Z -1 P 2" ) != std::string::npos ){
      drawBorders( 4, 0.5, 0.5 );
   }

   if( name.find( "reportSummaryMap" ) != std::string::npos ) 
   {
      TText t;
      t.SetTextAlign(22);
      t.DrawText(21,21,"ES+R");
      t.DrawText(61,21,"ES-R");
      t.DrawText(21,61,"ES+F");
      t.DrawText(61,61,"ES-F");

      drawBorders( 1, 0.5, 40.5 );
      drawBorders( 2, 40.5, 40.5 );
      drawBorders( 3, 0.5, 0.5 );
      drawBorders( 4, 40.5, 0.5 );
   }

} 

// Draw ES borders (Ming's copyright, the idea is borrowed from Giuseppe ;-))
void ESRenderPlugin::drawBorders( int plane, float sx, float sy )
{

   TLine l;

   switch (plane){

      case 1:	//ES+F
	 for ( int i=0; i<346; i=i+2) {
	    if (i<54*2) {
	       l.SetLineStyle(lsES[i/2]);
	       l.SetLineWidth(lwES[i/2]);
	    } else {
	       l.SetLineStyle(3);
	       l.SetLineWidth(2);
	    } 
	    l.DrawLine(ixES[i]+sx, iyES[i]+sy, ixES[i+1]+sx, iyES[i+1]+sy);
	 }
	 break;

      case 2:   //ES-F
	 for ( int i=0; i<346; i=i+2) {
	    if (i<54*2) {
	       l.SetLineStyle(lsES[i/2]);
	       l.SetLineWidth(lwES[i/2]);
	    } else {
	       l.SetLineStyle(3);
	       l.SetLineWidth(2);
	    } 
	    l.DrawLine(40-ixES[i]+sx, iyES[i]+sy, 40-ixES[i+1]+sx, iyES[i+1]+sy);
	 }
	 break;

      case 3:    //ES+R
	 for ( int i=0; i<346; i=i+2) {
	    if (i<54*2) {
	       l.SetLineStyle(lsES[i/2]);
	       l.SetLineWidth(lwES[i/2]);
	    } else {
	       l.SetLineStyle(3);
	       l.SetLineWidth(2);
	    } 
	    l.DrawLine(40-iyES[i]+sx, ixES[i]+sy, 40-iyES[i+1]+sx, ixES[i+1]+sy);
	 }
	 break;

      case 4:    //ES-R
	 for ( int i=0; i<346; i=i+2) {
	    if (i<54*2) {
	       l.SetLineStyle(lsES[i/2]);
	       l.SetLineWidth(lwES[i/2]);
	    } else {
	       l.SetLineStyle(3);
	       l.SetLineWidth(2);
	    } 
	    l.DrawLine(iyES[i]+sx, ixES[i]+sy, iyES[i+1]+sx, ixES[i+1]+sy);
	 }
	 break;

      default:
	 break;

   }
}

const int ESRenderPlugin::ixES[346] = {
   1, 13,  5,  5,  5,  7,  7,  7,  7,  9,  9,  9, 11, 11, 13, 13, 13, 15, 15, 15, 
   15, 15, 15, 19, 19, 19, 21, 21, 21, 23, 23, 23, 25, 25, 25, 27, 27, 27, 27, 29, 
   29, 29, 29, 31, 31, 31, 31, 31, 31, 33, 33, 33, 35, 35, 39, 27, 35, 35, 35, 33, 
   33, 33, 33, 31, 31, 31, 29, 29, 27, 27, 27, 25, 25, 25, 25, 25, 25, 21, 21, 21, 
   19, 19, 19, 17, 17, 17, 15, 15, 15, 13, 13, 13, 13, 11, 11, 11, 11,  9,  9,  9,
   9,  9,  9,  7,  7,  7,  5,  5, 

   1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  6, 
   6,  6,  6,  7,  7,  7,  7,  9,  9,  9,  9, 10, 10, 10, 10, 13, 13, 13, 13, 15, 
   15, 15, 15, 25, 25, 25, 25, 27, 27, 27, 27, 30, 30, 30, 30, 31, 31, 31, 31, 33, 
   33, 33, 33, 34, 34, 34, 34, 35, 35, 35, 35, 36, 36, 36, 36, 37, 37, 37, 37, 38, 
   38, 38, 38, 39, 39, 39, 39, 38, 38, 38, 38, 37, 37, 37, 37, 36, 36, 36, 36, 35, 
   35, 35, 35, 34, 34, 34, 34, 33, 33, 33, 33, 31, 31, 31, 31, 30, 30, 30, 30, 27, 
   27, 27, 27, 25, 25, 25, 25, 15, 15, 15, 15, 13, 13, 13, 13, 10, 10, 10, 10,  9, 
   9,  9,  9,  7,  7,  7,  7,  6,  6,  6,  6,  5,  5,  5,  5,  4,  4,  4,  4,  3, 
   3,  3,  3,  2,  2,  2,  2,  1,  1,  1,

   13, 13, 13, 14, 14, 14, 14, 15, 15, 16, 16, 16, 16, 18, 18, 18, 18, 22, 22, 22, 
   22, 24, 24, 24, 24, 25, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 26, 26, 26, 
   26, 25, 25, 24, 24, 24, 24, 22, 22, 22, 22, 18, 18, 18, 18, 16, 16, 16, 16, 15, 
   15, 15, 15, 14, 14, 14, 14, 13 
};

const int  ESRenderPlugin::iyES[346] = {
   20, 20, 20, 30, 30, 30, 30, 36, 32, 32, 32, 20, 20, 38, 39, 26, 26, 26, 26, 24, 
   40, 30, 30, 30, 40, 27, 40, 35, 35, 35, 35, 26, 24, 40, 26, 26, 26, 33, 33, 33, 
   38, 24, 24, 24, 20, 24, 24, 28, 28, 28, 28, 36, 20, 33, 20, 20, 20, 10, 10, 10, 
   10,  4,  8,  8,  8, 20, 20,  2,  1, 14, 14, 14, 14, 16,  0, 10, 10, 10, 13,  0, 
   0,  5,  5,  5,  5, 14, 16,  0, 14, 14, 14,  7,  7,  7,  2, 16, 16, 16, 20, 16, 
   16, 12, 12, 12, 12,  4,  7, 20, 

   20, 26, 26, 26, 26, 28, 28, 28, 28, 31, 31, 31, 31, 32, 32, 32, 32, 33, 33, 33, 
   33, 34, 34, 34, 34, 36, 36, 36, 36, 37, 37, 37, 37, 38, 38, 38, 38, 39, 39, 39, 
   39, 40, 40, 40, 40, 39, 39, 39, 39, 38, 38, 38, 38, 37, 37, 37, 37, 36, 36, 36, 
   36, 34, 34, 34, 34, 33, 33, 33, 33, 32, 32, 32, 32, 31, 31, 31, 31, 28, 28, 28, 
   28, 26, 26, 26, 26, 14, 14, 14, 14, 12, 12, 12, 12,  9,  9,  9,  9,  8,  8,  8, 
   8,  7,  7,  7,  7,  6,  6,  6,  6,  4,  4,  4,  4,  3,  3,  3,  3,  2,  2,  2, 
   2,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3, 
   3,  4,  4,  4,  4,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  8,  9,  9,  9, 
   9, 12, 12, 12, 12, 14, 14, 14, 14, 20,  

   18, 22, 22, 22, 22, 24, 24, 24, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 26, 
   26, 26, 26, 25, 25, 25, 25, 24, 24, 24, 24, 22, 22, 22, 22, 18, 18, 18, 18, 16, 
   16, 16, 15, 15, 15, 14, 14, 14, 14, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 
   15, 16, 16, 16, 16, 18, 18, 18
};

const int  ESRenderPlugin::lsES[54] = { // line style
   1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 
   1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 1, 
   2, 2, 2, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2
};

const int  ESRenderPlugin::lwES[54] = { // line width
   2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 
   2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 2, 
   1, 1, 1, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1
};

static ESRenderPlugin instance;
