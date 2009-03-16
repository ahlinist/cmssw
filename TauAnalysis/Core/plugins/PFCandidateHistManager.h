#ifndef TauAnalysis_Core_PFCandidateHistManager_h  
#define TauAnalysis_Core_PFCandidateHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include <vector>
#include <string>

class PFCandidateHistManager : public HistManagerBase 
{
 public:
  
  explicit PFCandidateHistManager(const edm::ParameterSet&);
  ~PFCandidateHistManager();
  
  void bookHistograms(const edm::EventSetup&);
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

 private:

//--- private functions
  void bookPFCandidateHistograms(DQMStore&, MonitorElement*&, MonitorElement*&, MonitorElement*&, const char*);
  
  void fillPFCandidateHistograms(const reco::PFCandidate&, MonitorElement*, MonitorElement*, MonitorElement*);

//--- configuration parameters
  edm::InputTag pfCandidateSrc_;

  std::string dqmDirectory_store_;

//--- histograms
  MonitorElement* hPFCandidatePt_;
  MonitorElement* hPFCandidateEta_;
  MonitorElement* hPFCandidatePtVsEta_;
  MonitorElement* hPFCandidatePhi_;
};

#endif  


