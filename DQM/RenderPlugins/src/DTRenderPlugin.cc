// $Id: DTRenderPlugin.cc,v 1.23 2008/07/03 17:36:38 ameyer Exp $

/*!
  \file EBRenderPlugin
  \brief Display Plugin for Quality Histograms
  \author G. Masetti
  \version $Revision: 1.23 $
  \date $Date: 2008/07/03 17:36:38 $
*/

#include "TProfile2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include <cassert>
#include "TLatex.h"
#include "TLine.h"

#include "DQM/RenderPlugins/src/DTRenderPlugin.h"
#include "DQM/RenderPlugins/src/utils.h"



DTRenderPlugin::DTRenderPlugin() {

  labelMB4Sect_global = 0;
  labelMB4Sect4and13_wheel = new TLatex(4,4.5,"4/13");
  labelMB4Sect10and14_wheel = new TLatex(9.85,4.5,"10/14");

}

bool DTRenderPlugin::applies( const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {
 
  if( o.name.find( "DT/" ) < o.name.size() ) {
    return true;
  } 

  return false;

}

void DTRenderPlugin::preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo &r ) {

#ifdef DEBUG
  std::cout << "DTRenderPlugin:preDraw " << o.name << std::endl;
#endif

  c->cd();

//  c->SetFrameFillColor(10);
//  if (o.error) c->SetFillColor(2);
//  if (o.warning) c->SetFillColor(5);
//  if (o.other) c->SetFillColor(16);

  if( dynamic_cast<TProfile2D*>( o.object ) ) {
    preDrawTProfile2D( c, o );
  }
  else if( dynamic_cast<TProfile*>( o.object ) ) {
    preDrawTProfile( c, o );
  }
  else if( dynamic_cast<TH2*>( o.object ) ) {
    preDrawTH2( c, o );
  }
  else if( dynamic_cast<TH1*>( o.object ) ) {
    preDrawTH1( c, o );
  }

#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}

void DTRenderPlugin::preDrawTProfile2D( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void DTRenderPlugin::preDrawTProfile( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void DTRenderPlugin::preDrawTH2( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH2F* obj = dynamic_cast<TH2F*>( o.object );
  assert( obj );

  // This applies to all
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBorderSize(0);

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

  obj->SetStats(kFALSE);
  obj->SetOption("colz");

  c->SetLogz(0);

  //gStyle->SetLabelSize(0.7);
  obj->GetXaxis()->SetLabelSize(0.07);
  obj->GetYaxis()->SetLabelSize(0.07);



  if(o.name.find("NoiseSummary") < o.name.size()) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    obj->GetXaxis()->SetTitleOffset(1.15);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.12);
    return;
  }


  
  // Summary map
  if( o.name.find( "reportSummaryMap" ) < o.name.size() ) {
    dqm::utils::reportSummaryMapPalette(obj);
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    return;
  }


  // --------------------------------------------------------------
  // Data integrity plots
  if( o.name.find("ROSStatus") < o.name.size() ) {
    c->SetGrid(1,1);
    c->SetBottomMargin(0.15);
    c->SetLeftMargin(0.15);
    return;
  }

  // --------------------------------------------------------------
  // Data integrity plots
  if( o.name.find("FIFOStatus") < o.name.size() ) {
    c->SetGrid(1,1);
    c->SetBottomMargin(0.15);
    c->SetLeftMargin(0.2);
    return;
  }

  if( o.name.find("ROSError") < o.name.size() ) {
    c->SetGrid(1,1);
    c->SetBottomMargin(0.15);
    c->SetLeftMargin(0.2);
    obj->GetXaxis()->SetLabelSize(0.05);
    obj->GetYaxis()->SetLabelSize(0.05);
    return;
  }

  if( o.name.find("TDCError") < o.name.size() ) {
    c->SetGrid(1,1);
    c->SetBottomMargin(0.15);
    c->SetLeftMargin(0.2);
    return;
  }

  if( o.name.find("ROSSummary") < o.name.size() ) {
    c->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->LabelsOption("v");
    c->SetBottomMargin(0.25);
    c->SetLeftMargin(0.15);
    return;
  }

  if(o.name.find("DataIntegritySummary") < o.name.size()) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->SetTitleOffset(1.15);
//     obj->GetXaxis()->LabelsOption("v");
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(3.0);
    
    int colorErrorDI[3];
    colorErrorDI[0] = 416;// kGreen
    colorErrorDI[1] = 594;// kind of blue
    colorErrorDI[2] = 632;// kRed
    gStyle->SetPalette(3, colorErrorDI);
    return;
  }



  
  if( o.name.find("OccupancyAllHits_W") < o.name.size() ) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);

//     obj->GetXaxis()->LabelsOption("v");
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.12);
    return;
  } else if(o.name.find("OccupancySummary_W") < o.name.size()) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->LabelsOption("v");
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(5.0);

    int colorError1[5];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 400;// kYellow
    colorError1[2] = 800;// kOrange
    colorError1[3] = 625;
    colorError1[4] = 632;// kRed
    gStyle->SetPalette(5, colorError1);
    return;
  }  else if(o.name.find("OccupancySummary") < o.name.size()) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->LabelsOption("v");
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(5.0);

    int colorError1[5];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 400;// kYellow
    colorError1[2] = 800;// kOrange
    colorError1[3] = 625;
    colorError1[4] = 632;// kRed
    gStyle->SetPalette(5, colorError1);
    return;
  } else if( o.name.find("Occupancy" ) < o.name.size() ) {
    c->SetGrid(0,4);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    c->SetLeftMargin(0.15);
    return;
  }

  
  if( o.name.find("NoiseRate_W" ) < o.name.size() ) {
    c->SetGrid(0,4);
    //     obj->GetXaxis()->SetLabelSize(0.07);
    //     obj->GetYaxis()->SetLabelSize(0.07);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.15);
    return;
  }


  if( o.name.find( "SCTriggerBX" ) < o.name.size() ) {
    obj->GetYaxis()->SetLabelSize(0.1);
    obj->GetXaxis()->SetTitle("Trigger BX");
    obj->GetYaxis()->SetRangeUser(0.,40.);
    return;
  }
  
  if( o.name.find( "SCTriggerQuality" ) < o.name.size() ) {
    obj->GetXaxis()->LabelsOption("h");
    obj->GetXaxis()->SetLabelSize(0.1);
    obj->GetYaxis()->SetLabelSize(0.1);
    return;
  }


  // --------------------------------------------------------------
  // Trigger plots
  if( o.name.find("CorrFractionSummary_W") < o.name.size() ||
      o.name.find("PhiSlopeSummary_W")     < o.name.size() ||
      o.name.find("PhibSlopeSummary_W")    < o.name.size() ||
      o.name.find("2ndFractionSummary_W")  < o.name.size() ) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.12);
    obj->GetXaxis()->LabelsOption("v");
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(2.0);

    int colorError1[3];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 594;// kind of blue
    colorError1[2] = 632;// kRed
    gStyle->SetPalette(3, colorError1);
    return;
  } else if( o.name.find("CorrFractionSummary") < o.name.size() ||
	     o.name.find("PhiSlopeSummary")     < o.name.size() ||
	     o.name.find("PhibSlopeSummary")    < o.name.size() ||
	     o.name.find("2ndFractionSummary")  < o.name.size() ) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    obj->GetXaxis()->LabelsOption("v");
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(5.0);

    int colorError1[5];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 400;// kYellow
    colorError1[2] = 800;// kOrange
    colorError1[3] = 625;
    colorError1[4] = 632;// kRed
    gStyle->SetPalette(5, colorError1);
    return;
  }  else if(o.name.find("2ndFraction")  < o.name.size() ||
	     o.name.find("CorrFraction") < o.name.size() ||
	     o.name.find("HFraction")    < o.name.size() ) {
    obj->GetXaxis()->SetNdivisions(13,true);
    if(o.name.find("Phi") < o.name.size())
      obj->GetYaxis()->SetNdivisions(5,true); //Phi Summary
    else
      obj->GetYaxis()->SetNdivisions(4,true); //Theta Summary     
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(1.0);
    return;
  }  else if(o.name.find("CorrectBX")        < o.name.size() ||
	     o.name.find("ResidualBX")       < o.name.size() ||
	     o.name.find("PhiTkvsTrig")      < o.name.size() ||
	     o.name.find("PhibTkvsTrig")     < o.name.size() ||
	     o.name.find("TriggerInclusive") < o.name.size() ) {
    obj->GetXaxis()->SetNdivisions(13,true);
    if(o.name.find("Phi") < o.name.size())
      obj->GetYaxis()->SetNdivisions(5,true); //Phi Summary
    else
      obj->GetYaxis()->SetNdivisions(4,true); //Theta Summary     
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    obj->GetXaxis()->LabelsOption("v");
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.12);
    if(o.name.find("TkvsTrigSlope") < o.name.size()) {
      obj->SetMaximum(1.15);
      obj->SetMinimum(0.85);
    }
    else if(o.name.find("TkvsTrigCorr") < o.name.size()) {
      obj->SetMaximum(1.00);
      obj->SetMinimum(0.90);
    }
    else if(o.name.find("TkvsTrigIntercept") < o.name.size()) {
      obj->SetMaximum(10.);
      obj->SetMinimum(-10.);
    }
    else if(o.name.find("TriggerInclusive") < o.name.size()) {
      obj->SetMinimum(-0.00000001);
    }
    else if(o.name.find("BX") < o.name.size()) {
      obj->SetOption("text");
      obj->SetMarkerSize( 2 );
      gStyle->SetPaintTextFormat("2.0f");
    }
    return;
  } else if (o.name.find("QualvsPhi")        < o.name.size() ||
             o.name.find("QualDDUvsQualDCC") < o.name.size() ||
             o.name.find("PositionvsQual")   < o.name.size() ||
             o.name.find("Flag1stvsQual")    < o.name.size() ) {
    obj->SetOption( "box" );
    return;
  }


  // --------------------------------------------------------------
  // Segments plots
  if(o.name.find("segmentSummary_W") < o.name.size()) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    //    obj->GetXaxis()->LabelsOption("v");
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(3.0);

    int colorError1[3];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 400;// kYellow
    colorError1[2] = 632;// kRed
    gStyle->SetPalette(3, colorError1);
    return;
  }  else if(o.name.find("segmentSummary") < o.name.size()) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    //    obj->GetXaxis()->LabelsOption("v");
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(3.0);

    int colorError1[3];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 400;// kYellow
    colorError1[2] = 632;// kRed
    gStyle->SetPalette(3, colorError1);
    return;
  } else if(o.name.find("numberOfSegments_W") < o.name.size()) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
//     obj->GetXaxis()->LabelsOption("v");
  }
  
}

void DTRenderPlugin::preDrawTH1( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH1* obj = dynamic_cast<TH1*>( o.object );

  assert( obj );

  // This applies to all
  gStyle->SetCanvasBorderMode( 0 );
  gStyle->SetPadBorderMode( 0 );
  gStyle->SetPadBorderSize( 0 );
  obj->SetStats( kFALSE );
  //gStyle->SetLabelSize(0.7);
  obj->GetXaxis()->SetLabelSize(0.07);
  obj->GetYaxis()->SetLabelSize(0.07);

  

  if( o.name.find("ROSEventLenght") < o.name.size() ) {
    c->SetLogy(1);
    gStyle->SetOptStat( 1111111 );
    obj->SetStats( kTRUE );
//     c->SetGrid(1,1);
//     c->SetBottomMargin(0.15);
//     c->SetLeftMargin(0.2);
//     obj->GetXaxis()->SetLabelSize(0.05);
//     obj->GetYaxis()->SetLabelSize(0.05);

    return;
  }

  if( o.name.find( "EventLenght" ) < o.name.size() ) {
    gStyle->SetOptStat( 1111111 );
    obj->SetStats( kTRUE );
    c->SetLogy(1);
    return;
  }


  if( o.name.find( "FED770TTSValues_Percent" ) < o.name.size() ) {
    c->SetLogy( 1 );
    return;
  }


  if( o.name.find( "hResDist" )        < o.name.size() ||
      o.name.find( "MeanTest" )        < o.name.size() ||
      o.name.find( "SigmaTest" )       < o.name.size() ||
      o.name.find( "xEfficiency" )     < o.name.size() ||
      o.name.find( "yEfficiency" )     < o.name.size() ||
      o.name.find( "Efficiency_" )     < o.name.size() ||
      o.name.find( "OccupancyDiff_" )  < o.name.size() ||
      o.name.find( "tTrigTest" )       < o.name.size() || 
      o.name.find( "2ndFraction" )     < o.name.size() ||
      o.name.find( "CorrFraction" )    < o.name.size() ) {
    
    TAttLine *line = dynamic_cast<TAttLine *> (o.object);
    assert (line);
    
    if (line) {
      if (o.flags & DQMNet::DQM_FLAG_REPORT_ERROR) {
	line->SetLineColor(TColor::GetColor("#CC0000"));
      }
      else if (o.flags & DQMNet::DQM_FLAG_REPORT_WARNING) {
	line->SetLineColor(TColor::GetColor("#993300"));
      }
      else if (o.flags & DQMNet::DQM_FLAG_REPORT_OTHER) { 
	line->SetLineColor(TColor::GetColor("#FFCC00"));
      }
      else {
	line->SetLineColor(TColor::GetColor("#000000"));
      }  
    }   

  }
  
    if ( o.name.find( "tTrigTest" ) < o.name.size() ) {
      obj->GetXaxis()->SetBinLabel(1,"SL1");
      obj->GetXaxis()->SetBinLabel(2,"SL2");
      obj->GetXaxis()->SetBinLabel(3,"SL3");
      return;
    }

    
  // --------------------------------------------------------------
  // Trigger plots
  if ( o.name.find( "2ndFraction" )  < o.name.size() ||
       o.name.find( "CorrFraction" ) < o.name.size() || 
       o.name.find( "HFraction" )    < o.name.size()) {
    obj->GetYaxis()->SetRangeUser(0.,1.1);
    return;
  } else if ( o.name.find( "CorrectBX" ) < o.name.size() ) {
    if (o.name.find( "DCC" ) < o.name.size())
      obj->GetYaxis()->SetRangeUser(-5.,5.);
    else
      obj->GetYaxis()->SetRangeUser(0.,20.);
    return;
  }

  if( o.name.find("DCC_ErrorsChamberID") < o.name.size() ) {
    c->SetGrid(1,0);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    return;
  }


  if( o.name.find( "NoiseRateSummary" ) < o.name.size() ) {
    c->SetLogy(1);
    c->SetLogx(1);
    return;
  }


  return;

}

void DTRenderPlugin::postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

#ifdef DEBUG
  std::cout << "DTRenderPlugin:postDraw " << o.name << std::endl;
#endif

  c->cd();

  if( dynamic_cast<TProfile2D*>( o.object ) ) {
    postDrawTProfile2D( c, o );
  }
  else if( dynamic_cast<TProfile*>( o.object ) ) {
    postDrawTProfile( c, o );
  }
  else if( dynamic_cast<TH2*>( o.object ) ) {
    postDrawTH2( c, o );
  }
  else if( dynamic_cast<TH1*>( o.object ) ) {
    postDrawTH1( c, o );
  }


#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}

void DTRenderPlugin::postDrawTProfile2D( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void DTRenderPlugin::postDrawTProfile( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void DTRenderPlugin::postDrawTH2( TCanvas *c, const DQMNet::CoreObject &o ) {
  if(o.name.find("Summary_W") < o.name.size()) {
    labelMB4Sect4and13_wheel->Draw("same");
    labelMB4Sect10and14_wheel->Draw("same");
    return;
  }
  if(o.name.find("OccupancyAllHits_perCh") < o.name.size()) {
    TH2F * histo =  dynamic_cast<TH2F*>( o.object );
    int nBinsX = histo->GetNbinsX();
    for(int i = 0; i !=12; ++i) {
      if(histo->GetBinContent(nBinsX+1,i+1) == -1) {
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
  
  return;

}

void DTRenderPlugin::postDrawTH1( TCanvas *c, const DQMNet::CoreObject &o ) {
  if( o.name.find("ROSEventLenght") < o.name.size() ) {
    TH1F * histo =  dynamic_cast<TH1F*>( o.object );
    int nBins = histo->GetNbinsX();
    if(histo->GetBinContent(nBins+1) != 0) {
      TLatex *labelOverflow = new TLatex(0.5,0.5,"Overflow");
      labelOverflow->SetTextColor(kRed);
      labelOverflow->SetNDC();
      labelOverflow->Draw("same");
    }
    return;
  }
  return;

}

