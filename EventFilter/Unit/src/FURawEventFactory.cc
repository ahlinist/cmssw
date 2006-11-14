#include "EventFilter/Unit/interface/FURawEventFactory.h"
#include "EventFilter/Unit/interface/FURawEvent.h"



//FURawEventFactory *FURawEventFactory::instance_ = 0;

FURawEventFactory::FURawEventFactory() : minBuiltInQueue_(0), nbProcessed_(0), resources_(maxHandles)
{
  FURawEvent::adapter_ = adapter_;
  for(unsigned int i = 0; i < maxHandles; i++)
    {
      resources_[i] = new FURawEvent(i);
      freeRes_.push(i);
    }
  int ret = sem_init(&empty_,0,0);
  if(ret!=0) std::cerr << "FURawEventFactory::error initializing resource counter " << std::endl;
  pthread_mutex_init(&mutex_,0);
  


}

FURawEvent *FURawEventFactory::getEvent(unsigned int ifset, unsigned long buinstance)
{
  // freeres should always contain some events
  // a null pointer is returned otherwise
  // protected since a free resource could be put back by a task while one 
  // is being released to the XDAQ thread
  // could be a different mutex than the one used for take
  //  cout << "entered getEvent to request new resource, free size ="
  //       << freeRes_.size() << endl;
  FURawEvent * ev = 0;
  pthread_mutex_lock(&mutex_);
  if(freeRes_.size()>0)
    {
      ev = resources_[freeRes_.top()];
      ev->setPending(ifset);
      ev->setBU(buinstance);
      freeRes_.pop();
    }
  pthread_mutex_unlock(&mutex_);
  return ev;
}

void FURawEventFactory::take(FURawEvent *ev)
{
  pthread_mutex_lock(&mutex_);
  builtRes_.push_back(ev->getInternalHandle());
  if(builtRes_.size()>minBuiltInQueue_)
    {
      int ret = sem_post(&empty_);
      if(ret!=0) std::cerr << "FURawEventFactory::error posting to resource counter " << std::endl;
    }
  pthread_mutex_unlock(&mutex_);

}

FURawEvent *FURawEventFactory::getBuiltEvent()
{
  //  cout << "Entered getBuiltEvent, going to wait on resource " << endl; 
  int ret = sem_wait(&empty_); // wait for an event if queue is empty
  //  cout << "getBuiltEvent, obtained resource, queuesize= " <<  builtRes_.size()
  //       << endl; 
  if(ret!=0) std::cerr << "FURawEventFactory::error waiting for built resource" << std::endl;
  int res = -1;
  pthread_mutex_lock(&mutex_);
  if(builtRes_.size()!=0)
    {
      res = builtRes_.front();
      builtRes_.pop_front();
    }
  pthread_mutex_unlock(&mutex_);
  if(res>=0)
    return resources_[res];
  else
    return 0;
}

unsigned int FURawEventFactory::spyBuiltEvent(unsigned int fedid, unsigned char *dest)
{

  int ret = sem_wait(&empty_); // wait for an event if queue is empty
  if(ret!=0) std::cerr << "FURawEventFactory::error waiting for built resource" << std::endl;
  pthread_mutex_lock(&mutex_);
  int res = builtRes_.front();
  unsigned int siz = (*resources_[res])[fedid]->size_;
  memcpy(dest, (*resources_[res])[fedid]->data_,siz);  
  ret = sem_post(&empty_); // wait for an event if queue is empty
  if(ret!=0) std::cerr << "FURawEventFactory::error giving back resource" << std::endl;  
  pthread_mutex_unlock(&mutex_);
  return siz;
}


unsigned int FURawEventFactory::queueSize() const
{
  return builtRes_.size();
}

FURawEvent *FURawEventFactory::checkEvent(unsigned int transaction, 
					  unsigned int handle)
{
  FURawEvent *retVal = 0;
  unsigned int ind = transaction%FURawEvent::INTERNAL_HANDLE_OFFSET;

  if(ind < maxHandles)
    {
      FURawEvent *ev = 
	resources_[ind];
      if(ev==0) 
	{
	  //throw an exception
	  std::cerr << "FURawEventFactory::error, invalid transaction " << std::endl;
	}
      else
	{
	  if(ev->getHandle()==FURawEvent::INVALID_EVENT_HANDLE)
	    {
	      ev->setHandle(handle);
	    }
	  if(ev->getHandle()!=handle)
	    {
	      std::cerr << "FURawEventFactory::error, handle mismatch " 
			<< handle << " should be " << ev->getHandle() 
			<< std::endl;
	    }
	  else
	    retVal = ev;
	}
    }
  else
    {
      //throw an exception here
      std::cerr << "FURawEventFactory::error, invalid transaction context" << std::endl;
    }
  //made an honest attempt
  return retVal;
}
  
void FURawEventFactory::removeEvent(FURawEvent *ev)
{
  // reset is not called here since it is the assumed interface for the factory
  // to discard events
  //  resources_[ev->getInternalHandle()]->reset(); // reset all buffers for this event
  pthread_mutex_lock(&mutex_);
  freeRes_.push(ev->getInternalHandle()); // put back into list of free evs
  pthread_mutex_unlock(&mutex_);
  nbProcessed_++;
}

void FURawEventFactory::postEndRunMaybe()
{
  pthread_mutex_lock(&mutex_);
  int semval = 0;
   sem_getvalue(&empty_,&semval);
  if(semval==0)
    {
      int ret = sem_post(&empty_);
       if(ret!=0) std::cerr << "FURawEventFactory::error posting to resource counter " << std::endl;
    }
  pthread_mutex_unlock(&mutex_);
  
}

void FURawEventFactory::resetFreeRes()
{
  while(!freeRes_.empty())
    freeRes_.pop();

  for(unsigned int i = 0; i < maxHandles; i++)
    {
      freeRes_.push(i);
    }
}

FUAdapter *FURawEventFactory::adapter_ = 0;

unsigned int FURawEventFactory::maxHandles=128;
