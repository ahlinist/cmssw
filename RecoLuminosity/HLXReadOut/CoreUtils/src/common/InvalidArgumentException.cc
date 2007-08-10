#include "InvalidArgumentException.hh"

namespace ICCoreUtils
{

  InvalidArgumentException::InvalidArgumentException(const std::string & aArgType, 
						     const std::string & aProblem):
    ICException("")
  {
    mErrorDescription="Argument of type: \"";
    mErrorDescription+=aArgType;
    mErrorDescription+="\" is invalid because ";
    mErrorDescription+=aProblem;
  }

  InvalidArgumentException::~InvalidArgumentException()throw()
  {

  }

}//~namespace ICCoreUtils
