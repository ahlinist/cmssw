#ifndef OracleDBException_HH
#define OracleDBException_HH

#include "ICException.hh"

namespace ICCoreUtils
{

  class OracleDBException:public ICException
  {
  public:
    
    OracleDBException(const std::string & aErrorDescription);

    
    virtual ~OracleDBException()throw();


    virtual inline void Raise(){throw *this;}

  private:



  };//~class OracleDBException

}//~namespace ICCoreUtils

#endif//~OracleDBException_HH
