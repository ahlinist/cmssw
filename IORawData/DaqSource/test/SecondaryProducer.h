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
  class SecondaryProducer: public DaqBaseReader{
  public:
    
    /** standard constructor*/
    explicit SecondaryProducer(const edm::ParameterSet& ps);
    
    /**Default destructor*/
    virtual ~SecondaryProducer();
    
    
    virtual bool fillRawData(edm::EventID& eID,
			     edm::Timestamp& tstamp, 
			     FEDRawDataCollection& data);
  private:
    
    edm::RunNumber_t runNum;
    edm::EventNumber_t eventNum;
    
    boost::shared_ptr<VectorInputSource> makeSecInput(ParameterSet const& ps);
    
    boost::shared_ptr<VectorInputSource> secInput_;
  };
}//edm

#endif
