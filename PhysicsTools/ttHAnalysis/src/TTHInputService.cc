#include "PhysicsTools/ttHAnalysis/interface/TTHInputService.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Wrapper.h"

using namespace edm;
using namespace tth;
using namespace std;

//used for defaults
static const unsigned long kNanoSecPerSec = 1000000000;
static const unsigned long kAveEventPerSec = 200;

TTHInputService::TTHInputService( ParameterSet const& pset, 
				  InputSourceDescription const& desc ) :
  GeneratedInputSource( pset, desc ) , 
  remainingEvents_(pset.getUntrackedParameter<int>("maxEvents", -1)), 
  numberEventsInRun_(pset.getUntrackedParameter<unsigned int>("numberEventsInRun", remainingEvents_+1)),
  presentRun_( pset.getUntrackedParameter<unsigned int>("firstRun",1)  ),
  nextTime_(pset.getUntrackedParameter<unsigned int>("firstTime",1)),  //time in ns
  timeBetweenEvents_(pset.getUntrackedParameter<unsigned int>("timeBetweenEvents",kNanoSecPerSec/kAveEventPerSec) ),
  numberEventsInThisRun_(0),
  nextID_(presentRun_, 1 ), 
  reader_( pset.getParameter<std::string>( "fileName" ) ) {
  cout << "ttHAnalysis InputService: starting " << endl;
  //  ModuleDescription      modDesc_; 
  //  modDesc_.pid = PS_ID("TTHInputService");
  //  modDesc_.moduleName_ = "TTHInputService";
  //  modDesc_.moduleLabel_ = "TTHInput";
  //  modDesc_.versionNumber_ = 1UL;
  //  modDesc_.processName_ = "PROD";
  //  modDesc_.pass = 1UL;  
    
  //  prodDesc_.module = modDesc_;   
  //  prodDesc_.fullClassName_= "NtupleData";
  //  prodDesc_.friendlyClassName_ = "NtupleData";   
  produces<NtupleData>();
}

bool TTHInputService::produce(Event& e)
{
  if ( remainingEvents_-- != 0 )                                                                                
    reader_.readNext();
  
  cout << "InputService NumbLep : " << reader_.data_.NumbLep << endl;
  std::auto_ptr<NtupleData> data( new NtupleData( reader_.data_ ) );
  
  e.put( data );

  return true;
}

