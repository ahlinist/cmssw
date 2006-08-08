#include "EventFilter/Unit/interface/FURawEvent.h"
#include "EventFilter/Unit/interface/FUAdapter.h"
#include "EventFilter/Unit/interface/FSetBroker.h"

#include <iostream>

#define REAL_SOID_MASK 0x0003FF00

using namespace std;

int FURawEvent::errors[10] = {0,0,0,0,0,0,0,0,0,0};

FURawEvent::RawData *FURawEvent::nulldata = new RawData();

FURawEvent::FURawEvent(unsigned int ihandle) :  
  fragmentCount_(0), blockCount_(0), 
  outstandingReqs_(0), myData_(nFEDs),
  eventHandle_(INVALID_EVENT_HANDLE),
  internalHandle_(ihandle), pendingFset(0),
  buInstance_(INVALID_BU_INSTANCE), isNew_(true), l1Id_(0)
{
  nulldata->size_ = 0;
  nulldata->data_ = 0;
  //initialize locks and conditions
  pthread_mutex_init(&lock_,0);
  pthread_cond_init(&ready_,0);


  //for the moment the initial fragment consists trivially of all the fragments
  //  outstandingReqs_ = FSetBroker::instance()->getNSF(pendingFset);

  unsigned int totalSF = FSetBroker::instance()->getNSF(0);
  builtFlags = new bool[totalSF];
  for(unsigned int i = 0; i < totalSF; i++)
    builtFlags[i]=false;
  for(unsigned int j = 0; j < nFEDs; j++)
    {
      myData_[j] = new RawData();
      myData_[j]->size_ = 0;
      myData_[j]->data_ = 0;
    }
}

FURawEvent::~FURawEvent()
{
  for(vector<RawData *>::iterator it = myData_.begin(); it != myData_.end(); it++)
    {
      delete [] (*it)->data_;
      delete (*it);
    }
  delete builtFlags;
}

int FURawEvent::processMsg(I2O_MESSAGE_FRAME *stdMsg)
{
  
  I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME *msg    = 
    (I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME*)stdMsg;
  
  size_t        sizeOfPayload   = 0;
  unsigned char *startOfPayload = 0;
  bool          startOfFragment = false;
  bool          firstFragment   = false;
  bool          lastFragment    = false;
  bool          startOfEvent    = false;
  bool          fragmentBuilt   = false;
  bool          eventBuilt      = false;
  
  bool errorFound = false;

  startOfFragment = msg->blockNb    == 0;
  firstFragment   = msg->superFragmentNb == 0;
  lastFragment    = msg->superFragmentNb == (msg->nbSuperFragmentsInEvent - 1);
  startOfEvent    = firstFragment && startOfFragment;
  fragmentBuilt   = msg->blockNb == (msg->nbBlocksInSuperFragment - 1);
  eventBuilt      = fragmentBuilt && lastFragment;

  block_adrs.resize(msg->nbBlocksInSuperFragment);
  /*  cout << "current block is " <<  (int)msg->blockNb  << " in a total of "
       << (int)msg->nbBlocksInSuperFragment << endl;
  */
  sizeOfPayload  = (stdMsg->MessageSize << 2) -
    sizeof(I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME);
  
  int fragInd = (int)msg->superFragmentNb;

  /*
  cout << "Now working on fragment " << fragInd << endl; 
  cout << "Total blocks " << (int) msg->totalBlocks;
  cout << "Size of Payload " << sizeOfPayload << endl;
  */
  if(firstFragment)
    {
      //check whether the assumed number of fragments is actually being delivered
      if(outstandingReqs_ != msg->nbSuperFragmentsInEvent) //yell but don't make a fuss about it 	
	{
	  outstandingReqs_ = msg->nbSuperFragmentsInEvent;
	}
    }

  if(startOfFragment)
    {
      /*      cout << "Start of fragment, create pointer array of size " 
	      << msg->totalFragments << endl;*/
      blockCount_ = 0;
    }
  else
    {
      blockCount_++;
    }


  // Check for block mismatch
  if(msg->blockNb != blockCount_)
    {
      if(errors[(-I2O_BLOCK_MISMATCH)]++ < 10)
	LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),"I2O block mismatch: current " 
			<<  (int)msg->blockNb << " Expected: "
			<< (int)blockCount_);
      errorFound = true;
    }


  startOfPayload = ((unsigned char*)stdMsg) +
    sizeof(I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME);
  block_adrs[msg->blockNb] = startOfPayload;
  dumpFrame((char*)startOfPayload,sizeOfPayload);
  if(fragmentBuilt)
    {
      int res = checkin_data(block_adrs);
      if(res<0) errorFound = true;
      fragmentCount_++;
      outstandingReqs_--;
      builtFlags[fragInd]=true;
      block_adrs.clear();
    }
  
  if(errorFound) 
    {
      return -1;
    }
  
  if(outstandingReqs_==0) 
    {
      release(); // in the case of a new event no 
                 // other thread is signaled
      if(isNew_) isNew_ = false; //set flag that this event is available
    }
  return fragInd;
}


#include <netinet/in.h>
#include "EventFilter/Utilities/interface/DebugUtils.h"


int FURawEvent::checkin_data(vector<unsigned char*> &block_adrs)
{

  int          retVal =  0;
  int           fedid = -1;
  int  current_trigno = -1;
  int          lvl1id = -1;

  unsigned char*sf_data = 0;
  unsigned long sf_size = 0;

  //check all frl headers
  for(unsigned int iblk = 0; iblk < block_adrs.size(); iblk++)
    {
      frlh_t *ph = (frlh_t *)block_adrs[iblk];

      int hd_trigno  = ph->trigno;
      int hd_segno   = ph->segno;
      int hd_segsize     = ph->segsize;
      int segsize_proper = hd_segsize & FRL_SEGSIZE_MASK;
      if(segsize_proper >= 1048576)
	{
	  LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),"Error in checkin_data for segment " 
			  << hd_segno << " size " 
			  << segsize_proper)
	    retVal = -1;
	  break;
	}

      // check trigno
      if (current_trigno == -1) {
	current_trigno = hd_trigno ;
      }
      else {
	if (current_trigno != hd_trigno) {
	  if(errors[(-FRL_TRIGGER_MISMATCH)]++ < 10)
	    LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),"FRL segment " << iblk 
			    << " of total " << block_adrs.size() 
			    << " has wrong trigger number. Expected " 
			    << current_trigno << " received " << hd_trigno);
	  retVal = FRL_TRIGGER_MISMATCH;
	  break;
	} 
      }
      
      // check that last block flagged as last segment and none of the others
      if (iblk == block_adrs.size()-1) {
	if  (!(hd_segsize & FRL_LAST_SEGM)) {
	  if(errors[(-FRL_LASTSEG_FLNOTSET)]++ < 10)
	  LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),"FRL segment " << iblk 
			  << " of total " << block_adrs.size() << 
			  " doesnt have last segment set" );
	  retVal = FRL_LASTSEG_FLNOTSET;
	  break;
	}
      }
      else {
	if  ((hd_segsize & FRL_LAST_SEGM)) {
	  if(errors[(-FRL_LASTSEG_FLISWRNG)]++ < 10)
	    LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),"FRL segment " << iblk 
			    << " of total " << block_adrs.size() << 
			    " has last segment set" );
	  retVal = FRL_LASTSEG_FLISWRNG ;
	  break;
	}
      }
      sf_size += segsize_proper;
    }

  if(retVal==0)
    {
      //make copy buffer, move data
      sf_data = new unsigned char[sf_size];
      unsigned long remnant = sf_size;
      vector<unsigned char *>vcursor(block_adrs.size());
      vector<unsigned int> scursor(block_adrs.size());
      unsigned long offset = 0;
      for(unsigned int iblk = 0; iblk < block_adrs.size(); iblk++)
	{
	  frlh_t *ph = (frlh_t *)block_adrs[iblk];
	  int hd_segsize     = ph->segsize;
	  int segsize_proper = hd_segsize & FRL_SEGSIZE_MASK;
	  scursor[iblk] = segsize_proper;
	  if(segsize_proper > remnant)
	    {
	      LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),
			      "FRL segment " << iblk
			      << " claims size " << segsize_proper 
			      << " while superfragment has only " << remnant
			      << " bytes left "); 
	      retVal = -1;
	      break;
	    }
	  vcursor[iblk] = new(sf_data+offset) unsigned char[segsize_proper];
	  offset += segsize_proper;
	  remnant -= segsize_proper;
	  memcpy(vcursor[iblk], block_adrs[iblk]+frlhs,segsize_proper);
	}

      unsigned char *cursor = sf_data + sf_size;
      unsigned long cursize = sf_size; 

      while(cursize>0)
	{
	  fedt_t *pft = (fedt_t*)(cursor-fedts);
	  unsigned int fedlen = pft->eventsize & FED_EVSZ_MASK;   // 24 bits  - len in 64-bit words fed payload + header/trailer      
	  if((pft->eventsize & FED_TCTRLID_MASK)!=FED_TCTRLID)
	    {
	      if(errors[(-FED_TRAILER_MMARKER)]++ < 10)      
		{
		  unsigned char *bd = cursor - 3*fedts;
		  unsigned int ds = 5*fedts;
		  while(bd<sf_data)
		    bd += fedts;
		  while((bd + ds) > (sf_data + sf_size))
		    ds -= fedts;
		  std::string dmp = evf::dumpFrame(bd,ds);
		  LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),
				  "Trigger # " 
				  << current_trigno 
				  << ": FED trailer marker not found: 0x" 
				  << hex
				  << int(pft->eventsize & FED_TCTRLID_MASK) 
				  << dec 
				  << ", expecting 0x" 
				  << hex << (int)FED_TCTRLID 
				  << dec
				  << "\n local dump \n"
				  << dmp
				  ); 
		}
	      retVal = FED_TRAILER_MMARKER;
	      break;
	    }
	  fedlen *= 8; // size in 64-bit words
	  if(fedlen>cursize)
	    {
	      LOG4CPLUS_FATAL(adapter_->getApplicationLogger(),
			      "FED size from trailer " << fedlen
			      << " bigger than segment size left " 
			      << cursize << ". Risk of memory corruption!!!" ); 
	      retVal = -100;
	      break;
	    }
	  short crc = (pft->conscheck & FED_CRCS_MASK >> 16);
	  cursor  -= fedlen;
	  cursize -= fedlen;
	  fedh_t *pfh = (fedh_t *)cursor;
	  if((pfh->eventid & FED_HCTRLID_MASK)!=FED_HCTRLID)
	    {
	      if(errors[(-FED_HEADER_MMARKER)]++ < 10)      
		{
		  unsigned char *bd = cursor - 2*fedhs;
		  unsigned int ds = 5*fedhs;
		  while(bd<sf_data)
		    bd += fedts;
		  while((bd + ds) > (sf_data + sf_size))
		    ds -= fedts;
		  std::string dmp = evf::dumpFrame(bd,ds);
		    LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),
				    "Trigger # " 
				    << current_trigno 
				    << ": FED header marker not found: 0x" << hex
				    << int(pfh->eventid & FED_HCTRLID_MASK) << dec 
				    << ", expecting 0x" << hex << (int)FED_HCTRLID 
				    << dec 
				    << "\n local dump \n"
				    << dmp
				    ); 
		}
	      retVal = FED_HEADER_MMARKER;
	      break;
	    }
	  fedid = pfh->sourceid & REAL_SOID_MASK;
	  fedid = fedid >> 8;
	  lvl1id = pfh->eventid & FED_LVL1_MASK;
	  

	  if (lvl1id != current_trigno) 
	    {
	      //level1id consistent with frl header one ? 
	      cout << "error, level1 for fed " << fedid << " " << lvl1id 
		   << " should be " 
		   << current_trigno << endl;
	      if(errors[(-FED_TRIGGER_MISMATCH)]++ < 10)
		LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),
				"FED level-1 id mismatch. Found: " 
				<< lvl1id << ", expecting " << current_trigno);
	      retVal = FED_TRIGGER_MISMATCH;
	      break;
	    }
	  if(myData_[fedid]->size_ != 0)
	    {
	      // FED is not a duplicate ? 
	      cout << "error, duplicated fed " << fedid << endl;
	      if(errors[(-FED_DROPPED_DUPLDATA)]++ < 10)
		LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),"FED data for id " << fedid 
				<< " already received, dropping data");
	      
	      retVal = FED_DROPPED_DUPLDATA;
	    }
	  else
	    {
	      LOG4CPLUS_DEBUG(adapter_->getApplicationLogger(),"FED data for id " << fedid 
			      << "with size " << fedlen << " was stored" );
	      //otherwise save data in the event
	      myData_[fedid]->data_ = new unsigned char[fedlen];
	      memcpy(myData_[fedid]->data_,cursor,fedlen);
	      myData_[fedid]->size_ = fedlen;
	  
	    }
	}
      if(cursize>0)
	{
	  retVal = -1000;
	}
    }
  if(l1Id_==0)
    l1Id_ = current_trigno;
  else if(l1Id_ != current_trigno)
    LOG4CPLUS_ERROR(adapter_->getApplicationLogger()," wrong trigger number. Expected " 
		    << l1Id_ << " received " << current_trigno);

  delete sf_data;
  return retVal;
}

FURawEvent::RawData *FURawEvent::operator[](int fedid)
{
  RawData *retVal = myData_[fedid];
  if(retVal->data_ == 0)
    {
      // this is currently commented out since bu does not support partial
      // event building
      /*
      retVal = timedRequest(fedid,3);
      // should be replaced by exception + logging message
      if(retVal->data_ == 0)
	cout << "FUEvent::Severe - could not get requested data for fed: " 
	     << fedid << endl;
      */
      retVal = nulldata;
    }
  return retVal;
}

#include <sys/time.h>

FURawEvent::RawData *FURawEvent::timedRequest(int fedid, int delay)
{
  struct timeval now;
  struct timespec timeout;
  RawData *retVal = myData_[fedid];
  int retcode = 0;
  gettimeofday(&now,0);
  timeout.tv_sec = now.tv_sec + delay;
  timeout.tv_nsec = now.tv_usec * 1000;
  int retries = 0;
  setPending(FSetBroker::instance()->getFS(fedid));
  //  XDAQ_NOTE(("FUEvent::%d - going to wait on take",eventHandle_));
  adapter_->sendCollect(this);
  while((retVal->data_==0 || retcode == ETIMEDOUT) && retries < 3)
    {
      retcode = pthread_cond_timedwait(&ready_,&lock_,&timeout);
      retVal = myData_[fedid];
      retries++;
    }
  
  return retVal->data_==0 ? 0 : retVal;
}



void FURawEvent::reset(bool clean)
{
  l1Id_ = 0;
  // if requested release memory allocated for FEDs
  if(clean)
    {
      for(unsigned int i=0; i<myData_.size(); i++)
	{
	  if(myData_[i]->data_)
	    {
	      delete [] myData_[i]->data_;
	      myData_[i]->data_ = 0;
	      myData_[i]->size_ = 0;
	    }
	  // missing feds, too late to do anything, should spot earlier
	  //	  else
	  //	    cout << "FED data pointer was not set!!!" << endl;
	}
    }
  outstandingReqs_= 0;
  blockCount_ = 0;
  fragmentCount_=0;
  unsigned int totalSF = FSetBroker::instance()->getNSF(0);
  for(unsigned int i = 0; i < totalSF; i++)
    builtFlags[i]=false;
  isNew_ = true;
  adapter_->sendDiscard(buInstance_,this);
  eventHandle_ = INVALID_EVENT_HANDLE;
  buInstance_ = INVALID_BU_INSTANCE;
  //  cout << "Finished resetting " << endl;
}

void FURawEvent::setPending(unsigned int ifset) 
{
  pendingFset = ifset;
  outstandingReqs_ = FSetBroker::instance()->getNSF(pendingFset);  
}

//thread safeness

void FURawEvent::lock()
{
  //  pthread_t ttid = pthread_self();
  pthread_mutex_lock(&lock_);
}
void FURawEvent::free()
{
  //  pthread_t ttid = pthread_self();
  pthread_mutex_unlock(&lock_);
}
void FURawEvent::release()
{
  pthread_cond_signal(&ready_);
  pthread_mutex_unlock(&lock_);
}

void FURawEvent::dumpFrame(char* data, int len)
{
  if(!adapter_->doDump()) return;
  cout << "Dump of frame at " << hex << (int) data << dec << " of expected size " << len << "bytes" << endl;
  //      PI2O_MESSAGE_FRAME  ptrFrame = (PI2O_MESSAGE_FRAME)data;
  //      printf ("\nMessageSize: %d Function %d\n",ptrFrame->MessageSize, ptrFrame->Function);
      
  char left[40];
  char right[40];
      
  //  LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),toolbox::toString("Byte  0  1  2  3  4  5  6  7\n"));
  printf ("Byte  0  1  2  3  4  5  6  7\n");
      
  int c = 0;
  int pos = 0;
      
      
  for (int i = 0; i < (len/8); i++) {
    int rpos = 0;
    for (pos = 0; pos < 8*3; pos += 3) {
      sprintf (&left[pos],"%2.2x ", ((unsigned char*)data)[c]);
      sprintf (&right[rpos],"%1c", ((data[c] > 32) && (data[c] < 127)) ? data[c]: '.' );
      rpos += 1;
      c++;
    }
    
    //    LOG4CPLUS_ERROR(adapter_->getApplicationLogger(),toolbox::toString("%4d: %s  ||  %s \n", c-8, left, right));
    printf ("%4d: %s  ||  %s \n", c-8, left, right);
  }
      
  fflush(stdout);	
      
      
}


FUAdapter *FURawEvent::adapter_ = 0;
