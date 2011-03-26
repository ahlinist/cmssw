#ifndef TauAnalysis_Core_CandidateHistManager_h  
#define TauAnalysis_Core_CandidateHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include <vector>
#include <string>

class CandidateHistManager : public HistManagerBase 
{
 public:  
  explicit CandidateHistManager(const edm::ParameterSet&);
  ~CandidateHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag candidateSrc_;

//--- histograms
  MonitorElement* hCandidatePt_;
  MonitorElement* hCandidateEta_;
  MonitorElement* hCandidatePhi_;

  MonitorElement* hCandidateMass_;

  MonitorElement* hCandidateCharge_;
};

#endif  


