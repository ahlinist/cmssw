#ifndef ASCIIReaderEngine_HH_
#define ASCIIReaderEngine_HH_

#include "ICTypeDefs.hh"
#include "ICUtilityToolbox.hh"
#include "ICException.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace GCT_SC
{

  using namespace ICCoreUtils;

  class ASCIIReaderEngine
  {
  public:
    
    
  protected:
    ASCIIReaderEngine(const std::string & aFileName);
    virtual ~ASCIIReaderEngine();
    
    void SetBaseHex();
    void SetBaseDec();    
    
    bool GetValue(const std::string & aIdentifier, std::string & aVal);
    bool GetValue(const std::string & aIdentifier, bool & aVal);
    bool GetValue(const std::string & aIdentifier, u8 & aVal);
    bool GetValue(const std::string & aIdentifier, i8 & aVal);
    bool GetValue(const std::string & aIdentifier, u16 & aVal);
    bool GetValue(const std::string & aIdentifier, i16 & aVal);
    bool GetValue(const std::string & aIdentifier, u32 & aVal);
    bool GetValue(const std::string & aIdentifier, i32 & aVal);
    
  private:
    
    // templated engine kept private for reasons of 
    // templating: what does is mean to read in an aggregate? 
    template <class T> 
    void ExtractValue(const std::string & aLine, 
		      T & aVal)
    {
      size_t lStartPos=aLine.rfind(':');

      if(lStartPos==aLine.npos){
	// raise:
	

      }
      
      if(mIsHex){
	std::istringstream(aLine.substr(lStartPos+1, aLine.size()))>>std::hex>>aVal; 
      }else{
	std::istringstream(aLine.substr(lStartPos+1, aLine.size()))>>std::dec>>aVal; 
      }
      
      //std::cout<<"ExtractValue : "<<aLine<<"   \n"<<aVal<<std::endl;
    }
    
    
    void ExtractValue(const std::string & aLine, 
		      bool & aVal)
    {
      // need to find both of the cases where the 
      // numeric or textual representations are used:
      size_t lStartPos=aLine.rfind(':');
      std::string lTemp(aLine.substr(lStartPos+1,aLine.size())); // make a copy
      // lower the case: 
      ToLower(lTemp);
      
      //std::cout<<__PRETTY_FUNCTION__<<"   "<<lTemp<<std::endl;

      size_t lPos=lTemp.find("true");
      if(lPos!=lTemp.npos){
	// the value is true:
	aVal=true;
	return;	
      }
      lPos=lTemp.find('1');
      if(lPos!=lTemp.npos){
	// the value is true:
	aVal=true;
	return;	
      }
      
      // search for the false options;

      lPos=lTemp.find("false");
      if(lPos!=lTemp.npos){
	// the value isfalse
	aVal=false;
	return;
      }  
      lPos=lTemp.find('0');
      if(lPos!=lTemp.npos){
	// the value isfalse
	aVal=false;
      } 
       
    }
    



    // specialisations required for the templating:
    // template <> 
    void ExtractValue(const std::string & aLine, 
		      std::string & aVal)
    {
      size_t lStartPos=aLine.find('\"');
      if(lStartPos==aLine.npos){
	return;
	// raise:
      }
      size_t lEndPos=aLine.rfind('\"'); 
      //std::cout<<__PRETTY_FUNCTION__<<"  "<<lStartPos<<"   "<<lEndPos<<std::endl;
      aVal=aLine.substr(lStartPos+1, lEndPos-lStartPos-1);
      //std::cout<<aVal<<std::endl;
    }
    


    void ExtractValue(const std::string & aLine, 
		      u8 & aVal)
    {
      try{
	u16 lTemp=0;
	ExtractValue(aLine, lTemp);
	aVal=static_cast<u8>(lTemp);
	
      }catch(ICException & aExc){
	RETHROW(aExc);
      }
    }
    void ExtractValue(const std::string & aLine, 
		      i8 & aVal)
    {
      try{
	i16 lTemp=0;
	ExtractValue(aLine, lTemp);
	aVal=static_cast<i8>(lTemp);
	
      }catch(ICException & aExc){
	RETHROW(aExc);
      }
    }
    
    //
    bool FindEntry(//std::ifstream & aIpStr, 
		   const std::string & aKey, 
		   std::string & aLine);
	

  private:
    
    ASCIIReaderEngine();
    ASCIIReaderEngine(const ASCIIReaderEngine &);
    ASCIIReaderEngine & operator=(const ASCIIReaderEngine &);


  private:

    std::string mFileName;
    std::ifstream mInputStream;
    bool mIsHex;
    
  };//~class ASCIIReaderEngine


  

}//~namsepace GCT_SC

#endif // ASCIIReaderEngine_HH_
