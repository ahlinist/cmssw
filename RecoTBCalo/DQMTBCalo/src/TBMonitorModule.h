#ifndef TBMonitorModule_H
#define TBMonitorModule_H

/*
 * \file TBMonitorModule.h
 *
 * $Date: 2006/08/01 20:47:49 $
 * $Revision: 1.3 $
 * \author W. Fisher
 *
*/

#include "RecoTBCalo/DQMTBCalo/interface/HcalEventPositionMonitor.h"
#include "RecoTBCalo/DQMTBCalo/interface/HcalQADCMonitor.h"
#include "RecoTBCalo/DQMTBCalo/interface/HcalTimingMonitor.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Daemon/interface/MonitorDaemon.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace cms;
using namespace std;

class TBMonitorModule: public edm::EDAnalyzer{

public:

/// Constructor
TBMonitorModule(const edm::ParameterSet& ps);

/// Destructor
~TBMonitorModule();

protected:

/// Analyze
void analyze(const edm::Event& e, const edm::EventSetup& c);

// BeginJob
void beginJob(const edm::EventSetup& c);

// EndJob
void endJob(void);

private:

  int m_ievt;
  int m_runNum;
  DaqMonitorBEInterface* m_dbe;
  MonitorElement* m_meStatus;
  MonitorElement* m_meRunNum;
  MonitorElement* m_meRunType;
  MonitorElement* m_meEvtNum;

  HcalEventPositionMonitor*  m_eposMon;
  HcalQADCMonitor*           m_qadcMon;
  HcalTimingMonitor*         m_timeMon;
  
  bool m_monitorDaemon;
  bool offline_;
  string m_outputFile;
  ofstream m_logFile;
  
};

#endif
