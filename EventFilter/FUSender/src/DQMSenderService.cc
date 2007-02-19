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
 */

#include "EventFilter/FUSender/interface/DQMSenderService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "IOPool/Streamer/interface/DQMEventMsgBuilder.h"
#include "DQMServices/CoreROOT/interface/MonitorElementRootT.h"

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
  if (DSS_DEBUG) {
    cout << "DQMSenderService::postEventProcessing called, event number "
         << event.id().event() << endl;
  }

  // ensure that our pointer to the backend interface is valid
  if (bei != NULL) {

    // determine the top level folders (these will be used for grouping
    // monitor elements into DQM Events)
    std::vector<std::string> topLevelFolderList;
    //std::cout << "### SenderService, pwd = " << bei->pwd() << std::endl;
    bei->cd();
    //std::cout << "### SenderService, pwd = " << bei->pwd() << std::endl;
    topLevelFolderList = bei->getSubdirs();

    // find the monitor elements under each top level folder (including
    // subdirectories)
    std::map< std::string, std::vector<MonitorElement *> > meMap;
    std::vector<std::string >::const_iterator dirIter;
    for (dirIter = topLevelFolderList.begin();
         dirIter != topLevelFolderList.end();
         dirIter++) {
      std::string dirName = *dirIter;
      std::vector<MonitorElement *> meList;

      // call the method to find the MEs
      findMonitorElements(meList, dirName);

      // store the list in the map
      meMap[dirName] = meList;
    }

    // dump out the results
    for (dirIter = topLevelFolderList.begin();
         dirIter != topLevelFolderList.end();
         dirIter++) {

      #define BUFF_SIZE 32000
      char transitBuff[BUFF_SIZE];
      TBuffer sendMsg(TBuffer::kWrite, BUFF_SIZE, &transitBuff[0], false);
      int numObjects = 0;

      std::string dirName = *dirIter;
      std::cout << "Top level directory " << dirName
                << " contains the following MEs:" << std::endl;
      std::vector<MonitorElement *> meList = meMap[dirName];
      std::vector<MonitorElement *>::const_iterator meIter;
      for (meIter = meList.begin(); meIter != meList.end(); meIter++) {
        MonitorElement *mePtr = (*meIter);
        bool wasUpdated = mePtr->wasUpdated();
        std::cout << "  " << mePtr->getPathname()
                  << "/"  << mePtr->getName()
                  << ", updated = "  << wasUpdated << std::endl;

        MonitorElementRootObject* rootObject = 
          dynamic_cast<MonitorElementRootObject *>((*meIter));
        if (rootObject) {
          sendMsg.WriteObject(rootObject->operator->());
          numObjects++;
        }
        else {
          FoldableMonitor *fm = dynamic_cast<FoldableMonitor *> (*meIter);
          if (fm) {
            sendMsg.WriteObject(fm->getTagObject());
            numObjects++;
          }
        }
      }

      TBuffer recvMsg(TBuffer::kRead, BUFF_SIZE, &transitBuff[0], false);
      for (int idx = 0; idx < numObjects /*meList.size()*/; idx++) {
        TObject *tmpPtr = recvMsg.ReadObject(TObject::Class());
        string cls = tmpPtr->IsA()->GetName();
        string nm = tmpPtr->GetName();
        std::cout << "receive message class = " << cls
                  << ", name = " << nm << std::endl;
      }
    }
  }
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
void DQMSenderService::findMonitorElements(std::vector<MonitorElement *> &put_here,
                                           std::string folderPath)
{
  if (bei == NULL) {return;}

  // fetch the monitor elements in the specified directory
  std::vector<MonitorElement *> localMEList = bei->getContents(folderPath);

  // add the local MEs to the list
  std::vector<MonitorElement *>::const_iterator meIter;
  for (meIter = localMEList.begin(); meIter != localMEList.end(); meIter++) {
    put_here.push_back(*meIter);
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
      findMonitorElements(put_here, subDirPath);
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
