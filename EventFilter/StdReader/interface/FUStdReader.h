#ifndef FUSTDREADER_H
#define FUSTDREADER_H 1


#include "EventFilter/ResourceBroker/interface/FEDProvider.h"
#include "IORawData/DaqSource/interface/DaqBaseReader.h"
#include "DataFormats/Common/interface/EventID.h"


class FUStdReader : public DaqBaseReader
{
public:
  //
  //construction/destruction
  //
  FUStdReader(const edm::ParameterSet& pset);
  virtual ~FUStdReader();
  
  
  //
  // memeber functions
  //

  // DaqBaseReader interface
  bool fillRawData(edm::EventID& eID,
		   edm::Timestamp& tstamp, 
		   FEDRawDataCollection*& data);
  
  
private:
  //
  // member data
  //
  FEDRawDataCollection* event_;
  
  //edm::RunNumber_t    runNumber_;
  //edm::EventNumber_t  evtNumber_;
  unsigned int          runNumber_;
  unsigned int          evtNumber_;
  unsigned int          buResourceId_;
  
  evf::FEDProvider*     fedProvider_;
  
};


#endif
