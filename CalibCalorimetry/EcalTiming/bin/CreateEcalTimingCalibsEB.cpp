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

  string fileNameBeg = "timingCalibsEB";
  
  string rootFilename = fileNameBeg+".root";
  TFile* outfile = new TFile(rootFilename.c_str(),"RECREATE");
  outfile->cd();

  TH1F* calibHistEB = new TH1F("timingCalibsEB","timingCalibs EB [ns]",2000,-100,100);
  TH1F* calibErrorHistEB = new TH1F("timingCalibErrorEB","timingCalibError EB [ns]",500,0,5);
  calibHistEB->Sumw2();
  calibErrorHistEB->Sumw2();

  TH2F* calibsVsErrors = new TH2F("timingCalibsAndErrors","TimingCalibs vs. errors [ns]",500,0,5,100,0,10);
  calibsVsErrors->Sumw2();

  TH1F* expectedStatPresHistEB = new TH1F("expectedStatPresEB","Avg. expected statistical precision EB [ns], all crys",200,0,2);
  TH2F* expectedStatPresVsObservedMeanErrHistEB = new TH2F("expectedStatPresVsObsEB","Expected stat. pres. vs. obs. error on mean each event EB [ns]",200,0,2,200,0,2);
  TH1F* expectedStatPresEachEventHistEB = new TH1F("expectedStatPresSingleEventEB","Expected stat. pres. each event EB [ns]",200,0,2);
  
  TH2F* errorOnMeanVsNumEvtsHist = new TH2F("errorOnMeanVsNumEvts","Error_on_mean vs. number of events",50,0,50,200,0,2);
  errorOnMeanVsNumEvtsHist->Sumw2();

  //=============Special Bins for TT and Modules borders=============================
  double ttEtaBins[36] = {-85, -80, -75, -70, -65, -60, -55, -50, -45, -40, -35, -30, -25, -20, -15, -10, -5, 0, 1, 6, 11, 16, 21, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 81, 86 };
  // double modEtaBins[10]={-85, -65, -45, -25, 0, 1, 26, 46, 66, 86};
  double ttPhiBins[73];
  double modPhiBins[19];
  double timingBins[79];
  double highEBins[11];
  for (int i = 0; i < 79; ++i)
  {
    timingBins[i]=-7.+double(i)*14./78.;
    if (i<73)
    {
      ttPhiBins[i]=1+5*i;
      if ( i < 19) 
      {
        modPhiBins[i]=1+20*i;
        if (i < 11)
        {
          highEBins[i]=10.+double(i)*20.;
        }
      }
    }

  } 
  TH2F* calibMapEB = new TH2F("calibMapEB","time calib map EB [ns];i#phi;i#eta",360,1.,361.,172,-86,86);
  calibMapEB->Sumw2();
  TProfile2D* calibTTMapEB = new TProfile2D("calibTTMapEB","time calib map EB (TT) [ns];i#phi;i#eta",360/5,ttPhiBins,35, ttEtaBins);

  TDirectory* cryDirEB = gDirectory->mkdir("crystalTimingHistsEB");
  cryDirEB->cd();
  TH1C* cryTimingHistsEB[61200];
  EBDetId det;
  for(int hi=0; hi < 61200; ++hi)
  {
    det = EBDetId::unhashIndex(hi);
    if(det==EBDetId())
      continue;
    string histname = "EB_cryTiming_ieta";
    histname+=intToString(det.ieta());
    histname+="_iphi";
    histname+=intToString(det.iphi());
    cryTimingHistsEB[hi] = new TH1C(histname.c_str(),histname.c_str(),660,-33,33);
    cryTimingHistsEB[hi]->Sumw2();
  }
  outfile->cd();
  
  cout << "Making calibs...";

  CrystalCalibration* ebCryCalibs[61200];
  //XXX: Making calibs with weighted/unweighted mean
  //XXX: Turn weighed mean back on
  for(int i=0; i < 61200; ++i)
    ebCryCalibs[i] = new CrystalCalibration(); //use weighted mean!
    //ebCryCalibs[i] = new CrystalCalibration(false); //don't use weighted mean!

  // Loop over the TTree
  int nEntries = myTree->GetEntries();

  for(int entry = 0; entry < nEntries; ++entry)
  {
    myTree->GetEntry(entry);
    //debug
    //cout << "Number of EB crys in event: " << numEBcrys << endl;

    // Loop once to calculate average event time
    float sumTime = 0;
    for(int cryIndex=0; cryIndex < numEBcrys; ++cryIndex)
    {
      float cryTime = cryTimesEB[cryIndex];
      sumTime+=cryTime;
    }
    if(sumTime/numEBcrys > 5.028 || sumTime/numEBcrys < 5.008)
    {
      cout << "Average event time: " << sumTime/numEBcrys  << " so event rejected." << endl;
      continue;
    }

    // Loop over the EB crys and fill the map
    for(int cryIndex=0; cryIndex < numEBcrys; ++cryIndex)
    {
      int hashedIndex = hashedIndicesEB[cryIndex];
      float cryTime = 25*(cryTimesEB[cryIndex]-5);
      float cryTimeError = 25*cryTimeErrorsEB[cryIndex];
      float cryAmp = cryAmpsEB[cryIndex];
      
      EBDetId det = EBDetId::unhashIndex(hashedIndex);
      if(det==EBDetId())
        continue;
      //XXX: SIC Turn this back on when we're done with the fitter
      ebCryCalibs[hashedIndex]->insertEvent(cryAmp,cryTime,cryTimeError,false);
      //ebCryCalibs[hashedIndex]->insertEvent(cryAmp,cryTime,35/(cryAmp/1.2),false);
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

  //Loop over all the crys
  for(int hashedIndex=0; hashedIndex < 61200; ++hashedIndex)
      
  {
    EBDetId det = EBDetId::unhashIndex(hashedIndex);
    if(det==EBDetId())
      continue;
    CrystalCalibration cryCalib = *(ebCryCalibs[hashedIndex]);
    int ieta = det.ieta();
    int iphi = det.iphi();

    //chiSquaredTotalHist->Fill(cryCalib.totalChi2);
    //expectedStatPresHistEB->Fill(sqrt(1/expectedPresSumEB));
    //expectedStatPresVsObservedMeanErrHistEB->Fill(sigmaM,sqrt(1/expectedPresSumEB));

    //Filter events at default 0.5*sigma threshold
    //XXX: SIC Oct. 2 2009 testing -- turn it off
    cryCalib.filterOutliers();
    //cout << "Filtering of outlier events enabled." << endl;
    
    //numPointsErasedHist->Fill(numPointsErased);
    
    //Write cryTimingHists
    vector<TimingEvent> times = cryCalib.timingEvents;
    for(vector<TimingEvent>::const_iterator timeItr = times.begin();
        timeItr != times.end(); ++timeItr)
    {
      cryTimingHistsEB[hashedIndex]->Fill(timeItr->time);
      cryTimingHistsEB[hashedIndex]->SetBinError(cryTimingHistsEB[hashedIndex]->FindBin(timeItr->time),timeItr->sigmaTime);
    }
    cryDirEB->cd();
    cryTimingHistsEB[hashedIndex]->Write();
    outfile->cd();
    
    // Make timing calibs
    float p1 = cryCalib.mean;
    float p1err = cryCalib.sigma;
    //XXX: SIC Oct. 2 2009 hardcoding sigma for testing...
    //XXX: Turn it back on
    //float p1err = 0.1;
    if(cryCalib.timingEvents.size() < 5)
    {
      fileStreamProb << "Cry (only " << cryCalib.timingEvents.size() << " events): " << ieta <<", " << iphi << ", hash: "
                                                                        << hashedIndex
                                                                          << "\t Calib: " << p1 << "\t Error: " << p1err << std::endl;
      continue;
    }
    // Make it so we can add calib to reco time
    p1*=-1;
    if(p1err < 0.5 && p1err > 0)
    {
      fileStream << "EB\t" << hashedIndex << "\t" << p1 << "\t\t" << p1err << endl;
      calibHistEB->Fill(p1);
      //calibMapEEMFlip->Fill(y-85,x+1,p1);
      calibMapEB->Fill(iphi,ieta,p1);
      calibTTMapEB->Fill(iphi,ieta,p1);
      //calibMapEEMPhase->Fill(x+1,y-85,p1/25-floor(p1/25));
      //errorOnMeanVsNumEvtsHist->Fill(times.size(),p1err);
      
      //Store in timeCalibration container
      EcalTimeCalibConstant tcConstant = p1;
      EcalTimeCalibError tcError = p1err;
      timeCalibConstants[det.rawId()] = tcConstant;
      timeCalibErrors[det.rawId()] = tcError;

    }
    else
    {
      //std::cout << "Cry: " << ieta <<", " << iphi << ", hash: " << itr->first
      //  << "\t Calib: " << p1 << "\t Error: " << p1err << std::endl;
      fileStreamProb << "Cry: " << ieta <<", " << iphi << ", hash: " << hashedIndex
        << "\t Calib: " << p1 << "\t Error: " << p1err << std::endl;
    }
    //calibsVsErrorsEB->Fill(p1err, p1 > 0 ? p1 : -1*p1);
    calibErrorHistEB->Fill(p1err);
  }
  
  fileStream.close();
  fileStreamProb.close();

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
  calibHistEB->SetXTitle("timingCalib [ns]");
  calibHistEB->Write();
  calibErrorHistEB->SetXTitle("uncertainty on mean [ns]");
  calibErrorHistEB->Write();
  //eventsEBHist->Write();

  //can->Print("calibs1D.png");
  //cout << "Writing calibVsErrors" << endl;
  //calibsVsErrors->SetYTitle("AbsCalibConst");
  //calibsVsErrors->SetXTitle("calibConstError");
  //calibsVsErrors->Write();

  //cout << "Writing calibErrorHists" << endl;
  //calibErrorHistEB->Write();

  //cout << "Writing calib maps" << endl;
  calibMapEB->Write();
  calibTTMapEB->Write();
  //calibMapEBFlip->SetXTitle("ieta");
  //calibMapEBFlip->SetYTitle("iphi");
  //calibMapEBFlip->Write();
  //calibMapEBPhase->SetXTitle("iphi");
  //calibMapEBPhase->SetYTitle("ieta");
  //calibMapEBPhase->Write();
  
  //Move empty bins out of the way
  //int nxbins = calibMapEEM->GetNbinsX();
  //int nybins = calibMapEEM->GetNbinsY();
  //for(int i=0;i<=(nxbins+2)*(nybins+2); ++i)
  //{
  //  double binentsM = calibMapEEM->GetBinContent(i);
  //  if(binentsM==0)
  //  {
  //    calibMapEEM->SetBinContent(i,-1000);
  //  }
  //  double binentsP = calibMapEEP->GetBinContent(i);
  //  if(binentsP==0)
  //  {
  //    calibMapEEP->SetBinContent(i,-1000);
  //  }
  //}
  //calibMapEEM->SetXTitle("ix");
  //calibMapEEM->SetYTitle("iy");
  //calibMapEEM->Write();
  //calibMapEEP->SetXTitle("ix");
  //calibMapEEP->SetYTitle("iy");
  //calibMapEEP->Write();

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

  expectedStatPresHistEB->Write();
  expectedStatPresVsObservedMeanErrHistEB->Write();
  expectedStatPresEachEventHistEB->Write();
  //ampEachEventHist->Write();
  //numPointsErasedHist->Write();

  //calibMapEtaAvgEB->SetXTitle("i#phi");
  //calibMapEtaAvgEB->SetYTitle("i#eta");
  //calibMapEtaAvgEB->Write();
  //calibHistEtaAvgEB->Write();
  
  //cout << "All done!  Close input." << endl;
  f->Close();
  //cout << "Close output and quit!" << endl;
  outfile->Close();
  cout << "done." << endl;
}


