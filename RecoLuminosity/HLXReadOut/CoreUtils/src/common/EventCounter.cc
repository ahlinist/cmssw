#include "ICException.hh"
#include "ConfigFileErrorException.hh"
#include "FileNotOpenedException.hh"

#include "EventCounter.hh"
#include "ICUtilityToolbox.hh"

#include <iostream>
#include <sstream>

namespace ICCoreUtils
{
  
  EventCounter::EventCounter(bool aAutoFileSync, const std::string & aFname):
    mAutoFileSync(aAutoFileSync),
    mFileName(aFname),
    mEventNumber(0)
  {
    if(mFileName.length()==0){
      mFileName=ICCoreUtils::GetEnv("IC_EVENT_COUNTER");
      if(mFileName.length()==0){
	ConfigFileErrorException lExc(mFileName, 
				 "Event Number Counter File",
				 "No filename passed to contructor or set in the right env.",0);
	RAISE(lExc);
      }
    }
    
    std::ifstream lInput(mFileName.c_str());
    if(!lInput){
      FileNotOpenedException lExc("Unable to open file: ");
      lExc<<mFileName;
      RAISE(lExc);
    }

    lInput>>std::dec>>mEventNumber;
    if(!lInput){
      	ConfigFileErrorException lExc(mFileName, 
				 "Event Number Counter File",
				 "Unable to read event number from file (is it empty?)",1);
	RAISE(lExc);      
    }
    
    // if programme flow reaches here, everything's ok. 
  }
  
  void EventCounter::SyncWithFile()
  {
    if(mFileName.length()==0){
      FileNotOpenedException lExc("Unable to open file: \"");
      lExc<<mFileName;
      lExc<<"\" because the filename has not been set. surprised?";
      RAISE(lExc);
    }
    
    std::ofstream lOutput;
    lOutput.open(mFileName.c_str());
    if(!lOutput){
      FileNotOpenedException lExc("Unable to open file: \"");
      lExc<<mFileName;
      lExc<<"\" for some reason. (path doesn\'t exist? disk full?). ";
      lExc<<" Last Event number = "<<mEventNumber<<" (write that down)";
      RAISE(lExc);
    }
    
    lOutput<<std::dec<<mEventNumber;
    lOutput.close();
  }
  
  EventCounter::~EventCounter()
  {
    //std::cout<<__PRETTY_FUNCTION__<<" Beginning"<<std::endl;
    try{
      this->SyncWithFile();
    }catch(ICException & aExc){
      std::cerr<<"!!!!HOLY CRAP!!!! WE REALLY ARE IN DEEP SHIT NOW: "<<std::endl;
      std::cerr<<aExc.What()<<std::endl;
      std::cerr<<aExc.History()<<std::endl; 
    }
    //std::cout<<__PRETTY_FUNCTION__<<" end."<<std::endl;
  }
  
  u32 EventCounter::GetEventNumber()
  {
    mEventNumber++;
    if(mAutoFileSync)
      this->SyncWithFile();
    return mEventNumber;
  }
  
  std::string EventCounter::GetEventNumber(u16 aNumDigits, EventCounter::StrFormat aFormat)
  {
    std::ostringstream lOs;
    lOs.width(aNumDigits);
    lOs.fill('0');
    
    if(aFormat==DEC){
      lOs<<std::right<<std::dec<<this->ViewEventNumber();
    }
    if(aFormat==HEX){
      lOs<<std::left<<"0x";
      lOs<<std::hex<<this->ViewEventNumber();
      
    }
    return lOs.str();
  }


  


}//~namespace ICCoreUtils
