#include <fstream>
#include <iomanip>
#include <iostream>

void CombineHistogram(){
  //Load file....
  //MC..
  TFile *f1 = new TFile("Zgamma_MC36x_eID95_pIDLoose_dR07_1pb_wPhotonLeptonSkim.root");
  //Data..
  TFile *f2 = new TFile("/data2/zkliu/VgNtuple/7TeV_data/MinBiasJetMetTau_May27_v2/SearchZg/Z_Data_Aug30_1p2pb_wCIso_dr7.root");

  Double_t McLumi=1.31255;//pb^-1
  Double_t kFactor[22]={1.3705,1.2481,1.8,1.8,1.5371,1.7333,1.3721,1.6578,1.,1.,1.,1.,1.,1.,
                        1.,1.,1.,1.,1.,1.,1.,1.};

  //Set Bin size and range
  //0-8:ecaliso, hcaliso, trkiso, combinedIso, sigmaietaieta, h/e, dEtaIn, dPhiIn
  Int_t   VnBin[8]={70,70,70,70,100,15,100,100};
  Float_t VuBin[8]={34,34,34,34,0.1,0.15,0.1,0.1};
  Float_t VlBin[8]={-1,-1,-1,-1,0,0,0,0};

  TString TName_Var[8] = {"EIso","HIso","TIso","CIso","SIE","HoE","dEtaIn","dPhiIn"};
  TString TName_Num[31]= {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20"
                         ,"21","22","23","24","25","26","27","28","29","30"};
  TString TName_EE[2]  = {"EB","EE"};
  TString TName_Var2;
  TString TName_Var3[8]={"EcalIso","HcalIso","TrkIso","ComIso","#sigma_{I#etaI#eta}","H/E","|#Delta#eta_{in}|","|#Delta#phi_{in}|"};
  TString TName_Pro[4]={"MC_Zg","MC_ZJetZg","MC_All","Data"};
  TString TName_Cut1[3]={"AfterEle","AfterZ","AfterAll"};
  TString TName_Cut2[3]={"AfterPho","AfterPhoID","AfterAll"};
  TString TName_Cut3[3]={"AfterZ","AfterPhoID","AfterAll"};
  TString TName_Cut4[3]={"AfterPhoID","AfterAll","empty"};

  Int_t   ProcessID=0;

  TH1D *hMee[4][3],  *hMeeg[4][3];
  TH1D *hpEt[4][3],  *hePt[4][3];
  TH1D *heEta[4][3], *hpEta[4][3];

  TH1D *hpVar[4][3][6][2];
  TH1D *heVar[4][3][8][2];
  TH1D *hdR[4];

  TH1D *hT0; 
  TH1D *hT1;
  TH1D *hT3;
  TH2D *hT2;

  for (Int_t ii=0;ii<4;ii++) {
  for (Int_t jj=0;jj<3;jj++) {
     TName_Var2 ="Mee_"+TName_Pro[ii]+"_"+TName_Cut3[jj];
     hMee[ii][jj]  = new TH1D(TName_Var2,TName_Var2,40,0,200);
     TName_Var2 = "Meeg_"+TName_Pro[ii]+"_"+TName_Cut4[jj];
     hMeeg[ii][jj] = new TH1D(TName_Var2,TName_Var2,40,0,200);
     
     TName_Var2 = "phoEt_"+TName_Pro[ii]+"_"+TName_Cut2[jj];
     hpEt[ii][jj]  = new TH1D(TName_Var2,TName_Var2,40,0,200);
     
     TName_Var2 = "elePt_"+TName_Pro[ii]+"_"+TName_Cut1[jj];
     hePt[ii][jj]  = new TH1D(TName_Var2,TName_Var2,40,0,200);
     
     if (jj==0){
       TName_Var2 ="dR_"+TName_Pro[ii];
       hdR[ii] = new TH1D(TName_Var2,TName_Var2,100,0,5);
     }
     TName_Var2="eleEta_"+TName_Pro[ii]+"_"+TName_Cut1[jj];;
     heEta[ii][jj] = new TH1D(TName_Var2,TName_Var2,30,-3,3);
     TName_Var2="phoEta_"+TName_Pro[ii]+"_"+TName_Cut2[jj];
     hpEta[ii][jj] = new TH1D(TName_Var2,TName_Var2,30,-3,3);

     for (Int_t kk=0;kk<2;kk++){
       for (Int_t ll=0;ll<8;ll++){
          TName_Var2="electron_"+TName_Pro[ii]+"_"+TName_Var[ll]+"_"+TName_EE[kk]+"_"+TName_Cut1[jj];
          heVar[ii][jj][ll][kk] = new TH1D(TName_Var2, TName_Var2, VnBin[ll], VlBin[ll], VuBin[ll]);
       }
       for (Int_t ll=0;ll<6;ll++){
          TName_Var2="photon_"+TName_Pro[ii]+"_"+TName_Var[ll]+"_"+TName_EE[kk]+"_"+TName_Cut2[jj];
          hpVar[ii][jj][ll][kk] = new TH1D(TName_Var2, TName_Var2, VnBin[ll], VlBin[ll], VuBin[ll]);
       }
     }
  }}
  
  int nfile(0);
  hT0 = (TH1D*)f1->Get("hInfo");
  nfile = hT0->GetBinContent(1);
  cout<<nfile<<endl;

  int Temp_index(0);
  TString name;

  //Get MC Histogram
  for (Int_t iSample=0;iSample<nfile;iSample++){
     name = "Sample_"+TName_Num[iSample]+"/hNEvt";
     hT0 = (TH1D*)f1->Get(name);
     ProcessID = int( hT0->GetBinContent(14) );

     if  (ProcessID==0) Temp_index=0; 
     else if (ProcessID==1) Temp_index=1;
     else if (ProcessID>1) Temp_index=2;

     for (Int_t ii=0;ii<3;ii++){
       name="Sample_"+TName_Num[iSample]+"/"+"hMee_"+TName_Num[ii+1];
       hT1 = ((TH1D*)f1->Get(name));
       hT1->Scale(kFactor[iSample]);
       hMee[Temp_index][ii]->Add(hT1);
       
       name="Sample_"+TName_Num[iSample]+"/"+"hMeeg_"+TName_Num[ii+1];
       hT1 = ((TH1D*)f1->Get(name));
       hT1->Scale(kFactor[iSample]);
       hMeeg[Temp_index][ii]->Add(hT1);

       name="Sample_"+TName_Num[iSample]+"/"+"hpEt"+TName_Num[ii+1];
       hT1 = ((TH1D*)f1->Get(name));
       hT1->Scale(kFactor[iSample]);
       hpEt[Temp_index][ii]->Add(hT1);

       name="Sample_"+TName_Num[iSample]+"/"+"heEt"+TName_Num[ii+1];
       hT1 = ((TH1D*)f1->Get(name));
       hT1->Scale(kFactor[iSample]);
       hePt[Temp_index][ii]->Add(hT1);

       name="Sample_"+TName_Num[iSample]+"/"+"heEta"+TName_Num[ii+1];
       hT1 = ((TH1D*)f1->Get(name));
       hT1->Scale(kFactor[iSample]);
       heEta[Temp_index][ii]->Add(hT1);

       name="Sample_"+TName_Num[iSample]+"/"+"hpEta"+TName_Num[ii+1];
       hT1 = ((TH1D*)f1->Get(name));
       hT1->Scale(kFactor[iSample]);
       hpEta[Temp_index][ii]->Add(hT1);
        
       if (ii==0) {
         name="Sample_"+TName_Num[iSample]+"/"+"hdR"+TName_Num[ii+1];
         hT1 = ((TH1D*)f1->Get(name));
         hT1->Scale(kFactor[iSample]);
         hdR[Temp_index]->Add(hT1);
       }

       for (Int_t kk=0;kk<2;kk++){
         for (Int_t ll=0;ll<8;ll++){
           name="Sample_"+TName_Num[iSample]+"/"+"he_"+TName_Var[ll]+"_"+TName_EE[kk]+"_"+TName_Num[ii+1];
           hT1 = ((TH1D*)f1->Get(name));
           hT1->Scale(kFactor[iSample]);
           heVar[Temp_index][ii][ll][kk]->Add(hT1);
         }
         for (Int_t ll=0;ll<6;ll++){
           name="Sample_"+TName_Num[iSample]+"/"+"hp_"+TName_Var[ll]+"_"+TName_EE[kk]+"_"+TName_Num[ii+1];
           hT1 = ((TH1D*)f1->Get(name));
           hT1->Scale(kFactor[iSample]);
           hpVar[Temp_index][ii][ll][kk]->Add(hT1);
         }
       } 
     }
  }//Loop: get histogram
  cout<<"Get MC Histogram!! done!!"<<endl; 
  //Get data histogram

  for (Int_t ii=0;ii<3;ii++){
       name="hMee_"+TName_Num[ii+1];
       hT1 = ((TH1D*)f2->Get(name));
       hMee[3][ii]->Add(hT1);

       name="hMeeg_"+TName_Num[ii+1];
       hT1 = ((TH1D*)f2->Get(name));
       hMeeg[3][ii]->Add(hT1);

       name="hpEt"+TName_Num[ii+1];
       hT1 = ((TH1D*)f2->Get(name));
       hpEt[3][ii]->Add(hT1);

       name="heEt"+TName_Num[ii+1];
       hT1 = ((TH1D*)f2->Get(name));
       hePt[3][ii]->Add(hT1);

       name="heEta"+TName_Num[ii+1];
       hT1 = ((TH1D*)f2->Get(name));
       heEta[3][ii]->Add(hT1);

       name="hpEta"+TName_Num[ii+1];
       hT1 = ((TH1D*)f2->Get(name));
       hpEta[3][ii]->Add(hT1);

       if (ii==0) {
         name="hdR"+TName_Num[ii+1];
         hT1 = ((TH1D*)f2->Get(name));
         hdR[3]->Add(hT1);
       }

       for (Int_t kk=0;kk<2;kk++){
         for (Int_t ll=0;ll<8;ll++){
           name="he_"+TName_Var[ll]+"_"+TName_EE[kk]+"_"+TName_Num[ii+1];
           hT1 = ((TH1D*)f2->Get(name));
           heVar[3][ii][ll][kk]->Add(hT1);
         }
         for (Int_t ll=0;ll<6;ll++){
           name="hp_"+TName_Var[ll]+"_"+TName_EE[kk]+"_"+TName_Num[ii+1];
           hT1 = ((TH1D*)f2->Get(name));
           hpVar[3][ii][ll][kk]->Add(hT1);
         }
       }
  }


  for (Int_t kk=0;kk<3;kk++){
    for (Int_t ii=0;ii<=1;ii++) hpEt[2][kk]->Add(hpEt[ii][kk]);
    hpEt[1][kk]->Add(hpEt[0][kk]);

    for (Int_t ii=0;ii<=1;ii++) hePt[2][kk]->Add(hePt[ii][kk]);
    hePt[1][kk]->Add(hePt[0][kk]);

    for (Int_t ii=0;ii<=1;ii++) heEta[2][kk]->Add(heEta[ii][kk]);
    heEta[1][kk]->Add(heEta[0][kk]);

    for (Int_t ii=0;ii<=1;ii++) hpEta[2][kk]->Add(hpEta[ii][kk]);
    hpEta[1][kk]->Add(hpEta[0][kk]);

    for (Int_t ii=0;ii<=1;ii++) hMee[2][kk]->Add(hMee[ii][kk]);
    hMee[1][kk]->Add(hMee[0][kk]);

    for (Int_t ii=0;ii<=1;ii++) hMeeg[2][kk]->Add(hMeeg[ii][kk]);
    hMeeg[1][kk]->Add(hMeeg[0][kk]);

    if (kk==0){
      for (Int_t ii=0;ii<=1;ii++) hdR[2]->Add(hdR[ii]);
      hdR[1]->Add(hdR[0]);
    }

    for (Int_t jj=0;jj<2;jj++){
         for (Int_t ll=0;ll<8;ll++){
            for (Int_t ii=0;ii<=1;ii++) heVar[2][kk][ll][jj]->Add(heVar[ii][kk][ll][jj]);
            heVar[1][kk][ll][jj]->Add(heVar[0][kk][ll][jj]);
         }
         for (Int_t ll=0;ll<6;ll++){
            for (Int_t ii=0;ii<=1;ii++) hpVar[2][kk][ll][jj]->Add(hpVar[ii][kk][ll][jj]);
            hpVar[1][kk][ll][jj]->Add(hpVar[0][kk][ll][jj]);
         }
    }  
  }

  //Scale MC to Luminosity
  for (Int_t ii=0;ii<3;ii++){
     for (Int_t jj=0;jj<3;jj++){
       hePt[ii][jj]  ->Scale(McLumi);
       heEta[ii][jj] ->Scale(McLumi); 
       hpEt[ii][jj]  ->Scale(McLumi);
       hpEta[ii][jj] ->Scale(McLumi);
       hMee[ii][jj]  ->Scale(McLumi);
       hMeeg[ii][jj] ->Scale(McLumi);
       for (Int_t kk=0;kk<2;kk++){
         for (Int_t ll=0;ll<8;ll++) heVar[ii][jj][ll][kk] ->Scale(McLumi);
         for (Int_t ll=0;ll<6;ll++) hpVar[ii][jj][ll][kk] ->Scale(McLumi);
       }
     }
     hdR[ii] ->Scale(McLumi);
  }
  
  cout<<"Combine done!"<<endl;
  cout<<"Save plot...."<<endl;

  TFile *fw = new TFile("Histogram.root","RECREATE");
  fw->cd();
  TH1D *hMee[4][3],  *hMeeg[4][3];
  TH1D *hpEt[4][3],  *hePt[4][3];
  TH1D *heEta[4][3], *hpEta[4][3];

  TH1D *hpVar[4][3][6][2];
  TH1D *heVar[4][3][8][2];
  TH1D *hdR[4];
  for (Int_t ii=0;ii<4;ii++){
     for (Int_t jj=0;jj<3;jj++){
       hePt[ii][jj]  -> Write();
       heEta[ii][jj] -> Write();
       hpEt[ii][jj]  -> Write();
       hpEta[ii][jj] -> Write();
       hMee[ii][jj]  -> Write();
       hMeeg[ii][jj] -> Write();
       for (Int_t kk=0;kk<2;kk++){
         for (Int_t ll=0;ll<8;ll++) heVar[ii][jj][ll][kk]->Write();
         for (Int_t ll=0;ll<6;ll++) hpVar[ii][jj][ll][kk]->Write();
       }
     }
     hdR[ii]->Write();
  }
  fw->Close();
}
