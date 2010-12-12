#ifndef TauAnalysis_DQMTools_DQMHistKolmogorovTest_h
#define TauAnalysis_DQMTools_DQMHistKolmogorovTest_h

/** \class DQMHistKolmogorovTest
 *  
 *  Class to test compatibility of two distributions
 *  via Kolmogorov-Smirnov test
 *
 *  $Date: 2010/10/31 15:21:08 $
 *  $Revision: 1.5 $
 *  \author Christian Veelken, UC Davis
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <string>
#include <vector>

class DQMHistKolmogorovTest : public edm::EDAnalyzer
{
  struct cfgEntryTest
  {
    explicit cfgEntryTest(const edm::ParameterSet&);
    void print() const;
    void initialize(DQMStore&);
    void save(DQMStore&);
    std::string meName_test_;      // use for Data
    MonitorElement* me_test_;
    std::string meName_reference_; // use for MC
    MonitorElement* me_reference_;
    std::string meName_compatibility_;
    MonitorElement* me_compatibility_;
    double compatibility_;
    int error_;
  };

 public:
  explicit DQMHistKolmogorovTest(const edm::ParameterSet&);
  virtual ~DQMHistKolmogorovTest();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

 private:
  std::vector<cfgEntryTest> cfgEntryTests_;
};

#endif


