/*
  ROOT mutex class
  Copyright John Jones 2007

  ROOT is not multi-threaded, so you have to keep
  a record of what thread is accessing it so they don't
  do it at the same time ;).
*/

#include "ROOTMutex.hh"
#include "MemoryAllocationException.hh"
#include <iostream>

// Namespace for the HCAL HLX
namespace HCAL_HLX
{

  // We shall be using the IC core utility library
  using namespace ICCoreUtils;
  using namespace std;

  pthread_mutex_t *ROOTMutex::mMutex = 0;
  u32 ROOTMutex::mLockCount = 0;

  // Constructor
  ROOTMutex::ROOTMutex() {
    if ( mMutex == 0 ) {
      mMutex = new pthread_mutex_t;
      if ( mMutex == 0 ) {
	MemoryAllocationException lExc("Unable to allocate memory for ROOT mutex");
	RAISE(lExc);
      }
      // TODO: Check init failed?
      pthread_mutex_init(mMutex,0);
    }
    mLockCount++;
    cout << "In " << __PRETTY_FUNCTION__ << endl;
    cout << "Lock count = " << dec << mLockCount << endl;
  }
  
  // Destructor
  ROOTMutex::~ROOTMutex() {
    mLockCount--;
    if ( mMutex && (mLockCount == 0) ) {
      pthread_mutex_destroy(mMutex);
      delete mMutex; mMutex = 0;
    }
  }
  
  void ROOTMutex::Lock() {
    pthread_mutex_lock(mMutex);
  }

  void ROOTMutex::Unlock() {
    pthread_mutex_unlock(mMutex);
  }

} //~namespace HCAL_HLX

