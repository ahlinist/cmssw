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
  using namespace std;

  char* infile = argv[1];
  if (!infile)
  {
    cout << " No input file specified !" << endl;
    return -1;
  }

  TFile* f = new TFile(infile);
  TTree* myTree = (TTree*)f->Get("eventTimingInfoTree");
  if(!myTree)
  {
    std::cout << "Couldn't find tree eventTimingInfoTree" << std::endl;
    return -1;
  }
  
  SetEStyle();
  // Ignore warnings
  gErrorIgnoreLevel = 2001;

  int numEBcrys;
  int numEEcrys;
  int hashedIndicesEB[61200];
  int hashedIndicesEE[14648];
  float cryTimesEB[61200];
  float cryTimesEE[14648];
  float cryTimeErrorsEB[61200];
  float cryTimeErrorsEE[14648];
  float cryAmpsEB[61200];
  float cryAmpsEE[14648];
  float correctionToSample5EB;
  float correctionToSample5EEP;
  float correctionToSample5EEM;
  myTree->SetBranchAddress("numberOfEBcrys",&numEBcrys);
  myTree->SetBranchAddress("numberOfEEcrys",&numEEcrys);
  myTree->SetBranchAddress("crystalHashedIndicesEB",hashedIndicesEB);
  myTree->SetBranchAddress("crystalHashedIndicesEE",hashedIndicesEE);
  myTree->SetBranchAddress("crystalTimesEB",cryTimesEB);
  myTree->SetBranchAddress("crystalTimesEE",cryTimesEE);
  myTree->SetBranchAddress("crystalTimeErrorsEB",cryTimeErrorsEB);
  myTree->SetBranchAddress("crystalTimeErrorsEE",cryTimeErrorsEE);
  myTree->SetBranchAddress("crystalAmplitudesEB",cryAmpsEB);
  myTree->SetBranchAddress("crystalAmplitudesEE",cryAmpsEE);
  myTree->SetBranchAddress("correctionToSampleEB",&correctionToSample5EB);
  myTree->SetBranchAddress("correctionToSampleEEM",&correctionToSample5EEM);
  myTree->SetBranchAddress("correctionToSampleEEP",&correctionToSample5EEP);

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
  
  cout << "Making calibs...";

  CrystalCalibration* eeCryCalibs[14648];
  //XXX: Making calibs with weighted/unweighted mean
  //XXX: Turn weighed mean back on
  for(int i=0; i < 14648; ++i)
    eeCryCalibs[i] = new CrystalCalibration(); //use weighted mean!
    //eeCryCalibs[i] = new CrystalCalibration(false); //don't use weighted mean!

  // Loop over the TTree
  int nEntries = myTree->GetEntries();

  for(int entry = 0; entry < nEntries; ++entry)
  {
    myTree->GetEntry(entry);
    //debug
    //cout << "Number of EE crys in event: " << numEEcrys << endl;
    // Loop once to calculate average event time
    float sumTime = 0;
    for(int cryIndex=0; cryIndex < numEEcrys; ++cryIndex)
    {
      float cryTime = cryTimesEE[cryIndex];
      sumTime+=cryTime;
    }
    if(sumTime/numEEcrys > 5.21 || sumTime/numEEcrys < 5.19)
    {
      cout << "Average event time: " << sumTime/numEEcrys  << " so event rejected." << endl;
      continue;
    }

    // Loop over the EE crys and fill the map
    for(int cryIndex=0; cryIndex < numEEcrys; ++cryIndex)
    {
      int hashedIndex = hashedIndicesEE[cryIndex];
      float cryTime = 25*(cryTimesEE[cryIndex]-5);
      float cryTimeError = 25*cryTimeErrorsEE[cryIndex];
      float cryAmp = cryAmpsEE[cryIndex];
      
      EEDetId det = EEDetId::unhashIndex(hashedIndex);
      if(det==EEDetId())
        continue;
      //ebCryCalibs[hashedIndex]->insertEvent(cryAmp,cryTime,cryTimeError,true);
      //XXX: SIC Turn this back on when we're done with the fitter
      eeCryCalibs[hashedIndex]->insertEvent(cryAmp,cryTime,cryTimeError,true);
      //eeCryCalibs[hashedIndex]->insertEvent(cryAmp,cryTime,35/(cryAmp/2.0),true);
      //debug
      //cout << "event inserted for cry with hash: " << hashedIndex << " det: "
      //  << det << endl;
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

  //Loop over all the crys
  for(int hashedIndex=0; hashedIndex < 14648; ++hashedIndex)
  {
    EEDetId det = EEDetId::unhashIndex(hashedIndex);
    if(det==EEDetId())
      continue;
    CrystalCalibration cryCalib = *(eeCryCalibs[hashedIndex]);
    int x = det.ix();
    int y = det.iy();

    //debug
    //cout << "Making calibs for cry" << hashedIndex << " : " << det 
    //  << " with entries: " << cryCalib.timingEvents.size() << endl;

    //chiSquaredTotalHist->Fill(cryCalib.totalChi2);
    //expectedStatPresHistEEM->Fill(sqrt(1/expectedPresSumEEM));
    //expectedStatPresVsObservedMeanErrHistEEM->Fill(sigmaM,sqrt(1/expectedPresSumEEM));

    //Filter events at default 0.5*sigma threshold
    //XXX: SIC Oct 2 2009 testing -- turn it off
    cryCalib.filterOutliers();
    //cout << "Filtering of outlier events enabled." << endl;

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
    
    // Make timing calibs
    float p1 = cryCalib.mean;
    float p1err = cryCalib.sigma;
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

  //cout << "All done!  Close input." << endl;
  f->Close();
  //cout << "Close output and quit!" << endl;
  outfile->Close();
  cout << "done." << endl;
}


