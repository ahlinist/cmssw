#ifndef ConfigFileErrorException_HH
#define ConfigFileErrorException_HH

#include "ICException.hh"
#include "ICTypeDefs.hh"

#include <string>

namespace ICCoreUtils
{

  class ConfigFileErrorException : public ICException
  {
  public:
    
    ConfigFileErrorException(const std::string & aFileName, 
			     const std::string & aFileType, 
			     const std::string & aErrorHint,
			     i32 aCfLineNo);
    
    virtual ~ConfigFileErrorException()throw();
    
    virtual inline void Raise(){throw *this;}


    inline std::string GetFileName()const{return mConfigFileName;}
    inline std::string GetFileType()const{return mConfigFileType;}
    inline std::string GetErrorHint()const{return mErrorHint;}
    inline i32 GetConfFileLineNumber()const{return mConfFileLine;}

  private:

    std::string mConfigFileName;
    std::string mConfigFileType;
    std::string mErrorHint;
    i32 mConfFileLine;
    
  };

}//~namespace IC

#endif//~ConfigFileErrorException_HH
