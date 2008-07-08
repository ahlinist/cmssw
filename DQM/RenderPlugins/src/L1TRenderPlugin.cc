/*!
  \file L1TRenderPlugin.cc
  \\
  \\ Code shamelessly borrowed from J. Temple's HcalRenderPlugin.cc code, 
  \\ which was shamelessly borrowed from S. Dutta's SiStripRenderPlugin.cc 
  \\ code, G. Della Ricca and B. Gobbo's EBRenderPlugin.cc, and other existing
  \\ subdetector plugins
  \\ preDraw and postDraw methods now check whether histogram was a TH1
  \\ or TH2, and call a private method appropriate for the histogram type
*/

#include "DQM/RenderPlugins/src/L1TRenderPlugin.h" 

#include "TProfile2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include "TPaveText.h"

#include <cassert>
#include "DQM/RenderPlugins/src/utils.h"

void
L1TRenderPlugin::initialise (int argc, char ** argv)
{
  // same as RenderPlugin default for now (no special action taken)


  //summaryText = new TH2C( "summaryText", "summaryText", 5, 1, 6, 4, 1, 5);

  return;
}



bool L1TRenderPlugin::applies(const DQMNet::CoreObject &o,
				  const VisDQMImgInfo &i)
{
  // determine whether core object is an L1T object
#ifdef DEBUG 
  std::cout << "L1TRenderPlugin:applies " << o.name << std::endl; 
#endif 
  if (o.name.find( "L1T/" ) ==0 )
    {
      return true;
    }
  
  return false;
}



void L1TRenderPlugin::preDraw (TCanvas * c,
			      const DQMNet::CoreObject &o,
			      const VisDQMImgInfo &i,
			      VisDQMRenderInfo &r)
{

#ifdef DEBUG 
  std::cout << "L1TRenderPlugin:preDraw " << o.name << std::endl; 
#endif 
  c->cd(); 
 
  // object is TH2 histogram
  if( dynamic_cast<TH2F*>( o.object ) )
    { 
      preDrawTH2F( c, o ); 
    } 

  // object is TH1 histogram
  else if( dynamic_cast<TH1F*>( o.object ) ) 
    { 
      preDrawTH1F( c, o ); 
    } 

  else
#ifdef DEBUG 
    std::cout << "L1TRenderPlugin:preDraw  -- Cannot identify object " << o.name << std::endl; 
#endif 

  return;
} // L1TRenderPlugin::preDraw(...)


void L1TRenderPlugin::postDraw (TCanvas * c,
			       const DQMNet::CoreObject & o,
			       const VisDQMImgInfo & i)
{

#ifdef DEBUG 
  std::cout << "L1TRenderPlugin:postDraw " << o.name << std::endl; 
#endif 

  // object is TH2 histogram
  if( dynamic_cast<TH2F*>( o.object ) )
    { 
      postDrawTH2F( c, o ); 
    } 

  // object is TH1 histogram
  else if( dynamic_cast<TH1F*>( o.object ) ) 
    { 
      postDrawTH1F( c, o ); 
    } 

  else
#ifdef DEBUG 
    std::cout << "L1TRenderPlugin:postDraw  -- Cannot identify object " << o.name << std::endl; 
#endif 

  return;
} // L1TRenderPlugin::postDraw(...)



////////////////////////////////////////////////////////////////////////

//  private functions


void L1TRenderPlugin::preDrawTH1F ( TCanvas *c, const DQMNet::CoreObject &o )
{
  // Do we want to do anything special yet with TH1F histograms?

  TH1F* obj = dynamic_cast<TH1F*>( o.object ); 
  assert (obj); // checks that object indeed exists

  // Code used in SiStripRenderPlugin -- do we want similar defaults?

  /*
    gStyle->SetOptStat(0111); 
    if ( obj->GetMaximum(1.e5) > 0. ) { 
    gPad->SetLogy(1); 
    } else { 
    gPad->SetLogy(0); 
    } 
  */

  return;

} // preDrawTH1(...)




void L1TRenderPlugin::preDrawTH2F ( TCanvas *c, const DQMNet::CoreObject &o )
{
  

  TH2F* obj = dynamic_cast<TH2F*>( o.object ); 
  assert( obj ); 
  
  //put in preDrawTH2F  
  if( o.name.find( "reportSummaryMap" )  != std::string::npos) {
    obj->SetStats( kFALSE );
    dqm::utils::reportSummaryMapPalette(obj);
    obj->SetOption("colz");
    obj->SetTitle("L1T Report Summary Map");

    obj->GetXaxis()->SetNdivisions(1,true);
    obj->GetYaxis()->SetNdivisions(8,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    
    gPad->SetGrid(1,1);




    //gStyle->SetPaintTextFormat("+g");
    
    //TPaveText tree(.01,.75,.15,1.00);
    //tree.SetFillColor(18);
    //tree.SetTextAlign(12);
    //TText *tnt = tree.AddText("Tree");
    //tnt.SetTextAlign(22);


    return;
  }

  gStyle->SetCanvasBorderMode( 0 ); 
  gStyle->SetPadBorderMode( 0 ); 
  gStyle->SetPadBorderSize( 0 ); 
  
  // I don't think we want to set stats to 0 for Hcal
  //gStyle->SetOptStat( 0 ); 
  //obj->SetStats( kFALSE ); 

  // Use same labeling format as SiStripRenderPlugin.cc
  TAxis* xa = obj->GetXaxis(); 
  TAxis* ya = obj->GetYaxis(); 
  
  xa->SetTitleOffset(0.7); 
  xa->SetTitleSize(0.05); 
  xa->SetLabelSize(0.04); 
  
  ya->SetTitleOffset(0.7); 
  ya->SetTitleSize(0.05); 
  ya->SetLabelSize(0.04); 

  // Now the important stuff -- set 2D hist drawing option to "colz"
  gStyle->SetPalette(1);
  obj->SetOption("colz");

  if(o.name.find("Occupancy_Summary") < o.name.size()) {
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
    //obj->SetMaximum(5.0);

    //int colorError1[5];
    //colorError1[0] = 416;// kGreen
    //colorError1[1] = 400;// kYellow
    //colorError1[2] = 800;// kOrange
    //colorError1[3] = 625;
    //colorError1[4] = 632;// kRed
    //gStyle->SetPalette(5, colorError1);
    gStyle->SetOptStat(0);
    return;
  }

  return;
} // preDrawTH2F(...)




void L1TRenderPlugin::postDrawTH1F( TCanvas *c, const DQMNet::CoreObject &o )
{

  // Add error/warning text to 1-D histograms.  Do we want this at this time?
  /*
  TText tt; 
  tt.SetTextSize(0.12); 

  if (o.flags == 0) return; 

  else 
    { 
      if (o.flags & DQMNet::DQM_FLAG_REPORT_ERROR) 
	{ 
	  tt.SetTextColor(2); // error color = RED 
	  tt.DrawTextNDC(0.5, 0.5, "Error"); 
	}  // DQM_FLAG_REPORT_ERROR

      else if (o.flags & DQMNet::DQM_FLAG_REPORT_WARNING) 
	{ 
	  tt.SetTextColor(5); 
	  tt.DrawTextNDC(0.5, 0.5, "Warning"); // warning color = YELLOW 
	} // DQM_FLAG_REPORT_WARNING

      else if (o.flags & DQMNet::DQM_FLAG_REPORT_OTHER) 
	{  
	  tt.SetTextColor(1); // other color = BLACK
	  tt.DrawTextNDC(0.5, 0.5, "Other ");       
	} // DQM_FLAG_REPORT_OTHER

      else 
	{ 
	  tt.SetTextColor(3); 
	  tt.DrawTextNDC(0.5, 0.5, "Ok "); 
	} //else
    } // else (  o.flags != 0  )
  */
  return;

} // postDrawTH1F(...)




void L1TRenderPlugin::postDrawTH2F( TCanvas *c, const DQMNet::CoreObject &o )
{
  // nothing to put here just yet
  // in the future, we can add text output based on error status,
  // or set bin range based on filled histograms, etc.  
  // Maybe add a big "OK" sign to histograms with no entries (i.e., no errors)?


  //int nSubsystems = 20;
  
  //TPaveText *pt[nSubsystems]; 

  //for(int i =0; i<nSubsystems; i++){
   
    // relative to pad dimensions
    //TText *text = pt->AddText("ECAL");
    
//     switch(i){
//     case 0 :   
//       pt[i]= new TPaveText(0.14, 0.20, 0.14, 0.20, "NDC"); 
//       pt[i]->AddText("ECAL");   
//       break;
//     case 1 :   
//       pt[i]= new TPaveText(0.30, 0.20, 0.30,0.20, "NDC");
//       pt[i]->AddText("HCAL");    
//       break;
//     case 2 :   
//       pt[i]= new TPaveText(0.47, 0.20, 0.47,0.20, "NDC");
//       pt[i]->AddText("RCT");    
//       break;
//     case 3 :   
//       pt[i]= new TPaveText(0.63, 0.20, 0.63,0.20, "NDC");
//       pt[i]->AddText("GCT");    
//       break;
//     case 4 :   
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG");    
//       break;
//     case 5 :   
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTF");      
//       break;
//     case 6 :   
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("CSCTPG"); 
//       break;
//     case 7 :   
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("CSCTF"); 
//       break;
//     case 8 :   
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_RPC");     break;
//     case 9 :
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_GMT");     break;
//     case 10 :  
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_GT");      break;
//     case 11 :  
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_RPCTG");   break;
//     case 12 :  
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_EMUL");    break;
//     case 13 : 
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_Timing");  break;
//     case 14 :  
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_Test1");   break;
//     case 15 :  
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_Test2");   break;
//     case 16 :  
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_Test3");   break;
//     case 17 :  
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_Test4");break;
//     case 18 :  
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_Test5");   break;
//     case 19 :  
//       pt[i]= new TPaveText(0.77, 0.20, 0.77,0.20, "NDC");
//       pt[i]->AddText("DTTPG"); 
//       sprintf(histo,"L1T_Test6");   break;
//     }  
    
//   TPaveText *pt[8];

//   pt[i]->SetFillColor(0); // text is black on white
//   pt[i]->SetTextSize(0.04); 
//   pt[i]->SetTextAlign(12);
//   pt[i]->SetFillStyle(4000);
  
//   pt[i]->Draw("same");
  //to draw your text object
  //  }
  return;

} // postDrawTH2F(...)
