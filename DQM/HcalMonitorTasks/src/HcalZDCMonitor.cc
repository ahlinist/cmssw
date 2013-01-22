#include "DQM/HcalMonitorTasks/interface/HcalZDCMonitor.h"

// constructor
HcalZDCMonitor::HcalZDCMonitor(const edm::ParameterSet & ps) {
  Online_                = ps.getUntrackedParameter<bool>("online",false);
  mergeRuns_             = ps.getUntrackedParameter<bool>("mergeRuns",false);
  enableCleanup_         = ps.getUntrackedParameter<bool>("enableCleanup",false);
  debug_                 = ps.getUntrackedParameter<int>("debug",0);
  prefixME_=ps.getUntrackedParameter<std::string>("subSystemFolder","Hcal/");
  if (prefixME_.substr(prefixME_.size()-1, prefixME_.size())!="/")
    prefixME_.append("/");
  subdir_=ps.getUntrackedParameter<std::string>("TaskFolder","ZDCMonitor_Hcal/");
  if (subdir_.size()>0 && subdir_.substr(subdir_.size()-1,subdir_.size())!="/")
    subdir_.append("/");
  subdir_=prefixME_+subdir_;

  rechitLabel_=ps.getParameter<edm::InputTag>("rechitLabel");
  digiLabel_=ps.getParameter<edm::InputTag>("digiLabel");
  AllowedCalibTypes_     = ps.getUntrackedParameter<std::vector<int> > ("AllowedCalibTypes");
  skipOutOfOrderLS_      = ps.getUntrackedParameter<bool>("skipOutOfOrderLS",true);
  NLumiBlocks_           = ps.getUntrackedParameter<int>("NLumiBlocks",4000);
  makeDiagnostics_       = ps.getUntrackedParameter<bool>("makeDiagnostics",false);
  OnlineColdThreshold_   = ps.getUntrackedParameter<int>("ZDC_OnlineColdThreshold");
  OnlineDeadThreshold_   = ps.getUntrackedParameter<int>("ZDC_OnlineDeadThreshold");
  OfflineColdThreshold_  = ps.getUntrackedParameter<int>("ZDC_OfflineColdThreshold");
  OfflineDeadThreshold_  = ps.getUntrackedParameter<int>("ZDC_OfflineDeadThreshold");
  ColdADCThreshold_      = ps.getUntrackedParameter<int>("ZDC_ColdADCThreshold");
  ChannelWeighting_      = ps.getUntrackedParameter<std::vector<double>> ("ZDC_ChannelWeighting");
  MaxErrorRates_         = ps.getUntrackedParameter<std::vector<double>> ("ZDC_AcceptableChannelErrorRates");
}

//destructor
HcalZDCMonitor::~HcalZDCMonitor() {
}

void HcalZDCMonitor::reset() {
}


void HcalZDCMonitor::setup() {
  debug_=0;

  HcalBaseDQMonitor::setup(); 


    // Set initial event # to 0
    ievt_ = 0;

     //Histograms
    if (dbe_) {
         dbe_->setCurrentFolder(subdir_);
	 meEVT_ = dbe_->bookInt("ZDC Event Number");
	 meEVT_->Fill(ievt_);
	 char name[128];
	 char title[128];
 
	 // Add histograms of your choice


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////// Quality of the P/N ZDC /////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
	 dbe_->setCurrentFolder(subdir_);
	 PZDC_QualityIndexVsLB_ = dbe_->book1D("PZDC_QualityIndexVSLB","Quality Index for the ZDC+ vs LS; LS; Quality Index", NLumiBlocks_,0,NLumiBlocks_);
    	 NZDC_QualityIndexVsLB_ = dbe_->book1D("NZDC_QualityIndexVSLB","Quality Index for the ZDC- vs  LS; LS; Quality Index", NLumiBlocks_,0,NLumiBlocks_);
	 EventsVsLS             = dbe_->book1D("EventsVsLS", "Total Number of Events per LS; LS; # of Events", NLumiBlocks_,0,NLumiBlocks_);
        


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////     DIGI ERROR PLOTS     ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////

	dbe_->setCurrentFolder(subdir_ + "Errors/Digis");
        ZDC_Digi_Errors = dbe_->book2D("ZDC_Digi_Errors", "Raw Number of Digi Errors Per ZDC Channel", 2, 0, 2, 9, 0, 9);
        ZDC_Digi_Errors->setBinLabel(1,"ZDC+",1);
        ZDC_Digi_Errors->setBinLabel(2,"ZDC-",1);
	ZDC_Digi_Errors->setBinLabel(1,"EM1",2);
        ZDC_Digi_Errors->setBinLabel(2,"EM2",2);
        ZDC_Digi_Errors->setBinLabel(3,"EM3",2);
        ZDC_Digi_Errors->setBinLabel(4,"EM4",2);
        ZDC_Digi_Errors->setBinLabel(5,"EM5",2);
        ZDC_Digi_Errors->setBinLabel(6,"HAD1",2);
        ZDC_Digi_Errors->setBinLabel(7,"HAD2",2);
        ZDC_Digi_Errors->setBinLabel(8,"HAD3",2);
        ZDC_Digi_Errors->setBinLabel(9,"HAD4",2);
        ZDC_Digi_Errors->getTH2F()->SetOption("coltext");


	ZDC_DigiErrorsVsLS=dbe_->book1D("ZDC_DigiErrorsVsLS","Total Number of (Digi) Errors found in the ZDCs vs. Lumi Section;LS;# errors",NLumiBlocks_,0,NLumiBlocks_);

   


	dbe_->setCurrentFolder(subdir_+"Errors/Digis/DigiErrorCauses");
	ZDC_DigiErrors_DVER = dbe_->book2D("ZDC_DigiErrors_DVER","Raw Number of Digi Errors Caused by Finding .dv()=0 or .er()=1",2,0,2,9,0,9);
	ZDC_DigiErrors_DVER->setBinLabel(1,"ZDC+",1);
        ZDC_DigiErrors_DVER->setBinLabel(2,"ZDC-",1);
	ZDC_DigiErrors_DVER->setBinLabel(1,"EM1",2);
        ZDC_DigiErrors_DVER->setBinLabel(2,"EM2",2);
        ZDC_DigiErrors_DVER->setBinLabel(3,"EM3",2);
        ZDC_DigiErrors_DVER->setBinLabel(4,"EM4",2);
        ZDC_DigiErrors_DVER->setBinLabel(5,"EM5",2);
        ZDC_DigiErrors_DVER->setBinLabel(6,"HAD1",2);
        ZDC_DigiErrors_DVER->setBinLabel(7,"HAD2",2);
        ZDC_DigiErrors_DVER->setBinLabel(8,"HAD3",2);
        ZDC_DigiErrors_DVER->setBinLabel(9,"HAD4",2);
        ZDC_DigiErrors_DVER->getTH2F()->SetOption("coltext");


	ZDC_DigiErrors_CAPID = dbe_->book2D("ZDC_DigiErrors_CAPID","Raw Number of Digi Errors Caused by the Caps not Alternating",2,0,2,9,0,9);
        ZDC_DigiErrors_CAPID->setBinLabel(1,"ZDC+",1);
        ZDC_DigiErrors_CAPID->setBinLabel(2,"ZDC-",1);
	ZDC_DigiErrors_CAPID->setBinLabel(1,"EM1",2);
        ZDC_DigiErrors_CAPID->setBinLabel(2,"EM2",2);
        ZDC_DigiErrors_CAPID->setBinLabel(3,"EM3",2);
        ZDC_DigiErrors_CAPID->setBinLabel(4,"EM4",2);
        ZDC_DigiErrors_CAPID->setBinLabel(5,"EM5",2);
        ZDC_DigiErrors_CAPID->setBinLabel(6,"HAD1",2);
        ZDC_DigiErrors_CAPID->setBinLabel(7,"HAD2",2);
        ZDC_DigiErrors_CAPID->setBinLabel(8,"HAD3",2);
        ZDC_DigiErrors_CAPID->setBinLabel(9,"HAD4",2);
        ZDC_DigiErrors_CAPID->getTH2F()->SetOption("coltext");

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////     Hot Channel PLOTS     ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////

     
	dbe_->setCurrentFolder(subdir_ + "Errors/HotChannel");
        ZDC_Hot_Channel_Errors = dbe_->book2D("ZDC_Hot_Channel_Errors", "Raw Number of Times Each Channel Appeared Hot", 2, 0, 2, 9, 0, 9);
        ZDC_Hot_Channel_Errors->setBinLabel(1,"ZDC+",1);
        ZDC_Hot_Channel_Errors->setBinLabel(2,"ZDC-",1);
        ZDC_Hot_Channel_Errors->setBinLabel(1,"EM1",2);
        ZDC_Hot_Channel_Errors->setBinLabel(2,"EM2",2);
        ZDC_Hot_Channel_Errors->setBinLabel(3,"EM3",2);
        ZDC_Hot_Channel_Errors->setBinLabel(4,"EM4",2);
        ZDC_Hot_Channel_Errors->setBinLabel(5,"EM5",2);
        ZDC_Hot_Channel_Errors->setBinLabel(6,"HAD1",2);
        ZDC_Hot_Channel_Errors->setBinLabel(7,"HAD2",2);
        ZDC_Hot_Channel_Errors->setBinLabel(8,"HAD3",2);
        ZDC_Hot_Channel_Errors->setBinLabel(9,"HAD4",2);    
        ZDC_Hot_Channel_Errors->getTH2F()->SetOption("coltext");


	 ZDC_HotChannelErrorsVsLS=dbe_->book1D("ZDC_HotChannelErrorsVsLS","Total Number of Hot Channel Errors in the ZDCs vs. Lumi Section; LS; # Hot channels", NLumiBlocks_,0,NLumiBlocks_);



	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////     Dead Channel PLOTS     ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

	dbe_->setCurrentFolder(subdir_ + "Errors/DeadChannel");
        ZDC_Dead_Channel_Errors = dbe_->book2D("ZDC_Dead_Channel_Errors", "Raw Number of Times Each Channel Appeared Dead", 2, 0, 2, 9, 0, 9);
        ZDC_Dead_Channel_Errors->setBinLabel(1,"ZDC+",1);
        ZDC_Dead_Channel_Errors->setBinLabel(2,"ZDC-",1);
        ZDC_Dead_Channel_Errors->setBinLabel(1,"EM1",2);
        ZDC_Dead_Channel_Errors->setBinLabel(2,"EM2",2);
        ZDC_Dead_Channel_Errors->setBinLabel(3,"EM3",2);
        ZDC_Dead_Channel_Errors->setBinLabel(4,"EM4",2);
        ZDC_Dead_Channel_Errors->setBinLabel(5,"EM5",2);
        ZDC_Dead_Channel_Errors->setBinLabel(6,"HAD1",2);
        ZDC_Dead_Channel_Errors->setBinLabel(7,"HAD2",2);
        ZDC_Dead_Channel_Errors->setBinLabel(8,"HAD3",2);
        ZDC_Dead_Channel_Errors->setBinLabel(9,"HAD4",2);    
        ZDC_Dead_Channel_Errors->getTH2F()->SetOption("coltext");
        
	ZDC_DeadChannelErrorsVsLS=dbe_->book1D("ZDC_DeadChannelErrorsVsLS","Total Number of Dead Channel Errors in the ZDC vs. Lumi Section; LS; # of Dead Chanels", NLumiBlocks_, 0, NLumiBlocks_);  


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////     Cold Channel PLOTS     ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

	dbe_->setCurrentFolder(subdir_ + "Errors/ColdChannel");
        ZDC_Cold_Channel_Errors = dbe_->book2D("ZDC_Cold_Channel_Errors", "Raw Number of Times Each Channel Appeared Cold", 2, 0, 2, 9, 0, 9);
        ZDC_Cold_Channel_Errors->setBinLabel(1,"ZDC+",1);
        ZDC_Cold_Channel_Errors->setBinLabel(2,"ZDC-",1);
        ZDC_Cold_Channel_Errors->setBinLabel(1,"EM1",2);
        ZDC_Cold_Channel_Errors->setBinLabel(2,"EM2",2);
        ZDC_Cold_Channel_Errors->setBinLabel(3,"EM3",2);
        ZDC_Cold_Channel_Errors->setBinLabel(4,"EM4",2);
        ZDC_Cold_Channel_Errors->setBinLabel(5,"EM5",2);
        ZDC_Cold_Channel_Errors->setBinLabel(6,"HAD1",2);
        ZDC_Cold_Channel_Errors->setBinLabel(7,"HAD2",2);
        ZDC_Cold_Channel_Errors->setBinLabel(8,"HAD3",2);
        ZDC_Cold_Channel_Errors->setBinLabel(9,"HAD4",2);    
        ZDC_Cold_Channel_Errors->getTH2F()->SetOption("coltext");
        
	ZDC_ColdChannelErrorsVsLS=dbe_->book1D("ZDC_ColdChannelErrorsVsLS","Total Number of Cold Channels in the ZDC vs. Lumi Section; LS; # of Cold Chanels", NLumiBlocks_, 0, NLumiBlocks_);  




	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////      Other Error PLOTS     ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////


	dbe_->setCurrentFolder(subdir_ + "Errors");
        ZDC_TotalChannelErrors = dbe_->book2D("ZDC_TotalChannelErrors","Total Number of Errors(Digi Error, Hot Cell or Dead Cell) Per Channel in the ZDC" ,2,0,2,9,0,9);
        ZDC_TotalChannelErrors->setBinLabel(1,"ZDC+",1);
        ZDC_TotalChannelErrors->setBinLabel(2,"ZDC-",1);
        ZDC_TotalChannelErrors->setBinLabel(1,"EM1",2);
        ZDC_TotalChannelErrors->setBinLabel(2,"EM2",2);
        ZDC_TotalChannelErrors->setBinLabel(3,"EM3",2);
        ZDC_TotalChannelErrors->setBinLabel(4,"EM4",2);
        ZDC_TotalChannelErrors->setBinLabel(5,"EM5",2);
        ZDC_TotalChannelErrors->setBinLabel(6,"HAD1",2);
        ZDC_TotalChannelErrors->setBinLabel(7,"HAD2",2);
        ZDC_TotalChannelErrors->setBinLabel(8,"HAD3",2);
        ZDC_TotalChannelErrors->setBinLabel(9,"HAD4",2);
        ZDC_TotalChannelErrors->getTH2F()->SetOption("coltext");
        
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////      Sercan or Jeff PLOTS     ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////

	 dbe_->setCurrentFolder(subdir_);
	 h_2D_charge = dbe_->book2D("2D_DigiCharge", "Digi Charge (fC)", 2, 0, 2, 9, 0, 9);
	 h_2D_charge->setBinLabel(1,"ZDC+",1);
	 h_2D_charge->setBinLabel(2,"ZDC-",1);
	 h_2D_charge->setBinLabel(1,"EM1",2);
	 h_2D_charge->setBinLabel(2,"EM2",2);
	 h_2D_charge->setBinLabel(3,"EM3",2);
	 h_2D_charge->setBinLabel(4,"EM4",2);
	 h_2D_charge->setBinLabel(5,"EM5",2);
	 h_2D_charge->setBinLabel(6,"HAD1",2);
	 h_2D_charge->setBinLabel(7,"HAD2",2);
	 h_2D_charge->setBinLabel(8,"HAD3",2);
	 h_2D_charge->setBinLabel(9,"HAD4",2);
	 
	 h_2D_TSMean = dbe_->book2D("2D_DigiTiming", "Digi Timing", 2, 0, 2, 9, 0, 9);
	 h_2D_TSMean->setBinLabel(1,"ZDC+",1);
	 h_2D_TSMean->setBinLabel(2,"ZDC-",1);
	 h_2D_TSMean->setBinLabel(1,"EM1",2);
	 h_2D_TSMean->setBinLabel(2,"EM2",2);
	 h_2D_TSMean->setBinLabel(3,"EM3",2);
	 h_2D_TSMean->setBinLabel(4,"EM4",2);
	 h_2D_TSMean->setBinLabel(5,"EM5",2);
	 h_2D_TSMean->setBinLabel(6,"HAD1",2);
	 h_2D_TSMean->setBinLabel(7,"HAD2",2);
	 h_2D_TSMean->setBinLabel(8,"HAD3",2);
	 h_2D_TSMean->setBinLabel(9,"HAD4",2);
	 
	 h_2D_RecHitEnergy = dbe_->book2D("2D_RecHitEnergy", "Rechit Energy", 2, 0, 2, 9, 0, 9);
	 h_2D_RecHitEnergy->setBinLabel(1,"ZDC+",1);
	 h_2D_RecHitEnergy->setBinLabel(2,"ZDC-",1);
	 h_2D_RecHitEnergy->setBinLabel(1,"EM1",2);
	 h_2D_RecHitEnergy->setBinLabel(2,"EM2",2);
	 h_2D_RecHitEnergy->setBinLabel(3,"EM3",2);
	 h_2D_RecHitEnergy->setBinLabel(4,"EM4",2);
	 h_2D_RecHitEnergy->setBinLabel(5,"EM5",2);
	 h_2D_RecHitEnergy->setBinLabel(6,"HAD1",2);
	 h_2D_RecHitEnergy->setBinLabel(7,"HAD2",2);
	 h_2D_RecHitEnergy->setBinLabel(8,"HAD3",2);
	 h_2D_RecHitEnergy->setBinLabel(9,"HAD4",2);
	 
	 h_2D_RecHitTime = dbe_->book2D("2D_RecHitTime", "Rechit Timing", 2, 0, 2, 9, 0, 9);
	 h_2D_RecHitTime->setBinLabel(1,"ZDC+",1);
	 h_2D_RecHitTime->setBinLabel(2,"ZDC-",1);
	 h_2D_RecHitTime->setBinLabel(1,"EM1",2);
	 h_2D_RecHitTime->setBinLabel(2,"EM2",2);
	 h_2D_RecHitTime->setBinLabel(3,"EM3",2);
	 h_2D_RecHitTime->setBinLabel(4,"EM4",2);
	 h_2D_RecHitTime->setBinLabel(5,"EM5",2);
	 h_2D_RecHitTime->setBinLabel(6,"HAD1",2);
	 h_2D_RecHitTime->setBinLabel(7,"HAD2",2);
	 h_2D_RecHitTime->setBinLabel(8,"HAD3",2);
	 h_2D_RecHitTime->setBinLabel(9,"HAD4",2);
	 
	 h_2D_saturation = dbe_->book2D("h_2D_QIE", "Saturation Check", 2, 0, 2, 9, 0, 9);
	 h_2D_saturation->setBinLabel(1,"ZDC+",1);
	 h_2D_saturation->setBinLabel(2,"ZDC-",1);
	 h_2D_saturation->setBinLabel(1,"EM1",2);
	 h_2D_saturation->setBinLabel(2,"EM2",2);
	 h_2D_saturation->setBinLabel(3,"EM3",2);
	 h_2D_saturation->setBinLabel(4,"EM4",2);
	 h_2D_saturation->setBinLabel(5,"EM5",2);
	 h_2D_saturation->setBinLabel(6,"HAD1",2);
	 h_2D_saturation->setBinLabel(7,"HAD2",2);
	 h_2D_saturation->setBinLabel(8,"HAD3",2);
	 h_2D_saturation->setBinLabel(9,"HAD4",2);
	 
	 dbe_->setCurrentFolder(subdir_ + "Digis");

        for (int i = 0; i < 5; ++i) {
            // pulse Plus Side 
            sprintf(title, "h_ZDCP_EMChan_%i_Pulse", i + 1);
            sprintf(name, "ZDC Plus EM Section Pulse for channel %i", i + 1);
            h_ZDCP_EM_Pulse[i]= dbe_->book1D(title, name, 10, -0.5, 9.5);
	    h_ZDCP_EM_Pulse[i]->setAxisTitle("Time Slice id",1);
	    h_ZDCP_EM_Pulse[i]->setAxisTitle("Pulse Height",2);
            // pulse Minus Side
            sprintf(title, "h_ZDCM_EMChan_%i_Pulse", i + 1);
            sprintf(name, "ZDC Minus EM Section Pulse for channel %i", i + 1);
            h_ZDCM_EM_Pulse[i] = dbe_->book1D(title, name, 10, -0.5, 9.5);
	    h_ZDCM_EM_Pulse[i]->setAxisTitle("Time Slice id",1);
	    h_ZDCM_EM_Pulse[i]->setAxisTitle("Pulse Height",2);
            // integrated charge over 10 time samples
            sprintf(title, "h_ZDCP_EMChan_%i_Charge", i + 1);
            sprintf(name, "ZDC Plus EM Section Charge for channel %i", i + 1);
            h_ZDCP_EM_Charge[i] = dbe_->book1D(title, name, 1000, 0., 30000.);
	    h_ZDCP_EM_Charge[i]->setAxisTitle("Charge (fC)",1);
	    h_ZDCP_EM_Charge[i]->setAxisTitle("Events",2);
            // integrated charge over 10 time samples
            sprintf(title, "h_ZDCM_EMChan_%i_Charge", i + 1);
            sprintf(name, "ZDC Minus EM Section Charge for channel %i", i + 1);
            h_ZDCM_EM_Charge[i] = dbe_->book1D(title, name, 1000, 0., 30000.);
	    h_ZDCM_EM_Charge[i]->setAxisTitle("Charge (fC)",1);
	    h_ZDCM_EM_Charge[i]->setAxisTitle("Events",2);
            // charge weighted time slice
            sprintf(title, "h_ZDCP_EMChan_%i_TSMean", i + 1);
            sprintf(name, "ZDC Plus EM Section TSMean for channel %i", i + 1);
            h_ZDCP_EM_TSMean[i] = dbe_->book1D(title, name, 100, -0.5, 9.5);
	    h_ZDCP_EM_TSMean[i]->setAxisTitle("Timing",1);
	    h_ZDCP_EM_TSMean[i]->setAxisTitle("Events",2);
            // charge weighted time slice
            sprintf(title, "h_ZDCM_EMChan_%i_TSMean", i + 1);
            sprintf(name, "ZDC Minus EM Section TSMean for channel %i", i + 1);
            h_ZDCM_EM_TSMean[i] = dbe_->book1D(title, name, 100, -0.5, 9.5);
	    h_ZDCM_EM_TSMean[i]->setAxisTitle("Timing",1);
	    h_ZDCM_EM_TSMean[i]->setAxisTitle("Events",2);
        }

        for (int i = 0; i < 4; ++i) {
            // pulse Plus Side 
            sprintf(title, "h_ZDCP_HADChan_%i_Pulse", i + 1);
            sprintf(name, "ZDC Plus HAD Section Pulse for channel %i", i + 1);
            h_ZDCP_HAD_Pulse[i] = dbe_->book1D(title, name, 10, -0.5, 9.5);
	    h_ZDCP_HAD_Pulse[i]->setAxisTitle("Time Slice id",1);
	    h_ZDCP_HAD_Pulse[i]->setAxisTitle("Pulse Height",2);
            // pulse Minus Side 
            sprintf(title, "h_ZDCM_HADChan_%i_Pulse", i + 1);
            sprintf(name, "ZDC Minus HAD Section Pulse for channel %i", i + 1);
            h_ZDCM_HAD_Pulse[i] = dbe_->book1D(title, name, 10, -0.5, 9.5);
	    h_ZDCP_HAD_Pulse[i]->setAxisTitle("Time Slice id",1);
	    h_ZDCP_HAD_Pulse[i]->setAxisTitle("Pulse Height",2);
            // integrated charge over 10 time samples 
            sprintf(title, "h_ZDCP_HADChan_%i_Charge", i + 1);
            sprintf(name, "ZDC Plus HAD Section Charge for channel %i", i + 1);
            h_ZDCP_HAD_Charge[i] = dbe_->book1D(title, name, 1000, 0., 30000.);
	    h_ZDCP_HAD_Charge[i]->setAxisTitle("Charge (fC)",1);
	    h_ZDCP_HAD_Charge[i]->setAxisTitle("Events",2);
            // integrated charge over 10 time samples 
            sprintf(title, "h_ZDCM_HADChan_%i_Charge", i + 1);
            sprintf(name, "ZDC Minus HAD Section Charge for channel %i", i + 1);
            h_ZDCM_HAD_Charge[i] = dbe_->book1D(title, name, 1000, 0., 30000.);
	    h_ZDCM_HAD_Charge[i]->setAxisTitle("Charge (fC)",1);
	    h_ZDCM_HAD_Charge[i]->setAxisTitle("Events",2);
            // charge weighted time slice 
            sprintf(title, "h_ZDCP_HADChan_%i_TSMean", i + 1);
            sprintf(name, "ZDC Plus HAD Section TSMean for channel %i", i + 1);
            h_ZDCP_HAD_TSMean[i] = dbe_->book1D(title, name, 100, -0.5, 9.5);
	    h_ZDCP_HAD_TSMean[i]->setAxisTitle("Timing",1);
	    h_ZDCP_HAD_TSMean[i]->setAxisTitle("Events",2);
            // charge weighted time slice 
            sprintf(title, "h_ZDCM_HADChan_%i_TSMean", i + 1);
            sprintf(name, "ZDC Minus HAD Section TSMean for channel %i", i + 1);
            h_ZDCM_HAD_TSMean[i] = dbe_->book1D(title, name, 100, -0.5, 9.5);
	    h_ZDCM_HAD_TSMean[i]->setAxisTitle("Timing",1);
	    h_ZDCM_HAD_TSMean[i]->setAxisTitle("Events",2);
        }

        dbe_->setCurrentFolder(subdir_ + "RecHits");
        for (int i = 0; i < 5; ++i) {
	    //RecHitEnergy Plus Side
            sprintf(title,"h_ZDCP_EMChan_%i_RecHit_Energy",i+1);
	    sprintf(name,"ZDC EM Section Rechit Energy for channel %i",i+1);
	    h_ZDCP_EM_RecHitEnergy[i] = dbe_->book1D(title, name, 1010, -100., 10000.);
	    h_ZDCP_EM_RecHitEnergy[i]->setAxisTitle("Energy (GeV)",1);
	    h_ZDCP_EM_RecHitEnergy[i]->setAxisTitle("Events",2);
	    //RecHitEnergy Minus Side
	    sprintf(title,"h_ZDCM_EMChan_%i_RecHit_Energy",i+1);
	    sprintf(name,"ZDC EM Section Rechit Energy for channel %i",i+1);
	    h_ZDCM_EM_RecHitEnergy[i] = dbe_->book1D(title, name, 1010, -100., 10000.);
	    h_ZDCM_EM_RecHitEnergy[i]->setAxisTitle("Energy (GeV)",1);
	    h_ZDCM_EM_RecHitEnergy[i]->setAxisTitle("Events",2);
	    //RecHit Timing Plus Side 
	    sprintf(title,"h_ZDCP_EMChan_%i_RecHit_Timing",i+1);
	    sprintf(name,"ZDC EM Section Rechit Timing for channel %i",i+1);
	    h_ZDCP_EM_RecHitTiming[i] = dbe_->book1D(title, name, 100, -100., 100.);
	    h_ZDCP_EM_RecHitTiming[i]->setAxisTitle("RecHit Time",1);
	    h_ZDCP_EM_RecHitTiming[i]->setAxisTitle("Events",2);
	    //RecHit Timing Minus Side 
	    sprintf(title,"h_ZDCM_EMChan_%i_RecHit_Timing",i+1);
	    sprintf(name,"ZDC EM Section Rechit Timing for channel %i",i+1);
	    h_ZDCM_EM_RecHitTiming[i] = dbe_->book1D(title, name, 100, -100., 100.);	
	    h_ZDCM_EM_RecHitTiming[i]->setAxisTitle("RecHit Time",1);
	    h_ZDCM_EM_RecHitTiming[i]->setAxisTitle("Events",2);
	}

        for (int i = 0; i < 4; ++i) {
	    //RecHitEnergy Plus Side
	    sprintf(title,"h_ZDCP_HADChan_%i_RecHit_Energy",i+1);
	    sprintf(name,"ZDC HAD Section Rechit Energy for channel %i",i+1);
	    h_ZDCP_HAD_RecHitEnergy[i] = dbe_->book1D(title, name, 1010, -100., 10000.);
	    h_ZDCP_HAD_RecHitEnergy[i]->setAxisTitle("Energy (GeV)",1);
	    h_ZDCP_HAD_RecHitEnergy[i]->setAxisTitle("Events",2);
	    //RecHitEnergy Minus Side
	    sprintf(title,"h_ZDCM_HADChan_%i_RecHit_Energy",i+1);
	    sprintf(name,"ZDC HAD Section Rechit Energy for channel %i",i+1);
	    h_ZDCM_HAD_RecHitEnergy[i] = dbe_->book1D(title, name, 1010, -100., 10000.);
	    h_ZDCM_HAD_RecHitEnergy[i]->setAxisTitle("Energy (GeV)",1);
	    h_ZDCM_HAD_RecHitEnergy[i]->setAxisTitle("Events",2);
	    //RecHit Timing Plus Side 
	    sprintf(title,"h_ZDCP_HADChan_%i_RecHit_Timing",i+1);
	    sprintf(name,"ZDC HAD Section Rechit Timing for channel %i",i+1);
	    h_ZDCP_HAD_RecHitTiming[i] = dbe_->book1D(title, name, 100, -100., 100.);	
	    h_ZDCP_HAD_RecHitTiming[i]->setAxisTitle("RecHit Time",1);
	    h_ZDCP_HAD_RecHitTiming[i]->setAxisTitle("Events",2);
	    //RecHit Timing Minus Side 
	    sprintf(title,"h_ZDCM_HADChan_%i_RecHit_Timing",i+1);
	    sprintf(name,"ZDC HAD Section Rechit Timing for channel %i",i+1);
	    h_ZDCM_HAD_RecHitTiming[i] = dbe_->book1D(title, name, 100, -100., 100.);	
	    h_ZDCM_HAD_RecHitTiming[i]->setAxisTitle("RecHit Time",1);
	    h_ZDCM_HAD_RecHitTiming[i]->setAxisTitle("Events",2);
	}
    } // if (dbe_)
    return;
}


void HcalZDCMonitor::beginRun(const edm::Run& run, const edm::EventSetup& c)
{
  HcalBaseDQMonitor::beginRun(run, c);
  if (tevt_==0) this->setup();  // call basic setup of histograms

  for (int i=0;i<18;++i)
    {
      ColdChannelCounter[i]=0;
      DeadChannelCounter[i]=0;
    }
  
  EventCounter=0;

}

void HcalZDCMonitor::endRun(const edm::Run& run, const edm::EventSetup& c)
{
  HcalBaseDQMonitor::endRun(run, c);
}

void HcalZDCMonitor::analyze(const edm::Event& e, const edm::EventSetup& c)
{
  if (!IsAllowedCalibType()) return;
  if (LumiInOrder(e.luminosityBlock())==false) return;
  HcalBaseDQMonitor::analyze(e,c);

  edm::Handle<ZDCDigiCollection> zdcdigis;
  e.getByLabel(digiLabel_,zdcdigis);
  edm::Handle<ZDCRecHitCollection> zdcrechits;
  e.getByLabel(rechitLabel_,zdcrechits);
  edm::Handle<HcalUnpackerReport> report;
  e.getByLabel(digiLabel_,report);
  processEvent(*zdcdigis, *zdcrechits, *report); // run the histogramming/analysis specific to this code
}

void HcalZDCMonitor::beginLuminosityBlock(const edm::LuminosityBlock& e, const edm::EventSetup& c)
{
  HcalBaseDQMonitor::beginLuminosityBlock(e,c);
  // ZDC_Dead_Channel_Errors->Fill(-1,-1,1);
  //  ZDC_Cold_Channel_Errors->Fill(-1,-1,1); READ COMMENT!!!!
  // The above line should be uncommented IF, in the fill (down in endlumiblock) we used a value of 1 to say that a specific ZDC channel had 1 Cold Error after >50 times that the cold error occured. Currently, I have it set so that if a ZDC channel goes over the threshold, the RAW number of times that the channel appeared cold shows up. For example: the cold threshold is at 50, EM 1 has a cold Error 51 times...currently, the number 51 will show up in the ZDC_Cold_Channel_Errors plot...however the other way....it would say 1. 
  //This is only a problem when you get to the client level since this line fills the underflow bin. The underflow bin is read in by the client as the divisor. If you try to divide a number of ColdEvents by a number of Lumis...most likely your rate will be greater than one. For now I am moving this line back to processEvent.  
  EventCounter=0;
  for (int i=0;i<18;++i)
    {
      ChannelRatio[i]=0.;
      TotalChannelErrors[i]=0;
    }
  
}



void HcalZDCMonitor::processEvent(const ZDCDigiCollection& digi, const ZDCRecHitCollection& rechit, const HcalUnpackerReport& report) {

    meEVT_->Fill(ievt_);

    //--------------------------------------
    // ZDC Digi part 
    //--------------------------------------
    double fSum = 0.;
    std::vector<double> fData;
    double digiThresh = 99.5; //corresponds to 40 ADC counts
    //int digiThreshADC = 40;
    int digiSaturation = 127;
    //double ZDCQIEConst = 2.6; 


    if (digi.size()>0)
	{
	  ZDC_Digi_Errors->Fill(-1,-1,1);
	  ZDC_Hot_Channel_Errors->Fill(-1,-1,1);
	  ZDC_TotalChannelErrors->Fill(-1,-1,1);
	  ZDC_Cold_Channel_Errors->Fill(-1,-1,1);
	  ZDC_Dead_Channel_Errors->Fill(-1,-1,1);
	  EventCounter+=1;
	}
    for (int i=0;i<18;++i)
      {
	ChannelHasDigiError[i]=false;
	DigiErrorDVER[i]=false;
	DigiErrorCAPID[i]=false;
	HotChannelError[i]=false;
	DeadChannelError[i]=true;
      }
    
    
    typedef std::vector<DetId> DetIdVector;
    
    for (DetIdVector::const_iterator baddigi_iter=report.bad_quality_begin();
	 baddigi_iter != report.bad_quality_end();
	 ++baddigi_iter)
      {
	DetId id(baddigi_iter->rawId());
	if (id.det()==DetId::Calo && id.subdetId()==HcalZDCDetId::SubdetectorId)
	//HcalDetId id(baddigi_iter->rawId());
	//if (id.subdet()==HcalZDCDetId::SubdetectorId)
	  {
	    // const ZDCDataFrame digi = (const ZDCDataFrame) (*baddigi_iter);
	    HcalZDCDetId id(baddigi_iter->rawId());
	    int iSide = id.zside();
	    int iSection = id.section();
	    int iChannel = id.channel();
	    if(iSection==1 || iSection==2){
	    ChannelHasDigiError[(9*((1-iSide)/2))+(iChannel-1)+(5*((iSection-1)%2))]=true;
	    DeadChannelError[(9*((1-iSide)/2))+(iChannel-1)+(5*((iSection-1)%2))]=false;
	    //do stuff
	    }//end of if i(Section==1 || iSection==2) 
	  }
	else continue;

      }//end unpacker section



    // ChannelHasError[18]:  [0-8] are iSide=1, [9-17] are iSide=2                                                                                      
    //  First 5 bins ([0-4],[9-13]) are EM bins                                                                                                         
    //  Last 4 bins are HAD bins   


    for (ZDCDigiCollection::const_iterator digi_iter = digi.begin(); 
	 digi_iter != digi.end(); ++digi_iter) 
      {
	const ZDCDataFrame digi = (const ZDCDataFrame) (*digi_iter);
	//HcalZDCDetId id(digi_iter->id());
	int iSide = digi_iter->id().zside();
	int iSection = digi_iter->id().section();
	int iChannel = digi_iter->id().channel();
 
	unsigned int fTS = digi_iter->size();
	while (fData.size()<fTS)
	  fData.push_back(-999);
	while (fData.size()>fTS)
	  fData.pop_back(); // delete last elements 

        if (iSection==1 || iSection==2)
	  {
     
	////////////////////////////////////////NEW Fall 2012 Stuff/////////////////////////////////////////////

	///////////////////////DEAD CELL ERROR///////////////////////////
	///Right now we are simply checking that the digi exists
	DeadChannelError[(9*((1-iSide)/2))+(iChannel-1)+(5*((iSection-1)%2))]=false;
        ////END DEAD CELL CHECK

        int iCapID=27;
        int iCapIDPrevious=27;
        int HotCounter=0;
	int ColdCounter=0;

        for (int iTS=0; iTS<digi.size(); ++iTS) //looping over all ten timeslices
	  {
            ////////////////HOT CELL////////////////////////////////
	    if (digi[iTS].adc()==127) HotCounter+=1;
	    else HotCounter=0;//require 3 consecutive saturated Time Slices in a single channel in a single event
            if (HotCounter >= 3) HotChannelError[(9*((1-iSide)/2))+(iChannel-1)+(5*((iSection-1)%2))]=true; 
	    /////////////END HOT CELL////////////////////////
            

	    ////////////////////////Cold Channel Error Counter//////////
	    if (digi[iTS].adc()<=10) ColdCounter+=1;
	    if (ColdCounter==10) 
	      {
		ColdChannelCounter[(9*((1-iSide)/2))+(iChannel-1)+(5*((iSection-1)%2))]+=1;
	      }
	    ///////////////////////END Cold Channel Error//////////////////

	      
	    ////////////CHECK DIGI HEALTH///////////////
	    ////The first if statement makes sure this digi was not in the unpacker report
	    if ((ChannelHasDigiError[(9*((1-iSide)/2))+(iChannel-1)+(5*((iSection-1)%2))]=false))
	      {
              iCapID=digi.sample(iTS).capid();
	      if (iTS>0) iCapIDPrevious=digi.sample(iTS-1).capid();
	                  
	      if (digi.sample(iTS).dv()==0 || digi.sample(iTS).er()==1)
		{
		  ChannelHasDigiError[(9*((1-iSide)/2))+(iChannel-1)+(5*((iSection-1)%2))]=true;
		  DigiErrorDVER[(9*((1-iSide)/2))+(iChannel-1)+(5*((iSection-1)%2))]=true;
		  break;
		}
	      else
		{
		  if (iTS==0) continue;
		  else
		    {
		      if ((iCapID-iCapIDPrevious+4)%4!=1)
			{
			  ChannelHasDigiError[(9*((1-iSide)/2))+(iChannel-1)+(5*((iSection-1)%2))]=true;
			  DigiErrorCAPID[(9*((1-iSide)/2))+(iChannel-1)+(5*((iSection-1)%2))]=true;
			  break;
			}//end of capid rotation check
		    }//checking if TS!=0
		} // end of the check for dv/er
	      }//END of unpacker double check
	  } // end of TS loop

	////////////END OF Fall 2012 Additions///////////////////////////




	fSum = 0.;
    	bool saturated = false;
	for (unsigned int i = 0; i < fTS; ++i) 
	  {
	     //fData[i]=digi[i].nominal_fC() * ZDCQIEConst;
	     fData[i]=digi[i].nominal_fC();
	     if (digi[i].adc()==digiSaturation){
		 saturated=true;
	     }
	  }
      
	double fTSMean = 0;
	if (fData.size()>6)
    fTSMean = getTime(fData, 4, 6, fSum); // tsmin = 4, tsmax = 6.
	//std::cout << "Side= " << iSide << " Section= " << iSection << " Channel= " << iChannel << "\tCharge\t" << fSum <<std::endl; 
	  if (saturated==true){
	     h_2D_saturation->Fill(iSide==1?0:1,iSection==1?iChannel-1:iChannel+4,1);
	  }
      
	if (iSection == 1) 
	  {    // EM
	    if (iSide == 1) {   // Plus
	      for (unsigned int i = 0; i < fTS; ++i) {
		if (fData[i] > digiThresh) h_ZDCP_EM_Pulse[iChannel - 1]->Fill(i, fData[i]);
	      }
	      if (fSum > digiThresh) {
	        h_ZDCP_EM_Charge[iChannel - 1]->Fill(fSum);
	        h_ZDCP_EM_TSMean[iChannel - 1]->Fill(fTSMean);
	        //std::cout<< "fSum " << fSum << " fTSMean " << fTSMean <<std::endl;
	      }
	    } // Plus
	    if (iSide == -1) {  // Minus
	      for (unsigned int i = 0; i < fTS; ++i) {
		if (fData[i] > digiThresh) h_ZDCM_EM_Pulse[iChannel - 1]->Fill(i, fData[i]);
	      }
	      if (fSum > digiThresh) {
	        h_ZDCM_EM_Charge[iChannel - 1]->Fill(fSum);
	        h_ZDCM_EM_TSMean[iChannel - 1]->Fill(fTSMean);
	      }
	    } // Minus
	  }// EM
      
	else if (iSection == 2) 
	  {    // HAD
	    if (iSide == 1) {   // Plus 
	      for (unsigned int i = 0; i < fTS; ++i) {
		if (fData[i] > digiThresh) h_ZDCP_HAD_Pulse[iChannel - 1]->Fill(i, fData[i]);
	      }
	      if (fSum > digiThresh) {
	        h_ZDCP_HAD_Charge[iChannel - 1]->Fill(fSum);
	        h_ZDCP_HAD_TSMean[iChannel - 1]->Fill(fTSMean);
	      }
	    } // Plus
	    if (iSide == -1) {  // Minus
	      for (unsigned int i = 0; i < fTS; ++i) {
		if (fData[i] > digiThresh) h_ZDCM_HAD_Pulse[iChannel - 1]->Fill(i, fData[i]);
	      } 
	      if (fSum > digiThresh) {
	        h_ZDCM_HAD_Charge[iChannel - 1]->Fill(fSum);
	        h_ZDCM_HAD_TSMean[iChannel - 1]->Fill(fTSMean);
	      }
	    }// Minus
	  } // HAD 
	  }//end of if (iSection==1 || iSection==2)
      } // loop on zdc digi collection
		    
		    
		    
   ////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    // Fill Fall 2012 histograms
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    int numdigierrors=0;
    int numhoterrors=0;

    for (int i = 0; i<18; i++){
      if (ChannelHasDigiError[i]==true)
	{
	  ++numdigierrors;
	  ZDC_Digi_Errors->Fill(i/9,i%9,1);
	}
      if (DigiErrorDVER[i]==true)
	{
	  ZDC_DigiErrors_DVER->Fill(i/9,i%9,1);
	}
      if (DigiErrorCAPID[i]==true)
	{
	  ZDC_DigiErrors_CAPID->Fill(i/9,i%9,1);
	}
      if(HotChannelError[i]==true)
	{
	  ++numhoterrors;
	  ZDC_Hot_Channel_Errors->Fill(i/9,(i%9),1);
	}
      if(DeadChannelError[i]==true)
	{
	  DeadChannelCounter[i]+=1;
	}
      // If any of the above is true, fill the total channel errors
      if (ChannelHasDigiError[i] || HotChannelError[i])
	{
	  ZDC_TotalChannelErrors->Fill(i/9,i%9,1);
	  TotalChannelErrors[i]+=1;
	}
    }//end the for i<18 loop
 

    if (numdigierrors>0)
      ZDC_DigiErrorsVsLS->Fill(currentLS,numdigierrors); //could make this ->Fill(currentLS,1) if I want to count each event as only having one error even if multiple channels are in error
    if (numhoterrors>0)
      ZDC_HotChannelErrorsVsLS->Fill(currentLS,numhoterrors);
   ////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    //End of  filling Fall 2012  histograms
///////////////////////////////////////////////////////////////////////////////////////////////////////////

    //--------------------------------------
    // ZDC RecHit part 
    //--------------------------------------
    for (ZDCRecHitCollection::const_iterator rechit_iter = rechit.begin(); 
	 rechit_iter != rechit.end(); ++rechit_iter)
      {		
	HcalZDCDetId id(rechit_iter->id());
	int Side      = (rechit_iter->id()).zside();
	int Section   = (rechit_iter->id()).section();
	int Channel   = (rechit_iter->id()).channel();
	//std::cout << "RecHitEnergy  " << zhit->energy() << "  RecHitTime  " << zhit->time() << std::endl;
	
	if(Section==1)
	  { //EM
	    if (Side ==1 ){ // Plus
	      h_ZDCP_EM_RecHitEnergy[Channel-1]->Fill(rechit_iter->energy());
	      h_ZDCP_EM_RecHitTiming[Channel-1]->Fill(rechit_iter->time());
	    }
	    if (Side == -1 ){ //Minus
	      h_ZDCM_EM_RecHitEnergy[Channel-1]->Fill(rechit_iter->energy());
	      h_ZDCM_EM_RecHitTiming[Channel-1]->Fill(rechit_iter->time());
	    }
	  } //EM
	else if(Section==2)
	  { //HAD
	    if (Side ==1 ){ //Plus
	      h_ZDCP_HAD_RecHitEnergy[Channel-1]->Fill(rechit_iter->energy());
	      h_ZDCP_HAD_RecHitTiming[Channel-1]->Fill(rechit_iter->time());
	    }
	    if (Side == -1 ){ //Minus
	      h_ZDCM_HAD_RecHitEnergy[Channel-1]->Fill(rechit_iter->energy());
	      h_ZDCM_HAD_RecHitTiming[Channel-1]->Fill(rechit_iter->time());
	    }
	  } // HAD
      } // loop on rechits
    
} // end of event processing 
/*
------------------------------------------------------------------------------------
// This is what we did to find the good signal. After we've started to use only time slice 4,5,6.
bool HcalZDCMonitor::isGood(std::vector<double>fData, double fCut, double fPercentage) {
  bool dec = false;
  int ts_max = -1;
  
  ts_max = getTSMax(fData);
  if (ts_max == 0 || ts_max == (int)(fData.size() - 1))
    return false;
  float sum = fData[ts_max - 1] + fData[ts_max + 1];
  
  // cout << "tsMax " << ts_max << " data[tsmax] " << mData[ts_max] << " sum " << sum << endl;
  if (fData[ts_max] > fCut && sum > (fData[ts_max] * fPercentage))
    dec = true;
  return dec;
} // bool HcalZDCMonitor::isGood

int HcalZDCMonitor::getTSMax(std::vector<double>fData) 
{
  int ts_max = -100;
  double max = -999.;
  
  for (unsigned int j = 0; j < fData.size(); ++j) {
    if (max < fData[j]) {
      max = fData[j];
      ts_max = j;
    }
  }
  return ts_max;
} // int HcalZDCMonitor::getTSMax()
------------------------------------------------------------------------------------
*/
double HcalZDCMonitor::getTime(std::vector<double>fData, unsigned int ts_min, unsigned int ts_max, double &fSum) {
  double weightedTime = 0.;
  double SumT = 0.; 
  double Time = -999.;
  double digiThreshf = 99.5;
 
  for (unsigned int ts=ts_min; ts<=ts_max; ++ts) {
    if (fData[ts] > digiThreshf){ 
    weightedTime += ts * fData[ts];
    SumT += fData[ts];
    }
  }

  if (SumT > 0.) {
    Time = weightedTime / SumT;
  }

  fSum = SumT;

  return Time;

} //double HcalZDCMonitor::getTime()


void HcalZDCMonitor::endLuminosityBlock(const edm::LuminosityBlock& lumiSeg, const edm::EventSetup& c) 
{

  bool HadLumiError[18]={false};

  EventsVsLS->Fill(currentLS,EventCounter);

  if(Online_ == false)
    {//checks if DQM is in OFFLINE MODE
      for (int i=0;i<18;++i)
	{
	  ChannelRatio[i]=(TotalChannelErrors[i])*1./EventCounter;
	  if (ChannelRatio[i] <= MaxErrorRates_[i])
	    {
	      if (i<9)
		{
		  PZDC_QualityIndexVsLB_->Fill(currentLS,ChannelWeighting_[i]);
		}
	      else
		{
		  NZDC_QualityIndexVsLB_->Fill(currentLS,ChannelWeighting_[i]);
		}
	      if (ColdChannelCounter[i] >= OfflineColdThreshold_)//Begin Cold Error Plots
		{
		  ZDC_Cold_Channel_Errors->Fill(i/9,i%9,ColdChannelCounter[i]);
		  ZDC_ColdChannelErrorsVsLS->Fill(currentLS,1);
		  ZDC_TotalChannelErrors->Fill(i/9,i%9,ColdChannelCounter[i]);//Can change this between 1, or the amount of errors (Currently the latter)
		  ColdChannelCounter[i]=0;
		  // HadLumiError[i]=true;
		}//END OF Cold Error Plot
	      if (DeadChannelCounter[i] >= OfflineDeadThreshold_)
		{//Begin Dead Error Plots
		  ZDC_Dead_Channel_Errors->Fill(i/9,i%9,DeadChannelCounter[i]);
		  ZDC_DeadChannelErrorsVsLS->Fill(currentLS,1);
		  ZDC_TotalChannelErrors->Fill(i/9,i%9,DeadChannelCounter[i]); //Could fill this with 1 or total dead errors (which is currently done)
		  DeadChannelCounter[i]=0;
		  HadLumiError[i]=true;
		}//END OF Dead Channel Plots
	      if (HadLumiError[i]==true)
		{//Removing the QI for Dead of Cold Channels
		  if (i<9)
		    {
		      PZDC_QualityIndexVsLB_->Fill(currentLS,(-1*ChannelWeighting_[i]));
		    }
		  else 
		    {
		      NZDC_QualityIndexVsLB_->Fill(currentLS,(-1*ChannelWeighting_[i]));
		    }
		}//END OF QI Removal
	    }//END OF ChannelRatio[i]<=MaxErrorRates_[i]
	  else
	    {//This part only happens if ChannelRatio[i] > MaxErrorRates_[i]...Above you notice the QI plots become 'un-filled'. If the plot was never filled to begin with, then we do not want to remove from the plot  causing there to be a negative QI
	      if (ColdChannelCounter[i] > OfflineColdThreshold_)
		{//Begin Cold Error Plots
		  ZDC_Cold_Channel_Errors->Fill(i/9,i%9,ColdChannelCounter[i]);
                  ZDC_ColdChannelErrorsVsLS->Fill(currentLS,1);
                  ZDC_TotalChannelErrors->Fill(i/9,i%9,ColdChannelCounter[i]);
		  ColdChannelCounter[i]=0;
		}//END OF Cold Error Plot
	      if (DeadChannelCounter[i] > OfflineDeadThreshold_)
		{//Begin Dead Error plots
		  ZDC_Dead_Channel_Errors->Fill(i/9,i%9,DeadChannelCounter[i]);
		  ZDC_DeadChannelErrorsVsLS->Fill(currentLS,1);
		  ZDC_TotalChannelErrors->Fill(i/9,i%9,DeadChannelCounter[i]);
		  DeadChannelCounter[i]=0;
		}//END OF Dead Error Plots
	    }
	}//END OF FOR LOOP
    }//END OF DQM OFFLINE PART

  if(Online_ == true)
    {//checks if DQM is in ONLINE MODE
      for (int i=0;i<18;++i)
	{
	  ChannelRatio[i]=(TotalChannelErrors[i])*1./EventCounter;
	  if (ChannelRatio[i] <= MaxErrorRates_[i])
	    {
	      if (i<9)
		{
		  PZDC_QualityIndexVsLB_->Fill(currentLS,ChannelWeighting_[i]);
		}
	      else
		{
		  NZDC_QualityIndexVsLB_->Fill(currentLS,ChannelWeighting_[i]);
		}
	      if (ColdChannelCounter[i] >= OnlineColdThreshold_)//Begin Cold Error Plots
		{
		  ZDC_Cold_Channel_Errors->Fill(i/9,i%9,ColdChannelCounter[i]);
		  ZDC_ColdChannelErrorsVsLS->Fill(currentLS,1);
		  ZDC_TotalChannelErrors->Fill(i/9,i%9,ColdChannelCounter[i]);//Can change this between 1, or the amount of errors (Currently the latter)
		  ColdChannelCounter[i]=0;
		  // HadLumiError[i]=true;
		}//END OF Cold Error Plot
	      if (DeadChannelCounter[i] >= OnlineDeadThreshold_)
		{//Begin Dead Error Plots
		  ZDC_Dead_Channel_Errors->Fill(i/9,i%9,DeadChannelCounter[i]);
		  ZDC_DeadChannelErrorsVsLS->Fill(currentLS,1);
		  ZDC_TotalChannelErrors->Fill(i/9,i%9,DeadChannelCounter[i]); //Could fill this with 1 or total dead errors (which is currently done)
		  DeadChannelCounter[i]=0;
		  HadLumiError[i]=true;
		}//END OF Dead Channel Plots
	      if (HadLumiError[i]==true)
		{//Removing the QI for Dead of Cold Channels
		  if (i<9)
		    {
		      PZDC_QualityIndexVsLB_->Fill(currentLS,(-1*ChannelWeighting_[i]));
		    }
		  else 
		    {
		      NZDC_QualityIndexVsLB_->Fill(currentLS,(-1*ChannelWeighting_[i]));
		    }
		}//END OF QI Removal
	    }//END OF ChannelRatio[i]<=MaxErrorRates_[i]
	  else
	    {//This part only happens if ChannelRatio[i] > MaxErrorRates_[i]...Above you notice the QI plots become 'un-filled'. If the plot was never filled to begin with, then we do not want to remove from the plot  causing there to be a negative QI
	      if (ColdChannelCounter[i] > OnlineColdThreshold_)
		{//Begin Cold Error Plots
		  ZDC_Cold_Channel_Errors->Fill(i/9,i%9,ColdChannelCounter[i]);
                  ZDC_ColdChannelErrorsVsLS->Fill(currentLS,1);
                  ZDC_TotalChannelErrors->Fill(i/9,i%9,ColdChannelCounter[i]);
		  ColdChannelCounter[i]=0;
		}//END OF Cold Error Plot
	      if (DeadChannelCounter[i] > OnlineDeadThreshold_)
		{//Begin Dead Error plots
		  ZDC_Dead_Channel_Errors->Fill(i/9,i%9,DeadChannelCounter[i]);
		  ZDC_DeadChannelErrorsVsLS->Fill(currentLS,1);
		  ZDC_TotalChannelErrors->Fill(i/9,i%9,DeadChannelCounter[i]);
		  DeadChannelCounter[i]=0;
		}//END OF Dead Error Plots
	    }//end of ChannelRatio[i] > MaxErrorRates_[i] part
	}//END OF FOR LOOP
    }//END OF DQM ONLINE PART



  //if (LumiInOrder(lumiSeg.luminosityBlock())==false) return; // WHY DOESN'T THIS WORK?
  if (dbe_==0)
    {
      return;
    }
  HcalBaseDQMonitor::endLuminosityBlock(lumiSeg, c);

  for (int i = 0; i < 5; ++i) {   // EM Channels 
    // ZDC Plus 
    h_2D_charge->setBinContent(1, i + 1, h_ZDCP_EM_Charge[i]->getMean());
    h_2D_TSMean->setBinContent(1, i + 1, h_ZDCP_EM_TSMean[i]->getMean());
    h_2D_RecHitEnergy->setBinContent(1, i + 1, h_ZDCP_EM_RecHitEnergy[i]->getMean());
    h_2D_RecHitTime->setBinContent(1, i + 1, h_ZDCP_EM_RecHitTiming[i]->getMean());
    // ZDC Minus
    h_2D_charge->setBinContent(2, i + 1, h_ZDCM_EM_Charge[i]->getMean());
    h_2D_TSMean->setBinContent(2, i + 1, h_ZDCM_EM_TSMean[i]->getMean());
    h_2D_RecHitEnergy->setBinContent(2, i + 1, h_ZDCM_EM_RecHitEnergy[i]->getMean());
    h_2D_RecHitTime->setBinContent(2, i + 1, h_ZDCM_EM_RecHitTiming[i]->getMean());
  }
  for (int i = 0; i < 4; ++i) {   // HAD channels 
    // ZDC Plus 
    h_2D_charge->setBinContent(1, i + 6, h_ZDCP_HAD_Charge[i]->getMean());
    h_2D_TSMean->setBinContent(1, i + 6, h_ZDCP_HAD_TSMean[i]->getMean());
    h_2D_RecHitEnergy->setBinContent(1, i + 6, h_ZDCP_HAD_RecHitEnergy[i]->getMean());
    h_2D_RecHitTime->setBinContent(1, i + 6, h_ZDCP_HAD_RecHitTiming[i]->getMean());
    // ZDC Minus
    //h_ZDCM_HAD_Pulse[i]->Scale(10. / h_ZDCM_HAD_Pulse[i]->getEntries());
    h_2D_charge->setBinContent(2, i + 6, h_ZDCM_HAD_Charge[i]->getMean());
    h_2D_TSMean->setBinContent(2, i + 6, h_ZDCM_HAD_TSMean[i]->getMean());
    h_2D_RecHitEnergy->setBinContent(2, i + 6, h_ZDCM_HAD_RecHitEnergy[i]->getMean());
    h_2D_RecHitTime->setBinContent(2, i + 6, h_ZDCM_HAD_RecHitTiming[i]->getMean());

    // use ZDCProblemsCurrentLB_ for stuff
  }
}// void HcalZDCMonitor::endLuminosityBlock()


DEFINE_FWK_MODULE(HcalZDCMonitor);
