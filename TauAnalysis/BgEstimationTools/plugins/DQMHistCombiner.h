#ifndef TauAnalysis_BgEstimationTools_DQMHistCombiner_h
#define TauAnalysis_BgEstimationTools_DQMHistCombiner_h

/** \class DQMHistCombiner
 *  
 *  Class to combine estimates
 *  for signal and background contributions
 *  determined independently by template and fake-rate method
 *  into single "best" estimate
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

class DQMHistCombiner : public edm::EDAnalyzer
{
  struct cfgEntryHist
  {
    explicit cfgEntryHist(const edm::ParameterSet&);
    std::string meNameInputShape_;
    std::string meNameInputNorm_;
    std::string meNameInputNormErr_;
  };

  struct cfgEntryCombJob
  {
    explicit cfgEntryCombJob(const edm::ParameterSet&);
    std::vector<cfgEntryHist> cfgEntriesHist_;
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
  std::vector<cfgEntryCombJob> cfgEntriesCombJob_;
};

#endif


