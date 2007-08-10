#include "ICException.hh"
#include "ConfigFileErrorException.hh"



namespace ICCoreUtils
{

  ConfigFileErrorException::ConfigFileErrorException(const std::string & aFileName, 
						     const std::string & aFileType,
						     const std::string & aErrorHint,
						     i32 aCfLineNo):
    ICException(""),
    mConfigFileName(aFileName),
    mConfigFileType(aFileType),
    mErrorHint(aErrorHint),
    mConfFileLine(aCfLineNo)
  {
    std::string lTemp(std::string("Configuration file name = ")+mConfigFileName+
		      std::string(" (type = ")+mConfigFileType+std::string("), line = ")+
		      ICException::ToString<i32>(mConfFileLine)+
		      std::string(" has error :")+
		      mErrorHint);
    ICException::SetErrorDescription(lTemp);
  }

  ConfigFileErrorException::~ConfigFileErrorException()throw()
  {

  }

}//~namespace ICCoreUtils
