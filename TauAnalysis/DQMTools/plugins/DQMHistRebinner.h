#ifndef TauAnalysis_DQMTools_DQMHistEffProducer_h
#define TauAnalysis_DQMTools_DQMHistEffProducer_h

/** \class DQMHistEffProducer
 *  
 *  Class to change binning of histograms;
 *  the new binning may have any number of fewer bins,
 *  the only requirement is that all bin-boundaries of the new histogram 
 *  match a bin-boundary (any one) of the old histogram
 *
 *  $Date: 2009/01/21 17:34:57 $
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

class DQMHistRebinner : public edm::EDAnalyzer
{
  struct cfgEntryPlot
  {
    explicit cfgEntryPlot(const edm::ParameterSet&);
    std::string meName_original_;
    std::string meName_rebinned_;
  };

 public:
  explicit DQMHistRebinner(const edm::ParameterSet&);
  virtual ~DQMHistRebinner();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  std::vector<cfgEntryPlot> cfgEntryPlot_;

  unsigned combineBinsX_;
  unsigned numBinsX_;
  double xMin_;
  double xMax_;
  std::vector<double> xBins_;
};

#endif


