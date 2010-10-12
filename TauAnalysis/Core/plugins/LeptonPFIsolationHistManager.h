#ifndef TauAnalysis_Core_LeptonPFIsolationHistManager_h
#define TauAnalysis_Core_LeptonPFIsolationHistManager_h 

/** \class LeptonPFIsolationHistManager
 *
 * Booking and filling of lepton isolation histograms
 * (sumPt of particles reconstructed by particle-flow algorithm within isolation cone)
 * 
 * \authors Christian Veelken
 *
 * \version $Revision: 1.3 $
 *
 * $Id: LeptonPFIsolationHistManager.h,v 1.3 2010/02/20 20:58:54 wmtan Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "TauAnalysis/Core/interface/FakeRateJetWeightExtractor.h"
#include "TauAnalysis/Core/interface/PATLeptonGenMatcher.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include <vector>
#include <string>

template <typename T>
class LeptonPFIsolationHistManager : public HistManagerBase 
{
 public:  
  explicit LeptonPFIsolationHistManager(const edm::ParameterSet&);
  ~LeptonPFIsolationHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- auxiliary functions
  double getLeptonWeight(const T&);

//--- labels of tau-jet collection being used
//    in event selection and when filling histograms
  edm::InputTag leptonSrc_;
  double leptonPtMin_;
  double leptonPtMax_;
  double leptonEtaMin_;
  double leptonEtaMax_;

  double dRisoCone_;

  edm::InputTag pfCandidateSrc_;
  
//--- "helper" class for accessing weight values
//    associated to second tau decay products
//    (efficiency/fake-rate with which the tau-jet passes the tau id. criteria)
  std::vector<FakeRateJetWeightExtractor<T>*> leptonWeightExtractors_;

  PATLeptonGenMatcher<T>* genLeptonMatch_;

//--- histograms  
  struct pfIsoHistogramEntryType
  {
    pfIsoHistogramEntryType(double);
    ~pfIsoHistogramEntryType();

    void bookHistograms(DQMStore&, const std::string&, double);
    void fillHistograms(const reco::Particle::LorentzVector&, const std::vector<const reco::PFCandidate*>&, double);

    double dRisoCone_;

    MonitorElement* hPFCandIso0_05dRsigCone_;
    MonitorElement* hPFCandIso0_10dRsigCone_;
    MonitorElement* hPFCandIso0_15dRsigCone_;
    MonitorElement* hPFCandIso0_20dRsigCone_;
    MonitorElement* hPFCandIso0_25dRsigCone_;
    MonitorElement* hPFCandIso0_30dRsigCone_;
  };
  
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold0_5GeVmatched_;
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold0_5GeVunmatched_;
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold1_0GeVmatched_;
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold1_0GeVunmatched_;
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold1_5GeVmatched_;
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold1_5GeVunmatched_;
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold2_0GeVmatched_;
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold2_0GeVunmatched_;
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold2_5GeVmatched_;
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold2_5GeVunmatched_;
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold3_0GeVmatched_;
  pfIsoHistogramEntryType* pfIsoHistogramsPtThreshold3_0GeVunmatched_;

  MonitorElement* hLeptonWeightPosLog_;
  MonitorElement* hLeptonWeightNegLog_;
  MonitorElement* hLeptonWeightZero_;
  MonitorElement* hLeptonWeightLinear_;
};

#endif     
