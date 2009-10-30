/**\class EcalTimingAnalysis

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  J. Haupt  
//
// 
#include "CalibCalorimetry/EcalTiming/interface/EcalTimingAnalysis.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalPnDiodeDigi.h"
#include <DataFormats/EcalRawData/interface/EcalRawDataCollections.h>

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"
// Geometry
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"


#include<fstream>
#include <iomanip>
#include <iostream>
#include "TFile.h"
#include<string>
#include<vector>

#include "TProfile2D.h"
#include "TProfile.h"


//
// constants, enums and
//
// static data member definitions
//

//
// constructors and destructor
//


//========================================================================
EcalTimingAnalysis::EcalTimingAnalysis( const edm::ParameterSet& iConfig )
//========================================================================
{
   //now do what ever initialization is needed
   rootfile_           = iConfig.getUntrackedParameter<std::string>("rootfile","LaserTiming.root");
   digiProducer_       = iConfig.getParameter<std::string>("digiProducer");
   hitCollection_      = iConfig.getParameter<std::string>("hitCollection");
   hitCollectionEE_      = iConfig.getParameter<std::string>("hitCollectionEE");
   hitProducer_        = iConfig.getParameter<std::string>("hitProducer");
   hitProducerEE_        = iConfig.getParameter<std::string>("hitProducerEE");
   txtFileName_        = iConfig.getUntrackedParameter<std::string>("TTPeakTime","TTPeakPositionFile.txt");
   txtFileForChGroups_ = iConfig.getUntrackedParameter<std::string>("ChPeakTime","ChPeakTime.txt");
   ampl_thr_           = (float)(iConfig.getUntrackedParameter<double>("amplThr", 500.)); 
   min_num_ev_         = (int) (iConfig.getUntrackedParameter<double>("minNumEvt", 100.)); 
   timerunstart_       = iConfig.getUntrackedParameter<double>("RunStart",1220192037.);
   timerunlength_       = iConfig.getUntrackedParameter<double>("RunLength",2.);
   EBradius_           = iConfig.getUntrackedParameter<double>("EBRadius",1.4);
   corrtimeEcal        = iConfig.getUntrackedParameter<bool>("CorrectEcalReadout",false);
   corrtimeBH          = iConfig.getUntrackedParameter<bool>("CorrectBH",false);
   bhplus_             = iConfig.getUntrackedParameter<bool>("BeamHaloPlus",true);
   allave_             = iConfig.getUntrackedParameter<double>("AllAverage",5.7);
   allshift_           = iConfig.getUntrackedParameter<double>("AllShift",1.5);   

   fromfile_           = iConfig.getUntrackedParameter<bool>("FromFile",false);  
   if (fromfile_) fromfilename_ = iConfig.getUntrackedParameter<std::string>("FromFileName","EMPTYFILE.root");
   
   std::vector<double> listDefaults;
   for (int ji = 0; ji<54; ++ji)
     {
        listDefaults.push_back(0.);
     } 
   sMAves_ = iConfig.getUntrackedParameter<std::vector<double> >("SMAverages", listDefaults);
   sMCorr_ = iConfig.getUntrackedParameter<std::vector<double> >("SMCorrections", listDefaults);

   writetxtfiles_      = iConfig.getUntrackedParameter<bool>("WriteTxtFiles",false);
   correctAVE_         = iConfig.getUntrackedParameter<bool>("CorrectAverage",false);



}


//========================================================================
EcalTimingAnalysis::~EcalTimingAnalysis()
//========================================================================
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}
//========================================================================
void
EcalTimingAnalysis::beginRun(edm::EventSetup const& eventSetup ) {
//========================================================================
  // edm::ESHandle< EcalElectronicsMapping > handle;
  // eventSetup.get< EcalMappingRcd >().get(handle);
  // ecalElectronicsMap_ = handle.product();
}
//========================================================================


//========================================================================
void
EcalTimingAnalysis::beginJob( ) {
//========================================================================
  char profName[150];char profTit[150];
 
  TFile *tf =0;

  if (fromfile_) {
    tf = new TFile(fromfilename_.c_str()); 
	std::cout << "Getting file " << fromfilename_ << std::endl;
  }

  std::cout << " Step 0 " << std::endl;
  for(int dcc=1;dcc<55; ++dcc){
    int fed=dcc+600;
    for(int l=0; l<4; ++l) {
      sprintf(profName,"SM_%d_ampl_prof_conv_%d",fed,l+1);
      sprintf(profTit," SM_%d profile of converged amplitude for lambda laser %d;xtal; Converged Amplitude (ADC)",fed,l+1);
      amplProfileConv_[dcc-1][l]= fromfile_ ? ((TProfile*) tf->Get(Form("SM_%d/%s",fed,profName)))  : (new TProfile(profName,profTit,numXtals-1, 1, numXtals, 0,50000));
      sprintf(profName,"SM_%d_timing_prof_conv_%d",fed,l+1);
      sprintf(profTit," SM_%d profile of converged timing for lambda laser %d;xtal;Time from fitted pulse (1 clock = 25 ns)",fed,l+1);
      absoluteTimingConv_[dcc-1][l]= fromfile_ ? ((TProfile*) tf->Get(Form("SM_%d/%s",fed,profName)))  : (new TProfile(profName,profTit,numXtals-1, 1, numXtals, 0,15));
      sprintf(profName,"SM_%d_ampl_prof_all_%d",fed,l+1);
      sprintf(profTit," SM_%d profile of all amplitude for lambda laser %d;xtal;Amplitude of all events",fed,l+1);
      amplProfileAll_[dcc-1][l]= fromfile_ ? ((TProfile*) tf->Get(Form("SM_%d/%s",fed,profName)))  : (new TProfile(profName,profTit,numXtals-1, 1, numXtals, 0,50000));
      sprintf(profName,"SM_%d_timing_prof_all_%d",fed,l+1);
      sprintf(profTit," SM_%d profile of all timing for lambda laser %d;xtal;Time from xtal pulse (1 clock = 25 ns)",fed,l+1);
      absoluteTimingAll_[dcc-1][l]= fromfile_ ? ((TProfile*) tf->Get(Form("SM_%d/%s",fed,profName)))  : (new TProfile(profName,profTit,numXtals-1, 1, numXtals, 0,50000));
      sprintf(profName,"SM_%d_chi2_prof_%d",fed,l+1);
      sprintf(profTit," SM_%d profile of chi2 for lambda laser %d;xtal;chi2",fed,l+1);
      Chi2ProfileConv_[dcc-1][l]= fromfile_ ? ((TProfile*) tf->Get(Form("SM_%d/%s",fed,profName)))  : (new TProfile(profName,profTit,numXtals-1, 1, numXtals, 0,50000));
    }
    sprintf(profName,"SM_%d_rel_timing_prof_conv_blu",fed);
    sprintf(profTit,"SM_%d_profile of converged relative timing for the blu laser;xtal;Relative Time from fitted pulse (1 clock = 25 ns)",fed);
	relativeTimingBlueConv_[dcc-1] = fromfile_ ? ((TProfile*) tf->Get(Form("SM_%d/%s",fed,profName)))  : (new TProfile(profName,profTit,numXtals-1, 1, numXtals, -10,10));
	relativeTimingBlueConv_[dcc-1]->SetErrorOption("s");
    char hName[100];char hTit[100];
    sprintf(hName,"SM_%d_timingCry1",fed);
    sprintf(hTit,"SM_%d_timing from a crystal in the first half;Time from fitted pulse (ns)",fed);
    timeCry1[dcc-1]=fromfile_ ? ((TH1F*) tf->Get(Form("SM_%d/%s",fed,hName))) : (new TH1F(hName,hTit,600,0.,250.)); 
    sprintf(hName,"SM_%d_timingCry2",fed);
    sprintf(hTit,"SM_%d_timing from a crystal in the second half;Time from fitted pulse (ns)",fed);
    timeCry2[dcc-1]=fromfile_ ? ((TH1F*) tf->Get(Form("SM_%d/%s",fed,hName))) : (new TH1F(hName,hTit,600,0.,250.)); 
    sprintf(hName,"SM_%d_reltimingCry1",fed);
    sprintf(hTit,"SM_%d_ rel timing from a crystal in the first half;Relative time from xtal fit to LM average",fed);
    timeRelCry1[dcc-1]=fromfile_ ? ((TH1F*) tf->Get(Form("SM_%d/%s",fed,hName))) : (new TH1F(hName,hTit,600,-50.,50.)); 
    sprintf(hName,"SM_%d_reltimingCry2",fed);
    sprintf(hTit,"SM_%d_rel timing from a crystal in the second half;Relative time from xtal fit to LM average",fed);
    timeRelCry2[dcc-1]=fromfile_ ? ((TH1F*) tf->Get(Form("SM_%d/%s",fed,hName))) : (new TH1F(hName,hTit,600,-50.,50.)); 
    ttTiming_[dcc-1]=new TGraphErrors(68);
    ttTiming_[dcc-1]->SetTitle(Form("TTMeanWithRMS_SM_%d;TT;Timing Mean (1 Unit = 25ns)",fed));
    ttTiming_[dcc-1]->SetName(Form("TTMeanWithRMS_SM_%d",fed));
    ttTimingRel_[dcc-1]=new TGraphErrors(68);
    ttTimingRel_[dcc-1]->SetTitle(Form("TTMeanRelWithRMS_SM_%d;TT;Relative Timing Mean (1 Unit = 25ns)",fed));
    ttTimingRel_[dcc-1]->SetName(Form("TTMeanRelWithRMS_SM_%d",fed));
    aveRelXtalTimebyDCC_[dcc-1]=new TH1F(Form("Rel_TimingSigma_SM_%d",fed),Form("RMS of Relative Timing for SM %d; RMS of xtal relative timing",fed),600,0.,12.);
    lasershiftVsTime_[dcc-1] = fromfile_ ? ((TGraph*) tf->Get(Form("SM_%d/laserShiftvsTime_SM_%d",fed,fed))) : (new TGraph());//I SHOULD SPLIT THIS INTO LMs, rather than SMs... June-29th-2008 JHaupt
    lasershiftVsTime_[dcc-1]->SetTitle(Form("laserShiftvsTime_SM_%d;Time Stamp (s);SM Laser Average (ns)",fed));
    lasershiftVsTime_[dcc-1]->SetName(Form("laserShiftvsTime_SM_%d",fed));
    lasershiftVsTimehist_[dcc-1] = fromfile_ ? ((TH2F*) tf->Get(Form("SM_%d/laserShiftvsTimeHist_SM_%d",fed,fed))) : (new TH2F(Form("laserShiftvsTimeHist_SM_%d",fed),Form("laserShiftvsTimehist_SM_%d;Time Stamp (s);SM Laser Average (ns)",fed),2400,0.,timerunlength_, 500,85.,185.)); 
    lasershiftLM_[dcc-1]=fromfile_ ? ((TH1F*) tf->Get(Form("SM_%d/laserShift_SM_%d",fed,fed))) : (new TH1F(Form("laserShift_SM_%d",fed),Form("SM %d laser Avereage;SM Average Time (ns)",fed),1800,50.,200.));
    
    
    numGoodEvtsPerSM_[dcc-1] = 0;
    
  }

  lasersPerEvt = fromfile_ ? ((TH1F*) tf->Get("XtalsPerEvt")) : (new TH1F("XtalsPerEvt","Number of Xtals Active per Event;Event;Xtals Active",20000,0,20000));
  
  ievt_ = 0;

  lasershift_= fromfile_ ? ((TH1F*) tf->Get("laserShift")) : (new TH1F("laserShift","laser Avereage;SM Average Time (ns)",1800,50.,200.));
  
  ttTimingAll_=new TGraphErrors((68*54));
  ttTimingAll_->SetTitle("TTMeanWithRMS_All_FEDS;FED_TT;Timing Mean (1 Unit = 25ns)");
  ttTimingAll_->SetName("TTMeanWithRMS_All_FEDS");
  
  ttTimingAllRel_=new TGraphErrors((68*54));
  ttTimingAllRel_->SetTitle("TTMeanRelWithRMS_All_FEDS;FED_TT;Relative Timing Mean (1 Unit = 25ns)");
  ttTimingAllRel_->SetName("TTMeanRelWithRMS_All_FEDS");
  
  ttTimingAllSMChng_=new TGraphErrors((68*54));
  ttTimingAllSMChng_->SetTitle("TTMeanWithRMS_All_FEDS_CHANGED;FED_TT;Timing Mean (1 Unit = 25ns)");
  ttTimingAllSMChng_->SetName("TTMeanWithRMS_All_FEDS_CHANGED");
  
  aveRelXtalTime_=new TH1F("Rel_TimingSigma","RMS of Relative Timing; RMS of xtal relative timing",600,0.,12.);
  
  aveRelXtalTimeVsAbsTime_ = new TH2F("RelRMS_vs_AbsTime","Relative RMS of xtal vs absolute time;Relative Timing RMS of an xtal (ns);Absolute Timing of the xtal (1 clock = 25 ns)",500,0.,8.,600,2.,9.);

  //Now for the 3D timing plots.
  double ttEtaBins[36] = {-85, -80, -75, -70, -65, -60, -55, -50, -45, -40, -35, -30, -25, -20, -15, -10, -5, 0, 1, 6, 11, 16, 21, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 81, 86 };
  double ttPhiBins[73];
  double timingBins[126];
  double timingtBins[501];
  double ttEtaEEBins[21];
  for (int i = 0; i < 501; ++i)
    {
      timingtBins[i]=2.5+double(i)*5./500.;
	  
      if ( i < 126) {timingBins[i]=2.5+double(i)*5./125.;}
      if ( i < 21) {ttEtaEEBins[i]=0.0+double(i)*100./20.;}
      if (i<73) 
	   {
          ttPhiBins[i]=1+5*i;
	   }
    }
  
  ttTimingEtaPhi_    = fromfile_ ? ((TH3F*) tf->Get("timeTTAllFEDs")) : (new TH3F("timeTTAllFEDs","(Phi,Eta,time) for all FEDs (SM,TT binning);i#phi;i#eta;Relative Time (1 clock = 25ns)",72,ttPhiBins,35,ttEtaBins,125,timingBins)); 
  ttTimingEtaPhiRel_ = fromfile_ ? ((TH3F*) tf->Get("timeTTAllFEDsREL")) : (new TH3F("timeTTAllFEDsREL","(Phi,Eta,time corrected) for all FEDs (SM,TT binning);i#phi;i#eta;Relative Time (1 clock = 25ns)",72,ttPhiBins,35,ttEtaBins,125,timingBins)); 
  chTimingEtaPhi_    = fromfile_ ? ((TH3F*) tf->Get("timeCHAllFEDs")) : (new TH3F("timeCHAllFEDs","(Phi,Eta,time) for all FEDs (SM,ch binning);i#phi;i#eta;Relative Time (1 clock = 25ns)",360,1.,361.,171,-85.,86.,700,3.5,8.5)); 
  chTimingEtaPhiRel_ = fromfile_ ? ((TH3F*) tf->Get("timeCHAllFEDsREL")) : (new TH3F("timeCHAllFEDsREL","(Phi,Eta,time corrected) for all FEDs (SM,ch binning);i#phi;i#eta;Relative Time (1 clock = 25ns)",360,1.,361.,171,-85.,86.,500,3.5,6.5)); 

  chTimingEtaPhiEEP_    = fromfile_ ? ((TH3F*) tf->Get("EEPtimeCHAllFEDs")) : (new TH3F("EEPtimeCHAllFEDs","(ix,iy,time) for all FEDs EE+ (SM,ch binning);ix;iy;Relative Time (1 clock = 25ns)",100,1.,101.,100,1.0,101.,700,3.5,8.5)); 
  chTimingEtaPhiEEM_    = fromfile_ ? ((TH3F*) tf->Get("EEMtimeCHAllFEDs")) : (new TH3F("EEMtimeCHAllFEDs","(ix,iy,time) for all FEDs EE- (SM,ch binning);ix;iy;Relative Time (1 clock = 25ns)",100,1.,101.,100,1.0,101.,700,3.5,8.5)); 
  chTimingEtaPhiRelEEP_ = fromfile_ ? ((TH3F*) tf->Get("EEPtimeCHAllFEDsREL")) : (new TH3F("EEPtimeCHAllFEDsREL","(ix,iy,time corrected) for all FEDs EE+ (SM,ch binning);ix;iy;Relative Time (1 clock = 25ns)",100,1.,101.,100,1.,101.,500,3.5,7.)); 
  chTimingEtaPhiRelEEM_ = fromfile_ ? ((TH3F*) tf->Get("EEMtimeCHAllFEDsREL")) : (new TH3F("EEMtimeCHAllFEDsREL","(ix,iy,time corrected) for all FEDs EE- (SM,ch binning);ix;iy;Relative Time (1 clock = 25ns)",100,1.,101.,100,1.,101.,500,3.5,7.)); 
  
  ttTimingEtaPhiEEP_    = fromfile_ ? ((TH3F*) tf->Get("EEPtimeTTAllFEDs")) : (new TH3F("EEPtimeTTAllFEDs","(ix,iy,time) for all FEDs EE+ (SM,tt binning);ix;iy;Relative Time (1 clock = 25ns)",100/5,1.,101.,100/5,1.0,101.,125,2.,8.)); 
  ttTimingEtaPhiEEM_    = fromfile_ ? ((TH3F*) tf->Get("EEMtimeTTAllFEDs")) : (new TH3F("EEMtimeTTAllFEDs","(ix,iy,time) for all FEDs EE- (SM,tt binning);ix;iy;Relative Time (1 clock = 25ns)",100/5,1.,101.,100/5,1.0,101.,125,2.,8.)); 
  ttTimingEtaPhiRelEEP_ = fromfile_ ? ((TH3F*) tf->Get("EEPtimeTTAllFEDsREL")) : (new TH3F("EEPtimeTTAllFEDsREL","(ix,iy,time corrected) for all FEDs EE+ (SM,tt binning);ix;iy;Relative Time (1 clock = 25ns)",100/5,1.,101.,100/5,1.,101.,125,2.,8.)); 
  ttTimingEtaPhiRelEEM_ = fromfile_ ? ((TH3F*) tf->Get("EEMtimeTTAllFEDsREL")) : (new TH3F("EEMtimeTTAllFEDsREL","(ix,iy,time corrected) for all FEDs EE- (SM,tt binning);ix;iy;Relative Time (1 clock = 25ns)",100/5,1.,101.,100/5,1.,101.,125,2.,8.)); 
 
  //Now the eta profiles
  ttTimingEta_    = fromfile_ ? ((TProfile*) tf->Get("timeTTAllFEDsEta")) : (new TProfile("timeTTAllFEDsEta","(Eta,time) for all FEDs (SM,TT binning);i#eta;Relative Time (1 clock = 25ns)",35,ttEtaBins,2.5,7.5)); 
  ttTimingEtaRel_ = fromfile_ ? ((TProfile*) tf->Get("timeTTAllFEDsEtaREL")) : (new TProfile("timeTTAllFEDsEtaREL","(Eta,time corrected) for all FEDs (SM,TT binning);i#eta;Relative Time (1 clock = 25ns)",35,ttEtaBins,2.5,7.5)); 
  chTimingEta_    = fromfile_ ? ((TProfile*) tf->Get("timeCHAllFEDsEta")) : (new TProfile("timeCHAllFEDsEta","(Eta,time) for all FEDs (SM,ch binning);i#eta;Relative Time (1 clock = 25ns)",171,-85.,86.,2.5,7.5)); 
  chTimingEtaRel_ = fromfile_ ? ((TProfile*) tf->Get("timeCHAllFEDsEtaREL")) : (new TProfile("timeCHAllFEDsEtaREL","(Eta,time corrected) for all FEDs (SM,ch binning);i#eta;Relative Time (1 clock = 25ns)",171,-85.,86.,2.5,7.5)); 

  ttTimingEtaEEP_    = fromfile_ ? ((TProfile*) tf->Get("timeTTAllFEDsEtaEEP")) : (new TProfile("timeTTAllFEDsEtaEEP","(Eta,time) for EEP FEDs (SM,TT binning);i#eta;Relative Time (1 clock = 25ns)",20,ttEtaEEBins,2.5,7.5)); 
  ttTimingEtaRelEEP_ = fromfile_ ? ((TProfile*) tf->Get("timeTTAllFEDsEtaEEPREL")) : (new TProfile("timeTTAllFEDsEtaEEPREL","(Eta,time corrected) for EEP FEDs (SM,TT binning);i#eta;Relative Time (1 clock = 25ns)",20,ttEtaEEBins,2.5,7.5)); 

  ttTimingEtaEEM_    = fromfile_ ? ((TProfile*) tf->Get("timeTTAllFEDsEtaEEM")) : (new TProfile("timeTTAllFEDsEtaEEM","(Eta,time) for EEM FEDs (SM,TT binning);i#eta;Relative Time (1 clock = 25ns)",20,ttEtaEEBins,2.5,7.5)); 
  ttTimingEtaRelEEM_ = fromfile_ ? ((TProfile*) tf->Get("timeTTAllFEDsEtaEEMREL")) : (new TProfile("timeTTAllFEDsEtaEEMREL","(Eta,time corrected) for EEM FEDs (SM,TT binning);i#eta;Relative Time (1 clock = 25ns)",20,ttEtaEEBins,2.5,7.5)); 
  //Here is some info needed to convert DetId's to FEDIds's (DDCIds)
  //It is basically an electronics mapping service

  
  fullAmpProfileEB_  = fromfile_ ? ((TProfile2D*) tf->Get("fullAmpProfileEB"))  : (new TProfile2D("fullAmpProfileEB", " Average Amplitude EB ;i#phi;i#eta",360,1.,361.,171,-85.,86.,0.0,50000.));
  fullAmpProfileEEP_ = fromfile_ ? ((TProfile2D*) tf->Get("fullAmpProfileEEP")) : (new TProfile2D("fullAmpProfileEEP"," Average Amplitude EE+;ix;iy",100,1.,101.,100,1.,101.,0.0,50000.));
  fullAmpProfileEEM_ = fromfile_ ? ((TProfile2D*) tf->Get("fullAmpProfileEEM")) : (new TProfile2D("fullAmpProfileEEM"," Average Amplitude EE-;ix;iy",100,1.,101.,100,1.,101.,0.0,50000.));
 


}

//========================================================================
void EcalTimingAnalysis::endJob() {
//========================================================================
// produce offsets for each TT 
  
  float mean[68],x2[68],nCry[68], RMS[68]; //Variables for Absolute Timing
  TH1F* absTT[54];
  TH1F* absCh[54];
  TH1F* absTTRMS[54];
  float meanr[68],x2r[68],nCryr[68], RMSr[68]; //Variable for Relative Timing
  TH1F* relTT[54];
  TH1F* relCh[54];
  TH1F* relTTRMS[54]; 
  float ttStart[68];
  float ttRStart[68];
  TProfile* ttTime[54];
  TProfile* ttRTime[54];

  ofstream smave_outfile;
  if (writetxtfiles_) {
     smave_outfile.open(Form("SMAverages_%s",txtFileName_.c_str()),std::ios::out);
     smave_outfile<< "# Average time of the peak for each SM, (sample units)"<<std::endl;
     smave_outfile<<"#   SM \t  time of the peak "<<std::endl;
  }
  
  TProfile *fullttTime = new TProfile("Inside_TT_timing","Inside TT timing;xtal in TT;relative time from first xtal (1 clock = 25 ns)",26,0.,26.,-1.,1.);
  TProfile *fullttRTime = new TProfile("Inside_TT_Reltiming","Inside TT Rel timing;xtal in TT;relative time from first xtal (1 clock = 25 ns)",26,0.,26.,-1.,1.);
  TProfile *fullLMTime = new TProfile("LM_timing","LM timing;LM Number;relative time (1 clock = 25 ns)",92,1.,93.,-1.,8.);
  TProfile *fullLMTimeCorr = new TProfile("LM_timingCorrected","LM timing Corrected for Laser Fiber Length;LM Number;relative time (1 clock = 25 ns)",92,1.,93.,-1.,8.);
  TProfile *fullSMTime = new TProfile("SM_timing","SM timing;FED;relative time (1 clock = 25 ns)",54,601.,655.,-1.,8.);
  TProfile *fullSMTimeCorr = new TProfile("SM_timingCorrected","SM timing Corrected for Laser Fiber Length;FED;relative time (1 clock = 25 ns)",54,601.,655.,-1.,8.);
  
  for(int dcc = 1; dcc <55; ++dcc){
    int fed = dcc+600;
    if( absoluteTimingConv_[dcc-1][0]->GetEntries() <= 0 ){
      edm::LogWarning("EcalTimingAnalysis")<<"The profile is empty !! For FED  " << fed;
    }

    // Absolute timing
    absTT[dcc-1]    = new TH1F(Form("SM_%d_absolute_TT_timing",fed), Form("SM_%d_absolute average timing per TT;TT Average timing (1 clock = 25ns)",fed),500,0,10);
    absCh[dcc-1]    = new TH1F(Form("SM_%d_absolute_Ch_timing",fed), Form("SM_%d_absolute average timing per channel;xtal Average timing (1 clock = 25ns)",fed),500,0,10);
    absTTRMS[dcc-1] = new TH1F(Form("SM_%d_absolute_TT_spread",fed), Form("SM_%d_RMS of the absolute timing in the TT;TT rms timing (1 clock = 25ns) ",fed),200,0,2);
  
    // Relative timing
    relTT[dcc-1]    = new TH1F(Form("SM_%d_relative_TT_timing",fed), Form("SM_%d_relative average timing per TT;TT relative timing (1 clock = 25ns)",fed),500,-5,5);
    relCh[dcc-1]    = new TH1F(Form("SM_%d_relative_Ch_timing",fed), Form("SM_%d_relative average timing per channel;Xtal relative timing (1 clock = 25ns)",fed),500,-5,5);
    relTTRMS[dcc-1] = new TH1F(Form("SM_%d_relative_TT_spread",fed), Form("SM_%d_RMS of the relative timing in the TT ;TT RMS relative timing (1 clock = 25ns)",fed),200,0,2);
  
    //TT Timing
	ttTime[dcc-1] = new TProfile(Form("SM_%d_inside_TT_timing",fed),Form("SM_%d_inside TT timing;xtal in TT;relative time from first xtal (1 clock =25 ns)",fed),26,0.,26.,-1.,1.);
	ttRTime[dcc-1] = new TProfile(Form("SM_%d_inside_TT_Reltiming",fed),Form("SM_%d_inside TT Rel timing;xtal in TT;relative time from first xtal (1 clock =25 ns)",fed),26,0.,26.,-1.,1.);
  
    for(int u=0;u<68;u++){
      mean[u] =0; x2[u]=0; nCry[u]=0;
      RMS[u] = -1;
      meanr[u] =0; x2r[u]=0; nCryr[u]=0;
      RMSr[u] = -1;
	  ttStart[u] = -100;
	  ttRStart[u]=-100;
    }
  
    int TT = 0;
    float absmean=0.;
    float absnum=0.;
	
	bool ebdcc = false;
	if (dcc > 9 && dcc < 46) ebdcc = true;
	
	
    for(int cry=1; cry < numXtals; cry++){
	  double numents = absoluteTimingConv_[dcc-1][0]->GetBinEntries(cry);
      if( numents  > min_num_ev_ ){
	    DetId mydet = (ebdcc ? DetId(EBDetId(dcc-9,cry,EBDetId::SMCRYSTALMODE)) : DetId(EEDetId::unhashIndex(cry)) );
		EcalElectronicsId elecId = ecalElectronicsMap_->getElectronicsId(mydet);
		TT = elecId.towerId();
		int TTi = TT -1;
        if(TTi>=0 && TTi<68){
        float time = absoluteTimingConv_[dcc-1][0]->GetBinContent(cry);
	    float trel = relativeTimingBlueConv_[dcc-1]->GetBinContent(cry);
		if (elecId.stripId() == 1 && elecId.xtalId() == 1 ) {ttStart[TTi]= time;ttRStart[TTi]= trel;}
	      absCh[dcc-1]->Fill(time);
	      //if (TTi>=21 && TTi<53){ 
	        absmean += time*((double)(numents));
	        absnum += numents;
	      //} 
        }     
		else { 
		   std::cout << " What the TTi is " << TTi << " in DCC " << dcc <<  std::cout;
		}
      } 
    }
    if (absnum > 0.) absmean /= absnum; //this puts the mean in a manageable form. 
    std::cout << " absmean " << absmean << " absnum " << absnum << std::endl;
    if (writetxtfiles_) smave_outfile << std::setw(4)<<dcc << " " << std::setw(6)<<std::setprecision(5)<< absmean <<std::endl;
	
    TT = 0;
    for(int cry=1; cry < numXtals; cry++){
	  double numents = absoluteTimingConv_[dcc-1][0]->GetBinEntries(cry);
      if( numents > min_num_ev_ ){
	    DetId mydet = (ebdcc ? DetId(EBDetId(dcc-9,cry,EBDetId::SMCRYSTALMODE)) : DetId(EEDetId::unhashIndex(cry)) );
		EcalElectronicsId elecId = ecalElectronicsMap_->getElectronicsId(mydet);
		TT = elecId.towerId();
        int TTi = TT -1;	
	
        if(TTi>=0 && TTi<68){
	      float time = absoluteTimingConv_[dcc-1][0]->GetBinContent(cry);
	      float trel = relativeTimingBlueConv_[dcc-1]->GetBinContent(cry);
	      mean[TTi] += time;
	      x2[TTi] += time*time;
          ttTime[dcc-1]->Fill((elecId.stripId()-1)*5+elecId.xtalId(),time - ttStart[TTi]);
          ttRTime[dcc-1]->Fill((elecId.stripId()-1)*5+elecId.xtalId(),trel - ttRStart[TTi]);
          fullttTime->Fill((elecId.stripId()-1)*5+elecId.xtalId(),time - ttStart[TTi]);
          fullttRTime->Fill((elecId.stripId()-1)*5+elecId.xtalId(),trel - ttRStart[TTi]);
	      fullLMTime->Fill(ecalElectronicsMap_->getLMNumber(mydet),time);
	      fullLMTimeCorr->Fill(ecalElectronicsMap_->getLMNumber(mydet),time-sMCorr_[dcc-1]);
	      fullSMTime->Fill(fed,time);
	      fullSMTimeCorr->Fill(fed,time-sMCorr_[dcc-1]);
	      ///time -= absmean;
		  time -= allave_;
	      meanr[TTi] += time;
	      relCh[dcc-1]->Fill(time);
	      x2r[TTi] += time*time;
	      nCry[TTi]++;
        }
      }
    }
    for(int u=0;u<68;u++){
      if(nCry[u] > 0){
        mean[u]  = mean[u]/nCry[u];
	meanr[u] = meanr[u]/nCry[u];
        float rms2  = x2[u]/nCry[u] - mean[u]*mean[u];
	float rmsr2 = x2r[u]/nCry[u] - meanr[u]*meanr[u];
        if( rms2 > 0)  {RMS[u]  = sqrt(rms2);}
	if( rmsr2 > 0) {RMSr[u] = sqrt(rmsr2);}
      }
      else{mean[u] = -100.; meanr[u]=-100.;}
      absTT[dcc-1]->Fill(mean[u]);
      absTTRMS[dcc-1]->Fill(RMS[u]);
      relTT[dcc-1]->Fill(meanr[u]);
      relTTRMS[dcc-1]->Fill(RMSr[u]);
      edm::LogInfo("EcalTimingAnalysis")<<"SM " << (dcc+600) << "  " << u+1<<"  "<< mean[u] << "  "<< RMS[u]<<"  " <<meanr[u] << "  "<< RMSr[u];
    }
    
    ofstream txt_outfile;
    if (writetxtfiles_) {
       txt_outfile.open(Form("SM_%d_%s",fed,txtFileName_.c_str()),std::ios::out);
       txt_outfile<< "# Average time of the peak for each TT, (sample units)"<<std::endl;
       txt_outfile<<"#   TT   time of the peak  \t  TT RMS \t rel timing \t TT RMS"<<std::endl;
    }
    for(int i=0;i<68;i++){
      if (writetxtfiles_) {
         txt_outfile <<"   "<<std::setw(4)<<i+1<<" \t "<<std::setw(6)<<std::setprecision(5)<< mean[i] <<" \t "
         <<std::setw(6)<<std::setprecision(5)<<RMS[i]<<" \t "<<std::setw(6)<<std::setprecision(5)<< meanr[i] <<" \t "
         <<std::setw(6)<<std::setprecision(5)<<RMSr[i]<< std::endl;
      }
      ttTiming_[dcc-1]->SetPoint(i,i+1,mean[i]);
      ttTiming_[dcc-1]->SetPointError(i,0.5,RMS[i]);
      ttTimingAll_  ->SetPoint((dcc-1)*68+i,double(fed)+double(i+1)/100.,mean[i]);
      ttTimingAll_  ->SetPointError((dcc-1)*68+i,1./200.,RMS[i]);
      ttTimingRel_[dcc-1]->SetPoint(i,i+1,meanr[i]);
      ttTimingRel_[dcc-1]->SetPointError(i,0.5,RMSr[i]);
      ttTimingAllRel_  ->SetPoint((dcc-1)*68+i,double(fed)+double(i+1)/100.,meanr[i]);
      ttTimingAllRel_  ->SetPointError((dcc-1)*68+i,1./200.,RMSr[i]);
      ttTimingAllSMChng_  ->SetPoint((dcc-1)*68+i,double(fed)+double(i+1)/100.,mean[i]-sMCorr_[dcc-1]);
      ttTimingAllSMChng_  ->SetPointError((dcc-1)*68+i,1./200.,RMS[i]);
    }
   if (writetxtfiles_) txt_outfile.close();
    
    ofstream txt_channels;
    if (writetxtfiles_) {
       txt_channels.open(Form("SM_%d_%s",fed,txtFileForChGroups_.c_str()),std::ios::out);
       for(int i=1;i<numXtals;i++){
         txt_channels <<fed<<"   "<<std::setw(4)<<i<<" \t "<<std::setw(6)<<std::setprecision(5)
                       <<relativeTimingBlueConv_[dcc-1]->GetBinContent(i)<< std::endl;
       }
       txt_channels.close();
    }
    
    //Now I add in a section that looks at the relative timing of the xtals
    for(int cry=1; cry < numXtals; cry++){
      if (relativeTimingBlueConv_[dcc-1]->GetBinEntries(cry) > min_num_ev_){
         aveRelXtalTime_ ->Fill((relativeTimingBlueConv_[dcc-1]->GetBinError(cry))*25.);
         aveRelXtalTimebyDCC_[dcc-1]->Fill((relativeTimingBlueConv_[dcc-1]->GetBinError(cry))*25.);
	 aveRelXtalTimeVsAbsTime_->Fill((relativeTimingBlueConv_[dcc-1]->GetBinError(cry))*25.,absoluteTimingConv_[dcc-1][0]->GetBinContent(cry) );
      }
    }
  
    //End section of special area of making relative plots
   
  }
  if(writetxtfiles_) smave_outfile.close();
  TProfile2D* ttTimingProfile = (TProfile2D*) ttTimingEtaPhi_->Project3DProfile("yx");
  ttTimingProfile->SetTitle("(Phi,Eta,time) for all FEDs (SM,TT binning);i#phi;i#eta");
  ttTimingProfile->SetName("timeTTProfile");
  ttTimingProfile->GetXaxis()->SetNdivisions(-18);
  ttTimingProfile->GetYaxis()->SetNdivisions(2);
  
  TProfile2D* chTimingProfile = (TProfile2D*) chTimingEtaPhi_->Project3DProfile("yx");
  chTimingProfile->SetTitle("(Phi,Eta,time) for all FEDs (SM,ch binning);i#phi;i#eta");
  chTimingProfile->SetName("timeCHProfile");
  chTimingProfile->GetXaxis()->SetNdivisions(-18);
  chTimingProfile->GetYaxis()->SetNdivisions(2);
  TProfile2D* ttTimingProfileRel = (TProfile2D*) ttTimingEtaPhiRel_->Project3DProfile("yx");
  ttTimingProfileRel->SetTitle("(Phi,Eta,time corrected) for all FEDs (SM,TT binning);i#phi;i#eta");
  ttTimingProfileRel->SetName("timeTTProfileRel");
  ttTimingProfileRel->GetXaxis()->SetNdivisions(-18);
  ttTimingProfileRel->GetYaxis()->SetNdivisions(2);
  TProfile2D* chTimingProfileRel = (TProfile2D*) chTimingEtaPhiRel_->Project3DProfile("yx");
  chTimingProfileRel->SetTitle("(Phi,Eta,time corrected) for all FEDs (SM,ch binning);i#phi;i#eta");
  chTimingProfileRel->SetName("timeCHProfileRel");
  chTimingProfileRel->GetXaxis()->SetNdivisions(-18);
  chTimingProfileRel->GetYaxis()->SetNdivisions(2);
  TProfile2D* chTimingProfileEEP = (TProfile2D*) chTimingEtaPhiEEP_->Project3DProfile("yx");
  chTimingProfileEEP->SetTitle("(ix,iy,time) for all EE+ FEDs (SM,ch binning);ix;iy");
  chTimingProfileEEP->SetName("EEPtimeCHProfile");
  chTimingProfileEEP->GetXaxis()->SetNdivisions(-18);
  chTimingProfileEEP->GetYaxis()->SetNdivisions(16);
  TProfile2D* chTimingProfileRelEEP = (TProfile2D*) chTimingEtaPhiRelEEP_->Project3DProfile("yx");
  chTimingProfileRelEEP->SetTitle("(ix,iy,time corrected) for all EE+ FEDs (SM,ch binning);ix;iy");
  chTimingProfileRelEEP->SetName("EEPtimeCHProfileRel");
  chTimingProfileRelEEP->GetXaxis()->SetNdivisions(-18);
  chTimingProfileRelEEP->GetYaxis()->SetNdivisions(16);
  TProfile2D* chTimingProfileEEM = (TProfile2D*) chTimingEtaPhiEEM_->Project3DProfile("yx");
  chTimingProfileEEM->SetTitle("(ix,iy,time) for all EE- FEDs (SM,ch binning);ix;iy");
  chTimingProfileEEM->SetName("EEMtimeCHProfile");
  chTimingProfileEEM->GetXaxis()->SetNdivisions(-18);
  chTimingProfileEEM->GetYaxis()->SetNdivisions(16);
  TProfile2D* chTimingProfileRelEEM = (TProfile2D*) chTimingEtaPhiRelEEM_->Project3DProfile("yx");
  chTimingProfileRelEEM->SetTitle("(ix,iy,time corrected) for all EE- FEDs (SM,ch binning);ix;iy");
  chTimingProfileRelEEM->SetName("EEMtimeCHProfileRel");
  chTimingProfileRelEEM->GetXaxis()->SetNdivisions(-18);
  chTimingProfileRelEEM->GetYaxis()->SetNdivisions(16);
  TProfile2D* ttTimingProfileEEP = (TProfile2D*) ttTimingEtaPhiEEP_->Project3DProfile("yx");
  ttTimingProfileEEP->SetTitle("(ix,iy,time) for all EE+ FEDs (SM,tt binning);ix;iy");
  ttTimingProfileEEP->SetName("EEPtimeTTProfile");
  ttTimingProfileEEP->GetXaxis()->SetNdivisions(-18);
  ttTimingProfileEEP->GetYaxis()->SetNdivisions(16);
  //TProfile2D* ttTimingProfileRelEEP = (TProfile2D*) ttTimingEtaPhiRelEEP_->Project3DProfile("yx");
  //ttTimingProfileRelEEP->SetTitle("(ix,iy,time corrected) for all EE+ FEDs (SM,tt binning);ix;iy");
  //ttTimingProfileRelEEP->SetName("EEPtimeTTProfileRel");
  //ttTimingProfileRelEEP->GetXaxis()->SetNdivisions(-18);
  //ttTimingProfileRelEEP->GetYaxis()->SetNdivisions(16);
  TProfile2D* ttTimingProfileEEM = (TProfile2D*) ttTimingEtaPhiEEM_->Project3DProfile("yx");
  ttTimingProfileEEM->SetTitle("(ix,iy,time) for all EE- FEDs (SM,tt binning);ix;iy");
  ttTimingProfileEEM->SetName("EEMtimeTTProfile");
  ttTimingProfileEEM->GetXaxis()->SetNdivisions(-18);
  ttTimingProfileEEM->GetYaxis()->SetNdivisions(16);
  //TProfile2D* ttTimingProfileRelEEM = (TProfile2D*) ttTimingEtaPhiRelEEM_->Project3DProfile("yx");
  //ttTimingProfileRelEEM->SetTitle("(ix,iy,time corrected) for all EE- FEDs (SM,tt binning);ix;iy");
  //ttTimingProfileRelEEM->SetName("EEMtimeTTProfileRel");
  //ttTimingProfileRelEEM->GetXaxis()->SetNdivisions(-18);
  //ttTimingProfileRelEEM->GetYaxis()->SetNdivisions(16);
  
  TFile f(rootfile_.c_str(),"RECREATE");
  
  for (int dcc=1; dcc<55; ++dcc){
    f.cd();
	int fed = dcc+600;
    TDirectory* hist = gDirectory->mkdir(Form("SM_%d",fed));
    hist->cd();
    for(int l=0; l<4; ++l) {
      if(l==0 || l==4 || l==2){
        amplProfileConv_[dcc-1][l]->Write();
        absoluteTimingConv_[dcc-1][l]->Write();
        //amplProfileAll_[l]->Write();
        absoluteTimingAll_[dcc-1][l]->Write();
        Chi2ProfileConv_[dcc-1][l]->Write();
      }
    }
    relativeTimingBlueConv_[dcc-1]->Write();
    absTT[dcc-1]->Write();absCh[dcc-1]->Write();absTTRMS[dcc-1]->Write();
    relTT[dcc-1]->Write();relCh[dcc-1]->Write();relTTRMS[dcc-1]->Write();
    timeCry2[dcc-1]->Write();
    timeCry1[dcc-1]->Write();
    timeRelCry2[dcc-1]->Write();
    timeRelCry1[dcc-1]->Write();
    ttTiming_[dcc-1]->Write();
    ttTimingRel_[dcc-1]->Write();
    ttTime[dcc-1]->Write();
    ttRTime[dcc-1]->Write();
    aveRelXtalTimebyDCC_[dcc-1]->Write();
    lasershiftLM_[dcc-1]->Write();
    lasershiftVsTime_[dcc-1]->Write();
    lasershiftVsTimehist_[dcc-1]->Write();
  }
  f.cd();
  lasersPerEvt->Write();
  ttTimingAll_->Write();
  ttTimingAllRel_->Write();
  ttTimingAllSMChng_->Write();
  
  ttTimingEtaPhi_->Write();
  chTimingEtaPhi_->Write();
  chTimingEtaPhiEEP_->Write();
  chTimingEtaPhiEEM_->Write();
  ttTimingEtaPhiEEP_->Write();
  ttTimingEtaPhiEEM_->Write();
  ttTimingEtaPhiRel_->Write(); //Taken out as not really needed yet
  chTimingEtaPhiRel_->Write(); //Taken out as not really needed yet
  ttTimingProfile->Write();
  chTimingProfile->Write();
  chTimingProfileEEP->Write();
  chTimingProfileEEM->Write();
  ttTimingProfileEEP->Write();
  ttTimingProfileEEM->Write();
  ttTimingProfileRel->Write();
  chTimingProfileRel->Write();
  chTimingProfileRelEEP->Write();
  chTimingProfileRelEEM->Write();
  chTimingEtaPhiRelEEP_->Write();
  chTimingEtaPhiRelEEM_->Write();
  
  ttTimingEta_->Write();
  chTimingEta_->Write();
  ttTimingEtaRel_->Write();
  chTimingEtaRel_->Write();
  
  ttTimingEtaEEP_->Write();
  ttTimingEtaRelEEP_->Write();
  
  ttTimingEtaEEM_->Write();
  ttTimingEtaRelEEM_->Write();
  
  fullttTime->Write();
  fullttRTime->Write();
  fullLMTime->Write();
  fullLMTimeCorr->Write();
  fullSMTime->Write();
  fullSMTimeCorr->Write();
  
  aveRelXtalTime_->Write();
  lasershift_->Write();
  aveRelXtalTimeVsAbsTime_->Write();
  
  fullAmpProfileEB_->Write();
  fullAmpProfileEEP_->Write();
  fullAmpProfileEEM_->Write();
  
  
  f.Close();

    ofstream new_outfile;
    if (writetxtfiles_) {
       new_outfile.open(Form("ZfromIeta_%s",txtFileName_.c_str()),std::ios::out);
       //new_outfile<< "# Average time of the peak for each TT, (sample units)"<<std::endl;
      // new_outfile<<"#   TT   time of the peak  \t  TT RMS \t rel timing \t TT RMS"<<std::endl;
       std::cout << " did i get here ? " << std::endl;
       for (std::map<int, double>::iterator iter = eta2zmap_.begin(); iter != eta2zmap_.end(); ++iter)
       {
          new_outfile << (*iter).first << "\t" << (*iter).second << std::endl;
          std::cout << " first " << (*iter).first << " second " << (*iter).second << std::endl;
       }
       new_outfile.close();
    }
    

}

//
// member functions
//

//========================================================================
void
EcalTimingAnalysis::analyze(  edm::Event const& iEvent,  edm::EventSetup const& iSetup ) {
//========================================================================
   edm::ESHandle< EcalElectronicsMapping > handle;
   iSetup.get< EcalMappingRcd >().get(handle);
   ecalElectronicsMap_ = handle.product();

   using namespace edm;
   using namespace cms;
   if (fromfile_) return;
   ievt_++;

   //edm::ESHandle< EcalElectronicsMapping > handle;
   //iSetup.get< EcalMappingRcd >().get(handle);
   //ecalElectronicsMap_ = handle.product();


   edm::Handle<EcalRawDataCollection> DCCHeaders;
   iEvent.getByLabel(digiProducer_, DCCHeaders);
   if (!DCCHeaders.isValid()) {
	edm::LogError("EcalTimingAnalysis") << "can't get the product for EcalRawDataCollection";
   }
   
   //Geometry information
   edm::ESHandle<CaloGeometry> geoHandle;
   iSetup.get<CaloGeometryRecord>().get(geoHandle);
   
   const CaloSubdetectorGeometry *geometry_pEB = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
   const CaloSubdetectorGeometry *geometry_pEE = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
   
   
   //Timing information
   unsigned int  timeStampLow = ( 0xFFFFFFFF & iEvent.time().value() );
   unsigned int  timeStampHigh = ( iEvent.time().value() >> 32 );
   double eventtime = ( double)(timeStampHigh)+((double )(timeStampLow)/1000000.) - timerunstart_;
   std::cout << "Event Time " << eventtime << " High " <<timeStampHigh<< " low"<<timeStampLow <<" value " <<iEvent.time().value() << std::endl;
   
   int lambda = -1;
  for ( EcalRawDataCollection::const_iterator headerItr= DCCHeaders->begin();headerItr != DCCHeaders->end(); 
	  ++headerItr ) {
    EcalDCCHeaderBlock::EcalDCCEventSettings settings = headerItr->getEventSettings();
   
    //std::cout << " Lambda 0 " << settings.wavelength  << std::endl; 
    lambda= settings.wavelength;
    LogDebug("EcalTimingAnalysis") << " Lambda " << lambda; //hmm... this isn't good, I should keep a record of the wavelength in the headers as an inactive SM might have a different wavelength for this field and make this not go through.
  }
  ///if(lambda <0 || lambda > 3){LogDebug ("EcalTimingAnalysis")<<"Stopping: Wrong value for laser wavelength: "<<lambda<<std::endl; return;}//TAKEN OUT TO RUN ON TEST PULSES
    lambda=0; ///ADDED TO USE TESTPULSES
   Handle<EcalUncalibratedRecHitCollection> phits;
   float absTime[54][numXtals];
   for(int dcc=1;dcc<55;++dcc){for(int i=0;i<numXtals;i++){absTime[dcc-1][i]=-10;}}
   
   iEvent.getByLabel( hitProducer_, hitCollection_,phits);
   if (!phits.isValid()){
	edm::LogError("EcalTimingAnalysis") << "can't get the product for " << hitCollection_;
   }
   
   // loop over hits
   const EcalUncalibratedRecHitCollection* hits = phits.product(); // get a ptr to the product
   
   if(ievt_%100 ==0){LogInfo("EcalTimingAnalysis") <<"Event: "<<ievt_<< "# of EcalUncalibratedRecHits hits: " << hits->size();}

   //Add the ability to calculate the average for each event, _ONLY_ if desired as this takes time
   double averagetimeEB = 0.0;
   int numberinaveEB = 0;
   if (correctAVE_) {
      for(EcalUncalibratedRecHitCollection::const_iterator ithit = hits->begin(); ithit != hits->end(); ++ithit) {
     
        EBDetId anid(ithit->id()); 
	    EcalElectronicsId elecId = ecalElectronicsMap_->getElectronicsId(anid);
        int DCCid = elecId.dccId();
        int SMind = anid.ic();
     
	    if(ithit->chi2()> -1. && ithit->chi2()<10000. && ithit->amplitude()> ampl_thr_ ) { // make sure fit has converged 
	     double extrajit = timecorr(geometry_pEB,anid);
	     double mytime = ithit->jitter() + extrajit+5.0;
         averagetimeEB += mytime;
		 numberinaveEB++;
	    }  
      }//end of loop over hits
      if (numberinaveEB > 0 ) averagetimeEB /= double (numberinaveEB); 
   }//End EB loop to calculate average

    
   for(EcalUncalibratedRecHitCollection::const_iterator ithit = hits->begin(); ithit != hits->end(); ++ithit) {
     
     EBDetId anid(ithit->id()); 
	 EcalElectronicsId elecId = ecalElectronicsMap_->getElectronicsId(anid);
     int DCCid = elecId.dccId();
     int SMind = anid.ic();
     
     LogInfo("EcalTimingAnalysis")<<"SM " << DCCid+600 <<" SMind " << SMind << " Chi sq " << ithit->chi2() << " ampl " << ithit->amplitude() << " lambda " << lambda << " jitter " << ithit->jitter();
     if (DCCid == 644 || DCCid == 645) std::cout << "SM " << DCCid+600 <<" SMind " << SMind << " Chi sq " << ithit->chi2() << " ampl " << ithit->amplitude() << " lambda " << lambda << " jitter " << ithit->jitter();
	 if(ithit->chi2()> -1. && ithit->chi2()<10000. && ithit->amplitude()> ampl_thr_ ) { // make sure fit has converged 
	  double extrajit = timecorr(geometry_pEB,anid);
	  double mytime = ithit->jitter() + extrajit+5.0;
	  if (correctAVE_) mytime += 5.0 - averagetimeEB;
      fullAmpProfileEB_->Fill(anid.iphi(),anid.ieta(),ithit->amplitude());
      lasersPerEvt->Fill(ievt_);
	  amplProfileConv_[DCCid-1][lambda]->Fill(SMind,ithit->amplitude());
       absoluteTimingConv_[DCCid-1][lambda]->Fill(SMind,mytime);
       Chi2ProfileConv_[DCCid-1][lambda]->Fill(SMind,ithit->chi2());
       if(lambda == 0){
           absTime[DCCid-1][SMind] = mytime;
	   ttTimingEtaPhi_->Fill(anid.iphi(),anid.ieta(),mytime);
	   ttTimingEtaPhiRel_->Fill(anid.iphi(),anid.ieta(),mytime-sMAves_[DCCid-1]+5.0);
	   chTimingEtaPhi_->Fill(anid.iphi(),anid.ieta(),mytime);
	   chTimingEtaPhiRel_->Fill(anid.iphi(),anid.ieta(),mytime-sMAves_[DCCid-1]+5.0);
	   ttTimingEta_->Fill(anid.ieta(),mytime);
	   ttTimingEtaRel_->Fill(anid.ieta(),mytime-sMAves_[DCCid-1]+5.0);
	   chTimingEta_->Fill(anid.ieta(),mytime);
	   chTimingEtaRel_->Fill(anid.ieta(),mytime-sMAves_[DCCid-1]+5.0);  
	}  
       if(SMind == 648  ){timeCry2[DCCid-1]->Fill((ithit->jitter()+5.0)*25.);}
       else if(SMind == 653  ){timeCry1[DCCid-1]->Fill((ithit->jitter()+5.0)*25.);}
     }
     
     amplProfileAll_[DCCid-1][lambda]->Fill(SMind,ithit->amplitude());
     absoluteTimingAll_[DCCid-1][lambda]->Fill(SMind,ithit->jitter()+5.0);

     if(ithit->chi2()<0 && false)
     std::cout << "analytic fit failed! EcalUncalibratedRecHit  id: "
               << EBDetId(ithit->id()) << "\t"
               << "amplitude: " << ithit->amplitude() << ", jitter: " << ithit->jitter()+5.0
               << std::endl;

   }//end of loop over hits
   
   ///START EE STUFF HERE
   Handle<EcalUncalibratedRecHitCollection> phitsEE;
   iEvent.getByLabel( hitProducerEE_, hitCollectionEE_,phitsEE);
   if (!phitsEE.isValid()){
	edm::LogError("EcalTimingAnalysis") << "can't get the product for " << hitCollectionEE_;
   }
   
   // loop over hits
   const EcalUncalibratedRecHitCollection* hitsEE = phitsEE.product(); // get a ptr to the product
   
   if(ievt_%100 ==0){LogInfo("EcalTimingAnalysis") <<"Event: "<<ievt_<< " # of EcalUncalibratedRecHits hits: " << hitsEE->size();}

   //Calcualte the average EE event timing first.
   double averagetimeEE = 0.0;
   int numberinaveEE = 0.0;
   if (correctAVE_) {
      for(EcalUncalibratedRecHitCollection::const_iterator ithit = hitsEE->begin(); ithit != hitsEE->end(); ++ithit) {
     
        EEDetId anid(ithit->id()); 
	    EcalElectronicsId elecId = ecalElectronicsMap_->getElectronicsId(anid);
        int DCCid = elecId.dccId();
	    int SMind = anid.hashedIndex();

	    if(ithit->chi2()> -1. && ithit->chi2()<10000. && ithit->amplitude()> ampl_thr_ ) { // make sure fit has converged 
	     double extrajit = timecorr(geometry_pEE,anid);
	     double mytime = ithit->jitter() + extrajit+5.0;
		 averagetimeEE += mytime;
		 numberinaveEE++;
        }
      }	//end rechit loop
	  if (numberinaveEE > 0 ) averagetimeEE /= double (numberinaveEE); 
    }//end EE averaging section
   
   
   for(EcalUncalibratedRecHitCollection::const_iterator ithit = hitsEE->begin(); ithit != hitsEE->end(); ++ithit) {
     
     EEDetId anid(ithit->id()); 
	 EcalElectronicsId elecId = ecalElectronicsMap_->getElectronicsId(anid);
     int DCCid = elecId.dccId();
	 ////////int FEDid = DCCid+600;
     //int SMind = anid.ic();
	 // int SMind = 100*elecId.towerId()+5*(elecId.stripId()-1)+elecId.xtalId();
       ///int SMind = 25*elecId.towerId()+5*elecId.stripId()+elecId.xtalId();
	   
	   //NEW WAY of indexing this fellow
	 int SMind = anid.hashedIndex();
	   
     
     LogInfo("EcalTimingAnalysis")<<"SM " << DCCid+600 <<" SMind " << SMind << " Chi sq " << ithit->chi2() << " ampl " << ithit->amplitude() << " lambda " << lambda << " jitter " << ithit->jitter();
     
	 if(ithit->chi2()> -1. && ithit->chi2()<10000. && ithit->amplitude()> ampl_thr_ ) { // make sure fit has converged 
	  double extrajit = timecorr(geometry_pEE,anid);
	  double mytime = ithit->jitter() + extrajit+5.0;
	  if (correctAVE_) mytime += 5.0 - averagetimeEE;
      lasersPerEvt->Fill(ievt_);
	  amplProfileConv_[DCCid-1][lambda]->Fill(SMind,ithit->amplitude());
       absoluteTimingConv_[DCCid-1][lambda]->Fill(SMind,mytime);
       Chi2ProfileConv_[DCCid-1][lambda]->Fill(SMind,ithit->chi2());
       if(lambda == 0){
           absTime[DCCid-1][SMind] = mytime;
		   if (anid.zside() == 1) {
		   chTimingEtaPhiEEP_->Fill(anid.ix(),anid.iy(),mytime);
		   chTimingEtaPhiRelEEP_->Fill(anid.ix(),anid.iy(),mytime-sMAves_[DCCid-1]+5.);
		   ttTimingEtaPhiEEP_->Fill(anid.ix(),anid.iy(),mytime);
		   ttTimingEtaPhiRelEEP_->Fill(anid.ix(),anid.iy(),mytime-sMAves_[DCCid-1]+5.);
		   ttTimingEtaEEP_->Fill(pow(anid.ix()*anid.ix()+anid.iy()*anid.iy(),0.5),mytime);
	       ttTimingEtaRelEEP_->Fill(pow(anid.ix()*anid.ix()+anid.iy()*anid.iy(),0.5),mytime-sMAves_[DCCid-1]+5.0);  
	       fullAmpProfileEEP_->Fill(anid.ix(),anid.iy(),ithit->amplitude());
		   }
		   else {
		   chTimingEtaPhiEEM_->Fill(anid.ix(),anid.iy(),mytime);
		   chTimingEtaPhiRelEEM_->Fill(anid.ix(),anid.iy(),mytime-sMAves_[DCCid-1]+5.);
		   ttTimingEtaPhiEEM_->Fill(anid.ix(),anid.iy(),mytime);
		   ttTimingEtaPhiRelEEM_->Fill(anid.ix(),anid.iy(),mytime-sMAves_[DCCid-1]+5.);
		   ttTimingEtaEEM_->Fill(pow(anid.ix()*anid.ix()+anid.iy()*anid.iy(),0.5),mytime);
	       ttTimingEtaRelEEM_->Fill(pow(anid.ix()*anid.ix()+anid.iy()*anid.iy(),0.5),mytime-sMAves_[DCCid-1]+5.0);
	       fullAmpProfileEEM_->Fill(anid.ix(),anid.iy(),ithit->amplitude());
		   }
	   
	   
	}  
       if(SMind == 648  ){timeCry2[DCCid-1]->Fill((ithit->jitter()+5.0)*25.);}
       else if(SMind == 653  ){timeCry1[DCCid-1]->Fill((ithit->jitter()+5.0)*25.);}
     }
     
     amplProfileAll_[DCCid-1][lambda]->Fill(SMind,ithit->amplitude());
     absoluteTimingAll_[DCCid-1][lambda]->Fill(SMind,ithit->jitter()+5.0);

     if(ithit->chi2()<0 && false)
     std::cout << "analytic fit failed! EcalUncalibratedRecHit  id: "
               << EEDetId(ithit->id()) << "\t"
               << "amplitude: " << ithit->amplitude() << ", jitter: " << ithit->jitter()+5.0
               << std::endl;

   }//end of loop over hits
   ///END EE STUFF HERE
   

   //Obsolete old code now 5-14-08 it was used to equalize the different SMs, but only equalized the different sides
   for (int dcc=1;dcc<55;++dcc){
     float ave_time = 0;
     float foundCh = 0;
     for ( int ch= 1; ch<numXtals;ch++){
       if(absTime[dcc-1][ch] >2 && absTime[dcc-1][ch]<8){ ave_time += absTime[dcc-1][ch]; foundCh++;}
     }
     //It may be needed to split this into half-SMs
     //To do so, I will need to know where the divided between halfs falls.
     if ( foundCh > 0 ){
       ave_time  = ave_time / foundCh;
       lasershiftLM_[dcc-1]->Fill((ave_time)*25.);
       lasershift_->Fill((ave_time)*25.);
      // if (eventtime > 10. && ave_time > 1. ) {
       lasershiftVsTime_[dcc-1]->SetPoint(numGoodEvtsPerSM_[dcc-1],eventtime,(ave_time)*25.); 
       lasershiftVsTimehist_[dcc-1]->Fill(eventtime,(ave_time)*25.);
       numGoodEvtsPerSM_[dcc-1]++;
      // }
       for( int ch =0; ch<numXtals; ch++){
         if(absTime[dcc-1][ch]>0) {
            relativeTimingBlueConv_[dcc-1]->Fill( ch, (absTime[dcc-1][ch]- ave_time) ); 
 	        if (ch == 648 ) timeRelCry2[dcc-1]->Fill((absTime[dcc-1][ch]- ave_time)*25.);
	        if (ch == 653 ) timeRelCry1[dcc-1]->Fill((absTime[dcc-1][ch]- ave_time)*25.);
	    
	     }
       }
     }
   }
   
}

double EcalTimingAnalysis::timecorr(const CaloSubdetectorGeometry *geometry_p, DetId id)
{
   double time = 0.0;

   if (!(corrtimeEcal || corrtimeBH) ) { return time;}
    
   bool inEB = true;
   if ((id.det() == DetId::Ecal) && (id.subdetId() == EcalEndcap)) {
      inEB = false;
   }
   
   const CaloCellGeometry *thisCell = geometry_p->getGeometry(id);
   GlobalPoint position = thisCell->getPosition();
   
   double speedlight = 0.299792458; //in meters/ns
   
   
   double z = position.z()/100.;
   //Correct Ecal IP readout time assumption
   if (corrtimeEcal && inEB){
   
      int ieta = (EBDetId(id)).ieta() ;

      eta2zmap_[ieta]=z;
	  double zz=0.0;
	  /*
      if (ieta > 65 )  zz=5.188213395;
	  else if (ieta > 45 )  zz=2.192428069;
	  else if (ieta > 25 )  zz=0.756752107;
	  else if (ieta > 1 ) zz=0.088368264;
	  else if (ieta > -26 )  zz=0.088368264;
	  else if (ieta > -45 )  zz=0.756752107;
	  else if (ieta > -65 ) zz=2.192428069;
	  else zz=5.188213395;
	  */
	  /*
	  if (ieta > 65 )  zz=5.06880196;
	  else if (ieta > 45 )  zz=2.08167184;
	  else if (ieta > 25 )  zz=0.86397025;
	  else if (ieta > 1 ) zz=0.088368264;
	  else if (ieta > -26 )  zz=0.088368264;
	  else if (ieta > -45 )  zz=0.86397025;
	  else if (ieta > -65 ) zz=2.08167184;
	  else zz=5.06880196;
          */
      double change = (pow(EBradius_*EBradius_+z*z,0.5)-EBradius_)/speedlight;
      ///double change = (pow(EBradius_*EBradius_+zz,0.5)-EBradius_)/speedlight;
	  time += change;
	  
	  //std::cout << " Woohoo... z is " << z << " ieta is " << (EBDetId(id)).ieta() << std::endl;
	  //std::cout << " Subtracting " << change << std::endl;
   }
   
   if (corrtimeEcal && !inEB){
      double x = position.x()/100.;
      double y = position.y()/100.;
	  double change = (pow(x*x+y*y+z*z,0.5)-EBradius_)/speedlight;
	  //double change = (pow(z*z,0.5)-EBradius_)/speedlight; //Assuming they are all the same length...
	  time += change; //Take this out for the time being
	  
	  //std::cout << " Woohoo... z is " << z << " ieta is " << (EBDetId(id)).ieta() << std::endl;
	  //std::cout << " Subtracting " << change << std::endl;
   }
   ///speedlight = (0.299792458*(1.0-.08));
   //Correct out the BH or Beam-shot assumption
   if (corrtimeBH){
      time += ((bhplus_) ? (z/speedlight) :  (-z/speedlight) );
	  //std::cout << " Adding " << z/speedlight << std::endl;

   }

   return (time/25.-allshift_);
}


