#ifndef FileNotOpenedException_HH
#define FileNotOpenedException_HH

#include "ICException.hh"

namespace ICCoreUtils
{

  class FileNotOpenedException:public ICException
  {
  public:
    
    FileNotOpenedException(const std::string & aErrorDescription);

    virtual ~FileNotOpenedException() throw();

    virtual inline void Raise(){throw *this;}

  private:



  };//~class FileNotOpenedException

}//~namespace ICCoreUtils

#endif//~FileNotOpenedException_HH
