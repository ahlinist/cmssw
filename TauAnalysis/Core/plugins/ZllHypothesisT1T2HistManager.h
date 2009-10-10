#ifndef TauAnalysis_Core_ZllHypothesisT1T2HistManager_h  
#define TauAnalysis_Core_ZllHypothesisT1T2HistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "TauAnalysis/Core/interface/FakeRateJetWeightExtractor.h"

#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2.h"
#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2Fwd.h"

#include <string>
#include <vector>

template<typename T1, typename T2>
class ZllHypothesisT1T2HistManager : public HistManagerBase 
{
 public:  
  explicit ZllHypothesisT1T2HistManager(const edm::ParameterSet&);
  ~ZllHypothesisT1T2HistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistograms();

  double getZllHypothesisWeight(const ZllHypothesisT1T2<T1,T2>&);
  
  void fillHistograms(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag ZllHypothesisSrc_;

  std::string dqmDirectory_store_;

//--- "helper" class for accessing weight values
//    associated to tau decay products
//    (efficiency/fake-rate with which the tau-jet passes the tau id. criteria)
  std::vector<FakeRateJetWeightExtractor<T1>*> lepton1WeightExtractors_;
  std::vector<FakeRateJetWeightExtractor<T2>*> lepton2WeightExtractors_;

//--- histograms
  MonitorElement* hGenLepton1Pt_;
  MonitorElement* hGenLepton1Eta_;
  MonitorElement* hGenLepton1Phi_;

  MonitorElement* hGenLepton2Pt_;
  MonitorElement* hGenLepton2Eta_;
  MonitorElement* hGenLepton2Phi_;

  MonitorElement* hGenVisMass_;

  MonitorElement* hLepton1bestMatchPt_;
  MonitorElement* hLepton1bestMatchEta_;
  MonitorElement* hLepton1bestMatchPhi_;
  MonitorElement* hLepton1bestMatchType_;

  MonitorElement* hLepton2bestMatchPt_;
  MonitorElement* hLepton2bestMatchEta_;
  MonitorElement* hLepton2bestMatchPhi_;
  MonitorElement* hLepton2bestMatchType_;

  MonitorElement* hLepton1bestMatchPtRes_;
  MonitorElement* hLepton1bestMatchEtaRes_;
  MonitorElement* hLepton1bestMatchPhiRes_;

  MonitorElement* hLepton2bestMatchPtRes_;
  MonitorElement* hLepton2bestMatchEtaRes_;
  MonitorElement* hLepton2bestMatchPhiRes_;

  MonitorElement* hVisMassBestMach_;
  
  MonitorElement* hVisMassFromCaloJets_;
  MonitorElement* hVisMassFromPFJets_;
  MonitorElement* hVisMassFromTracks_;
  MonitorElement* hVisMassFromGsfElectrons_;
  MonitorElement* hVisMassFromGsfTracks_;

  int dqmError_;
};

#endif  


