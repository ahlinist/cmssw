#include "UFAV.h"
#include "TSystem.h"
#include "TFile.h"
#include "TStyle.h"
#include "TColor.h"
#include "Rtypes.h"
#include "TAttFill.h"
#include "TROOT.h"
#include <vector> 
#include "TError.h"
using namespace std;
int main(int argc, char* argv[])
{  
  if(argc != 5 && argc != 6 ) 
    {
      cerr << argv[0] << " Usage: [NewRootFile1.root][ReferenceRootFile2.root][Run # of RootFile 1][Run # of RootFile 2][ProjectName]" << endl;
      return -1;
    }

  TString File1(argv[1]);
  TString File2(argv[2]);
  if( !File1.EndsWith(".root") )
    {
      cerr << argv[1] << " must be the name of the new DQM root file " << endl;
      return -2;
    }
  if( !File2.EndsWith(".root") )
    {
      cerr << argv[2] << " must be the name of the reference DQM root file " << endl;
      return -3;
    }

  TString NewRun(argv[3]);
  TString RefRun(argv[4]);
  
  TString ProjectName = argc == 6?  argv[5] : "Results" ;

  gStyle->SetPalette(1);          //Formatting, no need to change 
  gStyle->SetNumberContours(50);  //Formatting, no need to change
  gErrorIgnoreLevel = 1001;       //Silence Info/Warnings from ROOT :)

      
  UFAV *AnalysisViewer = new UFAV(ProjectName);

  //Declare and set formats for TH1's .  See TH1Format.h for defaults.
  //In principle you can use just one TH1Format object for all of your histograms.
  //If you are overlaying histograms, then you will want a separate TH1Format for each sample.
  TH1Format format1;
  format1.markercolor = kBlack;
  format1.linecolor   = kBlack;
  format1.markerstyle = 21;
  format1.drawoption  = "pe1";

  TH1Format format2;  
  format2.linecolor = kRed+1;
  format2.fillcolor = kRed+1;
  format2.fillstyle = 1001;

  //Define scalefactors (weights) for each file.  UFAV will scale your 1d histograms. (Default is 1.0)
  float weight1= 1.0;
  float weight2= 1.0;
  
  AnalysisViewer->AddFile(File1,"Run"+NewRun,format1, weight1, "DQMData/Run "+NewRun);
  AnalysisViewer->AddFile(File2,"Run"+RefRun,format2, weight2, "DQMData/Run "+RefRun);
  
  AnalysisViewer->SetDirectoryStatus("DQMData",0);  
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMET/BasicCleanup",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMET/BasicCleanup/Ele",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMET/BasicCleanup/Muon",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMET/BasicCleanup/PhysicsDeclared",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMET/BasicCleanup/HighMET",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMET/BasicCleanup/LowMET",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMET/BasicCleanup/HighPtJet",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMET/BasicCleanup/LowPtJet",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMETNoHF/BasicCleanup",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMETNoHF/BasicCleanup/Ele",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMETNoHF/BasicCleanup/Muon",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMETNoHF/BasicCleanup/PhysicsDeclared",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMETNoHF/BasicCleanup/HighMET",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMETNoHF/BasicCleanup/LowMET",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMETNoHF/BasicCleanup/HighPtJet",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/CaloMETNoHF/BasicCleanup/LowPtJet",1);
  /* Disabled until these histograms are actually filled
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/MuCorrMET/BasicCleanup",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/MuCorrMET/BasicCleanup/Ele",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/MuCorrMET/BasicCleanup/Muon",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/MuCorrMET/BasicCleanup/PhysicsDeclared",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/MuCorrMET/BasicCleanup/HighMET",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/MuCorrMET/BasicCleanup/LowMET",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/MuCorrMET/BasicCleanup/HighPtJet",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/MuCorrMET/BasicCleanup/LowPtJet",1);
  */
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/PfMET/BasicCleanup",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/PfMET/BasicCleanup/Ele",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/PfMET/BasicCleanup/Muon",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/PfMET/BasicCleanup/PhysicsDeclared",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/PfMET/BasicCleanup/HighMET",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/PfMET/BasicCleanup/LowMET",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/PfMET/BasicCleanup/HighPtJet",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/PfMET/BasicCleanup/LowPtJet",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/TcMET/BasicCleanup",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/TcMET/BasicCleanup/Ele",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/TcMET/BasicCleanup/Muon",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/TcMET/BasicCleanup/PhysicsDeclared",0);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/TcMET/BasicCleanup/HighMET",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/TcMET/BasicCleanup/LowMET",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/TcMET/BasicCleanup/HighPtJet",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/MET/TcMET/BasicCleanup/LowPtJet",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/Jet/PFJets",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/Jet/JPT",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/Jet/CleanedAntiKtJets",1);
  AnalysisViewer->SetDirectoryStatus("DQMData/Run "+NewRun+"/JetMET/Run summary/CaloTowers/SchemeB",1);
  AnalysisViewer->SetDirectoryStatus("Reference",0);
  
  AnalysisViewer->SetMode(2) ;             //Treat all rootfiles separately (Mode_ = 1) or compare up to 7 rootfiles (Mode_ = 2)
  AnalysisViewer->SetRebin(1);             //Rebin all 1D histograms (uses mean and rms to find optimal rebinning factor if one exists)
  AnalysisViewer->SetRebinMax(4);          //Set maximum rebinning value allowed (only necessary if SetRebin(1) is invoked)
  AnalysisViewer->SetCaptureTails(0);      //Zoom in on x-axis so that it captures the max and min occupied bin (overrules ZoomX())
  AnalysisViewer->SetZoomX(20) ;           //Zoom in on x-axis so that it captures at most "n" rms's of the distribution.
  AnalysisViewer->SetZoomY(1.3,12.);       //Zoom in on y-axis so that it captures n multiples of ymax (arg1 for linear plots, arg2 for log)
  AnalysisViewer->SetLogY(1);              //Will print each 1D histogram twice, one without logscale and one with logscale. 
  AnalysisViewer->Set2DHistoDraw("COLZ");  //Set Draw() properties for any potential 2D histograms that you will be printing.  
  AnalysisViewer->DisplayKSScore(1);       //Display Kolmogorov Test Score of 2 overlayed histograms. WARNING: Histograms are normalized to unity 
  AnalysisViewer->SetPrintHistos(1);       //If histos are already printed and sit in appropriate directories, then set to false to just make HTML's 
  AnalysisViewer->SetImageType("png");     //Set type of image to be printed and displayed by UFAV (e.g.  gif, png, jpeg )
  AnalysisViewer->SetVerbosity(1);         //Set verbosity of UFAV.  Do you want to know everything UFAV's doing? (binary argument)
  AnalysisViewer->SetDescription("JetMETDQM");
  AnalysisViewer->SetThumbTop(80);         //Set Thumbnail Pixel Position from Top of Browser (default is optimized for Macbook screen) 
  AnalysisViewer->SetThumbLeft(425);       //Set Thumbnail Pixel Position from Left of Browser (default is optimized for Macbook screen) 
  AnalysisViewer->SetThumbWidth(392);      //Set Pixel Width of Thumbnail  (default is optimized for Macbook screen) 
  AnalysisViewer->SetThumbHeight(310);     //Set Pixel Height of Thumbnail  (default is optimized for Macbook screen) 

  AnalysisViewer->MakeUFAV();

  delete AnalysisViewer;
}
