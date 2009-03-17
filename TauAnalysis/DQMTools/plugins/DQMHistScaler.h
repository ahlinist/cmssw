#ifndef ElectroWeakAnalysis_EWKTau_DQMHistScaler_h
#define ElectroWeakAnalysis_EWKTau_DQMHistScaler_h

/** \class DQMHistScaler
 *  
 *  Class to scale DQM monitoring elements according to 
 *  either fixed scale factor or ratio of monitor elements
 *  (NOTE: to be used for the purpose of factorizing efficiencies
 *         of muon isolation from other event selection criteria,
 *         in order to avoid problems with limited Monte Carlo statistics)
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

#include "DQMServices/Core/interface/DQMDefinitions.h"

#include <TH1.h>

#include <vector>
#include <string>

class DQMHistScaler : public edm::EDAnalyzer
{
 public:
  explicit DQMHistScaler(const edm::ParameterSet&);
  virtual ~DQMHistScaler();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

 private:
  std::string dqmDirectory_input_;
  typedef std::vector<std::string> vstring;
  vstring dqmSubDirectories_input_;
  std::string dqmDirectory_output_;

  double cfgScaleFactor_;

  std::string numerator_;
  std::string denominator_;
  std::string type_;

  int cfgError_;
};

#endif


