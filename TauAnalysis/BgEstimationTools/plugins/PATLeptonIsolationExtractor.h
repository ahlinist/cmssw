#ifndef TauAnalysis_BgEstimationTools_PATLeptonIsolationExtractor_h  
#define TauAnalysis_BgEstimationTools_PATLeptonIsolationExtractor_h

/** \class PATLeptonIsolationExtractor
 *
 * Auxiliary class for extracting electron, muon or tau-jet isolation value from IsoDeposits
 * (used for Ntuple filling)
 *
 * NOTE: the values are extracted from the PAT lepton
 *       specified by the "index" configuration parameter (**first** PAT lepton in case "index" is not specified)
 *       contained in the collection specified by the "src" configuration parameter;
 *       in case the collection of PAT objects is empty, 
 *       a substitute value of 1.e+3 is returned by operator();
 *       the same substitute value is returned in case no IsoDeposit of type specified in configuration parameters is found
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: PATLeptonIsolationExtractor.h,v 1.2 2009/07/30 16:08:12 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"

#include "PhysicsTools/Utilities/interface/StringObjectFunction.h" // for CMSSW_2_2_x
//#include "CommonTools/Utils/interface/StringObjectFunction.h"    // for CMSSW_3_1_x

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

#include <string>
#include <vector>

template <typename T>
class PATLeptonIsolationExtractor : public ObjValExtractorBase
{
 public:
  
  explicit PATLeptonIsolationExtractor(const edm::ParameterSet&);
  ~PATLeptonIsolationExtractor();
  
  double operator()(const edm::Event&) const;

 private:

//--- configuration parameters
  edm::InputTag src_;
  typedef std::vector<pat::IsolationKeys> vIsoKeys;
  vIsoKeys isoTypes_;
  double dR_;
  std::vector<reco::isodeposit::AbsVeto*> vetos_;
  enum { kSum, kCount };
  int mode_;
  StringObjectFunction<T>* scale_;
  
  int error_;
  
  unsigned index_;
};

#endif  


