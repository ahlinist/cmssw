// File: SecondaryProducer.cc
// Description:  see SecondaryProducer.h
// Author:  Bill Tanenbaum
//
//--------------------------------------------

#include "IORawData/DaqSource/test/SecondaryProducer.h"
#include "FWCore/Framework/interface/TypeID.h"
#include "FWCore/Framework/interface/BasicHandle.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "DataFormats/Common/interface/ProductRegistry.h"
#include "FWCore/Framework/src/VectorInputSourceFactory.h"

#include <DataFormats/FEDRawData/interface/FEDRawData.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>

#include "PluginManager/ModuleDef.h"
#include <IORawData/DaqSource/interface/DaqReaderPluginFactory.h>


using namespace std;
using namespace evf;


////////////////////////////////////////////////////////////////////////////////
// construction/destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
SecondaryProducer::SecondaryProducer(const edm::ParameterSet& ps)
{
  // make secondary input source
  secInput_=makeSecInput(ps);
  //    produces<FEDRawDataCollection>();
}

//______________________________________________________________________________
SecondaryProducer::~SecondaryProducer()
{

}  


////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////
  
//______________________________________________________________________________
bool SecondaryProducer::fillRawData(edm::EventID& eID,
				    edm::Timestamp& tstamp, 
				    FEDRawDataCollection*& data)
{ 
  typedef  FEDRawDataCollection TC;
  typedef edm::Wrapper<TC> WTC;
  
  VectorInputSource::EventPrincipalVector result;
  secInput_->readMany(1,result);
  
  EventPrincipal *p  =&**result.begin();
  unsigned int irun  =p->id().run();
  unsigned int ievent=p->id().event();
  //std::cout << "run "   << p->id().run()
  //          << " event "<< p->id().event()<<std::endl;
  eID = EventID(irun, ievent);
  EDProduct const* ep = p->getByType(TypeID(typeid(TC))).wrapper();
  assert(ep);
  WTC const* wtp = dynamic_cast<WTC const*>(ep);
  assert(wtp);
  TC const* tp = wtp->product();
  //    auto_ptr<TC> thing(new TC(*tp));
  //data = *tp;
  
  data=new FEDRawDataCollection(*tp);
  
  return true;
}


//______________________________________________________________________________
boost::shared_ptr<VectorInputSource>
SecondaryProducer::makeSecInput(ParameterSet const& ps)
{
  ParameterSet sec_input = ps.getParameter<ParameterSet>("input");
  
  boost::shared_ptr<VectorInputSource> 
    input_(static_cast<VectorInputSource*>
	   (VectorInputSourceFactory::get()
	    ->makeVectorInputSource(sec_input,
				    InputSourceDescription()).release()));
  return input_;
}


////////////////////////////////////////////////////////////////////////////////
// SEAL Framework Macros
////////////////////////////////////////////////////////////////////////////////

DEFINE_SEAL_MODULE();
typedef evf::SecondaryProducer RawDataPlayback;
DEFINE_SEAL_PLUGIN(DaqReaderPluginFactory, RawDataPlayback, "RawDataPlayback");
