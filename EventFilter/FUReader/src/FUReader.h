#ifndef DaqSource_FUReader_h
#define DaqSource_FUReader_h

/** \class FUReader
 *  $Date:$
 *  $Revision:$
 *  \author E. Meschi - CERN PH/CMD
 */
#include "EventFilter/Unit/interface/FUAdapter.h"
#include "EventFilter/Unit/interface/FURawEvent.h"
#include "EventFilter/Unit/interface/EventSink.h"
#include <IORawData/DaqSource/interface/DaqBaseReader.h>
#include <FWCore/EDProduct/interface/EventID.h>
#include <algorithm>


class FUReader : public DaqBaseReader, public EventSink {
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
 private:
  void fillFEDs(const std::pair<int,int>&,
		FEDRawDataCollection&,
		FURawEvent &);

  edm::RunNumber_t runNum;
  edm::EventNumber_t eventNum;

};
#endif

