#include "TauAnalysis/BgEstimationTools/plugins/FakeRateEventWeightProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/CaloTauFwd.h"
#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"

#include "DataFormats/Common/interface/ValueMap.h" 
#include "DataFormats/PatCandidates/interface/LookupTableRecord.h"

#include <vector>

typedef edm::ValueMap<pat::LookupTableRecord> LookupTableMap;

FakeRateEventWeightProducer::tauJetDiscrEntry::tauJetDiscrEntry(const edm::ParameterSet& cfg)
{
  tauJetIdEffSource_ = cfg.getParameter<edm::InputTag>("tauJetIdEffSource");
  qcdJetFakeRateSource_ = cfg.getParameter<edm::InputTag>("qcdJetFakeRateSource");
  tauJetDiscrSource_ = cfg.getParameter<edm::InputTag>("tauJetDiscrSource");
  tauJetDiscrThreshold_ = cfg.exists("tauJetDiscrThreshold") ? cfg.getParameter<double>("tauJetDiscrThreshold") : 0.5; 			 
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

FakeRateEventWeightProducer::FakeRateEventWeightProducer(const edm::ParameterSet& cfg)
  : cfgError_(0)
{
  tauJetSource_ = cfg.getParameter<edm::InputTag>("tauJetSource");

  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgTauJetDiscriminators = cfg.getParameter<vParameterSet>("tauJetDiscriminators");
  for ( vParameterSet::const_iterator cfgTauJetDiscriminator = cfgTauJetDiscriminators.begin();
	cfgTauJetDiscriminator != cfgTauJetDiscriminators.end(); ++cfgTauJetDiscriminator ) {
    tauJetDiscriminators_.push_back(tauJetDiscrEntry(*cfgTauJetDiscriminator));
  }

  if ( tauJetDiscriminators_.size() == 0 ) {
    edm::LogError("FakeRateEventWeightProducer") << " No tau-jet Discriminators defined !!";
    cfgError_ = 1;
  }

  produces<double>();
}

void FakeRateEventWeightProducer::produce(edm::Event& evt, const edm::EventSetup&) 
{ 
  //std::cout << "<FakeRateEventWeightProducer::produce>:" << std::endl;

  if ( cfgError_ ) return;

  edm::Handle<edm::View<reco::BaseTau> > tauJets;
  evt.getByLabel(tauJetSource_, tauJets);

  double tauJetIdEffSum = 0.;
  double qcdJetFakeRateSum = 0.;
  unsigned numTauJetDiscrPassed = 0;

  unsigned numTauJets = tauJets->size();
  for ( unsigned iTauJet = 0; iTauJet < numTauJets; ++iTauJet ) {
    edm::RefToBase<reco::BaseTau> tauJetRef = tauJets->refAt(iTauJet);

    double tauJetIdEff = 1.;
    double qcdJetFakeRate = 1.;

    bool tauJetDiscr_passed = true;

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
      
      double tauJetDiscr_value;
      if ( typeid(*tauJetRef) == typeid(reco::CaloTau) ) {
	edm::Handle<reco::CaloTauCollection> caloTauJets;
	evt.getByLabel(tauJetSource_, caloTauJets);
	
	edm::Handle<reco::CaloTauDiscriminator> caloTauJetDiscr;
	evt.getByLabel(tauJetDiscr->tauJetDiscrSource_, caloTauJetDiscr);
		     
	edm::Ref<reco::CaloTauCollection> caloTauJetRef(caloTauJets, iTauJet);

	tauJetDiscr_value = (*caloTauJetDiscr)[caloTauJetRef];
      } else if ( typeid(*tauJetRef) == typeid(reco::PFTau) ) {
	edm::Handle<reco::PFTauCollection> pfTauJets;
	evt.getByLabel(tauJetSource_, pfTauJets);
	
	edm::Handle<reco::PFTauDiscriminator> pfTauJetDiscr;
	evt.getByLabel(tauJetDiscr->tauJetDiscrSource_, pfTauJetDiscr);
	
	edm::Ref<reco::PFTauCollection> pfTauJetRef(pfTauJets, iTauJet);
	
	tauJetDiscr_value = (*pfTauJetDiscr)[pfTauJetRef];
      } else {
	edm::LogError("FakeRateEventWeightProducer::produce") << " Undefined tau Type = " << typeid(*tauJetRef).name() 
							      << " --> skipping !!";
	continue;
      }

      if ( !(tauJetDiscr_value > tauJetDiscr->tauJetDiscrThreshold_) ) tauJetDiscr_passed = false;
    }

    tauJetIdEffSum += tauJetIdEff;
    qcdJetFakeRateSum += qcdJetFakeRate;

    if ( tauJetDiscr_passed ) ++numTauJetDiscrPassed;
  }

  double effTerm = 0.5*(tauJetIdEffSum + qcdJetFakeRateSum);
  double frTerm = qcdJetFakeRateSum;

  double fakeRateEventWeight = 0.;
  if ( effTerm > frTerm ) {
    fakeRateEventWeight = ( numTauJetDiscrPassed > 0 ) ?
      -frTerm*(1 - effTerm)/(effTerm - frTerm) : frTerm*effTerm/(effTerm - frTerm);
  }

  //std::cout << " --> event weight = " << fakeRateEventWeight << std::endl;

  std::auto_ptr<double> fakeRateEventWeightPtr(new double(fakeRateEventWeight));
  
  evt.put(fakeRateEventWeightPtr);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(FakeRateEventWeightProducer);
