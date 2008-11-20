/*!
  \file HcalRenderPlugin.cc
  \brief Display Plugin for Hcal DQM Histograms
  \author J. Temple
  \version $Revision: 1.4 $
  \date $Date: 2008/08/22 11:52:02 $
  \\
  \\ Code shamelessly borrowed from S. Dutta's SiStripRenderPlugin.cc code,
  \\ G. Della Ricca and B. Gobbo's EBRenderPlugin.cc, and other existing
  \\ subdetector plugins
  \\ preDraw and postDraw methods now check whether histogram was a TH1
  \\ or TH2, and call a private method appropriate for the histogram type
*/

#include "DQM/RenderPlugins/src/HcalRenderPlugin.h" 
#include "DQM/RenderPlugins/src/utils.h"


//---define parameters for HTR/Channel Plots
#define MARGIN  1
#define FEDS_X0 3
#define SPIG_Y0 4
#define CHNS_Y0 3
#define XS 2
#define YS 4
////#define _XBINS (24*ERIC_HDP_XS+ERIC_HDP_X0)
////#define _YBINS (15*ERIC_HDP_YS+ERIC_HDP_Y0)

void
HcalRenderPlugin::initialise (int argc, char ** argv)
{
  // Error fraction colors, and standard colors
  // (error fractions colors are the reverse of standard colors)
  //  should probably make error colors run from yellow to red, don't include green?

  float rgb[20][3];

  for( int i=0; i<20; ++i )
    {
      if (i<2)
	summaryColors[i]=17;
      else summaryColors[i]=0;

      if ( i < 17 )
        {
          rgb[i][0] = 0.80+0.01*i;
          rgb[i][1] = 0.00+0.03*i;
          rgb[i][2] = 0.00;
        }
      else if ( i < 19 )
        {
          rgb[i][0] = 0.80+0.01*i;
          rgb[i][1] = 0.00+0.03*i+0.15+0.10*(i-17);
	  rgb[i][2] = 0.00;
        }
      else if ( i == 19 )
        {
          rgb[i][0] = 0.00;
          rgb[i][1] = 0.80;
          rgb[i][2] = 0.00;
        }
      // flip colors from standard values (1=bad, 0=good)
      errorFracColors[19-i] = 901+i;
      standardColors[i]=901+i;
      summaryColors[20+i]=901+i;
      TColor* color = gROOT->GetColor( 901+i );
      if( ! color ) color = new TColor( 901+i, 0, 0, 0, "" );
      color->SetRGB( rgb[i][0], rgb[i][1], rgb[i][2] );
    }

  return;
}



bool HcalRenderPlugin::applies(const DQMNet::CoreObject &o,
				  const VisDQMImgInfo &i)
{
  // determine whether core object is an Hcal object
#ifdef DEBUG 
  std::cout << "HcalRenderPlugin:applies " << o.name << std::endl; 
#endif 

  if (o.name.find( "Hcal/" ) != std::string::npos)
    {
      return true;
    }
  
  return false;
}



void
HcalRenderPlugin::preDraw (TCanvas * c,
			      const DQMNet::CoreObject &o,
			      const VisDQMImgInfo &i,
			      VisDQMRenderInfo &r)
{

#ifdef DEBUG 
  std::cout << "HcalRenderPlugin:preDraw " << o.name << std::endl; 
#endif 
  c->cd(); 

  gStyle->Reset("Default");
  
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(10);
  gStyle->SetFillColor(10);
  gStyle->SetStatColor(10);
  gStyle->SetTitleFillColor(10);
  
  TGaxis::SetMaxDigits(4);
  
  gStyle->SetOptTitle(kTRUE);
  gStyle->SetTitleBorderSize(0);

  gStyle->SetOptStat(kFALSE);
  gStyle->SetStatBorderSize(1);


  gROOT->ForceStyle();

 
  // object is TH2 histogram
  if( dynamic_cast<TH2*>( o.object ) )
    { 
      preDrawTH2( c, o ); 
    } 

  // object is TH1 histogram
  else if( dynamic_cast<TH1*>( o.object ) ) 
    { 
      preDrawTH1( c, o ); 
    } 

  else
#ifdef DEBUG 
    std::cout << "HcalRenderPlugin:preDraw  -- Cannot identify object " << o.name << std::endl; 
#endif 

  return;
} // HcalRenderPlugin::preDraw(...)


void
HcalRenderPlugin::postDraw (TCanvas * c,
			       const DQMNet::CoreObject & o,
			       const VisDQMImgInfo & i)
{

#ifdef DEBUG 
  std::cout << "HcalRenderPlugin:postDraw " << o.name << std::endl; 
#endif 

  // object is TH2 histogram
  if( dynamic_cast<TH2*>( o.object ) )
    { 
      postDrawTH2( c, o ); 
    } 

  // object is TH1 histogram
  else if( dynamic_cast<TH1*>( o.object ) ) 
    { 
      postDrawTH1( c, o ); 
    } 

  else
#ifdef DEBUG 
    std::cout << "HcalRenderPlugin:postDraw  -- Cannot identify object " << o.name << std::endl; 
#endif 

  return;
} // HcalRenderPlugin::postDraw(...)



////////////////////////////////////////////////////////////////////////

//  private functions


void HcalRenderPlugin::preDrawTH1 ( TCanvas *c, const DQMNet::CoreObject &o )
{
  // Do we want to do anything special yet with TH1 histograms?

  TH1* obj = dynamic_cast<TH1*>( o.object ); 
  assert (obj); // checks that object indeed exists

  // o.name is a std::string object
  // Add in list of names of histograms for which we want log plotting here.
  if (  (o.name.find("DataFormatMonitor/DCC Plots/BCN from DCCs")!=std::string::npos)   ||
	(o.name.find("DataFormatMonitor/DCC Plots/DCC Ev Fragment")!=std::string::npos) ||
	(o.name.find("DataFormatMonitor/DCC Plots/Spigot Format")!=std::string::npos)   ||
	(o.name.find("DataFormatMonitor/HTR Plots/BCN from HTRs")!=std::string::npos)   ||
	(o.name.find("DataFormatMonitor/HTR Plots/EvN Difference")!=std::string::npos)     )
    gPad->SetLogy(1);

  if (  (o.name.find("DataFormatMonitor/HTR Plots/")!=std::string::npos) &&
	(o.name.find("Data Format Error Word")!=std::string::npos)          )
    gPad->SetLogy(1);
  
  if (  (o.name.find("Digi Shape - over thresh")!=std::string::npos)  )
    obj->SetMinimum(0);

  
  return;

} // preDrawTH1(...)




void HcalRenderPlugin::preDrawTH2 ( TCanvas *c, const DQMNet::CoreObject &o )
{
  
  TH2* obj = dynamic_cast<TH2*>( o.object ); 
  assert( obj ); 
  
  gStyle->SetCanvasBorderMode( 0 ); 
  gStyle->SetPadBorderMode( 0 ); 
  gStyle->SetPadBorderSize( 0 ); 
  
  // Do we want to set stats to 0 for Hcal?
  gStyle->SetOptStat( 0 ); 
  obj->SetStats( kFALSE ); 

   // Set default color scheme

  // Always use most up-to-date color scheme for reportSummaryMap,
  // so that it's consistent with other maps.
  
  // reportSummaryMapPalette currently only takes TH2Fs;
  // need to make separate cast

  // I don't think we want to do this yet, because reportSummaryMap doesn't
  // yet provide useful colors when value = -1?

  // green when =1, red when = 0, grey when = -1
  if (o.name.find("reportSummaryMap" ) != std::string::npos)
    {
      gStyle->SetPalette(40,summaryColors);
      obj->SetOption("col");
    }
  // green when = 0, red when =1 -- don't yet know how to get # of events in order to normalize

  else if ( (o.name.find("SubPedestal") != std::string::npos )
	    || (o.name.find("RawPedestal") != std::string::npos)
	    || (o.name.find("Occupancy Map") != std::string::npos )

	    )
    {
      gStyle->SetPalette(1);
      obj->SetOption("colz");
    }

  // green when high, red when low
  else if ( (o.name.find("_abovePed") != std::string::npos )
	    || (o.name.find("above_pedestal_Depth") != std::string::npos )
	    )
    {
      gStyle->SetPalette(20, standardColors);
      obj->SetOption("colz");
    }

  // Rainbows and Ponies!
  else if ((o.name.find("DataFormatMonitor/Readout Chain DataIntegrity Check") != std::string::npos )||
	   (o.name.find("HTR Plots/ Channel Data Integrity/FED 7") != std::string::npos )            ||
	   (o.name.find("HTR Plots/Channel Integrity Summarized by Spigot") != std::string::npos )   ||
	   (o.name.find("HTR Plots/Half-HTR DataIntegrity Check") != std::string::npos )               ) {
      gStyle->SetPalette(1);
      obj->SetOption("colz");
      c->SetBottomMargin(0.200);
      gPad->SetLogz();}
  else if ( (o.name.find("DataFormatMonitor/DCC Plots/DCC Error and Warning") != std::string::npos ) ||
	    (o.name.find("DataFormatMonitor/DCC Plots/All Evt") != std::string::npos )               ||
	    (o.name.find("DataFormatMonitor/DCC Plots/DCC Nonzero") != std::string::npos )           
	    // Certain plots dissappear when we include them in the OR. WTF?
	    //(o.name.find("DataFormatMonitor/HTR Plots/BCN Inconsistent") != std::string::npos )           ||
	    //(o.name.find("DataFormatMonitor/HTR Plots/EvN Inconsistent") != std::string::npos )           ||
	    //(o.name.find("DataFormatMonitor/HTR Plots/HTR Error Word") != std::string::npos )           ||
	    //(o.name.find("DataFormatMonitor/HTR Plots/Unpacking") != std::string::npos )                 
	    )  {
    gStyle->SetPalette(1);
    obj->SetOption("colz");
    gPad->SetLogz();
    gPad->SetGridx();
    gPad->SetGridy();}
  else   // red when high, green when low
    {
      gStyle->SetPalette(20, errorFracColors);
      obj->SetOption("colz");
    }

  if ( (o.name.find("DataFormatMonitor/DCC Plots/DCC Status") != std::string::npos ) ||
       (o.name.find("DeadCellMonitor/ ProblemDeadCells") != std::string::npos )         ){
    if (obj->GetEntries() == 0) {
      obj->SetStats(1111);
      TText t;
      t.DrawText(1,1,"No News is Good News."); }
    else {
      gPad->SetGridx();
      gPad->SetGridy(); }}
  
  // This time, just rainbows.
  if (o.name.find("DataFormatMonitor/") != std::string::npos ) {
    gStyle->SetPalette(1);
    obj->SetOption("colz");}



///	    (o.name.find("DataFormatMonitor/HTR Plots/BCN Inconsistent") != std::string::npos )      ||
///	    (o.name.find("DataFormatMonitor/HTR Plots/EvN Inconsistent") != std::string::npos )        

  return;
} // preDrawTH2(...)




void HcalRenderPlugin::postDrawTH1( TCanvas *c, const DQMNet::CoreObject &o )
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

} // postDrawTH1(...)




void HcalRenderPlugin::postDrawTH2( TCanvas *c, const DQMNet::CoreObject &o )
{
  
  TH2* obj = dynamic_cast<TH2*>( o.object ); 
  assert( obj ); 

  obj->GetYaxis()->SetTickLength(0.0);
  obj->GetXaxis()->SetTickLength(0.0);
      
  // in the future, we can add text output based on error status,
  // or set bin range based on filled histograms, etc.  
  if ( (o.name.find("DataFormatMonitor/HTR Plots/BCN Inconsistent") != std::string::npos)     ||
       (o.name.find("DataFormatMonitor/HTR Plots/EvN Inconsistent") != std::string::npos)     ||
       (o.name.find("DataFormatMonitor/HTR Plots/HTR Error Word") != std::string::npos  )     ||
       (o.name.find("DataFormatMonitor/HTR Plots/Unpacking ") != std::string::npos)          ||
       (o.name.find("DataFormatMonitor/DCC Plots/DCC Error and Warning") != std::string::npos)||
       (o.name.find("DataFormatMonitor/DCC Plots/DCC Nonzero") != std::string::npos)          ||
       (o.name.find("DataFormatMonitor/DCC Plots/DCC Status Flags") != std::string::npos)       )  {
      TText t;
      t.SetTextSize( 0.1);
    if (obj->GetEntries() == 0) {
      t.DrawText(1, 1, "Empty == OK"); }
    //else {
    //  char yop[20];
    //  sprintf(yop,"%d", (double) obj->GetEntries());
    //  t.DrawText(1, 1, yop); } }
  }
  else if ( (o.name.find("HTR Plots/ Channel Data Integrity/FED 7") != std::string::npos )         ) {
    c->SetBottomMargin(0.200);
    TLine line;
    line.SetLineWidth(1);
    for (int i=0; i<24; i++) {   // x-axis:24 channels
      for (int j=0; j<15; j++) { // y-axis:15 spigots  
	line.DrawLine(MARGIN+(i*FEDS_X0), MARGIN+(j*CHNS_Y0),
		      ((i+1) *  FEDS_X0), MARGIN+(j*CHNS_Y0)    );
	line.DrawLine(MARGIN+(i*FEDS_X0), MARGIN+(j*CHNS_Y0)+2,
		      ((i+1) *  FEDS_X0), MARGIN+(j*CHNS_Y0)+2  );

	line.DrawLine(MARGIN+(i*FEDS_X0)  , MARGIN+(j*CHNS_Y0),
		      MARGIN+(i*FEDS_X0)  , ((j+1) *  CHNS_Y0)    );
	line.DrawLine(MARGIN+(i*FEDS_X0)+2, MARGIN+(j*CHNS_Y0),
		      MARGIN+(i*FEDS_X0)+2, ((j+1) *  CHNS_Y0)    );
      }}
    // Draw a legend above the plot
    line.DrawLine((FEDS_X0*20)  , (CHNS_Y0*16)  ,
		  (FEDS_X0*21)-1, (CHNS_Y0*16)  );
    line.DrawLine((FEDS_X0*20)  , (CHNS_Y0*17)-1,
		  (FEDS_X0*21)-1, (CHNS_Y0*17)-1);
			      	      	    
    line.DrawLine((FEDS_X0*20)  , (CHNS_Y0*16)  ,
		  (FEDS_X0*20)  , (CHNS_Y0*17)-1);
    line.DrawLine((FEDS_X0*21)-1, (CHNS_Y0*16)  ,
		  (FEDS_X0*21)-1, (CHNS_Y0*17)-1);
    TText tx;
    tx.SetTextSize( 0.02);
    tx.DrawText((FEDS_X0*20)-6, (CHNS_Y0*16)     , "DigiSize");
    tx.DrawText((FEDS_X0*20)-4, (CHNS_Y0*17)-1.75, "AOK"     );
			     
    tx.DrawText((FEDS_X0*21)  , (CHNS_Y0*16)    , "CapRotat" );
    tx.DrawText((FEDS_X0*21)  , (CHNS_Y0*17)-1.75,"!DV or ER");            
    return;}
  if ( (o.name.find("DataFormatMonitor/DCC Plots/DCC Error and Warning") != std::string::npos )        ||
       (o.name.find("DataFormatMonitor/DCC Plots/DCC Nonzero Spigot Conditions") != std::string::npos )||
       (o.name.find("DataFormatMonitor/DCC Plots/DCC Status") != std::string::npos )                     ) {
    obj->SetStats(1111);
    c->SetLeftMargin( 0.200); // in fractions of a TCanvas... ?
  }
  else if ( (o.name.find("HTR Plots/Channel Integrity Summarized by Spigot") != std::string::npos ) ||
       (o.name.find("HTR Plots/Half-HTR DataIntegrity Check") != std::string::npos )          ) {
      TLine line;
      line.SetLineWidth(1);
      for (int i=0; i<32; i++) {    // x-axis:32 DCC's (FEDs 700:731)
	for (int j=0; j<15; j++) {  // y-axis:15 spigots  
	  line.DrawLine(MARGIN+(i*FEDS_X0), MARGIN+(j*SPIG_Y0),
			((i+1) *  FEDS_X0), MARGIN+(j*SPIG_Y0)    );
	  line.DrawLine(MARGIN+(i*FEDS_X0), MARGIN+(j*SPIG_Y0)+3,
			((i+1) *  FEDS_X0), MARGIN+(j*SPIG_Y0)+3    );

	  line.DrawLine(MARGIN+(i*FEDS_X0)  , MARGIN+(j*SPIG_Y0),
			MARGIN+(i*FEDS_X0)  , ((j+1) *  SPIG_Y0)    );
	  line.DrawLine(MARGIN+(i*FEDS_X0)+2, MARGIN+(j*SPIG_Y0),
			MARGIN+(i*FEDS_X0)+2, ((j+1) *  SPIG_Y0)    );
	}}
      // Draw a legend above the plot
      line.DrawLine(FEDS_X0*30, (SPIG_Y0*16)-1,
		    FEDS_X0*31, (SPIG_Y0*16)-1);
      line.DrawLine(FEDS_X0*30, (SPIG_Y0*17)-1,
		    FEDS_X0*31, (SPIG_Y0*17)-1);
			      	      
      line.DrawLine(FEDS_X0*30, (SPIG_Y0*16)-1,
		    FEDS_X0*30, (SPIG_Y0*17)-1);
      line.DrawLine(FEDS_X0*31, (SPIG_Y0*16)-1,
		    FEDS_X0*31, (SPIG_Y0*17)-1);
      TText tx;
      tx.SetTextSize( 0.02);
      if (o.name.find("Channel") != std::string::npos ) {
	tx.DrawText((FEDS_X0*30)-5, (SPIG_Y0*17)-2  , "AOK");
	tx.DrawText((FEDS_X0*30)-8, (SPIG_Y0*17)-3.5, "DigiSize");
	tx.DrawText((FEDS_X0*30)-9, (SPIG_Y0*17)-5  , "WdCntErr");

	tx.DrawText((FEDS_X0*30)+4 , (SPIG_Y0*17)-2  , "!DV or ER");
	tx.DrawText((FEDS_X0*30)+4 , (SPIG_Y0*17)-3.5, "CapRotat");
	tx.DrawText((FEDS_X0*30)+4 , (SPIG_Y0*17)-5  , "SizeDecl");}
      if (o.name.find("Half-HTR") != std::string::npos ) {
	tx.DrawText((FEDS_X0*30)-5, (SPIG_Y0*17)-2  , "EvN");
	tx.DrawText((FEDS_X0*30)-5, (SPIG_Y0*17)-3.5, "BcN");
	tx.DrawText((FEDS_X0*30)-5, (SPIG_Y0*17)-5  , "OrN");
												        
	tx.DrawText((FEDS_X0*30)+4, (SPIG_Y0*17)-2  , "Cerr");
	tx.DrawText((FEDS_X0*30)+4, (SPIG_Y0*17)-3.5, "Uerr");
	tx.DrawText((FEDS_X0*30)+4, (SPIG_Y0*17)-5  , "NoData");}
      return;}
  else if (o.name.find("DataFormatMonitor/Readout Chain Data Integrity Check") != std::string::npos ) {
    TText tx;
    tx.SetTextSize( 0.05);
    tx.DrawText(36, -4, "Crate / FED ID");}
    
} // postDrawTH2(...)
