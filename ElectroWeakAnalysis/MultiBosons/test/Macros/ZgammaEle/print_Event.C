#include <fstream>
#include <iomanip>
#include <iostream>

void print_Event(){
  //Input file name without .root
  print_Event("Test_eID11_pID8_dR07");
}

void print_Event(TString LoadFileName){
  
  int ProcessID(0);

  Double_t EventYield[7][6];
  Double_t PhotonType[7][6];
  //Set luminosity(/pb)
  Double_t McLumi=10.94;

  for (Int_t ii=0; ii<7;ii++) {
     for (Int_t jj=0; jj<6; jj++) {
        EventYield[ii][jj]=0;  
        PhotonType[ii][jj]=0;
  } }

  cout<<LoadFileName<<endl;
  TFile *f1 = new TFile(LoadFileName+".root");
  TH1D  *hT0;
  int nfile(0);
  nfile=hInfo->GetBinContent(1);

  int Temp_index(0);

  char name[200];
 
  for (Int_t iSample=0;iSample<nfile;iSample++){
     sprintf(name,"Sample_%d/hNEvt",iSample);
     hT0 = (TH1D*)f1->Get(name);
     cout<<iSample<<endl;
     ProcessID = int( hT0->GetBinContent(14) );
     for (Int_t ii=0;ii<6;ii++) {
       EventYield[ProcessID][ii] += hT0->GetBinContent(ii+1);
     }


     for (Int_t ii=6;ii<11;ii++) PhotonType[ProcessID][ii-6] += hT0->GetBinContent(ii+1);
     PhotonType[ProcessID][3] += PhotonType[ProcessID][4];

  }//Loop: get histogram

  for (Int_t jj=0;jj<6;jj++){
     for (Int_t ii=0;ii<7;ii++){
        cout<<setw(11)<<EventYield[ii][jj]*McLumi;
     }
     cout<<endl;
  }
  

  //Write event yield:
  fstream writefile;
  writefile.open(LoadFileName+"_Event.txt",ios::out);

  TString TypeString1[6]={"elePt","eleID","Zmass","phoEt","phoID","dR>0.7"};
  TString TypeString2[6]={"FSR1","ISR","FSR2","Fake","Unknown",""};

  writefile<<setw(21)<<"Zg"<<setw(11)<<"ZJet"<<setw(11)<<"WJet"<<setw(11)<<"DiBoson"<<setw(11)<<"TTbar"<<setw(11)<<"PJet"<<setw(11)<<"QCD"<<endl;
  for (Int_t jj=0;jj<6;jj++){
     writefile<<setw(11)<<TypeString1[jj];
     for (Int_t ii=0;ii<7;ii++){
        writefile<<setw(11)<<EventYield[ii][jj]*McLumi;
     }             
     writefile<<endl; 
  }          
  writefile<<"-------------------------------------------------------------------------------"<<endl;
  writefile<<endl;
  writefile<<"In those final event:"<<endl;
  writefile<<endl;
  for (Int_t jj=0;jj<4;jj++){
     writefile<<setw(11)<<TypeString2[jj];
     for (Int_t ii=0;ii<7;ii++){
        writefile<<setw(11)<<PhotonType[ii][jj]*McLumi;
     }  
     writefile<<endl;
  }
}
