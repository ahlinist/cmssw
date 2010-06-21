#ifndef TauAnalysis_Core_DQMDumpGenMatrixProbabilities_h
#define TauAnalysis_Core_DQMDumpGenMatrixProbabilities_h

/** \class DQMDumpGenMatrixProbabilities
 *  
 *  Class to print-out binning results information contained in objects inheriting from BinningBase class
 *
 *  $Date: 2009/12/04 13:42:01 $
 *  $Revision: 1.1 $
 *  \author Christian Veelken, UC Davis
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/Core/interface/BinningBase.h"
#include "TauAnalysis/Core/interface/BinningServiceBase.h"

#include <string>
#include <vector>
#include <map>

class DQMDumpGenMatrixProbabilities : public edm::EDAnalyzer
{
 public:
  explicit DQMDumpGenMatrixProbabilities(const edm::ParameterSet&);
  virtual ~DQMDumpGenMatrixProbabilities();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  BinningBase* loadBinningResults(const std::string&);

  typedef std::vector<std::string> vstring;
  vstring processes_;

  std::map<std::string, std::string> dqmDirectories_;

  BinningServiceBase* binningService_;

  std::map<std::string, BinningBase*> binningResults_;

  int cfgError_;
};

#endif


