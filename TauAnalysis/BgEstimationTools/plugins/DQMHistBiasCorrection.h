#ifndef TauAnalysis_DQMTools_DQMHistBiasCorrection_h
#define TauAnalysis_DQMTools_DQMHistBiasCorrection_h

/** \class DQMHistBiasCorrection
 *  
 *  Class to correct for bias introduced by differences in event selection criteria
 *  between background enriched control regions and phase-space of "final" analysis
 *
 *  $Date: 2010/04/10 13:58:18 $
 *  $Revision: 1.4 $
 *  \author Christian Veelken, UC Davis
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <vector>

class DQMHistBiasCorrection : public edm::EDAnalyzer
{
  struct cfgEntryPlot
  {
    explicit cfgEntryPlot(const edm::ParameterSet&);
    explicit cfgEntryPlot(const std::string&, const std::string&, const std::string&, const std::string&);
    void print() const;
    std::string meName_uncorrected_;
    std::string meName_corrected_;
    std::string meName_corrNumerator_;
    std::string meName_corrDenominator_;
  };

 public:
  explicit DQMHistBiasCorrection(const edm::ParameterSet&);
  virtual ~DQMHistBiasCorrection();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  
  
 private:
  std::vector<cfgEntryPlot> cfgEntryPlots_;
};

#endif


