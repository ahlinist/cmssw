#ifndef DQMEVF_MSGBUF_H
#define DQMEVF_MSGBUF_H


//Queue definitions

#define QUEUE_ID 137      /* base ID of queue to generate.M->S = odd, S->M = even */
#define MAX_MSG_SIZE 0x2000  /* size (in bytes) of largest message we'll ever send. 
				This is the system max  */

#define MSGQ_MESSAGE_TYPE_RANGE 0xfff

#define MSQM_MESSAGE_TYPE_NOP  0x000
#define MSQM_MESSAGE_TYPE_STOP 0x002
#define MSQM_MESSAGE_TYPE_MCS  0x004
#define MSQM_MESSAGE_TYPE_PRG  0x008
#define MSQM_MESSAGE_TYPE_WEB  0x00a
#define MSQM_MESSAGE_TYPE_TRP  0x00c

#define MSQS_MESSAGE_TYPE_NOP  0x000
#define MSQS_MESSAGE_TYPE_SLA  0x200
#define MSQS_MESSAGE_TYPE_MCR  0x202
#define MSQS_MESSAGE_TYPE_STOP 0x204
#define MSQS_MESSAGE_TYPE_PRR  0x208
#define MSQS_MESSAGE_TYPE_WEB  0x20a
#define MSQS_MESSAGE_TYPE_TRR  0x20c
#define MSQS_MESSAGE_TYPE_LOGS 0x20e //will be used for child-master log forwarding

#define NUMERIC_MESSAGE_SIZE 32

#define PIPE_READ 0
#define PIPE_WRITE 1
#define MAX_PIPE_BUFFER_SIZE 0x1000

#include <sys/msg.h> 

#ifdef __APPLE__
// Unsupported on macosx. We define a dummy msgbuf struct just to make sure it
// compiles fine.
struct msgbuf
  {
    unsigned long int mtype;    /* type of received/sent message */
    char mtext[1];           /* message payload */
  };
#endif



//@EM ToDo move implementation to .cc file

namespace dqmevf{

  struct prg{
    prg():ls(0),nbp(0),nba(0),Ms(0),ms(0),dqm(0),trp(0){}
    unsigned int ls;
    unsigned int nbp;
    unsigned int nba;
    unsigned int Ms;
    unsigned int ms;
    unsigned int dqm;
    unsigned int trp;
  };

  class MsgBuf{
  public:
    MsgBuf() : msize_(MAX_MSG_SIZE+sizeof(struct msgbuf)+1)
      {
	buf = new unsigned char[MAX_MSG_SIZE+sizeof(struct msgbuf)+1];
	ptr_ = (msgbuf*)buf;
	ptr_->mtype = MSQM_MESSAGE_TYPE_NOP;
      }

    MsgBuf(unsigned int size, unsigned int type) : msize_(size)
      {
	buf = new unsigned char[msize_+sizeof(struct msgbuf)+1];
	ptr_ = (msgbuf*)buf;
	ptr_->mtype = type;
      }
    size_t msize(){return msize_;}
    virtual ~MsgBuf(){delete[] buf;}
    msgbuf* operator->(){return ptr_;}
  private:
    struct msgbuf *ptr_;
    unsigned char *buf;
    size_t msize_;
    friend class MasterQueue;
    friend class SlaveQueue;
  };
}
#endif
