#ifndef SiStripOfflinePedNoiseToDb_h
#define SiStripOfflinePedNoiseToDb_h

// $Id: $

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include <FWCore/Framework/interface/EventSetup.h>

#include "DQMServices/Core/interface/MonitorElement.h"

// cond formats
#include "CondFormats/SiStripObjects/interface/SiStripPedestals.h"
#include "CondFormats/SiStripObjects/interface/SiStripNoises.h"
#include "CondFormats/DataRecord/interface/SiStripPedestalsRcd.h"
#include "CondFormats/DataRecord/interface/SiStripNoisesRcd.h"

#include "CalibTracker/SiStripAPVAnalysis/interface/ApvAnalysisFactory.h"

#include "boost/cstdint.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

namespace cms{
  class SiStripOfflinePedNoiseToDb : public edm::EDAnalyzer {
  public:
    explicit SiStripOfflinePedNoiseToDb(const edm::ParameterSet&);
    ~SiStripOfflinePedNoiseToDb();
    
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void beginJob(edm::EventSetup const&) ;
    virtual void endJob() ;
      
   
  private:
    edm::ParameterSet conf_;
    edm::ParameterSet pedsPSet_;

    DaqMonitorBEInterface* dbe_;
    ApvAnalysisFactory* apvFactory_;

    std::vector<uint32_t> SelectedDetIds_;
	 
    SiStripPedestals* SiStripPedestals_;
    SiStripNoises* SiStripNoises_;

    std::vector< std::pair<uint32_t,std::vector<char> > > mSiStripPedestals_;
    std::vector< std::pair<uint32_t,std::vector<short> > > mSiStripNoises_;

    int nEvTot_;
    int theEventInitNumber_; 
    int theEventIterNumber_;
    int NumCMstripsInGroup_;
    bool appendMode_;
  };
}
#endif
