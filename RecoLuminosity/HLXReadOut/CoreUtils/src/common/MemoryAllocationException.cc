#include "MemoryAllocationException.hh"

namespace ICCoreUtils
{
  
  MemoryAllocationException::MemoryAllocationException(const std::string & aType, 
						       u32 aQuant):
    ICException("")
  {
    std::string lTemp("Memory allocation for ");
    lTemp+=ICCoreUtils::ToString(aQuant);
    lTemp+=" object";
    lTemp+=aQuant==1?" ":"s ";
    lTemp+="of type ";
    lTemp+=aType;
    mErrorDescription=lTemp;
  }
  
  MemoryAllocationException::~MemoryAllocationException()throw()
  {
  }



}//~namespace ICCoreUtils
