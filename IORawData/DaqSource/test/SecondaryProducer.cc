// File: SecondaryProducer.cc
// Description:  see SecondaryProducer.h
// Author:  Bill Tanenbaum
//
//--------------------------------------------

#include "SecondaryProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h" 

#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "FWCore/Utilities/interface/TypeID.h"
#include "DataFormats/Common/interface/BasicHandle.h"
#include "DataFormats/Common/interface/ConvertHandle.h"
#include "FWCore/Sources/interface/VectorInputSourceFactory.h"

#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>

#include <IORawData/DaqSource/interface/DaqReaderPluginFactory.h>

using namespace std;
using namespace evf;
using namespace edm;

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
int SecondaryProducer::fillRawData(edm::EventID& eID,
				    edm::Timestamp& tstamp, 
				    FEDRawDataCollection*& data)
{ 
  typedef  FEDRawDataCollection TC;
  typedef edm::Wrapper<TC> WTC;
  
  VectorInputSource::EventPrincipalVector result;
  secInput_->readMany(1,result);
  
  EventPrincipal *p  =&**result.begin();
  unsigned int irun  =p->id().run();
  unsigned int ilumi  =p->id().luminosityBlock();
  unsigned int ievent=p->id().event();
  //std::cout << "run "   << p->id().run()
  //          << " event "<< p->id().event()<<std::endl;
  eID = EventID(irun, ilumi, ievent);

  BasicHandle bh = p->getByType(TypeID(typeid(TC)));
  assert(bh.isValid());
  if(!(bh.interface()->dynamicTypeInfo() == typeid(TC))) {
    handleimpl::throwConvertTypeError(typeid(TC), bh.interface()->dynamicTypeInfo());
  }
  WTC const* wtp = static_cast<WTC const*>(bh.wrapper());
  assert(wtp);
  TC const* tp = wtp->product();
  //    auto_ptr<TC> thing(new TC(*tp));
  //data = *tp;
  
  data=new FEDRawDataCollection(*tp);
  
  return 1;
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
// Framework Macros
////////////////////////////////////////////////////////////////////////////////

typedef evf::SecondaryProducer RawDataPlayback;
DEFINE_EDM_PLUGIN(DaqReaderPluginFactory, RawDataPlayback, "RawDataPlayback");
