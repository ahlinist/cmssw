#ifndef TauAnalysis_Core_PFCandidateHistManager_h  
#define TauAnalysis_Core_PFCandidateHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include <vector>
#include <string>

class PFCandidateHistManager : public HistManagerBase 
{
 public:  
  explicit PFCandidateHistManager(const edm::ParameterSet&);
  ~PFCandidateHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- auxiliary functions
  void bookPFCandidateHistograms(MonitorElement*&, MonitorElement*&, MonitorElement*&, const char*);
  
  void fillPFCandidateHistograms(const reco::PFCandidate&, MonitorElement*, MonitorElement*, MonitorElement*, double);

//--- configuration parameters
  edm::InputTag pfCandidateSrc_;

//--- histograms
  MonitorElement* hNumPFCandidates_;

  MonitorElement* hPFCandidatePt_;
  MonitorElement* hPFCandidateEta_;
  MonitorElement* hPFCandidatePhi_;

  MonitorElement* hPFCandidateSumEt_;
};

#endif  


