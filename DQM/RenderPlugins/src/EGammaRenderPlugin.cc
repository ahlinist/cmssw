#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TProfile2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLine.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include <cassert>

class EGammaRenderPlugin : public DQMRenderPlugin
{
  TH2C* barreltext;
  TH2C* plusendcaptext;
  TH2C* minusendcaptext;

public:
  virtual bool applies( const DQMNet::CoreObject &o, const VisDQMImgInfo & )
    {
      if (o.name.find( "PhotonAnalyzer/" )   == std::string::npos)
	return false;

      if( o.name.find( "/Efficiencies/" ) != std::string::npos )
	return true;

      if( o.name.find( "/AllPhotons/" ) != std::string::npos )
	return true;

      if( o.name.find( "/GoodCandidatePhotons/" ) != std::string::npos )
	return true;

      if( o.name.find( "/BackgroundPhotons/" ) != std::string::npos )
	return true;

      return false;
    }

  virtual void preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo & )
    {
      c->cd();

      if( dynamic_cast<TH2F*>( o.object ) )
      {
	preDrawTH2F( c, o );
      }
      else if( dynamic_cast<TH1F*>( o.object ) )
      {
	preDrawTH1F( c, o );
      }
    }

  virtual void postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo & )
    {
      c->cd();
      if( dynamic_cast<TH1F*>( o.object ) )
      {
	postDrawTH1F( c, o );
      }
      if( dynamic_cast<TH2F*>( o.object ) )
      {
	postDrawTH2F( c, o );
      }
    }

private:
  void preDrawTH2F( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      gStyle->SetPalette(1);
      gStyle->SetOptStat("e");
      obj->SetOption( "colz" );
    }

  void preDrawTH1F( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert( obj );

      gStyle->SetOptStat("emr");

      if( o.name.find( "nPho" )  != std::string::npos)
	gStyle->SetOptStat("em");

      if( o.name.find( "nConv" )  != std::string::npos)
	gStyle->SetOptStat("em");

      if( o.name.find( "nIsoTracks" )  != std::string::npos)
	gStyle->SetOptStat("em");

      if( o.name.find( "phoEta" )  != std::string::npos)
	gStyle->SetOptStat("e");

      if( o.name.find( "phoConvEta" )  != std::string::npos)
	gStyle->SetOptStat("e");

      if( o.name.find( "phoPhi" )  != std::string::npos)
	gStyle->SetOptStat("e");

      if( o.name.find( "phoConvPhi" )  != std::string::npos)
	gStyle->SetOptStat("e");

      if( o.name.find( "VsEta" )  != std::string::npos)
	gStyle->SetOptStat("e");
    }

  void postDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o )
    {
      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert( obj );
      //gStyle->SetOptStat(11);
      obj->SetMinimum(0);

      if( o.name.find( "Filters" ) != std::string::npos )
      {
	c->SetBottomMargin(0.25);
	c->SetRightMargin(0.35);
	obj->SetStats(kFALSE);
	obj->SetMaximum(1.05);
      }
      if( o.name.find( "hOverE" )  != std::string::npos)
      {
	c->SetLogy(1);
	obj->SetMinimum(0.5);
      }
      if( o.name.find( "h1OverE" )  != std::string::npos)
      {
	c->SetLogy(1);
	obj->SetMinimum(0.5);
      }
      if( o.name.find( "h2OverE" )  != std::string::npos)
      {
	c->SetLogy(1);
	obj->SetMinimum(0.5);
      }
      if( o.name.find( "DeltaR" )  != std::string::npos)
      {
	c->SetLogy(1);
	obj->SetMinimum(0.5);
      }
    }

  void postDrawTH2F( TCanvas *, const DQMNet::CoreObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );
      //gStyle->SetOptStat("e");

      int ixSectorsEE[202] = {61, 61, 60, 60, 59, 59, 58, 58, 57, 57, 55, 55, 45, 45, 43, 43, 42, 42, 41, 41, 40, 40, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 45, 45, 55, 55, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 0,100,100, 97, 97, 95, 95, 92, 92, 87, 87, 85, 85, 80, 80, 75, 75, 65, 65, 60, 60, 40, 40, 35, 35, 25, 25, 20, 20, 15, 15, 13, 13,  8,  8,  5,  5,  3,  3,  0,  0,  3,  3,  5,  5,  8,  8, 13, 13, 15, 15, 20, 20, 25, 25, 35, 35, 40, 40, 60, 60, 65, 65, 75, 75, 80, 80, 85, 85, 87, 87, 92, 92, 95, 95, 97, 97,100,100,  0, 61, 65, 65, 70, 70, 80, 80, 90, 90, 92,  0, 61, 65, 65, 90, 90, 97,  0, 57, 60, 60, 65, 65, 70, 70, 75, 75, 80, 80,  0, 50, 50,  0, 43, 40, 40, 35, 35, 30, 30, 25, 25, 20, 20,  0, 39, 35, 35, 10, 10,  3,  0, 39, 35, 35, 30, 30, 20, 20, 10, 10,  8,  0, 45, 45, 40, 40, 35, 35,  0, 55, 55, 60, 60, 65, 65};
      int iySectorsEE[202] = {50, 55, 55, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 60, 60, 59, 59, 58, 58, 57, 57, 55, 55, 45, 45, 43, 43, 42, 42, 41, 41, 40, 40, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 45, 45, 50,  0, 50, 60, 60, 65, 65, 75, 75, 80, 80, 85, 85, 87, 87, 92, 92, 95, 95, 97, 97,100,100, 97, 97, 95, 95, 92, 92, 87, 87, 85, 85, 80, 80, 75, 75, 65, 65, 60, 60, 40, 40, 35, 35, 25, 25, 20, 20, 15, 15, 13, 13,  8,  8,  5,  5,  3,  3,  0,  0,  3,  3,  5,  5,  8,  8, 13, 13, 15, 15, 20, 20, 25, 25, 35, 35, 40, 40, 50,  0, 45, 45, 40, 40, 35, 35, 30, 30, 25, 25,  0, 50, 50, 55, 55, 60, 60,  0, 60, 60, 65, 65, 70, 70, 75, 75, 85, 85, 87,  0, 61,100,  0, 60, 60, 65, 65, 70, 70, 75, 75, 85, 85, 87,  0, 50, 50, 55, 55, 60, 60,  0, 45, 45, 40, 40, 35, 35, 30, 30, 25, 25,  0, 39, 30, 30, 15, 15,  5,  0, 39, 30, 30, 15, 15,  5};
      //rescaling arrays for x,y instead of ix,iy

      for (int i=0;i!=202;++i)
      {
	ixSectorsEE[i]=ixSectorsEE[i]*3-150;
	iySectorsEE[i]=iySectorsEE[i]*3-150;
      }

      barreltext = new TH2C( "eb_text", "ebtext", 18, -3.14+0.1745, 3.14+0.1745, 2, -1.479, 1.479);

      for ( short i=0; i<36; i++ )
      {
	int x = 1 + i%18;
	int y = 2 - i/18;
	int z = x + 10;
	if ( z > 18 ) z = z - 18;
	if (z!=10)
	{
	  if ( y == 1 )
	  {
	    barreltext->SetBinContent(x, y, -z);
	  }
	  else
	  {
	    barreltext->SetBinContent(x, y, +z);
	  }
	}
      }

      barreltext->SetMarkerSize( 1.5 );

      if( o.name.find( "DistributionAllEcal" ) != std::string::npos )
      {
	TLine l;
	l.SetLineWidth(1);
	l.DrawLine(-3.14,0, 3.14, 0.);
	l.DrawLine(-3.14,1.479, 3.14, 1.479);
	l.DrawLine(-3.14,-1.479, 3.14, -1.479);
	for (int i =1;i!=19;++i)
	{
	  l.DrawLine(-3.14-0.1745+3.14/9*i, -1.479,-3.14-0.1745+3.14/9*i, 1.479);
	}
	barreltext->Draw("text,same");
	return;
      }

      if( o.name.find( "DistributionBarrel" ) != std::string::npos )
      {
	TLine l;
	l.SetLineWidth(1);
	l.DrawLine(-3.14,0, 3.14, 0.);
	for (int i =1;i!=19;++i)
	{
	  l.DrawLine(-3.14-0.1745+3.14/9*i, -1.479,-3.14-0.1745+3.14/9*i, 1.479);
	}
	barreltext->Draw("text,same");
	return;
      }

      minusendcaptext = new TH2C( "ee-text", "ee-text", 10, -150., 150., 10, -150., 150. );
      plusendcaptext = new TH2C( "ee+text", "ee+text", 10, -150., 150., 10, -150., 150. );

      minusendcaptext->SetBinContent(2, 5, -3);
      minusendcaptext->SetBinContent(2, 7, -2);
      minusendcaptext->SetBinContent(4, 9, -1);
      minusendcaptext->SetBinContent(7, 9, -9);
      minusendcaptext->SetBinContent(9, 7, -8);
      minusendcaptext->SetBinContent(9, 5, -7);
      minusendcaptext->SetBinContent(8, 3, -6);
      minusendcaptext->SetBinContent(6, 2, -5);
      minusendcaptext->SetBinContent(3, 3, -4);

      plusendcaptext->SetBinContent(2, 5, +3);
      plusendcaptext->SetBinContent(2, 7, +2);
      plusendcaptext->SetBinContent(4, 9, +1);
      plusendcaptext->SetBinContent(7, 9, +9);
      plusendcaptext->SetBinContent(9, 7, +8);
      plusendcaptext->SetBinContent(9, 5, +7);
      plusendcaptext->SetBinContent(8, 3, +6);
      plusendcaptext->SetBinContent(5, 2, +5);
      plusendcaptext->SetBinContent(3, 3, +4);

      minusendcaptext->SetMarkerSize( 2.5 );
      plusendcaptext->SetMarkerSize( 2.5 );

      if( o.name.find( "DistributionEndcap" ) != std::string::npos )
      {
	TLine l;
	l.SetLineWidth(1);
	for ( int i=0; i<201; i=i+1)
	{
	  if ( (ixSectorsEE[i]!=-150 || iySectorsEE[i]!=-150) &&
	       (ixSectorsEE[i+1]!=-150 || iySectorsEE[i+1]!=-150) )
	  {
	    l.DrawLine(ixSectorsEE[i], iySectorsEE[i],
		       ixSectorsEE[i+1], iySectorsEE[i+1]);
	  }
	}
	if( o.name.find( "Minus" ) != std::string::npos )
	  minusendcaptext->Draw("text,same");
	if( o.name.find( "Plus" ) != std::string::npos )
	  plusendcaptext->Draw("text,same");
	return;
      }
    }
};

static EGammaRenderPlugin instance;
