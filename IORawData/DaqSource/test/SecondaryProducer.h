#ifndef SecondaryInput_SecondaryProducer_h
#define SecondaryInput_SecondaryProducer_h

/** \class SecondaryProducer
 *
 * \author Bill Tanenbaum
 *
 *
 ************************************************************/

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/VectorInputSource.h"
#include "IORawData/DaqSource/interface/DaqBaseReader.h"

using namespace edm;

namespace evf {

  class SecondaryProducer: public DaqBaseReader
  {
  public:
    //
    // construction/destruction
    //
    explicit SecondaryProducer(const edm::ParameterSet& ps);
    virtual ~SecondaryProducer();
    
    
    //
    // public member functions
    //
    
    // DaqBaseReader interface
    virtual bool fillRawData(edm::EventID& eID,
			     edm::Timestamp& tstamp, 
			     FEDRawDataCollection*& data);
    
    
  private:
    //
    // private member functions
    //
    boost::shared_ptr<VectorInputSource> makeSecInput(ParameterSet const& ps);


  private:
    //
    // member data
    //
    edm::RunNumber_t   runNum;
    edm::EventNumber_t eventNum;

    boost::shared_ptr<VectorInputSource> secInput_;

  };

} // evf


#endif
