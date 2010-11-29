#ifndef TauAnalysis_Core_GenPhaseSpaceEventInfoHistManager_h  
#define TauAnalysis_Core_GenPhaseSpaceEventInfoHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include <string>

class GenPhaseSpaceEventInfoHistManager : public HistManagerBase 
{
 public:  
  explicit GenPhaseSpaceEventInfoHistManager(const edm::ParameterSet&);
  ~GenPhaseSpaceEventInfoHistManager();

 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag generatorInfoSource_;
  edm::InputTag genPhaseSpaceEventInfoSource_;
  edm::InputTag genJetSource_;
  edm::InputTag genParticlesFromZsSource_;
  edm::InputTag genParticlesFromHsSource_;

  double genJetPtMin_;
  double genJetPtMax_;
  double genJetEtaMin_;
  double genJetEtaMax_;   

  bool makeBjorkenXratioHistogram_;
  bool makeLeptonPtVsPtHatHistograms_;

//--- histograms
  MonitorElement* hProcessId_;
  MonitorElement* hPtHat_;
  MonitorElement* hBjorkenX1_;
  MonitorElement* hBjorkenX2_;
  MonitorElement* hBjorkenXratio_;

  MonitorElement* hLeadingElectronPt_;
  MonitorElement* hLeadingElectronEta_;
  MonitorElement* hLeadingMuonPt_;
  MonitorElement* hLeadingMuonEta_;
  MonitorElement* hLeadingTauLeptonPt_;
  MonitorElement* hLeadingTauLeptonEta_;

  MonitorElement* hLeadingElectronPtVsPtHat_;
  MonitorElement* hLeadingMuonPtVsPtHat_;
  MonitorElement* hLeadingTauLeptonPtVsPtHat_;

  MonitorElement* hNumGenJets_;
  
  MonitorElement* hGenZsPt_;
  MonitorElement* hGenZsEta_;
  MonitorElement* hGenZsMass_;

  MonitorElement* hGenParticlesFromZsPt_;
  MonitorElement* hGenParticlesFromZsEta_;
  MonitorElement* hGenParticlesFromZsPdgId_;

  MonitorElement* hGenHsPt_;
  MonitorElement* hGenHsEta_;
  MonitorElement* hGenHsMass_;

  MonitorElement* hGenParticlesFromHsPt_;
  MonitorElement* hGenParticlesFromHsEta_;
  MonitorElement* hGenParticlesFromHsPdgId_;
};

#endif  


