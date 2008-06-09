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

  if ( o.name.find( "CSC/" ) < o.name.size() ) {
    return true;
  } 

  return false;

}

void CSCRenderPlugin::preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo &r ) {

  if(o.object == NULL) {
    return;
  }

  TH1* obj = dynamic_cast<TH1*>( o.object );

  if(obj == NULL) {
    return;
  }

  c->cd();

  gStyle->SetPalette(1,0);

  // ============== Start generated from emuDQMBooking.xml by emuBooking2RenderPlugin.xsl ==================
  
  if(REMATCH(".*/DMB_Unpacked$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_wo_ALCT$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_wo_CLCT$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_wo_CFEB$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_Reporting$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_input_fifo_full$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_input_timeout$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_Unpacked_with_errors$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_Unpacked_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_wo_ALCT_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_wo_CLCT_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_wo_CFEB_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_Format_Errors$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_Format_Errors_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
    if(obj->GetMinimum() == obj->GetMaximum()) {
      obj->SetMaximum(obj->GetMinimum() + 0.01);
    }
    gPad->SetLogz();
  
    return;
  }

  if(REMATCH(".*/DMB_Format_Warnings$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_Format_Warnings_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
    if(obj->GetMinimum() == obj->GetMaximum()) {
      obj->SetMaximum(obj->GetMinimum() + 0.01);
    }
    gPad->SetLogz();
  
    return;
  }

  if(REMATCH(".*/DMB_input_fifo_full_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/DMB_input_timeout_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_Unpacked$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_DMB_input_fifo_full$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_DMB_input_timeout$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_Reporting$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    return;
  }

  if(REMATCH(".*/CSC_wo_ALCT$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_wo_CLCT$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_wo_CFEB$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_Unpacked_with_errors$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_Unpacked_with_warnings$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_Format_Errors$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_Format_Errors_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
    if(obj->GetMinimum() == obj->GetMaximum()) {
      obj->SetMaximum(obj->GetMinimum() + 0.01);
    }
    gPad->SetLogz();
  
    return;
  }

  if(REMATCH(".*/CSC_DMB_input_fifo_full_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_DMB_input_timeout_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_wo_ALCT_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_wo_CLCT_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_wo_CFEB_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_Unpacked_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_Format_Warnings$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/CSC_Format_Warnings_Fract$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
    if(obj->GetMinimum() == obj->GetMaximum()) {
      obj->SetMaximum(obj->GetMinimum() + 0.01);
    }
    gPad->SetLogz();
  
    return;
  }

  if(REMATCH(".*/All_Readout_Errors$", o.name)) {
    gPad->SetLeftMargin(0.3);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/All_DDUs_in_Readout$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/All_DDUs_L1A_Increment$", o.name)) {
    obj->SetStats(false);
  
    return;
  }

  if(REMATCH(".*/All_DDUs_Trailer_Errors$", o.name)) {
    gPad->SetLeftMargin(0.3);
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/All_DDUs_Format_Errors$", o.name)) {
    gPad->SetLeftMargin(0.3);
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/All_DDUs_Event_Size$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/All_DDUs_Average_Event_Size$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/All_DDUs_Live_Inputs$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/All_DDUs_Average_Live_Inputs$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/All_DDUs_Inputs_with_Data$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/All_DDUs_Average_Inputs_with_Data$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/All_DDUs_Inputs_Errors$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
    return;
  }

  if(REMATCH(".*/All_DDUs_Inputs_Warnings$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
    gPad->SetGridx();
    gPad->SetGridy();
  
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
    gPad->SetLeftMargin(0.6);
    gStyle->SetOptStat("e");
  
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
    gPad->SetLeftMargin(0.6);
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/Word_Count$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/ALCT0_BXN$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/ALCT0_KeyWG$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/ALCT0_Pattern$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT0_Quality$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT0_Quality_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT0_dTime$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/ALCT0_dTime_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT0_dTime_vs_KeyWG$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/ALCT1_BXN$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/ALCT1_KeyWG$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/ALCT1_Pattern$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT1_Quality$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT1_Quality_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT1_dTime$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/ALCT1_dTime_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT1_dTime_vs_KeyWG$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/ALCT1_vs_ALCT0_KeyWG$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly1_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly2$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly2_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly3$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly3_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly4$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly4_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly5$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly5_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly6$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/ALCTTime_Ly6_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_BXN$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/ALCT_BXN_vs_DMB_BXN$", o.name)) {
    obj->SetStats(false);
  
    return;
  }

  if(REMATCH(".*/ALCT_DMB_BXN_diff$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/ALCT_DMB_L1A_diff$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/ALCT_L1A$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly1_Efficiency$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly1_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly2_Efficiency$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly2_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly3_Efficiency$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly3_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly4_Efficiency$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly4_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly5_Efficiency$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly5_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly6_Efficiency$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Ly6_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Match_Time$", o.name)) {
    gStyle->SetOptStat("emr");
  
    return;
  }

  if(REMATCH(".*/ALCT_Number_Efficiency$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/ALCT_Number_Of_Layers_With_Hits$", o.name)) {
    gStyle->SetOptStat("emo");
  
    return;
  }

  if(REMATCH(".*/ALCT_Number_Of_WireGroups_With_Hits$", o.name)) {
    gStyle->SetOptStat("emo");
    if(obj->GetMinimum() == obj->GetMaximum()) {
      obj->SetMaximum(obj->GetMinimum() + 0.01);
    }
    gPad->SetLogy();
  
    return;
  }

  if(REMATCH(".*/ALCT_Number_Rate$", o.name)) {
  
    return;
  }

  if(REMATCH(".*/ALCT_Word_Count$", o.name)) {
    gStyle->SetOptStat("emo");
  
    return;
  }

  if(REMATCH(".*/BinCheck_ErrorStat_Frequency$", o.name)) {
    gPad->SetLeftMargin(0);
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/BinCheck_ErrorStat_Table$", o.name)) {
    gPad->SetLeftMargin(0.4);
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/BinCheck_WarningStat_Frequency$", o.name)) {
    gPad->SetLeftMargin(0);
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/BinCheck_WarningStat_Table$", o.name)) {
    gPad->SetLeftMargin(0.4);
    gPad->SetRightMargin(0);
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB0_Free_SCA_Cells$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB0_L1A_Sync_Time$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CFEB0_L1A_Sync_Time_DMB_diff$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/CFEB0_L1A_Sync_Time_vs_DMB$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB0_LCT_PHASE_vs_L1A_PHASE$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB0_SCA_Block_Occupancy$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CFEB0_SCA_Blocks_Locked_by_LCTs$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB0_SCA_Blocks_Locked_by_LCTxL1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB1_Free_SCA_Cells$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB1_L1A_Sync_Time$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CFEB1_L1A_Sync_Time_DMB_diff$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/CFEB1_L1A_Sync_Time_vs_DMB$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB1_LCT_PHASE_vs_L1A_PHASE$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB1_SCA_Block_Occupancy$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CFEB1_SCA_Blocks_Locked_by_LCTs$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB1_SCA_Blocks_Locked_by_LCTxL1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB2_Free_SCA_Cells$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB2_L1A_Sync_Time$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CFEB2_L1A_Sync_Time_DMB_diff$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/CFEB2_L1A_Sync_Time_vs_DMB$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB2_LCT_PHASE_vs_L1A_PHASE$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB2_SCA_Block_Occupancy$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CFEB2_SCA_Blocks_Locked_by_LCTs$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB2_SCA_Blocks_Locked_by_LCTxL1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB3_Free_SCA_Cells$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB3_L1A_Sync_Time$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CFEB3_L1A_Sync_Time_DMB_diff$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/CFEB3_L1A_Sync_Time_vs_DMB$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB3_LCT_PHASE_vs_L1A_PHASE$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB3_SCA_Block_Occupancy$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CFEB3_SCA_Blocks_Locked_by_LCTs$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB3_SCA_Blocks_Locked_by_LCTxL1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB4_Free_SCA_Cells$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB4_L1A_Sync_Time$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CFEB4_L1A_Sync_Time_DMB_diff$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/CFEB4_L1A_Sync_Time_vs_DMB$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB4_LCT_PHASE_vs_L1A_PHASE$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB4_SCA_Block_Occupancy$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CFEB4_SCA_Blocks_Locked_by_LCTs$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB4_SCA_Blocks_Locked_by_LCTxL1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_ActiveStrips_Ly1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_ActiveStrips_Ly2$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_ActiveStrips_Ly3$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_ActiveStrips_Ly4$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_ActiveStrips_Ly5$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_ActiveStrips_Ly6$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly1_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly2$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly2_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly3$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly3_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly4$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly4_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly5$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly5_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly6$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Active_Samples_vs_Strip_Ly6_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Cluster_Duration_Ly_1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Cluster_Duration_Ly_2$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Cluster_Duration_Ly_3$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Cluster_Duration_Ly_4$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Cluster_Duration_Ly_5$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Cluster_Duration_Ly_6$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Clusters_Charge_Ly_1$", o.name)) {
    gStyle->SetOptStat("eom");
  
    return;
  }

  if(REMATCH(".*/CFEB_Clusters_Charge_Ly_2$", o.name)) {
    gStyle->SetOptStat("eom");
  
    return;
  }

  if(REMATCH(".*/CFEB_Clusters_Charge_Ly_3$", o.name)) {
    gStyle->SetOptStat("eom");
  
    return;
  }

  if(REMATCH(".*/CFEB_Clusters_Charge_Ly_4$", o.name)) {
    gStyle->SetOptStat("eom");
  
    return;
  }

  if(REMATCH(".*/CFEB_Clusters_Charge_Ly_5$", o.name)) {
    gStyle->SetOptStat("eom");
  
    return;
  }

  if(REMATCH(".*/CFEB_Clusters_Charge_Ly_6$", o.name)) {
    gStyle->SetOptStat("eom");
  
    return;
  }

  if(REMATCH(".*/CFEB_Number_of_Clusters_Ly_1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Number_of_Clusters_Ly_2$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Number_of_Clusters_Ly_3$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Number_of_Clusters_Ly_4$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Number_of_Clusters_Ly_5$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Number_of_Clusters_Ly_6$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Out_Off_Range_Strips_Ly1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Out_Off_Range_Strips_Ly2$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Out_Off_Range_Strips_Ly3$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Out_Off_Range_Strips_Ly4$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Out_Off_Range_Strips_Ly5$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Out_Off_Range_Strips_Ly6$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withEMV_Sample_01_Ly1$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withEMV_Sample_01_Ly2$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withEMV_Sample_01_Ly3$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withEMV_Sample_01_Ly4$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withEMV_Sample_01_Ly5$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withEMV_Sample_01_Ly6$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withRMS_Sample_01_Ly1$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withRMS_Sample_01_Ly2$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withRMS_Sample_01_Ly3$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withRMS_Sample_01_Ly4$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withRMS_Sample_01_Ly5$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Pedestal_withRMS_Sample_01_Ly6$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_PedestalRMS_Sample_01_Ly1$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_PedestalRMS_Sample_01_Ly2$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_PedestalRMS_Sample_01_Ly3$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_PedestalRMS_Sample_01_Ly4$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_PedestalRMS_Sample_01_Ly5$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_PedestalRMS_Sample_01_Ly6$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_SCA_Cell_Peak_Ly_1$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_SCA_Cell_Peak_Ly_2$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_SCA_Cell_Peak_Ly_3$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_SCA_Cell_Peak_Ly_4$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_SCA_Cell_Peak_Ly_5$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_SCA_Cell_Peak_Ly_6$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CFEB_Width_of_Clusters_Ly_1$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Width_of_Clusters_Ly_2$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Width_of_Clusters_Ly_3$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Width_of_Clusters_Ly_4$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Width_of_Clusters_Ly_5$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CFEB_Width_of_Clusters_Ly_6$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CLCT0_BXN$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CLCT0_CLCT1_Clssification$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT0_Clssification$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT0_DiStrip_Pattern$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT0_DiStrip_Quality$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT0_DiStrip_Quality_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT0_Half_Strip_Pattern$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT0_Half_Strip_Quality$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT0_Half_Strip_Quality_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT0_KeyDiStrip$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CLCT0_KeyHalfStrip$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CLCT0_dTime$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/CLCT0_dTime_vs_DiStrip$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT0_dTime_vs_Half_Strip$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT0_KeyDiStrip_vs_ALCT0_KeyWiregroup$", o.name)) {
    gStyle->SetOptStat("e");
    if(obj->GetMinimum() == obj->GetMaximum()) {
      obj->SetMaximum(obj->GetMinimum() + 0.01);
    }
    gPad->SetLogx();
  
    return;
  }

  if(REMATCH(".*/CLCT1_BXN$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CLCT1_DiStrip_Pattern$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT1_DiStrip_Quality$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT1_DiStrip_Quality_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT1_Half_Strip_Pattern$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT1_Half_Strip_Quality$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT1_Half_Strip_Quality_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT1_KeyDiStrip$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CLCT1_KeyHalfStrip$", o.name)) {
    gStyle->SetOptStat("em");
  
    return;
  }

  if(REMATCH(".*/CLCT1_dTime$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/CLCT1_dTime_vs_DiStrip$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT1_dTime_vs_Half_Strip$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT1_vs_CLCT0_Key_Strip$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly1$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly1_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly2$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly2_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly3$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly3_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly4$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly4_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly5$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly5_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly6$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCTTime_Ly6_Profile$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT_BXN$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CLCT_BXN_vs_DMB_BXN$", o.name)) {
    obj->SetStats(false);
  
    return;
  }

  if(REMATCH(".*/CLCT_DMB_BXN_diff$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/CLCT_DMB_L1A_diff$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/CLCT_L1A$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly1_Efficiency$", o.name)) {
    gStyle->SetOptStat("eou");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly1_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly2_Efficiency$", o.name)) {
    gStyle->SetOptStat("eou");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly2_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly3_Efficiency$", o.name)) {
    gStyle->SetOptStat("eou");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly3_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly4_Efficiency$", o.name)) {
    gStyle->SetOptStat("eou");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly4_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly5_Efficiency$", o.name)) {
    gStyle->SetOptStat("eou");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly5_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly6_Efficiency$", o.name)) {
    gStyle->SetOptStat("eou");
  
    return;
  }

  if(REMATCH(".*/CLCT_Ly6_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT_Number$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CLCT_Number_Of_HalfStrips_With_Hits$", o.name)) {
    gStyle->SetOptStat("emo");
    if(obj->GetMinimum() == obj->GetMaximum()) {
      obj->SetMaximum(obj->GetMinimum() + 0.01);
    }
    gPad->SetLogy();
  
    return;
  }

  if(REMATCH(".*/CLCT_Number_Of_Layers_With_Hits$", o.name)) {
    gStyle->SetOptStat("emo");
  
    return;
  }

  if(REMATCH(".*/CLCT_Number_Rate$", o.name)) {
  
    return;
  }

  if(REMATCH(".*/CSC_Efficiency$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/CSC_Rate$", o.name)) {
  
    return;
  }

  if(REMATCH(".*/DMB_BXN_Distrib$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/DMB_BXN_vs_DDU_BXN$", o.name)) {
    obj->SetStats(false);
  
    return;
  }

  if(REMATCH(".*/DMB_CFEB_Active$", o.name)) {
    obj->SetStats(false);
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_CFEB_Active_vs_DAV$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_CFEB_DAV$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_CFEB_DAV_multiplicity$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_CFEB_MOVLP$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_CFEB_Sync$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/DMB_DDU_BXN_diff$", o.name)) {
    gStyle->SetOptStat("emou");
  
    return;
  }

  if(REMATCH(".*/DMB_DDU_L1A_diff$", o.name)) {
    gStyle->SetOptStat("emou");
  
    return;
  }

  if(REMATCH(".*/DMB_FEB_DAV_Efficiency$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_FEB_DAV_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_FEB_Combinations_DAV_Efficiency$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_FEB_Combinations_DAV_Rate$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_FEB_Unpacked_vs_DAV$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_FEB_Combinations_Unpacked_vs_DAV$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_FEB_Timeouts$", o.name)) {
    gPad->SetLeftMargin(0.2);
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_FIFO_stats$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/DMB_L1A_Distrib$", o.name)) {
    gStyle->SetOptStat("eo");
  
    return;
  }

  if(REMATCH(".*/DMB_L1A_vs_ALCT_L1A$", o.name)) {
    obj->SetStats(false);
  
    return;
  }

  if(REMATCH(".*/DMB_L1A_vs_CLCT_L1A$", o.name)) {
    obj->SetStats(false);
  
    return;
  }

  if(REMATCH(".*/DMB_L1A_vs_DDU_L1A$", o.name)) {
    obj->SetStats(false);
  
    return;
  }

  if(REMATCH(".*/DMB_L1_Pipe$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/LCT0_Match_BXN_Difference$", o.name)) {
    gStyle->SetOptStat("emr");
  
    return;
  }

  if(REMATCH(".*/LCT1_Match_BXN_Difference$", o.name)) {
    gStyle->SetOptStat("emr");
  
    return;
  }

  if(REMATCH(".*/LCT_Match_Status$", o.name)) {
    gStyle->SetOptStat("e");
  
    return;
  }

  if(REMATCH(".*/TMB_ALCT_BXN_diff$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/TMB_ALCT_L1A_diff$", o.name)) {
    gStyle->SetOptStat("emuo");
  
    return;
  }

  if(REMATCH(".*/TMB_BXN_vs_ALCT_BXN$", o.name)) {
    obj->SetStats(false);
  
    return;
  }

  if(REMATCH(".*/TMB_L1A_vs_ALCT_L1A$", o.name)) {
    obj->SetStats(false);
  
    return;
  }

  if(REMATCH(".*/TMB_Word_Count$", o.name)) {
    gStyle->SetOptStat("emo");
  
    return;
  }

   
  // ============== End generated from emuDQMBooking.xml by emuBooking2RenderPlugin.xsl ==================

  return;

}

void CSCRenderPlugin::postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

  if(o.object == NULL) {
    return;
  }

  TH1* obj = dynamic_cast<TH1*>( o.object );

  if(obj == NULL) {
    return;
  }

  c->cd();

  if(REMATCH(".*EventInfo/reportSummaryMap$", o.name)) {
    TH2* tmp = dynamic_cast<TH2*>(obj);
    summaryMap.drawDetector(tmp); 
    return;
  }

  if(REMATCH(".*Summary/Summary_ME[1234]$", o.name)) {
    std::string station_str = o.name;
    REREPLACE(".*Summary/Summary_ME([1234])$", station_str, "$1");
    TH2* obj2 = dynamic_cast<TH2*>(obj);
    summaryMap.drawStation(obj2, atoi(station_str.c_str())); 
    return;
  }

  if(REMATCH(".*Summary/CSC_[a-zA-Z0-9_-]+$", o.name)) {
    TH2* obj2 = dynamic_cast<TH2*>(obj);
    chamberMap.draw(obj2); 
    return;
  }

  return;

}

