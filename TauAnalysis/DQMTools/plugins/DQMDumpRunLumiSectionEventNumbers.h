#ifndef TauAnalysis_DQMTools_DQMDumpRunLumiSectionEventNumbers_h
#define TauAnalysis_DQMTools_DQMDumpRunLumiSectionEventNumbers_h

/** \class DQMDumpRunLumiSectionEventNumbers
 *  
 *  Class to write run + luminosity section + event numbers of events selected in FilterStatistics into ASCII file
 *  (format = three columns of numbers separated by white-space character (space or tab);
 *            with the run#/luminosity section#/event# specified in the first/second/third column)
 *
 *  $Date: 2010/10/22 12:21:27 $
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

class DQMDumpRunLumiSectionEventNumbers : public edm::EDAnalyzer
{
 public:
  explicit DQMDumpRunLumiSectionEventNumbers(const edm::ParameterSet&);
  virtual ~DQMDumpRunLumiSectionEventNumbers();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  typedef std::vector<std::string> vstring;
  vstring dqmDirectories_;

  std::string runLumiSectionEventNumberFileName_;

  std::string separator_;

  int cfgError_;
};

#endif


