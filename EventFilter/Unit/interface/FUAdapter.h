#ifndef __FUADAPTER_H
#define __FUADAPTER_H
//-----------------------------------------------
//
/// Author: EM
/// PH/CMD
//
//  MODIFICATION:
//  $Log: FUAdapter.h,v $
//  Revision 1.6  2006/06/13 15:09:56  meschi
//  modifications to signal a thread waiting on input
//
//  Revision 1.5  2006/05/23 11:43:49  meschi
//  additional diagnostics
//
//  Revision 1.4  2005/11/22 09:42:10  meschi
//  fixed destructor, added clear for haltAction
//
//  Revision 1.3  2005/11/10 14:38:13  meschi
//  run number access function
//
//  Revision 1.2  2005/10/21 14:44:09  meschi
//  fixes to make it build
//
//  Revision 1.1  2005/10/19 09:10:35  meschi
//  first import from COSINE
//
//  Revision 1.10  2005/04/29 16:28:10  meschi
//  fixes for XDAQ_3_01
//
//  Revision 1.9  2005/02/28 09:35:52  meschi
//  porting to XDAQ3
//
//  Revision 1.8  2005/02/23 09:17:19  meschi
//  XDAQ3 Porting
//
//  Revision 1.7  2005/01/06 11:30:58  meschi
//  switchable debug printout
//
//  Revision 1.6  2004/09/22 09:47:28  meschi
//  mods for evb integration debugging
//
//  Revision 1.5  2004/08/16 14:05:53  meschi
//  fixed include
//
//  Revision 1.4  2004/06/30 22:16:34  meschi
//  now compatible with cobra task and daqevent
//
//  Revision 1.3  2004/06/22 16:53:55  meschi
//  almost working
//
//  Revision 1.2  2004/05/14 13:27:40  meschi
//  updated include paths
//
//  Revision 1.1  2004/05/14 13:23:58  meschi
//  set of Listener/Adapter/Proxy for new BU/FU
//
//
//-----------------------------------------------
// 
/** class FUAdapter
    A skeleton for the FU callback
    (replaces i2oFUAdapter)
*/

#include "interface/evb/include/i2oEVBMsgs.h"
#include "toolbox/include/BSem.h"
#include "xdata/include/xdata/String.h"
#include "xdata/include/xdata/Integer.h"
#include "xdata/include/xdata/Boolean.h"
#include "xdaq/include/xdaq/Application.h"
#include "EventFilter/Utilities/interface/RunBase.h"

#include <vector>
#include <string>

class BUProxy;
class FURawEvent;
class FURawEventFactory;
class EventSink;
class toolbox::mem::Reference;

class FUAdapter: public xdaq::Application, public evf::RunBase
{
 public:
  
  /** constructor specifying outgoing message size */
  FUAdapter(xdaq::ApplicationStub *s, string buClassName = "BU", 
	    unsigned long outsize=1024);

  /** destructor */
  virtual ~FUAdapter();

  /** implement listener callback */
  void realTake(toolbox::mem::Reference *ref);

  /** access to bu proxy, send Collect (uses transaction context)*/
  void sendCollect(FURawEvent *event);

  /** access to bu proxy, send Discard */
  void sendDiscard(unsigned long buInstance, FURawEvent *event);

  /** get switch for fragments screen dump */
  bool doDump() const {return doDumpFragments_.value_;}

  /** interface to event sink */
  virtual FURawEvent *rqstEvent()=0;

  /** interface to event loop*/
  virtual void signalWaitingInput()=0;

  /** interface to event sink */
  unsigned long getRunNumber() const { return runNumber_.value_;}

 protected:
  
  /** access to bu proxy, send nAllocate (generates transaction contexts) */
  void sendAllocate(unsigned long buInstance, unsigned long nbEvents, 
		    unsigned int ifset);

  /** create the bu array */
  void createBUArray();

  /** clear the bu array */
  void clearBUArray();

  FURawEventFactory *factory_; //factory is accessible to derived fu class

 private:

  void dumpFrame(char* data, int len)
    {
      
      PI2O_MESSAGE_FRAME  ptrFrame = (PI2O_MESSAGE_FRAME)data;
      printf ("\nMessageSize: %d Function %d\n",ptrFrame->MessageSize, ptrFrame->Function);
      
      char left[40];
      char right[40];
      
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
	
	printf ("%4d: %s  ||  %s \n", c-8, left, right);
	
      }
      
      fflush(stdout);	
      
      
    }


  vector<BUProxy*> bu_;
  string buName_;
  unsigned long bufSize_;
  BSem bSem_;
  vector<toolbox::mem::Reference *> toGo_;

 protected:

  //variables to be exported to XDAQ

  xdata::Integer pendingRequests_;
  xdata::Integer nbReceivedFragments_;
  xdata::Integer nbReceivedEvents_;
  xdata::Integer nbDataErrors_;
  xdata::Integer nbCrcErrors_;

  // debug mode (dump fragments)
  xdata::Boolean doDumpFragments_;
  xdata::Boolean doDropFragments_;
  xdata::Integer doCrcCheck_;

  // debug variables (message counters)
  xdata::Integer nAllocateSent_;
  xdata::Integer nAllocatedEvents_;
  xdata::Integer nTakeReceived_;
  xdata::Integer nDiscardSent_;

  
  /* The instance number of the application.
   */
  unsigned long instance_;
  
  string xmlClass_;
  toolbox::mem::Pool *pool_;
  xdata::String poolName_;
  
  void exportCommonParameters()
    {
      xdata::InfoSpace *s = getApplicationInfoSpace();
      
      
      poolName_ = "";
      
      s->fireItemAvailable("poolName", &poolName_);
      s->fireItemAvailable("pendingRequests", &pendingRequests_);
      s->fireItemAvailable("nbReceivedFragments", &nbReceivedFragments_);
      s->fireItemAvailable("nbReceivedEvents", &nbReceivedEvents_);
      s->fireItemAvailable("nbDataErrors", &nbDataErrors_);
      s->fireItemAvailable("nbCrcErrors", &nbCrcErrors_);

      s->fireItemAvailable("fragDumpSwitch", &doDumpFragments_);
      s->fireItemAvailable("fragDropSwitch", &doDropFragments_);
      s->fireItemAvailable("doCrcCheck",     &doCrcCheck_);

      s->fireItemAvailable("nAllocateSent", &nAllocateSent_);
      s->fireItemAvailable("nAllocatedEvents", &nAllocatedEvents_);
      s->fireItemAvailable("nTakeReceived", &nTakeReceived_);
      s->fireItemAvailable("nDiscardSent", &nDiscardSent_);
    }
  

  
  void findOrCreateMemoryPool() throw (xcept::Exception);

  EventSink             *sink_;  

  // PS
public:
  void crcErrorOccured() { nbCrcErrors_.value_++; }
  bool doCrcCheck() const {
    if (0!=doCrcCheck_.value_&&
	nbReceivedFragments_.value_%doCrcCheck_.value_==0) return true;
    return false;
  }
  
};

#endif
