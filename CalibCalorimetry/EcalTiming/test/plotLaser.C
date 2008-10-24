
int Wait() {
     cout << " Continue [<RET>|q]?  "; 
     char x;
     x = getchar();
     if ((x == 'q') || (x == 'Q')) return 1;
     return 0;
}

void DrawLaserPlots(Char_t* infile = 0, Int_t runNum=0, Bool_t printPics = kTRUE, Char_t* fileType = "png", Char_t* dirName = ".", Bool_t doWait=kFALSE)
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

  const int nHists1=25;
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
  if (printPics) { sprintf(name,"%s/LaserAnalysis_SM_timing_%i.%s",dirName,runNumber,fileType); c[0]->Print(name); }

  c[1]->cd();
  gStyle->SetOptStat(10);
  TProfile *SM_timingCorrected = (TProfile*) f->Get("SM_timingCorrected");
  customizeTProfile(SM_timingCorrected);
  SM_timingCorrected->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,SM_timingCorrected->GetTitle()); 
  SM_timingCorrected->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_SM_timingCorrected_%i.%s",dirName,runNumber,fileType); c[1]->Print(name); }

  //Timing by LM
  c[2]->cd();
  gStyle->SetOptStat(10);
  TProfile *LM_timing = (TProfile*) f->Get("LM_timing");
  customizeTProfile(LM_timing);
  LM_timing->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,LM_timing->GetTitle()); 
  LM_timing->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_LM_timing_%i.%s",dirName,runNumber,fileType); c[2]->Print(name); }

  c[3]->cd();
  gStyle->SetOptStat(10);
  TProfile *LM_timingCorrected = (TProfile*) f->Get("LM_timingCorrected");
  customizeTProfile(LM_timingCorrected);
  LM_timingCorrected->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,LM_timingCorrected->GetTitle()); 
  LM_timingCorrected->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_LM_timingCorrected_%i.%s",dirName,runNumber,fileType); c[3]->Print(name); }

  //Timing within the towers
  c[4]->cd();
  gStyle->SetOptStat(1111);
  TProfile *Inside_TT_timing = (TProfile*) f->Get("Inside_TT_timing");
  Inside_TT_timing->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,Inside_TT_timing->GetTitle()); 
  Inside_TT_timing->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_Inside_TT_timing_%i.%s",dirName,runNumber,fileType); c[4]->Print(name); }
 
  //Eta Profiles by TT
  c[5]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeTTAllFEDsEta = (TProfile*) f->Get("timeTTAllFEDsEta");
  timeTTAllFEDsEta->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeTTAllFEDsEta->GetTitle()); 
  timeTTAllFEDsEta->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_timeTTAllFEDsEta_%i.%s",dirName,runNumber,fileType); c[5]->Print(name); }
  
  c[6]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeTTAllFEDsEtaEEP = (TProfile*) f->Get("timeTTAllFEDsEtaEEP");
  timeTTAllFEDsEtaEEP->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeTTAllFEDsEtaEEP->GetTitle()); 
  timeTTAllFEDsEtaEEP->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_timeTTAllFEDsEtaEEP_%i.%s",dirName,runNumber,fileType); c[6]->Print(name); }
    
  c[7]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeTTAllFEDsEtaEEM = (TProfile*) f->Get("timeTTAllFEDsEtaEEM");
  timeTTAllFEDsEtaEEM->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeTTAllFEDsEtaEEM->GetTitle()); 
  timeTTAllFEDsEtaEEM->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_timeTTAllFEDsEtaEEM_%i.%s",dirName,runNumber,fileType); c[7]->Print(name); }
  
  //Eta profile by Ch
  c[8]->cd();
  gStyle->SetOptStat(1111);
  TProfile *timeCHAllFEDsEta = (TProfile*) f->Get("timeCHAllFEDsEta");
  timeCHAllFEDsEta->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeCHAllFEDsEta->GetTitle()); 
  timeCHAllFEDsEta->SetTitle(mytitle);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_timeCHAllFEDsEta_%i.%s",dirName,runNumber,fileType); c[8]->Print(name); }
  
//1-D Histograms
  c[9]->cd();
  gStyle->SetOptStat(111110);
  TH1F *Rel_TimingSigma = (TH1F*) f->Get("Rel_TimingSigma");
  Rel_TimingSigma->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,Rel_TimingSigma->GetTitle()); 
  Rel_TimingSigma->SetTitle(mytitle);
  c[9]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_Rel_TimingSigma_%i.%s",dirName,runNumber,fileType); c[9]->Print(name); }
  
  c[10]->cd();
  gStyle->SetOptStat(111110);
  TH1F *XtalsPerEvt = (TH1F*) f->Get("XtalsPerEvt");
  XtalsPerEvt->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,XtalsPerEvt->GetTitle()); 
  XtalsPerEvt->SetTitle(mytitle);
  //c[10]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_XtalsPerEvt_%i.%s",dirName,runNumber,fileType); c[10]->Print(name); }
  
  c[11]->cd();
  gStyle->SetOptStat(111110);
  TH1F *laserShift = (TH1F*) f->Get("laserShift");
  laserShift->Draw();
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,laserShift->GetTitle()); 
  laserShift->SetTitle(mytitle);
  c[11]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_laserShift_%i.%s",dirName,runNumber,fileType); c[11]->Print(name); }
  
//2-D Histogram
  c[12]->cd();
  gStyle->SetOptStat(111110);
  TH2F *RelRMS_vs_AbsTime = (TH2F*) f->Get("RelRMS_vs_AbsTime");
  RelRMS_vs_AbsTime->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,RelRMS_vs_AbsTime->GetTitle()); 
  RelRMS_vs_AbsTime->SetTitle(mytitle);
  c[12]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_RelRMS_vs_AbsTime_%i.%s",dirName,runNumber,fileType); c[12]->Print(name); }
  
//1-D TGraphs  
  c[13]->cd();
  gStyle->SetOptStat(111110);
  TGraph *TTMeanWithRMS_All_FEDS = (TGraph*) f->Get("TTMeanWithRMS_All_FEDS");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,TTMeanWithRMS_All_FEDS->GetTitle()); 
  TTMeanWithRMS_All_FEDS->SetTitle(mytitle);
  TTMeanWithRMS_All_FEDS->GetYaxis()->SetLimits(0.,9.);
  TTMeanWithRMS_All_FEDS->GetYaxis()->UnZoom();
  TTMeanWithRMS_All_FEDS->Draw("AP*");
  //c[13]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_TTMeanWithRMS_All_FEDS_%i.%s",dirName,runNumber,fileType); c[13]->Print(name); }
  
  c[14]->cd();
  gStyle->SetOptStat(111110);
  TGraph *TTMeanWithRMS_All_FEDS_CHANGED = (TGraph*) f->Get("TTMeanWithRMS_All_FEDS_CHANGED");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,TTMeanWithRMS_All_FEDS_CHANGED->GetTitle()); 
  TTMeanWithRMS_All_FEDS_CHANGED->SetTitle(mytitle);
  TTMeanWithRMS_All_FEDS_CHANGED->GetYaxis()->SetLimits(0.,9.);
  TTMeanWithRMS_All_FEDS_CHANGED->GetYaxis()->UnZoom();
  TTMeanWithRMS_All_FEDS_CHANGED->Draw("AP*");
  //c[13]->SetLogy(1);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_TTMeanWithRMS_All_FEDS_Corrected_%i.%s",dirName,runNumber,fileType); c[14]->Print(name); }
  
//2-D TGraphs/Profiles 
  //Ch by Ch timing profile
  c[15]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *timeCHProfile = (TProfile2D*) f->Get("timeCHProfile");
  timeCHProfile->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeCHProfile->GetTitle()); 
  timeCHProfile->SetTitle(mytitle);
  timeCHProfile->SetMinimum(3.0);
  timeCHProfile->GetXaxis()->SetNdivisions(-18);
  timeCHProfile->GetYaxis()->SetNdivisions(2);
  c[15]->SetLogy(0);
  c[15]->SetLogz(0);
  c[15]->SetGridx(1);
  c[15]->SetGridy(1);
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_timeCHProfile_%i.%s",dirName,runNumber,fileType); c[15]->Print(name); }
 
  //TT by TT timing profile
  c[16]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *timeTTProfile = (TProfile2D*) f->Get("timeTTProfile");
  timeTTProfile->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,timeTTProfile->GetTitle()); 
  timeTTProfile->SetTitle(mytitle);
  timeTTProfile->SetMinimum(3.0);
  timeTTProfile->GetXaxis()->SetNdivisions(-18);
  timeTTProfile->GetYaxis()->SetNdivisions(2);
  c[16]->SetLogy(0);
  c[16]->SetLogz(0);
  c[16]->SetGridx(1);
  c[16]->SetGridy(1);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_timeTTProfile_%i.%s",dirName,runNumber,fileType); c[16]->Print(name); }
 
  //Ch by Ch timing profile EE+
  c[17]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeCHProfile = (TProfile2D*) f->Get("EEPtimeCHProfile");
  EEPtimeCHProfile->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,EEPtimeCHProfile->GetTitle()); 
  EEPtimeCHProfile->SetTitle(mytitle);
  EEPtimeCHProfile->SetMinimum(3.0);
  EEPtimeCHProfile->GetXaxis()->SetNdivisions(18);
  c[17]->SetLogy(0);
  c[17]->SetLogz(0);
  drawEELines();
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_EEPtimeCHProfile_%i.%s",dirName,runNumber,fileType); c[17]->Print(name); }
   
  //Ch by Ch timing profile EE+
  c[18]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeCHProfile = (TProfile2D*) f->Get("EEMtimeCHProfile");
  EEMtimeCHProfile->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,EEMtimeCHProfile->GetTitle()); 
  EEMtimeCHProfile->SetTitle(mytitle);
  EEMtimeCHProfile->SetMinimum(3.0);
  EEMtimeCHProfile->GetXaxis()->SetNdivisions(18);
  c[18]->SetLogy(0);
  c[18]->SetLogz(0);
  drawEELines();
  //c[15]->SetLogz(1);
  if (printPics) { sprintf(name,"%s/LaserAnalysis_EEMtimeCHProfile_%i.%s",dirName,runNumber,fileType); c[18]->Print(name); }
  
  //TT by TT timing profile EE+
  c[19]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEPtimeTTProfile = (TProfile2D*) f->Get("EEPtimeTTProfile");
  EEPtimeTTProfile->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,EEPtimeTTProfile->GetTitle()); 
  EEPtimeTTProfile->SetTitle(mytitle);
  EEPtimeTTProfile->SetMinimum(3.0);
  EEPtimeTTProfile->GetXaxis()->SetNdivisions(18);
  c[19]->SetLogy(0);
  c[19]->SetLogz(0);
  drawEELines();
  if (printPics) { sprintf(name,"%s/LaserAnalysis_EEPtimeTTProfile_%i.%s",dirName,runNumber,fileType); c[19]->Print(name); }
  
  //TT by TT timing profile EE-
  c[20]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *EEMtimeTTProfile = (TProfile2D*) f->Get("EEMtimeTTProfile");
  EEMtimeTTProfile->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,EEMtimeTTProfile->GetTitle()); 
  EEMtimeTTProfile->SetTitle(mytitle);
  EEMtimeTTProfile->SetMinimum(3.0);
  EEMtimeTTProfile->GetXaxis()->SetNdivisions(18);
  c[20]->SetLogy(0);
  c[20]->SetLogz(0);
  drawEELines();
  if (printPics) { sprintf(name,"%s/LaserAnalysis_EEMtimeTTProfile_%i.%s",dirName,runNumber,fileType); c[20]->Print(name); }
  
  
  //Amplitude Profiles
  c[21]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *fullAmpProfileEB = (TProfile2D*) f->Get("fullAmpProfileEB");
  fullAmpProfileEB->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,fullAmpProfileEB->GetTitle()); 
  fullAmpProfileEB->SetTitle(mytitle);
  if (fullAmpProfileEB->GetMaximum() > 0 ) {
     fullAmpProfileEB->SetMinimum(1.0);
     c[21]->SetLogy(0);
     c[21]->SetLogz(1);
  }
  if (printPics) { sprintf(name,"%s/LaserAnalysis_fullAmpProfileEB_%i.%s",dirName,runNumber,fileType); c[21]->Print(name); }
  
  c[22]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *fullAmpProfileEEP = (TProfile2D*) f->Get("fullAmpProfileEEP");
  fullAmpProfileEEP->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,fullAmpProfileEEP->GetTitle()); 
  fullAmpProfileEEP->SetTitle(mytitle);
  if (fullAmpProfileEEP->GetMaximum() > 0 ) {
     fullAmpProfileEEP->SetMinimum(1.0);
     c[22]->SetLogy(0);
     c[22]->SetLogz(1);
  }
  drawEELines();
  if (printPics) { sprintf(name,"%s/LaserAnalysis_fullAmpProfileEEP_%i.%s",dirName,runNumber,fileType); c[22]->Print(name); }
  
  c[23]->cd();
  gStyle->SetOptStat(10);
  TProfile2D *fullAmpProfileEEM = (TProfile2D*) f->Get("fullAmpProfileEEM");
  fullAmpProfileEEM->Draw("colz");
  char mytitle[100]; sprintf(mytitle,"%s %s",runChar,fullAmpProfileEEM->GetTitle()); 
  fullAmpProfileEEM->SetTitle(mytitle);
  if (fullAmpProfileEEM->GetMaximum() > 0 ) {
     fullAmpProfileEEM->SetMinimum(1.0);
     c[23]->SetLogy(0);
     c[23]->SetLogz(1);
  }
  drawEELines();
  if (printPics) { sprintf(name,"%s/LaserAnalysis_fullAmpProfileEEM_%i.%s",dirName,runNumber,fileType); c[23]->Print(name); }
   
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
  myTProfile->SetLineWidth(2);
  myTProfile->SetMarkerStyle(kFullCircle);
  myTProfile->SetMarkerSize(0.7);
}



