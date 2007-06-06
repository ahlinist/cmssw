#ifndef FURAWEVENT_H
#define FURAWEVENT_H

#include "interface/shared/frl_header.h"
#include "interface/shared/fed_header.h"
#include "interface/shared/fed_trailer.h"

#include <string>
#include <vector>
#include <pthread.h>

#include "i2o/i2o.h"

class FUAdapter;


class FURawEvent
{
 private:
  static const unsigned long FED_HCTRLID = 0x50000000;
  static const unsigned long FED_TCTRLID = 0xa0000000;

  static const int fedhs = sizeof(fedh_t); // for convenience, store the header size
  static const int fedts = sizeof(fedt_t); // for convenience, store the trailer size
  static const int frlhs = sizeof(frlh_t); // for convenience, store the trailer size


 public:
  static const unsigned long INVALID_EVENT_HANDLE   = 0x10000000;
  static const unsigned long INTERNAL_HANDLE_OFFSET = 0x20000000;
  static const unsigned long INVALID_BU_INSTANCE    = 10000;

  /** ERROR CODES */
  static const int UFO_ERROR            =  -1;
  static const int FRL_TRIGGER_MISMATCH =  -2;
  static const int FRL_LASTSEG_FLNOTSET =  -3;
  static const int FRL_LASTSEG_FLISWRNG =  -4;
  static const int FED_DROPPED_DUPLDATA =  -5;
  static const int FED_TRIGGER_MISMATCH =  -6;
  static const int FED_TRAILER_MMARKER  =  -7;
  static const int FED_HEADER_MMARKER   =  -8;
  static const int I2O_BLOCK_MISMATCH   =  -9;
  static const int FED_CRCCHK_FAILED    = -10;

  /** destructor (only invoked when FU is halted) */
  virtual ~FURawEvent();
  
  /** Get the real event handle */
  unsigned int getHandle() const 
    {return eventHandle_;}

  /** Get the internal event handle */
  unsigned int getInternalHandle() const 
    {return internalHandle_;}

  /** Get the level1Id */
  unsigned int getLevel1Id() const 
    {return l1Id_;}  

  /** check the transaction context for this event */
  bool checkTransactionContext(unsigned int context) const
    {return (context%INTERNAL_HANDLE_OFFSET)==internalHandle_;}

   /** return the transaction context to be used for this event */
  unsigned int transactionContext() const 
    { return (internalHandle_ + INTERNAL_HANDLE_OFFSET);}

  /** reset (recycle) this event and free its memory */
  virtual void reset(bool);

  /** Process a message frame. upon success, return the superfragment number.
      On failure returns a negative code
   */
  virtual int processMsg(I2O_MESSAGE_FRAME *);


  /** a FED data structure */
  struct RawData
  {
    std::string name_;
    int size_;
    int free_;
    unsigned char * data_;
  };

  /** access to fed data by index (used by DaqEvent)*/
  virtual RawData *operator[](int fedid);

  unsigned char nextFragment() const { return fragmentCount_;}

  //  unsigned char nbReqs() const {return nbRUs;}
  
  unsigned int outstandingReqs() const {return outstandingReqs_;}

  unsigned int getPending()  const {return pendingFset;} 

  bool fragmentComplete(int fragind){return builtFlags[fragind];}

  bool isNew() const {return isNew_;}
  void lock();
  void free();

  /**get the BU instance to be used for subsequent transactions */
  unsigned long getBU() const
    {
      return buInstance_;
    }

    
 protected:

  /** constructor from internal handle (only used by factory at creation) */
  FURawEvent(unsigned int);   // constructor is not exposed 

  unsigned int fragmentCount_;
  unsigned int blockCount_;
  unsigned int outstandingReqs_; // the number of expected superfragments to declare
                        // the even as "built"

  //event data structure
  std::vector<RawData*> myData_;
  bool *builtFlags;

 private:

  void dumpFrame(char* data, int len);

  /** Set the level1Id */
  void setLevel1Id(unsigned int l1id)
    {l1Id_ = l1id;}

  /** Set the event internal handle, only to be used by the factory */
  void setHandle(unsigned int handle)
    {
      eventHandle_ = handle;
    }

  /** Reassembles data into fed blocks navigating the superfragments.
      Performs consistency checks */
  int checkin_data(std::vector<unsigned char*> &);

  void setPending(unsigned int ifset); 

  std::vector<unsigned char *> block_adrs;

  unsigned int eventHandle_;
  
  const unsigned long internalHandle_;

  unsigned int pendingFset;

  pthread_mutex_t lock_;
  pthread_cond_t ready_;

  /**request data for a certain fed and timeout if none is returned
     (only used with partial event transfer) */
  RawData *timedRequest(int, int);
  /**set the BU instance to be used for subsequent transactions */
  void setBU(unsigned long buinstance)
  {
    buInstance_ = buinstance;
  }
  
  unsigned long buInstance_;

  /**release the timed request */
  void release();
  
  bool isNew_;
  static const unsigned int nFEDs = 0x800; //10 bits
  static FUAdapter *adapter_;

  static RawData *nulldata;
  static int errors[11];
  unsigned int l1Id_;
  friend class FURawEventFactory;

  bool doCrcCheck_;
  
};
#endif
