#ifndef TimeStamp_HH
#define TimeStamp_HH

/////////////////////////////////////////////////
//
//Wrapper for system time functions
//providing a simple time stamp facility 
//
//Can be used and discarded, or 're-stamped' 
//using the Stamp() member functon
//
//M. Noy 
//15-02-2004
////////////////////////////////////////////////

//
// this code has now been integrated into the 
// Fed9UTestBench and may be out of date...
// use that version.
//

#include <string>
#include <time.h>
//#include <ctime.h>

namespace ICCoreUtils
{ 
  //using std::string;
  
  class TimeStamp
  {
    
  public:
    TimeStamp();
    ~TimeStamp(){}
    
    void Stamp();
    
    //int getTotalSeconds()const
    inline int getSeconds()const{return mTheTime->tm_sec;}
    inline int getMinutes()const{return mTheTime->tm_min;}
    inline int getHours()const{return mTheTime->tm_hour;}
    inline int getDayOfWeek()const{return mTheTime->tm_wday;}
    inline int getDayOfMonth()const{return mTheTime->tm_mday;}
    inline int getDayOfYear()const{return mTheTime->tm_yday;}
    //  inline int getWeek()const{return mTheTime->tm_}
    inline int getMonth()const{return mTheTime->tm_mon+1;}
    inline int getYear()const{return mTheTime->tm_year+1900;}
    
    std::string getHHmm(char timeSep=':')const;
    std::string getHHmmss(char timeSep=':') const;
    std::string getDDMMYYYY(char dateSep='-')const;
    std::string getYYYYMMDD(char dateSep='-')const;
    std::string getYYYYMMDDHHmm(char dateSep='-', char dateTimeSep='_', char timeSep=':')const;
    std::string getYYYYMMDDHHmmss(char dateSep='-', char dateTimeSep='_', char timeSep=':')const;
    
    
  private:

    struct tm mTime;
    struct tm * mTheTime;
    
  private:
    TimeStamp(const TimeStamp & ts);
    
  }; //~class TimeStamp

} //~namespace ICCoreUtils

#endif //TimeStamp_HH
