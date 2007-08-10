#include "FileNotFoundException.hh"

namespace ICCoreUtils
{
  
  FileNotFoundException::FileNotFoundException(const std::string & aFileName):
    ICException(std::string("File name = ")+aFileName)
  {
    
  }

  FileNotFoundException::~FileNotFoundException()throw()
  {
    
  }



}//~namespace ICCoreUtils
