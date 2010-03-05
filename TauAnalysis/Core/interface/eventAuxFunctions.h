#ifndef TauAnalysis_Core_eventAuxFunctions_h
#define TauAnalysis_Core_eventAuxFunctions_h

#include "DataFormats/Common/interface/Ref.h"

template<typename T>
bool isValidRef(const edm::Ref<T>& ref)
{
  return ( (ref.isAvailable() || ref.isTransient()) && ref.isNonnull() );  
}

#endif
