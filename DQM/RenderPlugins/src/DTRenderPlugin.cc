// $Id: DTRenderPlugin.cc,v 1.19 2008/06/10 15:48:31 cerminar Exp $

/*!
  \file EBRenderPlugin
  \brief Display Plugin for Quality Histograms
  \author G. Masetti
  \version $Revision: 1.19 $
  \date $Date: 2008/06/10 15:48:31 $
*/

#include "TProfile2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include <cassert>

#include "DQM/RenderPlugins/src/DTRenderPlugin.h"


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

//  gPad->SetFrameFillColor(10);
//  if (o.error) gPad->SetFillColor(2);
//  if (o.warning) gPad->SetFillColor(5);
//  if (o.other) gPad->SetFillColor(16);

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

  TH2* obj = dynamic_cast<TH2*>( o.object );
  assert( obj );

  // This applies to all
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBorderSize(0);

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

  obj->SetStats(kFALSE);
  obj->SetOption("colz");

  gPad->SetLogz(0);

  gStyle->SetLabelSize(0.7);
  obj->GetXaxis()->SetLabelSize(0.07);
  obj->GetYaxis()->SetLabelSize(0.07);

  // --------------------------------------------------------------
  // Data integrity plots
  if( o.name.find("ROSStatus") < o.name.size() ) {
    gPad->SetGrid(1,1);
    gPad->SetBottomMargin(0.15);
    gPad->SetLeftMargin(0.15);
    return;
  }

  // --------------------------------------------------------------
  // Data integrity plots
  if( o.name.find("FIFOStatus") < o.name.size() ) {
    gPad->SetGrid(1,1);
    gPad->SetBottomMargin(0.15);
    gPad->SetLeftMargin(0.2);
    return;
  }

  if( o.name.find("ROSError") < o.name.size() ) {
    gPad->SetGrid(1,1);
    gPad->SetBottomMargin(0.15);
    gPad->SetLeftMargin(0.2);
    obj->GetXaxis()->SetLabelSize(0.05);
    obj->GetYaxis()->SetLabelSize(0.05);
    return;
  }

  if( o.name.find("TDCError") < o.name.size() ) {
    gPad->SetGrid(1,1);
    gPad->SetBottomMargin(0.15);
    gPad->SetLeftMargin(0.2);
    return;
  }

  if( o.name.find("ROSSummary") < o.name.size() ) {
    gPad->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->LabelsOption("v");
    gPad->SetBottomMargin(0.25);
    gPad->SetLeftMargin(0.15);
    return;
  }

  if(o.name.find("DataIntegritySummary") < o.name.size()) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    gPad->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->SetTitleOffset(1.15);
//     obj->GetXaxis()->LabelsOption("v");
    gPad->SetBottomMargin(0.1);
    gPad->SetLeftMargin(0.15);
    gPad->SetRightMargin(0.12);
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
    gPad->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);

//     obj->GetXaxis()->LabelsOption("v");
    gPad->SetBottomMargin(0.1);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.12);
    return;
  } else if(o.name.find("OccupancySummary_W") < o.name.size()) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    gPad->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->LabelsOption("v");
    gPad->SetBottomMargin(0.1);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.12);
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
    gPad->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->LabelsOption("v");
    gPad->SetBottomMargin(0.1);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.12);
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
    gPad->SetGrid(0,4);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    gPad->SetLeftMargin(0.15);
    return;
  }


  if( o.name.find( "FED770_EventLenght" ) < o.name.size() ) {
    gStyle->SetOptStat( 1111111 );
    gStyle->SetPalette( 1 );
    obj->SetStats( kTRUE );
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
  if( o.name.find("CorrFractionSummary_W") < o.name.size() ) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    gPad->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);

//     obj->GetXaxis()->LabelsOption("v");
    gPad->SetBottomMargin(0.1);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.12);
    obj->GetXaxis()->LabelsOption("v");
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(2.0);

    int colorError1[3];
    colorError1[0] = 416;// kGreen
    colorError1[1] = 594;// kind of blue
    colorError1[2] = 632;// kRed
    gStyle->SetPalette(3, colorError1);
    return;
  } else if(o.name.find("CorrFractionSummary") < o.name.size()) {
    obj->GetXaxis()->SetNdivisions(13,true);
    obj->GetYaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    gPad->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->LabelsOption("v");
    gPad->SetBottomMargin(0.1);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.12);
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
  }  else if(o.name.find("2ndFraction") <  o.name.size() ||
	     o.name.find("CorrFraction") < o.name.size() ||
	     o.name.find("HFraction") <    o.name.size() ) {
    obj->GetXaxis()->SetNdivisions(13,true);
    if(o.name.find("Phi") < o.name.size())
      obj->GetYaxis()->SetNdivisions(5,true); //Phi Summary
    else
      obj->GetYaxis()->SetNdivisions(4,true); //Theta Summary     
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    gPad->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    //obj->GetXaxis()->LabelsOption("v");
    gPad->SetBottomMargin(0.1);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(1.0);
    return;
  }  else if(o.name.find("CorrectBX")        < o.name.size() ||
	     o.name.find("TriggerInclusive") < o.name.size() ) {
    obj->GetXaxis()->SetNdivisions(13,true);
    if(o.name.find("Phi") < o.name.size())
      obj->GetYaxis()->SetNdivisions(5,true); //Phi Summary
    else
      obj->GetYaxis()->SetNdivisions(4,true); //Theta Summary     
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    gPad->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->LabelsOption("v");
    gPad->SetBottomMargin(0.1);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.12);
    if(o.name.find("TriggerInclusive") < o.name.size())
      obj->SetMinimum(-0.00000001);
    if(o.name.find("CorrectBX") < o.name.size()) {
      obj->SetOption("text");
      obj->SetMarkerSize( 2 );
    }
    return;
  } else if (o.name.find( "QualvsPhi" ) <o.name.size() ||
      o.name.find( "QualDDUvsQualDCC" ) <o.name.size() ||
      o.name.find( "PositionvsQual" )   <o.name.size() ||
      o.name.find( "Flag1stvsQual" )    <o.name.size() ) {
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
    gPad->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    //    obj->GetXaxis()->LabelsOption("v");
    gPad->SetBottomMargin(0.1);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.12);
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
    gPad->SetGrid(1,1);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    //    obj->GetXaxis()->LabelsOption("v");
    gPad->SetBottomMargin(0.1);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.12);
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
  gStyle->SetLabelSize(0.7);
  obj->GetXaxis()->SetLabelSize(0.07);
  obj->GetYaxis()->SetLabelSize(0.07);


  if( o.name.find( "FED770TTSValues_Percent" ) < o.name.size() ) {
    gPad->SetLogy( 1 );
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
    gPad->SetGrid(1,0);
//     obj->GetXaxis()->SetLabelSize(0.07);
//     obj->GetYaxis()->SetLabelSize(0.07);
    obj->GetXaxis()->SetNdivisions(6,true);
    obj->GetXaxis()->CenterLabels();
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
    postDrawTH2( c, o );
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

  return;

}

void DTRenderPlugin::postDrawTH1( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

