/** \file
 *
 *  $Date: 2005/11/10 14:36:36 $
 *  $Revision: 1.3 $
 *  \author E. Meschi - CERN PH/CMD
 */

#include "EventFilter/FUReader/src/FUReader.h"
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>

#include <FWCore/EDProduct/interface/EventID.h>
#include <FWCore/EDProduct/interface/Timestamp.h>
#include <DataFormats/FEDRawData/interface/FEDRawData.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>

#include "EventFilter/Unit/interface/FUAdapter.h"
#include <FWCore/ParameterSet/interface/ParameterSet.h>

using namespace std;
using namespace edm;
#include <string.h>

FUReader::FUReader(const edm::ParameterSet& pset) : 
  runNum(1), eventNum(0) {
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
  if(sinking_)
    {
      pthread_mutex_lock(&lock_);
      pthread_cond_wait(&ready_,&lock_);
      pthread_mutex_unlock(&lock_);
    }      

  if(fwk_==0)
    {
      cerr << "FUReader::Error:Fatal no factory registered yet" << endl;
      exit(-1);
    }
  FURawEvent *event = fwk_->rqstEvent();
  runNum = fwk_->getRunNumber();
  eID = EventID(runNum,eventNum);
  eventNum++;



  fillFEDs(FEDNumbering::getSiPixelFEDIds(), data, *event);
  fillFEDs(FEDNumbering::getSiStripFEDIds(), data, *event);

  fillFEDs(FEDNumbering::getDTFEDIds(), data, *event);
  fillFEDs(FEDNumbering::getCSCFEDIds(), data, *event);
  fillFEDs(FEDNumbering::getRPCFEDIds(), data, *event);

  fillFEDs(FEDNumbering::getEcalFEDIds(), data, *event);
  fillFEDs(FEDNumbering::getHcalFEDIds(), data, *event);
  event->reset(true);
  return true;
}

void FUReader::fillFEDs(const pair<int,int>& fedRange,
			     FEDRawDataCollection& data,
			     FURawEvent &event)
{
  // Fill the EventID

  for (int fedId = fedRange.first; fedId <= fedRange.second; ++fedId ) 
    {
      FEDRawData& feddata = data.FEDData(fedId);
      // Allocate space for header+trailer+payload
      feddata.resize(event[fedId]->size_); 
      memcpy(feddata.data(),event[fedId]->data_,event[fedId]->size_);
    }  
}

#include "PluginManager/ModuleDef.h"
#include <IORawData/DaqSource/interface/DaqReaderPluginFactory.h>

DEFINE_SEAL_MODULE();
DEFINE_SEAL_PLUGIN (DaqReaderPluginFactory, FUReader, "FUReader");

