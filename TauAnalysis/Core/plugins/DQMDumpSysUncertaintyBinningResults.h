#ifndef TauAnalysis_Core_DQMDumpSysUncertaintyBinningResults_h
#define TauAnalysis_Core_DQMDumpSysUncertaintyBinningResults_h

/** \class DQMDumpSysUncertaintyBinningResults
 *  
 *  Class to print-out binning results information for estimating systematic uncertainties
 *  contained in DQM (sub)directory structure created by SysUncertaintyBinner class
 *
 *  $Date: 2009/12/04 13:42:01 $
 *  $Revision: 1.1 $
 *  \author Christian Veelken, UC Davis
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/Core/interface/BinningBase.h"
#include "TauAnalysis/Core/interface/BinningServiceBase.h"

#include <string>
#include <vector>
#include <map>

class DQMDumpSysUncertaintyBinningResults : public edm::EDAnalyzer
{
 public:
  explicit DQMDumpSysUncertaintyBinningResults(const edm::ParameterSet&);
  virtual ~DQMDumpSysUncertaintyBinningResults();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  struct sysUncertaintyEntryType 
  {
    sysUncertaintyEntryType(const edm::ParameterSet&);
    ~sysUncertaintyEntryType();

    void loadBinningResults(const std::string&);

    std::map<std::string, binResultType> getBinResults_systematic(unsigned, const std::string&) const;
    std::map<std::string, binResultType> getBinResultsSum_systematic(const std::string&) const;

    binResultType getBinResults_sysCentralValue(unsigned, const std::string&) const;
    binResultType getBinResultsSum_sysCentralValue(const std::string&) const;

    std::string sysCentralValue_;
    std::vector<std::string> sysNames_;
    std::string sysTitle_;

    int method_;

    std::string binningServiceType_;

    std::map<std::string, BinningBase*> binningPlugins_; // key = sysName

    int cfgError_;
  };

  struct processEntryType
  {
    processEntryType(const std::string&, const std::string&);
    ~processEntryType();

    std::string processName_;

    std::string dqmDirectory_;

    std::vector<sysUncertaintyEntryType> sysUncertaintyEntries_;
  };

  std::vector<processEntryType> processEntries_;

  std::vector<std::string> resultTypeNames_;

  int cfgError_;
};

#endif


