#include "TauAnalysis/RecoTools/plugins/PATLeptonPFIsolationSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

template <class T>
PATLeptonPFIsolationSelector<T>::PATLeptonPFIsolationSelector(const edm::ParameterSet& cfg)
  : sumPtMethod_(kAbsoluteIso),
    cfgError_(0)
{
  pfCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfCandidateSource");

  edm::ParameterSet cfgChargedHadronIso = cfg.getParameter<edm::ParameterSet>("chargedHadronIso");
  pfChargedHadronIso_ = new pfIsoConfigType(reco::PFCandidate::h, cfgChargedHadronIso);

  edm::ParameterSet cfgNeutralHadronIso = cfg.getParameter<edm::ParameterSet>("neutralHadronIso");
  pfNeutralHadronIso_ = new pfIsoConfigType(reco::PFCandidate::h0, cfgNeutralHadronIso);

  edm::ParameterSet cfgPhotonIso = cfg.getParameter<edm::ParameterSet>("photonIso");
  pfPhotonIso_ = new pfIsoConfigType(reco::PFCandidate::gamma, cfgPhotonIso);
  
  sumPtMin_ = cfg.exists("sumPtMin") ? 
    cfg.getParameter<double>("sumPtMin") : -1.;
  sumPtMax_ = cfg.getParameter<double>("sumPtMax");

  if ( cfg.exists("sumPtMethod") ) {
    std::string sumPtMethod_string = cfg.getParameter<std::string>("sumPtMethod");
    if ( sumPtMethod_string == "absolute" ) {
      sumPtMethod_ = kAbsoluteIso;
    } else if ( sumPtMethod_string == "relative" ) {
      sumPtMethod_ = kRelativeIso;
    } else {
      edm::LogError("PATLeptonPFIsolationSelector") 
	<< " Configuration parameter 'sumPtMethod' = " << sumPtMethod_string << " invalid !!";
      cfgError_ = 1;
    }
  }
}

template <class T>
PATLeptonPFIsolationSelector<T>::~PATLeptonPFIsolationSelector()
{
  delete pfChargedHadronIso_;
  delete pfNeutralHadronIso_;
  delete pfPhotonIso_;
}

template <class T>
void PATLeptonPFIsolationSelector<T>::select(const edm::Handle<collection>& patLeptons, 
					     const edm::Event& evt, const edm::EventSetup& es)
{
  selected_.clear();
  
  if ( cfgError_ ) {
    edm::LogError ("select") << " Error in Configuration ParameterSet --> no pat::Leptons will pass Selection !!";
    return;
  }

  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  evt.getByLabel(pfCandidateSrc_, pfCandidates);

  for ( typename collection::const_iterator patLepton = patLeptons->begin();
	patLepton != patLeptons->end(); ++patLepton ) {
    double sumPt = pfChargedHadronIso_->compSumPt(*pfCandidates, patLepton->p4());
    sumPt += pfNeutralHadronIso_->compSumPt(*pfCandidates, patLepton->p4());
    sumPt += pfPhotonIso_->compSumPt(*pfCandidates, patLepton->p4());

    if ( sumPtMethod_ == kAbsoluteIso ) {
      if ( sumPtMin_ > 0. && sumPt < sumPtMin_ ) continue;
      if ( sumPtMax_ > 0. && sumPt > sumPtMax_ ) continue;
    } else if ( sumPtMethod_ == kRelativeIso ) {
      double relIso = ( patLepton->pt() > 1. ) ? (sumPt/patLepton->pt()) : sumPt;
      if ( sumPtMin_ > 0. && relIso < sumPtMin_ ) continue;
      if ( sumPtMax_ > 0. && relIso > sumPtMax_ ) continue;
    }

    selected_.push_back(&(*patLepton)); 
  }
}

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef ObjectSelector<PATLeptonPFIsolationSelector<pat::Electron> > PATElectronPFIsolationSelector;
typedef ObjectSelector<PATLeptonPFIsolationSelector<pat::Muon> > PATMuonPFIsolationSelector;
typedef ObjectSelector<PATLeptonPFIsolationSelector<pat::Tau> > PATTauPFIsolationSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronPFIsolationSelector);
DEFINE_FWK_MODULE(PATMuonPFIsolationSelector);
DEFINE_FWK_MODULE(PATTauPFIsolationSelector);
