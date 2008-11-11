#ifndef TauHistManager_h  
#define TauHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include <vector>
#include <string>

class TauHistManager : public edm::EDAnalyzer {

 public:
  
  explicit TauHistManager(const edm::ParameterSet&);
  ~TauHistManager();
  
  virtual void beginJob(const edm::EventSetup&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

 private:

//--- private functions
  void bookTauHistograms(DQMStore& dqmStore, MonitorElement*& hTauPt, MonitorElement*& hTauEta, MonitorElement*& hTauPhi, const char* histoSetName);
  
  void fillTauHistograms(const std::vector<pat::Tau>& patTaus, MonitorElement* hTauPt, MonitorElement* hTauEta, MonitorElement* hTauPhi, const char* selVar);
  void fillTauIsoHistograms(const std::vector<pat::Tau>& patTaus);
  void fillTauIsoConeSizeDepHistograms(const std::vector<pat::Tau>& patTaus);

//--- configuration parameters
  edm::InputTag tauSrc_;

  std::string outputFileName_;
  std::string outputDirectoryName_;

  std::string tauKineSelVar_;
  std::string tauHLTmatchSelVar_;
  std::string tauLdgTrkSelVar_;
  std::string tauTrkIsoSelVar_;
  std::string tauEcalIsoSelVar_;
  std::string tauHcalIsoSelVar_;
  std::string tauElecRejVar_;
  std::string tauCrackRejVar_;
  std::string tauProngSelVar_;

  bool requireGenTauMatch_;

  unsigned numTauIsoConeSizes_;
  float tauIsoConeSizeIncr_;

//--- histograms
  MonitorElement* hTauPt_;
  MonitorElement* hTauEta_;
  MonitorElement* hTauPtVsEta_;
  MonitorElement* hTauPhi_;

  MonitorElement* hTauLdgTrkPt_;
  MonitorElement* hTauLdgTrkEta_;
  MonitorElement* hTauLdgTrkPhi_;

  MonitorElement* hTauTrkIsoPt_;
  MonitorElement* hTauEcalIsoPt_;
  MonitorElement* hTauHcalIsoPt_;

  std::vector<MonitorElement*> hTauTrkIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hTauEcalIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hTauHcalIsoPtConeSizeDep_;

  MonitorElement* hTauKineSelPt_;
  MonitorElement* hTauKineSelEta_;
  MonitorElement* hTauKineSelPhi_;

  MonitorElement* hTauHLTmatchSelPt_;
  MonitorElement* hTauHLTmatchSelEta_;
  MonitorElement* hTauHLTmatchSelPhi_;

  MonitorElement* hTauLdgTrkSelPt_;
  MonitorElement* hTauLdgTrkSelEta_;
  MonitorElement* hTauLdgTrkSelPhi_;

  MonitorElement* hTauTrkIsoSelPt_;
  MonitorElement* hTauTrkIsoSelEta_;
  MonitorElement* hTauTrkIsoSelPhi_;

  MonitorElement* hTauEcalIsoSelPt_;
  MonitorElement* hTauEcalIsoSelEta_;
  MonitorElement* hTauEcalIsoSelPhi_;

  MonitorElement* hTauHcalIsoSelPt_;
  MonitorElement* hTauHcalIsoSelEta_;
  MonitorElement* hTauHcalIsoSelPhi_;

  MonitorElement* hTauElecRejPt_; 
  MonitorElement* hTauElecRejEta_;
  MonitorElement* hTauElecRejPhi_;

  MonitorElement* hTauCrackRejPt_; 
  MonitorElement* hTauCrackRejEta_;
  MonitorElement* hTauCrackRejPhi_;

  MonitorElement* hTauProngSelPt_;
  MonitorElement* hTauProngSelEta_;
  MonitorElement* hTauProngSelPhi_;
};

#endif  


