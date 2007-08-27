#include "OracleDBException.hh"

namespace ICCoreUtils
{
  
  OracleDBException::OracleDBException(const std::string & aProblem):
    ICException(aProblem)
  {
    
  }

  OracleDBException::~OracleDBException()throw()
  {
    
  }



}//~namespace ICCoreUtils
