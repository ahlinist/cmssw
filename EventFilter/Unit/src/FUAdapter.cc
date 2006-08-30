#include "EventFilter/Unit/interface/FUAdapter.h"

#include "i2o/include/i2o/Method.h"
#include "xdaq/include/xdaq/ApplicationGroup.h"


#include "EventFilter/Unit/interface/BUProxy.h"
#include "EventFilter/Unit/interface/FURawEventFactory.h"
#include "EventFilter/Unit/interface/EventSink.h"


FUAdapter::FUAdapter(xdaq::ApplicationStub *s, string buClassName, 
		     unsigned long outsize) : xdaq::Application(s),
  factory_(0), buName_(buClassName), bufSize_(outsize), bSem_(BSem::FULL), 
  pendingRequests_(0), nbReceivedFragments_(0), nbReceivedEvents_(0), 
					      nbDataErrors_(0), 
					      nbCrcErrors_(0), 
					      doDumpFragments_(false), 
					      doDropFragments_(false),
					      doCrcCheck_(0),
					      nAllocateSent_(0),
					      nAllocatedEvents_(0), 
					      nTakeReceived_(0),
					      nDiscardSent_(0),
					      sink_(0)
{
  i2o::bind(this, 
	    &FUAdapter::realTake, 
	    I2O_FU_TAKE,
	    XDAQ_ORGANIZATION_ID);
  poolName_ = "";
  exportCommonParameters();
  xmlClass_ = getApplicationDescriptor()->getClassName();
  instance_ = getApplicationDescriptor()->getInstance();
  ostringstream sourcename;
  sourcename << xmlClass_ << "_" << instance_;
  sourceId_ = sourcename.str();
  FURawEventFactory::adapter_ = this;
  factory_ = new FURawEventFactory();

}

FUAdapter::~FUAdapter()
{
  if(factory_) delete factory_;
  if(sink_) delete sink_;
  for(unsigned int i = 0; i < bu_.size(); i++)
    {
      delete bu_[i];
    }
  bu_.clear();
}

void FUAdapter::clearBUArray()
{
  for(unsigned int i = 0; i < bu_.size(); i++)
    {
      delete bu_[i];
    }
  bu_.clear();
}

#include "xcept/include/xcept/tools.h"
#include "toolbox/include/toolbox/fsm/exception/Exception.h"

void FUAdapter::createBUArray()
  {
    vector<xdaq::ApplicationDescriptor*> buDescs;
    
    try{
      buDescs= 
	getApplicationContext()->getApplicationGroup()->
	getApplicationDescriptors(buName_);
      unsigned int maxBUinstance = 0;
      
      for (unsigned int ind=0; ind < buDescs.size(); ind++) 
	{
	  if(buDescs[ind]->getInstance() > maxBUinstance)
	    maxBUinstance = buDescs[ind]->getInstance();
	}
      unsigned buinstance = 0;
      bu_.resize(maxBUinstance+1);
      for (buinstance = 0; buinstance <= maxBUinstance; buinstance++) 
	bu_[buinstance] = 0;
      for (buinstance = 0; buinstance <= maxBUinstance; buinstance++) 
	{
	  unsigned int i = 0;
	  for(; i < buDescs.size(); i++)
	    if(buinstance==buDescs[i]->getInstance())
	      break;
	  LOG4CPLUS_INFO(this->getApplicationLogger(),"FUAdapter::BU descriptor for instance " << buinstance 
			 << " at " << hex << (int) buDescs[buinstance] << dec);
	  bu_[buinstance] = new BUProxy (
					 getApplicationDescriptor() , 
					 buDescs[i], 
					 getApplicationContext(),
					 pool_);
	} 
      
      
    }
    catch(xcept::Exception &e)
      {
	LOG4CPLUS_ERROR(this->getApplicationLogger(),
			"Exception in building BU array" 
			<< xcept::stdformat_exception_history(e));
	XCEPT_RETHROW(toolbox::fsm::exception::Exception, "Exception in building BU array", e);
      }
    catch(...)
      {
	LOG4CPLUS_FATAL(this->getApplicationLogger(),
			"Unknown error in looking up connectable BUs");
      } 
    
    LOG4CPLUS_INFO(this->getApplicationLogger(),
		   "Memory pool for BU messages at " << hex
		   << (int) pool_ << dec);
    
    
    LOG4CPLUS_INFO(this->getApplicationLogger(),"Connected " << bu_.size()
		   << " Builder Units ");
    
    if(sink_) delete sink_; sink_ = 0;
    if(doDropFragments_ && (sink_==0))
      {
	LOG4CPLUS_INFO(this->getApplicationLogger(),
		       "FU Running in drop mode");
	sink_ = new EventSink();
	EventSink::sinking_ = true;
      }

    EventSink::setFwk(this);
    
  }


#include "EventFilter/Unit/interface/FURawEvent.h"

/** the actual message slot, reimplemented from FUListener */
void FUAdapter::realTake(toolbox::mem::Reference *bufRef)
{
  nTakeReceived_++;
  bSem_.take();
  int currentFragment = -1;
  bool errorFound = false;
  bool isNew = false;
  
  // The FU maybe in the same executive as the BU and may therefore
  // receive a chain of messages / blocks.
  // Break up the chain into its individual messages / blocks
  
  FURawEvent *event = 0;
  


  while(bufRef != 0 && !errorFound)
    {
      I2O_MESSAGE_FRAME         *stdMsg = 
	(I2O_MESSAGE_FRAME*)bufRef->getDataLocation();
      I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME *msg    = 
	(I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME*)stdMsg;
      LOG4CPLUS_DEBUG(this->getApplicationLogger(),
		      "Take received for transaction: "
		      << msg->fuTransactionId << " Fragment is " 
		      << (int)msg->superFragmentNb+1
		      << " of " << (int)msg->nbSuperFragmentsInEvent 
		      << " total");
      
      
      // get the correct event 
      // (new if this event handle has never been used)

      event = factory_->checkEvent(msg->fuTransactionId,
				   msg->buResourceId);

      if(event==0)
	{
	  LOG4CPLUS_FATAL(this->getApplicationLogger(),
			  "Event does not exist for transaction: "
			  << msg->fuTransactionId );
	  nbDataErrors_.value_++;
	  errorFound = true;
	}
      isNew = event->isNew();
      // Check for incoming FU transaction id mismatch

      if(!event->checkTransactionContext(msg->fuTransactionId))
	{
	  LOG4CPLUS_FATAL(this->getApplicationLogger(),
			  "Transaction id mismatch.  Received: "
			  << msg->fuTransactionId << " Expected: "
			  << event->transactionContext());
	  nbDataErrors_.value_++;
	  errorFound = true;
	}
      // Check for fragment mismatch
      if(msg->superFragmentNb != event->nextFragment())
	{
	  int cf = (int) msg->superFragmentNb;
	  int rf = (int) event->nextFragment();
	  LOG4CPLUS_FATAL(this->getApplicationLogger(),
			  "Fragment mismatch.  Received: "
			  << cf << " Expected: "
			  << rf);
	  nbDataErrors_.value_++;
	  errorFound = true;
	}

      //      cout << "Error found up to now ? " << errorFound << endl;

      currentFragment = event->processMsg(stdMsg);
      if(currentFragment<0) nbDataErrors_.value_++;
      errorFound = errorFound || (currentFragment < 0);
      currentFragment=std::abs(currentFragment)-1;
      
      // if an error is found, bail out ¿ without telling why !!!
      //errorFound = errorFound || (currentFragment < 0);
      if(errorFound)
	{
	  //	  LOG4CPLUS_ERROR(this->getApplicationLogger(),"Error found in processing take message");
	}
      
      toGo_.push_back(bufRef); // we are not allowed to free buffers yet
      
      toolbox::mem::Reference *old = bufRef;
      bufRef = bufRef->getNextReference();
      old->setNextReference(0); // break the chain if there is one, this deals with local/remote problem
      
      // check if buffers can be freed
      

      if(event->fragmentComplete(currentFragment))
	{
	  nbReceivedFragments_.value_++;
	  for(unsigned int i = 0; i<toGo_.size(); i++)
	    {
	      //	      cout << "FUAdapter::freeing reference " << 0 << endl;
	      toGo_[i]->release();
	    }
	  toGo_.clear();
	}
      
    } // end of while loop

  bSem_.give();
  if(event->outstandingReqs()==0 && isNew) 
    {
      nbReceivedEvents_.value_++;
      pendingRequests_.value_--;
      factory_->take(event);

      if(sink_)
	{
	  sink_->sink();
	}
    }
}	

void FUAdapter::sendAllocate(unsigned long buInstance,
			     unsigned long nbEvents,
			     unsigned int ifset)
{
  FURawEvent *ev = 0;
  vector<U32> context(nbEvents);
  for(unsigned int i = 0; i<nbEvents; i++)
    {
      ev = factory_->getEvent(ifset, buInstance);
      context[i] = ev->transactionContext();
    }
  bSem_.take();
  LOG4CPLUS_DEBUG(this->getApplicationLogger(),
		  "FUAdapter::sending allocate to BU #" << buInstance);
  if(bu_.size()>buInstance)
    {
      try{
	bu_[buInstance]->allocate(ev->getPending(), 
				context, nbEvents);
	nAllocateSent_++;
	pendingRequests_.value_ += nbEvents;
	nAllocatedEvents_.value_ += nbEvents;

      }
      catch(xdaq::exception::Exception &e)
	{
	  LOG4CPLUS_ERROR(this->getApplicationLogger(),
		    "sendAllocate failed.buInstance: "
			  << buInstance << ". Error: " << e.what());
	}
    }
  else
    LOG4CPLUS_ERROR(this->getApplicationLogger(),
		    "sendAllocate failed.buInstance: "
		    << buInstance << " Not accessible ");
  LOG4CPLUS_DEBUG(this->getApplicationLogger(),
		  "FUAdapter::allocate sent to BU #" << buInstance);
    
  bSem_.give();
}

void FUAdapter::sendCollect(FURawEvent *event)
{
  bSem_.take();
  if(bu_.size()>event->getBU())
    bu_[event->getBU()]->collect(event->getHandle(),event->getPending(),
				 (U32)event);
  else
    LOG4CPLUS_ERROR(this->getApplicationLogger(),
		    "sendCollect failed.bu for event: "
		    << event->getBU() << " Not accessible ");
    
  bSem_.give();
}

void FUAdapter::sendDiscard(unsigned long buInstance, FURawEvent *event)
{
  bSem_.take();
  if(bu_.size()>buInstance)
    {
      try{
	bu_[buInstance]->discard(event->getHandle());
	nDiscardSent_++;
      }
      catch(xdaq::exception::Exception &e)
	{
	  LOG4CPLUS_ERROR(this->getApplicationLogger(),
		    "sendDiscard failed.buInstance: "
			  << buInstance << ". Error: " << e.what());
	}

    }

  else
    LOG4CPLUS_ERROR(this->getApplicationLogger(),
		    "sendDiscard failed.buInstance: "
		    << buInstance << " Not accessible ");
  factory_->removeEvent(event);
  bSem_.give();
  //event is recycled by factory after issuing a discard
}

#include "toolbox/include/toolbox/net/URN.h"
#include "toolbox/include/toolbox/mem/MemoryPoolFactory.h"
#include "toolbox/include/toolbox/mem/HeapAllocator.h"

void FUAdapter::findOrCreateMemoryPool() throw (xcept::Exception)
{
  // If a pool name has been given
  if(poolName_ != "")
    {
      // Find the memory pool with the given name
      try
	{
	  toolbox::net::URN urn("toolbox-mem-pool", poolName_);
	  
	  pool_ = toolbox::mem::getMemoryPoolFactory()->findPool(urn);
	  
	  LOG4CPLUS_INFO(getApplicationLogger(),
			 "Found memory pool: " << poolName_.toString());
	}
      catch(toolbox::mem::exception::MemoryPoolNotFound &e)
	{
	  string s = "Failed to find pool: " + poolName_.toString();
	  
	  LOG4CPLUS_FATAL(getApplicationLogger(), s);
	  XCEPT_RETHROW(xcept::Exception, s, e);
	}
    }
  else // Else no pool name has been given
    {
      stringstream oss;
      string poolName;
      
      oss << xmlClass_ << instance_ << "/heap";
      poolName = oss.str();
      
	  // Create a memory pool
      try
	{
	  toolbox::mem::HeapAllocator *allocator = new toolbox::mem::HeapAllocator();
	  toolbox::net::URN urn("toolbox-mem-pool", poolName);
	  toolbox::mem::MemoryPoolFactory *poolFactory =
	    toolbox::mem::getMemoryPoolFactory();
	  
	  pool_ = poolFactory->createPool(urn, allocator);
	  
	  LOG4CPLUS_INFO(getApplicationLogger(),
			 "Created memory pool: " << poolName);
	}
      catch (toolbox::mem::exception::Exception& e)
	{
	  string s = "Failed to create pool: " + poolName;
	  
	  LOG4CPLUS_FATAL(getApplicationLogger(), s);
	      XCEPT_RETHROW(xcept::Exception, s, e);
	}
    }
}
