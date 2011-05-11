#ifndef TauAnalysis_Core_CompositePtrCandidateT1T2MEtNSVfitHistManager_h  
#define TauAnalysis_Core_CompositePtrCandidateT1T2MEtNSVfitHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "TauAnalysis/Core/interface/FakeRateJetWeightExtractor.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include <vector>
#include <string>

template<typename T1, typename T2>
class CompositePtrCandidateT1T2MEtNSVfitHistManager : public HistManagerBase 
{
 public:  
  explicit CompositePtrCandidateT1T2MEtNSVfitHistManager(const edm::ParameterSet&);
  ~CompositePtrCandidateT1T2MEtNSVfitHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- auxiliary functions
  double getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>&);

  void fillHistogramMassResult(MonitorElement*&, const std::string&, const std::string&, const TH1*, double);
  
//--- configuration parameters
  edm::InputTag diTauCandidateSrc_;
  edm::InputTag genParticleSrc_;

  bool requireGenMatch_;

  typedef std::vector<int> vint;
  vint pdgIdsElectron_;
  vint pdgIdsMuon_;
  vint pdgIdsPhoton_;
  vint pdgIdsJet_;

//--- "helper" class for accessing weight values
//    associated to tau decay products
//    (efficiency/fake-rate with which the tau-jet passes the tau id. criteria)
  std::vector<FakeRateJetWeightExtractor<T1>*> diTauLeg1WeightExtractors_;
  std::vector<FakeRateJetWeightExtractor<T2>*> diTauLeg2WeightExtractors_;

  struct massHypothesisEntryType
  {
    edm::InputTag nSVfitEventHypothesisSrc_; // get NSVfitEventHypothesis via InputTag, directly from edm::Event 
    std::string nSVfitEventHypothesisName_;  // get NSVfitEventHypothesis via CompositeRefCandidateT1T2MEt::nSVfitSolution(name)

    std::string dqmDirectory_;

    MonitorElement* hMass_;
    MonitorElement* hMassL_; 
    MonitorElement* hMassXL_; 
    MonitorElement* hMassGenLeg2Electron_;
    MonitorElement* hMassGenLeg2Muon_;
    MonitorElement* hMassGenLeg2Photon_;
    MonitorElement* hMassGenLeg2Jet_;
    
    MonitorElement* hMassMedian_;
    MonitorElement* hMassMean_;
    MonitorElement* hMassMaximum_;
    MonitorElement* hMassMaxInterpol_;
    
    MonitorElement* hMassPull_;
    
    MonitorElement* hMassSum_;
    MonitorElement* hMassSumNormalized_;
    
    MonitorElement* hMassHadRecoilPtLt10_;
    MonitorElement* hMassHadRecoilPt10to20_;
    MonitorElement* hMassHadRecoilPt20to30_;
    MonitorElement* hMassHadRecoilPtGt30_;
    MonitorElement* hHadRecoilPt_;

    MonitorElement* hMassDPhiGt175_;
    MonitorElement* hMassDPhi170to175_;
    MonitorElement* hMassDPhi160to170_;
    MonitorElement* hMassDPhi140to160_;
    MonitorElement* hMassDPhi120to140_;
    MonitorElement* hMassDPhiLt120_;
    MonitorElement* hDPhi_;
    
    typedef std::map<std::string, std::map<std::string, MonitorElement*> > decayModeMapType;
    std::map<std::string, std::map<std::string, decayModeMapType> > hMassGenVsRecDecayModes_; // key = gen. decay mode leg1, 
                                                                                              //       rec. decay mode leg1,
                                                                                              //       gen. decay mode leg2,
                                                                                              //       rec. decay mode leg2
  };

  std::vector<massHypothesisEntryType*> massHypothesisEntries_;

  MonitorElement* hDiTauCandidateWeightPosLog_;
  MonitorElement* hDiTauCandidateWeightNegLog_;
  MonitorElement* hDiTauCandidateWeightZero_;
  MonitorElement* hDiTauCandidateWeightLinear_;
};

#endif  


