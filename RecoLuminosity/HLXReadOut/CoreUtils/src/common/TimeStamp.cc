#include "TimeStamp.hh"

#include <sstream>
#include <iostream>

namespace ICCoreUtils
{

  TimeStamp::TimeStamp():
    mTheTime(&mTime)
  {
    Stamp();
  }
  
  void TimeStamp::Stamp()
  {
    // there's no localtime_r on windows, so I've implemented 
    // this minimal effort hack... unfortunately it doesn't work! HAH! 
 
    time_t t=time(0);
    mTheTime=localtime(&t);
//     memcpy(mTheTime, &mTime, sizeof(mTime));
//     std::cout<<"Stamp "<<strncmp(reinterpret_cast<const char *>(mTheTime), 
// 				 reinterpret_cast<const char *>(&mTime),sizeof(mTime))
// 	     <<"   "<<sizeof(mTime)<<std::endl;
//     mTheTime=&mTime;
  }
  
  std::string TimeStamp::getHHmm(char timeSep)const
  {
    std::ostringstream oss;
    oss<<mTheTime->tm_hour;
    std::string hour=(mTheTime->tm_hour>9?oss.str():'0'+oss.str());
    oss.str("");
    oss<<mTheTime->tm_min;
    std::string min=(mTheTime->tm_min>9?oss.str():'0'+oss.str());
    return hour+timeSep+min;
  }
  std::string TimeStamp::getHHmmss(char timeSep) const
  {
    std::ostringstream oss;
    oss<<mTheTime->tm_sec;
    std::string sec=mTheTime->tm_sec>9?oss.str():'0'+oss.str(); 
    return getHHmm(timeSep)+timeSep+sec;    
  }

  std::string TimeStamp::getDDMMYYYY(char dateSep)const
  {
    using std::ostringstream;
    ostringstream oss;
    
    oss<<mTheTime->tm_mday;
    std::string day=(mTheTime->tm_mday>9?oss.str():'0'+oss.str());
    oss.str("");
    
    oss<<mTheTime->tm_mon+1;
    std::string month=((mTheTime->tm_mon+1)>9?oss.str():'0'+oss.str());
    oss.str("");
    
    oss<<day<<dateSep<<month<<dateSep<<(1900+(mTheTime->tm_year));
    return oss.str();
  }
  
  std::string TimeStamp::getYYYYMMDD(char dateSep)const
  {
    using std::ostringstream;
    ostringstream oss;
    oss<<mTheTime->tm_mon+1;
    std::string month=((mTheTime->tm_mon+1)>9?oss.str():'0'+oss.str());
    oss.str("");
    
    oss<<mTheTime->tm_mday;
    std::string day=(mTheTime->tm_mday>9?oss.str():'0'+oss.str());
    oss.str("");
    oss<<(1900+(mTheTime->tm_year))<<dateSep
       <<month<<dateSep
       <<day;
    return oss.str();
  }
  
  std::string TimeStamp::getYYYYMMDDHHmm(char dateSep, char dateTimeSep, char timeSep)const
  {
    return getYYYYMMDD(dateSep)+dateTimeSep+getHHmm(timeSep);
  }
  
  std::string TimeStamp::getYYYYMMDDHHmmss(char dateSep, char dateTimeSep, char timeSep)const
  {
    std::string ymdhm=getYYYYMMDDHHmm(dateSep,dateTimeSep,timeSep);
    std::ostringstream oss;
    oss<<mTheTime->tm_sec;
    std::string sec=mTheTime->tm_sec>9?oss.str():'0'+oss.str();  
    return ymdhm+timeSep+sec;
  }


} //~namespace ICCoreUtils
