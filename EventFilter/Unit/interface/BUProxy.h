#ifndef __BUPROXY_H
#define __BUPROXY_H
//-----------------------------------------------
//
/// Author: EM
/// PH/CMD
//
//  MODIFICATION:
//  $Log: BUProxy.h,v $
//  Revision 1.3  2006/09/26 16:26:01  schiefer
//  ready for xdaq 3.7 / CMSSW_1_1_0
//
//  Revision 1.2  2006/09/21 12:39:44  schiefer
//  fix crc check performed in FURawEvent
//
//  Revision 1.1  2005/10/19 09:10:35  meschi
//  first import from COSINE
//
//  Revision 1.6  2005/06/28 08:48:46  meschi
//  fix argument order
//
//  Revision 1.5  2005/06/23 15:13:34  meschi
//  remove warnings
//
//  Revision 1.4  2005/04/29 16:28:10  meschi
//  fixes for XDAQ_3_01
//
//  Revision 1.3  2005/02/28 09:35:52  meschi
//  porting to XDAQ3
//
//  Revision 1.2  2005/02/23 09:17:19  meschi
//  XDAQ3 Porting
//
//  Revision 1.1  2004/05/14 13:23:58  meschi
//  set of Listener/Adapter/Proxy for new BU/FU
//
//
//-----------------------------------------------
// 
/** class BUProxy
    Formats i2o messages from FU to BU (enslaved to a "FU" Application)
    (replaces i2oBUProxy)
*/

#include "toolbox/mem/Reference.h"
#include "toolbox/mem/MemoryPoolFactory.h"
#include "toolbox/mem/exception/Exception.h"

#include "i2o/Method.h"
#include "i2o/utils/AddressMap.h"

#include "xcept/tools.h"

#include "xdaq/ApplicationDescriptor.h"

#include "interface/evb/i2oEVBMsgs.h" 
#include "interface/shared/i2oXFunctionCodes.h"


#include <iostream>
#include <vector>


class BUProxy
{
public:  
  
  /** constructor from owner tid, destination tid, and buffer size */
  BUProxy (xdaq::ApplicationDescriptor *source, 
	   xdaq::ApplicationDescriptor *destination, 
	   xdaq::ApplicationContext *appContext, 
	   toolbox::mem::Pool *mpool = 0,
	   unsigned long size = 1024) :
    dataBufSize_(size), source_(source), destination_(destination),
    appContext_(appContext), mpool_(mpool)
    {
      if(mpool_==0)
	{
	  std::cout<<"BUProxy::shit, where's my memory pool ???"<<std::endl;
	  // do something
	}
      
    }
  
  /** destructor */
  ~BUProxy () 
    {
      
    }
  
  /** FU requests nbEvents new events to BU  (throws xdaq::exception::Exception*/
  void allocate(U32 fset, std::vector<U32> &context, U32 nbEvents) 
    { 
      //calculate size of message
      size_t msgSizeInBytes = sizeof(I2O_BU_ALLOCATE_MESSAGE_FRAME) +
	(nbEvents - 1) * sizeof(BU_ALLOCATE);
      //allocate a buffer from the bu pool (created elsewhere)

      toolbox::mem::Reference *bufRef = 0;
      try
	{
	  bufRef = 
	    toolbox::mem::getMemoryPoolFactory()->getFrame(mpool_,msgSizeInBytes);
	}
      //should rethrow to adapter
      catch(toolbox::mem::exception::Exception e)
	{
	  std::cout<<"BUProxy::exception in allocating frame " 
		   <<xcept::stdformat_exception_history(e)<<std::endl;
	  return;
	}
      catch(...)
	{
	  std::cout<<"BUProxy::unknown exception in allocating frame "<<std::endl;
	  return;
	}
      //prepare the message frame
      I2O_MESSAGE_FRAME *stdMsg = 
	(I2O_MESSAGE_FRAME*)bufRef->getDataLocation();
      I2O_PRIVATE_MESSAGE_FRAME*pvtMsg = (I2O_PRIVATE_MESSAGE_FRAME*)stdMsg;
      I2O_BU_ALLOCATE_MESSAGE_FRAME *msg =
	(I2O_BU_ALLOCATE_MESSAGE_FRAME*)stdMsg; 
      unsigned int i = 0;
      
      stdMsg->MessageSize      = msgSizeInBytes >> 2;
      try{
	stdMsg->InitiatorAddress = i2o::utils::getAddressMap()->getTid(source_);
      }
      catch(xdaq::exception::ApplicationDescriptorNotFound e)
	{
	  std::cout<<"BUProxy::exception in getting source tid " 
		   <<xcept::stdformat_exception_history(e)<<std::endl;
	}	    
      try{
	stdMsg->TargetAddress    = i2o::utils::getAddressMap()->getTid(destination_);
      
      }
      catch(xdaq::exception::ApplicationDescriptorNotFound e)
	{
	  std::cout<<"BUProxy::exception in getting source tid " 
		   <<xcept::stdformat_exception_history(e)<<std::endl;
	}	    


      stdMsg->Function         = I2O_PRIVATE_MESSAGE;
      stdMsg->VersionOffset    = 0;
      stdMsg->MsgFlags         = 0;  // Point-to-point
      
      pvtMsg->XFunctionCode    = I2O_BU_ALLOCATE;
      pvtMsg->OrganizationID   = XDAQ_ORGANIZATION_ID;
      msg->n                   = nbEvents;
      
      for(i=0; i<nbEvents; i++)
        {
	  msg->allocate[i].fuTransactionId = context[i];
	  msg->allocate[i].fset            = fset; // IGNORED¿!!!
        }

      bufRef->setDataSize(msgSizeInBytes);

      appContext_->postFrame(bufRef,source_,destination_);
    }
  
	
  /** collect more data for eventHandle: dummy for the moment */
  void collect(U32 eventHandle, U32 fs, U32 context) 
    { 
      std::cerr<<"BUProxy::dummy collect method called "<<std::endl;
    }
	
  /** discard event eventHandle (throws xdaq::exception::Exception*/
  void discard (U32 eventHandle) 
    { 
      //calculate size of message
      size_t msgSizeInBytes = sizeof(I2O_BU_DISCARD_MESSAGE_FRAME);
      //allocate a buffer from the bu pool (created elsewhere)
      toolbox::mem::Reference *bufRef =
	toolbox::mem::getMemoryPoolFactory()->getFrame(mpool_, msgSizeInBytes);
      
      I2O_MESSAGE_FRAME* stdMsg=(I2O_MESSAGE_FRAME*)bufRef->getDataLocation();
      I2O_PRIVATE_MESSAGE_FRAME* pvtMsg=(I2O_PRIVATE_MESSAGE_FRAME*)stdMsg;
      I2O_BU_DISCARD_MESSAGE_FRAME* msg=(I2O_BU_DISCARD_MESSAGE_FRAME*)stdMsg;
      
      stdMsg->MessageSize      = msgSizeInBytes >> 2;
      stdMsg->InitiatorAddress = i2o::utils::getAddressMap()->getTid(source_);
      stdMsg->TargetAddress    = i2o::utils::getAddressMap()->getTid(destination_);
      stdMsg->Function         = I2O_PRIVATE_MESSAGE;
      stdMsg->VersionOffset    = 0;
      stdMsg->MsgFlags         = 0;  // Point-to-point
      
      pvtMsg->XFunctionCode    = I2O_BU_DISCARD;
      pvtMsg->OrganizationID   = XDAQ_ORGANIZATION_ID;
      
      msg->n                   = 1;
      msg->buResourceId[0]     = eventHandle;

      bufRef->setDataSize(msgSizeInBytes);
      appContext_->postFrame(bufRef, source_, destination_);
    }

 private:

  unsigned long                dataBufSize_;
  xdaq::ApplicationDescriptor *source_;
  xdaq::ApplicationDescriptor *destination_;
  xdaq::ApplicationContext    *appContext_;
  toolbox::mem::Pool          *mpool_;
}; 



#endif


