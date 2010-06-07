#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TProfile2D.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TError.h"
#include "TTree.h"
#include "TGraphErrors.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "CondFormats/EcalObjects/interface/EcalTimeCalibConstants.h"
#include "CondFormats/EcalObjects/interface/EcalTimeCalibErrors.h"
//#include "CondTools/Ecal/interface/EcalTimeCalibConstantsXMLTranslator.h"
//#include "CondTools/Ecal/interface/EcalTimeCalibErrorsXMLTranslator.h"
#include "CondTools/Ecal/interface/EcalCondHeader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "CreateEcalTimingCalibs.h"

// ****************************************************************
void SetEStyle() {
  TStyle* EStyle = new TStyle("EStyle", "E's not Style");

  //set the background color to white
  EStyle->SetFillColor(10);
  EStyle->SetFrameFillColor(10);
  EStyle->SetFrameFillStyle(0);
  EStyle->SetFillStyle(0);
  EStyle->SetCanvasColor(10);
  EStyle->SetPadColor(10);
  EStyle->SetTitleFillColor(0);
  EStyle->SetStatColor(10);

  //dont put a colored frame around the plots
  EStyle->SetFrameBorderMode(0);
  EStyle->SetCanvasBorderMode(0);
  EStyle->SetPadBorderMode(0);

  //use the primary color palette
  EStyle->SetPalette(1,0);
  EStyle->SetNumberContours(255);

  //set the default line color for a histogram to be black
  EStyle->SetHistLineColor(kBlack);

  //set the default line color for a fit function to be red
  EStyle->SetFuncColor(kRed);

  //make the axis labels black
  EStyle->SetLabelColor(kBlack,"xyz");

  //set the default title color to be black
  EStyle->SetTitleColor(kBlack);
   
  // Sizes

  EStyle->SetPadBottomMargin(.15);
  EStyle->SetPadTopMargin(0.08);
  EStyle->SetPadLeftMargin(0.14);
  EStyle->SetPadRightMargin(0.12);
  
  //set axis label and title text sizes
  EStyle->SetLabelSize(0.04,"xyz");
  EStyle->SetTitleSize(0.06,"xyz");
  EStyle->SetTitleOffset(1.,"x");
  EStyle->SetTitleOffset(1.1,"yz");
  EStyle->SetStatFontSize(0.04);
  EStyle->SetTextSize(0.04);
  EStyle->SetTitleBorderSize(Width_t(0.5));
  //EStyle->SetTitleY(0.5);
  
  //set line widths
  EStyle->SetHistLineWidth(1);
  EStyle->SetFrameLineWidth(2);
  EStyle->SetFuncWidth(2);

  //Paper Size
  EStyle->SetPaperSize(TStyle::kUSLetter);

  // Misc

  //align the titles to be centered
  //Style->SetTextAlign(22);

  //set the number of divisions to show
  EStyle->SetNdivisions(506, "xy");

  //turn off xy grids
  EStyle->SetPadGridX(0);
  EStyle->SetPadGridY(0);

  //set the tick mark style
  //EStyle->SetPadTickX(1);
  //EStyle->SetPadTickY(1);

  //show the fit parameters in a box
  EStyle->SetOptFit(111111);

  //turn on all other stats
   //EStyle->SetOptStat(0000000);
  //EStyle->SetOptStat(1111111);
  //With errors
  EStyle->SetOptStat(1112211);

  //Move stats box
  //EStyle->SetStatX(0.85);

  //marker settings
  EStyle->SetMarkerStyle(8);
  EStyle->SetMarkerSize(0.8);
   
  // Fonts
   EStyle->SetStatFont(42);
   EStyle->SetLabelFont(42,"xyz");
   EStyle->SetTitleFont(42,"xyz");
   EStyle->SetTextFont(42);
//  EStyle->SetStatFont(82);
//   EStyle->SetLabelFont(82,"xyz");
//   EStyle->SetTitleFont(82,"xyz");
//   EStyle->SetTextFont(82);

  //done
  EStyle->cd();
}

// ****************************************************************
std::string intToString(int num)
{
  using namespace std;
  ostringstream myStream;
  myStream << num << flush;
  return(myStream.str()); //returns the string form of the stringstream object
}

// ****************************************************************
//XXX: main
int main(int argc, char* argv[])
{
  //
  // **** May 20 2010 update ****
  // Usage: CreateEcalTimingCalibsEB fileWithTree options...
  //

  using namespace std;

  // For selection cuts
  string inBxs, inOrbits, inTrig, inTTrig, inLumi, inTimes;
  float avgTimeMin, avgTimeMax;
  float minAmpEB, minAmpEE;
  float minEtEB, minEtEE;
  float maxE1E9, maxSwissCrossNoise;  // EB only, no spikes seen in EE
  float maxHitTimeEE, minHitTimeEE;
  // init to sensible defaults
  avgTimeMin = -5.01; // in BX's
  avgTimeMax = 5.01; // in BX's
  minAmpEB = 1; // GeV
  minAmpEE = 1; // GeV
  minEtEB = 0.3; // GeV
  minEtEE = 0.5; // GeV
  maxHitTimeEE = 5; // ns
  minHitTimeEE = -5; // ns
  maxE1E9 = 0.95; // EB only
  maxSwissCrossNoise = 0.95; // EB only
  inBxs = "-1";
  inOrbits = "-1";
  inTrig = "-1";
  inTTrig = "-1";
  inLumi = "-1";
  inTimes = "-1";

  char* infile = argv[1];
  if (!infile)
  {
    cout << " No input file specified !" << endl;
    return -1;
  }

  for (int i=2 ; i<argc ; i++) {
    if (argv[i] == std::string("-bxs") && argc>i+1) inBxs = std::string(argv[i+1]);
    if (argv[i] == std::string("-orbits") && argc>i+1) inOrbits = std::string(argv[i+1]);
    if (argv[i] == std::string("-trig") && argc>i+1) inTrig = std::string(argv[i+1]);
    if (argv[i] == std::string("-ttrig") && argc>i+1) inTTrig = std::string(argv[i+1]);
    if (argv[i] == std::string("-lumi") && argc>i+1) inLumi = std::string(argv[i+1]);
    if (argv[i] == std::string("-times") && argc>i+1) inTimes = std::string(argv[i+1]);
    if (argv[i] == std::string("-ebampmin") && argc>i+1) minAmpEB = atof(argv[i+1]);
    if (argv[i] == std::string("-eeampmin") && argc>i+1) minAmpEE = atof(argv[i+1]);
    if (argv[i] == std::string("-ebetmin") && argc>i+1) minEtEB = atof(argv[i+1]);
    if (argv[i] == std::string("-eeetmin") && argc>i+1) minEtEE = atof(argv[i+1]);
    if (argv[i] == std::string("-e1e9max") && argc>i+1) maxE1E9 = atof(argv[i+1]);
    if (argv[i] == std::string("-swisskmax") && argc>i+1) maxSwissCrossNoise = atof(argv[i+1]);
    if (argv[i] == std::string("-avgtimemin") && argc>i+1) avgTimeMin = atof(argv[i+1]);
    if (argv[i] == std::string("-avgtimemax") && argc>i+1) avgTimeMax = atof(argv[i+1]);
    if (argv[i] == std::string("-eehittimemax") && argc>i+1) maxHitTimeEE = atof(argv[i+1]);
    if (argv[i] == std::string("-eehittimemin") && argc>i+1) minHitTimeEE = atof(argv[i+1]);
  }

  TFile* f = TFile::Open(infile);
  TTree* myTree = (TTree*)f->Get("eventTimingInfoTree");
  if(!myTree)
  {
    std::cout << "Couldn't find tree eventTimingInfoTree" << std::endl;
    return -1;
  }
  else
  {
    cout << "Loaded tree: "<< infile << endl << "Running with options: "
      << "avgTimeMin: " << avgTimeMin << " avgTimeMax: " << avgTimeMax
      << " minAmpEB: " << minAmpEB << " minAmpEE: " << minAmpEE
      << " minEtEB: " << minEtEB << " minEtEE: " << minEtEE
      << " maxE1E9 (EB): " << maxE1E9 << " maxSwissCrossNoise (EB): " << maxSwissCrossNoise
      << " maxHitTimeEE: " << maxHitTimeEE << " minHitTimeEE: " << minHitTimeEE                                                                   
      << " inTrig: " << inTrig << " inTTrig: " << inTTrig << " inLumi: " << inLumi 
      << " inBxs: " << inBxs << " inTimes: " << inTimes << " inOrbits: " << inOrbits
      << endl;
  }

  SetEStyle();
  // Ignore warnings
  gErrorIgnoreLevel = 2001;

  TTreeMembers myTreeMembers;
  setBranchAddresses(myTree,myTreeMembers);

  // Generate all the vectors for skipping selections
  std::vector<std::vector<double> > bxIncludeVector;
  std::vector<std::vector<double> > bxExcludeVector;
  std::vector<std::vector<double> > orbitIncludeVector;
  std::vector<std::vector<double> > orbitExcludeVector;
  std::vector<std::vector<double> > trigIncludeVector;
  std::vector<std::vector<double> > trigExcludeVector;
  std::vector<std::vector<double> > ttrigIncludeVector;
  std::vector<std::vector<double> > ttrigExcludeVector;
  std::vector<std::vector<double> > lumiIncludeVector;
  std::vector<std::vector<double> > lumiExcludeVector;
  std::vector<std::vector<double> > timeIncludeVector;
  std::vector<std::vector<double> > timeExcludeVector;
  //recall: string inBxs, inOrbits, inTrig, inTTrig, inLumi, inTimes;
  genIncludeExcludeVectors(inBxs,bxIncludeVector,bxExcludeVector);
  genIncludeExcludeVectors(inOrbits,orbitIncludeVector,orbitExcludeVector);
  genIncludeExcludeVectors(inTrig,trigIncludeVector,trigExcludeVector);
  genIncludeExcludeVectors(inTTrig,ttrigIncludeVector,ttrigExcludeVector);
  genIncludeExcludeVectors(inLumi,lumiIncludeVector,lumiExcludeVector);
  genIncludeExcludeVectors(inTimes,timeIncludeVector,timeExcludeVector);

  // Open output file and book hists
  string fileNameBeg = "timingCalibsEE";
  string rootFilename = fileNameBeg+".root";
  TFile* outfile = new TFile(rootFilename.c_str(),"RECREATE");
  outfile->cd();

  TH1F* calibHistEE = new TH1F("timingCalibsEE","timingCalibs EE [ns]",2000,-100,100);
  TH1F* calibErrorHistEE = new TH1F("timingCalibErrorEE","timingCalibError EE [ns]",500,0,5);
  calibHistEE->Sumw2();
  calibErrorHistEE->Sumw2();

  TH2F* calibsVsErrors = new TH2F("timingCalibsAndErrors","TimingCalibs vs. errors [ns]",500,0,5,100,0,10);
  calibsVsErrors->Sumw2();

  //TH2F* calibMapEE = new TH2F("calibMapEE","time calib map EE",360,1,361,170,-86,86);
  //TH2F* calibMapEEFlip = new TH2F("calibMapEEFlip","time calib map EE",170,-86,86,360,1,361);
  //TH2F* calibMapEEPhase = new TH2F("calibMapEEPhase","time calib map EE (phase of Tmax)",360,1,361,170,-86,86);
  
  //TH2F* calibMapEtaAvgEE = new TH2F("calibMapEtaAvgEE","time calibs raw eta avg map EE",360,1,361,170,-86,86);
  //TH1F* calibHistEtaAvgEE = new TH1F("timingCalibsEtaAvgEE","EtaAvgTimingCalibs EE [ns]",2000,-100,100);

  //TH1F* eventsEEHist = new TH1F("numEventsEE","Number of events, EE",100,0,100);
  //TH1F* calibSigmaHist = new TH1F("timingSpreadEE","Crystal timing spread [ns]",1000,-5,5);

  //TH2F* avgAmpVsSigmaTHist = new TH2F("avgAmpVsSigmaT","Avg. amp. vs. #sigma_{t}",500,0,5,1000,0,1000);

  //TH1F* chiSquaredEachEventHist = new TH1F("chi2eachEvent","Chi2 of each event",500,0,500);
  //TH2F* chiSquaredVsAmpEachEventHist = new TH2F("chi2VsAmpEachEvent","Chi2 vs. amplitude of each event",500,0,500,750,0,750);
  //TH2F* chiSquaredHighMap = new TH2F("chi2HighMap","Channels with event #Chi^{2} > 100",360,1,361,170,-86,86);
  //TH1F* chiSquaredTotalHist = new TH1F("chi2Total","Total chi2 of all events in each crystal",500,0,500);
  //TH1F* chiSquaredSingleOverTotalHist = new TH1F("chi2SingleOverTotal","Chi2 of each event over total chi2",100,0,1);
  //TH1F* ampEachEventHist = new TH1F("energyEachEvent","Energy of all events [GeV]",1000,0,10);
  //TH1F* numPointsErasedHist = new TH1F("numPointsErased","Number of points erased per crystal",25,0,25);
  
  //TProfile2D* myAmpProfile = (TProfile2D*)EBampProfile->Clone();
  //myAmpProfile->Write();
  TH1F* expectedStatPresHistEEM = new TH1F("expectedStatPresEEM","Avg. expected statistical precision EEM [ns], all crys",200,0,2);
  TH2F* expectedStatPresVsObservedMeanErrHistEEM = new TH2F("expectedStatPresVsObsEEM","Expected stat. pres. vs. obs. error on mean each event EEM [ns]",200,0,2,200,0,2);
  TH1F* expectedStatPresEachEventHistEEM = new TH1F("expectedStatPresSingleEventEEM","Expected stat. pres. each event EEM [ns]",200,0,2);
  TH1F* expectedStatPresHistEEP = new TH1F("expectedStatPresEEP","Avg. expected statistical precision EEP [ns], all crys",200,0,2);
  TH2F* expectedStatPresVsObservedMeanErrHistEEP = new TH2F("expectedStatPresVsObsEEP","Expected stat. pres. vs. obs. error on mean each event [ns] EEP",200,0,2,200,0,2);
  TH1F* expectedStatPresEachEventHistEEP = new TH1F("expectedStatPresSingleEventEEP","Expected stat. pres. each event EEP [ns]",200,0,2);
  
  TH2F* errorOnMeanVsNumEvtsHist = new TH2F("errorOnMeanVsNumEvts","Error_on_mean vs. number of events",50,0,50,200,0,2);
  errorOnMeanVsNumEvtsHist->Sumw2();
  
  TH1F* calibHistEEM = new TH1F("timingCalibsEEM","timingCalibs EEM [ns]",500,-25,25);
  TH1F* calibHistEEP = new TH1F("timingCalibsEEP","timingCalibs EEP [ns]",500,-25,25);
  TH1F* calibErrorHistEEM = new TH1F("timingCalibErrorEEM","timingCalibError EEM [ns]",250,0,5);
  TH1F* calibErrorHistEEP = new TH1F("timingCalibErrorEEP","timingCalibError EEP [ns]",250,0,5);
  calibHistEEM->Sumw2();
  calibHistEEP->Sumw2();
  calibErrorHistEEM->Sumw2();
  calibErrorHistEEP->Sumw2();

  TH2F* calibMapEEM = new TH2F("calibMapEEM","time calib map EEM",100,1,101,100,1,101);
  TH2F* calibMapEEP = new TH2F("calibMapEEP","time calib map EEP",100,1,101,100,1,101);
  calibMapEEM->Sumw2();
  calibMapEEP->Sumw2();

  TProfile* ampProfileEEM = new TProfile("ampProfileEEM","Amp. profile EEM;hashedIndex",14648,0,14648);
  TProfile* ampProfileEEP = new TProfile("ampProfileEEP","Amp. profile EEP;hashedIndex",14648,0,14648);
  TProfile2D* ampProfileMapEEP = new TProfile2D("ampProfileMapEEP","Amp. profile EEP;ix;iy",100,1,101,100,1,101);
  TProfile2D* ampProfileMapEEM = new TProfile2D("ampProfileMapEEM","Amp. profile EEM;ix;iy",100,1,101,100,1,101);
  TH2F* hitsPerCryMapEEM = new TH2F("hitsPerCryMapEEM","Hits per cry EEM;ix;iy",100,1,101,100,1,101);
  TH2F* hitsPerCryMapEEP = new TH2F("hitsPerCryMapEEP","Hits per cry EEP;ix;iy",100,1,101,100,1,101);
  TH1F* hitsPerCryHistEEM = new TH1F("hitsPerCryHistEEM","Hits per cry EEM;hashedIndex",14648,0,14648);
  TH1F* hitsPerCryHistEEP = new TH1F("hitsPerCryHistEEP","Hits per cry EEP;hashedIndex",14648,0,14648);
  //TH1C* eventsEEMHist = new TH1C("numEventsEEM","Number of events, EEM",100,0,100);
  //TH1C* eventsEEPHist = new TH1C("numEventsEEP","Number of events, EEP",100,0,100);

  TDirectory* cryDirEEP = gDirectory->mkdir("crystalTimingHistsEEP");
  cryDirEEP->cd();
  TH1C* cryTimingHistsEEP[100][100]; // [0][0] = ix 1, iy 1
  for(int x=0; x < 100; ++x)
  {
    for(int y=0; y < 100; ++y)
    {
      if(!EEDetId::validDetId(x+1,y+1,1))
        continue;
      string histname = "EEP_cryTiming_ix";
      histname+=intToString(x+1);
      histname+="_iy";
      histname+=intToString(y+1);
      cryTimingHistsEEP[x][y] = new TH1C(histname.c_str(),histname.c_str(),660,-33,33);
      cryTimingHistsEEP[x][y]->Sumw2();
    }
  }
  outfile->cd();
  TDirectory* cryDirEEM = gDirectory->mkdir("crystalTimingHistsEEM");
  cryDirEEM->cd();
  TH1C* cryTimingHistsEEM[100][100]; // [0][0] = ix 1, iy 1
  for(int x=0; x < 100; ++x)
  {
    for(int y=0; y < 100; ++y)
    {
      if(!EEDetId::validDetId(x+1,y+1,-1))
        continue;
      string histname = "EEM_cryTiming_ix";
      histname+=intToString(x+1);
      histname+="_iy";
      histname+=intToString(y+1);
      cryTimingHistsEEM[x][y] = new TH1C(histname.c_str(),histname.c_str(),660,-33,33);
      cryTimingHistsEEM[x][y]->Sumw2();
    }
  }
  outfile->cd();
  
  cout << "Making calibs..." << endl;

  CrystalCalibration* eeCryCalibs[14648];
  //XXX: Making calibs with weighted/unweighted mean
  //XXX: Turn weighed mean back on
  for(int i=0; i < 14648; ++i)
    eeCryCalibs[i] = new CrystalCalibration(); //use weighted mean!
    //eeCryCalibs[i] = new CrystalCalibration(false); //don't use weighted mean!

  cout << "Looping over TTree...";

  // Loop over the TTree
  int numEventsUsed = 0;
  int nEntries = myTree->GetEntries();

  for(int entry = 0; entry < nEntries; ++entry)
  {
    myTree->GetEntry(entry);
    //debug
    //cout << "Number of EE crys in event: " << numEEcrys << endl;

    // Loop once to calculate average event time
    float sumTime = 0;
    for(int cryIndex=0; cryIndex < myTreeMembers.numEEcrys_; ++cryIndex)
    {
      float cryTime = myTreeMembers.cryUTimesEE_[cryIndex];
      sumTime+=cryTime;
    }
    //XXX: Event cuts
    if(sumTime/myTreeMembers.numEEcrys_ > avgTimeMax || sumTime/myTreeMembers.numEEcrys_ < avgTimeMin)
    {
      //cout << "Average event time: " << sumTime/numEBcrys  << " so event rejected." << endl;
      continue;
    }
    bool keepEvent = includeEvent(myTreeMembers.bx_,bxIncludeVector,bxExcludeVector)
      && includeEvent(myTreeMembers.orbit_,orbitIncludeVector,orbitExcludeVector)
      && includeEvent(myTreeMembers.triggers_,myTreeMembers.numTriggers_,trigIncludeVector,trigExcludeVector)
      && includeEvent(myTreeMembers.techtriggers_,myTreeMembers.numTechTriggers_,ttrigIncludeVector,ttrigExcludeVector)
      && includeEvent(myTreeMembers.lumiSection_,lumiIncludeVector,lumiExcludeVector)
      && includeEvent(myTreeMembers.absTime_,timeIncludeVector,timeExcludeVector);
    if(!keepEvent)
      continue;
      
    numEventsUsed++;

    // Loop over the EE crys and fill the map
    for(int cryIndex=0; cryIndex < myTreeMembers.numEEcrys_; ++cryIndex)
    {
      int hashedIndex = myTreeMembers.cryHashesEE_[cryIndex];
      float cryTime = 25*(myTreeMembers.cryUTimesEE_[cryIndex]-5);
      float cryTimeError = 25*myTreeMembers.cryTimeErrorsEE_[cryIndex];
      float cryAmp = myTreeMembers.cryAmpsEE_[cryIndex];
      float cryEt = myTreeMembers.cryETEE_[cryIndex];
      //cout << "STUPID DEBUG: " << hashedIndex << " cryTime: " << cryTime << " ctyTimeError: " << cryTimeError << " cryAmp: " << cryAmp << endl;
      
      EEDetId det = EEDetId::unhashIndex(hashedIndex);
      if(det==EEDetId()) // make sure DetId is valid
        continue;

      int ix = det.ix();
      int iy = det.iy();
      int zside = det.zside();

      //XXX: RecHit cuts
      bool keepHit = cryAmp >= minAmpEE
        && cryEt >= minEtEE
        && cryTime > minHitTimeEE
        && cryTime < maxHitTimeEE;
      if(!keepHit)
        continue;

      eeCryCalibs[hashedIndex]->insertEvent(cryAmp,cryTime,cryTimeError,true);
      //SIC Use when we don't have time_error available
      //ebCryCalibs[hashedIndex]->insertEvent(cryAmp,cryTime,35/(cryAmp/1.2),true);
      if(zside > 0)
      {
        ampProfileEEP->Fill(hashedIndex,cryAmp);
        ampProfileMapEEP->Fill(ix,iy,cryAmp);
      }
      else
      {
        ampProfileEEM->Fill(hashedIndex,cryAmp);
        ampProfileMapEEM->Fill(ix,iy,cryAmp);
      }
      //if(cryTime > 33 || cryTime < -33)
      //  cout << "Crystal: " << det << " event time is over/underflow: " << cryTime << endl;
    }
  }

  //create output text file
  ofstream fileStream;
  string fileName = fileNameBeg+".calibs.txt";
  fileStream.open(fileName.c_str());
  if(!fileStream.good() || !fileStream.is_open())
  {
    cout << "Couldn't open text file." << endl;
    return -1;
  }
  //create problem channels text file
  ofstream fileStreamProb;
  string fileName2 = fileNameBeg+".problems.txt";
  fileStreamProb.open(fileName2.c_str());
  if(!fileStreamProb.good() || !fileStreamProb.is_open())
  {
    cout << "Couldn't open text file." << endl;
    return -1;
  }

  // Create calibration container objects
  EcalTimeCalibConstants timeCalibConstants;
  EcalTimeCalibErrors timeCalibErrors;

  cout << "Using " << numEventsUsed << " out of " << nEntries << " in the tree." << endl;
  cout << "Creating calibs..." << endl;
  //Loop over all the crys
  for(int hashedIndex=0; hashedIndex < 14648; ++hashedIndex)
      
  {
    EEDetId det = EEDetId::unhashIndex(hashedIndex);
    if(det==EEDetId())
      continue;
    CrystalCalibration cryCalib = *(eeCryCalibs[hashedIndex]);
    int x = det.ix();
    int y = det.iy();

    //chiSquaredTotalHist->Fill(cryCalib.totalChi2);
    //expectedStatPresHistEB->Fill(sqrt(1/expectedPresSumEB));
    //expectedStatPresVsObservedMeanErrHistEB->Fill(sigmaM,sqrt(1/expectedPresSumEB));

    //XXX: Filter events at default 0.5*sigma threshold
    cryCalib.filterOutliers();
    
    //numPointsErasedHist->Fill(numPointsErased);
    
    //Write cryTimingHists
    vector<TimingEvent> times = cryCalib.timingEvents;
    for(vector<TimingEvent>::const_iterator timeItr = times.begin();
        timeItr != times.end(); ++timeItr)
    {
      if(det.zside() < 0)
      {
        cryTimingHistsEEM[x-1][y-1]->Fill(timeItr->time);
        cryTimingHistsEEM[x-1][y-1]->SetBinError(cryTimingHistsEEM[x-1][y-1]->FindBin(timeItr->time),timeItr->sigmaTime);
      }
      else
      {
        cryTimingHistsEEP[x-1][y-1]->Fill(timeItr->time);
        cryTimingHistsEEP[x-1][y-1]->SetBinError(cryTimingHistsEEP[x-1][y-1]->FindBin(timeItr->time),timeItr->sigmaTime);
      }
    }
    if(det.zside() < 0)
    {
      cryDirEEM->cd();
      cryTimingHistsEEM[x-1][y-1]->Write();
    }
    else
    {
      cryDirEEP->cd();
      cryTimingHistsEEP[x-1][y-1]->Write();
    }
    outfile->cd();

    if(det.zside() < 0)
    {
      hitsPerCryHistEEM->SetBinContent(hashedIndex+1,cryCalib.timingEvents.size());
      hitsPerCryMapEEM->Fill(x,y,cryCalib.timingEvents.size());
    }
    else
    {
      hitsPerCryHistEEP->SetBinContent(hashedIndex+1,cryCalib.timingEvents.size());
      hitsPerCryMapEEP->Fill(x,y,cryCalib.timingEvents.size());
    }
    
    // Make timing calibs
    float p1 = cryCalib.mean;
    float p1err = cryCalib.sigma;
    //cout << "cry ieta: " << ieta << " cry iphi: " << iphi << " p1: " << p1 << " p1err: " << p1err << endl;
    //XXX: SIC Oct. 2 2009 hardcoding sigma for testing...
    //XXX: Turn it back on
    //float p1err = 0.1;
    if(cryCalib.timingEvents.size() < 5)
    {
      fileStreamProb << "Cry (only " << cryCalib.timingEvents.size() << " events): " << det.zside() << ", "
                                                                                        << x <<", " << y << ", hash: "
                                                                        << hashedIndex
                                                                          << "\t Calib: " << p1 << "\t Error: " << p1err << std::endl;
      continue;
    }
    // Make it so we can add calib to reco time
    p1*=-1;
    if(p1err < 0.5 && p1err > 0)
    {
      fileStream << "EE\t" << hashedIndex << "\t" << p1 << "\t\t" << p1err << endl;
      if(det.zside() < 0)
      {
        calibHistEEM->Fill(p1);
        //calibMapEEMFlip->Fill(y-85,x+1,p1);
        calibMapEEM->Fill(x,y,p1);
        //calibMapEEMPhase->Fill(x+1,y-85,p1/25-floor(p1/25));
        //errorOnMeanVsNumEvtsHist->Fill(times.size(),p1err);
      }
      else
      {
        calibHistEEP->Fill(p1);
        //calibMapEEPFlip->Fill(y-85,x+1,p1);
        calibMapEEP->Fill(x,y,p1);
        //calibMapEEPPhase->Fill(x+1,y-85,p1/25-floor(p1/25));
        //errorOnMeanVsNumEvtsHist->Fill(times.size(),p1err);
      }
    }
    else
    {
      //std::cout << "Cry: " << y <<", " << x << ", hash: " << itr->first
      //  << "\t Calib: " << p1 << "\t Error: " << p1err << std::endl;
      fileStreamProb << "Cry: " << x <<", " << y << ", hash: " << hashedIndex
        << "\t Calib: " << p1 << "\t Error: " << p1err << std::endl;
    }

    if(det.zside() < 0)
    {
      //calibsVsErrorsEEM->Fill(p1err, p1 > 0 ? p1 : -1*p1);
      calibErrorHistEEM->Fill(p1err);
    }
    else
    {
      //calibsVsErrorsEEP->Fill(p1err, p1 > 0 ? p1 : -1*p1);
      calibErrorHistEEP->Fill(p1err);
    }
  }
  
  fileStream.close();
  fileStreamProb.close();

  cout << "Cleaning up." << endl;
  //Write XML files
  //EcalCondHeader header;
  //header.method_="testmethod";
  //header.version_="testversion";
  //header.datasource_="testdata";
  //header.since_=123;
  //header.tag_="testtag";
  //header.date_="Mar 24 1973";
  //string timeCalibFile = "EcalTimeCalibsEB.xml";
  //string timeCalibErrFile = "EcalTimeCalibErrorsEB.xml";
  //XXX TODO ADD LATER
  //EcalTimeCalibConstantsXMLTranslator::writeXML(timeCalibFile,header,timeCalibConstants);
  //EcalTimeCalibErrorsXMLTranslator::writeXML(timeCalibErrFile,header,timeCalibErrors);

  outfile->cd();
  calibHistEEM->SetXTitle("timingCalib [ns]");
  calibHistEEM->Write();
  calibHistEEP->SetXTitle("timingCalib [ns]");
  calibHistEEP->Write();
  calibErrorHistEEP->SetXTitle("uncertainty on mean [ns]");
  calibErrorHistEEP->Write();
  calibErrorHistEEM->SetXTitle("uncertainty on mean [ns]");
  calibErrorHistEEM->Write();

  //can->Print("calibs1D.png");
  //cout << "Writing calibVsErrors" << endl;
  //calibsVsErrors->SetYTitle("AbsCalibConst");
  //calibsVsErrors->SetXTitle("calibConstError");
  //calibsVsErrors->Write();

  //Move empty bins out of the way
  int nxbins = calibMapEEM->GetNbinsX();
  int nybins = calibMapEEM->GetNbinsY();
  for(int i=0;i<=(nxbins+2)*(nybins+2); ++i)
  {
    double binentsM = calibMapEEM->GetBinContent(i);
    if(binentsM==0)
    {
      calibMapEEM->SetBinContent(i,-1000);
    }
    double binentsP = calibMapEEP->GetBinContent(i);
    if(binentsP==0)
    {
      calibMapEEP->SetBinContent(i,-1000);
    }
  }
  calibMapEEM->SetXTitle("ix");
  calibMapEEM->SetYTitle("iy");
  calibMapEEM->Write();
  calibMapEEP->SetXTitle("ix");
  calibMapEEP->SetYTitle("iy");
  calibMapEEP->Write();

  //calibSigmaHist->SetXTitle("#sigma_{cryTime} [ns]");
  //calibSigmaHist->Write();
  
  // Old hist, commented Jun 15 2009
  //avgAmpVsSigmaTHist->SetXTitle("#sigma_{cryTime} [ns]");
  //avgAmpVsSigmaTHist->SetYTitle("Avg. amp. [adc]");
  //avgAmpVsSigmaTHist->Write();
 
  //errorOnMeanVsNumEvtsHist->SetXTitle("Events");
  //errorOnMeanVsNumEvtsHist->SetYTitle("Error_on_mean [ns]");
  //TProfile* theProf = (TProfile*) errorOnMeanVsNumEvtsHist->ProfileX();
  //TF1* myFit = new TF1("myFit","[0]/sqrt(x)+[1]",0,50);
  //myFit->SetRange(0,50);
  ////theProf->Fit("myFit");
  //theProf->Write();
  //errorOnMeanVsNumEvtsHist->Write();
  //
  //chiSquaredEachEventHist->Write();
  //chiSquaredVsAmpEachEventHist->SetXTitle("amplitude [ADC]");
  //chiSquaredVsAmpEachEventHist->SetYTitle("#Chi^{2}");
  //chiSquaredVsAmpEachEventHist->Write();
  //chiSquaredHighMap->SetXTitle("iphi");
  //chiSquaredHighMap->SetYTitle("ieta");
  //chiSquaredHighMap->Write();
  //chiSquaredTotalHist->Write();
  //chiSquaredSingleOverTotalHist->Write();

  expectedStatPresHistEEM->Write();
  expectedStatPresVsObservedMeanErrHistEEM->Write();
  expectedStatPresEachEventHistEEM->Write();
  expectedStatPresHistEEP->Write();
  expectedStatPresVsObservedMeanErrHistEEP->Write();
  expectedStatPresEachEventHistEEP->Write();
  //ampEachEventHist->Write();
  //numPointsErasedHist->Write();
  hitsPerCryHistEEP->Write();
  hitsPerCryMapEEP->Write();
  hitsPerCryHistEEM->Write();
  hitsPerCryMapEEM->Write();
  ampProfileEEP->Write();
  ampProfileMapEEP->Write();
  ampProfileEEM->Write();
  ampProfileMapEEM->Write();
  
  //cout << "All done!  Close input." << endl;
  f->Close();
  //cout << "Close output and quit!" << endl;
  outfile->Close();
  cout << "done." << endl;
}


