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

  // Static user count variable
  u32 ROOTMutex::mUserCount = 0;
  // Static mutex
  pthread_mutex_t ROOTMutex::mMutex = PTHREAD_MUTEX_INITIALIZER;

  // Constructor
  ROOTMutex::ROOTMutex() {
    mUserCount++;
    cout << "In " << __PRETTY_FUNCTION__ << endl;
    cout << "User count = " << dec << mUserCount << endl;
  }
  
  // Destructor
  ROOTMutex::~ROOTMutex() {
    mUserCount--;
  }
  
  void ROOTMutex::Lock() {
    pthread_mutex_lock(&mMutex);
  }

  void ROOTMutex::Unlock() {
    pthread_mutex_unlock(&mMutex);
  }

} //~namespace HCAL_HLX

