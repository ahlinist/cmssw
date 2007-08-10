#include "FileNotOpenedException.hh"

namespace ICCoreUtils
{
  
  FileNotOpenedException::FileNotOpenedException(const std::string & aFileName):
    ICException(std::string("Unable to open file with name = ")+aFileName)
  {
    
  }

  FileNotOpenedException::~FileNotOpenedException()throw()
  {
    
  }



}//~namespace ICCoreUtils
