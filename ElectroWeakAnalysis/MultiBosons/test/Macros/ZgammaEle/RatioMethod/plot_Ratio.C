#include <TF1.h>

void plot_Ratio(){
  plot_Ratio(10,3,5,10, 30, 60, 180);
}

void plot_Ratio(Int_t nIt, Int_t binS1, Int_t binS2, Int_t binS3, Double_t PtR1, Double_t PtR2, Double_t FitU){
  
  Int_t    DoFit=1;
  Int_t    nBin=5;
  Double_t FitL=18;
  
  gROOT->LoadMacro("tdrStyle.C");
  setTDRStyle();

  //Load File Name...
  //TString MCRatio      = "../RatioMethod/MC/MCRatio_Test.root";
  TString MCEvtYield   = "../Test_eID11_pID8_dR07.root";
  TString DataRatio    = "Ratio_Test_Trk3ToInf_x.root";
  TString DataEvtYield = "../Z_Data_Sep06_dr07.root";
  TString OutPutFileName="EstimationResult.root";

  //Some parameters setting
  Double_t McLumi =1.;     //MC lumi for bkg samples
  Double_t McLumi2=3.89;   //MC lumi for event yield
  Double_t McWeight;
  Int_t    nBin2=5;         //Estimation binning

  //Other variables
  Double_t pEB[6] ={0,0,0,0,0,0}, pEE[6] ={0,0,0,0,0,0};
  Double_t prEB[6]={0,0,0,0,0,0}, prEE[6]={0,0,0,0,0,0};

  TString TName_Num[31]= {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30"};

  Int_t    nfile, ProcessID;
  TString  name;
  Float_t  Temp_ratio=0,Temp_err=0;
  Float_t  Temp_V1=0, Temp_V2=0, Temp_err2=0,Temp_err3=0;

  //Binning test
 
  Int_t SizeN;
  SizeN = int((PtR1-18)/binS1) + int((PtR2-PtR1)/binS2) + int((FitU-PtR2)/binS3) + 1;
  Int_t BinValue[100];
  Int_t ct1=0,ct2=0;
  Double_t TempV=0;

  for (Int_t ii=0;ii<SizeN;ii++){
     TempV = 18+ii*binS1;
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
   
  TCanvas *c1 = new TCanvas("C1","C1",900,600);

  TLegend *legend = new TLegend(.65,.67,.95,.92);
  legend->AddEntry(hmIso[0][0],"MC","f");
  legend->AddEntry(hdIso[0][0],"data"  ,"lp");
  legend->SetFillColor(0);
  legend->SetFillStyle(0);
  legend->SetLineColor(0);
  legend->SetShadowColor(0); 

  TLegend *legend2 = new TLegend(.24,.6,.54,.78);
  if (DoFit==0)  legend2->AddEntry(hmR[0],"MC","lp");
  legend2->AddEntry(hdR[0],"data"  ,"lp");
  legend2->SetFillColor(0);
  legend2->SetFillStyle(0);
  legend2->SetLineColor(0);
  legend2->SetShadowColor(0);

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
  TF1 *FrealEB = new TF1("FrealEB","[0]*exp([1]*(x))+[2]",FitL,FitU);
  TF1 *FrealEE = new TF1("FrealEE","[0]*exp([1]*(x))+[2]",FitL,FitU);

  TF1 *FallEB  = new TF1("FallEB" ,"[0]*exp([1]*(x))+[2] + [3]*exp([4]*(x))+[5]",FitL,FitU);
  TF1 *FallEE  = new TF1("FallEE" ,"[0]*exp([1]*(x))+[2] + [3]*exp([4]*(x))+[5]",FitL,FitU);

  TF1 *Fall2EB  = new TF1("Fall2EB" ,"[0]*exp([1]*(x))+[2] + [3]*exp([4]*(x))+[5]",FitL,FitU);
  TF1 *Fall2EE  = new TF1("Fall2EE" ,"[0]*exp([1]*(x))+[2] + [3]*exp([4]*(x))+[5]",FitL,FitU);
  TF1 *Fall3EB  = new TF1("Fall3EB" ,"[0]*exp([1]*(x))+[2] + [3]*exp([4]*(x))+[5]",FitL,FitU);
  TF1 *Fall3EE  = new TF1("Fall3EE" ,"[0]*exp([1]*(x))+[2] + [3]*exp([4]*(x))+[5]",FitL,FitU);

  FrealEB-> SetParameters(0.15, 0.01,0);
  FrealEE-> SetParameters(0.15, 0.01,0);

  FallEB -> SetParameters(0.15, 0.01,0,0.01,-0.1,0.01);
  FallEE -> SetParameters(0.15, 0.01,0,0.01,-0.1,0.01);

  Fall2EB-> SetParameters(0.15, 0.01,0,0.01,-0.01,0.01);
  Fall2EE-> SetParameters(0.15, 0.01,0,0.01,-0.01,0.01);

  Fall3EB-> SetParameters(0.15, 0.01,0,0.01,-0.01,0.01);
  Fall3EE-> SetParameters(0.15, 0.01,0,0.01,-0.01,0.01);

  if (DoFit==1){
    //get initial values of p0~p5
    hdR[0]->Fit("FrealEB","Q","",100,FitU);
    hdR[1]->Fit("FrealEE","Q","",100,FitU);

    for (Int_t ii=0;ii<=2;ii++){
      pEB[ii]=FrealEB->GetParameter(ii);
      pEE[ii]=FrealEE->GetParameter(ii);

      FallEB -> FixParameter(ii,pEB[ii]);
      FallEE -> FixParameter(ii,pEE[ii]);
    }

    FallEB -> SetParLimits(3,0,5);
    FallEE -> SetParLimits(3,0,5);
    FallEB -> SetParLimits(4,-5,0);
    FallEE -> SetParLimits(4,-5,0);
    FallEB -> SetParLimits(5,0,0.5);
    FallEE -> SetParLimits(5,0,0.5);

    hdR[0]->Fit("FallEB","Q","",FitL,FitU);
    hdR[1]->Fit("FallEE","Q","",FitL,FitU);

    //Start iteration
    for (Int_t it=0;it<nIt;it++){

      for (Int_t jj=0;jj<6;jj++){
        Fall2EB->ReleaseParameter(jj);
        Fall2EE->ReleaseParameter(jj);
      }

      for (Int_t ii=3;ii<=5;ii++){
        pEB[ii]=FallEB->GetParameter(ii);
        pEE[ii]=FallEE->GetParameter(ii);

        Fall2EB -> FixParameter(ii,pEB[ii]);
        Fall2EE -> FixParameter(ii,pEE[ii]);
      }

      Fall2EB -> SetParLimits(1,0,15);
      Fall2EE -> SetParLimits(1,0,15);

      hdR[0]->Fit("Fall2EB","Q","",FitL,FitU);
      hdR[1]->Fit("Fall2EE","Q","",FitL,FitU);

      for (Int_t jj=0;jj<6;jj++){
        FallEB->ReleaseParameter(jj);
        FallEE->ReleaseParameter(jj);
      }

      for (Int_t ii=0;ii<=2;ii++){
        pEB[ii]=Fall2EB->GetParameter(ii);
        pEE[ii]=Fall2EE->GetParameter(ii);
        
        FallEB -> FixParameter(ii,pEB[ii]);
        FallEE -> FixParameter(ii,pEE[ii]);
      }
      FallEB -> SetParLimits(3,0,5);
      FallEE -> SetParLimits(3,0,5);
      FallEB -> SetParLimits(4,-5,0);
      FallEE -> SetParLimits(4,-5,0);
      FallEB -> SetParLimits(5,0.,0.2);
      FallEE -> SetParLimits(5,0.,0.2);

      hdR[0]->Fit("FallEB","Q","",FitL,FitU);
      hdR[1]->Fit("FallEE","Q","",FitL,FitU);

      for (Int_t ii=3;ii<=5;ii++){
        pEB[ii]=FallEB->GetParameter(ii);
        pEE[ii]=FallEE->GetParameter(ii);
        prEB[ii]=FallEB->GetParError(ii);
        prEE[ii]=FallEE->GetParError(ii);
      }
    }
  }
  //Make plot...

  TCanvas *c2 = new TCanvas("C2","C2",900,450);
  c2->Divide(2,1);

  for (Int_t kk=0;kk<2;kk++){
    c2->cd(kk+1);
    hdR[kk]->GetXaxis() -> SetRangeUser(FitL,FitU);
    hdR[kk]->GetYaxis() -> SetRangeUser(0,0.4);
    hdR[kk]->GetXaxis() -> SetTitle("phoEt");
    hdR[kk]->GetYaxis() -> SetTitle("Ratio (iso/non-iso)");
    hdR[kk]->Draw();
    if (DoFit ==0 )hmR[kk]->Draw("same");
    legend2->Draw();
    TLatex latexLabel;
    latexLabel.SetTextSize(0.035);
    latexLabel.SetNDC();
    latexLabel.DrawLatex(0.2, 0.96, "CMS Preliminary 2010");
    latexLabel.DrawLatex(0.24, 0.88, "#sqrt{s} = 7 TeV");
    latexLabel.DrawLatex(0.24, 0.8, "#int#font[12]{L} dt= 5.45 pb^{-1}");
  }

  //---------------------------------------------------------------

  //Check the Ratio
  if (DoFit==1){
    TF1 *PEB = new TF1("PEB","([0]*exp([1]*x)+[2])/([3]*exp([4]*x)+[5]+[6]*exp([7]*x)+[8])",FitL,FitU);
    TF1 *PEE = new TF1("PEE","([0]*exp([1]*x)+[2])/([3]*exp([4]*x)+[5]+[6]*exp([7]*x)+[8])",FitL,FitU);

    TF1 *FakeEB = new TF1("FakeEB","[0]*exp([1]*(x))+[2]",FitL,FitU);
    TF1 *FakeEE = new TF1("FakeEE","[0]*exp([1]*(x))+[2]",FitL,FitU);

    for (Int_t ii=0;ii<3;ii++){
      FakeEB->FixParameter(ii,pEB[ii+3]);
      FakeEE->FixParameter(ii,pEE[ii+3]);
    }

    TFile *fmc2 = new TFile(MCEvtYield);
    TH1D *hZIEB,*hZIEB,*hZNEB,*hZNEE,*hZR1,*hZR2;
    hT1=(TH1D*)fmc2->Get("Sample_0/IsoPho_Loose_EB");    hZIEB = (TH1D*) hT1 -> Clone();
    hT1=(TH1D*)fmc2->Get("Sample_0/NonIsoPho_Loose_EB"); hZNEB = (TH1D*) hT1 -> Clone();
    hT1=(TH1D*)fmc2->Get("Sample_0/IsoPho_Loose_EE");    hZIEE = (TH1D*) hT1 -> Clone();
    hT1=(TH1D*)fmc2->Get("Sample_0/NonIsoPho_Loose_EE"); hZNEE = (TH1D*) hT1 -> Clone();

    hZIEB->Rebin(nBin2);
    hZIEB->Scale(343.51);
    hZIEB->Sumw2();
    hZNEB->Rebin(nBin2);
    hZNEB->Scale(343.51);
    hZNEB->Sumw2();
    hZIEE->Rebin(nBin2);
    hZIEE->Scale(343.51);
    hZIEE->Sumw2();
    hZNEE->Rebin(nBin2);
    hZNEE->Scale(343.51);
    hZNEE->Sumw2();

    hZR1= new TH1D("ZR2", "", (500/nBin2), 0, 500);
    hZR2= new TH1D("ZR1", "", (500/nBin2), 0, 500);

    hZR1->Divide(hZIEB,hZNEB);
    hZR2->Divide(hZIEE,hZNEE);

    TLegend *legend3 = new TLegend(.62,.6,.95,.82);
    legend3->AddEntry(hZR1,"MC Z+Jet","lp");
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
    latexLabel.DrawLatex(0.62, 0.88, "#int#font[12]{L} dt= 5.45 pb^{-1}");

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
    latexLabel.DrawLatex(0.62, 0.88, "#int#font[12]{L} dt= 5.45 pb^{-1}");
    legend3->Draw();

    //Fit Results
    TFile *f_ZNIso = new TFile(DataEvtYield);
    Double_t Temp_Num=0;
    Double_t Temp_Est=0;
    Double_t TotEsEB=0,TotEsEE=0;  

    hT0 = (TH1F*) f_ZNIso->Get("NonIsoPho_Loose_EB"); hT0->Rebin(nBin2);
    hT2 = (TH1F*) f_ZNIso->Get("NonIsoPho_Loose_EE"); hT2->Rebin(nBin2); 

    TH1F *hEstEE;
    TH1F *hEstEB;

    hEstEB = (TH1F*) hT0->Clone();
    hEstEE = (TH1F*) hT2->Clone();

    for(Int_t ii=1;ii<=int(500/nBin2);ii++){
      Temp_Num = hT0 ->GetBinContent(ii);
      Temp_Est  = (pEB[3]*TMath::Exp(pEB[4]*((ii-1)*nBin2+(0.5*nBin2)))+pEB[5])*(Temp_Num);
      hEstEB->SetBinContent(ii,Temp_Est);

      TotEsEB += Temp_Est;

      Temp_Num = hT2 ->GetBinContent(ii);
      Temp_Est = (pEE[3]*TMath::Exp(pEE[4]*((ii-1)*nBin2+(0.5*nBin2)))+pEE[5])*(Temp_Num);
      hEstEE->SetBinContent(ii,Temp_Est);

      TotEsEE += Temp_Est;
    }  

    Float_t CentEB=0,CentEE=0;
    Float_t TotsErr=0;
    CentEB=TotEsEB;
    CentEE=TotEsEE;

    cout<<CentEB+CentEE<<endl;
    hEstEB->SetName("Estimated_Bkg_EB");
    hEstEE->SetName("Estimated_Bkg_EE");

    TFile *fout= new TFile(OutPutFileName,"RECREATE");
    fout->cd();
    hEstEB->Write();
    hEstEE->Write();
    fout->Close();

    //cout<<"Ploting..."<<endl;
    TH1D  *hmEE;
    TH1D  *hmEB;
    
    hT1 = (TH1D*)fmc2->Get("hInfo");
    nfile = hT1->GetBinContent(1);
    for (Int_t iSample=0;iSample<nfile;iSample++){
      name = "Sample_"+TName_Num[iSample]+"/hNEvt";
      hT1 = (TH1D*)fmc2->Get(name); 
      ProcessID = int( hT1->GetBinContent(14) );
      //cout<<ProcessID<<endl;
      if (ProcessID==1){
        hmEB = (TH1D*) fmc2 -> Get("Sample_"+TName_Num[iSample]+"/IsoPho_Loose_EB");
        hmEB ->Rebin(nBin2);
        hmEE = (TH1D*) fmc2 -> Get("Sample_"+TName_Num[iSample]+"/IsoPho_Loose_EE");
        hmEE ->Rebin(nBin2);
      }
    }     
    hmEB ->Scale(McLumi2);
    hmEE ->Scale(McLumi2);
    hEstEB->Sumw2();
    hEstEE->Sumw2();

    TLegend *legendR = new TLegend(.62,.6,.95,.82);
    legendR->AddEntry(hmEE,"MC Z+Jet","f");
    legendR->AddEntry(hEstEE,"Data"  ,"lp");
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
    hmEB->GetYaxis() -> SetTitle("background from Z+jet");
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
    hmEE->GetYaxis() -> SetTitle("background from Z+jet");
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
