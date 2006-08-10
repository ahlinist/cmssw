/**\class EcalTimingLaser

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  A. Ghezzi
//
//
#include "RecoTBCalo/EcalTBTiming/src/EcalTimingLaser.h"
//#include "RecoTBCalo/EcalTimingLaser/srcgeomTable.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalPnDiodeDigi.h"
#include <DataFormats/EcalRawData/interface/EcalRawDataCollections.h>

#include<fstream>
#include <iomanip>
#include <iostream>
#include "TFile.h"
#include<string>
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

using namespace std;
//========================================================================
EcalTimingLaser::EcalTimingLaser( const edm::ParameterSet& iConfig )
//========================================================================
{
   //now do what ever initialization is needed
   rootfile_           = iConfig.getUntrackedParameter<std::string>("rootfile","LaserTiming.root");
   digiProducer_       = iConfig.getParameter<std::string>("digiProducer");
   hitCollection_      = iConfig.getParameter<std::string>("hitCollection");
   hitProducer_        = iConfig.getParameter<std::string>("hitProducer");
   //   pndiodeProducer_   = iConfig.getParameter<std::string>("pndiodeProducer");
   txtFileName_        = iConfig.getUntrackedParameter<std::string>("TTPeakTime","TTPeakPositionFile.txt");
   txtFileForChGroups_ = iConfig.getUntrackedParameter<std::string>("ChPeakTime","ChPeakTime.txt");
   ampl_thr_           = (float)(iConfig.getUntrackedParameter<double>("amplThr", 500.)); 
   min_num_ev_         = (int) (iConfig.getUntrackedParameter<double>("minNumEvt", 100.)); 
   sm_                 = (int) (iConfig.getUntrackedParameter<double>("SM", 1.)); 
   //   std::cout << "Test fit : fetching hitCollection: " << hitCollection_.c_str()
   
   //	<< " produced by " << hitProducer_.c_str() << std::endl;

  
}


//========================================================================
EcalTimingLaser::~EcalTimingLaser()
//========================================================================
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}

//========================================================================
void
EcalTimingLaser::beginJob(edm::EventSetup const&) {
//========================================================================
  char profName[100];char profTit[100];
  for(int l=0; l<4; ++l) {
  
    sprintf(profName,"ampl_prof_conv_%d",l+1);
    sprintf(profTit," profile of converged amplitude for lambda laser %d",l+1);
    amplProfileConv_[l]= new TProfile(profName,profTit,1700, 1, 1701, 0,50000);

    sprintf(profName,"timing_prof_conv_%d",l+1);
    sprintf(profTit," profile of converged timing for lambda laser %d",l+1);
    absoluteTimingConv_[l]= new TProfile(profName,profTit,1700, 1, 1701, 0,15);

    sprintf(profName,"ampl_prof_all_%d",l+1);
    sprintf(profTit," profile of all amplitude for lambda laser %d",l+1);
    amplProfileAll_[l]= new TProfile(profName,profTit,1700, 1, 1701, 0,50000);

    sprintf(profName,"timing_prof_all_%d",l+1);
    sprintf(profTit," profile of all timing for lambda laser %d",l+1);
    absoluteTimingAll_[l]= new TProfile(profName,profTit,1700, 1, 1701, 0,50000);

    sprintf(profName,"chi2_prof_%d",l+1);
    sprintf(profTit," profile of chi2 for lambda laser %d",l+1);
    Chi2ProfileConv_[l]= new TProfile(profName,profTit,1700, 1, 1701, 0,50000);
  }
  sprintf(profName,"rel_timing_prof_conv_blu");
  sprintf(profTit," profile of converged relative timing for the blu laser");
  relativeTimingBlueConv_ = new TProfile(profName,profTit,1700, 1, 1701, -10,10);
  
  char hName[100];char hTit[100];
  sprintf(hName,"timingCry1");
  sprintf(hTit,"timing from a crystal in the first half");
  timeCry1=new TH1F(hName,hTit,600,0.,250.); 

  sprintf(hName,"timingCry2");
  sprintf(hTit,"timing from a crystal in the second half");
  timeCry2=new TH1F(hName,hTit,600,0.,250.); 


  ievt_ = 0;
}

//========================================================================
void EcalTimingLaser::endJob() {
//========================================================================
// produce offsets for each TT 
  if( absoluteTimingConv_[0]->GetEntries() <= 0 ){
    cout<<"Error:  the profilw is empty !! "<<endl;
  }

  int table_cry_tt[1701]={-1,
			1 ,1 ,1 ,1 ,1 ,2 ,2 ,2 ,2 ,2 ,3 ,3 ,3 ,3 ,3 ,4 ,4 ,4 ,4 ,4 ,
			1 ,1 ,1 ,1 ,1 ,2 ,2 ,2 ,2 ,2 ,3 ,3 ,3 ,3 ,3 ,4 ,4 ,4 ,4 ,4 ,
			1 ,1 ,1 ,1 ,1 ,2 ,2 ,2 ,2 ,2 ,3 ,3 ,3 ,3 ,3 ,4 ,4 ,4 ,4 ,4 ,
			1 ,1 ,1 ,1 ,1 ,2 ,2 ,2 ,2 ,2 ,3 ,3 ,3 ,3 ,3 ,4 ,4 ,4 ,4 ,4 ,
			1 ,1 ,1 ,1 ,1 ,2 ,2 ,2 ,2 ,2 ,3 ,3 ,3 ,3 ,3 ,4 ,4 ,4 ,4 ,4 ,
			5 ,5 ,5 ,5 ,5 ,6 ,6 ,6 ,6 ,6 ,7 ,7 ,7 ,7 ,7 ,8 ,8 ,8 ,8 ,8 ,
			5 ,5 ,5 ,5 ,5 ,6 ,6 ,6 ,6 ,6 ,7 ,7 ,7 ,7 ,7 ,8 ,8 ,8 ,8 ,8 ,
			5 ,5 ,5 ,5 ,5 ,6 ,6 ,6 ,6 ,6 ,7 ,7 ,7 ,7 ,7 ,8 ,8 ,8 ,8 ,8 ,
			5 ,5 ,5 ,5 ,5 ,6 ,6 ,6 ,6 ,6 ,7 ,7 ,7 ,7 ,7 ,8 ,8 ,8 ,8 ,8 ,
			5 ,5 ,5 ,5 ,5 ,6 ,6 ,6 ,6 ,6 ,7 ,7 ,7 ,7 ,7 ,8 ,8 ,8 ,8 ,8 ,
			9 ,9 ,9 ,9 ,9 ,10 ,10 ,10 ,10 ,10 ,11 ,11 ,11 ,11 ,11 ,12 ,12 ,12 ,12 ,12 ,
			9 ,9 ,9 ,9 ,9 ,10 ,10 ,10 ,10 ,10 ,11 ,11 ,11 ,11 ,11 ,12 ,12 ,12 ,12 ,12 ,
			9 ,9 ,9 ,9 ,9 ,10 ,10 ,10 ,10 ,10 ,11 ,11 ,11 ,11 ,11 ,12 ,12 ,12 ,12 ,12 ,
			9 ,9 ,9 ,9 ,9 ,10 ,10 ,10 ,10 ,10 ,11 ,11 ,11 ,11 ,11 ,12 ,12 ,12 ,12 ,12 ,
			9 ,9 ,9 ,9 ,9 ,10 ,10 ,10 ,10 ,10 ,11 ,11 ,11 ,11 ,11 ,12 ,12 ,12 ,12 ,12 ,
			13 ,13 ,13 ,13 ,13 ,14 ,14 ,14 ,14 ,14 ,15 ,15 ,15 ,15 ,15 ,16 ,16 ,16 ,16 ,16 ,
			13 ,13 ,13 ,13 ,13 ,14 ,14 ,14 ,14 ,14 ,15 ,15 ,15 ,15 ,15 ,16 ,16 ,16 ,16 ,16 ,
			13 ,13 ,13 ,13 ,13 ,14 ,14 ,14 ,14 ,14 ,15 ,15 ,15 ,15 ,15 ,16 ,16 ,16 ,16 ,16 ,
			13 ,13 ,13 ,13 ,13 ,14 ,14 ,14 ,14 ,14 ,15 ,15 ,15 ,15 ,15 ,16 ,16 ,16 ,16 ,16 ,
			13 ,13 ,13 ,13 ,13 ,14 ,14 ,14 ,14 ,14 ,15 ,15 ,15 ,15 ,15 ,16 ,16 ,16 ,16 ,16 ,
			17 ,17 ,17 ,17 ,17 ,18 ,18 ,18 ,18 ,18 ,19 ,19 ,19 ,19 ,19 ,20 ,20 ,20 ,20 ,20 ,
			17 ,17 ,17 ,17 ,17 ,18 ,18 ,18 ,18 ,18 ,19 ,19 ,19 ,19 ,19 ,20 ,20 ,20 ,20 ,20 ,
			17 ,17 ,17 ,17 ,17 ,18 ,18 ,18 ,18 ,18 ,19 ,19 ,19 ,19 ,19 ,20 ,20 ,20 ,20 ,20 ,
			17 ,17 ,17 ,17 ,17 ,18 ,18 ,18 ,18 ,18 ,19 ,19 ,19 ,19 ,19 ,20 ,20 ,20 ,20 ,20 ,
			17 ,17 ,17 ,17 ,17 ,18 ,18 ,18 ,18 ,18 ,19 ,19 ,19 ,19 ,19 ,20 ,20 ,20 ,20 ,20 ,
			21 ,21 ,21 ,21 ,21 ,22 ,22 ,22 ,22 ,22 ,23 ,23 ,23 ,23 ,23 ,24 ,24 ,24 ,24 ,24 ,
			21 ,21 ,21 ,21 ,21 ,22 ,22 ,22 ,22 ,22 ,23 ,23 ,23 ,23 ,23 ,24 ,24 ,24 ,24 ,24 ,
			21 ,21 ,21 ,21 ,21 ,22 ,22 ,22 ,22 ,22 ,23 ,23 ,23 ,23 ,23 ,24 ,24 ,24 ,24 ,24 ,
			21 ,21 ,21 ,21 ,21 ,22 ,22 ,22 ,22 ,22 ,23 ,23 ,23 ,23 ,23 ,24 ,24 ,24 ,24 ,24 ,
			21 ,21 ,21 ,21 ,21 ,22 ,22 ,22 ,22 ,22 ,23 ,23 ,23 ,23 ,23 ,24 ,24 ,24 ,24 ,24 ,
			25 ,25 ,25 ,25 ,25 ,26 ,26 ,26 ,26 ,26 ,27 ,27 ,27 ,27 ,27 ,28 ,28 ,28 ,28 ,28 ,
			25 ,25 ,25 ,25 ,25 ,26 ,26 ,26 ,26 ,26 ,27 ,27 ,27 ,27 ,27 ,28 ,28 ,28 ,28 ,28 ,
			25 ,25 ,25 ,25 ,25 ,26 ,26 ,26 ,26 ,26 ,27 ,27 ,27 ,27 ,27 ,28 ,28 ,28 ,28 ,28 ,
			25 ,25 ,25 ,25 ,25 ,26 ,26 ,26 ,26 ,26 ,27 ,27 ,27 ,27 ,27 ,28 ,28 ,28 ,28 ,28 ,
			25 ,25 ,25 ,25 ,25 ,26 ,26 ,26 ,26 ,26 ,27 ,27 ,27 ,27 ,27 ,28 ,28 ,28 ,28 ,28 ,
			29 ,29 ,29 ,29 ,29 ,30 ,30 ,30 ,30 ,30 ,31 ,31 ,31 ,31 ,31 ,32 ,32 ,32 ,32 ,32 ,
			29 ,29 ,29 ,29 ,29 ,30 ,30 ,30 ,30 ,30 ,31 ,31 ,31 ,31 ,31 ,32 ,32 ,32 ,32 ,32 ,
			29 ,29 ,29 ,29 ,29 ,30 ,30 ,30 ,30 ,30 ,31 ,31 ,31 ,31 ,31 ,32 ,32 ,32 ,32 ,32 ,
			29 ,29 ,29 ,29 ,29 ,30 ,30 ,30 ,30 ,30 ,31 ,31 ,31 ,31 ,31 ,32 ,32 ,32 ,32 ,32 ,
			29 ,29 ,29 ,29 ,29 ,30 ,30 ,30 ,30 ,30 ,31 ,31 ,31 ,31 ,31 ,32 ,32 ,32 ,32 ,32 ,
			33 ,33 ,33 ,33 ,33 ,34 ,34 ,34 ,34 ,34 ,35 ,35 ,35 ,35 ,35 ,36 ,36 ,36 ,36 ,36 ,
			33 ,33 ,33 ,33 ,33 ,34 ,34 ,34 ,34 ,34 ,35 ,35 ,35 ,35 ,35 ,36 ,36 ,36 ,36 ,36 ,
			33 ,33 ,33 ,33 ,33 ,34 ,34 ,34 ,34 ,34 ,35 ,35 ,35 ,35 ,35 ,36 ,36 ,36 ,36 ,36 ,
			33 ,33 ,33 ,33 ,33 ,34 ,34 ,34 ,34 ,34 ,35 ,35 ,35 ,35 ,35 ,36 ,36 ,36 ,36 ,36 ,
			33 ,33 ,33 ,33 ,33 ,34 ,34 ,34 ,34 ,34 ,35 ,35 ,35 ,35 ,35 ,36 ,36 ,36 ,36 ,36 ,
			37 ,37 ,37 ,37 ,37 ,38 ,38 ,38 ,38 ,38 ,39 ,39 ,39 ,39 ,39 ,40 ,40 ,40 ,40 ,40 ,
			37 ,37 ,37 ,37 ,37 ,38 ,38 ,38 ,38 ,38 ,39 ,39 ,39 ,39 ,39 ,40 ,40 ,40 ,40 ,40 ,
			37 ,37 ,37 ,37 ,37 ,38 ,38 ,38 ,38 ,38 ,39 ,39 ,39 ,39 ,39 ,40 ,40 ,40 ,40 ,40 ,
			37 ,37 ,37 ,37 ,37 ,38 ,38 ,38 ,38 ,38 ,39 ,39 ,39 ,39 ,39 ,40 ,40 ,40 ,40 ,40 ,
			37 ,37 ,37 ,37 ,37 ,38 ,38 ,38 ,38 ,38 ,39 ,39 ,39 ,39 ,39 ,40 ,40 ,40 ,40 ,40 ,
			41 ,41 ,41 ,41 ,41 ,42 ,42 ,42 ,42 ,42 ,43 ,43 ,43 ,43 ,43 ,44 ,44 ,44 ,44 ,44 ,
			41 ,41 ,41 ,41 ,41 ,42 ,42 ,42 ,42 ,42 ,43 ,43 ,43 ,43 ,43 ,44 ,44 ,44 ,44 ,44 ,
			41 ,41 ,41 ,41 ,41 ,42 ,42 ,42 ,42 ,42 ,43 ,43 ,43 ,43 ,43 ,44 ,44 ,44 ,44 ,44 ,
			41 ,41 ,41 ,41 ,41 ,42 ,42 ,42 ,42 ,42 ,43 ,43 ,43 ,43 ,43 ,44 ,44 ,44 ,44 ,44 ,
			41 ,41 ,41 ,41 ,41 ,42 ,42 ,42 ,42 ,42 ,43 ,43 ,43 ,43 ,43 ,44 ,44 ,44 ,44 ,44 ,
			45 ,45 ,45 ,45 ,45 ,46 ,46 ,46 ,46 ,46 ,47 ,47 ,47 ,47 ,47 ,48 ,48 ,48 ,48 ,48 ,
			45 ,45 ,45 ,45 ,45 ,46 ,46 ,46 ,46 ,46 ,47 ,47 ,47 ,47 ,47 ,48 ,48 ,48 ,48 ,48 ,
			45 ,45 ,45 ,45 ,45 ,46 ,46 ,46 ,46 ,46 ,47 ,47 ,47 ,47 ,47 ,48 ,48 ,48 ,48 ,48 ,
			45 ,45 ,45 ,45 ,45 ,46 ,46 ,46 ,46 ,46 ,47 ,47 ,47 ,47 ,47 ,48 ,48 ,48 ,48 ,48 ,
			45 ,45 ,45 ,45 ,45 ,46 ,46 ,46 ,46 ,46 ,47 ,47 ,47 ,47 ,47 ,48 ,48 ,48 ,48 ,48 ,
			49 ,49 ,49 ,49 ,49 ,50 ,50 ,50 ,50 ,50 ,51 ,51 ,51 ,51 ,51 ,52 ,52 ,52 ,52 ,52 ,
			49 ,49 ,49 ,49 ,49 ,50 ,50 ,50 ,50 ,50 ,51 ,51 ,51 ,51 ,51 ,52 ,52 ,52 ,52 ,52 ,
			49 ,49 ,49 ,49 ,49 ,50 ,50 ,50 ,50 ,50 ,51 ,51 ,51 ,51 ,51 ,52 ,52 ,52 ,52 ,52 ,
			49 ,49 ,49 ,49 ,49 ,50 ,50 ,50 ,50 ,50 ,51 ,51 ,51 ,51 ,51 ,52 ,52 ,52 ,52 ,52 ,
			49 ,49 ,49 ,49 ,49 ,50 ,50 ,50 ,50 ,50 ,51 ,51 ,51 ,51 ,51 ,52 ,52 ,52 ,52 ,52 ,
			53 ,53 ,53 ,53 ,53 ,54 ,54 ,54 ,54 ,54 ,55 ,55 ,55 ,55 ,55 ,56 ,56 ,56 ,56 ,56 ,
			53 ,53 ,53 ,53 ,53 ,54 ,54 ,54 ,54 ,54 ,55 ,55 ,55 ,55 ,55 ,56 ,56 ,56 ,56 ,56 ,
			53 ,53 ,53 ,53 ,53 ,54 ,54 ,54 ,54 ,54 ,55 ,55 ,55 ,55 ,55 ,56 ,56 ,56 ,56 ,56 ,
			53 ,53 ,53 ,53 ,53 ,54 ,54 ,54 ,54 ,54 ,55 ,55 ,55 ,55 ,55 ,56 ,56 ,56 ,56 ,56 ,
			53 ,53 ,53 ,53 ,53 ,54 ,54 ,54 ,54 ,54 ,55 ,55 ,55 ,55 ,55 ,56 ,56 ,56 ,56 ,56 ,
			57 ,57 ,57 ,57 ,57 ,58 ,58 ,58 ,58 ,58 ,59 ,59 ,59 ,59 ,59 ,60 ,60 ,60 ,60 ,60 ,
			57 ,57 ,57 ,57 ,57 ,58 ,58 ,58 ,58 ,58 ,59 ,59 ,59 ,59 ,59 ,60 ,60 ,60 ,60 ,60 ,
			57 ,57 ,57 ,57 ,57 ,58 ,58 ,58 ,58 ,58 ,59 ,59 ,59 ,59 ,59 ,60 ,60 ,60 ,60 ,60 ,
			57 ,57 ,57 ,57 ,57 ,58 ,58 ,58 ,58 ,58 ,59 ,59 ,59 ,59 ,59 ,60 ,60 ,60 ,60 ,60 ,
			57 ,57 ,57 ,57 ,57 ,58 ,58 ,58 ,58 ,58 ,59 ,59 ,59 ,59 ,59 ,60 ,60 ,60 ,60 ,60 ,
			61 ,61 ,61 ,61 ,61 ,62 ,62 ,62 ,62 ,62 ,63 ,63 ,63 ,63 ,63 ,64 ,64 ,64 ,64 ,64 ,
			61 ,61 ,61 ,61 ,61 ,62 ,62 ,62 ,62 ,62 ,63 ,63 ,63 ,63 ,63 ,64 ,64 ,64 ,64 ,64 ,
			61 ,61 ,61 ,61 ,61 ,62 ,62 ,62 ,62 ,62 ,63 ,63 ,63 ,63 ,63 ,64 ,64 ,64 ,64 ,64 ,
			61 ,61 ,61 ,61 ,61 ,62 ,62 ,62 ,62 ,62 ,63 ,63 ,63 ,63 ,63 ,64 ,64 ,64 ,64 ,64 ,
			61 ,61 ,61 ,61 ,61 ,62 ,62 ,62 ,62 ,62 ,63 ,63 ,63 ,63 ,63 ,64 ,64 ,64 ,64 ,64 ,
			65 ,65 ,65 ,65 ,65 ,66 ,66 ,66 ,66 ,66 ,67 ,67 ,67 ,67 ,67 ,68 ,68 ,68 ,68 ,68 ,
			65 ,65 ,65 ,65 ,65 ,66 ,66 ,66 ,66 ,66 ,67 ,67 ,67 ,67 ,67 ,68 ,68 ,68 ,68 ,68 ,
			65 ,65 ,65 ,65 ,65 ,66 ,66 ,66 ,66 ,66 ,67 ,67 ,67 ,67 ,67 ,68 ,68 ,68 ,68 ,68 ,
			65 ,65 ,65 ,65 ,65 ,66 ,66 ,66 ,66 ,66 ,67 ,67 ,67 ,67 ,67 ,68 ,68 ,68 ,68 ,68 ,
			65 ,65 ,65 ,65 ,65 ,66 ,66 ,66 ,66 ,66 ,67 ,67 ,67 ,67 ,67 ,68 ,68 ,68 ,68 ,68 };

  
  // Absolute timing
  TH1F* absTT = new TH1F("absolute_TT_timing", "absolute average timing per TT",500,0,10);
  TH1F* absCh = new TH1F("absolute_Ch_timing", "absolute average timing per channel",500,0,10);
  TH1F* absTTRMS = new TH1F("absolute_TT_spread", "RMS of the absolute timing in the TT ",200,0,2);
  float mean[68],x2[68],nCry[68], RMS[68];
  for(int u=0;u<68;u++){
    mean[u] =0; x2[u]=0; nCry[u]=0;
    RMS[u] = -1;
  }
  int TT = 0;
  for(int cry=1; cry < 1700l; cry++){
    if( absoluteTimingConv_[0]->GetBinEntries(cry) > min_num_ev_ ){
      
      if(cry>0 && cry< 1701){ TT = table_cry_tt[cry];}
      else {TT =0;}
      int TTi = TT -1;
      if(TTi>=0 && TTi<68){
	float time = absoluteTimingConv_[0]->GetBinContent(cry);
	absCh->Fill(time);
	mean[TTi] += time;
	x2[TTi] += time*time;
	nCry[TTi]++;
      }
    }
  }
  
  for(int u=0;u<68;u++){
    if(nCry[u] > 0){
      mean[u] = mean[u]/nCry[u];
      float rms2 = x2[u]/nCry[u] - mean[u]*mean[u];
      if( rms2 > 0) {RMS[u] = sqrt(rms2);}
    }
    else{mean[u] = -100.;}
    //cout<<u+1<<"  "<< mean[u] << "  "<< RMS[u]<<endl;
    absTT->Fill(mean[u]);
    absTTRMS->Fill(RMS[u]);
  }
  
 // Relative timing
  TH1F* relTT = new TH1F("relative_TT_timing", "relative average timing per TT",500,-5,5);
  TH1F* relCh = new TH1F("relative_Ch_timing", "relative average timing per channel",500,-5,5);
  TH1F* relTTRMS = new TH1F("relative_TT_spread", "RMS of the relative timing in the TT ",200,0,2);
  float meanr[68],x2r[68],nCryr[68], RMSr[68];
  for(int u=0;u<68;u++){
    meanr[u] =0; x2r[u]=0; nCryr[u]=0;
    RMSr[u] = -1;
  }
  int TTr = 0;
  for(int cry=1; cry < 1701; cry++){
    if( relativeTimingBlueConv_->GetBinEntries(cry) > min_num_ev_ ){
      
      if(cry>0 && cry< 1701){ TTr = table_cry_tt[cry];}
      else {TTr =0;}
      int TTi = TTr -1;
      if(TTi>=0 && TTi<68){
	float time = relativeTimingBlueConv_->GetBinContent(cry);
	relCh->Fill(time);
	meanr[TTi] += time;
	x2r[TTi] += time*time;
	nCryr[TTi]++;
      }
    }
  }
  
  for(int u=0;u<68;u++){
    if(nCryr[u] > 0){
      meanr[u] = meanr[u]/nCryr[u];
      float rms2 = x2r[u]/nCryr[u] - meanr[u]*meanr[u];
      if( rms2 > 0) {RMSr[u] = sqrt(rms2);}
    }
    else{meanr[u] = -100.;}
    relTT->Fill(meanr[u]);
    relTTRMS->Fill(RMSr[u]);
    cout<<u+1<<"  "<< mean[u] << "  "<< RMS[u]<<"  " <<meanr[u] << "  "<< RMSr[u]<<endl;
  }



  
  
  ofstream txt_outfile;
  txt_outfile.open(txtFileName_.c_str(),ios::out);
  txt_outfile<< "# Average time of the peak for each TT, (sample units)"<<endl;
  txt_outfile<<"#   TT   time of the peak  \t  TT RMS \t rel timing \t TT RMS"<<std::endl;
  for(int i=0;i<68;i++){
    txt_outfile <<"   "<<setw(4)<<i+1<<" \t "<<setw(6)<<setprecision(5)<< mean[i] <<" \t "<<setw(6)<<setprecision(5)<<RMS[i]<<" \t "<<setw(6)<<setprecision(5)<< meanr[i] <<" \t "<<setw(6)<<setprecision(5)<<RMSr[i]<< endl;
  }
  txt_outfile.close();

  ofstream txt_channels;
  txt_channels.open(txtFileForChGroups_.c_str(),ios::out);
  for(int i=1;i<1701;i++){
    
    //txt_channels <<sm_<<"   "<<setw(4)<<i<<" \t "<<setw(6)<<setprecision(5)<<absoluteTimingConv_[0]->GetBinContent(i)<< endl;
    //float half_corr  = 0;
    //if ( i<101 || (i-1)%20 > 9){half_corr=0;}
    //else {half_corr=0.08;}//2ns
    //txt_channels <<sm_<<"   "<<setw(4)<<i<<" \t "<<setw(6)<<setprecision(5)<<relativeTimingBlueConv_->GetBinContent(i)+5+half_corr<< endl;
    txt_channels <<sm_<<"   "<<setw(4)<<i<<" \t "<<setw(6)<<setprecision(5)<<relativeTimingBlueConv_->GetBinContent(i)<< endl;
  }
  txt_channels.close();



  TFile f(rootfile_.c_str(),"RECREATE");
  f.cd();
  for(int l=0; l<4; ++l) {
    if(l==0 || l==4){
      amplProfileConv_[l]->Write();
      absoluteTimingConv_[l]->Write();
      //amplProfileAll_[l]->Write();
      absoluteTimingAll_[l]->Write();
      Chi2ProfileConv_[l]->Write();
    }
  }
  relativeTimingBlueConv_->Write();
  TDirectory* hist = gDirectory->mkdir("histos");
  hist->cd();
  absTT->Write();absCh->Write();absTTRMS->Write();
  relTT->Write();relCh->Write();relTTRMS->Write();
  timeCry2->Write();
  timeCry1->Write();
  f.cd();
  f.Close();
}

//
// member functions
//

//========================================================================
void
EcalTimingLaser::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup ) {
//========================================================================

   using namespace edm;
   using namespace cms;
   ievt_++;
   edm::Handle<EcalRawDataCollection> DCCHeaders;

   try{ 
     iEvent.getByLabel(digiProducer_, DCCHeaders);
   } catch ( std::exception& ex ) {
     std::cerr << "Error! can't get the product for EcalRawDataCollection" << std::endl;
   }
   int lambda = -1;
  for ( EcalRawDataCollection::const_iterator headerItr= DCCHeaders->begin();headerItr != DCCHeaders->end(); 
	  ++headerItr ) {
    EcalDCCHeaderBlock::EcalDCCEventSettings settings = headerItr->getEventSettings();
    lambda= settings.wavelength;
  }
  if(lambda <0 || lambda > 3){std::cout<<"Error wrong value for laser wavelength: "<<lambda<<std::endl; return;}
   Handle<EcalUncalibratedRecHitCollection> phits;
   float absTime[1701];
   for(int i=0;i<1701;i++){absTime[i]=-10;}
   try {
     iEvent.getByLabel( hitProducer_, hitCollection_,phits);
   } catch ( std::exception& ex ) {
     std::cerr << "Error! can't get the product " << hitCollection_.c_str() << std::endl;
   }
   // loop over hits
   const EcalUncalibratedRecHitCollection* hits = phits.product(); // get a ptr to the product
   if(ievt_%100 ==0){std::cout <<"Event: "<<ievt_<< "# of EcalUncalibratedRecHits hits: " << hits->size() << std::endl;}
   for(EcalUncalibratedRecHitCollection::const_iterator ithit = hits->begin(); ithit != hits->end(); ++ithit) {

     EBDetId anid(ithit->id()); 
     //int SMind = (anid.ieta()-1)*20+ anid.iphi();
     int SMind = anid.ic();
     if(ithit->chi2()>0. && ithit->chi2()<300. && ithit->amplitude()> ampl_thr_ ) { // make sure fit has converged
       amplProfileConv_[lambda]->Fill(SMind,ithit->amplitude());
       absoluteTimingConv_[lambda]->Fill(SMind,ithit->jitter());
       Chi2ProfileConv_[lambda]->Fill(SMind,ithit->chi2());
       if(lambda == 0){absTime[SMind] = ithit->jitter();}
       if(SMind == 648  ){timeCry2->Fill(ithit->jitter()*25.);}
       else if(SMind == 653  ){timeCry1->Fill(ithit->jitter()*25.);}
     }
     
     amplProfileAll_[lambda]->Fill(SMind,ithit->amplitude());
     absoluteTimingAll_[lambda]->Fill(SMind,ithit->jitter());

     if(ithit->chi2()<0 && false)
     std::cout << "analytic fit failed! EcalUncalibratedRecHit  id: "
               << EBDetId(ithit->id()) << "\t"
               << "amplitude: " << ithit->amplitude() << ", jitter: " << ithit->jitter()
               << std::endl;

   }//end of loop over hits

   float ave_time = 0;
   float foundCh = 0;
   for ( int ch= 1; ch<1701;ch++){
     if(absTime[ch] >2 && absTime[ch]<8){ ave_time += absTime[ch]; foundCh++;}
   }
   
   if ( foundCh > 0 ){
     ave_time  = ave_time / foundCh;
     for( int ch =0; ch<1701; ch++){
       if(absTime[ch]>0) {relativeTimingBlueConv_->Fill( ch, absTime[ch]- ave_time );}
     }
   }
   
}
