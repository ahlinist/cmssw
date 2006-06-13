#ifndef DaqSource_FUReader_h
#define DaqSource_FUReader_h

/** \class FUReader
 *  $Date: 2006/05/16 10:46:22 $
 *  $Revision: 1.5 $
 *  \author E. Meschi - CERN PH/CMD
 */
#include "EventFilter/Unit/interface/FURawEvent.h"
#include "EventFilter/Unit/interface/EventSink.h"
#include "EventFilter/Utilities/interface/ShutDownListener.h"
#include <IORawData/DaqSource/interface/DaqBaseReader.h>
#include <DataFormats/Common/interface/EventID.h>
#include <algorithm>
#include <pthread.h>

class FUReader : public DaqBaseReader, public EventSink, public evf::ShutDownListener {
 public:
  /// Constructor
  FUReader(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~FUReader();

  /// Generate and fill FED raw data for a full event
  virtual bool fillRawData(edm::EventID& eID,
			   edm::Timestamp& tstamp, 
			   FEDRawDataCollection& data);

  void sink(){}
  void onShutDown();

 private:
  void fillFEDs(int, int,
		FEDRawDataCollection&,
		FURawEvent &);

  edm::RunNumber_t runNum;
  edm::EventNumber_t eventNum;
  pthread_cond_t ready_;
  pthread_mutex_t lock_;
  FURawEvent *event;
};
#endif

