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
#include "DQMServices/CoreROOT/interface/MonitorElementRootT.h"
#include "FWCore/Utilities/interface/GetReleaseVersion.h"
#include "DQMServices/NodeROOT/interface/SenderBase.h"
#include "EventFilter/FUSender/interface/FUi2oSender.h"
#include "EventFilter/Utilities/interface/i2oEvfMsgs.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "i2o/Method.h"
#include "i2o/utils/AddressMap.h"
#include "toolbox/mem/MemoryPoolFactory.h"
#include "xcept/tools.h"

extern xdaq::Application* getMyXDAQPtr();
extern toolbox::mem::Pool *getMyXDAQPool();
extern xdaq::ApplicationDescriptor* getMyXDAQDest();
extern xdaq::ApplicationDescriptor* getMyXDAQDest(unsigned int);

using namespace std;
using namespace toolbox::mem;

/**
 * Local debug flag.  0 => no debug output; 1 => tracing of necessary
 * routines; 2 => additional tracing of many framework signals;
 * 3 => deserialization test.
 */
#define DSS_DEBUG 0

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
  int initialSize =
    pset.getUntrackedParameter<int>("initialMessageBufferSize", 1000000);
  messageBuffer_.resize(initialSize);
  lumiSectionsPerUpdate_ = pset.getParameter<double>("lumiSectionsPerUpdate");
  // for the moment, only support a number of lumi sections per update >= 1
  if (lumiSectionsPerUpdate_ <= 1.0) {lumiSectionsPerUpdate_ = 1.0;}
  initializationIsNeeded_ = true;
  useCompression_ = pset.getParameter<bool>("useCompression");
  compressionLevel_ = pset.getParameter<int>("compressionLevel");
  lumiSectionInterval_ =
    pset.getUntrackedParameter<int>("lumiSectionInterval", 60); // seconds
  if (lumiSectionInterval_ < 1) {lumiSectionInterval_ = 1;}

  i2o_max_size_ = pset.getUntrackedParameter<int>("i2o_max_size",I2O_MAX_SIZE);
  // check the max i20 frame size is not above the value that causes a crash!
  if(i2o_max_size_ > I2O_ABSOLUTE_MAX_SIZE) {
    int old_i2o_max_size = i2o_max_size_;
    i2o_max_size_ = I2O_ABSOLUTE_MAX_SIZE;
    edm::LogWarning("DQMSenderService")
      << "user defined i2o_max_size too large for xdaq tcp, changed from " 
      << old_i2o_max_size << " to " << i2o_max_size_;
  }
  // check the total i20 frame size is a multiple of 64 bits (8 bytes)
  if((i2o_max_size_ & 0x7) != 0) {
    int old_i2o_max_size = i2o_max_size_;
    // round it DOWN as this is the maximum size (keep the 0 for illustration!)
    i2o_max_size_ = ((i2o_max_size_ >> 3) + 0) << 3;
    edm::LogWarning("DQMSenderService")
      << "user defined i2o_max_size not multiple of 64 bits, changed from " 
      << old_i2o_max_size << " to " << i2o_max_size_;
  }
  // get the actual max data size for DQM frames
  max_i2o_DQM_datasize_ = i2o_max_size_ - sizeof(I2O_SM_DQM_MESSAGE_FRAME); 

  thisXdaqApplication_ = getMyXDAQPtr();
  xdaqMemPool_ = getMyXDAQPool();
  int smInstance = pset.getUntrackedParameter<int>("smInstance", -1);
  if (smInstance < 0) {
    smXdaqDestination_ = getMyXDAQDest();
  }
  else {
    smXdaqDestination_ = getMyXDAQDest(smInstance);
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
  // 06-Mar-2006, KAB: fake the luminosity section until the real one is available
  time_t now = time(NULL);
  //edm::LuminosityBlockID thisLumiSection=(int) (now / lumiSectionInterval_);
  unsigned int thisLumiSection = (int) (now / lumiSectionInterval_);

  if (DSS_DEBUG) {
    cout << "DQMSenderService::postEventProcessing called, event number "
         << event.id().event() << ", lumi section "
         << thisLumiSection << endl;
  }

  // special handling for the first event
  if (initializationIsNeeded_) {
    initializationIsNeeded_ = false;
    lumiSectionOfPreviousUpdate_ = thisLumiSection;
    firstLumiSectionSeen_ = thisLumiSection;
  }

  // only continue if the correct number of luminosity sections have passed
  int lsDelta = (int) (thisLumiSection - lumiSectionOfPreviousUpdate_);
  double updateRatio = ((double) lsDelta) / lumiSectionsPerUpdate_;
  if (updateRatio < 1.0) {return;}

  // calculate the update ID and lumi ID for this update
  int fullLsDelta = (int) (thisLumiSection - firstLumiSectionSeen_);
  double fullUpdateRatio = ((double) fullLsDelta) / lumiSectionsPerUpdate_;
  uint32 updateNumber = -1 + (uint32) fullUpdateRatio;
  //edm::LuminosityBlockID lumiSectionTag = firstLumiSectionSeen_ +
  unsigned int lumiSectionTag = firstLumiSectionSeen_ +
    ((int) (updateNumber * lumiSectionsPerUpdate_)) - 1;

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
  std::map< std::string, DQMEvent::TObjectTable > toMap;
  std::vector<std::string>::const_iterator dirIter;
  for (dirIter = topLevelFolderList.begin();
       dirIter != topLevelFolderList.end();
       dirIter++) {
    std::string dirName = *dirIter;
    DQMEvent::TObjectTable toTable;

    // find the MEs
    findMonitorElements(toTable, dirName);

    // store the list in the map
    toMap[dirName] = toTable;
  }

  // create a DQMEvent message for each top-level folder
  // and send each to the SM
  for (dirIter = topLevelFolderList.begin();
       dirIter != topLevelFolderList.end();
       dirIter++) {
    std::string dirName = *dirIter;
    DQMEvent::TObjectTable toTable = toMap[dirName];
    if (toTable.size() == 0) {continue;}

    // serialize the monitor element data
    serializeWorker_.serializeDQMEvent(toTable, useCompression_,
                                       compressionLevel_);

    // resize the message buffer, if needed 
    unsigned int srcSize = serializeWorker_.currentSpaceUsed();
    unsigned int newSize = srcSize + 50000;  // allow for header
    if (messageBuffer_.size() < newSize) messageBuffer_.resize(newSize);

    // create the message
    DQMEventMsgBuilder dqmMsgBuilder(&messageBuffer_[0], messageBuffer_.size(),
                                     event.id().run(), event.id().event(),
                                     event.time(),
                                     lumiSectionTag, updateNumber,
                                     edm::getReleaseVersion(), dirName,
                                     toTable);

    // copy the serialized data into the message
    unsigned char* src = serializeWorker_.bufferPointer();
    std::copy(src,src + srcSize, dqmMsgBuilder.eventAddress());
    dqmMsgBuilder.setEventLength(srcSize);
    if (useCompression_) {
      dqmMsgBuilder.setCompressionFlag(serializeWorker_.currentEventSize());
    }

    // send the message
    writeI2ODQMData(dqmMsgBuilder);

    // test deserialization
    if (DSS_DEBUG >= 3) {
      DQMEventMsgView dqmEventView(&messageBuffer_[0]);
      std::cout << "  DQM Message data:" << std::endl; 
      std::cout << "    protocol version = "
                << dqmEventView.protocolVersion() << std::endl; 
      std::cout << "    header size = "
                << dqmEventView.headerSize() << std::endl; 
      std::cout << "    run number = "
                << dqmEventView.runNumber() << std::endl; 
      std::cout << "    event number = "
                << dqmEventView.eventNumberAtUpdate() << std::endl; 
      std::cout << "    lumi section = "
                << dqmEventView.lumiSection() << std::endl; 
      std::cout << "    update number = "
                << dqmEventView.updateNumber() << std::endl; 
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
      std::auto_ptr<DQMEvent::TObjectTable> toTablePtr =
        deserializeWorker_.deserializeDQMEvent(dqmEventView);
      DQMEvent::TObjectTable::const_iterator toIter;
      for (toIter = toTablePtr->begin();
           toIter != toTablePtr->end(); toIter++) {
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
  }

  // reset monitor elements that have requested it
  // TODO - enable this
  //bei->doneSending(true, true);

  // update the "previous" lumi section
  lumiSectionOfPreviousUpdate_ = thisLumiSection;
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
void DQMSenderService::findMonitorElements(DQMEvent::TObjectTable &toTable,
                                           std::string folderPath)
{
  if (bei == NULL) {return;}

  // fetch the monitor elements in the specified directory
  std::vector<MonitorElement *> localMEList = bei->getContents(folderPath);
  //MonitorElementRootFolder* folderPtr = bei->getDirectory(folderPath);

  // add the MEs that should be updated to the table
  std::vector<TObject *> updateTOList;
  for (int idx = 0; idx < (int) localMEList.size(); idx++) {
    MonitorElement *mePtr = localMEList[idx];
    bool wasUpdated = mePtr->wasUpdated();
    // TODO - enable the correct "never-sent" determination
    bool neverSent = false; //SenderBase::isNeverSent(folderPtr, mePtr->getName());
    if (wasUpdated || neverSent) {
      MonitorElementRootObject* rootObject = 
        dynamic_cast<MonitorElementRootObject *>(mePtr);
      if (rootObject) {
        updateTOList.push_back(rootObject->operator->());
      }
      else {
        FoldableMonitor *foldable =
          dynamic_cast<FoldableMonitor *>(mePtr);
        if (foldable) {
          updateTOList.push_back(foldable->getTagObject());
        }
        else {
          std::cerr << " *** Failed to extract and send object " 
                    << mePtr->getName() << std::endl;
        }
      }

      if (wasUpdated) {
        // TODO - enable this
        //bei->add2UpdatedContents(mePtr->getName(), folderPath);
      }

      if (neverSent) {
        // TODO - enable this
        //SenderBase::setNeverSent(folderPtr, mePtr->getName(), false);
      }
    }
  }
  if (updateTOList.size() > 0) {
    toTable[folderPath] = updateTOList;
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
      findMonitorElements(toTable, subDirPath);
    }
  }
}

/**
 * Sends the specified DQM event message to the storage manager.
 */
void DQMSenderService::writeI2ODQMData(DQMEventMsgBuilder const& dqmMsgBuilder)
{
  // fetch the location and size of the message buffer
  char* buffer = (char*) dqmMsgBuilder.startAddress();
  unsigned int size = dqmMsgBuilder.size();

  // fetch the run, event, and folder number for addition to the I2O fragments
  DQMEventMsgView dqmMsgView(buffer);
  edm::RunNumber_t runid = dqmMsgView.runNumber();
  edm::EventNumber_t eventid = dqmMsgView.eventNumberAtUpdate();

  // create the chain of I2O fragments
  toolbox::mem::Reference *head = 0;
  unsigned int numBytesSent = 0;
  head = FUi2oSender::createI2OFragmentChain(buffer, size,
                                             xdaqMemPool_,
                                             max_i2o_DQM_datasize_,
                                             sizeof(I2O_SM_DQM_MESSAGE_FRAME),
                                             (unsigned short)I2O_SM_DQM,
                                             thisXdaqApplication_,
                                             smXdaqDestination_,
                                             numBytesSent);

  // set the run and event numbers in each I2O fragment
  toolbox::mem::Reference *currentElement = head;
  while (currentElement != NULL) {
    I2O_SM_DQM_MESSAGE_FRAME *dataMsg =
      (I2O_SM_DQM_MESSAGE_FRAME *) currentElement->getDataLocation();
    dataMsg->runID = (unsigned int) runid;
    dataMsg->eventAtUpdateID = (unsigned int) eventid;
    currentElement = currentElement->getNextReference();
  }

  if (head != NULL) {
    if (smXdaqDestination_ != NULL) {
      try {
        thisXdaqApplication_->getApplicationContext()->
          postFrame(head,
                    thisXdaqApplication_->getApplicationDescriptor(),
                    smXdaqDestination_);
      }
      catch(xcept::Exception &excpt) {
        edm::LogError("writeI2ORegistry")
          << "Exception writeI2ODQMData postFrame" 
          << xcept::stdformat_exception_history(excpt);
        throw cms::Exception("CommunicationError",excpt.message());
      }
    }
    else
      edm::LogError("writeI2ORegistry") 
        << "DQMSenderService::writeI2ODQMData: No StorageManager "
        << "destination in configuration";
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
