#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TChain.h"
#include "TProfile.h"
#include "math.h"
#include <fstream>
#include <string>
#include <iostream>
#include <TStyle.h>
using namespace std;
int main(int argc,char **argv){


   //======================================================================
      printf("z1: gROOT Reset \n");
       // gROOT->Reset();
       // gROOT->SetStyle("Plain");
        gStyle->SetTitleFont(42);           // title font
        gStyle->SetTitleFontSize(0.07);     // title font size
        gStyle->SetLabelSize(0.05,"xy");     // 
        gStyle->SetStatFontSize(0.05);     //for box size 
        gStyle->SetStatX(0.97);     // for box x position
        gStyle->SetStatW(0.25);     // for box width
        gStyle->SetOptFit(1111); // pcev

// for several root files:
//ifstream fnames("fileList");
ifstream fnames("../fileListV3");
TList* trList=new TList();
const int N=58;
const int n=N; // for 1st N files from fileList  

 TChain muon("prom/Muon/muonHB");
string fname;
char tmp[100];
for (int i=0; i<N; i++)
        {
        fnames>>fname;
        fnames.getline(tmp,100,'\n');
        cout<<fname<<"\n";
        muon.Add(fname.c_str());
        }
//muon.Print(); 
/*
 TChain muon("prom/Muon/muonHB");
 muon.Add("/afs/cern.ch/user/k/kropiv/scratch0/CosmicMuons/CosmicMuons43410_01.root");
 muon.Add("/afs/cern.ch/user/k/kropiv/scratch0/CosmicMuons/CosmicMuons43410_02.root");
 muon.Add("/afs/cern.ch/user/k/kropiv/scratch0/CosmicMuons/CosmicMuons43410_03.root");
*/
// creat root-file with histo
   TH1::AddDirectory(true);  // automatic add histograms (for latest versions of ROOT)
   TFile *theFile    =new TFile("Golden.root", "RECREATE");
   theFile->cd();

/////////////////////////////////////////////////////////
//Declaration of leaves types
// ntuple creation:
//     TFile*      hOutputFile ;
     Int_t run, event, NumMuonHBphiPlane, NumHBTowersMuon[50];
     Int_t IdTowerPhiMuonIn[50], IdTowerPhiMuonOut[50], IdTowerEtaMuonIn[50], IdTowerEtaMuonOut[50];
     Float_t TimeAvMuonHB[50];
     Float_t PHIoutMuonHB[50], PHIinMuonHB[50],  ETAoutMuonHB[50], ETAinMuonHB[50];
     Float_t ImpXYmuonHB[50], ZImpXYmuonHB[50];
//     Float_t ImpXYmuon[50], ZImpXYmuon[50],
     Float_t PHIinTowerHB[50];
     Float_t EmuonHB[50];
//
     Float_t XinPosMuonDT[50], YinPosMuonDT[50], ZinPosMuonDT[50];
     Float_t XoutPosMuonDT[50], YoutPosMuonDT[50], ZoutPosMuonDT[50];
     Float_t LengthMuonHB[50], LengthMuonDT[50];
     Int_t NumHitsMuonDT[50], NumHitsMuonDTall[50];
//
     //int  NumHBTowersMuon2[50];
     Int_t IdTowerPhiMuonIn2[50], IdTowerPhiMuonOut2[50], IdTowerEtaMuonIn2[50], IdTowerEtaMuonOut2[50];
     Float_t TimeAvMuonHB2[50],  TimeAvMuonHBcut2[50], TimeAvMuonHBcutwide2[50], LengthMuonHB2[50];
     Float_t EmuonHB2[50], EmuonHBcut2[50], EmuonHBcutwide2[50];
// make initialisation of Muons that not belong to the same Phi Plane in HB
     Int_t NumMuonHBnoPhiPlane;
     Int_t IdTowerMuonNoPlanePhiIn[50], IdTowerMuonNoPlanePhiOut[50];
     Int_t IdTowerMuonNoPlaneEtaIn[50], IdTowerMuonNoPlaneEtaOut[50];
     Float_t XinPosMuonNoPlaneDT[50], YinPosMuonNoPlaneDT[50], ZinPosMuonNoPlaneDT[50];
     Float_t XoutPosMuonNoPlaneDT[50], YoutPosMuonNoPlaneDT[50], ZoutPosMuonNoPlaneDT[50];
     Float_t ImpXYmuonNoPlaneDT[50], ZImpXYmuonNoPlaneDT[50], LengthMuonNoPlaneDT[50];
     Int_t NumHitsMuonNoPlaneDTall[50];
// end ntuple creation

   // Set branch addresses.
   muon.SetBranchAddress("run",&run);
   muon.SetBranchAddress("event",&event);
   muon.SetBranchAddress("NumMuonHBphiPlane",&NumMuonHBphiPlane);
   muon.SetBranchAddress("NumHBTowersMuon",NumHBTowersMuon);
   muon.SetBranchAddress("TimeAvMuonHB",TimeAvMuonHB);
   muon.SetBranchAddress("PHIoutMuonHB",PHIoutMuonHB);
   muon.SetBranchAddress("ETAoutMuonHB",ETAoutMuonHB);
   muon.SetBranchAddress("PHIinMuonHB",PHIinMuonHB);
   muon.SetBranchAddress("ETAinMuonHB",ETAinMuonHB);
   muon.SetBranchAddress("IdTowerPhiMuonIn",IdTowerPhiMuonIn);
   muon.SetBranchAddress("IdTowerPhiMuonOut",IdTowerPhiMuonOut);
   muon.SetBranchAddress("IdTowerEtaMuonIn",IdTowerEtaMuonIn);
   muon.SetBranchAddress("IdTowerEtaMuonOut",IdTowerEtaMuonOut);
   muon.SetBranchAddress("LengthMuonHB",LengthMuonHB);
   muon.SetBranchAddress("ImpXYmuonHB",ImpXYmuonHB);
   muon.SetBranchAddress("ZImpXYmuonHB",ZImpXYmuonHB);
   muon.SetBranchAddress("EmuonHB",EmuonHB);
   muon.SetBranchAddress("PHIinTowerHB",PHIinTowerHB);
   muon.SetBranchAddress("IdTowerPhiMuonIn2",IdTowerPhiMuonIn2);
   muon.SetBranchAddress("IdTowerPhiMuonOut2",IdTowerPhiMuonOut2);
   muon.SetBranchAddress("IdTowerEtaMuonIn2",IdTowerEtaMuonIn2);
   muon.SetBranchAddress("IdTowerEtaMuonOut2",IdTowerEtaMuonOut2);
   muon.SetBranchAddress("LengthMuonHB2",LengthMuonHB2);
   muon.SetBranchAddress("TimeAvMuonHB2",TimeAvMuonHB2);
   muon.SetBranchAddress("TimeAvMuonHBcut2",TimeAvMuonHBcut2);
   muon.SetBranchAddress("TimeAvMuonHBcutwide2",TimeAvMuonHBcutwide2);
   muon.SetBranchAddress("EmuonHB2",EmuonHB2);
   muon.SetBranchAddress("EmuonHBcut2",EmuonHBcut2);
   muon.SetBranchAddress("EmuonHBcutwide2",EmuonHBcutwide2);
   muon.SetBranchAddress("LengthMuonDT",LengthMuonDT);
   muon.SetBranchAddress("NumHitsMuonDTall",NumHitsMuonDTall);
   muon.SetBranchAddress("NumHitsMuonDT",NumHitsMuonDT);
   muon.SetBranchAddress("XinPosMuonDT",XinPosMuonDT);
   muon.SetBranchAddress("YinPosMuonDT",YinPosMuonDT);
   muon.SetBranchAddress("ZinPosMuonDT",ZinPosMuonDT);
   muon.SetBranchAddress("XoutPosMuonDT",XoutPosMuonDT);
   muon.SetBranchAddress("YoutPosMuonDT",YoutPosMuonDT);
   muon.SetBranchAddress("ZoutPosMuonDT",ZoutPosMuonDT);
   //
   muon.SetBranchAddress("NumMuonHBnoPhiPlane",&NumMuonHBnoPhiPlane);
   muon.SetBranchAddress("IdTowerMuonNoPlanePhiIn",IdTowerMuonNoPlanePhiIn);
   muon.SetBranchAddress("IdTowerMuonNoPlanePhiOut",IdTowerMuonNoPlanePhiOut);
   muon.SetBranchAddress("IdTowerMuonNoPlaneEtaIn",IdTowerMuonNoPlaneEtaIn);
   muon.SetBranchAddress("IdTowerMuonNoPlaneEtaOut",IdTowerMuonNoPlaneEtaOut);
   muon.SetBranchAddress("XinPosMuonNoPlaneDT",XinPosMuonNoPlaneDT);
   muon.SetBranchAddress("YinPosMuonNoPlaneDT",YinPosMuonNoPlaneDT);
   muon.SetBranchAddress("ZinPosMuonNoPlaneDT",ZinPosMuonNoPlaneDT);
   muon.SetBranchAddress("XoutPosMuonNoPlaneDT",XoutPosMuonNoPlaneDT);
   muon.SetBranchAddress("YoutPosMuonNoPlaneDT",YoutPosMuonNoPlaneDT);
   muon.SetBranchAddress("ZoutPosMuonNoPlaneDT",ZoutPosMuonNoPlaneDT);
   muon.SetBranchAddress("ImpXYmuonNoPlaneDT",ImpXYmuonNoPlaneDT);
   muon.SetBranchAddress("ZImpXYmuonNoPlaneDT",ZImpXYmuonNoPlaneDT);
   muon.SetBranchAddress("LengthMuonNoPlaneDT",LengthMuonNoPlaneDT);
   muon.SetBranchAddress("NumHitsMuonNoPlaneDTall",NumHitsMuonNoPlaneDTall);

// Create histo
//***************************************************************************************************
// 2nd (Partner) Muon is good: in one Phi Plane in HB
   //matching between 2 muons
   TH1F *hAngleMuonHB2DT = new TH1F("hAngleMuonHB2DT","Cos Angle between two muons, 2nd muon in one phi plane ", 100, 0.95, 1.02);
   TH1F *hDeltaImpXYHB2DT = new TH1F("hDeltaImpXYHB2DT","delta ImpXY between two muons, 2nd muon in one phi plane ", 30, 0., 30.);
   TH1F *hDeltaZImpXYHB2DT = new TH1F("hDeltaZImpXYHB2DT","delta ZImpXY between two muons, 2nd muon in one phi plane ", 50, 0., 100.);
   TH1F *hPhiDeltaTowerHB2DT = new TH1F("hPhiDeltaTowerHB2DT","diviation from PhiTower center, 2nd muon in one phi plane ", 50, 0., 1.);
   //Energy
   TH1F *hEmuonHB2DTTopPlus = new TH1F("hEmuonHB2DTTopPlus","Emoun, Top+, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTTopMinus = new TH1F("hEmuonHB2DTTopMinus","Emoun, Top-, 2nd muon in one phi plane", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotPlus = new TH1F("hEmuonHB2DTBotPlus","Emoun, Bottom+, 2nd muon in one phi plane", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotMinus = new TH1F("hEmuonHB2DTBotMinus","Emoun, Bottom-, 2nd muon in one phi plane", 60, -2., 10.);
   //Time
   TH1F *hTimeMuonHB2DTTopPlus = new TH1F("hTimeMuonHB2DTTopPlus","TimeMoun, Top+, 2nd muon in one phi plane ", 60, -150., 150.);
   TH1F *hTimeMuonHB2DTTopMinus = new TH1F("hTimeMuonHB2DTTopMinus","TimeMoun, Top-, 2nd muon in one phi plane ", 60, -150., 150.);
   TH1F *hTimeMuonHB2DTBotPlus = new TH1F("hTimeMuonHB2DTBotPlus","TimeMoun, Bottom+, 2nd muon in one phi plane ", 60, -150., 150.);
   TH1F *hTimeMuonHB2DTBotMinus = new TH1F("hTimeMuonHB2DTBotMinus","TimeMoun, Bottom-, 2nd muon in one phi plane ", 60, -150., 150.);
   //Number Towers in Eta Plane passing by good muon
   TH1F *hNumTowerMuonHB2DTTopPlus = 
        new TH1F("hNumTowerMuonHB2DTTopPlus","Number Towers of Moun, Top+, 2nd muon in one phi plane ", 11, -0.5, 10.5);
   TH1F *hNumTowerMuonHB2DTTopMinus = 
        new TH1F("hNumTowerMuonHB2DTTopMinus","Number Towers of Moun, Top+, 2nd muon in one phi plane ", 11, -0.5, 10.5);
   TH1F *hNumTowerMuonHB2DTBotPlus = 
        new TH1F("hNumTowerMuonHB2DTBotPlus","Number Towers of Moun, Bottom+, 2nd muon in one phi plane ", 11, -0.5, 10.5);
   TH1F *hNumTowerMuonHB2DTBotMinus = 
        new TH1F("hNumTowerMuonHB2DTBotMinus","Number Towers of Moun, Bottom+, 2nd muon in one phi plane ", 11, -0.5, 10.5);
   //idPhiTower for eta+ and eta-
   TH1F *hIdPhiTowerHB2DTMinus = new TH1F("hIdPhiTowerHB2DTMinus", "IdPhiTower for eta-, 2nd muon in one phi plane ", 74, -0.5, 73.5);
   TH1F *hIdPhiTowerHB2DTPlus = new TH1F("hIdPhiTowerHB2DTPlus", "IdPhiTower for eta+, 2nd muon in one phi plane ", 74, -0.5, 73.5);
   // Muon Track quality in DT
   TH1F *hNumHitsHB2DT = new TH1F("hNumHitsHB2DT", "Number Hits for analysing muon, 2nd muon in one phi plane ",61 , -0.5, 60.5);
   TH1F *hNumHitsHB2DT2 = new TH1F("hNumHitsHB2DT2", "Number Hits for partner of analysing muon, 2nd muon in one phi plane ",61 , -0.5, 60.5);
   TH1F *hLengthMuonDTHB2DT = new TH1F("hLengthMuonDTHB2DT", "Length of analysing muon in DT, 2nd muon in one phi plane ",100 , 0, 600);
   TH1F *hLengthMuonDTHB2DT2 = new TH1F("hLengthMuonDTHB2DT2", "Length of partner of analysing muon in DT, 2nd muon in one phi plane ",100 , 0, 600);
   // energy for time- and time+
   TH1F *hEmuonHB2DTTopPlusTimePlus = new TH1F("hEmuonHB2DTTopPlusTimePlus","Emoun, Top+, Time+, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTTopMinusTimePlus = new TH1F("hEmuonHB2DTTopMinusTimePlus","Emoun, Top-, Time+, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotPlusTimePlus = new TH1F("hEmuonHB2DTBotPlusTimePlus","Emoun, Bot+, Time+, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotMinusTimePlus = new TH1F("hEmuonHB2DTBotMinusTimePlus","Emoun, Bot-, Time+, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTTopPlusTimeMinus = new TH1F("hEmuonHB2DTTopPlusTimeMinus","Emoun, Top+, Time-, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTTopMinusTimeMinus = new TH1F("hEmuonHB2DTTopMinusTimeMinus","Emoun, Top-, Time-, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotPlusTimeMinus = new TH1F("hEmuonHB2DTBotPlusTimeMinus","Emoun, Bot+, Time-, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotMinusTimeMinus = new TH1F("hEmuonHB2DTBotMinusTimeMinus","Emoun, Bot-, Time-, 2nd muon in one phi plane ", 60, -2., 10.);
   // in different time:
   TH1F *hEmuonHB2DTBotPlusTimePlus1 = new TH1F("hEmuonHB2DTBotPlusTimePlus1","Emoun, Bot+, Time+1, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotPlusTimePlus2 = new TH1F("hEmuonHB2DTBotPlusTimePlus2","Emoun, Bot+, Time+2, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotPlusTimePlus3 = new TH1F("hEmuonHB2DTBotPlusTimePlus3","Emoun, Bot+, Time+3, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotPlusTimePlus4 = new TH1F("hEmuonHB2DTBotPlusTimePlus4","Emoun, Bot+, Time+4, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotPlusTimeMinus1 = new TH1F("hEmuonHB2DTBotPlusTimeMinus1","Emoun, Bot+, Time-1, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotPlusTimeMinus2 = new TH1F("hEmuonHB2DTBotPlusTimeMinus2","Emoun, Bot+, Time-2, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotPlusTimeMinus3 = new TH1F("hEmuonHB2DTBotPlusTimeMinus3","Emoun, Bot+, Time-3, 2nd muon in one phi plane ", 60, -2., 10.);
   TH1F *hEmuonHB2DTBotPlusTimeMinus4 = new TH1F("hEmuonHB2DTBotPlusTimeMinus4","Emoun, Bot+, Time-4, 2nd muon in one phi plane ", 60, -2., 10.);
   // IdPhi vs Emuon
   TH2F *hIdPhiPlusVsE = new TH2F("hIdPhiPlusVsE","IdPhiTower vs Emuon, HB+",  60, -2, 10, 74, -0.5, 73.5); // x, y coordiants
   TH2F *hIdPhiMinusVsE = new TH2F("hIdPhiMinusVsE","IdPhiTower vs Emuon, HB-",  60, -2, 10, 74, -0.5, 73.5); // x, y coordiants
   // IdEta vs Emuon
   TH2F *hIdEtaTopVsE = new TH2F("hIdEtaTopVsE","IdEtaTower vs Emuon, Top HB",  60, -2, 10, 57, -14.25, 14.25); // x, y coordiants
   TH2F *hIdEtaBotVsE = new TH2F("hIdEtaBotVsE","IdEtaTower vs Emuon, Bottom HB",  60, -2, 10, 57, -14.25, 14.25); // x, y coordiants
  
   // contral plots
   TH1F *hImpXYHB2DT = new TH1F("hImpXYHB2DT","ImpXY of analyzing muon, 2nd muon in one phi plane ", 50, 0., 50.);
   TH1F *hZImpXYHB2DT = new TH1F("hZImpXYHB2DT","ZImpXY of analyzing muon, 2nd muon in one phi plane ", 100, -500., 500.);
   TH2F *hLmuonDTImpXY = new TH2F("hLmuonDTImpXY","ImpXT vs Muon Length", 100, 0, 600, 40, 0, 40); // x, y coordiants
   TH2F *hImpXYvsZ = new TH2F("hImpXYvsZ","ImpXY vs ZImpXY for muon", 40, 0, 40, 100, -500, 500); // x, y coordiants

   //energy of selected muon in different part of phi Towers
   TH1F *hEmuonPhiDetaTower1 = new TH1F("hEmuonPhiDetaTower1","Emoun in 1st:0-0.2 part of phi Towers ", 60, -2., 10.);
   TH1F *hEmuonPhiDetaTower2 = new TH1F("hEmuonPhiDetaTower2","Emoun in 2nd:0.2-0.4 part of phi Towers ", 60, -2., 10.);
   TH1F *hEmuonPhiDetaTower3 = new TH1F("hEmuonPhiDetaTower3","Emoun in 3th:0.4-0.6 part of phi Towers ", 60, -2., 10.);
   TH1F *hEmuonPhiDetaTower4 = new TH1F("hEmuonPhiDetaTower4","Emoun in 4th:0.6-0.8 part of phi Towers ", 60, -2., 10.);
   TH1F *hEmuonPhiDetaTower5 = new TH1F("hEmuonPhiDetaTower5","Emoun in 5th:0.8-0.9 part of phi Towers ", 60, -2., 10.);
   TH1F *hEmuonPhiDetaTower6 = new TH1F("hEmuonPhiDetaTower6","Emoun in 6th:0.9-1. part of phi Towers ", 60, -2., 10.);

   // fill TProfile for Time as iphi
   TProfile *hProfTimeAsIdPhiMinus = new TProfile("hProfTimeAsIdPhiMinus","mean Time as IdPhi for ETA-",72,0.5,72.5,-150,150);
   TProfile *hProfTimeAsIdPhiPlus = new TProfile("hProfTimeAsIdPhiPlus","mean Time as IdPhi for ETA+",72,0.5,72.5,-150,150);
   TProfile *hProfTimeAsIdEtaTop = new TProfile("hProfTimeAsIdEtaTop","mean Time as IdEta for Top",57,-14.25,14.25,-150,150);
   TProfile *hProfTimeAsIdEtaBot = new TProfile("hProfTimeAsIdEtaBot","mean Time as IdEta for Bot",57,-14.25,14.25,-150,150);
  
//***************************************************************************************************

//   Start main loop on all events
//   Event *eveall = new Event(); 
   Int_t nevent = muon.GetEntries();
// calculate pi:
   //float piG=acos(-1.);
   float DeltaPhiTower = 0.08726646; // width of PhiTower 
   for (Int_t i=0;i<nevent;i++) {
       muon.GetEntry(i);
// calculate anlge between good moun in one PhiPlane and other muons (could be also good or bad) 

        if(NumMuonHBphiPlane>0){ 
            for(Int_t ik = 0; ik<NumMuonHBphiPlane;ik++){
               int IsecondMuonDT = 0; // check if pair in DT to moun exists
               //int IsecondMuonHB = 0; // check if pair in HB to moun exists
               int maxTowerEta = 40; // maximal number of Towers crossing in eta 
               int minTowerEta = 0; // minimal number of Towers crossing in eta 
               // select muoun for wich we looking partner with |in and out IdTower|<=14, 
               // for partner it dosn't importent because we are not going to calculate it energy
               if(fabs(IdTowerEtaMuonIn[ik])>14 || fabs(IdTowerEtaMuonOut[ik])>14)continue;
               // check quality of muon
               if(NumHitsMuonDTall[ik]<25)continue; 
               // calculate angle between good muons
               for(Int_t jk = 0; jk<NumMuonHBphiPlane;jk++){
                  // check that this is not the same muon
                  if(ik==jk) continue;
                  // check quality of muon
                  //if(NumHitsMuonDTall[jk]<25)continue; 
                  //check that both muons from different place of DT R(inDT1,inDT2)>600 cm
                  float R12;
                  R12=pow((XinPosMuonDT[ik]-XinPosMuonDT[jk]),2)+pow((YinPosMuonDT[ik]-YinPosMuonDT[jk]),2);
                  R12=sqrt(R12+pow((ZinPosMuonDT[ik]-ZinPosMuonDT[jk]),2));
                  if(R12<600)continue;
                  // calculate cos angle
                  float CosAngle;
                  float r1;
                  float r2;
                  float deltaIm; 
                  float deltaZIm; 
                  r1 = pow((XinPosMuonDT[ik]-XoutPosMuonDT[ik]),2)+pow((YinPosMuonDT[ik]-YoutPosMuonDT[ik]),2);
                  r1 = sqrt(r1+pow((ZinPosMuonDT[ik]-ZoutPosMuonDT[ik]),2));
                  r2 = pow((XinPosMuonDT[jk]-XoutPosMuonDT[jk]),2)+pow((YinPosMuonDT[jk]-YoutPosMuonDT[jk]),2);
                  r2 = sqrt(r2+pow((ZinPosMuonDT[jk]-ZoutPosMuonDT[jk]),2));
                  CosAngle = -0.1;
                  if(r1*r2>0){
                      CosAngle = (XinPosMuonDT[ik]-XoutPosMuonDT[ik])*(XinPosMuonDT[jk]-XoutPosMuonDT[jk]);
                      CosAngle = CosAngle+(YinPosMuonDT[ik]-YoutPosMuonDT[ik])*(YinPosMuonDT[jk]-YoutPosMuonDT[jk]);
                      CosAngle = CosAngle+(ZinPosMuonDT[ik]-ZoutPosMuonDT[ik])*(ZinPosMuonDT[jk]-ZoutPosMuonDT[jk]);
                      CosAngle = CosAngle/r1/r2;
                      deltaIm = fabs(ImpXYmuonHB[ik]-ImpXYmuonHB[jk]);
                      deltaZIm = fabs(ZImpXYmuonHB[ik]-ZImpXYmuonHB[jk]);
                      hAngleMuonHB2DT->Fill(CosAngle);
                      hDeltaImpXYHB2DT->Fill(deltaIm); 
                      hDeltaZImpXYHB2DT->Fill(deltaZIm);
                      // make cut that 2nd muon in DT exists, do not make doulbe counting for histo filling
                      if(CosAngle>0.99&&deltaIm<10&&deltaZIm<20&&IsecondMuonDT==0){
                           IsecondMuonDT=1;
                           //calculate quality of Phi crossing:
                           float phiR;
                           phiR = fabs((PHIinMuonHB[ik]+PHIoutMuonHB[ik])/2-PHIinTowerHB[ik])/(DeltaPhiTower/2);
                           hPhiDeltaTowerHB2DT -> Fill(phiR);
                           if(phiR>0.8)continue;
                           float dLHB= 287.65-177.7;
               
                           // Top: 1<=iphi<=36, Bottom: 37<=iphi<=72 
                           // +: ieta>0, -: ieta<0
                           // number crossing towers in eta < 5 for quolity energy  
                           float eHB;
                           eHB = EmuonHB[ik]/LengthMuonHB[ik]*dLHB;
                           float tmuon;
                           tmuon = TimeAvMuonHB[ik]; 
                           float meanB=0; // difined mean time for top and bot
                           float rmsB=0; // difined mean time for top and but
                           // select that analized muon has good Time (+-1RMS) and belong to one part: Top+,Top-,Bot+ or Bot-  
                           if(IdTowerPhiMuonIn[ik]>0&&IdTowerPhiMuonIn[ik]<37&&NumHBTowersMuon[ik]<=maxTowerEta&&NumHBTowersMuon[ik]>=minTowerEta){ 
                             // cout << " *** check *** " << std::endl;
                              if(IdTowerEtaMuonIn[ik]>0&&IdTowerEtaMuonOut[ik]>0){
                                 //meanB=24.04;
                                 //rmsB=23.02;
                                 meanB=23.77;
                                 rmsB=22.74;
                                 if(fabs(tmuon-meanB)<=rmsB){
                                    hEmuonHB2DTTopPlus ->Fill(eHB);
                                    hNumTowerMuonHB2DTTopPlus->Fill(NumHBTowersMuon[ik]);
                                 }
                                 hTimeMuonHB2DTTopPlus ->Fill(tmuon); 
                                 if((tmuon-meanB)<0&&(tmuon-meanB)>(-rmsB))hEmuonHB2DTTopPlusTimeMinus->Fill(eHB); //cut on the tail one RMS
                                 if((tmuon-meanB)>=0&&(tmuon-meanB)<rmsB)hEmuonHB2DTTopPlusTimePlus->Fill(eHB);
                              } 
                              if(IdTowerEtaMuonIn[ik]<0&&IdTowerEtaMuonOut[ik]<0){
                                 //meanB=22.01;
                                 //rmsB=22.99;
                                 meanB=22.02;
                                 rmsB=22.8;
                                 if(fabs(tmuon-meanB)<=rmsB){
                                    hEmuonHB2DTTopMinus ->Fill(eHB);
                                    hNumTowerMuonHB2DTTopMinus->Fill(NumHBTowersMuon[ik]); 
                                 } 
                                 hTimeMuonHB2DTTopMinus ->Fill(tmuon);
                                 if((tmuon-meanB)<0&&(tmuon-meanB)>(-rmsB))hEmuonHB2DTTopMinusTimeMinus->Fill(eHB);
                                 if((tmuon-meanB)>=0&&(tmuon-meanB)<rmsB)hEmuonHB2DTTopMinusTimePlus->Fill(eHB);
                              } 
                           }
                           if(IdTowerPhiMuonIn[ik]>36&&IdTowerPhiMuonIn[ik]<73&&NumHBTowersMuon[ik]<=maxTowerEta&&NumHBTowersMuon[ik]>=minTowerEta){
                              if(IdTowerEtaMuonIn[ik]>0&&IdTowerEtaMuonOut[ik]>0){
                                 //meanB=37.99;
                                 //rmsB=23.7;
                                 meanB=39.56;
                                 rmsB=24.02;
                                 if(fabs(tmuon-meanB)<=rmsB){
                                    hEmuonHB2DTBotPlus ->Fill(eHB);
                                    hNumTowerMuonHB2DTBotPlus->Fill(NumHBTowersMuon[ik]); 
                                 } 
                                 hTimeMuonHB2DTBotPlus ->Fill(tmuon);
                                 if((tmuon-meanB)<0&&(tmuon-meanB)>(-rmsB))hEmuonHB2DTBotPlusTimeMinus->Fill(eHB);
                                 if((tmuon-meanB)>=0&&(tmuon-meanB)<rmsB)hEmuonHB2DTBotPlusTimePlus->Fill(eHB);
                                 // fill Emuon in different time:
                                 if((tmuon-meanB)>=0&&(tmuon-meanB)<rmsB/2)hEmuonHB2DTBotPlusTimePlus1->Fill(eHB);
                                 if((tmuon-meanB)>=rmsB/2&&(tmuon-meanB)<rmsB)hEmuonHB2DTBotPlusTimePlus2->Fill(eHB);
                                 if((tmuon-meanB)>=rmsB&&(tmuon-meanB)<rmsB*1.5)hEmuonHB2DTBotPlusTimePlus3->Fill(eHB);
                                 if((tmuon-meanB)>=rmsB*1.5)hEmuonHB2DTBotPlusTimePlus4->Fill(eHB);
                                 if((tmuon-meanB)<0&&(tmuon-meanB)>=(-rmsB/2))hEmuonHB2DTBotPlusTimeMinus1->Fill(eHB);
                                 if((tmuon-meanB)<(-rmsB/2)&&(tmuon-meanB)>=(-rmsB))hEmuonHB2DTBotPlusTimeMinus2->Fill(eHB);
                                 if((tmuon-meanB)<(-rmsB)&&(tmuon-meanB)>=(-1.5*rmsB))hEmuonHB2DTBotPlusTimeMinus3->Fill(eHB);
                                 if((tmuon-meanB)<(-rmsB*1.5))hEmuonHB2DTBotPlusTimeMinus4->Fill(eHB);
                              }
                              if(IdTowerEtaMuonIn[ik]<0&&IdTowerEtaMuonOut[ik]<0){
                                 //meanB=33.51;
                                 //rmsB=27.35;
                                 meanB=34.84;
                                 rmsB=27.31;
                                 if(fabs(tmuon-meanB)<=rmsB){
                                    hEmuonHB2DTBotMinus ->Fill(eHB);
                                    hNumTowerMuonHB2DTBotMinus->Fill(NumHBTowersMuon[ik]); 
                                 } 
                                 hTimeMuonHB2DTBotMinus ->Fill(tmuon);
                                 if((tmuon-meanB)<0&&(tmuon-meanB)>(-rmsB))hEmuonHB2DTBotMinusTimeMinus->Fill(eHB);
                                 if((tmuon-meanB)>=0&&(tmuon-meanB)<rmsB)hEmuonHB2DTBotMinusTimePlus->Fill(eHB);
                              }
                           }
                          // if(IdTowerEtaMuonIn[ik]<0&&IdTowerEtaMuonOut[ik]<0){
                          //       hProfTimeAsIdPhiMinus->Fill(IdTowerPhiMuonIn[ik],tmuon);
                          // }  
                          // if(IdTowerEtaMuonIn[ik]>0&&IdTowerEtaMuonOut[ik]>0){
                          //       hProfTimeAsIdPhiPlus->Fill(IdTowerPhiMuonIn[ik],tmuon);
                          // }  
                           if(IdTowerPhiMuonIn[ik]>=8&&IdTowerPhiMuonIn[ik]<=28&&IdTowerEtaMuonIn[ik]>0&&IdTowerEtaMuonOut[ik]>0){
                              hProfTimeAsIdPhiPlus->Fill(IdTowerPhiMuonIn[ik],tmuon);
                           }
                           if(IdTowerPhiMuonIn[ik]>=8&&IdTowerPhiMuonIn[ik]<=27&&IdTowerEtaMuonIn[ik]<0&&IdTowerEtaMuonOut[ik]<0){
                              hProfTimeAsIdPhiMinus->Fill(IdTowerPhiMuonIn[ik],tmuon);
                           }
                           if(IdTowerPhiMuonIn[ik]>=43&&IdTowerPhiMuonIn[ik]<=65&&IdTowerEtaMuonIn[ik]>0&&IdTowerEtaMuonOut[ik]>0){
                              hProfTimeAsIdPhiPlus->Fill(IdTowerPhiMuonIn[ik],tmuon);
                           }
                           if(IdTowerPhiMuonIn[ik]>=47&&IdTowerPhiMuonIn[ik]<=64&&IdTowerEtaMuonIn[ik]<0&&IdTowerEtaMuonOut[ik]<0){
                              hProfTimeAsIdPhiMinus->Fill(IdTowerPhiMuonIn[ik],tmuon);
                           }
                           if(abs(IdTowerEtaMuonIn[ik]-IdTowerEtaMuonOut[ik])<=3){
                              float ideta = float(IdTowerEtaMuonIn[ik]+IdTowerEtaMuonOut[ik])/2;
                              if(IdTowerPhiMuonIn[ik]>=8&&IdTowerPhiMuonIn[ik]<=28&&IdTowerEtaMuonIn[ik]>0){
                                    hProfTimeAsIdEtaTop->Fill(ideta,tmuon);
                              }
                              if(IdTowerPhiMuonIn[ik]>=8&&IdTowerPhiMuonIn[ik]<=27&&IdTowerEtaMuonIn[ik]<0&&IdTowerPhiMuonIn[ik]!=24){
                                    hProfTimeAsIdEtaTop->Fill(ideta,tmuon);
                              }
                              if(IdTowerPhiMuonIn[ik]>=43&&IdTowerPhiMuonIn[ik]<=65&&IdTowerEtaMuonIn[ik]>0){
                                    hProfTimeAsIdEtaBot->Fill(ideta,tmuon);
                              }
                              if(IdTowerPhiMuonIn[ik]>=47&&IdTowerPhiMuonIn[ik]<=64&&IdTowerEtaMuonIn[ik]<0){
                                    hProfTimeAsIdEtaBot->Fill(ideta,tmuon);
                              }
                           }

// this cut is also means that ower muon belong only to one from 4 HB Parts
                           if(fabs(tmuon-meanB)<=rmsB&&meanB!=0){
                              // fill energy in different id Phi
                              if(IdTowerEtaMuonIn[ik]<0&&IdTowerEtaMuonOut[ik]<0){
                                    hIdPhiTowerHB2DTMinus->Fill(float(IdTowerPhiMuonIn[ik]));
                                    hIdPhiMinusVsE->Fill(eHB,float(IdTowerPhiMuonIn[ik]));
                              }  
                              if(IdTowerEtaMuonIn[ik]>0&&IdTowerEtaMuonOut[ik]>0){
                                    hIdPhiTowerHB2DTPlus->Fill(float(IdTowerPhiMuonIn[ik]));
                                    hIdPhiPlusVsE->Fill(eHB,float(IdTowerPhiMuonIn[ik]));
                              }  
                              // fill energy in different id Eta Tower, max Eta Tower crossing by muon <= 4 
                              if(abs(IdTowerEtaMuonIn[ik]-IdTowerEtaMuonOut[ik])<=3){
                                 float ideta = float(IdTowerEtaMuonIn[ik]+IdTowerEtaMuonOut[ik])/2;
                                 if(IdTowerPhiMuonIn[ik]>=8&&IdTowerPhiMuonIn[ik]<=28&&IdTowerEtaMuonIn[ik]>0){
                                       hIdEtaTopVsE->Fill(eHB,ideta);
                                       //hProfTimeAsIdEtaTop->Fill(ideta,tmuon);
                                 }  
                                 if(IdTowerPhiMuonIn[ik]>=8&&IdTowerPhiMuonIn[ik]<=27&&IdTowerEtaMuonIn[ik]<0&&IdTowerPhiMuonIn[ik]!=24){
                                       hIdEtaTopVsE->Fill(eHB,ideta);
                                       //hProfTimeAsIdEtaTop->Fill(ideta,tmuon);
                                 }  
                                 if(IdTowerPhiMuonIn[ik]>=43&&IdTowerPhiMuonIn[ik]<=65&&IdTowerEtaMuonIn[ik]>0){
                                       hIdEtaBotVsE->Fill(eHB,ideta);
                                       //hProfTimeAsIdEtaBot->Fill(ideta,tmuon);
                                 }  
                                 if(IdTowerPhiMuonIn[ik]>=47&&IdTowerPhiMuonIn[ik]<=64&&IdTowerEtaMuonIn[ik]<0){
                                       hIdEtaBotVsE->Fill(eHB,ideta);
                                       //hProfTimeAsIdEtaBot->Fill(ideta,tmuon);
                                 }  
                              }  

                              // fill energy of muon in different phiR
                              if(phiR<=0.2)hEmuonPhiDetaTower1->Fill(eHB);
                              if(phiR>0.2&&phiR<=0.4)hEmuonPhiDetaTower2->Fill(eHB);
                              if(phiR>0.4&&phiR<=0.6)hEmuonPhiDetaTower3->Fill(eHB);
                              if(phiR>0.6&&phiR<=0.8)hEmuonPhiDetaTower4->Fill(eHB);
                              if(phiR>0.8&&phiR<=0.9)hEmuonPhiDetaTower5->Fill(eHB);
                              if(phiR>0.9&&phiR<=1.0)hEmuonPhiDetaTower6->Fill(eHB);
                              hNumHitsHB2DT->Fill(NumHitsMuonDTall[ik]);
                              hLengthMuonDTHB2DT->Fill(LengthMuonDT[ik]);  
                              hNumHitsHB2DT2->Fill(NumHitsMuonDTall[jk]);
                              hLengthMuonDTHB2DT2->Fill(LengthMuonDT[jk]); 
                              float XY;
                              XY = ImpXYmuonHB[ik];  
                              hImpXYHB2DT -> Fill(XY); 
                              XY = ZImpXYmuonHB[ik];  
                              hZImpXYHB2DT -> Fill(XY); 
                              hLmuonDTImpXY->Fill(LengthMuonDT[ik],ImpXYmuonHB[ik]);
                              hImpXYvsZ->Fill(ImpXYmuonHB[ik],ZImpXYmuonHB[ik]);
                           }
                      } // end check for muon in 2 DT exists
                  }
               } //end for for 2nd good muon
            } //end for for interested muon 
        } // end check if good moun exists
   } //end event loop
/////////////////////////////////////////////////
// write histo to file
   theFile->Write();
   theFile->Close();
/////////////////////////////////////////////////
/*
   TCanvas *c1 = new TCanvas("c1","Check that Muon passes though DT in 2 places");
   c1->Divide(2,2);
   c1->cd(1);
   gPad -> SetLogy();
   //     TAxis *axis = histo->GetXaxis();
   //     axis->SetLabelSize(0.06);   
   hAngleMuonHB2DT ->Draw();
   c1->cd(2);
   hDeltaImpXYHB2DT->Draw();
   c1->cd(3);
   hDeltaZImpXYHB2DT->Draw();
   c1->cd(4);
   hPhiDeltaTowerHB2DT->Draw();
   c1->Print("pic1.gif");
// wait
   c1->WaitPrimitive();
/////////////////////////////////////////////////
   TCanvas *c2 = new TCanvas("c2","E of Muon passes though DT in 2 places, NTowerEta<5");
   c2->Divide(2,2);
   c2->cd(1);
   hEmuonHB2DTTopMinus.Fit("gaus","E","",0.8.,2.4);
   c2->cd(2);
   hEmuonHB2DTTopPlus.Fit("gaus","E","",0.8.,2.4);
   c2->cd(3);
   hEmuonHB2DTBotMinus.Fit("gaus","E","",0.8.,2.4);
   c2->cd(4);
   hEmuonHB2DTBotPlus.Fit("gaus","E","",0.8.,2.4);
   c2->Print("pic2.gif");
// wait
   c2.WaitPrimitive();

/////////////////////////////////////////////////
   TCanvas *c21 = new TCanvas("c21","Time of Muon passes though DT in 2 places, NTowerEta<5");
   c21->Divide(2,2);
   c21->cd(1);
   hTimeMuonHB2DTTopMinus->Draw();
   c21->cd(2);
   hTimeMuonHB2DTTopPlus->Draw();
   c21->cd(3);
   hTimeMuonHB2DTBotMinus->Draw();
   c21->cd(4);
   hTimeMuonHB2DTBotPlus->Draw();
   c21->Print("pic21.gif");
  c21.WaitPrimitive();
/////////////////////////////////////////////////
   TCanvas *c22 = new TCanvas("c22","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c22->Divide(2,2);
   c22->cd(1);
   hNumTowerMuonHB2DTTopMinus ->Draw();
   c22->cd(2);
   hNumTowerMuonHB2DTTopPlus->Draw();
   c22->cd(3);
   hNumTowerMuonHB2DTBotMinus->Draw();
   c22->cd(4);
   hNumTowerMuonHB2DTBotPlus->Draw();
   c22->Print("pic22.gif");
// wait
  c22.WaitPrimitive();
/////////////////////////////////////////////////
   TCanvas *c4 = new TCanvas("c4","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c4->Divide(2,2);
   c4->cd(1);
   hIdPhiTowerHB2DTMinus->Draw();
   c4->cd(2);
   hIdPhiTowerHB2DTPlus->Draw();

   c4->cd(3);
//   hIdPhiTowerHB2DTsevMinus->Draw();
   c4->cd(4);
//   hIdPhiTowerHB2DTsevPlus->Draw();
   c4->Print("pic4.gif");
// wait
  c4.WaitPrimitive();

/////////////////////////////////////////////////
   TCanvas *c51 = new TCanvas("c51","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c51->Divide(2,2);
   c51->cd(1);
   hNumHitsHB2DT->Draw();
   c51->cd(2);
   hLengthMuonDTHB2DT->Draw();
   c51->cd(3);
   hNumHitsHB2DT2->Draw();
   c51->cd(4);
   hLengthMuonDTHB2DT2->Draw();
   c51->Print("pic51.gif");
// wait
  c51.WaitPrimitive();
/////////////////////////////////////////////////
   TCanvas *c61 = new TCanvas("c61","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c61->Divide(2,2);
   c61->cd(1);
   //hEmuonHB2DTTopMinusTimeMinus->Draw();
   hEmuonHB2DTTopMinusTimeMinus.Fit("gaus","E","",0.8.,2.4);
   c61->cd(2);
   hEmuonHB2DTTopPlusTimeMinus.Fit("gaus","E","",0.8.,2.4);
   c61->cd(3);
   hEmuonHB2DTBotMinusTimeMinus.Fit("gaus","E","",0.8.,2.4);
   c61->cd(4);
   hEmuonHB2DTBotPlusTimeMinus.Fit("gaus","E","",0.8.,2.4);
   c61->Print("pic61.gif");
// wait
  c61.WaitPrimitive();
/////////////////////////////////////////////////
   TCanvas *c62 = new TCanvas("c62","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c62->Divide(2,2);
   c62->cd(1);
   hEmuonHB2DTTopMinusTimePlus->Fit("gaus","E","",0.8.,2.4);
   c62->cd(2);
   hEmuonHB2DTTopPlusTimePlus->Fit("gaus","E","",0.8.,2.4);
   c62->cd(3);
   hEmuonHB2DTBotMinusTimePlus->Fit("gaus","E","",0.8.,2.4);
   c62->cd(4);
   hEmuonHB2DTBotPlusTimePlus->Fit("gaus","E","",0.8.,2.4);
   c62->Print("pic62.gif");
// wait
  c62.WaitPrimitive();
/////////////////////////////////////////////////
   TCanvas *c63 = new TCanvas("c63","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c63->Divide(2,2);
   c63->cd(1);
   hEmuonHB2DTBotPlusTimeMinus1->Fit("gaus","E","",0.8.,2.4);
   c63->cd(2);
   hEmuonHB2DTBotPlusTimeMinus2->Fit("gaus","E","",0.8.,2.4);
   c63->cd(3);
   hEmuonHB2DTBotPlusTimeMinus3->Fit("gaus","E","",0.8.,2.4);
   c63->cd(4);
   hEmuonHB2DTBotPlusTimeMinus4->Fit("gaus","E","",0.8.,2.4);
   c63->Print("pic63.gif");
// wait
  c63.WaitPrimitive();
/////////////////////////////////////////////////
   TCanvas *c64 = new TCanvas("c64","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c64->Divide(2,2);
   c64->cd(1);
   hEmuonHB2DTBotPlusTimePlus1->Fit("gaus","E","",0.8.,2.4);
   c64->cd(2);
   hEmuonHB2DTBotPlusTimePlus2->Fit("gaus","E","",0.8.,2.4);
   c64->cd(3);
   hEmuonHB2DTBotPlusTimePlus3->Fit("gaus","E","",0.8.,2.4);
   c64->cd(4);
   hEmuonHB2DTBotPlusTimePlus4->Fit("gaus","E","",0.8.,2.4);
   c64->Print("pic64.gif");
// wait
  c64.WaitPrimitive();
/////////////////////////////////////////////////
   TCanvas *c65 = new TCanvas("c65","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c65->Divide(3,2);
   c65->cd(1);
   hEmuonPhiDetaTower1->Fit("gaus","E","",0.8.,2.4);
   c65->cd(2);
   hEmuonPhiDetaTower2->Fit("gaus","E","",0.8.,2.4);
   c65->cd(3);
   hEmuonPhiDetaTower3->Fit("gaus","E","",0.8.,2.4);
   c65->cd(4);
   hEmuonPhiDetaTower4->Fit("gaus","E","",0.8.,2.4);
   c65->cd(5);
   hEmuonPhiDetaTower5->Fit("gaus","E","",0.8.,2.4);
   c65->cd(6);
   hEmuonPhiDetaTower6->Fit("gaus","E","",0.8.,2.4);
   c65->Print("pic65.gif");
// wait
  c65.WaitPrimitive();

////////////////////////////////////////////////
   TCanvas *c7 = new TCanvas("c7","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c7->Divide(2,2);
   c7->cd(1);
   hImpXYHB2DT->Draw();
   c7->cd(2);
   hZImpXYHB2DT->Draw();
   c7->cd(3);
   hLmuonDTImpXY->Draw();
   c7->cd(4);
   hImpXYvsZ->Draw();
   c7->Print("pic7.gif");
// wait
  c7.WaitPrimitive();
////////////////////////////////////////////////
   TCanvas *c8 = new TCanvas("c8","Number Eta Towers of Muon passes though DT in 2 places, NTowerEta<5");
   c8->Divide(2,2);
   c8->cd(1);
   hProfTimeAsIdPhiMinus->Draw();
   c8->cd(2);
   hProfTimeAsIdPhiPlus->Draw();
   c8->Print("pic8.gif");
// wait
  c8.WaitPrimitive();
*/
}
