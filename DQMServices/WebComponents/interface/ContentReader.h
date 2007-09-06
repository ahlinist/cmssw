#ifndef _ContentReader_h_
#define _ContentReader_h_

#include "xdaq/Application.h"
#include "DQMServices/CoreROOT/interface/DaqMonitorROOTBackEnd.h"


class ContentReader
{
private:

  DaqMonitorBEInterface *bei;

public:	

  ContentReader(DaqMonitorBEInterface *the_bei)
    {
      bei = the_bei;
    }

  ~ContentReader(){}

  void give_subdirs(std::string dir, std::list<std::string> &subdirs, std::string mode);
  void give_subdirs_su(std::string dir, std::list<std::string> &subdirs);

  void give_files(std::string dir, std::list<std::string> &files, bool only_contents);

  MonitorElement *give_ME(std::string filename);

protected:

};


#endif
