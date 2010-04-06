/*!
  \File BeamPixelRenderPlugin
  \Display Plugin for BeamSpot from Pixel-Vertices
  \author Mauro Dinardo
  \version $ Revision: 1.0 $
  \date $ Date: 2010/30/03 08:11:00 $
*/

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TProfile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include <cassert>


class BeamPixelRenderPlugin : public DQMRenderPlugin {

public:
  virtual bool applies(const VisDQMObject& o, const VisDQMImgInfo& )
  {
    if (o.name.find("BeamPixel/") != std::string::npos) return true;
    
    return false;
  }
  
  virtual void preDraw(TCanvas* c, const VisDQMObject& o, const VisDQMImgInfo& , VisDQMRenderInfo& )
  {
    c->cd();

    if (dynamic_cast<TH2F*>(o.object)) preDrawTH2F(c, o);

    if (dynamic_cast<TH1F*>(o.object)) preDrawTH1F(c, o);

    if (dynamic_cast<TProfile*>(o.object)) preDrawTProfile(c, o);
  }


private:
  void preDrawTH2F(TCanvas* c, const VisDQMObject& o)
  {  
    TH2F* obj = dynamic_cast<TH2F*>(o.object);
    assert(obj);

    // This applies to all
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadBorderSize(0);

    TAxis* xa = obj->GetXaxis();
    TAxis* ya = obj->GetYaxis();
    	
    if ((o.name.find("vertex zx") != std::string::npos) || (o.name.find("vertex zy") != std::string::npos) || (o.name.find("vertex xy") != std::string::npos))
      {
	xa->SetTitleOffset(1.15);
	ya->SetTitleOffset(1.15);

	xa->SetTitleSize(0.04);
	ya->SetTitleSize(0.04);

	xa->SetLabelSize(0.03);
	ya->SetLabelSize(0.03);

	c->SetGrid();
	obj->SetOption("colz");
	gStyle->SetOptStat(1110);

	return;
      }

    if (o.name.find("fit results") != std::string::npos)
      {
	xa->SetTitleOffset(1.15);
	ya->SetTitleOffset(1.15);

	xa->SetTitleSize(0.04);
	ya->SetTitleSize(0.04);

	xa->SetLabelSize(0.045);
	ya->SetLabelSize(0.045);

	c->SetGrid();
	obj->SetStats(kFALSE);
	obj->SetMarkerSize(2.);
	
	return;
      }

    if (o.name.find("reportSummaryMap") != std::string::npos)
      {
	c->SetGrid(kFALSE, kFALSE);
	obj->SetStats(kFALSE);
	
	return;
      }
  }

  void preDrawTH1F(TCanvas* c, const VisDQMObject& o)
  {
    TH1F* obj = dynamic_cast<TH1F*>(o.object);
    assert(obj);

    // This applies to all
    c->SetGrid(kFALSE, kFALSE);

    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadBorderSize(0);
    gStyle->SetOptStat(111110);

    TAxis* xa = obj->GetXaxis();
    TAxis* ya = obj->GetYaxis();

    xa->SetTitleOffset(1.15);
    ya->SetTitleOffset(1.15);

    xa->SetTitleSize(0.04);
    ya->SetTitleSize(0.04);

    xa->SetLabelSize(0.03);
    ya->SetLabelSize(0.03);

    if ((o.name.find("muX vs lumi") != std::string::npos) || (o.name.find("muY vs lumi") != std::string::npos) || (o.name.find("muZ vs lumi") != std::string::npos) ||
	(o.name.find("sigmaX vs lumi") != std::string::npos) || (o.name.find("sigmaY vs lumi") != std::string::npos) || (o.name.find("sigmaZ vs lumi") != std::string::npos) ||
	(o.name.find("dxdz vs lumi") != std::string::npos) || (o.name.find("dydz vs lumi") != std::string::npos))
      {
	gStyle->SetOptFit(1110);
	gStyle->SetOptStat(10);

	return;
      }
  }

  void preDrawTProfile(TCanvas* c, const VisDQMObject& o)
  {
    TProfile* obj = dynamic_cast<TProfile*>(o.object);
    assert(obj);

    // This applies to all
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadBorderSize(0);
    gStyle->SetOptStat(1110);

    TAxis* xa = obj->GetXaxis();
    TAxis* ya = obj->GetYaxis();
    
    xa->SetTitleOffset(1.15);
    ya->SetTitleOffset(1.15);

    xa->SetTitleSize(0.04);
    ya->SetTitleSize(0.04);

    xa->SetLabelSize(0.03);    
    ya->SetLabelSize(0.03);

    c->SetGrid();
  }

};


static BeamPixelRenderPlugin instance;
