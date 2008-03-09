// -*- C++ -*-
//
// Package:    HcalHPDNoiseAnalyser
// Class:      HcalHPDNoiseAnalyser
// 
/**\class HcalHPDNoiseAnalyser HcalHPDNoiseAnalyser.cc HcalHPDNoise/HcalHPDNoiseAnalyser/src/HcalHPDNoiseAnalyser.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Dmitry Vishnevskiy
//         Created:  Fri Feb  8 13:10:50 CET 2008
// $Id: HcalHPDNoiseAnalyser.cc,v 1.1 2008/03/08 14:30:00 dma Exp $
//
//

#include <iostream>
#include <cmath>
#include <iosfwd>
#include <bitset>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EventSetup.h"
using namespace edm;
// this is to retrieve HCAL digi's
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// Hardware working with ADC counts (Lin.ADC) (anything from HCAL reconstruction is not used) 
// for trigger LookupTables 1 Gev == 5 lin.adc
// actually this old style adc2fC convertion is ADC->Lin.ADC
// Before this convertion table was in DB hardcore (now adc2fC/0.92, later - nobody knows...)
// To not work with "Black Box", better to have this conversion table here...
// "static" defines adc2fC as local variable (only inside this file...)
static const float adc2fC[128]={-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5, 10.5,11.5,12.5,
                   13.5,15.,17.,19.,21.,23.,25.,27.,29.5,32.5,35.5,38.5,42.,46.,50.,54.5,59.5,
		   64.5,59.5,64.5,69.5,74.5,79.5,84.5,89.5,94.5,99.5,104.5,109.5,114.5,119.5,
		   124.5,129.5,137.,147.,157.,167.,177.,187.,197.,209.5,224.5,239.5,254.5,272.,
		   292.,312.,334.5,359.5,384.5,359.5,384.5,409.5,434.5,459.5,484.5,509.5,534.5,
		   559.5,584.5,609.5,634.5,659.5,684.5,709.5,747.,797.,847.,897.,947.,997.,
		   1047.,1109.5,1184.5,1259.5,1334.5,1422.,1522.,1622.,1734.5,1859.5,1984.5,
		   1859.5,1984.5,2109.5,2234.5,2359.5,2484.5,2609.5,2734.5,2859.5,2984.5,
		   3109.5,3234.5,3359.5,3484.5,3609.5,3797.,4047.,4297.,4547.,4797.,5047.,
		   5297.,5609.5,5984.5,6359.5,6734.5,7172.,7672.,8172.,8734.5,9359.5,9984.5};
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

class HcalHPDNoiseAnalyser : public edm::EDAnalyzer {
   public:
      explicit HcalHPDNoiseAnalyser(const edm::ParameterSet&);
      ~HcalHPDNoiseAnalyser();
   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      //////////////////////////////////////////////////////////////////////////////////////
      edm::ESHandle<HcalDbService> COND;
      HcalCalibrations calibs;
      const HcalQIEShape* shape_;
      //////////////////////////////////////////////////////////////////////////////////////
      void processHCALEvent();
      void storeEvent();
      void createRootStuff(int NumRun);
      int    local_event;
      int    global_event;
      int    run_number;
      double TrTreshold1;
      double TrTreshold2;
      double TrTreshold3;
      double ADCTreshold;
      int    EventsToStore;
      int    EventsStored;
      int    TPdigiTS;
      int    ADCdigiTS;
      //////////////////////////////////////////////////////////////////////////////////////
      TFile *theFile; 
      // Some common plots to estimate quality of the data 
      TH1F  *HBtowers;
      TH1F  *HEtowers;
      TH1F  *HOtowers;
      TH1F  *HFtowers;
      TH1F  *TPtowers;
      TH1F  *hbRaw;
      TH1F  *heRaw;
      TH1F  *hoRaw;
      TH1F  *hfRaw;
      TH1F  *hbEn;
      TH1F  *heEn;
      TH1F  *hoEn;
      TH1F  *hfEn;
      // some common TP plots
      TH1F  *RawTP;
      TH2F  *TPvsDigi;  
      TH1F  *NumOfTP; 
      TH1F  *TPTiming; 
      TH1F  *TPTimingTop; 
      TH1F  *TPTimingBot; 
      TH1F  *TP[10];
      TH2F  *TPOcc;
      TH1F  *TP_ADC;
      TH1F  *EnHPD;
      // due to HPD==Phi only for HB corrently it's easyer to analyze noise for HB only
      // Later (than Aram's map will be available is CMSSW) will be possible to add
      // the same things for HE,HO,HF
      // some histograms for TP threshold 1  : TriggerTreshold1 from CFG file          
      TH2F  *TPOccth1;
      TH2F  *TPOccthth1;
      TH1F  *TP_ADCth1;
      TH1F  *TP_ADCthth1;
      TH1F  *NumOfHPD1;
      TH1F  *NumOfPix1;
      TH1F  *HBPPhi1HPD1;
      TH1F  *HBMPhi1HPD1;
      TH1F  *HBPPhiNHPD1;
      TH1F  *HBMPhiNHPD1;
      // the same histograms for TP threshold 2  : TriggerTreshold2 from CFG file
      TH2F  *TPOccth2;
      TH2F  *TPOccthth2;
      TH1F  *TP_ADCth2;
      TH1F  *TP_ADCthth2;
      TH1F  *NumOfHPD2;
      TH1F  *NumOfPix2;
      TH1F  *HBPPhi1HPD2;
      TH1F  *HBMPhi1HPD2;
      TH1F  *HBPPhiNHPD2;
      TH1F  *HBMPhiNHPD2;
      // the same histograms for TP threshold 3  : TriggerTreshold3 from CFG file
      TH2F  *TPOccth3;
      TH2F  *TPOccthth3;
      TH1F  *TP_ADCth3;
      TH1F  *TP_ADCthth3;
      TH1F  *NumOfHPD3;
      TH1F  *NumOfPix3;
      TH1F  *HBPPhi1HPD3;
      TH1F  *HBMPhi1HPD3;
      TH1F  *HBPPhiNHPD3;
      TH1F  *HBMPhiNHPD3;
      //////////////////////////////////////////////////////////////////////////////////////
      // to not loop over all digi collecections several times
      // we can fill some event data "structure" to use it outside "digi loop"
      // it makes code possibly slower??, but for offline it's not so important...
      
      //HB: eta range: -16..-1,  1..16  , phi range 1..72, depth range 1..2
      //HE: eta range: -29..-16, 16..29 , phi range 1..72, depth range 1..3
      //HF: eta range: -41..-29, 29..41 , phi range 1..72, depth range 1..2
      //HO: eta range: -15..-1,  1..15  , phi range 1..72, depth range 4
      float        hbhe_adc[61][73][2][10]; 
      float        hf_adc  [83][73][2][10]; 
      float        ho_adc  [31][73][10]; 
      float        tp_digis[83][73][10];
      bool         is_hbhe [61][73][2];
      bool         is_hf   [83][73][2];
      bool         is_ho   [31][73];
      bool         is_tp   [83][73];
      void         clearEventData();
      void         set_hbhe_adc(int eta,int phi,int depth,float *val);
      void         set_hf_adc(int eta,int phi,int depth,float *val);
      void         set_ho_adc(int eta,int phi,float *val);
      void         set_tp_digi(int eta,int phi,float *val);
      bool         isset_hbhe(int eta,int phi,int depth);
      bool         isset_hf(int eta,int phi,int depth);
      bool         isset_ho(int eta,int phi);
      bool         isset_tp(int eta,int phi);
      float        *get_hbhe_adc(int eta,int phi,int depth);
      float        *get_hf_adc(int eta,int phi,int depth);
      float        *get_ho_adc(int eta,int phi);
      float        *get_tp_digi(int eta,int phi);

};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HcalHPDNoiseAnalyser::clearEventData(){
     memset(hbhe_adc,0,(sizeof(float)*61*73*2*10));
     memset(ho_adc,  0,(sizeof(float)*31*73*10));
     memset(hf_adc,  0,(sizeof(float)*83*73*2*10));
     memset(tp_digis,0,(sizeof(float)*83*73*10));
     memset(is_hbhe, 0,(sizeof(bool)*61*73*2));
     memset(is_ho,   0,(sizeof(bool)*31*73));
     memset(is_hf,   0,(sizeof(bool)*83*73*2));
     memset(is_tp,   0,(sizeof(bool)*83*73));
}
void HcalHPDNoiseAnalyser::set_hbhe_adc(int eta,int phi,int depth,float *val){
    // due to i have "not so nice" event structure, i should check index range to avoid segmentation fault...  
    if(eta<-29 || eta>29 || eta==0 || phi <1 || phi>72 || depth<1 || depth>3) return;
    for(int i=0;i<10;i++) hbhe_adc[eta+29][phi][depth][i]=val[i];
    is_hbhe[eta+29][phi][depth]=true;
}
void HcalHPDNoiseAnalyser::set_hf_adc(int eta,int phi,int depth,float *val){
    if(eta<-41 || eta>41 || (eta>-29 && eta<29) || phi <1 || phi>72 || depth<1 || depth>2) return;
    for(int i=0;i<10;i++) hf_adc[eta+41][phi][depth][i]=val[i];
    is_hf[eta+41][phi][depth]=true;
}
void HcalHPDNoiseAnalyser::set_ho_adc(int eta,int phi,float *val){
    if(eta<-15 || eta>15 || eta==0 || phi <1 || phi>72) return;
    for(int i=0;i<10;i++) ho_adc[eta+15][phi][i]=val[i];
    is_ho[eta+15][phi]=true;
}
void HcalHPDNoiseAnalyser::set_tp_digi(int eta,int phi,float *val){
    if(eta<-41 || eta>41 || phi <1 || phi>72) return;
    for(int i=0;i<10;i++) tp_digis[eta+41][phi][i]=val[i];
    is_tp[eta+41][phi]=true;
}
bool   HcalHPDNoiseAnalyser::isset_hbhe(int eta,int phi,int depth)  { return is_hbhe[eta+29][phi][depth]; } 
bool   HcalHPDNoiseAnalyser::isset_hf(int eta,int phi,int depth)    { return is_hf[eta+41][phi][depth];   } 
bool   HcalHPDNoiseAnalyser::isset_ho(int eta,int phi)              { return is_ho[eta+15][phi];          } 
bool   HcalHPDNoiseAnalyser::isset_tp(int eta,int phi)              { return is_tp[eta+41][phi];          } 
float* HcalHPDNoiseAnalyser::get_hbhe_adc(int eta,int phi,int depth){ return hbhe_adc[eta+29][phi][depth];}
float* HcalHPDNoiseAnalyser::get_hf_adc(int eta,int phi,int depth)  { return hf_adc[eta+41][phi][depth];  } 
float* HcalHPDNoiseAnalyser::get_ho_adc(int eta,int phi)            { return ho_adc[eta+15][phi];         } 
float* HcalHPDNoiseAnalyser::get_tp_digi(int eta,int phi)           { return tp_digis[eta+41][phi];       } 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

HcalHPDNoiseAnalyser::HcalHPDNoiseAnalyser(const edm::ParameterSet& iConfig){
     local_event=0;
     theFile=0;
     EventsStored=0;
     TrTreshold1   =      iConfig.getUntrackedParameter<double>("TriggerTreshold1",5.0);
     TrTreshold2   =      iConfig.getUntrackedParameter<double>("TriggerTreshold2",10.0);
     TrTreshold3   =      iConfig.getUntrackedParameter<double>("TriggerTreshold3",20.0);
     ADCTreshold   =      iConfig.getUntrackedParameter<double>("ADCTreshold",20.0);
     EventsToStore = (int)iConfig.getUntrackedParameter<double>("EventsToStore",100.0);
     TPdigiTS      = (int)iConfig.getUntrackedParameter<double>("TPdigiTS",3);
     ADCdigiTS     = (int)iConfig.getUntrackedParameter<double>("ADCdigiTS",5);
}

HcalHPDNoiseAnalyser::~HcalHPDNoiseAnalyser(){
     if(theFile!=0){ 
          theFile->cd(); 
	  theFile->Write(); 
	  theFile->Close();
     }
}
void HcalHPDNoiseAnalyser::createRootStuff(int NumRun){
char str[100];
  sprintf(str,"run%06i.root",NumRun);
  theFile  =new TFile(str, "RECREATE");
     HBtowers =new TH1F("Number of HB digits per event","Number of HB digits per event",100,0,2600);
     HEtowers =new TH1F("Number of HE digits per event","Number of HE digits per event",100,0,2600);
     HOtowers =new TH1F("Number of HO digits per event","Number of HO digits per event",100,0,2600);
     HFtowers =new TH1F("Number of HF digits per event","Number of HF digits per event",100,0,2600);
     TPtowers =new TH1F("Number of TP digits per event","Number of TP digits per event",100,0,4500);
  theFile->mkdir("HCAL ADC Plots");
  theFile->cd("HCAL ADC Plots");
     hbRaw    =new TH1F("HB ADC distribution (1TS)","HB ADC distribution (1TS)",256,0,255);
     heRaw    =new TH1F("HE ADC distribution (1TS)","HE ADC distribution (1TS)",256,0,255);
     hoRaw    =new TH1F("HO ADC distribution (1TS)","HO ADC distribution (1TS)",256,0,255);
     hfRaw    =new TH1F("HF ADC distribution (1TS)","HF ADC distribution (1TS)",256,0,255);
     hbEn     =new TH1F("HB energy distribution (10TS,Lin.ADC)","HB energy distribution (10TS,Lin.ADC)",1000,-5,995);
     heEn     =new TH1F("HE energy distribution (10TS,Lin.ADC)","HE energy distribution (10TS,Lin.ADC)",1000,-5,995);
     hoEn     =new TH1F("HO energy distribution (10TS,Lin.ADC)","HO energy distribution (10TS,Lin.ADC)",1000,-5,995);
     hfEn     =new TH1F("HF energy distribution (10TS,Lin.ADC)","HF energy distribution (10TS,Lin.ADC)",1000,-5,995);
  theFile->cd();
  theFile->mkdir("HCAL TP Plots");
  theFile->cd("HCAL TP Plots");
     RawTP    =new TH1F("raw TP distribution ((1TS)>0)","raw TP distribution ((1TS)>0)",200,0,200);
     TPvsDigi =new TH2F("HB: TP vs Digi","HB: TP vs Digi",500,-30.0,9970.0,500,0.0,2000.0); 
     TPvsDigi->SetXTitle("lin ADC digi");
     TPvsDigi->SetYTitle("TP digi");
     NumOfTP  =new TH1F("Number of TP>0 per event","Number of TP>0 per event",100,0,100); 
     TPTiming =new TH1F("TP Timing","TP Timing",10,0,10);
     TPTimingTop=new TH1F("TP Timing (Top wedges)","TP Timing (Top wedges)",10,0,10);
     TPTimingBot=new TH1F("TP Timing (Bottom wedges)","TP Timing (Bottom wedges)",10,0,10);  
     TP[0]    =new TH1F("TP spectrum (TS=0)","TP spectrum (TS=0)",200,0,200);
     TP[1]    =new TH1F("TP spectrum (TS=1)","TP spectrum (TS=1)",200,0,200);
     TP[2]    =new TH1F("TP spectrum (TS=2)","TP spectrum (TS=2)",200,0,200);
     TP[3]    =new TH1F("TP spectrum (TS=3)","TP spectrum (TS=3)",200,0,200);
     TP[4]    =new TH1F("TP spectrum (TS=4)","TP spectrum (TS=4)",200,0,200);
     TP[5]    =new TH1F("TP spectrum (TS=5)","TP spectrum (TS=5)",200,0,200);
     TP[6]    =new TH1F("TP spectrum (TS=6)","TP spectrum (TS=6)",200,0,200);
     TP[7]    =new TH1F("TP spectrum (TS=7)","TP spectrum (TS=7)",200,0,200);
     TP[8]    =new TH1F("TP spectrum (TS=8)","TP spectrum (TS=8)",200,0,200);
     TP[9]    =new TH1F("TP spectrum (TS=9)","TP spectrum (TS=9)",200,0,200);
     TPOcc    =new TH2F("TP Occupancy","TP Occupancy",81,-40,40,74,0,74);
     TP_ADC   =new TH1F("ADC spectrum TP>0","ADC spectrum TP>0",200,-10,500); 
     EnHPD    =new TH1F("HPD Energy","HPD Energy",1000,-100,9900); 
     
     sprintf(str,"TP Threshold %.0f",TrTreshold1);
     theFile->mkdir(str); 
     theFile->cd(str);
        sprintf(str,"TP Occupancy ( Threshols >= %.0f)",TrTreshold1); 
        TPOccth1   =new TH2F(str,str,81,-40,40,74,0,74);
        sprintf(str,"HB TP Occupancy ( TP Threshols >= %.0f,ADC Threshold >= %.0f)",TrTreshold1,ADCTreshold);
        TPOccthth1 =new TH2F(str,str,81,-40,40,74,0,74);
        sprintf(str,"ADC spectrum ( TP Threshols >= %.0f)",TrTreshold1); 
        TP_ADCth1  =new TH1F(str,str,200,-10,500);
        sprintf(str,"ADC spectrum ( TP Threshols >= %.0f,ADC Threshold >= %.0f)",TrTreshold1,ADCTreshold);
        TP_ADCthth1=new TH1F(str,str,200,-10,500);
        sprintf(str,"Number of HPD per event( TP Threshols >= %.0f)",TrTreshold1);
        NumOfHPD1  =new TH1F(str,str,100,0,100);    
        sprintf(str,"Number of HPD Pixels per event ( TP Threshols >= %.0f,ADC Threshold >= %.0f)",TrTreshold1,ADCTreshold);
        NumOfPix1  =new TH1F(str,str,17,0,17);
	sprintf(str,"HB+ phi distribution,TP Threshols >= %.0f, ADC Threshold > %.0f (only 1 HPD)",TrTreshold1,ADCTreshold);   
        HBPPhi1HPD1=new TH1F(str,str,74,0,74);
	sprintf(str,"HB- phi distribution,TP Threshols >= %.0f, ADC Threshold > %.0f (only 1 HPD)",TrTreshold1,ADCTreshold);     
        HBMPhi1HPD1=new TH1F(str,str,74,0,74);
	sprintf(str,"HB+ phi distribution,TP Threshols >= %.0f, ADC Threshold > %.0f (> 1 HPD)",TrTreshold1,ADCTreshold);    
	HBPPhiNHPD1=new TH1F(str,str,74,0,74);
	sprintf(str,"HB- phi distribution,TP Threshols >= %.0f, ADC Threshold > %.0f (> 1 HPD)",TrTreshold1,ADCTreshold);     
        HBMPhiNHPD1=new TH1F(str,str,74,0,74);
     theFile->cd();
     theFile->cd("HCAL TP Plots");
     sprintf(str,"TP Threshold %.0f",TrTreshold2);
     theFile->mkdir(str); 
     theFile->cd(str);
        sprintf(str,"TP Occupancy ( Threshols >= %.0f)",TrTreshold2); 
        TPOccth2   =new TH2F(str,str,81,-40,40,74,0,74);
        sprintf(str,"HB TP Occupancy ( TP Threshols >= %.0f,ADC Threshold >= %.0f)",TrTreshold2,ADCTreshold);
        TPOccthth2 =new TH2F(str,str,81,-40,40,74,0,74);
        sprintf(str,"ADC spectrum ( TP Threshols >= %.0f)",TrTreshold2); 
        TP_ADCth2  =new TH1F(str,str,200,-10,500);
        sprintf(str,"ADC spectrum ( TP Threshols >= %.0f,ADC Threshold >= %.0f)",TrTreshold2,ADCTreshold);
        TP_ADCthth2=new TH1F(str,str,200,-10,500);
        sprintf(str,"Number of HPD per event( TP Threshols >= %.0f)",TrTreshold2);
        NumOfHPD2  =new TH1F(str,str,100,0,100);    
        sprintf(str,"Number of HPD Pixels per event ( TP Threshols >= %.0f,ADC Threshold >= %.0f)",TrTreshold2,ADCTreshold);
        NumOfPix2  =new TH1F(str,str,17,0,17);
	sprintf(str,"HB+ phi distribution,TP Threshols >= %.0f, ADC Threshold > %.0f  (only 1 HPD)",TrTreshold2,ADCTreshold);     
        HBPPhi1HPD2=new TH1F(str,str,74,0,74);
	sprintf(str,"HB- phi distribution,TP Threshols >= %.0f, ADC Threshold > %.0f  (only 1 HPD)",TrTreshold2,ADCTreshold);   
        HBMPhi1HPD2=new TH1F(str,str,74,0,74);
	sprintf(str,"HB+ phi distribution,TP Threshols >= %.0f, ADC Threshold > %.0f  (> 1 HPD)",TrTreshold2,ADCTreshold);    
	HBPPhiNHPD2=new TH1F(str,str,74,0,74);
	sprintf(str,"HB- phi distribution,TP Threshols >= %.0f, ADC Threshold > %.0f  (> 1 HPD)",TrTreshold2,ADCTreshold);    
        HBMPhiNHPD2=new TH1F(str,str,74,0,74);
     theFile->cd();
     theFile->cd("HCAL TP Plots");
     sprintf(str,"TP Threshold %.0f",TrTreshold3);
     theFile->mkdir(str); 
     theFile->cd(str);
        sprintf(str,"TP Occupancy ( Threshols >= %.0f)",TrTreshold3); 
        TPOccth3   =new TH2F(str,str,81,-40,40,74,0,74);
        sprintf(str,"HB TP Occupancy ( TP Threshols >= %.0f,ADC Threshold >= %.0f)",TrTreshold3,ADCTreshold);
        TPOccthth3 =new TH2F(str,str,81,-40,40,74,0,74);
        sprintf(str,"ADC spectrum ( TP Threshols >= %.0f)",TrTreshold3); 
        TP_ADCth3  =new TH1F(str,str,200,-10,500);
        sprintf(str,"ADC spectrum ( TP Threshols >= %.0f,ADC Threshold >= %.0f)",TrTreshold3,ADCTreshold);
        TP_ADCthth3=new TH1F(str,str,200,-10,500);
        sprintf(str,"Number of HPD per event( TP Threshols >= %.0f)",TrTreshold3);
        NumOfHPD3  =new TH1F(str,str,100,0,100);    
        sprintf(str,"Number of HPD Pixels per event ( TP Threshols >= %.0f,ADC Threshold >= %.0f)",TrTreshold3,ADCTreshold);
        NumOfPix3  =new TH1F(str,str,17,0,17);
	sprintf(str,"HB+ phi distribution,TP Threshols >= %.0f, ADC Threshold > %.0f  (only 1 HPD)",TrTreshold3,ADCTreshold);       
        HBPPhi1HPD3=new TH1F(str,str,74,0,74);
	sprintf(str,"HB- phi distribution,TP Threshols >= %.0f, ADC Threshold > %.0f  (only 1 HPD)",TrTreshold3,ADCTreshold);        
        HBMPhi1HPD3=new TH1F(str,str,74,0,74);
	sprintf(str,"HB+ phi distribution,TP Threshols >= %.0f, ADC Threshold > %.0f (> 1 HPD)",TrTreshold3,ADCTreshold);       
	HBPPhiNHPD3=new TH1F(str,str,74,0,74);
	sprintf(str,"HB- phi distribution,TP Threshols >= %.0f , ADC Threshold > %.0f (> 1 HPD)",TrTreshold3,ADCTreshold);        
        HBMPhiNHPD3=new TH1F(str,str,74,0,74);  
  theFile->cd();
  theFile->mkdir("HCAL Events");
}

void HcalHPDNoiseAnalyser::storeEvent(){
char str[100]; 
  if(EventsStored>=EventsToStore) return;
  theFile->cd("HCAL Events");
  sprintf(str,"Run %i Event %i",run_number,global_event); 
  TH2F *hist=new TH2F(str,str,35,-17,17,74,0,74);
  for(int eta=-16;eta<=16;eta++) for(int phi=1;phi<=72;phi++){
       if(!isset_hbhe(eta,phi,1)) continue;
       float Energy=0; for(int j=0;j<10;j++) Energy+=get_hbhe_adc(eta,phi,1)[j]; 
       hist->Fill(eta,phi,Energy);
  }
  EventsStored++; 
}

void HcalHPDNoiseAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
int HBcnt=0,HEcnt=0,HOcnt=0,HFcnt=0,TPcnt=0,NumTP=0,eta,phi,depth,nTS;
float data[20],Energy;
   if(theFile==NULL) createRootStuff(iEvent.id().run()); 
   clearEventData();
   run_number=iEvent.id().run();
   global_event=iEvent.id().event();
 
   using namespace edm;
   
   try{
      edm::Handle<HBHEDigiCollection> hbhe; 
      iEvent.getByType(hbhe);
      for(HBHEDigiCollection::const_iterator digi=hbhe->begin();digi!=hbhe->end();digi++){
          eta=digi->id().ieta(); phi=digi->id().iphi(); depth=digi->id().depth(); nTS=digi->size();
          if(digi->id().subdet()==HcalBarrel) HBcnt++;
          if(digi->id().subdet()==HcalEndcap) HEcnt++;
	  try{
	     COND->makeHcalCalibration(digi->id(),&calibs);
	  }catch(...){
	     //printf("makeHcalCalibration failed: Eta,Phi,Dept=(%i,%i,%i)\n",eta,phi,depth);
	     continue;
	  }  
	  Energy=0;
          for(int i=0;i<nTS;i++){
	     data[i]=(double)adc2fC[digi->sample(i).adc()]-calibs.pedestal(digi->sample(i).capid()); 
	     if(digi->id().subdet()==HcalBarrel) hbRaw->Fill(digi->sample(i).adc());
	     if(digi->id().subdet()==HcalEndcap) heRaw->Fill(digi->sample(i).adc()); 
	     Energy+=data[i];
	  }
	  if(digi->id().subdet()==HcalBarrel) hbEn->Fill(Energy);
	  if(digi->id().subdet()==HcalEndcap) heEn->Fill(Energy); 
	  set_hbhe_adc(eta,phi,depth,data);
      }   
   }catch(...){}      
   try{
      edm::Handle<HODigiCollection> ho; 
      iEvent.getByType(ho);
      for(HODigiCollection::const_iterator digi=ho->begin();digi!=ho->end();digi++){
          eta=digi->id().ieta(); phi=digi->id().iphi(); depth=digi->id().depth(); nTS=digi->size();
          if(depth!=4) continue;
	  HOcnt++;
	  try{
	     COND->makeHcalCalibration(digi->id(),&calibs);
	  }catch(...){
	     //printf("makeHcalCalibration failed: Eta,Phi,Dept=(%i,%i,%i)\n",eta,phi,depth);
	     continue;
	  }  	  
	  Energy=0;
          for(int i=0;i<nTS;i++){
	     data[i]=(double)adc2fC[digi->sample(i).adc()]-calibs.pedestal(digi->sample(i).capid()); 
	     hoRaw->Fill(digi->sample(i).adc());
	     Energy+=data[i];
	  }
	  hoEn->Fill(Energy);
	  set_ho_adc(eta,phi,data);
      }   
   }catch(...){}      
   try{
      edm::Handle<HFDigiCollection> hf; 
      iEvent.getByType(hf);
      for(HFDigiCollection::const_iterator digi=hf->begin();digi!=hf->end();digi++){
          eta=digi->id().ieta(); phi=digi->id().iphi(); depth=digi->id().depth(); nTS=digi->size();
          HFcnt++;
	  try{
	     COND->makeHcalCalibration(digi->id(),&calibs);
	  }catch(...){
	     //printf("makeHcalCalibration failed: Eta,Phi,Dept=(%i,%i,%i)\n",eta,phi,depth);
	     continue;
	  }  	   	  
	  Energy=0;
          for(int i=0;i<nTS;i++){
	     data[i]=(double)adc2fC[digi->sample(i).adc()]-calibs.pedestal(digi->sample(i).capid());  
	     hfRaw->Fill(digi->sample(i).adc());
	     Energy+=data[i];
	  }
	  hfEn->Fill(Energy);
	  set_hf_adc(eta,phi,depth,data);
      }   
   }catch(...){}        
   try{
      edm::Handle<HcalTrigPrimDigiCollection> Tr_prim;
      iEvent.getByType(Tr_prim);
      for(HcalTrigPrimDigiCollection::const_iterator digi=Tr_prim->begin();digi!=Tr_prim->end();digi++){
	  eta=digi->id().ieta(); phi=digi->id().iphi(); nTS=digi->size();
	  TPcnt++;
	  for(int i=0;i<nTS;i++){
	     data[i]=digi->sample(i).compressedEt();
	     if(data[i]>0) RawTP->Fill(data[i]);
	     if(data[i]>2){ 
	         NumTP++; 
		 TPTiming->Fill(i);
		 if(phi>1 && phi<36)  TPTimingTop->Fill(i);  
		 if(phi>37 && phi<72) TPTimingBot->Fill(i); 
		 TPOcc->Fill(eta,phi);
		 if(data[i]>=TrTreshold1) TPOccth1->Fill(eta,phi);
		 if(data[i]>=TrTreshold2) TPOccth2->Fill(eta,phi);
		 if(data[i]>=TrTreshold3) TPOccth3->Fill(eta,phi);
		 TP[i]->Fill(data[i]);
 	     }
	  }
	  set_tp_digi(eta,phi,data);  
      }	   
   }catch(...){} 
   NumOfTP->Fill(NumTP);
   HBtowers->Fill(HBcnt);
   HEtowers->Fill(HEcnt);
   HOtowers->Fill(HOcnt);
   HFtowers->Fill(HFcnt);
   TPtowers->Fill(TPcnt);
   processHCALEvent();  
   if((++local_event%100)==0) 
   printf("Run: %i,Events processed: %i (HB: %i towers,HE: %i towers,HO: %i towers,HF: %i towers, TP: %i)\n",
             run_number,local_event,HBcnt,HEcnt,HOcnt,HFcnt,TPcnt);
}

void HcalHPDNoiseAnalyser::processHCALEvent(){
int eta,phi;
int HBPtp1[72],HBMtp1[72],HBPtp2[72],HBMtp2[72],HBPtp3[72],HBMtp3[72],HBPadc[72],HBMadc[72];
float Energy,ENERGYp,ENERGYm;
   for(int i=0;i<72;i++) HBPtp1[i]=HBMtp1[i]=HBPtp2[i]=HBMtp2[i]=HBPtp3[i]=HBMtp3[i]=HBPadc[i]=HBMadc[i]=0;
   // currently we want to look at HB only (HPD==Phi)... 
   for(phi=1;phi<=72;phi++){
     ENERGYp=ENERGYm=0;
     for(eta=-16;eta<=16;eta++){
       if(eta==0) continue;  
       if(!(isset_hbhe(eta,phi,1) || isset_hbhe(eta,phi,2)) || !isset_tp(eta,phi)) continue;
       int IsTP=0,IsTPth1=0,IsTPth2=0,IsTPth3=0;
       Energy=0;
       
       if(get_tp_digi(eta,phi)[TPdigiTS]>=TrTreshold1){
	   TPvsDigi->Fill(get_hbhe_adc(eta,phi,1)[ADCdigiTS]+get_hbhe_adc(eta,phi,1)[ADCdigiTS+1]+
		          get_hbhe_adc(eta,phi,2)[ADCdigiTS]+get_hbhe_adc(eta,phi,2)[ADCdigiTS+1], 
		          get_tp_digi(eta,phi)[TPdigiTS]);
       }
      
       for(int i=0;i<10;i++){
          if(get_tp_digi(eta,phi)[i]>2) IsTP++;
	  if(get_tp_digi(eta,phi)[i]>=TrTreshold1) IsTPth1++;   
	  if(get_tp_digi(eta,phi)[i]>=TrTreshold2) IsTPth2++;   
	  if(get_tp_digi(eta,phi)[i]>=TrTreshold3) IsTPth3++;   
       }
       for(int j=0;j<10;j++){ 
           Energy+=get_hbhe_adc(eta,phi,1)[j]+get_hbhe_adc(eta,phi,2)[j];
       }
       if(IsTP>0) TP_ADC->Fill(Energy);
             
       if(IsTPth1>0){ TP_ADCth1->Fill(Energy);}          
       if(IsTPth2>0){ TP_ADCth2->Fill(Energy);}                 
       if(IsTPth3>0){ TP_ADCth3->Fill(Energy);} 
      
       if(Energy>=ADCTreshold){	      
	  if(IsTPth1>0){ if(eta>0) HBPtp1[phi-1]++; if(eta<0) HBMtp1[phi-1]++; TPOccthth1->Fill(eta,phi); TP_ADCthth1->Fill(Energy); }
	  if(IsTPth2>0){ if(eta>0) HBPtp2[phi-1]++; if(eta<0) HBMtp2[phi-1]++; TPOccthth2->Fill(eta,phi); TP_ADCthth2->Fill(Energy); }
	  if(IsTPth3>0){ if(eta>0) HBPtp3[phi-1]++; if(eta<0) HBMtp3[phi-1]++; TPOccthth3->Fill(eta,phi); TP_ADCthth3->Fill(Energy); }
	  if(eta>0) HBPadc[phi-1]++;
	  if(eta<0) HBMadc[phi-1]++;      
       }
       if(eta>0) ENERGYp+=Energy;
       if(eta<0) ENERGYm+=Energy;
     }
     if(ENERGYp>ADCTreshold) EnHPD->Fill(ENERGYp);      
     if(ENERGYm>ADCTreshold) EnHPD->Fill(ENERGYm);      
   }
   int NHPD1=0,NHPD2=0,NHPD3=0; 
   for(int i=0;i<72;i++){ 
       if(HBPtp1[i]>0) NHPD1++; if(HBMtp1[i]>0) NHPD1++; 
       if(HBPtp2[i]>0) NHPD2++; if(HBMtp2[i]>0) NHPD2++; 
       if(HBPtp3[i]>0) NHPD3++; if(HBMtp3[i]>0) NHPD3++; 
       if(HBPtp1[i]>0 && HBPadc[i]>0) NumOfPix1->Fill(HBPadc[i]);
       if(HBPtp2[i]>0 && HBPadc[i]>0) NumOfPix2->Fill(HBPadc[i]);
       if(HBPtp3[i]>0 && HBPadc[i]>0) NumOfPix3->Fill(HBPadc[i]);
       if(HBMtp1[i]>0 && HBMadc[i]>0) NumOfPix1->Fill(HBMadc[i]);
       if(HBMtp2[i]>0 && HBMadc[i]>0) NumOfPix2->Fill(HBMadc[i]);
       if(HBMtp3[i]>0 && HBMadc[i]>0) NumOfPix3->Fill(HBMadc[i]);
   } 
   
   if(NHPD1>0)NumOfHPD1->Fill(NHPD1);
   if(NHPD2>0)NumOfHPD2->Fill(NHPD2);
   if(NHPD3>0)NumOfHPD3->Fill(NHPD3);
   if(NHPD1==1) for(int i=0;i<72;i++){ if(HBPadc[i]>0) HBPPhi1HPD1->Fill(i+1); if(HBMadc[i]>0) HBMPhi1HPD1->Fill(i+1); }   
   if(NHPD1>=2) for(int i=0;i<72;i++){ if(HBPadc[i]>0) HBPPhiNHPD1->Fill(i+1); if(HBMadc[i]>0) HBMPhiNHPD1->Fill(i+1); }
   if(NHPD2==1) for(int i=0;i<72;i++){ if(HBPadc[i]>0) HBPPhi1HPD2->Fill(i+1); if(HBMadc[i]>0) HBMPhi1HPD2->Fill(i+1); }   
   if(NHPD2>=2) for(int i=0;i<72;i++){ if(HBPadc[i]>0) HBPPhiNHPD2->Fill(i+1); if(HBMadc[i]>0) HBMPhiNHPD2->Fill(i+1); }
   if(NHPD3==1) for(int i=0;i<72;i++){ if(HBPadc[i]>0) HBPPhi1HPD3->Fill(i+1); if(HBMadc[i]>0) HBMPhi1HPD3->Fill(i+1); }   
   if(NHPD3>=2) for(int i=0;i<72;i++){ if(HBPadc[i]>0) HBPPhiNHPD3->Fill(i+1); if(HBMadc[i]>0) HBMPhiNHPD3->Fill(i+1); }
   
   // we want to store some events (for example with whole RBX noise...)
   if(NHPD1==4 || NHPD2==4 || NHPD3==4){ storeEvent(); }
}

void HcalHPDNoiseAnalyser::beginJob(const edm::EventSetup& iSetup){iSetup.get<HcalDbRecord>().get(COND);}
void HcalHPDNoiseAnalyser::endJob() {}
DEFINE_FWK_MODULE(HcalHPDNoiseAnalyser);
