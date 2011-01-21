#include "TauAnalysis/BgEstimationTools/interface/FakeRateWeightProducerBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/TauReco/interface/CaloTauFwd.h"
#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"

#include "DataFormats/JetReco/interface/Jet.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/PatCandidates/interface/LookupTableRecord.h"

#include <TMath.h>

typedef edm::ValueMap<pat::LookupTableRecord> LookupTableMap;

int FakeRateWeightProducerBase::gVerbosity_ = 0;

FakeRateWeightProducerBase::tauJetDiscrEntry::tauJetDiscrEntry(const edm::ParameterSet& cfg)
  : caloTauSelector_(0),
    pfTauSelector_(0),
    cfgError_(0)
{
  tauJetIdEffSource_ = cfg.getParameter<edm::InputTag>("tauJetIdEffSource");
  qcdJetFakeRateSource_ = cfg.getParameter<edm::InputTag>("qcdJetFakeRateSource");

  tauJetDiscrSource_ = cfg.exists("tauJetDiscrSource") ?
    cfg.getParameter<edm::InputTag>("tauJetDiscrSource") : edm::InputTag();
  tauJetDiscrThreshold_ = cfg.exists("tauJetDiscrThreshold") ?
    cfg.getParameter<double>("tauJetDiscrThreshold") : 0.5;

  tauJetCut_ = cfg.exists("tauJetCut") ?
    cfg.getParameter<std::string>("tauJetCut") : "";

  if ( tauJetDiscrSource_.label() == "" && tauJetCut_ == "" ) {
    edm::LogError("FakeRateWeightProducerBase::tauJetDiscrEntry")
      << " Either tauJetDiscrSource or tauJetCut must be specified !!";
    cfgError_ = 1;
  }

  if ( tauJetDiscrSource_.label() != "" && tauJetCut_ != "" ) {
    edm::LogError("FakeRateWeightProducerBase::tauJetDiscrEntry")
      << " Must not specify tauJetDiscrSource and tauJetCut !!";
    cfgError_ = 1;
  }

  if ( tauJetCut_ != "" ) {
    caloTauSelector_ = new StringCutObjectSelector<reco::CaloTau>(tauJetCut_);
    pfTauSelector_ = new StringCutObjectSelector<reco::PFTau>(tauJetCut_);
  }
}

FakeRateWeightProducerBase::tauJetDiscrEntry::~tauJetDiscrEntry()
{
  delete caloTauSelector_;
  delete pfTauSelector_;
}

FakeRateWeightProducerBase::fakeRateTypeEntry::fakeRateTypeEntry(const edm::ParameterSet& cfg)
  : cfgError_(0)
{
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgTauJetDiscriminators = cfg.getParameter<vParameterSet>("tauJetDiscriminators");
  for ( vParameterSet::const_iterator cfgTauJetDiscriminator = cfgTauJetDiscriminators.begin();
	cfgTauJetDiscriminator != cfgTauJetDiscriminators.end(); ++cfgTauJetDiscriminator ) {
    tauJetDiscriminators_.push_back(tauJetDiscrEntry(*cfgTauJetDiscriminator));
  }

  if ( tauJetDiscriminators_.size() == 0 ) {
    edm::LogError("FakeRateWeightProducerBase::fakeRateTypeEntry") << " No tau-jet Discriminators defined !!";
    cfgError_ = 1;
  }
}

FakeRateWeightProducerBase::fakeRateTypeEntry::~fakeRateTypeEntry()
{
//--- nothing to be done yet...
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

FakeRateWeightProducerBase::FakeRateWeightProducerBase(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    cfgError_(0),
    numJets_weightBelowMinimum_(0),
    numJets_weightAboveMaximum_(0),
    numJets_reverseWeightOrder_(0),
    numJets_outOfRange_(0),
    numJets_processed_(0)
{
  //std::cout << "<FakeRateWeightProducerBase::FakeRateWeightProducerBase>:" << std::endl;

  method_ = cfg.getParameter<std::string>("method");
  if ( !(method_ == "simple" || method_ == "CDF") ) {
    edm::LogError("FakeRateWeightProducerBase") << " Configuration parameter 'method' = " << method_ << " invalid !!";
    cfgError_ = 1;
  }

  allTauJetSource_ = cfg.getParameter<edm::InputTag>("allTauJetSource");
  preselTauJetSource_ = cfg.getParameter<edm::InputTag>("preselTauJetSource");
  //std::cout << " preselTauJetSource = " << preselTauJetSource_ << std::endl;

  dRmatch_ = cfg.getParameter<double>("dRmatch");
  //std::cout << " dRmatch = " << dRmatch_ << std::endl;

  edm::ParameterSet cfgFakeRateTypes = cfg.getParameter<edm::ParameterSet>("frTypes");
  typedef std::vector<std::string> vstring;
  vstring frTypeNames = cfgFakeRateTypes.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator frTypeName = frTypeNames.begin();
	frTypeName != frTypeNames.end(); ++frTypeName ) {
    edm::ParameterSet cfgFakeRateType = cfgFakeRateTypes.getParameter<edm::ParameterSet>(*frTypeName);

    fakeRateTypes_.insert(std::pair<std::string, fakeRateTypeEntry>(*frTypeName, cfgFakeRateType));
  }

  minJetWeight_ = cfg.getParameter<double>("minJetWeight");
  maxJetWeight_ = cfg.getParameter<double>("maxJetWeight");

  minJetPt_ = cfg.getParameter<double>("minJetPt");
  maxJetPt_ = cfg.getParameter<double>("maxJetPt");
  minJetEta_ = cfg.getParameter<double>("minJetEta");
  maxJetEta_ = cfg.getParameter<double>("maxJetEta");

  shiftTauIdEff_ = cfg.exists("shiftTauIdEff") ?
    cfg.getParameter<double>("shiftTauIdEff") : 0.;
}

FakeRateWeightProducerBase::~FakeRateWeightProducerBase()
{
  std::cout << "<~FakeRateWeightProducerBase>:" << std::endl;
  std::cout << "moduleLabel = " << moduleLabel_ << std::endl;
  std::cout << "number of tau-jet candidates" << std::endl;
  std::cout << " processed = " << numJets_processed_ << std::endl;
  std::cout << " jet-weight < miminum (= " << minJetWeight_ << ") = " << numJets_weightBelowMinimum_ << std::endl;
  std::cout << " jet-weight < maxinum (= " << maxJetWeight_ << ") = " << numJets_weightAboveMaximum_ << std::endl;
  std::cout << " tau id. efficiency < fake-rate = " << numJets_reverseWeightOrder_ << std::endl;
  std::cout << " in underflow/overflow bins = " << numJets_outOfRange_ << std::endl;
}

void FakeRateWeightProducerBase::getTauJetProperties(const edm::Event& evt,
						     edm::RefToBase<reco::BaseTau>& tauJetRef, unsigned iTauJet,
						     const edm::Handle<edm::View<reco::Candidate> >& preselTauJets,
						     const fakeRateTypeEntry& frType,
						     double& tauJetIdEff, double& qcdJetFakeRate, bool& tauJetDiscr_passed)
{
  if ( gVerbosity_ ) {
    std::cout << "<FakeRateWeightProducerBase::getTauJetProperties>:" << std::endl;
    std::cout << " tauJetRef: Pt = " << tauJetRef->pt() << ","
      << " eta = " << tauJetRef->eta() << ", phi = " << tauJetRef->phi() << std::endl;
  }

  if ( cfgError_ ) return;

  bool passesPreselection = false;
  for ( edm::View<reco::Candidate>::const_iterator preselTauJet = preselTauJets->begin();
	preselTauJet != preselTauJets->end(); ++preselTauJet ) {
    double dR = reco::deltaR(tauJetRef->p4(), preselTauJet->p4());
    if ( gVerbosity_ ) {
      std::cout << " preselTauJet: Pt = " << preselTauJet->pt() << ","
        << " eta = " << preselTauJet->eta() << ", phi = "
        << preselTauJet->phi() << ", dR = " << dR << std::endl;
    }
    if ( dR < dRmatch_ ) {
      if (gVerbosity_) std::cout << "--> matches tauJetRef." << std::endl;
      passesPreselection = true;
    }
  }

  if ( !passesPreselection ) {
    if ( gVerbosity_ ) std::cout << " (fails preselection)" << std::endl;

    tauJetIdEff = 0.;
    qcdJetFakeRate = 0.;

    tauJetDiscr_passed = false;

    return;
  }

  tauJetIdEff = 1.;
  qcdJetFakeRate = 1.;

  tauJetDiscr_passed = true;

  for ( std::vector<tauJetDiscrEntry>::const_iterator tauJetDiscr = frType.tauJetDiscriminators_.begin();
	tauJetDiscr != frType.tauJetDiscriminators_.end(); ++tauJetDiscr ) {

    edm::Handle<LookupTableMap> tauJetIdEffMap;
    evt.getByLabel(tauJetDiscr->tauJetIdEffSource_, tauJetIdEffMap);
    if (gVerbosity_) {
      std::cout << " tau id. efficiency (" << tauJetDiscr->tauJetIdEffSource_
        << ") = " << (*tauJetIdEffMap)[tauJetRef].value() << std::endl;
    }
    tauJetIdEff *= (*tauJetIdEffMap)[tauJetRef].value();

    edm::Handle<LookupTableMap> qcdJetFakeRateMap;
    evt.getByLabel(tauJetDiscr->qcdJetFakeRateSource_, qcdJetFakeRateMap);
    if (gVerbosity_) {
      std::cout << " fake-rate (" << tauJetDiscr->qcdJetFakeRateSource_ << ") = "
        << (*qcdJetFakeRateMap)[tauJetRef].value() << std::endl;
    }
    qcdJetFakeRate *= (*qcdJetFakeRateMap)[tauJetRef].value();

    double tauJetDiscr_value = -1.;
    if ( typeid(*tauJetRef) == typeid(reco::CaloTau) ) {
      edm::Handle<reco::CaloTauCollection> caloTauJets;
      evt.getByLabel(allTauJetSource_, caloTauJets);

      edm::Ref<reco::CaloTauCollection> caloTauJetRef(caloTauJets, iTauJet);

      if ( tauJetDiscr->tauJetDiscrSource_.label() != "" ) {
	edm::Handle<reco::CaloTauDiscriminator> caloTauJetDiscr;
	evt.getByLabel(tauJetDiscr->tauJetDiscrSource_, caloTauJetDiscr);

	tauJetDiscr_value = (*caloTauJetDiscr)[caloTauJetRef];
      } else if ( tauJetDiscr->tauJetCut_ != "" ) {
	tauJetDiscr_value = ( (*tauJetDiscr->caloTauSelector_)(*caloTauJetRef) ) ? 1. : 0.;
      }
    } else if ( typeid(*tauJetRef) == typeid(reco::PFTau) ) {
      edm::Handle<reco::PFTauCollection> pfTauJets;
      evt.getByLabel(allTauJetSource_, pfTauJets);

      edm::Ref<reco::PFTauCollection> pfTauJetRef(pfTauJets, iTauJet);

      if ( tauJetDiscr->tauJetDiscrSource_.label() != "" ) {
	edm::Handle<reco::PFTauDiscriminator> pfTauJetDiscr;
	evt.getByLabel(tauJetDiscr->tauJetDiscrSource_, pfTauJetDiscr);

	tauJetDiscr_value = (*pfTauJetDiscr)[pfTauJetRef];
      } else if ( tauJetDiscr->tauJetCut_ != "" ) {
	tauJetDiscr_value = ( (*tauJetDiscr->pfTauSelector_)(*pfTauJetRef) ) ? 1. : 0.;
      }
    } else {
      edm::LogError("FakeRateWeightProducerBase::getTauJetProperties")
	<< " Undefined tau Type = " << typeid(*tauJetRef).name()
	<< " --> skipping !!";
      continue;
    }

    if ( !(tauJetDiscr_value > tauJetDiscr->tauJetDiscrThreshold_) ) tauJetDiscr_passed = false;
  }

  if ( shiftTauIdEff_ != 0. ) {
    tauJetIdEff *= (1. + shiftTauIdEff_);
    if ( tauJetIdEff < 0. ) tauJetIdEff = 0.;
    if ( tauJetIdEff > 1. ) tauJetIdEff = 1.;
  }
}

double FakeRateWeightProducerBase::getFakeRateJetWeight(double tauJetIdEff, double qcdJetFakeRate,
							bool tauJetDiscr_passed, const reco::BaseTau* tauJet)
{
  double fakeRateJetWeight = 0.;

  if ( tauJet->pt()  > minJetPt_  && tauJet->pt()  < maxJetPt_ &&
       tauJet->eta() > minJetEta_ && tauJet->eta() < maxJetEta_ ) {
    if ( method_ == "simple" ) {
      fakeRateJetWeight = qcdJetFakeRate;
    } else if ( method_ == "CDF" ) {
      if ( tauJetIdEff > qcdJetFakeRate ) {
	fakeRateJetWeight = ( tauJetDiscr_passed ) ?
	  -qcdJetFakeRate*(1. - tauJetIdEff)/(tauJetIdEff - qcdJetFakeRate) : qcdJetFakeRate*tauJetIdEff/(tauJetIdEff - qcdJetFakeRate);

	if ( fakeRateJetWeight < minJetWeight_ ) {
          edm::LogWarning ("getFakeRateJetWeight")
           << " Jet-weight = " << fakeRateJetWeight << " falls below minimum value = " << minJetWeight_
           << " --> setting jet-weight to minimum value !!";
	  fakeRateJetWeight = minJetWeight_;
	  ++numJets_weightBelowMinimum_;
	}

	if ( fakeRateJetWeight > maxJetWeight_ ) {
          edm::LogWarning ("getFakeRateJetWeight")
           << " Jet-weight = " << fakeRateJetWeight << " exceeds maximum value = " << maxJetWeight_
           << " --> setting jet-weight to maximum value !!";
	  fakeRateJetWeight = maxJetWeight_;
	  ++numJets_weightAboveMaximum_;
	}
      } else if ( tauJetIdEff < qcdJetFakeRate ) {
	edm::LogWarning ("getFakeRateJetWeight")
	  << " QCD-jet fake-rate = " << qcdJetFakeRate << " exceeds Tau-jet id. efficiency = " << tauJetIdEff
	  << " for Pt = " << tauJet->pt() << ", eta = " << tauJet->eta() << ", phi = " << tauJet->phi()
	  << " jet-radius = " << getJetRadius(*tauJet) << " --> setting jet-weight to zero !!";
	++numJets_reverseWeightOrder_;
      }
    }
  } else {
    ++numJets_outOfRange_;
  }

  ++numJets_processed_;

  return fakeRateJetWeight;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

double getJetRadius(const reco::BaseTau& tauJet)
{
  if ( typeid(tauJet) == typeid(reco::CaloTau) ) {
    const reco::CaloTau* caloTauJet = dynamic_cast<const reco::CaloTau*>(&tauJet);
    reco::Jet::EtaPhiMoments etaPhiStatistics = caloTauJet->caloTauTagInfoRef()->calojetRef()->etaPhiStatistics();
    return TMath::Sqrt(etaPhiStatistics.etaEtaMoment + etaPhiStatistics.phiPhiMoment);
  } else if ( typeid(tauJet) == typeid(reco::PFTau) ) {
    const reco::PFTau* pfTauJet = dynamic_cast<const reco::PFTau*>(&tauJet);
    reco::Jet::EtaPhiMoments etaPhiStatistics = pfTauJet->pfTauTagInfoRef()->pfjetRef()->etaPhiStatistics();
    return TMath::Sqrt(etaPhiStatistics.etaEtaMoment + etaPhiStatistics.phiPhiMoment);
  } else {
    edm::LogError ("getJetRadius") << " Unsupported type of Tau-jet passed as function Argument !!";
    return -1.;
  }
}
