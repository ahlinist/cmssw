#include "HardwareAccessException.hh"

namespace ICCoreUtils
{
  
  HardwareAccessException::HardwareAccessException(const std::string & aProblem):
    ICException(aProblem)
  {
    
  }

  HardwareAccessException::~HardwareAccessException()throw()
  {
    
  }



}//~namespace ICCoreUtils
