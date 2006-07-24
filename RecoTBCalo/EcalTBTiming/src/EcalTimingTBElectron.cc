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
   //   pndiodeProducer_   = iConfig.getParameter<std::string>("pndiodeProducer");
   txtFileName_        = iConfig.getUntrackedParameter<std::string>("TTPeakTime","TTPeakPositionFileEle.txt");
   txtFileForChGroups_ = iConfig.getUntrackedParameter<std::string>("ChPeakTime","ChPeakTimeEle.txt");
   ampl_thr_           = (float)(iConfig.getUntrackedParameter<double>("amplThr", 500.)); 
   min_num_ev_         = (int) (iConfig.getUntrackedParameter<double>("minNumEvt", 100.)); 
   sm_                 = (int) (iConfig.getUntrackedParameter<double>("SM", 1.)); 
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
  amplProfile_ = TProfile(profName,profTit,1700, 1, 1701, 0,50000);

  sprintf(profName,"timingTDC_prof");
  sprintf(profTit," profile of converged timing compared to the TDC");
  absoluteTimingTDC_= TProfile(profName,profTit,1700, 1, 1701, 0,15);

  sprintf(profName,"timingNoTDC_prof");
  sprintf(profTit," profile of converged timing compared without TDC subtraction");
  absoluteTimingNoTDC_= TProfile(profName,profTit,1700, 1, 1701, 0,15);

  sprintf(profName,"chi2_prof");
  sprintf(profTit," profile of chi2");
  Chi2Profile_ = TProfile(profName,profTit,1700, 1, 1701, 0,50000);

  sprintf(profName,"rel_timing_prof");
  sprintf(profTit," profile of relative timing between channels");
  relativeTiming_ = TProfile(profName,profTit,1700, 1, 1701, -10,10);

  char histoName[100];char histoTit[100];
  sprintf(histoName,"tdc_time");
  sprintf(histoTit," time of the tdc");
  tTDC_ = TH1F(histoName, histoTit, 500, -30, 30);
}

//========================================================================
void
EcalTimingTBElectron::endJob() {
//========================================================================
// produce offsets for each TT 
  if( absoluteTimingTDC_.GetEntries() <= 0 ){
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
    if( absoluteTimingTDC_.GetBinEntries(cry) > min_num_ev_ ){
      
      if(cry>0 && cry< 1701){ TT = table_cry_tt[cry];}
      else {TT =0;}
      int TTi = TT -1;
      if(TTi>=0 && TTi<68){
	float time = absoluteTimingTDC_.GetBinContent(cry);
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
    if( relativeTiming_.GetBinEntries(cry) > min_num_ev_ ){
      
      if(cry>0 && cry< 1701){ TTr = table_cry_tt[cry];}
      else {TTr =0;}
      int TTi = TTr -1;
      if(TTi>=0 && TTi<68){
	float time = relativeTiming_.GetBinContent(cry);
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
  txt_outfile<<"#   TT   time of the peak  \t  TT RMS \t rel timing \t TT RMS"<<std::endl;
  for(int i=0;i<68;i++){
    txt_outfile <<"   "<<setw(4)<<i+1<<" \t "<<setw(6)<<setprecision(5)<< mean[i] <<" \t "<<setw(6)<<setprecision(5)<<RMS[i]<<" \t "<<setw(6)<<setprecision(5)<< meanr[i] <<" \t "<<setw(6)<<setprecision(5)<<RMSr[i]<< endl;
  }
  txt_outfile.close();

  ofstream txt_channels;
  txt_channels.open(txtFileForChGroups_.c_str(),ios::out);
  for(int i=1;i<1701;i++){
    txt_channels <<sm_<<"   "<<setw(4)<<i<<" \t "<<setw(6)<<setprecision(5)<<absoluteTimingTDC_.GetBinContent(i)<< endl;
  }
  txt_channels.close();



  TFile f(rootfile_.c_str(),"RECREATE");
  
  amplProfile_.Write();
  absoluteTimingTDC_.Write();
  absoluteTimingNoTDC_.Write();
  Chi2Profile_.Write();
  relativeTiming_.Write();
  tTDC_.Write();
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
  
  Handle<EcalTBTDCRecInfo> pTDC;
  const EcalTBTDCRecInfo* recTDC=0;
  try {
    iEvent.getByLabel( "tdcReco", "EcalTBTDCRecInfo", pTDC);
    recTDC = pTDC.product();
    //LogDebug("EBBeamHodoTask") << " TDC offset is: " << recTDC->offset() << endl;
  } catch ( std::exception& ex ) {
    //LogError("EcalBeamTask") << "Error! Can't get the product EcalTBTDCRecInfo" << std::endl;
  }
   
  float tdcVal = 0;
  if(recTDC) {tdcVal = recTDC->offset() ;}
  else {cout<<"Error no tdc in the data"<<endl; return;}
  tTDC_.Fill(tdcVal);
 
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
  std::cout << "# of EcalUncalibratedRecHits hits: " << hits->size() << std::endl;
  for(EcalUncalibratedRecHitCollection::const_iterator ithit = hits->begin(); ithit != hits->end(); ++ithit) {
    
    EBDetId anid(ithit->id()); 
    int SMind = (anid.ieta()-1)*20+ anid.iphi();
     
    if(ithit->chi2()>0. && ithit->amplitude()> ampl_thr_ ) { // make sure fit has converged
      amplProfile_.Fill(SMind,ithit->amplitude());
      absoluteTimingTDC_.Fill(SMind,ithit->jitter()-tdcVal);
      Chi2Profile_.Fill(SMind,ithit->chi2());
      absTime[SMind] = ithit->jitter();
    }
    if(ithit->chi2()<0 && false)
      std::cout << "analytic fit failed! EcalUncalibratedRecHit  id: "
		<< EBDetId(ithit->id()) << "\t"
		<< "amplitude: " << ithit->amplitude() << ", jitter: " << ithit->jitter()
		<< std::endl;
    
  }//end of loop over hits

  float ave_time = 0;
  float foundCh = 0;
  for ( int ch= 1; ch<1701;ch++){
    if(absTime[ch] >0){ ave_time += absTime[ch]; foundCh++;}
  }
   
  if ( foundCh > 0 ){
    ave_time  = ave_time / foundCh;
    for( int ch =0; ch<1701; ch++){
      if(absTime[ch]>0){relativeTiming_.Fill( ch, absTime[ch]- ave_time );}
    }
  }
   
}
