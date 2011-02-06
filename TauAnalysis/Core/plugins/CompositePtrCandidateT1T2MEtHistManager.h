#ifndef TauAnalysis_Core_CompositePtrCandidateT1T2MEtHistManager_h
#define TauAnalysis_Core_CompositePtrCandidateT1T2MEtHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "TauAnalysis/Core/interface/FakeRateJetWeightExtractor.h"
#include "TauAnalysis/RecoTools/interface/PATLeptonTrackExtractor.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include <vector>
#include <string>

template<typename T1, typename T2>
class CompositePtrCandidateT1T2MEtHistManager : public HistManagerBase
{
 public:
  explicit CompositePtrCandidateT1T2MEtHistManager(const edm::ParameterSet&);
  ~CompositePtrCandidateT1T2MEtHistManager();

 private:
//--- histogram booking and filling functions
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- auxiliary functions
  double getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>&);

//--- configuration parameters
  edm::InputTag diTauCandidateSrc_;
  edm::InputTag genParticleSrc_;
  edm::InputTag vertexSrc_;
  edm::InputTag visMassHypothesisSrc_;
  edm::InputTag pfCandidateSrc_;

  bool requireGenMatch_;

  typedef std::vector<int> vint;
  vint pdgIdsElectron_;
  vint pdgIdsMuon_;
  vint pdgIdsPhoton_;
  vint pdgIdsJet_;

  bool makeMEtProjResolutionHistograms_;

//--- "helper" class for accessing weight values
//    associated to tau decay products
//    (efficiency/fake-rate with which the tau-jet passes the tau id. criteria)
  std::vector<FakeRateJetWeightExtractor<T1>*> diTauLeg1WeightExtractors_;
  std::vector<FakeRateJetWeightExtractor<T2>*> diTauLeg2WeightExtractors_;

//--- "helper" classes for accessing the tracks
//    of the two tau decay products
  PATLeptonTrackExtractor<T1> trackExtractorLeg1_;
  PATLeptonTrackExtractor<T2> trackExtractorLeg2_;

//--- histograms
  MonitorElement* hGenDiTauCandidatePt_;
  MonitorElement* hGenDiTauCandidateVisPt_;
  MonitorElement* hGenDiTauCandidateEta_;
  MonitorElement* hGenDiTauCandidatePhi_;

  MonitorElement* hGenLeg1En_;
  MonitorElement* hGenLeg2En_;
  MonitorElement* hGenLeg1PtVsLeg2Pt_;
  MonitorElement* hGenLeg1EtaVsLeg2Eta_;
  MonitorElement* hGenDeltaRleg1VisNu_;
  MonitorElement* hGenDeltaRleg2VisNu_;
  MonitorElement* hGenLeg1DecayTime_;
  MonitorElement* hGenLeg2DecayTime_;
  MonitorElement* hGenMass_;

  MonitorElement* hGenSqrtX1X2VsDPhi12_;
  MonitorElement* hGenSqrtX1X2VsVisEnRatio_;
  MonitorElement* hGenSqrtX1X2VsVisPtRatio_;
  MonitorElement* hSqrtX1X2VsVisEnRatio_;
  MonitorElement* hSqrtX1X2VsVisPtRatio_;

  MonitorElement* hGenLeg1TauPlusDecayAngleLepton_;
  MonitorElement* hGenLeg1TauPlusDecayAngleOneProng_;
  MonitorElement* hGenLeg1TauPlusDecayAngleThreeProng_;
  MonitorElement* hGenLeg1TauMinusDecayAngleLepton_;
  MonitorElement* hGenLeg1TauMinusDecayAngleOneProng_;
  MonitorElement* hGenLeg1TauMinusDecayAngleThreeProng_;
  MonitorElement* hGenLeg2TauPlusDecayAngleLepton_;
  MonitorElement* hGenLeg2TauPlusDecayAngleOneProng_;
  MonitorElement* hGenLeg2TauPlusDecayAngleThreeProng_;
  MonitorElement* hGenLeg2TauMinusDecayAngleLepton_;
  MonitorElement* hGenLeg2TauMinusDecayAngleOneProng_;
  MonitorElement* hGenLeg2TauMinusDecayAngleThreeProng_;

  MonitorElement* hDiTauCandidatePt_;
  MonitorElement* hDiTauCandidateVisPt_;
  MonitorElement* hDiTauCandidateEta_;
  MonitorElement* hDiTauCandidatePhi_;
  MonitorElement* hDiTauCandidateCharge_;
  MonitorElement* hDiTauCandidateMass_;

  // MET resolutions
  MonitorElement* hMETresXvsSumEt_;
  MonitorElement* hMETresYvsSumEt_;
  MonitorElement* hMETresParMuonvsSumEt_;
  MonitorElement* hMETresPerpMuonvsSumEt_;
  MonitorElement* hMETresParDiTauvsSumPpar_;
  MonitorElement* hMETresPerpDiTauvsSumPperp_;

  MonitorElement* hLeg1PtVsLeg2Pt_;
  MonitorElement* hLeg1EtaVsLeg2Eta_;
  MonitorElement* hLeg1IsoVsLeg2Iso_;

  MonitorElement* hDiTauCandidateWeightPosLog_;
  MonitorElement* hDiTauCandidateWeightNegLog_;
  MonitorElement* hDiTauCandidateWeightZero_;
  MonitorElement* hDiTauCandidateWeightLinear_;

  MonitorElement* hDiTauCandidateImpParSig_;

  MonitorElement* hVisPt_;
  MonitorElement* hVisPhi_;
  MonitorElement* hVisMass_;
  MonitorElement* hVisMassL_;
  MonitorElement* hVisMassXL_;
  MonitorElement* hVisMassGenLeg2Electron_;
  MonitorElement* hVisMassGenLeg2Muon_;
  MonitorElement* hVisMassGenLeg2Photon_;
  MonitorElement* hVisMassGenLeg2Jet_;
  MonitorElement* hVisMassZllCombinedHypothesis_;

  MonitorElement* hCollinearApproxEta_;
  MonitorElement* hCollinearApproxMass_;
  MonitorElement* hCollinearApproxX1_;
  MonitorElement* hCollinearApproxX2_;

  MonitorElement* hCDFmethodMass_;

  MonitorElement* hMt12MET_;

  MonitorElement* hMt1MET_;
  MonitorElement* hMt2MET_;

  MonitorElement* hHt12MET_;

  MonitorElement* hDPhi12_;
  MonitorElement* hDR12_;

  MonitorElement* hVisEtaMin_;
  MonitorElement* hVisEtaMax_;

  MonitorElement* hDPhi1MET_;
  MonitorElement* hDPhi2MET_;
  MonitorElement* hDPhi1METvsDPhi2MET_;

  MonitorElement* hPzetaCorr_;
  MonitorElement* hPzetaDiff_;
  MonitorElement* hPzetaDiffVsMt1MET_;
};

#endif


