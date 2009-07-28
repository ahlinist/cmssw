/*!
  \file SiPixelRenderPlugin
  \brief Display Plugin for Pixel DQM Histograms
  \author P.Merkel
  \version $Revision: 1.13 $
  \date $Date: 2009/07/27 21:42:17 $
*/

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TProfile2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include <cassert>

class SiPixelRenderPlugin : public DQMRenderPlugin
{
public:
  virtual bool applies( const DQMNet::CoreObject &o, const VisDQMImgInfo & )
    {
      if( o.name.find( "Pixel/" ) != std::string::npos )
        return true;

      return false;
    }

  virtual void preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo & )
    {
      c->cd();
      if( dynamic_cast<TH2*>( o.object ) )
      {
        preDrawTH2( c, o );
      }
      else if( dynamic_cast<TH1*>( o.object ) )
      {
        preDrawTH1( c, o );
      }
    }

  virtual void postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo & )
    {
      c->cd();

      if( dynamic_cast<TH1*>( o.object ) )
      {
        postDrawTH1( c, o );
      }
    }

private:
  void preDrawTH2( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH2* obj = dynamic_cast<TH2*>( o.object );
      assert( obj );

      // This applies to all
      gStyle->SetCanvasBorderMode( 0 );
      gStyle->SetPadBorderMode( 0 );
      gStyle->SetPadBorderSize( 0 );
      //    (data->pad)->SetLogy( 0 );;
      gStyle->SetOptStat( 0 );
      obj->SetStats( kFALSE );

      if( o.name.find( "reportSummaryMap" ) == std::string::npos){
        TAxis* xa = obj->GetXaxis();
        TAxis* ya = obj->GetYaxis();
        xa->SetTitleOffset(0.7);
        xa->SetTitleSize(0.065);
        xa->SetLabelSize(0.065);
        ya->SetTitleOffset(0.75);
        ya->SetTitleSize(0.065);
        ya->SetLabelSize(0.065);
      }

      if( o.name.find( "endcapOccupancyMap" ) != std::string::npos ) obj->SetTitle("Endcap Digi Occupancy Map");
      if( o.name.find( "hitmap" ) != std::string::npos  ||
          o.name.find( "Occupancy" ) != std::string::npos)
      {
        gStyle->SetPalette(1);
        obj->SetOption("colz");
        return;
      }
      
      
      TH2F* obj2 = dynamic_cast<TH2F*>( o.object );

      if( o.name.find( "reportSummaryMap" ) != std::string::npos )
      {
        gPad->SetLeftMargin(0.3);
        dqm::utils::reportSummaryMapPalette(obj2);
        return;
      }
    }

  void preDrawTH1( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH1* obj = dynamic_cast<TH1*>( o.object );
      assert( obj );

      // This applies to all
      gStyle->SetOptStat(0111);
      if ( obj->GetMaximum(1.e5) > 0. )
      {
        gPad->SetLogy(1);
      }
      else
      {
        gPad->SetLogy(0);
      }
      
      TAxis* xa = obj->GetXaxis();
      TAxis* ya = obj->GetYaxis();
      xa->SetTitleOffset(0.7);
      xa->SetTitleSize(0.065);
      xa->SetLabelSize(0.065);
      ya->SetTitleOffset(0.75);
      ya->SetTitleSize(0.065);
      ya->SetLabelSize(0.065);

      if( o.name.find( "adcCOMB" ) != std::string::npos && obj->GetEntries() > 0. ) gPad->SetLogy(1);
      if( o.name.find( "chargeCOMB" ) != std::string::npos && obj->GetEntries() > 0. ){ obj->GetXaxis()->SetRange(1,51); gPad->SetLogy(1);}
      if( o.name.find( "OnTrack/charge_siPixelClusters" ) != std::string::npos ){ obj->GetXaxis()->SetRange(1,51); }
      if( o.name.find( "OffTrack/charge_siPixelClusters" ) != std::string::npos ){ obj->GetXaxis()->SetRange(1,51); }
      if( o.name.find( "barrelEventRate" ) != std::string::npos && obj->GetEntries() > 0. ) {gPad->SetLogx(1); gPad->SetLogy(1);}
      if( o.name.find( "endcapEventRate" ) != std::string::npos && obj->GetEntries() > 0. ) {gPad->SetLogx(1); gPad->SetLogy(1);}
      if( o.name.find( "ALLMODS_chargeCOMB" ) != std::string::npos ) obj->GetXaxis()->SetRange(1,51);
      if( o.name.find( "FEDEntries" ) != std::string::npos ) gStyle->SetOptStat(0);
      if( o.name.find( "size_siPixelClusters" ) != std::string::npos && obj->GetEntries() > 0. ) gPad->SetLogx(1);
      if( o.name.find( "OnTrack" ) != std::string::npos && o.name.find( "charge" ) != std::string::npos ) obj->SetTitle("ClusterCharge_OnTrack");
      if( o.name.find( "OnTrack" ) != std::string::npos && o.name.find( "size" ) != std::string::npos ) obj->SetTitle("ClusterSize_OnTrack");
      if( o.name.find( "OffTrack" ) != std::string::npos && o.name.find( "charge" ) != std::string::npos ) obj->SetTitle("ClusterCharge_OffTrack");
      if( o.name.find( "OffTrack" ) != std::string::npos && o.name.find( "size" ) != std::string::npos ) obj->SetTitle("ClusterSize_OffTrack");
   
     // prettify for shifters:
      if( o.name.find( "SUMDIG_ndigis_" ) != std::string::npos || 
          o.name.find( "SUMCLU_nclusters_" ) != std::string::npos || 
	  o.name.find( "SUMCLU_size_" ) != std::string::npos ) gPad->SetLogy(0);
      if( o.name.find( "SUMOFF_ndigis_" ) != std::string::npos || 
          o.name.find( "SUMOFF_nclusters_" ) != std::string::npos || 
	  o.name.find( "SUMOFF_size_" ) != std::string::npos ) gPad->SetLogy(0);
    }

  void postDrawTH1( TCanvas *, const DQMNet::CoreObject &o )
    {
      TText tt;
      tt.SetTextSize(0.12);
      if (o.flags == 0) return;
      else
      {
        /*    if (o.flags & DQMNet::DQM_FLAG_REPORT_ERROR)
              {
              tt.SetTextColor(2);
              tt.DrawTextNDC(0.5, 0.5, "Error");
              }
              else if (o.flags & DQMNet::DQM_FLAG_REPORT_WARNING)
              {
              tt.SetTextColor(5);
              tt.DrawTextNDC(0.5, 0.5, "Warning");
              }
              else if (o.flags & DQMNet::DQM_FLAG_REPORT_OTHER)
              {
              tt.SetTextColor(1);
              tt.DrawTextNDC(0.5, 0.5, "Other ");
              }
              else
              {
              tt.SetTextColor(3);
              tt.DrawTextNDC(0.5, 0.5, "Ok ");
              }
        */
      }
      
      TH1* obj = dynamic_cast<TH1*>( o.object );
      assert( obj );
      
      if( o.name.find( "SUMOFF_adc_Barrel" ) != std::string::npos ){ 
        TLine tl1; tl1.SetLineColor(4); tl1.DrawLine(0.,80.,192.,80.); 
	TLine tl2; tl2.SetLineColor(4); tl2.DrawLine(0.,120.,192.,120.); 
      }
      if( o.name.find( "SUMOFF_adc_Endcap" ) != std::string::npos ){ 
        TLine tl1; tl1.SetLineColor(4); tl1.DrawLine(0.,80.,96.,80.); 
	TLine tl2; tl2.SetLineColor(4); tl2.DrawLine(0.,120.,96.,120.); 
      }
      if( o.name.find( "SUMOFF_ndigis_Barrel" ) != std::string::npos ){ 
        TLine tl; tl.SetLineColor(4); tl.DrawLine(0.,20.,192.,20.); 
      }
      if( o.name.find( "SUMOFF_ndigis_Endcap" ) != std::string::npos ){ 
        TLine tl; tl.SetLineColor(4); tl.DrawLine(0.,20.,96.,20.); 
      }
      if( o.name.find( "SUMOFF_charge_Barrel" ) != std::string::npos ){ 
        TLine tl1; tl1.SetLineColor(4); tl1.DrawLine(0.,15.,192.,15.); 
	TLine tl2; tl2.SetLineColor(4); tl2.DrawLine(0.,30.,192.,30.); 
      }
      if( o.name.find( "SUMOFF_charge_Endcap" ) != std::string::npos ){ 
        TLine tl1; tl1.SetLineColor(4); tl1.DrawLine(0.,15.,96.,15.); 
	TLine tl2; tl2.SetLineColor(4); tl2.DrawLine(0.,30.,96.,30.); 
      }
      if( o.name.find( "OnTrack/charge_siPixelClusters" ) != std::string::npos ){ 
        Int_t ibin = obj->GetMaximumBin(); 
	Double_t val = obj->GetBinContent(ibin); 
	TLine tl; tl.SetLineColor(4); tl.DrawLine(21.,0.,21.,val); 
      }
      if( o.name.find( "OnTrack/size_siPixelClusters" ) != std::string::npos ||
          o.name.find( "OffTrack/size_siPixelClusters" ) != std::string::npos ){ 
        Int_t ibin = obj->GetMaximumBin(); 
	Double_t val = obj->GetBinContent(ibin); 
	TLine tl; tl.SetLineColor(4); tl.DrawLine(10.,0.,10.,val); 
      }
      
    }
};

static SiPixelRenderPlugin instance;
