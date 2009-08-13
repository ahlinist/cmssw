#ifndef TauAnalysis_DQMTools_DQMHistNormalizer_h
#define TauAnalysis_DQMTools_DQMHistNormalizer_h

/** \class DQMHistNormalizer
 *  
 *  Class to produce copy of histogram 
 *  with normalization equal to specified value
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

class DQMHistNormalizer : public edm::EDAnalyzer
{
  struct jobEntryType
  {
    std::string meNameInput_;
    std::string meNameOutput_;
  };

 public:
  explicit DQMHistNormalizer(const edm::ParameterSet&);
  virtual ~DQMHistNormalizer();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  std::vector<jobEntryType> jobs_;
  double norm_;
};

#endif


