#ifndef TauAnalysis_DQMTools_DQMHistIntegrator_h
#define TauAnalysis_DQMTools_DQMHistIntegrator_h

/** \class DQMHistIntegrator
 *  
 *  Class to produce histogram of integrated distribution,
 *  computed by summing bin-contents of histograms,
 *  either from left-to-right or from right-to-left
 *
 *  $Date: 2010/12/12 08:59:27 $
 *  $Revision: 1.1 $
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

class DQMHistIntegrator : public edm::EDAnalyzer
{
  struct cfgEntryPlot
  {
    explicit cfgEntryPlot(const edm::ParameterSet&);
    void print() const;
    void integrate(DQMStore&);
    std::string meName_input_;
    MonitorElement* me_input_;
    enum { kIntegrateFromLeft, kIntegrateFromRight };
    int mode_;
    std::string meName_output_;
    MonitorElement* me_output_;
    int cfgError_;
  };

 public:
  explicit DQMHistIntegrator(const edm::ParameterSet&);
  virtual ~DQMHistIntegrator();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

 private:
  std::vector<cfgEntryPlot> cfgEntryPlots_;
};

#endif


