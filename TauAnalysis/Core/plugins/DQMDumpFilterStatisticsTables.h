#ifndef TauAnalysis_Core_DQMDumpFilterStatisticsTables_h
#define TauAnalysis_Core_DQMDumpFilterStatisticsTables_h

/** \class DQMDumpFilterStatisticsTables
 *  
 *  Class to print-out cut-flow information contained in FilterStatisticsTables
 *
 *  $Date: 2009/04/16 13:27:31 $
 *  $Revision: 1.2 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/Core/interface/FilterStatisticsService.h"

#include <string>
#include <vector>
#include <map>

class DQMDumpFilterStatisticsTables : public edm::EDAnalyzer
{
 public:
  explicit DQMDumpFilterStatisticsTables(const edm::ParameterSet&);
  virtual ~DQMDumpFilterStatisticsTables();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  typedef std::vector<std::string> vstring;
  vstring processes_;

  std::map<std::string, std::string> dqmDirectories_;

  vstring columnsSummaryTable_;

  FilterStatisticsService* filterStatisticsService_;
  std::map<std::string, FilterStatisticsTable*> filterStatisticsTables_;

  int cfgError_;
};

#endif


