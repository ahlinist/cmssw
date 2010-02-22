#ifndef TauAnalysis_DQMTools_DQMDumpHistogram_h
#define TauAnalysis_DQMTools_DQMDumpHistogram_h

/** \class DQMDumpHistogram
 *  
 *  Class to print-out bin-contents of histogram
 *
 *  $Date: 2009/03/04 12:14:19 $
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

class DQMDumpHistogram : public edm::EDAnalyzer
{
 public:
  explicit DQMDumpHistogram(const edm::ParameterSet&);
  virtual ~DQMDumpHistogram();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  typedef std::vector<std::string> vstring;
  vstring meNames_;

  int cfgError_;
};

#endif


