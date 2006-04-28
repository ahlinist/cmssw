#include "PhysicsTools/ttHAnalysis/interface/TTHInputService.h"
#include "FWCore/Framework/interface/ProductRegistry.h"
#include "FWCore/EDProduct/interface/Timestamp.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/EDProduct/interface/Wrapper.h"

using namespace edm;
using namespace tth;
using namespace std;

//used for defaults
static const unsigned long kNanoSecPerSec = 1000000000;
static const unsigned long kAveEventPerSec = 200;

TTHInputService::TTHInputService( const ParameterSet & pset, 
				  InputSourceDescription const& desc ) :
  InputSource( desc ) , 
  remainingEvents_(pset.getUntrackedParameter<int>("maxEvents", -1)), 
  numberEventsInRun_(pset.getUntrackedParameter<unsigned int>("numberEventsInRun", remainingEvents_+1)),
  presentRun_( pset.getUntrackedParameter<unsigned int>("firstRun",1)  ),
  nextTime_(pset.getUntrackedParameter<unsigned int>("firstTime",1)),  //time in ns
  timeBetweenEvents_(pset.getUntrackedParameter<unsigned int>("timeBetweenEvents",kNanoSecPerSec/kAveEventPerSec) ),
  numberEventsInThisRun_(0),
  nextID_(presentRun_, 1 ),    
  //  retriever_( new FakeRetriever() ),
  reader_( pset.getParameter<std::string>( "fileName" ) ) {
  cout << "ttHAnalysis InputService: starting " << endl;
  ModuleDescription      modDesc_; 
  modDesc_.pid = PS_ID("TTHInputService");
  modDesc_.moduleName_ = "TTHInputService";
  modDesc_.moduleLabel_ = "TTHInput";
  modDesc_.versionNumber_ = 1UL;
  modDesc_.processName_ = "PROD";
  modDesc_.pass = 1UL;  
    
  prodDesc_.module = modDesc_;   
  prodDesc_.fullClassName_= "NtupleData";
  prodDesc_.friendlyClassName_ = "NtupleData";   
  preg_->addProduct( prodDesc_ );
}

auto_ptr<EventPrincipal> TTHInputService::read() {
  
  auto_ptr<EventPrincipal> result( 0 );

  // event loop
  if ( remainingEvents_-- != 0 ) {
    
    reader_.readNext();
    result = auto_ptr<EventPrincipal>( new EventPrincipal( nextID_, Timestamp( nextTime_ ),
							   *preg_ ) );
    //							   * retriever_, *preg_ ) );

    cout << "ttHAnalysis InputService: event: " << nextID_ << endl;

    //    NtupleData * data = new NtupleData( reader_.data_ );
    std::auto_ptr<NtupleData> data( new NtupleData( reader_.data_ ) );
    Wrapper<NtupleData> * wrapper = new Wrapper<NtupleData> ( data ); 
    auto_ptr<EDProduct>  prod( wrapper );
    auto_ptr<Provenance> prov( new Provenance( prodDesc_ ) );
    result->put( prod, prov );

    if( ++numberEventsInThisRun_ < numberEventsInRun_ ) {
      nextID_ = nextID_.next();
    } else {
      nextID_ = nextID_.nextRunFirstEvent();
      numberEventsInThisRun_ = 0;
    }
    nextTime_ += timeBetweenEvents_;
  }
  return result;
}
