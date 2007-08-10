#include "ICExampleException.hh"

//#include <iostream>

namespace ICCoreUtils
{
  ICExampleException::ICExampleException(ERROR_CODE aCode):
    ICException(ICExampleException::Messages()[aCode]),
    mError(aCode)
  {
    
  }

  
  const char * ICExampleException::what()const throw()
  {
    return this->What().c_str();
  }


  std::string ICExampleException::What()const throw()
  {
    // call the base method...
    std::string lErrBuf=ICException::What();
    /*
     * any additional things can go here...
     * for example, add the history data automatically...:
     */
    lErrBuf+=ICException::History();

    return lErrBuf;
  }


  /*
   * implement the constructor for the message lut initialiser
   * and define all the error messages. 
   *
   *
   */
  ICExampleException::ErrorCodeLUTPopulator::ErrorCodeLUTPopulator()
  {
    std::vector<std::string> & lMessageLUT=ICExampleException::Messages();
    /*
     * add the error strings to the LUT, placing them in order
     * to be correctly referred to by the ERROR_CODE enumerate.
     *
     */
    lMessageLUT.push_back("First Message: nothing.");
    lMessageLUT.push_back("Not enough memory available to fulfil the request.");
    lMessageLUT.push_back("Error in memory allocation.");
  }

  /*
   * instantiate the error code populator object. 
   *
   */
  ICExampleException::ErrorCodeLUTPopulator mErrorCodeLUTPopulator;

}//~namespace ICCoreUtils
