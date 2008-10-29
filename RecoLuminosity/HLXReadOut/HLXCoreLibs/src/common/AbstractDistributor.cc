/*
  Abstract distributor functions
  Copyright John Jones 2008
*/

// Prototype class definition
#include "AbstractDistributor.hh"

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;
  
  // Default constructor
  AbstractDistributor::AbstractDistributor() : mErrorCount(0), mErrorMsg("") {
    pthread_mutex_init(&mErrorMutex,NULL);
  }

  // Destructor deletes the hardware interface
  AbstractDistributor::~AbstractDistributor() {
    pthread_mutex_destroy(&mErrorMutex);
  }

  u32 AbstractDistributor::GetErrorCount() {
    return mErrorCount;
  }

  // Last error
  const std::string AbstractDistributor::GetLastError() {
    // Locked to make it thread safe
    pthread_mutex_lock(&mErrorMutex);
    std::string localError = mErrorMsg;
    pthread_mutex_unlock(&mErrorMutex);
    return localError;
  }

  u32 AbstractDistributor::GetNumLostLumiSections() {
    return mNumLostLumiSections;
  }

  void AbstractDistributor::SetError(const std::string & errorMsg) {
    // Locked to make it thread safe
    pthread_mutex_lock(&mErrorMutex);
    mErrorMsg = errorMsg;
    pthread_mutex_unlock(&mErrorMutex);
    mErrorCount++;
  }

} // namespace HCAL_HLX
