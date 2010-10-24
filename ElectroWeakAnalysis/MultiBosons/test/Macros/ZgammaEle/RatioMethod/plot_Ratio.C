#include <TF1.h>

void plot_Ratio(){
  plot_Ratio(10,5,10,20,60,80,180);
}

void plot_Ratio(Int_t nIt, Int_t binS1, Int_t binS2, Int_t binS3, Double_t PtR1, Double_t PtR2, Double_t FitU){
  
  Int_t    DoFit=1;
  Int_t    nBin=5;
  Double_t FitL=20;
  
  gROOT->LoadMacro("tdrStyle.C");
  setTDRStyle();

  //Load File Name...
  //TString MCRatio      = "../RatioMethod/MC/MCRatio_Test.root";
  TString MCEvtYield   = "/home/zkliu/ZgammaEle/Test_eID11_pID7_dR07_Trk5ToInfx.root";
  TString DataRatio    = "/home/zkliu/ZgammaEle/RatioMethod/Ratio_Test_pID7_Trk5ToInf_x10_v3.root";
  TString DataEvtYield = "/data2/zkliu/VG_marco/CommitToCVS/EventYield/ver2_HE/Z_Data_Oct18_pID7_AntiTrkIso5ToInf_kkk.root";
  TString OutPutFileName="EstimationResult.root";

  //Some parameters setting
  Double_t McLumi =10.94;     //MC lumi for bkg samples
  Double_t McLumi2=10.94;     //MC lumi for event yield
  Double_t McWeight;
  Int_t    nBin2=10;          //Estimation binning
  Int_t    nBin3=5;           //binning for Z+Jet validation

  //Other variables
  Double_t pEB[6] ={0,0,0,0,0,0}, pEE[6] ={0,0,0,0,0,0};
  Double_t prEB[6]={0,0,0,0,0,0}, prEE[6]={0,0,0,0,0,0};

  TString TName_Num[31]= {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30"};

  Int_t    nfile, ProcessID;
  TString  name;
  Float_t  Temp_ratio=0,Temp_err=0;
  Float_t  Temp_V1=0, Temp_V2=0, Temp_err2=0,Temp_err3=0;

  //Binning test: non-fixed binning
  Int_t SizeN;
  SizeN = int((PtR1-FitL)/binS1) + int((PtR2-PtR1)/binS2) + int((FitU-PtR2)/binS3) + 1;
  Int_t BinValue[100];
  Int_t ct1=0,ct2=0;
  Double_t TempV=0;

  for (Int_t ii=0;ii<SizeN;ii++){
     TempV = FitL+ii*binS1;
     if (TempV<PtR1) {
       BinValue[ii]=TempV;
       ct1++;
       ct2++;
     } else {
       TempV = PtR1 + (ii-ct1)*binS2;
       if (TempV<PtR2){
         BinValue[ii]=TempV;
         ct2++;
       } else {
         TempV = PtR2 + (ii-ct2)*binS3;
         if (TempV<=FitU){
           BinValue[ii]=TempV;
         }
       }
     }
  }
  
  const int nxBin=SizeN;
 
  Double_t xBin[nxBin];
  for (Int_t ii=0;ii<SizeN;ii++){
    xBin[ii]=BinValue[ii];
  }

  Int_t nxBin = sizeof(xBin)/sizeof(xBin[0])-1;
  //load file
  //TFile *fmc   = new TFile(MCRatio);
  TFile *fdata = new TFile(DataRatio);

  //Histogram
  TH1F *hmIso[2][3];   //mc   Iso&Non-Iso
  TH1F *hdIso[2][3];   //data Iso&Non-Iso
  TH1F *hmR[3],*hdR[3];//Ratio=Iso/Non-Iso

  TH1D *hT1;
  TH1F *hT0,*hT2;

  TH2F *hTF  = new TH2F("hTF", "",10, 20, 200,10,0,1);

  hmIso[0][0]= new TH1F("mIsoPho_Loose_EB", "", (500/nBin), 0, 500);
  hmIso[0][1]= new TH1F("mIsoPho_Loose_EE", "", (500/nBin), 0, 500);
  hmIso[0][2]= new TH1F("mIsoPho_LooseAll", "", (500/nBin), 0, 500);
  hmIso[1][0]= new TH1F("mNonIsoPho_Loose_EB", "", (500/nBin), 0, 500);
  hmIso[1][1]= new TH1F("mNonIsoPho_Loose_EE", "", (500/nBin), 0, 500);
  hmIso[1][2]= new TH1F("mNonIsoPho_LooseAll", "", (500/nBin), 0, 500);

  hdIso[0][0]= new TH1F("dIsoPho_Loose_EB", "", nxBin, xBin);
  hdIso[0][1]= new TH1F("dIsoPho_Loose_EE", "", nxBin, xBin);
  hdIso[0][2]= new TH1F("dIsoPho_LooseAll", "", nxBin, xBin);
  hdIso[1][0]= new TH1F("dNonIsoPho_Loose_EB", "", nxBin, xBin);
  hdIso[1][1]= new TH1F("dNonIsoPho_Loose_EE", "", nxBin, xBin);
  hdIso[1][2]= new TH1F("dNonIsoPho_LooseAll", "", nxBin, xBin);

  hmR[0]= new TH1F("mIsoNIso_EB", "", (500/nBin), 0, 500); 
  hmR[1]= new TH1F("mIsoNIso_EE", "", (500/nBin), 0, 500);
  hmR[2]= new TH1F("mIsoNIsoAll", "", (500/nBin), 0, 500);

  hdR[0]= new TH1F("dIsoNIso_EB", "", nxBin, xBin);           
  hdR[1]= new TH1F("dIsoNIso_EE", "", nxBin, xBin);
  hdR[2]= new TH1F("dIsoNIsoAll", "", nxBin, xBin);

  //Get Histogram
  //MC sample
  TString TName_EE[2]={"EB","EE"};

  //Compare MC Ratio... need ntuples with jetTrg stored...

/*
  hT1 = (TH1D*)fmc->Get("hInfo");
  nfile = hT1->GetBinContent(1);
  for (Int_t iSample=0;iSample<nfile;iSample++){
    for (Int_t iEE=0;iEE<2;iEE++) {
      name = "Sample_"+TName_Num[iSample]+"/hNEvt";
      hT1 = (TH1D*)fmc->Get(name);
      ProcessID = int( hT1->GetBinContent(14) );
      McWeight  = hT1->GetBinContent(15);
      
      //Iso
      name="Sample_"+TName_Num[iSample]+"/"+"IsoPho_Loose_"+TName_EE[iEE];
      hT0 = (TH1F*)fmc->Get(name);
      hT0 ->Rebin(nBin);
      hT0 ->Scale(McLumi);
      hmIso[0][iEE]->Add(hT0);
      hmIso[0][2]->Add(hT0);
     
      //Non-Iso
      name="Sample_"+TName_Num[iSample]+"/"+"NonIsoPho_Loose_"+TName_EE[iEE];
      hT0 = (TH1F*)fmc->Get(name);
      hT0 ->Rebin(nBin);
      hT0 ->Scale(McLumi);
      hmIso[1][iEE]->Add(hT0);
      hmIso[1][2]->Add(hT0);
    }
  }
*/
  //Data Sample
  TH1F *hTest;
  name="IsoPho_Loose_EB";
  hT0 = (TH1F*)fdata->Get(name);
  hTest = (TH1F*) hT0 ->Rebin(nxBin,"hTest",xBin);
  hdIso[0][0]->Add(hTest);
  hdIso[0][2]->Add(hTest);

  hTest->Reset();
  name="IsoPho_Loose_EE";
  hT0 = (TH1F*)fdata->Get(name);
  hTest = (TH1F*) hT0 ->Rebin(nxBin,"hTest",xBin);
  hdIso[0][1]->Add(hTest);
  hdIso[0][2]->Add(hTest);

  hTest->Reset();
  name="NonIsoPho_Loose_EB";
  hT0 = (TH1F*)fdata->Get(name);
  hTest = (TH1F*) hT0 ->Rebin(nxBin,"hTest",xBin);
  hdIso[1][0]->Add(hTest);
  hdIso[1][2]->Add(hTest);

  hTest->Reset();
  name="NonIsoPho_Loose_EE";
  hT0 = (TH1F*)fdata->Get(name);
  hTest = (TH1F*) hT0 ->Rebin(nxBin,"hTest",xBin);
  hdIso[1][1]->Add(hTest);
  hdIso[1][2]->Add(hTest);

  //------------------------------------------------------------
  //Calculate ratio

  for (Int_t ii=0;ii<3;ii++){
    hdIso[0][ii]->Sumw2();    
    hdIso[1][ii]->Sumw2();
  }

  for (Int_t ii=0;ii<2;ii++){
    hT0 = (TH1F*) hmIso[0][ii]->Clone(); hT0->Sumw2();
    hT2 = (TH1F*) hmIso[1][ii]->Clone(); hT2->Sumw2();    
    hmR[ii]->Divide(hT0,hT2);
    hdR[ii]->Divide(hdIso[0][ii],hdIso[1][ii]);
    hdR[ii]->SetLineColor(2+2*DoFit);
    hdR[ii]->SetMarkerColor(2+2*DoFit);
    hmR[ii]->SetMarkerStyle(8);
    hdR[ii]->SetMarkerStyle(8);
  }

  //Fitting....
  TF1 *FrealEB = new TF1("FrealEB","[0]*exp([1]*(x))",FitL,FitU);
  TF1 *FrealEE = new TF1("FrealEE","[0]*exp([1]*(x))",FitL,FitU);

  TF1 *FallEB  = new TF1("FallEB" ,"[0]*exp([1]*(x)) + [2]*exp([3]*(x))+[4]",FitL,FitU);
  TF1 *FallEE  = new TF1("FallEE" ,"[0]*exp([1]*(x)) + [2]*exp([3]*(x))+[4]",FitL,FitU);

  TF1 *Fall2EB  = new TF1("Fall2EB" ,"[0]*exp([1]*(x)) + [2]*exp([3]*(x))+[4]",FitL,FitU);
  TF1 *Fall2EE  = new TF1("Fall2EE" ,"[0]*exp([1]*(x)) + [2]*exp([3]*(x))+[4]",FitL,FitU);
  TF1 *Fall3EB  = new TF1("Fall3EB" ,"[0]*exp([1]*(x)) + [2]*exp([3]*(x))+[4]",FitL,FitU);
  TF1 *Fall3EE  = new TF1("Fall3EE" ,"[0]*exp([1]*(x)) + [2]*exp([3]*(x))+[4]",FitL,FitU);

  FrealEB-> SetParameters(0.15, 0.02);
  FrealEE-> SetParameters(0.15, 0.02);

  FallEB -> SetParameters(0.15, 0.02,0.01,-0.1,0.01);
  FallEE -> SetParameters(0.15, 0.02,0.01,-0.1,0.01);

  Fall2EB-> SetParameters(0.15, 0.02,0.01,-0.01,0.01);
  Fall2EE-> SetParameters(0.15, 0.02,0.01,-0.01,0.01);

  Fall3EB-> SetParameters(0.15, 0.02,0.01,-0.01,0.01);
  Fall3EE-> SetParameters(0.15, 0.02,0.01,-0.01,0.01);

  if (DoFit==1){
    //get initial values of p0~p5
    hdR[0]->Fit("FrealEB","Q","",100,FitU);
    hdR[1]->Fit("FrealEE","Q","",100,FitU);

    for (Int_t ii=0;ii<=1;ii++){
      pEB[ii]=FrealEB->GetParameter(ii);
      pEE[ii]=FrealEE->GetParameter(ii);

      FallEB -> FixParameter(ii,pEB[ii]);
      FallEE -> FixParameter(ii,pEE[ii]);
    }

    FallEB -> SetParLimits(2,0,15);
    FallEE -> SetParLimits(2,0,15);
    FallEB -> SetParLimits(3,-10,0);
    FallEE -> SetParLimits(3,-10,0);
    FallEB -> SetParLimits(4,0,0.5);
    FallEE -> SetParLimits(4,0,0.5);

    hdR[0]->Fit("FallEB","Q","",FitL,FitU);
    hdR[1]->Fit("FallEE","Q","",FitL,FitU);

    //Start iteration
    for (Int_t it=0;it<nIt;it++){

      for (Int_t jj=0;jj<5;jj++){
        Fall2EB->ReleaseParameter(jj);
        Fall2EE->ReleaseParameter(jj);
      }

      for (Int_t ii=2;ii<=4;ii++){
        pEB[ii]=FallEB->GetParameter(ii);
        pEE[ii]=FallEE->GetParameter(ii);

        Fall2EB -> FixParameter(ii,pEB[ii]);
        Fall2EE -> FixParameter(ii,pEE[ii]);
      }

      Fall2EB -> SetParLimits(1,0,15);
      Fall2EE -> SetParLimits(1,0,15);

      hdR[0]->Fit("Fall2EB","QL","",FitL,FitU);
      hdR[1]->Fit("Fall2EE","Q","",FitL,FitU);

      for (Int_t jj=0;jj<5;jj++){
        FallEB->ReleaseParameter(jj);
        FallEE->ReleaseParameter(jj);
      }

      for (Int_t ii=0;ii<=1;ii++){
        pEB[ii]=Fall2EB->GetParameter(ii);
        pEE[ii]=Fall2EE->GetParameter(ii);
        
        FallEB -> FixParameter(ii,pEB[ii]);
        FallEE -> FixParameter(ii,pEE[ii]);
      }
      FallEB -> SetParLimits(2,0,15);
      FallEE -> SetParLimits(2,0,15);
      FallEB -> SetParLimits(3,-10,0);
      FallEE -> SetParLimits(3,-10,0);
      FallEB -> SetParLimits(4,0.,0.2);
      FallEE -> SetParLimits(4,0.,0.2);

      hdR[0]->Fit("FallEB","Q","",FitL,FitU);
      hdR[1]->Fit("FallEE","Q","",FitL,FitU);

      for (Int_t ii=2;ii<=4;ii++){
        pEB[ii]=FallEB->GetParameter(ii);
        pEE[ii]=FallEE->GetParameter(ii);
        prEB[ii]=FallEB->GetParError(ii);
        prEE[ii]=FallEE->GetParError(ii);
      }
    }
  }

  //Make plot...
  TF1 *RealEB = new TF1("RealEB","[0]*exp([1]*(x))",10,FitU);
  TF1 *RealEE = new TF1("RealEE","[0]*exp([1]*(x))",10,FitU);

  TF1 *FakeEB = new TF1("FakeEB","[0]*exp([1]*(x))+[2]",10,FitU);
  TF1 *FakeEE = new TF1("FakeEE","[0]*exp([1]*(x))+[2]",10,FitU);

  for (Int_t ii=0;ii<2;ii++){
    RealEB->FixParameter(ii,pEB[ii]);
    RealEE->FixParameter(ii,pEE[ii]);
  }
  for (Int_t ii=0;ii<3;ii++){
    FakeEB->FixParameter(ii,pEB[ii+2]);
    FakeEE->FixParameter(ii,pEE[ii+2]);
  }

  TLegend *legend2 = new TLegend(.24,.6,.54,.78);
  legend2->AddEntry(hdR[0],"data"  ,"lp");
  legend2->AddEntry(RealEB,"fReal"  ,"l");
  legend2->AddEntry(FakeEB,"fFake"  ,"l");
  legend2->SetFillColor(0);
  legend2->SetFillStyle(0);
  legend2->SetLineColor(0);
  legend2->SetShadowColor(0);
    

  //(1) Ratio distribution
  TCanvas *c2 = new TCanvas("C2","C2",900,450);
  c2->Divide(2,1);

  for (Int_t kk=0;kk<2;kk++){
    c2->cd(kk+1);
    hdR[kk]->GetXaxis() -> SetRangeUser(FitL,FitU);
    hdR[kk]->GetYaxis() -> SetRangeUser(0,0.2);
    hdR[kk]->GetXaxis() -> SetTitle("phoEt");
    hdR[kk]->GetYaxis() -> SetTitle("Ratio (iso/non-iso)");
    hdR[kk]->Draw();
    if (kk==0){
      RealEB->SetLineStyle(2);
      RealEB->SetLineColor(6);
      FakeEB->SetLineStyle(2);
      FakeEB->SetLineColor(1);
      FakeEB->Draw("same");
      RealEB->Draw("same");
    } else if (kk==1){
      RealEE->SetLineStyle(2);
      RealEE->SetLineColor(6);
      FakeEE->SetLineStyle(2);
      FakeEE->SetLineColor(1);
      FakeEE->Draw("same");
      RealEE->Draw("same");
    }
    if (DoFit ==0 )hmR[kk]->Draw("same");
    legend2->Draw();
    TLatex latexLabel;
    latexLabel.SetTextSize(0.035);
    latexLabel.SetNDC();
    latexLabel.DrawLatex(0.2, 0.96, "CMS Preliminary 2010");
    latexLabel.DrawLatex(0.24, 0.88, "#sqrt{s} = 7 TeV");
    latexLabel.DrawLatex(0.24, 0.8,  Form("#int#font[12]{L} dt= %.1f pb^{-1}",McLumi));
  }
  //---------------------------------------------------------------

  //Check the Ratio
  if (DoFit==1){
    TFile *fmc2 = new TFile(MCEvtYield);
    TH1D *hZIEB,*hZIEB,*hZNEB,*hZNEE,*hZR1,*hZR2;
    hT1=(TH1D*)fmc2->Get("Sample_0/IsoPho_Loose_EB");    hZIEB = (TH1D*) hT1 -> Clone();
    hT1=(TH1D*)fmc2->Get("Sample_0/NonIsoPho_Loose_EB"); hZNEB = (TH1D*) hT1 -> Clone();
    hT1=(TH1D*)fmc2->Get("Sample_0/IsoPho_Loose_EE");    hZIEE = (TH1D*) hT1 -> Clone();
    hT1=(TH1D*)fmc2->Get("Sample_0/NonIsoPho_Loose_EE"); hZNEE = (TH1D*) hT1 -> Clone();

    hZIEB->Rebin(nBin3);
    hZIEB->Scale(343.51);//weight = 1/343.51, scale  back to MC statistic
    hZIEB->Sumw2();
    hZNEB->Rebin(nBin3);
    hZNEB->Scale(343.51);
    hZNEB->Sumw2();
    hZIEE->Rebin(nBin3);
    hZIEE->Scale(343.51);
    hZIEE->Sumw2();
    hZNEE->Rebin(nBin3);
    hZNEE->Scale(343.51);
    hZNEE->Sumw2();

    hZR1= new TH1D("ZR2", "", (500/nBin3), 0, 500);
    hZR2= new TH1D("ZR1", "", (500/nBin3), 0, 500);

    hZR1->Divide(hZIEB,hZNEB);
    hZR2->Divide(hZIEE,hZNEE);
    
    //(2) Use Z+Jet for Validiation
    TLegend *legend3 = new TLegend(.62,.6,.95,.82);
    legend3->AddEntry(hZR1,"MC W+Jet","lp");
    legend3->AddEntry(FakeEB,"Fitted fFake" ,"l");
    legend3->SetFillColor(0);
    legend3->SetFillStyle(0);
    legend3->SetLineColor(0);
    legend3->SetShadowColor(0);

    TCanvas *c3 = new TCanvas("C3","C3",900,300);
    c3->Divide(2,1);
    c3->cd(1);
    hZR1 -> Draw();
    hZR1 -> GetYaxis() -> SetRangeUser(0,0.1);
    hZR1 -> GetXaxis() -> SetRangeUser(10,100);
    hZR1->GetXaxis() -> SetTitle("p_{T}^{#gamma}");
    hZR1->GetYaxis() -> SetTitle("Ratio (iso/non-iso)");
    FakeEB->Draw("same");
    legend3->Draw();
    TLatex latexLabel;
    latexLabel.SetTextSize(0.035);
    latexLabel.SetNDC();
    latexLabel.DrawLatex(0.17, 0.96, "CMS Preliminary 2010");
    latexLabel.DrawLatex(0.80, 0.96, "#sqrt{s} = 7 TeV");
    latexLabel.DrawLatex(0.62, 0.88, Form("#int#font[12]{L} dt= %.1f pb^{-1}",McLumi));

    c3->cd(2);
    hZR2->Draw();
    hZR2->GetYaxis() -> SetRangeUser(0,0.2);
    hZR2->GetXaxis() -> SetRangeUser(10,100);
    hZR2->GetXaxis() -> SetTitle("p_{T}^{#gamma}");
    hZR2->GetYaxis() -> SetTitle("Ratio (iso/non-iso)");
    FakeEE->Draw("same");
    TLatex latexLabel;
    latexLabel.SetTextSize(0.035);
    latexLabel.SetNDC();
    latexLabel.DrawLatex(0.17, 0.96, "CMS Preliminary 2010");
    latexLabel.DrawLatex(0.80, 0.96, "#sqrt{s} = 7 TeV");
    latexLabel.DrawLatex(0.62, 0.88, Form("#int#font[12]{L} dt= %.1f pb^{-1}",McLumi));
    legend3->Draw();
    
    //Estimate the background
    TFile *f_ZNIso = new TFile(DataEvtYield);
    Double_t Temp_Num=0;
    Double_t Temp_Est=0;
    Double_t TotEsEB=0 ,TotEsEE=0;  
    Double_t TempErr=0;

    hT0 = (TH1F*) f_ZNIso->Get("IsoPho_Loose_EB");
    hT2 = (TH1F*) f_ZNIso->Get("IsoPho_Loose_EE");

    hSigEB = (TH1F*) hT0->Clone();
    hSigEE = (TH1F*) hT2->Clone();

    hT0->Reset(), hT2->Reset();

    hT0 = (TH1F*) f_ZNIso->Get("NonIsoPho_Loose_EB");// hT0->Rebin(nBin2);
    hT2 = (TH1F*) f_ZNIso->Get("NonIsoPho_Loose_EE");// hT2->Rebin(nBin2); 

    TH1F *hEstEE,*hSigEE;
    TH1F *hEstEB,*hSigEB;

    hEstEB = (TH1F*) hT0->Clone();
    hEstEE = (TH1F*) hT2->Clone();

    for(Int_t ii=1;ii<=500;ii++){
      Temp_Num = hT0 ->GetBinContent(ii);
      Temp_Est  = (pEB[2]*TMath::Exp(pEB[3]*((ii-1)+(0.5)))+pEB[4])*(Temp_Num);
      hEstEB->SetBinContent(ii,Temp_Est);

      TotEsEB += Temp_Est;

      Temp_Num = hT2 ->GetBinContent(ii);
      Temp_Est = (pEE[2]*TMath::Exp(pEE[3]*((ii-1)+(0.5)))+pEE[4])*(Temp_Num);
      hEstEE->SetBinContent(ii,Temp_Est);

      TotEsEE += Temp_Est;
    }

    Float_t CentEB=0,CentEE=0;
    Float_t TotsErr=0;

    CentEB=TotEsEB;
    CentEE=TotEsEE;

    hEstEB->Rebin(nBin2);
    hEstEE->Rebin(nBin2);

    hSigEB->Rebin(nBin2);
    hSigEE->Rebin(nBin2);
    hSigEB->Sumw2();
    hSigEE->Sumw2();
    
    for (Int_t ii=1;ii<=int(500/nBin2);ii++){
      Temp_Num = hEstEB->GetBinContent(ii);
      Temp_Est = hSigEB->GetBinContent(ii);
      TempErr  = sqrt(Temp_Num+Temp_Est);
      if ((Temp_Est-Temp_Num) > 0) {
        hSigEB -> SetBinContent(ii,Temp_Est-Temp_Num); 
        hSigEB -> SetBinError(ii,TempErr);
      }

      Temp_Num = hEstEE->GetBinContent(ii);
      Temp_Est = hSigEE->GetBinContent(ii);
      TempErr  = sqrt(Temp_Num+Temp_Est);
      if ((Temp_Est-Temp_Num) > 0) {
        hSigEE -> SetBinContent(ii,Temp_Est-Temp_Num);
        hSigEE -> SetBinError(ii,TempErr);
      }
    }
    hSigEB->Add(hSigEE);

    hEstEB->SetName("Estimated_Bkg_EB");
    hEstEE->SetName("Estimated_Bkg_EE");
    hSigEB->SetName("Estimated_Sig");

    TH1D  *hmEE;
    TH1D  *hmEB;
    TH1D  *hzgEE;
    TH1D  *hzgEB;

    hT1 = (TH1D*)fmc2->Get("hInfo");
    nfile = hT1->GetBinContent(1);
    for (Int_t iSample=0;iSample<nfile;iSample++){
      name = "Sample_"+TName_Num[iSample]+"/hNEvt";
      hT1 = (TH1D*)fmc2->Get(name); 
      ProcessID = int( hT1->GetBinContent(14) );
      if (ProcessID==1){
        hmEB = (TH1D*) fmc2 -> Get("Sample_"+TName_Num[iSample]+"/IsoPho_Loose_EB");
        hmEB ->Rebin(nBin2);
        hmEE = (TH1D*) fmc2 -> Get("Sample_"+TName_Num[iSample]+"/IsoPho_Loose_EE");
        hmEE ->Rebin(nBin2);
      } else if (ProcessID==0){
        hzgEB = (TH1D*) fmc2 -> Get("Sample_"+TName_Num[iSample]+"/IsoPho_Loose_EB");
        hzgEB ->Rebin(nBin2);
        hzgEE = (TH1D*) fmc2 -> Get("Sample_"+TName_Num[iSample]+"/IsoPho_Loose_EE");
        hzgEE ->Rebin(nBin2);
      }
    }     

    hmEB  ->Scale(McLumi2);
    hmEE  ->Scale(McLumi2);
    hzgEB ->Scale(McLumi2);
    hzgEE ->Scale(McLumi2);
    hzgEB ->Add(hzgEE);
    hEstEB->Sumw2();
    hEstEE->Sumw2();

    hzgEB->SetName("MCZgamma_Sig");

    TFile *fout= new TFile(OutPutFileName,"RECREATE");
    fout->cd();
      hEstEB->Write();
      hEstEE->Write();
      hSigEB->Write();
      hzgEB ->Write();
    fout->Close();
    
    //(3) Estimated Results:
    TLegend *legendR = new TLegend(.62,.6,.95,.82);
    legendR->AddEntry(hmEE,"MC W+Jet","f");
    legendR->AddEntry(hEstEE,"Data: bkg"  ,"lp");
    legendR->SetFillColor(0);
    legendR->SetFillStyle(0);
    legendR->SetLineColor(0);
    legendR->SetShadowColor(0);

    TCanvas *cR = new TCanvas("CR","CR",900,450);
    cR->Divide(2,1);
    cR->cd(1);
    hmEB->GetXaxis() -> SetRangeUser(10,100);
    hmEB->GetYaxis() -> SetRangeUser(0,4);
    hmEB->SetFillColor(kYellow);
    hmEB->GetXaxis() -> SetTitle("p_{T}^{#gamma}");
    hmEB->GetYaxis() -> SetTitle("background from W+jet");
    hEstEB->SetLineColor(2);
    hEstEB->SetMarkerColor(2);
    hEstEB->SetMarkerStyle(8);
    hmEB->Draw();
    hEstEB->Draw("SAMEP");
    legendR->Draw();
    TLatex latexLabel;
    latexLabel.SetTextSize(0.035);
    latexLabel.SetNDC();
    latexLabel.DrawLatex(0.17, 0.96, "CMS Preliminary 2010");
    latexLabel.DrawLatex(0.80, 0.96, "#sqrt{s} = 7 TeV");
    latexLabel.DrawLatex(0.62, 0.88, Form("#int#font[12]{L} dt= %.1f pb^{-1}",McLumi2));
    latexLabel.DrawLatex(0.62, 0.55, Form("Estimated:%.2f",hEstEB->Integral(1,(500/nBin2))));
    latexLabel.DrawLatex(0.62, 0.5 , Form("MC ZJets :%.2f",hmEB  ->Integral(1,(500/nBin2))));

    cR->cd(2);
    hmEE->GetXaxis() -> SetRangeUser(10,100);
    hmEE->GetYaxis() -> SetRangeUser(0,4);
    hmEE->SetFillColor(kYellow);
    hmEE->GetXaxis() -> SetTitle("p_{T}^{#gamma}");
    hmEE->GetYaxis() -> SetTitle("background from W+jet");
    hEstEE->SetLineColor(2);
    hEstEE->SetMarkerColor(2);
    hEstEE->SetMarkerStyle(8);
    hmEE->Draw();
    hEstEE->Draw("SAMEP");
    legendR->Draw();
    TLatex latexLabel;
    latexLabel.SetTextSize(0.035);
    latexLabel.SetNDC();
    latexLabel.DrawLatex(0.17, 0.96, "CMS Preliminary 2010");
    latexLabel.DrawLatex(0.80, 0.96, "#sqrt{s} = 7 TeV");
    latexLabel.DrawLatex(0.62, 0.88, Form("#int#font[12]{L} dt= %.1f pb^{-1}",McLumi2));
    latexLabel.DrawLatex(0.62, 0.55, Form("Estimated:%.2f",hEstEE->Integral(1,(500/nBin2))));
    latexLabel.DrawLatex(0.62, 0.5 , Form("MC ZJets :%.2f",hmEE  ->Integral(1,(500/nBin2)))); 
  }
}
