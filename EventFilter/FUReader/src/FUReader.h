#ifndef DaqSource_FUReader_h
#define DaqSource_FUReader_h

/** \class FUReader
 *  $Date: 2006/06/13 14:05:07 $
 *  $Revision: 1.6 $
 *  \author E. Meschi - CERN PH/CMD
 */
#include "EventFilter/Unit/interface/FURawEvent.h"
#include "EventFilter/Unit/interface/EventSink.h"
#include "EventFilter/Utilities/interface/ShutDownListener.h"
#include <IORawData/DaqSource/interface/DaqBaseReader.h>
#include <DataFormats/Common/interface/EventID.h>
#include <algorithm>
#include <pthread.h>


class FUReader : public DaqBaseReader,
		 public EventSink,
		 public evf::ShutDownListener
{
public:
  //
  // construction/destruction
  //
  FUReader(const edm::ParameterSet& pset);
  virtual ~FUReader();

  
  //
  // public member functions
  //
  
  /// DaqBaseReader interface, fill fed collection 'data'
  virtual bool fillRawData(edm::EventID& eID,
			   edm::Timestamp& tstamp, 
			   FEDRawDataCollection*& data);

  // EventSink interface
  void sink(){}

  // evf::ShutDownListener interface
  void onShutDown();
  
  
 private:
  //
  // private member functions
  //
  void fillFEDs(unsigned int b,unsigned int e,
		FEDRawDataCollection& data,
		FURawEvent& event);
  
private:
  //
  // member data
  //
  edm::RunNumber_t   runNum_;
  edm::EventNumber_t eventNum_;
  pthread_cond_t     ready_;
  pthread_mutex_t    lock_;
  FURawEvent        *event_;

};

#endif
