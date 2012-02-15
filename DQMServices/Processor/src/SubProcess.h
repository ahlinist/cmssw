#ifndef DQMEVF_SUBPROCESS_H
#define DQMEVF_SUBPROCESS_H

#include "MsgQueue.h"
#include <string>

#include <iostream>
#include <boost/shared_ptr.hpp>

// subprocess states: -1000 never started -1: crashed 0: exited successfully 1: running
// @@EM ToDo: replace magic numbers with enum

namespace dqmevf{
  
  class SubProcess{
  public:
    SubProcess()
      : ind_(100000)
      , pid_(-1)
      , alive_(-1000)
      , restart_countdown_(0)
      , save_nbp_(0)
      , save_nba_(0)
      , save_ndqm_(0)
      , save_scalers_(0)
      , reported_inconsistent_(false)
      {}
    SubProcess(int ind, pid_t pid)
      : ind_(ind)
      , pid_(pid)
      , alive_(-1)
      , mqS_(new MasterQueue(ind))
      , mqm_(new MasterQueue(monitor_queue_offset_+ind))
      , restart_countdown_(0)
      , save_nbp_(0)
      , save_nba_(0)
      , save_ndqm_(0)
      , save_scalers_(0)
      , reported_inconsistent_(false)
      {
	mqS_->drain();
	mqm_->drain();
      }
    SubProcess(const SubProcess &b)
      : ind_(b.ind_)
      , pid_(b.pid_)
      , alive_(b.alive_)
      , mqS_(b.mqS_)
      , mqm_(b.mqm_)
      , restart_countdown_(b.restart_countdown_)
      , reported_inconsistent_(b.reported_inconsistent_)
      {
      }
    SubProcess &operator=(const SubProcess &b);

    virtual ~SubProcess()
      {
      }
    void disconnect();

    void drainSupQueue() {mqS_->drain();}

    void setStatus(int st);

    //todo
    int queueId(){return (mqm_.get()!=0 ? mqm_->id() : 0);}
    int queueStatus(){return (mqm_.get() !=0 ? mqm_->status() : 0);}
    int queueOccupancy(){return (mqm_.get() !=0 ? mqm_->occupancy() : -1);}
    pid_t queuePidOfLastSend(){return (mqm_.get() !=0 ? mqm_->pidOfLastSend() : -1);}
    pid_t queuePidOfLastReceive(){return (mqm_.get() !=0 ? mqm_->pidOfLastReceive() : -1);}
    pid_t pid() const {return pid_;}
    int alive() const {return alive_;}
    struct prg &params(){return prg_;}
    void setParams(struct prg *p);

    //master post and receive on various queues
    int postSup(MsgBuf &ptr)  {return mqS_->post(ptr);}
    int postMon(MsgBuf &ptr)  {return mqm_->post(ptr);}

    unsigned long rcvSup(MsgBuf &ptr) {return mqS_->rcv(ptr);}
    unsigned long rcvMon(MsgBuf &ptr) {return mqm_->rcv(ptr);}

    unsigned long rcvNonBlockingSup(MsgBuf &ptr) {return mqS_->rcvNonBlocking(ptr);}
    unsigned long rcvNonBlockingMon(MsgBuf &ptr) {return mqm_->rcvNonBlocking(ptr);}

    //slave post and receive on various queues
    int postSlaveSup(MsgBuf &ptr) {return sqS_->post(ptr);}
    int postSlaveMon(MsgBuf &ptr) {return sqm_->post(ptr);}

    unsigned long rcvSlaveSup(MsgBuf &ptr) {return sqS_->rcv(ptr);}
    unsigned long rcvSlaveMon(MsgBuf &ptr) {return sqm_->rcv(ptr);}

    unsigned long rcvSlaveNonBlockingSup(MsgBuf &ptr) {return sqS_->rcvNonBlocking(ptr);}
    unsigned long rcvSlaveNonBlockingMon(MsgBuf &ptr) {return sqm_->rcvNonBlocking(ptr);}

    int forkNew();

    std::string const &reasonForFailed()const {return reasonForFailed_;}
    bool inInconsistentState() const {return reported_inconsistent_;}
    void setReasonForFailed(std::string r){reasonForFailed_ = r;}
    void setReportedInconsistent(){reported_inconsistent_ = true;}
    int &countdown(){return restart_countdown_;}
    static const unsigned int monitor_queue_offset_ = 200;

  private:
    int ind_;
    pid_t pid_;
    int alive_;
    boost::shared_ptr<MasterQueue> mqS_; //to be turned to real object not pointer later
    boost::shared_ptr<MasterQueue> mqm_; //to be turned to real object not pointer later

    SlaveQueue*                    sqS_;  // every subprocess will create its instance at fork 
    SlaveQueue*                    sqm_;  // every subprocess will create its instance at fork 
    std::string reasonForFailed_;
    struct prg prg_;
    int restart_countdown_;

    int save_nbp_;
    int save_nba_;
    unsigned int save_ndqm_;
    unsigned int save_scalers_;
    bool reported_inconsistent_;
  };


}
#endif
