#ifndef ICUtilityToolbox_HH
#define ICUtilityToolbox_HH

#include "ICTypeDefs.hh"

#include <string>
#include <sstream>

namespace ICCoreUtils
{
  template <class T> std::string ToString(const T & aT)
  {
    std::ostringstream oss;
    oss<<aT;
    return oss.str();
  }


  std::string GetEnv(const std::string & aName);
  
  void MkDir(const std::string & aDirPathName);


  void RemWS(std::string & aStr);

  void ToLower(std::string & aStr);

  bool IsWhite(char aCh);

  void Copy(const std::string & aSrc, const std::string & aDest);

  u32 GetFileSize(const std::string & aFileName);

  i32 KeyPress();


  double Mean(u16 * aBuf, u32 aLen);
  double StDev(u16 * aBuf, double aMean, u32 aLen); 

  double Mean(i16 * aBuf, u32 aLen);
  double StDev(i16 * aBuf, double aMean, u32 aLen); 

  void MSBtoLSB(u32 &l_in );
  void ToBin(unsigned long l_in, std::string& s_out , std::string& highChar, std::string& lowChar);
 

  void Sleep(u32 ams);


}//~namespace ICCoreUtils

#endif //~ICUtilityToolbox_HH
