/*
 * =====================================================================================
 *
 *       Filename:  CSCRenderPlugin.cc
 *
 *    Description:  CSC Render Plugin for DQM
 *
 *        Version:  1.0
 *        Created:  05/06/2008 03:53:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), Valdas.Rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */


#include "DQM/RenderPlugins/src/CSCRenderPlugin.h"


bool CSCRenderPlugin::applies( const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

  if( o.name.find( "CSC/" ) == 0 ) {
    return true;
  } 

  return false;

}

void CSCRenderPlugin::preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo &r ) {

  std::cout << "CSCRenderPlugin:preDraw " << o.name << std::endl;

  TH1* obj = dynamic_cast<TH1*>( o.object );

  gStyle->SetPalette(1,0);

  if(REMATCH(".*/DMB_Unpacked$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_wo_ALCT$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_wo_CLCT$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_wo_CFEB$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_Reporting$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_input_fifo_full$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_input_timeout$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_Unpacked_with_errors$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_Unpacked_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_wo_ALCT_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_wo_CLCT_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_wo_CFEB_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_Format_Errors$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_Format_Errors_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    return;
  }
  
  if(REMATCH(".*/DMB_Format_Warnings$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_Format_Warnings_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    return;
  }
  
  if(REMATCH(".*/DMB_input_fifo_full_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/DMB_input_timeout_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_Unpacked$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_DMB_input_fifo_full$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_DMB_input_timeout$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_Reporting$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_wo_ALCT$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_wo_CLCT$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_wo_CFEB$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_Unpacked_with_errors$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_Unpacked_with_warnings$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_Format_Errors$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_Format_Errors_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    return;
  }
  
  if(REMATCH(".*/CSC_DMB_input_fifo_full_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_DMB_input_timeout_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_wo_ALCT_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_wo_CLCT_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_wo_CFEB_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_Unpacked_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_Format_Warnings$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/CSC_Format_Warnings_Fract$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    return;
  }
  
  if(REMATCH(".*/All_Readout_Errors$", o.name)) {
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLeftMargin(0.3);
    return;
  }
  
  if(REMATCH(".*/Readout_Errors$", o.name)) {
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLeftMargin(0.3);
    return;
  }
  
  if(REMATCH(".*/All_DDUs_in_Readout$", o.name)) {
    gStyle->SetOptStat("em");
    obj->SetStats(false);
    return;
  }
  
  if(REMATCH(".*/All_DDUs_L1A_Increment$", o.name)) {
    obj->SetStats(false);
    return;
  }
  
  if(REMATCH(".*/All_DDUs_Trailer_Errors$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLeftMargin(0.3);
    return;
  }
  
  if(REMATCH(".*/All_DDUs_Format_Errors$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLeftMargin(0.3);
    return;
  }
  
  if(REMATCH(".*/All_DDUs_Event_Size$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    return;
  }
  
  if(REMATCH(".*/All_DDUs_Average_Event_Size$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    return;
  }
  
  if(REMATCH(".*/All_DDUs_Live_Inputs$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/All_DDUs_Average_Live_Inputs$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    return;
  }
  
  if(REMATCH(".*/All_DDUs_Inputs_with_Data$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/All_DDUs_Average_Inputs_with_Data$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    return;
  }
  
  if(REMATCH(".*/All_DDUs_Inputs_Errors$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/All_DDUs_Inputs_Warnings$", o.name)) {
    gStyle->SetOptStat("e");
    obj->SetStats(false);
    gPad->SetGridx();
    gPad->SetGridy();
    return;
  }
  
  if(REMATCH(".*/BinaryChecker_Errors$", o.name)) {
    gStyle->SetOptStat("e");
    gPad->SetLeftMargin(0.6);
    return;
  }
  
  if(REMATCH(".*/BinaryChecker_Warnings$", o.name)) {
    gStyle->SetOptStat("e");
    gPad->SetLeftMargin(0.6);
    return;
  }
  
  if(REMATCH(".*/Buffer_Size$", o.name)) {
    gStyle->SetOptStat("emro");
    return;
  }
  
  if(REMATCH(".*/BXN$", o.name)) {
    gStyle->SetOptStat("e");
    return;
  }
  
  if(REMATCH(".*/Buffer_Size$", o.name)) {
    gStyle->SetOptStat("emro");
    return;
  }
  
  if(REMATCH(".*/CSC_Errors$", o.name)) {
    gStyle->SetOptStat("emro");
    return;
  }
  
  if(REMATCH(".*/CSC_Errors_Rate$", o.name)) {
    return;
  }
  
  if(REMATCH(".*/CSC_Warnings$", o.name)) {
    gStyle->SetOptStat("emro");
    return;
  }
  
  if(REMATCH(".*/CSC_Warnings_Rate$", o.name)) {
    return;
  }
  
  if(REMATCH(".*/DMB_Active_Header_Count$", o.name)) {
    gStyle->SetOptStat("em");
    return;
  }
  
  if(REMATCH(".*/DMB_Connected_Inputs$", o.name)) {
    gStyle->SetOptStat("e");
    return;
  }
  
  if(REMATCH(".*/DMB_Connected_Inputs_Rate$", o.name)) {
    return;
  }
  
  if(REMATCH(".*/DMB_DAV_Header_Count_vs_DMB_Active_Header_Count$", o.name)) {
    gStyle->SetOptStat("e");
    return;
  }
  
  if(REMATCH(".*/DMB_DAV_Header_Occupancy$", o.name)) {
    gStyle->SetOptStat("e");
    return;
  }
  
  if(REMATCH(".*/DMB_DAV_Header_Occupancy_Rate$", o.name)) {
    return;
  }
  
  if(REMATCH(".*/DMB_unpacked_vs_DAV$", o.name)) {
    gStyle->SetOptStat("e");
    return;
  }
  
  if(REMATCH(".*/L1A_Increment$", o.name)) {
    gStyle->SetOptStat("em");
    return;
  }
  
  if(REMATCH(".*/Readout_Errors$", o.name)) {
    gStyle->SetOptStat("e");
    gPad->SetLeftMargin(0.6);
    return;
  }
  
  if(REMATCH(".*/Trailer_ErrorStat_Frequency$", o.name)) {
    gStyle->SetOptStat("e");
    return;
  }
  
  if(REMATCH(".*/Trailer_ErrorStat_Rate$", o.name)) {
    return;
  }
  
  if(REMATCH(".*/Trailer_ErrorStat_Table$", o.name)) {
    gStyle->SetOptStat("e");
    gPad->SetLeftMargin(0.6);
    return;
  }
  
  if(REMATCH(".*/Word_Count$", o.name)) {
    gStyle->SetOptStat("emuo");
    return;
  }

  return;

}

void CSCRenderPlugin::postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

  return;

}

