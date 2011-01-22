#ifndef TauAnalysis_Core_DQMDumpSysUncertaintyBinningResults_h
#define TauAnalysis_Core_DQMDumpSysUncertaintyBinningResults_h

/** \class DQMDumpSysUncertaintyBinningResults
 *  
 *  Class to print-out binning results information for estimating systematic uncertainties
 *  contained in DQM (sub)directory structure created by SysUncertaintyBinner class
 *
 *  $Date: 2011/01/20 14:38:09 $
 *  $Revision: 1.2 $
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
  struct binResultsMapppingType : std::vector<string_binResult_pair>
  {
    void insert(const std::string& name, binResultType& value)
    {
      this->push_back(string_binResult_pair(name, value));
    }

    binResultsMapppingType::const_iterator find(const std::string& name) const
    {
      for ( binResultsMapppingType::const_iterator it = this->begin();
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
  struct sysUncertaintyBaseType
  {
    sysUncertaintyBaseType(const edm::ParameterSet&);
    virtual ~sysUncertaintyBaseType();

    void loadBinningResults(const std::string&);
   
    DQMDumpSysUncertaintyBinningResults_namespace::binResultsMapppingType getBinResults_systematic(
      const std::vector<std::string>&, unsigned, const std::string&) const;
    DQMDumpSysUncertaintyBinningResults_namespace::binResultsMapppingType getBinResultsSum_systematic(
      const std::vector<std::string>&, const std::string&) const;

    binResultType getBinResults_sysCentralValue(const std::string&, unsigned, const std::string&) const;
    binResultType getBinResultsSum_sysCentralValue(const std::string&, const std::string&) const;

    virtual void printBinResult(int, const std::string&) const = 0;

    std::string sysTitle_;

    std::string binningServiceType_;

    std::vector<std::string> binningPluginNames_;

    std::map<std::string, BinningBase*> binningPlugins_; // key = sysName

    int cfgError_;
  };

  struct simpleUncertaintyType : public sysUncertaintyBaseType
  {
    simpleUncertaintyType(const edm::ParameterSet&);
    ~simpleUncertaintyType();

    void printBinResult(int, const std::string&) const;

    std::string sysCentralValue_;
    std::vector<std::string> sysNames_;
  };

  struct pdfUncertaintyType : public sysUncertaintyBaseType
  {
    pdfUncertaintyType(const edm::ParameterSet&);
    ~pdfUncertaintyType();

    void printBinResult(int, const std::string&) const;

    struct pdfSetEntryType
    {
      pdfSetEntryType(const std::vector<std::string>& sysCentralValues, const std::vector<std::string>& sysNames)
	: sysCentralValues_(sysCentralValues),
	  sysNames_(sysNames)
      {}
      ~pdfSetEntryType() {}

      std::vector<std::string> sysCentralValues_;
      std::vector<std::string> sysNames_;
    };

    std::vector<pdfSetEntryType> pdfSetEntries_;
  };

  struct processEntryType
  {
    processEntryType(const std::string&, const std::string&);
    ~processEntryType();

    std::string processName_;

    std::string dqmDirectory_;

    std::vector<sysUncertaintyBaseType*> sysUncertaintyEntries_;
  };

  std::vector<processEntryType> processEntries_;

  std::vector<std::string> resultTypeNames_;

  int cfgError_;
};

#endif


