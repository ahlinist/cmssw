#ifndef DQM_HCALMONITORTASKS_HCALQADCMONITOR_H
#define DQM_HCALMONITORTASKS_HCALQADCMONITOR_H

#include "DQM/HcalMonitorTasks/interface/HcalBaseMonitor.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBBeamCounters.h"
#include <map>

/** \class HcalQADCMonitor
  *  
  * $Date: 2006/08/23 18:51:47 $
  * $Revision: 1.1 $
  * \author W. Fisher - FNAL
  */
class HcalQADCMonitor: public HcalBaseMonitor {
public:
  HcalQADCMonitor(); 
  ~HcalQADCMonitor(); 

  void setup(const edm::ParameterSet& ps, DaqMonitorBEInterface* dbe);
  void processEvent(const HcalTBBeamCounters& bc);
  void clearME();

private:  ///Monitoring elements

  float occThresh_;

  int ievt_;

  MonitorElement* meCHER1_;
  MonitorElement* meCHER2_;
  MonitorElement* meCHER3_;
  MonitorElement* meTOF1_;
  MonitorElement* meTOF2_;
  MonitorElement* meEVT_;
  MonitorElement* meSci521_;  
  MonitorElement* meSci528_;  
  MonitorElement* meSciVLE_;  
  MonitorElement* meTrigSc1_;  
  MonitorElement* meTrigSc2_;  
  MonitorElement* meTrigSc3_;  
  MonitorElement* meTrigSc4_;
  MonitorElement* meVMF_;  
  MonitorElement* meVMB_;  
  MonitorElement* meVMu1_;  
  MonitorElement* meVMu2_; 
  MonitorElement* meVMu3_;  
  MonitorElement* meVMu4_;  
  MonitorElement* meVMu5_;  
  MonitorElement* meVMu6_;  
  MonitorElement* meVMu7_;  
  MonitorElement* meVMu8_;  
  //  MonitorElement* meBHalo1_;  
  //  MonitorElement* meBHalo2_;  
  //  MonitorElement* meBHalo3_;  
  //  MonitorElement* meBHalo4_; 

};

#endif
