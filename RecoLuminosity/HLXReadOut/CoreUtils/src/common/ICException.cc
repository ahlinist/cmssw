#include "ICException.hh"
#include "ICUtilityToolbox.hh"
#include "ICTypeDefs.hh"

#ifdef __GNUG__
#include <cxxabi.h>
#endif

namespace ICCoreUtils
{

  ICException::ICException(const std::string & aErrorDescription):
    mErrorDescription(aErrorDescription)
  {
    
  }

  /*
   * method inherited from std::exception.
   * 
   *
   */
  const char * ICException::what()const throw()
  {
    // let What() do the formatting, then just 
    // obtain the pointer to the c-string and return. 
    return this->What().c_str();
  }
  
  std::string ICException::What()const throw()
  {
    std::string lEol("\n========================================================\n");
    std::string lTempBuf(lEol);

    lTempBuf+=std::string("Exception of type: ")+mExceptionName;
    lTempBuf+=std::string("\nRaised in function: ")+GetFunctionOfOrigin();
    lTempBuf+=std::string(" in file: ")+GetFileOfOrigin()
      +std::string(", line: ")+ToString(GetLineOfOrigin());
    
    lTempBuf+=std::string("\nWith message: ")+mErrorDescription;
    
    lTempBuf+=lEol;
    return lTempBuf;
  }
  

  std::string ICException::History()const throw()
  {
    std::string lTempBuf("History:\n");
    u16 lCount=0;
    for(std::list<ExceptionHistoryPoint>::const_iterator i=mHistory.begin();
	i!=mHistory.end(); ++i)
      {
	lTempBuf+=ICException::ToString(lCount++);
	lTempBuf+=" : ";
	lTempBuf+=i->mFunction;
	lTempBuf+=std::string(" in file:")+i->mFile;
	lTempBuf+=std::string(", line:")+ToString(i->mLine);
	lTempBuf+='\n';
      }
    if(mHistory.begin()==mHistory.end())
      {
	lTempBuf+="History is empty.";
      }
    return lTempBuf;
  }

  void ICException::AddLocation(std::string aFunc,
				std::string aFile,
				int aLine)
  {
    // what am i?
    this->SetType();    
    // add information passed to the function
    // to the history list...
    this->AddHistoryElement(aFunc,aFile,aLine);
  }

  std::string ICException::GetFunctionOfOrigin() const
  {
    // check to see if the history contains
    // anything, if not, return the default string.
    if(mHistory.begin()!=mHistory.end())
      return mHistory.begin()->mFunction;
    return std::string("History unpopulated.");
  }

  std::string ICException::GetFileOfOrigin()const
  {
    // check to see if the history contains
    // anything, if not, return the default string.
    if(mHistory.begin()!=mHistory.end())
      return mHistory.begin()->mFile;
    return std::string("History unpopulated.");
  }

  int ICException::GetLineOfOrigin()const
  {   
    // check to see if the history contains
    // anything, if not, return -1.
    if(mHistory.begin()!=mHistory.end())
      return mHistory.begin()->mLine;
    return -1; 
  }

  void ICException::AddHistoryElement(const std::string & aFunc,
					 const std::string & aFile,
					 int aLine)
  {
    ExceptionHistoryPoint lHistPoint;
    lHistPoint.mFunction=aFunc;
    lHistPoint.mFile=aFile;
    lHistPoint.mLine=aLine;
    mHistory.push_back(lHistPoint);
  }
 
  void ICException::SetErrorDescription(const std::string & aNewDesc)
  {
    mErrorDescription=aNewDesc;
  }

  void ICException::SetType()
  {
    // this is fugly but necessary due to the 
    // way that typeid::name() returns the 
    // object type name under g++. 
    // #ifdef __GNUG__
    int lStatus=0;
    mExceptionName=std::string(abi::__cxa_demangle(typeid(*this).name(), 0,0, &lStatus));
    // #else
    // mExceptionName=std::string(typeid(*this).name());
    // #endif
  }

  ICException & operator<<(ICException & aExc, u8 aData)
  {
    std::ostringstream lOss;
    lOss<<aData;
    aExc.mErrorDescription+=lOss.str();
    return aExc;
  }  
  
  ICException & operator<<(ICException & aExc, i8 aData)
  {
    std::ostringstream lOss;
    lOss<<aData;
    aExc.mErrorDescription+=lOss.str();
    return aExc;
  }
  
  ICException & operator<<(ICException & aExc, u16 aData)
  {
    std::ostringstream lOss;
    lOss<<aData;
    aExc.mErrorDescription+=lOss.str();
    return aExc;
  }
  
  ICException & operator<<(ICException & aExc, i16 aData)
  {
    std::ostringstream lOss;
    lOss<<aData;
    aExc.mErrorDescription+=lOss.str();
    return aExc;
  }
  
  ICException & operator<<(ICException & aExc, u32 aData)
  {
    std::ostringstream lOss;
    lOss<<aData;
    aExc.mErrorDescription+=lOss.str();
    return aExc;
  }
  
  ICException & operator<<(ICException & aExc, i32 aData)
  {
    std::ostringstream lOss;
    lOss<<aData;
    aExc.mErrorDescription+=lOss.str();
    return aExc;
  }

  ICException & operator<<(ICException & aExc, const HardwareAddress & aData)
  {
    std::ostringstream lOss;
    lOss<<std::hex<<"0x"<<aData;
    aExc.mErrorDescription+=lOss.str();
    return aExc;
  }
  
  ICException & operator<<(ICException & aExc, const char * aData)
  {
    std::ostringstream lOss;
    lOss<<aData;
    aExc.mErrorDescription+=lOss.str();
    return aExc;
  }
  
  ICException & operator<<(ICException & aExc, const std::string & aData)
  {
    std::ostringstream lOss;
    lOss<<aData;
    aExc.mErrorDescription+=lOss.str();
    return aExc;
  }

}//~namespace ICCoreUtils
