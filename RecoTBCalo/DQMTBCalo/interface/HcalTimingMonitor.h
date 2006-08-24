#ifndef DQM_HCALMONITORTASKS_HCALTIMINGMONITOR_H
#define DQM_HCALMONITORTASKS_HCALTIMINGMONITOR_H

#include "DQM/HcalMonitorTasks/interface/HcalBaseMonitor.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTiming.h"
#include <map>

/** \class HcalTimingMonitor
  *  
  * $Date: 2006/08/23 18:51:47 $
  * $Revision: 1.1 $
  * \author W. Fisher - FNAL
  */

class HcalTimingMonitor: public HcalBaseMonitor {
public:
  HcalTimingMonitor(); 
  ~HcalTimingMonitor(); 

  void setup(const edm::ParameterSet& ps, DaqMonitorBEInterface* dbe);
  void processEvent(const HcalTBTiming& epos);
  void clearME();

private:  ///Monitoring elements

  float occThresh_;
  int ievt_;

  MonitorElement* meTOF1S_;
  MonitorElement* meTOF2S_;
  MonitorElement* meTOFTS_;
  MonitorElement* meTOF1J_;
  MonitorElement* meTOF2J_;
  MonitorElement* meTOFTJ_;
  MonitorElement* meTOF_DT_;
  MonitorElement* meTOF_DT1_;
  MonitorElement* meTOF_DT2_;

  MonitorElement* meTrigger_;
  MonitorElement* meTTCL1A_;
  MonitorElement* meBeamCoinc_;
  MonitorElement* meDT_[3];

  MonitorElement* meEVT_;

  //  MonitorElement* meBHal1_;
  //  MonitorElement* meBHal2_;
  //  MonitorElement* meBHal3_;
  //  MonitorElement* meBHal4_;



};

#endif
