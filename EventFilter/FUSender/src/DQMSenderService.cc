/**
 * This class is responsible for collecting data quality monitoring (DQM)
 * information, packaging it in DQMEvent objects, and transferring them
 * to the storage manager.
 *
 * 27-Dec-2006 - KAB  - Initial Implementation
 *
 * Reference code can be found in the following files:
 * - IOPool/Streamer/interface/StreamerOutputModule.h
 * - IOPool/Streamer/interface/StreamerSerializer.h
 * - IOPool/Streamer/src/StreamerSerializer.cc
 * - EventFilter/FUSender/src/FUStreamerI2OWriter.h
 * - EventFilter/FUSender/src/FUStreamerI2OWriter.cc
 * - DQMServices/Daemon/src/MonitorDaemon.cc
 * - FWCore/ServiceRegistry/interface/ActivityRegistry.h
 * - FWCore/ServiceRegistry/src/ActivityRegistry.cc
 * - DQMServices/NodeROOT/src/SenderBase.cc
 * - DQMServices/NodeROOT/src/ReceiverBase.cc
 */

#include "EventFilter/FUSender/interface/DQMSenderService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "IOPool/Streamer/interface/DQMEventMsgBuilder.h"
#include "IOPool/Streamer/interface/StreamSerializer.h"
#include "IOPool/Streamer/interface/StreamDeserializer.h"
#include "DQMServices/CoreROOT/interface/MonitorElementRootT.h"
#include "FWCore/Utilities/interface/GetReleaseVersion.h"
#include "DQMServices/NodeROOT/interface/SenderBase.h"

#include "i2o/Method.h"
#include "i2o/utils/include/i2o/utils/AddressMap.h"
#include "toolbox/mem/MemoryPoolFactory.h"
#include "xcept/tools.h"

using namespace std;
using namespace toolbox::mem;

/**
 * Local debug flag.  0 => no debug output; 1 => tracing of necessary
 * routines; 2 => additional tracing of many framework signals.
 */
#define DSS_DEBUG 1

/**
 * DQMSenderService constructor.
 */
DQMSenderService::DQMSenderService(const edm::ParameterSet &pset,
                                   edm::ActivityRegistry &actReg)
{
  if (DSS_DEBUG) {cout << "DQMSenderService Constructor" << endl;}

  // specify the routine to be called after event processing.  This routine
  // will be used to periodically fetch monitor elements from the DQM
  // backend and send them to the storage manager.
  actReg.watchPostProcessEvent(this, &DQMSenderService::postEventProcessing);

  // specify the routine to be called after the input source has been
  // constructed.  This routine will be used to initialize our connection
  // to the storage manager and any other needed setup.
  actReg.watchPostSourceConstruction(this,
         &DQMSenderService::postSourceConstructionProcessing);

  // specify the routine to be called when the job has finished.  It will
  // be used to disconnect from the SM, if needed, and any other shutdown
  // tasks that are needed.
  actReg.watchPostEndJob(this, &DQMSenderService::postEndJobProcessing);

  // helpful callbacks when trying to understand the signals that are
  // available to framework services
  if (DSS_DEBUG >= 2) {
    actReg.watchPostBeginJob(this, &DQMSenderService::postBeginJobProcessing);
    actReg.watchPreSource(this, &DQMSenderService::preSourceProcessing);
    actReg.watchPostSource(this, &DQMSenderService::postSourceProcessing);
    actReg.watchPreModule(this, &DQMSenderService::preModuleProcessing);
    actReg.watchPostModule(this, &DQMSenderService::postModuleProcessing);
    actReg.watchPreSourceConstruction(this,
           &DQMSenderService::preSourceConstructionProcessing);
    actReg.watchPreModuleConstruction(this,
           &DQMSenderService::preModuleConstructionProcessing);
    actReg.watchPostModuleConstruction(this,
           &DQMSenderService::postModuleConstructionProcessing);
  }

  // set internal values from the parameter set
  messageBuffer_.resize(pset.getParameter<int>("MessageBufferSize"));
  destinationName_ = pset.getParameter<string>("DestinationName");
  updateInterval_ = pset.getParameter<int>("UpdateInterval");

  // start the update interval now
  lastUpdateTime_ = time(0);
}

/**
 * DQMSenderService destructor.
 */
DQMSenderService::~DQMSenderService(void)
{
  if (DSS_DEBUG) {cout << "DQMSenderService Destructor" << endl;}
}

/**
 * Callback to be used after event processing has finished.  (The
 * "post event" signal is generated after all of the analysis modules
 * have run <b>and</b> any output modules have run.)  This routine is
 * used to periodically gather monitor elements from the DQM backend
 * and send them to the storage manager.
 */
void DQMSenderService::postEventProcessing(const edm::Event &event,
                                           const edm::EventSetup &eventSetup)
{
  // check if it is time for an update
  time_t now = time(0);
  if ((now - lastUpdateTime_) < updateInterval_) {return;}
  lastUpdateTime_ = now;

  if (DSS_DEBUG) {
    cout << "DQMSenderService::postEventProcessing called, event number "
         << event.id().event() << endl;
  }

  // retry the lookup of the backend interface, if needed
  if (bei == NULL) {
    bei = edm::Service<DaqMonitorBEInterface>().operator->();
  }

  // to go any further, a backend interface pointer is crucial
  if (bei == NULL) {
    throw cms::Exception("postEventProcessing", "DQMSenderService")
      << "Unable to lookup the DaqMonitorBEInterface service!\n";
  }

  // determine the top level folders (these will be used for grouping
  // monitor elements into DQM Events)
  std::vector<std::string> topLevelFolderList;
  //std::cout << "### SenderService, pwd = " << bei->pwd() << std::endl;
  bei->cd();
  //std::cout << "### SenderService, pwd = " << bei->pwd() << std::endl;
  topLevelFolderList = bei->getSubdirs();

  // find the monitor elements under each top level folder (including
  // subdirectories)
  std::map< std::string, DQMEvent::MonitorElementTable > meMap;
  std::vector<std::string>::const_iterator dirIter;
  for (dirIter = topLevelFolderList.begin();
       dirIter != topLevelFolderList.end();
       dirIter++) {
    std::string dirName = *dirIter;
    DQMEvent::MonitorElementTable meTable;

    // find the MEs
    findMonitorElements(meTable, dirName);

    // store the list in the map
    meMap[dirName] = meTable;
  }

  // create a DQMEvent message for each top-level folder
  // and send each to the SM
  for (dirIter = topLevelFolderList.begin();
       dirIter != topLevelFolderList.end();
       dirIter++) {
    std::string dirName = *dirIter;
    DQMEvent::MonitorElementTable meTable = meMap[dirName];
    if (meTable.size() == 0) {continue;}

    // create the message
    DQMEventMsgBuilder dqmMsgBuilder(&messageBuffer_[0], messageBuffer_.size(),
                                     event.id().run(), event.id().event(),
                                     edm::getReleaseVersion(), dirName);
    edm::StreamSerializer::serializeDQMEvent(meTable, dqmMsgBuilder);

    // send the message
    //coming soon...

    // test deserialization
    DQMEventMsgView dqmEventView(&messageBuffer_[0]);
    std::cout << "  Message data:" << std::endl; 
    std::cout << "    protocol version = "
              << dqmEventView.protocolVersion() << std::endl; 
    std::cout << "    header size = "
              << dqmEventView.headerSize() << std::endl; 
    std::cout << "    run number = "
              << dqmEventView.runNumber() << std::endl; 
    std::cout << "    event number = "
              << dqmEventView.eventNumberAtUpdate() << std::endl; 
    std::cout << "    compression flag = "
              << dqmEventView.compressionFlag() << std::endl; 
    std::cout << "    reserved word = "
              << dqmEventView.reserved() << std::endl; 
    std::cout << "    release tag = "
              << dqmEventView.releaseTag() << std::endl; 
    std::cout << "    top folder name = "
              << dqmEventView.topFolderName() << std::endl; 
    std::cout << "    sub folder count = "
              << dqmEventView.subFolderCount() << std::endl; 
    std::auto_ptr<DQMEvent::TObjectTable> toTable =
      edm::StreamDeserializer::deserializeDQMEvent(dqmEventView);
    DQMEvent::TObjectTable::const_iterator toIter;
    for (toIter = toTable->begin(); toIter != toTable->end(); toIter++) {
      std::string subFolderName = toIter->first;
      std::cout << "  folder = " << subFolderName << std::endl;
      std::vector<TObject *> toList = toIter->second;
      for (int tdx = 0; tdx < (int) toList.size(); tdx++) {
        TObject *toPtr = toList[tdx];
        string cls = toPtr->IsA()->GetName();
        string nm = toPtr->GetName();
        std::cout << "    TObject class = " << cls
                  << ", name = " << nm << std::endl;
      }
    }
  }

  // reset monitor elements that have requested it
  //bei->doneSending(true, true);
}

/**
 * Callback to be used after the input source has been constructed.  It
 * takes care of any intialization that is needed by this service.
 */
void DQMSenderService::postSourceConstructionProcessing(const edm::ModuleDescription &moduleDesc)
{
  if (DSS_DEBUG) {
    cout << "DQMSenderService::postSourceConstructionProcessing called for "
         << moduleDesc.moduleName() << endl;
  }

  bei = edm::Service<DaqMonitorBEInterface>().operator->();
}

/**
 * Callback to be used after the end job operation has finished.  It takes
 * care of any necessary cleanup.
 */
void DQMSenderService::postEndJobProcessing()
{
  if (DSS_DEBUG) {
    cout << "DQMSenderService::postEndJobProcessing called" << endl;
  }
}

/**
 * Finds all of the monitor elements under the specified folder,
 * including those in subdirectories.
 */
void DQMSenderService::findMonitorElements(DQMEvent::MonitorElementTable &meTable,
                                           std::string folderPath)
{
  if (bei == NULL) {return;}

  // fetch the monitor elements in the specified directory
  std::vector<MonitorElement *> localMEList = bei->getContents(folderPath);
  //MonitorElementRootFolder* folderPtr = bei->getDirectory(folderPath);

  // add the MEs that should be updated to the table
  std::vector<MonitorElement *> updatedMEList;
  for (int idx = 0; idx < (int) localMEList.size(); idx++) {
    MonitorElement *mePtr = localMEList[idx];
    bool wasUpdated = mePtr->wasUpdated();
    bool neverSent = false; //SenderBase::isNeverSent(folderPtr, mePtr->getName());
    if (wasUpdated || neverSent) {
      updatedMEList.push_back(mePtr);

      if (wasUpdated) {
        bei->add2UpdatedContents(mePtr->getName(), folderPath);
      }

      if (neverSent) {
        //SenderBase::setNeverSent(folderPtr, mePtr->getName(), false);
      }
    }
  }
  if (updatedMEList.size() > 0) {
    meTable[folderPath] = updatedMEList;
  }

  // find the subdirectories in this folder
  // (checking if the directory exists is probably overkill,
  // but we really don't want to create new folders using
  // setCurrentFolder())
  if (bei->dirExists(folderPath)) {
    bei->setCurrentFolder(folderPath);
    std::vector<std::string> subDirList = bei->getSubdirs();

    // loop over the subdirectories, find the MEs in each one
    std::vector<std::string>::const_iterator dirIter;
    for (dirIter = subDirList.begin(); dirIter != subDirList.end(); dirIter++) {
      std::string subDirPath = folderPath + "/" + (*dirIter);
      findMonitorElements(meTable, subDirPath);
    }
  }
}

/**
 * Callback for when the begin job operation is finishing.
 * Currently, this routine is only used for diagnostics.
 */
void DQMSenderService::postBeginJobProcessing()
{
  if (DSS_DEBUG >= 2) {
    cout << "DQMSenderService::postBeginJobProcessing called" << endl;
  }
}

/**
 * Callback for when the input source is about to read or generate a
 * physics event.
 * Currently, this routine is only used for diagnostics.
 */
void DQMSenderService::preSourceProcessing()
{
  if (DSS_DEBUG >= 2) {
    cout << "DQMSenderService::preSourceProcessing called" << endl;
  }
}

/**
 * Callback for when the input source has finished reading or generating a
 * physics event.
 * Currently, this routine is only used for diagnostics.
 */
void DQMSenderService::postSourceProcessing()
{
  if (DSS_DEBUG >= 2) {
    cout << "DQMSenderService::postSourceProcessing called" << endl;
  }
}

/**
 * Callback to be used before an analysis module begins its processing.
 * Currently, this routine is only used for diagnostics.
 */
void DQMSenderService::preModuleProcessing(const edm::ModuleDescription &moduleDesc)
{
  if (DSS_DEBUG >= 2) {
    cout << "DQMSenderService::preModuleProcessing called for "
         << moduleDesc.moduleName() << endl;
  }
}

/**
 * Callback to be used after an analysis module has completed its processing.
 * Currently, this routine is only used for diagnostics.
 */
void DQMSenderService::postModuleProcessing(const edm::ModuleDescription &moduleDesc)
{
  if (DSS_DEBUG >= 2) {
    cout << "DQMSenderService::postModuleProcessing called for "
         << moduleDesc.moduleName() << endl;
  }
}

/**
 * Callback to be used before the input source is constructed.
 * Currently, this routine is only used for diagnostics.
 */
void DQMSenderService::preSourceConstructionProcessing(const edm::ModuleDescription &moduleDesc)
{
  if (DSS_DEBUG >= 2) {
    cout << "DQMSenderService::preSourceConstructionProcessing called for "
         << moduleDesc.moduleName() << endl;
  }
}

/**
 * Callback to be used before analysis modules are constructed.
 * Currently, this routine is only used for diagnostics.
 */
void DQMSenderService::preModuleConstructionProcessing(const edm::ModuleDescription &moduleDesc)
{
  if (DSS_DEBUG >= 2) {
    cout << "DQMSenderService::preModuleConstructionProcessing called for "
         << moduleDesc.moduleName() << endl;
  }
}

/**
 * Callback to be used after analysis modules have been constructed.
 * Currently, this routine is only used for diagnostics.
 */
void DQMSenderService::postModuleConstructionProcessing(const edm::ModuleDescription &moduleDesc)
{
  if (DSS_DEBUG >= 2) {
    cout << "DQMSenderService::postModuleConstructionProcessing called for "
         << moduleDesc.moduleName() << endl;
  }
}
