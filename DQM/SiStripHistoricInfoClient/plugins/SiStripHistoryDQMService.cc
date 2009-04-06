#include "DQM/SiStripHistoricInfoClient/plugins/SiStripNewHistoricDQMService.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Diagnostic/interface/HDQMfitUtilities.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"


SiStripNewHistoricDQMService::SiStripNewHistoricDQMService(const edm::ParameterSet& iConfig,const edm::ActivityRegistry& aReg)
: DQMHistoryServiceBase::DQMHistoryServiceBase(iConfig, aReg), iConfig_(iConfig)
{
  edm::LogInfo("SiStripNewHistoricDQMService") <<  "[SiStripNewHistoricDQMService::SiStripNewHistoricDQMService]";
}


SiStripNewHistoricDQMService::~SiStripNewHistoricDQMService() { 
  edm::LogInfo("SiStripNewHistoricDQMService") <<  "[SiStripNewHistoricDQMService::~SiStripNewHistoricDQMService]";
}


uint32_t SiStripNewHistoricDQMService::returnDetComponent(std::string& str){
  LogTrace("SiStripNewHistoricDQMService") <<  "[SiStripNewHistoricDQMService::returnDetComponent]";

  size_t __key_length__=7;
  size_t __detid_length__=9;

  uint32_t layer=0,side=0;

  if(str.find("__det__")!= std::string::npos){
    return atoi(str.substr(str.find("__det__")+__key_length__,__detid_length__).c_str());
  }
  //TIB
  else if(str.find("TIB")!= std::string::npos){
    if (str.find("layer")!= std::string::npos) 
      layer=atoi(str.substr(str.find("layer__")+__key_length__,1).c_str());
    return TIBDetId(layer,0,0,0,0,0).rawId();
  }
  //TOB
  else if(str.find("TOB")!= std::string::npos){
    if (str.find("layer")!= std::string::npos) 
      layer=atoi(str.substr(str.find("layer__")+__key_length__,1).c_str());
    return TOBDetId(layer,0,0,0,0).rawId();
  }
  //TID
  else if(str.find("TID")!= std::string::npos){  
    if (str.find("side")!= std::string::npos){
      side=atoi(str.substr(str.find("_side__")+__key_length__,1).c_str());
      if (str.find("wheel")!= std::string::npos){
	layer=atoi(str.substr(str.find("wheel__")+__key_length__,1).c_str());
      }
    }
    return TIDDetId(side,layer,0,0,0,0).rawId();
  } 
  //TEC
  else if(str.find("TEC")!= std::string::npos){  
    if (str.find("side")!= std::string::npos){
      layer=atoi(str.substr(str.find("wheel__")+__key_length__,1).c_str());
      if (str.find("wheel")!= std::string::npos){
	side=atoi(str.substr(str.find("_side__")+__key_length__,1).c_str());
      }
    }
    return TECDetId(side,layer,0,0,0,0,0).rawId();
  } 
  else 
    return DetId::Tracker; //Full Tracker
}



