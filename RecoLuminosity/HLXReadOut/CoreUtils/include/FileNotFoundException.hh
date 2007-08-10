#ifndef FileNotFoundException_HH
#define FileNotFoundException_HH

#include "ICException.hh"

namespace ICCoreUtils
{

  class FileNotFoundException:public ICException
  {
  public:
    
    FileNotFoundException(const std::string & aErrorDescription);

    
    virtual ~FileNotFoundException()throw();


    virtual inline void Raise(){throw *this;}

  private:



  };//~class FileNotFoundException

}//~namespace ICCoreUtils

#endif//~FileNotFoundException_HH
