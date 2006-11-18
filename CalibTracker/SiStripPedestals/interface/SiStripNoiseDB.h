#ifndef SiStripNoiseDB_SiStripNoiseDBTemp_h
#define SiStripNoiseDB_SiStripNoiseDBTemp_h
// -*- C++ -*-
//
// Package:     SiStripNoiseDB
// Class  :     SiStripNoiseDB
// 
/**\class SiStripNoiseDB SiStripNoiseDB.h 

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  dkcira
//         Created:  Sat Feb  4 20:49:51 CET 2006
// $Id: SiStripNoiseDB.h,v 1.2 2006/10/26 16:25:43 giordano Exp $
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
//#include "DataFormats/SiStripDetId/interface/SiStripReadoutKey.h"
//#include "DQM/SiStripCommon/interface/SiStripGenerateKey.h"
//
#include "CalibTracker/SiStripAPVAnalysis/interface/ApvAnalysisFactory.h"

#include "CondFormats/SiStripObjects/interface/SiStripNoises.h"

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

class SiStripNoiseDB : public edm::EDAnalyzer {
   public:
      explicit SiStripNoiseDB(const edm::ParameterSet&);
      ~SiStripNoiseDB();

      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void beginJob(edm::EventSetup const&) ;
      virtual void endJob() ;
      
   
 private:
       DaqMonitorBEInterface* dbe_;
       edm::ParameterSet conf_;
       SiStripFedCabling* fedCabling_;

       SiStripNoises* SiStripNoises_;
       std::vector< std::pair<uint32_t,std::vector<short> > > mSiStripNoises;
	 
       std::vector<uint32_t> SelectedDetIds;
	 
       ApvAnalysisFactory* apvFactory_;
       edm::ParameterSet pedsPSet_;
       int nEvTot_;
       int  theEventInitNumber_; 
       int theEventIterNumber_;
       int NumCMstripsInGroup_;
       std::string outPutFileName;


};

#endif
