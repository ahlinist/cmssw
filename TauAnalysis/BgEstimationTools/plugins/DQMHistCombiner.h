#ifndef TauAnalysis_BgEstimationTools_DQMHistCombiner_h
#define TauAnalysis_BgEstimationTools_DQMHistCombiner_h

/** \class DQMHistCombiner
 *  
 *  Class to combine estimates
 *  for signal and background contributions
 *  determined independently by template and fake-rate method
 *  into single "best" estimate
 *
 *  $Date: 2010/05/18 09:48:17 $
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

class DQMHistCombiner : public edm::EDAnalyzer
{
  typedef std::vector<std::string> vstring;

  struct cfgEntryPlot
  {
    explicit cfgEntryPlot(const edm::ParameterSet&);
    vstring meNamesInput_;
    std::string meNameOutputShape_;
    std::string meNameOutputNorm_;
    std::string meNameOutputNormErr_;
  };

 public:
  explicit DQMHistCombiner(const edm::ParameterSet&);
  virtual ~DQMHistCombiner();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  std::vector<cfgEntryPlot> cfgEntriesPlot_;
};

#endif


