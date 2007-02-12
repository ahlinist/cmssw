#ifndef DQMSenderService_H
#define DQMSenderService_H

/**
 * This class is responsible for collecting data quality monitoring (DQM)
 * information, packaging it in DQMEvent objects, and transferring them
 * to the storage manager.
 *
 * 27-Dec-2006 - KAB  - Initial Implementation
 */

#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "toolbox/mem/Reference.h"
#include "toolbox/mem/Pool.h"
#include "xdaq/Application.h"
#include "xdaq/ApplicationContext.h"

class DQMSenderService 
{
 public:
  DQMSenderService(const edm::ParameterSet &pset,
                   edm::ActivityRegistry &actReg);
  ~DQMSenderService(void);

  void postEventProcessing(const edm::Event &event,
                           const edm::EventSetup &eventSetup);

  // test routines to check on timing of various signals
  void postBeginJobProcessing();
  void postEndJobProcessing();
  void preSourceProcessing();
  void postSourceProcessing();
  void preModuleProcessing(const edm::ModuleDescription &modDesc);
  void postModuleProcessing(const edm::ModuleDescription &modDesc);
  void preSourceConstructionProcessing(const edm::ModuleDescription &modDesc);
  void postSourceConstructionProcessing(const edm::ModuleDescription &modDesc);
  void preModuleConstructionProcessing(const edm::ModuleDescription &modDesc);
  void postModuleConstructionProcessing(const edm::ModuleDescription &modDesc);

 protected:
  DaqMonitorBEInterface *bei;

  void findMonitorElements(std::vector<MonitorElement *> &put_here,
                           std::string folderPath);

 private:

};

#endif
