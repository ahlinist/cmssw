/** \file
 *
 *  $Date: 2006/05/29 15:26:43 $
 *  $Revision: 1.7 $
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


using namespace std;
using namespace edm;
#include <string.h>

FUReader::FUReader(const edm::ParameterSet& pset) : 
  runNum(1), eventNum(0), event(0) {
  cout << "FUReader constructor " << endl;
  // mean = pset.getParameter<float>("mean");
  pthread_mutex_init(&lock_,0);
  pthread_cond_init(&ready_,0);
}


FUReader::~FUReader(){}


bool FUReader::fillRawData(EventID& eID,
			   Timestamp& tstamp, 
			   FEDRawDataCollection& data){
  //EM FIXME: use logging + exception
  // check if a previous event is held. Release so discard can be issued.
  if(event)
    {
      event->reset(true);
      event = 0;
    }
  if(sinking_)
    {
      pthread_mutex_lock(&lock_);
      pthread_cond_wait(&ready_,&lock_);
      pthread_mutex_unlock(&lock_);
    }      

  if(fwk_==0)
    {
      edm::LogError("FUReader")  << "Fatal error: No factory registered yet";
      throw cms::Exception("NullPointer") 
	<< "No factory registered yet for FUReader" << std::endl;
    }
  event = fwk_->rqstEvent();
  if(event != 0)
    {
      runNum = fwk_->getRunNumber();
      unsigned int ievent = event->getLevel1Id();
      eID = EventID(runNum,ievent);
      eventNum++;
      
      fillFEDs(0,FEDNumbering::lastFEDId(), data,*event);
  /*
  fillFEDs(FEDNumbering::getSiPixelFEDIds(), data, *event);
  fillFEDs(FEDNumbering::getSiStripFEDIds(), data, *event);

  fillFEDs(FEDNumbering::getDTFEDIds(), data, *event);
  fillFEDs(FEDNumbering::getCSCFEDIds(), data, *event);
  fillFEDs(FEDNumbering::getRPCFEDIds(), data, *event);

  fillFEDs(FEDNumbering::getEcalFEDIds(), data, *event);
  fillFEDs(FEDNumbering::getHcalFEDIds(), data, *event);
  */
      // this should be done on return by the source with a new request
      // this indicates the previous event has been processed
      //      event->reset(true);
      return true;
    }
  return false;
}

void FUReader::fillFEDs(int b, int e,
			FEDRawDataCollection& data,
			FURawEvent &event)
{
  // Fill the EventID

  for (int fedId = b; fedId <= e; ++fedId ) 
    {
      FURawEvent::RawData *rd = event[fedId];
      int sz = rd->size_;
      if(sz > 0)
	{
	  if(!FEDNumbering::inRange(fedId))
	    edm::LogInfo("FUReader")  
	      << "Severe error: fed ID " << fedId 
	      << " contains data but is out of valid ranges. ";	    
	  FEDRawData& feddata = data.FEDData(fedId);
	  // Allocate space for header+trailer+payload
	  feddata.resize(sz); 
	  memcpy(feddata.data(),event[fedId]->data_,sz);
	}  

    }
}

void FUReader::onShutDown()
{
  if(fwk_==0)
    {
      edm::LogError("FUReader")  << "Fatal error: No factory registered yet";
      throw cms::Exception("NullPointer") 
	<< "No factory registered yet for FUReader" << std::endl;
    }
  fwk_->signalWaitingInput();
}

#include "PluginManager/ModuleDef.h"
#include <IORawData/DaqSource/interface/DaqReaderPluginFactory.h>

DEFINE_SEAL_MODULE();
DEFINE_SEAL_PLUGIN (DaqReaderPluginFactory, FUReader, "FUReader");

