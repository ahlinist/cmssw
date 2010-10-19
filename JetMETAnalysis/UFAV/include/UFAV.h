#ifndef UFAV_H
#define UFAV_H

//UF Analsis Viewer (v1.5)
//Author : Ronny Remington, University of Florida
//Inspired by technology developed by M.Schmitt, B. Scurlock, and R. Remington (UF)
//Date : 02/12/09

#include "TObject.h"
#include "TH1.h"
#include "TH2F.h"
#include "TList.h"
#include "TKey.h"
#include "TDirectory.h"
#include "TString.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TIterator.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TError.h"
#include "TH1Format.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <sys/stat.h> 


#include "DataSet.h"
using namespace std;

class UFAV {
 public:
  
  UFAV(TString local_dir="UFAV/");
  
  //Make UF Analysis Viewer from start to scratch
  void MakeUFAV();
  //SuperImpose Plots From Two Directories
  void SuperimposeDirectories(TString  dir1, TString dir2, TString comparison_name, TH1Format& f1, TH1Format& f2);
  void SuperimposeDirectories(TString  dir1, TString dir2, TString comparison_name="");
  //Set Width of Thumbnail  (default is optimized for Macbook screen) 
  void SetThumbWidth(float width = 491){ThumbWidth_ = width; NoteWidth_ = width;}
  //Set Height of Thumbnail  (default is optimized for Macbook screen)
  void SetThumbHeight(float height = 378){ThumbHeight_ = height;}
  //Set Thumbnail Position from Top of Browser (default is optimized for Macbook screen)
  void SetThumbTop(float top = 80){ThumbTop_ = top; NoteTop_ = ThumbTop_ + ThumbHeight_ + 10;}
  //Set Thumbnail Positioin from Left of Browser (default is optimized for Macbook screen)
  void SetThumbLeft(float left = 420){ ThumbLeft_ = left; NoteLeft_ = left;}
  //Set PrintHistos Flag (turn off if you just want to generate the html)
  void SetPrintHistos(bool flag=true){PrintHistograms_ = flag;}
  //Set Line Color of all histograms
  void SetLineColor(Color_t linecolor=kBlack){LineColor_=linecolor;}
  //Set Fill Style of Histograms
  void SetFillStyle(Style_t style=3001){ FillStyle_ = style;}
  //Set Fill Color of Histograms
  void SetFillColor(Color_t fillcolor=kWhite){FillColor_=fillcolor;}
  //Set Scale Factor of all histograms
  void SetScaleFactor(float factor){Weight_ =  factor;}
  //Set whether or not to draw with default errors
  void SetDrawErrors(bool err){Errors_ = err;}
  //Set Description 
  void SetDescription(TString desc_){ Description_ = desc_;}
  //Set Rebin flag, which will rebin the histograms according to an algo based on nbinsX, mean, rms
  void SetRebin(bool rebin){Rebin_ = rebin;}
  //Set Maximum Rebinning Factor
  void SetRebinMax(int max){RebinMax_ = max;}
  //Get Rebinning Factor
  Int_t GetRebin(const TH1& histo);
  //Set X-Zoom based on number of sigmas to capture
  void SetZoomX(float sigmas=5){ Zoom_ = true; Sigmas_ = sigmas; }
  void SetZoom(float sigmas=5){ Zoom_ = true; Sigmas_ = sigmas; }
  //Capture Tails of distribution (overrules SetZoomX)
  void SetCaptureTails(bool x){CaptureTails_ =x ;}
  //Set Y-Zoom based on multiples of ymax to capture for linear and log plots
  void SetZoomY(float linmax= 1.3, float logmax = 10.) {YMaxLinearScale_=linmax; YMaxLogScale_ = logmax;}
  //Set 2D histo draw properties  
  void Set2DHistoDraw(TString draw="COLZ"){DrawOption2D_ = draw;}
  //Set Log plot if desired
  void SetLogY(bool flag) { LogY_ = flag; }
  //Add a Root File for UFAV to process
  void AddFile(TString FileName, TString description="", float weight=1);
  void AddFile(TString FileName, TString description, TH1Format formats, float weight = 1, TString UncommonSubdirectoryPrefix="");
  //Add Caption to Histogram
  void AddCaption(TString HistoPath, TString Caption);
  //Remove substring from string
  void RemoveSubstring(TString& string_ , TString substring_);
  //Remove text before first instance of delimiter 
  void RemovePrefix(TString& string_ , TString delimiter);
  //Set Image Extension
  void SetImageType(TString ext= "gif"){ Ext_ = ext;}
  //Set Verbosity (do you want to know what UFAV is doing?)
  void SetVerbosity(int verb){Verbosity_  = verb;}
  //Return Cumulative Histogram from left-to-right ('rtl') or from right-to-left ('ltr')
  TH1F GetCumulative(TH1F&,TString opt="ltr");
  //Set Mode : type of UFAV to produce [ 1 = normal plots for each file supplied, 2 =  superimpose histograms from all files ]
  void SetMode(int m ){Mode_ = m ;}
  //Veto/Allow UFAV production of a TDirectory
  void SetDirectoryStatus(TString dirname, bool allow);
  // Display Kolmogorov-Smirnov test result for superimpose histograms
  void DisplayKSScore(bool flag){KS_ = flag;}
  ~UFAV();

 private:
  ofstream *IndexHTML_;            //Top level HTML Document
  ofstream *Frame1HTML_;           //HTML Document which lists the directories 

  std::vector<TFile*> vFile_;
  TString HTMLReference_;
  TString UNIXBaseDirectory_;
  TString UNIXCurrentDirectory_;
  TString UNIXCommand_;
  TString NoDataIcon_;
  TString HistogramName_;
  TString HistogramTitle_;
  TString PrintName_;
  TString DirectoryName_;
  TString BranchName_;
  TString BranchTitle_;
  std::vector<TString> vRootFileName_;
  TString RootFileName_;
  std::vector<TString> vRootFilePath_;
  TString RootFilePath_;  
  TString RootFileSubDirectoryPath_;
  TString LocalDirectory_;
  TString Ext_; 
  int Mode_;
  float YMaxLinearScale_;
  float YMaxLogScale_;
  int RebinMax_;
  bool CaptureTails_;

  std::vector<TString> vVetoDirectory;
  std::vector<TString> vAllowDirectory;
  //Histogram Formating 
  TString Description_;         
  Color_t LineColor_; 
  Color_t FillColor_;
  Style_t FillStyle_;
  TString DrawOption2D_;
  TString DrawOption1D_;

  //Thumbnail Features
  float ThumbHeight_;
  float ThumbWidth_;
  float ThumbLeft_;
  float ThumbTop_;

  //Caption Features
  float NoteTop_;
  float NoteLeft_;
  float NoteHeight_;
  float NoteWidth_;

  
  int Layer_;                  // Keep track of which subdirectory layer currently in... 
  float Sigmas_;               // Number of sigmas's to span for the 1D histogram X-axis
  float Weight_;               // Scale Factor for the 1D histograms 
  float NormFactor_;           // Normalization Factor For 1D histograms   NEEDS TO BE IMPLEMENTED!!!!
  int  Verbosity_ ;            // Verbosity level [0 = nothing, 1 = updates]
  bool Errors_;                // Draw Errors ?
  bool PrintHistograms_;       // Print histograms or just make HTML Files ?
  bool Rebin_;                 // Rebin histograms based on mean, rms ?
  bool Zoom_;                  // Zoom in on histograms based on mean and rms?
  bool LogY_;                  // Set Log Scale ?
  bool SuperimposeDirectories_;// Superimpose 2 Directories
  bool RootFilesValidated_;    // Do user supplied root files exist and are they able to be opened?
  bool KS_;                    // Report score of Kolmogorov Test between 2 histogrmas (only used w/ Mode_ == 2 and vFile_.size() ==2)
  std::vector< TString > vSuperimposedDirectory1;
  std::vector< TH1Format > vSuperimposedDirectory1Format;
  std::vector< TString > vSuperimposedDirectory2;
  std::vector< TH1Format > vSuperimposedDirectory2Format;
  std::vector< TString > vSuperimposedDirectoryName;
  
  std::vector< TH1Format > vTH1Format; 
  std::vector< TString > vFileDescription;
  std::vector< float > vWeight;
  std::vector< TString > vUncommonPrefix;
  std::vector< TString > vCaption;

  //Link to Icons within Unix environment
  void LinkToIcon(TString iconname, TString target);
  //Begin HTML Document
  void BeginHTML(ofstream &html, TString webpage_title="Plots", TString bgcolor="#ffffff");
  //Close HTML Document
  void EndHTML(ofstream &html);
  //Rebin Histograms
  void Rebin(TH1& histo);
  //Insert Link to image in HTML
  void InsertLinkToImage(ofstream &html, TString gifname, TString link, bool insert_log = true, double ks_score = -1.);
  //Insert link to directory in HTML 
  void InsertLinkToDirectory( TDirectory *dir, TString HTMLReference);
  //Insert link to superimposed directories in HTML 
  void InsertLinkToSuperimposedDirectory( TString SuperimposedDirName, TString HTMLReference);
  //Find out if object is of a certain class
  Bool_t  IsItA(TKey *key, TString class_){ TString a = key->GetClassName(); return a.BeginsWith(class_) ;}
  //Make directory in Unix environment
  void MakeUNIXDirectory(TString directory_name);
  //Cd within UNIX environment
  void ChangeToUNIXDirectory(TString directory_name);
  //Make HTML Document for histos within root file "filename" (assumes gif files have already been made)
  //  void MakeHTML(ofstream &html, TFile* F, TDirectory *dir);
  void MakeHTML(ofstream &html, unsigned int f, TDirectory *dir);
  //Make HTML Document for contents in a unix directory
  void MakeHTML(TString UNIXDirectory);
  //Make HTML Document for histos superimposed from two different directories
  void MakeHTMLForSuperimposedDirectories(ofstream &html, TDirectory *dir1 , TDirectory *dir2 ,TString superimposed_dirname);
  //Initiate HTML Making Functions
  void InitiateHTMLProduction();
  //Format Histos using parameters input from user
  void FormatHistogram1D( TH1& H, unsigned int x = 0 );
  void FormatHistogram2D( TH2& H);
  //Iterate Through RootFile Directories and print plots to gif files
  //void MakePlots(TFile* F, TDirectory *dir);
  void MakePlots(unsigned int f, TDirectory *dir);
  //Make plots that consist of superimposed histos from two directories
  //void MakePlotsForSuperimposedDirectories(TString dir1name, TString dir2name, TString superimposed_dirname);
  void MakePlotsForSuperimposedDirectories(unsigned int f);
  //Check if directory should be superimposed
  Bool_t SuperimposeThisDirectory(TString dirname) ;
  // Format histograms from superimposed directories
  void FormatSuperimposedHistograms(TH1& h1, TH1& h2, TH1Format& f1, TH1Format& f2, TPaveStats* stats_1, TPaveStats* stats_2);
  // Print 1d histograms
  void PrintHistogram1D(TH1& H, TCanvas& c_, unsigned int index = 0);
  // Print 2d histograms
  void PrintHistogram2D(TH2& H, TCanvas& c_);
  // Print histograms from superimposed directories
  void PrintSuperimposedHistograms(TH1& h1, TH1& h2,TH1Format& f1, TH1Format &f2, TPaveStats* stats_1, TPaveStats* stats_2, TString printname);
  // Print histograms from TTree branches
  void PrintTTreeHistograms(TTree *tree );
  //Check to see if File actually exists before accessing it!
  Bool_t FileExists(TString FileName) ;
  //Insert Caption for histogram
  void InsertCaption(ofstream& html, TString HistogramName);
  //Check to see if this histogram has a caption
  int HasCaption(TString HistogramName);
  //Remove precedeing and succeeding backslashes from TString
  void RemoveBackslashes(TString &d);
  //Remove precedeing and succeeding blankspaces from TString
  void RemoveBlankspaces(TString &d);
  //Strip extension off of TSTring
  void StripExtension(TString &d);
  //Display version of UFAV 
  void DisplayVersion();
  //Get status of TDirectory
  bool GetDirectoryStatus(TDirectory *dir);
  // Get result of kolmogorov test score
  double GetKSScore(TString histoname);
  double GetKSScore(TH1F& H1, TH1F& H2);
};

     
#endif 

