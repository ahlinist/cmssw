#include "TauAnalysis/BgEstimationTools/interface/FakeRateWeightProducerBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/TauReco/interface/CaloTauFwd.h"
#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Common/interface/ValueMap.h" 
#include "DataFormats/PatCandidates/interface/LookupTableRecord.h"

typedef edm::ValueMap<pat::LookupTableRecord> LookupTableMap;

FakeRateWeightProducerBase::tauJetDiscrEntry::tauJetDiscrEntry(const edm::ParameterSet& cfg)
  : caloTauSelector_(0),
    pfTauSelector_(0),
    cfgError_(0)
{
  tauJetIdEffSource_ = cfg.getParameter<edm::InputTag>("tauJetIdEffSource");
  qcdJetFakeRateSource_ = cfg.getParameter<edm::InputTag>("qcdJetFakeRateSource");

  tauJetDiscrSource_ = cfg.exists("tauJetDiscrSource") ? cfg.getParameter<edm::InputTag>("tauJetDiscrSource") : edm::InputTag();
  tauJetDiscrThreshold_ = cfg.exists("tauJetDiscrThreshold") ? cfg.getParameter<double>("tauJetDiscrThreshold") : 0.5; 

  tauJetCut_ = cfg.exists("tauJetCut") ? cfg.getParameter<std::string>("tauJetCut") : "";

  if ( tauJetDiscrSource_.label() == "" && tauJetCut_ == "" ) {
    edm::LogError("FakeRateWeightProducerBase::tauJetDiscrEntry") << " Either tauJetDiscrSource or tauJetCut must be specified !!";
    cfgError_ = 1;
  }
  
  if ( tauJetDiscrSource_.label() != "" && tauJetCut_ != "" ) {
    edm::LogError("FakeRateWeightProducerBase::tauJetDiscrEntry") << " Must not specify tauJetDiscrSource and tauJetCut !!";
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

//
//-----------------------------------------------------------------------------------------------------------------------
//

FakeRateWeightProducerBase::FakeRateWeightProducerBase(const edm::ParameterSet& cfg)
  : cfgError_(0)
{
  allTauJetSource_ = cfg.getParameter<edm::InputTag>("allTauJetSource");
  preselTauJetSource_ = cfg.getParameter<edm::InputTag>("preselTauJetSource");

  dRmatch_ = cfg.getParameter<double>("dRmatch");
  
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgTauJetDiscriminators = cfg.getParameter<vParameterSet>("tauJetDiscriminators");
  for ( vParameterSet::const_iterator cfgTauJetDiscriminator = cfgTauJetDiscriminators.begin();
	cfgTauJetDiscriminator != cfgTauJetDiscriminators.end(); ++cfgTauJetDiscriminator ) {
    tauJetDiscriminators_.push_back(tauJetDiscrEntry(*cfgTauJetDiscriminator));
  }

  if ( tauJetDiscriminators_.size() == 0 ) {
    edm::LogError("FakeRateWeightProducerBase") << " No tau-jet Discriminators defined !!";
    cfgError_ = 1;
  }
}

FakeRateWeightProducerBase::~FakeRateWeightProducerBase()
{
//--- nothing to be done yet...
}

void FakeRateWeightProducerBase::getTauJetProperties(const edm::Event& evt,
						     edm::RefToBase<reco::BaseTau>& tauJetRef, unsigned iTauJet,
						     const edm::Handle<edm::View<reco::Candidate> >& preselTauJets,
						     double& tauJetIdEff, double& qcdJetFakeRate, bool& tauJetDiscr_passed)
{ 
  //std::cout << "<FakeRateWeightProducerBase::getTauJetProperties>:" << std::endl;

  if ( cfgError_ ) return;

  bool passesPreselection = false;
  for ( edm::View<reco::Candidate>::const_iterator preselTauJet = preselTauJets->begin();
	preselTauJet != preselTauJets->end(); ++preselTauJet ) {
    if ( reco::deltaR(tauJetRef->p4(), preselTauJet->p4()) < dRmatch_ ) passesPreselection = true;
  }

  if ( !passesPreselection ) {
    tauJetIdEff = 0.;
    qcdJetFakeRate = 0.;
    
    tauJetDiscr_passed = false;
    
    return;
  }

  tauJetIdEff = 1.;
  qcdJetFakeRate = 1.;

  tauJetDiscr_passed = true;

  for ( std::vector<tauJetDiscrEntry>::const_iterator tauJetDiscr = tauJetDiscriminators_.begin();
	tauJetDiscr != tauJetDiscriminators_.end(); ++tauJetDiscr ) {

    edm::Handle<LookupTableMap> tauJetIdEffMap;
    evt.getByLabel(tauJetDiscr->tauJetIdEffSource_, tauJetIdEffMap);
    //std::cout << " tau id. efficiency (" << tauJetDiscr->tauJetIdEffSource_ << ") = " 
    //	  << (*tauJetIdEffMap)[tauJetRef].value() << std::endl;
    tauJetIdEff *= (*tauJetIdEffMap)[tauJetRef].value();

    edm::Handle<LookupTableMap> qcdJetFakeRateMap;
    evt.getByLabel(tauJetDiscr->qcdJetFakeRateSource_, qcdJetFakeRateMap);
    //std::cout << " fake-rate (" << tauJetDiscr->qcdJetFakeRateSource_ << ") = " 
    //	  << (*qcdJetFakeRateMap)[tauJetRef].value() << std::endl;
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
      edm::LogError("FakeRateWeightProducerBase::getTauJetProperties") << " Undefined tau Type = " << typeid(*tauJetRef).name() 
								       << " --> skipping !!";
      continue;
    }
    
    if ( !(tauJetDiscr_value > tauJetDiscr->tauJetDiscrThreshold_) ) tauJetDiscr_passed = false;
  }
}
