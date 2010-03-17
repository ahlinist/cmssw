int Wait() {
     cout << " Continue [<RET>|q]?  "; 
     char x;
     x = getchar();
     if ((x == 'q') || (x == 'Q')) return 1;
     return 0;
}
  
struct TTreeMembers {
  int numEBcrys_;
  int numEEcrys_;
  int cryHashesEB_[61200];
  int cryHashesEE_[14648];
  float cryTimesEB_[61200];
  float cryTimesEE_[14648];
  float cryTimeErrorsEB_[61200];
  float cryTimeErrorsEE_[14648];
  float cryAmpsEB_[61200];
  float cryAmpsEE_[14648];
  int numTriggers_;
  int numTechTriggers_;
  int triggers_[200];
  int techtriggers_[200];
  float absTime_;
  int lumiSection_;
  int bx_;
  int orbit_;
  float correctionToSample5EB_;
  float correctionToSample5EEP_;
  float correctionToSample5EEM_;
} TTreeMembers_;


//gSystem->Load("libFWCoreFWLite.so");
//AutoLibraryLoader::enable();
//gSystem->Load("libDataFormatsFWLite.so");
//gSystem->Load("libDataFormatsPatCandidates.so");
#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#endif
EBDetId mydet = 0;
EEDetId mydete = 0;



double DrawLaserPlots(Char_t* infile = 0, Int_t runNum=0, Bool_t printPics = kTRUE, Char_t* fileType = "png", Char_t* dirName = ".", Bool_t doWait=kFALSE, Char_t* mType = "Laser")
{
 
  cout << "Loading FW Lite setup." << endl;

  gROOT->SetStyle("Plain");
  gStyle->SetNumberContours(99);
  gStyle->SetPalette(1,0); gStyle->SetOptStat(10);

  if (!infile) {
    cout << " No input file specified !" << endl;
    return;
  }
 
  cout << "Producing Laser plots for: " << infile << endl;

  TFile* f = new TFile(infile);
  f->cd(); //added by jason for completeness

  int runNumber = 0;
  runNumber = runNum;
  bool fit = true;

  char name[100];  
  char mytitle[200];

  const int nHists1=120;
  const int nHists = nHists1;
  //  const int nHists = 9;
  cout << nHists1 << " " << nHists << endl;;

  TCanvas* c[nHists];
  char cname[100]; 

  for (int i=0; i<nHists1; i++) {
    sprintf(cname,"c%i",i);
    int x = (i%3)*600;     //int x = (i%3)*600;
    int y = (i/3)*100;     //int y = (i/3)*200;
    c[i] =  new TCanvas(cname,cname,x,y,900,600);
    cout << "Hists1 " << i << " : " << x << " , " << y << endl;
  }

  char runChar[50];
  sprintf(runChar,"Run %i ",runNumber);
  
  //TTree helpers
  int tbins = 52;
  double tbinsL = -26.;
  double tbinsH = 26.;
  
  double thigh = 36.;
  double tlow  = -36.;
  
  double thighc = 50.;
  double tlowc  = -50.;
  
  double thight = 30.;
  double tlowt  = -30.;
  
//First thing is do print the profiles

  //Timing by FED/SM  
  c[0]->cd();
  gStyle->SetOptStat(10);
  TProfile *SM_timing = (TProfile*) f->Get("SM_timing");
  customizeTProfile(SM_timing);
  SM_timing->Draw();
   sprintf(mytitle,"%s %s",runChar,SM_timing->GetTitle()); 
  SM_timing->SetTitle(mytitle);

  c[1]->cd();
  gStyle->SetOptStat(10);
  TH1F *SM_timingh = CorrectProfToHist(SM_timing,"SM_timingh",-5,25.0);
  customizeTHist(SM_timingh);
  SM_timingh->Draw("p");
   sprintf(mytitle,"%s %s ;FED;Time (ns)",runChar,SM_timing->GetTitle()); 
  SM_timingh->SetMinimum(tlowt);
  SM_timingh->SetMaximum(thight);
  SM_timingh->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_SM_timingCorrected_%i.%s",dirName,mType,runNumber,fileType); c[1]->Print(name); }

  //Timing by LM
  c[2]->cd();
  gStyle->SetOptStat(10);
  TProfile *LM_timing = (TProfile*) f->Get("LM_timing");
  customizeTProfile(LM_timing);
  LM_timing->Draw();
   sprintf(mytitle,"%s %s",runChar,LM_timing->GetTitle()); 
  LM_timing->SetTitle(mytitle);

  c[3]->cd();
  gStyle->SetOptStat(10);
  TH1F *LM_timingh = CorrectProfToHist(LM_timing,"LM_timingh",-5,25.0);
  customizeTHist(LM_timingh);
  LM_timingh->Draw("p");
   sprintf(mytitle,"%s %s ;FED;Time (ns)",runChar,LM_timing->GetTitle()); 
  LM_timingh->SetMinimum(tlowt);
  LM_timingh->SetMaximum(thight);
  LM_timingh->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_LM_timingCorrected_%i.%s",dirName,mType,runNumber,fileType); c[3]->Print(name); }
 

  //Timing within the towers
  c[4]->cd();
  gStyle->SetOptStat(1111);
  TProfile *Inside_TT_timing = (TProfile*) f->Get("Inside_TT_timing");
  Inside_TT_timing->Draw();
   sprintf(mytitle,"%s %s",runChar,Inside_TT_timing->GetTitle()); 
  Inside_TT_timing->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_Inside_TT_timing_%i.%s",dirName,mType,runNumber,fileType); c[4]->Print(name); }
 
  //Eta Profiles by TT
  c[5]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeTTAllFEDsEta = (TProfile*) f->Get("timeTTAllFEDsEta");
  timeTTAllFEDsEta->Draw();
  //timeTTAllFEDsEta->SetMinimum(4.95);
  //timeTTAllFEDsEta->SetMaximum(5.05);
   sprintf(mytitle,"%s %s",runChar,timeTTAllFEDsEta->GetTitle()); 
  timeTTAllFEDsEta->SetTitle(mytitle);
   
  c[6]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeTTAllFEDsEtaEEP = (TProfile*) f->Get("timeTTAllFEDsEtaEEP");
  timeTTAllFEDsEtaEEP->Draw();
  //timeTTAllFEDsEtaEEP->SetMinimum(4.5);
  //timeTTAllFEDsEtaEEP->SetMaximum(5.5);
   sprintf(mytitle,"%s %s",runChar,timeTTAllFEDsEtaEEP->GetTitle()); 
  timeTTAllFEDsEtaEEP->SetTitle(mytitle);
  
  c[7]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeTTAllFEDsEtaEEM = (TProfile*) f->Get("timeTTAllFEDsEtaEEM");
  timeTTAllFEDsEtaEEM->Draw();
  //timeTTAllFEDsEtaEEM->SetMinimum(4.5);
  //timeTTAllFEDsEtaEEM->SetMaximum(5.5);
   sprintf(mytitle,"%s %s",runChar,timeTTAllFEDsEtaEEM->GetTitle()); 
  timeTTAllFEDsEtaEEM->SetTitle(mytitle);
  
  //Eta profile by Ch
  c[8]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeCHAllFEDsEta = (TProfile*) f->Get("timeCHAllFEDsEta");
  timeCHAllFEDsEta->Draw();
  //timeCHAllFEDsEta->SetMinimum(4.8);
  //timeCHAllFEDsEta->SetMaximum(5.2);
 
//1-D Histograms
  c[9]->cd();
  gStyle->SetOptStat(111110);
  TH1F *Rel_TimingSigma = (TH1F*) f->Get("Rel_TimingSigma");
  Rel_TimingSigma->Draw();
   sprintf(mytitle,"%s %s",runChar,Rel_TimingSigma->GetTitle()); 
  Rel_TimingSigma->SetTitle(mytitle);
  c[9]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_Rel_TimingSigma_%i.%s",dirName,mType,runNumber,fileType); c[9]->Print(name); }
  
  c[10]->cd();
  gStyle->SetOptStat(111110);
  TH1F *XtalsPerEvt = (TH1F*) f->Get("XtalsPerEvt");
  XtalsPerEvt->Draw();
  //XtalsPerEvt->GetXaxis()->SetRangeUser(0,100);
   sprintf(mytitle,"%s %s",runChar,XtalsPerEvt->GetTitle()); 
  XtalsPerEvt->SetTitle(mytitle);
  //c[10]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_XtalsPerEvt_%i.%s",dirName,mType,runNumber,fileType); c[10]->Print(name); }
  
  c[11]->cd();
  gStyle->SetOptStat(111110);
  TH1F *laserShift = (TH1F*) f->Get("laserShift");
  laserShift->Draw();
   sprintf(mytitle,"%s %s",runChar,laserShift->GetTitle()); 
  laserShift->SetTitle(mytitle);
  c[11]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_laserShift_%i.%s",dirName,mType,runNumber,fileType); c[11]->Print(name); }
  
//2-D Histogram
  c[12]->cd();
  gStyle->SetOptStat(111110);
  TH2F *RelRMS_vs_AbsTime = (TH2F*) f->Get("RelRMS_vs_AbsTime");
  RelRMS_vs_AbsTime->Draw("colz");
   sprintf(mytitle,"%s %s",runChar,RelRMS_vs_AbsTime->GetTitle()); 
  RelRMS_vs_AbsTime->SetTitle(mytitle);
  c[12]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_RelRMS_vs_AbsTime_%i.%s",dirName,mType,runNumber,fileType); c[12]->Print(name); }
  
//1-D TGraphs  
  c[13]->cd();
  gStyle->SetOptStat(111110);
  TGraph *TTMeanWithRMS_All_FEDS = (TGraph*) f->Get("TTMeanWithRMS_All_FEDS");
   sprintf(mytitle,"%s %s",runChar,TTMeanWithRMS_All_FEDS->GetTitle()); 
  TTMeanWithRMS_All_FEDS->SetTitle(mytitle);
  TTMeanWithRMS_All_FEDS->GetYaxis()->SetLimits(5.,6.);
  //TTMeanWithRMS_All_FEDS->GetYaxis()->SetLimits(4.,6.);
  TTMeanWithRMS_All_FEDS->GetYaxis()->UnZoom();
  TTMeanWithRMS_All_FEDS->Draw("AP*");
  //c[13]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TTMeanWithRMS_All_FEDS_%i.%s",dirName,mType,runNumber,fileType); c[13]->Print(name); }
  
  c[14]->cd();
  gStyle->SetOptStat(111110);
  TGraph *TTMeanWithRMS_All_FEDS_CHANGED = (TGraph*) f->Get("TTMeanWithRMS_All_FEDS_CHANGED");
   sprintf(mytitle,"%s %s",runChar,TTMeanWithRMS_All_FEDS_CHANGED->GetTitle()); 
  TTMeanWithRMS_All_FEDS_CHANGED->SetTitle(mytitle);
  TTMeanWithRMS_All_FEDS_CHANGED->GetYaxis()->SetLimits(-5.,5.);
  //TTMeanWithRMS_All_FEDS_CHANGED->GetYaxis()->SetLimits(4.,6.);
  TTMeanWithRMS_All_FEDS_CHANGED->GetYaxis()->UnZoom();
  TTMeanWithRMS_All_FEDS_CHANGED->Draw("AP*");
  //c[13]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TTMeanWithRMS_All_FEDS_Corrected_%i.%s",dirName,mType,runNumber,fileType); c[14]->Print(name); }
  
//2-D TGraphs/Profiles 
  //Ch by Ch timing profile
  c[15]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *timeCHProfile = (TProfile2D*) f->Get("timeCHProfile");
  timeCHProfile->Draw("colz");
   sprintf(mytitle,"%s %s",runChar,timeCHProfile->GetTitle()); 
  timeCHProfile->SetTitle(mytitle);

  c[15]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *timeCHProfilep = TProfToRelProf2D(timeCHProfile,"timeCHProfilep", -5., 25.);
  timeCHProfilep->Draw("colz");
   sprintf(mytitle,"%s in ns",timeCHProfile->GetTitle()); 
  timeCHProfilep->SetTitle(mytitle);
  
  c[39]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *timeCHProfileO = TProfile2DOccupancyFromProf2D(timeCHProfile,"timeCHProfileO");
  timeCHProfileO->Draw("colz");
   sprintf(mytitle,"CH occupancy"); 
  timeCHProfileO->SetTitle(mytitle);
  timeCHProfileO->SetMinimum(1.);
  timeCHProfileO->GetXaxis()->SetNdivisions(-18);
  timeCHProfileO->GetYaxis()->SetNdivisions(2);
  c[39]->SetLogy(0);
  c[39]->SetLogz(1);
  c[39]->SetGridx(1);
  c[39]->SetGridy(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_OccuCHProfile_%i.%s",dirName,mType,runNumber,fileType); c[39]->Print(name); }
  
  c[16]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *timeTTProfile = (TProfile2D*) f->Get("timeTTProfile");
  timeTTProfile->Draw("colz");
   sprintf(mytitle,"%s %s",runChar,timeTTProfile->GetTitle()); 
  timeTTProfile->SetTitle(mytitle);
  timeTTProfile->SetMinimum(4.0);
  //timeTTProfile->SetMinimum(5.85);
  //timeTTProfile->SetMinimum(4.8);
  //timeTTProfile->SetMaximum(6.45);
  timeTTProfile->GetXaxis()->SetNdivisions(-18);
  timeTTProfile->GetYaxis()->SetNdivisions(2);

  c[16]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *timeTTProfilep = TProfToRelProf2D(timeTTProfile,"timeTTProfilep", -5., 25.);
  timeTTProfilep->SetContour(10);
  timeTTProfilep->Draw("colz");
   sprintf(mytitle,"%s in ns",timeTTProfile->GetTitle()); 
  timeTTProfilep->SetTitle(mytitle);
  timeTTProfilep->SetMinimum(tlow);
  timeTTProfilep->SetMaximum(thigh);


  //timeTTProfile->SetMinimum(5.85);
  //timeTTProfile->SetMinimum(4.8);
  //timeTTProfile->SetMaximum(6.45);
  timeTTProfilep->GetXaxis()->SetNdivisions(-18);
  timeTTProfilep->GetYaxis()->SetNdivisions(2);
  c[16]->SetLogy(0);
  c[16]->SetLogz(0);
  c[16]->SetGridx(1);
  c[16]->SetGridy(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeTTProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[16]->Print(name); }

  c[40]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *timeTTProfileO = TProfile2DOccupancyFromProf2D(timeTTProfile,"timeTTProfileO");
  timeTTProfileO->Draw("colz");
   sprintf(mytitle,"TT occupancy"); 
  timeTTProfileO->SetTitle(mytitle);
  timeTTProfileO->SetMinimum(1.);
  timeTTProfileO->GetXaxis()->SetNdivisions(-18);
  timeTTProfileO->GetYaxis()->SetNdivisions(2);
  c[40]->SetLogy(0);
  c[40]->SetLogz(1);
  c[40]->SetGridx(1);
  c[40]->SetGridy(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_OccuTTProfile_%i.%s",dirName,mType,runNumber,fileType); c[40]->Print(name); }
  //Ch by Ch timing profile EE+
  c[17]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeCHProfile = (TProfile2D*) f->Get("EEPtimeCHProfile");
  EEPtimeCHProfile->Draw("colz");
   sprintf(mytitle,"%s %s",runChar,EEPtimeCHProfile->GetTitle()); 
  EEPtimeCHProfile->SetTitle(mytitle);
  EEPtimeCHProfile->SetMinimum(4.0);
  //EEPtimeCHProfile->SetMinimum(4.5);
  //EEPtimeCHProfile->SetMaximum(5.5);
  EEPtimeCHProfile->GetXaxis()->SetNdivisions(18);

  c[17]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeCHProfilep = TProfToRelProf2D(EEPtimeCHProfile,"EEPtimeCHProfilep", -5., 25.);
  EEPtimeCHProfilep->Draw("colz");
   sprintf(mytitle,"%s in ns",EEPtimeCHProfilep->GetTitle()); 
  EEPtimeCHProfilep->SetTitle(mytitle);
  EEPtimeCHProfilep->SetMinimum(tlowc);
  EEPtimeCHProfilep->SetMaximum(thighc);
  //EEPtimeCHProfile->SetMinimum(4.5);
  //EEPtimeCHProfile->SetMaximum(5.5);
  EEPtimeCHProfilep->GetXaxis()->SetNdivisions(18);
  c[17]->SetLogy(0);
  c[17]->SetLogz(0);
  drawEELines();
  //c[15]->SetLogz(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPtimeCHProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[17]->Print(name); }
 
  c[41]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeCHProfileO = TProfile2DOccupancyFromProf2D(EEPtimeCHProfile,"EEPtimeCHProfileO");
  EEPtimeCHProfileO->Draw("colz");
   sprintf(mytitle,"CH occupancy"); 
  EEPtimeCHProfileO->SetTitle(mytitle);
  EEPtimeCHProfileO->SetMinimum(1.);
  c[41]->SetLogy(0);
  c[41]->SetLogz(1);
  drawEELines();
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPOccuCHProfile_%i.%s",dirName,mType,runNumber,fileType); c[41]->Print(name); }
  //Ch by Ch timing profile EE+
  c[18]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeCHProfile = (TProfile2D*) f->Get("EEMtimeCHProfile");
  EEMtimeCHProfile->Draw("colz");
   sprintf(mytitle,"%s %s",runChar,EEMtimeCHProfile->GetTitle()); 
  EEMtimeCHProfile->SetTitle(mytitle);
  EEMtimeCHProfile->SetMinimum(4.);
  //EEMtimeCHProfile->SetMinimum(4.5);
  //EEMtimeCHProfile->SetMaximum(5.5);
  EEMtimeCHProfile->GetXaxis()->SetNdivisions(18);

 c[18]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeCHProfilep = TProfToRelProf2D(EEMtimeCHProfile,"EEMtimeCHProfilep", -5., 25.);
  EEMtimeCHProfilep->Draw("colz");
   sprintf(mytitle,"%s in ns",EEMtimeCHProfilep->GetTitle()); 
  EEMtimeCHProfilep->SetTitle(mytitle);
  EEMtimeCHProfilep->SetMinimum(tlowc);
  EEMtimeCHProfilep->SetMaximum(thighc);
  EEMtimeCHProfilep->GetXaxis()->SetNdivisions(18);
  c[18]->SetLogy(0);
  c[18]->SetLogz(0);
  drawEELines();
  //c[15]->SetLogz(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMtimeCHProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[18]->Print(name); } 
  
  c[42]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeCHProfileO = TProfile2DOccupancyFromProf2D(EEMtimeCHProfile,"EEMtimeCHProfileO");
  EEMtimeCHProfileO->Draw("colz");
   sprintf(mytitle,"CH occupancy"); 
  EEMtimeCHProfileO->SetTitle(mytitle);
  EEMtimeCHProfileO->SetMinimum(1.);
  c[42]->SetLogy(0);
  c[42]->SetLogz(1);
  drawEELines();
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMOccuCHProfile_%i.%s",dirName,mType,runNumber,fileType); c[42]->Print(name); }
  //TT by TT timing profile EE+
  c[19]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeTTProfile = (TProfile2D*) f->Get("EEPtimeTTProfile");
  EEPtimeTTProfile->Draw("colz");
   sprintf(mytitle,"%s %s",runChar,EEPtimeTTProfile->GetTitle()); 
  EEPtimeTTProfile->SetTitle(mytitle);
  EEPtimeTTProfile->SetMinimum(4.);
  //EEPtimeTTProfile->SetMinimum(4.5);
  //EEPtimeTTProfile->SetMaximum(5.5);
  EEPtimeTTProfile->GetXaxis()->SetNdivisions(18);


  c[19]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeTTProfilep = TProfToRelProf2D(EEPtimeTTProfile,"EEPtimeTTProfilep", -5., 25.);
  EEPtimeTTProfilep->SetContour(10);
  EEPtimeTTProfilep->Draw("colz");
   sprintf(mytitle,"%s in ns",EEPtimeTTProfile->GetTitle()); 
  EEPtimeTTProfilep->SetTitle(mytitle);
  EEPtimeTTProfilep->SetMinimum(tlow);
  EEPtimeTTProfilep->SetMaximum(thigh);

  //timeTTProfile->SetMinimum(5.85);
  //timeTTProfile->SetMinimum(4.8);
  //timeTTProfile->SetMaximum(6.45);
  EEPtimeTTProfilep->GetXaxis()->SetNdivisions(18);
  //EEPtimeTTProfilep->GetYaxis()->SetNdivisions(2);
  drawEELines();
  c[19]->SetLogy(0);
  c[19]->SetLogz(0);
  // c[19]->SetGridx(1);
  //c[19]->SetGridy(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPtimeTTProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[19]->Print(name); }
  c[44]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeTTProfileO = TProfile2DOccupancyFromProf2D(EEPtimeTTProfile,"EEPtimeTTProfileO");
  EEPtimeTTProfileO->Draw("colz");
   sprintf(mytitle,"TT occupancy"); 
  EEPtimeTTProfileO->SetTitle(mytitle);
  EEPtimeTTProfileO->SetMinimum(1.);
  c[44]->SetLogy(0);
  c[44]->SetLogz(1);
  drawEELines();
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPOccuTTProfile_%i.%s",dirName,mType,runNumber,fileType); c[44]->Print(name); }
 
  //TT by TT timing profile EE-
  c[20]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeTTProfile = (TProfile2D*) f->Get("EEMtimeTTProfile");
  EEMtimeTTProfile->Draw("colz");
   sprintf(mytitle,"%s %s",runChar,EEMtimeTTProfile->GetTitle()); 
  EEMtimeTTProfile->SetTitle(mytitle);
  EEMtimeTTProfile->SetMinimum(4.);
  //EEMtimeTTProfile->SetMinimum(4.5);
  //EEMtimeTTProfile->SetMaximum(5.5);
  EEMtimeTTProfile->GetXaxis()->SetNdivisions(18);

  c[20]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeTTProfilep = TProfToRelProf2D(EEMtimeTTProfile,"EEMtimeTTProfilep", -5., 25.);
  
  EEMtimeTTProfilep->Draw("colz");
   sprintf(mytitle,"%s in ns",EEMtimeTTProfile->GetTitle()); 
  EEMtimeTTProfilep->SetTitle(mytitle);
  EEMtimeTTProfilep->SetContour(10);
  EEMtimeTTProfilep->SetMinimum(tlow);
  EEMtimeTTProfilep->SetMaximum(thigh);
  EEMtimeTTProfilep->GetXaxis()->SetNdivisions(18);
  c[20]->SetLogy(0);
  c[20]->SetLogz(0);
  drawEELines();
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMtimeTTProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[20]->Print(name); }
  c[43]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeTTProfileO = TProfile2DOccupancyFromProf2D(EEMtimeTTProfile,"EEMtimeTTProfileO");
  EEMtimeTTProfileO->Draw("colz");
   sprintf(mytitle,"TT occupancy"); 
  EEMtimeTTProfileO->SetTitle(mytitle);
  EEMtimeTTProfileO->SetMinimum(1.);
  c[43]->SetLogy(0);
  c[43]->SetLogz(1);
  drawEELines();
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMOccuTTProfile_%i.%s",dirName,mType,runNumber,fileType); c[43]->Print(name); }

  //Amplitude Profiles
  c[21]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *fullAmpProfileEB = (TProfile2D*) f->Get("fullAmpProfileEB");
  fullAmpProfileEB->Draw("colz");
   sprintf(mytitle,"%s %s",runChar,fullAmpProfileEB->GetTitle()); 
  fullAmpProfileEB->SetTitle(mytitle);
  if (fullAmpProfileEB->GetMaximum() > 0 ) {
     fullAmpProfileEB->SetMinimum(0.1);
     c[21]->SetLogy(0);
     c[21]->SetLogz(1);
  }
  if (printPics) { sprintf(name,"%s/%sAnalysis_fullAmpProfileEB_%i.%s",dirName,mType,runNumber,fileType); c[21]->Print(name); }
  
  c[22]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *fullAmpProfileEEP = (TProfile2D*) f->Get("fullAmpProfileEEP");
  fullAmpProfileEEP->Draw("colz");
   sprintf(mytitle,"%s %s",runChar,fullAmpProfileEEP->GetTitle()); 
  fullAmpProfileEEP->SetTitle(mytitle);
  if (fullAmpProfileEEP->GetMaximum() > 0 ) {
     fullAmpProfileEEP->SetMinimum(0.1);
     c[22]->SetLogy(0);
     c[22]->SetLogz(1);
  }
  drawEELines();
  if (printPics) { sprintf(name,"%s/%sAnalysis_fullAmpProfileEEP_%i.%s",dirName,mType,runNumber,fileType); c[22]->Print(name); }
  
  c[23]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *fullAmpProfileEEM = (TProfile2D*) f->Get("fullAmpProfileEEM");
  fullAmpProfileEEM->Draw("colz");
   sprintf(mytitle,"%s %s",runChar,fullAmpProfileEEM->GetTitle()); 
  fullAmpProfileEEM->SetTitle(mytitle);
  if (fullAmpProfileEEM->GetMaximum() > 0 ) {
     fullAmpProfileEEM->SetMinimum(0.1);
     c[23]->SetLogy(0);
     c[23]->SetLogz(1);
  }
  drawEELines();
  if (printPics) { sprintf(name,"%s/%sAnalysis_fullAmpProfileEEM_%i.%s",dirName,mType,runNumber,fileType); c[23]->Print(name); }
   
   
  //Eta Profiles by TT Normalized
  c[24]->cd();
  TProfile *timeTTAllFEDsEtap =  TProfToRelProf(timeTTAllFEDsEta,"timeTTAllFEDsEtap",-5,25.);
  customizeTProfile(timeTTAllFEDsEtap);
  timeTTAllFEDsEtap->Draw("p");
  timeTTAllFEDsEtap->SetMinimum(tlowt);
  timeTTAllFEDsEtap->SetMaximum(thight);
   sprintf(mytitle,"%s ;i#eta;Time (ns)",timeTTAllFEDsEta->GetTitle()); 
  timeTTAllFEDsEtap->SetTitle(mytitle);
  gStyle->SetOptStat(100);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeTTAllFEDsEtaRel_%i.%s",dirName,mType,runNumber,fileType); c[24]->Print(name); }
  
  c[25]->cd();
  TProfile *timeTTAllFEDsEtaEEPp =  TProfToRelProf(timeTTAllFEDsEtaEEP,"timeTTAllFEDsEtaEEPp",-5,25.);
  customizeTProfile(timeTTAllFEDsEtaEEPp);
  timeTTAllFEDsEtaEEPp->Draw("p");
  timeTTAllFEDsEtaEEPp->SetMinimum(tlowt);
  timeTTAllFEDsEtaEEPp->SetMaximum(thight);
   sprintf(mytitle,"%s ;i#eta;Time (ns)",timeTTAllFEDsEtaEEP->GetTitle()); 
  timeTTAllFEDsEtaEEPp->SetTitle(mytitle);
  gStyle->SetOptStat(100);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeTTAllFEDsEtaEEPRel_%i.%s",dirName,mType,runNumber,fileType); c[25]->Print(name); }
  
  c[26]->cd();
  TProfile *timeTTAllFEDsEtaEEMp =  TProfToRelProf(timeTTAllFEDsEtaEEM,"timeTTAllFEDsEtaEEMp",-5,25.);
  customizeTProfile(timeTTAllFEDsEtaEEMp);
  timeTTAllFEDsEtaEEMp->Draw("p");
  timeTTAllFEDsEtaEEMp->SetMinimum(tlowt);
  timeTTAllFEDsEtaEEMp->SetMaximum(thight);
   sprintf(mytitle,"%s ;i#eta;Time (ns)",timeTTAllFEDsEtaEEM->GetTitle()); 
  timeTTAllFEDsEtaEEMp->SetTitle(mytitle);
  gStyle->SetOptStat(100);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeTTAllFEDsEtaEEMRel_%i.%s",dirName,mType,runNumber,fileType); c[26]->Print(name); }
  
  //Eta profile by Ch
  c[27]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeCHAllFEDsEta = (TProfile*) f->Get("timeCHAllFEDsEta");
  timeCHAllFEDsEta->Draw();
  TProfile *timeCHAllFEDsEtap =  TProfToRelProf(timeCHAllFEDsEta,"timeCHAllFEDsEtap",-5,25.);
  customizeTProfile(timeCHAllFEDsEtap);
  timeCHAllFEDsEtap->Draw("p");
  timeCHAllFEDsEtap->SetMinimum(tlowc);
  timeCHAllFEDsEtap->SetMaximum(thighc);
   sprintf(mytitle,"%s ;i#eta;Time (ns)",timeCHAllFEDsEta->GetTitle()); 
  timeCHAllFEDsEtap->SetTitle(mytitle);
  gStyle->SetOptStat(100);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeCHAllFEDsEtaRel_%i.%s",dirName,mType,runNumber,fileType); c[27]->Print(name); }
 
  //+_+_+_+_+_+_+_+_+__-----------------------------+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_
  //Now it is time to see if the timing tree is there and use the individual ntuple information

  char ebhashfilter[500];
  char eehashfilter[3000];
  sprintf(ebhashfilter,"crystalHashedIndicesEB != 25822 && crystalHashedIndicesEB != 32705 && crystalHashedIndicesEB != 56473");
  sprintf(eehashfilter,"crystalHashedIndicesEE != 11658 && crystalHashedIndicesEE != 11742 && crystalHashedIndicesEE != 10224 && crystalHashedIndicesEE != 10225 && crystalHashedIndicesEE != 10226 && crystalHashedIndicesEE != 10310 && crystalHashedIndicesEE != 10311 && crystalHashedIndicesEE != 10394 && crystalHashedIndicesEE != 10395 && crystalHashedIndicesEE != 10875 && crystalHashedIndicesEE != 11316 && crystalHashedIndicesEE != 11659 && crystalHashedIndicesEE != 11660 && crystalHashedIndicesEE != 11661 && crystalHashedIndicesEE != 11743  && crystalHashedIndicesEE != 11744 && crystalHashedIndicesEE != 11744 && crystalHashedIndicesEE != 11745 && crystalHashedIndicesEE != 11932 && crystalHashedIndicesEE != 11746 && crystalHashedIndicesEE != 12702 && crystalHashedIndicesEE != 4252 && crystalHashedIndicesEE != 4335 && crystalHashedIndicesEE != 4337 && crystalHashedIndicesEE != 4419 && crystalHashedIndicesEE != 4423 && crystalHashedIndicesEE != 4785 && crystalHashedIndicesEE != 6181 && crystalHashedIndicesEE != 14613 && crystalHashedIndicesEE != 13726 && crystalHashedIndicesEE != 13727 && crystalHashedIndicesEE != 7717 && crystalHashedIndicesEE != 7778 && crystalHashedIndicesEE != 4420 && crystalHashedIndicesEE != 4421 && crystalHashedIndicesEE != 4423 && crystalHashedIndicesEE != 2946 && crystalHashedIndicesEE != 2900 && crystalHashedIndicesEE != 2902 && crystalHashedIndicesEE != 2901 && crystalHashedIndicesEE != 2903 && crystalHashedIndicesEE != 2904 && crystalHashedIndicesEE != 2905 && crystalHashedIndicesEE != 2986 && crystalHashedIndicesEE != 2987 && crystalHashedIndicesEE != 2988 && crystalHashedIndicesEE != 2989 && crystalHashedIndicesEE != 3070 && crystalHashedIndicesEE != 3071 && crystalHashedIndicesEE != 4252 && crystalHashedIndicesEE != 4253 && crystalHashedIndicesEE != 4254 && crystalHashedIndicesEE != 4255 && crystalHashedIndicesEE != 4256");

  char ebtimefilter[100];
  char eetimefilter[100];
  sprintf(ebtimefilter,"(crystalTimeErrorsEB)*25. < 5.0");
  sprintf(eetimefilter,"(crystalTimeErrorsEE)*25. < 5.0");

  char ebfilter[500];
  char eefilter[3100];
  sprintf(ebfilter,"(%s) && (%s)",ebtimefilter,ebhashfilter);
  sprintf(eefilter,"(%s) && (%s) && crystalAmplitudesEE > 1.0",eetimefilter,eehashfilter);

  char eepfilter[3200];
  char eemfilter[3200];
  sprintf(eepfilter,"(%s) && (crystalHashedIndicesEE > 7341)",eefilter);
  sprintf(eemfilter,"(%s) && (crystalHashedIndicesEE < 7342)",eefilter);

  TTree* eventTimingInfoTree = ((TTree*) f->Get("eventTimingInfoTree"));
  if (!eventTimingInfoTree) { std::cout << " No TTree in the event, probalby expected" << std::endl; cout << name << endl; return 0;} 
  //Now, we will only do the below if there is a TTree in the event.
 
  
 
  
 
  
  
  
 


 
  //NEW plots that take into account the new TTree variables.



  //NOW I do some more compliacted stuff by actually looping over the TTree the old fashioned way
  //eventTimingInfoTree->SetBranchStatus("*",1);
  //using namespace edm;

  //Get the max and min times of the absolute time

  eventTimingInfoTree->Draw("absTime >> habsTime","","");
  double minTime = habsTime->GetXaxis()->GetXmin()/60;
  double maxTime = habsTime->GetXaxis()->GetXmax()/60 + 1.;
  double diffTime = maxTime - minTime;
  minTime = habsTime->GetXaxis()->GetXmin()/60.;
  
  eventTimingInfoTree->Draw("triggers >> htriggers","","");
  double minTrig = htriggers->GetXaxis()->GetXmin();
  double maxTrig = htriggers->GetXaxis()->GetXmax();
  double diffTtrig = maxTrig - minTrig;
  if ( minTrig < 0 ) cout << " Min trig is less than 0: " << minTrig;

  eventTimingInfoTree->Draw("techtriggers >> htechtriggers","","");
  double minTTrig = htechtriggers->GetXaxis()->GetXmin();
  double maxTTrig = htechtriggers->GetXaxis()->GetXmax();
  double diffTTtrig = maxTTrig - minTTrig;
  if ( minTTrig < 0 ) cout << " Min tech trig is less than 0: " << minTTrig;

  
  eventTimingInfoTree->SetBranchAddress("numberOfEBcrys",&TTreeMembers_.numEBcrys_);
  eventTimingInfoTree->SetBranchAddress("numberOfEEcrys",&TTreeMembers_.numEEcrys_);
  eventTimingInfoTree->SetBranchAddress("crystalHashedIndicesEB",&(TTreeMembers_.cryHashesEB_[0]));
  eventTimingInfoTree->SetBranchAddress("crystalHashedIndicesEE",&(TTreeMembers_.cryHashesEE_[0]));
  eventTimingInfoTree->SetBranchAddress("crystalTimesEB",&(TTreeMembers_.cryTimesEB_[0]));
  eventTimingInfoTree->SetBranchAddress("crystalTimesEE",&(TTreeMembers_.cryTimesEE_[0]));
  eventTimingInfoTree->SetBranchAddress("crystalTimeErrorsEB",&(TTreeMembers_.cryTimeErrorsEB_[0]));
  eventTimingInfoTree->SetBranchAddress("crystalTimeErrorsEE",&(TTreeMembers_.cryTimeErrorsEE_[0]));
  eventTimingInfoTree->SetBranchAddress("crystalAmplitudesEB",&(TTreeMembers_.cryAmpsEB_[0]));
  eventTimingInfoTree->SetBranchAddress("crystalAmplitudesEE",&(TTreeMembers_.cryAmpsEE_[0]));
  eventTimingInfoTree->SetBranchAddress("correctionToSampleEB",&TTreeMembers_.correctionToSample5EB_);
  eventTimingInfoTree->SetBranchAddress("correctionToSampleEEP",&TTreeMembers_.correctionToSample5EEP_);
  eventTimingInfoTree->SetBranchAddress("correctionToSampleEEM",&TTreeMembers_.correctionToSample5EEM_);
  eventTimingInfoTree->SetBranchAddress("numTriggers",&TTreeMembers_.numTriggers_);
  eventTimingInfoTree->SetBranchAddress("triggers",&(TTreeMembers_.triggers_[0]));
  eventTimingInfoTree->SetBranchAddress("numTechTriggers",&TTreeMembers_.numTechTriggers_);
  eventTimingInfoTree->SetBranchAddress("techtriggers",&(TTreeMembers_.techtriggers_[0]));
  eventTimingInfoTree->SetBranchAddress("absTime",&TTreeMembers_.absTime_);
  eventTimingInfoTree->SetBranchAddress("lumiSection",&TTreeMembers_.lumiSection_);
  eventTimingInfoTree->SetBranchAddress("bx",&TTreeMembers_.bx_);
  eventTimingInfoTree->SetBranchAddress("orbit",&TTreeMembers_.orbit_);
  int nents = eventTimingInfoTree->GetEntries();
  cout << "nentries is " << nents << endl;

  double EBTimeMax = 60.;
  double EETimeMax = 75.;
   
  TH1F *hctEB    = new TH1F("hctEB",  Form("%s EB Crystal Times;Time (ns);Number of Crystals",runChar),100, -EBTimeMax, EBTimeMax);
  TH1F *hctEE    = new TH1F("hctEE",  Form("%s EE Crystal Times;Time (ns);Number of Crystals",runChar),100, -EETimeMax, EETimeMax);
  TH1F *hctEEp   = new TH1F("hctEEp", Form("%s EE+ Crystal Times;Time (ns);Number of Crystals",runChar),100, -EETimeMax, EETimeMax);
  TH1F *hctEEm   = new TH1F("hctEEm", Form("%s EE- Crystal Times;Time (ns);Number of Crystals",runChar),100, -EETimeMax, EETimeMax);
  
  TH2F *hctEEtoAve  = new TH2F("hctEEtoAve", Form("%s EE Crystal Times to Average Time;Crystal Time (ns);Average EE Event Time (ns)",runChar),50, -EETimeMax, EETimeMax, 50, -EETimeMax, EETimeMax);
  TH2F *hctEBtoAve  = new TH2F("hctEBtoAve", Form("%s EB Crystal Times to Average Time;Crystal Time (ns);Average EB Event Time (ns)",runChar),50, -EBTimeMax, EBTimeMax, 50, -EBTimeMax, EBTimeMax);

  TH2F *hctEBtoTerr   = new TH2F("hctEBtoTerr", Form("%s EB Crystal Times to Time Error;Crystal Time (ns);Crystal Time Error (ns)",runChar),50, -EBTimeMax, EBTimeMax, 20, 0, 5.0);
  TH2F *hctEEtoTerr   = new TH2F("hctEEtoTerr", Form("%s EE Crystal Times to Time Error;Crystal Time (ns);Crystal Time Error (ns)",runChar),50, -EETimeMax, EETimeMax, 20, 0, 5.0);
  TH2F *hctEBtoAmp    = new TH2F("hctEBtoAmp", Form("%s EB Crystal Times to Amplitdue;Crystal Time (ns);Crystal Amplitude (GeV)",runChar),50, -EBTimeMax, EBTimeMax, 50, 0, 50.);
  TH2F *hctEEtoAmp    = new TH2F("hctEEtoAmp", Form("%s EE Crystal Times to Amplitdue;Crystal Time (ns);Crystal Amplitude (GeV)",runChar),50, -EETimeMax, EETimeMax, 50, 0, 50.);
  TH2F *hctEBtoAmpEvt = new TH2F("hctEBtoAmpEvt", Form("%s EB Event Time to Crystal Amplitudes;Average EB Event Time (ns);Crystal Amplitude (GeV)",runChar),50, -EBTimeMax, EBTimeMax, 50, 0, 50.);
  TH2F *hctEEtoAmpEvt = new TH2F("hctEEtoAmpEvt", Form("%s EE Event Time to Crystal Amplitudes;Average EE Event Time (ns);Crystal Amplitude (GeV)",runChar),50, -EETimeMax, EETimeMax, 50, 0, 50.);
  TH2F *hctEEtoAmpErr = new TH2F("hctEEtoAmpErr", Form("%s EE Time Error to Crystal Amplitudes;Crystal Amplitude (GeV);Time Error (ns)",runChar),50, 0., 50., 20, 0., 5. );
  TH2F *hctEBtoAmpErr = new TH2F("hctEBtoAmpErr", Form("%s EB Time Error to Crystal Amplitudes;Crystal Amplitude (GeV);Time Error (ns)",runChar),50, 0., 50., 20, 0., 5. );

  TH1F *hctEBHashed   = new TH1F("hctEBHashed",Form("%s EB Hashed Index Occupancy;EB Hashed Index", runChar),62000,0.,62000.);
  TH1F *hctEEHashed   = new TH1F("hctEEHashed",Form("%s EE Hashed Index Occupancy;EE Hashed Index", runChar),15000,0.,15000.);
  TH2F *hctEBtoHashed = new TH2F("hctEBtoHashed",Form("%s EB Hashed Index to Time;EB Hashed Index;Time(ns)",runChar),62000,0.,62000., 50, -EBTimeMax, EBTimeMax);
  TH2F *hctEEtoHashed = new TH2F("hctEEtoHashed",Form("%s EE Hashed Index to Time;EE Hashed Index;Time(ns)",runChar),15000,0.,15000., 50, -EETimeMax, EETimeMax);

  TH1F *hctEBCry   = new TH1F("hctEBCry",Form("%s EB Number of Crystals;Number of EB crystals",runChar),25,0,25);
  TH1F *hctEECry   = new TH1F("hctEECry",Form("%s EE Number of Crystals;Number of EE crystals",runChar),25,0,25);
  TH2F *hctEBCryT  = new TH2F("hctEBCryT",Form("%s EB Number of Crystals to EB average time;EB average time (ns);Number of EB crystals",runChar),50, -EBTimeMax, EBTimeMax,25,0,25);
  TH2F *hctEEpCryT  = new TH2F("hctEEpCryT",Form("%s EE Number of Crystals to EE+ average time;EE+ average time (ns);Number of EE+ crystals",runChar),50, -EETimeMax, EETimeMax,25,0,25);
  TH2F *hctEEmCryT  = new TH2F("hctEEmCryT",Form("%s EE Number of Crystals to EE- average time;EE- average time (ns);Number of EE- crystals",runChar),50, -EETimeMax, EETimeMax,25,0,25);

  TH2F *hctEEMEEP      = new TH2F("hctEEMEEP",Form("%s EE+ average time to EE- average time;EE- average time (ns);EE+ average time (ns)",runChar),50, -EETimeMax, EETimeMax,50, -EETimeMax, EETimeMax);
  TH2F *hctEEMDEEPcry  = new TH2F("hctEEMDEEPcry",Form("%s EE- minus EE+ average time vs EE crystals;(EEM - EEP) average time (ns);Number EE crystals",runChar),50, -EETimeMax/2., EETimeMax/2.,25,0, 25);
  TH1F *hctEEMDEEP     = new TH1F("hctEEMDEEP",Form("%s EE- minus EE+ average time;(EEM - EEP) average time (ns)",runChar),100, -EETimeMax, EETimeMax);
  
  TH2F *hctEEcryamp   = new TH2F("hctEEcryamp",Form("%s EE amplitudes vs number of crystals;Crystal Amp (GeV);Number EE crystals",runChar),50,0.,50.,25,0., 25.);
  TH2F *hctEBcryamp   = new TH2F("hctEBcryamp",Form("%s EB amplitudes vs number of crystals;Crystal Amp (GeV);Number EB crystals",runChar),50,0.,50.,25,0., 25.);

   

  TH1F *hEBTimeEtaLess5 = new TH1F("hEBTimeEtaLess5","EB Timing |ieta|<5; Crystal Time (ns); Entries",100, -EBTimeMax, EBTimeMax);
  TH1F *hEBPlusTime     = new TH1F("hEBPlusTime", "EB+ Timing; Crystal Time (ns); Entries",100, -EBTimeMax, EBTimeMax);
  TH1F *hEBMinusTime    = new TH1F("hEBMinusTime","EB- Timing; Crystal Time (ns); Entries",100, -EBTimeMax, EBTimeMax);
  TH2F *hEBPlus2Minus   = new TH2F("hEBPlus2Minus","EB+ to EB- Timing; EB+ Average Time (ns); EB- Average Time (ns)",50, -EBTimeMax,EBTimeMax,50, -EBTimeMax,EBTimeMax);
  TH2F *hEEPlus2Minus   = new TH2F("hEEPlus2Minus","EE+ to EE- Timing; EE+ Average Time (ns); EE- Average Time (ns)",50, -EETimeMax, EBTimeMax, 50, -EBTimeMax, EBTimeMax);
  TH1F *hAbsTime        = new TH1F("hAbsTime" ,     "Timing Occupancy ; Event Time Since Run Start (mins)",100, 0., diffTime);
  TH1F *hBX             = new TH1F("hBX     " ,     "BX Occupancy ; BX of Event",3500, 0, 3500);
  TH1F *hTriggers       = new TH1F("hTriggers",     "Trigger Occupancy; Active Trigger Bit",maxTrig, 0., maxTrig);
  TH1F *hTechTriggers   = new TH1F("hTechTriggers", "Technical Trigger Occupancy; Active Technical Trigger Bit",maxTTrig, 0., maxTTrig);
  TH2F *hAbsTimeVsEBPTime = new TH2F("hAbsTimeVsEBPTime","EB+ Average Timing vs. Absolute Time; EB+ Average Time (ns); Minutes since run start",50, -EBTimeMax, EBTimeMax, 50, 0., diffTime);
  TH2F *hAbsTimeVsEBMTime = new TH2F("hAbsTimeVsEBMTime","EB- Average Timing vs. Absolute Time; EB- Average Time (ns); Minutes since run start",50, -EBTimeMax, EBTimeMax, 50, 0., diffTime);
  TH2F *hAbsTimeVsEEPTime = new TH2F("hAbsTimeVsEEPTime","EE+ Average Timing vs. Absolute Time; EE+ Average Time (ns); Minutes since run start",50, -EETimeMax, EETimeMax, 50, 0., diffTime);
  TH2F *hAbsTimeVsEEMTime = new TH2F("hAbsTimeVsEEMTime","EE- Average Timing vs. Absolute Time; EE- Average Time (ns); Minutes since run start",50, -EETimeMax, EETimeMax, 50, 0., diffTime);
  TH2F *hTriggerVsEBPTime  = new TH2F("hTriggerVsEBPTime","EB+ Average Timing vs. Trigger; EB+ Average Time (ns); Active Trigger Bits",50, -EBTimeMax, EBTimeMax, maxTrig, 0., maxTrig);
  TH2F *hTTriggerVsEBPTime = new TH2F("hTTriggerVsEBPTime","EB+ Average Timing vs. Technical Trigger; EB+ Average Time (ns); Active Technical Trigger Bits",50, -EBTimeMax, EBTimeMax, maxTTrig, 0., maxTTrig);
  TH2F *hTriggerVsEBMTime  = new TH2F("hTriggerVsEBMTime","EB- Average Timing vs. Trigger; EB- Average Time (ns); Active Trigger Bits",50, -EBTimeMax, EBTimeMax, maxTrig, 0., maxTrig);
  TH2F *hTTriggerVsEBMTime = new TH2F("hTTriggerVsEBMTime","EB- Average Timing vs. Technical Trigger; EB- Average Time (ns); Active Technical Trigger Bits",50, -EBTimeMax, EBTimeMax, maxTTrig, 0., maxTTrig);
  TH2F *hTriggerVsEEPTime  = new TH2F("hTriggerVsEEPTime","EE+ Average Timing vs. Trigger; EE+ Average Time (ns); Active Trigger Bits",50, -EETimeMax, EETimeMax, maxTrig, 0., maxTrig);
  TH2F *hTTriggerVsEEPTime = new TH2F("hTTriggerVsEEPTime","EE+ Average Timing vs. Technical Trigger; EE+ Average Time (ns); Active Technical Trigger Bits",50, -EETimeMax, EETimeMax, maxTTrig, 0., maxTTrig);
  TH2F *hTriggerVsEEMTime  = new TH2F("hTriggerVsEEMTime","EE- Average Timing vs. Trigger; EE- Average Time (ns); Active Trigger Bits",50, -EETimeMax, EETimeMax, maxTrig, 0., maxTrig);
  TH2F *hTTriggerVsEEMTime = new TH2F("hTTriggerVsEEMTime","EE- Average Timing vs. Technical Trigger; EE- Average Time (ns); Active Technical Trigger Bits",50, -EETimeMax, EETimeMax, maxTTrig, 0., maxTTrig);
  TH2F *hBXVsEBPTime = new TH2F("hBXVsEBPTime","EB+ Average Timing vs. BX; EB+ Average Time (ns); BX",50, -EBTimeMax, EBTimeMax, 3500, 0., 3500);
  TH2F *hBXVsEBMTime = new TH2F("hBXVsEBMTime","EB- Average Timing vs. BX; EB- Average Time (ns); BX",50, -EBTimeMax, EBTimeMax, 3500, 0., 3500);
  TH2F *hBXVsEEPTime = new TH2F("hBXVsEEPTime","EE+ Average Timing vs. BX; EE+ Average Time (ns); BX",50, -EETimeMax, EETimeMax, 3500, 0., 3500);
  TH2F *hBXVsEEMTime = new TH2F("hBXVsEEMTime","EE- Average Timing vs. BX; EE- Average Time (ns); BX",50, -EETimeMax, EETimeMax, 3500, 0., 3500);
  TH2F *hTriggerVsAbsTime  = new TH2F("hTriggerVsAbsTime","Absolute Time vs. Trigger;Minutes Since Run Start ; Active Trigger Bits",50, 0, diffTime, maxTrig, 0., maxTrig);
  TH2F *hTTriggerVsAbsTime  = new TH2F("hTTriggerVsAbsTime","Absolute Time vs. Tech Trigger;Minutes Since Run Start ; Active Technical Trigger Bits",50, 0, diffTime, maxTTrig, 0., maxTTrig);
  TH2F *hBXVsAbsTime   = new TH2F("hBXVsAbsTime","Absolute Time vs. BX;Minutes Since Run Start ; BX",50, 0, diffTime, 3500, 0., 3500);
  TH2F *hTriggerVsBX   = new TH2F("hTriggerVsBX","BX vs. Trigger; BX; Active Trigger Bits",3500,0,3500, maxTrig, 0., maxTrig);
  TH2F *hTTriggerVsBX  = new TH2F("hTTriggerVsBX","BX vs. Technical Trigger; BX; Active Technical Trigger Bits",3500,0,3500, maxTTrig, 0., maxTTrig);
  
  
  TProfile2D* NtimeCHProfile = NewTProfile2D(timeCHProfile,"NtimeCHProfile");
  TProfile2D* NtimeTTProfile = NewTProfile2D(timeTTProfile,"NtimeTTProfile");
  TProfile2D* NEEPtimeCHProfile = NewTProfile2D(EEPtimeCHProfile,"NEEPtimeCHProfile");
  TProfile2D* NEEPtimeTTProfile = NewTProfile2D(EEPtimeTTProfile,"NEEPtimeTTProfile");
  TProfile2D* NEEMtimeCHProfile = NewTProfile2D(EEMtimeCHProfile,"NEEMtimeCHProfile");
  TProfile2D* NEEMtimeTTProfile = NewTProfile2D(EEMtimeTTProfile,"NEEMtimeTTProfile");

  //Now I need to define a few histograms that I will later fill

  for (int i=0; i<nents;i++) {
     eventTimingInfoTree->GetEvent(i);
     double EBave = -101.;
     double EEave = -101.;
     double EBaveO  = (TTreeMembers_.correctionToSample5EB_ -5.0)*25;
     double EEPaveO = (TTreeMembers_.correctionToSample5EEP_ -5.0)*25;
     double EEMaveO = (TTreeMembers_.correctionToSample5EEM_ -5.0)*25;
     double EBPave  = 0,EBMave = 0, EEPave  = 0, EEMave  = 0, EBnum = 0;
     double EBPn    = 0,EBMn   = 0, EEPn    = 0, EEMn    = 0, EEpnum = 0, EEmnum = 0, EEnum =0;
     double abstime = TTreeMembers_.absTime_/60. - minTime; //(puts thins in mins)
     double NumTriggers  = TTreeMembers_.numTriggers_;
     double NumTTriggers = TTreeMembers_.numTechTriggers_;
     double BX =  TTreeMembers_.bx_;

     for (int ebx=0; ebx < TTreeMembers_.numEBcrys_; ebx++) {
         int crystalHashedIndicesEB = TTreeMembers_.cryHashesEB_[ebx];
         if (crystalHashedIndicesEB == 25822 || crystalHashedIndicesEB == 32705 || crystalHashedIndicesEB == 56473) continue;
         mydet    = EBDetId::unhashIndex(crystalHashedIndicesEB);
         double myt     = (TTreeMembers_.cryTimesEB_[ebx] -5.0)*25;
         double myterr  = (TTreeMembers_.cryTimeErrorsEB_[ebx])*25;
	 if (myterr > 5.0 ) continue;
	 double amp = TTreeMembers_.cryAmpsEB_[ebx];
         int ieta = mydet.ieta();
         int iphi = mydet.iphi();
         if ( ieta > 0 ) {EBPave += myt/myterr; EBPn += 1./myterr; hEBPlusTime->Fill(myt);}
         else {EBMave += myt/myterr; EBMn += 1./myterr; hEBMinusTime->Fill(myt);}
	 EBnum++;
         hctEB->Fill(myt);
	 hctEBtoTerr->Fill(myt,myterr);
	 hctEBtoAmp->Fill(myt,amp);
	 hctEBtoAmpErr->Fill(amp,myterr);
	 hctEBHashed->Fill(crystalHashedIndicesEB);
	 hctEBtoHashed->Fill(crystalHashedIndicesEB,myt);
         NtimeCHProfile->Fill(iphi,ieta,myt);
         NtimeTTProfile->Fill(iphi,ieta,myt);
         if ( fabs(ieta) < 5 ) hEBTimeEtaLess5->Fill(myt);
     }
     for (int eex=0; eex < TTreeMembers_.numEEcrys_; eex++) {
         int crystalHashedIndicesEE = TTreeMembers_.cryHashesEE_[eex];
         if (crystalHashedIndicesEE == 11658 || crystalHashedIndicesEE == 11742 || crystalHashedIndicesEE == 10224 || crystalHashedIndicesEE == 10225 || crystalHashedIndicesEE == 10226 || crystalHashedIndicesEE == 10310 || crystalHashedIndicesEE == 10311 || crystalHashedIndicesEE == 10394 || crystalHashedIndicesEE == 10395 || crystalHashedIndicesEE == 10875 || crystalHashedIndicesEE == 11316 || crystalHashedIndicesEE == 11659 || crystalHashedIndicesEE == 11660 || crystalHashedIndicesEE == 11661 || crystalHashedIndicesEE == 11743  || crystalHashedIndicesEE == 11744 || crystalHashedIndicesEE == 11744 || crystalHashedIndicesEE == 11745 || crystalHashedIndicesEE == 11932 || crystalHashedIndicesEE == 11746 || crystalHashedIndicesEE == 12702 || crystalHashedIndicesEE == 4252 || crystalHashedIndicesEE == 4335 || crystalHashedIndicesEE == 4337 || crystalHashedIndicesEE == 4419 || crystalHashedIndicesEE == 4423 || crystalHashedIndicesEE == 4785 || crystalHashedIndicesEE == 6181 || crystalHashedIndicesEE == 14613 || crystalHashedIndicesEE == 13726 || crystalHashedIndicesEE == 13727 || crystalHashedIndicesEE == 7717 || crystalHashedIndicesEE == 7778 || crystalHashedIndicesEE == 4420 || crystalHashedIndicesEE == 4421 || crystalHashedIndicesEE == 4423 || crystalHashedIndicesEE == 2946 || crystalHashedIndicesEE == 2900 || crystalHashedIndicesEE == 2902 || crystalHashedIndicesEE == 2901 || crystalHashedIndicesEE == 2903 || crystalHashedIndicesEE == 2904 || crystalHashedIndicesEE == 2905 || crystalHashedIndicesEE == 2986 || crystalHashedIndicesEE == 2987 || crystalHashedIndicesEE == 2988 || crystalHashedIndicesEE == 2989 || crystalHashedIndicesEE == 3070 || crystalHashedIndicesEE == 3071 || crystalHashedIndicesEE == 4252 || crystalHashedIndicesEE == 4253 || crystalHashedIndicesEE == 4254 || crystalHashedIndicesEE == 4255 || crystalHashedIndicesEE == 4256) continue;
         mydete = mydete.unhashIndex(crystalHashedIndicesEE);
         double myt     = (TTreeMembers_.cryTimesEE_[eex] -5.0)*25;
         double myterr  = (TTreeMembers_.cryTimeErrorsEE_[eex])*25;
	 if (myterr > 5.0 ) continue;
	 double amp = TTreeMembers_.cryAmpsEE_[eex];
	 if (amp < 1.0) continue;
         int ix = mydete.ix();
         int iy = mydete.iy();
         int iz = mydete.zside();
	 hctEE->Fill(myt);
	 hctEEtoTerr->Fill(myt,myterr);
	 hctEEtoAmp->Fill(myt,amp);
	 hctEEtoAmpErr->Fill(amp,myterr);
	 hctEEHashed->Fill(crystalHashedIndicesEE);
	 hctEEtoHashed->Fill(crystalHashedIndicesEE,myt);
         if ( iz> 0 ) {
            EEPave += myt/myterr; EEPn += 1./myterr;EEpnum++; 
            hctEEp->Fill(myt);
            NEEPtimeCHProfile->Fill(ix,iy,myt);
            NEEPtimeTTProfile->Fill(ix,iy,myt);
         }
         else {
            EEMave += myt/myterr; EEMn += 1./myterr; EEmnum++; 
            hctEEm->Fill(myt);
            NEEMtimeCHProfile->Fill(ix,iy,myt);
            NEEMtimeTTProfile->Fill(ix,iy,myt);        
         }
     }
     EEnum = EEpnum + EEmnum;
     if ( EBPn > 0.0 || EBMn > 0.0 )
     {
        EBave = (EBPave + EBMave) / (EBPn+EBMn);
	hctEBCry->Fill(EBnum);
	hctEBCryT->Fill(EBave, EBnum);
     }
     if ( EEPn > 0.0 || EEMn > 0.0 )
     {
        EEave = (EEPave + EEMave) / (EEPn+EEMn);
	hctEECry->Fill(EEnum);	
     }
     
     if (EBPn > 0.0 ) {EBPave /= EBPn; }
     if (EBMn > 0.0 ) {EBMave /= EBMn; }
     if (EEPn > 0.0 ) {EEPave /= EEPn; hctEEpCryT->Fill(EEPave, EEpnum);}
     if (EEMn > 0.0 ) {EEMave /= EEMn; hctEEmCryT->Fill(EEMave, EEmnum);}

     if ( EBPn > 0.0 && EBMn > 0.0 ) { hEBPlus2Minus->Fill(EBPave,EBMave);}
     if ( EEPn > 0.0 && EEMn > 0.0 ) 
     {
       hEEPlus2Minus->Fill(EEPave,EEMave); 
       hctEEMEEP->Fill(EEMave,EEPave);
       hctEEMDEEPcry->Fill(EEMave-EEPave,EEnum);
       hctEEMDEEP->Fill(EEMave-EEPave);
     }

     double totnumb = EBPn + EBMn + EEPn + EEMn;
     if (totnumb < 0 ) continue;
 

     
     for (int ebx=0; ebx < TTreeMembers_.numEBcrys_; ebx++) {
         int crystalHashedIndicesEB = TTreeMembers_.cryHashesEB_[ebx];
         if (crystalHashedIndicesEB == 25822 || crystalHashedIndicesEB == 32705 || crystalHashedIndicesEB == 56473) continue;
         mydet    = EBDetId::unhashIndex(crystalHashedIndicesEB);
         double myt     = (TTreeMembers_.cryTimesEB_[ebx] -5.0)*25;
         double myterr  = (TTreeMembers_.cryTimeErrorsEB_[ebx])*25;
	 if (myterr > 5.0 ) continue;
	 double amp = TTreeMembers_.cryAmpsEB_[ebx];
         int ieta = mydet.ieta();
         int iphi = mydet.iphi();
	 hctEBtoAve->Fill(myt,EBave);
	 hctEBtoAmpEvt->Fill(EBave,amp);
         hctEBcryamp->Fill(amp,EBnum);
     }
     for (int eex=0; eex < TTreeMembers_.numEEcrys_; eex++) {
         int crystalHashedIndicesEE = TTreeMembers_.cryHashesEE_[eex];
         if (crystalHashedIndicesEE == 11658 || crystalHashedIndicesEE == 11742 || crystalHashedIndicesEE == 10224 || crystalHashedIndicesEE == 10225 || crystalHashedIndicesEE == 10226 || crystalHashedIndicesEE == 10310 || crystalHashedIndicesEE == 10311 || crystalHashedIndicesEE == 10394 || crystalHashedIndicesEE == 10395 || crystalHashedIndicesEE == 10875 || crystalHashedIndicesEE == 11316 || crystalHashedIndicesEE == 11659 || crystalHashedIndicesEE == 11660 || crystalHashedIndicesEE == 11661 || crystalHashedIndicesEE == 11743  || crystalHashedIndicesEE == 11744 || crystalHashedIndicesEE == 11744 || crystalHashedIndicesEE == 11745 || crystalHashedIndicesEE == 11932 || crystalHashedIndicesEE == 11746 || crystalHashedIndicesEE == 12702 || crystalHashedIndicesEE == 4252 || crystalHashedIndicesEE == 4335 || crystalHashedIndicesEE == 4337 || crystalHashedIndicesEE == 4419 || crystalHashedIndicesEE == 4423 || crystalHashedIndicesEE == 4785 || crystalHashedIndicesEE == 6181 || crystalHashedIndicesEE == 14613 || crystalHashedIndicesEE == 13726 || crystalHashedIndicesEE == 13727 || crystalHashedIndicesEE == 7717 || crystalHashedIndicesEE == 7778 || crystalHashedIndicesEE == 4420 || crystalHashedIndicesEE == 4421 || crystalHashedIndicesEE == 4423 || crystalHashedIndicesEE == 2946 || crystalHashedIndicesEE == 2900 || crystalHashedIndicesEE == 2902 || crystalHashedIndicesEE == 2901 || crystalHashedIndicesEE == 2903 || crystalHashedIndicesEE == 2904 || crystalHashedIndicesEE == 2905 || crystalHashedIndicesEE == 2986 || crystalHashedIndicesEE == 2987 || crystalHashedIndicesEE == 2988 || crystalHashedIndicesEE == 2989 || crystalHashedIndicesEE == 3070 || crystalHashedIndicesEE == 3071 || crystalHashedIndicesEE == 4252 || crystalHashedIndicesEE == 4253 || crystalHashedIndicesEE == 4254 || crystalHashedIndicesEE == 4255 || crystalHashedIndicesEE == 4256) continue;
         mydete = mydete.unhashIndex(crystalHashedIndicesEE);
         double myt     = (TTreeMembers_.cryTimesEE_[eex] -5.0)*25;
         double myterr  = (TTreeMembers_.cryTimeErrorsEE_[eex])*25;
	 if (myterr > 5.0 ) continue;
	 double amp = TTreeMembers_.cryAmpsEE_[eex];
	 if (amp < 1.0) continue;
         int ix = mydete.ix();
         int iy = mydete.iy();
         int iz = mydete.zside();
	 hctEEtoAve->Fill(myt,EEave);
	 hctEEtoAmpEvt->Fill(EEave,amp);
	 hctEEcryamp->Fill(amp,EEnum);
     }
     
     hAbsTime->Fill(abstime);
     hBX->Fill(BX);
     hBXVsAbsTime->Fill(abstime,BX);
     
     for ( int ti = 0; ti < NumTriggers; ++ti)
     {
         hTriggers->Fill(TTreeMembers_.triggers_[ti]);
	 hTriggerVsAbsTime->Fill(abstime,TTreeMembers_.triggers_[ti]);
	 hTriggerVsBX->Fill(BX,TTreeMembers_.triggers_[ti]);
         
     }
     for ( int ti = 0; ti < NumTTriggers; ++ti)
     {
         hTechTriggers->Fill(TTreeMembers_.techtriggers_[ti]);
         hTTriggerVsAbsTime->Fill(abstime,TTreeMembers_.techtriggers_[ti]);
	 hTTriggerVsBX->Fill(BX,TTreeMembers_.techtriggers_[ti]);
     }

     if (EBPn > 0.0 )
     {
	hAbsTimeVsEBPTime->Fill(EBPave,abstime);
        hBXVsEBPTime->Fill(EBPave,BX);
        for ( int ti = 0; ti < NumTriggers; ++ti)
        {
            hTriggerVsEBPTime->Fill(EBPave, TTreeMembers_.triggers_[ti]);
        }
        for ( int ti = 0; ti < NumTTriggers; ++ti)
        {
            hTTriggerVsEBPTime->Fill(EBPave,TTreeMembers_.techtriggers_[ti]);
        }
     }

     if (EBMn > 0.0 )
     {
	hAbsTimeVsEBMTime->Fill(EBMave,abstime);
        hBXVsEBMTime->Fill(EBMave,BX);
        for ( int ti = 0; ti < NumTriggers; ++ti)
        {
            hTriggerVsEBMTime->Fill(EBMave, TTreeMembers_.triggers_[ti]);
        }
        for ( int ti = 0; ti < NumTTriggers; ++ti)
        {
            hTTriggerVsEBMTime->Fill(EBMave,TTreeMembers_.techtriggers_[ti]);
        }
     }
     if (EEPn > 0.0 )
     {
	hAbsTimeVsEEPTime->Fill(EEPave,abstime);
        hBXVsEEPTime->Fill(EEPave,BX);
	for ( int ti = 0; ti < NumTriggers; ++ti)
        {
            hTriggerVsEEPTime->Fill(EEPave, TTreeMembers_.triggers_[ti]);
        }
        for ( int ti = 0; ti < NumTTriggers; ++ti)
        {
            hTTriggerVsEEPTime->Fill(EEPave,TTreeMembers_.techtriggers_[ti]);
        }
     }

     if (EEMn > 0.0 )
     {
	hAbsTimeVsEEMTime->Fill(EEMave,abstime);
	hBXVsEEMTime->Fill(EEMave,BX);
	for ( int ti = 0; ti < NumTriggers; ++ti)
        {
            hTriggerVsEEMTime->Fill(EEMave, TTreeMembers_.triggers_[ti]);
        }
        for ( int ti = 0; ti < NumTTriggers; ++ti)
        {
            hTTriggerVsEEMTime->Fill(EEMave,TTreeMembers_.techtriggers_[ti]);
        }
     }

  }
  //----------End of the fun looping stuff

  // Now I need to print some of these new histograms and plots
  
  c[28]->cd();
  gStyle->SetOptStat(1110);
  hctEB->Draw();
  hctEB->GetXaxis()->SetNdivisions(512);
  if ( fit ) hctEB->Fit("gaus");
  gStyle->SetOptFit(111);
  c[28]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTIMES_%i.%s",dirName,mType,runNumber,fileType); c[28]->Print(name); }

  c[29]->cd();
  gStyle->SetOptStat(1110);
  hctEE->Draw();
  c[29]->SetLogy(1);
  hctEE->GetXaxis()->SetNdivisions(512);
  if ( fit ) hctEE->Fit("gaus");
  gStyle->SetOptFit(111);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EETIMES_%i.%s",dirName,mType,runNumber,fileType); c[29]->Print(name); }

  c[54]->cd();
  gStyle->SetOptStat(1110);
  hctEEp->Draw();
  c[54]->SetLogy(1);
  hctEEp->GetXaxis()->SetNdivisions(512);
  if ( fit ) hctEEp->Fit("gaus");
  gStyle->SetOptFit(111);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPTIMES_%i.%s",dirName,mType,runNumber,fileType); c[54]->Print(name); }

  c[55]->cd();
  gStyle->SetOptStat(1110);
  hctEEm->Draw();
  c[55]->SetLogy(1);
  hctEEm->GetXaxis()->SetNdivisions(512);
  if ( fit ) hctEEm->Fit("gaus");
  gStyle->SetOptFit(111);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMTIMES_%i.%s",dirName,mType,runNumber,fileType); c[55]->Print(name); }
  
  //Time to average event time
  c[30]->cd();
  gStyle->SetOptStat(1110);
  hctEEtoAve->Draw("colz");
  hctEEtoAve->GetXaxis()->SetNdivisions(512);
  hctEEtoAve->GetYaxis()->SetNdivisions(512);
  c[30]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EETIMEStoAverage_%i.%s",dirName,mType,runNumber,fileType); c[30]->Print(name); }
  
  c[31]->cd();
  gStyle->SetOptStat(1110);
  hctEBtoAve->Draw("colz");
  hctEBtoAve->GetXaxis()->SetNdivisions(512);
  hctEBtoAve->GetYaxis()->SetNdivisions(512);
  c[31]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTIMEStoAverage_%i.%s",dirName,mType,runNumber,fileType); c[31]->Print(name); }

  //Time to Time error
  c[32]->cd();
  gStyle->SetOptStat(1110);
  hctEBtoTerr->Draw("colz");
  hctEBtoTerr->GetXaxis()->SetNdivisions(512);
  hctEBtoTerr->GetYaxis()->SetNdivisions(507);
  c[32]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTIMEStoTERR_%i.%s",dirName,mType,runNumber,fileType); c[32]->Print(name); }
  
  c[33]->cd();
  gStyle->SetOptStat(1110);
  hctEEtoTerr->Draw("colz");
  hctEEtoTerr->GetXaxis()->SetNdivisions(512);
  hctEEtoTerr->GetYaxis()->SetNdivisions(507);
  c[33]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EETIMEStoTERR_%i.%s",dirName,mType,runNumber,fileType); c[33]->Print(name); }
  
  //Amplitude to time
  c[34]->cd();
  gStyle->SetOptStat(1110);
  hctEEtoAmp->Draw("colz");
  hctEEtoAmp->GetXaxis()->SetNdivisions(512);
  hctEEtoAmp->GetYaxis()->SetNdivisions(507);
  c[34]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EETIMEStoAMP_%i.%s",dirName,mType,runNumber,fileType); c[34]->Print(name); } 
  c[35]->cd();
  gStyle->SetOptStat(1110);
  hctEBtoAmp->Draw("colz");
  hctEBtoAmp->GetXaxis()->SetNdivisions(512);
  hctEBtoAmp->GetYaxis()->SetNdivisions(507);
  c[35]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTIMEStoAMP_%i.%s",dirName,mType,runNumber,fileType); c[35]->Print(name); } 
  
   //Amplitdue to ave event time
  c[36]->cd();
  gStyle->SetOptStat(1110);
  hctEBtoAmpEvt->Draw("colz");
  hctEBtoAmpEvt->GetXaxis()->SetNdivisions(512);
  hctEBtoAmpEvt->GetYaxis()->SetNdivisions(507);
  c[36]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBEvtTIMEStoAMP_%i.%s",dirName,mType,runNumber,fileType); c[36]->Print(name); } 
  c[37]->cd();
  gStyle->SetOptStat(1110);
  hctEEtoAmpEvt->Draw("colz");
  hctEEtoAmpEvt->GetXaxis()->SetNdivisions(512);
  hctEEtoAmpEvt->GetYaxis()->SetNdivisions(507);
  c[37]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEEvtTIMEStoAMP_%i.%s",dirName,mType,runNumber,fileType); c[37]->Print(name); } 
  
  //Amplitude to time error.
  c[38]->cd();
  gStyle->SetOptStat(1110);
  hctEEtoAmpErr->Draw("colz");
  hctEEtoAmpErr->GetXaxis()->SetNdivisions(512);
  hctEEtoAmpErr->GetYaxis()->SetNdivisions(507);
  c[38]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EETIMESErrtoAMP_%i.%s",dirName,mType,runNumber,fileType); c[38]->Print(name); } 
  c[39]->cd();
  gStyle->SetOptStat(1110);
  hctEBtoAmpErr->Draw("colz");
  hctEBtoAmpErr->GetXaxis()->SetNdivisions(512);
  hctEBtoAmpErr->GetYaxis()->SetNdivisions(507);
  c[39]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTIMESErrtoAMP_%i.%s",dirName,mType,runNumber,fileType); c[39]->Print(name); } 

  //Hashed Index's
  c[50]->cd();
  gStyle->SetOptStat(1110);
  hctEBHashed->Draw();
  hctEBHashed->GetXaxis()->SetNdivisions(512);
  c[50]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBHashed_%i.%s",dirName,mType,runNumber,fileType); c[50]->Print(name); } 
  
  c[51]->cd();
  gStyle->SetOptStat(1110);
  hctEEHashed->Draw();
  c[51]->SetLogy(1);
  hctEEHashed->GetXaxis()->SetNdivisions(512);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEHashed_%i.%s",dirName,mType,runNumber,fileType); c[51]->Print(name); } 
  
  
  //Time to Hashed Index
  c[52]->cd();
  gStyle->SetOptStat(1110);
  hctEBtoHashed->Draw("colz");
  hctEBtoHashed->GetXaxis()->SetNdivisions(512);
  hctEBtoHashed->GetYaxis()->SetNdivisions(507);
  hctEBtoHashed->SetMinimum(1);
  c[52]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBHashedToTime_%i.%s",dirName,mType,runNumber,fileType); c[52]->Print(name); } 

  c[53]->cd();
  gStyle->SetOptStat(1110);
  hctEEtoHashed->Draw("colz");
  hctEEtoHashed->GetXaxis()->SetNdivisions(512);
  hctEEtoHashed->GetYaxis()->SetNdivisions(507);
  hctEEtoHashed->SetMinimum(1);
  c[53]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEHashedToTime_%i.%s",dirName,mType,runNumber,fileType); c[53]->Print(name); } 
  
   //1-D Number of crystal distributions
  c[56]->cd();
  gStyle->SetOptStat(111110);
  hctEBCry->Draw();
  hctEBCry->GetXaxis()->SetNdivisions(512);
  c[56]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBCrys_%i.%s",dirName,mType,runNumber,fileType); c[56]->Print(name); } 
  
  c[57]->cd();
  gStyle->SetOptStat(111110);
  hctEECry->Draw();
  hctEECry->GetXaxis()->SetNdivisions(512);
  c[57]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EECrys_%i.%s",dirName,mType,runNumber,fileType); c[57]->Print(name); } 


  //2-D crystal plots 
  c[58]->cd();
  gStyle->SetOptStat(111110);
  hctEBCryT->Draw("colz");
  hctEBCryT->GetXaxis()->SetNdivisions(512);
  hctEBCryT->GetYaxis()->SetNdivisions(507);
  hctEBCryT->SetMinimum(1);
  c[58]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBCrysToTime_%i.%s",dirName,mType,runNumber,fileType); c[58]->Print(name); } 
    
  c[59]->cd();
  gStyle->SetOptStat(111110);
  hctEEpCryT->Draw("colz");
  hctEEpCryT->GetXaxis()->SetNdivisions(512);
  hctEEpCryT->GetYaxis()->SetNdivisions(507);
  hctEEpCryT->SetMinimum(1);
  c[59]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPCrysToTime_%i.%s",dirName,mType,runNumber,fileType); c[59]->Print(name); } 
    
  c[60]->cd();
  gStyle->SetOptStat(111110);
  hctEEmCryT->Draw("colz");
  hctEEmCryT->GetXaxis()->SetNdivisions(512);
  hctEEmCryT->GetYaxis()->SetNdivisions(507);
  hctEEmCryT->SetMinimum(1);
  c[60]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMCrysToTime_%i.%s",dirName,mType,runNumber,fileType); c[60]->Print(name); } 
  
  c[61]->cd();
  gStyle->SetOptStat(111110);
  hctEEMEEP->Draw("colz");
  hctEEMEEP->GetXaxis()->SetNdivisions(512);
  hctEEMEEP->GetYaxis()->SetNdivisions(507);
  hctEEMEEP->SetMinimum(1);
  c[61]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPTimeToEEMTime_%i.%s",dirName,mType,runNumber,fileType); c[61]->Print(name); } 

  c[63]->cd();
  gStyle->SetOptStat(1110);
  hctEEMDEEPcry->Draw("colz");
  hctEEMDEEPcry->GetXaxis()->SetNdivisions(512);
  hctEEMDEEPcry->SetMinimum(1);
  c[63]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPDiffEEMTimeCrys_%i.%s",dirName,mType,runNumber,fileType); c[63]->Print(name); }
  

  c[62]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  hctEEMDEEP->Draw();
  hctEEMDEEP->GetXaxis()->SetNdivisions(512);
  cout << "mean is " << hctEEMDEEP->GetMean() << endl;
  if (hctEEMDEEP->GetMean() != 0 && fit) hctEEMDEEP->Fit("gaus");
  //hctEEMEEP->SetMinimum(.5);
  c[62]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPDiffEEMTime_%i.%s",dirName,mType,runNumber,fileType); c[62]->Print(name); } 
  
  //Number of crystals vs amplitude
  c[64]->cd();
  gStyle->SetOptStat(1110);
  hctEEcryamp->Draw("colz");
  hctEEcryamp->GetXaxis()->SetNdivisions(512);
  hctEEcryamp->SetMinimum(1);
  c[64]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EECrysAmp_%i.%s",dirName,mType,runNumber,fileType); c[64]->Print(name); }

  c[65]->cd();
  gStyle->SetOptStat(1110);
  hctEBcryamp->Draw("colz");
  hctEBcryamp->GetXaxis()->SetNdivisions(512);
  hctEBcryamp->SetMinimum(1);
  c[65]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBCrysAmp_%i.%s",dirName,mType,runNumber,fileType); c[65]->Print(name); }


  c[80]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  hEBTimeEtaLess5->Draw();
  if (hEBTimeEtaLess5->GetMean() != 0 && fit) hEBTimeEtaLess5->Fit("gaus");
  c[80]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTimeEtaLess5_%i.%s",dirName,mType,runNumber,fileType); c[80]->Print(name); }

  c[81]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  hEBPlusTime->Draw();
  if (hEBPlusTime->GetMean() != 0 && fit ) hEBPlusTime->Fit("gaus");
  c[81]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBPlusTime_%i.%s",dirName,mType,runNumber,fileType); c[81]->Print(name); }

  c[82]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  hEBMinusTime->Draw();
  if (hEBMinusTime->GetMean() != 0 && fit) hEBMinusTime->Fit("gaus");
  c[82]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBMinusTime_%i.%s",dirName,mType,runNumber,fileType); c[82]->Print(name); }

  c[83]->cd();
  gStyle->SetOptStat(10);
  hEBPlus2Minus->Draw("colz");
  c[83]->SetLogy(0);
  c[83]->SetLogz(0);
  c[83]->SetGridx(0);
  c[83]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBPlus2Minus_%i.%s",dirName,mType,runNumber,fileType); c[83]->Print(name); }

  c[84]->cd();
  gStyle->SetOptStat(111110);
  hAbsTime->Draw();
  c[84]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_AbsTime_%i.%s",dirName,mType,runNumber,fileType); c[84]->Print(name); }

  c[85]->cd();
  gStyle->SetOptStat(111110);
  hBX->Draw();
  c[85]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_BX_%i.%s",dirName,mType,runNumber,fileType); c[85]->Print(name); }

  c[86]->cd();
  gStyle->SetOptStat(111110);
  hTriggers->Draw();
  c[86]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_Triggers_%i.%s",dirName,mType,runNumber,fileType); c[86]->Print(name); }

  c[87]->cd();
  gStyle->SetOptStat(111110);
  hTechTriggers->Draw();
  c[87]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TechTriggers_%i.%s",dirName,mType,runNumber,fileType); c[87]->Print(name); }

  c[88]->cd();
  gStyle->SetOptStat(10);
  hAbsTimeVsEBPTime->Draw("colz");
  c[88]->SetLogy(0);
  c[88]->SetLogz(1);
  c[88]->SetGridx(0);
  c[88]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_AbsTimeVsEBPTime_%i.%s",dirName,mType,runNumber,fileType); c[88]->Print(name); }

  c[89]->cd();
  gStyle->SetOptStat(10);
  hAbsTimeVsEBMTime->Draw("colz");
  c[89]->SetLogy(0);
  c[89]->SetLogz(1);
  c[89]->SetGridx(0);
  c[89]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_AbsTimeVsEBMTime_%i.%s",dirName,mType,runNumber,fileType); c[89]->Print(name); }

  c[90]->cd();
  gStyle->SetOptStat(10);
  hAbsTimeVsEEPTime->Draw("colz");
  c[90]->SetLogy(0);
  c[90]->SetLogz(1);
  c[90]->SetGridx(0);
  c[90]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_AbsTimeVsEEPTime_%i.%s",dirName,mType,runNumber,fileType); c[90]->Print(name); }

  c[91]->cd();
  gStyle->SetOptStat(10);
  hAbsTimeVsEEMTime->Draw("colz");
  c[91]->SetLogy(0);
  c[91]->SetLogz(1);
  c[91]->SetGridx(0);
  c[91]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_AbsTimeVsEEMTime_%i.%s",dirName,mType,runNumber,fileType); c[91]->Print(name); }

  c[92]->cd();
  gStyle->SetOptStat(10);
  hTriggerVsEBPTime->Draw("colz");
  c[92]->SetLogy(0);
  c[92]->SetLogz(1);
  c[92]->SetGridx(0);
  c[92]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TriggerVsEBPTime_%i.%s",dirName,mType,runNumber,fileType); c[92]->Print(name); }

  c[93]->cd();
  gStyle->SetOptStat(10);
  hTTriggerVsEBPTime->Draw("colz");
  c[93]->SetLogy(0);
  c[93]->SetLogz(1);
  c[93]->SetGridx(0);
  c[93]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TechTriggerVsEBPTime_%i.%s",dirName,mType,runNumber,fileType); c[93]->Print(name); }

  c[94]->cd();
  gStyle->SetOptStat(10);
  hTriggerVsEBMTime->Draw("colz");
  c[94]->SetLogy(0);
  c[94]->SetLogz(1);
  c[94]->SetGridx(0);
  c[94]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TriggerVsEBMTime_%i.%s",dirName,mType,runNumber,fileType); c[94]->Print(name); }

  c[95]->cd();
  gStyle->SetOptStat(10);
  hTTriggerVsEBMTime->Draw("colz");
  c[95]->SetLogy(0);
  c[95]->SetLogz(1);
  c[95]->SetGridx(0);
  c[95]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TechTriggerVsEBMTime_%i.%s",dirName,mType,runNumber,fileType); c[95]->Print(name); }

  c[96]->cd();
  gStyle->SetOptStat(10);
  hTriggerVsEEPTime->Draw("colz");
  c[96]->SetLogy(0);
  c[96]->SetLogz(1);
  c[96]->SetGridx(0);
  c[96]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TriggerVsEEPTime_%i.%s",dirName,mType,runNumber,fileType); c[96]->Print(name); }

  c[97]->cd();
  gStyle->SetOptStat(10);
  hTTriggerVsEEPTime->Draw("colz");
  c[97]->SetLogy(0);
  c[97]->SetLogz(1);
  c[97]->SetGridx(0);
  c[97]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TechTriggerVsEEPTime_%i.%s",dirName,mType,runNumber,fileType); c[97]->Print(name); }

  c[98]->cd();
  gStyle->SetOptStat(10);
  hTriggerVsEEMTime->Draw("colz");
  c[98]->SetLogy(0);
  c[98]->SetLogz(1);
  c[98]->SetGridx(0);
  c[98]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TriggerVsEEMTime_%i.%s",dirName,mType,runNumber,fileType); c[98]->Print(name); }

  c[99]->cd();
  gStyle->SetOptStat(10);
  hTTriggerVsEEMTime->Draw("colz");
  c[99]->SetLogy(0);
  c[99]->SetLogz(1);
  c[99]->SetGridx(0);
  c[99]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TechTriggerVsEEMTime_%i.%s",dirName,mType,runNumber,fileType); c[99]->Print(name); }

  c[100]->cd();
  gStyle->SetOptStat(10);
  hBXVsEBPTime->Draw("colz");
  c[100]->SetLogy(0);
  c[100]->SetLogz(1);
  c[100]->SetGridx(0);
  c[100]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_BXVsEBPTime_%i.%s",dirName,mType,runNumber,fileType); c[100]->Print(name); }

  c[101]->cd();
  gStyle->SetOptStat(10);
  hBXVsEBMTime->Draw("colz");
  c[101]->SetLogy(0);
  c[101]->SetLogz(1);
  c[101]->SetGridx(0);
  c[101]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_BXVsEBMTime_%i.%s",dirName,mType,runNumber,fileType); c[101]->Print(name); }

  c[102]->cd();
  gStyle->SetOptStat(10);
  hBXVsEEPTime->Draw("colz");
  c[102]->SetLogy(0);
  c[102]->SetLogz(1);
  c[102]->SetGridx(0);
  c[102]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_BXVsEEPTime_%i.%s",dirName,mType,runNumber,fileType); c[102]->Print(name); }

  c[103]->cd();
  gStyle->SetOptStat(10);
  hBXVsEEMTime->Draw("colz");
  c[103]->SetLogy(0);
  c[103]->SetLogz(1);
  c[103]->SetGridx(0);
  c[103]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_BXVsEEMTime_%i.%s",dirName,mType,runNumber,fileType); c[103]->Print(name); }

  c[104]->cd();
  gStyle->SetOptStat(10);
  hTriggerVsAbsTime->Draw("colz");
  c[104]->SetLogy(0);
  c[104]->SetLogz(1);
  c[104]->SetGridx(0);
  c[104]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TriggerVsAbsTime_%i.%s",dirName,mType,runNumber,fileType); c[104]->Print(name); }

  c[105]->cd();
  gStyle->SetOptStat(10);
  hTTriggerVsAbsTime->Draw("colz");
  c[105]->SetLogy(0);
  c[105]->SetLogz(1);
  c[105]->SetGridx(0);
  c[105]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TechTriggerVsAbsTime_%i.%s",dirName,mType,runNumber,fileType); c[105]->Print(name); }

  c[106]->cd();
  gStyle->SetOptStat(10);
  hBXVsAbsTime->Draw("colz");
  c[106]->SetLogy(0);
  c[106]->SetLogz(1);
  c[106]->SetGridx(0);
  c[106]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_hBXVsAbsTime_%i.%s",dirName,mType,runNumber,fileType); c[106]->Print(name); }

  c[107]->cd();
  gStyle->SetOptStat(10);
  hTriggerVsBX->Draw("colz");
  c[107]->SetLogy(0);
  c[107]->SetLogz(1);
  c[107]->SetGridx(0);
  c[107]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TriggerVsBX_%i.%s",dirName,mType,runNumber,fileType); c[107]->Print(name); }

  c[108]->cd();
  gStyle->SetOptStat(10);
  hTTriggerVsBX->Draw("colz");
  c[108]->SetLogy(0);
  c[108]->SetLogz(1);
  c[108]->SetGridx(0);
  c[108]->SetGridy(0);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TechTriggerVsBX_%i.%s",dirName,mType,runNumber,fileType); c[108]->Print(name); }




  //FINAL 1D timing number, by crystal by TT
  c[66]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *tthistEB = HistFromTProfile2D(timeTTProfile,"tthistEB",200, -30., 40.,-5.,25.);
  sprintf(mytitle,"%s Average TT Timing (EB);TT time average (ns)",runChar); 
  tthistEB->SetTitle(mytitle);
  tthistEB->GetXaxis()->SetNdivisions(512);
  if (tthistEB->GetMean() != 0 && fit) tthistEB->Fit("gaus");
  c[66]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTTTIME_%i.%s",dirName,mType,runNumber,fileType); c[66]->Print(name); }

  c[67]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *chhistEB = HistFromTProfile2D(timeCHProfile,"chhistEB",200, -30., 40.,-5.,25.);
  sprintf(mytitle,"%s Average CH Timing (EB);CH time average (ns)",runChar); 
  chhistEB->SetTitle(mytitle);
  chhistEB->GetXaxis()->SetNdivisions(512);
  if (chhistEB->GetMean() != 0 && fit ) chhistEB->Fit("gaus");
  c[67]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBCHTIME_%i.%s",dirName,mType,runNumber,fileType); c[67]->Print(name); }

  c[68]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *tthistEEP = HistFromTProfile2D(EEPtimeTTProfile,"tthistEEP",200, -30., 40.,-5.,25.);
  sprintf(mytitle,"%s EE+ TT Timing;TT time average (ns)",runChar); 
  tthistEEP->SetTitle(mytitle);
  tthistEEP->GetXaxis()->SetNdivisions(512);
  if (tthistEEP->GetMean() != 0 && fit) tthistEEP->Fit("gaus");
  c[68]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPTTTIME_%i.%s",dirName,mType,runNumber,fileType); c[68]->Print(name); }

  c[69]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *chhistEEP = HistFromTProfile2D(EEPtimeCHProfile,"chhistEEP",200, -30., 40.,-5.,25.);
  sprintf(mytitle,"%s EE+ CH Timing;CH time average (ns)",runChar); 
  chhistEEP->SetTitle(mytitle);
  chhistEEP->GetXaxis()->SetNdivisions(512);
  if (chhistEEP->GetMean() != 0 && fit) chhistEEP->Fit("gaus");
  c[69]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPCHTIME_%i.%s",dirName,mType,runNumber,fileType); c[69]->Print(name); }

  c[70]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *tthistEEM = HistFromTProfile2D(EEMtimeTTProfile,"tthistEEM",200, -30., 40.,-5.,25.);
  sprintf(mytitle,"%s EE- TT Timing;TT time average (ns)",runChar); 
  tthistEEM->SetTitle(mytitle);
  tthistEEM->GetXaxis()->SetNdivisions(512);
  if (tthistEEM->GetMean() != 0 && fit) tthistEEM->Fit("gaus");
  c[70]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMTTTIME_%i.%s",dirName,mType,runNumber,fileType); c[70]->Print(name); }

  c[71]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *chhistEEM = HistFromTProfile2D(EEMtimeCHProfile,"chhistEEM",200, -30., 40.,-5.,25.);
  sprintf(mytitle,"%s EE- CH Timing;CH time average (ns)",runChar); 
  chhistEEM->SetTitle(mytitle);
  chhistEEM->GetXaxis()->SetNdivisions(512);
  if (chhistEEM->GetMean() != 0 && fit) chhistEEM->Fit("gaus");
  c[71]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMCHTIME_%i.%s",dirName,mType,runNumber,fileType); c[71]->Print(name); }

  c[15]->cd();
  gStyle->SetOptStat(10);
  Scale0TProfile2D(NtimeCHProfile);
  NtimeCHProfile->Draw("colz");
  sprintf(mytitle,"%s in ns",timeCHProfile->GetTitle()); 
  NtimeCHProfile->SetTitle(mytitle);
  NtimeCHProfile->SetMinimum(tlowc);
  NtimeCHProfile->SetMaximum(thighc);

  NtimeCHProfile->GetXaxis()->SetNdivisions(-18);
  NtimeCHProfile->GetYaxis()->SetNdivisions(2);
  c[15]->SetLogy(0);
  c[15]->SetLogz(0);
  c[15]->SetGridx(1);
  c[15]->SetGridy(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeCHProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[15]->Print(name); }
  //TT by TT timing profile
  c[67]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *chhistEBN = HistNFromTProfile2D(NtimeCHProfile,"chhistEBN",200, -30., 40.);
  sprintf(mytitle,"%s Average CH Timing (EB);CH time average (ns)",runChar); 
  chhistEBN->SetTitle(mytitle);
  chhistEBN->GetXaxis()->SetNdivisions(512);
  if (chhistEBN->GetMean() != 0 && fit ) chhistEBN->Fit("gaus");
  c[67]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBCHTIME_%i.%s",dirName,mType,runNumber,fileType); c[67]->Print(name); }

  c[15]->cd();
  gStyle->SetOptStat(10);
  Scale0TProfile2D(NtimeTTProfile);
  NtimeTTProfile->Draw("colz");
  sprintf(mytitle,"%s in ns",timeTTProfile->GetTitle()); 
  NtimeTTProfile->SetTitle(mytitle);
  NtimeTTProfile->SetMinimum(tlowc);
  NtimeTTProfile->SetMaximum(thighc);

  NtimeTTProfile->GetXaxis()->SetNdivisions(-18);
  NtimeTTProfile->GetYaxis()->SetNdivisions(2);
  c[15]->SetLogy(0);
  c[15]->SetLogz(0);
  c[15]->SetGridx(1);
  c[15]->SetGridy(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeTTProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[15]->Print(name); }
  //TT by TT timing profile
  c[67]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *tthistEBN = HistNFromTProfile2D(NtimeTTProfile,"tthistEBN",200, -30., 40.);
  sprintf(mytitle,"%s Average TT Timing (EB);TT time average (ns)",runChar); 
  tthistEBN->SetTitle(mytitle);
  tthistEBN->GetXaxis()->SetNdivisions(512);
  if (tthistEBN->GetMean() != 0 && fit ) tthistEBN->Fit("gaus");
  c[67]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTTTIME_%i.%s",dirName,mType,runNumber,fileType); c[67]->Print(name); }

  c[15]->cd();
  gStyle->SetOptStat(10);
  Scale0TProfile2D(NEEPtimeCHProfile);
  NEEPtimeCHProfile->Draw("colz");
  sprintf(mytitle,"%s in ns",EEPtimeCHProfile->GetTitle()); 
  NEEPtimeCHProfile->SetTitle(mytitle);
  NEEPtimeCHProfile->SetMinimum(tlowc);
  NEEPtimeCHProfile->SetMaximum(thighc);

  NEEPtimeCHProfile->GetXaxis()->SetNdivisions(18);
  //NEEPtimeCHProfile->GetYaxis()->SetNdivisions(2);
  c[15]->SetLogy(0);
  c[15]->SetLogz(0);
  c[15]->SetGridx(0);
  c[15]->SetGridy(0);
  drawEELines();
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPtimeCHProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[15]->Print(name); }
  //TT by TT timing profile
  c[67]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *chhistEEPN = HistNFromTProfile2D(NEEPtimeCHProfile,"chhistEEPN",200, -30., 40.);
  sprintf(mytitle,"%s Average CH Timing (EE+);CH time average (ns)",runChar); 
  chhistEEPN->SetTitle(mytitle);
  chhistEEPN->GetXaxis()->SetNdivisions(512);
  if (chhistEEPN->GetMean() != 0 && fit ) chhistEEPN->Fit("gaus");
  c[67]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPCHTIME_%i.%s",dirName,mType,runNumber,fileType); c[67]->Print(name); }

  c[15]->cd();
  gStyle->SetOptStat(10);
  Scale0TProfile2D(NEEPtimeTTProfile);
  NEEPtimeTTProfile->Draw("colz");
  sprintf(mytitle,"%s in ns",EEPtimeTTProfile->GetTitle()); 
  NEEPtimeTTProfile->SetTitle(mytitle);
  NEEPtimeTTProfile->SetMinimum(tlowc);
  NEEPtimeTTProfile->SetMaximum(thighc);

  NEEPtimeTTProfile->GetXaxis()->SetNdivisions(18);
  c[15]->SetLogy(0);
  c[15]->SetLogz(0);
  drawEELines();
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPtimeTTProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[15]->Print(name); }
  //TT by TT timing profile
  c[67]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *tthistEEPN = HistNFromTProfile2D(NEEPtimeTTProfile,"tthistEEPN",200, -30., 40.);
  sprintf(mytitle,"%s Average TT Timing (EE+);TT time average (ns)",runChar); 
  tthistEEPN->SetTitle(mytitle);
  tthistEEPN->GetXaxis()->SetNdivisions(512);
  if (tthistEEPN->GetMean() != 0 && fit ) tthistEEPN->Fit("gaus");
  c[67]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPTTTIME_%i.%s",dirName,mType,runNumber,fileType); c[67]->Print(name); }

  c[15]->cd();
  gStyle->SetOptStat(10);
  Scale0TProfile2D(NEEMtimeCHProfile);
  NEEMtimeCHProfile->Draw("colz");
  sprintf(mytitle,"%s in ns",EEMtimeCHProfile->GetTitle()); 
  NEEMtimeCHProfile->SetTitle(mytitle);
  NEEMtimeCHProfile->SetMinimum(tlowc);
  NEEMtimeCHProfile->SetMaximum(thighc);

  NEEMtimeCHProfile->GetXaxis()->SetNdivisions(18);
  //NEEMtimeCHProfile->GetYaxis()->SetNdivisions(2);
  c[15]->SetLogy(0);
  c[15]->SetLogz(0);
  drawEELines();
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMtimeCHProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[15]->Print(name); }
  //TT by TT timing profile
  c[67]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *chhistEEMN = HistNFromTProfile2D(NEEMtimeCHProfile,"chhistEEMN",200, -30., 40.);
  sprintf(mytitle,"%s Average CH Timing (EE-);CH time average (ns)",runChar); 
  chhistEEMN->SetTitle(mytitle);
  chhistEEMN->GetXaxis()->SetNdivisions(512);
  if (chhistEEMN->GetMean() != 0 && fit ) chhistEEMN->Fit("gaus");
  c[67]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMCHTIME_%i.%s",dirName,mType,runNumber,fileType); c[67]->Print(name); }

  c[15]->cd();
  gStyle->SetOptStat(10);
  Scale0TProfile2D(NEEMtimeTTProfile);
  NEEMtimeTTProfile->Draw("colz");
  sprintf(mytitle,"%s in ns",EEMtimeTTProfile->GetTitle()); 
  NEEMtimeTTProfile->SetTitle(mytitle);
  NEEMtimeTTProfile->SetMinimum(tlowc);
  NEEMtimeTTProfile->SetMaximum(thighc);

  NEEMtimeTTProfile->GetXaxis()->SetNdivisions(18);
  //NEEMtimeTTProfile->GetYaxis()->SetNdivisions(2);
  c[15]->SetLogy(0);
  c[15]->SetLogz(0);
  drawEELines();
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMtimeTTProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[15]->Print(name); }
  //TT by TT timing profile
  c[67]->cd();
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111);
  TH1F *tthistEEMN = HistNFromTProfile2D(NEEMtimeTTProfile,"tthistEEMN",200, -30., 40.);
  sprintf(mytitle,"%s Average TT Timing (EE-);TT time average (ns)",runChar); 
  tthistEEMN->SetTitle(mytitle);
  tthistEEMN->GetXaxis()->SetNdivisions(512);
  if (tthistEEMN->GetMean() != 0 && fit ) tthistEEMN->Fit("gaus");
  c[67]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMTTTIME_%i.%s",dirName,mType,runNumber,fileType); c[67]->Print(name); }


  cout << name << endl;

  cout << " The Starting Time is " << habsTime->GetXaxis()->GetXmin() << endl;
  return (habsTime->GetXaxis()->GetXmin()+ 1215192037.0); 

}

void drawEELines() {

  int ixSectorsEE[202] = {61, 61, 60, 60, 59, 59, 58, 58, 57, 57, 55, 55, 45, 45, 43, 43, 42, 42, 41, 41, 40, 40, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 45, 45, 55, 55, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 0,100,100, 97, 97, 95, 95, 92, 92, 87, 87, 85, 85, 80, 80, 75, 75, 65, 65, 60, 60, 40, 40, 35, 35, 25, 25, 20, 20, 15, 15, 13, 13,  8,  8,  5,  5,  3,  3,  0,  0,  3,  3,  5,  5,  8,  8, 13, 13, 15, 15, 20, 20, 25, 25, 35, 35, 40, 40, 60, 60, 65, 65, 75, 75, 80, 80, 85, 85, 87, 87, 92, 92, 95, 95, 97, 97,100,100,  0, 61, 65, 65, 70, 70, 80, 80, 90, 90, 92,  0, 61, 65, 65, 90, 90, 97,  0, 57, 60, 60, 65, 65, 70, 70, 75, 75, 80, 80,  0, 50, 50,  0, 43, 40, 40, 35, 35, 30, 30, 25, 25, 20, 20,  0, 39, 35, 35, 10, 10,  3,  0, 39, 35, 35, 30, 30, 20, 20, 10, 10,  8,  0, 45, 45, 40, 40, 35, 35,  0, 55, 55, 60, 60, 65, 65};
 
  int iySectorsEE[202] = {50, 55, 55, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 60, 60, 59, 59, 58, 58, 57, 57, 55, 55, 45, 45, 43, 43, 42, 42, 41, 41, 40, 40, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 45, 45, 50,  0, 50, 60, 60, 65, 65, 75, 75, 80, 80, 85, 85, 87, 87, 92, 92, 95, 95, 97, 97,100,100, 97, 97, 95, 95, 92, 92, 87, 87, 85, 85, 80, 80, 75, 75, 65, 65, 60, 60, 40, 40, 35, 35, 25, 25, 20, 20, 15, 15, 13, 13,  8,  8,  5,  5,  3,  3,  0,  0,  3,  3,  5,  5,  8,  8, 13, 13, 15, 15, 20, 20, 25, 25, 35, 35, 40, 40, 50,  0, 45, 45, 40, 40, 35, 35, 30, 30, 25, 25,  0, 50, 50, 55, 55, 60, 60,  0, 60, 60, 65, 65, 70, 70, 75, 75, 85, 85, 87,  0, 61,100,  0, 60, 60, 65, 65, 70, 70, 75, 75, 85, 85, 87,  0, 50, 50, 55, 55, 60, 60,  0, 45, 45, 40, 40, 35, 35, 30, 30, 25, 25,  0, 39, 30, 30, 15, 15,  5,  0, 39, 30, 30, 15, 15,  5};


 for ( int i=0; i<202; i++) {
   ixSectorsEE[i] += 1;
   iySectorsEE[i] += 1;
//   std::cout << i << " " << ixSectorsEE[i] << " " << iySectorsEE[i] << std::endl;
 }

 TLine l;
 l.SetLineWidth(1);
 for ( int i=0; i<201; i=i+1) {
   if ( (ixSectorsEE[i]!=1 || iySectorsEE[i]!=1) && 
	(ixSectorsEE[i+1]!=1 || iySectorsEE[i+1]!=1) ) {
     l.DrawLine(ixSectorsEE[i], iySectorsEE[i], 
		ixSectorsEE[i+1], iySectorsEE[i+1]);
   }
 }

}

void customizeTProfile (TProfile* myTProfile) {
  if (myTProfile) {
    myTProfile->SetLineWidth(2);
    myTProfile->SetMarkerStyle(kFullCircle);
    myTProfile->SetMarkerSize(0.7);
  }
}

void customizeTHist (TH1F* myTHist) {
  if (myTHist) {
    myTHist->SetLineWidth(2);
    myTHist->SetMarkerStyle(kFullCircle);
    myTHist->SetMarkerSize(0.7);
  }
}


TH1F* CorrectProfToHist(TProfile *prof, const char * histname, double numb=0, double myScale = 1.0 )
{
  TH1F *temphist = new TH1F(histname,histname,prof->GetNbinsX(),prof->GetXaxis()->GetXmin(),prof->GetXaxis()->GetXmax());
  for (int i = 1; i < prof->GetNbinsX()+1; ++i)
  {
    //std::cout << " bin " << i << " is " << temphist->GetBinContent(i) << std::endl;
    if (prof->GetBinEntries(i) > 0 )
        {
          temphist->SetBinContent(i,prof->GetBinContent(i)+numb);
          temphist->SetBinError(i,prof->GetBinError(i));
        }
	else {temphist->SetBinContent(i,-100.);}
        //std::cout << " bin " << i  << " bin content before " << prof->GetBinContent(i) << " bin entries " << prof->GetBinEntries(i)  << " new bi\ncontent " << temphist->GetBinContent(i) << std::endl;
  }
  temphist->Sumw2();
  temphist->Scale(myScale);
  return temphist;
}

void ScaleTProfile2D(TProfile2D* myprof, Double_t myfac, Double_t myscale)
{
int nxbins = myprof->GetNbinsX();
int nybins = myprof->GetNbinsY();

for (int i=0; i<=(nxbins+2)*(nybins+2); i++ ) {   
       Double_t oldcont = myprof->GetBinContent(i);
       Double_t binents = myprof->GetBinEntries(i);
       if (binents == 0 ) {binents =1.;myprof->SetBinEntries(i,1); }
       myprof->SetBinContent(i,myscale*(oldcont+myfac)*binents);
}
}


void Scale0TProfile2D(TProfile2D* myprof)
{
int nxbins = myprof->GetNbinsX();
int nybins = myprof->GetNbinsY();

for (int i=0; i<=(nxbins+2)*(nybins+2); i++ ) {   
       Double_t oldcont = myprof->GetBinContent(i);
       Double_t binents = myprof->GetBinEntries(i);
       if (binents == 0 ) {binents =1.;myprof->SetBinEntries(i,1);myprof->SetBinContent(i,-150.); }
}
}


TProfile2D* TProfToRelProf2D(TProfile2D *prof, const char * histname, double numb=0, double myScale = 1.0)
{
TProfile2D *myprof = prof->Clone(histname);
ScaleTProfile2D(myprof,numb,myScale);

return myprof;
}

TProfile* TProfToRelProf(TProfile *prof, const char * histname, double numb=0, double myScale = 1.0)
{
TProfile *myprof = prof->Clone(histname);
ScaleTProfile(myprof,numb,myScale);

return myprof;
}

void ScaleTProfile(TProfile* myprof, Double_t myfac, Double_t myscale)
{
int nxbins = myprof->GetNbinsX();

for (int i=1; i<(nxbins+1); i++ ) {   
       Double_t oldcont = myprof->GetBinContent(i);
       Double_t binents = myprof->GetBinEntries(i);
       Double_t binerrr = myprof->GetBinError(i);
       
       if (binents == 0 ) { continue; /*binents =1.;myprof->SetBinEntries(i,1);*/ }
       myprof->SetBinContent(i,myscale*(oldcont+myfac)*binents);
	   Double_t newentries = myprof->GetBinEntries(i);
	   Double_t newcont = myprof->GetBinContent(i);
	   //cout << " cont " << oldcont << " ent " << binents << " err " << binerrr << " new err " << myprof->GetBinError(i);
	   if ( newentries == 1) { myprof->SetBinError(i,5+fabs(myprof->GetBinContent(i)-myprof->GetBinContent(i)/2.5));}
           //else {myprof->SetBinError(i,binerrr*myscale+1.0);}
	   //cout << " newnew " << myprof->GetBinError(i) << std::endl;
	   if (newentries != binents) {std::cout << "NONONO" << std::endl;}
}
}

void EntryProfileFromTProfile2D(TProfile2D* myprof)
{
int nxbins = myprof->GetNbinsX();
int nybins = myprof->GetNbinsY();

for (int i=0; i<=(nxbins+2)*(nybins+2); i++ ) {   
       Double_t oldcont = myprof->GetBinContent(i);
       Double_t binents = myprof->GetBinEntries(i);
       if (binents == 0 ) { continue; }
       myprof->SetBinContent(i,binents*binents);
}
}


TProfile2D* TProfile2DOccupancyFromProf2D(TProfile2D *prof, const char * histname)
{
TProfile2D *myprof = prof->Clone(histname);
EntryProfileFromTProfile2D(myprof);
return myprof;
}

TH1F* HistFromTProfile2D(TProfile2D *prof, const char * histname, Int_t xbins, Double_t xmin, Double_t xmax, Double_t myfac, Double_t myscale)
{
int nxbins = prof->GetNbinsX();
int nybins = prof->GetNbinsY();

TH1F *temphist = new TH1F(histname,histname,xbins,xmin,xmax);

for (int i=0; i<=(nxbins+2)*(nybins+2); i++ ) {   
       Double_t oldcont = prof->GetBinContent(i);
       Double_t binents = prof->GetBinEntries(i);
       if (binents == 0 ) { continue; }
       temphist->Fill((oldcont+myfac)*myscale);
}
return temphist;
}

TH1F* HistNFromTProfile2D(TProfile2D *prof, const char * histname, Int_t xbins, Double_t xmin, Double_t xmax)
{
int nxbins = prof->GetNbinsX();
int nybins = prof->GetNbinsY();

TH1F *temphist = new TH1F(histname,histname,xbins,xmin,xmax);

for (int i=0; i<=(nxbins+2)*(nybins+2); i++ ) {   
       Double_t oldcont = prof->GetBinContent(i);
       Double_t binents = prof->GetBinEntries(i);
       if (binents == 0 ) { continue; }
       if (binents == 1 && oldcont < -99) {continue;}
       temphist->Fill(oldcont);
}
return temphist;
}

TProfile2D* NewTProfile2D(TProfile2D *prof, const char * histname)
{

TProfile2D *myprof = prof->Clone(histname);
EmptyTProfile2D(myprof);
return myprof;
}

void EmptyTProfile2D(TProfile2D* myprof)
{
int nxbins = myprof->GetNbinsX();
int nybins = myprof->GetNbinsY();

for (int i=0; i<=(nxbins+2)*(nybins+2); i++ ) {   
       Double_t oldcont = myprof->GetBinContent(i);
       Double_t binents = myprof->GetBinEntries(i);
       if (binents == 0 ) { continue; }
       myprof->SetBinContent(i,0);
       myprof->SetBinEntries(i,0);
}
}




