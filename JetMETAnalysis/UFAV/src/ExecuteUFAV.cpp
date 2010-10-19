#include "UFAV.h"
#include "TSystem.h"
#include "TFile.h"
#include "TStyle.h"
#include "TColor.h"
#include "Rtypes.h"
#include "TAttFill.h"
#include <vector> 

using namespace std;
int main(int argc, char* argv[])
{  
  gStyle->SetPalette(1);          //Formatting, no need to change 
  gStyle->SetNumberContours(50);  //Formatting, no need to change
  gErrorIgnoreLevel = 1001;       //Silence Info/Warnings from ROOT :)                                    

  //Declare & instantiate UFAV
  UFAV *AnalysisViewer = new UFAV("Results");

  //Declare and set formats for TH1's .  See TH1Format.h for defaults.
  //In principle you can use just one TH1Format object for all of your histograms.
  //If you are overlaying histograms, then you will want a separate TH1Format for each sample.
  TH1Format format1;  
  format1.linecolor = kRed+1;
  format1.fillcolor = kRed+1;
  format1.fillstyle = 1001;

  TH1Format format2;
  format2.markercolor = kBlack;
  format2.linecolor   = kBlack;
  format2.markerstyle = 21;
  format2.drawoption  = "pe1";

  //Define scalefactors (weights) for each file.  UFAV will scale your 1d histograms. (Default is 1.0)
  float weight1= 1.0;
  float weight2= 1.0;

  //-------------For DEMO (comment out or delete otherwise)--------//
  /*
  //Add root files for UFAV to process 
  AnalysisViewer->AddFile("TestFile1.root", "Description_of_file_1", format1, weight1);
  AnalysisViewer->AddFile("TestFile2.root", "Description_of_file_2", format2, weight2);

  //Enable/Disable subdirectories from  being processed by UFAV (all are enabled by default)
  AnalysisViewer->SetDirectoryStatus("DQMData",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/RecoMETV/MET_Global/met",1);  
  AnalysisViewer->SetDirectoryStatus("DQMData/RecoMETV/MET_Global/metNoHF",1);

  //Take histograms from two subdirectories within the same root file and overlay them (histograms must have same names and must set Mode_ == 1)
  AnalysisViewer->SuperimposeDirectories("DQMData/RecoMETV/MET_Global/met","DQMData/RecoMETV/MET_Global/metNoHF","met_vs_metNoHF");    
  */
  //----------- End DEMO --------//

  
  //--------- For JetMET DQM ------------//
  AnalysisViewer->AddFile("/afs/cern.ch/user/r/rcr/scratch0/UserCode/RRemington/DQM_V0001_R000132661__MinimumBias__Commissioning10-PromptReco-v8__RECO.root", "Run132661", format1, weight1,"DQMData/Run 132661");
  AnalysisViewer->AddFile("/afs/cern.ch/user/r/rcr/scratch0/UserCode/RRemington/DQM_V0001_R000132662__MinimumBias__Commissioning10-PromptReco-v8__RECO.root", "Run132662", format2, weight2,"DQMData/Run 132662");
  //Disable subdirectories from  being processed by UFAV
  AnalysisViewer->SetDirectoryStatus("DQMData",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run 132661/JetMET/Run summary/MET/CaloMET/All",1);
  AnalysisViewer->SetDirectoryStatus("Reference",0);
  //------ End DQM -------------//
 
  
  AnalysisViewer->SetMode(2) ;             //Treat all rootfiles separately (Mode_ = 1) or compare up to 7 rootfiles (Mode_ = 2)
  AnalysisViewer->SetRebin(0);             //Rebin 1D histograms (relies on mean and rms to find optimal rebinning if one exists)
  AnalysisViewer->SetZoom(5) ;             //Zoom in on x-axis so that it captures at most "n" rms's of the distribution.
  AnalysisViewer->SetLogY(1);              //Will print each 1D histogram twice, one without logscale and one with logscale. 
  AnalysisViewer->Set2DHistoDraw("COLZ");  //Set Draw() properties for any potential 2D histograms that you will be printing.  
  AnalysisViewer->DisplayKSScore(1);       //Display Kolmogorov Test Score of 2 overlayed histograms. WARNING: Histograms are normalized to unity 
  AnalysisViewer->SetPrintHistos(1);       //If histos are already printed and sit in appropriate directories, then set to false to just make HTML's 
  AnalysisViewer->SetImageType("gif");     //Set type of image to be printed and displayed by UFAV (e.g.  gif, png, jpeg )
  AnalysisViewer->SetVerbosity(1);         //Set verbosity of UFAV.  Do you want to know everything UFAV's doing? (binary argument)
  AnalysisViewer->SetThumbTop(80);         //Set Thumbnail Pixel Position from Top of Browser (default is optimized for Macbook screen) 
  AnalysisViewer->SetThumbLeft(425);       //Set Thumbnail Pixel Position from Left of Browser (default is optimized for Macbook screen) 
  AnalysisViewer->SetThumbWidth(392);      //Set Pixel Width of Thumbnail  (default is optimized for Macbook screen) 
  AnalysisViewer->SetThumbHeight(310);     //Set Pixel Height of Thumbnail  (default is optimized for Macbook screen) 

  AnalysisViewer->MakeUFAV();
  delete AnalysisViewer;
}
