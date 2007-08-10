#ifndef MyLog_HH
#define MyLog_HH
#include <string>
#include "TimeStamp.hh"
#include <fstream>





namespace ICCoreUtils
{

  class MyLog
  {
  public:
    // enum LOG_OPT{ECHO_TO_COUT=4};
    enum LOG_TYPE{LOG_INFO=1, LOG_ERROR=2};
    
  public:
    MyLog(std::string OutputDir, std::string OutputFile, std::string LogSig="Standard Log", LOG_TYPE=LOG_INFO);
    ~MyLog();
    
    
    //
    // streaming operator: streams the message to the output file. 
    //
    MyLog & operator<<(std::string & msg);
    MyLog & operator<<(const char * msg);
    
  private:
    TimeStamp mStamp;
    unsigned long mLogOptions;
    std::string mLogFile;
    std::string mLogSig; 
    std::ofstream mLogStream;
    LOG_TYPE mLogType;
    
  private:
    MyLog();
    MyLog(const MyLog & log);
    
    
  private:
    
    //
    // actual engine for the log writing. 
    // both overloaded << operators call this
    //
    void WriteLog(std::string & msg);
    
  }; //~class MyLog
  


} //~ namespace ICCoreUtils


#endif //MyLog_HH
