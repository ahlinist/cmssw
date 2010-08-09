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
 *  $Date: 2010/05/19 08:40:47 $
 *  $Revision: 1.7 $
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
  typedef std::vector<std::string> vstring;

  struct cfgEntryScaleJob
  {
    explicit cfgEntryScaleJob(const edm::ParameterSet&);

    std::string meName_input_;
    std::string meName_inputErr_;
    std::string meName_output_;
    std::string meName_outputErr_;

    std::string dqmDirectory_input_;
    vstring dqmSubDirectories_input_;

    double scaleFactor_;
    double scaleFactorErr_;
    
    std::string meNameScaleFactor_;
    std::string meNameScaleFactorErr_;
    
    std::string meNameNumerator_;
    std::string meNameNumeratorErr_;
    std::string meNameDenominator_;
    std::string meNameDenominatorErr_;

    std::string meType_;

    std::string dqmDirectory_factorizedLooseSel_;
    std::string dqmDirectory_factorizedTightSel_;
    
    std::string dqmDirectory_output_;
    
    int cfgError_;
  };

 public:
  explicit DQMHistScaler(const edm::ParameterSet&);
  virtual ~DQMHistScaler();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

 private:
  std::vector<cfgEntryScaleJob> cfgEntryScaleJobs_;

  vstring dqmDirectories_drop_;
};

#endif


