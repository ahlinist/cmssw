#ifndef TauAnalysis_BgEstimationTools_DQMBinErrorCalculator_h
#define TauAnalysis_BgEstimationTools_DQMBinErrorCalculator_h

/** \class DQMBinErrorCalculator
 *  
 *  Class to compute efficiency and binomal errors
 *  given number of events tested (denominator) and passed (numerator)
 *  in form of two MonitorElements of type float
 *
 *  $Date: 2009/11/27 18:36:05 $
 *  $Revision: 1.1 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <vector>

class DQMBinErrorCalculator : public edm::EDAnalyzer
{
  struct cfgEntryBinError
  {
    explicit cfgEntryBinError(const edm::ParameterSet&);
    std::string meName_numerator_;
    std::string meName_denominator_;

    std::string meName_passed_;
    std::string meName_failed_;

    std::string label_;

    mutable bool error_;
  };

 public:
  explicit DQMBinErrorCalculator(const edm::ParameterSet&);
  virtual ~DQMBinErrorCalculator();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  std::vector<cfgEntryBinError> cfgEntriesBinError_;
};

#endif


