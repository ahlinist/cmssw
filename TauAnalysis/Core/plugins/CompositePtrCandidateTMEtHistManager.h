#ifndef TauAnalysis_Core_CompositePtrCandidateTMEtHistManager_h  
#define TauAnalysis_Core_CompositePtrCandidateTMEtHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "TauAnalysis/Core/interface/FakeRateJetWeightExtractor.h"
#include "TauAnalysis/RecoTools/interface/PATLeptonTrackExtractor.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"

#include <vector>
#include <string>

template<typename T>
class CompositePtrCandidateTMEtHistManager : public HistManagerBase 
{
 public:  
  explicit CompositePtrCandidateTMEtHistManager(const edm::ParameterSet&);
  ~CompositePtrCandidateTMEtHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistograms();
  void fillHistograms(const edm::Event&, const edm::EventSetup&, double);

//--- auxiliary functions
  double getTauWeight(const CompositePtrCandidateTMEt<T>&);

//--- configuration parameters
  edm::InputTag tauNuCandidateSrc_;

  std::string tauJetWeightSrc_;
  
  std::string dqmDirectory_store_;
  
  bool requireGenMatch_;

//--- "helper" class for accessing weight values
//    associated to tau-jet
//    (efficiency/fake-rate with which the tau-jet passes the tau id. criteria)
  FakeRateJetWeightExtractor<T>* tauJetWeightExtractor_;

//--- histograms
  MonitorElement* hNuTauCandidatePt_;
  MonitorElement* hNuTauCandidatePhi_;

  MonitorElement* hNuTauCandidateDPhi_;
  MonitorElement* hNuTauCandidateMt_;
  
  MonitorElement* hMEtVsTauPt_;

  int dqmError_;
};

#endif  


