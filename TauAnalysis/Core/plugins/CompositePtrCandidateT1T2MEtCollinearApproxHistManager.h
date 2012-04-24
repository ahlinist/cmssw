#ifndef TauAnalysis_Core_CompositePtrCandidateT1T2MEtCollinearApproxHistManager_h  
#define TauAnalysis_Core_CompositePtrCandidateT1T2MEtCollinearApproxHistManager_h

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
class CompositePtrCandidateT1T2MEtCollinearApproxHistManager : public HistManagerBase 
{
 public:  
  explicit CompositePtrCandidateT1T2MEtCollinearApproxHistManager(const edm::ParameterSet&);
  ~CompositePtrCandidateT1T2MEtCollinearApproxHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void bookMEtHistograms();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);
  void fillMEtHistograms(const edm::Event&, const CompositePtrCandidateT1T2MEt<T1,T2>&, double);

//--- auxiliary functions
  double getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>&);

//--- configuration parameters
  edm::InputTag diTauCandidateSrc_;

  edm::InputTag genParticleSrc_;
  edm::InputTag genJetSrc_;
  edm::InputTag recoJetSrc_;

  bool requireGenMatch_;

  bool makeCollinearApproxX1X2histograms_;
  bool makeCollinearApproxMassDepHistograms_;

  double collinearApproxMassDPhi12Incr_;
  unsigned numCollinearApproxMassDiTauPtBins_;
  double collinearApproxMassDiTauPtIncr_;
  unsigned numCollinearApproxMassMEtPtBins_;
  double collinearApproxMassMEtPtIncr_;

//--- "helper" class for accessing weight values
//    associated to tau decay products
//    (efficiency/fake-rate with which the tau-jet passes the tau id. criteria)
  std::vector<FakeRateJetWeightExtractor<T1>*> diTauLeg1WeightExtractors_;
  std::vector<FakeRateJetWeightExtractor<T2>*> diTauLeg2WeightExtractors_;

//--- histograms
  MonitorElement* hGenX1_;
  MonitorElement* hGenX2_;
  MonitorElement* hGenX1vsX2_;

  MonitorElement* hX1_;
  MonitorElement* hX2_;

  MonitorElement* hX1vsGenX1_;
  MonitorElement* hX1vsGenX1Profile_;
  MonitorElement* hX2vsGenX2_;
  MonitorElement* hX2vsGenX2Profile_;

  MonitorElement* hGenLeg1ProjGenMEt_;
  MonitorElement* hGenLeg2ProjGenMEt_;

  MonitorElement* hDiTauCandidateWeightPosLog_;
  MonitorElement* hDiTauCandidateWeightNegLog_;
  MonitorElement* hDiTauCandidateWeightZero_;
  MonitorElement* hDiTauCandidateWeightLinear_;

  MonitorElement* hCollinearApproxEffDPhi12dep_;
  MonitorElement* hCollinearApproxEffDiTauPtDep_;
  MonitorElement* hCollinearApproxEffMEtDep_;

  MonitorElement* hCollinearApproxEta_;
  MonitorElement* hCollinearApproxMass_;
  MonitorElement* hCollinearApproxMassRes_;
  MonitorElement* hCollinearApproxMassVsPt_;
  MonitorElement* hCollinearApproxMassVsDPhi12_;
  MonitorElement* hCollinearApproxX1_;
  MonitorElement* hCollinearApproxX2_;
  MonitorElement* hCollinearApproxX1vsX2_;

//--- distributions of invariant mass of the two tau leptons
//    reconstructed via collinear approximation
//    for different acoplanarity angles/Pt values of the visible decay products
  std::vector<MonitorElement*> hCollinearApproxMassDPhi12dep_;
  std::vector<MonitorElement*> hCollinearApproxMassDiTauPtDep_;
  std::vector<MonitorElement*> hCollinearApproxMassMEtPtDep_;

//--- contributions to missing transverse momentum 
//    from difference sources 
  MonitorElement* hMEtTauNeutrinosParallelZ_;
  MonitorElement* hMEtTauNeutrinosPerpendicularZ_;
  MonitorElement* hMEtNonTauNeutrinosParallelZ_;
  MonitorElement* hMEtNonTauNeutrinosPerpendicularZ_;
  MonitorElement* hMEtLeg1MisMeasParallelZ_;
  MonitorElement* hMEtLeg1MisMeasPerpendicularZ_;
  MonitorElement* hMEtLeg2MisMeasParallelZ_;
  MonitorElement* hMEtLeg2MisMeasPerpendicularZ_;
  MonitorElement* hMEtJetMisMeasParallelZ_;
  MonitorElement* hMEtJetMisMeasPerpendicularZ_;
  MonitorElement* hMEtHighEtaJetsParallelZ_;
  MonitorElement* hMEtHighEtaJetsPerpendicularZ_;
  MonitorElement* hMEtUnaccountedParallelZ_;
  MonitorElement* hMEtUnaccountedPerpendicularZ_;
};

#endif  


