#ifndef TriggerReportHelpers_H
#define TriggerReportHelpers_H

#include "xdata/String.h"
#include "xdata/UnsignedInteger32.h"

#include "FWCore/Framework/interface/TriggerReport.h"

#include "MsgBuf.h"

#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>

namespace edm{
  class ModuleDescription;
}

namespace dqmevf{
  
  static const size_t max_paths = 5;
  static const size_t max_endpaths = 5;
  static const size_t max_label = 80;
  static const size_t max_modules = 50;

  class ShmOutputModuleRegistry;

  struct ModuleInPathsSummaryStatic{
    //max length of a module label is 80 characters - name is truncated otherwise
    int timesVisited;
    int timesPassed;
    int timesFailed;
    int timesExcept;
    char moduleLabel[max_label];
  };

  struct PathSummaryStatic
  {
    //    int bitPosition;
    int timesRun;
    int timesPassed;
    int timesFailed;
    int timesExcept;
    char name[max_label];
  };

  struct TriggerReportStatic{
    //max number of paths in a menu is 500
    //max number of endpaths in a menu is 20
    unsigned int           lumiSection;
    edm::EventSummary      eventSummary;
    int                    trigPathsInMenu;
    int                    endPathsInMenu;
    PathSummaryStatic      trigPathSummaries[max_paths];
    PathSummaryStatic      endPathSummaries[max_endpaths];
  };


  class TriggerReportHelpers{
    public:
      TriggerReportHelpers() 
	: eventsProcessed_(0)
	, eventsAccepted_(0)
	, tableFormatted_(false)
	, lumiSectionIndex_(0)
	, cache_(sizeof(TriggerReportStatic),MSQS_MESSAGE_TYPE_TRR)
	{
	  memset(cache_->mtext,0,sizeof(TriggerReportStatic));
	}
      void resetFormat(){tableFormatted_ = false;}
      void printReportTable();
      void printTriggerReport(edm::TriggerReport &);

      //master
      unsigned int getLumi(){return lumiSectionIndex_;}
      TriggerReportStatic *getPackedTriggerReportAsStruct(){return (TriggerReportStatic *)cache_->mtext;}
      void replace(TriggerReportStatic * newTrs); 
      void add(TriggerReportStatic * newTrs);
	
      //slave
      void formatReportTable(edm::TriggerReport &);
      void packTriggerReport(unsigned int lumi, edm::TriggerReport &, ShmOutputModuleRegistry *);
      //both
      dqmevf::MsgBuf & getPackedTriggerReport(){return cache_;}
      
      //unused
    private:
      void resetTriggerReport();
      xdata::UnsignedInteger32 eventsProcessed_;
      xdata::UnsignedInteger32 eventsAccepted_;
      
      bool         tableFormatted_;
      bool         copyNames;
      
      std::vector<std::string> outname_;
      static const std::string columns[3];
      std::vector<std::string> paths_;

      std::vector<xdata::UnsignedInteger32> accept_;
      std::vector<xdata::UnsignedInteger32> except_;
      std::vector<xdata::UnsignedInteger32> failed_;

      std::vector<unsigned int> paccept_;
      std::vector<unsigned int> pexcept_;
      std::vector<unsigned int> pfailed_;
      std::map<std::string, xdata::UnsignedInteger32> pathIndexMap_;
      
      unsigned int lumiSectionIndex_;

      edm::TriggerReport trp_;
      MsgBuf  cache_;

  };
}



#endif
