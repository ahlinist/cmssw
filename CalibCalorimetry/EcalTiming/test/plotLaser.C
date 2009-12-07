int Wait() {
     cout << " Continue [<RET>|q]?  "; 
     char x;
     x = getchar();
     if ((x == 'q') || (x == 'Q')) return 1;
     return 0;
}

void DrawLaserPlots(Char_t* infile = 0, Int_t runNum=0, Bool_t printPics = kTRUE, Char_t* fileType = "png", Char_t* dirName = ".", Bool_t doWait=kFALSE, Char_t* mType = "Laser")
{

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


  char name[100];  

  const int nHists1=50;
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
  
//First thing is do print the profiles

  //Timing by FED/SM  
  c[0]->cd();
  gStyle->SetOptStat(10);
  TProfile *SM_timing = (TProfile*) f->Get("SM_timing");
  customizeTProfile(SM_timing);
  SM_timing->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,SM_timing->GetTitle()); 
  SM_timing->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_SM_timing_%i.%s",dirName,mType,runNumber,fileType); c[0]->Print(name); }

  c[1]->cd();
  gStyle->SetOptStat(10);
  TH1F *SM_timingh = CorrectProfToHist(SM_timing,"SM_timingh",-5,25.0);
  customizeTHist(SM_timingh);
  SM_timingh->Draw("p");
  char mytitle[100]; sprintf(mytitle,"%s %s to optimal;FED;Time (ns)",runChar,SM_timing->GetTitle()); 
  SM_timingh->SetMinimum(-30.);
  SM_timingh->SetMaximum(50.);
  SM_timingh->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_SM_timingCorrected_%i.%s",dirName,mType,runNumber,fileType); c[1]->Print(name); }

  //Timing by LM
  c[2]->cd();
  gStyle->SetOptStat(10);
  TProfile *LM_timing = (TProfile*) f->Get("LM_timing");
  customizeTProfile(LM_timing);
  LM_timing->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,LM_timing->GetTitle()); 
  LM_timing->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_LM_timing_%i.%s",dirName,mType,runNumber,fileType); c[2]->Print(name); }

  c[3]->cd();
  gStyle->SetOptStat(10);
  TH1F *LM_timingh = CorrectProfToHist(LM_timing,"LM_timingh",-5,25.0);
  customizeTHist(LM_timingh);
  LM_timingh->Draw("p");
  char mytitle[100]; sprintf(mytitle,"%s %s to optimal;FED;Time (ns)",runChar,LM_timing->GetTitle()); 
  LM_timingh->SetMinimum(-30.);
  LM_timingh->SetMaximum(50.);
  LM_timingh->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_LM_timingCorrected_%i.%s",dirName,mType,runNumber,fileType); c[3]->Print(name); }
 

  //Timing within the towers
  c[4]->cd();
  gStyle->SetOptStat(1111);
  TProfile *Inside_TT_timing = (TProfile*) f->Get("Inside_TT_timing");
  Inside_TT_timing->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,Inside_TT_timing->GetTitle()); 
  Inside_TT_timing->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_Inside_TT_timing_%i.%s",dirName,mType,runNumber,fileType); c[4]->Print(name); }
 
  //Eta Profiles by TT
  c[5]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeTTAllFEDsEta = (TProfile*) f->Get("timeTTAllFEDsEta");
  timeTTAllFEDsEta->Draw();
  //timeTTAllFEDsEta->SetMinimum(4.95);
  //timeTTAllFEDsEta->SetMaximum(5.05);
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeTTAllFEDsEta->GetTitle()); 
  timeTTAllFEDsEta->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeTTAllFEDsEta_%i.%s",dirName,mType,runNumber,fileType); c[5]->Print(name); }
  
  c[6]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeTTAllFEDsEtaEEP = (TProfile*) f->Get("timeTTAllFEDsEtaEEP");
  timeTTAllFEDsEtaEEP->Draw();
  //timeTTAllFEDsEtaEEP->SetMinimum(4.5);
  //timeTTAllFEDsEtaEEP->SetMaximum(5.5);
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeTTAllFEDsEtaEEP->GetTitle()); 
  timeTTAllFEDsEtaEEP->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeTTAllFEDsEtaEEP_%i.%s",dirName,mType,runNumber,fileType); c[6]->Print(name); }
    
  c[7]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeTTAllFEDsEtaEEM = (TProfile*) f->Get("timeTTAllFEDsEtaEEM");
  timeTTAllFEDsEtaEEM->Draw();
  //timeTTAllFEDsEtaEEM->SetMinimum(4.5);
  //timeTTAllFEDsEtaEEM->SetMaximum(5.5);
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeTTAllFEDsEtaEEM->GetTitle()); 
  timeTTAllFEDsEtaEEM->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeTTAllFEDsEtaEEM_%i.%s",dirName,mType,runNumber,fileType); c[7]->Print(name); }
  
  //Eta profile by Ch
  c[8]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeCHAllFEDsEta = (TProfile*) f->Get("timeCHAllFEDsEta");
  timeCHAllFEDsEta->Draw();
  //timeCHAllFEDsEta->SetMinimum(4.8);
  //timeCHAllFEDsEta->SetMaximum(5.2);
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeCHAllFEDsEta->GetTitle()); 
  timeCHAllFEDsEta->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeCHAllFEDsEta_%i.%s",dirName,mType,runNumber,fileType); c[8]->Print(name); }
  
//1-D Histograms
  c[9]->cd();
  gStyle->SetOptStat(111110);
  TH1F *Rel_TimingSigma = (TH1F*) f->Get("Rel_TimingSigma");
  Rel_TimingSigma->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,Rel_TimingSigma->GetTitle()); 
  Rel_TimingSigma->SetTitle(mytitle);
  c[9]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_Rel_TimingSigma_%i.%s",dirName,mType,runNumber,fileType); c[9]->Print(name); }
  
  c[10]->cd();
  gStyle->SetOptStat(111110);
  TH1F *XtalsPerEvt = (TH1F*) f->Get("XtalsPerEvt");
  XtalsPerEvt->Draw();
  //XtalsPerEvt->GetXaxis()->SetRangeUser(0,100);
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,XtalsPerEvt->GetTitle()); 
  XtalsPerEvt->SetTitle(mytitle);
  //c[10]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_XtalsPerEvt_%i.%s",dirName,mType,runNumber,fileType); c[10]->Print(name); }
  
  c[11]->cd();
  gStyle->SetOptStat(111110);
  TH1F *laserShift = (TH1F*) f->Get("laserShift");
  laserShift->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,laserShift->GetTitle()); 
  laserShift->SetTitle(mytitle);
  c[11]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_laserShift_%i.%s",dirName,mType,runNumber,fileType); c[11]->Print(name); }
  
//2-D Histogram
  c[12]->cd();
  gStyle->SetOptStat(111110);
  TH2F *RelRMS_vs_AbsTime = (TH2F*) f->Get("RelRMS_vs_AbsTime");
  RelRMS_vs_AbsTime->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,RelRMS_vs_AbsTime->GetTitle()); 
  RelRMS_vs_AbsTime->SetTitle(mytitle);
  c[12]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_RelRMS_vs_AbsTime_%i.%s",dirName,mType,runNumber,fileType); c[12]->Print(name); }
  
//1-D TGraphs  
  c[13]->cd();
  gStyle->SetOptStat(111110);
  TGraph *TTMeanWithRMS_All_FEDS = (TGraph*) f->Get("TTMeanWithRMS_All_FEDS");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,TTMeanWithRMS_All_FEDS->GetTitle()); 
  TTMeanWithRMS_All_FEDS->SetTitle(mytitle);
  TTMeanWithRMS_All_FEDS->GetYaxis()->SetLimits(0.,9.);
  //TTMeanWithRMS_All_FEDS->GetYaxis()->SetLimits(4.,6.);
  TTMeanWithRMS_All_FEDS->GetYaxis()->UnZoom();
  TTMeanWithRMS_All_FEDS->Draw("AP*");
  //c[13]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_TTMeanWithRMS_All_FEDS_%i.%s",dirName,mType,runNumber,fileType); c[13]->Print(name); }
  
  c[14]->cd();
  gStyle->SetOptStat(111110);
  TGraph *TTMeanWithRMS_All_FEDS_CHANGED = (TGraph*) f->Get("TTMeanWithRMS_All_FEDS_CHANGED");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,TTMeanWithRMS_All_FEDS_CHANGED->GetTitle()); 
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
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeCHProfile->GetTitle()); 
  timeCHProfile->SetTitle(mytitle);
  timeCHProfile->SetMinimum(4.0);
  //timeCHProfile->SetMinimum(4.5);
  //timeCHProfile->SetMaximum(5.5);
  timeCHProfile->GetXaxis()->SetNdivisions(-18);
  timeCHProfile->GetYaxis()->SetNdivisions(2);
  c[15]->SetLogy(0);
  c[15]->SetLogz(0);
  c[15]->SetGridx(1);
  c[15]->SetGridy(1);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeCHProfile_%i.%s",dirName,mType,runNumber,fileType); c[15]->Print(name); }
  
   c[15]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *timeCHProfilep = TProfToRelProf2D(timeCHProfile,"timeCHProfilep", -5., 25.);
  timeCHProfilep->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s in ns",timeCHProfile->GetTitle()); 
  timeCHProfilep->SetTitle(mytitle);
  timeCHProfilep->SetMinimum(-30.);
  timeCHProfilep->SetMaximum(50.);
  //timeTTProfile->SetMinimum(5.85);
  //timeTTProfile->SetMinimum(4.8);
  //timeTTProfile->SetMaximum(6.45);
  timeCHProfilep->GetXaxis()->SetNdivisions(-18);
  timeCHProfilep->GetYaxis()->SetNdivisions(2);
  c[15]->SetLogy(0);
  c[15]->SetLogz(0);
  c[15]->SetGridx(1);
  c[15]->SetGridy(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeCHProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[15]->Print(name); }
  //TT by TT timing profile
  c[16]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *timeTTProfile = (TProfile2D*) f->Get("timeTTProfile");
  timeTTProfile->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeTTProfile->GetTitle()); 
  timeTTProfile->SetTitle(mytitle);
  timeTTProfile->SetMinimum(4.0);
  //timeTTProfile->SetMinimum(5.85);
  //timeTTProfile->SetMinimum(4.8);
  //timeTTProfile->SetMaximum(6.45);
  timeTTProfile->GetXaxis()->SetNdivisions(-18);
  timeTTProfile->GetYaxis()->SetNdivisions(2);
  c[16]->SetLogy(0);
  c[16]->SetLogz(0);
  c[16]->SetGridx(1);
  c[16]->SetGridy(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeTTProfile_%i.%s",dirName,mType,runNumber,fileType); c[16]->Print(name); }
 
  c[16]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *timeTTProfilep = TProfToRelProf2D(timeTTProfile,"timeTTProfilep", -5., 25.);
  timeTTProfilep->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s in ns",timeTTProfile->GetTitle()); 
  timeTTProfilep->SetTitle(mytitle);
  timeTTProfilep->SetMinimum(-30.);
  timeTTProfilep->SetMaximum(50.);
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
 
  //Ch by Ch timing profile EE+
  c[17]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeCHProfile = (TProfile2D*) f->Get("EEPtimeCHProfile");
  EEPtimeCHProfile->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,EEPtimeCHProfile->GetTitle()); 
  EEPtimeCHProfile->SetTitle(mytitle);
  EEPtimeCHProfile->SetMinimum(4.0);
  //EEPtimeCHProfile->SetMinimum(4.5);
  //EEPtimeCHProfile->SetMaximum(5.5);
  EEPtimeCHProfile->GetXaxis()->SetNdivisions(18);
  c[17]->SetLogy(0);
  c[17]->SetLogz(0);
  drawEELines();
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPtimeCHProfile_%i.%s",dirName,mType,runNumber,fileType); c[17]->Print(name); }
  
  c[17]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeCHProfilep = TProfToRelProf2D(EEPtimeCHProfile,"EEPtimeCHProfilep", -5., 25.);
  EEPtimeCHProfilep->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s in ns",EEPtimeCHProfilep->GetTitle()); 
  EEPtimeCHProfilep->SetTitle(mytitle);
  EEPtimeCHProfilep->SetMinimum(-30.);
  EEPtimeCHProfilep->SetMaximum(50.);
  //EEPtimeCHProfile->SetMinimum(4.5);
  //EEPtimeCHProfile->SetMaximum(5.5);
  EEPtimeCHProfilep->GetXaxis()->SetNdivisions(18);
  c[17]->SetLogy(0);
  c[17]->SetLogz(0);
  drawEELines();
  //c[15]->SetLogz(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPtimeCHProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[17]->Print(name); }
   
  //Ch by Ch timing profile EE+
  c[18]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeCHProfile = (TProfile2D*) f->Get("EEMtimeCHProfile");
  EEMtimeCHProfile->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,EEMtimeCHProfile->GetTitle()); 
  EEMtimeCHProfile->SetTitle(mytitle);
  EEMtimeCHProfile->SetMinimum(4.);
  //EEMtimeCHProfile->SetMinimum(4.5);
  //EEMtimeCHProfile->SetMaximum(5.5);
  EEMtimeCHProfile->GetXaxis()->SetNdivisions(18);
  c[18]->SetLogy(0);
  c[18]->SetLogz(0);
  drawEELines();
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMtimeCHProfile_%i.%s",dirName,mType,runNumber,fileType); c[18]->Print(name); }
 
 c[18]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeCHProfilep = TProfToRelProf2D(EEMtimeCHProfile,"EEMtimeCHProfilep", -5., 25.);
  EEMtimeCHProfilep->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s in ns",EEMtimeCHProfilep->GetTitle()); 
  EEMtimeCHProfilep->SetTitle(mytitle);
  EEMtimeCHProfilep->SetMinimum(-30.);
  EEMtimeCHProfilep->SetMaximum(50.);
  //EEPtimeCHProfile->SetMinimum(4.5);
  //EEPtimeCHProfile->SetMaximum(5.5);
  EEMtimeCHProfilep->GetXaxis()->SetNdivisions(18);
  c[18]->SetLogy(0);
  c[18]->SetLogz(0);
  drawEELines();
  //c[15]->SetLogz(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMtimeCHProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[18]->Print(name); } 
  
  //TT by TT timing profile EE+
  c[19]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeTTProfile = (TProfile2D*) f->Get("EEPtimeTTProfile");
  EEPtimeTTProfile->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,EEPtimeTTProfile->GetTitle()); 
  EEPtimeTTProfile->SetTitle(mytitle);
  EEPtimeTTProfile->SetMinimum(4.);
  //EEPtimeTTProfile->SetMinimum(4.5);
  //EEPtimeTTProfile->SetMaximum(5.5);
  EEPtimeTTProfile->GetXaxis()->SetNdivisions(18);
  c[19]->SetLogy(0);
  c[19]->SetLogz(0);
  drawEELines();
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEPtimeTTProfile_%i.%s",dirName,mType,runNumber,fileType); c[19]->Print(name); }
 

  c[19]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeTTProfilep = TProfToRelProf2D(EEPtimeTTProfile,"EEPtimeTTProfilep", -5., 25.);
  EEPtimeTTProfilep->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s in ns",EEPtimeTTProfile->GetTitle()); 
  EEPtimeTTProfilep->SetTitle(mytitle);
  EEPtimeTTProfilep->SetMinimum(-30.);
  EEPtimeTTProfilep->SetMaximum(50.);
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
  
  //TT by TT timing profile EE-
  c[20]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeTTProfile = (TProfile2D*) f->Get("EEMtimeTTProfile");
  EEMtimeTTProfile->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,EEMtimeTTProfile->GetTitle()); 
  EEMtimeTTProfile->SetTitle(mytitle);
  EEMtimeTTProfile->SetMinimum(4.);
  //EEMtimeTTProfile->SetMinimum(4.5);
  //EEMtimeTTProfile->SetMaximum(5.5);
  EEMtimeTTProfile->GetXaxis()->SetNdivisions(18);
  c[20]->SetLogy(0);
  c[20]->SetLogz(0);
  drawEELines();
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMtimeTTProfile_%i.%s",dirName,mType,runNumber,fileType); c[20]->Print(name); }
  
  c[20]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeTTProfilep = TProfToRelProf2D(EEMtimeTTProfile,"EEMtimeTTProfilep", -5., 25.);
  EEMtimeTTProfilep->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s in ns",EEMtimeTTProfile->GetTitle()); 
  EEMtimeTTProfilep->SetTitle(mytitle);
  EEMtimeTTProfilep->SetMinimum(-30.);
  EEMtimeTTProfilep->SetMaximum(50.);
  //timeTTProfile->SetMinimum(5.85);
  //timeTTProfile->SetMinimum(4.8);
  //timeTTProfile->SetMaximum(6.45);
  EEMtimeTTProfilep->GetXaxis()->SetNdivisions(18);
  //EEMtimeTTProfilep->GetYaxis()->SetNdivisions(2);
  c[20]->SetLogy(0);
  c[20]->SetLogz(0);
  drawEELines();
  //c[20]->SetGridx(1);
  //c[20]->SetGridy(1);
  gStyle->SetOptStat(0);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEMtimeTTProfileRel_%i.%s",dirName,mType,runNumber,fileType); c[20]->Print(name); }
  
  //Amplitude Profiles
  c[21]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *fullAmpProfileEB = (TProfile2D*) f->Get("fullAmpProfileEB");
  fullAmpProfileEB->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,fullAmpProfileEB->GetTitle()); 
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
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,fullAmpProfileEEP->GetTitle()); 
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
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,fullAmpProfileEEM->GetTitle()); 
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
  timeTTAllFEDsEtap->SetMinimum(-30.);
  timeTTAllFEDsEtap->SetMaximum(50.);
  char mytitle[100]; sprintf(mytitle,"%s to optimal;i#eta;Time (ns)",timeTTAllFEDsEta->GetTitle()); 
  timeTTAllFEDsEtap->SetTitle(mytitle);
  gStyle->SetOptStat(100);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeTTAllFEDsEtaRel_%i.%s",dirName,mType,runNumber,fileType); c[24]->Print(name); }
  
  c[25]->cd();
  TProfile *timeTTAllFEDsEtaEEPp =  TProfToRelProf(timeTTAllFEDsEtaEEP,"timeTTAllFEDsEtaEEPp",-5,25.);
  customizeTProfile(timeTTAllFEDsEtaEEPp);
  timeTTAllFEDsEtaEEPp->Draw("p");
  timeTTAllFEDsEtaEEPp->SetMinimum(-30.);
  timeTTAllFEDsEtaEEPp->SetMaximum(50.);
  char mytitle[100]; sprintf(mytitle,"%s to optimal;i#eta;Time (ns)",timeTTAllFEDsEtaEEP->GetTitle()); 
  timeTTAllFEDsEtaEEPp->SetTitle(mytitle);
  gStyle->SetOptStat(100);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeTTAllFEDsEtaEEPRel_%i.%s",dirName,mType,runNumber,fileType); c[25]->Print(name); }
  
  c[26]->cd();
  TProfile *timeTTAllFEDsEtaEEMp =  TProfToRelProf(timeTTAllFEDsEtaEEM,"timeTTAllFEDsEtaEEMp",-5,25.);
  customizeTProfile(timeTTAllFEDsEtaEEMp);
  timeTTAllFEDsEtaEEMp->Draw("p");
  timeTTAllFEDsEtaEEMp->SetMinimum(-30.);
  timeTTAllFEDsEtaEEMp->SetMaximum(50.);
  char mytitle[100]; sprintf(mytitle,"%s to optimal;i#eta;Time (ns)",timeTTAllFEDsEtaEEM->GetTitle()); 
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
  timeCHAllFEDsEtap->SetMinimum(-30.);
  timeCHAllFEDsEtap->SetMaximum(50.);
  char mytitle[100]; sprintf(mytitle,"%s to optimal;i#eta;Time (ns)",timeCHAllFEDsEta->GetTitle()); 
  timeCHAllFEDsEtap->SetTitle(mytitle);
  gStyle->SetOptStat(100);
  if (printPics) { sprintf(name,"%s/%sAnalysis_timeCHAllFEDsEtaRel_%i.%s",dirName,mType,runNumber,fileType); c[27]->Print(name); }
 
 
  //Now it is time to see if the timing tree is there and use the individual ntuple information
  TTree* eventTimingInfoTree = ((TTree*) f->Get("eventTimingInfoTree"));
  if (!eventTimingInfoTree) { std::cout << " No TTree in the event, probalby expected" << std::endl; cout << name << endl; return;} 
  //Now, we will only do the below if there is a TTree in the event.
  c[28]->cd();
  gStyle->SetOptStat(110);
  eventTimingInfoTree->Draw("(crystalTimesEB-5.)*25. >> hctEB(52,-26.,26.)");
  char mytitle[100]; sprintf(mytitle,"%s EB Crystal Times;Time (ns);Number of Crystals",runChar); 
  hctEB->SetTitle(mytitle);
  hctEB->GetXaxis()->SetNdivisions(512);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTIMES_%i.%s",dirName,mType,runNumber,fileType); c[28]->Print(name); }

  
  c[29]->cd();
  gStyle->SetOptStat(1110);
  eventTimingInfoTree->Draw("(crystalTimesEE-5.)*25. >> hctEE(52,-26.,26.)");
  char mytitle[100]; sprintf(mytitle,"%s EE Crystal Times;Time (ns);Number of Crystals",runChar); 
  hctEE->SetTitle(mytitle);
  hctEE->GetXaxis()->SetNdivisions(512);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EETIMES_%i.%s",dirName,mType,runNumber,fileType); c[29]->Print(name); }
  
  //Time to average event time
  c[30]->cd();
  gStyle->SetOptStat(1110);
  eventTimingInfoTree->Draw("(correctionToSampleEB-5.)*25.:(crystalTimesEE-5.)*25. >> hctEEtoAve(52,-26.,26.,52,-26.,26.)","","COLZ");
  char mytitle[100]; sprintf(mytitle,"%s EE Crystal Times to Average Time;Crystal Time (ns);Average EB Event Time (ns)",runChar); 
  //hctEEtoAve->Draw;
  hctEEtoAve->SetTitle(mytitle);
  hctEEtoAve->GetXaxis()->SetNdivisions(512);
  hctEEtoAve->GetYaxis()->SetNdivisions(512);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EETIMEStoAverage_%i.%s",dirName,mType,runNumber,fileType); c[30]->Print(name); }
  c[31]->cd();
  gStyle->SetOptStat(1110);
  eventTimingInfoTree->Draw("(correctionToSampleEB-5.)*25.:(crystalTimesEB-5.)*25. >> hctEBtoAve(52,-26.,26.,52,-26.,26.)","","COLZ");
  char mytitle[100]; sprintf(mytitle,"%s EB Crystal Times to Average Time;Crystal Time (ns);Average EB Event Time (ns)",runChar); 
  //hctEEtoAve->Draw;
  hctEBtoAve->SetTitle(mytitle);
  hctEBtoAve->GetXaxis()->SetNdivisions(512);
  hctEBtoAve->GetYaxis()->SetNdivisions(512);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTIMEStoAverage_%i.%s",dirName,mType,runNumber,fileType); c[31]->Print(name); }
  
  //Time to Time error
  c[32]->cd();
  gStyle->SetOptStat(1110);
  eventTimingInfoTree->Draw("(crystalTimeErrorsEB)*25.:(crystalTimesEB-5.)*25. >> hctEBtoTerr(52,-26.,26.,26,0.,26.)","","COLZ");
  char mytitle[100]; sprintf(mytitle,"%s EB Crystal Times to Time Error;Crystal Time (ns);Crystal Time Error (ns)",runChar); 
  hctEBtoTerr->SetTitle(mytitle);
  hctEBtoTerr->GetXaxis()->SetNdivisions(512);
  hctEBtoTerr->GetYaxis()->SetNdivisions(507);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTIMEStoTERR_%i.%s",dirName,mType,runNumber,fileType); c[32]->Print(name); }
  c[33]->cd();
  gStyle->SetOptStat(1110);
  eventTimingInfoTree->Draw("(crystalTimeErrorsEE)*25.:(crystalTimesEE-5.)*25. >> hctEEtoTerr(52,-26.,26.,26,0.,26.)","","COLZ");
  char mytitle[100]; sprintf(mytitle,"%s EE Crystal Times to Time Error;Crystal Time (ns);Crystal Time Error (ns)",runChar); 
  hctEEtoTerr->SetTitle(mytitle);
  hctEEtoTerr->GetXaxis()->SetNdivisions(512);
  hctEEtoTerr->GetYaxis()->SetNdivisions(507);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EETIMEStoTERR_%i.%s",dirName,mType,runNumber,fileType); c[33]->Print(name); }
  
  //Amplitude to time
  c[34]->cd();
  gStyle->SetOptStat(1110);
  eventTimingInfoTree->Draw("crystalAmplitudesEE:(crystalTimesEE-5.)*25. >> hctEEtoAmp(52,-26.,26.,30,0.,30.)","","COLZ");
  char mytitle[100]; sprintf(mytitle,"%s EE Crystal Times to Amplitdue;Crystal Time (ns);Crystal Amplitude (GeV)",runChar); 
  hctEEtoAmp->SetTitle(mytitle);
  hctEEtoAmp->GetXaxis()->SetNdivisions(512);
  hctEEtoAmp->GetYaxis()->SetNdivisions(507);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EETIMEStoAMP_%i.%s",dirName,mType,runNumber,fileType); c[34]->Print(name); } 
  c[35]->cd();
  gStyle->SetOptStat(1110);
  eventTimingInfoTree->Draw("crystalAmplitudesEB:(crystalTimesEB-5.)*25. >> hctEBtoAmp(52,-26.,26.,30,0.,30.)","","COLZ");
  char mytitle[100]; sprintf(mytitle,"%s EB Crystal Times to Amplitdue;Crystal Time (ns);Crystal Amplitude (GeV)",runChar); 
  hctEBtoAmp->SetTitle(mytitle);
  hctEBtoAmp->GetXaxis()->SetNdivisions(512);
  hctEBtoAmp->GetYaxis()->SetNdivisions(507);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTIMEStoAMP_%i.%s",dirName,mType,runNumber,fileType); c[35]->Print(name); } 
  
  //Amplitdue to ave event time
  c[36]->cd();
  gStyle->SetOptStat(1110);
  eventTimingInfoTree->Draw("crystalAmplitudesEB:(correctionToSampleEB-5.0)*25. >> hctEBtoAmpEvt(52,-26.,26.,30,0.,30.)","","COLZ");
  char mytitle[100]; sprintf(mytitle,"%s EB Event Time to Crystal Amplitudes;Average EB Event Time (ns);Crystal Amplitude (GeV)",runChar); 
  hctEBtoAmpEvt->SetTitle(mytitle);
  hctEBtoAmpEvt->GetXaxis()->SetNdivisions(512);
  hctEBtoAmpEvt->GetYaxis()->SetNdivisions(507);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBEvtTIMEStoAMP_%i.%s",dirName,mType,runNumber,fileType); c[36]->Print(name); } 
  c[37]->cd();
  gStyle->SetOptStat(1110);
  eventTimingInfoTree->Draw("crystalAmplitudesEE:(correctionToSampleEB-5.0)*25. >> hctEEtoAmpEvt(52,-26.,26.,30,0.,30.)","","COLZ");
  char mytitle[100]; sprintf(mytitle,"%s EE Event Time to Crystal Amplitudes;Average EB Event Time (ns);Crystal Amplitude (GeV)",runChar); 
  hctEEtoAmpEvt->SetTitle(mytitle);
  hctEEtoAmpEvt->GetXaxis()->SetNdivisions(512);
  hctEEtoAmpEvt->GetYaxis()->SetNdivisions(507);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EEEvtTIMEStoAMP_%i.%s",dirName,mType,runNumber,fileType); c[37]->Print(name); } 
  
  //Amplitude to time error.
  c[38]->cd();
  gStyle->SetOptStat(1110);
  eventTimingInfoTree->Draw("(crystalTimeErrorsEE)*25.:crystalAmplitudesEE >> hctEEtoAmpErr(30,0.,30.,26,0.,26.)","","COLZ");
  char mytitle[100]; sprintf(mytitle,"%s EE Time Error to Crystal Amplitudes;Crystal Amplitude (GeV);Time Error (ns)",runChar); 
  hctEEtoAmpErr->SetTitle(mytitle);
  hctEEtoAmpErr->GetXaxis()->SetNdivisions(512);
  hctEEtoAmpErr->GetYaxis()->SetNdivisions(507);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EETIMESErrtoAMP_%i.%s",dirName,mType,runNumber,fileType); c[38]->Print(name); } 
  c[39]->cd();
  gStyle->SetOptStat(1110);
  eventTimingInfoTree->Draw("(crystalTimeErrorsEB)*25.:crystalAmplitudesEB >> hctEBtoAmpErr(30,0.,30.,26,0.,26.)","","COLZ");
  char mytitle[100]; sprintf(mytitle,"%s EB Time Error to Crystal Amplitudes;Crystal Amplitude (GeV);Time Error (ns)",runChar); 
  hctEBtoAmpErr->SetTitle(mytitle);
  hctEBtoAmpErr->GetXaxis()->SetNdivisions(512);
  hctEBtoAmpErr->GetYaxis()->SetNdivisions(507);
  if (printPics) { sprintf(name,"%s/%sAnalysis_EBTIMESErrtoAMP_%i.%s",dirName,mType,runNumber,fileType); c[39]->Print(name); } 
cout << name << endl;



  return;

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

