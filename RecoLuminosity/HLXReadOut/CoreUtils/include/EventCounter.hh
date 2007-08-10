#ifndef EventCounter_HH_
#define EventCounter_HH_


#include <fstream>
#include <string>

#include "ICTypeDefs.hh"


namespace ICCoreUtils
{
  
  class EventCounter
  {
  public:
    
    enum StrFormat{DEC,HEX};

    /*
     * AutoFileSync writes the next number to file after 
     * each increment. this enables good behaviour the 
     * next time the function is run if the process bombs
     * out or is killed. Defaults to true because this is a 
     * good idea. 
     * 
     * aFname denotes the file name where the number can 
     * be read from. If left empty, the env. var.  
     * IC_EVENT_COUNTER is looked for. 
     * if neither is present, an exception is thrown. 
     */
    EventCounter(bool aAutoFileSync=true, const std::string & aFname="");
    
    /*
     * increments and returns the event no.
     */
    u32 GetEventNumber();
    
    /*
     * views the current event number without incrementing it.
     */
    inline u32 ViewEventNumber()const{return mEventNumber;}

    /*
     * calls u32 ViewEventNumber() and formats the number into a 
     * string; useful for filename formatting.
     */
    std::string GetEventNumber(u16 aNumDigits, StrFormat aFormat=DEC);
    
    /*
     * writes the value stores internally to the file used to 
     * store the event number.
     */
    void SyncWithFile();    

    /*
     *  calls SyncWithFile() and then exits.
     */
    ~EventCounter();
    

  private:
    bool mAutoFileSync;
    std::string mFileName;
    u32 mEventNumber;
    
  };//~class EventCounter

}//~namespace ICCoreUtils

#endif // EventCounter_HH_
