/**
This file contains Class definitions for the 
Class representing Output (Streamer/Index) file.
*/

#ifndef _StreamerFileIO_h
#define _StreamerFileIO_h

#include "IOPool/Streamer/interface/MsgTools.h"

#include <fstream>
//#include <iostream>
#include<string>

//-------------------------------------------------------
  
  class OutputFile 
  /**
  Class representing Output (Streamer/Index) file.
  */
  {
  public:
     explicit OutputFile(const string& name);
     /**
      CTOR, takes file path name as argument
     */
     ~OutputFile();

      ofstream* ost() {return ost_;}
      string fileName() const {return filename_;}

      uint64 current_offset_;  /** Location of current ioptr */
      uint64 first_event_offset_;
      uint64 last_event_offset_;
      uint32 events_;
      uint32 run_;

   private:
     ofstream* ost_;
     string filename_; 
  };

#endif

