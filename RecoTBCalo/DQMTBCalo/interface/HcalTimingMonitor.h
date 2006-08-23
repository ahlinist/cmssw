#ifndef DQM_HCALMONITORTASKS_HCALTIMINGMONITOR_H
#define DQM_HCALMONITORTASKS_HCALTIMINGMONITOR_H

#include "DQM/HcalMonitorTasks/interface/HcalBaseMonitor.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTiming.h"
#include <map>

/** \class HcalTimingMonitor
  *  
  * $Date: 2006/06/23 16:08:13 $
  * $Revision: 1.5 $
  * \author W. Fisher - FNAL
  */

class HcalTimingMonitor: public HcalBaseMonitor {
public:
  HcalTimingMonitor(); 
  ~HcalTimingMonitor(); 

  void setup(const edm::ParameterSet& ps, DaqMonitorBEInterface* dbe);
  void processEvent(const HcalTBTiming& epos);

private:  ///Monitoring elements

  float occThresh_;
  int ievt_;

  MonitorElement* meTOF1_;
  MonitorElement* meTOF2_;
  MonitorElement* meTOFT_;

  MonitorElement* meEVT_;

  //  MonitorElement* meBHal1_;
  //  MonitorElement* meBHal2_;
  //  MonitorElement* meBHal3_;
  //  MonitorElement* meBHal4_;



};

#endif
