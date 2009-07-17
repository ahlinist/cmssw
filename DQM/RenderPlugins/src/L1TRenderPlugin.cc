/*!
  \file L1TRenderPlugin.cc
  \\
  \\ Code shamelessly borrowed from J. Temple's HcalRenderPlugin.cc code,
  \\ which was shamelessly borrowed from S. Dutta's SiStripRenderPlugin.cc
  \\ code, G. Della Ricca and B. Gobbo's EBRenderPlugin.cc, and other existing
  \\ subdetector plugins
  \\ preDraw and postDraw methods now check whether histogram was a TH1
  \\ or TH2, and call a private method appropriate for the histogram type
*/

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TProfile2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include "TPRegexp.h"
#include <cassert>

#define REMATCH(pat, str) (TPRegexp(pat).MatchB(str))

class L1TRenderPlugin : public DQMRenderPlugin
{
  TH2F* dummybox;

public:
  virtual void initialise (int, char **)
    {
      // same as RenderPlugin default for now (no special action taken)

      //summaryText = new TH2C( "summaryText", "summaryText", 5, 1, 6, 4, 1, 5);
      //  float rgb[6][3] = {{1.00, 0.00, 0.00}, {0.00, 1.00, 0.00},
      //                     {1.00, 0.96, 0.00}, {0.50, 0.00, 0.00},
      //                     {0.00, 0.40, 0.00}, {0.94, 0.78, 0.00}};
      //   for( int i=0; i<6; i++ ) {
      //     TColor* color = gROOT->GetColor( 301+i );
      //     if ( ! color ) color = new TColor( 301+i, 0, 0, 0, "");
      //     color->SetRGB( rgb[i][0], rgb[i][1], rgb[i][2] );
      //   }
      //   for(int i=0; i<6; i++) cpal[i] = i + 301;
      dummybox = new  TH2F("dummy","",22,-0.5,21.5,18,-0.5,17.5);

      for(int i=0; i<22; i++)
      {
        for(int j=0; j<18; j++)
        {
          dummybox->Fill(i,j,0.1);
        }
      }

    }

  virtual bool applies(const DQMNet::CoreObject &o, const VisDQMImgInfo &)
    {
      // determine whether core object is an L1T object
      if (o.name.find( "L1T/" ) != std::string::npos )
        return true;

      return false;
    }

  virtual void preDraw (TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo &)
    {
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
    }

  virtual void postDraw (TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &)
    {
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
    }

private:
  void preDrawTH1F ( TCanvas *, const DQMNet::CoreObject &o )
    {
      // Do we want to do anything special yet with TH1F histograms?

      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert (obj); // checks that object indeed exists

      gStyle->SetOptStat(111111);

      if(REMATCH("Packed Charge *", o.name))
      {
        obj->GetXaxis()->SetTitle("charge");
        obj->GetXaxis()->SetNdivisions(2);
        return;
      }
      if(REMATCH("Packed Phi *", o.name))
      {
        obj->GetXaxis()->SetTitle("phi");
        return;
      }
      else if(REMATCH("Packed Eta *", o.name))
      {
        obj->GetXaxis()->SetTitle("eta");
        return;
      }
      else if(REMATCH("Packed Quality *", o.name))
      {
        obj->GetXaxis()->SetTitle("quality");
        obj->GetXaxis()->SetNdivisions(8);
        return;
      }
      else if(REMATCH("Packed PT *", o.name))
      {
        obj->GetXaxis()->SetTitle("pt");
        return;
      }
      else if(REMATCH("Integrated Packed Pt *", o.name))
      {
        obj->GetXaxis()->SetTitle("pt");
        return;
      }
      else if(REMATCH("Integrated Packed Charge", o.name))
      {
        obj->GetXaxis()->SetTitle("charge");
        obj->GetXaxis()->SetNdivisions(2);
        return;
      }
      else if(REMATCH("Number of Tracks", o.name))
      {
        obj->GetXaxis()->SetNdivisions(12);
        return;
      }
      else if(REMATCH("BX *", o.name))
      {
        obj->GetXaxis()->SetTitle("bx");
        obj->GetXaxis()->SetNdivisions(3);
        return;
      }
      else if(REMATCH("2nd Tracks", o.name))
      {
        obj->GetXaxis()->SetTitle("sector");
        obj->GetXaxis()->SetNdivisions(12);
        return;
      }
      else if(REMATCH("Num Tracks Per Event bx*", o.name))
      {
        obj->GetXaxis()->SetNdivisions(2);
        return;
      }
      else if(REMATCH("Num Tracks Per Event", o.name))
      {
        obj->GetXaxis()->SetNdivisions(11);
        return;
      }
      //   else if(REMATCH("Wheel * - BX*", o.name)) {
      //     obj->GetXaxis()->SetTitle("bx");
      //     obj->GetXaxis()->SetNdivisions(3);
      //     return;
      //   }
      else if(REMATCH("Integrated Num Tracks", o.name))
      {
        obj->GetXaxis()->CenterLabels();
        obj->GetYaxis()->CenterLabels();
        obj->GetXaxis()->SetTitle("wheel");
        obj->GetXaxis()->SetBinLabel(1,"N2");
        obj->GetXaxis()->SetBinLabel(2,"N1");
        obj->GetXaxis()->SetBinLabel(3,"N0");
        obj->GetXaxis()->SetBinLabel(4,"P0");
        obj->GetXaxis()->SetBinLabel(5,"P1");
        obj->GetXaxis()->SetBinLabel(6,"P2");
        return;
      }
      // rate histograms
      if ( o.name.find("rate_algobit") != std::string::npos ||
           o.name.find("rate_ttbit") != std::string::npos)
      {
        gStyle->SetOptStat(11);
        obj->GetXaxis()->SetTitle("Luminosity Segment Number");
        obj->GetYaxis()->SetTitle("Rate (Hz)");
        int nbins = obj->GetNbinsX();
        int maxRange = nbins;
        for ( int i = nbins; i > 0; --i )
        {
          if ( obj->GetBinContent(i) != 0 )
          {
            maxRange = i;
            break;
          }
        }
        int minRange = 0;
        for ( int i = 0; i <= nbins; ++i )
        {
          if ( obj->GetBinContent(i) != 0 )
          {
            minRange = i;
            break;
          }
        }
        obj->GetXaxis()->SetRange(minRange, maxRange);
      }
      //  if( o.name.find( "dttf_p_q_" )  != std::string::npos) {
      //     //dqm::utils::reportSummaryMapPalette(obj);
      //     //obj->SetOption("colz");
      //     //obj->SetTitle("L1T Report Summary Map");

      //     obj->GetXaxis()->SetNdivisions(1);
      //     return;
      //   }
      // Code used in SiStripRenderPlugin -- do we want similar defaults?

      /*
        gStyle->SetOptStat(0111);
        if ( obj->GetMaximum(1.e5) > 0. ) {
          gPad->SetLogy(1);
        } else {
          gPad->SetLogy(0);
        }
      */
    }

  void preDrawTH2F ( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      //put in preDrawTH2F
      if( o.name.find( "reportSummaryMap" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        dqm::utils::reportSummaryMapPalette(obj);
        obj->SetOption("colz");
        obj->SetTitle("L1T Report Summary Map");

        obj->GetXaxis()->SetNdivisions(1,true);
        obj->GetYaxis()->SetNdivisions(8,true);
        obj->GetXaxis()->CenterLabels();
        obj->GetYaxis()->CenterLabels();

        gPad->SetGrid(1,1);

        //gStyle->SetPaintTextFormat("+g");

        //TPaveText tree(.01,.75,.15,1.00);
        //tree.SetFillColor(18);
        //tree.SetTextAlign(12);
        //TText *tnt = tree.AddText("Tree");
        //tnt.SetTextAlign(22);

        return;
      }

      gStyle->SetCanvasBorderMode( 0 );
      gStyle->SetPadBorderMode( 0 );
      gStyle->SetPadBorderSize( 0 );

      // I don't think we want to set stats to 0 for Hcal
      //gStyle->SetOptStat( 0 );
      //obj->SetStats( kFALSE );

      // Use same labeling format as SiStripRenderPlugin.cc
      TAxis* xa = obj->GetXaxis();
      TAxis* ya = obj->GetYaxis();

      xa->SetTitleOffset(0.7);
      xa->SetTitleSize(0.05);
      xa->SetLabelSize(0.04);

      ya->SetTitleOffset(0.7);
      ya->SetTitleSize(0.05);
      ya->SetLabelSize(0.04);

      // Now the important stuff -- set 2D hist drawing option to "colz"
      gStyle->SetPalette(1);
      obj->SetOption("colz");

      //gStyle->SetOptStat(0);

/*      if(
        o.name.find( "RctEmIsoEmEtEtaPhi" ) != std::string::npos ||
        o.name.find( "RctEmIsoEmOccEtaPhi" ) != std::string::npos ||
        o.name.find( "RctEmNonIsoEmEtEtaPhi" ) != std::string::npos ||
        o.name.find( "RctEmNonIsoEmOccEtaPhi" ) != std::string::npos ||
        o.name.find( "RctRegionsEtEtaPhi" ) != std::string::npos ||
        o.name.find( "RctRegionsOccEtaPhi" ) != std::string::npos
         ) */
      if(
        o.name.find( "Rct" ) != std::string::npos &&
        o.name.find( "EtaPhi" ) != std::string::npos 
        )
      {
        gPad->SetGrid(1,1);
        gStyle->SetOptStat(11);
        obj->GetXaxis()->SetTitle("GCT eta");
        obj->GetYaxis()->SetTitle("GCT phi");
        return;
      }



      if(o.name.find("Summary") != std::string::npos)
      {
        obj->GetXaxis()->CenterLabels();
        obj->GetYaxis()->CenterLabels();

        obj->GetXaxis()->SetTitle("wheel");
        obj->GetXaxis()->SetBinLabel(1,"N2");
        obj->GetXaxis()->SetBinLabel(2,"N1");
        obj->GetXaxis()->SetBinLabel(3,"N0");
        obj->GetXaxis()->SetBinLabel(4,"P0");
        obj->GetXaxis()->SetBinLabel(5,"P1");
        obj->GetXaxis()->SetBinLabel(6,"P2");

        gPad->SetGrid(1,1);
        gPad->SetBottomMargin(0.1);
        gPad->SetLeftMargin(0.12);
        gPad->SetRightMargin(0.12);
        //obj->SetMinimum(-0.00000001);
        gStyle->SetOptStat(0);
      }

      if(o.name.find("BX Summary") != std::string::npos)
      {
        obj->GetYaxis()->SetTitle("bx");
        obj->GetXaxis()->SetNdivisions(6,true);
        obj->GetYaxis()->SetNdivisions(4,true);
        //gPad->SetGrid(1,1);
        return;
      }

      if(o.name.find("Occupancy Summary") != std::string::npos)
      {
        obj->GetYaxis()->SetTitle("sector");
        obj->GetXaxis()->SetNdivisions(6,true);
        obj->GetYaxis()->SetNdivisions(13,true);
        //gPad->SetGrid(1,1);
        //     obj->GetXaxis()->SetLabelSize(0.07);
        //     obj->GetYaxis()->SetLabelSize(0.07);
        //obj->GetXaxis()->LabelsOption("h");

        //obj->SetMaximum(5.0);

        //int colorError1[5];
        //colorError1[0] = 416;// kGreen
        //colorError1[1] = 400;// kYellow
        //colorError1[2] = 800;// kOrange
        //colorError1[3] = 625;
        //colorError1[4] = 632;// kRed
        //gStyle->SetPalette(5, colorError1);

        //gStyle->SetPalette(6, cpal);
        return;
      }

      else if(o.name.find("Fractional High Quality Summary") != std::string::npos)
      {
        obj->GetYaxis()->SetTitle("sector");
        obj->GetXaxis()->SetNdivisions(6,true);
        obj->GetYaxis()->SetNdivisions(13,true);
        return;
      }

      else if(o.name.find("2nd Track Summary") != std::string::npos)
      {
        obj->GetYaxis()->SetTitle("sector");
        obj->GetXaxis()->SetNdivisions(6,true);
        obj->GetYaxis()->SetNdivisions(13,true);
        return;
      }

      else if(o.name.find("Phi vs Eta") != std::string::npos)
      {
        obj->GetXaxis()->SetTitle("eta");
        obj->GetYaxis()->SetTitle("phi");
        return;
      }

      else if(REMATCH("BX_Correlation_*", o.name)) {
        TAxis* yBX = obj->GetYaxis();
        yBX->SetTitleOffset(1.1);
        return;
      }
    }

  void postDrawTH1F( TCanvas *, const DQMNet::CoreObject & )
    {



      /*
        // Add error/warning text to 1-D histograms.  Do we want this at this time?
        TText tt;
        tt.SetTextSize(0.12);

        if (o.flags == 0)
          return;
        else
        {
          if (o.flags & DQMNet::DQM_FLAG_REPORT_ERROR)
          {
            tt.SetTextColor(2); // error color = RED
            tt.DrawTextNDC(0.5, 0.5, "Error");
          } // DQM_FLAG_REPORT_ERROR
          else if (o.flags & DQMNet::DQM_FLAG_REPORT_WARNING)
          {
            tt.SetTextColor(5);
            tt.DrawTextNDC(0.5, 0.5, "Warning"); // warning color = YELLOW
          } // DQM_FLAG_REPORT_WARNING
          else if (o.flags & DQMNet::DQM_FLAG_REPORT_OTHER)
          {
            tt.SetTextColor(1); // other color = BLACK
            tt.DrawTextNDC(0.5, 0.5, "Other ");
          } // DQM_FLAG_REPORT_OTHER
          else
          {
            tt.SetTextColor(3);
            tt.DrawTextNDC(0.5, 0.5, "Ok ");
          } //else
        } // else (  o.flags != 0  )
      */
    }

  void postDrawTH2F( TCanvas *, const DQMNet::CoreObject &o )
    {


      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );


/*      if(
        o.name.find( "RctEmIsoEmEtEtaPhi" ) != std::string::npos ||
        o.name.find( "RctEmIsoEmOccEtaPhi" ) != std::string::npos ||
        o.name.find( "RctEmNonIsoEmEtEtaPhi" ) != std::string::npos ||
        o.name.find( "RctEmNonIsoEmOccEtaPhi" ) != std::string::npos ||
        o.name.find( "RctRegionsEtEtaPhi" ) != std::string::npos ||
        o.name.find( "RctRegionsOccEtaPhi" ) != std::string::npos
         ) */
      if(
        o.name.find( "Rct" ) != std::string::npos &&
        o.name.find( "EtaPhi" ) != std::string::npos
         ) 
      {

        dummybox->Draw("box,same");
        return;
      }




      // nothing to put here just yet
      // in the future, we can add text output based on error status,
      // or set bin range based on filled histograms, etc.
      // Maybe add a big "OK" sign to histograms with no entries (i.e., no errors)?

      //int nSubsystems = 20;

      //TPaveText *pt[nSubsystems];

      //for(int i =0; i<nSubsystems; i++) {
      // relative to pad dimensions
      //TText *text = pt->AddText("ECAL");
      //     switch(i) {
      //     case 0 :
      //       pt[i]= new TPaveText(0.14, 0.20, 0.14, 0.20, "NDC");
      //       pt[i]->AddText("ECAL");
      //       break;
      //     case 1 :
      //       pt[i]= new TPaveText(0.30, 0.20, 0.30,0.20, "NDC");
      //       pt[i]->AddText("HCAL");
      //       break;
      //     case 2 :
      //       pt[i]= new TPaveText(0.47, 0.20, 0.47,0.20, "NDC");
      //       pt[i]->AddText("RCT");
      //       break;
      //     case 3 :
      //       pt[i]= new TPaveText(0.63, 0.20, 0.63,0.20, "NDC");
      //       pt[i]->AddText("GCT");
      //       break;
      //     case 4 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       break;
      //     case 5 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTF");
      //       break;
      //     case 6 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("CSCTPG");
      //       break;
      //     case 7 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("CSCTF");
      //       break;
      //     case 8 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_RPC");     break;
      //     case 9 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_GMT");     break;
      //     case 10 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_GT");      break;
      //     case 11 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_RPCTG");   break;
      //     case 12 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_EMUL");    break;
      //     case 13 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_Timing");  break;
      //     case 14 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_Test1");   break;
      //     case 15 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_Test2");   break;
      //     case 16 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_Test3");   break;
      //     case 17 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_Test4");break;
      //     case 18 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_Test5");   break;
      //     case 19 :
      //       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
      //       pt[i]->AddText("DTTPG");
      //       sprintf(histo,"L1T_Test6");   break;
      //     }
      //   TPaveText *pt[8];

      //   pt[i]->SetFillColor(0); // text is black on white
      //   pt[i]->SetTextSize(0.04);
      //   pt[i]->SetTextAlign(12);
      //   pt[i]->SetFillStyle(4000);

      //   pt[i]->Draw("same");
      //to draw your text object
      //  }


    }
};

static L1TRenderPlugin instance;
