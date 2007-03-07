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
#include "IOPool/Streamer/interface/DQMEventMessage.h"
#include "IOPool/Streamer/interface/DQMEventMsgBuilder.h"
#include "DataFormats/Provenance/interface/LuminosityBlockID.h"
#include "IOPool/Streamer/interface/StreamDQMSerializer.h"
#include "IOPool/Streamer/interface/StreamDQMDeserializer.h"

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

  void findMonitorElements(DQMEvent::TObjectTable &toTable,
                           std::string folderPath);

 private:
  void writeI2ODQMData(DQMEventMsgBuilder const& dqmMsgBuilder);

  std::vector<char> messageBuffer_;
  int lumiSectionInterval_;  
  double lumiSectionsPerUpdate_;
  //edm::LuminosityBlockID lumiSectionOfPreviousUpdate_;
  //edm::LuminosityBlockID firstLumiSectionSeen_;
  unsigned int lumiSectionOfPreviousUpdate_;
  unsigned int firstLumiSectionSeen_;
  bool initializationIsNeeded_;
  bool useCompression_;
  int compressionLevel_;
  edm::StreamDQMSerializer serializeWorker_;
  edm::StreamDQMDeserializer deserializeWorker_;

  xdaq::Application *thisXdaqApplication_;
  toolbox::mem::Pool *xdaqMemPool_;
  xdaq::ApplicationDescriptor *smXdaqDestination_;

  unsigned int i2o_max_size_;
  unsigned int max_i2o_DQM_datasize_; 

};

#endif
