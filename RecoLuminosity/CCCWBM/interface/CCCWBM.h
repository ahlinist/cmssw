#ifndef _HLX_CCCWBM_H_
#define _HLX_CCCWBM_H_

// system include files
#include <iostream>
#include <string>
#include <memory>
#include <iomanip>
#include <cstdlib>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
//#include "DQMServices/Daemon/interface/MonitorDaemon.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "RecoLuminosity/HLXReadOut/CoreUtils/include/ICTypeDefs.hh"
#include "RecoLuminosity/HLXReadOut/HLXCoreLibs/include/LumiStructures.hh"
#include "RecoLuminosity/TCPReceiver/interface/TCPReceiver.h"

#include <TROOT.h>
#include <TFile.h>
#include <TH1F.h>
#include <time.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TImage.h>
#include <TCanvas.h>

using namespace HCAL_HLX;

//
// class decleration
//

class CCCWBM : public edm::EDAnalyzer {

public:
  explicit CCCWBM(const edm::ParameterSet&);
  ~CCCWBM();

private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();
  void resetHistos();

  TCPReceiver HT;      
  TFile * outputfile;
  TH1F* InstLumi;
  TH1F* IntLumi;
  TH1F* LumiVsBX;
  TH1F* LumiSpread;
  std::string outputDirectory;
  std::string InstLumiTitle;
  std::string IntLumiTitle;
  std::string LumiVsBXTitle;
  std::string LumiSpreadTitle;
  time_t start_time;
  struct tm * timeinfo;
  std::ostringstream start_date;
  Int_t offset_time; 

  //define arrays to keep int and inst lumi vals every min for 24 hours (1440 min)
  float instLumi[1440];
  float intLumi[1440];
  bool timeWasFilled[1440];
  int currentRunNumber;

  //connection params
  unsigned int AquireMode;
  unsigned int reconnTime;
  std::string DistribIP;
  int listenPort;

  HCAL_HLX::LUMI_SECTION lumiSection;
};

#endif
