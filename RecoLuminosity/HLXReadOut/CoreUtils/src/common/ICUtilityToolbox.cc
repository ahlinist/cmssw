#include "ICUtilityToolbox.hh"
#include "InvalidArgumentException.hh"
#include "FileNotOpenedException.hh"
#include "ICTypeDefs.hh"

#include <iostream>

#include <fstream>
#include <sstream>
#include <cstdlib>


#include <cmath>

//#ifdef Win32
//#include <io.h>
//#include <conio.h>
//#endif

//#ifdef Linux
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
//#endif



namespace ICCoreUtils
{

  std::string GetEnv(const std::string & aName)
  {
    char * lTemp=std::getenv(aName.c_str());
    if(lTemp==0){
      return std::string("");      

    }
    return std::string(lTemp);
  }

  void MkDir(const std::string & aDirPathName)
  {
    
//  #ifdef Win32
//    i32 lRet=mkdir(aDirPathName.c_str());
//  #endif
//    #ifdef Linux    
    i32 lRet=mkdir(aDirPathName.c_str(), 01777);
//    #endif

    if(lRet!=0)
      {
	InvalidArgumentException lExc("const std::string &",
	     "Unable to create the directory specified.");
	lExc<<" Return code = "<<lRet;
	RAISE(lExc);
      }
  }


  void RemWS(std::string & aStr)
  {
    if(aStr.size()==0)
      return;
    
    u32 j=0;
    for(u32 i=0; i<aStr.length(); i++)
      {     
	if(!IsWhite(aStr[i]))
	  aStr[j++]=aStr[i];
      }
    aStr=aStr.substr(0,j);
  }
  
  void ToLower(std::string & aStr)
  {
    for(u16 i=0; i<aStr.length();++i)
      aStr[i]=tolower(aStr[i]);
  }
  

  bool IsWhite(char aCh)
  {
    return ((aCh==' ')||(aCh=='\t')||(aCh=='\n'));
  }


  void Copy(const std::string & aSrc, const std::string & aDest)
  {
    std::cout<<"Copying "<<aSrc<<" \nto\n"<<aDest<<std::endl;

    std::ifstream lSrcStr(aSrc.c_str(), std::ifstream::binary);
    if(!lSrcStr){
      FileNotOpenedException lExc(aSrc);
      RAISE(lExc);
    }
    std::ofstream lDestStr(aDest.c_str(),std::ofstream::binary);
    if(!lDestStr){
      FileNotOpenedException lExc(aDest);
      RAISE(lExc);
    }
    lSrcStr.seekg(0,std::ifstream::end);
    u32 lNoBytes=lSrcStr.tellg();
    lSrcStr.seekg(0);
    
    char * lBuf=new char [lNoBytes];
    lSrcStr.read(lBuf, lNoBytes);
    lDestStr.write(lBuf, lNoBytes);
    delete [] lBuf;    
  }


  u32 GetFileSize(const std::string & aFileName)
  {
    std::ifstream lSrcStr(aFileName.c_str(), std::ifstream::binary);
    if(!lSrcStr){
      FileNotOpenedException lExc(aFileName);
      RAISE(lExc);
    }
    lSrcStr.seekg(0,std::ifstream::end);
    u32 lNoBytes=lSrcStr.tellg();
    lSrcStr.close();
    return lNoBytes;
  }

  i32 KeyPress(){
 //   #ifdef Win32
 //   return kbhit();
//    #endif
    // linux imp goes here.
    return 0;
  }

  // these should be templated:

  double Mean(u16 * aBuf, u32 aLen)
  {
    double lTemp=0.0;
    for(unsigned long i=0; i<aLen; i++){
      lTemp+=aBuf[i];
    }
    return lTemp/static_cast<double>(aLen);
  }

  double StDev(u16 * aBuf, double aMean, u32 aLen)
  {
    double lSumSq=0.0;
    for(unsigned long i=0; i<aLen;i++){
      lSumSq+=(aBuf[i]*aBuf[i]);
    }
    double lMeanSq=lSumSq/static_cast<double>(aLen);
    return std::sqrt(lMeanSq-(aMean*aMean));
  }
  
  double Mean(i16 * aBuf, u32 aLen)
  {
    double lTemp=0.0;
    for(unsigned long i=0; i<aLen; i++){
      lTemp+=aBuf[i];
    }
    return lTemp/static_cast<double>(aLen);
  }

  double StDev(i16 * aBuf, double aMean, u32 aLen)
  {
    double lSumSq=0.0;
    for(unsigned long i=0; i<aLen;i++){
      lSumSq+=(aBuf[i]*aBuf[i]);
    }
    double lMeanSq=lSumSq/static_cast<double>(aLen);
    return std::sqrt(lMeanSq-(aMean*aMean));
  }


  void MSBtoLSB(u32 &l_in ) {
	unsigned long temp=0x00000000;
	unsigned long testvector=0x80000000;
	unsigned long testvector2=0x00000001;

	for (int l=0; l<32; l++){
		if((l_in&testvector)!=0)temp=temp|testvector2;
		testvector/=2;
		testvector2*=2;
	}
    l_in=temp;
  }


void ToBin(unsigned long l_in, std::string& s_out , std::string& highChar, std::string& lowChar) {
	s_out="";
	unsigned long testvector=0x80000000;
	for (int l=0; l<32; l++){
		s_out+=( (testvector&l_in)==0?lowChar:highChar );
		testvector/=2;
	}
}




  void Sleep(u32 ams)
  {
//#ifdef Linux
    struct timespec lReq;
    struct timespec lRem;

    lReq.tv_sec=ams/1000;
    lReq.tv_nsec=(ams%1000)*1000000;
    nanosleep(&lReq, &lRem);
//#endif
//#ifdef Win32
    // need to implement this
//#endif
  }

}//~namespace ICCoreUtils

