////////////////////////////////////////////////////////////////////////////////
//
// FUStdReader
// -----------
//
//            11/03/2006 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "EventFilter/StdReader/interface/FUStdReader.h"

#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "IORawData/DaqSource/interface/DaqReaderPluginFactory.h"
#include "PluginManager/ModuleDef.h"


#include <iostream>


using namespace std;
using namespace edm;


////////////////////////////////////////////////////////////////////////////////
// construction/destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
FUStdReader::FUStdReader(const edm::ParameterSet& pset)
  : event_(0)
  , runNumber_(0)
  , evtNumber_(0)
  , buResourceId_(0)
  , fedProvider_(0)
{
  fedProvider_=evf::FEDProvider::instance();
}


//______________________________________________________________________________
FUStdReader::~FUStdReader()
{
  
}


////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
bool FUStdReader::fillRawData(EventID& eID,
			      Timestamp& tstamp, 
			      FEDRawDataCollection*& data)
{
  if (0==fedProvider_) fedProvider_==evf::FEDProvider::instance();
  
  if(0==fedProvider_) {
    edm::LogError("FUStdReader")<<"No FEDProvider!";
    throw cms::Exception("NullPointer")<<"FUStdReader: No FEDProvider"<<endl;
  }
  
  if(0!=event_) {
    fedProvider_->sendDiscard(buResourceId_);
    event_=0;
  }
  
  event_=fedProvider_->rqstEvent(evtNumber_,buResourceId_);
  
  if (0!=event_) {
    eID=EventID(runNumber_,evtNumber_);
    data=event_;
    return true;
  }
  
  return false;
}


////////////////////////////////////////////////////////////////////////////////
// CMSSW framwork macros
////////////////////////////////////////////////////////////////////////////////

DEFINE_SEAL_MODULE();
DEFINE_SEAL_PLUGIN (DaqReaderPluginFactory,FUStdReader,"FUStdReader");
