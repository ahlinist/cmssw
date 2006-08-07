/**\class EcalTimingLaser

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  A. Ghezzi
//
//
#include "RecoTBCalo/EcalTBTiming/src/EcalTimingTBElectron.h"
//#include "RecoTBCalo/EcalTimingLaser/srcgeomTable.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalPnDiodeDigi.h"
#include <DataFormats/EcalRawData/interface/EcalRawDataCollections.h>

#include "TBDataFormats/EcalTBObjects/interface/EcalTBCollections.h"
#include "TBDataFormats/EcalTBObjects/interface/EcalTBTDCRecInfo.h"

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
EcalTimingTBElectron::EcalTimingTBElectron( const edm::ParameterSet& iConfig )
//========================================================================
{
   //now do what ever initialization is needed
   rootfile_           = iConfig.getUntrackedParameter<std::string>("rootfile","timingEle.root");
   digiProducer_       = iConfig.getParameter<std::string>("digiProducer");
   hitCollection_      = iConfig.getParameter<std::string>("hitCollection");
   hitProducer_        = iConfig.getParameter<std::string>("hitProducer");
   tdcRecInfoProducer_ = iConfig.getParameter<std::string>("tdcProducer");
   tdcRecInfoCollection_ = iConfig.getParameter<std::string>("tdcCollection");
   //   pndiodeProducer_   = iConfig.getParameter<std::string>("pndiodeProducer");
   txtFileName_        = iConfig.getUntrackedParameter<std::string>("TTPeakTime","TTPeakPositionFileEle.txt");
   txtFileForChGroups_ = iConfig.getUntrackedParameter<std::string>("ChPeakTime","ChPeakTimeEle.txt");
   ampl_thr_           = (float)(iConfig.getUntrackedParameter<double>("amplThr", 500.)); 
   chi2_thr_           = (float)(iConfig.getUntrackedParameter<double>("Chi2Thr", 200.));
   min_num_ev_         = (int) (iConfig.getUntrackedParameter<double>("minNumEvt", 100.));
   sm_                 = (int) (iConfig.getUntrackedParameter<double>("SM", 1.)); 
   cry_                 = (int) (iConfig.getUntrackedParameter<double>("cry", 1.)); 
   //   std::cout << "Test fit : fetching hitCollection: " << hitCollection_.c_str()
   
   //	<< " produced by " << hitProducer_.c_str() << std::endl;

  
}


//========================================================================
EcalTimingTBElectron::~EcalTimingTBElectron()
//========================================================================
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

//========================================================================
void
EcalTimingTBElectron::beginJob(edm::EventSetup const&) {
//========================================================================
  char profName[100];char profTit[100];

  
  sprintf(profName,"ampl_prof");
  sprintf(profTit," profile of converged amplitude");
  amplProfile_ = new TProfile(profName,profTit,1700, 1, 1701, 0,50000);

  sprintf(profName,"timingTDC_prof");
  sprintf(profTit," profile of converged timing compared to the TDC");
  absoluteTimingTDC_= new TProfile(profName,profTit,1700, 1, 1701, 0,15);

  sprintf(profName,"timingNoTDC_prof");
  sprintf(profTit," profile of converged timing compared without TDC subtraction");
  absoluteTimingNoTDC_= new TProfile(profName,profTit,1700, 1, 1701, 0,15);

  sprintf(profName,"chi2_prof");
  sprintf(profTit," profile of chi2");
  Chi2Profile_ = new TProfile(profName,profTit,1700, 1, 1701, 0,50000);

  sprintf(profName,"rel_timing_prof");
  sprintf(profTit," profile of relative timing between channels");
  relativeTiming_ = new TProfile(profName,profTit,1700, 1, 1701, -10,10);

  char histoName[100];char histoTit[100];
  sprintf(histoName,"tdc_time");
  sprintf(histoTit," time of the tdc");
  tTDC_ = new TH1F(histoName, histoTit, 750, -1, 2);
  tTDC_->GetXaxis()->SetTitle("time (sample)");

  sprintf(histoName,"cry_time");
  sprintf(histoTit,"time from the crystal");
  tCry_ = new TH1F(histoName, histoTit, 1500,  3, 9);
  tCry_->GetXaxis()->SetTitle("time (sample)");

  sprintf(histoName,"cry_tdc");
  sprintf(histoTit,"crystal time - TDC");
  CryTdc_ = new TH1F(histoName, histoTit, 1000, -50, 50);
  CryTdc_->GetXaxis()->SetTitle("time (ns)");

  sprintf(histoName,"cry_cry");
  sprintf(histoTit,"crystal1 time - crystal2 time");
  cry_cry_= new TH1F(histoName, histoTit, 100, -5, 5);
  cry_cry_->GetXaxis()->SetTitle("time (ns)");

  sprintf(profName,"tVSamp");
  sprintf(profTit,"ttime vs amplitude");
  tvsAmpl_ = new TProfile(profName,profTit,100, 200, 2700, -50,50);

  sprintf(profName,"ampPr");
  sprintf(profTit,"amplitude vs sample (fixed tdc value)");
  amplProf_= new TProfile(profName,profTit,10,1,11,100,3000);
  
  ievt_ = 0;
}

//========================================================================
void
EcalTimingTBElectron::endJob() {
//========================================================================
// produce offsets for each TT 
  if( absoluteTimingTDC_->GetEntries() <= 0 ){
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
  float mean[68],x2[68],nCry[68], RMS[68];
  for(int u=0;u<68;u++){
    mean[u] =0; x2[u]=0; nCry[u]=0;
    RMS[u] = -1;
  }
  int TT = 0;
  for(int cry=1; cry < 1700l; cry++){
    if( absoluteTimingTDC_->GetBinEntries(cry) > min_num_ev_ ){
      
      if(cry>0 && cry< 1701){ TT = table_cry_tt[cry];}
      else {TT =0;}
      int TTi = TT -1;
      if(TTi>=0 && TTi<68){
	float time = absoluteTimingTDC_->GetBinContent(cry);
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
  }
  
 // Relative timing
  float meanr[68],x2r[68],nCryr[68], RMSr[68];
  for(int u=0;u<68;u++){
    meanr[u] =0; x2r[u]=0; nCryr[u]=0;
    RMSr[u] = -1;
  }
  int TTr = 0;
  for(int cry=1; cry < 1700l; cry++){
    //    if( relativeTiming_->GetBinEntries(cry) > min_num_ev_ ){
    if(absoluteTimingNoTDC_ ->GetBinEntries(cry) > min_num_ev_ ){
      
      if(cry>0 && cry< 1701){ TTr = table_cry_tt[cry];}
      else {TTr =0;}
      int TTi = TTr -1;
      if(TTi>=0 && TTi<68){
	//float time = relativeTiming_->GetBinContent(cry);
	float time =absoluteTimingNoTDC_->GetBinContent(cry);
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
    cout<<u+1<<"  "<< mean[u] << "  "<< RMS[u]<<"  " <<meanr[u] << "  "<< RMSr[u]<<endl;
  }



  
  
  ofstream txt_outfile;
  txt_outfile.open(txtFileName_.c_str(),ios::out);
  txt_outfile<< "# Average time of the peak for each TT, (sample units)"<<endl;
  txt_outfile<<"#   TT   time of the peak  \t  TT RMS \t timing no TDC \t TT RMS"<<std::endl;
  for(int i=0;i<68;i++){
    txt_outfile <<"   "<<setw(4)<<i+1<<" \t "<<setw(6)<<setprecision(5)<< mean[i] <<" \t "<<setw(6)<<setprecision(5)<<RMS[i]<<" \t "<<setw(6)<<setprecision(5)<< meanr[i] <<" \t "<<setw(6)<<setprecision(5)<<RMSr[i]<< endl;
  }
  txt_outfile.close();

  ofstream txt_channels;
  txt_channels.open(txtFileForChGroups_.c_str(),ios::out);
  for(int i=1;i<1701;i++){
    txt_channels <<sm_<<"   "<<setw(4)<<i<<" \t "<<setw(6)<<setprecision(5)<<absoluteTimingTDC_->GetBinContent(i)<< endl;
  }
  txt_channels.close();



  TFile f(rootfile_.c_str(),"RECREATE");
  
  amplProfile_->Write();
  absoluteTimingTDC_->Write();
  absoluteTimingNoTDC_->Write();
  Chi2Profile_->Write();
  relativeTiming_->Write();
  tTDC_->Write();
  tCry_->Write();
  CryTdc_->Write();
  cry_cry_->Write();
  tvsAmpl_->Write();
  amplProf_->Write();
  f.Close();

}

//
// member functions
//

//========================================================================
void
EcalTimingTBElectron::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup ) {
//========================================================================

  using namespace edm;
  using namespace cms;
  ievt_ ++;
  //return;
  //  edm::Handle<EcalRawDataCollection> DCCHeaders;

//   try{ 
//     iEvent.getByLabel(digiProducer_, DCCHeaders);
//   } catch ( std::exception& ex ) {
//     std::cerr << "Error! can't get the product for EcalRawDataCollection" << std::endl;
//   }
  //   int lambda = -1;
  //   for ( EcalRawDataCollection::const_iterator headerItr= DCCHeaders->begin();headerItr != DCCHeaders->end(); 
  // 	  ++headerItr ) {
  //     EcalDCCHeaderBlock::EcalDCCEventSettings settings = headerItr->getEventSettings();
  //     lambda= settings.wavelength;
  //   }

 Handle<EcalTBEventHeader> pEventHeader;
  const EcalTBEventHeader* evtHeader=0;
  try {
    iEvent.getByLabel( "ecalEBunpacker", pEventHeader );
    evtHeader = pEventHeader.product(); // get a ptr to the product
    //std::cout << "Taken EventHeader " << std::endl;
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product for the event header" << std::endl;
  }
  
  
  int cry_in_beam = 0; 
  bool tb_moving = false;
  int event = 0;
  
  if(evtHeader){
    //cry_in_beam = evtHeader->nominalCrystalInBeam(); 
    cry_in_beam = evtHeader->crystalInBeam(); 
    tb_moving = evtHeader->tableIsMoving();
    event = evtHeader->eventNumber();
  }
  
  if(cry_in_beam != cry_ || tb_moving){return;}
  
  Handle<EcalTBTDCRecInfo> pTDC;
   const EcalTBTDCRecInfo* recTDC=0;
   try {
     //std::cout << "EcalSimpleTBAnalyzer::analyze getting product with label: " << digiProducer_.c_str()<< " prodname: " << digiCollection_.c_str() << endl
;
     iEvent.getByLabel( tdcRecInfoProducer_, tdcRecInfoCollection_, pTDC);
     recTDC = pTDC.product(); // get a ptr to the product
   } catch ( std::exception& ex ) {
     std::cerr << "Error! can't get the product " << tdcRecInfoCollection_.c_str() << std::endl;
   }


   
  float tdcVal = 0;
  float AmpCry=0;
  if(recTDC) {tdcVal = recTDC->offset() ;}
  else {cout<<"Error no tdc in the data"<<endl; return;}
  if(tdcVal<0.05 || tdcVal > 0.95){return;}
  tTDC_->Fill(tdcVal);

  float t1=0, t2=0;
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
  if(ievt_%200 ==0){std::cout <<"Event: "<<ievt_<< "  # of EcalUncalibratedRecHits hits: " << hits->size() << std::endl;}
  for(EcalUncalibratedRecHitCollection::const_iterator ithit = hits->begin(); ithit != hits->end(); ++ithit) {
    
    EBDetId anid(ithit->id()); 
    //int SMind = (anid.ieta()-1)*20+ anid.iphi();
    int SMind = anid.ic();
     
    if(ithit->chi2()>0. && ithit->chi2()<chi2_thr_ && ithit->amplitude()> ampl_thr_ ) { // make sure fit has converged
      amplProfile_->Fill(SMind,ithit->amplitude());
      absoluteTimingTDC_->Fill(SMind,ithit->jitter()+tdcVal);
      absoluteTimingNoTDC_->Fill(SMind,ithit->jitter() );
      Chi2Profile_->Fill(SMind,ithit->chi2());
      absTime[SMind] = ithit->jitter();
      if(SMind == cry_){
	tCry_->Fill( ithit->jitter() );
	CryTdc_->Fill ( (ithit->jitter() -5. + tdcVal)*25);
	tvsAmpl_->Fill( ithit->amplitude(), (ithit->jitter() -5. + tdcVal)*25 );
	AmpCry = ithit->amplitude();
	t1=ithit->jitter();
      }
      if(ievt_%200 ==0){std::cout <<"Event: "<<ievt_<< " cry: "<<SMind <<"  ampl: "<< ithit->amplitude() << std::endl;}
      //if(SMind == 1203 ){t1=ithit->jitter();}
      if(SMind == cry_+20){t2=ithit->jitter();}
      //if(t2>0){}
    }

    if(ithit->chi2()<0 && false)
      std::cout << "analytic fit failed! EcalUncalibratedRecHit  id: "
		<< EBDetId(ithit->id()) << "\t"
		<< "amplitude: " << ithit->amplitude() << ", jitter: " << ithit->jitter()
		<< std::endl;
    
  }//end of loop over hits
  
  //if(t1>0 && t2 >0){cry_cry_->Fill((t1-t2)*25);}

  float ave_time = 0;
  float foundCh = 0;
  for ( int ch= 1; ch<1701;ch++){
    if(absTime[ch] >0){ ave_time += absTime[ch]; foundCh++;}
  }
   
  if ( foundCh > 0 ){
    ave_time  = ave_time / foundCh;
    for( int ch =0; ch<1701; ch++){
      if(absTime[ch]>0){relativeTiming_->Fill( ch, absTime[ch]- ave_time );}
    }
  }

 //  //Fill the profile 
//   if ( fabs((tdcVal - 0.5)*25.) > 0.3 ){return;}
//   if ( AmpCry < ampl_thr_ ){return;}

//   Handle< EBDigiCollection > pEBDigis;
//   const EBDigiCollection* EBdigis =0;
  
//   try {
//     iEvent.getByLabel( "ecalEBunpacker", "" , pEBDigis);
//     //evt.getByLabel( digiProducer_, pEBDigis);
//     EBdigis = pEBDigis.product(); // get a ptr to the produc
//     cout<< "EcalAnalFitUncalibRecHitProducer: total # EBdigis: " << EBdigis->size() <<endl;
//   } catch ( std::exception& ex ) {
//     //     edm::LogError("EcalUncalibRecHitError") << "Error! can't get the product " << EBdigiCollection_.c_str() ;
//   }
   
//   if (EBdigis)
//     {
//       for(EBDigiCollection::const_iterator itdg = EBdigis->begin(); itdg != EBdigis->end(); ++itdg) {
// 	float adc[10];
// 	EBDataFrame dataframe = (*itdg);
// 	EBDetId id = dataframe.id();

// 	if ( id.ic() != cry_ ){continue;}
// 	bool gainSw = false;
// 	for (int i = 0; i < 10; i++) {
// 	  adc[i]=0;
// 	  EcalMGPASample sample = dataframe.sample(i);
// 	  int madc = sample.adc();
// 	  int gainid = sample.gainId();
// 	  if ( gainid == 1 ){//gain 12 
// 	    adc[i] = (float(madc)); 
// 	  }
// 	  else {gainSw= true;}
	  
// 	}// end of loop over samples
// 	if (!gainSw){
// 	  for(int i=0;i<10;i++){ amplProf_->Fill(i+1.5,adc[i]); }
// 	}
//       }
//     }

   
}
