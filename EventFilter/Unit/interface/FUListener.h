#ifndef __FULISTENER_H
#define __FULISTENER_H
//-----------------------------------------------
//
/// Author: EM
/// PH/CMD
//
//  MODIFICATION:
//  $Log: FUListener.h,v $
//  Revision 1.1  2005/10/19 09:10:35  meschi
//  first import from COSINE
//
//  Revision 1.1  2004/05/14 13:23:58  meschi
//  set of Listener/Adapter/Proxy for new BU/FU
//
//
//-----------------------------------------------
//

/** class FUListener
    define call back function for FU message take
    (replaces i2oFUListener)
*/

#include "xdaq/i2oListener.h" //from daq/xdaq/include

class FUListener:  public virtual i2oListener {
 public:

  /** default constructor */
  FUListener();

 protected:
  /** called back upon reception of I2O_FU_MSG_TAKE */
  virtual void realTake(BufRef* ref) = 0;

};
	

#endif
