{
#include "TCanvas.h"
#include <TStyle.h>
//   hfile->Close();
   //======================================================================
      printf("z1: gROOT Reset \n");
        gROOT->Reset();
        gROOT->SetStyle("Plain");
        gStyle->SetTitleFont(42);           // title font
        gStyle->SetTitleFontSize(0.07);     // title font size
        gStyle->SetLabelSize(0.06,"xy");     //
        gStyle->SetStatFontSize(0.05);     //for box size
        gStyle->SetStatX(0.97);     // for box x position
        gStyle->SetStatW(0.25);     // for box width
        gStyle->SetOptFit(1111); // pcev
/*
        gStyle->SetOptStat(0);   //  no statistics _or_
          gStyle->SetStatX(0.91);
          gStyle->SetStatY(0.75);
          gStyle->SetStatW(0.20);
          gStyle->SetStatH(0.10);
        Float_t LeftOffset = 0.12;
        Float_t TopOffset = 0.22;
                                                                                                                                      
        gStyle->SetLineWidth(1);
        //  gStyle->SetLineWidth(1);
        gStyle->SetErrorX(0);
//---=[ Titles,Labels ]=-----------
        gStyle->SetOptTitle(1);             // title on/off
        //      gStyle->SetTitleColor(0);           // title color
        gStyle->SetTitleColor(1);           // title color
        //      gStyle->SetTitleX(0.35);            // title x-position
        gStyle->SetTitleX(0.15);            // title x-position
        gStyle->SetTitleH(0.15);             // title height
        //      gStyle->SetTitleW(0.53);            // title width
        gStyle->SetTitleW(0.60);            // title width
                                                                                                                                      
//---=[ Histogram style ]=----------
      gStyle->SetHistFillColor(45);
      gStyle->SetHistFillStyle(3003);
      gStyle->SetFrameFillColor(41);
                                                                                                                                      
//---=[ Pad style ]=----------------
        gStyle->SetPadTopMargin(TopOffset);
        gStyle->SetPadBottomMargin(LeftOffset);
        gStyle->SetPadRightMargin(TopOffset);
        gStyle->SetPadLeftMargin(LeftOffset);
                                                                                                                                      
*/
// for one root file
      TFile *hfile1 = new TFile("Golden.root", "READ");     //open file
      TFile *hfile2 = new TFile("Good.root", "READ");     //open file
      TFile *hfile3 = new TFile("Calo.root", "READ");     //open file
// creat histo - summ of 3 files
   TH2F *hIdPhiPlusVsESUM = new TH2F("hIdPhiPlusVsESUM","IdPhiTower vs Emuon, HB+",  60, -2, 10, 74, -0.5, 73.5); // x, y coordiants
   TH2F *hIdPhiMinusVsESUM = new TH2F("hIdPhiMinusVsESUM","IdPhiTower vs Emuon, HB-",  60, -2, 10, 74, -0.5, 73.5); // x, y coordiants
   TH2F *hIdEtaTopVsESUM = new TH2F("hIdEtaTopVsESUM","IdEtaTower vs Emuon, Top HB",  60, -2, 10, 57, -14.25, 14.25); // x, y coordiants
   TH2F *hIdEtaBotVsESUM = new TH2F("hIdEtaBotVsESUM","IdEtaTower vs Emuon, Bottom HB",  60, -2, 10, 57, -14.25, 14.25); // x, y coordiants

      hfile1 -> cd();
      hIdPhiPlusVsESUM -> Add(hIdPhiPlusVsESUM,hIdPhiPlusVsE);  
      hIdPhiMinusVsESUM -> Add(hIdPhiMinusVsESUM,hIdPhiMinusVsE);  
      hIdEtaTopVsESUM -> Add(hIdEtaTopVsESUM,hIdEtaTopVsE);  
      hIdEtaBotVsESUM -> Add(hIdEtaBotVsESUM,hIdEtaBotVsE);  
      hfile2 -> cd();
      hIdPhiPlusVsESUM -> Add(hIdPhiPlusVsESUM,hIdPhiPlusVsE);  
      hIdPhiMinusVsESUM -> Add(hIdPhiMinusVsESUM,hIdPhiMinusVsE);  
      hIdEtaTopVsESUM -> Add(hIdEtaTopVsESUM,hIdEtaTopVsE);  
      hIdEtaBotVsESUM -> Add(hIdEtaBotVsESUM,hIdEtaBotVsE);  
      hfile3 -> cd();
      hIdPhiPlusVsESUM -> Add(hIdPhiPlusVsESUM,hIdPhiPlusVsE);  
      hIdPhiMinusVsESUM -> Add(hIdPhiMinusVsESUM,hIdPhiMinusVsE);  
      hIdEtaTopVsESUM -> Add(hIdEtaTopVsESUM,hIdEtaTopVsE);  
      hIdEtaBotVsESUM -> Add(hIdEtaBotVsESUM,hIdEtaBotVsE);  
////////////////////////////////////////////////
// make histo projection for hIdPhiPlusVsESUM

   Int_t nbiny = 74; // number or IdPhiTowers + 2
   Int_t nbinx = 60; // number of bins for Energy
   TH1F *hEmuon = new TH1F("hEmuon","Emuom for one phi slice", 60, -2,10);
   TH1F *hEmuonGaussVsIdPhiTowersPlus = 
       new TH1F("hEmuonGaussVsIdPhiTowersPlus","Emuom mean Gauss as IdPhiTowers, HB+", 72, 0.5,72.5);
   TH1F *hEmuonGaussVsIdPhiTowersMinus = 
       new TH1F("hEmuonGaussVsIdPhiTowersMinus","Emuom mean Gauss as IdPhiTowers, HB-", 72, 0.5,72.5);
   TH1F *hEmuonGaussSigmaVsIdPhiTowersPlus = 
       new TH1F("hEmuonGaussSigmaVsIdPhiTowersPlus","Emuom sigma Gauss as IdPhiTowers, HB+", 72, 0.5,72.5);
   TH1F *hEmuonGaussSigmaVsIdPhiTowersMinus = 
       new TH1F("hEmuonGaussSigmaVsIdPhiTowersMinus","Emuom sigma Gauss as IdPhiTowers, HB-", 72, 0.5,72.5);
   TH1F *hEmeanGaussTopPlus = new TH1F("hEmeanGaussTopPlus","Emean Gauss Top HB+, IdPhi: 11-27",40,1.4,2.3); 
   TH1F *hEmeanGaussBotPlus = new TH1F("hEmeanGaussBotPlus","Emean Gauss Bottom HB+, IdPhi: 47-62",40,1.4,2.3); 
   TH1F *hEmeanGaussTopMinus = new TH1F("hEmeanGaussTopMinus","Emean Gauss Top HB-, IdPhi: 11-27",40,1.4,2.3); 
   TH1F *hEmeanGaussBotMinus = new TH1F("hEmeanGaussBotMinus","Emean Gauss Bottom HB-, IdPhi: 47-62",40,1.4,2.3); 
   TH1F *hEmeanGauss = new TH1F("hEmeanGauss","Emean Gauss HB all, IdPhi ",40,1.4,2.3); 

   Float_t frac = 0.05; // frac - maximum relative error 
/*
   Int_t iPhiTopMin = 11; 
   Int_t iPhiTopMax = 27; 
   Int_t iPhiBotMin = 47; 
   Int_t iPhiBotMax = 62; 
*/

   Int_t iPhiTopPlusMin = 8; 
   Int_t iPhiTopMinusMin = 8; 
   Int_t iPhiTopPlusMax = 28; 
   Int_t iPhiTopMinusMax = 27; 
   Int_t iPhiBotPlusMin = 43; 
   Int_t iPhiBotMinusMin = 47; 
   Int_t iPhiBotPlusMax = 65; 
   Int_t iPhiBotMinusMax = 64; 

   // HB+
   for(Int_t ik=2; ik<=nbiny-1; ik++){ // make cicle by IdPhiTowers
      // creat histo
      hEmuon->Clear(); //Clean historgram contents = 0;
      for(Int_t jk=1; jk<=nbinx; jk++){ // make cicle by Energy bins 
          Int_t bin2 = hIdPhiPlusVsESUM->GetBin(jk,ik);// get index for 2d histo
          Double_t binContent = hIdPhiPlusVsESUM->GetBinContent(bin2); // get content    
          Double_t binError = hIdPhiPlusVsESUM->GetBinError(bin2); // get error
          hEmuon->SetBinContent(jk,binContent);    
          hEmuon->SetBinError(jk,binError);    
 
      } //end cicle by Energy bins   
      hEmuon->Fit("gaus","E","",0.8.,2.6);
      Double_t Emean = gaus->GetParameter(1);// value of the 2nd parameter (mean) 
      Double_t errEmean = gaus->GetParError(1);// value of the 2nd parameter (mean) 
      Double_t Esigma = gaus->GetParameter(2);// value of the 3nd parameter (mean) 
      Double_t errEsigma = gaus->GetParError(2);// value of the 3nd parameter (mean) 
      if(errEmean>fabs(Emean)*frac||errEmean>0.5){errEmean=0;Emean=0;Esigma=0;errEsigma=0;} 
      if(errEsigma>fabs(Esigma)/2){errEmean=0;Emean=0;Esigma=0;errEsigma=0;} 
       cout << " idPhi = "<< ik-1<<"par mean = "<< Emean <<"+/-"<<errEmean << std::endl;
      hEmuonGaussVsIdPhiTowersPlus->SetBinContent(ik-1,Emean); // ik-1 - IdPhiTower 
      hEmuonGaussVsIdPhiTowersPlus->SetBinError(ik-1,errEmean); 
      hEmuonGaussSigmaVsIdPhiTowersPlus->SetBinContent(ik-1,Esigma); 
      hEmuonGaussSigmaVsIdPhiTowersPlus->SetBinError(ik-1,errEsigma);
      if((ik-1)>=iPhiTopPlusMin&&(ik-1)<=iPhiTopPlusMax){
           hEmeanGaussTopPlus->Fill(Emean); 
           hEmeanGauss->Fill(Emean);
      }
      if((ik-1)>=iPhiBotPlusMin&&(ik-1)<=iPhiBotPlusMax){
           hEmeanGaussBotPlus->Fill(Emean); 
           hEmeanGauss->Fill(Emean);
      }
      //c9.WaitPrimitive();
   } // end  cicle by IdPhiTowers 
   // HB-
   for(Int_t ik=2; ik<=nbiny-1; ik++){ // make cicle by IdPhiTowers
      // creat histo
      hEmuon->Clear(); //Clean historgram contents = 0;
      for(Int_t jk=1; jk<=nbinx; jk++){ // make cicle by Energy bins
          Int_t bin2 = hIdPhiMinusVsESUM->GetBin(jk,ik);// get index for 2d histo
          Double_t binContent = hIdPhiMinusVsESUM->GetBinContent(bin2); // get content
          Double_t binError = hIdPhiMinusVsESUM->GetBinError(bin2); // get error
          hEmuon->SetBinContent(jk,binContent);
          hEmuon->SetBinError(jk,binError);
                                                                                                       
      } //end cicle by Energy bins
      hEmuon->Fit("gaus","E","",0.8.,2.6);
      Double_t Emean = gaus->GetParameter(1);// value of the 2nd parameter (mean)
      Double_t errEmean = gaus->GetParError(1);// value of the 2nd parameter (mean)
      Double_t Esigma = gaus->GetParameter(2);// value of the 3nd parameter (mean) 
      Double_t errEsigma = gaus->GetParError(2);// value of the 3nd parameter (mean) 
      if(errEmean>fabs(Emean)*frac||errEmean>0.5){errEmean=0;Emean=0;Esigma=0;errEsigma=0;}
      if(errEsigma>fabs(Esigma)/2){errEmean=0;Emean=0;Esigma=0;errEsigma=0;} 
       cout << " idPhi = "<< ik-1<<"par mean = "<< Emean <<"+/-"<<errEmean << std::endl;
      hEmuonGaussVsIdPhiTowersMinus->SetBinContent(ik-1,Emean);
      hEmuonGaussVsIdPhiTowersMinus->SetBinError(ik-1,errEmean);
      hEmuonGaussSigmaVsIdPhiTowersMinus->SetBinContent(ik-1,Esigma); 
      hEmuonGaussSigmaVsIdPhiTowersMinus->SetBinError(ik-1,errEsigma); 
      if((ik-1)>=iPhiTopMinusMin&&(ik-1)<=iPhiTopMinusMax&&(ik-1)!=24){
           hEmeanGaussTopMinus->Fill(Emean); // without problematic point 24 
           hEmeanGauss->Fill(Emean);
      }
      if((ik-1)>=iPhiBotMinusMin&&(ik-1)<=iPhiBotMinusMax){
           hEmeanGaussBotMinus->Fill(Emean); 
           hEmeanGauss->Fill(Emean);
      }
     // c8.WaitPrimitive();
   } // end  cicle by IdPhiTowers
   TH1F *hEmuonGaussVsIdEtaTowersTop =
       new TH1F("hEmuonGaussVsIdEtaTowersTop","Emuom mean Gauss as IdEtaTowers, Top HB", 57, -14.25, 14.25);
   TH1F *hEmuonGaussVsIdEtaTowersBot =
       new TH1F("hEmuonGaussVsIdEtaTowersBot","Emuom mean Gauss as IdEtaTowers, Bottom HB", 57, -14.25, 14.25);
   TH1F *hEmeanGaussTopPlusETA = new TH1F("hEmeanGaussTopPlusETA","Emean Gauss Top HB+, IdPhi: 11-27",40,1.4,2.3); 
   TH1F *hEmeanGaussBotPlusETA = new TH1F("hEmeanGaussBotPlusETA","Emean Gauss Bottom HB+, IdPhi: 47-62",40,1.4,2.3); 
   TH1F *hEmeanGaussTopMinusETA = new TH1F("hEmeanGaussTopMinusETA","Emean Gauss Top HB-, IdPhi: 11-27",40,1.4,2.3); 
   TH1F *hEmeanGaussBotMinusETA = new TH1F("hEmeanGaussBotMinusETA","Emean Gauss Bottom HB-, IdPhi: 47-62",40,1.4,2.3); 
   TH1F *hEmeanGaussETA = new TH1F("hEmeanGaussETA","Emean Gauss HB all, IdEta",40,1.4,2.3); 
   Int_t nbinyEta = 59;
   Int_t nbinxEta = 60;
   for(Int_t ik=1; ik<=nbinyEta; ik++){ // make cicle by IdEtaTowers
      // creat histo
      hEmuon->Clear(); //Clean historgram contents = 0;
      for(Int_t jk=1; jk<=nbinxEta; jk++){ // make cicle by Energy bins
          Int_t bin2 = hIdEtaTopVsESUM->GetBin(jk,ik);// get index for 2d histo
          Double_t binContent = hIdEtaTopVsESUM->GetBinContent(bin2); // get content
          Double_t binError = hIdEtaTopVsESUM->GetBinError(bin2); // get error
          hEmuon->SetBinContent(jk,binContent);
          hEmuon->SetBinError(jk,binError);
                                                                                                                                            
      } //end cicle by Energy bins
      hEmuon->Fit("gaus","E","",0.8.,2.6);
      Double_t Emean = gaus->GetParameter(1);// value of the 2nd parameter (mean)
      Double_t errEmean = gaus->GetParError(1);// value of the 2nd parameter (mean)
      Double_t Esigma = gaus->GetParameter(2);// value of the 3nd parameter (mean)
      Double_t errEsigma = gaus->GetParError(2);// value of the 3nd parameter (mean)
      if(errEmean>fabs(Emean)*frac||errEmean>0.5){errEmean=0;Emean=0;Esigma=0;errEsigma=0;}
      if(errEsigma>fabs(Esigma)/2){errEmean=0;Emean=0;Esigma=0;errEsigma=0;}
      cout << " idEta = "<< (float(ik)-29)/2 <<"par mean = "<< Emean <<"+/-"<<errEmean << std::endl;
      hEmuonGaussVsIdEtaTowersTop->SetBinContent(ik,Emean);
      hEmuonGaussVsIdEtaTowersTop->SetBinError(ik,errEmean);
      if(Emean!=0&&(float(ik)-29)/2<0){
           hEmeanGaussTopMinusETA->Fill(Emean);
           hEmeanGaussETA->Fill(Emean);
      }  
      if(Emean!=0&&(float(ik)-29)/2>0){
           hEmeanGaussTopPlusETA->Fill(Emean);
           hEmeanGaussETA->Fill(Emean);
      }  
      //c81.WaitPrimitive();
   } // end cicle by IdEtaTowers
   for(Int_t ik=1; ik<=nbinyEta; ik++){ // make cicle by IdEtaTowers
      // creat histo
      hEmuon->Clear(); //Clean historgram contents = 0;
      for(Int_t jk=1; jk<=nbinxEta; jk++){ // make cicle by Energy bins
          Int_t bin2 = hIdEtaBotVsESUM->GetBin(jk,ik);// get index for 2d histo
          Double_t binContent = hIdEtaBotVsESUM->GetBinContent(bin2); // get content
          Double_t binError = hIdEtaBotVsESUM->GetBinError(bin2); // get error
          hEmuon->SetBinContent(jk,binContent);
          hEmuon->SetBinError(jk,binError);
                                                                                                                                            
      } //end cicle by Energy bins
      hEmuon->Fit("gaus","E","",0.8.,2.6);
      Double_t Emean = gaus->GetParameter(1);// value of the 2nd parameter (mean)
      Double_t errEmean = gaus->GetParError(1);// value of the 2nd parameter (mean)
      Double_t Esigma = gaus->GetParameter(2);// value of the 3nd parameter (mean)
      Double_t errEsigma = gaus->GetParError(2);// value of the 3nd parameter (mean)
      if(errEmean>fabs(Emean)*frac||errEmean>0.5){errEmean=0;Emean=0;Esigma=0;errEsigma=0;}
      if(errEsigma>fabs(Esigma)/2){errEmean=0;Emean=0;Esigma=0;errEsigma=0;}
      cout << " idEta = "<< (float(ik)-29)/2 <<"par mean = "<< Emean <<"+/-"<<errEmean << std::endl;
      hEmuonGaussVsIdEtaTowersBot->SetBinContent(ik,Emean);
      hEmuonGaussVsIdEtaTowersBot->SetBinError(ik,errEmean);
      if(Emean!=0&&(float(ik)-29)/2<0){
           hEmeanGaussBotMinusETA->Fill(Emean);
           hEmeanGaussETA->Fill(Emean);
      }  
      if(Emean!=0&&(float(ik)-29)/2>0){
           hEmeanGaussBotPlusETA->Fill(Emean);
           hEmeanGaussETA->Fill(Emean);
      }  
      //c81.WaitPrimitive();
   } // end cicle by IdEtaTowers
////////////////////////////////////////////////
        gStyle->SetOptStat(0);   //  no statistics _or_
        gStyle->SetPadGridX(kTRUE);
        gStyle->SetPadGridY(kTRUE);
        //gStyle->SetGrid();   //  no statistics _or_
////////////////////////////////////////////////
   TCanvas *c9 = new TCanvas("c9","Emuon Gauss mean for different IdPhiTower, HB+");
   c9->Divide(1,2);
   c9->cd(1); 
   hEmuonGaussVsIdPhiTowersPlus->Draw();
   c9->cd(2); 
   hEmuonGaussVsIdPhiTowersMinus->Draw();
   c9->Print("picAll9.gif");
      c9.WaitPrimitive();
/////////////////////////////////////
   TCanvas *c10 = new TCanvas("c10","Emuon Gauss sigma for different IdPhiTower, HB+");
   c10->Divide(1,2);
   c10->cd(1); 
   hEmuonGaussSigmaVsIdPhiTowersPlus->Draw();
   c10->cd(2); 
   hEmuonGaussSigmaVsIdPhiTowersMinus->Draw();
   c10->Print("picAll10.gif");
      c10.WaitPrimitive();
/////////////////////////////////////
   TCanvas *c11 = new TCanvas("c11","Emuon Gauss mean for HB from PHI slice");
        gStyle->SetOptStat(1);   //  no statistics _or_
        gStyle->SetPadGridX(0);
        gStyle->SetPadGridY(0);
   c11->Divide(2,2);
   c11->cd(1); 
   hEmeanGaussTopMinus->Draw();
   c11->cd(2); 
   hEmeanGaussTopPlus->Draw();
   c11->cd(3); 
   hEmeanGaussBotMinus->Draw();
   c11->cd(4); 
   hEmeanGaussBotPlus->Draw();
   c11->Print("picAll11.gif");
      c11.WaitPrimitive();
/////////////////////////////////////
   TCanvas *c12 = new TCanvas("c12","Emuon Gauss mean for different IdEtaTower,HB");
        gStyle->SetOptStat(0);   //  no statistics _or_
        gStyle->SetPadGridX(1);
        gStyle->SetPadGridY(1);
   c12->Divide(1,2);
   c12->cd(1);
                                                                                                                                            
   hEmuonGaussVsIdEtaTowersTop->Draw();
   //hEmuonGaussVsIdEtaTowersTop -> Fit("pol1","same","S",-13,-1);
   //hEmuonGaussVsIdEtaTowersTop -> Fit("pol1","E","same",1,13);
   c12->cd(2);
   hEmuonGaussVsIdEtaTowersBot->Draw();
   //hEmuonGaussVsIdEtaTowersBot -> Fit("pol0","E","S",-13,-1);
   //hEmuonGaussVsIdEtaTowersBot -> Fit("pol0","E","same",1,13);
   c12->Print("picAll12.gif");
      c12.WaitPrimitive();
/////////////////////////////////////
/////////////////////////////////////
   TCanvas *c13 = new TCanvas("c13","Emuon Gauss mean for HB from ETA slice");
        gStyle->SetOptStat(1);   //  no statistics _or_
        gStyle->SetPadGridX(0);
        gStyle->SetPadGridY(0);
   c13->Divide(2,2);
   c13->cd(1); 
   hEmeanGaussTopMinusETA->Draw();
   c13->cd(2); 
   hEmeanGaussTopPlusETA->Draw();
   c13->cd(3); 
   hEmeanGaussBotMinusETA->Draw();
   c13->cd(4); 
   hEmeanGaussBotPlusETA->Draw();
   c13->Print("picAll13.gif");
      c13.WaitPrimitive();
/////////////////////////////////////
   TCanvas *c14 = new TCanvas("c14","Emuon Gauss mean for HB from ETA slice");
   c14->Divide(1,2);
   c14->cd(1); 
   hEmeanGauss->Draw();
   c14->cd(2); 
   hEmeanGaussETA->Draw();
   c14->Print("picAll14.gif");
      c14.WaitPrimitive();
// caluclate energy of 3 types of muons
   TH1F *hEmuonTopPlus = new TH1F("hEmuonTopPlus","Emoun, Top+", 60, -2., 10.);
   TH1F *hEmuonTopMinus = new TH1F("hEmuonTopMinus","Emoun, Top-", 60, -2., 10.);
   TH1F *hEmuonBotPlus = new TH1F("hEmuonBotPlus","Emoun, Bottom+", 60, -2., 10.);
   TH1F *hEmuonBotMinus = new TH1F("hEmuonBotMinus","Emoun, Bottom-", 60, -2., 10.);
   //Time
   TH1F *hTimeMuonTopPlus = new TH1F("hTimeMuonTopPlus","TimeMoun, Top+", 60, -150., 150.);
   TH1F *hTimeMuonTopMinus = new TH1F("hTimeMuonTopMinus","TimeMoun, Top-", 60, -150., 150.);
   TH1F *hTimeMuonBotPlus = new TH1F("hTimeMuonBotPlus","TimeMoun, Bottom+", 60, -150., 150.);
   TH1F *hTimeMuonBotMinus = new TH1F("hTimeMuonBotMinus","TimeMoun, Bottom-", 60, -150., 150.);
   // in different time:
   TH1F *hEmuonBotPlusTimePlus1 = new TH1F("hEmuonBotPlusTimePlus1","Emoun, Bot+, Time+1", 60, -2., 10.);
   TH1F *hEmuonBotPlusTimePlus2 = new TH1F("hEmuonBotPlusTimePlus2","Emoun, Bot+, Time+2", 60, -2., 10.);
   TH1F *hEmuonBotPlusTimePlus3 = new TH1F("hEmuonBotPlusTimePlus3","Emoun, Bot+, Time+3", 60, -2., 10.);
   TH1F *hEmuonBotPlusTimePlus4 = new TH1F("hEmuonBotPlusTimePlus4","Emoun, Bot+, Time+4", 60, -2., 10.);
   // fill TProfile for Time as iphi
   TProfile *hProfTimeAsIdPhiMinusSUM = new TProfile("hProfTimeAsIdPhiMinusSUM","mean Time as IdPhi for ETA-",72,0.5,72.5,-150,150);
   TProfile *hProfTimeAsIdPhiPlusSUM = new TProfile("hProfTimeAsIdPhiPlusSUM","mean Time as IdPhi for ETA+",72,0.5,72.5,-150,150);
   TProfile *hProfTimeAsIdEtaTopSUM = new TProfile("hProfTimeAsIdEtaTopSUM","mean Time as IdEta for Top",57,-14.25,14.25,-150,150);
   TProfile *hProfTimeAsIdEtaBotSUM = new TProfile("hProfTimeAsIdEtaBotSUM","mean Time as IdEta for Bot",57,-14.25,14.25,-150,150);
   //Number Towers in Eta Plane passing by good muon
   TH1F *hNumTowerMuon =
        new TH1F("hNumTowerMuon","Number Towers of Moun", 11, -0.5, 10.5);
   //idPhiTower for eta+ and eta-
   TH1F *hIdPhiTowerMinus = new TH1F("hIdPhiTowerMinus", "IdPhiTower for HB- ", 74, -0.5, 73.5);
   TH1F *hIdPhiTowerPlus = new TH1F("hIdPhiTowerPlus", "IdPhiTower for HB+", 74, -0.5, 73.5);
                                                                                                                                                    

      hfile1 -> cd();
      hEmuonTopPlus -> Add(hEmuonTopPlus,hEmuonHB2DTTopPlus);
      hEmuonTopMinus -> Add(hEmuonTopMinus,hEmuonHB2DTTopMinus);
      hEmuonBotPlus -> Add(hEmuonBotPlus,hEmuonHB2DTBotPlus);
      hEmuonBotMinus -> Add(hEmuonBotMinus,hEmuonHB2DTBotMinus);
      hTimeMuonTopPlus -> Add(hTimeMuonTopPlus,hTimeMuonHB2DTTopPlus);
      hTimeMuonTopMinus -> Add(hTimeMuonTopMinus,hTimeMuonHB2DTTopMinus);
      hTimeMuonBotPlus -> Add(hTimeMuonBotPlus,hTimeMuonHB2DTBotPlus);
      hTimeMuonBotMinus -> Add(hTimeMuonBotMinus,hTimeMuonHB2DTBotMinus);
      hEmuonBotPlusTimePlus1 -> Add(hEmuonBotPlusTimePlus1,hEmuonHB2DTBotPlusTimePlus1);
      hEmuonBotPlusTimePlus2 -> Add(hEmuonBotPlusTimePlus2,hEmuonHB2DTBotPlusTimePlus2);
      hEmuonBotPlusTimePlus3 -> Add(hEmuonBotPlusTimePlus3,hEmuonHB2DTBotPlusTimePlus3);
      hEmuonBotPlusTimePlus4 -> Add(hEmuonBotPlusTimePlus4,hEmuonHB2DTBotPlusTimePlus4);
      // fill TProfile
      hProfTimeAsIdPhiMinusSUM -> Add((TProfile*)hfile1->Get("hProfTimeAsIdPhiMinus"), 1);
      hProfTimeAsIdPhiPlusSUM -> Add((TProfile*)hfile1->Get("hProfTimeAsIdPhiPlus"), 1);
      hProfTimeAsIdEtaTopSUM -> Add((TProfile*)hfile1->Get("hProfTimeAsIdEtaTop"),1);
      hProfTimeAsIdEtaBotSUM -> Add((TProfile*)hfile1->Get("hProfTimeAsIdEtaBot"),1);
      //idPhiTower for eta+ and eta-
      hIdPhiTowerMinus -> Add(hIdPhiTowerMinus,hIdPhiTowerHB2DTMinus);
      hIdPhiTowerPlus -> Add(hIdPhiTowerPlus,hIdPhiTowerHB2DTPlus);  
      // number muon Towers
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTTopPlus);
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTTopMinus);
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTBotPlus);
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTBotMinus);
      // 
      hfile2 -> cd();
      hEmuonTopPlus -> Add(hEmuonTopPlus,hEmuonHB2DTTopPlus);
      hEmuonTopMinus -> Add(hEmuonTopMinus,hEmuonHB2DTTopMinus);
      hEmuonBotPlus -> Add(hEmuonBotPlus,hEmuonHB2DTBotPlus);
      hEmuonBotMinus -> Add(hEmuonBotMinus,hEmuonHB2DTBotMinus);
      hTimeMuonTopPlus -> Add(hTimeMuonTopPlus,hTimeMuonHB2DTTopPlus);
      hTimeMuonTopMinus -> Add(hTimeMuonTopMinus,hTimeMuonHB2DTTopMinus);
      hTimeMuonBotPlus -> Add(hTimeMuonBotPlus,hTimeMuonHB2DTBotPlus);
      hTimeMuonBotMinus -> Add(hTimeMuonBotMinus,hTimeMuonHB2DTBotMinus);
      hEmuonBotPlusTimePlus1 -> Add(hEmuonBotPlusTimePlus1,hEmuonHB2DTBotPlusTimePlus1);
      hEmuonBotPlusTimePlus2 -> Add(hEmuonBotPlusTimePlus2,hEmuonHB2DTBotPlusTimePlus2);
      hEmuonBotPlusTimePlus3 -> Add(hEmuonBotPlusTimePlus3,hEmuonHB2DTBotPlusTimePlus3);
      hEmuonBotPlusTimePlus4 -> Add(hEmuonBotPlusTimePlus4,hEmuonHB2DTBotPlusTimePlus4);
      // fill TProfile
      hProfTimeAsIdPhiMinusSUM -> Add((TProfile*)hfile2->Get("hProfTimeAsIdPhiMinus"), 1);
      hProfTimeAsIdPhiPlusSUM -> Add((TProfile*)hfile2->Get("hProfTimeAsIdPhiPlus"), 1);
      hProfTimeAsIdEtaTopSUM -> Add((TProfile*)hfile2->Get("hProfTimeAsIdEtaTop"),1);
      hProfTimeAsIdEtaBotSUM -> Add((TProfile*)hfile2->Get("hProfTimeAsIdEtaBot"),1);
      //idPhiTower for eta+ and eta-
      hIdPhiTowerMinus -> Add(hIdPhiTowerMinus,hIdPhiTowerHB2DTMinus);
      hIdPhiTowerPlus -> Add(hIdPhiTowerPlus,hIdPhiTowerHB2DTPlus);  
      // number muon Towers
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTTopPlus);
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTTopMinus);
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTBotPlus);
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTBotMinus);
      //
      hfile3 -> cd();
      hEmuonTopPlus -> Add(hEmuonTopPlus,hEmuonHB2DTTopPlus);
      hEmuonTopMinus -> Add(hEmuonTopMinus,hEmuonHB2DTTopMinus);
      hEmuonBotPlus -> Add(hEmuonBotPlus,hEmuonHB2DTBotPlus);
      hEmuonBotMinus -> Add(hEmuonBotMinus,hEmuonHB2DTBotMinus);
      hTimeMuonTopPlus -> Add(hTimeMuonTopPlus,hTimeMuonHB2DTTopPlus);
      hTimeMuonTopMinus -> Add(hTimeMuonTopMinus,hTimeMuonHB2DTTopMinus);
      hTimeMuonBotPlus -> Add(hTimeMuonBotPlus,hTimeMuonHB2DTBotPlus);
      hTimeMuonBotMinus -> Add(hTimeMuonBotMinus,hTimeMuonHB2DTBotMinus);
      hEmuonBotPlusTimePlus1 -> Add(hEmuonBotPlusTimePlus1,hEmuonHB2DTBotPlusTimePlus1);
      hEmuonBotPlusTimePlus2 -> Add(hEmuonBotPlusTimePlus2,hEmuonHB2DTBotPlusTimePlus2);
      hEmuonBotPlusTimePlus3 -> Add(hEmuonBotPlusTimePlus3,hEmuonHB2DTBotPlusTimePlus3);
      hEmuonBotPlusTimePlus4 -> Add(hEmuonBotPlusTimePlus4,hEmuonHB2DTBotPlusTimePlus4);
      // fill TProfile
      hProfTimeAsIdPhiMinusSUM -> Add((TProfile*)hfile3->Get("hProfTimeAsIdPhiMinus"), 1);
      hProfTimeAsIdPhiPlusSUM -> Add((TProfile*)hfile3->Get("hProfTimeAsIdPhiPlus"), 1);
      hProfTimeAsIdEtaTopSUM -> Add((TProfile*)hfile3->Get("hProfTimeAsIdEtaTop"),1);
      hProfTimeAsIdEtaBotSUM -> Add((TProfile*)hfile3->Get("hProfTimeAsIdEtaBot"),1);
      //idPhiTower for eta+ and eta-
      hIdPhiTowerMinus -> Add(hIdPhiTowerMinus,hIdPhiTowerHB2DTMinus);
      hIdPhiTowerPlus -> Add(hIdPhiTowerPlus,hIdPhiTowerHB2DTPlus);  
      // number muon Towers
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTTopPlus);
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTTopMinus);
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTBotPlus);
      hNumTowerMuon -> Add(hNumTowerMuon,hNumTowerMuonHB2DTBotMinus);
/////////////////////////////////////////////////
        gStyle->SetOptTitle(0); 
        gStyle->SetOptStat(1);   //  no statistics _or_
   TCanvas *c2 = new TCanvas("c2","E of Muon passes though DT in 2 places, NTowerEta<5");
   c2->Divide(2,2);
   c2->cd(1);
   hEmuonTopMinus.Fit("gaus","E","",0.8.,2.4);
   c2->cd(2);
   hEmuonTopPlus.Fit("gaus","E","",0.8.,2.4);
   c2->cd(3);
   hEmuonBotMinus.Fit("gaus","E","",0.8.,2.4);
   c2->cd(4);
   hEmuonBotPlus.Fit("gaus","E","",0.8.,2.4);
   c2->Print("picAll2.gif");
// wait
   c2.WaitPrimitive();
                                                                                                                                     
                                                                                                                                     
/////////////////////////////////////////////////
   TCanvas *c21 = new TCanvas("c21","Time of Muon passes though DT in 2 places, NTowerEta<5");
   c21->Divide(2,2);
   c21->cd(1);
   hTimeMuonTopMinus->Draw();
   c21->cd(2);
   hTimeMuonTopPlus->Draw();
   c21->cd(3);
   hTimeMuonBotMinus->Draw();
   c21->cd(4);
   hTimeMuonBotPlus->Draw();
   c21->Print("picAll21.gif");
  c21.WaitPrimitive();
/////////////////////////////////////////////////
   TCanvas *c64 = new TCanvas("c64","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c64->Divide(2,2);
   c64->cd(1);
   hEmuonBotPlusTimePlus1->Fit("gaus","E","",0.8.,2.4);
   c64->cd(2);
   hEmuonBotPlusTimePlus2->Fit("gaus","E","",0.8.,2.4);
   c64->cd(3);
   hEmuonBotPlusTimePlus3->Fit("gaus","E","",0.8.,2.4);
   c64->cd(4);
   hEmuonBotPlusTimePlus4->Fit("gaus","E","",0.8.,2.4);
   c64->Print("picAll64.gif");
// wait
  c64.WaitPrimitive();
/////////////////////////////////////////////////
        gStyle->SetOptStat(0);   //  no statistics _or_
        //gStyle->SetGrid();   //  no statistics _or_
////////////////////////////////////////////////
   TCanvas *c8 = new TCanvas("c8","Time vs Idphi");
   c8->Divide(2,2);
   c8->cd(1);
   hProfTimeAsIdPhiMinusSUM->Draw();
   c8->cd(2);
   hProfTimeAsIdPhiPlusSUM->Draw();
   c8->cd(3);
   hProfTimeAsIdEtaTopSUM->Draw();
   c8->cd(4);
   hProfTimeAsIdEtaBotSUM->Draw();
   c8->Print("picAll8.gif");
// wait
  c8.WaitPrimitive();
////////////////////////////////////////////////
        gStyle->SetOptStat(1);   //  no statistics _or_
   TCanvas *c4 = new TCanvas("c4","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c4->Divide(2,2);
   c4->cd(1);
   hIdPhiTowerMinus->Draw();
   c4->cd(2);
   hIdPhiTowerPlus->Draw();
                                                                                                                                                   
   c4->cd(3);
//   hIdPhiTowerHB2DTsevMinus->Draw();
   c4->cd(4);
   hNumTowerMuon -> Draw(); 
//   hIdPhiTowerHB2DTsevPlus->Draw();
   c4->Print("picAll4.gif");
// wait
  c4.WaitPrimitive();
                                                                                                                                                   
/////////////////////////////////////////////////

  
}

