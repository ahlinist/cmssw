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
  virtual bool applies( const DQMNet::CoreObject &o, const VisDQMImgInfo & ){
    if (o.name.find( "HLT/HeavyFlavor" ) != std::string::npos  )
      return true;
    return false;
  }

  virtual void preDraw( TCanvas * c, const DQMNet::CoreObject &o, const VisDQMImgInfo &, VisDQMRenderInfo & ){
    c->cd();
    if( dynamic_cast<TH2F*>( o.object ) ){
      preDrawTH2F( c, o );
    }
    else if( dynamic_cast<TH1F*>( o.object ) ){
      preDrawTH1F( c, o );
    }
  }

  virtual void postDraw( TCanvas * c, const DQMNet::CoreObject &o, const VisDQMImgInfo & ){
    return;
    if( dynamic_cast<TH2F*>( o.object ) ){
      postDrawTH2F( c, o );
    }
    else if( dynamic_cast<TH1F*>( o.object ) ){
      postDrawTH1F( c, o );
    }
  }

private:
  void preDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o ){
    TH1F* obj = dynamic_cast<TH1F*>( o.object );
    assert( obj );
    if( TString(obj->GetXaxis()->GetTitle()).BeginsWith(' ') ){
      c->SetLogx();
    }
    if( o.name.find("eff") != std::string::npos ){
      gStyle->SetOptStat(0);
      obj->SetOption("PE");
      obj->SetTitle("");
      obj->Scale(100);
      //"colz" doesn't draw zero bins so increase them a bit. If bin has 0/0 i.e. 50+-50 make it -1 to avoid drawing
      for(int i=0; i<obj->GetSize(); i++){
        if( obj->GetBinError(i)>49 )
          obj->operator[](i) = -1;
      }
      obj->GetYaxis()->SetRangeUser(-0.001,100.001);
      c->SetGridy();
    }else{
      gStyle->SetOptStat("e");
    }
    obj->SetLineColor(2);
    obj->SetLineWidth(2);
    obj->SetMarkerStyle(20);
    obj->SetMarkerSize(0.8);
  }

  void preDrawTH2F ( TCanvas *c, const DQMNet::CoreObject &o ){
    TH2F* obj = dynamic_cast<TH2F*>( o.object );
    assert( obj );
    if( TString(obj->GetXaxis()->GetTitle()).BeginsWith(' ') ){
      c->SetLogx();
    }
    if( TString(obj->GetYaxis()->GetTitle()).BeginsWith(' ') ){
      c->SetLogy();
    }
    if( o.name.find("eff") != std::string::npos ){
      gStyle->SetOptStat(0);
      gStyle->SetPaintTextFormat(".0f");
      obj->SetOption("colztexte");
      obj->SetTitle("");
      //convert to percents, less digits to draw
      obj->Scale(100);
      //"colz" doesn't draw zero bins so increase them a bit. If bin has 0/0 i.e. 50+-50 make it -1 to avoid drawing
      for(int i=0; i<obj->GetSize(); i++){
        if( obj->operator[](i)==0 )
          obj->operator[](i) = 0.0001;
        if( obj->GetBinError(i)>49 )
          obj->operator[](i) = -1;
      }
      obj->GetZaxis()->SetRangeUser(-0.001,100.001);
    }else if( o.name.find("deltaEtaDeltaPhi") != std::string::npos ){
      gStyle->SetOptStat("emr");
      obj->SetOption("colz");
      c->SetLogz();
    }else{
      gStyle->SetOptStat("e");
      obj->SetOption("colztext");
    }
    gStyle->SetPalette(1);
  }

  void postDrawTH1F( TCanvas *, const DQMNet::CoreObject & ){
  }

  void postDrawTH2F( TCanvas *, const DQMNet::CoreObject & ){
  }
};

static HLTHeavyFlavorRenderPlugin instance;

