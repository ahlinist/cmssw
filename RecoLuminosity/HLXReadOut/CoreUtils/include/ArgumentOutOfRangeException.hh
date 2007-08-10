#include "ICException.hh"

#include <string>

namespace ICCoreUtils
{
  
  template <class T> 
  class ArgumentOutOfRangeException:public ICException
  {
  public:
    /*
     * this exception should be used when the argument passed to a 
     * is out of the acceptable range.
     */
    //template <class T>
    ArgumentOutOfRangeException(const std::string & aVariableName,
				const T & aLowBound, 
				bool aLowerEq,
				const T & aArg,
				bool aUpperEq,
				const T & aUpperBound):
      ICException("")
    {
      std::string lTemp("The following inequality is not true for variable\" ");
      lTemp+=aVariableName+"\":\n";
      lTemp+=ICCoreUtils::ICException::ToString(aLowBound);
      lTemp+=aLowerEq?std::string(" <= "):std::string(" < ");
      lTemp+=ICCoreUtils::ICException::ToString(aArg);
      lTemp+=aUpperEq?std::string(" <= "):std::string(" < ");
      lTemp+=ICCoreUtils::ICException::ToString(aUpperBound);
      ICException::mErrorDescription=lTemp;
    }

    

    /*
     * raise this exception.
     */
    virtual inline void Raise(){throw *this;}
    
    
  private:
    

  };//~class ...ArgumentOutOfRangeException...


//   template <class T>
//   ArgumentOutOfRangeException::ArgumentOutOfRangeException(
// 				const std::string & aVariableName,
// 				const T & aLowBound, 
// 				bool aLowerEq,
// 				const T & aArg,
// 				bool aUpperEq,
// 				const T & aUpperBound)


}//~namespace ICCoreUtils
