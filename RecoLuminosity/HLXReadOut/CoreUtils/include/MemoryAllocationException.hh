#include "ICException.hh"
#include "ICTypeDefs.hh"
#include "ICUtilityToolbox.hh"

#include <string>

namespace ICCoreUtils
{
  
  class MemoryAllocationException:public ICException
  {
  public:
    
    MemoryAllocationException(const std::string & aType, u32 aQuant=1);
    ~MemoryAllocationException() throw();

    virtual inline void Raise(){throw *this;}
    
  private:
    MemoryAllocationException();
    //MemoryAllocationException(const MemoryAllocationException &);
    MemoryAllocationException & operator=(const MemoryAllocationException &);

  };//~class MemoryAllocationException

}//~namespace ICCoreUtils
