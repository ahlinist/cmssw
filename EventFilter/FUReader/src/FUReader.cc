/** \file
 *
 *  $Date: 2006/06/13 14:09:00 $
 *  $Revision: 1.8 $
 *  \author E. Meschi - CERN PH/CMD
 */

#include "EventFilter/FUReader/src/FUReader.h"
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>

#include <DataFormats/Common/interface/EventID.h>
#include <DataFormats/Common/interface/Timestamp.h>
#include <DataFormats/FEDRawData/interface/FEDRawData.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>

#include "EventFilter/Unit/interface/FUAdapter.h"
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "PluginManager/ModuleDef.h"
#include <IORawData/DaqSource/interface/DaqReaderPluginFactory.h>

#include <string>


using namespace std;
using namespace edm;


////////////////////////////////////////////////////////////////////////////////
// construction/destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
FUReader::FUReader(const edm::ParameterSet& pset) 
  : runNum_(1)
  , eventNum_(0)
  , event_(0)
{
  cout<<"FUReader constructor "<<endl;
  // mean = pset.getParameter<float>("mean");
  pthread_mutex_init(&lock_,0);
  pthread_cond_init(&ready_,0);
}


//______________________________________________________________________________
FUReader::~FUReader(){}


////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
bool FUReader::fillRawData(EventID& eID,
			   Timestamp& tstamp, 
			   FEDRawDataCollection*& data)
{
  // EM FIXME: use logging + exception
  
  // check if a previous event is held. Release so discard can be issued.
  if(event_) {
    event_->reset(true);
    event_=0;
  }
  
  if(sinking_) {
    pthread_mutex_lock(&lock_);
    pthread_cond_wait(&ready_,&lock_);
    pthread_mutex_unlock(&lock_);
  }      
  
  // check that a pointer to the FilterUnitFramework [FUAdapter] is present
  if(fwk_==0) {
    edm::LogError("FUReader")<<"Fatal error: No factory registered yet";
    throw cms::Exception("NullPointer")<<"No factory registered for FUReader"<<endl;
  }
  
  // retrieve event data from FilterUnitFramework (via FUAdapter)
  event_=fwk_->rqstEvent();
  
  if(event_!=0) {
    // allocate fed collection
    data=new FEDRawDataCollection();
    
    // set collision id
    runNum_=fwk_->getRunNumber();
    unsigned int ievent=event_->getLevel1Id();
    eID=EventID(runNum_,ievent);
    eventNum_++;
    
    // fill fed buffers
    fillFEDs(0,FEDNumbering::lastFEDId(),*data,*event_);
    
    /*
      fillFEDs(FEDNumbering::getSiPixelFEDIds(), data, *event_);
      fillFEDs(FEDNumbering::getSiStripFEDIds(), data, *event_);
      
      fillFEDs(FEDNumbering::getDTFEDIds(), data, *event_);
      fillFEDs(FEDNumbering::getCSCFEDIds(), data, *event_);
      fillFEDs(FEDNumbering::getRPCFEDIds(), data, *event_);
      
      fillFEDs(FEDNumbering::getEcalFEDIds(), data, *event_);
      fillFEDs(FEDNumbering::getHcalFEDIds(), data, *event_);
    */
    
    // this should be done on return by the source with a new request
    // this indicates the previous event has been processed
    // event_->reset(true);
    
    return true;
  }
  
  return false;
}


//______________________________________________________________________________
void FUReader::fillFEDs(unsigned int b,unsigned int e,
			FEDRawDataCollection& data,
			FURawEvent &event)
{
  for (unsigned int fedId=b;fedId<=e;++fedId) {

    // retrieve fed data destination
    FURawEvent::RawData *rd=event[fedId];

    // check fed buffer size
    int sz = rd->size_;
    if(sz>0) {
      // check that fed it is valid
      if(!FEDNumbering::inRange(fedId)) {
	edm::LogInfo("FUReader")  
	  <<"WARNING: fed ID "<<fedId<<" contains data but is out of valid ranges.";
      }

      // retrieve the target buffer
      FEDRawData& feddata=data.FEDData(fedId);
      // Allocate space for header+trailer+payload
      feddata.resize(sz); 
      // copy the fed buffer
      memcpy(feddata.data(),event[fedId]->data_,sz);
    }  
  }
}


//______________________________________________________________________________
void FUReader::onShutDown()
{
  if(fwk_==0) {
    edm::LogError("FUReader")<<"Fatal error: No factory registered yet";
    throw cms::Exception("NullPointer")<< "No factory registered for FUReader"<<endl;
  }
  fwk_->signalWaitingInput();
}

DEFINE_SEAL_MODULE();
DEFINE_SEAL_PLUGIN (DaqReaderPluginFactory,FUReader,"FUReader");

