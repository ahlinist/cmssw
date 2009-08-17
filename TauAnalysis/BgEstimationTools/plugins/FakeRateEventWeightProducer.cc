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

FakeRateEventWeightProducer::FakeRateEventWeightProducer(const edm::ParameterSet& cfg)
{
  tauJetSource_ = cfg.getParameter<edm::InputTag>("tauJetSource");
  tauJetIdEffSource_ = cfg.getParameter<edm::InputTag>("tauJetIdEffSource");
  qcdJetFakeRateSource_ = cfg.getParameter<edm::InputTag>("qcdJetFakeRateSource");
  tauJetDiscrSource_ = cfg.getParameter<edm::InputTag>("tauJetDiscrSource");
  tauJetDiscrThreshold_ = cfg.exists("tauJetDiscrThreshold") ? cfg.getParameter<double>("tauJetDiscrThreshold") : 0.5; 

  produces<double>();
}

void FakeRateEventWeightProducer::produce(edm::Event& evt, const edm::EventSetup&) 
{ 
  edm::Handle<edm::View<reco::BaseTau> > tauJets;
  evt.getByLabel(tauJetSource_, tauJets);

  edm::Handle<LookupTableMap> tauJetIdEffMap;
  evt.getByLabel(tauJetIdEffSource_, tauJetIdEffMap);

  edm::Handle<LookupTableMap> qcdJetFakeRateMap;
  evt.getByLabel(qcdJetFakeRateSource_, qcdJetFakeRateMap);

  double tauJetIdEffSum = 0.;
  double qcdJetFakeRateSum = 0.;
  unsigned numTauJetDiscrPassed = 0;

  unsigned numTauJets = tauJets->size();
  for ( unsigned iTauJet = 0; iTauJet < numTauJets; ++iTauJet ) {
    edm::RefToBase<reco::BaseTau> tauJetRef = tauJets->refAt(iTauJet);

    double tauJetIdEff = (*tauJetIdEffMap)[tauJetRef].value();
    double qcdJetFakeRate = (*qcdJetFakeRateMap)[tauJetRef].value();

    tauJetIdEffSum += tauJetIdEff;
    qcdJetFakeRateSum += qcdJetFakeRate;

    double tauJetDiscr;
    if ( typeid(*tauJetRef) == typeid(reco::CaloTau) ) {
      edm::Handle<reco::CaloTauCollection> caloTauJets;
      evt.getByLabel(tauJetSource_, caloTauJets);
      
      edm::Handle<reco::CaloTauDiscriminator> caloTauJetDiscr;
      evt.getByLabel(tauJetDiscrSource_, caloTauJetDiscr);
		     
      edm::Ref<reco::CaloTauCollection> caloTauJetRef(caloTauJets, iTauJet);

      tauJetDiscr = (*caloTauJetDiscr)[caloTauJetRef];
    } else if ( typeid(*tauJetRef) == typeid(reco::PFTau) ) {
      edm::Handle<reco::PFTauCollection> pfTauJets;
      evt.getByLabel(tauJetSource_, pfTauJets);
      
      edm::Handle<reco::PFTauDiscriminator> pfTauJetDiscr;
      evt.getByLabel(tauJetDiscrSource_, pfTauJetDiscr);
		     
      edm::Ref<reco::PFTauCollection> pfTauJetRef(pfTauJets, iTauJet);

      tauJetDiscr = (*pfTauJetDiscr)[pfTauJetRef];
    } else {
      edm::LogError("FakeRateEventWeightProducer::produce") << " Undefined tau Type = " << typeid(*tauJetRef).name() << " --> skipping !!";
      continue;
    }

    if ( tauJetDiscr > tauJetDiscrThreshold_ ) {
      ++numTauJetDiscrPassed;
    }
  }

  double effTerm = 0.5*(tauJetIdEffSum + qcdJetFakeRateSum);
  double frTerm = qcdJetFakeRateSum;

  double fakeRateEventWeight = 0.;
  if ( numTauJetDiscrPassed > 0 ) {
    fakeRateEventWeight = -frTerm*(1 - effTerm)/(effTerm - frTerm);
  } else {
    fakeRateEventWeight = frTerm*effTerm/(effTerm - frTerm);
  }

  std::auto_ptr<double> fakeRateEventWeightPtr(new double(fakeRateEventWeight));

  evt.put(fakeRateEventWeightPtr);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(FakeRateEventWeightProducer);
