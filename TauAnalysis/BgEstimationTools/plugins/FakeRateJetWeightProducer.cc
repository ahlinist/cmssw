#include "TauAnalysis/BgEstimationTools/plugins/FakeRateJetWeightProducer.h"

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

FakeRateJetWeightProducer::tauJetDiscrEntry::tauJetDiscrEntry(const edm::ParameterSet& cfg)
{
  tauJetIdEffSource_ = cfg.getParameter<edm::InputTag>("tauJetIdEffSource");
  qcdJetFakeRateSource_ = cfg.getParameter<edm::InputTag>("qcdJetFakeRateSource");
  tauJetDiscrSource_ = cfg.getParameter<edm::InputTag>("tauJetDiscrSource");
  tauJetDiscrThreshold_ = cfg.exists("tauJetDiscrThreshold") ? cfg.getParameter<double>("tauJetDiscrThreshold") : 0.5; 			 
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

FakeRateJetWeightProducer::FakeRateJetWeightProducer(const edm::ParameterSet& cfg)
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
    edm::LogError("FakeRateJetWeightProducer") << " No tau-jet Discriminators defined !!";
    cfgError_ = 1;
  }
   
  produces<edm::ValueMap<pat::LookupTableRecord> >();
}

void FakeRateJetWeightProducer::produce(edm::Event& evt, const edm::EventSetup&) 
{ 
  //std::cout << "<FakeRateJetWeightProducer::produce>:" << std::endl;

  if ( cfgError_ ) return;

  edm::Handle<edm::View<reco::BaseTau> > tauJets;
  evt.getByLabel(tauJetSource_, tauJets);

  std::vector<pat::LookupTableRecord> fakeRateJetWeights;

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
	edm::LogError("FakeRateJetWeightProducer::produce") << " Undefined tau Type = " << typeid(*tauJetRef).name() 
							    << " --> skipping !!";
	continue;
      }

      if ( !(tauJetDiscr_value > tauJetDiscr->tauJetDiscrThreshold_) ) tauJetDiscr_passed = false;
    }

    double fakeRateJetWeight = 0.;
    if ( tauJetIdEff > qcdJetFakeRate ) {
      fakeRateJetWeight = ( tauJetDiscr_passed ) ? 
	-qcdJetFakeRate*(1 - tauJetIdEff)/(tauJetIdEff - qcdJetFakeRate) : qcdJetFakeRate*tauJetIdEff/(tauJetIdEff - qcdJetFakeRate);
    }

    //std::cout << " --> jet weight = " << fakeRateJetWeight << std::endl;

    fakeRateJetWeights.push_back(pat::LookupTableRecord(fakeRateJetWeight));
  }

  std::auto_ptr<LookupTableMap> fakeRateJetWeightMap(new LookupTableMap());
  LookupTableMap::Filler valueMapFiller(*fakeRateJetWeightMap);
  valueMapFiller.insert(tauJets, fakeRateJetWeights.begin(), fakeRateJetWeights.end());
  valueMapFiller.fill();

  evt.put(fakeRateJetWeightMap);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(FakeRateJetWeightProducer);
