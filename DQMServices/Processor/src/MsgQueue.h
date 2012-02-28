#ifndef DQMEVF_MSGQUEUE_H
#define DQMEVF_MSGQUEUE_H

#include <stdio.h>       /* standard I/O functions.              */
#include <stdlib.h>      /* malloc(), free() etc.                */
#include <sys/types.h>   /* various type definitions.            */
#include <sys/ipc.h>     /* general SysV IPC structures          */
#include <sys/msg.h>     /* message queue functions and structs. */
#include <errno.h>
#include <string.h>

#include <iostream>
#include <sstream>

#include "Exception.h"
#include "MsgBuf.h"

namespace dqmevf{

  class MasterQueue{

  public:

    MasterQueue(unsigned int ind) : status_(0)
      {
	
	/* create or attach a public message queue, with read/write access to every user. */
	queue_id_ = msgget(QUEUE_ID+ind, IPC_CREAT | 0666); 
	if (queue_id_ == -1) {
	  std::ostringstream ost;
	  ost << "failed to get message queue:" 
	      << strerror(errno);
	  XCEPT_RAISE(dqmevf::Exception, ost.str());
	}
	// it may be necessary to drain the queue here if it already exists !!! 
	drain();
      }
    ~MasterQueue()
      {
	if(status_>0) msgctl(queue_id_,IPC_RMID,0);
      }

    int post(MsgBuf &ptr)
      {
	int rc;                  /* error code returned by system calls. */
	rc = msgsnd(queue_id_, ptr.ptr_, ptr.msize()+1,0);
	//if(rc==-1)
	  //std::cout << "snd::Master failed to post message - error:"
	  //	    << strerror(errno) << std::endl;
	//	delete ptr;
	return rc;
      }
    int postNonBlocking(MsgBuf &ptr)
      {
        int rc;                  /* error code returned by system calls. */
        rc = msgsnd(queue_id_, ptr.ptr_, ptr.msize()+1,IPC_NOWAIT);
        //if(rc==-1)
          //std::cout << "snd::Master failed to post message - error:"
          //        << strerror(errno) << std::endl;
        //      delete ptr;
        return rc;
      }

    unsigned long rcv(MsgBuf &ptr)
      {
	unsigned long msg_type = ptr->mtype;
	int rc = msgrcv(queue_id_, ptr.ptr_, ptr.msize()+1, msg_type, 0);
	if (rc == -1 && errno != ENOMSG)  
	  {
	    std::string serr = "rcv::Master failed to get message from queue - error:";
	    serr += strerror(errno);
	    XCEPT_RAISE(dqmevf::Exception, serr);
	  }
	else if(rc == -1 && errno == ENOMSG) return MSGQ_MESSAGE_TYPE_RANGE;
	return ptr->mtype;
      }
    unsigned long rcvNonBlocking(MsgBuf &ptr)
      {
	unsigned long msg_type = ptr->mtype;
	int rc = msgrcv(queue_id_, ptr.ptr_, ptr.msize()+1, msg_type, IPC_NOWAIT);
	if (rc == -1 && errno != ENOMSG)  
	  {
	    std::string serr = "rcvnb::Master failed to get message from queue - error:";
	    serr += strerror(errno);
	    XCEPT_RAISE(dqmevf::Exception, serr);
	  }
	else if(rc == -1 && errno == ENOMSG) return MSGQ_MESSAGE_TYPE_RANGE;
	return ptr->mtype;
      }
    int disconnect()
      {
	int ret = msgctl(queue_id_,IPC_RMID,0);
	status_ = -1000;
	return ret;
      }
    int id(){return queue_id_;}
    int status()
      {
	char cbuf[sizeof(struct msqid_ds)];
	struct msqid_ds *buf= (struct msqid_ds*)cbuf;
	int ret = msgctl(queue_id_,IPC_STAT,buf);	
	if(ret!=0) status_ = -1; 
	else
	  {
	    status_ = 1;
	    occup_ = buf->msg_qnum;
	    pidOfLastSend_ = buf->msg_lspid;
	    pidOfLastReceive_ = buf->msg_lrpid;
	    //	    std::cout << "queue " << buf->msg_qnum << " " 
	    //	      << buf->msg_lspid << " " 
	    //	      << buf->msg_lrpid << std::endl;
	  }
	return status_;
      }
    int occupancy()const{return occup_;}
    void drain(){
      status();
      if(occup_>0)
	std::cout << "message queue id " << queue_id_ << " contains " << occup_ << "leftover messages, going to drain " 
		  << std::endl;
      //drain the queue before using it
      MsgBuf msg;
      while(occup_>0)
	{
	  msgrcv(queue_id_, msg.ptr_, msg.msize()+1, 0, 0);
	  status();
	  std::cout << "drained one message, occupancy now " << occup_ << std::endl;
	}
    }
    pid_t pidOfLastSend()const{return pidOfLastSend_;}
    pid_t pidOfLastReceive()const{return pidOfLastReceive_;}
  private:

    int queue_id_;             /* ID of the created queue.            */
    int status_;
    int occup_;
    int pidOfLastSend_;
    int pidOfLastReceive_;
  };


  class SlaveQueue{

  public:

    SlaveQueue(unsigned int ind) : queue_id_(0)
      {
	
	/* get an (existing) public message queue */
	queue_id_ = msgget(QUEUE_ID+ind, 0);
	if (queue_id_ == -1) {
	  XCEPT_RAISE(dqmevf::Exception, "failed to get message queue");
	}
      }
    ~SlaveQueue()
      {
      }

    int post(MsgBuf &ptr)
      {
	int rc;                  /* error code retuend by system calls. */
	rc = msgsnd(queue_id_,ptr.ptr_, ptr.msize()+1,0);
	//	delete ptr;
	if(rc==-1)
	  std::cout << "snd::Slave failed to post message - error:"
		    << strerror(errno) << std::endl;
	return rc;
      }
    int postNonBlocking(MsgBuf &ptr)
      {
        int rc;                  /* error code retuend by system calls. */
        rc = msgsnd(queue_id_,ptr.ptr_, ptr.msize()+1,IPC_NOWAIT);
        //      delete ptr;
        if(rc==-1)
          std::cout << "snd::Slave failed to post message - error:"
                    << strerror(errno) << std::endl;
        return rc;
      }

    unsigned long rcv(MsgBuf &ptr)
      {
	unsigned long msg_type = MSQS_MESSAGE_TYPE_SLA;
	//unsigned long msg_type = ptr->mtype;
	int rc = msgrcv(queue_id_, ptr.ptr_, ptr.msize()+1, -msg_type, 0);
	if (rc == -1 && errno != ENOMSG) 
	  {
	    std::string serr = "rcv::Slave failed to get message from queue - error:";
	    serr += strerror(errno);
	    XCEPT_RAISE(dqmevf::Exception, serr);
	  }
	else if(rc == -1 && errno == ENOMSG) return MSGQ_MESSAGE_TYPE_RANGE;
	return ptr->mtype;
      }
    unsigned long rcvNonBlocking(MsgBuf &ptr)
      {
	unsigned long msg_type = MSQS_MESSAGE_TYPE_SLA;
	//unsigned long msg_type = ptr->mtype;
	int rc = msgrcv(queue_id_, ptr.ptr_, ptr.msize()+1, -msg_type, IPC_NOWAIT);
	if (rc == -1 && errno != ENOMSG) 
	  {
	    std::string serr = "rcvnb::Slave failed to get message from queue - error:";
	    serr += strerror(errno);
	    XCEPT_RAISE(dqmevf::Exception, serr);
	  }
	else if(rc == -1 && errno == ENOMSG) return MSGQ_MESSAGE_TYPE_RANGE;
	return ptr->mtype;
      }
  private:

    int queue_id_;             /* ID of the created queue.            */

  };
}
#endif
