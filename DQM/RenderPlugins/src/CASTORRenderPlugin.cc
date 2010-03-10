
#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include <cassert>
#include "TROOT.h"
#include "TLatex.h"
#include "TLine.h"

 
class CASTORRenderPlugin : public DQMRenderPlugin
{

 public:


  ////---- define the histograms 
  virtual bool applies(const VisDQMObject &o, const VisDQMImgInfo &) {
 
     ////---- determine whether the object is a CASTOR object
    if( (o.name.find( "Castor/EventInfo/reportSummary" ) != std::string::npos) ||
        (o.name.find( "CastorRecHit 2D Energy Map- above threshold" ) != std::string::npos) )
        return true;

      return false;
    }


  virtual void preDraw( TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo &, VisDQMRenderInfo &) {
      c->cd();

      ////---- do it for TH2 
      if( dynamic_cast<TH2*>( o.object ) ) {
        preDrawTH2( c, o );
      }

  }


  virtual void postDraw( TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo &){
      c->cd();

      ////--- do it for TH2 
      if( dynamic_cast<TH2*>( o.object ) ) {
        postDrawTH2( c, o );
      }
  }




private:




  void preDrawTH2( TCanvas *c, const VisDQMObject &o ) {

      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      gStyle->SetCanvasBorderMode(0);
      gStyle->SetPadBorderMode(0);
      gStyle->SetPadBorderSize(0);
      gStyle->SetOptStat(0);
      obj->SetStats(kFALSE);
      obj->SetOption("colz");
      obj->GetXaxis()->SetLabelSize(0.05);
      obj->GetYaxis()->SetLabelSize(0.05);
      obj->GetXaxis()->SetTitle("module");
      obj->GetYaxis()->SetTitle("sector");
   

      if(o.name.find("reportSummaryMap") != std::string::npos)
      {
        obj->GetXaxis()->SetNdivisions(15,true);
        obj->GetYaxis()->SetNdivisions(17,true);
        obj->GetXaxis()->CenterLabels();
        obj->GetYaxis()->CenterLabels();
        c->SetGrid(1,1);

	int colorError1[3];
        colorError1[0] = 632;// kRed
        colorError1[1] = 400;// kYellow
	colorError1[2] = 416;// kGreen
        gStyle->SetPalette(3, colorError1);


        return;
      }
      
    if(o.name.find("CastorRecHit 2D Energy Map- above threshold") != std::string::npos)
      {
       
        obj->GetXaxis()->SetNdivisions(15,true);
        obj->GetYaxis()->SetNdivisions(17,true);
        obj->GetXaxis()->CenterLabels();
        obj->GetYaxis()->CenterLabels();
       
        gStyle->SetPalette(1);
        c->SetGrid(1,1); 
        //int colorError1[3];
        //colorError1[0] = 416;// kGreen
        //colorError1[1] = 400;// kYellow
        //colorError1[2] = 632;// kRed
        // gStyle->SetPalette(3, colorError1);
        return;
      }
    

    }


void postDrawTH2( TCanvas *, const VisDQMObject &o )
    {


      if(o.name.find("testOccupancy") != std::string::npos)
      {
        TH2F * histo =  dynamic_cast<TH2F*>( o.object );
        int nBinsX = histo->GetNbinsX();
        for(int i = 0; i !=12; ++i)
        {
          if(histo->GetBinContent(nBinsX+1,i+1) == -1)
          {
            TLine *lineLow = new TLine(1,i,nBinsX,i);
            lineLow->SetLineColor(kRed);
            TLine *lineHigh = new TLine(1,i+1,nBinsX,i+1);
            lineHigh->SetLineColor(kRed);
            lineLow->Draw("same");
            lineHigh->Draw("same");
          }
        }
        return;
      }
    }
};

static CASTORRenderPlugin instance;
