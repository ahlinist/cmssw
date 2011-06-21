#ifndef TauAnalysis_Core_VBFCompositePtrCandidateT1T2MEtEventT3HistManager_h  
#define TauAnalysis_Core_VBFCompositePtrCandidateT1T2MEtEventT3HistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "TauAnalysis/Core/interface/FakeRateJetWeightExtractor.h"

#include "AnalysisDataFormats/TauAnalysis/interface/VBFCompositePtrCandidateT1T2MEtEventT3.h"

#include <vector>
#include <string>

template<typename T1, typename T2, typename T3>
class VBFCompositePtrCandidateT1T2MEtEventT3HistManager : public HistManagerBase 
{
 public:  
  explicit VBFCompositePtrCandidateT1T2MEtEventT3HistManager(const edm::ParameterSet&);
  ~VBFCompositePtrCandidateT1T2MEtEventT3HistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- auxiliary functions
  double getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>&);
  
//--- configuration parameters
  edm::InputTag vbfEventSrc_;
  edm::InputTag genParticleSrc_;

  bool requireGenMatch_;

//--- "helper" class for accessing weight values
//    associated to tau decay products
//    (efficiency/fake-rate with which the tau-jet passes the tau id. criteria)
  std::vector<FakeRateJetWeightExtractor<T1>*> diTauLeg1WeightExtractors_;
  std::vector<FakeRateJetWeightExtractor<T2>*> diTauLeg2WeightExtractors_;

  MonitorElement* hTagJet1Pt_;
  MonitorElement* hTagJet1Eta_;
  MonitorElement* hTagJet1Phi_;

  MonitorElement* hTagJet2Pt_;
  MonitorElement* hTagJet2Eta_;
  MonitorElement* hTagJet2Phi_;
  
  MonitorElement* hTagJetMass_;
  MonitorElement* hTagJetDEta_;
  MonitorElement* hTagJetDPhi_;
  MonitorElement* hTagJetEtaProd_;
  
  MonitorElement* hCentralJetPt_;
  MonitorElement* hCentralJetEta_;
  MonitorElement* hCentralJetPhi_;

  MonitorElement* hDiTauCandidateWeightPosLog_;
  MonitorElement* hDiTauCandidateWeightNegLog_;
  MonitorElement* hDiTauCandidateWeightZero_;
  MonitorElement* hDiTauCandidateWeightLinear_;
};

#endif  


