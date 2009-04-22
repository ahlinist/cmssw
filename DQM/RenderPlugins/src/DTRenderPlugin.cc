// $Id: DTRenderPlugin.cc,v 1.47 2009/04/22 09:19:11 cerminar Exp $

/*!
  \file EBRenderPlugin
  \brief Display Plugin for Quality Histograms
  \author G. Masetti
  \version $Revision: 1.47 $
  \date $Date: 2009/04/22 09:19:11 $
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
 
  if((o.name.find( "DT/R" ) != std::string::npos) ||
     (o.name.find( "DT/0" ) != std::string::npos) ||
     (o.name.find( "DT/1" ) != std::string::npos) ||
     (o.name.find( "DT/9" ) != std::string::npos) ||
     (o.name.find( "DT/E" ) != std::string::npos) ||
     (o.name.find( "DT/F" ) != std::string::npos) ||
     (o.name.find( "DT/B" ) != std::string::npos) ||
     (o.name.find( "DT/C" ) != std::string::npos) ||
     (o.name.find( "DT/L" ) != std::string::npos)) {
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

  if(obj->GetEntries() != 0) c->SetLogz(0);

  //gStyle->SetLabelSize(0.7);
  obj->GetXaxis()->SetLabelSize(0.05);
  obj->GetYaxis()->SetLabelSize(0.05);



  if(o.name.find("Noise/NoiseSummary") != std::string::npos) {
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
  if( o.name.find( "reportSummaryMap" ) != std::string::npos ) {
    dqm::utils::reportSummaryMapPalette(obj);
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
//     obj->SetOption("text");
//     obj->SetMarkerSize( 2 );
//     gStyle->SetPaintTextFormat("2.0f");
    c->SetGrid(1,1);
    return;
  }

  if( o.name.find( "GlbSummary" ) != std::string::npos ) {
    dqm::utils::reportSummaryMapPalette(obj);
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
//     obj->SetOption("text,colz");
    obj->SetMarkerSize( 2 );
//     gStyle->SetPaintTextFormat("2.0f");
    c->SetGrid(1,1);
    return;
  }

  // --------------------------------------------------------------
  // Data integrity plots
  if( o.name.find("ROSStatus") != std::string::npos ) {
    c->SetGrid(1,1);
    c->SetBottomMargin(0.15);
    c->SetLeftMargin(0.15);
    return;
  }

  if( o.name.find("SCSizeVsROSSize") != std::string::npos ) {
    c->SetGrid(1,1);
    c->SetBottomMargin(0.15);
    c->SetLeftMargin(0.15);
    obj->GetXaxis()->CenterLabels();
  //   obj->GetYaxis()->CenterLabels();
    obj->GetXaxis()->SetNdivisions(13,true);

    return;
  }



  // --------------------------------------------------------------
  // Data integrity plots
  if( o.name.find("FIFOStatus") != std::string::npos ) {
    c->SetGrid(1,1);
    c->SetBottomMargin(0.15);
    c->SetLeftMargin(0.2);
    return;
  }

  if( o.name.find("ROSError") != std::string::npos ) {
    c->SetGrid(1,1);
    c->SetBottomMargin(0.15);
    c->SetLeftMargin(0.12);
    obj->GetXaxis()->SetLabelSize(0.05);
    obj->GetYaxis()->SetLabelSize(0.05);
    return;
  }

  if( o.name.find("TDCError") != std::string::npos ) {
    c->SetGrid(1,1);
    c->SetBottomMargin(0.15);
    c->SetLeftMargin(0.2);
    return;
  }

  if( o.name.find("ROSSummary") != std::string::npos ) {
    c->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->LabelsOption("v");
    c->SetBottomMargin(0.28);
    c->SetLeftMargin(0.14);
    c->SetRightMargin(0.14);

    return;
  }

  if(o.name.find("DataIntegritySummary") != std::string::npos) {
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

  if(o.name.find("SynchNoise/SynchNoiseSummary") != std::string::npos) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
//     obj->GetXaxis()->SetTitleOffset(1.15);
//     obj->GetXaxis()->LabelsOption("v");
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00001);
    obj->SetMaximum(2.0);

    int colorErrorDI[2];
    colorErrorDI[0] = 416;// kGreen
//     colorErrorDI[1] = 594;// kind of blue
    colorErrorDI[1] = 632;// kRed
    gStyle->SetPalette(2, colorErrorDI);
//     obj->SetOption("colz");
    return;
  }

  if( o.name.find("SyncNoiseEvents_W") != std::string::npos ) {
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
  }

  
  if((o.name.find("OccupancyAllHits_W") != std::string::npos) ||
     (o.name.find("OccupancyInTimeHits_W") != std::string::npos) ||
     (o.name.find("OccupancyNoiseHits_W") != std::string::npos)) {
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
  } else if(o.name.find("OccupancySummary_W") != std::string::npos) {
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
  }  else if(o.name.find("OccupancySummary") != std::string::npos) {
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
  } else if( o.name.find("Occupancy" ) != std::string::npos ) {
    c->SetGrid(0,4);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);

//     obj->SetStats(kTRUE); // FIXME: remove
//     gStyle->SetOptStat( 1111111 ); // FIXME: remove
    c->SetLeftMargin(0.15);
    return;
  }

  
  if( o.name.find("NoiseRate_W" ) != std::string::npos ) {
    c->SetGrid(0,4);
    //     obj->GetXaxis()->SetLabelSize(0.07);
    //     obj->GetYaxis()->SetLabelSize(0.07);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.15);
    return;
  }


  if( o.name.find( "SCTriggerBX" ) != std::string::npos ) {
    obj->GetYaxis()->SetLabelSize(0.1);
    obj->GetXaxis()->SetTitle("Trigger BX");
    obj->GetYaxis()->SetRangeUser(0.,40.);
    return;
  }
  
  if( o.name.find( "SCTriggerQuality" ) != std::string::npos ) {
    obj->GetXaxis()->LabelsOption("h");
    obj->GetXaxis()->SetLabelSize(0.1);
    obj->GetYaxis()->SetLabelSize(0.1);
    return;
  }


  // --------------------------------------------------------------
  // Trigger plots
  if( o.name.find("CorrFractionSummary_W") != std::string::npos ||
      o.name.find("PhiSlopeSummary_W")     != std::string::npos ||
      o.name.find("PhibSlopeSummary_W")    != std::string::npos ||
      o.name.find("2ndFractionSummary_W")  != std::string::npos ||
      o.name.find("MatchingSummary_W")     != std::string::npos ) {
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
    obj->SetMaximum(3.0);

    int colorError1[4];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 594;// kind of blue
    colorError1[2] = 632;// kRed
    colorError1[3] = 400;// kYellow
    gStyle->SetPalette(4, colorError1);
    return;
  } else if( o.name.find("CorrFractionSummary") != std::string::npos ||
	     o.name.find("PhiSlopeSummary")     != std::string::npos ||
 	     o.name.find("PhibSlopeSummary")    != std::string::npos ||
	     o.name.find("MatchingSummary")     != std::string::npos ||
	     o.name.find("2ndFractionSummary")  != std::string::npos ) {
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
    obj->SetMaximum(5.01);

    int colorError1[6];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 400;// kYellow
    colorError1[2] = 800;// kOrange
    colorError1[3] = 625;
    colorError1[4] = 632;// kRed
    colorError1[5] = 594;// kind of blue
    gStyle->SetPalette(6, colorError1);
    return;
  }  else if(o.name.find("2ndFraction")  != std::string::npos ||
	     o.name.find("CorrFraction") != std::string::npos ||
	     o.name.find("HFraction")    != std::string::npos ) {
    obj->GetXaxis()->SetNdivisions(13,true);
    if(o.name.find("Phi") != std::string::npos)
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
  }  else if(o.name.find("CorrectBX")        != std::string::npos ||
	     o.name.find("ResidualBX")       != std::string::npos ||
	     o.name.find("PhiTkvsTrig")      != std::string::npos ||
	     o.name.find("PhibTkvsTrig")     != std::string::npos ||
	     o.name.find("Matching")         != std::string::npos ||
	     o.name.find("TriggerInclusive") != std::string::npos ) {
    obj->GetXaxis()->SetNdivisions(13,true);
    if(o.name.find("Phi") != std::string::npos)
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
    if(o.name.find("TkvsTrigSlope") != std::string::npos) {
      obj->SetMaximum(1.15);
      obj->SetMinimum(0.85);
    }
    else if(o.name.find("TkvsTrigCorr") != std::string::npos) {
      obj->SetMaximum(1.00);
      obj->SetMinimum(0.90);
    }
    else if(o.name.find("TkvsTrigIntercept") != std::string::npos) {
      obj->SetMaximum(10.);
      obj->SetMinimum(-10.);
    } 
    else if(o.name.find("Matching")         != std::string::npos ||
	    o.name.find("TriggerInclusive") != std::string::npos) {
      obj->SetMinimum(-0.00000001);
    }
    else if(o.name.find("BX") != std::string::npos) {
      obj->SetOption("text");
      obj->SetMarkerSize( 2 );
      gStyle->SetPaintTextFormat("2.0f");
    }
    return;
  } else if (o.name.find("QualvsPhi")        != std::string::npos ||
             o.name.find("QualDDUvsQualDCC") != std::string::npos ||
             o.name.find("PositionvsQual")   != std::string::npos ||
             o.name.find("Flag1stvsQual")    != std::string::npos ) {
    obj->SetOption( "box" );
    return;
  }


  // --------------------------------------------------------------
  // Segments plots
  if(o.name.find("segmentSummary_W") != std::string::npos) {
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
  }  else if(o.name.find("segmentSummary") != std::string::npos) {
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
  } else if(o.name.find("numberOfSegments_W") != std::string::npos) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
//     obj->GetXaxis()->LabelsOption("v");
    return;
  }

  // --------------------------------------------------------------
  // Residuals plots
  if(o.name.find("MeanSummaryRes_W") != std::string::npos) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(12,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(3.0);

    int colorError1[3];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 632;// kRed
    colorError1[2] = 632;// kRed
    gStyle->SetPalette(3, colorError1);
    return;
    }  else if(o.name.find("00-MeanRes/MeanSummaryRes") != std::string::npos) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    return;
  } else if(o.name.find("SigmaSummaryRes_W") != std::string::npos) {
    labelMB4Sect4and13_wheel->Draw("same");
    labelMB4Sect10and14_wheel->Draw("same");
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(12,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(3.0);

    int colorError1[3];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 632;// kRed
    colorError1[2] = 632;// kRed
    gStyle->SetPalette(3, colorError1);
    return;
  }  else if(o.name.find("SigmaSummaryRes") != std::string::npos) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
  return;
  }

  if(o.name.find("EfficiencyMap") != std::string::npos) {
    obj->GetXaxis()->SetNdivisions(15,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    obj->SetMinimum(0.0);
    obj->SetMaximum(1.0);
    c->SetGrid(1,1);
    return;
  }

  if(o.name.find("CountSectVsChamb") != std::string::npos ||
     o.name.find("ExtrapSectVsChamb") != std::string::npos) {
    obj->GetXaxis()->SetNdivisions(15,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    c->SetRightMargin(0.15);
    return;
  }

  //----------------- calib validation plots ---------------------

    if(o.name.find("MeanSummaryRes_testFailed_") != std::string::npos) {
       //   obj->GetXaxis()->SetNdivisions(13,true);
       //obj->GetYaxis()->SetNdivisions(12,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(3.0);

    int colorError1[3];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 800;// kOrange
    colorError1[2] = 632;// kRed
    gStyle->SetPalette(3, colorError1);
    return;
   }  else if(o.name.find("MeanSummaryRes_testFailedByAtLeastBadSL") != std::string::npos) {
     // obj->GetXaxis()->SetNdivisions(13,true);
     //obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(2.0);

    int colorError1[2];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 632;// kRed
    gStyle->SetPalette(2, colorError1);
    return;
   }

    /*if(o.name.find("SetRange_2D") != std::string::npos) {
      obj->GetXaxis()->SetBinLabel(1,"MB1_SL1");
      obj->GetXaxis()->SetBinLabel(2,"MB1_SL2");
      obj->GetXaxis()->SetBinLabel(3,"MB1_SL3");
      obj->GetXaxis()->SetBinLabel(4,"MB2_SL1");
      obj->GetXaxis()->SetBinLabel(5,"MB2_SL2");
      obj->GetXaxis()->SetBinLabel(6,"MB2_SL3");
      obj->GetXaxis()->SetBinLabel(7,"MB3_SL1");
      obj->GetXaxis()->SetBinLabel(8,"MB3_SL2");
      obj->GetXaxis()->SetBinLabel(9,"MB3_SL3");
      obj->GetXaxis()->SetBinLabel(10,"MB4_SL1");
      obj->GetXaxis()->SetBinLabel(11,"MB4_SL3");
      obj->GetXaxis()->SetLabelSize(0.04);
      obj->GetYaxis()->SetLabelSize(0.04);
    }

  if((o.name.find("MeanWrong_") != std::string::npos) && (o.name.find("SetRange_2D") != std::string::npos)) {
    obj->GetXaxis()->SetNdivisions(11,true);
    obj->GetYaxis()->SetTitle("Mean(cm)");
    //obj->GetYaxis()->SetTitleOffset(2.0);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.1);
    c->SetRightMargin(0.12);
    c->SetGrid(1,0);
    
    return;
  }
  if((o.name.find("SigmaWrong_") != std::string::npos) && (o.name.find("SetRange_2D") != std::string::npos)) {
    obj->GetXaxis()->SetNdivisions(11,true);
    obj->GetYaxis()->SetTitle("Sigma(cm)");
    c->SetGrid(1,0);
    //obj->GetYaxis()->SetTitleOffset(2.0);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.1);
    c->SetRightMargin(0.12);
    return;
  }
  if((o.name.find("SlopeWrong_") != std::string::npos) && (o.name.find("SetRange_2D") != std::string::npos)) {
    obj->GetXaxis()->SetNdivisions(11,true);
    obj->GetYaxis()->SetTitle("Slope(cm)");
    c->SetGrid(1,0);
    //obj->GetYaxis()->SetTitleOffset(2.0);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.1);
    c->SetRightMargin(0.12);
    return;
  }
  if(o.name.find("hResDistVsDist_") != std::string::npos) {
    obj->GetXaxis()->SetLabelSize(0.04);
    obj->GetYaxis()->SetLabelSize(0.04);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.1);
    c->SetRightMargin(0.12);
    return;
    }*/

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
  obj->GetXaxis()->SetLabelSize(0.05);
  obj->GetYaxis()->SetLabelSize(0.05);

  if( o.name.find("MeanDistr") != std::string::npos ) {
    gStyle->SetOptStat( 1111111 );
    obj->SetStats( kTRUE );
    obj->GetXaxis()->SetLabelSize(0.05);
    obj->GetYaxis()->SetLabelSize(0.05);

    return;
  }
  
  if( o.name.find("SigmaDistr") != std::string::npos ) {
    gStyle->SetOptStat( 1111111 );
    obj->SetStats( kTRUE );
    obj->GetXaxis()->SetLabelSize(0.05);
    obj->GetYaxis()->SetLabelSize(0.05);

    return;
  }

  if( o.name.find("ROSEventLenght") != std::string::npos ) {
     if(obj->GetEntries() != 0) c->SetLogy(1);
    gStyle->SetOptStat( 1111111 );
    obj->SetStats( kTRUE );
//     c->SetGrid(1,1);
//     c->SetBottomMargin(0.15);
//     c->SetLeftMargin(0.2);
//     obj->GetXaxis()->SetLabelSize(0.05);
//     obj->GetYaxis()->SetLabelSize(0.05);

    return;
  }


  if( o.name.find("hResDist") != std::string::npos ) {
    gStyle->SetOptStat("rme" );
    gStyle->SetOptFit(1);
    obj->SetStats( kTRUE );
  }



  if( o.name.find( "EventLenght" ) != std::string::npos ) {
    gStyle->SetOptStat( 1111111 );
    obj->SetStats( kTRUE );
     if(obj->GetEntries() != 0) c->SetLogy(1);
    return;
  }


  if( o.name.find( "FED770TTSValues_Percent" ) != std::string::npos ) {
     if(obj->GetEntries() != 0) c->SetLogy( 1 );
    return;
  }
  
  if( o.name.find( "MeanTest" ) != std::string::npos ) {
    obj->GetYaxis()->SetRangeUser(-0.1,0.1);
  }

   if( o.name.find( "SigmaTest" ) != std::string::npos ) {
    obj->GetYaxis()->SetRangeUser(0.,0.2);
  }
  
   if( o.name.find( "SlopeTest" ) != std::string::npos ) {
    obj->GetYaxis()->SetRangeUser(-0.05,0.05);
  }


  if( o.name.find( "hResDist" )        != std::string::npos ||
      o.name.find( "MeanTest" )        != std::string::npos ||
      o.name.find( "SigmaTest" )       != std::string::npos ||
      o.name.find( "SlopeTest" )       != std::string::npos ||
      o.name.find( "xEfficiency" )     != std::string::npos ||
      o.name.find( "yEfficiency" )     != std::string::npos ||
      o.name.find( "Efficiency_" )     != std::string::npos ||
      o.name.find( "OccupancyDiff_" )  != std::string::npos ||
      o.name.find( "tTrigTest" )       != std::string::npos || 
      o.name.find( "2ndFraction" )     != std::string::npos ||
      o.name.find( "CorrFraction" )    != std::string::npos ) {
    
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
  
    if ( o.name.find( "tTrigTest" ) != std::string::npos ) {
      obj->GetXaxis()->SetBinLabel(1,"SL1");
      obj->GetXaxis()->SetBinLabel(2,"SL2");
      obj->GetXaxis()->SetBinLabel(3,"SL3");
      return;
    }


    // ----------------- Calib validation plots -------------------
    if(o.name.find("SetRange") != std::string::npos) {
      obj->GetXaxis()->SetBinLabel(1,"MB1_SL1");
      obj->GetXaxis()->SetBinLabel(2,"MB1_SL2");
      obj->GetXaxis()->SetBinLabel(3,"MB1_SL3");
      obj->GetXaxis()->SetBinLabel(4,"MB2_SL1");
      obj->GetXaxis()->SetBinLabel(5,"MB2_SL2");
      obj->GetXaxis()->SetBinLabel(6,"MB2_SL3");
      obj->GetXaxis()->SetBinLabel(7,"MB3_SL1");
      obj->GetXaxis()->SetBinLabel(8,"MB3_SL2");
      obj->GetXaxis()->SetBinLabel(9,"MB3_SL3");
      obj->GetXaxis()->SetBinLabel(10,"MB4_SL1");
      obj->GetXaxis()->SetBinLabel(11,"MB4_SL3");
      obj->GetXaxis()->SetLabelSize(0.04);
      obj->GetYaxis()->SetLabelSize(0.04);
      return;
    }
    

    
  // --------------------------------------------------------------
  // Trigger plots
  if ( o.name.find( "2ndFraction" )  != std::string::npos ||
       o.name.find( "CorrFraction" ) != std::string::npos || 
       o.name.find( "HFraction" )    != std::string::npos) {
    obj->GetYaxis()->SetRangeUser(0.,1.1);
    return;
  } else if ( o.name.find( "CorrectBX" ) != std::string::npos ) {
    if (o.name.find( "DCC" ) != std::string::npos)
      obj->GetYaxis()->SetRangeUser(-5.,5.);
    else
      obj->GetYaxis()->SetRangeUser(0.,20.);
    return;
  }

  if( o.name.find("DCC_ErrorsChamberID") != std::string::npos ) {
    c->SetGrid(1,0);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    return;
  }


  if( o.name.find( "NoiseRateSummary" ) != std::string::npos ) {

     if(obj->GetEntries() != 0) c->SetLogy(1);
    if(obj->GetEntries() != 0) c->SetLogx(1);
    return;
  }


  if(o.name.find("FEDIntegrity" ) < o.name.size()) {
    obj->GetXaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    c->SetGrid(1,0);
    return;
  }

  
  if(o.name.find("NSegmPerEvent_W" ) < o.name.size()) {
//     obj->GetXaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    c->SetGrid(0,1);
    c->SetBottomMargin(0.1);
    obj->GetXaxis()->LabelsOption("u");
    obj->GetXaxis()->SetLabelSize(0.03);

    return;
  }

  if(o.name.find("NevtPerLS" ) < o.name.size()) {
//     obj->GetXaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    c->SetGrid(0,1);
    c->SetBottomMargin(0.1);
    obj->GetXaxis()->LabelsOption("u");
    obj->GetXaxis()->SetLabelSize(0.03);

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

  if(o.name.find("DataIntegritySummary") != std::string::npos) {
    static TLatex *whm2Label =  new TLatex(-1.5,770.1,"(Wheel -2)");
    whm2Label->SetTextSize(0.042);
    whm2Label->Draw("same");

    static TLatex *whm1Label =  new TLatex(-1.5,771.1,"(Wheel -1)");
    whm1Label->SetTextSize(0.042);
    whm1Label->Draw("same");

    static TLatex *wh0Label =  new TLatex(-1.5,772.1,"(Wheel 0)");
    wh0Label->SetTextSize(0.042);
    wh0Label->Draw("same");

    static TLatex *whp1Label =  new TLatex(-1.5,773.1,"(Wheel +1)");
    whp1Label->SetTextSize(0.042);
    whp1Label->Draw("same");

    static TLatex *whp2Label =  new TLatex(-1.5,774.1,"(Wheel +2)");
    whp2Label->SetTextSize(0.042);
    whp2Label->Draw("same");


    return;
  }

  if(o.name.find("ROSError") != std::string::npos) {
    TH2F * histo =  dynamic_cast<TH2F*>( o.object );
    int nBinsX = histo->GetNbinsX();

    static TLine *lineMB1 = new TLine(0,6,nBinsX,6); 
    lineMB1->Draw("same");
    static TLatex *mb1Label = new TLatex(5,1.5,"MB1");
    mb1Label->SetTextColor(kGray);
    mb1Label->SetTextSize(0.11);
    mb1Label->Draw("same");

    static TLine *lineMB2 = new TLine(0,12,nBinsX,12); 
    lineMB2->Draw("same");
    static TLatex *mb2Label = new TLatex(5,7.5,"MB2");
    mb2Label->SetTextColor(kGray);
    mb2Label->SetTextSize(0.11);
    mb2Label->Draw("same");

    static TLine *lineMB3 = new TLine(0,18,nBinsX,18); 
    lineMB3->Draw("same");
    static TLatex *mb3Label = new TLatex(5,13.5,"MB3");
    mb3Label->SetTextColor(kGray);
    mb3Label->SetTextSize(0.11);
    mb3Label->Draw("same");

    static TLine *lineMB4 = new TLine(0,24,nBinsX,24); 
    lineMB4->Draw("same");
    static TLatex *mb4Label = new TLatex(5,19.5,"MB4");
    mb4Label->SetTextColor(kGray);
    mb4Label->SetTextSize(0.11);
    mb4Label->Draw("same");

    static TLine *lineMB3b = new TLine(0,25,nBinsX,25); 
    lineMB3b->Draw("same");

    return;
  }
  

  if(o.name.find("Summary_W") != std::string::npos) {
    labelMB4Sect4and13_wheel->Draw("same");
    labelMB4Sect10and14_wheel->Draw("same");
    return;
  }
  if(o.name.find("OccupancyAllHits_perCh") != std::string::npos) {
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

  if(o.name.find("MeanSummaryRes_W") != std::string::npos
     || o.name.find("SigmaSummaryRes_W") != std::string::npos) {
    static TLatex * lblMB4Sect4and13_res = new TLatex(4,10.75,"4/13");
    static TLatex * lblMB4Sect10and14_res = new TLatex(9.75,10.75,"10/14");

    lblMB4Sect4and13_res->Draw("same");
    lblMB4Sect10and14_res->Draw("same");
    static TLine *lineMB1_res = new TLine(1,4,13,4); 
    lineMB1_res->Draw("same");
    static TLine *lineMB2_res = new TLine(1,7,13,7); 
    lineMB2_res->Draw("same");
    static TLine *lineMB3_res = new TLine(1,10,13,10); 
    lineMB3_res->Draw("same");



    return;
  }

  return;

}

void DTRenderPlugin::postDrawTH1( TCanvas *c, const DQMNet::CoreObject &o ) {

  if( o.name.find("EventLenght") != std::string::npos ) {
    TH1F * histo =  dynamic_cast<TH1F*>( o.object );
    int nBins = histo->GetNbinsX();
    if(histo->GetBinContent(nBins) != 0 || histo->GetBinContent(nBins+1) != 0) {
      TLatex *labelOverflow = new TLatex(0.5,0.5,"Overflow");
      labelOverflow->SetTextColor(kRed);
      labelOverflow->SetNDC();
      labelOverflow->Draw("same");
    }
    return;
  }
  return;

}

