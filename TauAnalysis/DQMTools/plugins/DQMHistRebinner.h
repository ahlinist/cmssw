#ifndef TauAnalysis_DQMTools_DQMHistEffProducer_h
#define TauAnalysis_DQMTools_DQMHistEffProducer_h

/** \class DQMHistEffProducer
 *  
 *  Class to change binning of histograms;
 *  the new binning may have any number of fewer bins,
 *  the only requirement is that all bin-boundaries of the new histogram 
 *  match a bin-boundary (any one) of the old histogram
 *
 *  $Date: 2009/11/28 14:28:32 $
 *  $Revision: 1.3 $
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
  struct plotEntryType
  {
    explicit plotEntryType(const edm::ParameterSet&);
    void print() const;
    std::string meName_original_;
    std::string meName_rebinned_;
  };

  struct axisEntryType
  {
    explicit axisEntryType(const edm::ParameterSet&);
    void print() const;
    unsigned combineBins_;
    unsigned numBins_;
    std::vector<double> binEdges_;
  };

 public:
  explicit DQMHistRebinner(const edm::ParameterSet&);
  virtual ~DQMHistRebinner();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  std::vector<plotEntryType> plotEntries_;
  std::vector<axisEntryType> axisEntries_;
};

#endif


