#ifndef SiStripMonitorDigi_SiStripMonitorDigiTemp_h
#define SiStripMonitorDigi_SiStripMonitorDigiTemp_h
// -*- C++ -*-
//
// Package:     SiStripMonitorDigi_temp
// Class  :     SiStripMonitorDigi_temp
// 
/**\class SiStripMonitorDigi SiStripMonitorDigi.h 

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  dkcira
//         Created:  Sat Feb  4 20:49:51 CET 2006
// $Id: SiStripMonitorDigi.h,v 1.2 2006/03/08 13:00:30 dkcira Exp $
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

// data formats
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripEventSummary.h"
// cabling
#include "CondFormats/SiStripObjects/interface/SiStripFedCabling.h"
#include "CondFormats/DataRecord/interface/SiStripFedCablingRcd.h"
#include "CondFormats/SiStripObjects/interface/FedChannelConnection.h"
#include "DataFormats/SiStripDetId/interface/SiStripReadoutKey.h"
//#include "DQM/SiStripCommon/interface/SiStripGenerateKey.h"
//
#include "CalibTracker/SiStripAPVAnalysis/interface/ApvAnalysisFactory.h"

//#include "Fed9UUtils.hh"
#include "boost/cstdint.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>


class FEDRawDataCollection;
class FEDRawData;
class SiStripDigi;
class SiStripRawDigi;
class SiStripEventSummary;
class SiStripFedCabling;


class SiStripPedDB : public edm::EDAnalyzer {
   public:
      explicit SiStripPedDB(const edm::ParameterSet&);
      ~SiStripPedDB();

      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void beginJob(edm::EventSetup const&) ;
      virtual void endJob() ;
      
   
 private:
       DaqMonitorBEInterface* dbe_;
       edm::ParameterSet conf_;
       //       Fed9U::Fed9UEvent* fedEvent_;
       //       Fed9U::Fed9UDescription* fedDescription_;
       SiStripFedCabling* fedCabling_;


       int16_t fedid;
       int16_t freq;
       int16_t fedkey;
       int16_t headerBytes_;
       int16_t dumpFrequency_;

       vector<uint32_t> SelectedDetIds;
	 
       ApvAnalysisFactory* apvFactory_;
       edm::ParameterSet pedsPSet_;
       int nEvTot_;
       int  theEventInitNumber_; 
       int theEventIterNumber_;
       int NumCMstripsInGroup_;
       string outPutFileName;
       std::string userEnv_;  
       std::string passwdEnv_;   
       bool printdebug_;


};

#endif
