#ifndef TauAnalysis_Core_CompositePtrCandidateTMEtHistManager_h  
#define TauAnalysis_Core_CompositePtrCandidateTMEtHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

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
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- auxiliary functions
  double getCandidateWeight(const CompositePtrCandidateTMEt<T>&);

//--- configuration parameters
  edm::InputTag tauNuCandidateSrc_;
  
  bool requireGenMatch_;

//--- "helper" class for accessing weight values
//    associated to tau-jet
//    (efficiency/fake-rate with which the tau-jet passes the tau id. criteria)
  std::vector<FakeRateJetWeightExtractor<T>*> tauJetWeightExtractors_;

//--- histograms
  MonitorElement* hNuTauCandidatePt_;
  MonitorElement* hNuTauCandidatePhi_;

  MonitorElement* hNuTauCandidateWeightPosLog_;
  MonitorElement* hNuTauCandidateWeightNegLog_;
  MonitorElement* hNuTauCandidateWeightZero_;
  MonitorElement* hNuTauCandidateWeightLinear_;

  MonitorElement* hNuTauCandidateDPhi_;
  MonitorElement* hNuTauCandidateMt_;
  MonitorElement* hNuTauCandidateMtBySVfit_;
  
  MonitorElement* hMEtVsTauPt_;
};

#endif  


