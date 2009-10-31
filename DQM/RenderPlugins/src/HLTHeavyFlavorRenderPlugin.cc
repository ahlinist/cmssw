// Render Plugin for Heavy Flavor HLT validation
// author: Zoltan Gecse

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TProfile2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include <cassert>

class HLTHeavyFlavorRenderPlugin : public DQMRenderPlugin{
public:
  virtual bool applies( const VisDQMObject &o, const VisDQMImgInfo & ){
    return o.name.find( "HLT/HeavyFlavor" ) != std::string::npos;
  }

  virtual void preDraw( TCanvas * c, const VisDQMObject &o, const VisDQMImgInfo &, VisDQMRenderInfo & ){
    c->cd();
    TH2* h2 = dynamic_cast<TH2*>( o.object );
    if( h2 ){
      preDrawTH2( c, h2, o.name );
      return;
    }
    TH1* h1 = dynamic_cast<TH1*>( o.object );
    if( h1 ){
      preDrawTH1( c, h1, o.name );
      return;
    }
  }

  virtual void postDraw( TCanvas * , const VisDQMObject &, const VisDQMImgInfo & ){
    return;
  }

private:
  void preDrawTH1( TCanvas *c, TH1* obj, const std::string &name ){
    if( name.find("eff") != std::string::npos ){
      gStyle->SetOptStat(0);
      obj->SetOption("PE");
      obj->SetTitle("");
//      obj->Scale(100);
      // "colz" doesn't draw zero bins so increase them a bit. If bin has 0/0 i.e. 50+-50 make it -1 to avoid drawing
      for(int i=1; i<=obj->GetNbinsX(); i++){
        if( obj->GetBinError(i) > 0.49 )
          obj->SetBinContent(i, -1);
      }
      obj->GetYaxis()->SetRangeUser(-0.001,1.001);
      c->SetGridy();
    }else{
      gStyle->SetOptStat("e");
    }
    // if axis label starts with space, set log scale
    if( TString(obj->GetXaxis()->GetTitle()).BeginsWith(' ') ){
      c->SetLogx();
    }
    obj->SetLineColor(2);
    obj->SetLineWidth(2);
    obj->SetMarkerStyle(20);
    obj->SetMarkerSize(0.8);
  }

  void preDrawTH2 ( TCanvas *c, TH2* obj, const std::string &name ){
    if( name.find("eff") != std::string::npos ){
      gStyle->SetOptStat(0);
      gStyle->SetPaintTextFormat(".0f");
      obj->SetOption("colztexte");
      obj->SetTitle("");
      //convert to percents, less digits to draw
      obj->Scale(100);
      //"colz" doesn't draw zero bins so increase them a bit. If bin has 0/0 i.e. 50+-50 make it -1 to avoid drawing
      for(int i=1; i<=obj->GetNbinsX(); i++){
        for(int j=1; j<=obj->GetNbinsY(); j++){
          if( obj->GetBinContent(i, j) == 0 )
            obj->SetBinContent(i, j, 0.0001);
          if( obj->GetBinError(i, j) > 49 )
            obj->SetBinContent(i, j, -1);
        }
      }
      obj->GetZaxis()->SetRangeUser(-0.001,100.001);
    }else if( name.find("deltaEtaDeltaPhi") != std::string::npos ){
      gStyle->SetOptStat("emr");
      obj->SetOption("colz");
      c->SetLogz();
    }else{
      gStyle->SetOptStat("e");
      obj->SetOption("colztext");
    }
    // if axis label starts with space, set log scale
    if( TString(obj->GetXaxis()->GetTitle()).BeginsWith(' ') ){
      c->SetLogx();
    }
    if( TString(obj->GetYaxis()->GetTitle()).BeginsWith(' ') ){
      c->SetLogy();
    }
    gStyle->SetPalette(1);
  }

};

static HLTHeavyFlavorRenderPlugin instance;

