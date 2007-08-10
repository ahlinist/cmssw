#include "MyLog.hh"
#include "TimeStamp.hh"
#include <iostream>
#include <fstream>
// #include <iomanip>

//
// see comment in header file about this code.
//


namespace ICCoreUtils
{

  MyLog::MyLog(std::string OutputDir, std::string OutputFile, std::string LogSig, LOG_TYPE LogType):
    mStamp(),
    mLogOptions(0),  
    mLogFile(OutputDir),
    mLogSig(LogSig),
    mLogType(LogType)
  {
    // std::cout<<__PRETTY_FUNCTION__<<"Constructing "<<std::endl;

    if(mLogFile[mLogFile.size()-1]!='\\')
      mLogFile+='\\';
    mLogFile+=OutputFile;
    
    mLogStream.open(mLogFile.c_str(), std::ios::app);
    if(!mLogStream.is_open()){
      std::cerr<<"unable to open file "<<mLogFile <<std::endl;
      return; // throw...
    }
    
    mLogStream<<"\n\n"
	      <<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
	      <<"\n"
	      <<mLogFile
	      <<" opened for logging on \n"
	      <<mStamp.getDDMMYYYY()
	      <<" at "<<mStamp.getHHmm()<<'\n'
	      <<"For the purpose of: "<<mLogSig<<"\n\n"
	      <<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n";
    
    
  }
  
  MyLog::~MyLog()
  {
    //std::cout<<__PRETTY_FUNCTION__<<" Beginning" <<std::endl;
    mStamp.Stamp();
    mLogStream<<"\n\n"
	      <<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"
	      <<mLogFile
	      <<"\nclosed on "
	      <<mStamp.getDDMMYYYY()
	      <<" at "<<mStamp.getHHmm()<<'\n'
	      <<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
    mLogStream.flush(); // shouldn't be needed, but call anyway just in case...
    mLogStream.close();
    //    std::cout<<__PRETTY_FUNCTION__<<" end." <<std::endl;

  }

  
  
  
  MyLog & MyLog::operator<<(const char * msg)
  {
    std::string message(msg);
    WriteLog(message);
    return *this;
  }
  
  MyLog & MyLog::operator<<(std::string & msg)
  {
    WriteLog(msg);
    return *this;
  }

  void MyLog::WriteLog(std::string & msg)
  {
    mStamp.Stamp();
    
    mLogStream<<mStamp.getHHmmss()<<" : "<<msg<<'\n';
    
    //
    // performance issue? error log flushes the buffer at 
    // each call... and echos to std::cerr
    //

    if(mLogType==LOG_ERROR){
      mLogStream<<std::flush;
      
      std::cerr<<"\n***** ERROR *****\n"
	  <<mStamp.getHHmmss()<<" : "
	  <<msg
	  <<std::endl;
    }
    
  }
  

} //~namespace ICCoreUtils
