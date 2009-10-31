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

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "CSCRenderPlugin_ChamberMap.h"
#include "CSCRenderPlugin_SummaryMap.h"

#include <math.h>
#include <string>
#include <iostream>
#include <bitset>
#include <TH1.h>
#include <TH2.h>
#include <TBox.h>
#include <TText.h>
#include <TPRegexp.h>
#include <TStyle.h>
#include <TCanvas.h>
#define REREPLACE(pat, str, rep) { TString s(str); TPRegexp(pat).Substitute(s, rep); str = s; }
typedef std::map<std::string, bool> MapOfPatternResults;

/**
 * @class CSCRenderPlugin
 * @brief Actual render plugin for CSCs
 */
class CSCRenderPlugin : public DQMRenderPlugin {
  ChamberMap chamberMap;
  SummaryMap summaryMap;
  MapOfPatternResults mopr;

public:
  virtual bool applies( const VisDQMObject &o, const VisDQMImgInfo & )
    {
      if ( o.name.find( "CSC/" ) != std::string::npos )
        return true;

      return false;
    }

  virtual void preDraw( TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo &, VisDQMRenderInfo & )
    {
      if(o.object == NULL)
        return;

      TH1* obj = dynamic_cast<TH1*>( o.object );
      if(obj == NULL)
        return;

      c->cd();

      gStyle->SetPalette(1,0);

      // ============== Start generated from emuDQMBooking.xml by emuBooking2RenderPlugin.xsl ==================
      if(reMatch(".*/DMB_Unpacked$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_wo_ALCT$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_wo_CLCT$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_wo_CFEB$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_Reporting$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_input_fifo_full$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_input_timeout$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_Unpacked_with_errors$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_Unpacked_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_wo_ALCT_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_wo_CLCT_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_wo_CFEB_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_Format_Errors$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_Format_Errors_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();
        if(obj->GetMinimum() == obj->GetMaximum())
        {
          obj->SetMaximum(obj->GetMinimum() + 0.01);
        }
        gPad->SetLogz();

        return;
      }

      if(reMatch(".*/DMB_Format_Warnings$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_Format_Warnings_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();
        if(obj->GetMinimum() == obj->GetMaximum())
        {
          obj->SetMaximum(obj->GetMinimum() + 0.01);
        }
        gPad->SetLogz();

        return;
      }

      if(reMatch(".*/DMB_input_fifo_full_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/DMB_input_timeout_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_Unpacked$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_DMB_input_fifo_full$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_DMB_input_timeout$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_Reporting$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        return;
      }

      if(reMatch(".*/CSC_wo_ALCT$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_wo_CLCT$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_wo_CFEB$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_Unpacked_with_errors$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_Unpacked_with_warnings$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_Format_Errors$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_Format_Errors_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();
        if(obj->GetMinimum() == obj->GetMaximum())
        {
          obj->SetMaximum(obj->GetMinimum() + 0.01);
        }
        gPad->SetLogz();

        return;
      }

      if(reMatch(".*/CSC_DMB_input_fifo_full_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_DMB_input_timeout_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_wo_ALCT_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_wo_CLCT_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_wo_CFEB_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_Unpacked_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_Format_Warnings$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/CSC_Format_Warnings_Fract$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();
        if(obj->GetMinimum() == obj->GetMaximum())
        {
          obj->SetMaximum(obj->GetMinimum() + 0.01);
        }
        gPad->SetLogz();

        return;
      }

      if(reMatch(".*/All_Readout_Errors$", o.name))
      {
        gPad->SetLeftMargin(0.3);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/All_DDUs_in_Readout$", o.name))
      {
        obj->SetStats(false);
        obj->SetFillColor(45);
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/All_DDUs_L1A_Increment$", o.name))
      {
        obj->SetStats(false);

        return;
      }

      if(reMatch(".*/All_DDUs_Trailer_Errors$", o.name))
      {
        gPad->SetLeftMargin(0.3);
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/All_DDUs_Format_Errors$", o.name))
      {
        gPad->SetLeftMargin(0.3);
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/All_DDUs_Event_Size$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/All_DDUs_Average_Event_Size$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/All_DDUs_Live_Inputs$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/All_DDUs_Average_Live_Inputs$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/All_DDUs_Inputs_with_Data$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/All_DDUs_Average_Inputs_with_Data$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/All_DDUs_Inputs_Errors$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/All_DDUs_Inputs_Warnings$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();

        return;
      }

      if(reMatch(".*/BXN$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/Buffer_Size$", o.name))
      {
        gStyle->SetOptStat("emro");

        return;
      }

      if(reMatch(".*/CSC_Errors$", o.name))
      {
        gStyle->SetOptStat("emro");

        return;
      }

      if(reMatch(".*/CSC_Errors_Rate$", o.name))
      {
        return;
      }

      if(reMatch(".*/CSC_Warnings$", o.name))
      {
        gStyle->SetOptStat("emro");

        return;
      }

      if(reMatch(".*/CSC_Warnings_Rate$", o.name))
      {
        return;
      }

      if(reMatch(".*/DMB_Active_Header_Count$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/DMB_Connected_Inputs$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_Connected_Inputs_Rate$", o.name))
      {
        return;
      }

      if(reMatch(".*/DMB_DAV_Header_Count_vs_DMB_Active_Header_Count$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_DAV_Header_Occupancy$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_DAV_Header_Occupancy_Rate$", o.name))
      {
        return;
      }

      if(reMatch(".*/DMB_unpacked_vs_DAV$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/L1A_Increment$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/Readout_Errors$", o.name))
      {
        gPad->SetLeftMargin(0.6);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/Trailer_ErrorStat_Frequency$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/Trailer_ErrorStat_Rate$", o.name))
      {
        return;
      }

      if(reMatch(".*/Trailer_ErrorStat_Table$", o.name))
      {
        gPad->SetLeftMargin(0.6);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/Word_Count$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/ALCT0_BXN$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/ALCT0_KeyWG$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/ALCT0_Pattern$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT0_Quality$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT0_Quality_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT0_dTime$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/ALCT0_dTime_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT0_dTime_vs_KeyWG$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/ALCT1_BXN$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/ALCT1_KeyWG$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/ALCT1_Pattern$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT1_Quality$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT1_Quality_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT1_dTime$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/ALCT1_dTime_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT1_dTime_vs_KeyWG$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/ALCT1_vs_ALCT0_KeyWG$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly1_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly2$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly2_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly3$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly3_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly4$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly4_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly5$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly5_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly6$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/ALCTTime_Ly6_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_BXN$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/ALCT_BXN_vs_DMB_BXN$", o.name))
      {
        obj->SetStats(false);

        return;
      }

      if(reMatch(".*/ALCT_DMB_BXN_diff$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/ALCT_DMB_L1A_diff$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/ALCT_L1A$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/ALCT_Ly1_Efficiency$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Ly1_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Ly2_Efficiency$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Ly2_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Ly3_Efficiency$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Ly3_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Ly4_Efficiency$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Ly4_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Ly5_Efficiency$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Ly5_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Ly6_Efficiency$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Ly6_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Match_Time$", o.name))
      {
        gStyle->SetOptStat("emr");

        return;
      }

      if(reMatch(".*/ALCT_Number_Efficiency$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/ALCT_Number_Of_Layers_With_Hits$", o.name))
      {
        gStyle->SetOptStat("emo");

        return;
      }

      if(reMatch(".*/ALCT_Number_Of_WireGroups_With_Hits$", o.name))
      {
        gStyle->SetOptStat("emo");
        if(obj->GetMinimum() == obj->GetMaximum())
        {
          obj->SetMaximum(obj->GetMinimum() + 0.01);
        }
        gPad->SetLogy();

        return;
      }

      if(reMatch(".*/ALCT_Number_Rate$", o.name))
      {
        return;
      }

      if(reMatch(".*/ALCT_Word_Count$", o.name))
      {
        gStyle->SetOptStat("emo");

        return;
      }

      if(reMatch(".*/BinCheck_ErrorStat_Frequency$", o.name))
      {
        gPad->SetLeftMargin(0);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/BinCheck_ErrorStat_Table$", o.name))
      {
        gPad->SetLeftMargin(0.4);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/BinCheck_WarningStat_Frequency$", o.name))
      {
        gPad->SetLeftMargin(0);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/BinCheck_WarningStat_Table$", o.name))
      {
        gPad->SetLeftMargin(0.4);
        gPad->SetRightMargin(0);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB0_Free_SCA_Cells$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB0_L1A_Sync_Time$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CFEB0_L1A_Sync_Time_DMB_diff$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/CFEB0_L1A_Sync_Time_vs_DMB$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB0_LCT_PHASE_vs_L1A_PHASE$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB0_SCA_Block_Occupancy$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CFEB0_SCA_Blocks_Locked_by_LCTs$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB0_SCA_Blocks_Locked_by_LCTxL1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB1_Free_SCA_Cells$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB1_L1A_Sync_Time$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CFEB1_L1A_Sync_Time_DMB_diff$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/CFEB1_L1A_Sync_Time_vs_DMB$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB1_LCT_PHASE_vs_L1A_PHASE$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB1_SCA_Block_Occupancy$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CFEB1_SCA_Blocks_Locked_by_LCTs$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB1_SCA_Blocks_Locked_by_LCTxL1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB2_Free_SCA_Cells$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB2_L1A_Sync_Time$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CFEB2_L1A_Sync_Time_DMB_diff$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/CFEB2_L1A_Sync_Time_vs_DMB$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB2_LCT_PHASE_vs_L1A_PHASE$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB2_SCA_Block_Occupancy$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CFEB2_SCA_Blocks_Locked_by_LCTs$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB2_SCA_Blocks_Locked_by_LCTxL1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB3_Free_SCA_Cells$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB3_L1A_Sync_Time$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CFEB3_L1A_Sync_Time_DMB_diff$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/CFEB3_L1A_Sync_Time_vs_DMB$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB3_LCT_PHASE_vs_L1A_PHASE$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB3_SCA_Block_Occupancy$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CFEB3_SCA_Blocks_Locked_by_LCTs$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB3_SCA_Blocks_Locked_by_LCTxL1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB4_Free_SCA_Cells$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB4_L1A_Sync_Time$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CFEB4_L1A_Sync_Time_DMB_diff$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/CFEB4_L1A_Sync_Time_vs_DMB$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB4_LCT_PHASE_vs_L1A_PHASE$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB4_SCA_Block_Occupancy$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CFEB4_SCA_Blocks_Locked_by_LCTs$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB4_SCA_Blocks_Locked_by_LCTxL1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_ActiveStrips_Ly1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_ActiveStrips_Ly2$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_ActiveStrips_Ly3$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_ActiveStrips_Ly4$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_ActiveStrips_Ly5$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_ActiveStrips_Ly6$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly1_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly2$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly2_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly3$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly3_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly4$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly4_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly5$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly5_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly6$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Active_Samples_vs_Strip_Ly6_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Cluster_Duration_Ly_1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Cluster_Duration_Ly_2$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Cluster_Duration_Ly_3$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Cluster_Duration_Ly_4$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Cluster_Duration_Ly_5$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Cluster_Duration_Ly_6$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Clusters_Charge_Ly_1$", o.name))
      {
        gStyle->SetOptStat("eom");

        return;
      }

      if(reMatch(".*/CFEB_Clusters_Charge_Ly_2$", o.name))
      {
        gStyle->SetOptStat("eom");

        return;
      }

      if(reMatch(".*/CFEB_Clusters_Charge_Ly_3$", o.name))
      {
        gStyle->SetOptStat("eom");

        return;
      }

      if(reMatch(".*/CFEB_Clusters_Charge_Ly_4$", o.name))
      {
        gStyle->SetOptStat("eom");

        return;
      }

      if(reMatch(".*/CFEB_Clusters_Charge_Ly_5$", o.name))
      {
        gStyle->SetOptStat("eom");

        return;
      }

      if(reMatch(".*/CFEB_Clusters_Charge_Ly_6$", o.name))
      {
        gStyle->SetOptStat("eom");

        return;
      }

      if(reMatch(".*/CFEB_Number_of_Clusters_Ly_1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Number_of_Clusters_Ly_2$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Number_of_Clusters_Ly_3$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Number_of_Clusters_Ly_4$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Number_of_Clusters_Ly_5$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Number_of_Clusters_Ly_6$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Out_Off_Range_Strips_Ly1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Out_Off_Range_Strips_Ly2$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Out_Off_Range_Strips_Ly3$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Out_Off_Range_Strips_Ly4$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Out_Off_Range_Strips_Ly5$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Out_Off_Range_Strips_Ly6$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withEMV_Sample_01_Ly1$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withEMV_Sample_01_Ly2$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withEMV_Sample_01_Ly3$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withEMV_Sample_01_Ly4$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withEMV_Sample_01_Ly5$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withEMV_Sample_01_Ly6$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withRMS_Sample_01_Ly1$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withRMS_Sample_01_Ly2$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withRMS_Sample_01_Ly3$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withRMS_Sample_01_Ly4$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withRMS_Sample_01_Ly5$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Pedestal_withRMS_Sample_01_Ly6$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_PedestalRMS_Sample_01_Ly1$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_PedestalRMS_Sample_01_Ly2$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_PedestalRMS_Sample_01_Ly3$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_PedestalRMS_Sample_01_Ly4$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_PedestalRMS_Sample_01_Ly5$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_PedestalRMS_Sample_01_Ly6$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_SCA_Cell_Peak_Ly_1$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_SCA_Cell_Peak_Ly_2$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_SCA_Cell_Peak_Ly_3$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_SCA_Cell_Peak_Ly_4$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_SCA_Cell_Peak_Ly_5$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_SCA_Cell_Peak_Ly_6$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CFEB_Width_of_Clusters_Ly_1$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Width_of_Clusters_Ly_2$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Width_of_Clusters_Ly_3$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Width_of_Clusters_Ly_4$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Width_of_Clusters_Ly_5$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CFEB_Width_of_Clusters_Ly_6$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CLCT0_BXN$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CLCT0_CLCT1_Clssification$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT0_Clssification$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT0_DiStrip_Pattern$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT0_DiStrip_Quality$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT0_DiStrip_Quality_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT0_Half_Strip_Pattern$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT0_Half_Strip_Quality$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT0_Half_Strip_Quality_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT0_KeyDiStrip$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CLCT0_KeyHalfStrip$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CLCT0_dTime$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/CLCT0_dTime_vs_DiStrip$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT0_dTime_vs_Half_Strip$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT0_KeyDiStrip_vs_ALCT0_KeyWiregroup$", o.name))
      {
        gStyle->SetOptStat("e");
        if(obj->GetMinimum() == obj->GetMaximum())
        {
          obj->SetMaximum(obj->GetMinimum() + 0.01);
        }
        gPad->SetLogx();

        return;
      }

      if(reMatch(".*/CLCT1_BXN$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CLCT1_DiStrip_Pattern$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT1_DiStrip_Quality$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT1_DiStrip_Quality_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT1_Half_Strip_Pattern$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT1_Half_Strip_Quality$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT1_Half_Strip_Quality_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT1_KeyDiStrip$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CLCT1_KeyHalfStrip$", o.name))
      {
        gStyle->SetOptStat("em");

        return;
      }

      if(reMatch(".*/CLCT1_dTime$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/CLCT1_dTime_vs_DiStrip$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT1_dTime_vs_Half_Strip$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT1_vs_CLCT0_Key_Strip$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly1$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly1_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly2$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly2_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly3$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly3_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly4$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly4_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly5$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly5_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly6$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCTTime_Ly6_Profile$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT_BXN$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CLCT_BXN_vs_DMB_BXN$", o.name))
      {
        obj->SetStats(false);

        return;
      }

      if(reMatch(".*/CLCT_DMB_BXN_diff$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/CLCT_DMB_L1A_diff$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/CLCT_L1A$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/CLCT_Ly1_Efficiency$", o.name))
      {
        gStyle->SetOptStat("eou");

        return;
      }

      if(reMatch(".*/CLCT_Ly1_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT_Ly2_Efficiency$", o.name))
      {
        gStyle->SetOptStat("eou");

        return;
      }

      if(reMatch(".*/CLCT_Ly2_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT_Ly3_Efficiency$", o.name))
      {
        gStyle->SetOptStat("eou");

        return;
      }

      if(reMatch(".*/CLCT_Ly3_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT_Ly4_Efficiency$", o.name))
      {
        gStyle->SetOptStat("eou");

        return;
      }

      if(reMatch(".*/CLCT_Ly4_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT_Ly5_Efficiency$", o.name))
      {
        gStyle->SetOptStat("eou");

        return;
      }

      if(reMatch(".*/CLCT_Ly5_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT_Ly6_Efficiency$", o.name))
      {
        gStyle->SetOptStat("eou");

        return;
      }

      if(reMatch(".*/CLCT_Ly6_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT_Number$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CLCT_Number_Of_HalfStrips_With_Hits$", o.name))
      {
        gStyle->SetOptStat("emo");
        if(obj->GetMinimum() == obj->GetMaximum())
        {
          obj->SetMaximum(obj->GetMinimum() + 0.01);
        }
        gPad->SetLogy();

        return;
      }

      if(reMatch(".*/CLCT_Number_Of_Layers_With_Hits$", o.name))
      {
        gStyle->SetOptStat("emo");

        return;
      }

      if(reMatch(".*/CLCT_Number_Rate$", o.name))
      {
        return;
      }

      if(reMatch(".*/CSC_Efficiency$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/CSC_Rate$", o.name))
      {
        return;
      }

      if(reMatch(".*/DMB_BXN_Distrib$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/DMB_BXN_vs_DDU_BXN$", o.name))
      {
        obj->SetStats(false);

        return;
      }

      if(reMatch(".*/DMB_CFEB_Active$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_CFEB_Active_vs_DAV$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_CFEB_DAV$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_CFEB_DAV_multiplicity$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_CFEB_MOVLP$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_CFEB_Sync$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/DMB_DDU_BXN_diff$", o.name))
      {
        gStyle->SetOptStat("emou");

        return;
      }

      if(reMatch(".*/DMB_DDU_L1A_diff$", o.name))
      {
        gStyle->SetOptStat("emou");

        return;
      }

      if(reMatch(".*/DMB_FEB_DAV_Efficiency$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_FEB_DAV_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_FEB_Combinations_DAV_Efficiency$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_FEB_Combinations_DAV_Rate$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_FEB_Unpacked_vs_DAV$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_FEB_Combinations_Unpacked_vs_DAV$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_FEB_Timeouts$", o.name))
      {
        gPad->SetLeftMargin(0.2);
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_FIFO_stats$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/DMB_L1A_Distrib$", o.name))
      {
        gStyle->SetOptStat("eo");

        return;
      }

      if(reMatch(".*/DMB_L1A_vs_ALCT_L1A$", o.name))
      {
        obj->SetStats(false);

        return;
      }

      if(reMatch(".*/DMB_L1A_vs_CLCT_L1A$", o.name))
      {
        obj->SetStats(false);

        return;
      }

      if(reMatch(".*/DMB_L1A_vs_DDU_L1A$", o.name))
      {
        obj->SetStats(false);

        return;
      }

      if(reMatch(".*/DMB_L1_Pipe$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/LCT0_Match_BXN_Difference$", o.name))
      {
        gStyle->SetOptStat("emr");

        return;
      }

      if(reMatch(".*/LCT1_Match_BXN_Difference$", o.name))
      {
        gStyle->SetOptStat("emr");

        return;
      }

      if(reMatch(".*/LCT_Match_Status$", o.name))
      {
        gStyle->SetOptStat("e");

        return;
      }

      if(reMatch(".*/TMB_ALCT_BXN_diff$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/TMB_ALCT_L1A_diff$", o.name))
      {
        gStyle->SetOptStat("emuo");

        return;
      }

      if(reMatch(".*/TMB_BXN_vs_ALCT_BXN$", o.name))
      {
        obj->SetStats(false);

        return;
      }

      if(reMatch(".*/TMB_L1A_vs_ALCT_L1A$", o.name))
      {
        obj->SetStats(false);

        return;
      }

      if(reMatch(".*/TMB_Word_Count$", o.name))
      {
        gStyle->SetOptStat("emo");

        return;
      }

      // ============== End generated from emuDQMBooking.xml by emuBooking2RenderPlugin.xsl ==================

      if(reMatch(".*FEDIntegrity/FED(Entries|Fatal|NonFatal)$", o.name))
      {
        obj->SetStats(false);
        obj->SetFillColor(45);
        gStyle->SetOptStat("em");
        return;
      }

      if(reMatch(".*/CSC_L1A_out_of_sync$", o.name))
      {
        obj->SetStats(false);
        gStyle->SetOptStat("e");
        gPad->SetGridx();
        gPad->SetGridy();
        return;
      }
    }

  virtual void postDraw( TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo & ) {

      if(o.object == NULL)
        return;

      TH1* obj = dynamic_cast<TH1*>( o.object );
      if(obj == NULL)
        return;

      c->cd();

      if(reMatch(".*Summary/Physics_EMU$", o.name))
      {
        TH2* tmp = dynamic_cast<TH2*>(obj);
        summaryMap.drawDetector(tmp);
        return;
      }

      if(reMatch(".*Summary/Physics_ME[1234]$", o.name))
      {
        std::string station_str = o.name;
        REREPLACE(".*Summary/Physics_ME([1234])$", station_str, "$1");
        TH2* obj2 = dynamic_cast<TH2*>(obj);
        summaryMap.drawStation(obj2, atoi(station_str.c_str()));
        return;
      }

      if(reMatch(".*Summary/CSC_STATS_[a-zA-Z0-9_-]+$", o.name) ||
         reMatch(".*EventInfo/reportSummaryMap$", o.name))
      {
        TH2* obj2 = dynamic_cast<TH2*>(obj);
        chamberMap.drawStats(obj2);
        return;
      }

      if(reMatch(".*Summary/CSC_[a-zA-Z0-9_-]+$", o.name) ||
         reMatch(".*FEDIntegrity/CSC_[a-zA-Z0-9_-]+$", o.name))
      {
        TH2* obj2 = dynamic_cast<TH2*>(obj);
        chamberMap.draw(obj2);
        return;
      }
    }

private:
  
  bool reMatch(const std::string pat, const std::string str) {
      bool result = false;

      std::string key(pat);
      key.append(1, (char)9);
      key.append(str);

      MapOfPatternResults::iterator iter = mopr.find(key);

      if (iter == mopr.end())
      {
        result = TPRegexp(pat).MatchB(str);
        mopr[key] = result;
      }
      else
      {
        result = iter->second;
      }

      return result;
    }
};

static CSCRenderPlugin instance;
