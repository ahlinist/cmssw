#include "ICException.hh"

namespace ICCoreUtils
{
  
  class GenericException:public ICException
  {
  public:
    GenericException();
    
    virtual inline void Raise(){throw *this;}
    
    virtual ~GenericException()throw();


  };//~class GenericExcxeption


}//~namespace ICCoreUtils
