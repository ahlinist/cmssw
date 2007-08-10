#include "ICException.hh"

namespace ICCoreUtils
{
  
  class InvalidArgumentException:public ICException
  {
  public:
    InvalidArgumentException(const std::string & aArgType, 
			     const std::string & aProblem);
    
    virtual inline void Raise(){throw *this;}
    
    virtual ~InvalidArgumentException()throw();


  };//~class InvalidArgumentExcxeption


}//~namespace ICCoreUtils
