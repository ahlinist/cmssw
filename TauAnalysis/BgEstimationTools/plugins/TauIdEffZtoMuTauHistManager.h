#ifndef TauAnalysis_BgEstimationTools_TauIdEffZtoMuTauHistManager_h  
#define TauAnalysis_BgEstimationTools_TauIdEffZtoMuTauHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include <vector>
#include <string>

class TauIdEffZtoMuTauHistManager : public HistManagerBase 
{
 public:  
  explicit TauIdEffZtoMuTauHistManager(const edm::ParameterSet&);
  ~TauIdEffZtoMuTauHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag muonSrc_;
  edm::InputTag tauSrc_;
  edm::InputTag diTauSrc_;
  edm::InputTag centralJetSrc_;

  std::string tauIdDiscriminator_;
  ObjValExtractorBase* diTauChargeSignExtractor_;

  std::vector<reco::isodeposit::AbsVeto*> tauIsolationVetos_;

//--- histograms
  MonitorElement* hTauDiscriminatorByEwkTauId_;
  MonitorElement* hTauExtParticleFlowIsoPt_;
  MonitorElement* hTauExtPFChargedHadronIsoPt_;
  MonitorElement* hTauExtPFNeutralHadronIsoPt_;
  MonitorElement* hTauExtPFGammaIsoPt_;

  MonitorElement* hMuonPt_;
  MonitorElement* hMuonAbsEta_;
  MonitorElement* hMuonPtVsAbsEta_;
  MonitorElement* hMuonExtTrkIsoPt_;
  MonitorElement* hMuonExtEcalIsoPt_;
  MonitorElement* hMuonExtHcalIsoPt_;
  MonitorElement* hMuonExtIsoSumPt_;

  MonitorElement* hDiTauChargeSign_;
  MonitorElement* hDiTauPtProj_;
  MonitorElement* hDiTauMEtProj_;
  MonitorElement* hDiTauVisMassFromJetP4_;
  MonitorElement* hDiTauVisMassFromTauJetP4_;
  MonitorElement* hDiTauCollinearApproxMassFromJetP4_;
  MonitorElement* hDiTauCollinearApproxMassFromTauJetP4_;

  MonitorElement* hNumCentralJets_;
  MonitorElement* hCentralJetPt_;
  MonitorElement* hCentralJetEta_;
  MonitorElement* hCentralJetPhi_;
};

#endif  


