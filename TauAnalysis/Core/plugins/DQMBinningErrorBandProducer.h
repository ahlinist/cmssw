#ifndef TauAnalysis_DQMTools_DQMBinningErrorBandProducer_h
#define TauAnalysis_DQMTools_DQMBinningErrorBandProducer_h

/** \class DQMBinningErrorBandProducer
 *  
 *  Class to compute variations of binning results
 * (representing e.g. effects of systematic uncertainties)
 *
 *  $Date: 2010/02/04 16:11:38 $
 *  $Revision: 1.2 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/DQMTools/interface/DQMErrorBandProducerBase.h"

#include <vector>
#include <string>

class DQMBinningErrorBandProducer : public DQMErrorBandProducerBase
{
 public:
  explicit DQMBinningErrorBandProducer(const edm::ParameterSet&);
  virtual ~DQMBinningErrorBandProducer();

 private:
  void computeErrorBandImp(DQMStore&, const std::string&, const std::vector<std::string>&, 
			   const std::string&, const std::string&, int);
};

#endif


