#include "DQM/SiStripMonitorSummary/interface/SiStripThresholdDQM.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "TCanvas.h"

// -----
SiStripThresholdDQM::SiStripThresholdDQM(const edm::EventSetup & eSetup,
                                         edm::ParameterSet const& hPSet,
                                         edm::ParameterSet const& fPSet):SiStripBaseCondObjDQM(eSetup, hPSet, fPSet){
  WhichThreshold=hPSet.getParameter<std::string>("WhichThreshold");
}
// -----



// -----
SiStripThresholdDQM::~SiStripThresholdDQM(){}
// -----


// -----
void SiStripThresholdDQM::getActiveDetIds(const edm::EventSetup & eSetup){
  
  getConditionObject(eSetup);
  thresholdHandle_->getDetIds(activeDetIds);

}
// -----

//=====================================================================================
// -----
void SiStripThresholdDQM::fillModMEs(const std::vector<uint32_t> & selectedDetIds){
   
  ModMEs CondObj_ME;
  
    
  for(std::vector<uint32_t>::const_iterator detIter_ = selectedDetIds.begin();
      detIter_!= selectedDetIds.end();detIter_++){
      
    fillMEsForDet(CondObj_ME,*detIter_);
      
  }
}    
// -----



//======================================================================================
// -----
void SiStripThresholdDQM::fillMEsForDet(ModMEs selModME_, uint32_t selDetId_){

  std::vector<uint32_t> DetIds;
  thresholdHandle_->getDetIds(DetIds);
  
  SiStripThreshold::Range ThresholdRange = thresholdHandle_->getRange(selDetId_);
  int nStrip =  reader->getNumberOfApvsAndStripLength(selDetId_).first*128;

  getModMEs(selModME_,selDetId_);
  
  for( int istrip=0;istrip<nStrip;++istrip){
    try{      
      if( CondObj_fillId_ =="onlyProfile" || CondObj_fillId_ =="ProfileAndCumul"){
	if(WhichThreshold=="Low")
	  selModME_.ProfileDistr->Fill(istrip+1,thresholdHandle_->getData(istrip,ThresholdRange).getLth());
	if(WhichThreshold=="High")
	  selModME_.ProfileDistr->Fill(istrip+1,thresholdHandle_->getData(istrip,ThresholdRange).getHth());
      }
    } 
    catch(cms::Exception& e){
      edm::LogError("SiStripThresholdDQM")          
	<< "[SiStripThresholdDQM::fillMEsForDet] cms::Exception accessing thresholdHandle_->getData(istrip,ThresholdRange) for strip "  
	<< istrip 
	<< " and detid " 
	<< selDetId_  
	<< " :  " 
	<< e.what() ;
    }

	// Fill the TkMap
	if(fPSet_.getParameter<bool>("TkMap_On") || hPSet_.getParameter<bool>("TkMap_On")){
	  if(WhichThreshold=="Low") fillTkMap(selDetId_, thresholdHandle_->getData(istrip,ThresholdRange).getLth());
	  if(WhichThreshold=="High") fillTkMap(selDetId_, thresholdHandle_->getData(istrip,ThresholdRange).getHth());
    }
  

  }// istrip
  
}    
// -----


//=======================================================================================
// -----
void SiStripThresholdDQM::fillSummaryMEs(const std::vector<uint32_t> & selectedDetIds){
   
  for(std::vector<uint32_t>::const_iterator detIter_ = selectedDetIds.begin();
      detIter_!= selectedDetIds.end();detIter_++){
    fillMEsForLayer(SummaryMEsMap_, *detIter_);

  }

  for (std::map<uint32_t, ModMEs>::iterator iter=SummaryMEsMap_.begin(); iter!=SummaryMEsMap_.end(); iter++){

    ModMEs selME;
    selME = iter->second;

    if(hPSet_.getParameter<bool>("FillSummaryProfileAtLayerLevel") && fPSet_.getParameter<bool>("OutputSummaryProfileAtLayerLevelAsImage")){

      if( CondObj_fillId_ =="onlyProfile" || CondObj_fillId_ =="ProfileAndCumul"){

	TCanvas c1("c1");
	selME.SummaryOfProfileDistr->getTProfile()->Draw();
	std::string name (selME.SummaryOfProfileDistr->getTProfile()->GetTitle());
	if(WhichThreshold=="Low") name+="_low.png";
	if(WhichThreshold=="High") name+="_high.png";
	c1.Print(name.c_str());
      }
    }
    if(hPSet_.getParameter<bool>("FillSummaryAtLayerLevel") && fPSet_.getParameter<bool>("OutputSummaryAtLayerLevelAsImage")){

      TCanvas c1("c1");
      selME.SummaryDistr->getTH1()->Draw();
      std::string name (selME.SummaryDistr->getTH1()->GetTitle());
      if(WhichThreshold=="Low") name+="_low.png";
      if(WhichThreshold=="High") name+="_high.png";
      c1.Print(name.c_str());
    }
  }

}    
// -----


//=======================================================================================
// -----
void SiStripThresholdDQM::fillMEsForLayer( std::map<uint32_t, ModMEs> selMEsMap_, uint32_t selDetId_){

  // ----
  int subdetectorId_ = ((selDetId_>>25)&0x7);
    
  if( subdetectorId_<3 || subdetectorId_>6 ){ 
    edm::LogError("SiStripThresholdDQM")
      << "[SiStripThresholdDQM::fillMEsForLayer] WRONG INPUT : no such subdetector type : "
      << subdetectorId_ << " no folder set!" 
      << std::endl;
    return;
  }
  // ----
     
  std::map<uint32_t, ModMEs>::iterator selMEsMapIter_ = selMEsMap_.find(getLayerNameAndId(selDetId_).second);
  ModMEs selME_;
  selME_ =selMEsMapIter_->second;
  getSummaryMEs(selME_,selDetId_);

    
  SiStripThreshold::Range ThresholdRange = thresholdHandle_->getRange(selDetId_);
  
  int nStrip =  reader->getNumberOfApvsAndStripLength(selDetId_).first*128;
  
  SiStripHistoId hidmanager;
  
  
  if(hPSet_.getParameter<bool>("FillSummaryProfileAtLayerLevel")){
    // --> profile summary    

    std::string hSummaryOfProfile_description;
    hSummaryOfProfile_description  = hPSet_.getParameter<std::string>("SummaryOfProfile_description");
  
    std::string hSummaryOfProfile_name; 
  
  
    hSummaryOfProfile_name = hidmanager.createHistoLayer(hSummaryOfProfile_description, 
							 "layer", 
							 getLayerNameAndId(selDetId_).first, 
							 "") ;

  
  
    for( int istrip=0;istrip<nStrip;++istrip){
    
      try{ 
	if( CondObj_fillId_ =="onlyProfile" || CondObj_fillId_ =="ProfileAndCumul"){
	  if(WhichThreshold=="Low")
	    selME_.SummaryOfProfileDistr->Fill(istrip+1,thresholdHandle_->getData(istrip,ThresholdRange).getLth());
	  if(WhichThreshold=="High")
	    selME_.SummaryOfProfileDistr->Fill(istrip+1,thresholdHandle_->getData(istrip,ThresholdRange).getHth());
	}
      } 
      catch(cms::Exception& e){
	edm::LogError("SiStripThresholdDQM")          
	  << "[SiStripThresholdDQM::fillMEsForLayer] cms::Exception accessing thresholdHandle_->getData(istrip,ThresholdRange) for strip "  
	  << istrip 
	  << " and detid " 
	  << selDetId_  
	  << " :  " 
	  << e.what() ;
      }

      // Fill the TkMap
      if(fPSet_.getParameter<bool>("TkMap_On") || hPSet_.getParameter<bool>("TkMap_On")){
	if(WhichThreshold=="Low") fillTkMap(selDetId_, thresholdHandle_->getData(istrip,ThresholdRange).getLth());
	if(WhichThreshold=="High") fillTkMap(selDetId_, thresholdHandle_->getData(istrip,ThresholdRange).getHth());
      }
      
    }// istrip	
  }//if Fill

  if(hPSet_.getParameter<bool>("FillSummaryAtLayerLevel")){
    
    // --> summary  

    std::string hSummary_description;
    hSummary_description  = hPSet_.getParameter<std::string>("Summary_description");
  
    std::string hSummary_name; 
    hSummary_name = hidmanager.createHistoLayer(hSummary_description, 
						"layer", 
						getLayerNameAndId(selDetId_).first, 
						"") ;


    float meanLowThreshold=0;
    float meanHighThreshold=0;
  
    for( int istrip=0;istrip<nStrip;++istrip){
    
      try{
	meanLowThreshold = meanLowThreshold + thresholdHandle_->getData(istrip,ThresholdRange).getLth();
	meanHighThreshold = meanHighThreshold + thresholdHandle_->getData(istrip,ThresholdRange).getHth();
      }
      catch(cms::Exception& e){
	edm::LogError("SiStripNoisesDQM")          
	  << "[SiStripNoisesDQM::fillMEsForLayer] cms::Exception accessing thresholdHandle_->getThreshold(istrip,ThresholdRange) for strip "  
	  << istrip 
	  << "and detid " 
	  << selDetId_  
	  << " :  " 
	  << e.what() ;      
      }
    
    }//istrip
  
    meanLowThreshold = meanLowThreshold/nStrip;
    meanHighThreshold = meanHighThreshold/nStrip;
  
  
    // -----
    // get detIds belonging to same layer to fill X-axis with detId-number
  
    std::vector<uint32_t> sameLayerDetIds_=GetSameLayerDetId(activeDetIds,selDetId_);

    unsigned int iBin=0;
    for(unsigned int i=0;i<sameLayerDetIds_.size();i++){
      if(sameLayerDetIds_[i]==selDetId_){iBin=i+1;}
    }   
  
    if(WhichThreshold=="Low")
      selME_.SummaryDistr->Fill(iBin,meanLowThreshold);
    if(WhichThreshold=="High")
      selME_.SummaryDistr->Fill(iBin,meanHighThreshold);
  
  }//if Fill ...
  
}  
// -----

