#ifndef TauAnalysis_Core_DQMDumpSysUncertaintyBinningResults_h
#define TauAnalysis_Core_DQMDumpSysUncertaintyBinningResults_h

/** \class DQMDumpSysUncertaintyBinningResults
 *  
 *  Class to print-out binning results information for estimating systematic uncertainties
 *  contained in DQM (sub)directory structure created by SysUncertaintyBinner class
 *
 *  $Date: 2010/02/16 15:12:43 $
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

namespace DQMDumpSysUncertaintyBinningResults_namespace
{
  typedef std::pair<std::string, binResultType> string_binResult_pair;
  struct binResultsMappping_type : std::vector<string_binResult_pair>
  {
    void insert(const std::string& name, binResultType& value)
    {
      this->push_back(string_binResult_pair(name, value));
    }

    binResultsMappping_type::const_iterator find(const std::string& name) const
    {
      for ( binResultsMappping_type::const_iterator it = this->begin();
            it != this->end(); ++it ) {
        if ( it->first == name ) return it;
      }
      return this->end(); // no entry with given name found
    } 
  }; 
}

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
   
    DQMDumpSysUncertaintyBinningResults_namespace::binResultsMappping_type getBinResults_systematic(unsigned, const std::string&) const;
    DQMDumpSysUncertaintyBinningResults_namespace::binResultsMappping_type getBinResultsSum_systematic(const std::string&) const;

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


