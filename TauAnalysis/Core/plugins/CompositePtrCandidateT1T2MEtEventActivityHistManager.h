#ifndef TauAnalysis_Core_CompositePtrCandidateT1T2MEtEventActivityHistManager_h  
#define TauAnalysis_Core_CompositePtrCandidateT1T2MEtEventActivityHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "TauAnalysis/Core/interface/FakeRateJetWeightExtractor.h"
#include "TauAnalysis/RecoTools/interface/PATLeptonTrackExtractor.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include <vector>
#include <string>

template<typename T1, typename T2>
class CompositePtrCandidateT1T2MEtEventActivityHistManager : public HistManagerBase 
{
 public:  
  explicit CompositePtrCandidateT1T2MEtEventActivityHistManager(const edm::ParameterSet&);
  ~CompositePtrCandidateT1T2MEtEventActivityHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- auxiliary functions
  double getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>&);

//--- configuration parameters
  edm::InputTag diTauCandidateSrc_;

  edm::InputTag pfCandidateSrc_;
  edm::InputTag patJetSrc_;

  bool requireGenMatch_;

  typedef std::vector<double> vdouble;
  vdouble etaGaps_;

//--- "helper" class for accessing weight values
//    associated to tau decay products
//    (efficiency/fake-rate with which the tau-jet passes the tau id. criteria)
  std::vector<FakeRateJetWeightExtractor<T1>*> diTauLeg1WeightExtractors_;
  std::vector<FakeRateJetWeightExtractor<T2>*> diTauLeg2WeightExtractors_;

  struct histogramCollectionType
  {
    histogramCollectionType(const edm::ParameterSet&);
    ~histogramCollectionType() {};

    void bookHistograms(DQMStore&, const std::string&);

    void fillHistograms(const CompositePtrCandidateT1T2MEt<T1,T2>&, const reco::PFCandidateCollection&, const pat::JetCollection&, double);

    std::vector<MonitorElement*> hNumChargedParticles_;
    std::vector<MonitorElement*> hNumJets_;
    std::vector<MonitorElement*> hChargedParticleEtSum_;
    std::vector<MonitorElement*> hParticleEtSum_;

    double dRveto_;

    double etaGap_;

    vdouble chargedParticlePtThresholds_;
    vdouble particlePtThresholds_;
    vdouble jetPtThresholds_;
  };

//--- histograms
  std::vector<histogramCollectionType> histograms_;
};

#endif  


