#ifndef TauAnalysis_DQMTools_DQMHistSubtractor_h
#define TauAnalysis_DQMTools_DQMHistSubtractor_h

/** \class DQMHistSubtractor
 *  
 *  Class to produce histogram computed as
 *  difference of two other histograms
 *
 *  $Date: 2009/08/13 15:02:09 $
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
    jobEntryType(const edm::ParameterSet&);

    std::string meName_minuend_;
    std::string meName_minuendErr_;

    std::string meName_subtrahend_;
    std::string meName_subtrahendErr_;

    std::string meName_difference_;
    std::string meName_differenceErr_;
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


