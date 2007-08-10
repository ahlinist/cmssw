#ifndef HardwareAccessException_HH
#define HardwareAccessException_HH

#include "ICException.hh"

namespace ICCoreUtils
{

  class HardwareAccessException:public ICException
  {
  public:
    
    HardwareAccessException(const std::string & aErrorDescription);

    
    virtual ~HardwareAccessException()throw();


    virtual inline void Raise(){throw *this;}

  private:



  };//~class HardwareAccessException

}//~namespace ICCoreUtils

#endif//~HardwareAccessException_HH
