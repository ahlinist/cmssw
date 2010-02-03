#ifndef TauAnalysis_DQMTools_DQMHistEffProducer_h
#define TauAnalysis_DQMTools_DQMHistEffProducer_h

/** \class DQMHistEffProducer
 *  
 *  Class to produce efficiency histograms by dividing nominator by denominator histograms
 *
 *  $Date: 2009/11/20 15:26:14 $
 *  $Revision: 1.2 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <vector>

class DQMHistEffProducer : public edm::EDAnalyzer
{
  struct cfgEntryPlot
  {
    explicit cfgEntryPlot(const edm::ParameterSet&);
    explicit cfgEntryPlot(const std::string&, const std::string&, const std::string&);
    void print() const;
    std::vector<std::string> meNames_numerator_;
    std::vector<std::string> meNames_denominator_;
    std::string meName_efficiency_;
    int cfgError_;
  };

 public:
  explicit DQMHistEffProducer(const edm::ParameterSet&);
  virtual ~DQMHistEffProducer();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  std::vector<cfgEntryPlot> cfgEntryPlot_;
};

#endif


