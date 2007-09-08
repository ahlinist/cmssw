/*
  ROOT mutex class
  Copyright John Jones 2007

  ROOT is not multi-threaded, so you have to keep
  a record of what thread is accessing it so they don't
  do it at the same time ;).
*/

#ifndef ROOTMUTEX_HH
#define ROOTMUTEX_HH

// Using pthreads
#include <pthread.h>

// Type definitions used by the HAL, etc...
#include "ICTypeDefs.hh"

// Namespace for the HCAL HLX
namespace HCAL_HLX
{

  // We shall be using the IC core utility library
  using namespace ICCoreUtils;
  using namespace std;

  // Prototype class definition
  class ROOTMutex {

  public:

    // Constructor
    ROOTMutex();

    // Destructor
    ~ROOTMutex();

    void Lock();
    void Unlock();

  private:
    // The one-and-only mutex object
    static pthread_mutex_t *mMutex;
    // Lock count
    static u32 mLockCount;

  }; //~class ROOTMutex

} //~namespace HCAL_HLX

#endif //~ROOTMUTEX_HH
