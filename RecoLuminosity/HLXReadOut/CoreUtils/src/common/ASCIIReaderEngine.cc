#include "ASCIIReaderEngine.hh"
#include "ConfigFileErrorException.hh"
#include "FileNotOpenedException.hh"

namespace GCT_SC
{
  using namespace std;

  ASCIIReaderEngine::ASCIIReaderEngine(const std::string & aFileName):
    mFileName(aFileName), mIsHex(false)
  {
    mInputStream.open(mFileName.c_str());
    if(!mInputStream.is_open()){
      FileNotOpenedException lExc(mFileName);
      RAISE(lExc);      
    }
  }


  ASCIIReaderEngine::~ASCIIReaderEngine()
  {
    mInputStream.close();
  }

  
  void ASCIIReaderEngine::SetBaseHex(){
    mIsHex=true;
  }
  void ASCIIReaderEngine::SetBaseDec(){
    mIsHex=false;
  }
  

  bool ASCIIReaderEngine::GetValue(const std::string & aIdentifier, std::string & aVal)
  {
    std::string lBuf;
    if(this->FindEntry(aIdentifier, lBuf)){
      // note that this call uses the 
      // template specialisation for <std::string &>
      this->ExtractValue(lBuf, aVal);
      return true;
    }else{
      return false;
      // raise:
      /*ConfigFileErrorException lExc(mFileName,"",
				    std::string("Identifier ")+
				    aIdentifier+
				    std::string(" not found"),
				    -1);
				    RAISE(lExc);*/
    }
  }

  bool ASCIIReaderEngine::GetValue(const std::string & aIdentifier, bool & aVal){
    std::string lBuf;
    if(this->FindEntry(aIdentifier, lBuf)){
      // note that this call uses the 
      // template specialisation for <std::string &>
      this->ExtractValue(lBuf, aVal);
      return true;
    }else{
      return false;
    }    
  }

  bool ASCIIReaderEngine::GetValue(const std::string & aIdentifier, u8 & aVal)
  {
    std::string lBuf;
    if(this->FindEntry(aIdentifier, lBuf)){
      this->ExtractValue(lBuf, aVal);
      return true;
    }else{
      return false;
    }
  }


  bool ASCIIReaderEngine::GetValue(const std::string & aIdentifier, i8 & aVal)
  {
    std::string lBuf;
    if(this->FindEntry(aIdentifier, lBuf)){
      this->ExtractValue(lBuf, aVal);
      return true;
    }else{
      return false;
    }

  }


  bool ASCIIReaderEngine::GetValue(const std::string & aIdentifier, u16 & aVal)
  {
    std::string lBuf;
    if(this->FindEntry(aIdentifier, lBuf)){
      this->ExtractValue(lBuf, aVal);
      return true;
    }else{
      return false;
    }

  }

  bool ASCIIReaderEngine::GetValue(const std::string & aIdentifier, i16 & aVal)
  {
    std::string lBuf;
    if(this->FindEntry(aIdentifier, lBuf)){
      this->ExtractValue(lBuf, aVal);
      return true;
    }else{
      return false;
    }

  }

  bool ASCIIReaderEngine::GetValue(const std::string & aIdentifier, u32 & aVal)
  {
    std::string lBuf;
    if(this->FindEntry(aIdentifier, lBuf)){
      this->ExtractValue(lBuf, aVal);
      return true;
    }else{
      return false;
    }

  }


  bool ASCIIReaderEngine::GetValue(const std::string & aIdentifier, i32 & aVal)
  {
    std::string lBuf;
    if(this->FindEntry(aIdentifier, lBuf)){
      this->ExtractValue(lBuf, aVal);
      return true;
    }else{
      return false;
    }
  
  }


  bool ASCIIReaderEngine::FindEntry(//std::ifstream & aIpStr, 
				    const std::string & aKey, 
				    std::string & aLine)
  {
//cout<<"ASCIIReaderEngine::Key = "<<aKey<<endl;

    mInputStream.seekg(0);  
    do {
      std::getline(mInputStream, aLine, '\n');
//cout<<">>>> ASCIIReaderEngine::Line = "<<aLine<<endl;
      if(mInputStream.eof()){
	break;
      }
      if(aLine.find(aKey)!=aLine.npos){
	return true;
      }
    }while(true); 
  
    return false;
  }


}
