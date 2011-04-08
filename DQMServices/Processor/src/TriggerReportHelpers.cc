#include "Exception.h"
//#include "EventFilter/Utilities/interface/ShmOutputModuleRegistry.h"
//#include "EventFilter/Modules/src/FUShmOutputModule.h"
//#include "DataFormats/Provenance/interface/ModuleDescription.h"
#include "TriggerReportHelpers.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>

#include "boost/tokenizer.hpp"

namespace dqmevf{
const std::string TriggerReportHelpers::columns[3] = {"pAccept","pExcept","pReject"};

void TriggerReportHelpers::formatReportTable(edm::TriggerReport &tr)
{

  if(tableFormatted_) return;
  else tableFormatted_ = true;

  
  trp_ = tr;
  lumiSectionIndex_ = 0;
  resetTriggerReport();
  copyNames=true;
  TriggerReportStatic *trp = (TriggerReportStatic *)cache_->mtext;

  
  paths_.resize(tr.trigPathSummaries.size()+tr.endPathSummaries.size());
  //adjust number of trigger- and end-paths in static structure
  trp->trigPathsInMenu = tr.trigPathSummaries.size();
  trp->endPathsInMenu = tr.endPathSummaries.size();
  ///


  outname_.resize(tr.trigPathSummaries.size()+tr.endPathSummaries.size());

  accept_.resize(tr.trigPathSummaries.size()+tr.endPathSummaries.size(),0);
  except_.resize(tr.trigPathSummaries.size()+tr.endPathSummaries.size(),0);
  failed_.resize(tr.trigPathSummaries.size()+tr.endPathSummaries.size(),0);

  paccept_.resize(tr.trigPathSummaries.size()+tr.endPathSummaries.size(),0);
  pexcept_.resize(tr.trigPathSummaries.size()+tr.endPathSummaries.size(),0);
  pfailed_.resize(tr.trigPathSummaries.size()+tr.endPathSummaries.size(),0);

  unsigned int i=0;
  for(; i<tr.trigPathSummaries.size(); i++) {
    paths_[i] = tr.trigPathSummaries[i].name;


  }
  for(; i<tr.endPathSummaries.size()+tr.trigPathSummaries.size(); i++) {
    paths_[i] = tr.endPathSummaries[i-tr.trigPathSummaries.size()].name;

    outname_[i] = "";

    for(unsigned int j=0;
	j<tr.endPathSummaries[i-tr.trigPathSummaries.size()].moduleInPathSummaries.size();
	j++) {
      std::string label = tr.endPathSummaries[i-tr.trigPathSummaries.size()].moduleInPathSummaries[j].moduleLabel;

    }
  }


}


void TriggerReportHelpers::packTriggerReport(unsigned int lumi, edm::TriggerReport &tr,
					     ShmOutputModuleRegistry *sor)
{
  TriggerReportStatic *trp = (TriggerReportStatic *)cache_->mtext;
  trp->lumiSection = lumi;

  //copy the event summary
  trp->eventSummary.totalEvents = 
    tr.eventSummary.totalEvents - trp_.eventSummary.totalEvents;
  trp->eventSummary.totalEventsPassed = 
    tr.eventSummary.totalEventsPassed - trp_.eventSummary.totalEventsPassed;
  trp->eventSummary.totalEventsFailed = 
    tr.eventSummary.totalEventsFailed - trp_.eventSummary.totalEventsFailed;

  //get total paths in the menu
  trp->trigPathsInMenu = std::min(tr.trigPathSummaries.size(),max_paths);
  trp->endPathsInMenu = std::min(tr.endPathSummaries.size(),max_endpaths);
  //traverse the trigger report to get a copy of relevant parts in the static structure
  // loop on paths

  for(int i = 0; i < trp->trigPathsInMenu; i++)
  {
    //copy path name..:todo do only once
    if (copyNames) {
      std::string & pname = tr.trigPathSummaries[i].name;
      if (pname.size()<max_label) strcpy(trp->trigPathSummaries[i].name,pname.c_str());
      else strcpy(trp->trigPathSummaries[i].name,pname.substr(0,max_label-1).c_str());
    }

    trp->trigPathSummaries[i].timesRun = 
      tr.trigPathSummaries[i].timesRun - trp_.trigPathSummaries[i].timesRun;

    trp->trigPathSummaries[i].timesPassed = 
      tr.trigPathSummaries[i].timesPassed - trp_.trigPathSummaries[i].timesPassed;

    trp->trigPathSummaries[i].timesFailed = 
      tr.trigPathSummaries[i].timesFailed-trp_.trigPathSummaries[i].timesFailed;

    trp->trigPathSummaries[i].timesExcept = 
      tr.trigPathSummaries[i].timesExcept - trp_.trigPathSummaries[i].timesExcept;
  }

  /*
  for(int i = 0; i < trp->endPathsInMenu; i++)
  {
    unsigned int j = i + trp->trigPathsInMenu;
    edm::FUShmOutputModule *o = sor->get(outname_[j]);
    if(!o) {
      //sor->dumpRegistry();
      continue;
    }
    trp->endPathSummaries[i].timesRun    = 
      tr.endPathSummaries[i].timesRun - trp_.endPathSummaries[i].timesRun;

    trp->endPathSummaries[i].timesPassed = 
      o->getCounts() - trp_.endPathSummaries[i].timesPassed;

    trp->endPathSummaries[i].timesFailed = 
      (tr.endPathSummaries[i].timesRun - o->getCounts()) 
      - trp_.endPathSummaries[i].timesFailed;

    trp->endPathSummaries[i].timesExcept = 
      tr.endPathSummaries[i].timesExcept - trp_.endPathSummaries[i].timesExcept;
  }
  */
  trp_ = tr;
  /*
  for(int i = 0; i < trp->endPathsInMenu; i++)
  {
    edm::FUShmOutputModule *o = sor->get(outname_[i+trp->trigPathsInMenu]);
    if(!o) {
      //sor->dumpRegistry();
      continue;
    }
    trp_.endPathSummaries[i].timesPassed = o->getCounts();
    trp_.endPathSummaries[i].timesFailed = tr.endPathSummaries[i].timesRun - o->getCounts();
  }
  */
  copyNames=false;  
}

void TriggerReportHelpers::replace(TriggerReportStatic * newTrs) {
	memcpy((TriggerReportStatic *)cache_->mtext,newTrs,sizeof(TriggerReportStatic));
}

void TriggerReportHelpers::add(TriggerReportStatic * newTrs) {

  TriggerReportStatic *trs = (TriggerReportStatic *)cache_->mtext;

  // add check for LS consistency
  trs->lumiSection = lumiSectionIndex_;

  //add to the event summary
  trs->eventSummary.totalEvents += newTrs->eventSummary.totalEvents;
  trs->eventSummary.totalEventsPassed += newTrs->eventSummary.totalEventsPassed;
  trs->eventSummary.totalEventsFailed += newTrs->eventSummary.totalEventsFailed;

  //get total paths in the menu
  if(trs->trigPathsInMenu != newTrs->trigPathsInMenu) 
    XCEPT_RAISE(dqmevf::Exception,"trig summary inconsistency");
  if(trs->endPathsInMenu != newTrs->endPathsInMenu)
    XCEPT_RAISE(dqmevf::Exception,"trig summary inconsistency");

  //traverse the trigger report and sum relevant parts, check otherwise
  // loop on paths
  for(int i = 0; i < newTrs->trigPathsInMenu; i++)
  {
    // fill individual path summaries
    trs->trigPathSummaries[i].timesRun += newTrs->trigPathSummaries[i].timesRun;
    trs->trigPathSummaries[i].timesPassed += newTrs->trigPathSummaries[i].timesPassed;
    trs->trigPathSummaries[i].timesFailed += newTrs->trigPathSummaries[i].timesFailed; 
    trs->trigPathSummaries[i].timesExcept += newTrs->trigPathSummaries[i].timesExcept;
  }
  for(int i = 0; i < newTrs->endPathsInMenu; i++)
  {
    trs->endPathSummaries[i].timesRun += newTrs->endPathSummaries[i].timesRun;
    trs->endPathSummaries[i].timesPassed += newTrs->endPathSummaries[i].timesPassed;
    trs->endPathSummaries[i].timesFailed += newTrs->endPathSummaries[i].timesFailed;
    trs->endPathSummaries[i].timesExcept += newTrs->endPathSummaries[i].timesExcept;
  }

}  

void TriggerReportHelpers::resetTriggerReport()
{

  //copy the event summary
  trp_.eventSummary.totalEvents = 0;
  trp_.eventSummary.totalEventsPassed = 0;
  trp_.eventSummary.totalEventsFailed = 0;

  for(unsigned int i = 0; i < trp_.trigPathSummaries.size(); i++)
    {
      // reset individual path summaries
      trp_.trigPathSummaries[i].timesRun = 0;
      trp_.trigPathSummaries[i].timesPassed = 0;
      trp_.trigPathSummaries[i].timesFailed = 0;
      trp_.trigPathSummaries[i].timesExcept = 0;

      //loop over modules in path
      for(unsigned int j = 0; j<trp_.trigPathSummaries[i].moduleInPathSummaries.size(); j++)
	{
	  //reset module summaries
	  trp_.trigPathSummaries[i].moduleInPathSummaries[j].timesVisited = 0;
	  trp_.trigPathSummaries[i].moduleInPathSummaries[j].timesPassed = 0;
	  trp_.trigPathSummaries[i].moduleInPathSummaries[j].timesFailed = 0;
	  trp_.trigPathSummaries[i].moduleInPathSummaries[j].timesExcept = 0;
	}
    }
  for(unsigned int i = 0; i < trp_.endPathSummaries.size(); i++)
    {
      trp_.endPathSummaries[i].timesRun    = 0;
      trp_.endPathSummaries[i].timesPassed = 0;
      trp_.endPathSummaries[i].timesFailed = 0;
      trp_.endPathSummaries[i].timesExcept = 0;
      for(unsigned int j = 0; j<trp_.endPathSummaries[i].moduleInPathSummaries.size(); j++)
	{
	  trp_.endPathSummaries[i].moduleInPathSummaries[j].timesVisited = 0;
	  trp_.endPathSummaries[i].moduleInPathSummaries[j].timesPassed = 0;
	  trp_.endPathSummaries[i].moduleInPathSummaries[j].timesFailed = 0;
	  trp_.endPathSummaries[i].moduleInPathSummaries[j].timesExcept = 0;
	}
    }

}

}//end namespace dqmevf
