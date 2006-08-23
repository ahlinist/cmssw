#ifndef DQM_HCALMONITORTASKS_HCALEVTPOSMONITOR_H
#define DQM_HCALMONITORTASKS_HCALEVTPOSMONITOR_H

#include "DQM/HcalMonitorTasks/interface/HcalBaseMonitor.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBEventPosition.h"
#include <map>

/** \class HcalEventPositionMonitor
  *  
  * $Date: 2006/06/23 16:08:13 $
  * $Revision: 1.5 $
  * \author W. Fisher - FNAL
  */
class HcalEventPositionMonitor: public HcalBaseMonitor {
public:
  HcalEventPositionMonitor(); 
  ~HcalEventPositionMonitor(); 

  void setup(const edm::ParameterSet& ps, DaqMonitorBEInterface* dbe);
  void processEvent(const HcalTBEventPosition& epos);

private:  ///Monitoring elements

  float occThresh_;
  int ievt_;

  vector<vector<double> > xhits_;
  vector<vector<double> > yhits_;  
  vector<char> layers_;

  MonitorElement* meEVT_;
  vector<MonitorElement*> mePlanes_;
  vector<MonitorElement*> meXvalue_;
  vector<MonitorElement*> meYvalue_;


};

#endif
