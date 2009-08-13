#ifndef TauAnalysis_DQMTools_DQMHistSubtractor_h
#define TauAnalysis_DQMTools_DQMHistSubtractor_h

/** \class DQMHistSubtractor
 *  
 *  Class to produce histogram computed as
 *  difference of two other histograms
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

class DQMHistSubtractor : public edm::EDAnalyzer
{
  struct jobEntryType
  { 
    std::string meNameMinuend_;
    std::string meNameSubtrahend_;
    std::string meNameDifference_;
  };

 public:
  explicit DQMHistSubtractor(const edm::ParameterSet&);
  virtual ~DQMHistSubtractor();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  std::vector<jobEntryType> jobs_; 
};

#endif


