#include "TauAnalysis/RecoTools/plugins/PATLeptonIsoDepositSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"

#include "DataFormats/PatCandidates/interface/Lepton.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

#include <sstream>

template <class T>
PATLeptonIsoDepositSelector<T>::PATLeptonIsoDepositSelector(const edm::ParameterSet& cfg)
{
  std::string isoDepositType_string = cfg.getParameter<std::string>("type");
  if ( isoDepositType_string == "tracker" ) {
    isoDepositType_ = pat::TrackerIso;
  } else if ( isoDepositType_string == "ecal" ) {
    isoDepositType_ = pat::ECalIso;
  } else if ( isoDepositType_string == "hcal" ) {
    isoDepositType_ = pat::HCalIso;
  } else if ( isoDepositType_string == "pfAllParticles" ) {
    isoDepositType_ = pat::ParticleIso;
  } else if ( isoDepositType_string == "pfChargedHadron" ) {
    isoDepositType_ = pat::ChargedParticleIso;
  } else if ( isoDepositType_string == "pfNeutralHadron" ) {
    isoDepositType_ = pat::NeutralParticleIso;
  } else if ( isoDepositType_string == "pfGamma" ) {
    isoDepositType_ = pat::GammaParticleIso;
  } else {
    edm::LogError ("PATLeptonIsoDepositSelector") << " Type = " << isoDepositType_string << " not defined for IsoDeposits !!";
    cfgError_ = 1;
  }

  dRvetoCone_ = cfg.exists("dRvetoCone") ? cfg.getParameter<double>("dRvetoCone") : -1.;
  dRisoCone_ = cfg.getParameter<double>("dRisoCone");

  ptMin_ = cfg.exists("ptMin") ? cfg.getParameter<double>("ptMin") : -1.;
  
  sumPtMax_ = cfg.exists("sumPtMax") ? cfg.getParameter<double>("sumPtMax") : -1.;
  numMax_ = cfg.exists("numMax") ? cfg.getParameter<int>("numMax") : -1;
  if ( !(cfg.exists("sumPtMax") || cfg.exists("numMax")) ) {
    edm::LogError ("PATLeptonIsoDepositSelector") << " Need to specify either 'sumPtMax' or 'numMax' Parameters !!";
    cfgError_ = 1;
  }
}

template <class T>
PATLeptonIsoDepositSelector<T>::~PATLeptonIsoDepositSelector()
{
// nothing to be done yet...
}

std::string getString(double value)
{
  std::ostringstream value_string;
  value_string << value;
  return value_string.str();
}

template <class T>
void PATLeptonIsoDepositSelector<T>::select(const edm::Handle<collection>& patLeptons, 
					    const edm::Event& evt, const edm::EventSetup& es)
{
  selected_.clear();
  
  if ( cfgError_ ) {
    edm::LogError ("select") << " Error in Configuration ParameterSet --> no pat::Leptons will pass Selection !!";
    return;
  }

  for ( typename collection::const_iterator patLepton = patLeptons->begin();
	patLepton != patLeptons->end(); ++patLepton ) {
    const reco::IsoDeposit* isoDeposit = patLepton->isoDeposit(isoDepositType_);

    if ( isoDeposit ) {

      reco::isodeposit::AbsVetos isoDepositParam;
      std::string dRvetoCone_string = getString(dRvetoCone_);
      if ( dRvetoCone_ > 0. ) isoDepositParam.push_back(IsoDepositVetoFactory::make(dRvetoCone_string.data()));
      std::string ptMin_string = std::string("Threshold(").append(getString(ptMin_)).append(")");
      if ( ptMin_      > 0. ) isoDepositParam.push_back(IsoDepositVetoFactory::make(ptMin_string.data()));

      double sumPt = isoDeposit->depositWithin(dRisoCone_, isoDepositParam);
      double num = isoDeposit->countWithin(dRisoCone_, isoDepositParam);

      if ( sumPtMax_ > 0. && sumPt > sumPtMax_ ) continue;
      if ( num       > 0  && num   > numMax_   ) continue;

      selected_.push_back(&(*patLepton)); 
    } else {
      edm::LogError ("select") << " No IsoDeposit of type = " << isoDepositType_ << " associated to pat::Lepton with "
			       << " Pt = " << patLepton->pt() << "," 
			       << " theta = " << patLepton->theta() << "," 
			       << " phi = " << patLepton->phi() << " --> pat::Lepton fails Selection !!";
      continue;
    }
  }
}

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef ObjectSelector<PATLeptonIsoDepositSelector<pat::Electron> > PATElectronIsoDepositSelector;
typedef ObjectSelector<PATLeptonIsoDepositSelector<pat::Muon> > PATMuonIsoDepositSelector;
typedef ObjectSelector<PATLeptonIsoDepositSelector<pat::Tau> > PATTauIsoDepositSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_ANOTHER_FWK_MODULE(PATElectronIsoDepositSelector);
DEFINE_ANOTHER_FWK_MODULE(PATMuonIsoDepositSelector);
DEFINE_ANOTHER_FWK_MODULE(PATTauIsoDepositSelector);
