#ifndef ICExampleException_HH_
#define ICExampleException_HH_

#include "ICException.hh"

#include <string>
#include <vector>

namespace ICCoreUtils
{

  class ICExampleException:virtual public ICException
  {
  public: 
    enum ERROR_CODE{EXAMPLE_ERROR_CODE=0, EXAMPLE_NO_MEM=1, EXAMPLE_ALLOCATE_ERROR=2};
    
    ICExampleException(ERROR_CODE aCode);

    /*
     * 
     *
     */
    virtual const char * what()const throw();

    /*
     *
     *
     *
     */
    virtual std::string What()const throw();

    /*
     *
     *
     *
     */
    virtual inline void Raise(){throw *this;}
    
    
  public:
    inline static std::vector<std::string> & Messages()
    {
      static std::vector<std::string> lErrorMessageLUT;
      return lErrorMessageLUT;
    }
    
    friend class ErrorCodeLUTPopulator;
    class ErrorCodeLUTPopulator
    {
    public:
      ErrorCodeLUTPopulator();
    };
    
  private:
    ERROR_CODE mError;
    static ErrorCodeLUTPopulator mErrorCodeLUTPopulator;
    
  };//~class ICExampleException




}//~namespace ICCoreUtils

#endif//~ICExampleException_HH_
